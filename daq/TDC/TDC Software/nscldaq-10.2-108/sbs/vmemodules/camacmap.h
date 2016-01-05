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


#ifndef __CAMACMAP_H
#define __CAMACMAP_H

  /*
  ** CAMAC/BIT3 device interface functions:
  */
void* daq_OpenCamacDriver();	        /* Open the Camac device driver  */
void* daq_MapCamacSpace(void* fid);	/* Map to camac address space.   */
void  daq_UnmapCamacSpace(void* fid,void* pSpace); /* Unmap CAMAC address space.   */
void  daq_CloseCamacDriver(void* fid);    /* deaccess the CAMAC.           */



#endif
