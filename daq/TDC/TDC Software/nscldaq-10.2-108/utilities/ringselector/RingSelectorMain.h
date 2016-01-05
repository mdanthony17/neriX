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
#ifndef __RINGSELECTORMAIN_H
#define __RINGSELECTORMAIN_H

#ifndef __STL_STRING
#include <string>
#ifndef __STL_STRING
#define __STL_STRING
#endif
#endif


#ifndef __CRT_UNISTD_H
#include <unistd.h>
#ifndef __CRT_UNISTD_H
#define __CRT_UNISTD_H
#endif
#endif


class CRingBuffer;
class CRingSelectionPredicate;
struct gengetopt_args_info;

/*!
   This class is the ring selector application. Written as a separate class, we
   can think of ways to get some automated tests written that don't require running the program.
*/
class RingSelectorMain {
  // Object storage:
private:
  CRingBuffer*              m_pRing;          // Data source.
  CRingSelectionPredicate*  m_pPredicate;     // Predicate used to select data.
  bool                      m_formatted;      // Format output.
  bool                      m_exitOnEnd;      // If true exit when end run seen.
  // Constructors..
public:
  RingSelectorMain();
  ~RingSelectorMain();

  // uniimplemented canonicals/constructors:

private:
  RingSelectorMain(const RingSelectorMain&);
  RingSelectorMain& operator=(const RingSelectorMain&);
  int operator==(const RingSelectorMain&) const;
  int operator!=(const RingSelectorMain&) const;

  // Entry point:

public:
  int operator()(int argc, char** argv);

  // Sub-chunks of the program:

  CRingSelectionPredicate*  createPredicate(struct gengetopt_args_info* parse);
  CRingBuffer*              selectRing(struct gengetopt_args_info* parse);
  void                      processData();


  // Utilities:

  void        writeBlock(int fd, void* pData, size_t size);
};
#endif
