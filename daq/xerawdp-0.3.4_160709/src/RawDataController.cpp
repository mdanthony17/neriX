#include "RawDataController.h"

#include "RawData.h"

#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>

using std::cout;
using std::endl;

RawDataController::RawDataController()
{
	m_pDataFile = NULL;
}

RawDataController::~RawDataController()
{
	if(m_pDataFile)
		close();
}

bool
RawDataController::open(const std::string &hFileName)
{
	return ((m_pDataFile = xdio_open_read(hFileName.c_str())) != NULL);
}

void
RawDataController::close()
{
	xdio_close(m_pDataFile);

	m_pDataFile = 0;
}

int
RawDataController::getNbEvents()
{
	if(m_pDataFile != NULL)
		return xdio_get_nb_events(m_pDataFile);
	else
	{
		cout << "raw data controller error: no file opened!" << endl;
		return -1;
	}
}

int
RawDataController::getEventRawData(int iEventIndex, RawData **a_pRawData)
{
	RawData *pRawData = new RawData();
	RawData *pRawData_tdc;

	// get the event from the xdio file
	xdio_event_t *pEvent;
	//printf("\nAbout to get event\n");
	if((pEvent = xdio_get_event(iEventIndex, m_pDataFile)) == NULL)
	{
		cout << "raw data controller error: could not get event at index " << iEventIndex << endl;
		delete pRawData;
		return 0;
	}
	
	
	// set the event time and event number
	pRawData->setTimeSec(xdio_get_trigger_time_sec(pEvent));
	pRawData->setTimeMicroSec(xdio_get_trigger_time_usec(pEvent));
	pRawData->setEventNb(xdio_get_event_nb(pEvent));

	// get the number of chunks
	int iNbChunks;
	if((iNbChunks = xdio_get_nb_chunks(pEvent)) < 1)
	{
		cout << "raw data controller error: event has no chunks!" << endl;
		delete pRawData;
		return 0;
	}
	//printf("\nNumber of Chunks = %d\n", iNbChunks);
	
	
	// get the first chunk, should be a zle0 or raw0 chunk
	//printf("about to load adc chunk");
	xdio_chunk_t *pChunk = xdio_get_chunk(0, pEvent);
	//printf("\nADC Chunk ptr: %p\n", pChunk);
	
	if (iNbChunks == 2)
	{
		pRawData_tdc = new RawData();
		//a_pRawData = (RawData **)malloc(iNbChunks * sizeof(RawData *));
		//a_pRawData is array of size 2 with 2 pointers to raw data
	}

	if(!xdio_chunk_type_cmp("raw0", pChunk))
	{
		// get the raw0 attributes
		xdio_chunk_raw0_att_t *pAtt = (xdio_chunk_raw0_att_t *) xdio_get_attributes(pChunk);

		pRawData->setType("raw0");
		pRawData->setSampleType(pAtt->sample_type);
		pRawData->setBytesPerSample(pAtt->bytes_per_sample);
		pRawData->setNbSamples(pAtt->nb_samples);
		pRawData->setVoltageRange(pAtt->voltage_range);
		pRawData->setSamplingFrequency(pAtt->sampling_frequency);
		pRawData->setNbChannels(pAtt->nb_channels);

		for(int iChannelIndex = 0; iChannelIndex < pAtt->nb_channels; iChannelIndex++)
		{
			int iChannelNb = iChannelIndex;
			int iChannelLength = pAtt->bytes_per_sample*pAtt->nb_samples;

			RawDataChannel *pRawDataChannel = new RawDataChannel();
			pRawDataChannel->allocateBytes(iChannelLength);

			pRawData->setChannel(iChannelNb, pRawDataChannel);

			xdio_attach_data_dst(iChannelNb, pRawDataChannel->getBytes(), iChannelLength, pChunk);
		}

		xdio_extract_data(pChunk);
	}
	else if(!xdio_chunk_type_cmp("zle0", pChunk))
	{
		// get the zle0 attributes
		xdio_chunk_zle0_att_t *pAtt = (xdio_chunk_zle0_att_t *) xdio_get_attributes(pChunk);

		pRawData->setType("zle0");
		pRawData->setSampleType(pAtt->sample_type);
		pRawData->setBytesPerSample(pAtt->bytes_per_sample);
		pRawData->setNbSamples(pAtt->nb_samples);
		pRawData->setVoltageRange(pAtt->voltage_range);
		pRawData->setSamplingFrequency(pAtt->sampling_frequency);
		pRawData->setNbChannels(pAtt->nb_channels);

		for(int iChannelIndex = 0; iChannelIndex < pAtt->nb_channels_included; iChannelIndex++)
		{
			int iChannelNb = pAtt->channels[iChannelIndex];
			int iChannelLength =  pAtt->channel_lengths[iChannelIndex];

			RawDataChannel *pRawDataChannel = new RawDataChannel();
			pRawDataChannel->allocateBytes(iChannelLength);

			pRawData->setChannel(iChannelNb, pRawDataChannel);

			xdio_attach_data_dst(iChannelNb, pRawDataChannel->getBytes(), iChannelLength, pChunk);
		}

		xdio_extract_data(pChunk);
	}
	else
	{
		cout << "raw data controller error: chunk should be 'raw0' or 'zle0'!" << endl;
		delete pRawData;
		return 0;
	}
	
	// now do TDC portion
	if (iNbChunks == 2)
	{
		pRawData_tdc->setTimeSec(xdio_get_trigger_time_sec(pEvent));
		pRawData_tdc->setTimeMicroSec(xdio_get_trigger_time_usec(pEvent));
		pRawData_tdc->setEventNb(xdio_get_event_nb(pEvent));

		//printf("about to load tdc chunk");

		// get the second chunk, should be a raw0 chunk for tdc
		xdio_chunk_t *pChunk_tdc = xdio_get_chunk(1, pEvent);
		//printf("\nTDC Chunk ptr: %p\n", pChunk_tdc);

		// get the raw0 attributes
		xdio_chunk_raw0_att_t *pAtt = (xdio_chunk_raw0_att_t *) xdio_get_attributes(pChunk_tdc);

		pRawData_tdc->setType("raw0");
		pRawData_tdc->setSampleType(pAtt->sample_type);
		pRawData_tdc->setBytesPerSample(pAtt->bytes_per_sample);
		pRawData_tdc->setNbSamples(pAtt->nb_samples);
		pRawData_tdc->setVoltageRange(pAtt->voltage_range);
		pRawData_tdc->setSamplingFrequency(pAtt->sampling_frequency);
		pRawData_tdc->setNbChannels(pAtt->nb_channels);
		
		//printf("\nChannel Length: %d\n", pAtt->bytes_per_sample*pAtt->nb_samples); // gives length in bytes (2 bytes)

		for(int iChannelIndex = 0; iChannelIndex < pAtt->nb_channels; iChannelIndex++)
		{
			int iChannelNb = iChannelIndex;
			int iChannelLength = pAtt->bytes_per_sample*pAtt->nb_samples;

			RawDataChannel *pRawData_tdcChannel = new RawDataChannel();
			pRawData_tdcChannel->allocateBytes(iChannelLength);

			pRawData_tdc->setChannel(iChannelNb, pRawData_tdcChannel);

			xdio_attach_data_dst(iChannelNb, pRawData_tdcChannel->getBytes(), iChannelLength, pChunk_tdc);
		}

		xdio_extract_data(pChunk_tdc);
		
		// extracting same values for each event - problem before here

		// parse its data	
		if(!pRawData_tdc->parse())
		{
			cout << "raw data controller error: could not parse raw data!" << endl;
			delete pRawData_tdc;
			return 0;
		}
	}

	// parse its data	
	if(!pRawData->parse())
	{
		cout << "raw data controller error: could not parse raw data!" << endl;
		delete pRawData;
		return 0;
	}

	if (iNbChunks == 1)
	{
		a_pRawData[0] = pRawData;
		a_pRawData[1] = 0;
		return 1;
	}
	else if (iNbChunks == 2)
	{
		a_pRawData[0] = pRawData;
		a_pRawData[1] = pRawData_tdc;
		return 2;
	}
	else return 0;
}

