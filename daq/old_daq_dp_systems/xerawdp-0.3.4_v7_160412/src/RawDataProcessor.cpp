#include "RawDataProcessor.h"

#include "XmlConfig.h"
#include "RawData.h"
#include "RawDataController.h"
#include "EventData.h"
#include "Waveform.h"
#include "AnalysisManager.h"

#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <ios>
#include <list>
#include <map>
#include <vector>
#include <cstdlib>
#include <math.h>

using std::string;
using std::list;
using std::map;
using std::vector;
using std::cout;
using std::endl;
using std::flush;

RawDataProcessor::RawDataProcessor()
{
	m_bPretend = false;
	m_bOverwrite = false;

	m_bUseMergedData = false;

	m_pXmlConfig = 0;
	m_pRawDataController = 0;
	m_pEventData = 0;

	m_iVerbosity = -1;
}

RawDataProcessor::~RawDataProcessor()
{
	if(m_pXmlConfig)
		delete m_pXmlConfig;
	if(m_pRawDataController)
		delete m_pRawDataController;
	if(m_pEventData)
		delete m_pEventData;
}

bool
RawDataProcessor::configure(const std::string &hConfigFileName, bool bPretend, bool bOverwrite)
{

	if(access(hConfigFileName.c_str(), R_OK))
	{
	        cout << "raw data processor error: cannot open xml configuration!" << endl;
		return false;
	}

	m_hConfigFileName = hConfigFileName;

	m_pXmlConfig = new XmlConfig();

	if(!m_pXmlConfig->readConfigFile(m_hConfigFileName))
	{
		cout << "raw data processor error: problem encountered during configuration!" << endl;
		return false;
	}

	m_bPretend = bPretend;
	m_bOverwrite = bOverwrite;

	if(m_iVerbosity == -1)
		m_iVerbosity = m_pXmlConfig->getVerbosity();
	else
		m_pXmlConfig->setVerbosity(m_iVerbosity);

	if(m_iVerbosity >= 2)
		m_pXmlConfig->printSettings();

	if(m_iVerbosity)
		cout << "RawDataProcessor" << endl;

	return true;
}

bool
RawDataProcessor::setDataset(const std::string &hDatasetFileName)
{
	// check that the raw data processor was configured
	if(!m_hConfigFileName.empty())
	{
		string hDatasetNameFormat = m_pXmlConfig->getDatasetNameFormat();
		string hMergedDatasetNameFormat = extractMergedDatasetNameFormat(hDatasetNameFormat);
		string hMergedDatasetName = hDatasetFileName.substr(0, hMergedDatasetNameFormat.length());

		// if we got a raw dataset as input
		if(verifyNameFormat(hDatasetFileName, hDatasetNameFormat))
		{
			// get the raw data location
			string hRawDataDir = m_pXmlConfig->getRawDataDir();

			if(*(hRawDataDir.rbegin()) != '/')
				hRawDataDir.append("/");

			string hDatasetDir = hMergedDatasetName + '/';

			m_hDatasetFileName = hDatasetFileName;

			string hDatasetFileNameWithPath = hRawDataDir + hDatasetDir + hDatasetFileName + ".xed";

			// verify that the raw data file exists
			if(!access(hDatasetFileNameWithPath.c_str(), R_OK))
				m_hDatasetFileNameWithPath = hDatasetFileNameWithPath;
			else
			{
				cout << "raw data processor error: dataset " << hDatasetFileNameWithPath << " does not exist!" << endl;
				return false;
			}

			// get the processed data destination
			string hProcessedDataDir = m_pXmlConfig->getProcessedDataDir();

			if(*(hProcessedDataDir.rbegin()) != '/')
				hProcessedDataDir.append("/");

			string hDatasetDirWithPath = hProcessedDataDir + hDatasetDir;

			// create the processed data directory for that dataset if it doesn't exist
			if(access(hDatasetDirWithPath.c_str(), R_OK) && !m_bPretend)
				mkdir(hDatasetDirWithPath.c_str(), 0775);

			string hVersionDirWithPath = hDatasetDirWithPath + "v" + VERSION + "/";

			// create the processed data directory for this version if it doesn't exist
			if(access(hVersionDirWithPath.c_str(), R_OK) && !m_bPretend)
				mkdir(hVersionDirWithPath.c_str(), 0775);

			m_hRootFileNameWithPath = hVersionDirWithPath + hDatasetFileName + ".root";

			// create a raw data controller
			m_pRawDataController = new RawDataController();
			m_pRawDataController->open(m_hDatasetFileNameWithPath);

			m_bUseMergedData = false;
		}
		// else if we got a merged dataset as input
		else if(verifyNameFormat(hDatasetFileName, hMergedDatasetNameFormat))
		{
			// get the merged data directory
			string hMergedDataDir = m_pXmlConfig->getMergedDataDir();

			if(*(hMergedDataDir.rbegin()) != '/')
				hMergedDataDir.append("/");

			string hMergedDatasetFileNameWithPath = hMergedDataDir + hDatasetFileName + ".root";

			// verify that the merged file exists
			if(access(hMergedDatasetFileNameWithPath.c_str(), R_OK))
			{
				cout << "raw data processor error: merged dataset " << hMergedDatasetFileNameWithPath << " does not exist!" << endl;
				return false;
			}

			m_hDatasetFileNameWithPath = hMergedDatasetFileNameWithPath;
			m_hRootFileNameWithPath = hMergedDatasetFileNameWithPath;

			m_bUseMergedData = true;
		}
		// the dataset does not obey any of the name formats
		else
		{
			cout << "raw data processor error: dataset name " << hDatasetFileName << " does not obey format!" << endl;
			return false;
		}
	}
	else
	{
		cout << "raw data processor error: not configured!" << endl;
		return false;
	}

	return true;
}

