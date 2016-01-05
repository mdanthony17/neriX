#ifndef __XMLCONFIG_H__
#define __XMLCONFIG_H__

#include "xerawdp.h"

#include <string>
#include <vector>
#include <map>
#include <set>

#include <libxml++/libxml++.h>

typedef std::map<int,float> ExternalGainsTable;
typedef std::vector<std::string> PmtCalibrationTableHeader;
typedef std::map<std::string,float> PmtCalibrationTableRow;
typedef std::map<int, PmtCalibrationTableRow> PmtCalibrationTable;
typedef std::set<int> PmtSet;

typedef std::vector<std::string> LiqSciMultiplexingTableHeader;
typedef std::vector<int> LiqSciMultiplexingTableEntry;
typedef std::vector<LiqSciMultiplexingTableEntry> LiqSciMultiplexingTable;

typedef std::vector<std::string> TacCalibrationTableHeader;
typedef std::vector<float> TacCalibrationTableEntry;
typedef std::vector<TacCalibrationTableEntry> TacCalibrationTable;

typedef std::map<int,float> RelativeEfficienciesTable;

class XmlConfig
{
public:
	XmlConfig();
	~XmlConfig();

public:
	// configuration
	const std::string &getConfigFileContents() const { return m_hConfigFileContents; }

	// global settings
	void setVerbosity(int iVerbosity) { m_iVerbosity = iVerbosity; }
	int getVerbosity() const { return m_iVerbosity; }
	const std::string &getDatasetNameFormat() const { return m_hDatasetNameFormat; }
	const std::string &getRawDataDir() const { return m_hRawDataDir; }
	const std::string &getProcessedDataDir() const { return m_hProcessedDataDir; }
	const std::string &getMergedDataDir() const { return m_hMergedDataDir; }
	int getProcessingLevel() const { return m_iProcessingLevel; }

	// pmts settings
	const std::string &getTopPmts() const { return m_hTopPmts; }
	const std::string &getBottomPmts() const { return m_hBottomPmts; }
	const std::string &getTopVetoPmts() const { return m_hTopVetoPmts; }
	const std::string &getBottomVetoPmts() const { return m_hBottomVetoPmts; }
	const std::string &getUnusedChannels() const { return m_hUnusedChannels; }

	int getFirstTopPmt() const { return m_iFirstTopPmt; }
	int getFirstBottomPmt() const { return m_iFirstBottomPmt; }
	int getFirstTopVetoPmt() const { return m_iFirstTopVetoPmt; }
	int getFirstBottomVetoPmt() const { return m_iFirstBottomVetoPmt; }
	int getFirstUnusedChannel() const { return m_iFirstUnusedChannel; }

	int getLastTopPmt() const { return m_iLastTopPmt; }
	int getLastBottomPmt() const { return m_iLastBottomPmt; }
	int getLastTopVetoPmt() const { return m_iLastTopVetoPmt; }
	int getLastBottomVetoPmt() const { return m_iLastBottomVetoPmt; }
	int getLastUnusedChannel() const { return m_iLastUnusedChannel; }

	int getNbTopPmts() const { return (m_iFirstTopPmt != -1)?(m_iLastTopPmt-m_iFirstTopPmt+1):(0); }
	int getNbBottomPmts() const { return (m_iFirstBottomPmt != -1)?(m_iLastBottomPmt-m_iFirstBottomPmt+1):(0); }
	int getNbTopVetoPmts() const { return (m_iFirstTopVetoPmt != -1)?(m_iLastTopVetoPmt-m_iFirstTopVetoPmt+1):(0); }
	int getNbBottomVetoPmts() const { return (m_iFirstBottomVetoPmt != -1)?(m_iLastBottomVetoPmt-m_iFirstBottomVetoPmt+1):(0); }
	int getNbUnusedChannels() const { return (m_iFirstUnusedChannel != -1)?(m_iLastUnusedChannel-m_iFirstUnusedChannel+1):(0); }

	int getNbPmts() const { return getNbTopPmts()+getNbBottomPmts()+getNbTopVetoPmts()+getNbBottomVetoPmts(); }

