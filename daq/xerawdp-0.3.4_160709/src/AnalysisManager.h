#ifndef __ANALYSISMANAGER_H__
#define __ANALYSISMANAGER_H__

#include "xerawdp.h"

#include <stdint.h>

#include <string>

#include "XmlConfig.h"

class TFile;
class TTree;
class PeakFinder;
class SignalFitter;
class PositionReconstruction;
class SignalCorrections;
class EventData;
class Waveform;

class Branch
{
public:
	Branch(const std::string &hName, const std::string &hType, void *pAddress, const std::string &hDescription = "");
	~Branch();

public:
	const std::string &getName() const { return m_hName; }
	const std::string &getType() const { return m_hType; }
	void *getAddress() const { return m_pAddress; }
	const std::string &getDescription() const { return m_hDescription; }

private:
	std::string m_hName;
	std::string m_hType;
	void *m_pAddress;
	std::string m_hDescription;
};

class AnalysisManager
{
public:
	AnalysisManager(const XmlConfig &hXmlConfig, EventData *pEventData);
	~AnalysisManager();

public:
	static int getRootFileNbEvents(const std::string &hRootFileName);
	bool openRootFileUpdate(const std::string &hRootFileName);
	bool openRootFileRecreate(const std::string &hRootFileName);
	int getNbEvents();
	void setEventIndex(int iEventIndex) { m_iEventIndex = iEventIndex; }

	bool needRawData();

	void analyze();
	void write();

	void closeRootFile();

private:
	void describeLevel0Tree();
	void describeLevel1Tree();
	void describeLevel2Tree();
	void describeLevel3Tree();

	void createLevel0Tree();
	void createLevel1Tree();
	void createLevel2Tree();
	void createLevel3Tree();

	bool verifyLevel1Tree();
	bool verifyLevel2Tree();
	bool verifyLevel3Tree();

	void getSingles();

	void sumWaveforms();
	void findPeaks();
	void getSignalsRaw();
	void getSignals();
	void reconstructPosition();
	void correctSignals();

	bool isTopPmt(int iPmt);
	bool isBottomPmt(int iPmt);
	bool isTopVetoPmt(int iPmt);
	bool isBottomVetoPmt(int iPmt);

public:
	static const float m_fInputImpedance = 50.;
	static const float m_fElectricCharge = 1.60217733e-19;
	
	int getUseTDC() const { return m_iUseTDC; }

private:
	static const int64_t m_iMaxTreeSize = (int64_t) 10e9;

	std::string m_hConfigFileContents;

	int m_iVerbosity;

	int m_iProcessingLevel;
	int m_iUseTDC;

	int m_iFirstTopPmt, m_iLastTopPmt;
	int m_iFirstBottomPmt, m_iLastBottomPmt;
	int m_iFirstTopVetoPmt, m_iLastTopVetoPmt;
	int m_iFirstBottomVetoPmt, m_iLastBottomVetoPmt;

	int m_iNbPmts;
	int m_iNbVetoPmts;

	ExternalGainsTable const *m_pExternalGainsTable;

	std::string m_hPmtCalibrationFiles;
	PmtCalibrationTable const *m_pPmtCalibrationTable;

	int m_iNbTriggerSignals;
	int m_iFirstTriggerEfficiencyChannel;
	int m_iTriggerWindowStart;
	int m_iTriggerWindowWidth;
	float m_fTriggerSignalHeightThreshold;

#ifdef ENABLE_LIQ_SCI
	int m_iNbLiqScis;
	int m_iFirstLiqSciChannel;
	int m_iLiqSciTailStart;
	bool m_bLiqSciMultiplexed;
	int m_iLiqSciMultiplexingStart;
	float m_fLiqSciMultiplexingSignalHeightThreshold;
	int m_iLiqSciMultiplexingSignalWidthThreshold;

	LiqSciMultiplexingTable const *m_pLiqSciMultiplexingTable;
	
	int m_iPmtToIntegrate;
	int m_iSamplesBeforeLiqSciPeak;
	int m_iWindowForS1Integral;
	int m_fThresholdForS1Integral;
	
#endif

#ifdef ENABLE_NAI
	int m_iNbNaiDetectors;
	int m_iFirstNaiChannel;
	int m_iLastNaiChannel;
#endif

#ifdef ENABLE_TRIG
	int m_iFirstTrigChannel;
	int m_iLastTrigChannel;
	float m_fTrigSignalThresholdHeight;
	float m_fTrigSignalThresholdArea;
#endif

#ifdef ENABLE_HPGE
	int m_iNbGeDetectors;
	int m_iFirstGeChannel;
	int m_iLastGeChannel;
#endif

#ifdef ENABLE_TAC
	int m_iS1Delay;
	int m_iS1DelayTime;
	int m_iNbTacs;
	int m_iFirstTacChannel;
	int m_iTacWindowStart;
	int m_iTacWindowWidth;

	TacCalibrationTable const *m_pTacCalibrationTable;
#endif

	int m_iSingleNbBaselineSamples;
	int m_iSingleWindowStart;
	int m_iSingleWindowWidth;
	int m_iSinglePrePeakSamples;
	int m_iSinglePostPeakSamples;
	SingleIntegralTable const *m_pSingleIntegralTable;

	PmtSet const *m_pS1PeakFindingExcludedPmtSet;
	PmtSet const *m_pS2PeakFindingExcludedPmtSet;

	float m_fSignalThreshold;
	float m_fSaturationThreshold;
	PmtSet const *m_pS1CoincidenceExcludedPmtSet;
	PmtSet const *m_pS1TotalSignalExcludedPmtSet;
	int m_iS1CoincidenceWindow;
	PmtSet const *m_pS2CoincidenceExcludedPmtSet;
	PmtSet const *m_pS2TotalSignalExcludedPmtSet;

	float m_fFitThreshold;

	bool m_bUseMaxPmtAlgorithm;
	bool m_bUseSvmAlgorithm;
	bool m_bUseNnAlgorithm;
	bool m_bUseChi2Algorithm;
	bool m_bUseFannAlgorithm;

	bool m_bCorrectForS1SpatialDependence;
	bool m_bCorrectForS2AxialDependence;
	float m_fElectronLifetime;

	bool m_bRedoLevel1Processing, m_bRedoLevel2Processing, m_bRedoLevel3Processing;

	int m_iEventIndex;

	EventData *m_pEventData;

	std::vector<Branch> *m_pLevel0Branches, *m_pLevel1Branches, *m_pLevel2Branches, *m_pLevel3Branches;

	TFile *m_pRootFile;
	TTree *m_pLevel0Tree, *m_pLevel1Tree, *m_pLevel2Tree, *m_pLevel3Tree;

	PeakFinder *m_pPeakFinder;
	SignalFitter *m_pSignalFitter;
	PositionReconstruction *m_pPositionReconstruction;
	SignalCorrections *m_pSignalCorrections;
};

#endif // __ANALYSISMANAGER_H__