int
RawDataProcessor::getNbEvents()
{
	// check that the raw data processor knows which dataset to process
	if(m_hDatasetFileNameWithPath.empty())
	{
		cout << "raw data processor error: no dataset specified!" << endl;
		return 0;
	}

	// if we got a merged dataset as input get the number of events from there, otherwise, from the raw data
	if(m_bUseMergedData)
		AnalysisManager::getRootFileNbEvents(m_hDatasetFileNameWithPath);
	else
		return m_pRawDataController->getNbEvents();
}

bool
RawDataProcessor::processEvents()
{
	// check that the raw data processor knows which dataset to process
	if(m_hDatasetFileNameWithPath.empty())
	{
		cout << "raw data processor error: no dataset specified!" << endl;
		return false;
	}

	// create an event data object from which the root tree branches will be filled
	m_pEventData = new EventData();

	// create an analysis manager for this dataset
	AnalysisManager hAnalysisManager(*m_pXmlConfig, m_pEventData);

	// if not in pretend mode
	if(!m_bPretend)
	{
		// if we should overwrite any existing output file
		if(m_bOverwrite)
			hAnalysisManager.openRootFileRecreate(m_hRootFileNameWithPath);
		else
			hAnalysisManager.openRootFileUpdate(m_hRootFileNameWithPath);
	}

	// get the number of events in the raw data or the merged data
	int iNbEvents = (!m_bUseMergedData)?(m_pRawDataController->getNbEvents()):(hAnalysisManager.getNbEvents());

	// get the raw data from the event and give it to the analysis manager
	for(int iEventIndex = 0; iEventIndex < iNbEvents; iEventIndex++)
	{
		// if we got a merged dataset as input then set the event index, otherwise read the raw data
		if(m_bUseMergedData)
			hAnalysisManager.setEventIndex(iEventIndex);
		else
		{
			if(hAnalysisManager.getUseTDC())
			{
				if(hAnalysisManager.needRawData() && !readRawData_tdc(iEventIndex))
				return false;
			}
			else
			{
			if(hAnalysisManager.needRawData() && !readRawData(iEventIndex))
				return false;
			}
		}

		// analyze the waveforms and extract the quantities to the event data object
		hAnalysisManager.analyze();

		if(!m_bPretend)
			hAnalysisManager.write();

		m_pEventData->clear();

		if(m_iVerbosity)
			cout << "\r  " << m_hDatasetFileName << "  Processed: " << iEventIndex << flush;

		if(m_iVerbosity >= 4)
			cout << endl << *m_pEventData << endl << flush;
	}

	cout << endl;

	if(!m_bPretend)
		hAnalysisManager.closeRootFile();

	return true;
}

