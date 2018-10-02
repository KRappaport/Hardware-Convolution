# This tcl script will setup and create the FPGA block diagram in Vivado

# Creates the project in Vivado and selects the ZedBoard as the target device
# The path should be changed accordingly
create_project conv2d_bd ./conv2d_shoup/conv2d_bd -part xc7z020clg484-1
set_property board_part em.avnet.com:zed:part0:1.4 [current_project]

# Create block design
create_bd_design "design_1"
update_compile_order -fileset sources_1

# Add HLS IP to the IP repository (change the path accordingly)
set_property  ip_repo_paths  ./conv2d_shoup/solution1 [current_project]
update_ip_catalog

# Add the Conv2d block to the block design
startgroup
create_bd_cell -type ip -vlnv xilinx.com:hls:conv2d:1.0 conv2d_0
endgroup

# Add the Zynq Processing System to the block design
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system7_0
endgroup
apply_bd_automation -rule xilinx.com:bd_rule:processing_system7 -config {make_external "FIXED_IO, DDR" apply_board_preset "1" Master "Disable" Slave "Disable" }  [get_bd_cells processing_system7_0]

# Add AXI Interconnect to the block design
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/processing_system7_0/M_AXI_GP0} Slave {/conv2d_0/s_axi_DIM} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins conv2d_0/s_axi_DIM]

# Add a High Performance port to the processing system
startgroup
set_property -dict [list CONFIG.PCW_USE_S_AXI_HP0 {1}] [get_bd_cells processing_system7_0]
endgroup

# Add AXI DMA to the block design
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dma:7.1 axi_dma_0
endgroup
set_property -dict [list CONFIG.c_include_sg {0} CONFIG.c_sg_length_width {26} CONFIG.c_sg_include_stscntrl_strm {0} CONFIG.c_include_mm2s_dre {1} CONFIG.c_include_s2mm_dre {1}] [get_bd_cells axi_dma_0]

# Connect the AXI DMA and the processing system
startgroup
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/axi_dma_0/M_AXI_MM2S} Slave {/processing_system7_0/S_AXI_HP0} intc_ip {Auto} master_apm {0}}  [get_bd_intf_pins processing_system7_0/S_AXI_HP0]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/processing_system7_0/FCLK_CLK0 (100 MHz)} Clk_slave {Auto} Clk_xbar {/processing_system7_0/FCLK_CLK0 (100 MHz)} Master {/processing_system7_0/M_AXI_GP0} Slave {/axi_dma_0/S_AXI_LITE} intc_ip {/ps7_0_axi_periph} master_apm {0}}  [get_bd_intf_pins axi_dma_0/S_AXI_LITE]
endgroup
startgroup
set_property -dict [list CONFIG.NUM_SI {2}] [get_bd_cells axi_smc]
endgroup
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {/processing_system7_0/FCLK_CLK0 (100 MHz)} Clk_xbar {/processing_system7_0/FCLK_CLK0 (100 MHz)} Master {/axi_dma_0/M_AXI_S2MM} Slave {/processing_system7_0/S_AXI_HP0} intc_ip {/axi_smc} master_apm {0}}  [get_bd_intf_pins axi_dma_0/M_AXI_S2MM]

# Add BRAM block to the block design
apply_bd_automation -rule xilinx.com:bd_rule:bram_cntlr -config {BRAM "Auto" }  [get_bd_intf_pins conv2d_0/ker_PORTA]
startgroup
set_property -dict [list CONFIG.Memory_Type {True_Dual_Port_RAM} CONFIG.Enable_B {Use_ENB_Pin} CONFIG.Use_RSTB_Pin {true} CONFIG.Port_B_Clock {100} CONFIG.Port_B_Write_Rate {50} CONFIG.Port_B_Enable_Rate {100}] [get_bd_cells conv2d_0_bram]
endgroup

# Add AXI BRAM Controller to the block design
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_0
endgroup
set_property -dict [list CONFIG.SINGLE_PORT_BRAM {1}] [get_bd_cells axi_bram_ctrl_0]

# Connect the BRAM to the processing system
apply_bd_automation -rule xilinx.com:bd_rule:bram_cntlr -config {BRAM "Auto" }  [get_bd_intf_pins axi_bram_ctrl_0/BRAM_PORTA]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/processing_system7_0/FCLK_CLK0 (100 MHz)} Clk_slave {Auto} Clk_xbar {/processing_system7_0/FCLK_CLK0 (100 MHz)} Master {/processing_system7_0/M_AXI_GP0} Slave {/axi_bram_ctrl_0/S_AXI} intc_ip {/ps7_0_axi_periph} master_apm {0}}  [get_bd_intf_pins axi_bram_ctrl_0/S_AXI]

