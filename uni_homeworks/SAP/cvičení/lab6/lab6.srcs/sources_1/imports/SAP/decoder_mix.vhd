library IEEE;
use IEEE.std_logic_1164.all;

entity decoder_7seg is
    port(
        a   : in  std_logic;
        b   : in  std_logic;
        c   : in  std_logic;
        d   : in  std_logic;
        f_a : out std_logic;
        f_b : out std_logic;
        f_c : out std_logic;
        f_d : out std_logic;
        f_e : out std_logic;
        f_f : out std_logic;
        f_g : out std_logic
    );
end decoder_7seg;

architecture behavioral of decoder_7seg is

    signal input        : std_logic_vector (3 downto 0);
    signal internal_f_d : std_logic;
    signal internal_f_e : std_logic;
    signal internal_f_f : std_logic;
    signal internal_f_g : std_logic;

begin

    input(3) <= d;
    input(2) <= c;
    input(1) <= b;
    input(0) <= a;

-- 7-segment display layout
-- Znazorneni 7segmentoveho displeje

--        f_A
--        ---
--  f_F  |   |  f_B
--        ---    <- f_G
--  f_E  |   |  f_C
--        ---
--        f_D

--  A:      B:      C:      D:      E:      F:
--    ---             ---             ---     ---
--   |   |   |       |           |   |       |
--    ---     ---             ---     ---     ---
--   |   |   |   |   |       |   |   |       |
--            ---     ---     ---     ---

-------------------------------------------------------------------
-------------------------------------------------------------------
-- Zde doplnte 3 algebraicke vyrazy pro segmenty f_a, f_b, f_c   --
-------------------------------------------------------------------
-------------------------------------------------------------------

-- you can use the following operators:
-- ----------------------------------
-- | operator |   example           |
-- ----------------------------------
-- |    and   | a and b and c       |
-- |    or    | a or b or (c and d) |
-- |    not   | not a               |
-- ----------------------------------
-- There is no priority of operators. Be sure to use brackets!
-- example:
--   given: f(a, b, c) = a * #b * #c + a * b
--   in VHDL: f <= (a and (not b) and (not c)) or (a and b)

-- ve vyrazech pouzivejte nasledujici operatory:
-- ----------------------------------
-- | operator | priklad pouziti     |
-- ----------------------------------
-- |    and   | a and b and c       |
-- |    or    | a or b or (c and d) |
-- |    not   | not a               |
-- ----------------------------------
-- pouzivejte zavorky!
-- priklad:
--   zadani: f(a, b, c) = a * #b * #c + a * b
--   ve VHDL: f <= (a and (not b) and (not c)) or (a and b)

-- Funkce jsou negovane, protoze jednotlive segmenty jsou aktivni v log. 0.
-- Napriklad:
--  f_a <= (not(((not a) and b) or (b and (not c)) or ((not c) and d) or ((not b) and c and (not d))));

    f_a <= not('1');
    f_b <= not('1');
    f_c <= not('1');


                         
-- V pripade, ze pri vyplnovani tabulek zapomneli na to, ze jednotlive segmenty jsou aktivni v log. 0, muzete na nasledujici radky pridat negaci.
    f_d <= internal_f_d;
    f_e <= internal_f_e;
    f_f <= internal_f_f;
    f_g <= internal_f_g;

-----------------------------------------------------------------------
-----------------------------------------------------------------------
-- Zde doplnte vyrazy tabulkou pro zbyle segmenty f_d, f_e, f_f, f_g --
-----------------------------------------------------------------------
-----------------------------------------------------------------------

-- Zmena poradi vstupu a, b, c, d v zahlavi nema smysl - jedna se pouze o komentar.

-- ------------------------------------------------------------------------
-- |   funkcni hodnota f_d(d, c, b, a)    | index |  d  |  c  |  b  |  a  |
-- ------------------------------------------------------------------------
    with input select
    internal_f_d <= '0' when "0000",   -- |   0   |  0  |  0  |  0  |  0  |
                    '0' when "0001",   -- |   1   |  0  |  0  |  0  |  1  |
                    '0' when "0010",   -- |   2   |  0  |  0  |  1  |  0  |
                    '0' when "0011",   -- |   3   |  0  |  0  |  1  |  1  |
                    '0' when "0100",   -- |   4   |  0  |  1  |  0  |  0  |
                    '0' when "0101",   -- |   5   |  0  |  1  |  0  |  1  |
                    '0' when "0110",   -- |   6   |  0  |  1  |  1  |  0  |
                    '0' when "0111",   -- |   7   |  0  |  1  |  1  |  1  |
                    '0' when "1000",   -- |   8   |  1  |  0  |  0  |  0  |
                    '0' when "1001",   -- |   9   |  1  |  0  |  0  |  1  |
                    '0' when "1010",   -- |  10   |  1  |  0  |  1  |  0  |
                    '0' when "1011",   -- |  11   |  1  |  0  |  1  |  1  |
                    '0' when "1100",   -- |  12   |  1  |  1  |  0  |  0  |
                    '0' when "1101",   -- |  13   |  1  |  1  |  0  |  1  |
                    '0' when "1110",   -- |  14   |  1  |  1  |  1  |  0  |
                    '0' when "1111",   -- |  15   |  1  |  1  |  1  |  1  |
                    '0' when others;

