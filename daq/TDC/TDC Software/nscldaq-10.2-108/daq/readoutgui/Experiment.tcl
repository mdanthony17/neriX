
#    This software is Copyright by the Board of Trustees of Michigan
#    State University (c) Copyright 2005.
#
#    You may use this software under the terms of the GNU public license
#    (GPL).  The terms of this license are described at:
#
#     http://www.gnu.org/licenses/gpl.txt
#
#     Author:
#             Ron Fox
#	     NSCL
#	     Michigan State University
#	     East Lansing, MI 48824-1321
#


package provide Experiment 2.0
package require ExpFileSystem
package require ReadoutControl
package require InstallRoot
package require DAQParameters
package require Diagnostics
package require fileutil

#
#  Horrid kludge for etch systems that don't have fileutil::relative:
#
if {[info proc ::fileutil::relative] eq ""} {
    namespace eval ::fileutil {}; # Otherwise pkg_mkIndex fails.
    # ::fileutil::relative --
    #
    #	Taking two _directory_ paths, a base and a destination, computes the path
    #	of the destination relative to the base.
    #
    # Arguments:
    #	base	The path to make the destination relative to.
    #	dst	The destination path
    #
    # Results:
    #	The path of the destination, relative to the base.

    proc ::fileutil::relative {base dst} {
	# Ensure that the link to directory 'dst' is properly done relative to
	# the directory 'base'.

	if {![string equal [file pathtype $base] [file pathtype $dst]]} {
	    return -code error "Unable to compute relation for paths of different pathtypes: [file pathtype $base] vs. [file pathtype $dst]"
	}

	set save $dst
	set base [file split $base]
	set dst  [file split $dst]

	while {[string equal [lindex $dst 0] [lindex $base 0]]} {
	    set dst  [lrange $dst  1 end]
	    set base [lrange $base 1 end]
	    if {![llength $dst]} {break}
	}

	set dstlen [llength $dst]
	set baselen [llength $base]

	if {($dstlen == 0) && ($baselen == 0)} {
	    # Cases:
	    # (a) base == dst

	    set dst .
	} else {
	    # Cases:
	    # (b) base is: base/sub = sub
	    #     dst  is: base     = {}

	    # (c) base is: base     = {}
	    #     dst  is: base/sub = sub

	    while {$baselen > 0} {
		set dst [linsert $dst 0 ..]
		incr baselen -1
	    }
	    set dst [eval file join $dst]
	}

	return $dst
    }
}

namespace eval  Experiment {
    variable Logrecorder [DAQParameters::getEventLogger]
    puts "Log recorder path is $Logrecorder"
    variable EventlogPid 0
    variable fileWaitTimeout 45


    #  Define exports:

    namespace export Register
    namespace export BiggestRun
    namespace export EmergencyEnd
    namespace export CleanOrphans
    namespace export RunFileExists


}

#-------------- The procs below should be considered local to Experiment:: ---
#
#  Utility to make a filesystem link from source -> destination.
#  - link is made relative so that the link ports in the presence of backup
#    and restoration in a different filesystem (as happens in NSCL experiments
#    as part of finalizing the experiment
# Parameters:
#    destination  - The real file (target of the link)
#    source       - The link to be made.
proc Experiment::link {destination source} {
    #  fileutil::relative needs absolute paths:

    set absDest [file normalize $destination]
    set absLink [file normalize $source]

    puts "absDest: $absDest \nabsLink $absLink"

    set relTarget [::fileutil::relative \
		       [file dirname $absLink]  [file dirname $absDest]]

    puts "relTarget: $relTarget"
    #set name [file tail $destination]
    set name $destination
    puts "name: $name"


    # exec touch $absDest; # Force the existence of the target.
    puts "linking: $absLink to [file join $relTarget $name]"
    exec ln -s [file join $relTarget $name] $absLink
    # file link -symbolic $absLink [file join $relTarget $name]
}

