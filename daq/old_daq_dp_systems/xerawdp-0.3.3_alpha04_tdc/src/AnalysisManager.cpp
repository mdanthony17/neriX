/*!
 * @file AnalysisManager.cpp
 * @author Guillaume Plante
 * @date 03-28-08
 */
#include "AnalysisManager.h"

#include "EventData.h"
#include "Waveform.h"
#include "PeakFinder.h"
#include "SignalFitter.h"
#include "PositionReconstruction.h"
#include "SignalCorrections.h"
#include "RawData.h"

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TObjString.h>

#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <cassert>
#include <utility>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <math.h>

#include <sys/stat.h>

using std::cout;
using std::endl;
using std::map;
using std::vector;
using std::list;
using std::string;
using std::stringstream;
using std::pair;
using std::sqrt;
using std::min;
using std::max;
using std::accumulate;
using std::find_if;
using std::greater;

Branch::Branch(const std::string &hName, const std::string &hType, void *pAddress, const std::string &hDescription)
{
	m_hName = hName;
	m_hType = hType;
	m_pAddress = pAddress;
	m_hDescription = hDescription;
}

Branch::~Branch()
{
}

/*! @brief AnalysisManager constructor 
 *
 * @param hXmlConfig
 * @param pEventData
 */
AnalysisManager::AnalysisManager(const XmlConfig &hXmlConfig, EventData *pEventData)
{
	m_hConfigFileContents = hXmlConfig.getConfigFileContents();

	m_iVerbosity = hXmlConfig.getVerbosity();

	m_iProcessingLevel = hXmlConfig.getProcessingLevel();
	m_iUseTDC = hXmlConfig.getUseTDC();

	m_iFirstTopPmt = hXmlConfig.getFirstTopPmt();
	m_iLastTopPmt = hXmlConfig.getLastTopPmt();
	m_iFirstBottomPmt = hXmlConfig.getFirstBottomPmt();
	m_iLastBottomPmt = hXmlConfig.getLastBottomPmt();
	m_iFirstTopVetoPmt = hXmlConfig.getFirstTopVetoPmt();
	m_iLastTopVetoPmt = hXmlConfig.getLastTopVetoPmt();
	m_iFirstBottomVetoPmt = hXmlConfig.getFirstBottomVetoPmt();
	m_iLastBottomVetoPmt = hXmlConfig.getLastBottomVetoPmt();

	m_iNbPmts = hXmlConfig.getNbPmts();
	m_iNbVetoPmts = hXmlConfig.getNbTopVetoPmts()+hXmlConfig.getNbBottomVetoPmts();

	m_pExternalGainsTable = hXmlConfig.getExternalGainsTable();

	m_hPmtCalibrationFiles = hXmlConfig.getPmtCalibrationFiles();
	m_pPmtCalibrationTable = hXmlConfig.getPmtCalibrationTable();

	m_iNbTriggerSignals = hXmlConfig.getNbTriggerSignals();
	m_iFirstTriggerEfficiencyChannel = hXmlConfig.getFirstTriggerEfficiencyChannel();
	m_iTriggerWindowStart = hXmlConfig.getTriggerWindowStart();
	m_iTriggerWindowWidth = hXmlConfig.getTriggerWindowWidth();
	m_fTriggerSignalHeightThreshold = hXmlConfig.getTriggerSignalHeightThreshold();

#ifdef ENABLE_LIQ_SCI
	m_iNbLiqScis = hXmlConfig.getNbLiqScis();
	m_iFirstLiqSciChannel = hXmlConfig.getFirstLiqSciChannel();
	m_iLiqSciTailStart = hXmlConfig.getLiqSciTailStart();
	m_bLiqSciMultiplexed = hXmlConfig.getLiqSciMultiplexed();
	m_iLiqSciMultiplexingStart = hXmlConfig.getLiqSciMultiplexingStart();
	m_fLiqSciMultiplexingSignalHeightThreshold = hXmlConfig.getLiqSciMultiplexingSignalHeightThreshold();
	m_iLiqSciMultiplexingSignalWidthThreshold = hXmlConfig.getLiqSciMultiplexingSignalWidthThreshold();
	m_pLiqSciMultiplexingTable = hXmlConfig.getLiqSciMultiplexingTable();
#endif

#ifdef ENABLE_NAI
	m_iNbNaiDetectors = hXmlConfig.getNbNaiDetectors();
	m_iFirstNaiChannel = hXmlConfig.getFirstNaiChannel();
	m_iLastNaiChannel = hXmlConfig.getFirstNaiChannel();
#endif

#ifdef ENABLE_HPGE
	m_iNbGeDetectors = hXmlConfig.getNbGeDetectors();
	m_iFirstGeChannel = hXmlConfig.getFirstGeChannel();
	m_iLastGeChannel = hXmlConfig.getFirstGeChannel();
#endif

#ifdef ENABLE_TAC
	m_iNbTacs = hXmlConfig.getNbTacs();
	m_iFirstTacChannel = hXmlConfig.getFirstTacChannel();
	m_iTacWindowStart = hXmlConfig.getTacWindowStart();
	m_iTacWindowWidth = hXmlConfig.getTacWindowWidth();
	m_pTacCalibrationTable = hXmlConfig.getTacCalibrationTable();
#endif

	m_iSingleNbBaselineSamples = hXmlConfig.getSingleNbBaselineSamples();
	m_iSingleWindowStart = hXmlConfig.getSingleWindowStart();
	m_iSingleWindowWidth = hXmlConfig.getSingleWindowWidth();
	m_iSinglePrePeakSamples = hXmlConfig.getSinglePrePeakSamples();
	m_iSinglePostPeakSamples = hXmlConfig.getSinglePostPeakSamples();

	m_pS1PeakFindingExcludedPmtSet = hXmlConfig.getS1PeakFindingExcludedPmtSet();
	m_pS2PeakFindingExcludedPmtSet = hXmlConfig.getS2PeakFindingExcludedPmtSet();

	m_fSignalThreshold = hXmlConfig.getSignalThreshold();
	m_fSaturationThreshold = hXmlConfig.getSaturationThreshold();
	m_pS1CoincidenceExcludedPmtSet = hXmlConfig.getS1CoincidenceExcludedPmtSet();
	m_pS1TotalSignalExcludedPmtSet = hXmlConfig.getS1TotalSignalExcludedPmtSet();
	m_iS1CoincidenceWindow = hXmlConfig.getS1CoincidenceWindow();
	m_pS2CoincidenceExcludedPmtSet = hXmlConfig.getS2CoincidenceExcludedPmtSet();
	m_pS2TotalSignalExcludedPmtSet = hXmlConfig.getS2TotalSignalExcludedPmtSet();

	m_bUseMaxPmtAlgorithm = hXmlConfig.getUseMaxPmtAlgorithm();
	m_bUseSvmAlgorithm = hXmlConfig.getUseSvmAlgorithm();
	m_bUseNnAlgorithm = hXmlConfig.getUseNnAlgorithm();
	m_bUseChi2Algorithm = hXmlConfig.getUseChi2Algorithm();
	m_bUseFannAlgorithm = hXmlConfig.getUseFannAlgorithm();

	m_bCorrectForS1SpatialDependence = hXmlConfig.getCorrectForS1SpatialDependence();
	m_bCorrectForS2AxialDependence = hXmlConfig.getCorrectForS2AxialDependence();
	m_fElectronLifetime = hXmlConfig.getElectronLifetime();

	m_pEventData = pEventData;

	m_bRedoLevel1Processing = true;
	m_bRedoLevel2Processing = true;
	m_bRedoLevel3Processing = true;

	m_pRootFile = 0;

	m_pLevel0Tree = 0;
	m_pLevel1Tree = 0;
	m_pLevel2Tree = 0;
	m_pLevel3Tree = 0;

	describeLevel0Tree();
	describeLevel1Tree();
	describeLevel2Tree();
	describeLevel3Tree();

	m_pPeakFinder = new PeakFinder(hXmlConfig);

	m_pSignalFitter = new SignalFitter();

	m_pPositionReconstruction = new PositionReconstruction(hXmlConfig);

	m_pSignalCorrections = new SignalCorrections();
}

AnalysisManager::~AnalysisManager()
{
	delete m_pLevel1Branches;
	delete m_pLevel2Branches;
	delete m_pLevel3Branches;

	if(m_pRootFile)
		delete m_pRootFile;

	if(m_pPeakFinder)
		delete m_pPeakFinder;

	if(m_pPositionReconstruction)
		delete m_pPositionReconstruction;
}

int
AnalysisManager::getRootFileNbEvents(const std::string &hRootFileName)
{
	TFile hFile(hRootFileName.c_str(), "read");
	TTree *pTree = (TTree *) hFile.Get("T1");
	int iNbEvents = pTree->GetEntries();
	hFile.Close();

	return iNbEvents;
}

/*!
 * @param
 * @return
 */
bool
AnalysisManager::openRootFileUpdate(const std::string &hRootFileName)
{
	m_pRootFile = new TFile(hRootFileName.c_str(), "update", "XENON100 Data");

	gROOT->ProcessLine("#include <vector>");
	gROOT->ProcessLine("vector<vector<float>> x;");

	// get the xerawdp version and configuration that were used to process the file
	TObjString *pPreviousVersion = (TObjString *) m_pRootFile->Get("xerawdp_version");
	TObjString *pPreviousConfig = (TObjString *) m_pRootFile->Get("xerawdp_xml_config");

	// create the level 0 tree
	if(m_iProcessingLevel == 0)
	{
		createLevel1Tree();
	}

	// create (or verify) the level 1 tree
	if(m_iProcessingLevel >= 1)
	{
		if(((m_pLevel1Tree = (TTree *) m_pRootFile->Get("T1")) == 0) || (verifyLevel1Tree() == false))
			createLevel1Tree();
		else
		{
			// we are not redoing the level 1 processing
			m_bRedoLevel1Processing = false;

			// save the previous xerawdp version and configuration for T1 unless it has it already
			if((m_pRootFile->Get("xerawdp_version_T1") == 0) && (m_pRootFile->Get("xerawdp_xml_config_T1") == 0))
			{
				pPreviousVersion->Write("xerawdp_version_T1");
				pPreviousConfig->Write("xerawdp_xml_config_T1");
			}
		}
	}
	
	// create (or verify) the level 2 tree
	if(m_iProcessingLevel >= 2)
	{
		if(((m_pLevel2Tree = (TTree *) m_pRootFile->Get("T2")) == 0) || (verifyLevel2Tree() == false))
			createLevel2Tree();
		else
		{
			// we are not redoing the level 2 processing
			m_bRedoLevel2Processing = false;

			// save the previous xerawdp version and configuration for T2 unless it has it already
			if((m_pRootFile->Get("xerawdp_version_T2") == 0) && (m_pRootFile->Get("xerawdp_xml_config_T2") == 0))
			{
				pPreviousVersion->Write("xerawdp_version_T2");
				pPreviousConfig->Write("xerawdp_xml_config_T2");
			}
		}
	}
	
	// create (or verify) the level 3 tree
	if(m_iProcessingLevel >= 3)
	{
		if(((m_pLevel3Tree = (TTree *) m_pRootFile->Get("T3")) == 0) || (verifyLevel3Tree() == false))
			createLevel3Tree();
		else
		{
			// we are not redoing the level 2 processing
			m_bRedoLevel3Processing = false;

			// save the previous xerawdp version and configuration for T3 unless it has it already
			if((m_pRootFile->Get("xerawdp_version_T3") == 0) && (m_pRootFile->Get("xerawdp_xml_config_T3") == 0))
			{
				pPreviousVersion->Write("xerawdp_version_T3");
				pPreviousConfig->Write("xerawdp_xml_config_T3");
			}
		}
	}

	// save the xerawdp version and configuration in the root file
	TObjString hVersion(VERSION);
	hVersion.Write("xerawdp_version", TObject::kOverwrite);

	TObjString hConfig(m_hConfigFileContents.c_str());
	hConfig.Write("xerawdp_xml_config",  TObject::kOverwrite);
}

