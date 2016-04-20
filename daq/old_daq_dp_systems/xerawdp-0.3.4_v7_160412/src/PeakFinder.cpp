#include "PeakFinder.h"

#include "AnalysisManager.h"

#include <cmath>
#include <stack>
#include <algorithm>
#ifdef ENABLE_PEAK_FINDER_OUTPUT
#include <iostream>
#endif

using std::stack;
using std::vector;
using std::min;
using std::max;
#ifdef ENABLE_PEAK_FINDER_OUTPUT
using std::cout;
using std::endl;
#endif

Peak::Peak()
{
	m_iPosition = -1;
	m_iLeftEdge = -1;
	m_iRightEdge = -1;
	m_fWidth = -1;
	m_fLowWidth = -1;
	m_fFilteredLowWidth = -1;
	m_fIntegral = 0.;

	m_iOrder = -1;
	m_iSizeOrder = -1;
	m_iCoincidences = -1;
}

Peak::Peak(const Peak &hPeak)
{
	m_iPosition = hPeak.m_iPosition;
	m_iLeftEdge = hPeak.m_iLeftEdge;
	m_iRightEdge = hPeak.m_iRightEdge;
	m_fWidth = hPeak.m_fWidth;
	m_fLowWidth = hPeak.m_fLowWidth;
	m_fFilteredLowWidth = hPeak.m_fFilteredLowWidth;
	m_fIntegral = hPeak.m_fIntegral;

	m_iOrder = hPeak.m_iOrder;
	m_iSizeOrder = hPeak.m_iSizeOrder;
	m_iCoincidences = hPeak.m_iCoincidences;
}

Peak::Peak(int iPosition, int iLeftEdge, int iRightEdge, float fWidth, float fLowWidth, float fFilteredLowWidth, float fIntegral)
{
	m_iPosition = iPosition;
	m_iLeftEdge = iLeftEdge;
	m_iRightEdge = iRightEdge;
	m_fWidth = fWidth;
	m_fLowWidth = fLowWidth;
	m_fFilteredLowWidth = fFilteredLowWidth;
	m_fIntegral = fIntegral;

	m_iOrder = -1;
	m_iSizeOrder = -1;
	m_iCoincidences = -1;
}

bool
Peak::operator<(const Peak &hPeak) const
{
	return m_fIntegral < hPeak.m_fIntegral;
}

PeakFinder::PeakFinder(const XmlConfig &hXmlConfig)
{
	// get all peak finding settings from the xml configuration
	m_bS1Filter = hXmlConfig.getS1Filter();

	if(m_bS1Filter)
	{
		m_hS1Filter = Filter();

		if(hXmlConfig.getS1FilterType() == "RaisedCosine")
		{
			bool bLowPass = hXmlConfig.getS1RaisedCosineLowPass();
			float fLimit = hXmlConfig.getS1RaisedCosineLimit();
			float fRollOff = hXmlConfig.getS1RaisedCosineRollOff();
			int iLength = hXmlConfig.getS1RaisedCosineLength();

			m_hS1Filter.buildRaisedCosine(bLowPass, fLimit, fRollOff, iLength);
		}
		else if(hXmlConfig.getS1FilterType() == "Custom")
		{
			m_hS1Filter.buildCustom(hXmlConfig.getS1CustomFilterCoefficients());
		}
	}

	m_iS1MaxNbPeaks = hXmlConfig.getS1MaxNbPeaks();
	m_fS1RightLimitHeightThreshold = hXmlConfig.getS1RightLimitHeightThreshold();
	m_fS1SignalThreshold = hXmlConfig.getS1SignalThreshold();
	m_iS1PeakWindow = hXmlConfig.getS1PeakWindow();
	m_iS1PrePeakSamples = hXmlConfig.getS1PrePeakSamples();
	m_iS1MaxLength = hXmlConfig.getS1MaxLength();
	m_iS1PrePeakAvgWindow = hXmlConfig.getS1PrePeakAvgWindow();
	m_iS1PostPeakAvgWindow = hXmlConfig.getS1PostPeakAvgWindow();
	m_fS1PrePeakAvgThreshold = hXmlConfig.getS1PrePeakAvgThreshold();
	m_fS1PostPeakAvgThreshold = hXmlConfig.getS1PostPeakAvgThreshold();
	m_fS1FilteredWidthThreshold = hXmlConfig.getS1FilteredWidthThreshold();
	m_fS1NegativeExcursionFractionThreshold = hXmlConfig.getS1NegativeExcursionFractionThreshold();
	m_fS1HeightFractionThreshold = hXmlConfig.getS1HeightFractionThreshold();
	m_iS1SamplesBelowThreshold = hXmlConfig.getS1SamplesBelowThreshold();
	m_fS1CoincidenceThreshold = hXmlConfig.getS1CoincidenceThreshold();

	m_iS2MaxNbPeaks = hXmlConfig.getS2MaxNbPeaks();

	m_bS2LargePeakFilter = hXmlConfig.getS2LargePeakFilter();

	if(m_bS2LargePeakFilter)
	{
		m_hS2LargePeakFilter = Filter();

		if(hXmlConfig.getS2LargePeakFilterType() == "RaisedCosine")
		{
			bool bLowPass = hXmlConfig.getS2LargePeakRaisedCosineLowPass();
			float fLimit = hXmlConfig.getS2LargePeakRaisedCosineLimit();
			float fRollOff = hXmlConfig.getS2LargePeakRaisedCosineRollOff();
			int iLength = hXmlConfig.getS2LargePeakRaisedCosineLength();

			m_hS2LargePeakFilter.buildRaisedCosine(bLowPass, fLimit, fRollOff, iLength);
		}
	}

	m_fS2LargePeakSignalThreshold = hXmlConfig.getS2LargePeakSignalThreshold();
	m_fS2LargePeakDynamicFractionSignalThreshold = hXmlConfig.getS2LargePeakDynamicFractionSignalThreshold();
	m_iS2LargePeakMinWidth = hXmlConfig.getS2LargePeakMinWidth();
	m_iS2LargePeakMinIntervalWidth = hXmlConfig.getS2LargePeakMinIntervalWidth();
	m_iS2LargePeakPreIntervalAvgWindow = hXmlConfig.getS2LargePeakPreIntervalAvgWindow();
	m_iS2LargePeakPostIntervalAvgWindow = hXmlConfig.getS2LargePeakPostIntervalAvgWindow();
	m_fS2LargePeakPreTopLevelIntervalAvgThreshold = hXmlConfig.getS2LargePeakPreTopLevelIntervalAvgThreshold();
	m_fS2LargePeakPostTopLevelIntervalAvgThreshold = hXmlConfig.getS2LargePeakPostTopLevelIntervalAvgThreshold();
	m_fS2LargePeakPreIntervalAvgThreshold = hXmlConfig.getS2LargePeakPreIntervalAvgThreshold();
	m_fS2LargePeakPostIntervalAvgThreshold = hXmlConfig.getS2LargePeakPostIntervalAvgThreshold();
	m_fS2LargePeakOverlappingPeaksThreshold = hXmlConfig.getS2LargePeakOverlappingPeaksThreshold();
	m_fS2LargePeakLeftHeightFractionThreshold = hXmlConfig.getS2LargePeakLeftHeightFractionThreshold();
	m_fS2LargePeakRightHeightFractionThreshold = hXmlConfig.getS2LargePeakRightHeightFractionThreshold();
	m_fS2LargePeakSlopeThreshold = hXmlConfig.getS2LargePeakSlopeThreshold();
	m_fS2SmallPeakSlopeThreshold = hXmlConfig.getS2SmallPeakSlopeThreshold();
	m_fS2SmallPeakThreshold = hXmlConfig.getS2SmallPeakThreshold();

	m_bS2TinyPeakFilter = hXmlConfig.getS2TinyPeakFilter();

	if(m_bS2TinyPeakFilter)
	{
		m_hS2TinyPeakFilter = Filter();

		if(hXmlConfig.getS2TinyPeakFilterType() == "RaisedCosine")
		{
			bool bLowPass = hXmlConfig.getS2TinyPeakRaisedCosineLowPass();
			float fLimit = hXmlConfig.getS2TinyPeakRaisedCosineLimit();
			float fRollOff = hXmlConfig.getS2TinyPeakRaisedCosineRollOff();
			int iLength = hXmlConfig.getS2TinyPeakRaisedCosineLength();

			m_hS2TinyPeakFilter.buildRaisedCosine(bLowPass, fLimit, fRollOff, iLength);
		}
		else if(hXmlConfig.getS2TinyPeakFilterType() == "Custom")
		{
			m_hS2TinyPeakFilter.buildCustom(hXmlConfig.getS2TinyPeakCustomFilterCoefficients());
		}
	}

	m_fS2TinyPeakRightLimitHeightThreshold = hXmlConfig.getS2TinyPeakRightLimitHeightThreshold();
	m_fS2TinyPeakSignalThreshold = hXmlConfig.getS2TinyPeakSignalThreshold();
	m_iS2TinyPeakMinIntervalWidth = hXmlConfig.getS2TinyPeakMinIntervalWidth();
	m_iS2TinyPeakMaxIntervalWidth = hXmlConfig.getS2TinyPeakMaxIntervalWidth();
	m_iS2TinyPeakPrePeakAvgWindow = hXmlConfig.getS2TinyPeakPrePeakAvgWindow();
	m_iS2TinyPeakPostPeakAvgWindow = hXmlConfig.getS2TinyPeakPostPeakAvgWindow();
	m_fS2TinyPeakPrePeakAvgThreshold = hXmlConfig.getS2TinyPeakPrePeakAvgThreshold();
	m_fS2TinyPeakPostPeakAvgThreshold = hXmlConfig.getS2TinyPeakPostPeakAvgThreshold();
	m_fS2TinyPeakAspectRatioThreshold = hXmlConfig.getS2TinyPeakAspectRatioThreshold();
	m_fS2TinyPeakHeightFractionThreshold = hXmlConfig.getS2TinyPeakHeightFractionThreshold();

	m_iNbPmts = hXmlConfig.getNbPmts();

	m_iFirstTopPmt = hXmlConfig.getFirstTopPmt();
	m_iLastTopPmt = hXmlConfig.getLastTopPmt();
	m_iFirstBottomPmt = hXmlConfig.getFirstBottomPmt();
	m_iLastBottomPmt = hXmlConfig.getLastBottomPmt();

	m_pExternalGainsTable = hXmlConfig.getExternalGainsTable();

	m_pS1CoincidenceExcludedPmtSet = hXmlConfig.getS1CoincidenceExcludedPmtSet();
}

