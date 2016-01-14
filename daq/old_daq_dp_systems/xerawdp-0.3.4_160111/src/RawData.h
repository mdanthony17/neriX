#ifndef __RAWDATA_H__
#define __RAWDATA_H__

#include "xerawdp.h"

#include <stdint.h>

#include <vector>
#include <string>
#include <map>
#include <list>
#include <iostream>

class Pulse;
class Waveform;

class RawDataChannel
{
public:
	RawDataChannel();
	~RawDataChannel();

public:
	void allocateBytes(int iSize);
	void setBytesPerSample(int iBytesPerSample) { m_iBytesPerSample = iBytesPerSample; }
	void setNbSamples(int iNbSamples) { m_iNbSamples = iNbSamples; }

	int8_t *getBytes() const { return m_pBytes; }
	int getSize() const { return m_iSize; }
	int getBytesPerSample() const { return m_iBytesPerSample; }
	int getNbSamples() const { return m_iNbSamples; }

	const std::vector<int> &getSamplesPositions() const { return m_hSamplesPositions; }
	const std::vector<int> &getOffsets() const { return m_hOffsets; }
	const std::vector<int> &getLengths() const { return m_hLengths; }

	const std::vector<float> &getBaselineAvg() const { return m_hBaselineAvg; }
	const std::vector<float> &getBaselineMax() const { return m_hBaselineMax; }

	std::list<Pulse *> *getPulses(float fFactor) const;

	bool parseBytesAsRaw();
	bool parseBytesAsZeroLengthEncoded();

	bool computeBaseline(int iNbBaselineSamples);

private:

private:
	int8_t *m_pBytes;      // array of bytes containing the samples
	int m_iSize;           // size of the array of bytes
	int m_iBytesPerSample; // bytes per sample
	int m_iNbSamples;      // number of samples the channel should have

	std::vector<int> m_hSamplesPositions; // position of the samples in the byte array
	std::vector<int> m_hOffsets;          // offset in the original trace
	std::vector<int> m_hLengths;          // number of samples

	std::vector<float> m_hBaselineAvg;
	std::vector<float> m_hBaselineMax;

	bool m_bParsed;
	bool m_bBaselinesComputed;
	int m_iNbBaselineSamples;
};

std::ostream& operator<<(std::ostream& hOut, const RawDataChannel &hRawDataChannel);

class RawData
{
public:
	RawData();
	~RawData();

public:
	void setTimeSec(int iTimeSec) { m_iTimeSec = iTimeSec; }
	void setTimeMicroSec(int iTimeMicroSec) { m_iTimeMicroSec = iTimeMicroSec; }
	void setEventNb(int iEventNb) { m_iEventNb = iEventNb; }

	void setType(const std::string &hType) { m_hType = hType; }

	void setSampleType(int iSampleType) { m_iSampleType = iSampleType; }
	void setBytesPerSample(int iBytesPerSample) { m_iBytesPerSample = iBytesPerSample; }
	void setNbSamples(int iNbSamples) { m_iNbSamples = iNbSamples; }
	void setVoltageRange(float fVoltageRange) { m_fVoltageRange = fVoltageRange; }
	void setSamplingFrequency(float fSamplingFrequency) { m_fSamplingFrequency = fSamplingFrequency; }
	void setNbChannels(int iNbChannels) { m_iNbChannels = iNbChannels; }

	void setChannel(int iChannelNb, RawDataChannel *pChannel);

	int getTimeSec() const { return m_iTimeSec; }
	int getTimeMicroSec() const { return m_iTimeMicroSec; }
	int getEventNb() const { return m_iEventNb; }

	const std::string &getType() const { return m_hType; }

	int getSampleType() const { return m_iSampleType; }
	int getBytesPerSample() const { return m_iBytesPerSample; }
	int getNbSamples() const { return m_iNbSamples; }
	float getVoltageRange() const { return m_fVoltageRange; }
	float getSamplingFrequency() const { return m_fSamplingFrequency; }
	int getNbChannels() const { return m_iNbChannels; }

	std::map<int, RawDataChannel *> const *getChannels() const { return &m_hChannels; }

	std::map<int, Waveform *> *getWaveforms() const;

	bool parse();

	bool computeBaselines(int iBaselineSamples);

private:

private:
	// xdio event layer
	int m_iTimeSec;
	int m_iTimeMicroSec;
	int m_iEventNb;

	// xdio chunk layer
	std::string m_hType;

	int m_iSampleType;
	int m_iBytesPerSample;
	int m_iNbSamples;
	float m_fVoltageRange;
	float m_fSamplingFrequency;
	int m_iNbChannels;

	std::map<int, RawDataChannel *> m_hChannels;
};

std::ostream& operator<<(std::ostream& hOut, const RawData &hRawData);

#endif // __RAWDATA_H__

