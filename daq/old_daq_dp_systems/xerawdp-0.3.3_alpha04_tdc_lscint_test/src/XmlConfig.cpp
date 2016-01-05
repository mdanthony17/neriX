#include "XmlConfig.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <libxml++/libxml++.h>

using std::string;
using std::stringstream;
using std::ostringstream;
using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::ifstream;
using std::min;
using std::max;
using std::getline;

XmlConfig::XmlConfig()
{
	// default values
	m_hConfigFileContents = "";
	
	m_iProcessingLevel = 0;
	m_iUseTDC = 0;

	m_iFirstTopPmt = -1;
	m_iFirstBottomPmt = -1;
	m_iFirstTopVetoPmt = -1;
	m_iFirstBottomVetoPmt = -1;
	m_iFirstUnusedChannel = -1;

	m_iLastTopPmt = -1;
	m_iLastBottomPmt = -1;
	m_iLastTopVetoPmt = -1;
	m_iLastBottomVetoPmt = -1;
	m_iLastUnusedChannel = -1;

	m_fExternalGainTopPmts = 10.;
	m_fExternalGainBottomPmts = 10.;
	m_fExternalGainVetoPmts = 10.;

#ifdef ENABLE_LIQ_SCI
	m_iFirstLiqSciChannel = -1;
	m_iLastLiqSciChannel = -1;

	m_iLiqSciMultiplexingStart = -1;
#endif

#ifdef ENABLE_NAI
	m_iFirstNaiChannel = -1;
	m_iLastNaiChannel = -1;
#endif

#ifdef ENABLE_HPGE
	m_iFirstGeChannel = -1;
	m_iLastGeChannel = -1;
#endif

#ifdef ENABLE_TAC
	m_iFirstTacChannel = -1;
	m_iLastTacChannel = -1;
#endif

	m_iSingleNbBaselineSamples = -1;
	m_iSingleWindowStart = -1;
	m_iSingleWindowWidth = -1;
	m_iSinglePrePeakSamples = -1;
	m_iSinglePostPeakSamples = -1;

	m_bUseMaxPmtAlgorithm = false;
	m_bUseSvmAlgorithm = false;
	m_bUseNnAlgorithm = false;
	m_bUseChi2Algorithm = false;

	m_bCorrectForS1SpatialDependence = false;
	m_bCorrectForS2AxialDependence = false;
	m_fElectronLifetime = -1.;
}

XmlConfig::~XmlConfig()
{
}

bool
XmlConfig::readConfigFile(const string &hConfigFileName)
{
	ifstream hConfigFile(hConfigFileName.c_str(), std::ios::binary);
	ostringstream hStream(std::ios::binary);

	hStream << hConfigFile.rdbuf();
	m_hConfigFileContents = hStream.str();
	hConfigFile.close();

	xmlpp::DomParser hXmlParser;
	hXmlParser.parse_memory(m_hConfigFileContents);

	const xmlpp::Node* pRootNode = hXmlParser.get_document()->get_root_node();

	readGlobalSettings(pRootNode);

	if(!readPmtSettings(pRootNode))
		return false;

	readTriggerEfficiencySettings(pRootNode);

#ifdef ENABLE_LIQ_SCI
	if(!readLiqSciSettings(pRootNode))
		return false;
#endif

#ifdef ENABLE_NAI
	readNaiSettings(pRootNode);
#endif

#ifdef ENABLE_HPGE
	readGeSettings(pRootNode);
#endif

#ifdef ENABLE_TAC
	readTacSettings(pRootNode);
#endif

	readSinglePhotoelectronSettings(pRootNode);

	readRawDataSettings(pRootNode);

	readPeakFindingSettings(pRootNode);

	readSignalsSettings(pRootNode);

	readPositionReconstructionSettings(pRootNode);

	if(!readSignalCorrectionsSettings(pRootNode))
		return false;

	return true;
}