	ExternalGainsTable const *getExternalGainsTable() const { return &m_hExternalGainsTable; }

	const std::string &getPmtCalibrationSource() const { return m_hPmtCalibrationSource; }
	const std::string &getPmtCalibrationFiles() const { return m_hPmtCalibrationFiles; }

	PmtCalibrationTable const *getPmtCalibrationTable() const { return &m_hPmtCalibrationTable; }

	RelativeEfficienciesTable const *getRelativeEfficienciesTable() const { return &m_hRelativeEfficienciesTable; }

	// trigger efficiency
	int getNbTriggerSignals() const { return m_iNbTriggerSignals; }

	const std::string &getTriggerEfficiencyChannels() const { return m_hTriggerEfficiencyChannels; }

	int getFirstTriggerEfficiencyChannel() const { return m_iFirstTriggerEfficiencyChannel; }
	int getLastTriggerEfficiencyChannel() const { return m_iLastTriggerEfficiencyChannel; }

	int getNbTriggerEfficiencyChannels() const { return (m_iFirstTriggerEfficiencyChannel != -1)?(m_iLastTriggerEfficiencyChannel-m_iFirstTriggerEfficiencyChannel):(0); }

	int getTriggerWindowStart() const { return m_iTriggerWindowStart; }
	int getTriggerWindowWidth() const { return m_iTriggerWindowWidth; }
	float getTriggerSignalHeightThreshold() const { return m_fTriggerSignalHeightThreshold; }

#ifdef ENABLE_LIQ_SCI
	// liquid scintillators
	int getNbLiqScis() const { return m_iNbLiqScis; }

	const std::string &getLiqSciChannels() const { return m_hLiqSciChannels; }

	int getFirstLiqSciChannel() const { return m_iFirstLiqSciChannel; }
	int getLastLiqSciChannel() const { return m_iLastLiqSciChannel; }

	int getNbLiqSciChannels() const { return (m_iFirstLiqSciChannel != -1)?(m_iLastLiqSciChannel-m_iFirstLiqSciChannel):(0); }

	int getLiqSciTailStart() const { return m_iLiqSciTailStart; }

	bool getLiqSciMultiplexed() const { return m_bLiqSciMultiplexed; }
	int getLiqSciMultiplexingStart() const { return m_iLiqSciMultiplexingStart; }
	const std::string &getLiqSciMultiplexingWindows() const { return m_hLiqSciMultiplexingWindows; }
	float getLiqSciMultiplexingSignalHeightThreshold() const { return m_fLiqSciMultiplexingSignalHeightThreshold; }
	int getLiqSciMultiplexingSignalWidthThreshold() const { return m_iLiqSciMultiplexingSignalWidthThreshold; }

	LiqSciMultiplexingTable const *getLiqSciMultiplexingTable() const { return &m_hLiqSciMultiplexingTable; }
#endif

#ifdef ENABLE_NAI
	// sodium iodide detectors
	int getNbNaiDetectors() const { return m_iNbNaiDetectors; }

	const std::string &getNaiChannels() const { return m_hNaiChannels; }

	int getFirstNaiChannel() const { return m_iFirstNaiChannel; }
	int getLastNaiChannel() const { return m_iLastNaiChannel; }

	int getNbNaiChannels() const { return (m_iFirstNaiChannel != -1)?(m_iLastNaiChannel-m_iFirstNaiChannel):(0); }
#endif

#ifdef ENABLE_HPGE
	// high purity germanium detectors
	int getNbGeDetectors() const { return m_iNbGeDetectors; }

	const std::string &getGeChannels() const { return m_hGeChannels; }

	int getFirstGeChannel() const { return m_iFirstGeChannel; }
	int getLastGeChannel() const { return m_iLastGeChannel; }

	int getNbGeChannels() const { return (m_iFirstGeChannel != -1)?(m_iLastGeChannel-m_iFirstGeChannel):(0); }
#endif

#ifdef ENABLE_TAC
	// time to amplitude converters
	int getNbTacs() const { return m_iNbTacs; }

