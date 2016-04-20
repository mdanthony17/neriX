/*  xerawdp -- program to process raw data from the XENON100 detector
 *
 *  version 0.1, March 23rd 2008
 *
 *  Guillaume Plante <guillaum@phys.columbia.edu>
 *
 *  The program will read raw data and write processed data in the directories
 *  supplied in its xml configuration file.
 */

#include "xerawdp.h"

#include "RawDataProcessor.h"
#include "XmlConfig.h"
#include "EventData.h"
#include "Waveform.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <cmath>
#include <getopt.h>

#include <TROOT.h>
#include <TSystem.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TMarker.h>
#include <TPolyMarker.h>
#include <TBox.h>
#include <TLine.h>
#include <TText.h>
#include <TLatex.h>
#include <TEllipse.h>
#include <TColor.h>

#ifdef HAVE_LIBPMTPATTERN
	#include <pmtpattern.h>
#endif // HAVE_LIBPMTPATTERN

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::list;
using std::map;

#define XERAWDP_OK 0
#define XERAWDP_ERR (-1)

#define XERAWDP_S1_0_ARROW_COLOR 4
#define XERAWDP_S1_N_ARROW_COLOR 9
#define XERAWDP_S1_LINE_COLOR 9
#define XERAWDP_S1_LABEL_COLOR 9

#define XERAWDP_S2_0_ARROW_COLOR 2
#define XERAWDP_S2_N_ARROW_COLOR 46
#define XERAWDP_S2_LINE_COLOR 46
#define XERAWDP_S2_LABEL_COLOR 46

#define XERAWDP_S1VETO_N_ARROW_COLOR 8

#define XERAWDP_LIQ_SCI_WINDOWS_LINE_COLOR 8

#define XERAWDP_TAC_WINDOW_LINE_COLOR 6

typedef enum {S1, S2, S1VETO} Signal;

void
usage()
{
	cout << "xerawdp [switches] <xml_config_file> <dataset> [event_index]" << endl << endl;
	cout << "switches:" << endl;
	cout << "  -p                  pretend, do not save data to file" << endl;
	cout << "  -w                  overwrite, overwrite ouput file even if it exists" << endl;
	cout << "  -g                  graphical, graphical output" << endl;
	cout << "  -v <verbosity>      verbosity, " << endl;
	exit(0);
}

Waveform *getTwoFoldCoincidenceWaveform(Waveform const *pTotalWaveform, map<int, Waveform *> const *pWaveforms)
{
	const int iDesiredCoincidence = 2;

	float fSamplingFrequency = pWaveforms->begin()->second->getSamplingFrequency();
	int iNbSamples = pWaveforms->begin()->second->getNbSamples();

	Waveform *pCoincidentWaveform = new Waveform(fSamplingFrequency, iNbSamples, new list<Pulse *>());

	for(int iSample = 0; iSample < iNbSamples; iSample++)
	{
		int iCoincidence = 0;

		map<int, Waveform *>::const_iterator pIterator = pWaveforms->begin();
		while(pIterator != pWaveforms->end())
		{
			if((*(pIterator->second))[iSample] != 0.)
				iCoincidence++;

			if(iCoincidence == iDesiredCoincidence)
			{
				(*pCoincidentWaveform)[iSample] = (*pTotalWaveform)[iSample];
				break;
			}
			else
				pIterator++;
		}
	}

	return pCoincidentWaveform;
}

TH1F *
plotWaveform(const char *szName, const Waveform * const pWaveform)
{
	int iNbSamples = pWaveform->getNbSamples();

	TH1F *pHistogram = new TH1F(szName, "", iNbSamples+1, -0.5/100., (iNbSamples+0.5)/100.);

	for(int i = 0; i < pWaveform->getNbSamples(); i++)
		pHistogram->SetBinContent(i+1, (*pWaveform)[i]);

	return pHistogram;
}

TMarker *
createS1Marker(EventData const *pEventData)
{
	Waveform const *pTotalWaveform = pEventData->getTotalWaveform();
	TMarker *pS1PeakMarker = 0;

	if(pEventData->getNbS1Peaks())
	{
		float fX = pEventData->getS1sPeak(0)/100.;
		float fY = pTotalWaveform->maximum(pEventData->getS1sLeftEdge(0), pEventData->getS1sRightEdge(0));

		pS1PeakMarker = new TMarker(fX, fY, 23);
		pS1PeakMarker->SetMarkerColor(XERAWDP_S1_0_ARROW_COLOR);
		pS1PeakMarker->SetMarkerSize(1.3);
	}

	return pS1PeakMarker;
}

TPolyMarker *
createS1PolyMarkers(EventData const *pEventData)
{
	Waveform const *pTotalWaveform = pEventData->getTotalWaveform();

	int iNbS1Peaks = pEventData->getNbS1Peaks();

	float *pfX = new float[iNbS1Peaks];
	float *pfY = new float[iNbS1Peaks];

	TPolyMarker *pS1PeaksPM = new TPolyMarker();
	pS1PeaksPM->SetMarkerStyle(23);
	pS1PeaksPM->SetMarkerColor(XERAWDP_S1_N_ARROW_COLOR);
	pS1PeaksPM->SetMarkerSize(1.1);

	for(int iPeak = 0; iPeak < iNbS1Peaks; iPeak++)
	{
		pfX[iPeak] = pEventData->getS1sPeak(iPeak)/100.;
		pfY[iPeak] = pTotalWaveform->maximum(pEventData->getS1sLeftEdge(iPeak), pEventData->getS1sRightEdge(iPeak));
	}

	pS1PeaksPM->SetPolyMarker(iNbS1Peaks, pfX, pfY);

	delete[] pfX;
	delete[] pfY;

	return pS1PeaksPM;
}

