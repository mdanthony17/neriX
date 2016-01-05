#include "SignalFitter.h"

#include "Waveform.h"

#include <TGraph.h>
#include <TF1.h>

using std::map;
using std::vector;
using std::cout;
using std::endl;

SignalFitter::SignalFitter()
{
	std::string hS1FitExpression = "([0]/([2]-[3]))*(exp(-(x-[1])/([2]))-exp(-(x-[1])/[3]))*((x-[1]) > 0)";

	m_pS1SignalFunction = new TF1("S1SignalFunction", hS1FitExpression.c_str());

	m_pS1FitParameters = new map<int, vector<float> >();
}

void
SignalFitter::fitS1Signal(std::map<int, Waveform *> const *pWaveforms, int iLeft, int iRight)
{
	m_pS1FitParameters->clear();

	const int iNbParameters = 4;

	map<int, Waveform *>::const_iterator pIter;
	for(pIter = pWaveforms->begin(); pIter != pWaveforms->end(); pIter++)
	{
		int iPmt = pIter->first+1;
		int iNbSamples = iRight-iLeft;

		Waveform *pWaveform = pIter->second;

		float fMaximum = pWaveform->maximum(iLeft, iRight);
		int iMaxPos = pWaveform->maximumPosition(iLeft, iRight);

		// create a graph with the waveform (time in ns)
		TGraph hWaveformGraph(iNbSamples);

		for(int i = 0; i < iNbSamples; i++)
			hWaveformGraph.SetPoint(i, 10.*(iLeft+i), (*pWaveform)[iLeft+i]);

		// fit the graph
		m_pS1SignalFunction->SetParameter(0, fMaximum*(45.-4.));
		m_pS1SignalFunction->SetParameter(1, 10.*iMaxPos);
		m_pS1SignalFunction->SetParameter(2, 45.); // ns
		m_pS1SignalFunction->FixParameter(3, 4.); // ns

		hWaveformGraph.Fit(m_pS1SignalFunction, "QN", "", 10.*iLeft, 10.*iRight);

		vector<float> hPmtS1FitParameters;
		for(int iParameter = 0; iParameter < iNbParameters; iParameter++)
			hPmtS1FitParameters.push_back(m_pS1SignalFunction->GetParameter(iParameter));
		
		(*m_pS1FitParameters)[iPmt] = hPmtS1FitParameters;

//        cout << "S1s[?][" << iPmt << "]:" << endl;
//        cout << "  A: " << m_pS1SignalFunction->GetParameter(0) << " V*ns";
//        cout << "  Chi2: " << m_pS1SignalFunction->GetChisquare();
//        cout << "  Ndf: " << m_pS1SignalFunction->GetNDF();
//        cout << endl;

//        m_fS1SignalChi2 = m_pS1SignalFunction->GetChisquare();
//        m_fS1SignalNdf = m_pS1SignalFunction->GetNDF();
	}
}
