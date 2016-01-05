#ifndef __POSITIONRECONSTRUCTION_H__
#define __POSITIONRECONSTRUCTION_H__

#include "xerawdp.h"

#include "XmlConfig.h"

#include <string>
#include <vector>
#include <map>

#ifdef HAVE_LIBSVM
struct svm_model;
struct svm_node;
#endif // HAVE_LIBSVM

#ifdef HAVE_LIBCHI2
#include <libchi2.h>
#endif // HAVE_LIBCHI2

#ifdef HAVE_LIBFANN
struct fann;
#endif // HAVE_LIBFANN

class PositionReconstructionAlgorithm
{
public:
	PositionReconstructionAlgorithm(std::string hName, int iNbTopPmts, PmtCalibrationTable const *pPmtCalibrationTable, RelativeEfficienciesTable const *pRelativeEfficienciesTable=0);

public:
	const std::string &getName() const { return m_hName; }

public:
	virtual const std::vector<float> &reconstructPosition(const std::vector<float> &hS2) = 0;

public:
	std::string m_hName;
	std::vector<float> m_hResult;

	int m_iNbTopPmts;
	PmtCalibrationTable const *m_pPmtCalibrationTable;
	RelativeEfficienciesTable const *m_pRelativeEfficienciesTable;
};

typedef std::map<std::string, PositionReconstructionAlgorithm *> AlgorithmsTable;

class PositionReconstruction
{
public:
	PositionReconstruction(const XmlConfig &hXmlConfig);
	~PositionReconstruction();

public:
	const std::vector<float> &reconstructPosition(const std::string &hAlgorithmName, const std::vector<float> &hS2);

private:
	AlgorithmsTable m_hAlgorithmsTable;
};

class MaxPmtAlgorithm: public PositionReconstructionAlgorithm
{
public:
	MaxPmtAlgorithm(int iNbTopPmts, PmtCalibrationTable const *pPmtCalibrationTable);
	~MaxPmtAlgorithm();

public:
	const std::vector<float> &reconstructPosition(const std::vector<float> &hS2);

private:
	std::vector<float> *m_pPmtXPositions;
	std::vector<float> *m_pPmtYPositions;
};

#ifdef HAVE_LIBSVM
class SvmAlgorithm: public PositionReconstructionAlgorithm
{
public:
	SvmAlgorithm(int iNbTopPmts, PmtCalibrationTable const *pPmtCalibrationTable, const std::string &hRModelFileName, const std::string &hXModelFileName, const std::string &hYModelFileName);
	~SvmAlgorithm();

public:
	const std::vector<float> &reconstructPosition(const std::vector<float> &hS2);

private:
	void initialize();

private:
	struct svm_model *m_pRModel, *m_pXModel, *m_pYModel;  
	struct svm_node *m_pNodes;

	std::string m_hRModelFileName, m_hXModelFileName, m_hYModelFileName;
};
#endif // HAVE_LIBSVM

#ifdef HAVE_LIBNN
class NnAlgorithm: public PositionReconstructionAlgorithm
{
public:
	NnAlgorithm(int iNbTopPmts, PmtCalibrationTable const *pPmtCalibrationTable, const std::string &hXYModelName);
	~NnAlgorithm();

public:
	const std::vector<float> &reconstructPosition(const std::vector<float> &hS2);

private:
	float *m_pfS2;
	float *m_pfXY;

	std::string m_hXYModelName;
	std::map<std::string,int(*)(float*, float*,int)> m_hModelDispatchTable;
};
#endif // HAVE_LIBNN

#ifdef HAVE_LIBCHI2
class Chi2Algorithm: public PositionReconstructionAlgorithm
{
public:
	Chi2Algorithm(int iNbTopPmts, PmtCalibrationTable const *pPmtCalibrationTable, const std::string &hLceMapFileName);
	~Chi2Algorithm();

public:
	const std::vector<float> &reconstructPosition(const std::vector<float> &hS2);

private:
	void initialize();

private:
	double *m_pdS2;
	std::string m_hChi2Version;
	chi2_handler *m_pChi2Handler;

	std::string m_hLceMapFileName;
};
#endif // HAVE_LIBCHI2

#ifdef HAVE_LIBFANN
class FannAlgorithm: public PositionReconstructionAlgorithm
{
public:
	FannAlgorithm(int iNbTopPmts, PmtCalibrationTable const *pPmtCalibrationTable, RelativeEfficienciesTable const *pRelativeEfficienciesTable, const std::string &hXYModelName);
	~FannAlgorithm();

public:
	const std::vector<float> &reconstructPosition(const std::vector<float> &hS2);

private:
	void initialize();

private:
	float *m_pfS2;

	struct fann *m_pXYModel;

	std::string m_hXYModelFileName;
};
#endif // HAVE_LIBFANN

#endif // __POSITIONRECONSTRUCTION_H__