vector<TLine *> *
createS1Lines(EventData const *pEventData)
{
	Waveform const *pTotalWaveform = pEventData->getTotalWaveform();

	int iNbS1Peaks = pEventData->getNbS1Peaks();

	vector<TLine *> *pS1Lines = new vector<TLine *>;

	for(int iPeak = 0; iPeak < iNbS1Peaks; iPeak++)
	{
		float fX, fY;

		for(int iEdge = 0; iEdge < 2; iEdge++)
		{
			if(iEdge == 0)
				fX = pEventData->getS1sLeftEdge(iPeak)/100.;
			else
				fX = pEventData->getS1sRightEdge(iPeak)/100.;

			fY = (*pTotalWaveform)[pEventData->getS1sPeak(iPeak)];

			TLine *pLine = new TLine(fX, 0., fX, fY);
			pLine->SetLineWidth(1); 
			pLine->SetLineColor(XERAWDP_S1_LINE_COLOR); 
			pLine->SetLineStyle(2); 

			pS1Lines->push_back(pLine);
		}
	}

	return pS1Lines;
}

TMarker *
createS2Marker(EventData const *pEventData)
{
	Waveform const *pTotalWaveform = pEventData->getTotalWaveform();
	TMarker *pS2PeakMarker = 0;

	if(pEventData->getNbS2Peaks())
	{
		float fX = pEventData->getS2sPeak(0)/100.;
		float fY = pTotalWaveform->maximum(pEventData->getS2sLeftEdge(0), pEventData->getS2sRightEdge(0));

		pS2PeakMarker = new TMarker(fX, fY, 23);
		pS2PeakMarker->SetMarkerColor(XERAWDP_S2_0_ARROW_COLOR);
		pS2PeakMarker->SetMarkerSize(1.3);
	}

	return pS2PeakMarker;
}

TPolyMarker *
createS2PolyMarkers(EventData const *pEventData)
{
	Waveform const *pTotalWaveform = pEventData->getTotalWaveform();

	int iNbS2Peaks = pEventData->getNbS2Peaks();

	float *pfX = new float[iNbS2Peaks];
	float *pfY = new float[iNbS2Peaks];

	TPolyMarker *pS2PeaksPM = new TPolyMarker();
	pS2PeaksPM->SetMarkerStyle(23);
	pS2PeaksPM->SetMarkerColor(XERAWDP_S2_N_ARROW_COLOR);
	pS2PeaksPM->SetMarkerSize(1.1);

	for(int iPeak = 0; iPeak < iNbS2Peaks; iPeak++)
	{
		pfX[iPeak] = pEventData->getS2sPeak(iPeak)/100.;
		pfY[iPeak] = pTotalWaveform->maximum(pEventData->getS2sLeftEdge(iPeak), pEventData->getS2sRightEdge(iPeak));
	}

	pS2PeaksPM->SetPolyMarker(iNbS2Peaks, pfX, pfY);

	delete[] pfX;
	delete[] pfY;

	return pS2PeaksPM;
}

vector<TLine *> *
createS2Lines(EventData const *pEventData)
{
	Waveform const *pTotalWaveform = pEventData->getTotalWaveform();

	int iNbS2Peaks = pEventData->getNbS2Peaks();

	vector<TLine *> *pS2Lines = new vector<TLine *>;

	for(int iPeak = 0; iPeak < iNbS2Peaks; iPeak++)
	{
		float fX, fY;

		for(int iEdge = 0; iEdge < 2; iEdge++)
		{
			if(iEdge == 0)
				fX = pEventData->getS2sLeftEdge(iPeak)/100.;
			else
				fX = pEventData->getS2sRightEdge(iPeak)/100.;

			fY = pTotalWaveform->maximum(pEventData->getS2sLeftEdge(iPeak), pEventData->getS2sRightEdge(iPeak));

			TLine *pLine = new TLine(fX, 0., fX, fY);
			pLine->SetLineWidth(1); 
			pLine->SetLineColor(XERAWDP_S2_LINE_COLOR); 
			pLine->SetLineStyle(2); 

			pS2Lines->push_back(pLine);
		}
	}

	return pS2Lines;
}

TPolyMarker *
createS1VetoPolyMarkers(EventData const *pEventData)
{
	Waveform const *pTotalWaveformVeto = pEventData->getTotalWaveformVeto();

	int iNbS1PeaksVeto = pEventData->getNbS1PeaksVeto();

	float *pfX = new float[iNbS1PeaksVeto];
	float *pfY = new float[iNbS1PeaksVeto];

	TPolyMarker *pS1PeaksVetoPM = new TPolyMarker();
	pS1PeaksVetoPM->SetMarkerStyle(23);
	pS1PeaksVetoPM->SetMarkerColor(XERAWDP_S1VETO_N_ARROW_COLOR);
	pS1PeaksVetoPM->SetMarkerSize(1.1);

	for(int iPeak = 0; iPeak < iNbS1PeaksVeto; iPeak++)
	{
		pfX[iPeak] = pEventData->getS1sPeakVeto(iPeak)/100.;
		pfY[iPeak] = pTotalWaveformVeto->maximum(pEventData->getS1sLeftEdgeVeto(iPeak), pEventData->getS1sRightEdgeVeto(iPeak));
	}

	pS1PeaksVetoPM->SetPolyMarker(iNbS1PeaksVeto, pfX, pfY);

	delete[] pfX;
	delete[] pfY;

	return pS1PeaksVetoPM;
}

