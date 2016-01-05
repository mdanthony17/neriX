#ifndef __PEAKFINDER_H__
#define __PEAKFINDER_H__

#include "xerawdp.h"

#include "XmlConfig.h"
#include "Waveform.h"

#include <vector>
#include <map>

class Peak
{
public:
	Peak();
	Peak(const Peak &hPeak);
	Peak(int iPosition, int iLeftEdge, int iRightEdge, float fWidth, float fLowWidth, float fFilteredLowWidth, float fIntegral);

public:
	int getPosition() const { return m_iPosition; }
	int getLeftEdge() const { return m_iLeftEdge; }
	int getRightEdge() const { return m_iRightEdge; }
	float getWidth() const { return m_fWidth; }
	float getLowWidth() const { return m_fLowWidth; }
	float getFilteredLowWidth() const { return m_fFilteredLowWidth; }
	float getIntegral() const { return m_fIntegral; }

	void setOrder(int iOrder) { m_iOrder = iOrder; }
	int getOrder() const { return m_iOrder; }
	void setSizeOrder(int iSizeOrder) { m_iSizeOrder = iSizeOrder; }
	int getSizeOrder() const { return m_iSizeOrder; }
	void setCoincidences(int iCoincidences) { m_iCoincidences = iCoincidences; }
	int getCoincidences() const { return m_iCoincidences; }

	bool operator<(const Peak & hPeak) const;
	static bool leftOf(const Peak &hPeak1, const Peak &hPeak2) { return hPeak1.m_iLeftEdge < hPeak2.m_iLeftEdge; }
	static bool rightOf(const Peak &hPeak1, const Peak &hPeak2) { return hPeak1.m_iRightEdge < hPeak2.m_iRightEdge; }
	static bool higherCoincidenceAndLargerThan(const Peak &hPeak1, const Peak &hPeak2) { return (hPeak1.m_iCoincidences != hPeak2.m_iCoincidences)?(hPeak1.m_iCoincidences > hPeak2.m_iCoincidences):(hPeak1.m_fIntegral > hPeak2.m_fIntegral); }

private:
	int m_iPosition;
	int m_iLeftEdge;
	int m_iRightEdge;
	float m_fWidth;
	float m_fLowWidth;
	float m_fFilteredLowWidth;
	float m_fIntegral;

	int m_iOrder;
	int m_iSizeOrder;
	int m_iCoincidences;
};

class PeakFinder
{
public:
	PeakFinder(const XmlConfig &hXmlConfig);
	~PeakFinder();

public:
	const std::vector<Peak> &getS1Peaks() const { return m_hS1Peaks; }
	const std::vector<Peak> &getS2Peaks() const { return m_hS2Peaks; }

#ifdef ENABLE_LIQ_SCI
	const Peak &getLiqSciPeak() const { return m_hLiqSciPeak; }
#endif
#ifdef ENABLE_NAI
	const Peak &getNaiPeak() const { return m_hNaiPeak; }
#endif

	void findS1Peaks(Waveform const *pWaveform, std::map<int, Waveform *> const *pWaveforms, bool bConsiderS2PeaksFound, bool bOrderByCoincidences);
	void findS2Peaks(Waveform const *pWaveform);

#ifdef ENABLE_LIQ_SCI
	void findLiqSciPeak(Waveform const *pWaveform, int iRightMax);
#endif
#ifdef ENABLE_NAI
	void findNaiPeak(Waveform const *pWaveform, int iRightMax);
#endif

private:
	void computeS1Extent(int iPeak, int iLeftMax, int iRightMax, int &iLeftExtent, int &iRightExtent);
	void computeS2LargePeakExtent(int iPeak, int iLeftMax, int iRightMax, int &iLeftExtent, int &iRightExtent);
	void computeS2TinyPeakExtent(int iPeak, int iLeftMax, int iRightMax, int &iLeftExtent, int &iRightExtent);
	int closestS2LeftEdge(int iPeak, int iRightEdge);
	int closestS2RightEdge(int iPeak, int iLeftEdge);

#ifdef ENABLE_LIQ_SCI
	void computeLiqSciExtent(int iPeak, int iLeftMax, int iRightMax, int &iLeftExtent, int &iRightExtent);
#endif
#ifdef ENABLE_NAI
	void computeNaiExtent(int iPeak, int iLeftMax, int iRightMax, int &iLeftExtent, int &iRightExtent);
#endif

private:
	bool m_bS1Filter;
	Filter m_hS1Filter;

