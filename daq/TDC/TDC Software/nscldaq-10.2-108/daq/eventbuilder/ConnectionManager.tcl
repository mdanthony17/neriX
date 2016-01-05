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


#--------------------------------------------------------------------------------
#
#  Event builder connection manager:
# 
#  This must be run in an event loop such as in a vwait or in 
#  tk
#
#  Two snit objects are defined here:
# 
#   Connection - The state machine that makes up an actual connection.
#                What a connection does with input depends on
#                the state a connection is in.
#                Connections are created in the 'FORMING' state and have the following 
#                additional states:
#                - 'ACTIVE'  - The connection is in a state where it can handle data'
#                              from a data source
#                - 'CLOSED'  - The connection closed normally (as per protocol).
#                - 'LOST'    - The connection was lost without normal close protocol.
#                - 'ERROR'   - The connection saw a protocol error and was dropped from
#                              our end.
#
#  ConnectionManager - Handles TCP/IP level connection requests creating connections
#                      as needed and killing them as well.
#

package require snit
package require EVB::CallbackManager
package require EvbOrderer;	# C/C++ orderer.
package require Observer;	# Observer pattern component

package provide EVB::ConnectionManager 1.0

namespace eval EVB {}

##
# Connection object
#
# OPTIONS
#   -state - Current socket state (see above).  This is readonly.
#   -clientaddr        - Client TCP/IP address (this is readonly).
#   -description       - Client description as provided in the the connection negotiation.
#   -socket            - Socket connected to client.
#   -disconnectcommand - Script  to call on disconnect.
#   -fragmentcommand   - Script to call on a fragment
#
# METHODS
#   Only construction and destrution are are public.
#
snit::type EVB::Connection {
    option -state -default FORMING -readonly yes
    option -description -default "" -readonly yes

    option -socket;		#  The socket connected to client.
    option -clientaddr;#  -default "not-set"
    option -disconnectcommand -default [list] -configuremethod _SetCallback 
    option -fragmentcommand   -default [list] -configuremethod _SetCallback


    variable callbacks
    variable expecting



    constructor args {
	$self configurelist $args
	fconfigure $options(-socket) -blocking 1 -buffering none -translation {binary lf} \
	    -encoding binary
	set callbacks [EVB::CallbackManager %AUTO%]
	flush stdout
	$callbacks define -disconnectcommand
	$callbacks define -fragmentcommand

	$self _Expecting _Connect FORMING; # We are now expecting a CONNECT command.

    }
    destructor {
	if {$options(-socket) != -1} {
	    catch {$self _Close 'CLOSED'}
	}
	$callbacks destroy
    }
    ##
    # tryRead
    #    Check to see if there are data currently buffered in our channel
    #    and if so dispatch.  During this , the -fragmentcommand is disabled.
    #
    method tryRead {} {
	if {[chan pending input $options(-socket)] > 0} {
	    $callbacks register -fragmentcommand [list]; # turn off callback
	    $expecting $options(-socket)
	    $callbacks register -fragmentcommand $options(-fragmentcommand); #  Restore.
	}
    }

    #----------------------------------------------------------------------------
    # Configuration
    #

    ##
    # _SetCallback - register a callback handler for an option.
    #
    # @param option - name of the option.
    # @param script - New script to register.
    #
    method _SetCallback {option script} {
	$callbacks register $option $script
	set options($option) $script
    }


    #------------------------------------------------------------------------------
    # Private methods:

    ##
    # _DecodeConnectBody
    #
    #  Decodes the body of a connect message.  This consists of a null terminated
    #  ASCII String that is then followed by:
    #  - A count of the source ids
    #  - The source ids themselves.
    #  
    #  The count and source ids are all little endian uint32_t numbers.
    #
    # @param body - The body binary blob.
    # @return list - 2 element list.
    # @retval First list element is the description string.  The second, a list of source ids.
    #
    method _DecodeConnectBody body {
       
	# The loop below pulls the characters out of the description
	# one at a time.  cursor is the current position in the binary 
	# array.  Characters are appended to description until a null is seen:

	set cursor 0
	set description ""

	while {1} {
	    binary scan $body "@${cursor}c1" char
	    incr cursor

	    if {$char} {
		append description [format %c $char]
	    } else {
		break
	    }
	}
	# cursor now point just past the null in the string.. at the
	# number of source ids:

	binary scan $body "@${cursor}i1" sourceCount
	incr cursor 4

	set sourceList [list]
	for {} {$sourceCount > 0} {
	    incr sourceCount -1
	    incr cursor 4
	} {
	    binary scan $body "@${cursor}i1" source
	    lappend sourceList $source
	}


	return [list $description $sourceList]
    }

    ##
    # Read binary data from the socket with a leading uint32_t that contains
    # the number of bytes in the data:
    #
    # @param socket - The socket to read from.
    #
    # @return byte array containing the data read from the socket.
    #
    method _ReadBinaryData socket {
	set count [read $socket 4]; # Read the count.

	if {[string length $count] != 4} {
	    error "Read of byte array length failed"
	}
	binary scan $count i1 size

	if {$size > 0} {
	    return [read $socket $size]
	} else {
	    return ""
	}
    }

    ##
    # Reads a counted string from the socket turning it into a 
    # Tcl string.
    #
    # @param socket
    # @return string
    # @note If a read on the socket fails an error is thrown.
    #
    method _ReadCountedString socket {
	#
	# Note that [string length] is documented to
	# return the length of a byte array object if that's what it's
	# applied to so it should work below:
	#
	set count [read $socket 4]; # uint32_t.
	if {[string length $count] != 4} {
	    error "read of string length failed"
	}
	binary scan $count i1 size
	set stringBytes [read $socket $size]
	if {[string length $stringBytes] != $size} {
	    error "read of string itself failed"
	}
	#
	# Decode the string as a tcl string via binary scan
	#
	binary scan $stringBytes a$size result
	return $result

    }

    ##
    # Read/Decode a message whose header and body payloads are ASCII Strings.
    #
    # @param socket - The socket open on the client.
    #
    # @return a 2 element list.  [lindex .. 0] is the header text.
    #         [lindex .... 1] is the body text.
    #
    method _ReadTextMessage socket {

	set header [$self _ReadCountedString $socket]
	set body   [$self _ReadCountedString $socket]

	return [list $header $body]

    }

    ##
    #   Indicate which method shouild be associated with socket readability as part
    #   of a state transition.
    #
    # @param method - the new method that shouild handle readability.
    # @param newState - The new state value.
    #
    method _Expecting {method newState} {
	fileevent $options(-socket) readable [mymethod $method $options(-socket)] 
	set options(-state) $newState
	set expecting $method
    }
    ##
    # Called to close the connection
    #  - The new state is set.
    #  - The socket is closed.
    #  - The callback, if set, is called.
    #
    # @param newState - New state to set.
    method _Close {newState} {
	set options(-state) $newState
	fileevent $options(-socket) readable [list]
	close $options(-socket)

	set options(-socket) -1

	$callbacks invoke -disconnectcommand [list] [list]

    }

   
    ##
    #  Input handler for when we are expecting a CONNECT message.
    #  when one is properly handled, we transition to the ACTIVE state.
    #
    # @param socket - the socket on which we are going to get our
    #                 message.
    # @note:  If the message given is not a valid CONNECT,
    #         We just transition to ERROR, and close out
    #
    method _Connect socket {

	# Get the header and get the body.
	# The header is a string, but the body is a counted binary block:

	if {[catch {$self _ReadCountedString $socket} header]} {
	    catcl {puts $socket "ERROR {Could not read header expecting CONNECT}"}; # Might fail.
	    $self _Close ERROR
	    return
	}

	if {[catch {$self _ReadBinaryData $socket} body]} {
	    puts $socket "ERROR {Corrupt body in CONNECT message}"
	    $self _Close ERROR
	}

	# Must be a CONNECT message with a non-zero body.
	#

	if {$header ne "CONNECT"} {
	    catch {puts $socket "ERROR {Expected CONNECT}"}
	    $self _Close ERROR
	    return
	}


	if {[string length $body] == 0} {
	    catch {puts $socket "ERROR {Empty Body}"}
	    $self _Close ERROR
	    return
	}
	puts $socket "OK"

	# Pull out the description and the source id list

	set decodedBody [$self _DecodeConnectBody $body]
	set description [lindex $decodedBody 0]
	set sourceIds   [lindex $decodedBody 1]



	# Save the description and transition to the active state:
        # Register ourself with the event builder core

	set options(-description) $description
	$self _Expecting _Fragments ACTIVE


	EVB::source $socket {*}$sourceIds
 
    }
    #
    # Expecting fragments if the next message is
    # DISCONNECT, close the socket after responding.
    # If the next message has a FRAGMENTS header
    # pass the socket down to the C/C++ code
    # for further processing.
    #
    # @param socket - socket that is readable.
    #
    method _Fragments socket {
	set status [catch {
	    set header [$self _ReadCountedString $socket]
#	    set body   [$self _ReadBinaryData    $socket]
	} msg]
	if {[eof $options(-socket)]} {
	    $self _Close LOST
	    return;		# Nothing else to do.
	}

	# Protocol allows a DISCONNECT here:

	if {$header eq "DISCONNECT"} {
	    puts $socket "OK"
	    $self _Close

	} elseif {$header eq "FRAGMENTS"} {

	    # protocol allows FRAGMENTS here:
	    # TODO: Handle errors as a close

	    if {[catch {EVB::handleFragment $socket} msg]} {
		puts stderr "Event orderer failed call to handleFragment: : $msg"
		tk_messageBox -type ok -icon error -title {Fragment Handling error} \
		    -message "C++ Fragment handler reported an error: $msg"
		exit;		# can't really continue.
	    }


	    $callbacks invoke -fragmentcommand [list] [list]

	    
	    puts $socket "OK"

	} else {
	    # Anything else is a crime against The Protocol:


	    puts $socket "ERROR {Unexpected header: $header}"

	}
    }

}