EventData const *
RawDataProcessor::processEvent(int iEventIndex)
{
	// check that the raw data processor knows which dataset to process
	if(m_hDatasetFileNameWithPath.empty())
	{
		cout << "raw data processor error: no dataset specified!" << endl;
		return false;
	}

	// create an event data object
	if(!m_pEventData)
		m_pEventData = new EventData();
	else
		m_pEventData->clear();

	// create an analysis manager for this dataset
	AnalysisManager hAnalysisManager(*m_pXmlConfig, m_pEventData);

	// read the raw data
	if(hAnalysisManager.getUseTDC())
	{
		if(!readRawData_tdc(iEventIndex))
			return false;
	}
	else
	{
		if(!readRawData(iEventIndex))
			return false;
	}
	
	
	// analyze the waveforms and extract the quantities to the event data object
	hAnalysisManager.analyze();

	if(m_iVerbosity)
		cout << "\r  " << m_hDatasetFileName << "  Processed: " << iEventIndex;

	return m_pEventData;
}

std::string
RawDataProcessor::extractMergedDatasetNameFormat(const std::string &hDatasetNameFormat)
{
	// check if there is an id field for datasets
	string::size_type iLast = hDatasetNameFormat.find_first_of("N");

	// if there is an id field
	if(hDatasetNameFormat[iLast] != string::npos)
	{
		// check if the previous caracter is an allowed field separator
		if(hDatasetNameFormat[iLast-1] == '-' || hDatasetNameFormat[iLast-1] == '_')
			iLast = iLast - 1;
	}
	else
	{
		// there is no field id, the merged dataset name will be the same name as the dataset
		iLast = hDatasetNameFormat.length();
	}

	string hMergedDatasetNameFormat = hDatasetNameFormat.substr(0, iLast);

	return hMergedDatasetNameFormat;
}

bool
RawDataProcessor::verifyNameFormat(const std::string &hFileName, const std::string &hNameFormat)
{
	// check that the lengths are equal
	if(hFileName.length() != hNameFormat.length())
		return false;

	// the allowed fields for the dataset name
	string hFields = "YMDhmsN";

	string::size_type iPos = 0;
	while(iPos < hNameFormat.length())
	{
		// if it's a field digit
		if(hFields.find(hNameFormat[iPos]) != string::npos)
		{
			// check if it's in the range
			if(hFileName[iPos] < '0' || hFileName[iPos] > '9')
				return false;
		}
		else
		{
			// it's not a field so it has to match exactly
			if(hFileName[iPos] != hNameFormat[iPos])
				return false;
		}

		iPos++;
	}

	return true;
}

bool
RawDataProcessor::readRawData(int iEventIndex)
{
	RawData *pRawData;
	RawData *a_pRawData[2];
	// need to remove tdc geteventrawdata and combine both into single function
	if((m_pRawDataController->getEventRawData(iEventIndex, a_pRawData)) == 0)
	{
		cout << "raw data processor error: could not get event raw data!" << endl;
		return false;
	}
	
	// take pointers from array and then free it
	pRawData = *a_pRawData;
	//free(a_pRawData);

	// seg faults somewhere in next four lines
	// pRawData is null

	// save information from raw data
	m_pEventData->setFilename(m_hDatasetFileName);
	m_pEventData->setEventId(pRawData->getEventNb());
	m_pEventData->setTimeSec(pRawData->getTimeSec());
	m_pEventData->setTimeMicroSec(pRawData->getTimeMicroSec());

//    if(pRawData->getNbChannels() != m_pXmlConfig->getNbPmts()+m_pXmlConfig->getNbUnusedChannels())
//    {
//        cout << "raw data processor error: number of channels in raw data does not match configuration!" << endl;
//        return false;
//    }

	int iNbBaselineSamples = (m_pXmlConfig->getProcessingLevel() == 0)?(m_pXmlConfig->getSingleNbBaselineSamples()):(m_pXmlConfig->getNbBaselineSamples());
	if(!pRawData->computeBaselines(iNbBaselineSamples))
	{
		cout << "raw data processor error: could not compute raw data baselines!" << endl;
		return false;
	}

	// save baseline information
	vector<int> hChannels;
	vector<vector<float> > hBaselinesAvg;
	vector<vector<float> > hBaselinesMax;

	map<int, RawDataChannel *> const *pChannels = pRawData->getChannels();
	map<int, RawDataChannel *>::const_iterator pIter;
	for(pIter = pChannels->begin(); pIter != pChannels->end(); pIter++)
	{
		hChannels.push_back(pIter->first);
		hBaselinesAvg.push_back(pIter->second->getBaselineAvg());
		hBaselinesMax.push_back(pIter->second->getBaselineMax());
	}

	m_pEventData->setChannels(hChannels);
	m_pEventData->setBaselinesAvg(hBaselinesAvg);
	m_pEventData->setBaselinesMax(hBaselinesMax);

	// get the waveform representation of the raw data
	m_pEventData->setWaveforms(pRawData->getWaveforms());

	delete pRawData;


	return true;
}

