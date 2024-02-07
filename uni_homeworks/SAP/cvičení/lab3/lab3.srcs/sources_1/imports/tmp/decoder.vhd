library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity decoder_vhdl is
   port (
      a   :  in  std_logic;
      b   :  in  std_logic;
      c   :  in  std_logic;
      d   :  in  std_logic;
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
end entity decoder_vhdl;

architecture Behavioral of decoder_vhdl is
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



      signal a_0_1              :    std_logic;      
      signal b_0_1              :    std_logic;      
      signal c_0_1              :    std_logic;      
      signal d_0_1              :    std_logic;      
      signal decoder_7seg_0_f_a :    std_logic;      
      signal decoder_7seg_0_f_b :    std_logic;      
      signal decoder_7seg_0_f_c :    std_logic;      
      signal decoder_7seg_0_f_d :    std_logic;      
      signal decoder_7seg_0_f_e :    std_logic;      
      signal decoder_7seg_0_f_f :    std_logic;      
      signal decoder_7seg_0_f_g :    std_logic;      
      signal gnd_0_O            :    std_logic;      
      signal vcc_0_O            :    std_logic;      
      signal vcc_1_O            :    std_logic;      
      signal vcc_2_O            :    std_logic;      
begin
	a_0_1 <= a;
	b_0_1 <= b;
	c_0_1 <= c;
	d_0_1 <= d;
	AN0 <= gnd_0_O;
	AN1 <= vcc_1_O;
	AN2 <= vcc_2_O;
	AN3 <= vcc_0_O;
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
			a   =>  a_0_1,
			b   =>  b_0_1,
			c   =>  c_0_1,
			d   =>  d_0_1,
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

	-- vcc_0
	vcc_0_O <= '1';

	-- vcc_1
	vcc_1_O <= '1';

	-- vcc_2
	vcc_2_O <= '1';

end Behavioral;