	const std::string &getTacChannels() const { return m_hTacChannels; }

	int getFirstTacChannel() const { return m_iFirstTacChannel; }
	int getLastTacChannel() const { return m_iLastTacChannel; }

	int getNbTacChannels() const { return (m_iFirstTacChannel != -1)?(m_iLastTacChannel-m_iFirstTacChannel):(0); }

	int getTacWindowStart() const { return m_iTacWindowStart; }
	int getTacWindowWidth() const { return m_iTacWindowWidth; }

	const std::string &getTacCalibration() const { return m_hTacCalibration; }
	TacCalibrationTable const *getTacCalibrationTable() const { return &m_hTacCalibrationTable; }
#endif

	// single photoelectron settings
	int getSingleNbBaselineSamples() const { return m_iSingleNbBaselineSamples; }
	int getSingleWindowStart() const { return m_iSingleWindowStart; }
	int getSingleWindowWidth() const { return m_iSingleWindowWidth; }
	int getSinglePrePeakSamples() const { return m_iSinglePrePeakSamples; }
	int getSinglePostPeakSamples() const { return m_iSinglePostPeakSamples; }

	// raw data settings
	int getNbBaselineSamples() const { return m_iNbBaselineSamples; }

	// peak finding settings
	PmtSet const *getS1PeakFindingExcludedPmtSet() const { return &m_hS1PeakFindingExcludedPmtSet; }	
	bool getS1Filter() const { return m_bS1Filter; }
	const std::string &getS1FilterType() const { return m_hS1FilterType; }
	bool getS1RaisedCosineLowPass() const { return m_bS1RaisedCosineLowPass; }
	float getS1RaisedCosineLimit() const { return m_fS1RaisedCosineLimit; }
	float getS1RaisedCosineRollOff() const { return m_fS1RaisedCosineRollOff; }
	int getS1RaisedCosineLength() const { return m_iS1RaisedCosineLength; }
	const std::vector<float> &getS1CustomFilterCoefficients() const { return m_hS1CustomFilterCoefficients; }

	int getS1MaxNbPeaks() const { return m_iS1MaxNbPeaks; }
	float getS1RightLimitHeightThreshold() const { return m_fS1RightLimitHeightThreshold; }
	float getS1SignalThreshold() const { return m_fS1SignalThreshold; }
	int getS1PeakWindow() const { return m_iS1PeakWindow; }
	int getS1PrePeakSamples() const { return m_iS1PrePeakSamples; }
	int getS1MaxLength() const { return m_iS1MaxLength; }
	int getS1PrePeakAvgWindow() const { return m_iS1PrePeakAvgWindow; }
	int getS1PostPeakAvgWindow() const { return m_iS1PostPeakAvgWindow; }
	float getS1PrePeakAvgThreshold() const { return m_fS1PrePeakAvgThreshold; }
	float getS1PostPeakAvgThreshold() const { return m_fS1PostPeakAvgThreshold; }
	float getS1FilteredWidthThreshold() const { return m_fS1FilteredWidthThreshold; }
	float getS1NegativeExcursionFractionThreshold() const { return m_fS1NegativeExcursionFractionThreshold; }
	float getS1HeightFractionThreshold() const { return m_fS1HeightFractionThreshold; }
	int getS1SamplesBelowThreshold() const { return m_iS1SamplesBelowThreshold; }
	float getS1CoincidenceThreshold() const { return m_fS1CoincidenceThreshold; }

	PmtSet const *getS2PeakFindingExcludedPmtSet() const { return &m_hS2PeakFindingExcludedPmtSet; }	

	int getS2MaxNbPeaks() const { return m_iS2MaxNbPeaks; }

	bool getS2LargePeakFilter() const { return m_bS2LargePeakFilter; }
	const std::string &getS2LargePeakFilterType() const { return m_hS2LargePeakFilterType; }
	bool getS2LargePeakRaisedCosineLowPass() const { return m_bS2LargePeakRaisedCosineLowPass; }
	float getS2LargePeakRaisedCosineLimit() const { return m_fS2LargePeakRaisedCosineLimit; }
	float getS2LargePeakRaisedCosineRollOff() const { return m_fS2LargePeakRaisedCosineRollOff; }
	int getS2LargePeakRaisedCosineLength() const { return m_iS2LargePeakRaisedCosineLength; }

