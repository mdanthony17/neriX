#ifndef __CSCRIPTEDSEGMENT_H
#define __CSCRIPTEDSEGMENT_H

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


#ifndef __CEVENTSEGMENT_H
#include <CEventSegment.h>
#endif


#ifndef __STL_STRING
#include <string>
#ifndef __STL_STRING
#define __STL_STRING
#endif
#endif

class CDigitizerDictionary;
class CReadOrder;
class CTCLInterpreter;
class CModuleCommand;
class CModuleCreator;

struct ScriptedBundle;

/*!
  Class that provides an event segment that is driven by the
  scripted readout software within the productino readout
  software.
*/
class CScriptedSegment : public CEventSegment
{
  // Member data:
private:
  ScriptedBundle*   m_pBundle;
  CTCLInterpreter*  m_pInterp;

public:
  CScriptedSegment();

public:
  virtual void initialize();
  virtual void clear();
  virtual void disable();
  virtual size_t read(void* pBuffer, size_t maxwords);

  // protected members:

protected:
  void addCreator(CModuleCreator& creator);
  void addStandardCreators();
  virtual void addUserWrittenCreators(); // Hook for user written readout modules.
  virtual std::string getConfigurationFile(); // Hook for users to override default config file

};
#endif
