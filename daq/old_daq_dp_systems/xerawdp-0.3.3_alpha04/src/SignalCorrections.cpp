#include "SignalCorrections.h"

#include <sstream>
#include <algorithm>

using std::stringstream;
using std::min;
using std::max;

SignalCorrections::SignalCorrections()
{
	initializeLightYield();
}

SignalCorrections::~SignalCorrections()
{
}

float
SignalCorrections::getS1Correction(float fR, float fZ) const
{
	float fS1Correction = m_fAverageLightYield/getLightYield(fR, fZ);

	return fS1Correction;
}

void
SignalCorrections::initializeLightYield()
{
	const double pdParametersF1[11][5] = {
		{+5.9587102106e+00, -6.0799636979e-03, +3.9512999763e-04, -6.9519059977e-06, +2.5958781603e-08},
		{+5.5611571906e+00, +1.2488171520e-02, -4.4516440358e-04, +2.8653986640e-06, -7.0602436033e-09},
		{+4.8354294953e+00, +7.7621725239e-03, -2.7830982494e-04, +1.7237946797e-06, -4.2120140816e-09},
		{+4.1267691559e+00, +1.1200244199e-03, -3.1236053611e-05, -2.3959063777e-07, +7.5185302441e-10},
		{+3.8170102539e+00, +8.0960105872e-03, -3.2971381724e-04, +3.1305815568e-06, -1.0419839236e-08},
		{+3.4394851206e+00, +4.9800414139e-03, -1.5973010238e-04, +1.3599625733e-06, -5.0238366927e-09},
		{+3.1073497177e+00, +3.6119908263e-03, -1.3631853628e-04, +1.4121994687e-06, -5.6452481033e-09},
		{+2.9931903849e+00, +2.9480990465e-03, -1.3655836142e-04, +1.4040999004e-06, -5.4214512469e-09},
		{+2.7427051116e+00, +1.9531959701e-03, -7.7439655795e-05, +9.1568728557e-07, -4.3653176523e-09},
		{+2.4812365780e+00, -4.0186628970e-03, +1.7086307337e-04, -1.7915777593e-06, +4.9133855204e-09},
		{+2.2664242596e+00, -6.0183181478e-03, +2.6609578191e-04, -2.9082501328e-06, +8.8816924500e-09}};

	const double pdParametersF2[11][5] = {
		{+1.9525864588e+00, +3.9167516797e-02, -7.0646884157e-04, +7.0367528674e-06, -1.9626476915e-08},
		{+2.2043936356e+00, +1.9138376448e-02, -2.3687522481e-04, +2.7200785358e-06, -6.5037849403e-09},
		{+2.0709315488e+00, +2.8380032098e-02, -4.1683541543e-04, +3.7422699703e-06, -8.0577987346e-09},
		{+2.4019680935e+00, +6.3260143391e-03, +6.2425593983e-05, -3.2576542010e-07, +3.1655611911e-09},
		{+2.2627851300e+00, +1.6022658399e-02, -2.1098810353e-04, +2.4426611020e-06, -6.1584543354e-09},
		{+2.2297550283e+00, +1.5990217547e-02, -2.0993883562e-04, +2.4392029657e-06, -6.5037698457e-09},
		{+2.0644826564e+00, +2.0794202944e-02, -2.8022474898e-04, +2.8984952774e-06, -7.9426588012e-09},
		{+2.1860339829e+00, +1.4396792195e-02, -1.8952355348e-04, +2.3943144605e-06, -7.4822321745e-09},
		{+1.9281425942e+00, +2.8155157996e-02, -5.0371703429e-04, +5.2813064205e-06, -1.6921360190e-08},
		{+1.9754695580e+00, +1.9065205052e-02, -2.9755886402e-04, +3.4683001615e-06, -1.1798896297e-08},
		{+1.9578992613e+00, +1.4582566277e-02, -2.8387097953e-04, +3.8159389442e-06, -1.3755796254e-08}};

	stringstream hStream;

	for(int iFunctionId = 0; iFunctionId < 11; iFunctionId++)
	{
		hStream << "F1[" << iFunctionId << "]";
		m_hLightYieldF1.push_back(TF1(hStream.str().c_str(), "pol4", 0, 150));
		m_hLightYieldF1.back().SetParameters(pdParametersF1[iFunctionId]);
		hStream.str(""); hStream.clear();
	}

	for(int iFunctionId = 0; iFunctionId < 11; iFunctionId++)
	{
		hStream << "F2[" << iFunctionId << "]";
		m_hLightYieldF2.push_back(TF1(hStream.str().c_str(), "pol4", 0, 165));
		m_hLightYieldF2.back().SetParameters(pdParametersF2[iFunctionId]);
		hStream.str(""); hStream.clear();
	}
}

float
SignalCorrections::getLightYield(float fR, float fZ) const
{
	float fDriftTime = fZ/m_fDriftVelocity;
	float fValue1, fValue2, fResult1, fResult2;
	int iBin;

	// if the drift time or the radius are out of range
	if((fDriftTime < 0.) || (fDriftTime > 165.) || (fR < 0.) || (fR > 150.))
		return -1.;

	// compute the result along z
	const float pfDriftTimeBinCenters[] = {0., 7.5, 22.5, 37.5, 52.5, 67.5, 82.5, 97.5, 112.5, 127.5, 142.5, 157.5, 170.};

	iBin = 0;
	while(fDriftTime >= pfDriftTimeBinCenters[iBin+1])
		iBin++;

	fR = min(1.*fR, 145.);
	fValue1 = m_hLightYieldF1[min(10, 11-iBin)].Eval(fR, 0, 0);
	fValue2 = m_hLightYieldF1[max(0, 10-iBin)].Eval(fR, 0, 0);
	fResult1 = (fValue2 - fValue1)/15. * (fDriftTime - pfDriftTimeBinCenters[iBin]) + fValue1;

	// compute the result along r
	const float pfRBinCenters[] = {0., 20., 50., 65., 75., 85., 95., 105., 115., 125., 135., 145., 150.};

	iBin = 0;
	while(fR >= pfRBinCenters[iBin+1])
		iBin++;

	fDriftTime = min(max(1.*fDriftTime, 7.5), 157.5);
	fValue1 = m_hLightYieldF2[max(0, iBin-1)].Eval(fDriftTime, 0, 0);
	fValue2 = m_hLightYieldF2[min(10, iBin)].Eval(fDriftTime, 0, 0);
	fResult2 = (fValue2 - fValue1)/(pfRBinCenters[iBin+1] - pfRBinCenters[iBin]) * (fR - pfRBinCenters[iBin]) + fValue1;

	return 0.5*(fResult1 + fResult2);
}

