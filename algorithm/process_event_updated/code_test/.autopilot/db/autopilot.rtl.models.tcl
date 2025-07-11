set SynModuleInfo {
  {SRCNAME odd_even_sort MODELNAME odd_even_sort RTLNAME top_odd_even_sort}
  {SRCNAME et_calculation.1_Pipeline_VITIS_LOOP_59_1 MODELNAME et_calculation_1_Pipeline_VITIS_LOOP_59_1 RTLNAME top_et_calculation_1_Pipeline_VITIS_LOOP_59_1
    SUBMODULES {
      {MODELNAME top_flow_control_loop_pipe_sequential_init RTLNAME top_flow_control_loop_pipe_sequential_init BINDTYPE interface TYPE internal_upc_flow_control INSTNAME top_flow_control_loop_pipe_sequential_init_U}
    }
  }
  {SRCNAME et_calculation.1 MODELNAME et_calculation_1 RTLNAME top_et_calculation_1
    SUBMODULES {
      {MODELNAME top_et_calculation_1_bitmask_RAM_AUTO_1R1W RTLNAME top_et_calculation_1_bitmask_RAM_AUTO_1R1W BINDTYPE storage TYPE ram IMPL auto LATENCY 2 ALLOW_PRAGMA 1}
    }
  }
  {SRCNAME top MODELNAME top RTLNAME top IS_TOP 1}
}