PeakFinder::~PeakFinder()
{
}

void
PeakFinder::findS1Peaks(Waveform const *pWaveform, std::map<int, Waveform *> const *pWaveforms, bool bConsiderS2PeaksFound, bool bOrderByCoincidences)
{
#ifdef ENABLE_PEAK_FINDER_OUTPUT
	cout << "PeakFinder::findS1Peaks()" << endl;
#endif

	// keep a handle on the waveform
	m_pWaveform = pWaveform;
	if(m_bS1Filter)
	{
		m_pS1FilteredWaveform = new Waveform(*pWaveform);
		m_pS1FilteredWaveform->convolve(m_hS1Filter);
	}
	m_iNbSamples = m_pWaveform->getNbSamples();

	// if we should consider the S2 peaks found, don't look for S1 peaks past the largest S2 peak
	int iLastRegion = (bConsiderS2PeaksFound)?((m_hS2Peaks.size())?(m_hS2Peaks.front().getOrder()):(0)):(0);

	// sort the S2 peaks found from left to right
	vector<Peak> hOrderedS2Peaks;
	if(bConsiderS2PeaksFound && m_hS2Peaks.size())
	{
		hOrderedS2Peaks = m_hS2Peaks;
		sort(hOrderedS2Peaks.begin(), hOrderedS2Peaks.end(), Peak::leftOf);
	}
	
	// clear any previously found peaks
	m_hS1Peaks = vector<Peak>();

	// check for S1 peaks in each region where we haven't found S2 peaks
	for(int iRegion = 0; iRegion <= iLastRegion; iRegion++)
	{
#ifdef ENABLE_PEAK_FINDER_OUTPUT
		int iRegionStart = (iRegion == 0)?(m_iS1PrePeakSamples+2):(hOrderedS2Peaks[iRegion-1].getRightEdge());
		int iRegionEnd = (iRegion == hOrderedS2Peaks.size())?(m_iNbSamples-m_iS1PeakWindow):(hOrderedS2Peaks[iRegion].getLeftEdge());
		cout << "  Region " << iRegion << " [" << iRegionStart << ", " << iRegionEnd << "]" << endl; 

		if(iRegion > 0)
		{
			float fS2Height = (*m_pWaveform)[hOrderedS2Peaks[iRegion-1].getPosition()];
			if(fS2Height > m_fS1RightLimitHeightThreshold)
				cout << "    S1RightLimitHeightThreshold reached (" << fS2Height << " V)" << endl;
		}
#endif

		// if the large S2 peak we just passed is huge, don't bother looking for S1 peaks after it
		if((iRegion > 0) && (*m_pWaveform)[hOrderedS2Peaks[iRegion-1].getPosition()] > m_fS1RightLimitHeightThreshold)
			break;

		// get the boundaries of the region
		int iSample = (iRegion == 0)?(m_iS1PrePeakSamples+2):(hOrderedS2Peaks[iRegion-1].getRightEdge());
		int iMax = (iRegion == hOrderedS2Peaks.size())?(m_iNbSamples-m_iS1PeakWindow):(hOrderedS2Peaks[iRegion].getLeftEdge());

		int iLastS1Boundary = iSample;

		while(iSample < iMax)
		{
			// if we go above threshold, check if this could be an S1 peak
			if((*m_pWaveform)[iSample] > m_fS1SignalThreshold)
			{
#ifdef ENABLE_PEAK_FINDER_OUTPUT
				cout << "    Sample: " << iSample << endl;
#endif
				int iMaxPos = m_pWaveform->maximumPosition(iSample, min(iSample+m_iS1PeakWindow, iMax));

				int iLeftLimit = max(iMaxPos-m_iS1PrePeakSamples-2, m_iS1PrePeakSamples);
				int iRightLimit = iMaxPos+m_iS1MaxLength;

				int iLeftExtent, iRightExtent;
				computeS1Extent(iMaxPos, iLeftLimit, iRightLimit, iLeftExtent, iRightExtent);

				int iPreAvgWindow = min(m_iS1PrePeakAvgWindow, iSample-iLastS1Boundary);
				int iPostAvgWindow = min(m_iS1PostPeakAvgWindow, iMax-iSample);

				float fMaxValue = (*m_pWaveform)[iMaxPos];
				float fPrePeakAvg = m_pWaveform->average(iLeftExtent-iPreAvgWindow/2-1, iPreAvgWindow)/fMaxValue;
				float fPostPeakAvg = m_pWaveform->average(iRightExtent+iPostAvgWindow/2+1, iPostAvgWindow)/fMaxValue;

				// compute the 25% full width from the filtered signal
				int iTmpMaxPos = m_pS1FilteredWaveform->maximumPosition(iLeftExtent, iRightExtent);
				int iFilteredWidth = m_pS1FilteredWaveform->width(iTmpMaxPos, iLeftLimit, iRightLimit, 0.25);

				// find the largest negative excursion around the peak
				float fLargestNegativeValue = m_pWaveform->minimum(iMaxPos-iPreAvgWindow, iMaxPos+iPostAvgWindow);

#ifdef ENABLE_PEAK_FINDER_OUTPUT
				cout << "      S1? ("
					<< "iLeftLimit: " << iLeftLimit << ", "
					<< "iRightLimit:" << iRightLimit << ", "
					<< "iLeftExtent: " << iLeftExtent << ", "
					<< "iRightExtent: " << iRightExtent << ")"
					<< endl;
				if(fPrePeakAvg > m_fS1PrePeakAvgThreshold)
					cout << "        -> Failed PrePeakAvg (fPrePeakAvg: " << fPrePeakAvg << ")" << endl;
				else if(fPostPeakAvg > m_fS1PostPeakAvgThreshold)
					cout << "        -> Failed PosPeakAvg (fPosPeakAvg: " << fPostPeakAvg << ")" << endl;
				else if(iFilteredWidth > m_fS1FilteredWidthThreshold)
					cout << "        -> Failed FilteredWidth (iFilteredWidth: " << iFilteredWidth << ")" << endl;
				else if(fMaxValue < m_fS1NegativeExcursionFractionThreshold*fabsf(fLargestNegativeValue))
					cout << "        -> Failed MaxValue (fMaxValue: " << fMaxValue << ", fLargestNegativeValue: " << fLargestNegativeValue << ")" << endl;
				else
					cout << "        -> Accepted" << endl;
#endif

				// if the conditions are met, save it as an S1 peak
				if((fPrePeakAvg < m_fS1PrePeakAvgThreshold) && (fPostPeakAvg < m_fS1PostPeakAvgThreshold)
					&& (iFilteredWidth < m_fS1FilteredWidthThreshold)
					&& (fMaxValue > m_fS1NegativeExcursionFractionThreshold*fabsf(fLargestNegativeValue)))
				{
					float fS1 = m_pWaveform->integrate(iLeftExtent, iRightExtent);
					float fS1Width = m_pWaveform->interpolatedWidth(iMaxPos, iLeftExtent, iRightExtent, 0.50);
					float fS1LowWidth = m_pWaveform->interpolatedWidth(iMaxPos, iLeftExtent, iRightExtent, 0.10);
					float fS1FilteredLowWidth = m_pS1FilteredWaveform->interpolatedWidth(iTmpMaxPos, iLeftLimit, iRightLimit, 0.10);

					Peak hPeak(iMaxPos, iLeftExtent-2, iRightExtent+2, fS1Width, fS1LowWidth, fS1FilteredLowWidth, fS1);

					m_hS1Peaks.push_back(hPeak);

					iLastS1Boundary = iRightExtent;
				}
				
				iSample = iRightExtent;
			}

			iSample++;
		}
	}

	if(bOrderByCoincidences)
	{
		// compute the number of PMT coincidences for every peak found
		vector<Peak>::iterator pS1Peak;
		for(pS1Peak = m_hS1Peaks.begin(); pS1Peak != m_hS1Peaks.end(); pS1Peak++)
		{
			int iLeftEdge = pS1Peak->getLeftEdge();
			int iRightEdge = pS1Peak->getRightEdge();
			int iCoincidences = 0;

			for(int iPmt = 1; iPmt < m_iNbPmts+1; iPmt++)
			{
				// if we had a waveform for that channel
				if(pWaveforms->count(iPmt-1))
				{
					const Waveform * const pWaveform = pWaveforms->find(iPmt-1)->second;

					// waveform integral is in V*s as measured by the digitizer
					float fExternalGain = m_pExternalGainsTable->find(iPmt)->second;
					float fToElectronConversionFactor = 1./(AnalysisManager::m_fInputImpedance*fExternalGain*AnalysisManager::m_fElectricCharge);
					float fS1Raw = pWaveform->integrate(iLeftEdge, iRightEdge)*fToElectronConversionFactor;

					bool bExcludedFromCoincidence = m_pS1CoincidenceExcludedPmtSet->count(iPmt);

					if(((iPmt >= m_iFirstTopPmt) && (iPmt <= m_iLastTopPmt)) || ((iPmt >= m_iFirstBottomPmt) && (iPmt <= m_iLastBottomPmt)))
					{
						if(!bExcludedFromCoincidence && (fS1Raw > m_fS1CoincidenceThreshold))
							iCoincidences++;
					}
				}
			}

			pS1Peak->setCoincidences(iCoincidences);
		}

		// sort all the S1 peaks found in reverse order of number of coincidences
		sort(m_hS1Peaks.begin(), m_hS1Peaks.end(), Peak::higherCoincidenceAndLargerThan);
	}
	else
	{
		// sort all the S1 peaks found in reverse order of size
		sort(m_hS1Peaks.begin(), m_hS1Peaks.end());
		reverse(m_hS1Peaks.begin(), m_hS1Peaks.end());
	}

	if(m_hS1Peaks.size() > m_iS1MaxNbPeaks)
		m_hS1Peaks.resize(m_iS1MaxNbPeaks);

	if(bOrderByCoincidences)
	{
		// compute the size order of peaks
		vector<Peak>::iterator pS1Peak;
		vector<float> hOrderedSizes;
		for(pS1Peak = m_hS1Peaks.begin(); pS1Peak != m_hS1Peaks.end(); pS1Peak++)
			hOrderedSizes.push_back(pS1Peak->getIntegral());

		sort(hOrderedSizes.begin(), hOrderedSizes.end());

		for(pS1Peak = m_hS1Peaks.begin(); pS1Peak != m_hS1Peaks.end(); pS1Peak++)
			pS1Peak->setSizeOrder(find(hOrderedSizes.begin(), hOrderedSizes.end(), pS1Peak->getIntegral()) - hOrderedSizes.begin());
	}

	// compute the left to right order of peaks
	vector<Peak>::iterator pS1Peak;
	vector<int> hOrderedPositions;
	for(pS1Peak = m_hS1Peaks.begin(); pS1Peak != m_hS1Peaks.end(); pS1Peak++)
		hOrderedPositions.push_back(pS1Peak->getPosition());

	sort(hOrderedPositions.begin(), hOrderedPositions.end());

	for(pS1Peak = m_hS1Peaks.begin(); pS1Peak != m_hS1Peaks.end(); pS1Peak++)
		pS1Peak->setOrder(find(hOrderedPositions.begin(), hOrderedPositions.end(), pS1Peak->getPosition()) - hOrderedPositions.begin());

	if(m_bS1Filter)
		delete m_pS1FilteredWaveform;
}