/*!
 * @param
 * @return
 */
bool
AnalysisManager::openRootFileRecreate(const std::string &hRootFileName)
{
	m_pRootFile = new TFile(hRootFileName.c_str(), "recreate", "XENON100 Data");

	gROOT->ProcessLine("#include <vector>");
	gROOT->ProcessLine("vector<vector<float>> x;");

	// make the processed file group writeable
	chmod(hRootFileName.c_str(), S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);

	// save the xerawdp version in the file
	TObjString hVersion(VERSION);
	hVersion.Write("xerawdp_version");

	TObjString hConfig(m_hConfigFileContents.c_str());
	hConfig.Write("xerawdp_xml_config");

	if(m_iProcessingLevel == 0)
		createLevel0Tree();

	if(m_iProcessingLevel >= 1)
		createLevel1Tree();

	if(m_iProcessingLevel >= 2)
		createLevel2Tree();

	if(m_iProcessingLevel >= 3)
		createLevel3Tree();
}

int
AnalysisManager::getNbEvents()
{
	if(m_pLevel1Tree)
		return m_pLevel1Tree->GetEntries();
}

bool
AnalysisManager::needRawData()
{
	return (m_iProcessingLevel == 0) || m_bRedoLevel1Processing;
}

/*! @brief Analyze
 *
 *
 */
void
AnalysisManager::analyze()
{
	// level 0 processing
	if(m_iProcessingLevel == 0)
	{
		getSingles();
	}

	// level 1 processing
	if(m_iProcessingLevel >= 1)
	{
		// if we have to redo the level 1 processing
		if(m_bRedoLevel1Processing)
		{
			// if the event has waveforms
			if(m_pEventData->m_pWaveforms->size())
			{
				// sum up waveforms
				sumWaveforms();

				// find peaks
				findPeaks();	

				// get signals
				getSignalsRaw();
			}
		}
		// otherwise just get the values from the tree
		else
			m_pLevel1Tree->GetEntry(m_iEventIndex);
	}

	// level 2 processing
	if(m_iProcessingLevel >= 2)
	{
		// if we have to redo the level 2 processing
		if(m_bRedoLevel2Processing)
		{
			// get signals
			getSignals();

			// reconstruct position
			reconstructPosition();
		}
		else
			m_pLevel2Tree->GetEntry(m_iEventIndex);
	}

	// level 3 processing
	if(m_iProcessingLevel >= 3)
	{
		if(m_bRedoLevel3Processing)
		{
			// correct signals
			correctSignals();
		}
		else
			m_pLevel3Tree->GetEntry(m_iEventIndex);
	}

	if(m_iVerbosity >= 3)
		cout << *m_pEventData;
}

/*! @brief Write all the event data to the root trees.
 *
 * This function
 */
void
AnalysisManager::write()
{
	if(m_iProcessingLevel == 0)
		m_pLevel0Tree->Fill();

	if(m_iProcessingLevel >= 1 && m_bRedoLevel1Processing)
		m_pLevel1Tree->Fill();

	if(m_iProcessingLevel >= 2 && m_bRedoLevel2Processing)
		m_pLevel2Tree->Fill();

	if(m_iProcessingLevel >= 3 && m_bRedoLevel3Processing)
		m_pLevel3Tree->Fill();
}

void
AnalysisManager::closeRootFile()
{
	m_pRootFile->Write("", TObject::kOverwrite);
	m_pRootFile->Close();
}

void
AnalysisManager::describeLevel0Tree()
{
	m_pLevel0Branches = new vector<Branch>;

	m_pLevel0Branches->push_back(Branch("Filename", "", &m_pEventData->m_szFilename, "Filename/C"));
	m_pLevel0Branches->push_back(Branch("EventId", "", &m_pEventData->m_iEventId, "EventId/I"));
	m_pLevel0Branches->push_back(Branch("TimeSec", "", &m_pEventData->m_iTimeSec, "TimeSec/I"));
	m_pLevel0Branches->push_back(Branch("TimeMicroSec", "", &m_pEventData->m_iTimeMicroSec, "TimeMicroSec/I"));

	m_pLevel0Branches->push_back(Branch("Channels", "vector<int>", &m_pEventData->m_pChannels));
	m_pLevel0Branches->push_back(Branch("BaselinesAvg", "vector<vector<float> >", &m_pEventData->m_pBaselinesAvg));
	m_pLevel0Branches->push_back(Branch("BaselinesMax", "vector<vector<float> >", &m_pEventData->m_pBaselinesMax));

	m_pLevel0Branches->push_back(Branch("SinglePeak", "vector<float>", &m_pEventData->m_pSinglePeak));
	m_pLevel0Branches->push_back(Branch("SingleHeight", "vector<float>", &m_pEventData->m_pSingleHeight));
	m_pLevel0Branches->push_back(Branch("SingleSample", "vector<int>", &m_pEventData->m_pSingleSample));
	m_pLevel0Branches->push_back(Branch("SingleBefore", "vector<vector<float> >", &m_pEventData->m_pSingleBefore));
	m_pLevel0Branches->push_back(Branch("SingleAfter", "vector<vector<float> >", &m_pEventData->m_pSingleAfter));

	m_pLevel0Branches->push_back(Branch("SingleFixed", "vector<vector<float> >", &m_pEventData->m_pSingleFixed));
	
}

void
AnalysisManager::describeLevel1Tree()
{
	m_pLevel1Branches = new vector<Branch>;

	m_pLevel1Branches->push_back(Branch("Filename", "", &m_pEventData->m_szFilename, "Filename/C"));
	m_pLevel1Branches->push_back(Branch("EventId", "", &m_pEventData->m_iEventId, "EventId/I"));
	m_pLevel1Branches->push_back(Branch("TimeSec", "", &m_pEventData->m_iTimeSec, "TimeSec/I"));
	m_pLevel1Branches->push_back(Branch("TimeMicroSec", "", &m_pEventData->m_iTimeMicroSec, "TimeMicroSec/I"));

	m_pLevel1Branches->push_back(Branch("Channels", "vector<int>", &m_pEventData->m_pChannels));
	m_pLevel1Branches->push_back(Branch("BaselinesAvg", "vector<vector<float> >", &m_pEventData->m_pBaselinesAvg));
	m_pLevel1Branches->push_back(Branch("BaselinesMax", "vector<vector<float> >", &m_pEventData->m_pBaselinesMax));

	m_pLevel1Branches->push_back(Branch("AreaTotRaw", "", &m_pEventData->m_fAreaTotRaw, "AreaTotRaw/F"));
	if(m_iNbVetoPmts)
		m_pLevel1Branches->push_back(Branch("AreaTotRawVeto", "", &m_pEventData->m_fAreaTotRawVeto, "AreaTotRawVeto/F"));
	m_pLevel1Branches->push_back(Branch("AreaRaw", "vector<float>", &m_pEventData->m_pAreaRaw));

	m_pLevel1Branches->push_back(Branch("AbsAreaTotRaw", "", &m_pEventData->m_fAbsAreaTotRaw, "AbsAreaTotRaw/F"));
	m_pLevel1Branches->push_back(Branch("AbsAreaRaw", "vector<float>", &m_pEventData->m_pAbsAreaRaw));

	m_pLevel1Branches->push_back(Branch("NbNonZeroLast", "", &m_pEventData->m_iNbNonZeroLast, "NbNonZeroLast/I"));

	m_pLevel1Branches->push_back(Branch("NbS1Peaks", "", &m_pEventData->m_iNbS1Peaks, "NbS1Peaks/I"));
	m_pLevel1Branches->push_back(Branch("S1sPeak", "vector<int>", &m_pEventData->m_pS1sPeak));
	m_pLevel1Branches->push_back(Branch("S1sLeftEdge", "vector<int>", &m_pEventData->m_pS1sLeftEdge));
	m_pLevel1Branches->push_back(Branch("S1sRightEdge", "vector<int>", &m_pEventData->m_pS1sRightEdge));
	m_pLevel1Branches->push_back(Branch("S1sWidth", "vector<float>", &m_pEventData->m_pS1sWidth));
	m_pLevel1Branches->push_back(Branch("S1sLowWidth", "vector<float>", &m_pEventData->m_pS1sLowWidth));
	m_pLevel1Branches->push_back(Branch("S1sFilteredLowWidth", "vector<float>", &m_pEventData->m_pS1sFilteredLowWidth));
	m_pLevel1Branches->push_back(Branch("S1sHeight", "vector<float>", &m_pEventData->m_pS1sHeight));
	m_pLevel1Branches->push_back(Branch("S1sMinimum", "vector<float>", &m_pEventData->m_pS1sMinimum));
	m_pLevel1Branches->push_back(Branch("S1sMeanTime", "vector<float>", &m_pEventData->m_pS1sMeanTime));
	m_pLevel1Branches->push_back(Branch("S1sTimeSpread", "vector<float>", &m_pEventData->m_pS1sTimeSpread));
	m_pLevel1Branches->push_back(Branch("S1sSaturation", "vector<int>", &m_pEventData->m_pS1sSaturation));

	m_pLevel1Branches->push_back(Branch("S1Order", "vector<int>", &m_pEventData->m_pS1Order));
	m_pLevel1Branches->push_back(Branch("S1SizeOrder", "vector<int>", &m_pEventData->m_pS1SizeOrder));

	m_pLevel1Branches->push_back(Branch("NbS2Peaks", "", &m_pEventData->m_iNbS2Peaks, "NbS2Peaks/I"));
	m_pLevel1Branches->push_back(Branch("S2sPeak", "vector<int>", &m_pEventData->m_pS2sPeak));
	m_pLevel1Branches->push_back(Branch("S2sLeftEdge", "vector<int>", &m_pEventData->m_pS2sLeftEdge));
	m_pLevel1Branches->push_back(Branch("S2sRightEdge", "vector<int>", &m_pEventData->m_pS2sRightEdge));
	m_pLevel1Branches->push_back(Branch("S2sWidth", "vector<float>", &m_pEventData->m_pS2sWidth));
	m_pLevel1Branches->push_back(Branch("S2sLowWidth", "vector<float>", &m_pEventData->m_pS2sLowWidth));
	m_pLevel1Branches->push_back(Branch("S2sHeight", "vector<float>", &m_pEventData->m_pS2sHeight));
	m_pLevel1Branches->push_back(Branch("S2sSaturation", "vector<int>", &m_pEventData->m_pS2sSaturation));
	//m_pLevel1Branches->push_back(Branch("S2sTrigger", "vector<int>", &m_pEventData->m_pS2sTrigger));
	m_pLevel1Branches->push_back(Branch("CoinRaw", "vector<float>", &m_pEventData->m_pCoinRaw));
	if(m_iUseTDC)
	{
		m_pLevel1Branches->push_back(Branch("TimeTDC", "vector<float>", &m_pEventData->m_pTimeTDC));
	}

	m_pLevel1Branches->push_back(Branch("S2Order", "vector<int>", &m_pEventData->m_pS2Order));

	if(m_iNbVetoPmts)
	{
		m_pLevel1Branches->push_back(Branch("NbS1PeaksVeto", "", &m_pEventData->m_iNbS1PeaksVeto, "NbS1PeaksVeto/I"));
		m_pLevel1Branches->push_back(Branch("S1sPeakVeto", "vector<int>", &m_pEventData->m_pS1sPeakVeto));
		m_pLevel1Branches->push_back(Branch("S1sLeftEdgeVeto", "vector<int>", &m_pEventData->m_pS1sLeftEdgeVeto));
		m_pLevel1Branches->push_back(Branch("S1sRightEdgeVeto", "vector<int>", &m_pEventData->m_pS1sRightEdgeVeto));
		m_pLevel1Branches->push_back(Branch("S1sWidthVeto", "vector<float>", &m_pEventData->m_pS1sWidthVeto));
		m_pLevel1Branches->push_back(Branch("S1sLowWidthVeto", "vector<float>", &m_pEventData->m_pS1sLowWidthVeto));
		m_pLevel1Branches->push_back(Branch("S1sHeightVeto", "vector<float>", &m_pEventData->m_pS1sHeightVeto));

		m_pLevel1Branches->push_back(Branch("S1OrderVeto", "vector<int>", &m_pEventData->m_pS1OrderVeto));
	}

	m_pLevel1Branches->push_back(Branch("S1sRaw", "vector<vector<float> >", &m_pEventData->m_pS1sRaw));
	m_pLevel1Branches->push_back(Branch("S2sRaw", "vector<vector<float> >", &m_pEventData->m_pS2sRaw));
	if(m_iNbVetoPmts)
		m_pLevel1Branches->push_back(Branch("S1sRawVeto", "vector<vector<float> >", &m_pEventData->m_pS1sRawVeto));

	m_pLevel1Branches->push_back(Branch("S1FitParameters", "vector<vector<float> >", &m_pEventData->m_pS1FitParameters));

#ifdef ENABLE_LIQ_SCI
	m_pLevel1Branches->push_back(Branch("LiqSciPeak", "vector<int>", &m_pEventData->m_pLiqSciPeak));
	m_pLevel1Branches->push_back(Branch("LiqSciLeftEdge", "vector<int>", &m_pEventData->m_pLiqSciLeftEdge));
	m_pLevel1Branches->push_back(Branch("LiqSciRightEdge", "vector<int>", &m_pEventData->m_pLiqSciRightEdge));
	m_pLevel1Branches->push_back(Branch("LiqSciWidth", "vector<float>", &m_pEventData->m_pLiqSciWidth));
	m_pLevel1Branches->push_back(Branch("LiqSciLowWidth", "vector<float>", &m_pEventData->m_pLiqSciLowWidth));
	m_pLevel1Branches->push_back(Branch("LiqSciHeight", "vector<float>", &m_pEventData->m_pLiqSciHeight));

	m_pLevel1Branches->push_back(Branch("LiqSciRaw", "vector<float>", &m_pEventData->m_pLiqSciRaw));
	m_pLevel1Branches->push_back(Branch("LiqSciTailRaw", "vector<float>", &m_pEventData->m_pLiqSciTailRaw));
#endif

#ifdef ENABLE_NAI
	m_pLevel1Branches->push_back(Branch("NaiPeak", "vector<int>", &m_pEventData->m_pNaiPeak));
	m_pLevel1Branches->push_back(Branch("NaiLeftEdge", "vector<int>", &m_pEventData->m_pNaiLeftEdge));
	m_pLevel1Branches->push_back(Branch("NaiRightEdge", "vector<int>", &m_pEventData->m_pNaiRightEdge));
	m_pLevel1Branches->push_back(Branch("NaiWidth", "vector<float>", &m_pEventData->m_pNaiWidth));
	m_pLevel1Branches->push_back(Branch("NaiLowWidth", "vector<float>", &m_pEventData->m_pNaiLowWidth));
	m_pLevel1Branches->push_back(Branch("NaiHeight", "vector<float>", &m_pEventData->m_pNaiHeight));

	m_pLevel1Branches->push_back(Branch("NaiRaw", "vector<float>", &m_pEventData->m_pNaiRaw));
#endif

#ifdef ENABLE_HPGE
	m_pLevel1Branches->push_back(Branch("GeHeight", "vector<float>", &m_pEventData->m_pGeHeight));
	m_pLevel1Branches->push_back(Branch("TrigArea", "vector<float>", &m_pEventData->m_pTrigArea));
	m_pLevel1Branches->push_back(Branch("TrigLeftEdge", "vector<int>", &m_pEventData->m_pTrigLeftEdge));
#endif

#ifdef ENABLE_TAC
	m_pLevel1Branches->push_back(Branch("TacHeight", "vector<float>", &m_pEventData->m_pTacHeight));
	m_pLevel1Branches->push_back(Branch("TacMaximum", "vector<float>", &m_pEventData->m_pTacMaximum));
#endif
}

