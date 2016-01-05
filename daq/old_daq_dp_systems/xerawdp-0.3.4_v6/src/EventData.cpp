#include "EventData.h"

#include "Waveform.h"
#include "RawData.h"

#include <cstring>

using std::ostream;
using std::endl;
using std::map;
using std::vector;

EventData::EventData()
{
	initializeWaveforms();

	allocate();
	clear();
}

EventData::~EventData()
{
	deallocate();
}

void
EventData::setFilename(const std::string &hFilename)
{
	strcpy(m_szFilename, hFilename.c_str());
}

void
EventData::setPmtCalibrationFiles(const std::string &hPmtCalibrationFiles)
{
	strcpy(m_szPmtCalibrationFiles, hPmtCalibrationFiles.c_str());
}

void
EventData::allocate()
{
	m_pRawData_tdc = new RawData;

	// level1 quantities
	m_pChannels = new vector<int>();
	m_pBaselinesAvg = new vector<vector<float> >();
	m_pBaselinesMax = new vector<vector<float> >();

	m_pAreaRaw = new std::vector<float>();
	m_pAbsAreaRaw = new std::vector<float>();

	m_pSinglePeak = new std::vector<float>();
	m_pSingleHeight = new std::vector<float>();
	m_pSingleSample = new std::vector<int>();
	m_pSingleBefore = new std::vector<std::vector<float> >();
	m_pSingleAfter = new std::vector<std::vector<float> >();
	m_pSingleFixed = new std::vector<std::vector<float> >();

	m_pS1sPeak = new vector<int>();
	m_pS1sLeftEdge = new vector<int>();
	m_pS1sRightEdge = new vector<int>();
	m_pS1sWidth = new vector<float>();
	m_pS1sLowWidth = new vector<float>();
	m_pS1sFilteredLowWidth = new vector<float>();
	m_pS1sHeight = new vector<float>();
	m_pS1sMinimum = new vector<float>();
	m_pS1sMeanTime = new vector<float>();
	m_pS1sTimeSpread = new vector<float>();
	m_pS1sSaturation = new vector<int>();

	m_pS1Order = new vector<int>();
	m_pS1SizeOrder = new vector<int>();

	m_pS2sPeak = new vector<int>();
	m_pS2sLeftEdge = new vector<int>();
	m_pS2sRightEdge = new vector<int>();
	m_pS2sWidth = new vector<float>();
	m_pS2sLowWidth = new vector<float>();
	m_pS2sHeight = new vector<float>();
	m_pS2sHeightChan = new std::vector<std::vector<float> >();
	m_pS2sSaturation = new vector<int>();
	m_pS2sTrigger = new vector<int>();
	m_pCoinRaw = new vector<float>();

	m_pTimeTDC = new vector<float>();

	m_pS2Order = new vector<int>();

	m_pS1sPeakVeto = new vector<int>();
	m_pS1sLeftEdgeVeto = new vector<int>();
	m_pS1sRightEdgeVeto = new vector<int>();
	m_pS1sWidthVeto = new vector<float>();
	m_pS1sLowWidthVeto = new vector<float>();
	m_pS1sHeightVeto = new vector<float>();

	m_pS1OrderVeto = new vector<int>();

	m_pS1sRaw = new vector<vector<float> >();
	m_pS1sRawNoiseMedian = new vector<vector<float> >();
	m_pS1sRawNoiseTrapezoid = new vector<vector<float> >();
	m_pS2sRaw = new vector<vector<float> >();
	m_pS1sRawVeto = new vector<vector<float> >();

	m_pS1sMaxPosition = new vector<vector<int> >();
	m_pS1sMaxPositionVeto = new vector<vector<int> >();

	m_pS1FitParameters = new vector<vector<float> > ();

#ifdef ENABLE_LIQ_SCI
	m_pLiqSciPeak = new vector<int>();
	m_pLiqSciLeftEdge = new vector<int>();
	m_pLiqSciRightEdge = new vector<int>();
	m_pLiqSciWidth = new vector<float>();
	m_pLiqSciLowWidth = new vector<float>();
	m_pLiqSciHeight = new vector<float>();

	m_pLiqSciRaw = new vector<float>();
	m_pLiqSciTailRaw = new vector<float>();
#endif

#ifdef ENABLE_NAI
	m_pNaiPeak = new vector<int>();
	m_pNaiLeftEdge = new vector<int>();
	m_pNaiRightEdge = new vector<int>();
	m_pNaiWidth = new vector<float>();
	m_pNaiLowWidth = new vector<float>();
	m_pNaiHeight = new vector<float>();

	m_pNaiRaw = new vector<float>();
#endif

#ifdef ENABLE_TRIG
	m_pTrigArea = new vector<float>();
	m_pTrigLeftEdge = new vector<int>();
	m_pTrigLength = new vector<int>();
#endif

#ifdef ENABLE_HPGE
	m_pGeHeight = new vector<float>();
#endif

#ifdef ENABLE_TAC
	m_pTacHeight = new vector<float>();
	m_pTacMaximum = new vector<float>();
#endif

	// level2 quantities
	m_pArea = new std::vector<float>();
	m_pAbsArea = new std::vector<float>();

	m_pS1s = new vector<vector<float> >();
	m_pS1sNoiseMedian = new vector<vector<float> >();
	m_pS1sNoiseTrapezoid = new vector<vector<float> >();
	m_pS1sTot = new vector<float>();
	m_pS1sTotTop = new vector<float>();
	m_pS1sTotBottom = new vector<float>();

	m_pfS1sTot = new vector<float>();

	m_pS2s = new vector<vector<float> >();
	m_pS2sTot = new vector<float>();
	m_pS2sTotTop = new vector<float>();
	m_pS2sTotBottom = new vector<float>();

	m_pS1sVeto = new vector<vector<float> >();
	m_pS1sTotVeto = new vector<float>();

	m_pS1sCoin = new vector<int>();
	m_pS1sCoinTop = new vector<int>();
	m_pS1sCoinBottom = new vector<int>();

	m_pS2sCoin = new vector<int>();
	m_pS2sCoinTop = new vector<int>();
	m_pS2sCoinBottom = new vector<int>();

	m_pS1sCoinVeto = new vector<int>();

	m_pS1sPmtCoin = new std::vector<std::vector<int> >();
	m_pS1sPmtOrder = new std::vector<std::vector<int> >();	
	m_pS2sPmtOrder = new std::vector<std::vector<int> >();	

	m_pS2sPosMaxPmt = new vector<vector<float> >();
	m_pS2sPosSvm = new vector<vector<float> >();
	m_pS2sPosNn = new vector<vector<float> >();
	m_pS2sPosCoG = new vector<vector<float> >();//added by Qing
	m_pS2sPosChi2 = new vector<vector<float> >();
	m_pS2sPosFann = new vector<vector<float> >();

#ifdef ENABLE_LIQ_SCI
	m_pLiqSci = new vector<float>();
	m_pLiqSciTail = new vector<float>();
#endif

#ifdef ENABLE_NAI
	m_pNai = new vector<float>();
#endif

#ifdef ENABLE_TAC
	m_pTimeOfFlight = new vector<float>();	
#endif

	// level3 quantities
	m_pcS1sTot = new vector<float>();
	m_pcS2sTot = new vector<float>();
	m_pcS2sTotTop = new vector<float>();
	m_pcS2sTotBottom = new vector<float>();
	
	m_pS1sPmtMaxHeight = new vector<vector<float> >();
	m_pS1sBeforeMaximum = new vector<vector<float> >();
	m_pS1sAfterMaximum = new vector<vector<float> >();
	m_pS1sEntropy  = new vector<vector<float> >();		       
}