void
PeakFinder::findS2Peaks(Waveform const *pWaveform)
{
#ifdef ENABLE_PEAK_FINDER_OUTPUT
	cout << "PeakFinder::findS2Peaks()" << endl;
#endif

	// keep a handle on the waveform and get a filtered version for S2 peak finding
	m_pWaveform = pWaveform;
	if(m_bS2LargePeakFilter)
	{
		m_pS2LargePeakWaveform = new Waveform(*m_pWaveform);
		m_pS2LargePeakWaveform->convolve(m_hS2LargePeakFilter);
	}
	m_iNbSamples = m_pWaveform->getNbSamples();

	// clear any previously found peaks
	m_hS2Peaks = vector<Peak>();

	// dynamic threshold
	float fS2SignalThreshold = m_fS2LargePeakSignalThreshold;
	bool bAboveThreshold = false;

	float fAbsoluteMaxValue = 0.;

	stack<int> hLeftEdgeStack;
	stack<int> hRightEdgeStack;
	int iLeftEdge, iRightEdge;
	int iRightmostExtent;

	// check all potential S2 signal regions for peaks
	int iSample = 0, iMax = m_iNbSamples;
	while(iSample < iMax)
	{
		// if we go above threshold and we weren't above already
		if(((*m_pS2LargePeakWaveform)[iSample] >= fS2SignalThreshold) && !bAboveThreshold)
		{
			// then this might be the start of a large S2
			bAboveThreshold = true;
			iLeftEdge = iSample;
		}

		// if we go below threshold and we were above
		if(((*m_pS2LargePeakWaveform)[iSample] < fS2SignalThreshold) && bAboveThreshold)
		{
#ifdef ENABLE_PEAK_FINDER_OUTPUT
				cout << "  Top Interval: [" << iLeftEdge << ", " << iSample << "]" << endl;
#endif
			// this is the end of this interval
			bAboveThreshold = false;
			iRightEdge = iRightmostExtent = iSample;

			bool bTopLevelInterval = true, bS2Peak = false;

			float fTopLevelMaxValue = 0;

			hLeftEdgeStack.push(iLeftEdge);
			hRightEdgeStack.push(iRightEdge);

			while(!hLeftEdgeStack.empty())
			{
				iLeftEdge = hLeftEdgeStack.top(); hLeftEdgeStack.pop();
				iRightEdge = hRightEdgeStack.top(); hRightEdgeStack.pop();

#ifdef ENABLE_PEAK_FINDER_OUTPUT
				cout << "    Interval? [" << iLeftEdge << ", " << iRightEdge << "]" << endl;

				if((iRightEdge - iLeftEdge) < m_iS2LargePeakMinIntervalWidth)
				{
					cout << "      -> Failed S2LargePeakMinIntervalWidth ("
						<< "iRightEdge-iLeftEdge: " << (iRightEdge - iLeftEdge) << ")" << endl;
				}
#endif
				// if the interval width is large enough to hold an S2 peak
				if((iRightEdge - iLeftEdge) > m_iS2LargePeakMinIntervalWidth)
				{
#ifdef ENABLE_PEAK_FINDER_OUTPUT
					cout << "      -> Accepted (iRightEdge-iLeftEdge: " << (iRightEdge - iLeftEdge) << ")" << endl;
#endif
					int iMaxPos = m_pS2LargePeakWaveform->maximumPosition(iLeftEdge, iRightEdge);
					float fMaxValue = (*m_pS2LargePeakWaveform)[iMaxPos];
					fAbsoluteMaxValue = max(fMaxValue, fAbsoluteMaxValue);

					// get the boundaries between which we can compute the extent
					int iLeftMax = closestS2RightEdge(iMaxPos, iLeftEdge);
					int iRightMax = closestS2LeftEdge(iMaxPos, iRightEdge);

					int iLeftExtent, iRightExtent;

					computeS2LargePeakExtent(iMaxPos, iLeftMax, iRightMax, iLeftExtent, iRightExtent);

					// if it's the top level interval
					if(bTopLevelInterval)
					{
						fTopLevelMaxValue = fMaxValue;
						int iMinLeftEdge = min(iLeftEdge, iLeftExtent);
						int iMaxRightEdge = max(iRightEdge, iRightExtent);

						const int iPreAvgWindow = m_iS2LargePeakPreIntervalAvgWindow;
						const int iPostAvgWindow = m_iS2LargePeakPostIntervalAvgWindow;

						float fPreIntervalAvg = m_pS2LargePeakWaveform->average(iMinLeftEdge-iPreAvgWindow/2-1, iPreAvgWindow)/fMaxValue;
						float fPostIntervalAvg = m_pS2LargePeakWaveform->average(iMaxRightEdge+iPostAvgWindow/2+1, iPostAvgWindow)/fMaxValue;

#ifdef ENABLE_PEAK_FINDER_OUTPUT
						cout << "      Quiet? ("
							<< "fMaxValue: " << fMaxValue << ", "
							<< "iMinLeftEdge: " << iMinLeftEdge << ", "
							<< "iMaxRightEdge: " << iMaxRightEdge << ")" << endl;

						if((fPreIntervalAvg > m_fS2LargePeakPreTopLevelIntervalAvgThreshold)
							&& (fPostIntervalAvg > m_fS2LargePeakPostTopLevelIntervalAvgThreshold))
						{
							if(fPreIntervalAvg > m_fS2LargePeakPreTopLevelIntervalAvgThreshold)
								cout << "        -> Failed PreTopLevelIntervalAvg (fPreIntervalAvg: " << fPreIntervalAvg << ")" << endl;
							else
								cout << "        -> Failed PostTopLevelIntervalAvg (fPostIntervalAvg: " << fPostIntervalAvg << ")" << endl;
						}
						else
							cout << "        -> Accepted" << endl;
#endif
						// if the signal before and after the interval is not low enough break
						if((fPreIntervalAvg > m_fS2LargePeakPreTopLevelIntervalAvgThreshold)
							&& (fPostIntervalAvg > m_fS2LargePeakPostTopLevelIntervalAvgThreshold))
							break;
					}

					float fS2Width = m_pS2LargePeakWaveform->interpolatedWidth(iMaxPos, iLeftExtent, iRightExtent, 0.50);
					float fS2LowWidth = m_pS2LargePeakWaveform->interpolatedWidth(iMaxPos, iLeftExtent, iRightExtent, 0.10);

					if((fMaxValue/fTopLevelMaxValue > 0.05))
					{
						bS2Peak = (fS2Width > m_iS2LargePeakMinWidth);
					}
					else
					{
						const int iPreAvgWindow = m_iS2LargePeakPreIntervalAvgWindow;
						const int iPostAvgWindow = m_iS2LargePeakPostIntervalAvgWindow;

						float fPreIntervalAvg = m_pS2LargePeakWaveform->average(iLeftExtent-iPreAvgWindow/2-1, iPreAvgWindow)/fMaxValue;
						float fPostIntervalAvg = m_pS2LargePeakWaveform->average(iRightExtent+iPostAvgWindow/2+1, iPostAvgWindow)/fMaxValue;

#ifdef ENABLE_PEAK_FINDER_OUTPUT
						cout << "      Quiet? ("
							<< "fMaxValue: " << fMaxValue << ", "
							<< "iLeftExtent: " << iLeftExtent << ", "
							<< "iRightExtent: " << iRightExtent << ")" << endl;

						if(fPreIntervalAvg > m_fS2LargePeakPreIntervalAvgThreshold)
							cout << "        -> Failed PreIntervalAvg (fPreIntervalAvg: " << fPreIntervalAvg << endl;
						else if (fPostIntervalAvg > m_fS2LargePeakPostIntervalAvgThreshold)
							cout << "        -> Failed PostIntervalAvg (fPostIntervalAvg: " << fPostIntervalAvg << endl;
						else
							cout << "        -> Accepted" << endl;
#endif
						bS2Peak = (fS2Width > m_iS2LargePeakMinWidth)
							&& (fPreIntervalAvg < m_fS2LargePeakPreIntervalAvgThreshold)
							&& (fPostIntervalAvg < m_fS2LargePeakPostIntervalAvgThreshold);
					}

#ifdef ENABLE_PEAK_FINDER_OUTPUT
					cout << "        S2? (fS2Width: " << fS2Width << ")" << endl;
					if(bS2Peak)
						cout << "          -> Accepted" << endl;
					else
						cout << "          -> Failed S2Width (fS2Width: " << fS2Width << ")" << endl;
#endif
					if(bS2Peak)
					{
						float fS2 = m_pS2LargePeakWaveform->integrate(iLeftExtent, iRightExtent);

						Peak hPeak(iMaxPos, iLeftExtent, iRightExtent, fS2Width, fS2LowWidth, 0., fS2);
						m_hS2Peaks.push_back(hPeak);

						// push the subinterval right of the peak on the stack (don't change the ordering!)
						hLeftEdgeStack.push(iRightExtent);
						hRightEdgeStack.push(iRightEdge);

						// push the subinterval left of the peak on the stack (don't change the ordering!)
						hLeftEdgeStack.push(iLeftEdge);
						hRightEdgeStack.push(iLeftExtent);

						// update the iRightmostExtent to the rightmost peak iRightExtent
						iRightmostExtent = max(iRightExtent, iRightmostExtent);
					}

					if(bTopLevelInterval)
						bTopLevelInterval = false;
				}
			}

			fS2SignalThreshold = max(m_fS2LargePeakSignalThreshold, m_fS2LargePeakDynamicFractionSignalThreshold*fAbsoluteMaxValue);

			// jump to the rightmost peak extent
			iSample = iRightmostExtent;
		}

		iSample++;
	}

	// sort from left to right the S2 peaks obtained so far
	vector<Peak> hOrderedLargeS2Peaks(m_hS2Peaks);
	sort(hOrderedLargeS2Peaks.begin(), hOrderedLargeS2Peaks.end(), Peak::leftOf);

	// apply the filter to help find tiny S2 peaks
	if(m_bS2TinyPeakFilter)
	{
		m_pS2TinyPeakWaveform = new Waveform(*m_pWaveform);
		m_pS2TinyPeakWaveform->convolve(m_hS2TinyPeakFilter);
	}

	// check regions with no S2 peaks for tiny peaks (e.g. single electrons S2 peaks)
	for(int iRegion = 0; iRegion <= hOrderedLargeS2Peaks.size(); iRegion++)
	{
#ifdef ENABLE_PEAK_FINDER_OUTPUT
		int iRegionStart = (iRegion == 0)?(0):(hOrderedLargeS2Peaks[iRegion-1].getRightEdge());
		int iRegionEnd = (iRegion == hOrderedLargeS2Peaks.size())?(m_iNbSamples):(hOrderedLargeS2Peaks[iRegion].getLeftEdge());
		cout << "  Region " << iRegion << " [" << iRegionStart << ", " << iRegionEnd << "]" << endl; 

		if(iRegion > 0)
		{
			float fS2Height = (*m_pS2LargePeakWaveform)[hOrderedLargeS2Peaks[iRegion-1].getPosition()];
			if(fS2Height > m_fS2TinyPeakRightLimitHeightThreshold)
				cout << "    S2TinyPeakRightLimitHeightThreshold reached (" << fS2Height << " V)" << endl;
		}
#endif
		// if the large S2 peak we just passed is huge, don't bother looking for tiny S2 peaks after it
		if((iRegion > 0) && (*m_pS2LargePeakWaveform)[hOrderedLargeS2Peaks[iRegion-1].getPosition()] > m_fS2TinyPeakRightLimitHeightThreshold)
			break;

		// get the boundaries of the region
		iSample = (iRegion == 0)?(0):(hOrderedLargeS2Peaks[iRegion-1].getRightEdge());
		iMax = (iRegion == hOrderedLargeS2Peaks.size())?(m_iNbSamples):(hOrderedLargeS2Peaks[iRegion].getLeftEdge());

		bAboveThreshold = false;

		int iLastS2Boundary = iSample;

		while(iSample < iMax)
		{
			// if we go above threshold and we weren't above already
			if(((*m_pS2TinyPeakWaveform)[iSample] >= m_fS2TinyPeakSignalThreshold) && !bAboveThreshold)
			{
				// then this might be the start of a large S2
				bAboveThreshold = true;
				iLeftEdge = iSample;
			}

			// if we go below threshold and we were above
			if(((*m_pS2TinyPeakWaveform)[iSample] < m_fS2TinyPeakSignalThreshold) && bAboveThreshold)
			{
				// this is the end of this interval
				bAboveThreshold = false;
				iRightEdge = iSample;

				int iIntervalWidth = iRightEdge-iLeftEdge;

				int iMaxPos = m_pS2LargePeakWaveform->maximumPosition(iLeftEdge, iRightEdge);
				float fMaxValue = (*m_pS2LargePeakWaveform)[iMaxPos];

				int iLeftMax = iLastS2Boundary;
				int iRightMax = iMax;

				int iLeftExtent, iRightExtent;

				computeS2TinyPeakExtent(iMaxPos, iLeftMax, iRightMax, iLeftExtent, iRightExtent);

				float fAspectRatio = m_pWaveform->maximum(iLeftExtent, iRightExtent)/(iRightExtent-iLeftExtent);

				const int iPreAvgWindow = min(m_iS2TinyPeakPrePeakAvgWindow, iSample-iLeftMax);
				const int iPostAvgWindow = min(m_iS2TinyPeakPostPeakAvgWindow, iRightMax-iSample);

				float fPrePeakAvg = m_pS2LargePeakWaveform->average(iLeftExtent-iPreAvgWindow/2-1, iPreAvgWindow)/fMaxValue;
				float fPostPeakAvg = m_pS2LargePeakWaveform->average(iRightExtent+iPostAvgWindow/2+1, iPostAvgWindow)/fMaxValue;					

#ifdef ENABLE_PEAK_FINDER_OUTPUT
				cout << "    S2TinyPeak? [" << iLeftEdge << ", " << iRightEdge << "]" << endl;

				if((iIntervalWidth) < m_iS2TinyPeakMinIntervalWidth)
					cout << "      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: " << (iIntervalWidth) << ")" << endl;
				else if(iIntervalWidth > m_iS2TinyPeakMaxIntervalWidth)
					cout << "      -> Failed S2TinyPeakMaxIntervalWidth (iIntervalWidth: " << (iIntervalWidth) << ")" << endl;
				else if(fAspectRatio > m_fS2TinyPeakAspectRatioThreshold)
					cout << "      -> Failed S2TinyPeakAspectRatioThreshold (fAspectRatio: " << (fAspectRatio) << ")" << endl;
				else if(fPrePeakAvg > m_fS2TinyPeakPrePeakAvgThreshold)
					cout << "      -> Failed S2TinyPeakPrePeakAvgThreshold (fPrePeakAvg: " << (fPrePeakAvg) << ")" << endl;
				else if(fPostPeakAvg > m_fS2TinyPeakPostPeakAvgThreshold)
					cout << "      -> Failed S2TinyPeakPostPeakAvgThreshold (fPostPeakAvg: " << (fPostPeakAvg) << ")" << endl;
				else
					cout << "      -> Accepted" << endl;
#endif
				// if the conditions are met, save it as an S2 peak
				if((iIntervalWidth > m_iS2TinyPeakMinIntervalWidth) && (iIntervalWidth < m_iS2TinyPeakMaxIntervalWidth)
						&& (fAspectRatio < m_fS2TinyPeakAspectRatioThreshold)
						&& (fPrePeakAvg < m_fS2TinyPeakPrePeakAvgThreshold)
						&& (fPostPeakAvg < m_fS2TinyPeakPostPeakAvgThreshold))
				{
					float fS2 = m_pS2LargePeakWaveform->integrate(iLeftExtent, iRightExtent);

					float fS2Width = m_pS2LargePeakWaveform->interpolatedWidth(iMaxPos, iLeftExtent, iRightExtent, 0.50);
					float fS2LowWidth = m_pS2LargePeakWaveform->interpolatedWidth(iMaxPos, iLeftExtent, iRightExtent, 0.10);

					Peak hPeak(iMaxPos, iLeftExtent, iRightExtent, fS2Width, fS2LowWidth, 0., fS2);
					m_hS2Peaks.push_back(hPeak);

					iLastS2Boundary = iRightExtent;
				}

				iSample = iRightExtent;
			}

			iSample++;
		}
	}

	// sort all the S2 peaks found in reverse order of size
	sort(m_hS2Peaks.begin(), m_hS2Peaks.end());	
	reverse(m_hS2Peaks.begin(), m_hS2Peaks.end());	

	if(m_hS2Peaks.size() > m_iS2MaxNbPeaks)
		m_hS2Peaks.resize(m_iS2MaxNbPeaks);

	// compute the left to right order of peaks
	vector<int> hOrderedPositions;
	vector<Peak>::iterator pS2Peak;
	for(pS2Peak = m_hS2Peaks.begin(); pS2Peak != m_hS2Peaks.end(); pS2Peak++)
		hOrderedPositions.push_back(pS2Peak->getPosition());

	sort(hOrderedPositions.begin(), hOrderedPositions.end());

	for(pS2Peak = m_hS2Peaks.begin(); pS2Peak != m_hS2Peaks.end(); pS2Peak++)
		pS2Peak->setOrder(find(hOrderedPositions.begin(), hOrderedPositions.end(), pS2Peak->getPosition()) - hOrderedPositions.begin());

	if(m_bS2LargePeakFilter)
		delete m_pS2LargePeakWaveform;

	if(m_bS2TinyPeakFilter)
		delete m_pS2TinyPeakWaveform;
}