void
AnalysisManager::describeLevel2Tree()
{
	m_pLevel2Branches = new vector<Branch>;

	m_pLevel2Branches->push_back(Branch("PmtCalibrationFiles", "", &m_pEventData->m_szPmtCalibrationFiles, "PmtCalibrationFiles/C"));

	m_pLevel2Branches->push_back(Branch("AreaTot", "", &m_pEventData->m_fAreaTot, "AreaTot/F"));
	m_pLevel2Branches->push_back(Branch("AreaTotVeto", "", &m_pEventData->m_fAreaTotVeto, "AreaTotVeto/F"));
	m_pLevel2Branches->push_back(Branch("Area", "vector<float>", &m_pEventData->m_pArea));

	m_pLevel2Branches->push_back(Branch("AbsAreaTot", "", &m_pEventData->m_fAbsAreaTot, "AbsAreaTot/F"));
	m_pLevel2Branches->push_back(Branch("AbsArea", "vector<float>", &m_pEventData->m_pAbsArea));

	m_pLevel2Branches->push_back(Branch("S1Tot", "", &m_pEventData->m_fS1Tot, "S1Tot/F"));
	m_pLevel2Branches->push_back(Branch("S1TotTop", "", &m_pEventData->m_fS1TotTop, "S1TotTop/F"));
	m_pLevel2Branches->push_back(Branch("S1TotBottom", "", &m_pEventData->m_fS1TotBottom, "S1TotBottom/F"));

	m_pLevel2Branches->push_back(Branch("S1s", "vector<vector<float> >", &m_pEventData->m_pS1s));

	m_pLevel2Branches->push_back(Branch("S1sTot", "vector<float>", &m_pEventData->m_pS1sTot));
	m_pLevel2Branches->push_back(Branch("S1sTotTop", "vector<float>", &m_pEventData->m_pS1sTotTop));
	m_pLevel2Branches->push_back(Branch("S1sTotBottom", "vector<float>", &m_pEventData->m_pS1sTotBottom));

	m_pLevel2Branches->push_back(Branch("fS1sTot", "vector<float>", &m_pEventData->m_pfS1sTot));

	m_pLevel2Branches->push_back(Branch("S2Tot", "", &m_pEventData->m_fS2Tot, "S2Tot/F"));
	m_pLevel2Branches->push_back(Branch("S2TotTop", "", &m_pEventData->m_fS2TotTop, "S2TotTop/F"));
	m_pLevel2Branches->push_back(Branch("S2TotBottom", "", &m_pEventData->m_fS2TotBottom, "S2TotBottom/F"));
	m_pLevel2Branches->push_back(Branch("S2TotAfter", "", &m_pEventData->m_fS2TotAfter, "S2TotAfter/F"));

	m_pLevel2Branches->push_back(Branch("S2s", "vector<vector<float> >", &m_pEventData->m_pS2s));

	m_pLevel2Branches->push_back(Branch("S2sTot", "vector<float>", &m_pEventData->m_pS2sTot));
	m_pLevel2Branches->push_back(Branch("S2sTotTop", "vector<float>", &m_pEventData->m_pS2sTotTop));
	m_pLevel2Branches->push_back(Branch("S2sTotBottom", "vector<float>", &m_pEventData->m_pS2sTotBottom));

	if(m_iNbVetoPmts)
	{
		m_pLevel2Branches->push_back(Branch("S1TotVeto", "", &m_pEventData->m_fS1TotVeto, "S1TotVeto/F"));
		m_pLevel2Branches->push_back(Branch("S1sVeto", "vector<vector<float> >", &m_pEventData->m_pS1sVeto));
		m_pLevel2Branches->push_back(Branch("S1sTotVeto", "vector<float>", &m_pEventData->m_pS1sTotVeto));
	}

	m_pLevel2Branches->push_back(Branch("S1sCoin", "vector<int>", &m_pEventData->m_pS1sCoin));
	m_pLevel2Branches->push_back(Branch("S1sCoinTop", "vector<int>", &m_pEventData->m_pS1sCoinTop));
	m_pLevel2Branches->push_back(Branch("S1sCoinBottom", "vector<int>", &m_pEventData->m_pS1sCoinBottom));

	m_pLevel2Branches->push_back(Branch("S2sCoin", "vector<int>", &m_pEventData->m_pS2sCoin));
	m_pLevel2Branches->push_back(Branch("S2sCoinTop", "vector<int>", &m_pEventData->m_pS2sCoinTop));
	m_pLevel2Branches->push_back(Branch("S2sCoinBottom", "vector<int>", &m_pEventData->m_pS2sCoinBottom));

	if(m_iNbVetoPmts)
		m_pLevel2Branches->push_back(Branch("S1sCoinVeto", "vector<int>", &m_pEventData->m_pS1sCoinVeto));

	m_pLevel2Branches->push_back(Branch("S1sPmtCoin", "vector<vector<int> >",  &m_pEventData->m_pS1sPmtCoin));
	m_pLevel2Branches->push_back(Branch("S1sPmtOrder", "vector<vector<int> >", &m_pEventData->m_pS1sPmtOrder));
	m_pLevel2Branches->push_back(Branch("S2sPmtOrder", "vector<vector<int> >", &m_pEventData->m_pS2sPmtOrder));

	if(m_bUseMaxPmtAlgorithm)
		m_pLevel2Branches->push_back(Branch("S2sPosMaxPmt", "vector<vector<float> >", &m_pEventData->m_pS2sPosMaxPmt));

	if(m_bUseSvmAlgorithm)
		m_pLevel2Branches->push_back(Branch("S2sPosSvm", "vector<vector<float> >", &m_pEventData->m_pS2sPosSvm));

	if(m_bUseNnAlgorithm)
		m_pLevel2Branches->push_back(Branch("S2sPosNn", "vector<vector<float> >", &m_pEventData->m_pS2sPosNn));

	if(m_bUseChi2Algorithm)
		m_pLevel2Branches->push_back(Branch("S2sPosChi2", "vector<vector<float> >", &m_pEventData->m_pS2sPosChi2));

	if(m_bUseFannAlgorithm)
		m_pLevel2Branches->push_back(Branch("S2sPosFann", "vector<vector<float> >", &m_pEventData->m_pS2sPosFann));

#ifdef ENABLE_LIQ_SCI
	m_pLevel2Branches->push_back(Branch("LiqSci", "vector<float>", &m_pEventData->m_pLiqSci));
	m_pLevel2Branches->push_back(Branch("LiqSciTail", "vector<float>", &m_pEventData->m_pLiqSciTail));
#endif

#ifdef ENABLE_NAI
	m_pLevel2Branches->push_back(Branch("Nai", "vector<float>", &m_pEventData->m_pNai));
#endif

#ifdef ENABLE_TAC
	m_pLevel2Branches->push_back(Branch("TimeOfFlight", "vector<float>", &m_pEventData->m_pTimeOfFlight));
#endif
}

void
AnalysisManager::describeLevel3Tree()
{
	m_pLevel3Branches = new vector<Branch>;

	if(m_bCorrectForS1SpatialDependence)
	{
		m_pLevel3Branches->push_back(Branch("cS1Tot", "", &m_pEventData->m_fcS1Tot, "cS1Tot/F"));
		m_pLevel3Branches->push_back(Branch("cS1sTot", "vector<float>", &m_pEventData->m_pcS1sTot));
	}

	if(m_bCorrectForS2AxialDependence)
	{
		m_pLevel3Branches->push_back(Branch("cS2Tot", "", &m_pEventData->m_fcS2Tot, "cS2Tot/F"));
		m_pLevel3Branches->push_back(Branch("cS2TotTop", "", &m_pEventData->m_fcS2TotTop, "cS2TotTop/F"));
		m_pLevel3Branches->push_back(Branch("cS2TotBottom", "", &m_pEventData->m_fcS2TotBottom, "cS2TotBottom/F"));
		m_pLevel3Branches->push_back(Branch("cS2sTot", "vector<float>", &m_pEventData->m_pcS2sTot));
		m_pLevel3Branches->push_back(Branch("cS2sTotTop", "vector<float>", &m_pEventData->m_pcS2sTotTop));
		m_pLevel3Branches->push_back(Branch("cS2sTotBottom", "vector<float>", &m_pEventData->m_pcS2sTotBottom));
	}
}

