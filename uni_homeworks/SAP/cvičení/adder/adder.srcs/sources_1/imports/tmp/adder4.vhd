library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity adder4_vhdl is
   port (
      A0   :  in  std_logic;
      A1   :  in  std_logic;
      A2   :  in  std_logic;
      A3   :  in  std_logic;
      B0   :  in  std_logic;
      B1   :  in  std_logic;
      B2   :  in  std_logic;
      B3   :  in  std_logic;
      Cin  :  in  std_logic;
      Cout :  out std_logic;
      S0   :  out std_logic;
      S1   :  out std_logic;
      S2   :  out std_logic;
      S3   :  out std_logic
   );
end entity adder4_vhdl;

architecture Behavioral of adder4_vhdl is
	component full_adder_vhdl
		port (
			A    :  in  std_logic;
			B    :  in  std_logic;
			Cin  :  in  std_logic;
			Cout :  out std_logic;
			S    :  out std_logic
		);
	end component;



      signal A_0_1                  :    std_logic;          
      signal A_1_1                  :    std_logic;          
      signal A_2_1                  :    std_logic;          
      signal A_3_1                  :    std_logic;          
      signal B_0_1                  :    std_logic;          
      signal B_1_1                  :    std_logic;          
      signal B_2_1                  :    std_logic;          
      signal B_3_1                  :    std_logic;          
      signal Cin_0_1                :    std_logic;          
      signal full_adder_vhdl_0_Cout :    std_logic;          
      signal full_adder_vhdl_0_S    :    std_logic;          
      signal full_adder_vhdl_1_Cout :    std_logic;          
      signal full_adder_vhdl_1_S    :    std_logic;          
      signal full_adder_vhdl_2_Cout :    std_logic;          
      signal full_adder_vhdl_2_S    :    std_logic;          
      signal full_adder_vhdl_3_Cout :    std_logic;          
      signal full_adder_vhdl_3_S    :    std_logic;          
begin
	A_0_1 <= A0;
	A_1_1 <= A1;
	A_2_1 <= A2;
	A_3_1 <= A3;
	B_0_1 <= B0;
	B_1_1 <= B1;
	B_2_1 <= B2;
	B_3_1 <= B3;
	Cin_0_1 <= Cin;
	Cout <= full_adder_vhdl_3_Cout;
	S0 <= full_adder_vhdl_0_S;
	S1 <= full_adder_vhdl_1_S;
	S2 <= full_adder_vhdl_2_S;
	S3 <= full_adder_vhdl_3_S;


	-- full_adder_vhdl_0
	inst_full_adder_vhdl_0 : full_adder_vhdl
		port map (
			A    =>  A_0_1,
			B    =>  B_0_1,
			Cin  =>  Cin_0_1,
			Cout =>  full_adder_vhdl_0_Cout,
			S    =>  full_adder_vhdl_0_S
		);

	-- full_adder_vhdl_1
	inst_full_adder_vhdl_1 : full_adder_vhdl
		port map (
			A    =>  A_1_1,
			B    =>  B_1_1,
			Cin  =>  full_adder_vhdl_0_Cout,
			Cout =>  full_adder_vhdl_1_Cout,
			S    =>  full_adder_vhdl_1_S
		);

	-- full_adder_vhdl_2
	inst_full_adder_vhdl_2 : full_adder_vhdl
		port map (
			A    =>  A_2_1,
			B    =>  B_2_1,
			Cin  =>  full_adder_vhdl_1_Cout,
			Cout =>  full_adder_vhdl_2_Cout,
			S    =>  full_adder_vhdl_2_S
		);

	-- full_adder_vhdl_3
	inst_full_adder_vhdl_3 : full_adder_vhdl
		port map (
			A    =>  A_3_1,
			B    =>  B_3_1,
			Cin  =>  full_adder_vhdl_2_Cout,
			Cout =>  full_adder_vhdl_3_Cout,
			S    =>  full_adder_vhdl_3_S
		);

end Behavioral;
