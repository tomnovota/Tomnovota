library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity mux_7seg is
    port( 
        a   : out std_logic;
        a0  : in  std_logic;
        a1  : in  std_logic;
        a2  : in  std_logic;
        a3  : in  std_logic;
        b   : out std_logic;
        b0  : in  std_logic;
        b1  : in  std_logic;
        b2  : in  std_logic;
        b3  : in  std_logic;
        c   : out std_logic;
        c0  : in  std_logic;
        c1  : in  std_logic;
        c2  : in  std_logic;
        c3  : in  std_logic;
        d   : out std_logic;
        d0  : in  std_logic;
        d1  : in  std_logic;
        d2  : in  std_logic;
        d3  : in  std_logic;
        an0 : in  std_logic;
        an1 : in  std_logic;
        an2 : in  std_logic;
        an3 : in  std_logic
    );
end mux_7seg;

architecture Behavioral of mux_7seg is
    signal ans : std_logic_vector(3 downto 0);
begin

    ans <= an3 & an2 & an1 & an0;

    a <= a0 when ans = b"1110" else 
         a1 when ans = b"1101" else 
         a2 when ans = b"1011" else 
         a3;

    b <= b0 when ans = b"1110" else 
         b1 when ans = b"1101" else 
         b2 when ans = b"1011" else 
         b3;

    c <= c0 when ans = b"1110" else 
         c1 when ans = b"1101" else 
         c2 when ans = b"1011" else 
         c3;

    d <= d0 when ans = b"1110" else 
         d1 when ans = b"1101" else 
         d2 when ans = b"1011" else 
         d3;

end;
