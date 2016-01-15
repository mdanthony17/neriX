#ifndef __NRSIGNALSIMULATOR_CPP__
#define __NRSIGNALSIMULATOR_CPP__


#include <iostream>
#include <fstream>
#include <cmath>

#include <TROOT.h>
#include <TGraph.h>
#include <TMath.h>
#include <TStopwatch.h>

#include "GetRandomValue.cpp"

#include <vector>

using namespace std;




//// Simulate the S1 and S2 with one energy deposition
vector<float>
SimulateNR(double Energy, double PY, double CY, double NexNi, double g1, double SPESigma, double GasGain, double GasGainSigma, double ExtraEff, double R_Sys_res, double S1_sys_res, double S2_sys_res)
{
	//// Calculate the number of quanta created in the energy deposition
	double pQuantaMean = Energy*PY + Energy*CY;
	
	//// Fluctuate the Quanta number. 
	//// Note here the fluctuation by Fano process, heat quenching are included
	int pQuanta = GetRandomNumberFromPoisson(pQuantaMean);
	
	//// Get the number of Ions
	int pIons = GetRandomBinomialNumber(pQuanta, 1./(1+NexNi) );
	
	//// recombination fraction
	double pR = ( (NexNi+1) * PY )/(PY+CY) - NexNi;
	
	//// True recombination fraction in this trial.
	//// Take into account the fluctuation of recombination itself
	double pTrueR = Get1DGaussianRandom(pR, pR*R_Sys_res);
	
	//// Calculate the number of electrons
	//// Note in this process the statistical fluctuation of recombination is included
	int pNumElectrons = GetRandomBinomialNumber(pIons, 1-pTrueR);
	
	//// Then the number of photons are just correlated with number of electrons
	int pNumPhotons = pQuanta - pNumElectrons;
	
	//// Calculate the detected PE numbers
	int pNumPE = GetRandomNumberFromPoisson( ((double)pNumPhotons)*g1);
	
	//// Calculate the extracted electron number
	int pNumExtractedElectrons = GetRandomBinomialNumber(pNumElectrons, ExtraEff);
	
	//// Reconstruct S1
	double S1 = Get1DGaussianRandom((double)pNumPE, sqrt( (double)pNumPE) * SPESigma);
	
	//// Reconstruct S2
	double S2 = Get1DGaussianRandom( ((double)pNumExtractedElectrons) * GasGain, sqrt((double)pNumExtractedElectrons) * GasGainSigma);
	
	
	if(S1_sys_res>0) S1 = Get1DGaussianRandom(S1, S1*S1_sys_res);
	if(S2_sys_res>0) S2 = Get1DGaussianRandom(S2, S2*S2_sys_res);
	
	
	vector<float> pOutput;
	pOutput.push_back(S1);
	pOutput.push_back(S2);
	return pOutput;
}




#endif