vector<TText *> *
createS1Labels(EventData const *pEventData)
{
	Waveform const *pTotalWaveform = pEventData->getTotalWaveform();

	int iNbS1Peaks = pEventData->getNbS1Peaks();

	vector<TText *> *pS1Labels = new vector<TText *>;

	for(int iPeak = 0; iPeak < iNbS1Peaks; iPeak++)
	{
		float fX, fY;

		fX = pEventData->getS1sPeak(iPeak)/100.;
		fY = pTotalWaveform->maximum(pEventData->getS1sLeftEdge(iPeak), pEventData->getS1sRightEdge(iPeak));

		stringstream hTmp;
		hTmp.setf(std::ios::fixed);
		hTmp << std::setprecision(1) << "   [" << iPeak << "]: " << pEventData->getS1sTot(iPeak) << " pe";
		hTmp << ", " << pEventData->getS1sCoin(iPeak);

		if(pEventData->getS1sCoin(iPeak) < 5)
		{
			const vector<int> hS1PmtOrder = pEventData->getS1sPmtOrder(iPeak);

			hTmp << " {";
			vector<int>::const_iterator pPmt;
			for(pPmt = hS1PmtOrder.begin(); pPmt != hS1PmtOrder.begin()+pEventData->getS1sCoin(iPeak); pPmt++)
			{
				hTmp << *pPmt+1;
				if(pPmt != hS1PmtOrder.begin()+pEventData->getS1sCoin(iPeak)-1)
					hTmp << ", ";
			}
			hTmp << "}";
		}

		TText *pCoinLabel = new TText(fX, fY, hTmp.str().c_str());
		pCoinLabel->SetTextColor(XERAWDP_S1_LABEL_COLOR); 
		pCoinLabel->SetTextSize(0.03); 

		pS1Labels->push_back(pCoinLabel);
	}

	return pS1Labels;
}

vector<TText *> *
createS2Labels(EventData const *pEventData)
{
	Waveform const *pTotalWaveform = pEventData->getTotalWaveform();

	int iNbS2Peaks = pEventData->getNbS2Peaks();

	vector<TText *> *pS2Labels = new vector<TText *>;

	for(int iPeak = 0; iPeak < iNbS2Peaks; iPeak++)
	{
		float fX, fY;

		fX = pEventData->getS2sPeak(iPeak)/100.;
		fY = pTotalWaveform->maximum(pEventData->getS2sLeftEdge(iPeak), pEventData->getS2sRightEdge(iPeak));

		stringstream hTmp;
		hTmp.setf(std::ios::fixed);
		hTmp << std::setprecision(0) << "   [" << iPeak << "]: " << pEventData->getS2sTot(iPeak) << " pe";
		hTmp << " (" << roundf(pEventData->getS2sPosFann(iPeak)[0]);
		hTmp << ", " << roundf(pEventData->getS2sPosFann(iPeak)[1]) << ")";

		TText *pPosLabel = new TText(fX, fY, hTmp.str().c_str());
		pPosLabel->SetTextColor(XERAWDP_S2_LABEL_COLOR); 
		pPosLabel->SetTextSize(0.03); 

		pS2Labels->push_back(pPosLabel);
	}

	return pS2Labels;
}

#ifdef ENABLE_LIQ_SCI
TMarker *
createLiqSciMarker(EventData const *pEventData)
{
	TMarker *pLiqSciPeakMarker = 0;

	for(int iLiqSci = 0; iLiqSci < pEventData->getNbLiqScis(); iLiqSci++)
		if(pEventData->getLiqSciPeak(iLiqSci) != -1)
		{
			float fX = pEventData->getLiqSciPeak(iLiqSci)/100.;
			float fY = pEventData->getLiqSciHeight(iLiqSci);

			pLiqSciPeakMarker = new TMarker(fX, fY, 23);
			pLiqSciPeakMarker->SetMarkerColor(kGreen);
			pLiqSciPeakMarker->SetMarkerSize(1.3);
		}

	return pLiqSciPeakMarker;
}

vector<TLine *> *
createLiqSciWindowsLines(EventData const *pEventData, XmlConfig const *pXmlConfig)
{
	vector<TLine *> *pLiqSciWindowsLines = new vector<TLine *>;

	std::map<int, Waveform *> const *pWaveforms = pEventData->getWaveforms();

	std::map<int, Waveform *>::const_iterator pIterator;
	pIterator = pWaveforms->find(pXmlConfig->getFirstLiqSciChannel());

	if(pIterator != pWaveforms->end())
	{
		Waveform const *pEJ1Waveform = pIterator->second;

		LiqSciMultiplexingTable const *pLiqSciMultiplexingTable = pXmlConfig->getLiqSciMultiplexingTable();

		LiqSciMultiplexingTable::const_iterator pEntry;
		for(pEntry = pLiqSciMultiplexingTable->begin(); pEntry != pLiqSciMultiplexingTable->end(); pEntry++)
		{
			float fX, fY;

			for(int iEdge = 0; iEdge < 2; iEdge++)
			{
				if(iEdge == 0)
					fX = (*pEntry)[0]/100.;
				else
					fX = ((*pEntry)[0]+(*pEntry)[1])/100.;

				fY = pEJ1Waveform->maximum(0, pEJ1Waveform->getNbSamples());

				TLine *pLine = new TLine(fX, 0., fX, fY);
				pLine->SetLineWidth(1); 
				pLine->SetLineColor(XERAWDP_LIQ_SCI_WINDOWS_LINE_COLOR); 
				pLine->SetLineStyle(2); 

				pLiqSciWindowsLines->push_back(pLine);
			}
		}
	}

	return pLiqSciWindowsLines;
}
#endif

