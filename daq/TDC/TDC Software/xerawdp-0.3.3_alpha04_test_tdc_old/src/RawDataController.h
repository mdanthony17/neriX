#ifndef __RAWDATACONTROLLER_H__
#define __RAWDATACONTROLLER_H__

#include "xerawdp.h"

#include <string>
#include <libxdio.h>

class RawData;

class RawDataController
{
public:
	RawDataController();
	~RawDataController();

public:
	bool open(const std::string &hFileName);
	void close();

	int getNbEvents();
	int getEventRawData(int iEventIndex, RawData ** a_pRawData);
	RawData *getEventRawData_tdc(int iEventIndex);

private:

private:
	xdio_file_t *m_pDataFile;
};

#endif // __RAWDATACONTROLLER_H__

