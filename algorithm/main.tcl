# Tcl file handling all the looping for easier data collection

open_project -reset process_event_updated
set_top top
add_files process_event_updated.cc 

for {set i 0} {$i < 1} {incr i} {
    set sol_name "code_test"
    
    open_solution $sol_name
    set_part {xcvp1802-lsvc4072-2MP-e-S}
    create_clock -period 4 -name default

    csynth_design
}

exit