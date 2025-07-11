set moduleName et_calculation_1
set isTopModule 0
set isCombinational 0
set isDatapathOnly 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set hasInterrupt 0
set DLRegFirstOffset 0
set DLRegItemOffset 0
set svuvm_can_support 1
set cdfgNum 6
set C_modelName {et_calculation.1}
set C_modelType { void 0 }
set ap_memory_interface_dict [dict create]
dict set ap_memory_interface_dict inputObjectValues_1_0 { MEM_WIDTH 27 MEM_SIZE 128 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict inputObjectValues_1_1 { MEM_WIDTH 27 MEM_SIZE 128 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict inputObjectValues_1_2 { MEM_WIDTH 27 MEM_SIZE 128 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict inputObjectValues_1_3 { MEM_WIDTH 27 MEM_SIZE 128 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict inputObjectValues_0_0 { MEM_WIDTH 27 MEM_SIZE 128 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict inputObjectValues_0_1 { MEM_WIDTH 27 MEM_SIZE 128 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict inputObjectValues_0_2 { MEM_WIDTH 27 MEM_SIZE 128 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict inputObjectValues_0_3 { MEM_WIDTH 27 MEM_SIZE 128 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
set C_modelArgList {
	{ inputObjectValues_1_0 int 27 regular {array 32 { 1 3 } 1 1 }  }
	{ inputObjectValues_1_1 int 27 regular {array 32 { 1 3 } 1 1 }  }
	{ inputObjectValues_1_2 int 27 regular {array 32 { 1 3 } 1 1 }  }
	{ inputObjectValues_1_3 int 27 regular {array 32 { 1 3 } 1 1 }  }
	{ inputObjectValues_0_0 int 27 regular {array 32 { 2 3 } 1 1 }  }
	{ inputObjectValues_0_1 int 27 regular {array 32 { 2 3 } 1 1 }  }
	{ inputObjectValues_0_2 int 27 regular {array 32 { 2 3 } 1 1 }  }
	{ inputObjectValues_0_3 int 27 regular {array 32 { 2 3 } 1 1 }  }
	{ numSeed int 1 regular  }
	{ seedValues_0_val int 27 regular  }
	{ seedValues_1_val int 27 regular  }
	{ outputJetValues_0 int 27 regular {pointer 1}  }
	{ outputJetValues_1 int 27 regular {pointer 1}  }
}
set hasAXIMCache 0
set l_AXIML2Cache [list]
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "inputObjectValues_1_0", "interface" : "memory", "bitwidth" : 27, "direction" : "READONLY"} , 
 	{ "Name" : "inputObjectValues_1_1", "interface" : "memory", "bitwidth" : 27, "direction" : "READONLY"} , 
 	{ "Name" : "inputObjectValues_1_2", "interface" : "memory", "bitwidth" : 27, "direction" : "READONLY"} , 
 	{ "Name" : "inputObjectValues_1_3", "interface" : "memory", "bitwidth" : 27, "direction" : "READONLY"} , 
 	{ "Name" : "inputObjectValues_0_0", "interface" : "memory", "bitwidth" : 27, "direction" : "READWRITE"} , 
 	{ "Name" : "inputObjectValues_0_1", "interface" : "memory", "bitwidth" : 27, "direction" : "READWRITE"} , 
 	{ "Name" : "inputObjectValues_0_2", "interface" : "memory", "bitwidth" : 27, "direction" : "READWRITE"} , 
 	{ "Name" : "inputObjectValues_0_3", "interface" : "memory", "bitwidth" : 27, "direction" : "READWRITE"} , 
 	{ "Name" : "numSeed", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "seedValues_0_val", "interface" : "wire", "bitwidth" : 27, "direction" : "READONLY"} , 
 	{ "Name" : "seedValues_1_val", "interface" : "wire", "bitwidth" : 27, "direction" : "READONLY"} , 
 	{ "Name" : "outputJetValues_0", "interface" : "wire", "bitwidth" : 27, "direction" : "WRITEONLY"} , 
 	{ "Name" : "outputJetValues_1", "interface" : "wire", "bitwidth" : 27, "direction" : "WRITEONLY"} ]}
# RTL Port declarations: 
set portNum 45
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ inputObjectValues_1_0_address0 sc_out sc_lv 5 signal 0 } 
	{ inputObjectValues_1_0_ce0 sc_out sc_logic 1 signal 0 } 
	{ inputObjectValues_1_0_q0 sc_in sc_lv 27 signal 0 } 
	{ inputObjectValues_1_1_address0 sc_out sc_lv 5 signal 1 } 
	{ inputObjectValues_1_1_ce0 sc_out sc_logic 1 signal 1 } 
	{ inputObjectValues_1_1_q0 sc_in sc_lv 27 signal 1 } 
	{ inputObjectValues_1_2_address0 sc_out sc_lv 5 signal 2 } 
	{ inputObjectValues_1_2_ce0 sc_out sc_logic 1 signal 2 } 
	{ inputObjectValues_1_2_q0 sc_in sc_lv 27 signal 2 } 
	{ inputObjectValues_1_3_address0 sc_out sc_lv 5 signal 3 } 
	{ inputObjectValues_1_3_ce0 sc_out sc_logic 1 signal 3 } 
	{ inputObjectValues_1_3_q0 sc_in sc_lv 27 signal 3 } 
	{ inputObjectValues_0_0_address0 sc_out sc_lv 5 signal 4 } 
	{ inputObjectValues_0_0_ce0 sc_out sc_logic 1 signal 4 } 
	{ inputObjectValues_0_0_we0 sc_out sc_logic 1 signal 4 } 
	{ inputObjectValues_0_0_d0 sc_out sc_lv 27 signal 4 } 
	{ inputObjectValues_0_0_q0 sc_in sc_lv 27 signal 4 } 
	{ inputObjectValues_0_1_address0 sc_out sc_lv 5 signal 5 } 
	{ inputObjectValues_0_1_ce0 sc_out sc_logic 1 signal 5 } 
	{ inputObjectValues_0_1_we0 sc_out sc_logic 1 signal 5 } 
	{ inputObjectValues_0_1_d0 sc_out sc_lv 27 signal 5 } 
	{ inputObjectValues_0_1_q0 sc_in sc_lv 27 signal 5 } 
	{ inputObjectValues_0_2_address0 sc_out sc_lv 5 signal 6 } 
	{ inputObjectValues_0_2_ce0 sc_out sc_logic 1 signal 6 } 
	{ inputObjectValues_0_2_we0 sc_out sc_logic 1 signal 6 } 
	{ inputObjectValues_0_2_d0 sc_out sc_lv 27 signal 6 } 
	{ inputObjectValues_0_2_q0 sc_in sc_lv 27 signal 6 } 
	{ inputObjectValues_0_3_address0 sc_out sc_lv 5 signal 7 } 
	{ inputObjectValues_0_3_ce0 sc_out sc_logic 1 signal 7 } 
	{ inputObjectValues_0_3_we0 sc_out sc_logic 1 signal 7 } 
	{ inputObjectValues_0_3_d0 sc_out sc_lv 27 signal 7 } 
	{ inputObjectValues_0_3_q0 sc_in sc_lv 27 signal 7 } 
	{ numSeed sc_in sc_lv 1 signal 8 } 
	{ seedValues_0_val sc_in sc_lv 27 signal 9 } 
	{ seedValues_1_val sc_in sc_lv 27 signal 10 } 
	{ outputJetValues_0 sc_out sc_lv 27 signal 11 } 
	{ outputJetValues_0_ap_vld sc_out sc_logic 1 outvld 11 } 
	{ outputJetValues_1 sc_out sc_lv 27 signal 12 } 
	{ outputJetValues_1_ap_vld sc_out sc_logic 1 outvld 12 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "inputObjectValues_1_0_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "inputObjectValues_1_0", "role": "address0" }} , 
 	{ "name": "inputObjectValues_1_0_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inputObjectValues_1_0", "role": "ce0" }} , 
 	{ "name": "inputObjectValues_1_0_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "inputObjectValues_1_0", "role": "q0" }} , 
 	{ "name": "inputObjectValues_1_1_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "inputObjectValues_1_1", "role": "address0" }} , 
 	{ "name": "inputObjectValues_1_1_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inputObjectValues_1_1", "role": "ce0" }} , 
 	{ "name": "inputObjectValues_1_1_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "inputObjectValues_1_1", "role": "q0" }} , 
 	{ "name": "inputObjectValues_1_2_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "inputObjectValues_1_2", "role": "address0" }} , 
 	{ "name": "inputObjectValues_1_2_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inputObjectValues_1_2", "role": "ce0" }} , 
 	{ "name": "inputObjectValues_1_2_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "inputObjectValues_1_2", "role": "q0" }} , 
 	{ "name": "inputObjectValues_1_3_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "inputObjectValues_1_3", "role": "address0" }} , 
 	{ "name": "inputObjectValues_1_3_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inputObjectValues_1_3", "role": "ce0" }} , 
 	{ "name": "inputObjectValues_1_3_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "inputObjectValues_1_3", "role": "q0" }} , 
 	{ "name": "inputObjectValues_0_0_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "inputObjectValues_0_0", "role": "address0" }} , 
 	{ "name": "inputObjectValues_0_0_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inputObjectValues_0_0", "role": "ce0" }} , 
 	{ "name": "inputObjectValues_0_0_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inputObjectValues_0_0", "role": "we0" }} , 
 	{ "name": "inputObjectValues_0_0_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "inputObjectValues_0_0", "role": "d0" }} , 
 	{ "name": "inputObjectValues_0_0_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "inputObjectValues_0_0", "role": "q0" }} , 
 	{ "name": "inputObjectValues_0_1_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "inputObjectValues_0_1", "role": "address0" }} , 
 	{ "name": "inputObjectValues_0_1_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inputObjectValues_0_1", "role": "ce0" }} , 
 	{ "name": "inputObjectValues_0_1_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inputObjectValues_0_1", "role": "we0" }} , 
 	{ "name": "inputObjectValues_0_1_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "inputObjectValues_0_1", "role": "d0" }} , 
 	{ "name": "inputObjectValues_0_1_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "inputObjectValues_0_1", "role": "q0" }} , 
 	{ "name": "inputObjectValues_0_2_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "inputObjectValues_0_2", "role": "address0" }} , 
 	{ "name": "inputObjectValues_0_2_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inputObjectValues_0_2", "role": "ce0" }} , 
 	{ "name": "inputObjectValues_0_2_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inputObjectValues_0_2", "role": "we0" }} , 
 	{ "name": "inputObjectValues_0_2_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "inputObjectValues_0_2", "role": "d0" }} , 
 	{ "name": "inputObjectValues_0_2_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "inputObjectValues_0_2", "role": "q0" }} , 
 	{ "name": "inputObjectValues_0_3_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "inputObjectValues_0_3", "role": "address0" }} , 
 	{ "name": "inputObjectValues_0_3_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inputObjectValues_0_3", "role": "ce0" }} , 
 	{ "name": "inputObjectValues_0_3_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inputObjectValues_0_3", "role": "we0" }} , 
 	{ "name": "inputObjectValues_0_3_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "inputObjectValues_0_3", "role": "d0" }} , 
 	{ "name": "inputObjectValues_0_3_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "inputObjectValues_0_3", "role": "q0" }} , 
 	{ "name": "numSeed", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "numSeed", "role": "default" }} , 
 	{ "name": "seedValues_0_val", "direction": "in", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "seedValues_0_val", "role": "default" }} , 
 	{ "name": "seedValues_1_val", "direction": "in", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "seedValues_1_val", "role": "default" }} , 
 	{ "name": "outputJetValues_0", "direction": "out", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "outputJetValues_0", "role": "default" }} , 
 	{ "name": "outputJetValues_0_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "outputJetValues_0", "role": "ap_vld" }} , 
 	{ "name": "outputJetValues_1", "direction": "out", "datatype": "sc_lv", "bitwidth":27, "type": "signal", "bundle":{"name": "outputJetValues_1", "role": "default" }} , 
 	{ "name": "outputJetValues_1_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "outputJetValues_1", "role": "ap_vld" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2"],
		"CDFG" : "et_calculation_1",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "8834", "EstimateLatencyMax" : "8834",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "inputObjectValues_1_0", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "inputObjectValues_1_1", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "inputObjectValues_1_2", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "inputObjectValues_1_3", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "inputObjectValues_0_0", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "inputObjectValues_0_1", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "inputObjectValues_0_2", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "inputObjectValues_0_3", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "numSeed", "Type" : "None", "Direction" : "I"},
			{"Name" : "seedValues_0_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "seedValues_1_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "outputJetValues_0", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "outputJetValues_1", "Type" : "Vld", "Direction" : "O"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.bitmask_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_et_calculation_1_Pipeline_VITIS_LOOP_59_1_fu_4306", "Parent" : "0", "Child" : ["3"],
		"CDFG" : "et_calculation_1_Pipeline_VITIS_LOOP_59_1",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "8194", "EstimateLatencyMax" : "8194",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "bitmask", "Type" : "Memory", "Direction" : "O"}],
		"Loop" : [
			{"Name" : "VITIS_LOOP_59_1", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter1", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter1", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "3", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_et_calculation_1_Pipeline_VITIS_LOOP_59_1_fu_4306.flow_control_loop_pipe_sequential_init_U", "Parent" : "2"}]}


set ArgLastReadFirstWriteLatency {
	et_calculation_1 {
		inputObjectValues_1_0 {Type I LastRead 124 FirstWrite -1}
		inputObjectValues_1_1 {Type I LastRead 125 FirstWrite -1}
		inputObjectValues_1_2 {Type I LastRead 126 FirstWrite -1}
		inputObjectValues_1_3 {Type I LastRead 127 FirstWrite -1}
		inputObjectValues_0_0 {Type IO LastRead 624 FirstWrite 132}
		inputObjectValues_0_1 {Type IO LastRead 628 FirstWrite 136}
		inputObjectValues_0_2 {Type IO LastRead 632 FirstWrite 140}
		inputObjectValues_0_3 {Type IO LastRead 636 FirstWrite 144}
		numSeed {Type I LastRead 132 FirstWrite -1}
		seedValues_0_val {Type I LastRead 132 FirstWrite -1}
		seedValues_1_val {Type I LastRead 132 FirstWrite -1}
		outputJetValues_0 {Type O LastRead -1 FirstWrite 640}
		outputJetValues_1 {Type O LastRead -1 FirstWrite 640}}
	et_calculation_1_Pipeline_VITIS_LOOP_59_1 {
		bitmask {Type O LastRead -1 FirstWrite 1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "8834", "Max" : "8834"}
	, {"Name" : "Interval", "Min" : "8834", "Max" : "8834"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	inputObjectValues_1_0 { ap_memory {  { inputObjectValues_1_0_address0 mem_address 1 5 }  { inputObjectValues_1_0_ce0 mem_ce 1 1 }  { inputObjectValues_1_0_q0 mem_dout 0 27 } } }
	inputObjectValues_1_1 { ap_memory {  { inputObjectValues_1_1_address0 mem_address 1 5 }  { inputObjectValues_1_1_ce0 mem_ce 1 1 }  { inputObjectValues_1_1_q0 mem_dout 0 27 } } }
	inputObjectValues_1_2 { ap_memory {  { inputObjectValues_1_2_address0 mem_address 1 5 }  { inputObjectValues_1_2_ce0 mem_ce 1 1 }  { inputObjectValues_1_2_q0 mem_dout 0 27 } } }
	inputObjectValues_1_3 { ap_memory {  { inputObjectValues_1_3_address0 mem_address 1 5 }  { inputObjectValues_1_3_ce0 mem_ce 1 1 }  { inputObjectValues_1_3_q0 mem_dout 0 27 } } }
	inputObjectValues_0_0 { ap_memory {  { inputObjectValues_0_0_address0 mem_address 1 5 }  { inputObjectValues_0_0_ce0 mem_ce 1 1 }  { inputObjectValues_0_0_we0 mem_we 1 1 }  { inputObjectValues_0_0_d0 mem_din 1 27 }  { inputObjectValues_0_0_q0 mem_dout 0 27 } } }
	inputObjectValues_0_1 { ap_memory {  { inputObjectValues_0_1_address0 mem_address 1 5 }  { inputObjectValues_0_1_ce0 mem_ce 1 1 }  { inputObjectValues_0_1_we0 mem_we 1 1 }  { inputObjectValues_0_1_d0 mem_din 1 27 }  { inputObjectValues_0_1_q0 mem_dout 0 27 } } }
	inputObjectValues_0_2 { ap_memory {  { inputObjectValues_0_2_address0 mem_address 1 5 }  { inputObjectValues_0_2_ce0 mem_ce 1 1 }  { inputObjectValues_0_2_we0 mem_we 1 1 }  { inputObjectValues_0_2_d0 mem_din 1 27 }  { inputObjectValues_0_2_q0 mem_dout 0 27 } } }
	inputObjectValues_0_3 { ap_memory {  { inputObjectValues_0_3_address0 mem_address 1 5 }  { inputObjectValues_0_3_ce0 mem_ce 1 1 }  { inputObjectValues_0_3_we0 mem_we 1 1 }  { inputObjectValues_0_3_d0 mem_din 1 27 }  { inputObjectValues_0_3_q0 mem_dout 0 27 } } }
	numSeed { ap_none {  { numSeed in_data 0 1 } } }
	seedValues_0_val { ap_none {  { seedValues_0_val in_data 0 27 } } }
	seedValues_1_val { ap_none {  { seedValues_1_val in_data 0 27 } } }
	outputJetValues_0 { ap_vld {  { outputJetValues_0 out_data 1 27 }  { outputJetValues_0_ap_vld out_vld 1 1 } } }
	outputJetValues_1 { ap_vld {  { outputJetValues_1 out_data 1 27 }  { outputJetValues_1_ap_vld out_vld 1 1 } } }
}