void
AnalysisManager::createLevel0Tree()
{
	m_pLevel0Tree = new TTree("T0", "Level0 Quantities");
	m_pLevel0Tree->SetMaxTreeSize(m_iMaxTreeSize);

	vector<Branch>::iterator pBranch;
	for(pBranch = m_pLevel0Branches->begin(); pBranch != m_pLevel0Branches->end(); pBranch++)
	{
		if(pBranch->getDescription() == "")
			m_pLevel0Tree->Branch(pBranch->getName().c_str(), pBranch->getType().c_str(), pBranch->getAddress());
		else
			m_pLevel0Tree->Branch(pBranch->getName().c_str(), pBranch->getAddress(), pBranch->getDescription().c_str());
	}
}

void
AnalysisManager::createLevel1Tree()
{
	m_pLevel1Tree = new TTree("T1", "Level1 Quantities");
	m_pLevel1Tree->SetMaxTreeSize(m_iMaxTreeSize);

	vector<Branch>::iterator pBranch;
	for(pBranch = m_pLevel1Branches->begin(); pBranch != m_pLevel1Branches->end(); pBranch++)
	{
		if(pBranch->getDescription() == "")
			m_pLevel1Tree->Branch(pBranch->getName().c_str(), pBranch->getType().c_str(), pBranch->getAddress());
		else
			m_pLevel1Tree->Branch(pBranch->getName().c_str(), pBranch->getAddress(), pBranch->getDescription().c_str());
	}
}
	
void
AnalysisManager::createLevel2Tree()
{
	m_pLevel2Tree = new TTree("T2", "Level2 Quantities");
	m_pLevel2Tree->SetMaxTreeSize(m_iMaxTreeSize);

	vector<Branch>::iterator pBranch;
	for(pBranch = m_pLevel2Branches->begin(); pBranch != m_pLevel2Branches->end(); pBranch++)
	{
		if(pBranch->getDescription() == "")
			m_pLevel2Tree->Branch(pBranch->getName().c_str(), pBranch->getType().c_str(), pBranch->getAddress());
		else
			m_pLevel2Tree->Branch(pBranch->getName().c_str(), pBranch->getAddress(), pBranch->getDescription().c_str());
	}
}

void
AnalysisManager::createLevel3Tree()
{
	m_pLevel3Tree = new TTree("T3", "Level3 Quantities");
	m_pLevel3Tree->SetMaxTreeSize(m_iMaxTreeSize);

	vector<Branch>::iterator pBranch;
	for(pBranch = m_pLevel3Branches->begin(); pBranch != m_pLevel3Branches->end(); pBranch++)
	{
		if(pBranch->getDescription() == "")
			m_pLevel3Tree->Branch(pBranch->getName().c_str(), pBranch->getType().c_str(), pBranch->getAddress());
		else
			m_pLevel3Tree->Branch(pBranch->getName().c_str(), pBranch->getAddress(), pBranch->getDescription().c_str());
	}
}

bool
AnalysisManager::verifyLevel1Tree()
{
	int iNbBranches = m_pLevel1Tree->GetListOfBranches()->GetEntries();

	if(m_pLevel1Branches->size() == iNbBranches)
	{
		// build a list of branch names from the tree
		list<string> hBranchNames;
		for(int iBranch = 0; iBranch < iNbBranches; iBranch++)
			hBranchNames.push_back(m_pLevel1Tree->GetListOfBranches()->At(iBranch)->GetName());

		// verify that each branch of the current version is present is the tree
		bool bMissingBranch = false;
		list<string>::iterator pResult;
		vector<Branch>::iterator pBranch;
		for(pBranch = m_pLevel1Branches->begin(); pBranch != m_pLevel1Branches->end(); pBranch++)
		{
			pResult = find(hBranchNames.begin(), hBranchNames.end(), pBranch->getName());

			if(pResult == hBranchNames.end())
				bMissingBranch = true;
			else
				hBranchNames.erase(pResult);
		}

		// if there is no missing branch, set all branch addresses
		if(!bMissingBranch)
		{
			for(pBranch = m_pLevel1Branches->begin(); pBranch != m_pLevel1Branches->end(); pBranch++)
				m_pLevel1Tree->SetBranchAddress(pBranch->getName().c_str(), pBranch->getAddress());

			return true;
		}
	}

	return false;
}

bool
AnalysisManager::verifyLevel2Tree()
{
	return false;
}

bool
AnalysisManager::verifyLevel3Tree()
{
	return false;
}

void
AnalysisManager::getSingles()
{
	// get the map of waveforms
	map<int, Waveform *> *pWaveforms = m_pEventData->m_pWaveforms;

	if(pWaveforms->size())
	{
		const float fSamplingFrequency = pWaveforms->begin()->second->getSamplingFrequency();

		const int iWindowStart = m_iSingleWindowStart;
		const int iWindowWidth = m_iSingleWindowWidth;

		const int iFixedStart = 112; // m_iSingleFixedStart;
		const int iFixedIterations = 10; // m_iSingleFixedIterations;
		const int iFixedPrePostSamples = 2; // m_iSingleFixedPrePostSamples;

		for(int iPmt = 1; iPmt < m_iNbPmts+1; iPmt++)
		{
			// if we had a waveform for that channel
			if(pWaveforms->count(iPmt-1))
			{
				const Waveform &hPmtWaveform = *(*pWaveforms)[iPmt-1];

				// waveform integral is in V*s as measured by the digitizer
				float fExternalGain = m_pExternalGainsTable->find(iPmt)->second;
				float fToElectronConversionFactor = 1./(m_fInputImpedance*fExternalGain*m_fElectricCharge);

				assert(iWindowStart+iWindowWidth < hPmtWaveform.getNbSamples());

				// find the maximum in the window
				float fPeak = -HUGE_VAL;
				int iPeakSample = -1;
				for(int iSample = iWindowStart; iSample < iWindowStart+iWindowWidth; iSample++)
				{
					if(hPmtWaveform[iSample] > fPeak)
					{
						fPeak = hPmtWaveform[iSample];
						iPeakSample = iSample;
					}
				}

				// save the peak
				m_pEventData->m_pSinglePeak->push_back(fPeak/fSamplingFrequency*fToElectronConversionFactor);
				m_pEventData->m_pSingleHeight->push_back(hPmtWaveform[iPeakSample]);
				m_pEventData->m_pSingleSample->push_back(iPeakSample);
				
				// save the samples before
				vector<float> hSamplesBefore;
				for(int iOffset = 1; iOffset <= m_iSinglePrePeakSamples; iOffset++)
					hSamplesBefore.push_back(hPmtWaveform[iPeakSample-iOffset]/fSamplingFrequency*fToElectronConversionFactor);
				m_pEventData->m_pSingleBefore->push_back(hSamplesBefore);

				// save the samples after
				vector<float> hSamplesAfter;
				for(int iOffset = 1; iOffset < m_iSinglePostPeakSamples; iOffset++)
					hSamplesAfter.push_back(hPmtWaveform[iPeakSample+iOffset]/fSamplingFrequency*fToElectronConversionFactor);
				m_pEventData->m_pSingleAfter->push_back(hSamplesAfter);

				// integrate the samples over the fixed window
				vector<float> hSamplesFixed;
				for(int iIteration = iFixedStart; iIteration < iFixedStart+iFixedIterations; iIteration++)
				{
					fPeak = 0;
					for(int iOffset = -iFixedPrePostSamples; iOffset < iFixedPrePostSamples+1; iOffset++)
						fPeak += hPmtWaveform[iIteration+iOffset]/fSamplingFrequency*fToElectronConversionFactor;
					hSamplesFixed.push_back(fPeak);
				}
				m_pEventData->m_pSingleFixed->push_back(hSamplesFixed);
			}
			else
			{
				m_pEventData->m_pSinglePeak->push_back(0);
				m_pEventData->m_pSingleHeight->push_back(0);
				m_pEventData->m_pSingleSample->push_back(0);
				m_pEventData->m_pSingleBefore->push_back(vector<float>(m_iSinglePrePeakSamples, 0));
				m_pEventData->m_pSingleAfter->push_back(vector<float>(m_iSinglePostPeakSamples, 0));
				m_pEventData->m_pSingleFixed->push_back(vector<float>(iFixedIterations, 0));
			}
		}
	}
}

void
AnalysisManager::sumWaveforms()
{
	map<int, Waveform *> *pWaveforms = m_pEventData->m_pWaveforms;

	float fSamplingFrequency = pWaveforms->begin()->second->getSamplingFrequency();
	int iNbSamples = pWaveforms->begin()->second->getNbSamples();

	m_pEventData->m_pTotalWaveform = new Waveform(fSamplingFrequency, iNbSamples, new list<Pulse *>());
	m_pEventData->m_pTotalWaveformVeto = new Waveform(fSamplingFrequency, iNbSamples, new list<Pulse *>());

	m_pEventData->m_pS1PeakFindingTotalWaveform = new Waveform(fSamplingFrequency, iNbSamples, new list<Pulse *>());
	m_pEventData->m_pS2PeakFindingTotalWaveform = new Waveform(fSamplingFrequency, iNbSamples, new list<Pulse *>());
	m_pEventData->m_pS1PeakFindingTotalWaveformVeto =  new Waveform(fSamplingFrequency, iNbSamples, new list<Pulse *>());	

	map<int, Waveform *>::iterator pIter;
	for(pIter = pWaveforms->begin(); pIter != pWaveforms->end(); pIter++)
	{
		int iPmt = pIter->first+1;
		
		if(isTopPmt(iPmt) || isBottomPmt(iPmt))
		{
			*m_pEventData->m_pTotalWaveform += *(pIter->second);
			
			if(!m_pS1PeakFindingExcludedPmtSet->count(iPmt))
				*m_pEventData->m_pS1PeakFindingTotalWaveform += *(pIter->second);

			if(!m_pS2PeakFindingExcludedPmtSet->count(iPmt))
				*m_pEventData->m_pS2PeakFindingTotalWaveform += *(pIter->second);
		}
		else if(isTopVetoPmt(iPmt) || isBottomVetoPmt(iPmt))
		{
			*m_pEventData->m_pTotalWaveformVeto += *(pIter->second);

			if(!m_pS1PeakFindingExcludedPmtSet->count(iPmt))
				*m_pEventData->m_pS1PeakFindingTotalWaveformVeto += *(pIter->second);
		}
	}
}