bool
RawDataProcessor::readRawData_tdc(int iEventIndex)
{
	RawData *pRawData;
	RawData *pRawData_tdc = new RawData;
	RawData *a_pRawData[2];
	// need to remove tdc geteventrawdata and combine both into single function
	if((m_pRawDataController->getEventRawData(iEventIndex, a_pRawData)) == 0)
	{
		cout << "raw data processor error: could not get event raw data!" << endl;
		return false;
	}
	
	// take pointers from array and then free it
	pRawData = *a_pRawData;
	pRawData_tdc = *(a_pRawData + 1);
	//free(a_pRawData);

	// save information from raw data
	m_pEventData->setFilename(m_hDatasetFileName);
	m_pEventData->setEventId(pRawData->getEventNb());
	m_pEventData->setTimeSec(pRawData->getTimeSec());
	m_pEventData->setTimeMicroSec(pRawData->getTimeMicroSec());

//    if(pRawData->getNbChannels() != m_pXmlConfig->getNbPmts()+m_pXmlConfig->getNbUnusedChannels())
//    {
//        cout << "raw data processor error: number of channels in raw data does not match configuration!" << endl;
//        return false;
//    }

	int iNbBaselineSamples = (m_pXmlConfig->getProcessingLevel() == 0)?(m_pXmlConfig->getSingleNbBaselineSamples()):(m_pXmlConfig->getNbBaselineSamples());
	if(!pRawData->computeBaselines(iNbBaselineSamples))
	{
		cout << "raw data processor error: could not compute raw data baselines!" << endl;
		return false;
	}

	// save baseline information
	vector<int> hChannels;
	vector<vector<float> > hBaselinesAvg;
	vector<vector<float> > hBaselinesMax;

	map<int, RawDataChannel *> const *pChannels = pRawData->getChannels();
	map<int, RawDataChannel *>::const_iterator pIter;
	for(pIter = pChannels->begin(); pIter != pChannels->end(); pIter++)
	{
		hChannels.push_back(pIter->first);
		hBaselinesAvg.push_back(pIter->second->getBaselineAvg());
		hBaselinesMax.push_back(pIter->second->getBaselineMax());
	}

	m_pEventData->setChannels(hChannels);
	m_pEventData->setBaselinesAvg(hBaselinesAvg);
	m_pEventData->setBaselinesMax(hBaselinesMax);

	// get the waveform representation of the raw data
	m_pEventData->setWaveforms(pRawData->getWaveforms());

	delete pRawData;

	/*
	map<int, RawDataChannel *> const *pChannels_test = pRawData_tdc->getChannels();
	for(pIter = pChannels_test->begin(); pIter != pChannels_test->end(); pIter++)
	{
		uint8_t msb = pIter->second->getBytes()[1];
		uint8_t lsb = pIter->second->getBytes()[0];
		unsigned short rawValue = (msb << 8) | (0xFF & lsb);
		float rawTimeValue = rawValue * pRawData_tdc->getVoltageRange()/ (pow(2, 12));
		//float timeValue = 180 - rawTimeValue;
		if (rawTimeValue != 0)
			printf("\nChannel %d with value: %f", pIter->first, rawTimeValue);
	}
	*/
	
	m_pEventData->setRawData_tdc(pRawData_tdc);

	return true;
}

/*
bool
RawDataProcessor::readRawData_tdc(int iEventIndex)
{
	RawData *pRawData;
	if((pRawData = m_pRawDataController->getEventRawData_tdc(iEventIndex)) == 0)
	{
		cout << "raw data processor error: could not get event raw data!" << endl;
		return false;
	}
	
	printf("\nInside readrawdata_tdc\n");

	// save information from raw data
	m_pEventData->setFilename(m_hDatasetFileName);
	m_pEventData->setEventId(pRawData->getEventNb());
	m_pEventData->setTimeSec(pRawData->getTimeSec());
	m_pEventData->setTimeMicroSec(pRawData->getTimeMicroSec());

	// get the waveform representation of the raw data
	// may need to change this so that all waveforms are included
	// create function called setWaveforms_tdc
	m_pEventData->setWaveforms_tdc(pRawData->getWaveforms());

	delete pRawData;

	return true;
}
*/
