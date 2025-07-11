# This script segment is generated automatically by AutoPilot

# clear list
if {${::AESL::PGuard_autoexp_gen}} {
    cg_default_interface_gen_dc_begin
    cg_default_interface_gen_bundle_begin
    AESL_LIB_XILADAPTER::native_axis_begin
}

set axilite_register_dict [dict create]
# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 20 \
    name inputObjectValues_0_0 \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename inputObjectValues_0_0 \
    op interface \
    ports { inputObjectValues_0_0_address0 { O 5 vector } inputObjectValues_0_0_ce0 { O 1 bit } inputObjectValues_0_0_we0 { O 1 bit } inputObjectValues_0_0_d0 { O 27 vector } inputObjectValues_0_0_q0 { I 27 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'inputObjectValues_0_0'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 21 \
    name inputObjectValues_0_1 \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename inputObjectValues_0_1 \
    op interface \
    ports { inputObjectValues_0_1_address0 { O 5 vector } inputObjectValues_0_1_ce0 { O 1 bit } inputObjectValues_0_1_we0 { O 1 bit } inputObjectValues_0_1_d0 { O 27 vector } inputObjectValues_0_1_q0 { I 27 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'inputObjectValues_0_1'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 22 \
    name inputObjectValues_0_2 \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename inputObjectValues_0_2 \
    op interface \
    ports { inputObjectValues_0_2_address0 { O 5 vector } inputObjectValues_0_2_ce0 { O 1 bit } inputObjectValues_0_2_we0 { O 1 bit } inputObjectValues_0_2_d0 { O 27 vector } inputObjectValues_0_2_q0 { I 27 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'inputObjectValues_0_2'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 23 \
    name inputObjectValues_0_3 \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename inputObjectValues_0_3 \
    op interface \
    ports { inputObjectValues_0_3_address0 { O 5 vector } inputObjectValues_0_3_ce0 { O 1 bit } inputObjectValues_0_3_we0 { O 1 bit } inputObjectValues_0_3_d0 { O 27 vector } inputObjectValues_0_3_q0 { I 27 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'inputObjectValues_0_3'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 24 \
    name inputObjectValues_1_0 \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename inputObjectValues_1_0 \
    op interface \
    ports { inputObjectValues_1_0_address0 { O 5 vector } inputObjectValues_1_0_ce0 { O 1 bit } inputObjectValues_1_0_we0 { O 1 bit } inputObjectValues_1_0_d0 { O 27 vector } inputObjectValues_1_0_q0 { I 27 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'inputObjectValues_1_0'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 25 \
    name inputObjectValues_1_1 \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename inputObjectValues_1_1 \
    op interface \
    ports { inputObjectValues_1_1_address0 { O 5 vector } inputObjectValues_1_1_ce0 { O 1 bit } inputObjectValues_1_1_we0 { O 1 bit } inputObjectValues_1_1_d0 { O 27 vector } inputObjectValues_1_1_q0 { I 27 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'inputObjectValues_1_1'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 26 \
    name inputObjectValues_1_2 \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename inputObjectValues_1_2 \
    op interface \
    ports { inputObjectValues_1_2_address0 { O 5 vector } inputObjectValues_1_2_ce0 { O 1 bit } inputObjectValues_1_2_we0 { O 1 bit } inputObjectValues_1_2_d0 { O 27 vector } inputObjectValues_1_2_q0 { I 27 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'inputObjectValues_1_2'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 27 \
    name inputObjectValues_1_3 \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename inputObjectValues_1_3 \
    op interface \
    ports { inputObjectValues_1_3_address0 { O 5 vector } inputObjectValues_1_3_ce0 { O 1 bit } inputObjectValues_1_3_we0 { O 1 bit } inputObjectValues_1_3_d0 { O 27 vector } inputObjectValues_1_3_q0 { I 27 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'inputObjectValues_1_3'"
}
}


# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 18 \
    name seedValues_0 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_seedValues_0 \
    op interface \
    ports { seedValues_0 { I 27 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 19 \
    name seedValues_1 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_seedValues_1 \
    op interface \
    ports { seedValues_1 { I 27 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 28 \
    name outputJetValues_0 \
    type other \
    dir O \
    reset_level 1 \
    sync_rst true \
    corename dc_outputJetValues_0 \
    op interface \
    ports { outputJetValues_0 { O 27 vector } outputJetValues_0_ap_vld { O 1 bit } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 29 \
    name outputJetValues_1 \
    type other \
    dir O \
    reset_level 1 \
    sync_rst true \
    corename dc_outputJetValues_1 \
    op interface \
    ports { outputJetValues_1 { O 27 vector } outputJetValues_1_ap_vld { O 1 bit } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id -1 \
    name ap_ctrl \
    type ap_ctrl \
    reset_level 1 \
    sync_rst true \
    corename ap_ctrl \
    op interface \
    ports { ap_start { I 1 bit } ap_ready { O 1 bit } ap_done { O 1 bit } ap_idle { O 1 bit } } \
} "
}


# Adapter definition:
set PortName ap_clk
set DataWd 1 
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc cg_default_interface_gen_clock] == "cg_default_interface_gen_clock"} {
eval "cg_default_interface_gen_clock { \
    id -2 \
    name ${PortName} \
    reset_level 1 \
    sync_rst true \
    corename apif_ap_clk \
    data_wd ${DataWd} \
    op interface \
}"
} else {
puts "@W \[IMPL-113\] Cannot find bus interface model in the library. Ignored generation of bus interface for '${PortName}'"
}
}


# Adapter definition:
set PortName ap_rst
set DataWd 1 
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc cg_default_interface_gen_reset] == "cg_default_interface_gen_reset"} {
eval "cg_default_interface_gen_reset { \
    id -3 \
    name ${PortName} \
    reset_level 1 \
    sync_rst true \
    corename apif_ap_rst \
    data_wd ${DataWd} \
    op interface \
}"
} else {
puts "@W \[IMPL-114\] Cannot find bus interface model in the library. Ignored generation of bus interface for '${PortName}'"
}
}



# merge
if {${::AESL::PGuard_autoexp_gen}} {
    cg_default_interface_gen_dc_end
    cg_default_interface_gen_bundle_end
    AESL_LIB_XILADAPTER::native_axis_end
}