	float getS2LargePeakSignalThreshold() const { return m_fS2LargePeakSignalThreshold; }
	float getS2LargePeakDynamicFractionSignalThreshold() const { return m_fS2LargePeakDynamicFractionSignalThreshold; }
	int getS2LargePeakMinWidth() const { return m_iS2LargePeakMinWidth; }
	int getS2LargePeakMinIntervalWidth() const { return m_iS2LargePeakMinIntervalWidth; }
	int getS2LargePeakPreIntervalAvgWindow() const { return m_iS2LargePeakPreIntervalAvgWindow; }
	int getS2LargePeakPostIntervalAvgWindow() const { return m_iS2LargePeakPostIntervalAvgWindow; }
	float getS2LargePeakPreTopLevelIntervalAvgThreshold() const { return m_fS2LargePeakPreTopLevelIntervalAvgThreshold; }
	float getS2LargePeakPostTopLevelIntervalAvgThreshold() const { return m_fS2LargePeakPostTopLevelIntervalAvgThreshold; }
	float getS2LargePeakPreIntervalAvgThreshold() const { return m_fS2LargePeakPreIntervalAvgThreshold; }
	float getS2LargePeakPostIntervalAvgThreshold() const { return m_fS2LargePeakPostIntervalAvgThreshold; }
	float getS2LargePeakOverlappingPeaksThreshold() const { return m_fS2LargePeakOverlappingPeaksThreshold; }
	float getS2LargePeakLeftHeightFractionThreshold() const { return m_fS2LargePeakLeftHeightFractionThreshold; }
	float getS2LargePeakRightHeightFractionThreshold() const { return m_fS2LargePeakRightHeightFractionThreshold; }
	float getS2LargePeakSlopeThreshold() const { return m_fS2LargePeakSlopeThreshold; }
	float getS2SmallPeakSlopeThreshold() const { return m_fS2SmallPeakSlopeThreshold; }
	float getS2SmallPeakThreshold() const { return m_fS2SmallPeakThreshold; }

	bool getS2TinyPeakFilter() const { return m_bS2TinyPeakFilter; }
	const std::string &getS2TinyPeakFilterType() const { return m_hS2TinyPeakFilterType; }
	bool getS2TinyPeakRaisedCosineLowPass() const { return m_bS2TinyPeakRaisedCosineLowPass; }
	float getS2TinyPeakRaisedCosineLimit() const { return m_fS2TinyPeakRaisedCosineLimit; }
	float getS2TinyPeakRaisedCosineRollOff() const { return m_fS2TinyPeakRaisedCosineRollOff; }
	int getS2TinyPeakRaisedCosineLength() const { return m_iS2TinyPeakRaisedCosineLength; }	
	const std::vector<float> &getS2TinyPeakCustomFilterCoefficients() const { return m_hS2TinyPeakCustomFilterCoefficients; }

	float getS2TinyPeakRightLimitHeightThreshold() const { return m_fS2TinyPeakRightLimitHeightThreshold; }
	float getS2TinyPeakSignalThreshold() const { return m_fS2TinyPeakSignalThreshold; }
	int getS2TinyPeakMinIntervalWidth() const { return m_iS2TinyPeakMinIntervalWidth; }
	int getS2TinyPeakMaxIntervalWidth() const { return m_iS2TinyPeakMaxIntervalWidth; }
	int getS2TinyPeakPrePeakAvgWindow() const { return m_iS2TinyPeakPrePeakAvgWindow; }
	int getS2TinyPeakPostPeakAvgWindow() const { return m_iS2TinyPeakPostPeakAvgWindow; }
	float getS2TinyPeakPrePeakAvgThreshold() const { return m_fS2TinyPeakPrePeakAvgThreshold; }
	float getS2TinyPeakPostPeakAvgThreshold() const { return m_fS2TinyPeakPostPeakAvgThreshold; }
	float getS2TinyPeakAspectRatioThreshold() const { return m_fS2TinyPeakAspectRatioThreshold; }
	float getS2TinyPeakHeightFractionThreshold() const { return m_fS2TinyPeakHeightFractionThreshold; }

