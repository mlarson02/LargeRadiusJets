set SynModuleInfo {
  {SRCNAME top_Pipeline_loop1 MODELNAME top_Pipeline_loop1 RTLNAME top_top_Pipeline_loop1
    SUBMODULES {
      {MODELNAME top_sparsemux_257_7_13_1_1 RTLNAME top_sparsemux_257_7_13_1_1 BINDTYPE op TYPE sparsemux IMPL compactencoding_dontcare}
      {MODELNAME top_flow_control_loop_pipe_sequential_init RTLNAME top_flow_control_loop_pipe_sequential_init BINDTYPE interface TYPE internal_upc_flow_control INSTNAME top_flow_control_loop_pipe_sequential_init_U}
    }
  }
  {SRCNAME top_Pipeline_loop2 MODELNAME top_Pipeline_loop2 RTLNAME top_top_Pipeline_loop2
    SUBMODULES {
      {MODELNAME top_bitselect_1ns_4096ns_32ns_1_1_1 RTLNAME top_bitselect_1ns_4096ns_32ns_1_1_1 BINDTYPE op TYPE bitselect IMPL auto}
    }
  }
  {SRCNAME top MODELNAME top RTLNAME top IS_TOP 1}
}
