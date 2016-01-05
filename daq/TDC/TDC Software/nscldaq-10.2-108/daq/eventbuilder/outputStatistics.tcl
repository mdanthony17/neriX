#    This software is Copyright by the Board of Trustees of Michigan
#    State University (c) Copyright 2005.
#
#    You may use this software under the terms of the GNU public license
#    (GPL).  The terms of this license are described at:
#
#     http://www.gnu.org/licenses/gpl.txt
#
#    Author:
#             Ron Fox
#	     NSCL
#	     Michigan State University
#	     East Lansing, MI 48824-1321


##
# @file outputStatistics.tcl
# @author Ron Fox
# @brief Event builder output statistics.
#

# If this location is not in the path, add it.

set here [file dirname [info script]]
if {[lsearch -exact $auto_path $here] == -1} {
    lappend auto_path $here
}


# This file provides the widgets needed to create a set of output statistics.
# the ouptut statistics is a single widget named:
#  
#  EVB::outputStatistics defined in this file below.
#

package require Tk
package require snit
package require EVBUtilities

package provide EVB::outputStatistics 1.0

##
#  Ensure that the EVB namespace is defined so that we can put stuff into it.

namespace eval EVB {
}

#-----------------------------------------------------------------------------------

##
# @class EVB::outputSummary
#
#  Widget that summarizes the output statistics of the event orderer.
#
# OPTIONS:
#  - -fragments        - Number of fragments that have been output
#  - -hottestid        - Source id with the most fragments output.
#  - -hottestcount     - Number of fragments output by -hottestid.
#  - -coldestid        - Source id with the fewest output fragments.
#  - -coldestcount     - Number of fragments output by -coldestid.
#
#  LAYOUT:
#   Rough layout.
# \verbatim
#
#   +-----------------------------------------+
#   |Total Fragments          <fragcount>     |
#   |           id            count           |
#   | hottest   <id>          <fragcount>     |
#   | coldest   <id>          <fragcount>     |
#   +-----------------------------------------+
# \endverbatim
#
snit::widgetadaptor ::EVB::outputSummary {
    option -fragments    -default 0
    option -hottestid    -default ""
    option -hottestcount -default ""
    option -coldestid    -default ""
    option -coldestcount -default ""
    
    delegate option -text to hull
    
    constructor args {
        installhull using ttk::labelframe
        
        #  Create the widgets (all ttk::label s)
        
        ttk::label $win.fragl   -text {Total fragments}
        ttk::label $win.idl     -text {source id}
        ttk::label $win.countl  -text {fragment count}
        ttk::label $win.hotl    -text {Hottest}
        ttk::label $win.coldl   -text {Coldest}
        
        ttk::label $win.frag           -textvariable ${selfns}::options(-fragments)
        ttk::label $win.hotid          -textvariable ${selfns}::options(-hottestid)
        ttk::label $win.hotcount       -textvariable ${selfns}::options(-hottestcount)
        ttk::label $win.coldid         -textvariable ${selfns}::options(-coldestid)
        ttk::label $win.coldcount      -textvariable ${selfns}::options(-coldestcount)
        
        # Lay them out
        
        grid $win.fragl   -           $win.frag
        grid x            $win.idl    $win.countl
        grid $win.hotl    $win.hotid  $win.hotcount  -sticky e
        grid $win.coldl   $win.coldid $win.coldcount -sticky e
        
        # Process the initial configuration.
        
        $self configurelist $args
    }
}

##
# @class EVB::outputStatistics
#
#   This snit::widget provides a display of the output statistics for the event
#   orderer.   This display is divorced from the mechanisms of getting the statistics.
#   the only thing we do is provide a widget that presents the statistics.
#
# OPTIONS:
#   - -totalfragments - Total number of output fragments emitted.
#
# METHODS:
#   - addSource           - adds an event source to the widget.
#   - removeSource        - removes an event source from the widget.
#   - setSourceFragments  - Provides the number of fragments a source has emitted.
#   - clear               - Clears the fragment counters on all sources.
#   - listSources         - Returns the list of sources.
#
# LAYOUT:
#
# \verbatim
#   +-----------------------------------+
#   | Total Fragments <frag-count>      |
#   +-----------------------------------+
#   | Source            Fragments       |
#   |  id1              count1          |
#   \                                   \
#   /                                   /
#  +-----------------------------------+
# \endveratim
snit::widget ::EVB::outputStatistics {
    component innerhull
    component sourcestats
    
    option -totalfragments -default 0 -configuremethod configTotals
    
    delegate method addSource to sourcestats as setItem
    delegate method clear     to sourcestats
    delegate method reinit    to sourcestats
    delegate method listSources to sourcestats as idlist

    delegate option * to innerhull
    delegate method  * to innerhull

    variable sourceIds [list];		# ordered list of ids.

    ##
    # constructor
    #    creates the initial widgets 

    constructor args {

	#  Create the widgets.

	install innerhull using ttk::frame $win.hull; # ttk frame for a backdrop.
	
	set top [ttk::frame $innerhull.top -relief groove -borderwidth 3]
	ttk::label $top.totall -text {Total Fragments}
	ttk::label $top.total  -textvariable ${selfns}::options(-totalfragments)
        
        install sourcestats using EVB::utility::sortedPair $innerhull.bottom \
            -title {Source stats} -lefttitle {src id} -righttitle Fragments



	#  Layout the widgets.

	grid $top.totall $top.total
	grid $top -sticky new

	grid $sourcestats -sticky nswe

	grid  $innerhull -sticky nsew

	# Process the options
	
	$self configurelist $args
    }


}

#----------------------------------------------------------------------------
#
# internal self test methods.

namespace eval EVB {
    namespace eval test {}
}
##
# test the outputSummary widget:
#
proc ::EVB::test::outputSummary {} {
    ::EVB::outputSummary .target
    pack .target
    
    # now the control panel:
    
    toplevel .control
    
    label .control.fragl -text Fragments
    entry .control.frag  -textvariable -fragments
    
    label .control.hotidl -text {Hot id}
    entry .control.hotid  -textvariable -hottestid
    
    label .control.hotcountl -text {Hot count}
    entry .control.hotcount  -textvariable -hottestcount
    
    label .control.coldidl  -text {Cold id}
    entry .control.coldid   -textvariable -coldestid
    
    label .control.coldcountl -text {Cold Count}
    entry .control.coldcount -textvariable -coldestcount
    
    grid .control.fragl .control.frag
    grid .control.hotidl .control.hotid
    
    grid .control.hotcountl .control.hotcount
    
    grid .control.coldidl .control.coldid
    grid .control.coldcountl .control.coldcount

    # Add variable traces to propagate values -> the .target.
    
    foreach var [list -fragments -hottestid -hottestcount -coldestid -coldestcount] {
        trace add variable ::$var write [list ::EVB::test::updateWidgetOption .target]
    }
}
##
# Testing utility to update a widget option from a trace on a variable
# that has a name matching the option:
#
# @param widget - the widget to modify.
# @param name1  - name of the variable (and option)
# @param name2  - index of the variable if an array.
# @param op     - operation performed.
#

if {[info proc ::EVB::test::updateWidgetOption] eq ""} {
    proc ::EVB::test::updateWidgetOption {widget name1 name2 op} {
        upvar #0 $name1 value
        $widget configure $name1 $value
        return ""
    }
}