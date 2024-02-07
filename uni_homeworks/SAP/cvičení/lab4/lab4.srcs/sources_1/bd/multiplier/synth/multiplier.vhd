--Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2018.2.1 (win64) Build 2288692 Thu Jul 26 18:24:02 MDT 2018
--Date        : Tue Mar  8 08:02:58 2022
--Host        : PC-1042-114 running 64-bit major release  (build 9200)
--Command     : generate_target multiplier.bd
--Design      : multiplier
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity multiplier is
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
  attribute CORE_GENERATION_INFO : string;
  attribute CORE_GENERATION_INFO of multiplier : entity is "multiplier,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=multiplier,x_ipVersion=1.00.a,x_ipLanguage=VHDL,numBlks=6,numReposBlks=6,numNonXlnxBlks=4,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=2,numPkgbdBlks=0,bdsource=USER,synth_mode=OOC_per_IP}";
  attribute HW_HANDOFF : string;
  attribute HW_HANDOFF of multiplier : entity is "multiplier.hwdef";
end multiplier;

architecture STRUCTURE of multiplier is
  component multiplier_gnd_0_0 is
  port (
    O : out STD_LOGIC
  );
  end component multiplier_gnd_0_0;
  component multiplier_vcc_0_0 is
  port (
    O : out STD_LOGIC
  );
  end component multiplier_vcc_0_0;
  component multiplier_vcc_0_1 is
  port (
    O : out STD_LOGIC
  );
  end component multiplier_vcc_0_1;
  component multiplier_vcc_0_2 is
  port (
    O : out STD_LOGIC
  );
  end component multiplier_vcc_0_2;
  component multiplier_decoder_7seg_0_0 is
  port (
    a : in STD_LOGIC;
    b : in STD_LOGIC;
    c : in STD_LOGIC;
    d : in STD_LOGIC;
    f_a : out STD_LOGIC;
    f_b : out STD_LOGIC;
    f_c : out STD_LOGIC;
    f_d : out STD_LOGIC;
    f_e : out STD_LOGIC;
    f_f : out STD_LOGIC;
    f_g : out STD_LOGIC
  );
  end component multiplier_decoder_7seg_0_0;
  component multiplier_multiplier_2x2_0_1 is
  port (
    b1 : in STD_LOGIC;
    b0 : in STD_LOGIC;
    a1 : in STD_LOGIC;
    a0 : in STD_LOGIC;
    y3 : out STD_LOGIC;
    y2 : out STD_LOGIC;
    y1 : out STD_LOGIC;
    y0 : out STD_LOGIC
  );
  end component multiplier_multiplier_2x2_0_1;
  signal a0_0_1 : STD_LOGIC;
  signal a1_0_1 : STD_LOGIC;
  signal b0_0_1 : STD_LOGIC;
  signal b1_0_1 : STD_LOGIC;
  signal decoder_7seg_0_f_a : STD_LOGIC;
  signal decoder_7seg_0_f_b : STD_LOGIC;
  signal decoder_7seg_0_f_c : STD_LOGIC;
  signal decoder_7seg_0_f_d : STD_LOGIC;
  signal decoder_7seg_0_f_e : STD_LOGIC;
  signal decoder_7seg_0_f_f : STD_LOGIC;
  signal decoder_7seg_0_f_g : STD_LOGIC;
  signal gnd_0_O : STD_LOGIC;
  signal multiplier_2x2_0_y0 : STD_LOGIC;
  signal multiplier_2x2_0_y1 : STD_LOGIC;
  signal multiplier_2x2_0_y2 : STD_LOGIC;
  signal multiplier_2x2_0_y3 : STD_LOGIC;
  signal vcc_0_O : STD_LOGIC;
  signal vcc_1_O : STD_LOGIC;
  signal vcc_2_O : STD_LOGIC;
begin
  AN0 <= gnd_0_O;
  AN1 <= vcc_0_O;
  AN2 <= vcc_1_O;
  AN3 <= vcc_2_O;
  a0_0_1 <= a0;
  a1_0_1 <= a1;
  b0_0_1 <= b0;
  b1_0_1 <= b1;
  f_a <= decoder_7seg_0_f_a;
  f_b <= decoder_7seg_0_f_b;
  f_c <= decoder_7seg_0_f_c;
  f_d <= decoder_7seg_0_f_d;
  f_e <= decoder_7seg_0_f_e;
  f_f <= decoder_7seg_0_f_f;
  f_g <= decoder_7seg_0_f_g;
decoder_7seg_0: component multiplier_decoder_7seg_0_0
     port map (
      a => multiplier_2x2_0_y0,
      b => multiplier_2x2_0_y1,
      c => multiplier_2x2_0_y2,
      d => multiplier_2x2_0_y3,
      f_a => decoder_7seg_0_f_a,
      f_b => decoder_7seg_0_f_b,
      f_c => decoder_7seg_0_f_c,
      f_d => decoder_7seg_0_f_d,
      f_e => decoder_7seg_0_f_e,
      f_f => decoder_7seg_0_f_f,
      f_g => decoder_7seg_0_f_g
    );
gnd_0: component multiplier_gnd_0_0
     port map (
      O => gnd_0_O
    );
multiplier_2x2_0: component multiplier_multiplier_2x2_0_1
     port map (
      a0 => a0_0_1,
      a1 => a1_0_1,
      b0 => b0_0_1,
      b1 => b1_0_1,
      y0 => multiplier_2x2_0_y0,
      y1 => multiplier_2x2_0_y1,
      y2 => multiplier_2x2_0_y2,
      y3 => multiplier_2x2_0_y3
    );
vcc_0: component multiplier_vcc_0_0
     port map (
      O => vcc_0_O
    );
vcc_1: component multiplier_vcc_0_1
     port map (
      O => vcc_1_O
    );
vcc_2: component multiplier_vcc_0_2
     port map (
      O => vcc_2_O
    );
end STRUCTURE;
