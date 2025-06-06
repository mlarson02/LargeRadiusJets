# to run: vitis-run --mode hls --tcl jet_tagger.tcl
# Create project
open_project jet_tagger

# Add design files
add_files ../algorithm/jet_tagger_top.h
add_files ../algorithm/jet_tagger_top.cc
add_files ../algorithm/process_event.h
add_files ../algorithm/process_event.cc
add_files ../algorithm/helperFunctions.h
add_files ../algorithm/constants.h

# Add test bench & data files
add_files -tb ../algorithm/testbench/jet_tagger_testbench.cpp
add_files -tb ../algorithm/fileRead.h
add_files -tb /home/larsonma/LargeRadiusJets/data/LUTs/deltaR2Cut.dat
add_files -tb /home/larsonma/LargeRadiusJets/data/MemPrints/gFex/mc21_14TeV_hh_bbbb_vbf_novhh_gfex_smallrj.dat
add_files -tb /home/larsonma/LargeRadiusJets/data/MemPrints/gFex/mc21_14TeV_jj_JZ3_gfex_smallrj.dat
add_files -tb /home/larsonma/LargeRadiusJets/data/MemPrints/CaloTopo_422/mc21_14TeV_hh_bbbb_vbf_novhh_topo422.dat
add_files -tb /home/larsonma/LargeRadiusJets/data/MemPrints/CaloTopo_422/mc21_14TeV_jj_JZ3_topo422.dat

# Set the top-level function
set_top jet_tagger_top

# ########################################################
# Create a solution
open_solution partition_unroll_64_unrollseed -flow_target vivado
# Define technology and clock rate
set_part {xcvp1802-lsvc4072-2MP-e-S}
create_clock -period 4

#Set variable to select which steps to execute
set hls_exec 1


csim_design

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