void
XmlConfig::printSettings()
{
	// print everything
	cout
		<< "  XmlConfig" << endl
		<< "    Global" << endl
		<< "      DatasetNameFormat: " << m_hDatasetNameFormat << endl
		<< "      RawDataDir: " << m_hRawDataDir << endl
		<< "      ProcessedDataDir: " << m_hProcessedDataDir << endl
		<< "      MergedDataDir: " << m_hMergedDataDir << endl
		<< "      ProcessingLevel: " << m_iProcessingLevel << endl
		<< "    Pmts" << endl
		<< "      TopPmts: " << m_hTopPmts << endl
		<< "      BottomPmts: " << m_hBottomPmts << endl
		<< "      TopVetoPmts: " << m_hTopVetoPmts << endl
		<< "      BottomVetoPmts: " << m_hBottomVetoPmts << endl
		<< "      UnusedChannels: " << m_hUnusedChannels << endl;

	if(m_hExternalGains.size())
	{
		cout
			<< "      ExternalGains: " << m_hExternalGains << endl;
	}
	else
	{
		cout
			<< "      ExternalGain " << endl
			<< "        TopPmts: " << m_fExternalGainTopPmts << endl
			<< "        BottomPmts: " << m_fExternalGainBottomPmts << endl
			<< "        VetoPmts: " << m_fExternalGainVetoPmts << endl;
	}

	if(m_iProcessingLevel == 0)
	{
		cout
			<< "    SinglePhotoelectron:" << endl
			<< "      NbBaselineSamples: " << m_iSingleNbBaselineSamples << endl
			<< "      WindowStart: " << m_iSingleWindowStart << endl
			<< "      WindowWidth: " << m_iSingleWindowWidth << endl
			<< "      PrePeakSamples: " << m_iSinglePrePeakSamples << endl
			<< "      PostPeakSamples: " << m_iSinglePostPeakSamples << endl;
	}

	if(m_iProcessingLevel > 0)
	{
		cout
			<< "      CalibrationSource: " << m_hPmtCalibrationSource << endl;

		cout
			<< "    TriggerEfficiency" << endl
			<< "      NbTriggerSignals: " << m_iNbTriggerSignals << endl
			<< "      Channels: " << m_hTriggerEfficiencyChannels << endl
			<< "      TriggerWindowStart: " << m_iTriggerWindowStart << endl
			<< "      TriggerWindowWidth: " << m_iTriggerWindowWidth << endl
			<< "      TriggerSignalHeightThreshold: " << m_fTriggerSignalHeightThreshold << endl;

#ifdef ENABLE_LIQ_SCI
		cout
			<< "    LiquidScintillators" << endl
			<< "      NbLiquidScintillators: " << m_iNbLiqScis << endl
			<< "      Channels: " << m_hLiqSciChannels << endl
			<< "      TailStart: " << m_iLiqSciTailStart << endl
			<< "      Multiplexed: " << m_bLiqSciMultiplexed << endl;
		if(m_bLiqSciMultiplexed)
		{
			cout
				<< "      MultiplexingStart: " << m_iLiqSciMultiplexingStart << endl
				<< "      MultiplexingWindows: " << m_hLiqSciMultiplexingWindows << endl
				<< "      MultiplexingSignalHeightThreshold: " << m_fLiqSciMultiplexingSignalHeightThreshold << endl
				<< "      MultiplexingSignalWidthThreshold: " << m_iLiqSciMultiplexingSignalWidthThreshold << endl;
		}
#endif

#ifdef ENABLE_NAI
		cout
			<< "    SodiumIodideDetectors" << endl
			<< "      NbSodiumIodideDetectors: " << m_iNbNaiDetectors << endl
			<< "      Channels: " << m_hNaiChannels << endl;
#endif

#ifdef ENABLE_HPGE
		cout
			<< "    GermaniumDetectors" << endl
			<< "      NbGermaniumDetectors: " << m_iNbGeDetectors << endl
			<< "      Channels: " << m_hGeChannels << endl;
#endif

#ifdef ENABLE_TAC
		cout
			<< "    TimeToAmplitudeConverters" << endl
			<< "      NbTimeToAmplitudeConverters: " << m_iNbTacs << endl
			<< "      Channels: " << m_hTacChannels << endl
			<< "      WindowStart: " << m_iTacWindowStart << endl
			<< "      WindowWidth: " << m_iTacWindowWidth << endl
			<< "      Calibration: " << m_hTacCalibration << endl;
#endif

		cout
			<< "    RawData" << endl
			<< "      NbBaselineSamples: " << m_iNbBaselineSamples << endl
			<< "    PeakFinding" << endl
			<< "      S1: " << endl
			<< "        ExcludedPmts: " << m_hS1PeakFindingExcludedPmts << endl;

		if(m_bS1Filter)
		{
			cout
				<< "        Filter: " << endl
				<< "          Type: " << m_hS1FilterType << endl;
			if(m_hS1FilterType == "RaisedCosine")
			{
				cout
					<< "          LowPass: " << m_bS1RaisedCosineLowPass << endl
					<< "          Limit: " << m_fS1RaisedCosineLimit << endl
					<< "          RollOff: " << m_fS1RaisedCosineRollOff << endl
					<< "          Length: " << m_iS1RaisedCosineLength << endl;
			}
			else if(m_hS1FilterType == "Custom")
			{
				cout
					<< "            Coefficients: " << m_hS1CustomFilterCoefficientList << endl;
			}
		}

		cout
			<< "        MaxNbPeaks: " << m_iS1MaxNbPeaks << endl
			<< "        RightLimitHeightThreshold: " << m_fS1RightLimitHeightThreshold << endl
			<< "        SignalThreshold: " << m_fS1SignalThreshold << endl
			<< "        PeakWindow: " << m_iS1PeakWindow << endl
			<< "        PrePeakSamples: " << m_iS1PrePeakSamples << endl
			<< "        MaxLength: " << m_iS1MaxLength << endl
			<< "        PrePeakAvgWindow: " << m_iS1PrePeakAvgWindow << endl
			<< "        PostPeakAvgWindow: " << m_iS1PostPeakAvgWindow << endl
			<< "        PrePeakAvgThreshold: " << m_fS1PrePeakAvgThreshold << endl
			<< "        PostPeakAvgThreshold: " << m_fS1PostPeakAvgThreshold << endl
			<< "        FilteredWidthThreshold: " << m_fS1FilteredWidthThreshold << endl
			<< "        NegativeExcursionFractionThreshold: " << m_fS1NegativeExcursionFractionThreshold << endl
			<< "        HeightFractionThreshold: " << m_fS1HeightFractionThreshold << endl
			<< "        SamplesBelowThreshold: " << m_iS1SamplesBelowThreshold << endl;

		cout
			<< "      S2" << endl
			<< "        ExcludedPmts: " << m_hS2PeakFindingExcludedPmts << endl
			<< "        MaxNbPeaks: " << m_iS2MaxNbPeaks << endl;

		cout
			<< "        LargePeaks" << endl;

		if(m_bS2LargePeakFilter)
		{
			cout
				<< "          Filter" << endl
				<< "            Type: " << m_hS2LargePeakFilterType << endl;
			if(m_hS2LargePeakFilterType == "RaisedCosine")
			{
				cout
					<< "            LowPass: " << m_bS2LargePeakRaisedCosineLowPass << endl
					<< "            Limit: " << m_fS2LargePeakRaisedCosineLimit << endl
					<< "            RollOff: " << m_fS2LargePeakRaisedCosineRollOff << endl
					<< "            Length: " << m_iS2LargePeakRaisedCosineLength << endl;
			}
		}

		cout
			<< "          SignalThreshold: " << m_fS2LargePeakSignalThreshold << endl
			<< "          DynamicFractionSignalThreshold: " << m_fS2LargePeakDynamicFractionSignalThreshold << endl
			<< "          MinWidth: " << m_iS2LargePeakMinWidth << endl
			<< "          MinIntervalWidth: " << m_iS2LargePeakMinIntervalWidth << endl
			<< "          PreIntervalAvgWindow: " << m_iS2LargePeakPreIntervalAvgWindow << endl
			<< "          PostIntervalAvgWindow: " << m_iS2LargePeakPostIntervalAvgWindow << endl
			<< "          PreTopLevelIntervalAvgThreshold: " << m_fS2LargePeakPreTopLevelIntervalAvgThreshold << endl
			<< "          PostTopLevelIntervalAvgThreshold: " << m_fS2LargePeakPostTopLevelIntervalAvgThreshold << endl
			<< "          PreIntervalAvgThreshold: " << m_fS2LargePeakPreIntervalAvgThreshold << endl
			<< "          PostIntervalAvgThreshold: " << m_fS2LargePeakPostIntervalAvgThreshold << endl
			<< "          OverlappingPeaksThreshold: " << m_fS2LargePeakOverlappingPeaksThreshold << endl
			<< "          LeftHeightFractionThreshold: " << m_fS2LargePeakLeftHeightFractionThreshold << endl
			<< "          RightHeightFractionThreshold: " << m_fS2LargePeakRightHeightFractionThreshold << endl
			<< "          LargePeakSlopeThreshold: " << m_fS2LargePeakSlopeThreshold << endl
			<< "          SmallPeakSlopeThreshold: " << m_fS2SmallPeakSlopeThreshold << endl
			<< "          SmallPeakThreshold: " << m_fS2SmallPeakThreshold << endl;

		cout
			<< "        TinyPeaks" << endl;

		if(m_bS2TinyPeakFilter)
		{
			cout
				<< "          Filter" << endl
				<< "            Type: " << m_hS2TinyPeakFilterType << endl;
			if(m_hS2TinyPeakFilterType == "RaisedCosine")
			{
				cout
					<< "            LowPass: " << m_bS2TinyPeakRaisedCosineLowPass << endl
					<< "            Limit: " << m_fS2TinyPeakRaisedCosineLimit << endl
					<< "            RollOff: " << m_fS2TinyPeakRaisedCosineRollOff << endl
					<< "            Length: " << m_iS2TinyPeakRaisedCosineLength << endl;
			}
			else if(m_hS2TinyPeakFilterType == "Custom")
			{
				cout
					<< "            Coefficients: " << m_hS2TinyPeakCustomFilterCoefficientList << endl;
			}
		}

		cout
			<< "          RightLimitHeightThreshold: " << m_fS2TinyPeakRightLimitHeightThreshold << endl
			<< "          SignalThreshold: " << m_fS2TinyPeakSignalThreshold << endl
			<< "          MinIntervalWidth: " << m_iS2TinyPeakMinIntervalWidth << endl
			<< "          MaxIntervalWidth: " << m_iS2TinyPeakMaxIntervalWidth << endl
			<< "          PrePeakAvgWindow: " << m_iS2TinyPeakPrePeakAvgWindow << endl
			<< "          PostPeakAvgWindow: " << m_iS2TinyPeakPostPeakAvgWindow << endl
			<< "          PrePeakAvgThreshold: " << m_fS2TinyPeakPrePeakAvgThreshold << endl
			<< "          PostPeakAvgThreshold: " << m_fS2TinyPeakPostPeakAvgThreshold << endl
			<< "          AspectRatioThreshold: " << m_fS2TinyPeakAspectRatioThreshold << endl
			<< "          HeightFractionThreshold: " << m_fS2TinyPeakHeightFractionThreshold << endl;

		cout
			<< "    Signals" << endl
			<< "      SignalThreshold: " << m_fSignalThreshold << endl
			<< "      SaturationThreshold: " << m_fSaturationThreshold << endl
			<< "      S1" << endl
			<< "        CoincidenceExcludedPmts: " << m_hS1CoincidenceExcludedPmts << endl
			<< "        TotalSignalExcludedPmts: " << m_hS1TotalSignalExcludedPmts << endl
			<< "        CoincidenceWindow: " << m_iS1CoincidenceWindow << endl
			<< "      S2" << endl
			<< "        CoincidenceExcludedPmts: " << m_hS2CoincidenceExcludedPmts << endl
			<< "        TotalSignalExcludedPmts: " << m_hS2TotalSignalExcludedPmts << endl
			<< "        CoincidenceWindow: " << m_iS2CoincidenceWindow << endl;

		cout
			<< "    PositionReconstruction " << endl
			<< "      RelativeEfficiencies: " << m_hRelativeEfficiencies << endl;

		if(m_bUseMaxPmtAlgorithm)
		{
			cout
				<< "      MaxPmt: " << endl;
		}

		if(m_bUseSvmAlgorithm)
		{
			cout
				<< "      Svm: " << endl
				<< "        RModelFileName: " << m_hSvmRModelFileName << endl
				<< "        XModelFileName: " << m_hSvmXModelFileName << endl
				<< "        YModelFileName: " << m_hSvmYModelFileName << endl;
		}

		if(m_bUseNnAlgorithm)
		{
			cout
				<< "      Nn: " << endl
				<< "        XYModelName: " << m_hNnXYModelName << endl;
		}

		if(m_bUseChi2Algorithm)
		{
			cout
				<< "      Chi2: " << endl
				<< "        LceMapFileName: " << m_hChi2LceMapFileName << endl;
		}

		if(m_bUseFannAlgorithm)
		{
			cout
				<< "      Fann: " << endl
				<< "        XYModelFileName: " << m_hFannXYModelFileName << endl;
		}

		cout
			<< "    SignalCorrections " << endl;

		if(m_bCorrectForS1SpatialDependence)
		{
			cout
				<< "      S1: " << endl
				<< "        SpatialCorrection: " << endl;
		}

		if(m_bCorrectForS2AxialDependence)
		{
			cout
				<< "      S2: " << endl
				<< "        AxialCorrection: " << endl
				<< "          ElectronLifetime: " << m_fElectronLifetime << endl;
		}
	}
}

