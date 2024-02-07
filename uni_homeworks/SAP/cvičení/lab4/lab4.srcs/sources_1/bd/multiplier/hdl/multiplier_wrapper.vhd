--Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2018.2.1 (win64) Build 2288692 Thu Jul 26 18:24:02 MDT 2018
--Date        : Tue Mar  8 08:02:58 2022
--Host        : PC-1042-114 running 64-bit major release  (build 9200)
--Command     : generate_target multiplier_wrapper.bd
--Design      : multiplier_wrapper
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity multiplier_wrapper is
  port (
    AN0 : out STD_LOGIC;
    AN1 : out STD_LOGIC;
    AN2 : out STD_LOGIC;
    AN3 : out STD_LOGIC;
    a0 : in STD_LOGIC;
    a1 : in STD_LOGIC;
    b0 : in STD_LOGIC;
    b1 : in STD_LOGIC;
    f_a : out STD_LOGIC;
    f_b : out STD_LOGIC;
    f_c : out STD_LOGIC;
    f_d : out STD_LOGIC;
    f_e : out STD_LOGIC;
    f_f : out STD_LOGIC;
    f_g : out STD_LOGIC
  );
end multiplier_wrapper;

architecture STRUCTURE of multiplier_wrapper is
  component multiplier is
  port (
    AN1 : out STD_LOGIC;
    AN2 : out STD_LOGIC;
    AN0 : out STD_LOGIC;
    AN3 : out STD_LOGIC;
    a0 : in STD_LOGIC;
    a1 : in STD_LOGIC;
    b0 : in STD_LOGIC;
    b1 : in STD_LOGIC;
    f_a : out STD_LOGIC;
    f_b : out STD_LOGIC;
    f_c : out STD_LOGIC;
    f_d : out STD_LOGIC;
    f_e : out STD_LOGIC;
    f_f : out STD_LOGIC;
    f_g : out STD_LOGIC
  );
  end component multiplier;
begin
multiplier_i: component multiplier
     port map (
      AN0 => AN0,
      AN1 => AN1,
      AN2 => AN2,
      AN3 => AN3,
      a0 => a0,
      a1 => a1,
      b0 => b0,
      b1 => b1,
      f_a => f_a,
      f_b => f_b,
      f_c => f_c,
      f_d => f_d,
      f_e => f_e,
      f_f => f_f,
      f_g => f_g
    );
end STRUCTURE;
