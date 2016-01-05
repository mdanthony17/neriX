#include "RawData.h"

#include "Waveform.h"

#include <iostream>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <cmath>

using std::vector;
using std::map;
using std::list;
using std::ostream;
using std::cout;
using std::endl;
using std::accumulate;
using std::max;

RawDataChannel::RawDataChannel()
{
	m_pBytes = 0;
	m_iSize = 0;
	m_iBytesPerSample = 0;
	m_iNbSamples = 0;

	m_hSamplesPositions = vector<int>();
	m_hOffsets = vector<int>();
	m_hLengths = vector<int>();

	m_hBaselineAvg = vector<float>();
	m_hBaselineMax = vector<float>();

	m_bParsed = false;
	m_bBaselinesComputed = false;
	m_iNbBaselineSamples = 0;
}

RawDataChannel::~RawDataChannel()
{
	if(m_pBytes)
		delete[] m_pBytes;
}

void
RawDataChannel::allocateBytes(int iSize)
{
	if(m_pBytes)
		delete[] m_pBytes;

	m_pBytes = new int8_t[iSize];
	m_iSize = iSize;
}

list<Pulse *> *
RawDataChannel::getPulses(float fFactor) const
{
	if(!m_bBaselinesComputed)
	{
		cout << "raw data error: cannot compute waveforms if baselines are not computed!" << endl;
		return 0;
	}

	list<Pulse *> *pPulses = new list<Pulse *>;

	int iPulseNb = 0;
	int iNextOffset = -1;
	vector<float> *pSamples;
	while(iPulseNb < m_hSamplesPositions.size())
	{
		// if the sequence of samples are not immediately after the previous sequence
		if(iNextOffset != m_hOffsets[iPulseNb])
			pSamples = new vector<float>;

		// get a pointer to the samples
		int32_t *pPtr = (int32_t *) &m_pBytes[m_hSamplesPositions[iPulseNb]];

		// save all samples
		for(int iSampleNb = 0; iSampleNb < m_hLengths[iPulseNb]; iSampleNb += 2)
		{
			int iDoubleSample = *pPtr++;

			pSamples->push_back(fFactor*(m_hBaselineAvg[iPulseNb] - (0xFFFF & iDoubleSample)));
			pSamples->push_back(fFactor*(m_hBaselineAvg[iPulseNb] - (0xFFFF & (iDoubleSample >> 16))));
		}

		// if we didn't append the samples to the previous pulse, we have to insert it in the list 
		if(iNextOffset != m_hOffsets[iPulseNb])
			pPulses->push_back(new Pulse(m_hOffsets[iPulseNb], pSamples));

		// compute the offset immediately after this pulse
		iNextOffset = m_hOffsets[iPulseNb]+m_hLengths[iPulseNb];
		iPulseNb++;
	}

	return pPulses;
}

bool
RawDataChannel::parseBytesAsRaw()
{
	if(!m_bParsed)
	{
		m_hSamplesPositions.push_back(0);
		m_hOffsets.push_back(0);
		m_hLengths.push_back(m_iNbSamples);

		m_bParsed = true;
	}

	return true;
}

bool
RawDataChannel::parseBytesAsZeroLengthEncoded()
{
	if(!m_bParsed)
	{
		int32_t *pPtr = (int32_t *) m_pBytes;

		int iSize = 4*(*pPtr++);

		// verify that the size is indeed the same

		// parse the zle data
		int iOffset = 0;
		while(iOffset < m_iNbSamples)
		{
			int iControlWord = *pPtr++;
			bool bGood = ((1 << 31) & iControlWord);
			int iLength = 4*(0x001FFFFF & iControlWord);

			if(!bGood)
				iOffset += iLength/m_iBytesPerSample;
			else
			{
				m_hSamplesPositions.push_back((int8_t *) pPtr - m_pBytes);
				m_hOffsets.push_back(iOffset);
				m_hLengths.push_back(iLength/m_iBytesPerSample);

				iOffset += iLength/m_iBytesPerSample;
				pPtr += iLength/4;
			}
		}

		m_bParsed = true;
	}

	return true;
}