bool
XmlConfig::readGlobalSettings(const xmlpp::Node* pRootNode)
{
	stringstream hStream;

	// get global settings
	const xmlpp::NodeSet hGlobalNodeSet = pRootNode->find("/xerawdp/global/*");

	for(int i = 0; i < (int) hGlobalNodeSet.size(); i++)
	{
		string hName = hGlobalNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hGlobalNodeSet[i]->get_children().front())->get_content());

		if(hName == "verbosity")
			hStream >> m_iVerbosity;
		else if(hName == "dataset_name_format")
			m_hDatasetNameFormat = hStream.str();
		else if(hName == "raw_data_dir")
			m_hRawDataDir = hStream.str();
		else if(hName == "processed_data_dir")
			m_hProcessedDataDir = hStream.str();
		else if(hName == "merged_data_dir")
			m_hMergedDataDir = hStream.str();
		else if(hName == "processing_level")
			hStream >> m_iProcessingLevel;
		else if(hName == "use_tdc")
			hStream >> m_iUseTDC;

		hStream.clear();
	}	
}

bool
XmlConfig::readSinglePhotoelectronSettings(const xmlpp::Node* pRootNode)
{
	stringstream hStream;

	// get global settings
	const xmlpp::NodeSet hSinglePhotoelectronNodeSet = pRootNode->find("/xerawdp/single_photoelectron/*");

	for(int i = 0; i < (int) hSinglePhotoelectronNodeSet.size(); i++)
	{
		string hName = hSinglePhotoelectronNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hSinglePhotoelectronNodeSet[i]->get_children().front())->get_content());

		if(hName == "nb_baseline_samples")
			hStream >> m_iSingleNbBaselineSamples;
		else if(hName == "window_start")
			hStream >> m_iSingleWindowStart;
		else if(hName == "window_width")
			hStream >> m_iSingleWindowWidth;
		else if(hName == "pre_peak_samples")
			hStream >> m_iSinglePrePeakSamples;
		else if(hName == "post_peak_samples")
			hStream >> m_iSinglePostPeakSamples;

		hStream.clear();
	}	
}

bool
XmlConfig::readPmtSettings(const xmlpp::Node* pRootNode)
{
	stringstream hStream;	

	// get pmt number settings
	const xmlpp::NodeSet hPmtsNodeSet = pRootNode->find("/xerawdp/pmts/*");

	for(int i = 0; i < (int) hPmtsNodeSet.size(); i++)
	{
		string hName = hPmtsNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hPmtsNodeSet[i]->get_children().front())->get_content());

		if(hName == "top_pmts")
		{
			m_hTopPmts = hStream.str();
			parsePmtString(m_hTopPmts, m_iFirstTopPmt, m_iLastTopPmt);
		}
		else if(hName == "bottom_pmts")
		{
			m_hBottomPmts = hStream.str();
			parsePmtString(m_hBottomPmts, m_iFirstBottomPmt, m_iLastBottomPmt);
		}
		else if(hName == "top_veto_pmts")
		{
			m_hTopVetoPmts = hStream.str();
			parsePmtString(m_hTopVetoPmts, m_iFirstTopVetoPmt, m_iLastTopVetoPmt);
		}
		else if(hName == "bottom_veto_pmts")
		{
			m_hBottomVetoPmts = hStream.str();
			parsePmtString(m_hBottomVetoPmts, m_iFirstBottomVetoPmt, m_iLastBottomVetoPmt);
		}
		else if(hName == "unused_channels")
		{
			m_hUnusedChannels = hStream.str();
			parsePmtString(m_hUnusedChannels, m_iFirstUnusedChannel, m_iLastUnusedChannel);
		}

		hStream.clear();
	}

	// get pmt external gains settings
	const xmlpp::NodeSet hPmtExternalGainsNodeSet = pRootNode->find("/xerawdp/pmts/*");

	for(int i = 0; i < (int) hPmtExternalGainsNodeSet.size(); i++)
	{
		string hName = hPmtExternalGainsNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hPmtExternalGainsNodeSet[i]->get_children().front())->get_content());

		if(hName == "external_gains")
		{
			m_hExternalGains = hStream.str();
			parsePmtExternalGainsString(m_hExternalGains, m_hExternalGainsTable);
		}

		hStream.clear();
	}

	// get pmt external gain settings
	const xmlpp::NodeSet hPmtExternalGainNodeSet = pRootNode->find("/xerawdp/pmts/external_gain/*");

	for(int i = 0; i < (int) hPmtExternalGainNodeSet.size(); i++)
	{
		string hName = hPmtExternalGainNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hPmtExternalGainNodeSet[i]->get_children().front())->get_content());

		if(hName == "top_pmts")
			hStream >> m_fExternalGainTopPmts;
		else if(hName == "bottom_pmts")
			hStream >> m_fExternalGainBottomPmts;
		else if(hName == "veto_pmts")
			hStream >> m_fExternalGainVetoPmts;

		hStream.clear();
	}

	// fill the external gains table if it is empty
	if(!m_hExternalGainsTable.size())
	{
		for(int iPmt = m_iFirstTopPmt; iPmt <= m_iLastTopPmt; iPmt++)
			m_hExternalGainsTable[iPmt] = m_fExternalGainTopPmts;

		for(int iPmt = m_iFirstBottomPmt; iPmt <= m_iLastBottomPmt; iPmt++)
			m_hExternalGainsTable[iPmt] = m_fExternalGainBottomPmts;

		for(int iPmt = m_iFirstTopVetoPmt; iPmt <= m_iLastTopVetoPmt; iPmt++)
			m_hExternalGainsTable[iPmt] = m_fExternalGainVetoPmts;

		for(int iPmt = m_iFirstBottomVetoPmt; iPmt <= m_iLastBottomVetoPmt; iPmt++)
			m_hExternalGainsTable[iPmt] = m_fExternalGainVetoPmts;
	}

	// get pmt calibration source settings
	const xmlpp::NodeSet hPmtCalibrationSourceNodeSet = pRootNode->find("/xerawdp/pmts/calibration_source/*");

	for(int i = 0; i < (int) hPmtCalibrationSourceNodeSet.size(); i++)
	{
		string hName = hPmtCalibrationSourceNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hPmtCalibrationSourceNodeSet[i]->get_children().front())->get_content());

		if(hName == "file")
		{
			m_hPmtCalibrationSource = "file";
			m_hPmtCalibrationFileName = hStream.str();
		}
		else if(hName == "here")
			m_hPmtCalibrationSource = "here";
		else if(hName == "database")
			m_hPmtCalibrationSource = "database";

		hStream.clear();
	}

	// get pmt calibration source here settings
	const xmlpp::NodeSet hPmtHereNodeSet = pRootNode->find("/xerawdp/pmts/calibration_source/here/*");

	for(int i = 0; i < (int) hPmtHereNodeSet.size(); i++)
	{
		string hName = hPmtHereNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hPmtHereNodeSet[i]->get_children().front())->get_content());

		if(hName == "header")
			m_hPmtCalibrationHeader = hStream.str();
		else if(hName == "values")
			m_hPmtCalibrationValues = hStream.str();

		hStream.clear();
	}

	// get pmt calibration source database settings
	const xmlpp::NodeSet hPmtDatabaseNodeSet = pRootNode->find("/xerawdp/pmts/calibration_source/database/*");

	for(int i = 0; i < (int) hPmtDatabaseNodeSet.size(); i++)
	{
		string hName = hPmtDatabaseNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hPmtDatabaseNodeSet[i]->get_children().front())->get_content());

		if(hName == "files")
			m_hPmtCalibrationFiles = hStream.str();
		else if(hName == "header")
			m_hPmtCalibrationHeader = hStream.str();
		else if(hName == "values")
			m_hPmtCalibrationValues = hStream.str();

		hStream.clear();
	}

	if(!fillPmtCalibrationTable())
		return false;
	else
		return true;
}