##
#  Connection manager object.  Creating one of these
#  creates a connection manager on the specified port
#  which will create Connection objects each time a connection arrives.
#  and will destroy them as they die.
#
# OPTIONS
#                the port manager.
#   -port - port on which we are listening for connections.
#   -connectcommand - script to call when a connection has been added.
#                    Substitutions:
#                     %H - Host that is connecting with us.
#                     %O - Connection object created to manage this connection.
#   -disconnectcommand - Script to call when a connection has been lost.
#                     %O - Connection object created to manage this connection.
#                     %H - Host from which the client came.
#                     %D - Connection descdription.
#   -sourcetimeout - Number of seconds of not getting a fragment
#                    from a data source after which it is considered timed out.
#
#                     
# METHODS:
#  getConnections - List the connections and their states.
#
snit::type EVB::ConnectionManager {
    component TimeoutObservers

    option -port;		# Port on which we listen for connections.
    option -connectcommand   -default [list]  -configuremethod _SetCallback
    option -disconnectcommand -default [list] -configuremethod _SetCallback
    option -sourcetimeout    -default 10

    variable serverSocket;	           # Socket run by us.
    variable connections -array {};        # Key is connection, value last received timestamp.
    variable lastFragment;		   # When the last fragment arrived.
    variable callbacks;		           # Callback manager.
    variable timedoutSources [list];	   # list of connections that are timed out.
   

    delegate method addObserver    to TimeoutObservers as addTimeoutObserver
    delegate method removeObserver to TimeoutObservers as removeTimeoutObserver


    constructor args {
        set result [catch {
	$self configurelist $args; # To get the port.

	set lastFragment [clock seconds]

	#
	# Set up the callback manager.
	#
	set callbacks [EVB::CallbackManager %AUTO%]
	$callbacks define -connectcommand
	$callbacks define -disconnectcommand

	set serverSocket [socket -server [mymethod _NewConnection] $options(-port)]

	# watch timeouts at 1/2 the timeout interval:

	after [$self _TimeoutCheckInterval] [mymethod _CheckSourceTimeouts]

	install TimeoutObservers using  Observer %AUTO%
        } msg]
        if {$result} {
            puts stderr "$msg\n $::errorInfo"
            exit
        }
    }
    destructor {
	foreach object [array names connections] {
	    $object destroy
	    unset connections($object)
	}
	close $serverSocket
	$callbacks destroy
	# $TimeoutObservers destroy
    }
    #-----------------------------------------------------------------
    # Public methods:
    #

    ##
    # Get the list of connections
    #
    # @return list of three element sublists (possibly empty).
    # @retval Each list element contains in order:
    #  - Client's host.
    #  - description of client.
    #  - Connection state string.
    #  - "yes" if the connection is in the timedout list
    #    "" if not.
    method getConnections {} {
	set result [list];	# possibly empty return list.
	foreach connection [array names connections] {
	    set host  [$connection cget -clientaddr]
	    set desc  [$connection cget -description]
	    set state [$connection cget -state]
	    set timedout ""
	    if {[lsearch -exact $timedoutSources $connection] != -1} {
		set timedout "yes"
	    }

	    lappend result [list $host $desc $state $timedout]
	}
	return $result
    }
    #-----------------------------------------------------------------
    #  Private methods.

    ##
    # Client disconnect is just removing it from the list of connections
    # and invoking the disconnect callback.  Once all that dust has settled,
    # the connection object is destroyed, and its socket closed.
    #
    #
    # @param object - connection object name.
    # @note  Since it should not be possible for an object to disconnect that
    #        is not in our list, we toss an error if we are handed one.
    #
    method _DisconnectClient object {

	if {[array names connections $object] ne ""} {

	    $callbacks invoke -disconnectcommand [list %O %H %D] \
		[list $object [$object cget -clientaddr] [$object cget -description]]
	    
	    # If the connection is stalled, remove it from the stalled list:

	    set stallIndex [lsearch -exact $timedoutSources $object]
	    if {$stallIndex != -1} {
		set timedoutSources [lreplace $timedoutSources $stallIndex $stallIndex]
	    }

	    unset connections($object)
	    $object destroy
	} else {
	    error "BUG - $object not in ConnectionManager known connections"
	}
    }


    ##
    #  React to client connections:
    #  - A new Connection object is created to interact witht the client.
    #  - The object is aded to the connections list.
    #  - The object's -disconnectcommand is configured to invoke
    #    _DisconnectClient
    #  - We invoke the -connectcommand callback.
    #
    #
    #  @param sock - socket connected to the client
    #  @param client - Client IP addrerss.
    #  @param cport - client port (we could care less).
    #
    method _NewConnection {sock client cport} {
	set connection [EVB::Connection %AUTO% -socket $sock  -clientaddr $client]
	set connections($connection) [clock seconds]; # connection's last frag time to be not-timedout.
	$connection configure -disconnectcommand [mymethod _DisconnectClient $connection]
	$connection configure -fragmentcommand   [mymethod _RecordFragment $connection]
	$callbacks invoke -connectcommand [list %H %O] [list $client $connection]
    }
    ## 
    # We very carefully ensured the callbacks registered with the callback manager
    # are the same as the options that set/get them.
    # this ensures that we can have a single configuremethod take care of both of them.
    # 
    # @param option - Option being set (callback name).
    # @param value  - New value for the option (the callback).
    #
    method _SetCallback {option value} {
	$callbacks register $option $value
	set options($option) $value
    }
    ##
    # _RecordFragment
    #
    #  Record the time at which the most recent fragment arrived and which 
    #  connection received it.
    #
    # @param connection - the connection that just got the fragment.
    #
    method _RecordFragment connection {
	set connections($connection) [clock seconds]
	set lastFragment [clock seconds]

	if {$connection in $timedoutSources} {
	    EVB::reviveSocket [$connection cget -socket]
	    set cindex [lsearch -exact $timedoutSources $connection]
	    set timedoutSources [lreplace $timedoutSources $cindex $cindex]
	}
	# Now ensure none of the connections gets starved by calling their
	# tryRead -- this is because I think under heavy unbalanced load, the
	# event dispatcher may not be fair, only dispatching the first readable socket
	# even if other sockets are readable...and if that socket becomes readable...
	#
	foreach c [array names connections] {
	    if {$c != $connection} {
		$c tryRead
	    }
	}
    }
    ##
    #  Check for lack of data on data sources;
    #  - If we've not had any data in a timeout period, nobody's timed out.
    #  - If we have had data within a timeout but a source has not contributed,
    #  - It's timed out.
    #
    method _CheckSourceTimeouts {} {

	set previouslyTimedOut $timedoutSources

	# Only care if we're getting fragments:

	set now [clock seconds]
	if {($now - $lastFragment) < $options(-sourcetimeout)} {
	    set timedoutSources [list]; # Assume it's all just peachy.
	    foreach connection [array names connections] {
		if {($now - $connections($connection)) > $options(-sourcetimeout)} {
		    lappend timedoutSources $connection
		}
	    }
	}

	# Invoke observers for each newly timed out source
	# Invocation is of the form:
	#   observer STALLED connection

	foreach source $timedoutSources {
	    if {$source ni $previouslyTimedOut} {
		catch {$TimeoutObservers invoke STALLED $source} msg
		EVB::deadsource [$source cget  -socket]; # Indicate source dead to fragment handler.
	    }
	}

	# invoke observers that are no longer  stalled:
	# Invocation is of the form.
	#    observer UNSTALLED $source
	#
	foreach source $previouslyTimedOut {
	    if {$source ni $timedoutSources} {
		catch {$TimeoutObservers invoke  UNSTALLED $source} msg
	    }
	}


	# Reschedule:

	after [$self _TimeoutCheckInterval] [mymethod _CheckSourceTimeouts]
    }
    ##
    # compute the timeout check interval:
    #
    # @return ms to next timeout check.
    #
    method _TimeoutCheckInterval {} {
	return [expr {int(1000*$options(-sourcetimeout)*2.0)}]
    }
}

