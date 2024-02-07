library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity schema_4_vhdl is
   port (
      a :  in  std_logic;
      b :  in  std_logic;
      c :  in  std_logic;
      d :  in  std_logic;
      f :  out std_logic
   );
end entity schema_4_vhdl;

architecture Behavioral of schema_4_vhdl is


      signal A_1     :    std_logic;
      signal and_0_O :    std_logic;
      signal and_1_O :    std_logic;
      signal and_2_O :    std_logic;
      signal B_1     :    std_logic;
      signal C_1     :    std_logic;
      signal D_1     :    std_logic;
      signal inv_0_O :    std_logic;
      signal inv_1_O :    std_logic;
      signal or_0_O  :    std_logic;
begin
	A_1 <= a;
	B_1 <= b;
	C_1 <= c;
	D_1 <= d;
	f <= or_0_O;


	-- and_0
	and_0_O <= inv_0_O and B_1 and C_1;

	-- and_1
	and_1_O <= B_1 and C_1 and D_1;

	-- and_2
	and_2_O <= A_1 and inv_1_O;

	-- inv_0
	inv_0_O <= not(A_1);

	-- inv_1
	inv_1_O <= not(C_1);

	-- or_0
	or_0_O <= and_2_O or and_0_O or and_1_O;

end Behavioral;
