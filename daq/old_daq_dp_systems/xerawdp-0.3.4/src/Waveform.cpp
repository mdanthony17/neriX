#include "Waveform.h"

#include <cmath>
#include <cassert>
#include <algorithm>
#include <numeric>

using std::vector;
using std::list;
using std::min;
using std::max;
using std::inner_product;
using std::ostream;

Pulse::Pulse()
{
	m_iOffset = -1;
	m_pSamples = 0;
}

Pulse::Pulse(const Pulse &hPulse)
{
	m_iOffset = hPulse.m_iOffset;
	m_pSamples = new vector<float>(*(hPulse.m_pSamples));
}

Pulse::Pulse(int iOffset, vector<float> *pSamples)
{
	m_iOffset = iOffset;
	m_pSamples = pSamples;
}

Pulse::~Pulse()
{
	if(m_pSamples)
		delete m_pSamples;
}

void
Pulse::prepend(float fSample)
{
	m_iOffset -= 1;
	m_pSamples->insert(m_pSamples->begin(), fSample);
}

void
Pulse::append(float fSample)
{
	m_pSamples->push_back(fSample);
}

bool
Pulse::overlaps(const Pulse &hPulse)
{
	bool bRet;

	if(hPulse.m_iOffset > m_iOffset)
		bRet = hPulse.m_iOffset <= m_iOffset+m_pSamples->size();
	else
		bRet = m_iOffset <= hPulse.m_iOffset+hPulse.m_pSamples->size();

	return bRet;
}

void
Pulse::convolve(const Filter &hFilter)
{
	const vector<float> hCoefficients = hFilter.getCoefficients();
	int iLength = hFilter.getLength();

	if(m_pSamples->size() < iLength)
		return;

	vector<float> *pNewSamples = new vector<float>;
	int iNewOffset = m_iOffset+(iLength-1)/2;

	vector<float>::iterator pSample;
	for(pSample = m_pSamples->begin(); pSample < m_pSamples->end()-iLength; pSample++)
		pNewSamples->push_back(inner_product(hCoefficients.begin(), hCoefficients.end(), pSample, 0.));

	delete m_pSamples;

	m_iOffset = iNewOffset;
	m_pSamples = pNewSamples;
}

void
Pulse::flatten(float fThreshold)
{
	vector<float>::iterator pSample;
	for(pSample = m_pSamples->begin(); pSample < m_pSamples->end(); pSample++)
		if(*pSample < fThreshold)
			*pSample = 0.;
}

Pulse &
Pulse::operator=(const Pulse &hPulse)
{
	if(m_pSamples)
		delete m_pSamples;

	m_iOffset = hPulse.m_iOffset;
	m_pSamples = new vector<float>(*hPulse.m_pSamples);

	return *this;
}

Pulse &
Pulse::operator+=(const Pulse &hPulse)
{
	assert(this->overlaps(hPulse));

	const int iLeftMostOffset = min(m_iOffset, hPulse.m_iOffset);
	const int iRightMostOffset = max(m_iOffset, hPulse.m_iOffset);
	const int iBefore = iRightMostOffset-iLeftMostOffset;

	if(iLeftMostOffset == m_iOffset)
	{
		const int iOverlap = min(m_pSamples->size()-iBefore, hPulse.m_pSamples->size());
		const int iAfter = (int)(iRightMostOffset+hPulse.m_pSamples->size()) - (int)(iLeftMostOffset+m_pSamples->size());

		for(int i = 0; i < iOverlap; i++)
			(*m_pSamples)[iBefore+i] += (*hPulse.m_pSamples)[i];

		if(iAfter > 0)
			for(int i = 0; i < iAfter; i++)
				m_pSamples->push_back((*hPulse.m_pSamples)[iOverlap+i]);
	}
	else
	{
		m_iOffset = iLeftMostOffset;

		const int iOverlap = min(hPulse.m_pSamples->size()-iBefore, m_pSamples->size());
		const int iAfter = (int)(iRightMostOffset+m_pSamples->size()) - (int)(iLeftMostOffset+hPulse.m_pSamples->size());

		m_pSamples->insert(m_pSamples->begin(), hPulse.m_pSamples->begin(), hPulse.m_pSamples->begin()+iBefore);

		for(int i = 0; i < iOverlap; i++)
			(*m_pSamples)[iBefore+i] += (*hPulse.m_pSamples)[iBefore+i];

		if(iAfter < 0)
			for(int i = 0; i < -iAfter; i++)
				m_pSamples->push_back((*hPulse.m_pSamples)[iBefore+iOverlap+i]);
	}

	return *this;
}

