library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

entity digit_selector is
    port(
        clk : in  std_logic;
        rst : in  std_logic;
        an0 : out std_logic;
        an1 : out std_logic;
        an2 : out std_logic;
        an3 : out std_logic
    );
end digit_selector;

architecture behavioral of digit_selector is

    constant clk_freq  : integer := 100000000;         -- 1s
    constant one_tick  : integer := (clk_freq/1000)*2; -- 2ms
    constant const_top : integer := one_tick*4;        -- 8ms

    signal sig_counter : integer range 0 to const_top;

    ATTRIBUTE X_INTERFACE_PARAMETER : STRING;
    ATTRIBUTE X_INTERFACE_PARAMETER OF rst: SIGNAL IS "XIL_INTERFACENAME Reset, POLARITY ACTIVE_HIGH";

begin

    AN0 <= '0' when sig_counter < one_tick else '1';
    AN1 <= '0' when sig_counter < one_tick*2 and sig_counter >= one_tick else '1';
    AN2 <= '0' when sig_counter < one_tick*3 and sig_counter >= one_tick*2 else '1';
    AN3 <= '0' when sig_counter < one_tick*4 and sig_counter >= one_tick*3 else '1';

    proc_counter: process(clk, rst)
    begin
        if (rst='1') then
            sig_counter <= 0;
        elsif (clk='1' and clk'event) then
            sig_counter <= sig_counter + 1;
            if (sig_counter=const_top) then
                sig_counter <= 0;
            end if;
        end if;
    end process;

end;