bool
XmlConfig::readTriggerEfficiencySettings(const xmlpp::Node* pRootNode)
{
	stringstream hStream;	

	// get trigger efficiency settings
	const xmlpp::NodeSet hTriggerEfficiencyNodeSet = pRootNode->find("/xerawdp/trigger_efficiency/*");

	for(int i = 0; i < (int) hTriggerEfficiencyNodeSet.size(); i++)
	{
		string hName = hTriggerEfficiencyNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hTriggerEfficiencyNodeSet[i]->get_children().front())->get_content());

		if(hName == "nb_trigger_signals")
			hStream >> m_iNbTriggerSignals;
		else if(hName == "channels")
		{
			m_hTriggerEfficiencyChannels = hStream.str();
			parsePmtString(m_hTriggerEfficiencyChannels, m_iFirstTriggerEfficiencyChannel, m_iLastTriggerEfficiencyChannel);
		}
		else if(hName == "trigger_window_start")
			hStream >> m_iTriggerWindowStart;
		else if(hName == "trigger_window_width")
			hStream >> m_iTriggerWindowWidth;
		else if(hName == "trigger_signal_height_threshold")
			hStream >> m_fTriggerSignalHeightThreshold;

		hStream.clear();
	}
}

#ifdef ENABLE_LIQ_SCI
bool
XmlConfig::readLiqSciSettings(const xmlpp::Node* pRootNode)
{
	stringstream hStream;	

	// get liquid scintillator settings
	const xmlpp::NodeSet hLiquidScintillatorsNodeSet = pRootNode->find("/xerawdp/liquid_scintillators/*");

	for(int i = 0; i < (int) hLiquidScintillatorsNodeSet.size(); i++)
	{
		string hName = hLiquidScintillatorsNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hLiquidScintillatorsNodeSet[i]->get_children().front())->get_content());

		if(hName == "nb_liquid_scintillators")
			hStream >> m_iNbLiqScis;
		else if(hName == "channels")
		{
			m_hLiqSciChannels = hStream.str();
			parsePmtString(m_hLiqSciChannels, m_iFirstLiqSciChannel, m_iLastLiqSciChannel);
		}
		else if(hName == "tail_start")
			hStream >> m_iLiqSciTailStart;
		else if(hName == "multiplexed")
			hStream >> m_bLiqSciMultiplexed;
		else if(hName == "multiplexing_start")
			hStream >> m_iLiqSciMultiplexingStart;
		else if(hName == "multiplexing_windows")
		{
			m_hLiqSciMultiplexingWindows = hStream.str();
			parseLiqSciMultiplexingWindowsString(m_hLiqSciMultiplexingWindows, m_hLiqSciMultiplexingTable);
		}
		else if(hName == "multiplexing_signal_height_threshold")
			hStream >> m_fLiqSciMultiplexingSignalHeightThreshold;
		else if(hName == "multiplexing_signal_width_threshold")
			hStream >> m_iLiqSciMultiplexingSignalWidthThreshold;
		hStream.clear();
	}

	// verify liquid scintillator mutliplexing table
	return verifyLiqSciMultiplexingTable();
}
#endif

#ifdef ENABLE_NAI
bool
XmlConfig::readNaiSettings(const xmlpp::Node* pRootNode)
{
	stringstream hStream;	

	// get sodium iodide detector settings
	const xmlpp::NodeSet hNaiNodeSet = pRootNode->find("/xerawdp/sodium_iodide_detectors/*");

	for(int i = 0; i < (int) hNaiNodeSet.size(); i++)
	{
		string hName = hNaiNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hNaiNodeSet[i]->get_children().front())->get_content());

		if(hName == "nb_sodium_iodide_detectors")
			hStream >> m_iNbNaiDetectors;
		else if(hName == "channels")
		{
			m_hNaiChannels = hStream.str();
			parsePmtString(m_hNaiChannels, m_iFirstNaiChannel, m_iLastNaiChannel);
		}

		hStream.clear();
	}
}
#endif

#ifdef ENABLE_HPGE
bool
XmlConfig::readGeSettings(const xmlpp::Node* pRootNode)
{
	stringstream hStream;	

	// get germanium detector settings
	const xmlpp::NodeSet hGeNodeSet = pRootNode->find("/xerawdp/germanium_detectors/*");

	for(int i = 0; i < (int) hGeNodeSet.size(); i++)
	{
		string hName = hGeNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hGeNodeSet[i]->get_children().front())->get_content());

		if(hName == "nb_germanium_detectors")
			hStream >> m_iNbGeDetectors;
		else if(hName == "channels")
		{
			m_hGeChannels = hStream.str();
			parsePmtString(m_hGeChannels, m_iFirstGeChannel, m_iLastGeChannel);
		}

		hStream.clear();
	}
}
#endif

#ifdef ENABLE_TAC
bool
XmlConfig::readTacSettings(const xmlpp::Node* pRootNode)
{
	stringstream hStream;	

	// get tac settings
	const xmlpp::NodeSet hTimeToAmplitudeConvertersNodeSet = pRootNode->find("/xerawdp/time_to_amplitude_converters/*");

	for(int i = 0; i < (int) hTimeToAmplitudeConvertersNodeSet.size(); i++)
	{
		string hName = hTimeToAmplitudeConvertersNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hTimeToAmplitudeConvertersNodeSet[i]->get_children().front())->get_content());

		if(hName == "nb_time_to_amplitude_converters")
			hStream >> m_iNbTacs;
		else if(hName == "channels")
		{
			m_hTacChannels = hStream.str();
			parsePmtString(m_hTacChannels, m_iFirstTacChannel, m_iLastTacChannel);
		}
		else if(hName == "window_start")
			hStream >> m_iTacWindowStart;
		else if(hName == "window_width")
			hStream >> m_iTacWindowWidth;
		else if(hName == "calibration")
		{
			m_hTacCalibration = hStream.str();
			parseTacCalibrationString(m_hTacCalibration, m_hTacCalibrationTable);
		}

		hStream.clear();
	}
}
#endif

bool
XmlConfig::readRawDataSettings(const xmlpp::Node* pRootNode)
{
	stringstream hStream;	

	// get raw data settings
	const xmlpp::NodeSet hRawDataNodeSet = pRootNode->find("/xerawdp/raw_data/*");

	for(int i = 0; i < (int) hRawDataNodeSet.size(); i++)
	{
		string hName = hRawDataNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hRawDataNodeSet[i]->get_children().front())->get_content());

		if(hName == "nb_baseline_samples")
			hStream >> m_iNbBaselineSamples;

		hStream.clear();
	}

	return true;
}

