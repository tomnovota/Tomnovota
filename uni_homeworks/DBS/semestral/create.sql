-- zdroj: https://users.fit.cvut.cz/~hunkajir/dbs2/create.sql
-- odeberu pokud existuje funkce na oodebrání tabulek a sekvencí
DROP FUNCTION IF EXISTS remove_all();

-- vytvořím funkci která odebere tabulky a sekvence
-- chcete také umět psát PLSQL? Zapište si předmět BI-SQL ;-)
CREATE or replace FUNCTION remove_all() RETURNS void AS $$
DECLARE
    rec RECORD;
    cmd text;
BEGIN
    cmd := '';

    FOR rec IN SELECT
            'DROP SEQUENCE ' || quote_ident(n.nspname) || '.'
                || quote_ident(c.relname) || ' CASCADE;' AS name
        FROM
            pg_catalog.pg_class AS c
        LEFT JOIN
            pg_catalog.pg_namespace AS n
        ON
            n.oid = c.relnamespace
        WHERE
            relkind = 'S' AND
            n.nspname NOT IN ('pg_catalog', 'pg_toast') AND
            pg_catalog.pg_table_is_visible(c.oid)
    LOOP
        cmd := cmd || rec.name;
    END LOOP;

    FOR rec IN SELECT
            'DROP TABLE ' || quote_ident(n.nspname) || '.'
                || quote_ident(c.relname) || ' CASCADE;' AS name
        FROM
            pg_catalog.pg_class AS c
        LEFT JOIN
            pg_catalog.pg_namespace AS n
        ON
            n.oid = c.relnamespace WHERE relkind = 'r' AND
            n.nspname NOT IN ('pg_catalog', 'pg_toast') AND
            pg_catalog.pg_table_is_visible(c.oid)
    LOOP
        cmd := cmd || rec.name;
    END LOOP;

    EXECUTE cmd;
    RETURN;
END;
$$ LANGUAGE plpgsql;
-- zavolám funkci co odebere tabulky a sekvence - Mohl bych dropnout celé schéma a znovu jej vytvořit, použíjeme však PLSQL
select remove_all();

-- tvoření tabulek
CREATE TABLE adresa (
    id_adresa SERIAL NOT NULL,
    ulice VARCHAR(100),
    cislo_popisne INTEGER NOT NULL,
    psc VARCHAR(100) NOT NULL,
    obec VARCHAR(100) NOT NULL,
    stat VARCHAR(100) NOT NULL
);
ALTER TABLE adresa ADD CONSTRAINT pk_adresa PRIMARY KEY (id_adresa);

CREATE TABLE auto (
    id_auto SERIAL NOT NULL,
    id_objednavka INTEGER NOT NULL,
    registracni_znacka VARCHAR(100) NOT NULL
);
ALTER TABLE auto ADD CONSTRAINT pk_auto PRIMARY KEY (id_auto);

CREATE TABLE extra_sluzba (
    id_sluzba SERIAL NOT NULL,
    nazev VARCHAR(100) NOT NULL,
    cena INTEGER NOT NULL,
    dostupnost INTEGER NOT NULL
);
ALTER TABLE extra_sluzba ADD CONSTRAINT pk_extra_sluzba PRIMARY KEY (id_sluzba);

CREATE TABLE faktura (
    id_objednavka INTEGER NOT NULL,
    castka INTEGER NOT NULL,
    zpusob_platby VARCHAR(100) NOT NULL,
    probehla BOOLEAN NOT NULL
);
ALTER TABLE faktura ADD CONSTRAINT pk_faktura PRIMARY KEY (id_objednavka);

CREATE TABLE host (
    id_host SERIAL NOT NULL,
    jmeno VARCHAR(100) NOT NULL,
    prijmeni VARCHAR(100) NOT NULL
);
ALTER TABLE host ADD CONSTRAINT pk_host PRIMARY KEY (id_host);

CREATE TABLE objednatel (
    id_host INTEGER NOT NULL,
    id_adresa INTEGER NOT NULL,
    telefon VARCHAR(50) NOT NULL,
    email VARCHAR(100) NOT NULL,
    druh_dokladu VARCHAR(100) NOT NULL,
    cislo_dokladu VARCHAR(50) NOT NULL
);
ALTER TABLE objednatel ADD CONSTRAINT pk_objednatel PRIMARY KEY (id_host);

CREATE TABLE objednavka (
    id_objednavka SERIAL NOT NULL,
    id_host INTEGER NOT NULL,
    id_pokoj INTEGER,
    stav_objednavky VARCHAR(100) NOT NULL,
    datum_zalozeni DATE NOT NULL,
    datum_prijezdu DATE NOT NULL,
    datum_odjezdu DATE NOT NULL
);
ALTER TABLE objednavka ADD CONSTRAINT pk_objednavka PRIMARY KEY (id_objednavka);