// raw data for tdc
RawData *
RawDataController::getEventRawData_tdc(int iEventIndex)
{
	printf("\nInside geteventrawdata_tdc\n");
	RawData *pRawData = new RawData();

	// get the event from the xdio file
	xdio_event_t *pEvent;
	if((pEvent = xdio_get_event(iEventIndex, m_pDataFile)) == NULL)
	{
		cout << "raw data controller error: could not get event at index " << iEventIndex << endl;
		delete pRawData;
		return 0;
	}
	printf("Passed event creation\n");

	// set the event time and event number
	pRawData->setTimeSec(xdio_get_trigger_time_sec(pEvent));
	pRawData->setTimeMicroSec(xdio_get_trigger_time_usec(pEvent));
	pRawData->setEventNb(xdio_get_event_nb(pEvent));

	// get the number of chunks
	int iNbChunks;
	if((iNbChunks = xdio_get_nb_chunks(pEvent)) < 1)
	{
		cout << "raw data controller error: event has no chunks!" << endl;
		delete pRawData;
		return 0;
	}
	printf("about to load tdc chunk");

	// get the second chunk, should be a raw0 chunk for tdc
	xdio_chunk_t *pChunk = xdio_get_chunk(1, pEvent);

	// get the raw0 attributes
	xdio_chunk_raw0_att_t *pAtt = (xdio_chunk_raw0_att_t *) xdio_get_attributes(pChunk);

	pRawData->setType("raw0");
	pRawData->setSampleType(pAtt->sample_type);
	pRawData->setBytesPerSample(pAtt->bytes_per_sample);
	pRawData->setNbSamples(pAtt->nb_samples);
	pRawData->setVoltageRange(pAtt->voltage_range);
	pRawData->setSamplingFrequency(pAtt->sampling_frequency);
	pRawData->setNbChannels(pAtt->nb_channels);

	for(int iChannelIndex = 0; iChannelIndex < pAtt->nb_channels; iChannelIndex++)
	{
		int iChannelNb = iChannelIndex;
		int iChannelLength = pAtt->bytes_per_sample*pAtt->nb_samples;

		RawDataChannel *pRawDataChannel = new RawDataChannel();
		pRawDataChannel->allocateBytes(iChannelLength);

		pRawData->setChannel(iChannelNb, pRawDataChannel);

		xdio_attach_data_dst(iChannelNb, pRawDataChannel->getBytes(), iChannelLength, pChunk);
	}

	xdio_extract_data(pChunk);

	// parse its data	
	if(!pRawData->parse())
	{
		cout << "raw data controller error: could not parse raw data!" << endl;
		delete pRawData;
		return 0;
	}

	return pRawData;
}