void
AnalysisManager::findPeaks()
{
	// find S2 peaks
	m_pPeakFinder->findS2Peaks(m_pEventData->m_pS2PeakFindingTotalWaveform);
	const vector<Peak> hS2Peaks = m_pPeakFinder->getS2Peaks();

	// compute S2 peak related level1 quantities
	m_pEventData->m_pS2Order->assign(hS2Peaks.size(), -1);
	vector<Peak>::const_iterator pS2Peak;
	for(pS2Peak = hS2Peaks.begin(); pS2Peak != hS2Peaks.end(); pS2Peak++)
	{
		m_pEventData->m_pS2sPeak->push_back(pS2Peak->getPosition());
		m_pEventData->m_pS2sLeftEdge->push_back(pS2Peak->getLeftEdge());
		m_pEventData->m_pS2sRightEdge->push_back(pS2Peak->getRightEdge());
		m_pEventData->m_pS2sWidth->push_back(pS2Peak->getWidth());
		m_pEventData->m_pS2sLowWidth->push_back(pS2Peak->getLowWidth());
		m_pEventData->m_pS2sHeight->push_back((*m_pEventData->m_pS2PeakFindingTotalWaveform)[pS2Peak->getPosition()]);

		(*(m_pEventData->m_pS2Order))[pS2Peak->getOrder()] = pS2Peak - hS2Peaks.begin();
	}
	m_pEventData->m_iNbS2Peaks = hS2Peaks.size();

	// find S1 peaks
	m_pPeakFinder->findS1Peaks(m_pEventData->m_pS1PeakFindingTotalWaveform, m_pEventData->m_pWaveforms, true, true);
	const vector<Peak> hS1Peaks = m_pPeakFinder->getS1Peaks();

	// compute S1 peak related level1 quantities
	m_pEventData->m_pS1Order->assign(hS1Peaks.size(), -1);
	m_pEventData->m_pS1SizeOrder->assign(hS1Peaks.size(), -1);
	vector<Peak>::const_iterator pS1Peak;
	for(pS1Peak = hS1Peaks.begin(); pS1Peak != hS1Peaks.end(); pS1Peak++)
	{
		m_pEventData->m_pS1sPeak->push_back(pS1Peak->getPosition());
		m_pEventData->m_pS1sLeftEdge->push_back(pS1Peak->getLeftEdge());
		m_pEventData->m_pS1sRightEdge->push_back(pS1Peak->getRightEdge());

		m_pEventData->m_pS1sWidth->push_back(pS1Peak->getWidth());
		m_pEventData->m_pS1sLowWidth->push_back(pS1Peak->getLowWidth());
		m_pEventData->m_pS1sFilteredLowWidth->push_back(pS1Peak->getFilteredLowWidth());
		m_pEventData->m_pS1sHeight->push_back((*m_pEventData->m_pS1PeakFindingTotalWaveform)[pS1Peak->getPosition()]);
		m_pEventData->m_pS1sMinimum->push_back(m_pEventData->m_pS1PeakFindingTotalWaveform->minimum(pS1Peak->getLeftEdge(), pS1Peak->getRightEdge()));
		m_pEventData->m_pS1sMeanTime->push_back(m_pEventData->m_pS1PeakFindingTotalWaveform->meanTime(pS1Peak->getLeftEdge(), pS1Peak->getRightEdge()));
		m_pEventData->m_pS1sTimeSpread->push_back(m_pEventData->m_pS1PeakFindingTotalWaveform->timeSpread(pS1Peak->getLeftEdge(), pS1Peak->getRightEdge()));

		(*(m_pEventData->m_pS1Order))[pS1Peak->getOrder()] = pS1Peak - hS1Peaks.begin();
		(*(m_pEventData->m_pS1SizeOrder))[pS1Peak->getSizeOrder()] = pS1Peak - hS1Peaks.begin();
	}
	m_pEventData->m_iNbS1Peaks = hS1Peaks.size();

	// look for S1 peaks in the veto
	m_pPeakFinder->findS1Peaks(m_pEventData->m_pS1PeakFindingTotalWaveformVeto, 0, false, false);
	const vector<Peak> hS1PeaksVeto = m_pPeakFinder->getS1Peaks();

	// compute veto S1 peak related level1 quantities
	m_pEventData->m_pS1OrderVeto->assign(hS1PeaksVeto.size(), -1);
	vector<Peak>::const_iterator pS1PeakVeto;
	for(pS1PeakVeto = hS1PeaksVeto.begin(); pS1PeakVeto != hS1PeaksVeto.end(); pS1PeakVeto++)
	{
		m_pEventData->m_pS1sPeakVeto->push_back(pS1PeakVeto->getPosition());
		m_pEventData->m_pS1sLeftEdgeVeto->push_back(pS1PeakVeto->getLeftEdge());
		m_pEventData->m_pS1sRightEdgeVeto->push_back(pS1PeakVeto->getRightEdge());
		m_pEventData->m_pS1sWidthVeto->push_back(pS1PeakVeto->getWidth());
		m_pEventData->m_pS1sLowWidthVeto->push_back(pS1PeakVeto->getLowWidth());
		m_pEventData->m_pS1sHeightVeto->push_back((*m_pEventData->m_pS1PeakFindingTotalWaveformVeto)[pS1PeakVeto->getPosition()]);

		(*(m_pEventData->m_pS1OrderVeto))[pS1PeakVeto->getOrder()] = pS1PeakVeto - hS1PeaksVeto.begin();
	}
	m_pEventData->m_iNbS1PeaksVeto = hS1PeaksVeto.size();

#ifdef ENABLE_LIQ_SCI
	// look for peaks in the liquid scintillators
	map<int, Waveform *> *pWaveforms = m_pEventData->m_pWaveforms;

	// if the liquid scintillators are not multiplexed in a single channel
	if(!m_bLiqSciMultiplexed)
	{
		for(int iLiqSci = 0; iLiqSci < m_iNbLiqScis; iLiqSci++)
		{
			// if we had a waveform for that channel
			if(pWaveforms->count(m_iFirstLiqSciChannel+iLiqSci))
			{
				const Waveform *pLiqSciWaveform = (*pWaveforms)[m_iFirstLiqSciChannel+iLiqSci];
				m_pPeakFinder->findLiqSciPeak(pLiqSciWaveform, pLiqSciWaveform->getNbSamples());
				const Peak *pLiqSciPeak = &(m_pPeakFinder->getLiqSciPeak());

				// compute liquid scintillator peak related level1 quantities
				m_pEventData->m_pLiqSciPeak->push_back(pLiqSciPeak->getPosition());
				m_pEventData->m_pLiqSciLeftEdge->push_back(pLiqSciPeak->getLeftEdge());
				m_pEventData->m_pLiqSciRightEdge->push_back(pLiqSciPeak->getRightEdge());
				m_pEventData->m_pLiqSciWidth->push_back(pLiqSciPeak->getWidth());
				m_pEventData->m_pLiqSciLowWidth->push_back(pLiqSciPeak->getLowWidth());
				m_pEventData->m_pLiqSciHeight->push_back((*pLiqSciWaveform)[pLiqSciPeak->getPosition()]);
			}
			else
			{
				m_pEventData->m_pLiqSciPeak->push_back(-1);
				m_pEventData->m_pLiqSciLeftEdge->push_back(-1);
				m_pEventData->m_pLiqSciRightEdge->push_back(-1);
				m_pEventData->m_pLiqSciWidth->push_back(-1.);
				m_pEventData->m_pLiqSciLowWidth->push_back(-1.);
				m_pEventData->m_pLiqSciHeight->push_back(-1.);
			}
		}
	}
	else
	{
		// the liquid scintillators are multiplexed, find out which one had a signal
		int iLiqSciWithSignal = -1;

		// if we had a waveform for that channel
		if(pWaveforms->count(m_iFirstLiqSciChannel))
		{
			const Waveform *pLiqSciWaveform = (*pWaveforms)[m_iFirstLiqSciChannel];

			for(int iLiqSci = 0; iLiqSci < m_iNbLiqScis; iLiqSci++)
			{
				int iWindowStart = (*m_pLiqSciMultiplexingTable)[iLiqSci][0];
				int iWindowEnd = iWindowStart+(*m_pLiqSciMultiplexingTable)[iLiqSci][1];

				if(pLiqSciWaveform->maximum(iWindowStart, iWindowEnd) > m_fLiqSciMultiplexingSignalHeightThreshold)
				{
					int iMaximumPosition = pLiqSciWaveform->maximumPosition(iWindowStart, iWindowEnd);
					
					if(pLiqSciWaveform->width(iMaximumPosition, iWindowStart, iWindowEnd, 0.5) > m_iLiqSciMultiplexingSignalWidthThreshold)
					{
						if(iLiqSciWithSignal == -1)
							iLiqSciWithSignal = iLiqSci;
						else
						{
							// we already had a liquid scintillator with a signal, ignore both
							iLiqSciWithSignal = -1;
							break;
						}
					}
				}
			}
		}

		// now we know which liquid scintillator had a signal, find the signal itself
		for(int iLiqSci = 0; iLiqSci < m_iNbLiqScis; iLiqSci++)
		{
			if(iLiqSci == iLiqSciWithSignal)
			{
				const Waveform *pLiqSciWaveform = (*pWaveforms)[m_iFirstLiqSciChannel];

				m_pPeakFinder->findLiqSciPeak(pLiqSciWaveform, m_iLiqSciMultiplexingStart);
				const Peak *pLiqSciPeak = &(m_pPeakFinder->getLiqSciPeak());

				// compute liquid scintillator peak related level1 quantities
				m_pEventData->m_pLiqSciPeak->push_back(pLiqSciPeak->getPosition());
				m_pEventData->m_pLiqSciLeftEdge->push_back(pLiqSciPeak->getLeftEdge());
				m_pEventData->m_pLiqSciRightEdge->push_back(pLiqSciPeak->getRightEdge());
				m_pEventData->m_pLiqSciWidth->push_back(pLiqSciPeak->getWidth());
				m_pEventData->m_pLiqSciLowWidth->push_back(pLiqSciPeak->getLowWidth());
				m_pEventData->m_pLiqSciHeight->push_back((*pLiqSciWaveform)[pLiqSciPeak->getPosition()]);
			}
			else
			{
				m_pEventData->m_pLiqSciPeak->push_back(-1);
				m_pEventData->m_pLiqSciLeftEdge->push_back(-1);
				m_pEventData->m_pLiqSciRightEdge->push_back(-1);
				m_pEventData->m_pLiqSciWidth->push_back(-1.);
				m_pEventData->m_pLiqSciLowWidth->push_back(-1.);
				m_pEventData->m_pLiqSciHeight->push_back(-1.);
			}
		}
	}
#endif

#ifdef ENABLE_NAI
	// look for peaks in the sodium iodide detectors
	map<int, Waveform *> *pWaveforms = m_pEventData->m_pWaveforms;

	for(int iNai = 0; iNai < m_iNbNaiDetectors; iNai++)
	{
		// if we had a waveform for that channel
		if(pWaveforms->count(m_iFirstNaiChannel+iNai))
		{
			const Waveform *pNaiWaveform = (*pWaveforms)[m_iFirstNaiChannel+iNai];
			m_pPeakFinder->findNaiPeak(pNaiWaveform, pNaiWaveform->getNbSamples()-400);
			const Peak *pNaiPeak = &(m_pPeakFinder->getNaiPeak());

			// compute liquid scintillator peak related level1 quantities
			m_pEventData->m_pNaiPeak->push_back(pNaiPeak->getPosition());
			m_pEventData->m_pNaiLeftEdge->push_back(pNaiPeak->getLeftEdge());
			m_pEventData->m_pNaiRightEdge->push_back(pNaiPeak->getRightEdge());
			m_pEventData->m_pNaiWidth->push_back(pNaiPeak->getWidth());
			m_pEventData->m_pNaiLowWidth->push_back(pNaiPeak->getLowWidth());
			m_pEventData->m_pNaiHeight->push_back((*pNaiWaveform)[pNaiPeak->getPosition()]);
		}
		else
		{
			m_pEventData->m_pNaiPeak->push_back(-1);
			m_pEventData->m_pNaiLeftEdge->push_back(-1);
			m_pEventData->m_pNaiRightEdge->push_back(-1);
			m_pEventData->m_pNaiWidth->push_back(-1.);
			m_pEventData->m_pNaiLowWidth->push_back(-1.);
			m_pEventData->m_pNaiHeight->push_back(-1.);
		}
	}
#endif
}