bool
XmlConfig::readPeakFindingSettings(const xmlpp::Node* pRootNode)
{
	stringstream hStream;	

	// get s1 peak finding raised cosine filter settings
	const xmlpp::NodeSet hS1RaisedCosineNodeSet = pRootNode->find("/xerawdp/peak_finding/s1/filter/raised_cosine/*");

	if(hS1RaisedCosineNodeSet.size())
	{
		m_bS1Filter = true;
		m_hS1FilterType = "RaisedCosine";
	}

	for(int i = 0; i < (int) hS1RaisedCosineNodeSet.size(); i++)
	{
		string hName = hS1RaisedCosineNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hS1RaisedCosineNodeSet[i]->get_children().front())->get_content());

		if(hName == "low_pass")
			hStream >> m_bS1RaisedCosineLowPass;
		else if(hName == "limit")
			hStream >> m_fS1RaisedCosineLimit;
		else if(hName == "roll_off")
			hStream >> m_fS1RaisedCosineRollOff;
		else if(hName == "length")
			hStream >> m_iS1RaisedCosineLength;

		hStream.clear();
	}

	// get s1 custom filter settings
	const xmlpp::NodeSet hS1CustomFilterNodeSet = pRootNode->find("/xerawdp/peak_finding/s1/filter/custom/*");

	if(hS1CustomFilterNodeSet.size())
	{
		m_bS1Filter = true;
		m_hS1FilterType = "Custom";
	}

	for(int i = 0; i < (int) hS1CustomFilterNodeSet.size(); i++)
	{
		string hName = hS1CustomFilterNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hS1CustomFilterNodeSet[i]->get_children().front())->get_content());

		if(hName == "coefficients")
		{
			m_hS1CustomFilterCoefficientList = hStream.str();
			parseValueList(m_hS1CustomFilterCoefficientList, m_hS1CustomFilterCoefficients);
		}

		hStream.clear();
	}

	// get s1 peak finding settings
	const xmlpp::NodeSet hS1NodeSet = pRootNode->find("/xerawdp/peak_finding/s1/*");

	for(int i = 0; i < (int) hS1NodeSet.size(); i++)
	{
		string hName = hS1NodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hS1NodeSet[i]->get_children().front())->get_content());

		if(hName == "excluded_pmts")
		{
			m_hS1PeakFindingExcludedPmts = hStream.str();
			parsePmtEnumerationString(m_hS1PeakFindingExcludedPmts, m_hS1PeakFindingExcludedPmtSet);
		}
		else if(hName == "max_nb_peaks")
			hStream >> m_iS1MaxNbPeaks;
		else if(hName == "right_limit_height_threshold")
			hStream >> m_fS1RightLimitHeightThreshold;
		else if(hName == "signal_threshold")
			hStream >> m_fS1SignalThreshold;
		else if(hName == "peak_window")
			hStream >> m_iS1PeakWindow;
		else if(hName == "pre_peak_samples")
			hStream >> m_iS1PrePeakSamples;
		else if(hName == "max_length")
			hStream >> m_iS1MaxLength;
		else if(hName == "pre_peak_avg_window")
			hStream >> m_iS1PrePeakAvgWindow;
		else if(hName == "post_peak_avg_window")
			hStream >> m_iS1PostPeakAvgWindow;
		else if(hName == "pre_peak_avg_threshold")
			hStream >> m_fS1PrePeakAvgThreshold;
		else if(hName == "post_peak_avg_threshold")
			hStream >> m_fS1PostPeakAvgThreshold;
		else if(hName == "filtered_width_threshold")
			hStream >> m_fS1FilteredWidthThreshold;
		else if(hName == "negative_excursion_fraction_threshold")
			hStream >> m_fS1NegativeExcursionFractionThreshold;
		else if(hName == "height_fraction_threshold")
			hStream >> m_fS1HeightFractionThreshold;
		else if(hName == "samples_below_threshold")
			hStream >> m_iS1SamplesBelowThreshold;
		else if(hName == "coincidence_threshold")
			hStream >> m_fS1CoincidenceThreshold;

		hStream.clear();
	}

	// get s2 peak finding settings
	const xmlpp::NodeSet hS2NodeSet = pRootNode->find("/xerawdp/peak_finding/s2/*");

	for(int i = 0; i < (int) hS2NodeSet.size(); i++)
	{
		string hName = hS2NodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hS2NodeSet[i]->get_children().front())->get_content());

		if(hName == "excluded_pmts")
		{
			m_hS2PeakFindingExcludedPmts = hStream.str();
			parsePmtEnumerationString(m_hS2PeakFindingExcludedPmts, m_hS2PeakFindingExcludedPmtSet);
		}
		else if(hName == "max_nb_peaks")
			hStream >> m_iS2MaxNbPeaks;

		hStream.clear();		
	}

	// get s2 large peak raised cosine filter settings
	const xmlpp::NodeSet hS2LargePeakRaisedCosineNodeSet = pRootNode->find("/xerawdp/peak_finding/s2/large_peaks/filter/raised_cosine/*");

	if(hS2LargePeakRaisedCosineNodeSet.size())
	{
		m_bS2LargePeakFilter = true;
		m_hS2LargePeakFilterType = "RaisedCosine";
	}

	for(int i = 0; i < (int) hS2LargePeakRaisedCosineNodeSet.size(); i++)
	{
		string hName = hS2LargePeakRaisedCosineNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hS2LargePeakRaisedCosineNodeSet[i]->get_children().front())->get_content());

		if(hName == "low_pass")
			hStream >> m_bS2LargePeakRaisedCosineLowPass;
		else if(hName == "limit")
			hStream >> m_fS2LargePeakRaisedCosineLimit;
		else if(hName == "roll_off")
			hStream >> m_fS2LargePeakRaisedCosineRollOff;
		else if(hName == "length")
			hStream >> m_iS2LargePeakRaisedCosineLength;

		hStream.clear();
	}

	// get s2 large peak settings
	const xmlpp::NodeSet hS2LargePeaksNodeSet = pRootNode->find("/xerawdp/peak_finding/s2/large_peaks/*");

	for(int i = 0; i < (int) hS2LargePeaksNodeSet.size(); i++)
	{
		string hName = hS2LargePeaksNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hS2LargePeaksNodeSet[i]->get_children().front())->get_content());

		if(hName == "signal_threshold")
			hStream >> m_fS2LargePeakSignalThreshold;
		else if(hName == "dynamic_fraction_signal_threshold")
			hStream >> m_fS2LargePeakDynamicFractionSignalThreshold;
		else if(hName == "min_width")
			hStream >> m_iS2LargePeakMinWidth;
		else if(hName == "min_interval_width")
			hStream >> m_iS2LargePeakMinIntervalWidth;
		else if(hName == "pre_interval_avg_window")
			hStream >> m_iS2LargePeakPreIntervalAvgWindow;
		else if(hName == "post_interval_avg_window")
			hStream >> m_iS2LargePeakPostIntervalAvgWindow;
		else if(hName == "pre_top_level_interval_avg_threshold")
			hStream >> m_fS2LargePeakPreTopLevelIntervalAvgThreshold;
		else if(hName == "post_top_level_interval_avg_threshold")
			hStream >> m_fS2LargePeakPostTopLevelIntervalAvgThreshold;
		else if(hName == "pre_interval_avg_threshold")
			hStream >> m_fS2LargePeakPreIntervalAvgThreshold;
		else if(hName == "post_interval_avg_threshold")
			hStream >> m_fS2LargePeakPostIntervalAvgThreshold;
		else if(hName == "overlapping_peaks_threshold")
			hStream >> m_fS2LargePeakOverlappingPeaksThreshold;
		else if(hName == "left_height_fraction_threshold")
			hStream >> m_fS2LargePeakLeftHeightFractionThreshold;
		else if(hName == "right_height_fraction_threshold")
			hStream >> m_fS2LargePeakRightHeightFractionThreshold;
		else if(hName == "large_peak_slope_threshold")
			hStream >> m_fS2LargePeakSlopeThreshold;
		else if(hName == "small_peak_slope_threshold")
			hStream >> m_fS2SmallPeakSlopeThreshold;
		else if(hName == "small_peak_threshold")
			hStream >> m_fS2SmallPeakThreshold;

		hStream.clear();
	}

	// get s2 tiny peak raised cosine filter settings
	const xmlpp::NodeSet hS2TinyPeakRaisedCosineNodeSet = pRootNode->find("/xerawdp/peak_finding/s2/tiny_peaks/filter/raised_cosine/*");

	if(hS2TinyPeakRaisedCosineNodeSet.size())
	{
		m_bS2TinyPeakFilter = true;
		m_hS2TinyPeakFilterType = "RaisedCosine";
	}

	for(int i = 0; i < (int) hS2TinyPeakRaisedCosineNodeSet.size(); i++)
	{
		string hName = hS2TinyPeakRaisedCosineNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hS2TinyPeakRaisedCosineNodeSet[i]->get_children().front())->get_content());

		if(hName == "low_pass")
			hStream >> m_bS2TinyPeakRaisedCosineLowPass;
		else if(hName == "limit")
			hStream >> m_fS2TinyPeakRaisedCosineLimit;
		else if(hName == "roll_off")
			hStream >> m_fS2TinyPeakRaisedCosineRollOff;
		else if(hName == "length")
			hStream >> m_iS2TinyPeakRaisedCosineLength;

		hStream.clear();
	}

	// get s2 tiny peak custom filter settings
	const xmlpp::NodeSet hS2TinyPeakCustomFilterNodeSet = pRootNode->find("/xerawdp/peak_finding/s2/tiny_peaks/filter/custom/*");

	if(hS2TinyPeakCustomFilterNodeSet.size())
	{
		m_bS2TinyPeakFilter = true;
		m_hS2TinyPeakFilterType = "Custom";
	}

	for(int i = 0; i < (int) hS2TinyPeakCustomFilterNodeSet.size(); i++)
	{
		string hName = hS2TinyPeakCustomFilterNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hS2TinyPeakCustomFilterNodeSet[i]->get_children().front())->get_content());

		if(hName == "coefficients")
		{
			m_hS2TinyPeakCustomFilterCoefficientList = hStream.str();
			parseValueList(m_hS2TinyPeakCustomFilterCoefficientList, m_hS2TinyPeakCustomFilterCoefficients);
		}

		hStream.clear();
	}

	// get s2 tiny peak settings
	const xmlpp::NodeSet hS2TinyPeaksNodeSet = pRootNode->find("/xerawdp/peak_finding/s2/tiny_peaks/*");

	for(int i = 0; i < (int) hS2TinyPeaksNodeSet.size(); i++)
	{
		string hName = hS2TinyPeaksNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hS2TinyPeaksNodeSet[i]->get_children().front())->get_content());

		if(hName == "right_limit_height_threshold")
			hStream >> m_fS2TinyPeakRightLimitHeightThreshold;
		else if(hName == "signal_threshold")
			hStream >> m_fS2TinyPeakSignalThreshold;
		else if(hName == "min_interval_width")
			hStream >> m_iS2TinyPeakMinIntervalWidth;
		else if(hName == "max_interval_width")
			hStream >> m_iS2TinyPeakMaxIntervalWidth;
		else if(hName == "pre_peak_avg_window")
			hStream >> m_iS2TinyPeakPrePeakAvgWindow;
		else if(hName == "post_peak_avg_window")
			hStream >> m_iS2TinyPeakPostPeakAvgWindow;
		else if(hName == "pre_peak_avg_threshold")
			hStream >> m_fS2TinyPeakPrePeakAvgThreshold;
		else if(hName == "post_peak_avg_threshold")
			hStream >> m_fS2TinyPeakPostPeakAvgThreshold;
		else if(hName == "aspect_ratio_threshold")
			hStream >> m_fS2TinyPeakAspectRatioThreshold;
		else if(hName == "height_fraction_threshold")
			hStream >> m_fS2TinyPeakHeightFractionThreshold;

		hStream.clear();
	}

	return true;
}

