library STD;
use STD.textio.all;

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_textio.all;

entity multiplier_test is
end multiplier_test;

architecture behavioral of multiplier_test is 

    signal b : std_logic_vector(1 downto 0);
    signal a : std_logic_vector(1 downto 0);
    signal y : std_logic_vector(3 downto 0);

    constant DELAY : time := 60 ns;

    -- poznamka pro cvicici: deklarace komponenty neni nutna - vyber testovaneho obvodu je az u konkretni instance

begin
 
-- definition of tested entity and its inputs and outputs
-- inputs:  a1, a0, b1, b0 mapped to a, b signals
-- outputs: y3, y2, y1, y0 mapped to y signal
-- ATTENTION!
-- the entity name (entity work.multiplier) must be equal to the tested component name
-- in this case: multiplier
-- the input and output names (left part before arrow) must be equal to the names in the tested design

-- definice vstupu a vystupu testovane komponenty
-- vstupy:  a1, a0, b1, b0 pripojene na signaly a, b
-- vystupy: y3, y2, y1, y0 pripojene na signal y
-- POZOR!
-- nazev entity (entity work.multiplier) musi byt stejny jako nazev testovane entity
-- v tomto pripade: multiplier
-- vstupy a vystupy (v leve casti pred sipkou) musi byt stejne jako popisy vstupu a vystupu testovane entity

    uut: entity work.multiplier
    port map(
        a1 => a(1),
        a0 => a(0),
        b1 => b(1),
        b0 => b(0),
        y3 => y(3),
        y2 => y(2),
        y1 => y(1),
        y0 => y(0)
    );

    stim_proc: process
        variable file_line : line;
    begin

        write(file_line, string'("### Simulation start ###"));
        writeline(output, file_line);
        write(file_line, string'("Index: B * A = Y"));
        writeline(output, file_line);

        -- testing all combinations
        -- 2 2-bit inputs => 2 for-cycles => 4^2 combinations
        -- testovani vsech moznych kombinaci
        -- 2 2bitove vstupy => 2 for-cykly => 4^2 moznych kombinaci
        for i in 0 to 3 loop
            for j in 0 to 3 loop
                b <= conv_std_logic_vector(i, 2);
                a <= conv_std_logic_vector(j, 2);
                wait for 1 ns;
                write(file_line, i*4+j);
                write(file_line, string'(": "));
                write(file_line, b);
                write(file_line, string'(" * "));
                write(file_line, a);
                write(file_line, string'(" = "));
                write(file_line, y);
                writeline(output, file_line);
                wait for DELAY;
            end loop;
        end loop;

        -- konec simulace
        write(file_line, string'("### Total number of errors = Unknown - check logs manually / zkontrolujte logy rucne"));
        writeline(output, file_line);

        write(file_line, string'("### Simulation finished ###"));
        writeline(output, file_line);

        wait;

    end process;

end;