bool
RawDataChannel::computeBaseline(int iNbBaselineSamples)
{
	bool bRet = true;

	if(!m_bBaselinesComputed || (m_iNbBaselineSamples != iNbBaselineSamples))
	{
		// clear baseline(s) average, rms and maximum
		if(m_bBaselinesComputed)
		{
			m_bBaselinesComputed = false;
			m_hBaselineAvg.clear();
			m_hBaselineMax.clear();
		}

		// for all pulses
		for(int iPulseNb = 0; iPulseNb < m_hSamplesPositions.size(); iPulseNb++)
		{
			// get a pointer to the samples
			int32_t *pPtr = (int32_t *) &m_pBytes[m_hSamplesPositions[iPulseNb]];

			// if this pulse starts right after the previous pulse then no need to compute the baseline
			if((iPulseNb != 0) && (m_hOffsets[iPulseNb] == m_hOffsets[iPulseNb-1]+m_hLengths[iPulseNb-1]))
			{
				m_hBaselineAvg.push_back(m_hBaselineAvg[iPulseNb-1]);
				m_hBaselineMax.push_back(m_hBaselineMax[iPulseNb-1]);

				continue;
			}

			// check that we have enough samples before we went above threshold or that we are not at
			// the end of the trace (where it is possible that the pulse is short than npre+npos)
			if((m_hLengths[iPulseNb] < 2*iNbBaselineSamples)
				&& (m_hOffsets[iPulseNb]+m_hLengths[iPulseNb] < m_iNbSamples))
			{
				// we don't have enough samples to compute the baseline, this should
				// really only happen at the beginning of the trace

				// if the pulse is at the beginning
				if(iPulseNb == 0)
				{
					// set the samples pointer to compute the baseline from the last samples
					int iPositionDelta = m_iBytesPerSample*(m_hLengths[iPulseNb]-iNbBaselineSamples);
					pPtr = (int32_t *) &m_pBytes[m_hSamplesPositions[iPulseNb]+iPositionDelta];
				}
				else
				{
					// there is something really wrong
					cout << "raw data error: not enough samples to compute baseline!" << endl;
					bRet = false;
					break;
				}
			}

			// for all samples
			vector<float> hSamples;
			for(int iSampleNb = 0; iSampleNb < iNbBaselineSamples; iSampleNb += 2)
			{
				int iDoubleSample = *pPtr++;

				hSamples.push_back(0xFFFF & iDoubleSample);
				hSamples.push_back(0xFFFF & (iDoubleSample >> 16));
			}

			// get mean and max
			float fAvg = accumulate(hSamples.begin(), hSamples.end(), 0.)/iNbBaselineSamples;
			float fHigh = *(max_element(hSamples.begin(), hSamples.end()));
			float fLow = *(min_element(hSamples.begin(), hSamples.end()));
			float fMax = max(fabs(fHigh-fAvg), fabs(fLow-fAvg));

			// store the results
			m_hBaselineAvg.push_back(fAvg);
			m_hBaselineMax.push_back(fMax);
		}

		if(bRet)
		{
			m_bBaselinesComputed = true;
			m_iNbBaselineSamples = iNbBaselineSamples;
		}
	}

	return bRet;
}

ostream &
operator<<(ostream &hOut, const RawDataChannel &hRawDataChannel)
{
	hOut << "RawDataChannel: ";
	hOut << "Size: " << hRawDataChannel.getSize() << " ";
	hOut << "BytesPerSample: " << hRawDataChannel.getBytesPerSample();

	if(hRawDataChannel.getOffsets().size())
	{
		hOut << " Samples:";
		vector<int> hOffsets = hRawDataChannel.getOffsets();
		vector<int> hLengths = hRawDataChannel.getLengths();
		for(int i = 0; i < hOffsets.size(); i++)
			cout << " [" << hOffsets[i] << ", " << hLengths[i] << "]";
	}

	if(hRawDataChannel.getBaselineAvg().size())
	{
		hOut << " Baselines:";
		vector<float> hMean = hRawDataChannel.getBaselineAvg();
		vector<float> hMax = hRawDataChannel.getBaselineMax();
		for(int i = 0; i < hMean.size(); i++)
			cout << std::fixed << std::setprecision(1) << " [" << hMean[i] << "\xb1" << hMax[i] << "]";
	}

	return hOut;
}

