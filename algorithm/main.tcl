# Tcl file handling all the looping for easier data collection

open_project -reset process_event
set_top process_event
add_files source/process_event.cc

for {set i 0} {$i < 8} {incr i} {
    set sol_name "cycle16unrollpow$i"
    
    open_solution $sol_name
    set_part {xcvp1802-lsvc4072-2MP-e-S}
    create_clock -period 4 -name default
    
    set_directive_unroll -skip_exit_check -factor [expr int(pow(2, $i))] process_event/loop2
    set_directive_array_partition -type cyclic -factor 16 process_event inputObjectValues
    
    # Run synthesis
    csynth_design
}

exit