
################################################################
# This is a generated script based on design: multiplier
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2018.2
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   catch {common::send_msg_id "BD_TCL-109" "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source multiplier_script.tcl


# The design that will be created by this Tcl script contains the following 
# module references:
# decoder_7seg, multiplier_2x2

# Please add the sources of those modules before sourcing this Tcl script.

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xc7a35tcpg236-1
   set_property BOARD_PART digilentinc.com:basys3:part0:1.1 [current_project]
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name multiplier

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      common::send_msg_id "BD_TCL-001" "INFO" "Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   common::send_msg_id "BD_TCL-002" "INFO" "Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   common::send_msg_id "BD_TCL-003" "INFO" "Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   common::send_msg_id "BD_TCL-004" "INFO" "Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

common::send_msg_id "BD_TCL-005" "INFO" "Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   catch {common::send_msg_id "BD_TCL-114" "ERROR" $errMsg}
   return $nRet
}

##################################################################
# DESIGN PROCs
##################################################################



# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  variable script_folder
  variable design_name

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_msg_id "BD_TCL-100" "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_msg_id "BD_TCL-101" "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports

  # Create ports
  set AN0 [ create_bd_port -dir O AN0 ]
  set AN1 [ create_bd_port -dir O AN1 ]
  set AN2 [ create_bd_port -dir O AN2 ]
  set AN3 [ create_bd_port -dir O AN3 ]
  set a0 [ create_bd_port -dir I a0 ]
  set a1 [ create_bd_port -dir I a1 ]
  set b0 [ create_bd_port -dir I b0 ]
  set b1 [ create_bd_port -dir I b1 ]
  set f_a [ create_bd_port -dir O f_a ]
  set f_b [ create_bd_port -dir O f_b ]
  set f_c [ create_bd_port -dir O f_c ]
  set f_d [ create_bd_port -dir O f_d ]
  set f_e [ create_bd_port -dir O f_e ]
  set f_f [ create_bd_port -dir O f_f ]
  set f_g [ create_bd_port -dir O f_g ]

  # Create instance: decoder_7seg_0, and set properties
  set block_name decoder_7seg
  set block_cell_name decoder_7seg_0
  if { [catch {set decoder_7seg_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $decoder_7seg_0 eq "" } {
     catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: gnd_0, and set properties
  set gnd_0 [ create_bd_cell -type ip -vlnv FIT:user:gnd:1.0 gnd_0 ]

  # Create instance: multiplier_2x2_0, and set properties
  set block_name multiplier_2x2
  set block_cell_name multiplier_2x2_0
  if { [catch {set multiplier_2x2_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $multiplier_2x2_0 eq "" } {
     catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: vcc_0, and set properties
  set vcc_0 [ create_bd_cell -type ip -vlnv FIT:user:vcc:1.0 vcc_0 ]

  # Create instance: vcc_1, and set properties
  set vcc_1 [ create_bd_cell -type ip -vlnv FIT:user:vcc:1.0 vcc_1 ]

  # Create instance: vcc_2, and set properties
  set vcc_2 [ create_bd_cell -type ip -vlnv FIT:user:vcc:1.0 vcc_2 ]

  # Create port connections
  connect_bd_net -net a0_0_1 [get_bd_ports a0] [get_bd_pins multiplier_2x2_0/a0]
  connect_bd_net -net a1_0_1 [get_bd_ports a1] [get_bd_pins multiplier_2x2_0/a1]
  connect_bd_net -net b0_0_1 [get_bd_ports b0] [get_bd_pins multiplier_2x2_0/b0]
  connect_bd_net -net b1_0_1 [get_bd_ports b1] [get_bd_pins multiplier_2x2_0/b1]
  connect_bd_net -net decoder_7seg_0_f_a [get_bd_ports f_a] [get_bd_pins decoder_7seg_0/f_a]
  connect_bd_net -net decoder_7seg_0_f_b [get_bd_ports f_b] [get_bd_pins decoder_7seg_0/f_b]
  connect_bd_net -net decoder_7seg_0_f_c [get_bd_ports f_c] [get_bd_pins decoder_7seg_0/f_c]
  connect_bd_net -net decoder_7seg_0_f_d [get_bd_ports f_d] [get_bd_pins decoder_7seg_0/f_d]
  connect_bd_net -net decoder_7seg_0_f_e [get_bd_ports f_e] [get_bd_pins decoder_7seg_0/f_e]
  connect_bd_net -net decoder_7seg_0_f_f [get_bd_ports f_f] [get_bd_pins decoder_7seg_0/f_f]
  connect_bd_net -net decoder_7seg_0_f_g [get_bd_ports f_g] [get_bd_pins decoder_7seg_0/f_g]
  connect_bd_net -net gnd_0_O [get_bd_ports AN0] [get_bd_pins gnd_0/O]
  connect_bd_net -net multiplier_2x2_0_y0 [get_bd_pins decoder_7seg_0/a] [get_bd_pins multiplier_2x2_0/y0]
  connect_bd_net -net multiplier_2x2_0_y1 [get_bd_pins decoder_7seg_0/b] [get_bd_pins multiplier_2x2_0/y1]
  connect_bd_net -net multiplier_2x2_0_y2 [get_bd_pins decoder_7seg_0/c] [get_bd_pins multiplier_2x2_0/y2]
  connect_bd_net -net multiplier_2x2_0_y3 [get_bd_pins decoder_7seg_0/d] [get_bd_pins multiplier_2x2_0/y3]
  connect_bd_net -net vcc_0_O [get_bd_ports AN1] [get_bd_pins vcc_0/O]
  connect_bd_net -net vcc_1_O [get_bd_ports AN2] [get_bd_pins vcc_1/O]
  connect_bd_net -net vcc_2_O [get_bd_ports AN3] [get_bd_pins vcc_2/O]

  # Create address segments


  # Restore current instance
  current_bd_instance $oldCurInst

  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


