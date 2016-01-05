/*
    This software is Copyright by the Board of Trustees of Michigan
    State University (c) Copyright 2005.

    You may use this software under the terms of the GNU public license
    (GPL).  The terms of this license are described at:

     http://www.gnu.org/licenses/gpl.txt

     Author:
             Ron Fox
	     NSCL
	     Michigan State University
	     East Lansing, MI 48824-1321
*/

#include "CEVBClientApp.h"
#include "CEVBFrameworkApp.h"

/**
 * CEVBCleitnApp.cpp - implementation of the methods of the CEVBClientApp ABC that are
 *                     not abstract.
 */


/**
 * Constructor: Registers this with the frameowork.
 */
CEVBClientApp::CEVBClientApp()
{
  CEVBFrameworkApp::getInstance()->addSource(this);
 
}
/**
 * Destructor: Unregister this with the framwork.
 */
CEVBClientApp::~CEVBClientApp()
{
  CEVBFrameworkApp::getInstance()->removeSource(this);
}


/**
 *  initialize: 
 *    Connect to the event builder.  This is now done here to allow
 *    the user's initialize to decide when to do this relative
 *    to the other initialization stuff.
 */
void
CEVBClientApp::initialize() {

}

/**
 * shutdown: As for initialize but the system is shutting down.
 */
void
CEVBClientApp::shutdown() {}
