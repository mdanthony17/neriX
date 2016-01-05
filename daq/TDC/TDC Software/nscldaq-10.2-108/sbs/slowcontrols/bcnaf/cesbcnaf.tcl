#!/bin/sh
# Start Wish. \
exec tclsh ${0} ${@}


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




# (C) Copyright Michigan State University 1938, All rights reserved 


# usage: bcnaf.tcl b c n a f [d]


#  Set up the auto_path to include our install directories.
#  We assume we're one down from the top.

set here [file dirname [info script]]
set libdir [file join $here .. TclLibs]
#
# Canonicalize $here/..
#
set wd [pwd]
cd $libdir
set libdir [pwd]
cd $wd
#   
#  Prepend to auto_path only if it's not already 
#  there.
#
if {[lsearch -exact $auto_path $libdir] == -1} {
    set auto_path [concat $libdir $auto_path]
}

package require camac

proc usage {} {
    puts stderr " usage:\n\t  bcnaf.tcl b c n a f \[d\]"
}


set nparams [llength $argv]

if {$nparams < 5} {
  usage
  exit -1
}

set b [lindex $argv 0]
set c [lindex $argv 1]
set n [lindex $argv 2]
set a [lindex $argv 3]
set f [lindex $argv 4]
set d ""
if {($f > 15) && ($f < 24)} {
    if {$nparams != 6} {
	usage
	exit -1
    }
    set d [lindex $argv 5]
}

set module [camac::cdreg $b $c $n]
set output [camac::cfsa $module  $f $a $d]
set data [lindex $output 0]
set q    [lindex $output 1]
set x    [lindex $output 2]

set dataout [format "Data = %d(10), %x(16), %o(8)" $data $data $data]
puts "$dataout X=$x Q=$q"

