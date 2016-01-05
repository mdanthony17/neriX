#include "PositionReconstruction.h"

#ifdef HAVE_LIBSVM
#include <svm.h>
#endif // HAVE_LIBSVM
#ifdef HAVE_LIBNN
#include <libnn.h>
#endif // HAVE_LIBNN
#ifdef HAVE_LIBCHI2
#include <libchi2.h>
#endif // HAVE_LIBCHI2
#ifdef HAVE_LIBFANN
#include <fann.h>
#endif // HAVE_LIBFANN
#include <iostream>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <stdio.h>

#include <unistd.h>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::accumulate;

PositionReconstructionAlgorithm::PositionReconstructionAlgorithm(std::string hName, int iNbTopPmts, PmtCalibrationTable const *pPmtCalibrationTable, RelativeEfficienciesTable const *pRelativeEfficienciesTable)
{
	m_hName = hName;
	m_iNbTopPmts = iNbTopPmts;
	m_pPmtCalibrationTable = pPmtCalibrationTable;
	m_pRelativeEfficienciesTable = pRelativeEfficienciesTable;
}

PositionReconstruction::PositionReconstruction(const XmlConfig &hXmlConfig)
{
	int iNbTopPmts = hXmlConfig.getNbTopPmts();
	PmtCalibrationTable const *pPmtCalibrationTable = hXmlConfig.getPmtCalibrationTable();
	RelativeEfficienciesTable const *pRelativeEfficienciesTable = hXmlConfig.getRelativeEfficienciesTable();

	PositionReconstructionAlgorithm *pAlgorithm;

	if(hXmlConfig.getUseMaxPmtAlgorithm())
	{
		pAlgorithm = new MaxPmtAlgorithm(iNbTopPmts, pPmtCalibrationTable);
		m_hAlgorithmsTable[pAlgorithm->getName()] = pAlgorithm;
	}

#ifdef HAVE_LIBSVM
	if(hXmlConfig.getUseSvmAlgorithm())
	{
		pAlgorithm = new SvmAlgorithm(iNbTopPmts, pPmtCalibrationTable, hXmlConfig.getSvmRModelFileName(), hXmlConfig.getSvmXModelFileName(), hXmlConfig.getSvmYModelFileName());
		m_hAlgorithmsTable[pAlgorithm->getName()] = pAlgorithm;
	}
#endif // HAVE_LIBSVM

#ifdef HAVE_LIBNN
	if(hXmlConfig.getUseNnAlgorithm())
	{
		pAlgorithm = new NnAlgorithm(iNbTopPmts, pPmtCalibrationTable, hXmlConfig.getNnXYModelName());
		m_hAlgorithmsTable[pAlgorithm->getName()] = pAlgorithm;
	}
#endif // HAVE_LIBNN

#ifdef HAVE_LIBCHI2
	if(hXmlConfig.getUseChi2Algorithm())
	{
		pAlgorithm = new Chi2Algorithm(iNbTopPmts, pPmtCalibrationTable, hXmlConfig.getChi2LceMapFileName());
		m_hAlgorithmsTable[pAlgorithm->getName()] = pAlgorithm;
	}
#endif // HAVE_LIBCHI2

#ifdef HAVE_LIBFANN
	if(hXmlConfig.getUseFannAlgorithm())
	{
		pAlgorithm = new FannAlgorithm(iNbTopPmts, pPmtCalibrationTable, pRelativeEfficienciesTable, hXmlConfig.getFannXYModelFileName());
		m_hAlgorithmsTable[pAlgorithm->getName()] = pAlgorithm;
	}
#endif // HAVE_LIBFANN
}

PositionReconstruction::~PositionReconstruction()
{
	AlgorithmsTable::iterator pAlgorithm;
	for(pAlgorithm = m_hAlgorithmsTable.begin(); pAlgorithm != m_hAlgorithmsTable.end(); pAlgorithm++)
		delete pAlgorithm->second;
}

