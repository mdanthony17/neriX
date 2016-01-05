# Meant to be run on Tclserver.
# 

puts  "---------------------------------"


puts  $argc
puts  $argv


#  We require DAQHOST to be an environment
#  variable that is the name of the spdaq system.
#

if {[array names env DAQHOST] == ""} {
	tk_dialog .failure "No DAQ HOST" \
	"The DAQHOST environment variable must be set to the spdaq system you're using" \
	error 0 Dismiss
	exit -1
}
set spdaq  $env(DAQHOST)


#  The user must have supplied a command line argument 
#  which is the name of their setup file.
#

if {[llength $argv] < 2} {
   tk_dialog .failure "No setup file" \
   "You must supply a command argument that is your scaler config file" \
   error 0 Dismiss
   exit -1
}
set file [lindex $argv 0]
puts "Setup file: $file"

#  And the file must be readable...
#
if {![file readable $file]} {
	tk_dialog .failure "Unreadable setup file" \
	"The setup file $file  is not readable by me and must be" \
	error 0 Dismiss
	exit -1
}


#  We need to establish our location and source in the
#  scaler GUI support. We assume that we are installed
#  1 level below 'prefix' and that the scaler display
#  GUI support packages are installed in the Scripts
#  dir of that directory.

set me [info script]
set mydirectory [file dirname $me]
set bindir     $mydirectory/../bin
set scriptdir  $mydirectory/../TclLibs/ScalerDisplay
puts "Running $scriptdir/scaler.tcl"
source $scriptdir/scaler.tcl
puts running

# Canonicalize the TclLibs directory subtree location and
# if needed, prepend to auto_path.


set libDir [file join  $mydirectory ..]
set wd [pwd]
cd $libDir
set libDir [pwd]
cd $wd

if {[lsearch -exact $auto_path $libDir] == -1} {
    set auto_path [concat $libDir $auto_path]
}


#
#   Now the setup file is sourced we can start the scalerclient
#   and set things up so that we will kill it on our exit:
#
set clientpid 0

proc Cleanup {widget pid} {
    if {$widget != "."} return
    exec kill $pid
}

proc StartClient {} {
    global bindir
    global TclServerPort
    global clientpid
    global spdaq
    global tcl_platform

    set ringname $tcl_platform(user)
    if {[array names ::env SCALER_RINGNAME] ne ""} {
	set ringname $::env(SCALER_RINGNAME)
    }
    set spdaqurl "tcp://$spdaq/$ringname";    # The ring url

    set clientpid [exec $bindir/sclclient --source=$spdaqurl --host=localhost --port=$TclServerPort &]
    bind . <Destroy> "Cleanup %W $clientpid"

}
after 2000 StartClient



#
#  Source the user's setup file:
#

if {[catch "source $file" msg]} {
   tk_dialog .failure "Setup file failure" \
   "There is an error in the setup file $argv : $msg" \
   error 0 Dismiss
   exit -1
}