bool
XmlConfig::readSignalsSettings(const xmlpp::Node* pRootNode)
{
	stringstream hStream;	

	// get signals settings
	const xmlpp::NodeSet hSignalsNodeSet = pRootNode->find("/xerawdp/signals/*");

	for(int i = 0; i < (int) hSignalsNodeSet.size(); i++)
	{
		string hName = hSignalsNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hSignalsNodeSet[i]->get_children().front())->get_content());

		if(hName == "signal_threshold")
			hStream >> m_fSignalThreshold;
		else if(hName == "saturation_threshold")
			hStream >> m_fSaturationThreshold;

		hStream.clear();
	}

	// get s1 signals settings
	const xmlpp::NodeSet hS1SignalsNodeSet = pRootNode->find("/xerawdp/signals/s1/*");

	for(int i = 0; i < (int) hS1SignalsNodeSet.size(); i++)
	{
		string hName = hS1SignalsNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hS1SignalsNodeSet[i]->get_children().front())->get_content());

		if(hName == "coincidence_excluded_pmts")
		{
			m_hS1CoincidenceExcludedPmts = hStream.str();
			parsePmtEnumerationString(m_hS1CoincidenceExcludedPmts, m_hS1CoincidenceExcludedPmtSet);
		}			
		else if(hName == "total_signal_excluded_pmts")
		{
			m_hS1TotalSignalExcludedPmts = hStream.str();
			parsePmtEnumerationString(m_hS1TotalSignalExcludedPmts, m_hS1TotalSignalExcludedPmtSet);
		}
		else if(hName == "coincidence_window")
			hStream >> m_iS1CoincidenceWindow;

		hStream.clear();
	}	

	// get s2 signals settings
	const xmlpp::NodeSet hS2SignalsNodeSet = pRootNode->find("/xerawdp/signals/s2/*");

	for(int i = 0; i < (int) hS2SignalsNodeSet.size(); i++)
	{
		string hName = hS2SignalsNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hS2SignalsNodeSet[i]->get_children().front())->get_content());

		if(hName == "coincidence_excluded_pmts")
		{
			m_hS2CoincidenceExcludedPmts = hStream.str();
			parsePmtEnumerationString(m_hS2CoincidenceExcludedPmts, m_hS2CoincidenceExcludedPmtSet);
		}
		else if(hName == "total_signal_excluded_pmts")
		{
			m_hS2TotalSignalExcludedPmts = hStream.str();
			parsePmtEnumerationString(m_hS2TotalSignalExcludedPmts, m_hS2TotalSignalExcludedPmtSet);
		}
		else if(hName == "coincidence_window")
			hStream >> m_iS2CoincidenceWindow;

		hStream.clear();
	}	
}

bool
XmlConfig::readPositionReconstructionSettings(const xmlpp::Node* pRootNode)
{
	stringstream hStream;

	// get position reconstruction settings
	const xmlpp::NodeSet hPositionReconstructionNodeSet = pRootNode->find("/xerawdp/position_reconstruction/*");

	for(int i = 0; i < (int) hPositionReconstructionNodeSet.size(); i++)
	{
		string hName = hPositionReconstructionNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hPositionReconstructionNodeSet[i]->get_children().front())->get_content());

		if(hName == "relative_efficiencies")
		{
			m_hRelativeEfficiencies = hStream.str();
			parseRelativeEfficienciesString(m_hRelativeEfficiencies, m_hRelativeEfficienciesTable);
		}

		hStream.clear();
	}

	// get max pmt algorithm settings
	const xmlpp::NodeSet hMaxPmtAlgorithmNodeSet = pRootNode->find("/xerawdp/position_reconstruction/max_pmt/*");

	for(int i = 0; i < (int) hMaxPmtAlgorithmNodeSet.size(); i++)
	{
		string hName = hMaxPmtAlgorithmNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hMaxPmtAlgorithmNodeSet[i]->get_children().front())->get_content());

		if(hName == "use_algorithm")
			hStream >> m_bUseMaxPmtAlgorithm;

		hStream.clear();
	}

	// get svm algorithm settings
	const xmlpp::NodeSet hSvmAlgorithmNodeSet = pRootNode->find("/xerawdp/position_reconstruction/svm/*");

	for(int i = 0; i < (int) hSvmAlgorithmNodeSet.size(); i++)
	{
		string hName = hSvmAlgorithmNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hSvmAlgorithmNodeSet[i]->get_children().front())->get_content());

		if(hName == "use_algorithm")
			hStream >> m_bUseSvmAlgorithm;
		else if(hName == "r_model_file_name")
			m_hSvmRModelFileName = hStream.str();
		else if(hName == "x_model_file_name")
			m_hSvmXModelFileName = hStream.str();
		else if(hName == "y_model_file_name")
			m_hSvmYModelFileName = hStream.str();

		hStream.clear();
	}

	// get neural network algorithm settings
	const xmlpp::NodeSet hNnAlgorithmNodeSet = pRootNode->find("/xerawdp/position_reconstruction/nn/*");

	for(int i = 0; i < (int) hNnAlgorithmNodeSet.size(); i++)
	{
		string hName = hNnAlgorithmNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hNnAlgorithmNodeSet[i]->get_children().front())->get_content());

		if(hName == "use_algorithm")
			hStream >> m_bUseNnAlgorithm;
		else if(hName == "xy_model_name")
			m_hNnXYModelName = hStream.str();

		hStream.clear();
	}	

	// get chi^2 algorithm settings
	const xmlpp::NodeSet hChi2AlgorithmNodeSet = pRootNode->find("/xerawdp/position_reconstruction/chi2/*");

	for(int i = 0; i < (int) hChi2AlgorithmNodeSet.size(); i++)
	{
		string hName = hChi2AlgorithmNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hChi2AlgorithmNodeSet[i]->get_children().front())->get_content());

		if(hName == "use_algorithm")
			hStream >> m_bUseChi2Algorithm;
		else if(hName == "lce_map_file_name")
			m_hChi2LceMapFileName = hStream.str();

		hStream.clear();
	}	

	// get fann algorithm settings
	const xmlpp::NodeSet hFannAlgorithmNodeSet = pRootNode->find("/xerawdp/position_reconstruction/fann/*");

	for(int i = 0; i < (int) hFannAlgorithmNodeSet.size(); i++)
	{
		string hName = hFannAlgorithmNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hFannAlgorithmNodeSet[i]->get_children().front())->get_content());

		if(hName == "use_algorithm")
			hStream >> m_bUseFannAlgorithm;
		else if(hName == "xy_model_file_name")
			m_hFannXYModelFileName = hStream.str();

		hStream.clear();
	}
}

