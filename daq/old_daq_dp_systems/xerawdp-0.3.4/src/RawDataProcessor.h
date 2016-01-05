#ifndef __RAWDATAPROCESSOR_H__
#define __RAWDATAPROCESSOR_H__

#include "xerawdp.h"

#include <string>

class XmlConfig;
class RawDataController;
class EventData;

class RawDataProcessor
{
public:
	RawDataProcessor();
	~RawDataProcessor();

public:
	bool configure(const std::string &hConfigFileName, bool bPretend, bool bOverwrite);
	bool setDataset(const std::string &hDatasetFileName);
	void setVerbosity(int iVerbosity) { m_iVerbosity = iVerbosity; }
	int getNbEvents();
	XmlConfig const *getXmlConfig() { return m_pXmlConfig; }

	bool processEvents();
	EventData const *processEvent(int iEventIndex);

private:
	std::string extractMergedDatasetNameFormat(const std::string &hDatasetNameFormat);
	bool verifyNameFormat(const std::string &hFileName, const std::string &hNameFormat);
	bool readRawData(int iEventIndex);
	bool readRawData_tdc(int iEventIndex);

private:
	std::string m_hConfigFileName;
	std::string m_hDatasetFileName;
	std::string m_hDatasetFileNameWithPath;
	std::string m_hRootFileNameWithPath;

	bool m_bPretend;
	bool m_bOverwrite;

	bool m_bUseMergedData;

	XmlConfig *m_pXmlConfig;

	int m_iVerbosity;

	RawDataController *m_pRawDataController;
	EventData *m_pEventData;
};

#endif // __RAWDATAPROCESSOR_H__

