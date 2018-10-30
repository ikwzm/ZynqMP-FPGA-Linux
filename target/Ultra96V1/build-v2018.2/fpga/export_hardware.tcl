#
# export_hardware.tcl  Tcl script for export hardware
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
# Make SDK Workspace 
#
if {[info exists sdk_workspace] == 0} {
    set sdk_workspace       [file join $project_directory $project_name.sdk]
}
if { [file exists $sdk_workspace] == 0 } {
    file mkdir $sdk_workspace
}
#
# Copy Hardware File
#
set design_top_name [get_property "top" [current_fileset]]
file copy -force [file join $project_directory $project_name.runs "impl_1" $design_top_name.sysdef] [file join $sdk_workspace $design_top_name.hdf]
#
# Close Project
#
close_project