# Experiment::runcompare
#    Compares two run files and determines which
#    is the smaller.
proc Experiment::runcompare {r1 r2} {
    set r1 [file tail $r1]
    set r2 [file tail $r2]

    scan $r1 run%d d1
    scan $r2 run%d d2
    return [expr {($d1 > $d2) ? 1 : -1}]
}
# Experiment::spectrodaqURL system
#    Given a system name returns the URL to contact
#    the spectrodaq on that system.
# Parameters:
#   system   - The ip address or dns of the system;.
proc Experiment::spectrodaqURL {system} {
    global tcl_platform
    global env
    #
    # URl is of the form:
    #  tcp://host/ringname
    #
    # The ringname can be gotten from env(RINGNAME) but
    # defaults to tcl_platform(user)
    #

    if {[array names env RINGNAME] ne ""} {
	set ring $env(RINGNAME)
    } else {
	set ring $tcl_platform(user)
    }

    set url "tcp://$system/$ring"

    return $url
}
# Experiment::waitFile name ?granularity ?timeout??
#    Wait for a file to exist.
# Parameters:
#   name        - Sufficient path to the file.
#   granularity - Polling period (1 sec by default).
#   timeout     - Limit on passes (infinite by default).
# Returns:
#    1     - File now exists.
#    0     - Timeout.
# NOTE:
#    If tk is loaded, then we will run a few update idle
#    commands each pass through the loop.
#
proc Experiment::waitFile {name {granularity 1000} {timeout 0}} {
    set passes 0
    set done   0

    while {1} {
        if {[file exists $name]} {
            return 1
        }
        incr passes
        if {($passes > $timeout) && ($timeout != 0)} {
            return 0
        }
        after $granularity
        if {[info command tk] != ""} {
            update idle
            update idle
            update idle
            update idle
        }
    }
}
# Experiment::makeEventLink run
#    Make a link to the event file in the current directory of the
#    metadata tree.
# Parameters:
#  run   - Number of the run
# NOTE:
#   Only segment 0 is linked.
#
proc Experiment::makeEventLink run {
    after 1000;                # For debugging let eventlog start
    set linkpath [ExpFileSystem::WhereisCurrentData]
    set filename [ExpFileSystem::GenRunFile $run]
    set filepath [file join [ExpFileSystem::GetStage] current]
    set linkname [file join $linkpath $filename]
    if {[file exists $linkname] || ([catch {file readlink $linkname}] == 0)} {
        file delete -force $linkname
    }
    set targetAbsPath [file join $filepath $filename]
    puts "makeEventLink: $targetAbsPath $linkname"

    ::Experiment::link $targetAbsPath $linkname


}
# Experiment::callback cbproc arg...
#    If cbproc is defined as a procedure it is called
#    with the args as a parameter.
#
proc Experiment::callback {cbproc args} {
    if {[info proc ::$cbproc] != ""} {
        eval ::$cbproc $args
    }
}

