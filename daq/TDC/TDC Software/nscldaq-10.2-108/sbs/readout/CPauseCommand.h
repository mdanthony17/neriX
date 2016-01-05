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

#ifndef __CPAUSECOMMAND_H
#define __CPAUSECOMMAND_H


#ifndef __TCLPACKAGEDOBJECTPROCESSOR_H
#include <TCLPackagedObjectProcessor.h>
#endif

#ifndef __STL_VECTOR
#include <vector>
#ifndef __STL_VECTOR
#define __STL_VECTOR
#endif
#endif


#ifndef __STL_STRING
#include <string>
#ifndef __STL_STRING
#define __STL_STRING
#endif
#endif


// Forward class defs:

class CTCLInterpreter;
class CTCLObject;

/*!
   This class provides the pause command.  The pause command is valid only during
   an active run.  It temprorarily halts data taking.  Paused runs can either be 
   permanently ended ('end' command). Or resumed ('resume' command).

*/
class CPauseCommand : public CTCLPackagedObjectProcessor
{
  // Canonicals. Copy etc. are not allowed:
public:
  CPauseCommand(CTCLInterpreter& interp);
  virtual ~CPauseCommand();

private:
  CPauseCommand(const CPauseCommand& rhs);
  CPauseCommand& operator=(const CPauseCommand& rhs);
  int operator==(const CPauseCommand& rhs) const;
  int operator!=(const CPauseCommand& rhs) const;

  // command entry point:

public:
  virtual int operator()(CTCLInterpreter& interp,
			 std::vector<CTCLObject>& objv);

private:
  std::string usage();
};



#endif