#ifdef ENABLE_NAI
TMarker *
createNaiMarker(EventData const *pEventData)
{
	TMarker *pNaiPeakMarker = 0;

	for(int iNai = 0; iNai < pEventData->getNbNaiDetectors(); iNai++)
		if(pEventData->getNaiPeak(iNai) != -1)
		{
			float fX = pEventData->getNaiPeak(iNai)/100.;
			float fY = pEventData->getNaiHeight(iNai);

			pNaiPeakMarker = new TMarker(fX, fY, 23);
			pNaiPeakMarker->SetMarkerColor(kViolet);
			pNaiPeakMarker->SetMarkerSize(1.3);
		}

	return pNaiPeakMarker;
}
#endif

#ifdef ENABLE_TAC
vector<TLine *> *
createTacWindowLines(EventData const *pEventData, XmlConfig const *pXmlConfig)
{
	vector<TLine *> *pTacWindowLines = new vector<TLine *>;

	std::map<int, Waveform *> const *pWaveforms = pEventData->getWaveforms();

	std::map<int, Waveform *>::const_iterator pIterator;
	pIterator = pWaveforms->find(pXmlConfig->getFirstTacChannel());

	if(pIterator != pWaveforms->end())
	{
		Waveform const *pTacWaveform = pIterator->second;

		float fX, fY;

		for(int iEdge = 0; iEdge < 2; iEdge++)
		{
			if(iEdge == 0)
				fX = pXmlConfig->getTacWindowStart()/100.;
			else
				fX = (pXmlConfig->getTacWindowStart()+pXmlConfig->getTacWindowWidth())/100.;

			fY = pTacWaveform->maximum(0, pTacWaveform->getNbSamples());

			TLine *pLine = new TLine(fX, 0., fX, fY);
			pLine->SetLineWidth(1); 
			pLine->SetLineColor(XERAWDP_TAC_WINDOW_LINE_COLOR); 
			pLine->SetLineStyle(2); 

			pTacWindowLines->push_back(pLine);
		}
	}

	return pTacWindowLines;
}
#endif