# Experiment::BiggestRun
#   Returns the next free run number.  The next free run
#   is determined by discovering the highest run number for
#   which there is a data file and incrementing that.
#   This is no longer done by relying on a .lastrun file.
#   instead we just look at the experiment directory, and figure
#   out which run is largest from the names of the directories present.
#   If no runs have been taken yet, 0 is returned.
proc Experiment::BiggestRun {} {
    set runDirectory [ExpFileSystem::WhereisMetadata]
    set runs [glob -nocomplain [file join $runDirectory run*]]
    if {[llength $runs] == 0} {
        return 0
    } else {
        set runs [lsort -decreasing -command Experiment::runcompare $runs]
        set biggest [file tail [lindex $runs 0]]
        scan $biggest run%d run

        incr run
        return $run
    }
}
# Experiment::finalizeEventData run
#     Does all of the end of run activities on the event data and
#     event metadata.  This involves:
#    - Removing the experiment/current link to the event data.
#    - Moving the entire contents of the experiment/current dir
#      into a run dir.
#    - Moving the event data into the run dir.
#    - Creating the compatibility link in stagearea/complete to the
#      event data in the run dir.
# Parameters:
#   run   - The number of the run to finalize.
#
proc Experiment::finalizeEventData run {
    set current   [ExpFileSystem::WhereisCurrentData]
    set stage     [ExpFileSystem::GetStage]
    set targetdir [file dirname [ExpFileSystem::WhereisRunFile $run]]
    set eventdir  [ExpFileSystem::WhereisCurrentEventData]
    set complete  [ExpFileSystem::WhereareCompleteEventFiles]

    # First remove all links in the current dir to the event data.
    # These are assumed to constitute a contiguous set of event segments.

    set segment 0
    set name [file join $current [ExpFileSystem::GenRunFile $run $segment]]
    puts "Looking at $name"
    while {[file exists $name]} {
        file delete -force $name
        incr segment
        set name [file join $current [ExpFileSystem::GenRunFile $run $segment]]
    }
    # Now we can create the target dir and tar the contents of experiment current to it.

    puts "Creating $targetdir"
    file mkdir $targetdir
    catch {
        exec sh << \
       "(cd $current; tar chf - .) | (cd $targetdir; tar xpf -)"
    }
    # Move the event segments from stagearea/current -> experiment/runn
    # and make a compatibility link in stagarea/complete

    # Ensure the targetdir is writable...
   
    file attributes $targetdir -permissions 0750;   # rwxr-x---
    file attributes $complete  -permissions 0750;   # Allow me to write the link.

    set segment 0
    while {1} {
	puts "Segment $segment"
        set file [ExpFileSystem::GenRunFile $run $segment]
        set srcfile [file join $eventdir  $file]
	puts "Ready to copy $srcfile"
        if {![file exists $srcfile]} {
            break
        }
        set dstfile [file join $targetdir $file]
	puts "..to $dstfile"
        set linkfile [file join $complete $file]
	puts "..with a link $linkfile"
        file rename -force $srcfile $dstfile
	file attributes $dstfile -permissions 0440;   # Make the saved file read-only
	puts "FinalizeRun $dstfile $linkfile"
	file delete -force $linkfile
	::Experiment::link $dstfile $linkfile

        incr segment

    }
    # Remove writability from the target dir so Dirk can't screw up.

    file attributes $targetdir -permissions 0550;   # r-xr-x---
    file attributes $complete  -permissions 0550;   # and the complete directory too.
}

#------------ The procs below should be considered public -------------

# Experiment::RunBeginning
#
# Run is about to begin.
# If taping was enabled the following:
#    1. Start the taper in single shot, ftp mode.
#    2. Make a symbolic link in the current directory to the event log
#       file.
# If the proc OnBegin is defined invoke it.
#
proc Experiment::RunBeginning {} {
    variable Logrecorder
    variable EventlogPid
    variable fileWaitTimeout


    set ::Diagnostics::isTk 1;   #Ugly but works... forces tk dialogs from warning
    set      nrun [ReadoutControl::GetRun]
    if {[ReadoutControl::isTapeOn]} {
	set Logrecorder [DAQParameters::getEventLogger]
	#
	# Start the event logger.
	#
	set Stagedir     [ExpFileSystem::WhereisCurrentEventData]
	set currentdata  [ExpFileSystem::WhereisCurrentData]
	set user         $::tcl_platform(user)
	set sourceHost   [DAQParameters::getSourceHost]
	set SourceURL    [Experiment::spectrodaqURL $sourceHost]


	set Pid [exec $Logrecorder --oneshot --path=$Stagedir --source=$SourceURL &]
	
	
	Experiment::makeEventLink $nrun

	set startFile [file join $Stagedir .started]
	
	Experiment::waitFile $startFile 1000 $fileWaitTimeout
	if {![file exists $startFile]} {
	    error "The event logger is not yet ready after a very long time"
	}
	file delete -force $startFile
    } else {
	set EventlogPid 0;		# So emergency end won't make abnormal end file.
    }
    Experiment::callback OnBegin $nrun
}
    # Experiment::RunEnded
    #    Run has ended.
    # If taping is on:
    #  1. Wait for the taper to end (.done appears in this dir).
    #  2. Delete the .done file.
    #  3. unlink the current link to the event file.
    #  4. Move the event data to the complete dir.
    #  5. Copy current to the appropriate run directory (following links)
    #  6. Create a new link to the event data in the run directory.
    # If OnEnd is defined, call it.
    #