	int m_iS1MaxNbPeaks;

	float m_fS1RightLimitHeightThreshold;
	float m_fS1SignalThreshold;
	int m_iS1PeakWindow;
	int m_iS1PrePeakSamples;
	int m_iS1MaxLength;
	int m_iS1PrePeakAvgWindow;
	int m_iS1PostPeakAvgWindow;
	float m_fS1PrePeakAvgThreshold;
	float m_fS1PostPeakAvgThreshold;
	float m_fS1FilteredWidthThreshold;
	float m_fS1NegativeExcursionFractionThreshold;
	float m_fS1HeightFractionThreshold;
	int m_iS1SamplesBelowThreshold;
	float m_fS1CoincidenceThreshold;

	bool m_bS2LargePeakFilter;
	Filter m_hS2LargePeakFilter;

	int m_iS2MaxNbPeaks;

	float m_fS2LargePeakSignalThreshold;
	float m_fS2LargePeakDynamicFractionSignalThreshold;
	int m_iS2LargePeakMinWidth;
	int m_iS2LargePeakMinIntervalWidth;
	int m_iS2LargePeakPreIntervalAvgWindow;
	int m_iS2LargePeakPostIntervalAvgWindow;
	float m_fS2LargePeakPreTopLevelIntervalAvgThreshold;
	float m_fS2LargePeakPostTopLevelIntervalAvgThreshold;
	float m_fS2LargePeakPreIntervalAvgThreshold;
	float m_fS2LargePeakPostIntervalAvgThreshold;
	float m_fS2LargePeakOverlappingPeaksThreshold;
	float m_fS2LargePeakLeftHeightFractionThreshold;
	float m_fS2LargePeakRightHeightFractionThreshold;
	float m_fS2LargePeakSlopeThreshold;
	float m_fS2SmallPeakSlopeThreshold;
	float m_fS2SmallPeakThreshold;

	bool m_bS2TinyPeakFilter;
	Filter m_hS2TinyPeakFilter;

	float m_fS2TinyPeakRightLimitHeightThreshold;
	float m_fS2TinyPeakSignalThreshold;
	int m_iS2TinyPeakMinIntervalWidth;
	int m_iS2TinyPeakMaxIntervalWidth;
	int m_iS2TinyPeakPrePeakAvgWindow;
	int m_iS2TinyPeakPostPeakAvgWindow;
	float m_fS2TinyPeakPrePeakAvgThreshold;
	float m_fS2TinyPeakPostPeakAvgThreshold;
	float m_fS2TinyPeakAspectRatioThreshold;
	float m_fS2TinyPeakHeightFractionThreshold;

	int m_iNbPmts;

	int m_iFirstTopPmt, m_iLastTopPmt;
	int m_iFirstBottomPmt, m_iLastBottomPmt;

	PmtSet const *m_pS1CoincidenceExcludedPmtSet;

	ExternalGainsTable const *m_pExternalGainsTable;

	int m_iNbSamples;

	Waveform const *m_pWaveform;
	Waveform *m_pS1FilteredWaveform;
	Waveform *m_pS2LargePeakWaveform;
	Waveform *m_pS2TinyPeakWaveform;

	std::vector<Peak> m_hS1Peaks;
	std::vector<Peak> m_hS2Peaks;
#ifdef ENABLE_LIQ_SCI
	Peak m_hLiqSciPeak;
#endif
#ifdef ENABLE_NAI
	Peak m_hNaiPeak;
#endif
};

#endif // __PEAKFINDER_H__