	// signals settings
	float getSignalThreshold() const { return m_fSignalThreshold; }
	float getSaturationThreshold() const { return m_fSaturationThreshold; }
	PmtSet const *getS1CoincidenceExcludedPmtSet() const { return &m_hS1CoincidenceExcludedPmtSet; }	
	PmtSet const *getS1TotalSignalExcludedPmtSet() const { return &m_hS1TotalSignalExcludedPmtSet; }	
	int getS1CoincidenceWindow() const { return m_iS1CoincidenceWindow; }
	PmtSet const *getS2CoincidenceExcludedPmtSet() const { return &m_hS2CoincidenceExcludedPmtSet; }	
	PmtSet const *getS2TotalSignalExcludedPmtSet() const { return &m_hS2TotalSignalExcludedPmtSet; }	
	int getS2CoincidenceWindow() const { return m_iS2CoincidenceWindow; }

	// position reconstruction setting
	bool getUseMaxPmtAlgorithm() const { return m_bUseMaxPmtAlgorithm; }

	bool getUseSvmAlgorithm() const { return m_bUseSvmAlgorithm; }
	const std::string &getSvmRModelFileName() const { return m_hSvmRModelFileName; }
	const std::string &getSvmXModelFileName() const { return m_hSvmXModelFileName; }
	const std::string &getSvmYModelFileName() const { return m_hSvmYModelFileName; }

	bool getUseNnAlgorithm() const { return m_bUseNnAlgorithm; }
	const std::string &getNnXYModelName() const { return m_hNnXYModelName; }

	bool getUseChi2Algorithm() const { return m_bUseChi2Algorithm; }
	const std::string &getChi2LceMapFileName() const { return m_hChi2LceMapFileName; }

	bool getUseFannAlgorithm() const { return m_bUseFannAlgorithm; }
	const std::string &getFannXYModelFileName() const { return m_hFannXYModelFileName; }

	// signal corrections settings
	bool getCorrectForS1SpatialDependence() const { return m_bCorrectForS1SpatialDependence; }

	bool getCorrectForS2AxialDependence() const { return m_bCorrectForS2AxialDependence; }
	float getElectronLifetime() const { return m_fElectronLifetime; }

	// read the configuration file
	bool readConfigFile(const std::string &hConfigFileName);
	void printSettings();

private:
	bool readGlobalSettings(const xmlpp::Node* pRootNode);
	bool readPmtSettings(const xmlpp::Node* pRootNode);
	bool readTriggerEfficiencySettings(const xmlpp::Node* pRootNode);
#ifdef ENABLE_LIQ_SCI
	bool readLiqSciSettings(const xmlpp::Node* pRootNode);
#endif
#ifdef ENABLE_NAI
	bool readNaiSettings(const xmlpp::Node* pRootNode);
#endif
#ifdef ENABLE_HPGE
	bool readGeSettings(const xmlpp::Node* pRootNode);
#endif
#ifdef ENABLE_TAC
	bool readTacSettings(const xmlpp::Node* pRootNode);
#endif
	bool readSinglePhotoelectronSettings(const xmlpp::Node* pRootNode);
	bool readRawDataSettings(const xmlpp::Node* pRootNode);
	bool readPeakFindingSettings(const xmlpp::Node* pRootNode);
	bool readSignalsSettings(const xmlpp::Node* pRootNode);
	bool readPositionReconstructionSettings(const xmlpp::Node* pRootNode);
	bool readSignalCorrectionsSettings(const xmlpp::Node* pRootNode);

	bool parsePmtString(const std::string &hString, int &iFirstPmt, int &iLastPmt);
	bool parsePmtEnumerationString(const std::string &hString, PmtSet &hPmtSet);
	bool parsePmtCalibrationHeader(const std::string &hHeader);
	bool parsePmtCalibrationFile(const std::string &hFile);
	bool parsePmtCalibrationValues(const std::string &hValues);
	bool fillPmtCalibrationTable();
	bool verifyPmtCalibrationTable();