TCanvas *
drawNerixPmtPattern(EventData const *pEventData, Signal iSignal, int iIndex)
{
	const bool bMultiAnode = 1;
	const bool bLogarithmicColorScale = 0;

	const int iNbPmts = (bMultiAnode)?(17):(5);
	const int iNbTopPmts = (bMultiAnode)?(16):(4);
	const float fPmtWidth = (bMultiAnode)?(12.7):(25.4);

	const float fTpcRadius = 21.5;

	TCanvas *pCanvas = 0;

	// draw pattern
	vector<vector<float> > const *pSXs;
	int iNbPeaks;
	stringstream hCanvasName, hCanvasTitle;

	switch(iSignal)
	{
		case S1:
			pSXs = pEventData->getS1s(); iNbPeaks = pEventData->getNbS1Peaks();
			hCanvasName << "s1_" << iIndex;
			hCanvasTitle << "S1s[" << iIndex << "]";
			break;
		case S2:
			pSXs = pEventData->getS2s(); iNbPeaks = pEventData->getNbS2Peaks();
			hCanvasName << "s2_" << iIndex;
			hCanvasTitle << "S2s[" << iIndex << "]";
			break;
	}

	if(iIndex < iNbPeaks)
	{
		vector<float> hSX((*pSXs)[iIndex]);
		float fSXMax = *max_element(hSX.begin(), hSX.begin()+iNbTopPmts);

		float pfXRange[] = {-32., 32.};
		float pfYRange[] = {-32., 32.};

		int iNbContours = 100;
		double pdStops[] = {0.00, 0.34, 0.61, 0.84, 1.00};
		double pdReds[] = {0.00, 0.00, 0.87, 1.00, 0.51};
		double pdGreens[] = {0.00, 0.81, 1.00, 0.20, 0.00};
		double pdBlues[] = {0.51, 1.00, 0.12, 0.00, 0.00};

		int iPalette = TColor::CreateGradientColorTable(5, pdStops, pdReds, pdGreens, pdBlues, iNbContours);

		float pfMultiAnodePmtLayout[][3] = {
			 1, -20.065, +20.065,
			 2,  -7.365, +20.065,
			 3, -20.065,  +7.365,
			 4,  -7.365,  +7.365,
			 5,  +7.365, +20.065,
			 6, +20.065, +20.065,
			 7,  +7.365,  +7.365,
			 8, +20.065,  +7.365,
			 9, -20.065,  -7.365,
			10,  -7.365,  -7.365,
			11, -20.065, -20.065,
			12,  -7.365, -20.065,
			13,  +7.365,  -7.365,
			14, +20.065,  -7.365,
			15,  +7.365, -20.065,
			16, +20.065, -20.065};

		float pfSingleAnodePmtLayout[][3] = {
			1, -13.715, +13.715,
			2, +13.715, +13.715,
			3, -13.715, -13.715,
			4, +13.715, -13.715};

		float (*pfPmtLayout)[3] = (bMultiAnode)?(pfMultiAnodePmtLayout):(pfSingleAnodePmtLayout);

		vector<int> hS2Colors;
		vector<float>::const_iterator pSX;
		for(pSX = hSX.begin(); pSX != hSX.end(); pSX++)
			if(bLogarithmicColorScale)
				hS2Colors.push_back(iPalette+rint(((*pSX>0)?(log10(*pSX)/log10(fSXMax)*(iNbContours-1)):(0))));
			else
				hS2Colors.push_back(iPalette+rint(*pSX/fSXMax*(iNbContours-1)));

		vector<TBox *> hTopPmtBoxes;
		vector<TText *> hTopPmtTexts;
		for(int iPmtId = 0; iPmtId < iNbTopPmts; iPmtId++)
		{
			TBox *pPmtBox = new TBox(pfPmtLayout[iPmtId][1]-0.5*fPmtWidth, pfPmtLayout[iPmtId][2]-0.5*fPmtWidth, pfPmtLayout[iPmtId][1]+0.5*fPmtWidth, pfPmtLayout[iPmtId][2]+0.5*fPmtWidth);
			if(hS2Colors[iPmtId] > iPalette)
				pPmtBox->SetFillColor(hS2Colors[iPmtId]);
				pPmtBox->SetLineWidth(1);
			hTopPmtBoxes.push_back(pPmtBox);

			stringstream hTmp;

			hTmp << iPmtId+1;
			TText *pPmtText = new TText(pfPmtLayout[iPmtId][1], pfPmtLayout[iPmtId][2], hTmp.str().c_str());
			pPmtText->SetTextSize(0.04);
			pPmtText->SetTextAlign(22);
			hTopPmtTexts.push_back(pPmtText);
		}

		TEllipse *pTpcCircle = new TEllipse(0., 0., fTpcRadius);
		pTpcCircle->SetLineWidth(2);
		pTpcCircle->SetLineStyle(7);
		pTpcCircle->SetFillStyle(0);

		// draw
		pCanvas = new TCanvas(hCanvasName.str().c_str(), hCanvasTitle.str().c_str(), 480, 400);
		pCanvas->SetLeftMargin(0.0711);
		pCanvas->SetRightMargin(0.0711);
		pCanvas->SetTopMargin(0.05);
		pCanvas->SetBottomMargin(0.112);

		TPad *pPad1 = new TPad("p1", "", 0.00, 0., 0.80, 1., 0);
		TPad *pPad2 = new TPad("p2", "", 0.80, 0., 1.00, 1., 0);

		pPad1->Range(pfXRange[0], pfYRange[0], pfXRange[1], pfYRange[1]);

		pPad1->Draw();
		pPad2->Draw();

		pPad1->cd();
		vector<TBox *>::const_iterator ppPmtBox;
		for(ppPmtBox = hTopPmtBoxes.begin(); ppPmtBox != hTopPmtBoxes.end(); ppPmtBox++)
			(*ppPmtBox)->Draw("same");
		vector<TText *>::const_iterator ppPmtText;
		for(ppPmtText = hTopPmtTexts.begin(); ppPmtText != hTopPmtTexts.end(); ppPmtText++)
			(*ppPmtText)->Draw("same");
		pTpcCircle->Draw("same");

		pCanvas->Update();
	}

	return pCanvas;
}