const std::vector<float> &
PositionReconstruction::reconstructPosition(const std::string &hAlgorithmName, const std::vector<float> &hS2)
{
	if(m_hAlgorithmsTable.count(hAlgorithmName))
		return m_hAlgorithmsTable[hAlgorithmName]->reconstructPosition(hS2);
}

MaxPmtAlgorithm::MaxPmtAlgorithm(int iNbTopPmts, PmtCalibrationTable const *pPmtCalibrationTable):
	PositionReconstructionAlgorithm("MaxPmt", iNbTopPmts, pPmtCalibrationTable)
{
	const float pfPmtXPositions[] = {-166.8, -163.2, -152.4, -135.0, -111.6, -83.4, -51.6, -17.4, 17.4, 51.6,
		83.4, 111.6, 135.0, 152.4, 163.2, 166.8, 163.2, 152.4, 135.0, 111.6, 83.4 , 51.6 , 17.4 , -17.4,
		-51.6, -83.4, -111.6, -135.0, -152.4, -163.2, -136.5, -131.9, -118.2, -96.5 , -68.3 , -35.3 , -0.0  ,
		35.3  , 68.3  , 96.5  , 118.2 , 131.9 , 136.5 , 131.9 , 118.2 , 96.5  , 68.3  , 35.3 , 0.0, -35.3,
		-68.3, -96.5, -118., -131., -106., -101., -85.9, -62.4, -32.8, -0.0 , 32.8 , 62.4 , 85.9 , 101.0,
		106.2, 101.0, 85.9 , 62.4 , 32.8 , 0.0, -32.8, -62.4, -85.9, -101.0, -75.9 , -68.4 , -47.3 , -16.9 ,
		16.9 , 47.3  , 68.4  , 75.9  , 68.4  , 47.3  , 16.9  , -16.9 , -47.3 , -68.4 , -45.0 , -30.0 , 0.0,
		30.0, 45.0, 30.0, 0.0, -30.0, -15.0, 15.0};

	const float pfPmtYPositions[] = {0.0, 34.7, 67.9, 98.1, 124.0, 144.5, 158.7, 165.9, 165.9, 158.7, 144.5,
		124.0, 98.1, 67.9, 34.7, 0.0, -34.7, -67.9, -98.1, -124.0, -144.5, -158.7, -165.9, -165.9, -158.7,
		-144.5, -124.0, -98.1, -67.9, -34.7, 0.0, 35.3, 68.3, 96.5, 118.2, 131.9, 136.5, 131.9, 118.2, 96.5,
		68.3, 35.3, 0.0, -35.3, -68.3, -96.5, -118.2, -131.9, -136.5, -131.9, -118.2, -96.5, -68.3, -35.3,
		0.0, 32.8, 62.4, 85.9, 101.0, 106.2, 101.0, 85.9, 62.4, 32.8, 0.0, -32.8, -62.4, -85.9, -101.0,
		-106.2, -101.0, -85.9, -62.4, -32.8, 0.0, 32.9, 59.3, 74.0, 74.0, 59.3, 32.9, 0.0, -32.9, -59.3,
		-74.0, -74.0, -59.3, -32.9, 0.0, 30.0, 45.0, 30.0, 0.0, -30.0, -45.0, -30.0, 0.0, 0.0};

	m_pPmtXPositions = new vector<float>(pfPmtXPositions, pfPmtXPositions+m_iNbTopPmts);
	m_pPmtYPositions = new vector<float>(pfPmtYPositions, pfPmtYPositions+m_iNbTopPmts);
}

MaxPmtAlgorithm::~MaxPmtAlgorithm()
{
	delete m_pPmtXPositions;
	delete m_pPmtYPositions;
}

const std::vector<float> &
MaxPmtAlgorithm::reconstructPosition(const std::vector<float> &hS2)
{
	int iMaxS2PmtIndex = max_element(hS2.begin(), hS2.begin()+m_iNbTopPmts)-hS2.begin();

	m_hResult.clear();
	m_hResult.push_back((*m_pPmtXPositions)[iMaxS2PmtIndex]);
	m_hResult.push_back((*m_pPmtYPositions)[iMaxS2PmtIndex]);

	return m_hResult;
}