proc Experiment::RunEnded {} {
    variable EventlogPid
    variable fileWaitTimeout

    set nrun [ReadoutControl::GetRun]
    # IF OnEnd is defined, call it:
    #
    Experiment::callback OnEnd $nrun

    if {[ReadoutControl::isTapeOn]} {
        #
        # Wait for eventlog to finish.

	set donefile [file join [ExpFileSystem::WhereisCurrentEventData] .exited]

        Experiment::waitFile $donefile 1000 $fileWaitTimeout
        if {![file exists $donefile]} {
            Diagnostics::Warning "eventlog may not have finished normally continuing with post run actions"
        }
        #  TODO:   Perhaps we should force event log to end if .done is not present yet?

        file delete -force  $donefile
	set EventlogPid     0


        Experiment::finalizeEventData $nrun


    }
}
# Experiment::RunPaused
#  If the OnPause proc is defined, call it.
#
proc Experiment::RunPaused {} {

    set nrun [ReadoutControl::GetRun]
    Experiment::callback OnPause $nrun
}
# Experiment::RunResuming
#  If the onResume proc is defined, call it.
#
proc Experiment::RunResuming {} {
    set nrun [ReadoutControl::GetRun]
    Experiment::callback OnResume $nrun
}
# Experiment::ReadoutStarting
#
#    If the OnStart proc is defined call it.
#
proc Experiment::ReadoutStarting {} {
    Experiment::callback OnStart
}
    #
    #  Perform an emergency end:  If the enentlog program is
    #  running it is killed, and the .done file created.
    #  Next the RunEnded process is called.
    #
proc Experiment::EmergencyEnd {} {
    variable EventlogPid
    if {$EventlogPid != 0} {
        catch {exec kill -KILL $EventlogPid}
        exec touch .done
    }
    ::Experiment::RunEnded
    set nrun [ReadoutControl::GetRun]
    set rundir [file dirname [ExpFileSystem::WhereisRunFile $nrun]]
    file mkdir  $rundir
    file attributes $rundir -permissions 0750
    exec touch $rundir/000RunAbnormallyEnded
    file attributes $rundir/000RunAbnormallyEnded -permissions 0440
    file attributes $rundir -permissions 0550
}
#Experiment::CleanOrphans
#  Clean up orphaned event files in the stage area's current
#  and, manage any dangling links in the experiment current dir.
#  If there is a corresponding run directory, complete the link.
#  If not, make a link in the orphaned dir.
#
proc Experiment::CleanOrphans {} {
    set Eventdir [ExpFileSystem::WhereisCurrentEventData]
    set root     [ExpFileSystem::GetStage]
    file mkdir $root/orphans


    # If there are event files in the stagearea,
    # we finalize their runs.
    # and put a file named 000orphaned in their metadata
    # directory to let people know where these  came from.

    while {1} {
        set orphans [glob -nocomplain [file join $Eventdir run*.evt]]

        if {[llength $orphans] == 0} {
            break
        }
        scan [file tail [lindex $orphans 0]] run-%d run
        finalizeEventData $run

        set target [ExpFileSystem::WhereisRun $run]
	file attributes $target -permissions 0750

        set fd [open [file join $target 000orphaned] w]
        close $fd
	file attributes $target -permissions 0550
    }
    #  If current has dangling event data links, they will be destroyed.

    set evts [ExpFileSystem::WhereisCurrentEventData]
    foreach filename [glob -nocomplain [file join $evts run*.evt]] {
        file delete $filename
    }

}
#
#  Register callbacks with runcontrol stuff:
#
proc Experiment::Register {} {
    ReadoutControl::SetOnBegin  ::Experiment::RunBeginning
    ReadoutControl::SetOnEnd    ::Experiment::RunEnded
    ReadoutControl::SetOnPause  ::Experiment::RunPaused
    ReadoutControl::SetOnResume ::Experiment::RunResuming
    ReadoutControl::SetOnLoad   ::Experiment::ReadoutStarting
}

#   Check to see if an event file already exists for a
#   specific run.
# Parameters:
#     run   - Number of the run to check.
# Returns:
#     1     - the file exists.
#     0     - the file does not exist.
#
proc Experiment::RunFileExists {run} {
    set name [ExpFileSystem::WhereisRunFile $run]
    return [file exists $name]

}