void
PeakFinder::computeS1Extent(int iPeak, int iLeftMax, int iRightMax, int &iLeftExtent, int &iRightExtent)
{
#ifdef ENABLE_PEAK_FINDER_OUTPUT
	cout << "      PeakFinder::computeS1Extent(" << iPeak << ", " << iLeftMax << ", " << iRightMax << ")" << endl;
#endif
	float fMaxValue = (*m_pWaveform)[iPeak];
	float fS1HeightThreshold = m_fS1HeightFractionThreshold*fMaxValue;

	int iLeft, iRight;
	iLeft = iRight = iPeak;

	// find the left boundary
	int iCount = 0;
	while(1)
	{
		if((iLeft < iLeftMax) || ((*m_pWaveform)[iLeft] < fS1HeightThreshold))
		{
			if(++iCount > m_iS1SamplesBelowThreshold)
				break;
		}
		else if((*m_pWaveform)[iLeft] > fMaxValue)
			break;
		else if(iCount > 0)
			iCount = 0;

		iLeft--;
	}

	iLeftExtent = (iCount > m_iS1SamplesBelowThreshold)?(iLeft+m_iS1SamplesBelowThreshold):(iLeft);

	// find the right boundary
	iCount = 0;
	while(1)
	{
		if((iRight > iRightMax) || ((*m_pWaveform)[iRight] < fS1HeightThreshold))
		{
			if(++iCount > m_iS1SamplesBelowThreshold)
				break;
		}
		else if((*m_pWaveform)[iRight] > fMaxValue)
			break;
		else if(iCount > 0)
			iCount = 0;

		iRight++;
	}

	iRightExtent = (iCount > m_iS1SamplesBelowThreshold)?(iRight-m_iS1SamplesBelowThreshold):(iRight);
}

