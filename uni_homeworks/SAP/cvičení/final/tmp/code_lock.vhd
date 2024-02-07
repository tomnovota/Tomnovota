library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity code_lock_vhdl is
   port (
      a     :  in  std_logic;
      b     :  in  std_logic;
      c     :  in  std_logic;
      clk   :  in  std_logic;
      reset :  in  std_logic;
      AN0   :  out std_logic;
      AN1   :  out std_logic;
      AN2   :  out std_logic;
      AN3   :  out std_logic;
      f_a   :  out std_logic;
      f_b   :  out std_logic;
      f_c   :  out std_logic;
      f_d   :  out std_logic;
      f_e   :  out std_logic;
      f_f   :  out std_logic;
      f_g   :  out std_logic;
      y     :  out std_logic
   );
end entity code_lock_vhdl;

architecture Behavioral of code_lock_vhdl is
	component button_vhdl
		port (
			clk    :  in  std_logic;
			tl_in  :  in  std_logic;
			tl_out :  out std_logic
		);
	end component;

	component decoder_7seg
		port (
			a   :  in  std_logic;
			b   :  in  std_logic;
			c   :  in  std_logic;
			d   :  in  std_logic;
			f_a :  out std_logic;
			f_b :  out std_logic;
			f_c :  out std_logic;
			f_d :  out std_logic;
			f_e :  out std_logic;
			f_f :  out std_logic;
			f_g :  out std_logic
		);
	end component;

	component dekoder
		port (
			a  :  in  std_logic;
			b  :  in  std_logic;
			c  :  in  std_logic;
			x0 :  out std_logic;
			x1 :  out std_logic
		);
	end component;

	component next_state_logic
		port (
			q0 :  in  std_logic;
			q1 :  in  std_logic;
			q2 :  in  std_logic;
			x0 :  in  std_logic;
			x1 :  in  std_logic;
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
			y  :  out std_logic
		);
	end component;



      signal a_1                   :    std_logic;         
      signal b_1                   :    std_logic;         
      signal button_vhdl_0_tl_out  :    std_logic;         
      signal button_vhdl_1_tl_out  :    std_logic;         
      signal button_vhdl_2_tl_out  :    std_logic;         
      signal c_1                   :    std_logic;         
      signal clk_1                 :    std_logic;         
      signal decoder_7seg_0_f_a    :    std_logic;         
      signal decoder_7seg_0_f_b    :    std_logic;         
      signal decoder_7seg_0_f_c    :    std_logic;         
      signal decoder_7seg_0_f_d    :    std_logic;         
      signal decoder_7seg_0_f_e    :    std_logic;         
      signal decoder_7seg_0_f_f    :    std_logic;         
      signal decoder_7seg_0_f_g    :    std_logic;         
      signal dekoder_0_x0          :    std_logic;         
      signal dekoder_0_x1          :    std_logic;         
      signal dff_0_q               :    std_logic;         
      signal dff_1_q               :    std_logic;         
      signal dff_2_q               :    std_logic;         
      signal gnd_0_O               :    std_logic;         
      signal gnd_1_O               :    std_logic;         
      signal next_state_logic_0_d0 :    std_logic;         
      signal next_state_logic_0_d1 :    std_logic;         
      signal next_state_logic_0_d2 :    std_logic;         
      signal output_logic_0_y      :    std_logic;         
      signal reset_1               :    std_logic;         
      signal vcc_1_O               :    std_logic;         
      signal vcc_2_O               :    std_logic;         
      signal vcc_3_O               :    std_logic;         
begin
	a_1 <= a;
	b_1 <= b;
	c_1 <= c;
	clk_1 <= clk;
	reset_1 <= reset;
	AN0 <= gnd_0_O;
	AN1 <= vcc_3_O;
	AN2 <= vcc_2_O;
	AN3 <= vcc_1_O;
	f_a <= decoder_7seg_0_f_a;
	f_b <= decoder_7seg_0_f_b;
	f_c <= decoder_7seg_0_f_c;
	f_d <= decoder_7seg_0_f_d;
	f_e <= decoder_7seg_0_f_e;
	f_f <= decoder_7seg_0_f_f;
	f_g <= decoder_7seg_0_f_g;
	y <= output_logic_0_y;


	-- button_vhdl_0
	inst_button_vhdl_0 : button_vhdl
		port map (
			clk    =>  clk_1 ,
			tl_in  =>  c_1   ,
			tl_out =>  button_vhdl_0_tl_out
		);

	-- button_vhdl_1
	inst_button_vhdl_1 : button_vhdl
		port map (
			clk    =>  clk_1 ,
			tl_in  =>  b_1   ,
			tl_out =>  button_vhdl_1_tl_out
		);

	-- button_vhdl_2
	inst_button_vhdl_2 : button_vhdl
		port map (
			clk    =>  clk_1 ,
			tl_in  =>  a_1   ,
			tl_out =>  button_vhdl_2_tl_out
		);

	-- decoder_7seg_0
	inst_decoder_7seg_0 : decoder_7seg
		port map (
			a   =>  dff_0_q,
			b   =>  dff_1_q,
			c   =>  dff_2_q,
			d   =>  gnd_1_O,
			f_a =>  decoder_7seg_0_f_a,
			f_b =>  decoder_7seg_0_f_b,
			f_c =>  decoder_7seg_0_f_c,
			f_d =>  decoder_7seg_0_f_d,
			f_e =>  decoder_7seg_0_f_e,
			f_f =>  decoder_7seg_0_f_f,
			f_g =>  decoder_7seg_0_f_g
		);

	-- dekoder_0
	inst_dekoder_0 : dekoder
		port map (
			a  =>  button_vhdl_2_tl_out,
			b  =>  button_vhdl_1_tl_out,
			c  =>  button_vhdl_0_tl_out,
			x0 =>  dekoder_0_x0,
			x1 =>  dekoder_0_x1
		);

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

	-- gnd_0
	gnd_0_O <= '0';

	-- gnd_1
	gnd_1_O <= '0';

	-- next_state_logic_0
	inst_next_state_logic_0 : next_state_logic
		port map (
			q0 =>  dff_0_q,
			q1 =>  dff_1_q,
			q2 =>  dff_2_q,
			x0 =>  dekoder_0_x0,
			x1 =>  dekoder_0_x1,
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
			y  =>  output_logic_0_y
		);

	-- vcc_1
	vcc_1_O <= '1';

	-- vcc_2
	vcc_2_O <= '1';

	-- vcc_3
	vcc_3_O <= '1';

end Behavioral;