#ifdef HAVE_LIBSVM
SvmAlgorithm::SvmAlgorithm(int iNbTopPmts, PmtCalibrationTable const *pPmtCalibrationTable, const std::string &hRModelFileName, const std::string &hXModelFileName, const std::string &hYModelFileName):
	PositionReconstructionAlgorithm("Svm", iNbTopPmts, pPmtCalibrationTable)
{
	m_hRModelFileName = hRModelFileName;
	m_hXModelFileName = hXModelFileName;
	m_hYModelFileName = hYModelFileName;

	initialize();
}

SvmAlgorithm::~SvmAlgorithm()
{
//    svm_destroy_model(m_pRModel);
//    svm_destroy_model(m_pXModel);
//    svm_destroy_model(m_pYModel);

	delete[] m_pNodes;
}

const std::vector<float> &
SvmAlgorithm::reconstructPosition(const std::vector<float> &hS2)
{
	float fS2TotTop = accumulate(hS2.begin(), hS2.begin()+m_iNbTopPmts, 0.);

	// fill the svm nodes
	for(int iPmtIndex = 0; iPmtIndex < m_iNbTopPmts; iPmtIndex++)
	{
		m_pNodes[iPmtIndex].index = iPmtIndex;
		if(hS2[iPmtIndex] > 0.)
			m_pNodes[iPmtIndex].value = hS2[iPmtIndex]/fS2TotTop*1000.;
		else
			m_pNodes[iPmtIndex].value = 0.;
	}
	m_pNodes[m_iNbTopPmts].index = -1;

	// compute the xy position
	float fSvmR = svm_predict(m_pRModel, m_pNodes);
	float fSvmX = svm_predict(m_pXModel, m_pNodes);
	float fSvmY = svm_predict(m_pYModel, m_pNodes);
	float fSvmPhi = atan2(fSvmY, fSvmX);

	float fR, fRFromXY = sqrt(fSvmX*fSvmX+fSvmY*fSvmY);
	
	if(fSvmR > 70.)
		fR = fSvmR;
	else if(fSvmR < 30.)
		fR = fRFromXY;
	else
		fR = (1-((fSvmR-30.)/40.))*fRFromXY+((fSvmR-30.)/40.)*fSvmR;

	float fX = fR*cos(fSvmPhi);
	float fY = fR*sin(fSvmPhi);

	m_hResult.clear();
	m_hResult.push_back(fX);
	m_hResult.push_back(fY);

	return m_hResult;
}

void
SvmAlgorithm::initialize()
{
	m_pNodes = new struct svm_node[m_iNbTopPmts];

	if((m_pRModel = svm_load_model(m_hRModelFileName.c_str())) == 0)
		cerr << "can't open model file " << m_hRModelFileName << endl;

	if((m_pXModel = svm_load_model(m_hXModelFileName.c_str())) == 0)
		cerr << "can't open model file " << m_hXModelFileName << endl;

	if((m_pYModel = svm_load_model(m_hYModelFileName.c_str())) == 0)
		cerr << "can't open model file " << m_hYModelFileName << endl;
}
#endif // HAVE_LIBSVM

#ifdef HAVE_LIBNN
NnAlgorithm::NnAlgorithm(int iNbTopPmts, PmtCalibrationTable const *pPmtCalibrationTable, const std::string &hXYModelName):
	PositionReconstructionAlgorithm("Nn", iNbTopPmts, pPmtCalibrationTable)
{
	m_hXYModelName = hXYModelName;

	m_pfS2 = new float[m_iNbTopPmts];
	m_pfXY = new float[2];

	m_hModelDispatchTable["nn_missing_9_39_58"] = nn_missing_9_39_58;
}

NnAlgorithm::~NnAlgorithm()
{
	delete[] m_pfS2;
	delete[] m_pfXY;
}