bool
Pulse::operator<(const Pulse &hPulse) const
{
	return (m_iOffset+m_pSamples->size() < hPulse.m_iOffset);
}

float
Pulse::operator[](const unsigned int iOffset) const
{
	assert((iOffset >= m_iOffset) && (iOffset < m_iOffset+m_pSamples->size()));

	return (*m_pSamples)[iOffset-m_iOffset];
}

float &
Pulse::operator[](const unsigned int iOffset)
{
	assert((iOffset >= m_iOffset) && (iOffset < m_iOffset+m_pSamples->size()));

	return (*m_pSamples)[iOffset-m_iOffset];
}

ostream &
operator<<(ostream& hOut, const Pulse &hPulse)
{
	hOut << "Pulse: [" << hPulse.getOffset() << ":" << hPulse.getLength() << ", [";
	vector<float> const *pSamples = hPulse.getSamples();
	vector<float>::const_iterator pSample;
	for(pSample = pSamples->begin(); pSample != pSamples->end()-1; pSample++)
		hOut << *pSample << " ";
	hOut << *(pSamples->end()-1) << "]]";

	return hOut;
}

Waveform::Waveform()
{
	m_fSamplingFrequency = 0;
	m_iNbSamples = 0;
	m_pPulses = 0;
}

Waveform::Waveform(const Waveform &hWaveform)
{
	m_fSamplingFrequency = hWaveform.m_fSamplingFrequency;
	m_iNbSamples = hWaveform.m_iNbSamples;
	m_pPulses = new list<Pulse *>;
	list<Pulse *>::const_iterator ppPulse;
	for(ppPulse = hWaveform.m_pPulses->begin(); ppPulse !=  hWaveform.m_pPulses->end(); ppPulse++)
		m_pPulses->push_back(new Pulse(**ppPulse));
}

Waveform::Waveform(float fSamplingFrequency, int iNbSamples, std::list<Pulse *> *pPulses)
{
	m_fSamplingFrequency = fSamplingFrequency;
	m_iNbSamples = iNbSamples;
	m_pPulses = pPulses;
}

Waveform::~Waveform()
{
	if(m_pPulses)
	{
		list<Pulse *>::const_iterator ppPulse;
		for(ppPulse = m_pPulses->begin(); ppPulse !=  m_pPulses->end(); ppPulse++)
			delete *ppPulse;

		delete m_pPulses;
	}
}

float
Waveform::integrate(int iLeft, int iRight) const
{
	float fIntegral = 0.;

	for(int i = iLeft; i < iRight; i++)
		fIntegral += (*this)[i];

	return(fIntegral/m_fSamplingFrequency);
}

float
Waveform::integrate(int iLeft, int iRight, float (*pFunction)(float)) const
{
	float fIntegral = 0.;

	for(int i = iLeft; i < iRight; i++)
		fIntegral += (*pFunction)((*this)[i]);

	return(fIntegral/m_fSamplingFrequency);
}

float
Waveform::average(int iPosition, int iWindow) const
{
	const int iHalfWindow = iWindow/2;
	float fAverage = 0.;

	for(int i = 0; i < iWindow; i++)
		fAverage += (*this)[iPosition-iHalfWindow+i];

	return(fAverage/iWindow);	
}

float
Waveform::median(int iStart, int iEnd) const
{
	const int iWindow = iEnd - iStart;
	vector<int> vSamples;
	vSamples.resize(iWindow*2);

	for(int i = 0; i < iWindow; i++)
		vSamples[i] = (*this)[iStart+i];
	
	sort(vSamples.begin(), vSamples.end());

	return vSamples[(int)(vSamples.size()/2)];
}

/*! @brief Compute the slope of a waveform at a given position
 *
 * This function computes an approximation to the slope at a given position using a 9-tap derivative filter
 *
 * @param
 * @return
 */
