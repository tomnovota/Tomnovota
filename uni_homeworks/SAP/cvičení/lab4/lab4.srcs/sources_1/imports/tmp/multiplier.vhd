library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity multiplier_vhdl is
   port (
      a0  :  in  std_logic;
      a1  :  in  std_logic;
      b0  :  in  std_logic;
      b1  :  in  std_logic;
      AN0 :  out std_logic;
      AN1 :  out std_logic;
      AN2 :  out std_logic;
      AN3 :  out std_logic;
      f_a :  out std_logic;
      f_b :  out std_logic;
      f_c :  out std_logic;
      f_d :  out std_logic;
      f_e :  out std_logic;
      f_f :  out std_logic;
      f_g :  out std_logic
   );
end entity multiplier_vhdl;

architecture Behavioral of multiplier_vhdl is
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

	component multiplier_2x2
		port (
			a0 :  in  std_logic;
			a1 :  in  std_logic;
			b0 :  in  std_logic;
			b1 :  in  std_logic;
			y0 :  out std_logic;
			y1 :  out std_logic;
			y2 :  out std_logic;
			y3 :  out std_logic
		);
	end component;



      signal a0_1                :    std_logic;       
      signal a1_1                :    std_logic;       
      signal b0_1                :    std_logic;       
      signal b1_1                :    std_logic;       
      signal decoder_7seg_0_f_a  :    std_logic;       
      signal decoder_7seg_0_f_b  :    std_logic;       
      signal decoder_7seg_0_f_c  :    std_logic;       
      signal decoder_7seg_0_f_d  :    std_logic;       
      signal decoder_7seg_0_f_e  :    std_logic;       
      signal decoder_7seg_0_f_f  :    std_logic;       
      signal decoder_7seg_0_f_g  :    std_logic;       
      signal gnd_0_O             :    std_logic;       
      signal multiplier_2x2_0_y0 :    std_logic;       
      signal multiplier_2x2_0_y1 :    std_logic;       
      signal multiplier_2x2_0_y2 :    std_logic;       
      signal multiplier_2x2_0_y3 :    std_logic;       
      signal vcc_0_O             :    std_logic;       
      signal vcc_1_O             :    std_logic;       
      signal vcc_2_O             :    std_logic;       
begin
	a0_1 <= a0;
	a1_1 <= a1;
	b0_1 <= b0;
	b1_1 <= b1;
	AN0 <= gnd_0_O;
	AN1 <= vcc_0_O;
	AN2 <= vcc_1_O;
	AN3 <= vcc_2_O;
	f_a <= decoder_7seg_0_f_a;
	f_b <= decoder_7seg_0_f_b;
	f_c <= decoder_7seg_0_f_c;
	f_d <= decoder_7seg_0_f_d;
	f_e <= decoder_7seg_0_f_e;
	f_f <= decoder_7seg_0_f_f;
	f_g <= decoder_7seg_0_f_g;


	-- decoder_7seg_0
	inst_decoder_7seg_0 : decoder_7seg
		port map (
			a   =>  multiplier_2x2_0_y0,
			b   =>  multiplier_2x2_0_y1,
			c   =>  multiplier_2x2_0_y2,
			d   =>  multiplier_2x2_0_y3,
			f_a =>  decoder_7seg_0_f_a,
			f_b =>  decoder_7seg_0_f_b,
			f_c =>  decoder_7seg_0_f_c,
			f_d =>  decoder_7seg_0_f_d,
			f_e =>  decoder_7seg_0_f_e,
			f_f =>  decoder_7seg_0_f_f,
			f_g =>  decoder_7seg_0_f_g
		);

	-- gnd_0
	gnd_0_O <= '0';

	-- multiplier_2x2_0
	inst_multiplier_2x2_0 : multiplier_2x2
		port map (
			a0 =>  a0_1,
			a1 =>  a1_1,
			b0 =>  b0_1,
			b1 =>  b1_1,
			y0 =>  multiplier_2x2_0_y0,
			y1 =>  multiplier_2x2_0_y1,
			y2 =>  multiplier_2x2_0_y2,
			y3 =>  multiplier_2x2_0_y3
		);

	-- vcc_0
	vcc_0_O <= '1';

	-- vcc_1
	vcc_1_O <= '1';

	-- vcc_2
	vcc_2_O <= '1';

end Behavioral;
