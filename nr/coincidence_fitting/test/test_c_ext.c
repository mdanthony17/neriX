#include "test_c_ext.h"
#include <TH1.h>
#include <assert.h>

void test_c_ext(int numTrials, double *aEnergy, TH1D *hEnergy)
{
	//assert (numTrials == sizeof(aEnergy)/sizeof(*aEnergy));
	for (int i = 0; i < numTrials; i++)
	{
		aEnergy[i] = hEnergy->GetRandom();
	}
}