bool
XmlConfig::readSignalCorrectionsSettings(const xmlpp::Node* pRootNode)
{
	stringstream hStream;	

	// get s1 spatial correction settings
	const xmlpp::NodeSet hS1SpatialCorrectionNodeSet = pRootNode->find("/xerawdp/signal_corrections/s1/spatial_correction/*");

	for(int i = 0; i < (int) hS1SpatialCorrectionNodeSet.size(); i++)
	{
		string hName = hS1SpatialCorrectionNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hS1SpatialCorrectionNodeSet[i]->get_children().front())->get_content());

		if(hName == "apply_correction")
			hStream >> m_bCorrectForS1SpatialDependence;
		if(hName == "electron_lifetime")
			hStream >> m_fElectronLifetime;

		hStream.clear();
	}

	// get s2 axial correction settings
	const xmlpp::NodeSet hS2AxialCorrectionNodeSet = pRootNode->find("/xerawdp/signal_corrections/s2/axial_correction/*");

	for(int i = 0; i < (int) hS2AxialCorrectionNodeSet.size(); i++)
	{
		string hName = hS2AxialCorrectionNodeSet[i]->get_name();
		hStream.str(((xmlpp::ContentNode *) hS2AxialCorrectionNodeSet[i]->get_children().front())->get_content());

		if(hName == "apply_correction")
			hStream >> m_bCorrectForS2AxialDependence;
		else if(hName == "electron_lifetime")
			hStream >> m_fElectronLifetime;

		hStream.clear();
	}

	if(m_bCorrectForS2AxialDependence && m_fElectronLifetime <= 0.)
	{
		cout << "xml config error: electron lifetime value required for s2 axial signal correction!" << endl;
		return false;
	}

	return true;
}

bool
XmlConfig::parsePmtString(const string &hString, int &iFirstPmt, int &iLastPmt)
{
	stringstream hTmp(hString);

	hTmp >> iFirstPmt;
	hTmp.ignore();
	hTmp >> iLastPmt;

	return true;
}

bool
XmlConfig::parsePmtExternalGainsString(const std::string &hString, ExternalGainsTable &hExternalGainsTable)
{
	stringstream hTmp(hString);
	string::size_type hNextDigit = hTmp.str().find_first_of("0123456789");
	hTmp.seekg(hNextDigit);

	hExternalGainsTable.clear();

	while(!hTmp.eof() && hTmp.good())
	{
		// get the pmt range
		int iFirstPmt, iLastPmt;
		hTmp >> iFirstPmt;
		if(hTmp.peek() == '-')
		{
			hTmp.ignore();
			hTmp >> iLastPmt;
		}
		else
			iLastPmt = iFirstPmt;

		// get the external gain
		float fExternalGain;
		hTmp.ignore();
		hTmp >> fExternalGain;

		for(int iPmt = iFirstPmt; iPmt <= iLastPmt; iPmt++)
			hExternalGainsTable[iPmt] = fExternalGain;

		// find the next digit
		hTmp.seekg(hTmp.str().find_first_of("0123456789", hTmp.tellg()));
	}

	return true;
}

bool
XmlConfig::parsePmtEnumerationString(const std::string &hString, PmtSet &hPmtSet)
{
	stringstream hTmp(hString);
	string::size_type hNextDigit = hTmp.str().find_first_of("0123456789");
	hTmp.seekg(hNextDigit);

	hPmtSet.clear();

	while(!hTmp.eof() && hTmp.good())
	{
		// get the pmt range
		int iFirstPmt, iLastPmt;
		hTmp >> iFirstPmt;
		if(hTmp.peek() == '-')
		{
			hTmp.ignore();
			hTmp >> iLastPmt;
		}
		else
			iLastPmt = iFirstPmt;

		for(int iPmt = iFirstPmt; iPmt <= iLastPmt; iPmt++)
			hPmtSet.insert(iPmt);

		// find the next digit
		hTmp.seekg(hTmp.str().find_first_of("0123456789", hTmp.tellg()));
	}

	return true;
}

bool
XmlConfig::parsePmtCalibrationHeader(const std::string &hHeader)
{
	stringstream hStream(hHeader);

	string hTmp;
	while(!hStream.eof())
	{
		hStream >> hTmp;
		m_hPmtCalibrationTableHeader.push_back(hTmp);	
	}

	// if this is the header of a calibration file
	if(m_hPmtCalibrationSource == "file")
	{
		// the pmt column is required
		if(count(m_hPmtCalibrationTableHeader.begin(), m_hPmtCalibrationTableHeader.end(), "pmt") == 0)
		{
			cout << "xml config error: the pmt calibration file needs to contain the pmt number column!" << endl;
			return false;
		}
	}

	// verify that at least the gain is there
	if(count(m_hPmtCalibrationTableHeader.begin(), m_hPmtCalibrationTableHeader.end(), "gain") == 0)
	{
		cout << "xml config error: the pmt calibration needs to contain at least the gain!" << endl;
		return false;
	}

	return true;
}

bool
XmlConfig::parsePmtCalibrationFile(const string &hFile)
{
	ifstream hIn(hFile.c_str());

	if(hIn.fail())
	{
		cout << "xml config error: cannot open file " << hFile << "!" << endl;
		return false;
	}

	// parse the header
	string hHeader;
	getline(hIn, hHeader);
	if(!parsePmtCalibrationHeader(hHeader))
		return false;

	m_hPmtCalibrationTable.clear();	

	// read the pmt calibration values as described by the calibration table header
	int iExpectedPmt = 1;
	while(!hIn.eof())
	{
		map<string, float> hCalibration;

		PmtCalibrationTableHeader::iterator pColumn;
		for(pColumn = m_hPmtCalibrationTableHeader.begin(); pColumn != m_hPmtCalibrationTableHeader.end(); pColumn++)
		{
			float fValue = 0.;

			hIn >> fValue;
			if(hIn.fail())
				break;
			hCalibration[*pColumn] = fValue;
		}

		if(!hIn.fail())
		{
			int iPmt = (int) hCalibration["pmt"];

			// store them in the pmt calibration table
			m_hPmtCalibrationTable[iPmt] = hCalibration;

			iExpectedPmt++;
		}
	}

	// verify pmt calibration table
	return verifyPmtCalibrationTable();
}

bool
XmlConfig::parsePmtCalibrationValues(const string &hValues)
{
	// parse the header
	if(!parsePmtCalibrationHeader(m_hPmtCalibrationHeader))
		return false;

	m_hPmtCalibrationTable.clear();

	string::size_type hPos = 0;
	string::size_type hColonPos;
	
	// find the next colon
	while((hColonPos = hValues.find(":", hPos)) != string::npos)
	{
		stringstream hTmp;
		
		hTmp.clear();
		hTmp.str(hValues.substr(hPos, hColonPos-hPos));

		// get the pmt range
		int iFirstPmt, iLastPmt;
		hTmp >> iFirstPmt;
		if(hTmp.peek() == '-')
		{
			hTmp.ignore();
			hTmp >> iLastPmt;
		}
		else
			iLastPmt = iFirstPmt;

		hPos = hColonPos+1;

		string::size_type hCommaPos;
		hTmp.clear();
		if((hCommaPos = hValues.find(",", hPos)) != string::npos)
			hTmp.str(hValues.substr(hPos+1, hCommaPos-hPos-2));
		else
			hTmp.str(hValues.substr(hPos+1, hValues.size()-hPos-2));

		// get the pmt calibration parameters
		while(!hTmp.eof())
		{
			map<string, float> hCalibration;

			PmtCalibrationTableHeader::iterator pColumn;
			for(pColumn = m_hPmtCalibrationTableHeader.begin(); pColumn != m_hPmtCalibrationTableHeader.end(); pColumn++)
			{
				float fValue = 0.;

				hTmp >> fValue;
				if(hTmp.fail())
					break;
				hCalibration[*pColumn] = fValue;
			}

			if(!hTmp.fail())
			{
				// store them in the pmt calibration table
				for(int iPmt = iFirstPmt; iPmt <= iLastPmt; iPmt++)
					m_hPmtCalibrationTable[iPmt] = hCalibration;
			}
		}

		hPos = hCommaPos+1;

		if(hCommaPos == string::npos)
			break;
	}

	// verify pmt calibration table
	return verifyPmtCalibrationTable();
}

bool
XmlConfig::fillPmtCalibrationTable()
{
	if(m_hPmtCalibrationSource == "file")
		return parsePmtCalibrationFile(m_hPmtCalibrationFileName);
	else if(m_hPmtCalibrationSource == "here" || m_hPmtCalibrationSource == "database")
		return parsePmtCalibrationValues(m_hPmtCalibrationValues);
}