void
PeakFinder::computeS2LargePeakExtent(int iPeak, int iLeftMax, int iRightMax, int &iLeftExtent, int &iRightExtent)
{
#ifdef ENABLE_PEAK_FINDER_OUTPUT
	cout << "      PeakFinder::computeS2LargePeakExtent(" << iPeak << ", " << iLeftMax << ", " << iRightMax << ")" << endl;
#endif
	const float fLeftS2HeightThreshold = m_fS2LargePeakLeftHeightFractionThreshold*(*m_pS2LargePeakWaveform)[iPeak];
	const float fRightS2HeightThreshold = m_fS2LargePeakRightHeightFractionThreshold*(*m_pS2LargePeakWaveform)[iPeak];

	int iLeft = iPeak, iRight = iPeak, iLeftMinimum = iPeak, iRightMinimum = iPeak;
	float fValue, fMaxValue = (*m_pS2LargePeakWaveform)[iPeak];
	float fLeftMinimumValue = fMaxValue, fRightMinimumValue = fMaxValue;

	bool bCheckSlope;
	float fSlopeThreshold;

	// find the left boundary
	while(1)
	{
		if((fValue = (*m_pS2LargePeakWaveform)[iLeft]) < fLeftMinimumValue)
		{
			iLeftMinimum = iLeft;
			fLeftMinimumValue = fValue;
		}

		bCheckSlope = (fValue > m_fS2LargePeakOverlappingPeaksThreshold) && (iLeft < (m_iNbSamples-16-5));

		if(fValue < fLeftS2HeightThreshold)
			break;

		fSlopeThreshold = (fValue < m_fS2SmallPeakThreshold)?(m_fS2SmallPeakSlopeThreshold):(m_fS2LargePeakSlopeThreshold);
		if((iLeft == iLeftMax) || (bCheckSlope && (m_pS2LargePeakWaveform->slope(iLeft)/fValue < -fSlopeThreshold)))
		{
			// we stopped because of the slope or we hit the border, use the minimum for the extent
			iLeft = iLeftMinimum;
			break;
		}
		
		iLeft--;
	}

	// find the right boundary
	while(1)
	{
		if((fValue = (*m_pS2LargePeakWaveform)[iRight]) < fRightMinimumValue)
		{
			iRightMinimum = iRight;
			fRightMinimumValue = fValue;
		}

		bCheckSlope = (fValue > m_fS2LargePeakOverlappingPeaksThreshold);

		if((fValue < fRightS2HeightThreshold))
			break;

		fSlopeThreshold = (fValue < m_fS2SmallPeakThreshold)?(m_fS2SmallPeakSlopeThreshold):(m_fS2LargePeakSlopeThreshold);
		if((iRight == iRightMax) || (bCheckSlope &&  (m_pS2LargePeakWaveform->slope(iRight)/fValue > fSlopeThreshold)))
		{
			// if we stopped because of the slope threshold, use the minimum for the extent
			iRight = iRightMinimum;
			break;
		}

		iRight++;
	}

	iLeftExtent = iLeft;
	iRightExtent = iRight;
}

