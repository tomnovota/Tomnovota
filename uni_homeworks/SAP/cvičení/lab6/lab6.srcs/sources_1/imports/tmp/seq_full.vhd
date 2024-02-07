library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity seq_full_vhdl is
   port (
      btn    :  in  std_logic;
      clk    :  in  std_logic;
      reset  :  in  std_logic;
      switch :  in  std_logic;
      detect :  out std_logic
   );
end entity seq_full_vhdl;

architecture Behavioral of seq_full_vhdl is
	component seq_detector_vhdl
		port (
			clk    :  in  std_logic;
			reset  :  in  std_logic;
			switch :  in  std_logic;
			detect :  out std_logic
		);
	end component;

	component debounce
		generic (
			width: integer := 22
		);
		port (
			clk    : in  std_logic;
			tl_in  : in  std_logic;
			tl_out : out std_logic
		);
	end component;


      signal clk_1                      :    std_logic;              
      signal debounce_0_tl_out          :    std_logic;              
      signal reset_1                    :    std_logic;              
      signal seq_detector_vhdl_0_detect :    std_logic;              
      signal switch_1                   :    std_logic;              
      signal tl_in_0_1                  :    std_logic;              
begin
	tl_in_0_1 <= btn;
	clk_1 <= clk;
	reset_1 <= reset;
	switch_1 <= switch;
	detect <= seq_detector_vhdl_0_detect;


	-- debounce_0
	debounce_0 : debounce
		generic map (
			width => 22
		)
		port map (
			clk	=> clk_1,
			tl_in	=> tl_in_0_1,
			tl_out	=> debounce_0_tl_out
		);

	-- seq_detector_vhdl_0
	inst_seq_detector_vhdl_0 : seq_detector_vhdl
		port map (
			clk    =>  debounce_0_tl_out,
			reset  =>  reset_1,
			switch =>  switch_1,
			detect =>  seq_detector_vhdl_0_detect
		);

end Behavioral;


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;

entity debounce is
    generic (
		width: integer := 22 -- 100 MHz clock -> 4 - simulation (90 ns); 22 - synthesis (ca. 20 ms)
	);
    port (
		clk : in STD_LOGIC;
        tl_in : in STD_LOGIC;
        tl_out : out STD_LOGIC
	);
end entity debounce;

architecture Behavioral of debounce is

signal cnt       : std_logic_vector(width-1 downto 0) := (others => '0');
signal reset, ce : std_logic := '0';
signal tl_prev   : std_logic := '0';


begin

    counter: process(clk) begin
        if rising_edge(clk) then 
            if (reset = '1') then
                cnt <= (others => '0');
            elsif (ce = '1') then
                cnt <= cnt + 1;
            end if;
        end if;
    end process;

    process(clk) begin
        if rising_edge(clk) then
            if (tl_prev /= tl_in) then
                reset <= '1';
            else 
                reset <= '0';
            end if;
        end if;
    end process;
                
    process(clk) begin
        if rising_edge(clk) then
            if (reset = '1') then
                tl_prev <= tl_in;
            end if;
        end if;
    end process;
                
    process(clk) begin
        if rising_edge(clk) then
            if ((cnt(width-1) = '1') and (cnt(0) = '0')) then
                tl_out <= tl_prev;
            end if;
        end if;
    end process;
                

    ce <= '0' when ((cnt(width-1) = '1') and (cnt(0) = '1'))
                else '1';

end Behavioral;

