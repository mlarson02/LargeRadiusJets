# Tcl file handling all the looping for easier data collection

open_project -reset filtering_tests
set_top top
add_files filtering.cc

for {set i 0} {$i < 1} {incr i} {
    set sol_name "loop1_base"
    
    open_solution $sol_name
    # set_directive_unroll -skip_exit_check -factor [expr int(pow(2, $i))] top/loop1
    set_part {xcvp1802-lsvc4072-2MP-e-S}
    create_clock -period 4 -name default

    csynth_design
}

exit