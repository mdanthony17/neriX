#include "RawDataController.h"

#include "RawData.h"

#include <iostream>

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

RawData *
RawDataController::getEventRawData(int iEventIndex)
{
	RawData *pRawData = new RawData();

	// get the event from the xdio file
	xdio_event_t *pEvent;
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

	// get the first chunk, should be a zle0 or raw0 chunk
	xdio_chunk_t *pChunk = xdio_get_chunk(0, pEvent);

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

	// parse its data	
	if(!pRawData->parse())
	{
		cout << "raw data controller error: could not parse raw data!" << endl;
		delete pRawData;
		return 0;
	}

	return pRawData;
}