void
drawEvent(EventData const *pEventData, TCanvas *pCanvas, XmlConfig const *pXmlConfig, bool bSeparateWaveforms, bool bSingleElectronFilter)
{
	pCanvas->SetFillColor(0);
	pCanvas->Divide(2, 2);
	pCanvas->GetPad(1)->SetTopMargin(0.05);
	pCanvas->GetPad(1)->SetBottomMargin(0.05);
	pCanvas->GetPad(1)->SetLeftMargin(0.06);
	pCanvas->GetPad(1)->SetRightMargin(0.06);
	pCanvas->GetPad(2)->SetTopMargin(0.05);
	pCanvas->GetPad(2)->SetBottomMargin(0.05);
	pCanvas->GetPad(2)->SetLeftMargin(0.06);
	pCanvas->GetPad(2)->SetRightMargin(0.06);
	pCanvas->GetPad(3)->SetTopMargin(0.05);
	pCanvas->GetPad(3)->SetBottomMargin(0.05);
	pCanvas->GetPad(3)->SetLeftMargin(0.06);
	pCanvas->GetPad(3)->SetRightMargin(0.06);
	pCanvas->GetPad(4)->SetTopMargin(0.05);
	pCanvas->GetPad(4)->SetBottomMargin(0.05);
	pCanvas->GetPad(4)->SetLeftMargin(0.06);
	pCanvas->GetPad(4)->SetRightMargin(0.06);

	std::map<int, Waveform *> const *pWaveforms = pEventData->getWaveforms();

	TText *pTitleX = new TLatex(0.885, 0.01, "Time [#mus]");
	pTitleX->SetNDC();
	pTitleX->SetTextSize(0.035);

	TText *pTitleY = new TText(0.02, 0.74, "Amplitude [V]");
	pTitleY->SetNDC();
	pTitleY->SetTextAngle(90);
	pTitleY->SetTextSize(0.035);

	TText *pTitleLXe = new TText(0.06, 0.96, "LXe");
	pTitleLXe->SetNDC();
	pTitleLXe->SetTextSize(0.045);

	TText *pTitleEJ1 = new TText(0.06, 0.96, "EJ1");
	pTitleEJ1->SetNDC();
	pTitleEJ1->SetTextSize(0.045);

	if(!bSeparateWaveforms)
	{
		Waveform const *pTotalWaveform = pEventData->getTotalWaveform();

		pCanvas->cd(1);
		TH1F *pTotalHist = plotWaveform("totalhist", pTotalWaveform);
		pTotalHist->SetDirectory(0);
		pTotalHist->SetStats(0);
		pTotalHist->SetLineColor(17);
		pTotalHist->SetLabelSize(0.035, "x");
		pTotalHist->SetLabelSize(0.035, "y");
		pTotalHist->Draw();

		if(bSingleElectronFilter)
		{
			const float pfSingleElectronFilterCoefficients[] = {0, 0.103, 0.371, 0.691, 0.933, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.933, 0.691, 0.371, 0.103, 0};
//            const float pfSingleElectronFilterCoefficients[] = {0, 0.103, 0.371, 0.691, 0.933, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.933, 0.691, 0.371, 0.103, 0};
			vector<float> hSingleElectronFilterCoefficients(pfSingleElectronFilterCoefficients, pfSingleElectronFilterCoefficients+31);

			Filter hFilter;
			hFilter.buildCustom(hSingleElectronFilterCoefficients);

			Waveform *pFilteredWaveform = new Waveform(*pTotalWaveform);
			pFilteredWaveform->convolve(hFilter);

			TH1F *pSingleElectronHist = plotWaveform("singleelectronhist", pFilteredWaveform);
			pSingleElectronHist->SetDirectory(0);
			pSingleElectronHist->SetLineColor(kMagenta);
			pSingleElectronHist->SetLineWidth(2);
			pSingleElectronHist->Draw("same");
		}

		Waveform *pCoincidentWaveform = getTwoFoldCoincidenceWaveform(pTotalWaveform, pEventData->getWaveforms());

		TH1F *pCoincidentHist = plotWaveform("coincidenthist", pCoincidentWaveform);
		pCoincidentHist->SetDirectory(0);
		pCoincidentHist->Draw("same");

		pTitleX->Draw();
		pTitleY->Draw();
		pTitleLXe->Draw();
	}
	else
	{
		Waveform const *pTotalWaveform = pEventData->getTotalWaveform();

		pCanvas->cd(1);
		TH1F *pTotalHist = plotWaveform("totalhist", pTotalWaveform);
		pTotalHist->SetDirectory(0);
		pTotalHist->SetStats(0);
		pTotalHist->SetLineColor(17);
		pTotalHist->SetLineWidth(2);
		pTotalHist->SetLabelSize(0.035, "x");
		pTotalHist->SetLabelSize(0.035, "y");
		pTotalHist->Draw("l");

		const int piColors[] = {2, 3, 4, 5, 6, 7};

		stringstream hTmp;
		int iIndex = 0;

		std::map<int, Waveform *>::const_iterator pIterator = pWaveforms->begin();
		for(pIterator = pWaveforms->begin(); pIterator != pWaveforms->end(); pIterator++)
		{
			Waveform const *pWaveform = pIterator->second;

			if(pIterator->first < 5)
			{
				hTmp << "hist" << iIndex;

				pCanvas->cd(1);
				TH1F *pHist = plotWaveform(hTmp.str().c_str(), pWaveform);
				pHist->SetDirectory(0);
				pHist->SetStats(0);
				pHist->SetLineColor(piColors[pIterator->first % (sizeof(piColors)/sizeof(int))]);
				pHist->SetLabelSize(0.035, "x");
				pHist->SetLabelSize(0.035, "y");
				pHist->Draw("l same");

				pTitleX->Draw();
				pTitleY->Draw();
				pTitleLXe->Draw();

				hTmp.str(""); hTmp.clear();
				iIndex++;
			}
		}
	}

//    cout << endl << "S2Filter: ";
//    Filter hS2Filter;
//    hS2Filter.buildRaisedCosine(true, 0.03, 0.2, 31);
//    for(int z = 0; z < 31; z++)
//        cout << hS2Filter.getCoefficients()[z] << " ";
//    cout << endl;

	TPolyMarker *pS1PeaksPM = createS1PolyMarkers(pEventData);
	TPolyMarker *pS2PeaksPM = createS2PolyMarkers(pEventData);
	pS1PeaksPM->Draw("same");
	pS2PeaksPM->Draw("same");

	vector<TLine *> *pS1Lines = createS1Lines(pEventData);
	vector<TLine *>::iterator pS1Line;
	for(pS1Line = pS1Lines->begin(); pS1Line != pS1Lines->end(); pS1Line++)
		(*pS1Line)->Draw("same");

	vector<TLine *> *pS2Lines = createS2Lines(pEventData);
	vector<TLine *>::iterator pS2Line;
	for(pS2Line = pS2Lines->begin(); pS2Line != pS2Lines->end(); pS2Line++)
		(*pS2Line)->Draw("same");

	if(pEventData->getNbS1Peaks())
	{
		TMarker *pS1PeakMarker = createS1Marker(pEventData);
		pS1PeakMarker->Draw("same");

		stringstream hTmp;
		hTmp << "S1sTot[0]: " << pEventData->getS1sTot(0) << " pe";
		TText *pS1s0 = new TText(0.15, 0.8, hTmp.str().c_str());
		pS1s0->SetNDC();
		pS1s0->Draw("same");

		vector<TText *> *pS1Labels = createS1Labels(pEventData);
		vector<TText *>::iterator pS1Label;
		for(pS1Label = pS1Labels->begin(); pS1Label != pS1Labels->end(); pS1Label++)
			(*pS1Label)->Draw("same");
	}

//    if(0)
	if(pEventData->getNbS2Peaks())
	{
		TMarker *pS2PeakMarker = createS2Marker(pEventData);
		pS2PeakMarker->Draw("same");

		stringstream hTmp;
		hTmp << "S2sTot[0]: " << pEventData->getS2sTot(0) << " pe";
		TText *pS2s0 = new TText(0.15, 0.73, hTmp.str().c_str());
		pS2s0->SetNDC();
		pS2s0->Draw("same");

		vector<TText *> *pS2Labels = createS2Labels(pEventData);
		vector<TText *>::iterator pS2Label;
		for(pS2Label = pS2Labels->begin(); pS2Label != pS2Labels->end(); pS2Label++)
			(*pS2Label)->Draw("same");
	}

	std::map<int, Waveform *>::const_iterator pIterator;

#ifdef ENABLE_LIQ_SCI
	pIterator = pWaveforms->find(6);
	if(pIterator != pWaveforms->end())
	{
		Waveform const *pEJ1Waveform = pIterator->second;

		pCanvas->cd(3);
		TH1F *pHist3 = plotWaveform("hist3", pEJ1Waveform);
		pHist3->SetDirectory(0);
		pHist3->SetStats(0);
		pHist3->Draw();

		TMarker *pLiqSciPeakMarker = createLiqSciMarker(pEventData);
		if(pLiqSciPeakMarker)
			pLiqSciPeakMarker->Draw("same");

		vector<TLine *> *pLiqSciWindowsLines = createLiqSciWindowsLines(pEventData, pXmlConfig);
		vector<TLine *>::iterator pLiqSciWindowsLine;
		for(pLiqSciWindowsLine = pLiqSciWindowsLines->begin(); pLiqSciWindowsLine != pLiqSciWindowsLines->end(); pLiqSciWindowsLine++)
			(*pLiqSciWindowsLine)->Draw("same");
	}
#endif

#ifdef ENABLE_NAI
	pIterator = pWaveforms->find(6);
	if(pIterator != pWaveforms->end())
	{
		Waveform const *pNaiWaveform = pIterator->second;

		pCanvas->cd(3);
		TH1F *pHist3 = plotWaveform("hist3", pNaiWaveform);
		pHist3->SetDirectory(0);
		pHist3->SetStats(0);
		pHist3->Draw();

		TMarker *pNaiPeakMarker = createNaiMarker(pEventData);
		if(pNaiPeakMarker)
			pNaiPeakMarker->Draw("same");
	}
#endif

#ifdef ENABLE_HPGE
	pIterator = pWaveforms->find(18);
	if(pIterator != pWaveforms->end())
	{
		Waveform const *pGeWaveform = pIterator->second;

		pCanvas->cd(3);
		TH1F *pHist3 = plotWaveform("hist3", pGeWaveform);
		pHist3->SetDirectory(0);
		pHist3->SetStats(0);
		pHist3->Draw();

//        TMarker *pGePeakMarker = createGeMarker(pEventData);
//        if(pGePeakMarker)
//            pGePeakMarker->Draw("same");
	}
#endif

#ifdef ENABLE_TAC
	pIterator = pWaveforms->find(7);
	if(pIterator != pWaveforms->end())
	{
		Waveform const *pTacWaveform = pIterator->second;

		pCanvas->cd(4);
		TH1F *pTacHist = plotWaveform("tachist", pTacWaveform);
		pTacHist->SetDirectory(0);
		pTacHist->SetStats(0);
		pTacHist->Draw();

		vector<TLine *> *pTacWindowLines = createTacWindowLines(pEventData, pXmlConfig);
		vector<TLine *>::iterator pTacWindowLine;
		for(pTacWindowLine = pTacWindowLines->begin(); pTacWindowLine != pTacWindowLines->end(); pTacWindowLine++)
			(*pTacWindowLine)->Draw("same");
	}
#endif

	pIterator = pWaveforms->find(17);
	if(pIterator != pWaveforms->end())
	{
		Waveform const *pTriggerWaveform = pIterator->second;

		pCanvas->cd(3);
		TH1F *pTriggerHist = plotWaveform("triggerhist", pTriggerWaveform);
		pTriggerHist->SetDirectory(0);
		pTriggerHist->SetStats(0);
		pTriggerHist->Draw();

//        vector<TLine *> *pTacWindowLines = createTacWindowLines(pEventData, pXmlConfig);
//        vector<TLine *>::iterator pTacWindowLine;
//        for(pTacWindowLine = pTacWindowLines->begin(); pTacWindowLine != pTacWindowLines->end(); pTacWindowLine++)
//            (*pTacWindowLine)->Draw("same");
	}

	pCanvas->Update();

#ifdef HAVE_LIBPMTPATTERN
//    vector<vector<float> > const *pS2s = pEventData->getS2s();
//    if(pEventData->getNbS2Peaks())
//    {
//        vector<float> hS2_0((*pS2s)[0]);
//        float fS2Max = *max_element(hS2_0.begin(), hS2_0.end());

//        pmtpattern *p = new pmtpattern(1200, 1, 235.);

//        vector<float>::const_iterator pS2;
//        for(pS2 = hS2_0.begin(); pS2 != hS2_0.end(); pS2++)
//            p->SetPmt(pS2-hS2_0.begin()+1, *pS2, 0., fS2Max);

//        p->Legend(0, fS2Max);
//        p->SetMarker(pEventData->getS2sPosChi2(0)[0], pEventData->getS2sPosChi2(0)[1]);
//        p->SetGrid();
//        p->Update();
//    }
#endif // HAVE_LIBPMTPATTERN	
}