void
EventData::deallocate()
{
	delete m_pRawData_tdc;

	// level1 quantities
	delete m_pChannels;
	delete m_pBaselinesAvg;
	delete m_pBaselinesMax;

	delete m_pAreaRaw;
	delete m_pAbsAreaRaw;

	delete m_pSinglePeak;
	delete m_pSingleHeight;
	delete m_pSingleSample;
	delete m_pSingleBefore;
	delete m_pSingleAfter;
	delete m_pSingleFixed;

	delete m_pS1sPeak;
	delete m_pS1sLeftEdge;
	delete m_pS1sRightEdge;
	delete m_pS1sWidth;
	delete m_pS1sLowWidth;
	delete m_pS1sFilteredLowWidth;
	delete m_pS1sHeight;
	delete m_pS1sMinimum;
	delete m_pS1sMeanTime;
	delete m_pS1sTimeSpread;
	delete m_pS1sSaturation;

	delete m_pS1Order;
	delete m_pS1SizeOrder;

	delete m_pS2sPeak;
	delete m_pS2sLeftEdge;
	delete m_pS2sRightEdge;
	delete m_pS2sWidth;
	delete m_pS2sLowWidth;
	delete m_pS2sHeight;
	delete m_pS2sHeightChan;
	delete m_pS2sSaturation;
	delete m_pS2sTrigger;
	delete m_pCoinRaw;
	
	delete m_pTimeTDC;

	delete m_pS2Order;

	delete m_pS1sPeakVeto;
	delete m_pS1sLeftEdgeVeto;
	delete m_pS1sRightEdgeVeto;
	delete m_pS1sWidthVeto;
	delete m_pS1sLowWidthVeto;
	delete m_pS1sHeightVeto;

	delete m_pS1OrderVeto;

	delete m_pS1sRaw;
	delete m_pS1sRawNoiseMedian;
	delete m_pS1sRawNoiseTrapezoid;
	delete m_pS2sRaw;
	delete m_pS1sRawVeto;

	delete m_pS1sMaxPosition;
	delete m_pS1sMaxPositionVeto;

	delete m_pS1FitParameters;

#ifdef ENABLE_LIQ_SCI
	delete m_pLiqSciPeak;
	delete m_pLiqSciLeftEdge;
	delete m_pLiqSciRightEdge;
	delete m_pLiqSciWidth;
	delete m_pLiqSciLowWidth;
	delete m_pLiqSciHeight;

	delete m_pLiqSciRaw;
	delete m_pLiqSciTailRaw;
#endif

#ifdef ENABLE_NAI
	delete m_pNaiPeak;
	delete m_pNaiLeftEdge;
	delete m_pNaiRightEdge;
	delete m_pNaiWidth;
	delete m_pNaiLowWidth;
	delete m_pNaiHeight;

	delete m_pNaiRaw;
#endif

#ifdef ENABLE_TRIG
	delete m_pTrigArea;
	delete m_pTrigLeftEdge;
	delete m_pTrigLength;
#endif

#ifdef ENABLE_HPGE
	delete m_pGeHeight;
#endif

#ifdef ENABLE_TAC
	delete m_pTacHeight;
	delete m_pTacMaximum;
#endif

	// level2 quantities
	delete m_pArea;

	delete m_pS1s;
	delete m_pS1sNoiseMedian;
	delete m_pS1sNoiseTrapezoid;
	delete m_pS1sTot;
	delete m_pS1sTotTop;
	delete m_pS1sTotBottom;

	delete m_pfS1sTot;

	delete m_pS2s;
	delete m_pS2sTot;
	delete m_pS2sTotTop;
	delete m_pS2sTotBottom;

	delete m_pS1sVeto;
	delete m_pS1sTotVeto;

	delete m_pS1sCoin;
	delete m_pS1sCoinTop;
	delete m_pS1sCoinBottom;

	delete m_pS2sCoin;
	delete m_pS2sCoinTop;
	delete m_pS2sCoinBottom;

	delete m_pS1sCoinVeto;

	delete m_pS1sPmtOrder;
	delete m_pS2sPmtOrder;

	delete m_pS2sPosMaxPmt;
	delete m_pS2sPosSvm;
	delete m_pS2sPosNn;
	delete m_pS2sPosCoG;//added by Qing
	delete m_pS2sPosChi2;
	delete m_pS2sPosFann;

#ifdef ENABLE_LIQ_SCI
	delete m_pLiqSci;
	delete m_pLiqSciTail;
#endif

#ifdef ENABLE_NAI
	delete m_pNai;
#endif

#ifdef ENABLE_TAC
	delete m_pTimeOfFlight;
#endif

	// level3 quantities
	delete m_pcS1sTot;
	delete m_pcS2sTot;
	delete m_pcS2sTotTop;
	delete m_pcS2sTotBottom;

	delete m_pS1sPmtMaxHeight;
	delete m_pS1sBeforeMaximum;
	delete m_pS1sAfterMaximum;
	delete m_pS1sEntropy;
}

