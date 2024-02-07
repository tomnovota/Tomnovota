library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity seq_detector_vhdl is
   port (
      clk    :  in  std_logic;
      reset  :  in  std_logic;
      switch :  in  std_logic;
      detect :  out std_logic
   );
end entity seq_detector_vhdl;

architecture Behavioral of seq_detector_vhdl is
	component next_state_logic
		port (
			q0 :  in  std_logic;
			q1 :  in  std_logic;
			q2 :  in  std_logic;
			sw :  in  std_logic;
			d0 :  out std_logic;
			d1 :  out std_logic;
			d2 :  out std_logic
		);
	end component;

	component output_logic
		port (
			q0 :  in  std_logic;
			q1 :  in  std_logic;
			q2 :  in  std_logic;
			sw :  in  std_logic;
			y  :  out std_logic
		);
	end component;



      signal clk_1                 :    std_logic;         
      signal dff_0_q               :    std_logic;         
      signal dff_1_q               :    std_logic;         
      signal dff_2_q               :    std_logic;         
      signal next_state_logic_0_d0 :    std_logic;         
      signal next_state_logic_0_d1 :    std_logic;         
      signal next_state_logic_0_d2 :    std_logic;         
      signal output_logic_0_y      :    std_logic;         
      signal reset_1               :    std_logic;         
      signal switch_1              :    std_logic;         
begin
	clk_1 <= clk;
	reset_1 <= reset;
	switch_1 <= switch;
	detect <= output_logic_0_y;


	-- dff_0
	process (clk_1)
	begin
		if rising_edge(clk_1) then
			if reset_1 = '1' then
				dff_0_q <= '0';
			else
				dff_0_q <= next_state_logic_0_d0;
			end if;
		end if;
	end process;

	-- dff_1
	process (clk_1)
	begin
		if rising_edge(clk_1) then
			if reset_1 = '1' then
				dff_1_q <= '0';
			else
				dff_1_q <= next_state_logic_0_d1;
			end if;
		end if;
	end process;

	-- dff_2
	process (clk_1)
	begin
		if rising_edge(clk_1) then
			if reset_1 = '1' then
				dff_2_q <= '0';
			else
				dff_2_q <= next_state_logic_0_d2;
			end if;
		end if;
	end process;

	-- next_state_logic_0
	inst_next_state_logic_0 : next_state_logic
		port map (
			q0 =>  dff_0_q,
			q1 =>  dff_1_q,
			q2 =>  dff_2_q,
			sw =>  switch_1,
			d0 =>  next_state_logic_0_d0,
			d1 =>  next_state_logic_0_d1,
			d2 =>  next_state_logic_0_d2
		);

	-- output_logic_0
	inst_output_logic_0 : output_logic
		port map (
			q0 =>  dff_0_q,
			q1 =>  dff_1_q,
			q2 =>  dff_2_q,
			sw =>  switch_1,
			y  =>  output_logic_0_y
		);

end Behavioral;