	bool parsePmtExternalGainsString(const std::string &hString, ExternalGainsTable &hExternalGainsTable);
	bool fillPmtExternalGainsTable();

	bool parseRelativeEfficienciesString(const std::string &hString, RelativeEfficienciesTable &hRelativeEfficienciesTable);

	bool parseValueList(const std::string &hString, std::vector<float> &hValues);

#ifdef ENABLE_LIQ_SCI
	void parseLiqSciMultiplexingWindowsString(const std::string &hString, LiqSciMultiplexingTable &hLiqSciMultiplexingTable);
	bool verifyLiqSciMultiplexingTable();
#endif

#ifdef ENABLE_TAC
	void parseTacCalibrationString(const std::string &hString, TacCalibrationTable &hTacCalibrationTable);
	bool verifyTacCalibrationTable();
#endif

private:
	// configuration
	std::string m_hConfigFileContents;
	
	// global settings
	int m_iVerbosity;
	std::string m_hDatasetNameFormat;
	std::string m_hRawDataDir;
	std::string m_hProcessedDataDir;
	std::string m_hMergedDataDir;
	int m_iProcessingLevel;

	// pmts settings
	std::string m_hTopPmts;
	std::string m_hBottomPmts;
	std::string m_hTopVetoPmts;
	std::string m_hBottomVetoPmts;
	std::string m_hUnusedChannels;

	int m_iFirstTopPmt;
	int m_iFirstBottomPmt;
	int m_iFirstTopVetoPmt;
	int m_iFirstBottomVetoPmt;
	int m_iFirstUnusedChannel;

	int m_iLastTopPmt;
	int m_iLastBottomPmt;
	int m_iLastTopVetoPmt;
	int m_iLastBottomVetoPmt;
	int m_iLastUnusedChannel;

	std::string m_hExternalGains;
	ExternalGainsTable m_hExternalGainsTable;
	float m_fExternalGainTopPmts;
	float m_fExternalGainBottomPmts;
	float m_fExternalGainVetoPmts;

	std::string m_hPmtCalibrationSource;
	std::string m_hPmtCalibrationHeader;
	std::string m_hPmtCalibrationValues;
	std::string m_hPmtCalibrationFiles;
	std::string m_hPmtCalibrationFileName;
	PmtCalibrationTableHeader m_hPmtCalibrationTableHeader;
	PmtCalibrationTable m_hPmtCalibrationTable;

	// trigger efficiency
	int m_iNbTriggerSignals;

	std::string m_hTriggerEfficiencyChannels;

	int m_iFirstTriggerEfficiencyChannel;
	int m_iLastTriggerEfficiencyChannel;

	int m_iTriggerWindowStart;
	int m_iTriggerWindowWidth;
	float m_fTriggerSignalHeightThreshold;

#ifdef ENABLE_LIQ_SCI
	// liquid scintillators
	int m_iNbLiqScis;

	std::string m_hLiqSciChannels;

	int m_iFirstLiqSciChannel;
	int m_iLastLiqSciChannel;

	int m_iLiqSciTailStart;

	bool m_bLiqSciMultiplexed;
	int  m_iLiqSciMultiplexingStart;
	std::string m_hLiqSciMultiplexingWindows;
	float m_fLiqSciMultiplexingSignalHeightThreshold;
	int  m_iLiqSciMultiplexingSignalWidthThreshold;

	LiqSciMultiplexingTableHeader m_hLiqSciMultiplexingTableHeader;
	LiqSciMultiplexingTable m_hLiqSciMultiplexingTable;
#endif

#ifdef ENABLE_NAI
	// sodium iodide detectors
	int m_iNbNaiDetectors;

	std::string m_hNaiChannels;

	int m_iFirstNaiChannel;
	int m_iLastNaiChannel;
#endif

#ifdef ENABLE_HPGE
	// high purity germanium detectors
	int m_iNbGeDetectors;