void
EventData::clear()
{
	// level1 quantities
	m_szFilename[0] = '\0';
	m_iEventId = 0;
	m_iTimeSec = 0;
	m_iTimeMicroSec = 0;

	m_fAreaTotRaw = 0.;
	m_fAreaTotRawVeto = 0.;
	m_pAreaRaw->clear();

	m_fAbsAreaTotRaw = 0.;
	m_pAbsAreaRaw->clear();

	m_pSinglePeak->clear();
	m_pSingleHeight->clear();
	m_pSingleSample->clear();
	m_pSingleBefore->clear();
	m_pSingleAfter->clear();
	m_pSingleFixed->clear();

	m_pChannels->clear();
	m_pBaselinesAvg->clear();
	m_pBaselinesMax->clear();

	m_iNbNonZeroLast = 0;

	m_iNbS1Peaks = 0;
	m_pS1sPeak->clear();
	m_pS1sLeftEdge->clear();
	m_pS1sRightEdge->clear();
	m_pS1sWidth->clear();
	m_pS1sLowWidth->clear();
	m_pS1sFilteredLowWidth->clear();
	m_pS1sHeight->clear();
	m_pS1sMinimum->clear();
	m_pS1sMeanTime->clear();
	m_pS1sTimeSpread->clear();
	m_pS1sSaturation->clear();

	m_pS1Order->clear();
	m_pS1SizeOrder->clear();

	m_iNbS2Peaks = 0;
	m_pS2sPeak->clear();
	m_pS2sLeftEdge->clear();
	m_pS2sRightEdge->clear();
	m_pS2sWidth->clear();
	m_pS2sLowWidth->clear();
	m_pS2sHeight->clear();
	m_pS2sHeightChan->clear();
	m_pS2sSaturation->clear();
	m_pS2sTrigger->clear();
	m_pCoinRaw->clear();
	
	m_pTimeTDC->clear();

	m_pS2Order->clear();

	m_iNbS1PeaksVeto = 0;
	m_pS1sPeakVeto->clear();
	m_pS1sLeftEdgeVeto->clear();
	m_pS1sRightEdgeVeto->clear();
	m_pS1sWidthVeto->clear();
	m_pS1sLowWidthVeto->clear();
	m_pS1sHeightVeto->clear();

	m_pS1OrderVeto->clear();

	m_pS1sRaw->clear();
	m_pS1sRawNoiseMedian->clear();
	m_pS1sRawNoiseTrapezoid->clear();
	m_pS2sRaw->clear();
	m_pS1sRawVeto->clear();

	m_pS1sMaxPosition->clear();
	m_pS1sMaxPositionVeto->clear();

	m_pS1FitParameters->clear();

#ifdef ENABLE_LIQ_SCI
	m_pLiqSciPeak->clear();
	m_pLiqSciLeftEdge->clear();
	m_pLiqSciRightEdge->clear();
	m_pLiqSciWidth->clear();
	m_pLiqSciLowWidth->clear();
	m_pLiqSciHeight->clear();

	m_pLiqSciRaw->clear();
	m_pLiqSciTailRaw->clear();
#endif

#ifdef ENABLE_NAI
	m_pNaiPeak->clear();
	m_pNaiLeftEdge->clear();
	m_pNaiRightEdge->clear();
	m_pNaiWidth->clear();
	m_pNaiLowWidth->clear();
	m_pNaiHeight->clear();

	m_pNaiRaw->clear();
#endif

#ifdef ENABLE_TRIG
	m_pTrigArea->clear();
	m_pTrigLeftEdge->clear();
	m_pTrigLength->clear();
#endif

#ifdef ENABLE_HPGE
	m_pGeHeight->clear();
#endif

#ifdef ENABLE_TAC
	m_pTacHeight->clear();
	m_pTacMaximum->clear();
#endif

	// level2 quantities
	m_szPmtCalibrationFiles[0] = '\0';

	m_fAreaTot = 0.;
	m_fAreaTotVeto = 0.;
	m_pArea->clear();

	m_fAbsAreaTot = 0.;
	m_pAbsArea->clear();

	m_fS1Tot = 0.;
	m_fS1TotTop = 0.;
	m_fS1TotBottom = 0.;

	m_pS1s->clear();
	m_pS1sNoiseMedian->clear();
	m_pS1sNoiseTrapezoid->clear();
	m_pS1sTot->clear();
	m_pS1sTotTop->clear();
	m_pS1sTotBottom->clear();

	m_pfS1sTot->clear();

	m_fS2Tot = 0.;
	m_fS2TotTop = 0.;
	m_fS2TotBottom = 0.;
	m_fS2TotAfter = 0.;

	m_pS2s->clear();
	m_pS2sTot->clear();
	m_pS2sTotTop->clear();
	m_pS2sTotBottom->clear();

	m_fS1TotVeto = 0.;

	m_pS1sVeto->clear();
	m_pS1sTotVeto->clear();

	m_pS1sCoin->clear();
	m_pS1sCoinTop->clear();
	m_pS1sCoinBottom->clear();

	m_pS2sCoin->clear();
	m_pS2sCoinTop->clear();
	m_pS2sCoinBottom->clear();

	m_pS1sCoinVeto->clear();

	m_pS1sPmtCoin->clear();
	m_pS1sPmtOrder->clear();
	m_pS2sPmtOrder->clear();

	m_pS2sPosMaxPmt->clear();
	m_pS2sPosSvm->clear();
	m_pS2sPosNn->clear();
	m_pS2sPosCoG->clear();//added by Qing
	m_pS2sPosChi2->clear();
	m_pS2sPosFann->clear();

#ifdef ENABLE_LIQ_SCI
	m_pLiqSci->clear();
	m_pLiqSciTail->clear();
#endif

#ifdef ENABLE_NAI
	m_pNai->clear();
#endif

#ifdef ENABLE_TAC
	m_pTimeOfFlight->clear();	
#endif

	// level3 quantities
	m_fcS1Tot = 0.;

	m_pcS1sTot->clear();

	m_fcS2Tot = 0.;
	m_fcS2TotTop = 0.;
	m_fcS2TotBottom = 0.;

	m_pcS2sTot->clear();
	m_pcS2sTotTop->clear();
	m_pcS2sTotBottom->clear();

	m_pS1sPmtMaxHeight->clear();
	m_pS1sBeforeMaximum->clear();
	m_pS1sAfterMaximum->clear();
	m_pS1sEntropy->clear();

	// delete and initialize waveforms
	deleteWaveforms();
	initializeWaveforms();
}