float
Waveform::slope(int iPosition) const
{
	float fSlope = 0.;

	const int iLength = 9;
	const int iHalfLength = iLength/2;
	const float pfKernel[] = {-0.003059, -0.035187, -0.118739, -0.143928, 0.000000, 0.143928, 0.118739, 0.035187, 0.003059};

	for(int i = 0; i < iLength; i++)
		fSlope += pfKernel[i]*(*this)[iPosition-iHalfLength+i];

	return(fSlope);
}

float
Waveform::minimum(int iLeft, int iRight) const
{
	float fMinimumValue = HUGE_VALF;

	for(int i = iLeft; i < iRight; i++)
		if((*this)[i] < fMinimumValue)
			fMinimumValue = (*this)[i];

	return fMinimumValue;	
}

float
Waveform::maximum(int iLeft, int iRight) const
{
	float fMaximumValue = -HUGE_VALF;

	for(int i = iLeft; i < iRight; i++)
		if((*this)[i] > fMaximumValue)
			fMaximumValue = (*this)[i];

	return fMaximumValue;	
}

float
Waveform::beforemaximum(int iLeft, int iRight) const
{
        float fMaximumValue = -HUGE_VALF;
	float fBeforeMaximum;
	for(int i = iLeft; i < iRight; i++)
		if((*this)[i] > fMaximumValue){
			fMaximumValue = (*this)[i];
			fBeforeMaximum = (*this) [i-1];											  }													
	return fBeforeMaximum;
}


float
Waveform::aftermaximum(int iLeft, int iRight) const
{
        float fMaximumValue = -HUGE_VALF;
	float fAfterMaximum;
	for(int i = iLeft; i < iRight; i++)
		if((*this)[i] > fMaximumValue){
			fMaximumValue = (*this)[i];
			fAfterMaximum = (*this) [i+1];
		}

        return fAfterMaximum;
		
}

float
Waveform::entropy(int iLeft, int iRight) const
{
        vector<float> fSquare;
	float fSum =0.;
	float fEntropy = 0.;
	float fNorm = 0.;
	for (int i = iLeft; i < iRight; i++)
		fSquare.push_back((*this)[i]*(*this)[i]);
	fSum = accumulate(fSquare.begin(),fSquare.end(),0.);

	for (vector<float>::const_iterator it = fSquare.begin(); it != fSquare.end() ; it++) {
		fNorm =  (*it)/fSum ;
		if ( fNorm ==0.)
			fEntropy += 0.;
		else
		        fEntropy += fNorm*log(fNorm);
	}
        
	return -fEntropy;

}							

int
Waveform::minimumPosition(int iLeft, int iRight) const
{
	float fMinimumValue = HUGE_VALF;
	int iMinimumPosition = iLeft;

	for(int i = iLeft; i < iRight; i++)
	{
		if((*this)[i] < fMinimumValue)
		{
			fMinimumValue = (*this)[i];
			iMinimumPosition = i;
		}
	}

	return iMinimumPosition;	
}

int
Waveform::maximumPosition(int iLeft, int iRight) const
{
	float fMaximumValue = -HUGE_VALF;
	int iMaximumPosition = iLeft;

	for(int i = iLeft; i < iRight; i++)
	{
		if((*this)[i] > fMaximumValue)
		{
			fMaximumValue = (*this)[i];
			iMaximumPosition = i;
		}
	}

	return iMaximumPosition;	
}

int
Waveform::width(int iPeak, int iLeftMax, int iRightMax, float fThreshold) const
{
	float fThresholdHeight = fThreshold*(*this)[iPeak];
	int iLeft, iRight;

	iLeft = iRight = iPeak;

	while((iLeft > iLeftMax) && ((*this)[iLeft] > fThresholdHeight)) iLeft--;
	while((iRight < iRightMax) && ((*this)[iRight] > fThresholdHeight)) iRight++;

	return(iRight-iLeft);
}