void
PeakFinder::computeS2TinyPeakExtent(int iPeak, int iLeftMax, int iRightMax, int &iLeftExtent, int &iRightExtent)
{
#ifdef ENABLE_PEAK_FINDER_OUTPUT
	cout << "      PeakFinder::computeS2TinyPeakExtent(" << iPeak << ", " << iLeftMax << ", " << iRightMax << ")" << endl;
#endif
	const float fMaxValue = (*m_pS2LargePeakWaveform)[iPeak];
	const float fS2HeightThreshold = m_fS2TinyPeakHeightFractionThreshold*fMaxValue;
	int iLeft = iPeak, iRight = iPeak;

	// find the left boundary
	while(1)
	{
		if((iLeft < iLeftMax) || ((*m_pS2LargePeakWaveform)[iLeft] < fS2HeightThreshold))
			break;
		else if((*m_pS2LargePeakWaveform)[iLeft] > fMaxValue)
			break;
		
		iLeft--;
	}

	// find the right boundary
	while(1)
	{
		if((iRight > iRightMax) || ((*m_pS2LargePeakWaveform)[iRight] < fS2HeightThreshold))
			break;
		else if((*m_pS2LargePeakWaveform)[iRight] > fMaxValue)
			break;

		iRight++;
	}

	iLeftExtent = iLeft;
	iRightExtent = iRight;
}

