# Create project
open_project -reset jet_tagger

# Add design files
add_files ../algorithm/sim_tagger.cpp
add_files ../algorithm/helperFunctions.h
# Add test bench & data files
add_files -tb ../algorithm/testbench/sim_tagger_testbench.cpp
add_files -tb /home/larsonma/LargeRadiusJets/data/MemPrints/gFex/mc21_14TeV_hh_bbbb_vbf_novhh_gfex_smallrj.dat
add_files -tb /home/larsonma/LargeRadiusJets/data/MemPrints/gFex/mc21_14TeV_jj_JZ3_gfex_smallrj.dat
add_files -tb /home/larsonma/LargeRadiusJets/data/MemPrints/CaloTopo_422/mc21_14TeV_hh_bbbb_vbf_novhh_topo422.dat
add_files -tb /home/larsonma/LargeRadiusJets/data/MemPrints/CaloTopo_422/mc21_14TeV_jj_JZ3_topo422.dat

# Set the top-level function
set_top callEventLoop

# ########################################################
# Create a solution
open_solution -reset solution1 -flow_target vivado
# Define technology and clock rate
set_part  {XC7Z020-CLG484-1}
create_clock -period 4

#Set variable to select which steps to execute
set hls_exec 2


csim_design
# Set any optimization directives
set_directive_interface -mode m_axi pointer_basic d -depth 1
# End of directives

# cosim_design option -trace_level all is used to create a VCD waveform file
if {$hls_exec == 1} {
	# Run Synthesis and Exit
	csynth_design
	
} elseif {$hls_exec == 2} {
	# Run Synthesis, RTL Simulation and Exit
	csynth_design
	
	cosim_design -trace_level all
} elseif {$hls_exec == 3} { 
	# Run Synthesis, RTL Simulation, RTL implementation and Exit
	csynth_design
	
	cosim_design -trace_level all
	export_design
} else {
	# Default is to exit after setup
	csynth_design
}


exit