float
Waveform::interpolatedWidth(int iPeak, int iLeftMax, int iRightMax, float fThreshold) const
{
	float fThresholdHeight = fThreshold*(*this)[iPeak];
	int iLeft, iRight;

	iLeft = iRight = iPeak;

	while((iLeft > iLeftMax) && ((*this)[iLeft] > fThresholdHeight)) iLeft--;
	while((iRight < iRightMax) && ((*this)[iRight] > fThresholdHeight)) iRight++;

	float fLeft = iLeft, fRight = iRight;

	if(iLeft > iLeftMax)
		fLeft = iLeft+(fThresholdHeight-(*this)[iLeft])/((*this)[iPeak]-(*this)[iLeft])*(iPeak-iLeft);

	if(iRight < iRightMax)
		fRight = iRight-(fThresholdHeight-(*this)[iRight])/((*this)[iPeak]-(*this)[iRight])*(iRight-iPeak);

	return(fRight-fLeft);
}

float
Waveform::interpolatedRiseTime(int iPeak, int iLeftMax, float fThreshold) const
{
	float fThresholdHeight = fThreshold*(*this)[iPeak];
	int iLeft;

	iLeft = iPeak;

	while((iLeft > iLeftMax) && ((*this)[iLeft] > fThresholdHeight)) iLeft--;

	float fLeft = iLeft;

	if(iLeft > iLeftMax)
		fLeft = iLeft+(fThresholdHeight-(*this)[iLeft])/((*this)[iPeak]-(*this)[iLeft])*(iPeak-iLeft);

	return(iPeak-fLeft);
}

float
Waveform::meanTime(int iLeft, int iRight) const
{
	float fSum = 0.;
	float fMeanTime = 0.;

	for(int i = iLeft; i < iRight; i++)
	{
		fMeanTime += i*(*this)[i];
		fSum += (*this)[i];
	}

	return fMeanTime/fSum;
}

float
Waveform::timeSpread(int iLeft, int iRight) const
{
	float fMeanTime = meanTime(iLeft, iRight);
	float fTimeSpread = 0;
	float fSum = 0;

	for(int i = iLeft; i < iRight; i++)
	{
		fTimeSpread += (i-fMeanTime)*(i-fMeanTime)*(*this)[i];
		fSum += (*this)[i];
	}

	return sqrtf(fabs(fTimeSpread/fSum));
}

void
Waveform::convolve(const Filter &hFilter)
{
	list<Pulse *>::iterator ppPulse = m_pPulses->begin();

	while(ppPulse != m_pPulses->end())
		(*ppPulse++)->convolve(hFilter);
}

void
Waveform::flatten(float fThreshold)
{
	list<Pulse *>::iterator ppPulse = m_pPulses->begin();

	while(ppPulse != m_pPulses->end())
		(*ppPulse++)->flatten(fThreshold);
}

Waveform &
Waveform::operator+=(const Waveform &hWaveform)
{
	assert(m_fSamplingFrequency == hWaveform.m_fSamplingFrequency);
	assert(m_iNbSamples == hWaveform.m_iNbSamples);

	// get iterators over the pulses
	list<Pulse *>::iterator ppPulse = m_pPulses->begin();
	list<Pulse *>::iterator ppSummandPulse = hWaveform.m_pPulses->begin();

	while((ppPulse != m_pPulses->end()) && (ppSummandPulse != hWaveform.m_pPulses->end()))
	{
		if(**ppPulse < **ppSummandPulse)
		{
			// the pulse is entirely before the summand pulse, leave it there
			ppPulse++;
		}
		else if(**ppSummandPulse < **ppPulse)
		{
			// the summand pulse is entirely before the pulse, insert it
			m_pPulses->insert(ppPulse, new Pulse(**ppSummandPulse));
			ppSummandPulse++;
		}
		else
		{
			// ok, the pulses are overlapping add them up until it's no longer possible
			Pulse *pPulseSum = *ppPulse++;

			while(1)
			{
				if((ppSummandPulse != hWaveform.m_pPulses->end()) && (pPulseSum->overlaps(**ppSummandPulse)))
				{
					*pPulseSum += **ppSummandPulse;
					ppSummandPulse++;
				}
				else if((ppPulse != m_pPulses->end()) && (pPulseSum->overlaps(**ppPulse)))
				{
					*pPulseSum += **ppPulse;
					delete *ppPulse;
					m_pPulses->erase(ppPulse++);
				}
				else
					break;
			}
		}
	}

	// copy any remaining summand pulses
	while(ppSummandPulse != hWaveform.m_pPulses->end())
	{
		m_pPulses->push_back(new Pulse(**ppSummandPulse));
		ppSummandPulse++;
	}

	return *this;
}

