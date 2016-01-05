#ifndef __SIGNALFITTER_H__
#define __SIGNALFITTER_H__

#include "xerawdp.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Waveform;
class TF1;

class SignalFitter
{
public:
	SignalFitter();
	~SignalFitter();

public:
//    void setS1FitExpression(const std::string &hS1FitExpression) { m_hS1FitExpression = hS1FitExpression; }

	std::map<int, std::vector<float> > const *getS1FitParameters() const { return m_pS1FitParameters; }

public:
	void fitS1Signal(std::map<int, Waveform *> const *pWaveforms, int iLeft, int iRight);

private:
	// s1 signal fitter
//    std::string m_hS1FitExpression;
	TF1 *m_pS1SignalFunction;

	std::map<int, std::vector<float> > *m_pS1FitParameters;
};

#endif // __SIGNALFITTER_H__