# Connect the stream ports from Conv2d to the AXI DMA
connect_bd_intf_net [get_bd_intf_pins axi_dma_0/M_AXIS_MM2S] [get_bd_intf_pins conv2d_0/img]
connect_bd_intf_net [get_bd_intf_pins conv2d_0/img_out] [get_bd_intf_pins axi_dma_0/S_AXIS_S2MM]

# Rearrange the block design
regenerate_bd_layout

# Set the clock frequency for the FPGA (set to 100MHz)
startgroup
set_property -dict [list CONFIG.PCW_FPGA0_PERIPHERAL_FREQMHZ {100}] [get_bd_cells processing_system7_0]
endgroup

# Save block design
save_bd_design

# Generate output products (change paths accordingly)
generate_target all [get_files  ./conv2d_shoup/conv2d_bd/conv2d_bd.srcs/sources_1/bd/design_1/design_1.bd]
catch { config_ip_cache -export [get_ips -all design_1_conv2d_0_0] }
catch { config_ip_cache -export [get_ips -all design_1_processing_system7_0_0] }
catch { config_ip_cache -export [get_ips -all design_1_rst_ps7_0_100M_0] }
catch { config_ip_cache -export [get_ips -all design_1_axi_dma_0_0] }
catch { config_ip_cache -export [get_ips -all design_1_axi_smc_0] }
catch { config_ip_cache -export [get_ips -all design_1_conv2d_0_bram_0] }
catch { config_ip_cache -export [get_ips -all design_1_axi_bram_ctrl_0_0] }
catch { config_ip_cache -export [get_ips -all design_1_xbar_0] }
catch { config_ip_cache -export [get_ips -all design_1_auto_pc_1] }
catch { config_ip_cache -export [get_ips -all design_1_auto_pc_0] }
catch { config_ip_cache -export [get_ips -all design_1_auto_pc_2] }
# Change the paths here
export_ip_user_files -of_objects [get_files ./conv2d_shoup/conv2d_bd/conv2d_bd.srcs/sources_1/bd/design_1/design_1.bd] -no_script -sync -force -quiet
create_ip_run [get_files -of_objects [get_fileset sources_1] ./conv2d_shoup/conv2d_bd/conv2d_bd.srcs/sources_1/bd/design_1/design_1.bd]

# Runs here are launched using 16 jobs, this should be changed to fit the availabilty of threads on the CPU running the jobs
launch_runs -jobs 16 {design_1_conv2d_0_0_synth_1 design_1_processing_system7_0_0_synth_1 design_1_rst_ps7_0_100M_0_synth_1 design_1_axi_dma_0_0_synth_1 design_1_axi_smc_0_synth_1 design_1_conv2d_0_bram_0_synth_1 design_1_axi_bram_ctrl_0_0_synth_1 design_1_xbar_0_synth_1 design_1_auto_pc_1_synth_1 design_1_auto_pc_0_synth_1 design_1_auto_pc_2_synth_1}
# Change the paths here
export_simulation -of_objects [get_files ./conv2d_shoup/conv2d_bd/conv2d_bd.srcs/sources_1/bd/design_1/design_1.bd] -directory ./conv2d_shoup/conv2d_bd/conv2d_bd.ip_user_files/sim_scripts -ip_user_files_dir ./conv2d_shoup/conv2d_bd/conv2d_bd.ip_user_files -ipstatic_source_dir ./conv2d_shoup/conv2d_bd/conv2d_bd.ip_user_files/ipstatic -lib_map_path [list {modelsim=./conv2d_shoup/conv2d_bd/conv2d_bd.cache/compile_simlib/modelsim} {questa=./conv2d_shoup/conv2d_bd/conv2d_bd.cache/compile_simlib/questa} {ies=./conv2d_shoup/conv2d_bd/conv2d_bd.cache/compile_simlib/ies} {xcelium=./conv2d_shoup/conv2d_bd/conv2d_bd.cache/compile_simlib/xcelium} {vcs=./conv2d_shoup/conv2d_bd/conv2d_bd.cache/compile_simlib/vcs} {riviera=./conv2d_shoup/conv2d_bd/conv2d_bd.cache/compile_simlib/riviera}] -use_ip_compiled_libs -force -quiet

# Create HDL wrapper (change paths accordingly)
make_wrapper -files [get_files ./conv2d_shoup/conv2d_bd/conv2d_bd.srcs/sources_1/bd/design_1/design_1.bd] -top
add_files -norecurse ./conv2d_shoup/conv2d_bd/conv2d_bd.srcs/sources_1/bd/design_1/hdl/design_1_wrapper.v

# Generate Bitstream (here as well number of jobs should be set correctly)
launch_runs impl_1 -to_step write_bitstream -jobs 16

# Open GUI and block design
start_gui
# Change the path here
open_bd_design {./conv2d_shoup/conv2d_bd/conv2d_bd.srcs/sources_1/bd/design_1/design_1.bd}