float
Waveform::operator[](const unsigned int iPosition) const
{
	float fSample = 0.;

	list<Pulse *>::const_iterator ppPulse = m_pPulses->begin();
	while((fSample == 0.) && (ppPulse != m_pPulses->end()))
	{
		if((iPosition >= (*ppPulse)->getOffset()) && (iPosition < (*ppPulse)->getOffset()+(*ppPulse)->getLength()))
			fSample = (**ppPulse)[iPosition];
		ppPulse++;
	}

	return fSample;
}

float &
Waveform::operator[](const unsigned int iPosition)
{
	// if there is a pulse which includes the position, return the sample (by reference)
	list<Pulse *>::iterator ppPulse = m_pPulses->begin();
	while(ppPulse != m_pPulses->end())
	{
		if((iPosition >= (*ppPulse)->getOffset()) && (iPosition < (*ppPulse)->getOffset()+(*ppPulse)->getLength()))
			return (**ppPulse)[iPosition];

		if(iPosition < (*ppPulse)->getOffset())
			break;

		ppPulse++;
	}

	// if we are here it's because no pulses included that position
	list<Pulse *>::iterator ppRightPulse = ppPulse--;
	list<Pulse *>::iterator ppLeftPulse = ppPulse;
	
	// if we are at the end (most common case if we are adding samples from left to right one by one)
	if(ppRightPulse == m_pPulses->end())
	{
		// if the position is one sample after the last pulse, extend it
		if((!m_pPulses->empty()) && (iPosition == (*ppLeftPulse)->getOffset()+(*ppLeftPulse)->getLength()))
		{
			(*ppLeftPulse)->append(0.);
			return (**ppLeftPulse)[iPosition];
		}
		// otherwise, add a new pulse
		else
		{
			Pulse *pNewPulse = new Pulse(iPosition, new vector<float>(1, 0.));
			m_pPulses->push_back(pNewPulse);
			return (*pNewPulse)[iPosition];
		}
	}

	// if the position is before the first pulse
	if(ppRightPulse == m_pPulses->begin())
	{
		// if the position is one sample before the first pulse, prepend it
		if(iPosition == (*ppRightPulse)->getOffset()-1)
		{
			(*ppRightPulse)->prepend(0.);
			return (**ppRightPulse)[iPosition];
		}
		// otherwise, add a new pulse
		else
		{
			Pulse *pNewPulse = new Pulse(iPosition, new vector<float>(1, 0.));
			m_pPulses->insert(ppRightPulse, pNewPulse);
			return (*pNewPulse)[iPosition];
		}
	}

	// we are in between two pulses
	
	// if the position is immediately after the left pulse, append it
	if(iPosition == ((*ppLeftPulse)->getOffset()+(*ppLeftPulse)->getLength()))
	{
		(*ppLeftPulse)->append(0.);
		return (**ppLeftPulse)[iPosition];
	}
	// else if the position is immediately before the right pulse, prepend it
	else if(iPosition == (*ppRightPulse)->getOffset()-1)
	{
		(*ppRightPulse)->prepend(0.);
		return (**ppRightPulse)[iPosition];
	}
	// otherwise add a new pulse and insert it
	else
	{
		Pulse *pNewPulse = new Pulse(iPosition, new vector<float>(1, 0.));
		m_pPulses->insert(ppRightPulse, pNewPulse);

		return (*pNewPulse)[iPosition];
	}
}

ostream &
operator<<(ostream& hOut, const Waveform &hWaveform)
{
	hOut << "Waveform:";
	hOut << " SamplingFrequency: " << hWaveform.getSamplingFrequency();
	hOut << " NbSamples: " << hWaveform.getNbSamples();
	list<Pulse *> const *pPulses = hWaveform.getPulses();
	list<Pulse *>::const_iterator ppPulse;
	for(ppPulse = pPulses->begin(); ppPulse != pPulses->end(); ppPulse++)
		hOut << " " << **ppPulse;

	return hOut;
}

