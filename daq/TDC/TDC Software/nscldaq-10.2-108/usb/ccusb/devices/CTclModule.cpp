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

#include <config.h>
#include "CTclModule.h"
#include <TCLInterpreter.h>
#include <arpa/inet.h>

/**
 * Construction simply saves the nam of the command ensemble:
 *
 * @param command - base name of the command ensemble.
 * @param interp  - Tcl interpreter on which we'll operate.
 */
CTclModule::CTclModule(std::string command, CTCLInterpreter& interp) :
  m_command(command),
  m_pInterp(&interp)
{
}

/** Copy construction jut has to copy the command name:
 */
CTclModule::CTclModule(const CTclModule& rhs)
{
  m_command = rhs.m_command;
  m_pInterp = rhs.m_pInterp;
}
/**
 * Destructor is all done by the base class:
 */
CTclModule::~CTclModule()
{
}
/**
 * Assignment is possible too. Simply copy in the new command name:
 */
CTclModule&
CTclModule::operator=(const CTclModule& rhs)
{
  m_command = rhs.m_command;
  m_pInterp = rhs.m_pInterp;
  return *this;
}
/**
 * onAttach is a no-op as the Tcl code must maintain the configuration.
 *
 */
void
CTclModule::onAttach(CReadoutModule& config)
{
}
/**
 * Initialize must generate the swig 'pointer', execute the command's
 * Initialize subcommand, passing the swig pointer on our interpreter.
 *
 * @param controller - Reference to a CCCUSB controller object.
 */

void
CTclModule::Initialize(CCCUSB& controller)
{
  std::string pointer = swigPointer(&controller, "CCCUSB");
  std::string command = m_command;
  command            += " Initialize ";
  command            += pointer;

  m_pInterp->GlobalEval(command.c_str());
}
/**
 * Similarly wrap the list in a swig pointer and execute the command's
 * addReadoutList method 
 *
 * @param list - CCCUSBReadoutList that has to have items added.
 */
void
CTclModule::addReadoutList(CCCUSBReadoutList& list)
{
  std::string pointer = swigPointer(&list, "CCCUSBReadoutList");
  std::string command = m_command;
  command            += " addReadoutList ";
  command            += pointer;

  m_pInterp->GlobalEval(command.c_str());
}
/**
 * Clone is just a virtual copy construction:
 *
 */
CReadoutHardware*
CTclModule::clone() const
{
  return new CTclModule(*this);
}
/**
 * Generate a swig pointer from the C++ Pointer and its type.
 * This is of the form _address_p_typename
 * @param obj - pointer to the object.
 * @param type - Type name.
 *
 * @return std::string
 */
std::string
CTclModule::swigPointer(void* p, std::string  type)
{
  char pointerArray[1000];
  sprintf(pointerArray, "_%x_p_%s", htonl((uint64_t)p), type.c_str());
  return std::string(pointerArray);
}
