#ifndef GETRANDOM_CPP
#define GETRANDOM_CPP

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

#include <TROOT.h>
#include <TMath.h>


double 
GetRandomValue(double lower, double upper) // precision to 1e-7
{
	if(lower>=upper) return lower;
	double step = upper - lower;
	if(step <= 0) return 0;
	int max = 10000000;
	int randnum = rand()%max;
	double value = ((double)randnum) / ((double)max) ;// 0~1
	value *=  step ;
	value += lower;
	
	return value;
}


int
GetRandomNumberFromPoisson(double mean)
{
	double max = TMath::PoissonI(mean, mean);
	for(int r=0;r<1e6;r++)
	{
		int x = TMath::Floor( GetRandomValue(0, mean+10.*sqrt(mean)) );
		double mag = GetRandomValue(0, max);
		if(mag<TMath::PoissonI(x, mean)) return x;
	}
	return 0;
}

double 
Get1DGaussianRandom(double mean, double sigma)
{
	double cdf = GetRandomValue(0, 1.);
	return sqrt(2.)*sigma*TMath::ErfInverse(2.*cdf - 1.) + mean;
}


int
GetRandomBinomialNumber(int TotalNum, double fraction)
{
	if(fraction==0) return 0;
	if(fraction==1) return TotalNum;
	double dRandom = GetRandomValue(0,1); // CDF
	//cout<<dRandom<<endl;
	double mean = ((double)TotalNum)*fraction;
	for(int i =0;i<TotalNum;i++)
	{
		if(TMath::BinomialI(fraction, TotalNum,i) < dRandom) return i-1;
	}
	
	return TotalNum;
}
	

double 
GetExpRandom(double tau)
{
	double cdf = GetRandomValue(0, 1);
	return -tau*log(cdf);
}

#endif
