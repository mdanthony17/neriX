#ifndef __FILTER_H__
#define __FILTER_H__

#include "xerawdp.h"

#include <vector>

class Filter
{
public:
	Filter();
	~Filter();

public:
	const std::vector<float> &getCoefficients() const { return m_hCoefficients; }
	int getLength() const { return m_hCoefficients.size(); }

	void buildCustom(const std::vector<float> hCoefficients);
	void buildDerivative();
	void buildRaisedCosine(bool bLowPass, float fLimit, float fRollOff, int iLength);
	
private:
	static const int m_iMaximumLength = 2048;
	std::vector<float> m_hCoefficients;
};


#endif // __FILTER_H__