void
drawLedEvent(EventData const *pEventData, TCanvas *pCanvas)
{
	pCanvas->SetFillColor(0);
	pCanvas->Divide(3, 2);
	for(int iPadId = 1; iPadId <= 6; iPadId++)
	{
		pCanvas->GetPad(iPadId)->SetTopMargin(0.05);
		pCanvas->GetPad(iPadId)->SetBottomMargin(0.05);
		pCanvas->GetPad(iPadId)->SetLeftMargin(0.12);
		pCanvas->GetPad(iPadId)->SetRightMargin(0.06);
	}

	std::map<int, Waveform *> const *pWaveforms = pEventData->getWaveforms();
	std::map<int, Waveform *>::const_iterator pIterator;

	for(int iPmtId = 0; iPmtId < 7; iPmtId++)
	{
		pIterator = pWaveforms->find(iPmtId);
		if(pIterator != pWaveforms->end())
		{
			stringstream hHistName;
			hHistName << "hist" << iPmtId+1;

			Waveform const *pWaveform = pIterator->second;

			pCanvas->cd(iPmtId+1);
			TH1F *pHist = plotWaveform(hHistName.str().c_str(), pWaveform);
			pHist->SetDirectory(0);
			pHist->SetStats(0);
			pHist->Draw();
		}
	}

	pCanvas->Update();
}