	std::string m_hGeChannels;

	int m_iFirstGeChannel;
	int m_iLastGeChannel;
#endif

#ifdef ENABLE_TAC
	// time to amplitude converters
	int m_iNbTacs;

	std::string m_hTacChannels;

	int m_iFirstTacChannel;
	int m_iLastTacChannel;

	int m_iTacWindowStart;
	int m_iTacWindowWidth;

	std::string m_hTacCalibration;
	TacCalibrationTableHeader m_hTacCalibrationTableHeader;
	TacCalibrationTable m_hTacCalibrationTable;
#endif

	// single photoelectron settings
	int m_iSingleNbBaselineSamples;
	int m_iSingleWindowStart;
	int m_iSingleWindowWidth;
	int m_iSinglePrePeakSamples;
	int m_iSinglePostPeakSamples;

	// raw data settings
	int m_iNbBaselineSamples;

	// peak finding settings
	std::string m_hS1PeakFindingExcludedPmts;
	PmtSet m_hS1PeakFindingExcludedPmtSet;

	bool m_bS1Filter;
	std::string m_hS1FilterType;
	bool m_bS1RaisedCosineLowPass;
	float m_fS1RaisedCosineLimit;
	float m_fS1RaisedCosineRollOff;
	int m_iS1RaisedCosineLength;
	std::string m_hS1CustomFilterCoefficientList;
	std::vector<float> m_hS1CustomFilterCoefficients;
	
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

	std::string m_hS2PeakFindingExcludedPmts;
	PmtSet m_hS2PeakFindingExcludedPmtSet;

	int m_iS2MaxNbPeaks;

	bool m_bS2LargePeakFilter;
	std::string m_hS2LargePeakFilterType;
	bool m_bS2LargePeakRaisedCosineLowPass;
	float m_fS2LargePeakRaisedCosineLimit;
	float m_fS2LargePeakRaisedCosineRollOff;
	int m_iS2LargePeakRaisedCosineLength;
	
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
	std::string m_hS2TinyPeakFilterType;
	bool m_bS2TinyPeakRaisedCosineLowPass;
	float m_fS2TinyPeakRaisedCosineLimit;
	float m_fS2TinyPeakRaisedCosineRollOff;
	int m_iS2TinyPeakRaisedCosineLength;	
	std::string m_hS2TinyPeakCustomFilterCoefficientList;
	std::vector<float> m_hS2TinyPeakCustomFilterCoefficients;

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

	// signals settings
	float m_fSignalThreshold;
	float m_fSaturationThreshold;

	std::string m_hS1CoincidenceExcludedPmts;
	PmtSet m_hS1CoincidenceExcludedPmtSet;
	std::string m_hS1TotalSignalExcludedPmts;
	PmtSet m_hS1TotalSignalExcludedPmtSet;
	int m_iS1CoincidenceWindow;

	std::string m_hS2CoincidenceExcludedPmts;
	PmtSet m_hS2CoincidenceExcludedPmtSet;
	std::string m_hS2TotalSignalExcludedPmts;
	PmtSet m_hS2TotalSignalExcludedPmtSet;
	int m_iS2CoincidenceWindow;

	// position reconstruction settings
	std::string m_hRelativeEfficiencies;
	RelativeEfficienciesTable m_hRelativeEfficienciesTable;

	bool m_bUseMaxPmtAlgorithm;

	bool m_bUseSvmAlgorithm;
	std::string m_hSvmRModelFileName;
	std::string m_hSvmXModelFileName;
	std::string m_hSvmYModelFileName;

	bool m_bUseNnAlgorithm;
	std::string m_hNnXYModelName;

	bool m_bUseChi2Algorithm;
	std::string m_hChi2LceMapFileName;

	bool m_bUseFannAlgorithm;
	std::string m_hFannXYModelFileName;

	// signal corrections settings
	bool m_bCorrectForS1SpatialDependence;

	bool m_bCorrectForS2AxialDependence;
	float m_fElectronLifetime;
};

#endif // __XMLCONFIG_H__