void
AnalysisManager::getSignalsRaw()
{
	// get the map of waveforms
	
	map<int, Waveform *> *pWaveforms = m_pEventData->m_pWaveforms;
	
	if(m_iUseTDC)
	{
			// ------------- Add TDC events
			map<int, RawDataChannel *>::const_iterator pIter;
			map<int, RawDataChannel *> const *pChannels_test = m_pEventData->getRawData_tdc()->getChannels();
			unsigned short rawValue;
			for(pIter = pChannels_test->begin(); pIter != pChannels_test->end(); pIter++)
			{
				uint8_t msb = pIter->second->getBytes()[1];
				uint8_t lsb = pIter->second->getBytes()[0];
				rawValue = (msb << 8) | (0xFF & lsb);
				float rawTimeValue = rawValue * m_pEventData->getRawData_tdc()->getVoltageRange()/ (pow(2, 12));
				//printf("\nChannel %d with value: %f", pIter->first, rawTimeValue);
				m_pEventData->m_pTimeTDC->push_back(rawTimeValue);
			}
			// ------------- End addition of TDC events
	}
	// get the total area of waveforms
	for(int iPmt = 1; iPmt < m_iNbPmts+1; iPmt++)
	{
		// if we had a waveform for that channel
		if(pWaveforms->count(iPmt-1))
		{
			int iNbSamples = (*pWaveforms)[iPmt-1]->getNbSamples();

			// waveform integral is in V*s as measured by the digitizer
			float fExternalGain = m_pExternalGainsTable->find(iPmt)->second;
			float fToElectronConversionFactor = 1./(m_fInputImpedance*fExternalGain*m_fElectricCharge);
			float fAreaRaw = (*pWaveforms)[iPmt-1]->integrate(0, iNbSamples)*fToElectronConversionFactor;
			float fAbsAreaRaw = (*pWaveforms)[iPmt-1]->integrate(0, iNbSamples, fabsf)*fToElectronConversionFactor;

			m_pEventData->m_pAreaRaw->push_back(fAreaRaw);
			m_pEventData->m_pAbsAreaRaw->push_back(fAbsAreaRaw);

			if(isTopPmt(iPmt) || isBottomPmt(iPmt))
			{
				m_pEventData->m_fAreaTotRaw += fAreaRaw;
				m_pEventData->m_fAbsAreaTotRaw += fAbsAreaRaw;
			}
			else
				m_pEventData->m_fAreaTotRawVeto += fAreaRaw;

			if((*((*pWaveforms)[iPmt-1]))[iNbSamples-1] != 0.)
				m_pEventData->m_iNbNonZeroLast++;
		}
		else
		{
			m_pEventData->m_pAreaRaw->push_back(0.);
			m_pEventData->m_pAbsAreaRaw->push_back(0.);
		}
	}

	// for all S1 peaks found in the sensitive volume
	for(int iPeak = 0; iPeak < m_pEventData->m_iNbS1Peaks; iPeak++)
	{
		int iLeftEdge = m_pEventData->getS1sLeftEdge(iPeak);
		int iRightEdge = m_pEventData->getS1sRightEdge(iPeak);

		vector<float> hS1Raw;
		vector<int> hS1MaxPosition;
		int iS1Saturation = 0;
		for(int iPmt = 1; iPmt < m_iNbPmts+1; iPmt++)
		{
			// if we had a waveform for that channel
			if(pWaveforms->count(iPmt-1))
			{
				// waveform integral is in V*s as measured by the digitizer
				float fExternalGain = m_pExternalGainsTable->find(iPmt)->second;
				float fToElectronConversionFactor = 1./(m_fInputImpedance*fExternalGain*m_fElectricCharge);
				float fS1Raw = (*pWaveforms)[iPmt-1]->integrate(iLeftEdge, iRightEdge)*fToElectronConversionFactor;

				hS1Raw.push_back(fS1Raw);

				// if it's a sensitive volume PMT
				if(isTopPmt(iPmt) || isBottomPmt(iPmt))
				{
					int iS1MaxPos = (*pWaveforms)[iPmt-1]->maximumPosition(iLeftEdge, iRightEdge);

					hS1MaxPosition.push_back(iS1MaxPos);

					if((*((*pWaveforms)[iPmt-1]))[iS1MaxPos] > m_fSaturationThreshold)
						iS1Saturation++;
				}
				else
					hS1MaxPosition.push_back(INT_MAX);

			}
			else
			{
				hS1Raw.push_back(0.);
				hS1MaxPosition.push_back(INT_MAX);
			}
		}
		
		float fTriggerSignalArea = 0;
		int iS2Trigger = 0;
		int triggerDelay = 0;//S1 trigger is 20 us behind S1 peak
		int triggerWindow = 100;
		// if we had a waveform for trigger efficiency analysis
		if(pWaveforms->count(m_iFirstTriggerEfficiencyChannel))
		{
			const Waveform *pTriggerWaveform = (*pWaveforms)[m_iFirstTriggerEfficiencyChannel];
			fTriggerSignalArea = pTriggerWaveform->integrate(iLeftEdge+m_iTriggerWindowStart+triggerDelay, iLeftEdge+m_iTriggerWindowStart+triggerWindow+triggerDelay);
			//cout << fTriggerSignalArea << endl;

			// waveform is in V as measured by the digitizer
			//float fTriggerSignalHeight = pTriggerWaveform->maximum(iLeftEdge+m_iTriggerWindowStart+triggerDelay, iLeftEdge+m_iTriggerWindowStart+m_iTriggerWindowWidth+triggerDelay);
			//taking out warning for nonstandard version
			//if(fTriggerSignalHeight > m_fTriggerSignalHeightThreshold)
			//{
			//	iS2Trigger = 1;
				//cout << "S1 Trigger Added" << endl;
			//}
			
			//if(fTriggerSignalHeight > .4 && fTriggerSignalHeight < 1.)
			//{
			//	iS2Trigger = 1; //Ge Signal
			//}
			//else if(fTriggerSignalHeight > 1. && fTriggerSignalHeight < 1.85)
			//{
			//	iS2Trigger = 2; //S1 Signal
			//}
			//else if(fTriggerSignalHeight > 1.85)
			//{
			//	iS2Trigger = 3; //S1*Ge Coincidence
			//}
		}
		//m_pEventData->m_pS2sTrigger->push_back(iS2Trigger);
		m_pEventData->m_pCoinRaw->push_back(fTriggerSignalArea);
		
		m_pEventData->m_pS1sRaw->push_back(hS1Raw);
		m_pEventData->m_pS1sMaxPosition->push_back(hS1MaxPosition);
		m_pEventData->m_pS1sSaturation->push_back(iS1Saturation);
	}

	// fit the S1s[0] signal if it is large enough
	m_fFitThreshold = 1.0;
	if((m_pEventData->getNbS1Peaks() > 0) && (m_pEventData->getS1sHeight(0) > m_fFitThreshold))
	{
		m_pSignalFitter->fitS1Signal(pWaveforms, m_pEventData->getS1sLeftEdge(0), m_pEventData->getS1sRightEdge(0));
		map<int, vector<float> > const *pS1FitParameters = m_pSignalFitter->getS1FitParameters();
		for(int iPmt = 1; iPmt < m_iNbPmts+1; iPmt++)
		{
			m_pSignalFitter->getS1FitParameters();
			if(pWaveforms->count(iPmt-1))
				m_pEventData->m_pS1FitParameters->push_back(pS1FitParameters->find(iPmt)->second);
			else
				m_pEventData->m_pS1FitParameters->push_back(vector<float>());
		}
	}

	// for all S2 peaks found in the sensitive volume
	for(int iPeak = 0; iPeak < m_pEventData->m_iNbS2Peaks; iPeak++)
	{
		int iLeftEdge = m_pEventData->getS2sLeftEdge(iPeak);
		int iRightEdge = m_pEventData->getS2sRightEdge(iPeak);

		vector<float> hS2Raw;
		int iS2Saturation = 0;
		for(int iPmt = 1; iPmt < m_iNbPmts+1; iPmt++)
		{
			// if it's a sensitive volume PMT and if we had a waveform for that channel
			if((isTopPmt(iPmt) || isBottomPmt(iPmt)) && pWaveforms->count(iPmt-1))
			{
				// waveform integral is in V*s as measured by the digitizer
				float fExternalGain = m_pExternalGainsTable->find(iPmt)->second;
				float fToElectronConversionFactor = 1./(m_fInputImpedance*fExternalGain*m_fElectricCharge);
				float fS2Raw = (*pWaveforms)[iPmt-1]->integrate(iLeftEdge, iRightEdge)*fToElectronConversionFactor;

				hS2Raw.push_back(fS2Raw);

				int iS2MaxPos = (*pWaveforms)[iPmt-1]->maximumPosition(iLeftEdge, iRightEdge);

				if((*((*pWaveforms)[iPmt-1]))[iS2MaxPos] > m_fSaturationThreshold)
					iS2Saturation++;
			}
			else
				hS2Raw.push_back(0.);
		}
		/*
		int iS2Trigger = 0;
		// if we had a waveform for trigger efficiency analysis
		//int triggerDelay = 2000;
		int triggerDelay = 0;
		//first efficiency channel was 22 so trying to hard code
		if(pWaveforms->count(m_iFirstTriggerEfficiencyChannel))
		{
			const Waveform *pTriggerWaveform = (*pWaveforms)[m_iFirstTriggerEfficiencyChannel];

			// waveform is in V as measured by the digitizer
			float fTriggerSignalHeight = pTriggerWaveform->maximum(iLeftEdge+m_iTriggerWindowStart+triggerDelay, iLeftEdge+m_iTriggerWindowStart+m_iTriggerWindowWidth+triggerDelay);

			//if(fTriggerSignalHeight > m_fTriggerSignalHeightThreshold)
				//iS2Trigger = 1;

			if(fTriggerSignalHeight > .4 && fTriggerSignalHeight < 1.)
			{
				iS2Trigger = 1; //Ge Signal
			}
			else if(fTriggerSignalHeight > 1. && fTriggerSignalHeight < 1.85)
			{
				iS2Trigger = 2; //S1 Signal
			}
			else if(fTriggerSignalHeight > 1.85)
			{
				iS2Trigger = 3; //S1*Ge Coincidence
			}

		}
		*/
		m_pEventData->m_pS2sRaw->push_back(hS2Raw);
		m_pEventData->m_pS2sSaturation->push_back(iS2Saturation);
		//m_pEventData->m_pS2sTrigger->push_back(iS2Trigger);
	}

	// for all S1 peaks found in the veto
	for(int iPeak = 0; iPeak < m_pEventData->m_iNbS1PeaksVeto; iPeak++)
	{
		int iLeftEdge = m_pEventData->getS1sLeftEdgeVeto(iPeak);
		int iRightEdge = m_pEventData->getS1sRightEdgeVeto(iPeak);

		vector<float> hS1RawVeto;
		for(int iPmt = 1; iPmt < m_iNbPmts+1; iPmt++)
		{
			// if it's a sensitive volume PMT and if we had a waveform for that channel
			if((isTopVetoPmt(iPmt) || isBottomVetoPmt(iPmt)) && pWaveforms->count(iPmt-1))
			{
				// waveform integral is in V*s as measured by the digitizer
				float fExternalGain = m_pExternalGainsTable->find(iPmt)->second;
				float fToElectronConversionFactor = 1./(m_fInputImpedance*fExternalGain*m_fElectricCharge);
				float fS1RawVeto = (*pWaveforms)[iPmt-1]->integrate(iLeftEdge, iRightEdge)*fToElectronConversionFactor;

				hS1RawVeto.push_back(fS1RawVeto);
			}
			else
				hS1RawVeto.push_back(0.);
		}

		m_pEventData->m_pS1sRawVeto->push_back(hS1RawVeto);
	}
	
	// add in processing for tdc
	/*
	RawData::RawData *rawData_tdc = getEventRawData_tdc();
	RawData::map<int, RawDataChannel *> const *pChannels_tdc = rawData_tdc->getChannels();
	RawData::map<int, RawDataChannel *>::const_iterator pIter;
	
	float timeFromTDC = 0;
	
	for(pIter = pChannels->begin(); pIter != pChannels->end(); pIter++)
	{
		int channel = pIter->first; // get channel number
		int size = pIter->second->getSize(); // give size of bytes array
		int32_t *data = (int32_t *)pIter->second->getBytes();
		printf("\ndata: %d", *data);
	}
	*/

#ifdef ENABLE_LIQ_SCI
	for(int iLiqSci = 0; iLiqSci < m_iNbLiqScis; iLiqSci++)
	{
		// if liquid scintillators are not multiplexed and we had a waveform for that channel or we had a waveform for the multiplexed channel
		int iChannel = (m_bLiqSciMultiplexed)?(m_iFirstLiqSciChannel):(m_iFirstLiqSciChannel+iLiqSci);

		// if we had a peak for that liquid scintillator
		if(m_pEventData->getLiqSciPeak(iLiqSci) != -1)
		{
			const Waveform *pLiqSciWaveform = (*pWaveforms)[iChannel];

			int iPeak = m_pEventData->getLiqSciPeak(iLiqSci);
			int iLeftEdge = m_pEventData->getLiqSciLeftEdge(iLiqSci);
			int iRightEdge = m_pEventData->getLiqSciRightEdge(iLiqSci);

			// waveform integral is in V*s as measured by the digitizer
			float fExternalGain = 10.;
			float fToElectronConversionFactor = 1./(m_fInputImpedance*fExternalGain*m_fElectricCharge);
			float fLiqSciRaw = pLiqSciWaveform->integrate(iLeftEdge, iRightEdge)*fToElectronConversionFactor;
			float fLiqSciTailRaw = pLiqSciWaveform->integrate(iPeak+m_iLiqSciTailStart, iRightEdge)*fToElectronConversionFactor;

			m_pEventData->m_pLiqSciRaw->push_back(fLiqSciRaw);
			m_pEventData->m_pLiqSciTailRaw->push_back(fLiqSciTailRaw);
		}
		else
		{
			m_pEventData->m_pLiqSciRaw->push_back(0.);
			m_pEventData->m_pLiqSciTailRaw->push_back(0.);
		}
	}
#endif

#ifdef ENABLE_NAI
	for(int iNai = 0; iNai < m_iNbNaiDetectors; iNai++)
	{
		// if liquid scintillators are not multiplexed and we had a waveform for that channel or we had a waveform for the multiplexed channel
		int iChannel = m_iFirstNaiChannel+iNai;

		// if we had a peak for that liquid scintillator
		if(m_pEventData->getNaiPeak(iNai) != -1)
		{
			const Waveform *pNaiWaveform = (*pWaveforms)[iChannel];

			int iPeak = m_pEventData->getNaiPeak(iNai);
			int iLeftEdge = m_pEventData->getNaiLeftEdge(iNai);
			int iRightEdge = m_pEventData->getNaiRightEdge(iNai);

			// waveform integral is in V*s as measured by the digitizer
			float fExternalGain = 10.;
			float fToElectronConversionFactor = 1./(m_fInputImpedance*fExternalGain*m_fElectricCharge);
			float fNaiRaw = pNaiWaveform->integrate(iLeftEdge, iRightEdge)*fToElectronConversionFactor;

			m_pEventData->m_pNaiRaw->push_back(fNaiRaw);
		}
		else
		{
			m_pEventData->m_pNaiRaw->push_back(0.);
		}
	}
#endif

#ifdef ENABLE_HPGE
	for(int iGe = 0; iGe < m_iNbGeDetectors; iGe++)
	{
		// if we had a waveform for that channel
		if(pWaveforms->count(m_iFirstGeChannel+iGe))
		{
			const Waveform *pGeWaveform = (*pWaveforms)[m_iFirstGeChannel+iGe];

			// waveform is in V as measured by the digitizer
			float fGeHeight = pGeWaveform->maximum(0, pGeWaveform->getNbSamples());

			m_pEventData->m_pGeHeight->push_back(fGeHeight);
		}
		else
		{
			m_pEventData->m_pGeHeight->push_back(0.);
		}
	}
	// check for trigger peaks
	// trigger is in channel 19 so use index 18
	if(pWaveforms->count(18))
	{
		const Waveform *pTriggerWaveform = (*pWaveforms)[m_iFirstTriggerEfficiencyChannel];
		
		float triggerThreshold = 0.05; // fill in threshold using cout below
		int iNbSamples = (*pWaveforms)[18]->getNbSamples();
		int startSample = -1;
		int endSample = -1;
		float fTriggerSignalArea;
		for (int iSample = 0; iSample < iNbSamples; iSample++)
		{
			float fTriggerSignalHeight = pTriggerWaveform->maximum(iSample, iSample+1);
			//cout << "\nfTriggerSignalHeight: " << fTriggerSignalHeight << endl;
			//mark a start and stop bin to assure you get full peak
			
			if(fTriggerSignalHeight > triggerThreshold)
			{
				if(startSample < 0)
				{
					startSample = iSample;
				}
				else
				{
					endSample = iSample;
				}
			}
			else if(fTriggerSignalHeight < triggerThreshold && endSample > 0)
			{
				fTriggerSignalArea = pTriggerWaveform->integrate(startSample - 1, endSample + 1);
				
				// check outputs
				//cout << "/nArea: " << fTriggerSignalArea << endl;
				//cout << "Left Edge: " << startSample << endl;
				
				
				// fill in vectors with area and leftedge
				if(fTriggerSignalArea > 1e-8)
				{
					m_pEventData->m_pTrigArea->push_back(fTriggerSignalArea);
					//cout << "Left Edge: " << startSample << endl;
					m_pEventData->m_pTrigLeftEdge->push_back(startSample);
				}
				// reset startSample and endSample
				startSample = -1;
				endSample = -1;
			}
		}
	}
#endif

#ifdef ENABLE_TAC
	for(int iTac = 0; iTac < m_iNbTacs; iTac++)
	{
		// if we had a waveform for that channel
		if(pWaveforms->count(m_iFirstTacChannel+iTac))
		{
			const Waveform *pTacWaveform = (*pWaveforms)[m_iFirstTacChannel+iTac];

			// waveform is in V as measured by the digitizer
			int iTacWindowPosition = m_iTacWindowStart+m_iTacWindowWidth/2;
			float fTacHeight = pTacWaveform->average(iTacWindowPosition, m_iTacWindowWidth);
			float fTacMaximum = pTacWaveform->maximum(0, pTacWaveform->getNbSamples());

			m_pEventData->m_pTacHeight->push_back(fTacHeight);
			m_pEventData->m_pTacMaximum->push_back(fTacMaximum);
		}
		else
		{
			m_pEventData->m_pTacHeight->push_back(0.);
			m_pEventData->m_pTacMaximum->push_back(0.);
		}
	}
#endif
}