bool
XmlConfig::verifyPmtCalibrationTable()
{
	int iFirstPmt = INT_MAX, iLastPmt = 1;

	if(m_iFirstTopPmt != -1)
	{
		iFirstPmt = min(iFirstPmt, m_iFirstTopPmt);
		iLastPmt = max(iLastPmt, m_iFirstTopPmt);
	}

	if(m_iFirstBottomPmt != -1)
	{
		iFirstPmt = min(iFirstPmt, m_iFirstBottomPmt);
		iLastPmt = max(iLastPmt, m_iFirstBottomPmt);
	}

	if(m_iFirstTopVetoPmt != -1)
	{
		iFirstPmt = min(iFirstPmt, m_iFirstTopVetoPmt);
		iLastPmt = max(iLastPmt, m_iFirstTopVetoPmt);
	}

	if(m_iFirstBottomVetoPmt != -1)
	{
		iFirstPmt = min(iFirstPmt, m_iFirstBottomVetoPmt);
		iLastPmt = max(iLastPmt, m_iFirstBottomVetoPmt);
	}

	for(int iPmt = iFirstPmt; iPmt <= iLastPmt; iPmt++)
	{
		if(m_hPmtCalibrationTable.count(iPmt) != 1)
		{
			cout << "xml config error: pmt calibration table failed sanity check!" << endl;
			return false;
		}
	}

	return true;
}

bool
XmlConfig::parseValueList(const std::string &hString, std::vector<float> &hValues)
{
	stringstream hTmp(hString);
	string::size_type hNextDigit = hTmp.str().find_first_of("-0123456789");
	hTmp.seekg(hNextDigit);

	hValues.clear();

	float fValue;

	while(!hTmp.eof() && hTmp.good())
	{
		hTmp >> fValue;
		hValues.push_back(fValue);

		// find the next digit
		hTmp.seekg(hTmp.str().find_first_of("-0123456789", hTmp.tellg()));
	}

	return true;
}

bool
XmlConfig::parseRelativeEfficienciesString(const std::string &hString, RelativeEfficienciesTable &hRelativeEfficienciesTable)
{
	stringstream hTmp(hString);
	string::size_type hNextDigit = hTmp.str().find_first_of("0123456789");
	hTmp.seekg(hNextDigit);

	hRelativeEfficienciesTable.clear();

	while(!hTmp.eof() && hTmp.good())
	{
		// get the pmt range
		int iFirstPmt, iLastPmt;
		hTmp >> iFirstPmt;
		if(hTmp.peek() == '-')
		{
			hTmp.ignore();
			hTmp >> iLastPmt;
		}
		else
			iLastPmt = iFirstPmt;

		// get the external gain
		float fRelativeSensitivity;
		hTmp.ignore();
		hTmp >> fRelativeSensitivity;

		for(int iPmt = iFirstPmt; iPmt <= iLastPmt; iPmt++)
			hRelativeEfficienciesTable[iPmt] = fRelativeSensitivity;

		// find the next digit
		hTmp.seekg(hTmp.str().find_first_of("0123456789", hTmp.tellg()));
	}

	return true;
}

#ifdef ENABLE_LIQ_SCI
void
XmlConfig::parseLiqSciMultiplexingWindowsString(const string &hString, LiqSciMultiplexingTable &hLiqSciMultiplexingTable)
{
	// create the header
	m_hLiqSciMultiplexingTableHeader.clear();
	m_hLiqSciMultiplexingTableHeader.push_back("window_start");
	m_hLiqSciMultiplexingTableHeader.push_back("window_width");

	m_hLiqSciMultiplexingTable.clear();

	string::size_type hPos = 0;
	string::size_type hColonPos;
	
	// find the next colon
	while((hColonPos = hString.find(":", hPos)) != string::npos)
	{
		stringstream hTmp;
		
		hTmp.clear();
		hTmp.str(hString.substr(hPos, hColonPos-hPos));

		// get the liquid scintillator channel range
		int iFirstLiqSciChannel, iLastLiqSciChannel;
		hTmp >> iFirstLiqSciChannel;
		if(hTmp.peek() == '-')
		{
			hTmp.ignore();
			hTmp >> iLastLiqSciChannel;
		}
		else
			iLastLiqSciChannel = iFirstLiqSciChannel;

		hPos = hColonPos+1;

		string::size_type hCommaPos;
		hTmp.clear();
		if((hCommaPos = hString.find(",", hPos)) != string::npos)
			hTmp.str(hString.substr(hPos+1, hCommaPos-hPos-2));
		else
			hTmp.str(hString.substr(hPos+1, hString.size()-hPos-2));

		// get the liquid scintillator multiplexing parameters
		while(!hTmp.eof())
		{
			LiqSciMultiplexingTableEntry hLiqSciMultiplexingTableEntry;

			LiqSciMultiplexingTableHeader::iterator pColumn;
			for(pColumn = m_hLiqSciMultiplexingTableHeader.begin(); pColumn != m_hLiqSciMultiplexingTableHeader.end(); pColumn++)
			{
				int iValue = 0;

				hTmp >> iValue;
				if(hTmp.fail())
					break;
				hLiqSciMultiplexingTableEntry.push_back(iValue);
			}

			if(!hTmp.fail())
			{
				// store them in the pmt calibration table
				for(int iLiqSciChannel = iFirstLiqSciChannel; iLiqSciChannel <= iLastLiqSciChannel; iLiqSciChannel++)
					m_hLiqSciMultiplexingTable.push_back(hLiqSciMultiplexingTableEntry);
			}
		}

		hPos = hCommaPos+1;

		if(hCommaPos == string::npos)
			break;
	}
}

bool
XmlConfig::verifyLiqSciMultiplexingTable()
{
	if(m_bLiqSciMultiplexed && (m_hLiqSciMultiplexingTable.size() != m_iNbLiqScis))
	{
		cout << "xml config error: liquid scintillator multiplexing table failed sanity check!" << endl;
		return false;
	}

	return true;
}
#endif

#ifdef ENABLE_TAC
void
XmlConfig::parseTacCalibrationString(const std::string &hString, TacCalibrationTable &hTacCalibrationTable)
{
	// create the header
	m_hTacCalibrationTableHeader.clear();
	m_hTacCalibrationTableHeader.push_back("intercept");
	m_hTacCalibrationTableHeader.push_back("slope");

	m_hTacCalibrationTable.clear();

	string::size_type hPos = 0;
	string::size_type hColonPos;
	
	// find the next colon
	while((hColonPos = hString.find(":", hPos)) != string::npos)
	{
		stringstream hTmp;
		
		hTmp.clear();
		hTmp.str(hString.substr(hPos, hColonPos-hPos));

		// get the channel range
		int iFirstChannel, iLastChannel;
		hTmp >> iFirstChannel;
		if(hTmp.peek() == '-')
		{
			hTmp.ignore();
			hTmp >> iLastChannel;
		}
		else
			iLastChannel = iFirstChannel;

		hPos = hColonPos+1;

		string::size_type hCommaPos;
		hTmp.clear();
		if((hCommaPos = hString.find(",", hPos)) != string::npos)
			hTmp.str(hString.substr(hPos+1, hCommaPos-hPos-2));
		else
			hTmp.str(hString.substr(hPos+1, hString.size()-hPos-2));

		// get the time to amplitude converter calibration parameters
		while(!hTmp.eof())
		{
			TacCalibrationTableEntry hTacCalibrationTableEntry;

			TacCalibrationTableHeader::iterator pColumn;
			for(pColumn = m_hTacCalibrationTableHeader.begin(); pColumn != m_hTacCalibrationTableHeader.end(); pColumn++)
			{
				float fValue = 0.;

				hTmp >> fValue;
				if(hTmp.fail())
					break;
				hTacCalibrationTableEntry.push_back(fValue);
			}

			if(!hTmp.fail())
			{
				// store them in the pmt calibration table
				for(int iChannel = iFirstChannel; iChannel <= iLastChannel; iChannel++)
					m_hTacCalibrationTable.push_back(hTacCalibrationTableEntry);
			}
		}

		hPos = hCommaPos+1;

		if(hCommaPos == string::npos)
			break;
	}
}

bool
XmlConfig::verifyTacCalibrationTable()
{
#ifdef ENABLE_LIQ_SCI
	if(m_hTacCalibrationTable.size() != (m_bLiqSciMultiplexed)?(m_iNbLiqScis):(m_iNbTacs))
	{
		cout << "xml config error: time to amplitude converter calibration table failed sanity check!" << endl;
		return false;
	}
#else
	if(m_hTacCalibrationTable.size() != m_iNbTacs)
	{
		cout << "xml config error: time to amplitude converter calibration table failed sanity check!" << endl;
		return false;
	}
#endif

	return true;
}
#endif

