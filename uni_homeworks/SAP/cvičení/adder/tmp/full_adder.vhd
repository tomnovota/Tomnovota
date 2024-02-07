library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity full_adder_vhdl is
   port (
      A    :  in  std_logic;
      B    :  in  std_logic;
      Cin  :  in  std_logic;
      Cout :  out std_logic;
      S    :  out std_logic
   );
end entity full_adder_vhdl;

architecture Behavioral of full_adder_vhdl is


      signal A_1     :    std_logic;
      signal and_0_O :    std_logic;
      signal and_1_O :    std_logic;
      signal B_1     :    std_logic;
      signal Cin_1   :    std_logic;
      signal or_0_O  :    std_logic;
      signal xor_0_O :    std_logic;
      signal xor_1_O :    std_logic;
begin
	A_1 <= A;
	B_1 <= B;
	Cin_1 <= Cin;
	Cout <= or_0_O;
	S <= xor_1_O;


	-- and_0
	and_0_O <= B_1 and A_1;

	-- and_1
	and_1_O <= Cin_1 and xor_0_O;

	-- or_0
	or_0_O <= and_1_O or and_0_O;

	-- xor_0
	xor_0_O <= A_1 xor B_1;

	-- xor_1
	xor_1_O <= xor_0_O xor Cin_1;

end Behavioral;
