#ifndef __EVENTDATA_H__
#define __EVENTDATA_H__

#include "xerawdp.h"
#include "RawData.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Waveform;

class EventData
{
public:
	EventData();
	~EventData();

public:
	// level1 quantities, stored in T1
	void setFilename(const std::string &hFilename);
	void setEventId(int iEventId) { m_iEventId = iEventId; }
	void setTimeSec(int iTimeSec) { m_iTimeSec = iTimeSec; }
	void setTimeMicroSec(int iTimeMicroSec) { m_iTimeMicroSec = iTimeMicroSec; }
	
	// set raw data from tdc
	void setRawData_tdc(RawData *pRawData_tdc) { *m_pRawData_tdc = *pRawData_tdc; }
	// get raw data from tdc
	RawData *getRawData_tdc() const { return m_pRawData_tdc; }

	void setChannels(const std::vector<int> &hChannels) { *m_pChannels = hChannels; }
	void setBaselinesAvg(const std::vector<std::vector<float> > &hBaselinesAvg) { *m_pBaselinesAvg = hBaselinesAvg; }
	void setBaselinesMax(const std::vector<std::vector<float> > &hBaselinesMax) { *m_pBaselinesMax = hBaselinesMax; }

	int getEventId() const { return m_iEventId; }
	int getTimeSec() const { return m_iTimeSec; }
	int getTimeMicroSec() const { return m_iTimeMicroSec; }

	std::vector<int> const *getChannels() const { return m_pChannels; }
	std::vector<std::vector<float> > const *getBaselinesAvg() const { return m_pBaselinesAvg; }
	std::vector<std::vector<float> > const *getBaselinesMax() const { return m_pBaselinesMax; }

	std::vector<float> const *getAreaRaw() const { return m_pAreaRaw; }
	std::vector<float> const *getAbsAreaRaw() const { return m_pAbsAreaRaw; }

	int getNbNonZeroLast() const { return m_iNbNonZeroLast; }

	int getNbS1Peaks() const { return m_iNbS1Peaks; }
	int getS1sPeak(int iPeak) const { return (*m_pS1sPeak)[iPeak]; }
	int getS1sLeftEdge(int iPeak) const { return (*m_pS1sLeftEdge)[iPeak]; }
	int getS1sRightEdge(int iPeak) const { return (*m_pS1sRightEdge)[iPeak]; }
	float getS1sWidth(int iPeak) const { return (*m_pS1sWidth)[iPeak]; }
	float getS1sLowWidth(int iPeak) const { return (*m_pS1sLowWidth)[iPeak]; }
	float getS1sFilteredLowWidth(int iPeak) const { return (*m_pS1sFilteredLowWidth)[iPeak]; }
	float getS1sHeight(int iPeak) const { return (*m_pS1sHeight)[iPeak]; }
	float getS1sMinimum(int iPeak) const { return (*m_pS1sMinimum)[iPeak]; }
	float getS1sMeanTime(int iPeak) const { return (*m_pS1sMeanTime)[iPeak]; }
	float getS1sTimeSpread(int iPeak) const { return (*m_pS1sTimeSpread)[iPeak]; }

	int getNbS2Peaks() const { return m_iNbS2Peaks; }
	int getS2sPeak(int iPeak) const { return (*m_pS2sPeak)[iPeak]; }
	int getS2sLeftEdge(int iPeak) const { return (*m_pS2sLeftEdge)[iPeak]; }
	int getS2sRightEdge(int iPeak) const { return (*m_pS2sRightEdge)[iPeak]; }
	float getS2sWidth(int iPeak) const { return (*m_pS2sWidth)[iPeak]; }
	float getS2sLowWidth(int iPeak) const { return (*m_pS2sLowWidth)[iPeak]; }

	int getNbS1PeaksVeto() const { return m_iNbS1PeaksVeto; }
	int getS1sPeakVeto(int iPeak) const { return (*m_pS1sPeakVeto)[iPeak]; }
	int getS1sLeftEdgeVeto(int iPeak) const { return (*m_pS1sLeftEdgeVeto)[iPeak]; }
	int getS1sRightEdgeVeto(int iPeak) const { return (*m_pS1sRightEdgeVeto)[iPeak]; }

	std::vector<std::vector<float> > const *getS1sRaw() const { return m_pS1sRaw; }
	std::vector<std::vector<float> > const *getS1sRawVeto() const { return m_pS1sRawVeto; }
	std::vector<std::vector<float> > const *getS2sRaw() const { return m_pS2sRaw; }