void
EventData::initializeWaveforms()
{
	// initialize all waveform pointers
	m_pWaveforms = 0;
	m_pTotalWaveform = 0;
	m_pTotalWaveformVeto = 0;

	m_pS1PeakFindingTotalWaveform = 0;
	m_pS2PeakFindingTotalWaveform = 0;
	m_pS1PeakFindingTotalWaveformVeto = 0;
}

void
EventData::deleteWaveforms()
{
	// delete waveforms
	if(m_pWaveforms)
	{
		map<int, Waveform *>::iterator pIter;
		for(pIter = m_pWaveforms->begin(); pIter != m_pWaveforms->end(); pIter++)
			delete pIter->second;

		delete m_pWaveforms;
	}

	if(m_pTotalWaveform)
		delete m_pTotalWaveform;

	if(m_pTotalWaveformVeto)
		delete m_pTotalWaveformVeto;

	if(m_pS1PeakFindingTotalWaveform)
		delete m_pS1PeakFindingTotalWaveform;

	if(m_pS2PeakFindingTotalWaveform)
		delete m_pS2PeakFindingTotalWaveform;

	if(m_pS1PeakFindingTotalWaveformVeto)
		delete m_pS1PeakFindingTotalWaveformVeto;
}

ostream& operator<<(ostream& hOut, const EventData &hEventData)
{
	hOut << "EventData:" << endl;
	hOut << "  EventId: " << hEventData.getEventId() << endl;
	hOut << "  NbS1Peaks: " << hEventData.getNbS1Peaks() << endl;
	if(hEventData.getNbS1Peaks())
		hOut << "  S1Peaks:" << endl;
	for(int iS1PeakId = 0; iS1PeakId < hEventData.getNbS1Peaks(); iS1PeakId++)
	{
		hOut << "    [" << iS1PeakId << "]:";
		hOut << " Peak: " << hEventData.getS1sPeak(iS1PeakId)
			<< ", LeftEdge: " << hEventData.getS1sLeftEdge(iS1PeakId)
			<< ", RightEdge: " << hEventData.getS1sRightEdge(iS1PeakId) << endl;
		hOut << "         Tot: " << hEventData.getS1sTot(iS1PeakId)
			<< ", TotTop: " << hEventData.getS1sTotTop(iS1PeakId)
			<< ", TotBottom: " << hEventData.getS1sTotBottom(iS1PeakId) << endl;
		hOut << "         Coin: " << hEventData.getS1sCoin(iS1PeakId)
			<< ", CoinTop: " << hEventData.getS1sCoinTop(iS1PeakId)
			<< ", CoinBottom: " << hEventData.getS1sCoinBottom(iS1PeakId) << endl;
		hOut << "         Width: " << hEventData.getS1sWidth(iS1PeakId)
			<< ", LowWidth: " << hEventData.getS1sLowWidth(iS1PeakId)
			<< ", FilteredLowWidth: " << hEventData.getS1sFilteredLowWidth(iS1PeakId)
			<< ", Height: " << hEventData.getS1sHeight(iS1PeakId)
			<< ", Minimum: " << hEventData.getS1sMinimum(iS1PeakId) << endl;
		hOut << "         MeanTime: " << hEventData.getS1sMeanTime(iS1PeakId)
			<< ", TimeSpread: " << hEventData.getS1sTimeSpread(iS1PeakId) << endl;
	}

	hOut << "  NbS2Peaks: " << hEventData.getNbS2Peaks() << endl;
	if(hEventData.getNbS2Peaks())
		hOut << "  S2Peaks:" << endl;
	for(int iS2PeakId = 0; iS2PeakId < hEventData.getNbS2Peaks(); iS2PeakId++)
	{
		hOut << "    [" << iS2PeakId << "]:";
		hOut << " Peak: " << hEventData.getS2sPeak(iS2PeakId)
			<< ", LeftEdge: " << hEventData.getS2sLeftEdge(iS2PeakId)
			<< ", RightEdge: " << hEventData.getS2sRightEdge(iS2PeakId) << endl;
		hOut << "         Tot: " << hEventData.getS2sTot(iS2PeakId)
			<< ", TotTop: " << hEventData.getS2sTotTop(iS2PeakId)
			<< ", TotBottom: " << hEventData.getS2sTotBottom(iS2PeakId) << endl;
		hOut << "         Coin: " << hEventData.getS2sCoin(iS2PeakId)
			<< ", CoinTop: " << hEventData.getS2sCoinTop(iS2PeakId)
			<< ", CoinBottom: " << hEventData.getS2sCoinBottom(iS2PeakId) << endl;
	}

	hOut << "  NbS1PeaksVeto: " << hEventData.getNbS1PeaksVeto() << endl;

	return hOut;
}