const std::vector<float> &
NnAlgorithm::reconstructPosition(const std::vector<float> &hS2)
{
	float fS2Tot = accumulate(hS2.begin(), hS2.end(), 0.);

	for(int iPmtIndex = 0; iPmtIndex < m_iNbTopPmts; iPmtIndex++)
	{
		if(hS2[iPmtIndex] > 0.)
			m_pfS2[iPmtIndex] = hS2[iPmtIndex]/fS2Tot;
		else
			m_pfS2[iPmtIndex] = 0.;
	}

	m_hModelDispatchTable["nn_missing_9_39_58"](m_pfS2, m_pfXY, 0);

	m_hResult.clear();
	m_hResult.push_back(m_pfXY[0]);
	m_hResult.push_back(m_pfXY[1]);

	return m_hResult;
}
#endif // HAVE_LIBNN

#ifdef HAVE_LIBCHI2
Chi2Algorithm::Chi2Algorithm(int iNbTopPmts, PmtCalibrationTable const *pPmtCalibrationTable, const std::string &hLceMapFileName):
	PositionReconstructionAlgorithm("Chi2", iNbTopPmts, pPmtCalibrationTable)
{
	m_hLceMapFileName = hLceMapFileName;

	initialize();
}

Chi2Algorithm::~Chi2Algorithm()
{
	chi2_release(m_pChi2Handler);

	delete[] m_pdS2;
}

void
Chi2Algorithm::initialize()
{
	if(access(m_hLceMapFileName.c_str(), R_OK))
		cerr << "can't open map file " << m_hLceMapFileName << endl;

	char szChi2Version[256];
    m_pChi2Handler = chi2rec_init(m_hLceMapFileName.c_str(), szChi2Version);
	m_hChi2Version = szChi2Version;

	// fill the pmt info from the calibration table
	chi2_pmt_info *pPmtInfos = new chi2_pmt_info[m_iNbTopPmts];

	for(int iPmt=1; iPmt <= m_iNbTopPmts; iPmt++)
	{
		PmtCalibrationTableRow const *pRow = &((m_pPmtCalibrationTable->find(iPmt))->second);

		pPmtInfos[iPmt-1].gain = pRow->find("gain")->second;
		pPmtInfos[iPmt-1].gain_dev = pRow->find("gain_err")->second;
		pPmtInfos[iPmt-1].qe = pRow->find("qe")->second;
		pPmtInfos[iPmt-1].qe_dev = 0.;
	}

    chi2_read_pmt_info(m_pChi2Handler, pPmtInfos);

	m_pdS2 = new double[m_iNbTopPmts];	
}

const std::vector<float> &
Chi2Algorithm::reconstructPosition(const std::vector<float> &hS2)
{
	for(int iPmtIndex = 0; iPmtIndex < m_iNbTopPmts; iPmtIndex++)
	{
		if(hS2[iPmtIndex] > 0.)
			m_pdS2[iPmtIndex] = hS2[iPmtIndex];
		else
			m_pdS2[iPmtIndex] = 0.;
	}

    double dX, dY, dChi2, dReducedChi2;
	int iNdf;
	
	dReducedChi2 = chi2_reconstruct(m_pChi2Handler, m_pdS2, &dX, &dY, &dChi2, &iNdf);

	m_hResult.clear();
	m_hResult.push_back((float) dX);
	m_hResult.push_back((float) dY);
	m_hResult.push_back((float) dReducedChi2);

	return m_hResult;
}
#endif // HAVE_LIBCHI2

#ifdef HAVE_LIBFANN
FannAlgorithm::FannAlgorithm(int iNbTopPmts, PmtCalibrationTable const *pPmtCalibrationTable, RelativeEfficienciesTable const *pRelativeEfficienciesTable, const std::string &hXYModelFileName):
	PositionReconstructionAlgorithm("Fann", iNbTopPmts, pPmtCalibrationTable, pRelativeEfficienciesTable)
{
	m_hXYModelFileName = hXYModelFileName;

	m_pfS2 = new float[m_iNbTopPmts];

	initialize();
}

FannAlgorithm::~FannAlgorithm()
{
}