	std::vector<std::vector<int> > const *getS1sMaxPosition() const { return m_pS1sMaxPosition; }
	std::vector<std::vector<int> > const *getS1sMaxPositionVeto() const { return m_pS1sMaxPositionVeto; }

#ifdef ENABLE_LIQ_SCI
	int getNbLiqScis() const { return m_pLiqSciPeak->size(); }
	int getLiqSciPeak(int iLiqSci) const { return (*m_pLiqSciPeak)[iLiqSci]; }
	int getLiqSciLeftEdge(int iLiqSci) const { return (*m_pLiqSciLeftEdge)[iLiqSci]; }
	int getLiqSciRightEdge(int iLiqSci) const { return (*m_pLiqSciRightEdge)[iLiqSci]; }
	float getLiqSciHeight(int iLiqSci) const { return (*m_pLiqSciHeight)[iLiqSci]; }
#endif

#ifdef ENABLE_NAI
	int getNbNaiDetectors() const { return m_pNaiPeak->size(); }
	int getNaiPeak(int iNai) const { return (*m_pNaiPeak)[iNai]; }
	int getNaiLeftEdge(int iNai) const { return (*m_pNaiLeftEdge)[iNai]; }
	int getNaiRightEdge(int iNai) const { return (*m_pNaiRightEdge)[iNai]; }
	float getNaiHeight(int iNai) const { return (*m_pNaiHeight)[iNai]; }
#endif

#ifdef ENABLE_HPGE
	int getNbGeDetectors() const { return m_pGeHeight->size(); }
	float getGeHeight(int iGe) const { return (*m_pGeHeight)[iGe]; }
#endif

	// level2 quantities, stored in T2
	void setPmtCalibrationFiles(const std::string &hPmtCalibrationFiles);

	float getS1Tot() const { return m_fS1Tot; }

	std::vector<std::vector<float> > const *getS1s() const { return m_pS1s; }
	float getS1sTot(int iPeak) const { return (*m_pS1sTot)[iPeak]; }
	float getS1sTotTop(int iPeak) const { return (*m_pS1sTotTop)[iPeak]; }
	float getS1sTotBottom(int iPeak) const { return (*m_pS1sTotBottom)[iPeak]; }

	std::vector<std::vector<float> > const *getS2s() const { return m_pS2s; }
	float getS2sTot(int iPeak) const { return (*m_pS2sTot)[iPeak]; }
	float getS2sTotTop(int iPeak) const { return (*m_pS2sTotTop)[iPeak]; }
	float getS2sTotBottom(int iPeak) const { return (*m_pS2sTotBottom)[iPeak]; }

	std::vector<std::vector<float> > const *getS1sVeto() const { return m_pS1sVeto; }	

	int getS1sCoin(int iPeak) const { return (*m_pS1sCoin)[iPeak]; }
	int getS1sCoinTop(int iPeak) const { return (*m_pS1sCoinTop)[iPeak]; }
	int getS1sCoinBottom(int iPeak) const { return (*m_pS1sCoinBottom)[iPeak]; }

	int getS2sCoin(int iPeak) const { return (*m_pS2sCoin)[iPeak]; }
	int getS2sCoinTop(int iPeak) const { return (*m_pS2sCoinTop)[iPeak]; }
	int getS2sCoinBottom(int iPeak) const { return (*m_pS2sCoinBottom)[iPeak]; }

	const std::vector<int> &getS1sPmtOrder(int iPeak) const { return (*m_pS1sPmtOrder)[iPeak]; }
	const std::vector<int> &getS2sPmtOrder(int iPeak) const { return (*m_pS2sPmtOrder)[iPeak]; }

	const std::vector<float> &getS2sPosMaxPmt(int iPeak) const { return (*m_pS2sPosMaxPmt)[iPeak]; }
	const std::vector<float> &getS2sPosSvm(int iPeak) const { return (*m_pS2sPosSvm)[iPeak]; }
	const std::vector<float> &getS2sPosNn(int iPeak) const { return (*m_pS2sPosNn)[iPeak]; }
	const std::vector<float> &getS2sPosCoG(int iPeak) const { return (*m_pS2sPosCoG)[iPeak]; }// added by Qing
	const std::vector<float> &getS2sPosChi2(int iPeak) const { return (*m_pS2sPosChi2)[iPeak]; }
	const std::vector<float> &getS2sPosFann(int iPeak) const { return (*m_pS2sPosFann)[iPeak]; }

	// level3 quantities, stored in T3
	std::vector<std::vector<float> > const *getS1sPmtMaxHeight() const { return m_pS1sPmtMaxHeight; }
	std::vector<std::vector<float> > const *getS1sBeforeMaximum() const { return m_pS1sBeforeMaximum; }
	std::vector<std::vector<float> > const *getS1sAfterMaximum() const { return m_pS1sAfterMaximum; }

	// waveforms, not stored in the trees
	void setWaveforms(std::map<int, Waveform *> *pWaveforms) { m_pWaveforms = pWaveforms; }
	void setWaveforms_tdc(std::map<int, Waveform *> *pWaveforms) { m_pWaveforms_tdc = pWaveforms; }