CREATE TABLE objednavka_extra (
    id_objed_extra SERIAL NOT NULL,
    id_objednavka INTEGER NOT NULL,
    id_sluzba INTEGER NOT NULL
);
ALTER TABLE objednavka_extra ADD CONSTRAINT pk_objednavka_extra PRIMARY KEY (id_objed_extra);

CREATE TABLE parkovani (
    id_objednavka INTEGER NOT NULL,
    cena INTEGER NOT NULL
);
ALTER TABLE parkovani ADD CONSTRAINT pk_parkovani PRIMARY KEY (id_objednavka);

CREATE TABLE pokoj (
    id_pokoj SERIAL NOT NULL,
    cena_noc INTEGER NOT NULL,
    kategorie VARCHAR(100) NOT NULL,
    kapacita INTEGER NOT NULL
);
ALTER TABLE pokoj ADD CONSTRAINT pk_pokoj PRIMARY KEY (id_pokoj);

CREATE TABLE pokoj_host (
    id_pokoj_host SERIAL NOT NULL,
    id_pokoj INTEGER NOT NULL,
    id_host INTEGER NOT NULL
);
ALTER TABLE pokoj_host ADD CONSTRAINT pk_pokoj_host PRIMARY KEY (id_pokoj_host);

CREATE TABLE vybaveni (
    id_vybaveni SERIAL NOT NULL,
    id_pokoj INTEGER NOT NULL,
    nazev VARCHAR(100) NOT NULL,
    cena INTEGER NOT NULL,
    predchozi_stav VARCHAR(100) NOT NULL,
    aktualni_stav VARCHAR(100) NOT NULL
);
ALTER TABLE vybaveni ADD CONSTRAINT pk_vybaveni PRIMARY KEY (id_vybaveni);

CREATE TABLE vybaveni_faktura (
    id_vybaveni INTEGER NOT NULL,
    id_objednavka INTEGER NOT NULL
);
ALTER TABLE vybaveni_faktura ADD CONSTRAINT pk_vybaveni_faktura PRIMARY KEY (id_vybaveni, id_objednavka);

ALTER TABLE auto ADD CONSTRAINT fk_auto_parkovani FOREIGN KEY (id_objednavka) REFERENCES parkovani (id_objednavka) ON DELETE CASCADE;

ALTER TABLE faktura ADD CONSTRAINT fk_faktura_objednavka FOREIGN KEY (id_objednavka) REFERENCES objednavka (id_objednavka) ON DELETE CASCADE;

ALTER TABLE objednatel ADD CONSTRAINT fk_objednatel_host FOREIGN KEY (id_host) REFERENCES host (id_host) ON DELETE CASCADE;
ALTER TABLE objednatel ADD CONSTRAINT fk_objednatel_adresa FOREIGN KEY (id_adresa) REFERENCES adresa (id_adresa) ON DELETE CASCADE;

ALTER TABLE objednavka ADD CONSTRAINT fk_objednavka_objednatel FOREIGN KEY (id_host) REFERENCES objednatel (id_host) ON DELETE CASCADE;
ALTER TABLE objednavka ADD CONSTRAINT fk_objednavka_pokoj FOREIGN KEY (id_pokoj) REFERENCES pokoj (id_pokoj) ON DELETE CASCADE;

ALTER TABLE objednavka_extra ADD CONSTRAINT fk_objednavka_extra_objednavka FOREIGN KEY (id_objednavka) REFERENCES objednavka (id_objednavka) ON DELETE CASCADE;
ALTER TABLE objednavka_extra ADD CONSTRAINT fk_objednavka_extra_extra_sluzb FOREIGN KEY (id_sluzba) REFERENCES extra_sluzba (id_sluzba) ON DELETE CASCADE;

ALTER TABLE parkovani ADD CONSTRAINT fk_parkovani_objednavka FOREIGN KEY (id_objednavka) REFERENCES objednavka (id_objednavka) ON DELETE CASCADE;

ALTER TABLE pokoj_host ADD CONSTRAINT fk_pokoj_host_pokoj FOREIGN KEY (id_pokoj) REFERENCES pokoj (id_pokoj) ON DELETE CASCADE;
ALTER TABLE pokoj_host ADD CONSTRAINT fk_pokoj_host_host FOREIGN KEY (id_host) REFERENCES host (id_host) ON DELETE CASCADE;

ALTER TABLE vybaveni ADD CONSTRAINT fk_vybaveni_pokoj FOREIGN KEY (id_pokoj) REFERENCES pokoj (id_pokoj) ON DELETE CASCADE;

ALTER TABLE vybaveni_faktura ADD CONSTRAINT fk_vybaveni_faktura_vybaveni FOREIGN KEY (id_vybaveni) REFERENCES vybaveni (id_vybaveni) ON DELETE CASCADE;
ALTER TABLE vybaveni_faktura ADD CONSTRAINT fk_vybaveni_faktura_faktura FOREIGN KEY (id_objednavka) REFERENCES faktura (id_objednavka) ON DELETE CASCADE;