const std::vector<float> &
FannAlgorithm::reconstructPosition(const std::vector<float> &hS2)
{
	float fS2TotTop = accumulate(hS2.begin(), hS2.begin()+m_iNbTopPmts, 0.);

	if(m_pRelativeEfficienciesTable && m_pRelativeEfficienciesTable->size())
	{
		for(int iPmtIndex = 0; iPmtIndex < m_iNbTopPmts; iPmtIndex++)
		{
			float fRelativeEfficiency = m_pRelativeEfficienciesTable->find(iPmtIndex+1)->second;
			m_pfS2[iPmtIndex] = (hS2[iPmtIndex] > 0.)?(hS2[iPmtIndex]/(fS2TotTop*fRelativeEfficiency)):(0.);
		}
	}
	else
	{
		for(int iPmtIndex = 0; iPmtIndex < m_iNbTopPmts; iPmtIndex++)
			m_pfS2[iPmtIndex] = (hS2[iPmtIndex] > 0.)?(hS2[iPmtIndex]/fS2TotTop):(0.);
	}

	// compute the xy position
	//float *pfOutput = fann_run(m_pXYModel, m_pfS2);
	
	int numSubdivisions = 12;
	int numPMTsInSubDivision = 1;
	
	//int subDivisionPMTs[9][4] = { {0,1,2,3}, {1,3,4,6}, {4,5,6,7}, {2,3,8,9}, {3,6,9,12}, {6,7,12,13}, {8,9,10,11}, {9,11,12,14}, {12,13,14,15}};
	int subDivisionPMTs[numSubdivisions][numPMTsInSubDivision];
	subDivisionPMTs[0][0] = 1;
	subDivisionPMTs[1][0] = 2;
	subDivisionPMTs[2][0] = 3;
	subDivisionPMTs[3][0] = 4;
	subDivisionPMTs[4][0] = 6;
	subDivisionPMTs[5][0] = 7;
	subDivisionPMTs[6][0] = 8;
	subDivisionPMTs[7][0] = 9;
	subDivisionPMTs[8][0] = 11;
	subDivisionPMTs[9][0] = 12;
	subDivisionPMTs[10][0] = 13;
	subDivisionPMTs[11][0] = 14;
	float subDivisionValues[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	

	
	for (int i = 0; i < numSubdivisions; i++)
	{
		int currentSubset[numPMTsInSubDivision];
		for (int j = 0; j < numPMTsInSubDivision; j++)
		{
			currentSubset[j] = subDivisionPMTs[i][j];
		}
		for (int j = 0; j < numPMTsInSubDivision; j++)
		{
			subDivisionValues[i] = subDivisionValues[i] + m_pfS2[currentSubset[j]];
		}
	}
	
	int maxIndex = -1;
	float maxValue = -1;
	
	for (int i = 0; i < numSubdivisions; i++)
	{
		float newValue = subDivisionValues[i];
		if (newValue > maxValue)
		{
			maxIndex = i;
			maxValue = newValue;
		}
	}
	
	float *pfOutput = fann_run(m_hXYModelFileNameSubdivisions[maxIndex], m_pfS2);
	float maxRadius = 25.;	
	float fX = pfOutput[0]*maxRadius;
	float fY = pfOutput[1]*maxRadius;

	m_hResult.clear();
	m_hResult.push_back(fX);
	m_hResult.push_back(fY);

	return m_hResult;
}

void
FannAlgorithm::initialize()
{
	if((m_pXYModel = fann_create_from_file(m_hXYModelFileName.c_str())) == 0)
		cerr << "can't open model file " << m_hXYModelFileName << endl;
	
	int numSubdivisions = 12;
	
	for (int i = 0; i < numSubdivisions; i++)
	{
		char charNetName[200];
		std::string netName;
		sprintf(charNetName, "./models/subdivision_nets/pr%d.net", i);
		netName = std::string(charNetName);
		m_hXYModelFileNameSubdivisions.push_back(fann_create_from_file(netName.c_str()));
	}
}
#endif // HAVE_LIBFANN

