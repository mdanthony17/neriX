#ifndef __SIGNALCORRECTIONS_H__
#define __SIGNALCORRECTIONS_H__

#include "xerawdp.h"

#include <TF1.h>

#include <vector>

class SignalCorrections
{
public:
	SignalCorrections();
	~SignalCorrections();

public:
	float getS1Correction(float fR, float fZ) const;

private:
	void initializeLightYield();
	float getLightYield(float fR, float fZ) const;

private:
	// average light yield (pe/keV), should be the TPC volume average of the getLightYield() function
	static const float m_fAverageLightYield = 3.30;

	// electron drift velocity (mm/us)
	static const float m_fDriftVelocity = 1.85;

	std::vector<TF1> m_hLightYieldF1;
	std::vector<TF1> m_hLightYieldF2;
};

#endif // __SIGNALCORRECTIONS_H__

