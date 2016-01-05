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
using namespace std;

#include "CModuleCommand.h"
#include <TCLInterpreter.h>
#include <TCLObject.h>
#include <CControlModule.h>
#include <CControlHardware.h>
#include <TclServer.h>
#include <CGDG.h>
#include <CV812.h>
#include <CV6533.h>
#include <CVMUSBModule.h>
#include <ChicoTrigger.h>

/*!
   Construct the command. 
   - The base class is created with the command string "Module".
     creation is done with registration.
   - the member data consists of a reference to the list of currently 
     defined modules (this is held by the interpreter thread main object
     as member data so that it will never go out of scope).
     \param interp : CTCLInterpreter&
       The interpreter on which this command will be registered.
     \param modules :  vector<CControlModule*>&
        Reference to the list of modules that have been defined already.
*/
CModuleCommand::CModuleCommand(CTCLInterpreter& interp,
			      TclServer&       server) :
  CTCLObjectProcessor(interp, "Module"),
  m_Server(server)  
{}
//! Destroy the module.. no op provided only as a chain to the base class destructor.
CModuleCommand::~CModuleCommand()
{}


/*!
   The command executor.  The command must have at least 2 object elements:
   - the command name ("Module"),
   - The subcommand .. which must be either "create", "configure" or cget.

   \param interp : CTCLInterpreter& 
       Reference to the interpreter that is running this command.
    \param vector<TCLObject>& objv
       Reference to an array of objectified Tcl_Obj objects that contain the
       command line prarameters.
    \return int
    \retval - TCL_OK  - If eveything worked.
    \retval - TCL_ERROR - on failures.
*/
int
CModuleCommand::operator()(CTCLInterpreter& interp,
			   vector<CTCLObject>& objv)
{
  // validate the parameter count.

  if (objv.size() < 2) {
    m_Server.setResult("module: Insufficient parameters need module create | config | cget");
    return TCL_ERROR;
  }

  // dispatch to the correct function depending on the command keyword.
  

  if (string(objv[1]) == string("create")) {
    return create(interp, objv);
  }
  else if (string(objv[1]) == string("config")) {
    return configure(interp, objv);
  }
  else if (string(objv[1]) == string("cget")) {
    return cget(interp, objv);
  }
  else {
    m_Server.setResult("module: Invalid subcommand need module create | config |cget");
    return TCL_ERROR;
  }
}
/*
   create a new module at present we hardcode the module type to
   be jtecgdg.  The full form of the command is:
   module create jtecgdg name

   This module must later be configured via e.g. 
   module config name ...

   If module types proliferate a more scalable mechanism like an extensible
   object factory might be a better way to do this.
*/
int
CModuleCommand::create(CTCLInterpreter& interp,
		       vector<CTCLObject>& objv)
{
  if (objv.size() != 4) {
    m_Server.setResult("module create: Wrong number of params need: module create type name");
    return TCL_ERROR;
  }
  string type = objv[2];
  string name = objv[3];

  CControlHardware* pModule;

  if (type == "jtecgdg") {
    pModule = new CGDG(name);
  }
  else if (type == "caenv812") {
    pModule = new CV812(name);
  }
  else if (type == "caenv895") {
    pModule = new CV812(name);	// Not a typo.  the 895/812 module are the same.
  }
  else if (type == "vmusb") {
    pModule = new CVMUSBModule(name);
  }
  else if (type == "v6533") {
    pModule = new CV6533(name);
  }
  else if (type == "ChicoTrigger") {
    pModule = new ChicoTrigger(name);
  }
  else {
    m_Server.setResult("module create: Invalid type, must be one of jtecgdg, caenv182, caenvg895, vmusb, v6533 ChicoTrigger");
    return TCL_ERROR;
  }
  CControlModule*   pConfig = pModule->getConfiguration();
  pModule->onAttach(*pConfig);
  m_Server.addModule(pConfig);
  m_Server.setResult(name);
  

  return TCL_OK;
}
/*! 
   Configures a module. The form of the command is:
   module  config name key1 value1...

   Configuration items are therefore keyword value pairs.
   Each pair passed to the configure member of the matching
   configuration object.
*/
int
CModuleCommand::configure(CTCLInterpreter& interp,
			  vector<CTCLObject>& objv)
{
  // Must be at least 3 command elements and an odd number.

  size_t nelements = objv.size();
  if ((nelements < 3) || ((nelements % 2) == 0)) {
    m_Server.setResult("module config : invalid number of command elements.");
    return TCL_ERROR;
  }
  string name = objv[2];

  CControlModule* pModule = m_Server.findModule(name);
  if (!pModule) {
    string msg("module config: ");
    msg += name;
    msg += " not found.";
    m_Server.setResult(msg);
    return TCL_ERROR;
  }
  for (int i = 3; i < nelements; i+=2) {
    string key   = objv[i];
    string value = objv[i+1];
    try {
      pModule->configure(key, value);
    }
    catch (string failmsg) {
      string msg("module config: Failed to configure ");
      msg += name;
      msg += " with: ";
      msg += key;
      msg += " ";
      msg += value;
      msg += " because: ";
      msg += failmsg;
      m_Server.setResult( msg);
      return TCL_ERROR;
    }
  }


  // Success if we got this far.

  return TCL_OK;		// No result string.
  
}

/*
  Return a configuration item.  There are two forms:
  module cget name ?key?
  
  If key is provided, only that item is returned, as just a simple value.
  If key is not provided, the entire configuration is dumped
  as a Tcl list of 2 element {key value} sublists.
*/
int
CModuleCommand::cget(CTCLInterpreter& interp, vector<CTCLObject>& objv)
{
  if ((objv.size() < 3) || (objv.size() > 4)) {
    m_Server.setResult("module cget : invalid number of parameters; need module cget name ?key?");
    return TCL_ERROR;
  }

  string          name    = objv[2];
  CControlModule* pModule = m_Server.findModule(name);
  if(!pModule) {
    string msg("module cget ");
    msg += name;
    msg += " module not found";
    m_Server.setResult( msg);
    return TCL_ERROR;
  }

  // The two cases:

  if (objv.size() == 3) {	// module cget name - dump the lot.
    CConfigurableObject::ConfigurationArray config = pModule->cget();
    Tcl_Obj* result = Tcl_NewListObj(0, NULL);
    for (int i =0; i < config.size(); i++) {
      string key   = config[i].first;
      string value = config[i].second;

      Tcl_Obj* keyObj = Tcl_NewStringObj(key.c_str(), -1);
      Tcl_Obj* valObj = Tcl_NewStringObj(value.c_str(), -1);
      Tcl_Obj*  objVector[2] = {keyObj, valObj};
      Tcl_Obj* element = Tcl_NewListObj(2, objVector);
      Tcl_ListObjAppendElement(interp, result, element);

    }
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
  }
  else {			// module cget name key dump the single key.
    string key = objv[3];
    string value;
    try {
      value = pModule->cget(key);
    }
    catch (string failmsg) {
      string msg("module cget: Failed for key: ");
      msg += key;
      msg += " because: ";
      msg += failmsg;
      m_Server.setResult( msg);
      return TCL_ERROR;
    }
    m_Server.setResult(value);
    return TCL_OK;
    
  }
}