	std::map<int, Waveform *> const *getWaveforms() const { return m_pWaveforms; }
	std::map<int, Waveform *> const *getWaveforms_tdc() const { return m_pWaveforms; }

	Waveform const *getTotalWaveform() const { return m_pTotalWaveform; }
	Waveform const *getTotalWaveformVeto() const { return m_pTotalWaveformVeto; }

	Waveform const *getS1PeakFindingTotalWaveform() const { return m_pS1PeakFindingTotalWaveform; }
	Waveform const *getS2PeakFindingTotalWaveform() const { return m_pS2PeakFindingTotalWaveform; }
	Waveform const *getS1PeakFindingTotalWaveformVeto() const { return m_pS1PeakFindingTotalWaveformVeto; }

public:
	void clear();

private:
	void allocate();
	void deallocate();

	void initializeWaveforms();
	void deleteWaveforms();

private:
	// level1 quantities, stored in T1
	char m_szFilename[32];
	int m_iEventId;
	int m_iTimeSec;
	int m_iTimeMicroSec;
	
	// add raw data from TDC (rather than waveforms)
	RawData *m_pRawData_tdc;

	std::vector<int> *m_pChannels;
	std::vector<std::vector<float> > *m_pBaselinesAvg;
	std::vector<std::vector<float> > *m_pBaselinesMax;

	float m_fAreaTotRaw;
	float m_fAreaTotRawVeto;
	std::vector<float> *m_pAreaRaw;

	float m_fAbsAreaTotRaw;
	std::vector<float> *m_pAbsAreaRaw;

	int m_iNbNonZeroLast;

	std::vector<float> *m_pSinglePeak;
	std::vector<float> *m_pSingleHeight;
	std::vector<int> *m_pSingleSample;
	std::vector<std::vector<float> > *m_pSingleBefore;
	std::vector<std::vector<float> > *m_pSingleAfter;
	std::vector<std::vector<float> > *m_pSingleFixed;

	int m_iNbS1Peaks;
	std::vector<int> *m_pS1sPeak;
	std::vector<int> *m_pS1sLeftEdge;
	std::vector<int> *m_pS1sRightEdge;
	std::vector<float> *m_pS1sWidth;
	std::vector<float> *m_pS1sLowWidth;
	std::vector<float> *m_pS1sFilteredLowWidth;
	std::vector<float> *m_pS1sHeight;
	std::vector<float> *m_pS1sMinimum;
	std::vector<float> *m_pS1sMeanTime;
	std::vector<float> *m_pS1sTimeSpread;
	std::vector<int> *m_pS1sSaturation;	

	std::vector<int> *m_pS1Order;	
	std::vector<int> *m_pS1SizeOrder;	

	int m_iNbS2Peaks;
	std::vector<int> *m_pS2sPeak;
	std::vector<int> *m_pS2sLeftEdge;
	std::vector<int> *m_pS2sRightEdge;
	std::vector<float> *m_pS2sWidth;
	std::vector<float> *m_pS2sLowWidth;
	std::vector<float> *m_pS2sHeight;
	std::vector<std::vector<float> > *m_pS2sHeightChan;
	std::vector<int> *m_pS2sSaturation;	
	std::vector<int> *m_pS2sTrigger;
	std::vector<float> *m_pCoinRaw;
	std::vector<float> *m_pTrigArea;
	std::vector<int> *m_pTrigLeftEdge;
	
	// tdc storage
	std::vector<float> *m_pTimeTDC;

	std::vector<int> *m_pS2Order;	

	int m_iNbS1PeaksVeto;
	std::vector<int> *m_pS1sPeakVeto;
	std::vector<int> *m_pS1sLeftEdgeVeto;
	std::vector<int> *m_pS1sRightEdgeVeto;
	std::vector<float> *m_pS1sWidthVeto;
	std::vector<float> *m_pS1sLowWidthVeto;
	std::vector<float> *m_pS1sHeightVeto;

	std::vector<int> *m_pS1OrderVeto;

	std::vector<std::vector<float> > *m_pS1sRaw;
	std::vector<std::vector<float> > *m_pS2sRaw;
	std::vector<std::vector<float> > *m_pS1sRawVeto;

	std::vector<std::vector<int> > *m_pS1sMaxPosition;
	std::vector<std::vector<int> > *m_pS1sMaxPositionVeto;

	std::vector<std::vector<float> > *m_pS1FitParameters;
	

#ifdef ENABLE_LIQ_SCI
	std::vector<int> *m_pLiqSciPeak;
	std::vector<int> *m_pLiqSciLeftEdge;
	std::vector<int> *m_pLiqSciRightEdge;
	std::vector<float> *m_pLiqSciWidth;
	std::vector<float> *m_pLiqSciLowWidth;
	std::vector<float> *m_pLiqSciHeight;

