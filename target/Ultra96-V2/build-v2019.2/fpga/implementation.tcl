#
# implementation.tcl  Tcl script for implementation
#
#
# Open Project
#
set project_directory       [file dirname [info script]]
set project_name            "project"
#
#
#
if {[info exists project_name     ] == 0} {
    set project_name        "project"
}
if {[info exists project_directory] == 0} {
    set project_directory   [pwd]
}
open_project [file join $project_directory $project_name]
#
# Run Synthesis
#
launch_runs synth_1 -job 4
wait_on_run synth_1
#
# Run Implementation
#
launch_runs impl_1  -job 4
wait_on_run impl_1
open_run    impl_1
report_utilization -file [file join $project_directory "project.rpt" ]
report_timing      -file [file join $project_directory "project.rpt" ] -append
#
# Write Bitstream File
#
launch_runs impl_1 -to_step write_bitstream -job 4
wait_on_run impl_1
#
# Close Project
#
close_project