void
AnalysisManager::getSignals()
{
	m_pEventData->setPmtCalibrationFiles(m_hPmtCalibrationFiles);

	// if the event has waveforms
	if(m_pEventData->getAreaRaw()->size())
	{
		// for all PMTs
		for(int iPmt = 1; iPmt < m_iNbPmts+1; iPmt++)
		{
			float fGain = (m_pPmtCalibrationTable->find(iPmt)->second).find("gain")->second;
			float fArea = (fGain != 0.)?((*m_pEventData->getAreaRaw())[iPmt-1]/fGain):(0.);
			float fAbsArea = (fGain != 0.)?((*m_pEventData->getAbsAreaRaw())[iPmt-1]/fGain):(0.);

			m_pEventData->m_pArea->push_back(fArea);
			m_pEventData->m_pAbsArea->push_back(fAbsArea);

			if(isTopPmt(iPmt) || isBottomPmt(iPmt))
			{
				m_pEventData->m_fAreaTot += fArea;
				m_pEventData->m_fAbsAreaTot += fAbsArea;
			}
			else
				m_pEventData->m_fAreaTotVeto += fArea;
		}
	}

	// for all S1 peaks found in the sensitive volume
	for(int iPeak = 0; iPeak < m_pEventData->m_iNbS1Peaks; iPeak++)
	{
		// the vector we will fill for one peak
		vector<float> hPeakS1;
		vector<pair<float,int> > hPeakS1Pmts;
		float fPeakS1TotTop = 0., fPeakS1TotBottom = 0.;
		int iPeakS1CoinTop = 0, iPeakS1CoinBottom = 0;
		vector<int> hPeakS1PmtOrder;
		vector<int> hPeakS1PmtCoin;
		float fPeakS1TotOutside = 0.;

		for(int iPmt = 1; iPmt < m_iNbPmts+1; iPmt++)
		{
			float fS1Raw = (*m_pEventData->getS1sRaw())[iPeak][iPmt-1];
			float fGain = (m_pPmtCalibrationTable->find(iPmt)->second).find("gain")->second;
			float fS1 = (fGain != 0.)?(((fS1Raw > 0.)?(fS1Raw):(0.))/fGain):(0.);
			bool bExcludedFromCoincidence = m_pS1CoincidenceExcludedPmtSet->count(iPmt);
			bool bExcludedFromTotalSignal = m_pS1TotalSignalExcludedPmtSet->count(iPmt);
			int iDelay = 0.;
			
			if(m_pEventData->getS1sMaxPosition()->size())
				iDelay = abs((*m_pEventData->getS1sMaxPosition())[iPeak][iPmt-1]-m_pEventData->getS1sPeak(iPeak));

			if((isTopPmt(iPmt) || isBottomPmt(iPmt)) && !bExcludedFromTotalSignal)
			{
				hPeakS1.push_back(fS1);

				if(fS1 != 0.)
					hPeakS1Pmts.push_back(pair<float,int>(fS1,iPmt-1));

				if(isTopPmt(iPmt))
				{
					fPeakS1TotTop += fS1;
					if(!bExcludedFromCoincidence && (fS1 > m_fSignalThreshold) && (iDelay <= m_iS1CoincidenceWindow))			  {
							iPeakS1CoinTop++;
							hPeakS1PmtCoin.push_back(iPmt-1);
					}
				}
				else if(isBottomPmt(iPmt))
				{
					fPeakS1TotBottom += fS1;
					if(!bExcludedFromCoincidence && (fS1 > m_fSignalThreshold) && (iDelay <= m_iS1CoincidenceWindow))			  {	
						iPeakS1CoinBottom++;
						hPeakS1PmtCoin.push_back(iPmt-1);
					}
				}
			}
			else
				hPeakS1.push_back(0.);

			if(isTopVetoPmt(iPmt) || isBottomVetoPmt(iPmt))
				fPeakS1TotOutside += fS1;
		}
		

		// compute the pmt order and coin
		sort(hPeakS1Pmts.begin(), hPeakS1Pmts.end());
		vector<pair<float,int> >::reverse_iterator pPeakS1Pmt;
		for(pPeakS1Pmt = hPeakS1Pmts.rbegin(); pPeakS1Pmt < hPeakS1Pmts.rend(); pPeakS1Pmt++)
			hPeakS1PmtOrder.push_back(pPeakS1Pmt->second);

		m_pEventData->m_pS1s->push_back(hPeakS1);
		m_pEventData->m_pS1sTot->push_back(fPeakS1TotTop+fPeakS1TotBottom);
		m_pEventData->m_pS1sTotTop->push_back(fPeakS1TotTop);
		m_pEventData->m_pS1sTotBottom->push_back(fPeakS1TotBottom);

		m_pEventData->m_fS1Tot += fPeakS1TotTop+fPeakS1TotBottom;
		m_pEventData->m_fS1TotTop += fPeakS1TotTop;
		m_pEventData->m_fS1TotBottom += fPeakS1TotBottom;

		m_pEventData->m_pS1sCoin->push_back(iPeakS1CoinTop+iPeakS1CoinBottom);
		m_pEventData->m_pS1sCoinTop->push_back(iPeakS1CoinTop);
		m_pEventData->m_pS1sCoinBottom->push_back(iPeakS1CoinBottom);

		m_pEventData->m_pS1sPmtOrder->push_back(hPeakS1PmtOrder);
		m_pEventData->m_pS1sPmtCoin->push_back(hPeakS1PmtCoin);
	}

	// for all S2 peaks
	for(int iPeak = 0; iPeak < m_pEventData->m_iNbS2Peaks; iPeak++)
	{
		// the vector we will fill for one peak
		vector<float> hPeakS2;
		vector<pair<float,int> > hPeakS2Pmts;
		float fPeakS2TotTop = 0., fPeakS2TotBottom = 0.;
		int iPeakS2CoinTop = 0, iPeakS2CoinBottom = 0;
		vector<int> hPeakS2PmtOrder;

		for(int iPmt = 1; iPmt < m_iNbPmts+1; iPmt++)
		{
			float fS2Raw = (*m_pEventData->getS2sRaw())[iPeak][iPmt-1];
			float fGain = (m_pPmtCalibrationTable->find(iPmt)->second).find("gain")->second;
			float fS2 = (fGain != 0.)?(((fS2Raw > 0.)?(fS2Raw):(0.))/fGain):(0.);
			bool bExcludedFromCoincidence = m_pS2CoincidenceExcludedPmtSet->count(iPmt);
			bool bExcludedFromTotalSignal = m_pS2TotalSignalExcludedPmtSet->count(iPmt);

			if((isTopPmt(iPmt) || isBottomPmt(iPmt)) && !bExcludedFromTotalSignal)
			{
				hPeakS2.push_back(fS2);

				if(fS2 != 0.)
					hPeakS2Pmts.push_back(pair<float,int>(fS2,iPmt-1));

				if(isTopPmt(iPmt))
				{
					fPeakS2TotTop += fS2;
					if(!bExcludedFromCoincidence && (fS2 > m_fSignalThreshold))
						iPeakS2CoinTop++;
				}
				else if(isBottomPmt(iPmt))
				{
					fPeakS2TotBottom += fS2;
					if(!bExcludedFromCoincidence && (fS2 > m_fSignalThreshold))
						iPeakS2CoinBottom++;
				}
			}
			else
				hPeakS2.push_back(0.);
		}

		// compute the pmt order
		sort(hPeakS2Pmts.begin(), hPeakS2Pmts.end());
		vector<pair<float,int> >::reverse_iterator pPeakS2Pmt;
		for(pPeakS2Pmt = hPeakS2Pmts.rbegin(); pPeakS2Pmt < hPeakS2Pmts.rend(); pPeakS2Pmt++)
			hPeakS2PmtOrder.push_back(pPeakS2Pmt->second);

		m_pEventData->m_pS2s->push_back(hPeakS2);
		m_pEventData->m_pS2sTot->push_back(fPeakS2TotTop+fPeakS2TotBottom);
		m_pEventData->m_pS2sTotTop->push_back(fPeakS2TotTop);
		m_pEventData->m_pS2sTotBottom->push_back(fPeakS2TotBottom);

		m_pEventData->m_fS2Tot += fPeakS2TotTop+fPeakS2TotBottom;
		m_pEventData->m_fS2TotTop += fPeakS2TotTop;
		m_pEventData->m_fS2TotBottom += fPeakS2TotBottom;

		m_pEventData->m_pS2sCoin->push_back(iPeakS2CoinTop+iPeakS2CoinBottom);
		m_pEventData->m_pS2sCoinTop->push_back(iPeakS2CoinTop);
		m_pEventData->m_pS2sCoinBottom->push_back(iPeakS2CoinBottom);

		m_pEventData->m_pS2sPmtOrder->push_back(hPeakS2PmtOrder);
	}

	// compute the total S2 in peaks after the largest
	bool bAfter = false;
	vector<int>::const_iterator pOrder;
	for(pOrder = m_pEventData->m_pS2Order->begin(); pOrder != m_pEventData->m_pS2Order->end(); pOrder++)
	{
		if(bAfter)
			m_pEventData->m_fS2TotAfter += (*m_pEventData->m_pS2sTot)[*pOrder];
		if(*pOrder == 0)
			bAfter = true;
	}

	// for all S1 peaks found in the veto
	for(int iPeak = 0; iPeak < m_pEventData->m_iNbS1PeaksVeto; iPeak++)
	{
		// the vector we will fill for one peak
		vector<float> hPeakS1Veto;
		float fPeakS1TotVeto = 0.;
		int iPeakS1CoinVeto = 0;

		for(int iPmt = 1; iPmt < m_iNbPmts+1; iPmt++)
		{
			float fS1RawVeto = (*m_pEventData->getS1sRawVeto())[iPeak][iPmt-1];
			float fGain = (m_pPmtCalibrationTable->find(iPmt)->second).find("gain")->second;
			float fS1 = (fGain != 0.)?(((fS1RawVeto > 0.)?(fS1RawVeto):(0.))/fGain):(0.);
			bool bExcludedFromCoincidence = m_pS1CoincidenceExcludedPmtSet->count(iPmt);			
			bool bExcludedFromTotalSignal = m_pS1TotalSignalExcludedPmtSet->count(iPmt);

			if((isTopVetoPmt(iPmt) || isBottomVetoPmt(iPmt)) && !bExcludedFromTotalSignal)
			{
				hPeakS1Veto.push_back(fS1);

				fPeakS1TotVeto += fS1;
				if(!bExcludedFromCoincidence && (fS1 > m_fSignalThreshold))
					iPeakS1CoinVeto++;
			}
			else
				hPeakS1Veto.push_back(0.);
		}

		m_pEventData->m_pS1sVeto->push_back(hPeakS1Veto);
		m_pEventData->m_pS1sTotVeto->push_back(fPeakS1TotVeto);

		m_pEventData->m_fS1TotVeto += fPeakS1TotVeto;

		m_pEventData->m_pS1sCoinVeto->push_back(iPeakS1CoinVeto);
	}

#ifdef ENABLE_TAC
	vector<float>::iterator pTacHeight;
	for(pTacHeight = m_pEventData->m_pTacHeight->begin(); pTacHeight != m_pEventData->m_pTacHeight->end(); pTacHeight++)
	{
#ifdef ENABLE_LIQ_SCI
		vector<int>::iterator pLiqSci;
		pLiqSci = find_if(m_pEventData->m_pLiqSciPeak->begin(), m_pEventData->m_pLiqSciPeak->end(), bind2nd(greater<int>(), 0));

		if(pLiqSci != m_pEventData->m_pLiqSciPeak->end())
		{
			int iLiqSci = pLiqSci-m_pEventData->m_pLiqSciPeak->begin();
			float fTacIntercept = (*m_pTacCalibrationTable)[iLiqSci][0];
			float fTacSlope = (*m_pTacCalibrationTable)[iLiqSci][1];

			float fTimeOfFlight = ((*pTacHeight)-fTacIntercept)/fTacSlope;

			m_pEventData->m_pTimeOfFlight->push_back(fTimeOfFlight);
		}
#else
		int iTac = pTacHeight-m_pEventData->m_pTacHeight->begin();

		float fTacIntercept = (*m_pTacCalibrationTable)[iTac][0];
		float fTacSlope = (*m_pTacCalibrationTable)[iTac][1];

		float fTimeOfFlight = ((*pTacHeight)-fTacIntercept)/fTacSlope;

		m_pEventData->m_pTimeOfFlight->push_back(fTimeOfFlight);
#endif
	}
#endif
}

