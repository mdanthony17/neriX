#ifndef __WAVEFORM_H__
#define __WAVEFORM_H__

#include "xerawdp.h"

#include "Filter.h"

#include <vector>
#include <list>
#include <iostream>

class Pulse
{
public:
	Pulse();
	Pulse(const Pulse &hPulse);
	Pulse(int iOffset, std::vector<float> *pSamples);
	~Pulse();

public:
	void setOffset(int iOffset) { m_iOffset = iOffset; }
	void setSamples(std::vector<float> *pSamples) { m_pSamples = pSamples; }

	inline int getOffset() const { return m_iOffset; }
	inline std::vector<float> const *getSamples() const { return m_pSamples; }
	inline int getLength() const { return m_pSamples->size(); }

	void prepend(float fSample);
	void append(float fSample);

	bool overlaps(const Pulse &hPulse);

	void convolve(const Filter &hFilter);
	void flatten(float fThreshold);

	Pulse &operator=(const Pulse &hPulse);
	Pulse &operator+=(const Pulse &hPulse);
	bool operator<(const Pulse &hPulse) const;
	float operator[](const unsigned int iOffset) const;
	float &operator[](const unsigned int iOffset);

private:
	std::vector<float> *m_pSamples;
	int m_iOffset;
};

std::ostream& operator<<(std::ostream& hOut, const Pulse &hPulse);

class Waveform
{
public:
	Waveform();
	Waveform(const Waveform &hWaveform);
	Waveform(float fSamplingFrequency, int iNbSamples, std::list<Pulse *> *pPulses);
	~Waveform();

public:
	void setSamplingFrequency(float fSamplingFrequency) { m_fSamplingFrequency = fSamplingFrequency; }
	void setNbSamples(int iNbSamples) { m_iNbSamples = iNbSamples; }
	void setPulses(std::list<Pulse *> *pPulses) { m_pPulses = pPulses; }

	inline float getSamplingFrequency() const { return m_fSamplingFrequency; }
	inline int getNbSamples() const { return m_iNbSamples; }
	inline std::list<Pulse *> const *getPulses() const { return m_pPulses; }

	float integrate(int iLeft, int iRight) const;
	float integrate(int iLeft, int iRight, float (*pFunction)(float)) const;
	float average(int iPosition, int iWindow) const;
	float median(int iStart, int iEnd) const;
	float slope(int iPosition) const;
	float minimum(int iLeft, int iRight) const;
	float maximum(int iLeft, int iRight) const;
	float beforemaximum(int iLeft, int iRight) const;
	float aftermaximum(int iLeft, int iRight) const;
	float entropy(int iLeft, int iRight) const;
	int minimumPosition(int iLeft, int iRight) const;
	int maximumPosition(int iLeft, int iRight) const;
	int width(int iPeak, int iLeftMax, int iRightMax, float fThreshold) const;
	float interpolatedWidth(int iPeak, int iLeftMax, int iRightMax, float fThreshold) const;
	float interpolatedRiseTime(int iPeak, int iLeftMax, float fThreshold) const;
	float meanTime(int iLeft, int iRight) const;
	float timeSpread(int iLeft, int iRight) const;

	void convolve(const Filter &hFilter);
	void flatten(float fThreshold);

	Waveform &operator+=(const Waveform &hWaveform);
	float operator[](const unsigned int iPosition) const;
	float &operator[](const unsigned int iPosition);

private:

private:
	float m_fSamplingFrequency;
	int m_iNbSamples;
	std::list<Pulse *> *m_pPulses;
};

std::ostream& operator<<(std::ostream& hOut, const Waveform &hWaveform);

#endif // __WAVEFORM_H__