int PeakFinder::closestS2LeftEdge(int iPeak, int iRightEdge)
{
	int iRightMax = min(iRightEdge+100, m_iNbSamples);

	if(!m_hS2Peaks.empty())
	{
		vector<Peak>::iterator pS2Peak;
		for(pS2Peak = m_hS2Peaks.begin(); pS2Peak != m_hS2Peaks.end(); pS2Peak++)
		{
			if((pS2Peak->getLeftEdge() >= iPeak) && (pS2Peak->getLeftEdge() < iRightMax))
				iRightMax = pS2Peak->getLeftEdge();
		}
	}

	return iRightMax;
}

int
PeakFinder::closestS2RightEdge(int iPeak, int iLeftEdge)
{
	int iLeftMax = max(iLeftEdge-100, 0);

	if(!m_hS2Peaks.empty())
	{
		vector<Peak>::iterator pS2Peak;
		for(pS2Peak = m_hS2Peaks.begin(); pS2Peak != m_hS2Peaks.end(); pS2Peak++)
		{
			if((pS2Peak->getRightEdge() <= iPeak) && (pS2Peak->getRightEdge() > iLeftMax))
				iLeftMax = pS2Peak->getRightEdge();
		}
	}

	return iLeftMax;
}

#ifdef ENABLE_LIQ_SCI
void
PeakFinder::findLiqSciPeak(Waveform const *pWaveform, int iRightMax)
{
	// keep a handle on the waveform
	m_pWaveform = pWaveform;
	m_iNbSamples = m_pWaveform->getNbSamples();
	
	// clear any previously found peaks
	m_hLiqSciPeak = Peak();

	// start at the first non-zero sample of the waveform 
	int iMaxPos = m_pWaveform->maximumPosition(50, iRightMax);

	int iLeftExtent, iRightExtent;
	computeS1Extent(iMaxPos, 50, m_iNbSamples, iLeftExtent, iRightExtent);

	float fLiqSci = m_pWaveform->integrate(iLeftExtent, iRightExtent);
	float fLiqSciWidth = m_pWaveform->interpolatedWidth(iMaxPos, iLeftExtent, iRightExtent, 0.50);
	float fLiqSciLowWidth = m_pWaveform->interpolatedWidth(iMaxPos, iLeftExtent, iRightExtent, 0.10);

	m_hLiqSciPeak = Peak(iMaxPos, iLeftExtent-2, iRightExtent+2, fLiqSciWidth, fLiqSciLowWidth, 0, fLiqSci);
}

