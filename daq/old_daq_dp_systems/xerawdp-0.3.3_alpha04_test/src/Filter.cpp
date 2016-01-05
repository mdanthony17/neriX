#include "Filter.h"

#include <cmath>
#include <numeric>
#include <fftw3.h>

using std::vector;

Filter::Filter()
{
}

Filter::~Filter()
{
}

void
Filter::buildCustom(const vector<float> hCoefficients)
{
	m_hCoefficients = hCoefficients;
	float fGain = accumulate(m_hCoefficients.begin(), m_hCoefficients.end(), 0);

	vector<float>::iterator pCoefficient;
	for(pCoefficient = m_hCoefficients.begin(); pCoefficient != m_hCoefficients.end(); pCoefficient++)
		*pCoefficient /= fGain;
}

void
Filter::buildDerivative()
{
	const int iDerivativeKernelLength = 9;
	const float pdDerivativeKernel[] = {0.003059, 0.035187, 0.118739, 0.143928, 0.000000, -0.143928, -0.118739, -0.035187, -0.003059};

	m_hCoefficients = vector<float>(pdDerivativeKernel, &pdDerivativeKernel[iDerivativeKernelLength]);
}

void
Filter::buildRaisedCosine(bool bLowPass, float fLimit, float fRollOff, int iLength)
{
	fftw_complex *pDataIn = new fftw_complex[m_iMaximumLength];
	double *pDataOut = new double[m_iMaximumLength];
	fftw_plan aPlan;
	
	const double dLowerFrequency = (1.0 - fRollOff) * fLimit;
	const double dUpperFrequency = (1.0 + fRollOff) * fLimit;
	const double dConstant = 0.5/fLimit;

	// fill half the vector in with the Fourier space representation of the filter
	for(int i = 0; i <= m_iMaximumLength/2; i++)
	{
		const double dFrequency = ((double) i)/m_iMaximumLength;
		
		if(bLowPass)
		{
			pDataIn[i][0] = (dFrequency <= dLowerFrequency) ?
				1.0 : (dFrequency <= dUpperFrequency) ?
					0.5*(1.0 + cos((M_PI*dConstant/fRollOff)*(dFrequency-dLowerFrequency))) : 0.0;
		}
		else
		{
			pDataIn[i][0] = (dFrequency <= dLowerFrequency) ?
				0.0 : (dFrequency <= dUpperFrequency) ?
				0.5*(1.0 - cos((M_PI*dConstant/fRollOff)*(dFrequency-dLowerFrequency))) : 1.0;
		}
		
		pDataIn[i][0] *= dConstant;
		pDataIn[i][1] = 0;
	}

	// mirror the other half
	for(int i = 1; i < m_iMaximumLength/2; i++)
	{
		pDataIn[m_iMaximumLength - i][0] = pDataIn[i][0];
		pDataIn[m_iMaximumLength - i][1] = pDataIn[i][1];
	}

	// inverse fourier transform the filter to get its time representation
	aPlan = fftw_plan_dft_c2r_1d(m_iMaximumLength, pDataIn, pDataOut, FFTW_ESTIMATE);
	fftw_execute(aPlan);

	fftw_destroy_plan(aPlan);
		
	m_hCoefficients.clear();

	// fill the desired number of coefficients
	const int iHalf = (iLength - 1) / 2;
	for(int i = 0; i < iLength; i++)
		m_hCoefficients.push_back(pDataOut[(m_iMaximumLength - iHalf + i) % m_iMaximumLength] / m_iMaximumLength);

	float fGain = accumulate(m_hCoefficients.begin(), m_hCoefficients.end(), 0.);

	vector<float>::iterator pCoefficient;
	for(pCoefficient = m_hCoefficients.begin(); pCoefficient != m_hCoefficients.end(); pCoefficient++)
		*pCoefficient = *pCoefficient/fGain;

	delete[] pDataIn;
	delete[] pDataOut;
}

