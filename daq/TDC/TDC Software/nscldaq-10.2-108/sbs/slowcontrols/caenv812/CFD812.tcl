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

#
#   CFD812
#    Provides a support package for the CAEN V812 VME CFD.
#   This packages is layered on top of the Vme access package:
#

package require Vme
package provide CFD812 2.0

namespace eval CFD812 {

    namespace export Map SetThreshold SetWidth SetDeadtime 
    namespace export SetMultiplicity GetSerialNumber GetBase
}
#  Map a module.  
#    base - base address of the module in a32 space.
#    name - name of the module/map. 
proc ::CFD812::Map {base name {crate 0}} {
    # Make the map.
    
    vme create $name -device extended -crate $crate $base 0x100
    
    # if the module is not an 812 then unmap and error:
    
    set signature [format 0x%x [$name get -w 0xfa]]
    set type [format 0x%x [expr 0x3ff & [$name get -w 0xfc]]]
    if {($signature != 0xfaf5) && ($type != 0x51)} {
	vme delete $name
	error "No CAENV812 at $base in crate $crate" "::CFD812::Map"
    }
    return [list $base $name]
}
proc ::CFD812::Unmap {name} {
    set device [lindex $name 1]
    vme delete $device
}

#  Set a threshold
#    module  - The list returned from map.
#    channel - Number of the channel to set [0,15].
# 	value   - mv [-1,-255].
proc ::CFD812::SetThreshold {module channel value} {
    
    # do the error checking required.
    
    if {($channel < 0) || ($channel > 15)} {
	error "Channel $channel is invalid" ::CFD812::SetThreshold
    }
    if {($value > -1) || ($value < -255)} {
	error "Threshold $value is invalid" ::CFD812::SetThreshold
    }
    set device [lindex $module 1]
    set threshold [expr -$value]
    $device set -w [expr $channel * 2] $threshold
}	
# Set width for an 8 channel bank.  
# module  - the list returned from map.
# bank    - Selects the bank:
#            0 = Channels 0-7.
#            1 = Channels 8-15.
# value   - Value to set the register.
proc ::CFD812::SetWidth {module bank value} {
    if {($bank != 0) && ($bank != 1)} {
	error "Invalid bank select $bank" ::CFD812::SetWidth
    } else {
	set device [lindex $module 1]
	$device set -w [expr 0x40 + 2*$bank] $value
	
    }
}
#
#  Set the dead time register for an 8 channel bank:
#  module - the list returned from map.
#  bank   - Selects a bank:
#  		0 = Channels 0-7
#		1 = Channels 8-15
#  value  - Value to set the register.`
proc ::CFD812::SetDeadtime {module bank value} {
    if {($bank != 0) && ($bank != 1)} {
	error "Invalid bankd select $bank" ::CFD812::SetDeadtime
    } else  {
	set device [lindex $module 1]
	$device set -w [expr 0x44 + 2*$bank] $value
    }
}

proc ::CFD812::SetMask {module mval} {
    set device [lindex $module 1]
    $device set -w 0x4a $mval
}

#	Sets the multiplicity threshold.
#  module	- List returned from map.
#  value	- The multiplicity value [1,20]
#   The value programmed into the register is
#     int(($value*50.0 - 25.0)/4.0 + 0.5)
#
proc ::CFD812::SetMultiplicityThreshold {module value} {
    if {($value < 1) || ($value > 50) } {
	error "Multiplicity $value out of range" ::CFD812::SetMultiplicityThreshold
    } else {
	set device [lindex $module 1]
	set regvalue [expr int(($value*50.0 - 25.0)/4.0 + 0.5)]
	$device set -w 0x48 $regvalue
    }
}
#
#  Returns the module serial number.
#
#  Module  - List returned from map.
#
proc ::CFD812::GetSerialNumber {module} {
    set device [lindex $module 1]
    return [expr [$device get -w 0xfe] & 0xfff]
}
#  Returns the module base address
#  Module - List returned from map.
#
proc ::CFD812::GetBase {module} {
    return [lindex $module 0]
}