	std::vector<float> *m_pLiqSciRaw;
	std::vector<float> *m_pLiqSciTailRaw;
#endif

#ifdef ENABLE_NAI
	std::vector<int> *m_pNaiPeak;
	std::vector<int> *m_pNaiLeftEdge;
	std::vector<int> *m_pNaiRightEdge;
	std::vector<float> *m_pNaiWidth;
	std::vector<float> *m_pNaiLowWidth;
	std::vector<float> *m_pNaiHeight;

	std::vector<float> *m_pNaiRaw;
#endif

#ifdef ENABLE_HPGE
	std::vector<float> *m_pGeHeight;
#endif

#ifdef ENABLE_TAC
	std::vector<float> *m_pTacHeight;
	std::vector<float> *m_pTacMaximum;
#endif

	// level2 quantities, stored in T2
	char m_szPmtCalibrationFiles[256];

	float m_fAreaTot;
	float m_fAreaTotVeto;
	std::vector<float> *m_pArea;

	float m_fAbsAreaTot;
	std::vector<float> *m_pAbsArea;

	float m_fS1Tot;
	float m_fS1TotTop;
	float m_fS1TotBottom;

	std::vector<std::vector<float> > *m_pS1s;
	std::vector<float> *m_pS1sTot;
	std::vector<float> *m_pS1sTotTop;
	std::vector<float> *m_pS1sTotBottom;

	std::vector<float> *m_pfS1sTot;

	float m_fS2Tot;
	float m_fS2TotTop;
	float m_fS2TotBottom;
	float m_fS2TotAfter;

	std::vector<std::vector<float> > *m_pS2s;
	std::vector<float> *m_pS2sTot;
	std::vector<float> *m_pS2sTotTop;
	std::vector<float> *m_pS2sTotBottom;

	float m_fS1TotVeto;

	std::vector<std::vector<float> > *m_pS1sVeto;
	std::vector<float> *m_pS1sTotVeto;

	std::vector<int> *m_pS1sCoin;
	std::vector<int> *m_pS1sCoinTop;
	std::vector<int> *m_pS1sCoinBottom;

	std::vector<int> *m_pS2sCoin;
	std::vector<int> *m_pS2sCoinTop;
	std::vector<int> *m_pS2sCoinBottom;

	std::vector<int> *m_pS1sCoinVeto;

	std::vector<std::vector<int> > *m_pS1sPmtCoin;

	std::vector<std::vector<int> > *m_pS1sPmtOrder;	
	std::vector<std::vector<int> > *m_pS2sPmtOrder;	

	std::vector<std::vector<float> > *m_pS2sPosMaxPmt;
	std::vector<std::vector<float> > *m_pS2sPosSvm;
	std::vector<std::vector<float> > *m_pS2sPosNn;
	std::vector<std::vector<float> > *m_pS2sPosCoG; // added by Qing
	std::vector<std::vector<float> > *m_pS2sPosChi2;
	std::vector<std::vector<float> > *m_pS2sPosFann;

#ifdef ENABLE_LIQ_SCI
	std::vector<float> *m_pLiqSci;
	std::vector<float> *m_pLiqSciTail;
#endif

#ifdef ENABLE_NAI
	std::vector<float> *m_pNai;
#endif

#ifdef ENABLE_TAC
	std::vector<float> *m_pTimeOfFlight;	
#endif

	// level3 quantities, stored in T3
	float m_fcS1Tot;

	std::vector<float> *m_pcS1sTot;

	float m_fcS2Tot;
	float m_fcS2TotTop;
	float m_fcS2TotBottom;

	std::vector<float> *m_pcS2sTot;
	std::vector<float> *m_pcS2sTotTop;
	std::vector<float> *m_pcS2sTotBottom;

	std::vector<std::vector<float> > *m_pS1sPmtMaxHeight;
	std::vector<std::vector<float> > *m_pS1sBeforeMaximum;
	std::vector<std::vector<float> > *m_pS1sAfterMaximum;

	std::vector<std::vector<float> > *m_pS1sEntropy;

	// waveforms, not stored in the trees
	std::map<int, Waveform *> *m_pWaveforms;
	std::map<int, Waveform *> *m_pWaveforms_tdc;
	Waveform *m_pTotalWaveform;
	Waveform *m_pTotalWaveformVeto;
	Waveform *m_pS1PeakFindingTotalWaveform;
	Waveform *m_pS2PeakFindingTotalWaveform;
	Waveform *m_pS1PeakFindingTotalWaveformVeto;

	friend class AnalysisManager;
};

std::ostream& operator<<(std::ostream& hOut, const EventData &hEventData);

#endif // __EVENTDATA_H__