int
main(int iArgc, char **szArgv)
{
	// switches
	int c = 0;

	bool bPretend = false;
	bool bOverwrite = false;
	bool bGraphical = false;
	bool bOutput = false;
	bool bSeparateWaveforms = false;
	bool bSingleElectronFilter = false;
	int iVerbosity = -1;
	
	string hConfigFileName, hDatasetFileName;

	int iFirstEventIndex;

	stringstream hStream;

	// parse switches
	while((c = getopt(iArgc, szArgv, "pwgosev:V")) != -1)
	{
		switch(c)
		{
			case 'p':
				bPretend = true;
				break;

			case 'w':
				bOverwrite = true;
				break;

			case 'g':
				bGraphical = true;
				bPretend = true;
				break;

			case 'o':
				bOutput = true;
				bGraphical = true;
				bPretend = true;
				break;

			case 's':
				bSeparateWaveforms = true;
				break;

			case 'e':
				bSingleElectronFilter = true;
				break;

			case 'v':
				hStream.str(optarg);
				hStream >> iVerbosity;
				hStream.str(""); hStream.clear();
				break;

			case 'V':
				cout << "xerawdp " << VERSION << endl << endl;
				cout << "XENON Raw Data Processor." << endl;
				exit(0);

			default:
				usage();
		}
	}

	if(iArgc < optind+2)
		usage();

	hConfigFileName = szArgv[optind];
	hDatasetFileName = szArgv[optind+1];
	iFirstEventIndex = 0;

	if(iArgc > optind+2)
	{
		stringstream hStream;
		hStream.str(szArgv[optind+2]);
		hStream >> iFirstEventIndex;
	}

	// create a raw data processor object and configure it
	RawDataProcessor *pRawDataProcessor = new RawDataProcessor();

	if(iVerbosity != -1)
		pRawDataProcessor->setVerbosity(iVerbosity);

	if(!pRawDataProcessor->configure(hConfigFileName, bPretend, bOverwrite))
		return XERAWDP_ERR;

	XmlConfig const *pXmlConfig = pRawDataProcessor->getXmlConfig();

	if(!pRawDataProcessor->setDataset(hDatasetFileName))
		return XERAWDP_ERR;

	if(!bGraphical)
	{
		if(!pRawDataProcessor->processEvents())
			return XERAWDP_ERR;
	}
	else
	{
		TApplication *theApp;
		
		if(bOutput)
			gROOT->SetBatch();
		else
		{
			theApp = new TApplication("xerawdp", 0, 0);
//            theApp->Connect("KeyPressed(Int_t)", 0, 0, "");
		}

		TCanvas *pCanvas = new TCanvas("c1", "", 1200, 800);
		pCanvas->SetFillColor(0);

		int iNbEvents = pRawDataProcessor->getNbEvents();

		for(int iEventIndex = iFirstEventIndex; iEventIndex < iNbEvents; iEventIndex++)
		{
			EventData const *pEventData;
			
			if(!(pEventData = pRawDataProcessor->processEvent(iEventIndex)))
				return XERAWDP_ERR;

			drawEvent(pEventData, pCanvas, pXmlConfig, bSeparateWaveforms, bSingleElectronFilter);
//            drawLedEvent(pEventData, pCanvas);

			TCanvas *pS2Canvas = drawNerixPmtPattern(pEventData, S2, 0);

			if(bOutput)
			{
				stringstream hEventFileName;
				hEventFileName << hDatasetFileName << "-" << iEventIndex << ".C";
				pCanvas->Print(hEventFileName.str().c_str());
				
				return XERAWDP_OK;
			}
			else
			{
				// handle the event loop ourselves
				while(1)
				{
					gSystem->ProcessEvents();
				}
			}
		}
	}

	delete pRawDataProcessor;

	return XERAWDP_OK;
}