-- ------------------------------------------------------------------------
-- |   funkcni hodnota f_e(d, c, b, a)    | index |  d  |  c  |  b  |  a  |
-- ------------------------------------------------------------------------
    with input select
    internal_f_e <= '0' when "0000",   -- |   0   |  0  |  0  |  0  |  0  |
                    '0' when "0001",   -- |   1   |  0  |  0  |  0  |  1  |
                    '0' when "0010",   -- |   2   |  0  |  0  |  1  |  0  |
                    '0' when "0011",   -- |   3   |  0  |  0  |  1  |  1  |
                    '0' when "0100",   -- |   4   |  0  |  1  |  0  |  0  |
                    '0' when "0101",   -- |   5   |  0  |  1  |  0  |  1  |
                    '0' when "0110",   -- |   6   |  0  |  1  |  1  |  0  |
                    '0' when "0111",   -- |   7   |  0  |  1  |  1  |  1  |
                    '0' when "1000",   -- |   8   |  1  |  0  |  0  |  0  |
                    '0' when "1001",   -- |   9   |  1  |  0  |  0  |  1  |
                    '0' when "1010",   -- |  10   |  1  |  0  |  1  |  0  |
                    '0' when "1011",   -- |  11   |  1  |  0  |  1  |  1  |
                    '0' when "1100",   -- |  12   |  1  |  1  |  0  |  0  |
                    '0' when "1101",   -- |  13   |  1  |  1  |  0  |  1  |
                    '0' when "1110",   -- |  14   |  1  |  1  |  1  |  0  |
                    '0' when "1111",   -- |  15   |  1  |  1  |  1  |  1  |
                    '0' when others;

-- ------------------------------------------------------------------------
-- |   funkcni hodnota f_f(d, c, b, a)    | index |  d  |  c  |  b  |  a  |
-- ------------------------------------------------------------------------
    with input select
    internal_f_f <= '0' when "0000",   -- |   0   |  0  |  0  |  0  |  0  |
                    '0' when "0001",   -- |   1   |  0  |  0  |  0  |  1  |
                    '0' when "0010",   -- |   2   |  0  |  0  |  1  |  0  |
                    '0' when "0011",   -- |   3   |  0  |  0  |  1  |  1  |
                    '0' when "0100",   -- |   4   |  0  |  1  |  0  |  0  |
                    '0' when "0101",   -- |   5   |  0  |  1  |  0  |  1  |
                    '0' when "0110",   -- |   6   |  0  |  1  |  1  |  0  |
                    '0' when "0111",   -- |   7   |  0  |  1  |  1  |  1  |
                    '0' when "1000",   -- |   8   |  1  |  0  |  0  |  0  |
                    '0' when "1001",   -- |   9   |  1  |  0  |  0  |  1  |
                    '0' when "1010",   -- |  10   |  1  |  0  |  1  |  0  |
                    '0' when "1011",   -- |  11   |  1  |  0  |  1  |  1  |
                    '0' when "1100",   -- |  12   |  1  |  1  |  0  |  0  |
                    '0' when "1101",   -- |  13   |  1  |  1  |  0  |  1  |
                    '0' when "1110",   -- |  14   |  1  |  1  |  1  |  0  |
                    '0' when "1111",   -- |  15   |  1  |  1  |  1  |  1  |
                    '0' when others;

-- ------------------------------------------------------------------------
-- |   funkcni hodnota f_g(d, c, b, a)    | index |  d  |  c  |  b  |  a  |
-- ------------------------------------------------------------------------
    with input select
    internal_f_g <= '0' when "0000",   -- |   0   |  0  |  0  |  0  |  0  |
                    '0' when "0001",   -- |   1   |  0  |  0  |  0  |  1  |
                    '0' when "0010",   -- |   2   |  0  |  0  |  1  |  0  |
                    '0' when "0011",   -- |   3   |  0  |  0  |  1  |  1  |
                    '0' when "0100",   -- |   4   |  0  |  1  |  0  |  0  |
                    '0' when "0101",   -- |   5   |  0  |  1  |  0  |  1  |
                    '0' when "0110",   -- |   6   |  0  |  1  |  1  |  0  |
                    '0' when "0111",   -- |   7   |  0  |  1  |  1  |  1  |
                    '0' when "1000",   -- |   8   |  1  |  0  |  0  |  0  |
                    '0' when "1001",   -- |   9   |  1  |  0  |  0  |  1  |
                    '0' when "1010",   -- |  10   |  1  |  0  |  1  |  0  |
                    '0' when "1011",   -- |  11   |  1  |  0  |  1  |  1  |
                    '0' when "1100",   -- |  12   |  1  |  1  |  0  |  0  |
                    '0' when "1101",   -- |  13   |  1  |  1  |  0  |  1  |
                    '0' when "1110",   -- |  14   |  1  |  1  |  1  |  0  |
                    '0' when "1111",   -- |  15   |  1  |  1  |  1  |  1  |
                    '0' when others;

end;