RawData::RawData()
{
	m_iTimeSec = -1;
	m_iTimeMicroSec = -1;
	m_iEventNb = -1;

	m_hType = "";

	m_iSampleType = 0;
	m_iBytesPerSample = 0;
	m_iNbSamples = 0;
	m_fVoltageRange = 0.;
	m_fSamplingFrequency = 0.;
	m_iNbChannels = 0;

	m_hChannels = map<int, RawDataChannel *>();
}

RawData::~RawData()
{
	// delete all channels
	map<int, RawDataChannel *>::iterator pIter;
	for(pIter = m_hChannels.begin(); pIter != m_hChannels.end(); pIter++)
		delete pIter->second;
}

void
RawData::setChannel(int iChannelNb, RawDataChannel *pChannel)
{
	pChannel->setBytesPerSample(this->getBytesPerSample());
	pChannel->setNbSamples(this->getNbSamples());
	m_hChannels[iChannelNb] = pChannel;
}

map<int, Waveform *> *
RawData::getWaveforms() const
{
	map<int, Waveform *> *pWaveforms = new map<int, Waveform *>;

	float fFactor = getVoltageRange()/(1 << getSampleType());

	std::map<int, RawDataChannel *>::const_iterator ppRawDataChannel;
	for(ppRawDataChannel = m_hChannels.begin(); ppRawDataChannel != m_hChannels.end(); ppRawDataChannel++)
	{
		Waveform *pWaveform = new Waveform(m_fSamplingFrequency, m_iNbSamples, ppRawDataChannel->second->getPulses(fFactor));
		(*pWaveforms)[ppRawDataChannel->first] = pWaveform;
	}

	return pWaveforms;
}

bool
RawData::parse()
{
	bool bRet = true;

	map<int, RawDataChannel *>::iterator pIter;
	if(m_hType == "raw0")
	{
		for(pIter = m_hChannels.begin(); pIter != m_hChannels.end(); pIter++)
			bRet = bRet && pIter->second->parseBytesAsRaw();
	}
	else if(m_hType == "zle0")
	{
		for(pIter = m_hChannels.begin(); pIter != m_hChannels.end(); pIter++)
			bRet = bRet && pIter->second->parseBytesAsZeroLengthEncoded();
	}
	else
	{
		cout << "raw data error: raw data is in an unknown format for parsing!" << endl;
		bRet = false;
	}

	return bRet;
}

bool
RawData::computeBaselines(int iNbBaselineSamples)
{
	bool bRet = true;

	// computes baselines for all channels
	map<int, RawDataChannel *>::iterator pIter;
	for(pIter = m_hChannels.begin(); pIter != m_hChannels.end(); pIter++)
	{
//        cout << "Channel " << pIter->first << endl;
		if(!(bRet = bRet && pIter->second->computeBaseline(iNbBaselineSamples)))
			break;
	}

	return bRet;
}

ostream &
operator<<(ostream &hOut, const RawData &hRawData)
{
	hOut << "RawData:";
	hOut << " TimeSec: " << hRawData.getTimeSec();
	hOut << " TimeMicroSec: " << hRawData.getTimeMicroSec();
	hOut << " EventNb: " << hRawData.getEventNb();
	hOut << " Type: " << hRawData.getType();
	hOut << " SampleType: " << hRawData.getSampleType();
	hOut << " BytesPerSample: " << hRawData.getBytesPerSample();
	hOut << " NbSamples: " << hRawData.getNbSamples();
	hOut << " SampleType: " << hRawData.getSampleType();
	hOut << " VoltageRange: " << hRawData.getVoltageRange();
	hOut << " SamplingFrequency: " << hRawData.getSamplingFrequency();
	hOut << " NbChannels: " << hRawData.getNbChannels();

	hOut << " Channels:";
	map<int, RawDataChannel *> const *pChannels = hRawData.getChannels();
	map<int, RawDataChannel *>::const_iterator pIter;
	for(pIter = pChannels->begin(); pIter != pChannels->end(); pIter++)
		hOut << " Channel " << pIter->first << ": " << *(pIter->second);

	return hOut;
}