void
PeakFinder::computeLiqSciExtent(int iPeak, int iLeftMax, int iRightMax, int &iLeftExtent, int &iRightExtent)
{
	float m_fLiqSciHeightFractionThreshold = 0.0001;
	int m_iLiqSciSamplesBelowThreshold = 2;

	float fMaxValue = (*m_pWaveform)[iPeak];
	float fLiqSciHeightThreshold = m_fLiqSciHeightFractionThreshold*fMaxValue;

	int iLeft, iRight;
	iLeft = iRight = iPeak;

	// find the left boundary
	int iCount = 0;
	while(1)
	{
		if((iLeft < iLeftMax) || ((*m_pWaveform)[iLeft] < fLiqSciHeightThreshold))
		{
			if(++iCount > m_iLiqSciSamplesBelowThreshold)
				break;
		}
		else if((*m_pWaveform)[iLeft] > fMaxValue)
			break;
		else if(iCount > 0)
			iCount = 0;

		iLeft--;
	}

	iLeftExtent = (iCount > m_iLiqSciSamplesBelowThreshold)?(iLeft+m_iLiqSciSamplesBelowThreshold):(iLeft);

	// find the right boundary
	iCount = 0;
	while(1)
	{
		if((iRight > iRightMax) || ((*m_pWaveform)[iRight] < fLiqSciHeightThreshold))
		{
			if(++iCount > m_iLiqSciSamplesBelowThreshold)
				break;
		}
		else if((*m_pWaveform)[iRight] > fMaxValue)
			break;
		else if(iCount > 0)
			iCount = 0;

		iRight++;
	}

	iRightExtent = (iCount > m_iLiqSciSamplesBelowThreshold)?(iRight-m_iLiqSciSamplesBelowThreshold):(iRight);
}
#endif

#ifdef ENABLE_NAI
void
PeakFinder::findNaiPeak(Waveform const *pWaveform, int iRightMax)
{
	// keep a handle on the waveform
	m_pWaveform = pWaveform;
	m_iNbSamples = m_pWaveform->getNbSamples();
	
	// clear any previously found peaks
	m_hNaiPeak = Peak();

	// start at the first non-zero sample of the waveform 
	int iMaxPos = m_pWaveform->maximumPosition(50, iRightMax);

	int iLeftExtent, iRightExtent;
	computeS1Extent(iMaxPos, 50, m_iNbSamples, iLeftExtent, iRightExtent);

	float fNai = m_pWaveform->integrate(iLeftExtent, iRightExtent);
	float fNaiWidth = m_pWaveform->interpolatedWidth(iMaxPos, iLeftExtent, iRightExtent, 0.50);
	float fNaiLowWidth = m_pWaveform->interpolatedWidth(iMaxPos, iLeftExtent, iRightExtent, 0.10);

	m_hNaiPeak = Peak(iMaxPos, iLeftExtent-2, iRightExtent+2, fNaiWidth, fNaiLowWidth, 0,  fNai);
}

void
PeakFinder::computeNaiExtent(int iPeak, int iLeftMax, int iRightMax, int &iLeftExtent, int &iRightExtent)
{
	float m_fNaiHeightFractionThreshold = 0.0001;
	int m_iNaiSamplesBelowThreshold = 2;

	float fMaxValue = (*m_pWaveform)[iPeak];
	float fNaiHeightThreshold = m_fNaiHeightFractionThreshold*fMaxValue;

	int iLeft, iRight;
	iLeft = iRight = iPeak;

	// find the left boundary
	int iCount = 0;
	while(1)
	{
		if((iLeft < iLeftMax) || ((*m_pWaveform)[iLeft] < fNaiHeightThreshold))
		{
			if(++iCount > m_iNaiSamplesBelowThreshold)
				break;
		}
		else if((*m_pWaveform)[iLeft] > fMaxValue)
			break;
		else if(iCount > 0)
			iCount = 0;

		iLeft--;
	}

	iLeftExtent = (iCount > m_iNaiSamplesBelowThreshold)?(iLeft+m_iNaiSamplesBelowThreshold):(iLeft);

	// find the right boundary
	iCount = 0;
	while(1)
	{
		if((iRight > iRightMax) || ((*m_pWaveform)[iRight] < fNaiHeightThreshold))
		{
			if(++iCount > m_iNaiSamplesBelowThreshold)
				break;
		}
		else if((*m_pWaveform)[iRight] > fMaxValue)
			break;
		else if(iCount > 0)
			iCount = 0;

		iRight++;
	}

	iRightExtent = (iCount > m_iNaiSamplesBelowThreshold)?(iRight-m_iNaiSamplesBelowThreshold):(iRight);
}
#endif