void
AnalysisManager::reconstructPosition()
{
	// for all S2 peaks
	vector<vector<float> >::iterator pPeakS2;
	for(pPeakS2 = m_pEventData->m_pS2s->begin(); pPeakS2 != m_pEventData->m_pS2s->end(); pPeakS2++)
	{
		float fX, fY;

		if(m_bUseMaxPmtAlgorithm)
			m_pEventData->m_pS2sPosMaxPmt->push_back(m_pPositionReconstruction->reconstructPosition("MaxPmt", (*pPeakS2)));

#ifdef HAVE_LIBSVM
		if(m_bUseSvmAlgorithm)
			m_pEventData->m_pS2sPosSvm->push_back(m_pPositionReconstruction->reconstructPosition("Svm", (*pPeakS2)));
#endif // HAVE_LIBSVM

#ifdef HAVE_LIBNN
		if(m_bUseNnAlgorithm)
			m_pEventData->m_pS2sPosNn->push_back(m_pPositionReconstruction->reconstructPosition("Nn", (*pPeakS2)));
#endif // HAVE_LIBNN

#ifdef HAVE_LIBCHI2
		if(m_bUseChi2Algorithm)
			m_pEventData->m_pS2sPosChi2->push_back(m_pPositionReconstruction->reconstructPosition("Chi2", (*pPeakS2)));
#endif // HAVE_LIBCHI2

#ifdef HAVE_LIBFANN
		if(m_bUseFannAlgorithm)
			m_pEventData->m_pS2sPosFann->push_back(m_pPositionReconstruction->reconstructPosition("Fann", (*pPeakS2)));
#endif // HAVE_LIBFANN
	}
}

void
AnalysisManager::correctSignals()
{
	if(m_bCorrectForS1SpatialDependence)
	{
		float fS1Correction = 1.;

		if(m_pEventData->getNbS2Peaks() && m_pEventData->getNbS1Peaks())
		{
			float fX = m_pEventData->getS2sPosSvm(0)[0];
			float fY = m_pEventData->getS2sPosSvm(0)[1];
			float fR = min(1.*sqrt(fX*fX + fY*fY), 150.);

			float fZ = 1.85*min((m_pEventData->getS2sPeak(0)-m_pEventData->getS1sPeak(0))/100., 165.);

			fS1Correction = m_pSignalCorrections->getS1Correction(fR, fZ);
		}

		for(int iS1Id = 0; iS1Id < m_pEventData->getNbS1Peaks(); iS1Id++)
			m_pEventData->m_pcS1sTot->push_back(fS1Correction*m_pEventData->getS1sTot(iS1Id));

		m_pEventData->m_fcS1Tot = fS1Correction*m_pEventData->getS1Tot();
	}

	if(m_bCorrectForS2AxialDependence)
	{
		if(m_pEventData->getNbS1Peaks())
		{
			for(int iS2Id = 0; iS2Id < m_pEventData->getNbS2Peaks(); iS2Id++)
			{
				float fDriftTime = (m_pEventData->getS2sPeak(iS2Id)-m_pEventData->getS1sPeak(0))/100.;
				float fS2Correction = exp(fDriftTime/m_fElectronLifetime);

				float fcS2 = fS2Correction*m_pEventData->getS2sTot(iS2Id);
				float fcS2Top = fS2Correction*m_pEventData->getS2sTotTop(iS2Id);
				float fcS2Bottom = fS2Correction*m_pEventData->getS2sTotBottom(iS2Id);

				m_pEventData->m_pcS2sTot->push_back(fcS2);
				m_pEventData->m_pcS2sTotTop->push_back(fcS2Top);
				m_pEventData->m_pcS2sTotBottom->push_back(fcS2Bottom);

				m_pEventData->m_fcS2Tot += fcS2;
				m_pEventData->m_fcS2TotTop += fcS2Top;
				m_pEventData->m_fcS2TotBottom += fcS2Bottom;
			}
		}
	}
}

bool
AnalysisManager::isTopPmt(int iPmt)
{
	return (iPmt >= m_iFirstTopPmt) && (iPmt <= m_iLastTopPmt);
}

bool
AnalysisManager::isBottomPmt(int iPmt)
{
	return (iPmt >= m_iFirstBottomPmt) && (iPmt <= m_iLastBottomPmt);
}

bool
AnalysisManager::isTopVetoPmt(int iPmt)
{
	return (iPmt >= m_iFirstTopVetoPmt) && (iPmt <= m_iLastTopVetoPmt);
}

bool
AnalysisManager::isBottomVetoPmt(int iPmt)
{
	return (iPmt >= m_iFirstBottomVetoPmt) && (iPmt <= m_iLastBottomVetoPmt);
}

