#include "NerixEventData.hh"

NerixEventData::NerixEventData()
{
	m_iEventId = 0;
	m_pEngineStatus = new vector<unsigned long>;

	m_pVolumes = new vector<string>;

	m_iNbPmtHits = 0;
	m_pPmtHits = new vector<int>;
	m_pPmtHitsAngles = new vector<vector<float> >;

	m_fTotalEnergyDeposited = 0.;
	m_iNbSteps = 0;

	m_pTrackId = new vector<int>;
	m_pParentId = new vector<int>;
	m_pParticleType = new vector<string>;
	m_pParentType = new vector<string>;
	m_pCreatorProcess = new vector<string>;
	m_pDepositingProcess = new vector<string>;
	m_pX = new vector<float>;
	m_pY = new vector<float>;
	m_pZ = new vector<float>;
	m_pEnergyDeposited = new vector<float>;
	m_pKineticEnergy = new vector<float>;
	m_pTime = new vector<float>;

	m_iLiquidScintillatorId = 0;
	m_fTotalEnergyDepositedLiquidScintillator = 0.;
	m_iTrackIdLiquidScintillator = 0;
	m_pParticleTypeLiquidScintillator = new vector<string>;
	m_fTimeLiquidScintillator = 0.;
	m_fEnergyInLiquidScintillator = 0.;
	m_fEnergyOutLiquidScintillator = 0.;
	m_fEnergyOtherLiquidScintillator = 0.;

	m_iSodiumIodideDetectorId = 0;
	m_fTotalEnergyDepositedSodiumIodideDetector = 0.;
	m_iTrackIdSodiumIodideDetector = 0;
	m_pParticleTypeSodiumIodideDetector = new vector<string>;
	m_fTimeSodiumIodideDetector = 0.;
	m_fEnergyInSodiumIodideDetector = 0.;
	m_fEnergyOutSodiumIodideDetector = 0.;
	m_fEnergyOtherSodiumIodideDetector = 0.;

	m_iGeDetectorId = 0;
	m_fTotalEnergyDepositedGeDetector = 0.;
	m_iTrackIdGeDetector = 0;
	m_pParticleTypeGeDetector = new vector<string>;
	m_fTimeGeDetector = 0.;
	m_fEnergyInGeDetector = 0.;
	m_fEnergyOutGeDetector = 0.;
	m_fEnergyOtherGeDetector = 0.;

	m_pPrimaryParticleType = new vector<string>;
	m_fPrimaryEnergy = 0.;
	m_fPrimaryX = 0.;
	m_fPrimaryY = 0.;
	m_fPrimaryZ = 0.;	
	m_fPrimaryDirectionX = 0.;
	m_fPrimaryDirectionY = 0.;
	m_fPrimaryDirectionZ = 0.;	
}

NerixEventData::~NerixEventData()
{
	delete m_pEngineStatus;

	delete m_pPmtHits;
	delete m_pPmtHitsAngles;
	delete m_pTrackId;
	delete m_pParentId;
	delete m_pParticleType;
	delete m_pParentType;
	delete m_pCreatorProcess;
	delete m_pDepositingProcess;
	delete m_pX;
	delete m_pY;
	delete m_pZ;
	delete m_pEnergyDeposited;
	delete m_pKineticEnergy;
	delete m_pTime;

	delete m_pParticleTypeLiquidScintillator;

	delete m_pParticleTypeSodiumIodideDetector;

	delete m_pParticleTypeGeDetector;

	delete m_pPrimaryParticleType;
}

void
NerixEventData::Clear()
{
	m_pEngineStatus->clear();

	m_pVolumes->clear();

	m_iEventId = 0;
	m_iNbPmtHits = 0;

	m_pPmtHits->clear();
	m_pPmtHitsAngles->clear();

	m_fTotalEnergyDeposited = 0.0;
	m_iNbSteps = 0;

	m_pTrackId->clear();
	m_pParentId->clear();
	m_pParticleType->clear();
	m_pParentType->clear();
	m_pCreatorProcess->clear();
	m_pDepositingProcess->clear();
	m_pX->clear();
	m_pY->clear();
	m_pZ->clear();
	m_pEnergyDeposited->clear();
	m_pKineticEnergy->clear();
	m_pTime->clear();

	m_iLiquidScintillatorId = 0;
	m_fTotalEnergyDepositedLiquidScintillator = 0.;
	m_iTrackIdLiquidScintillator = 0;
	m_pParticleTypeLiquidScintillator->clear();
	m_fTimeLiquidScintillator = 0.;
	m_fEnergyInLiquidScintillator = 0.;
	m_fEnergyOutLiquidScintillator = 0.;
	m_fEnergyOtherLiquidScintillator = 0.;

	m_iSodiumIodideDetectorId = 0;
	m_fTotalEnergyDepositedSodiumIodideDetector = 0.;
	m_iTrackIdSodiumIodideDetector = 0;
	m_pParticleTypeSodiumIodideDetector->clear();
	m_fTimeSodiumIodideDetector = 0.;
	m_fEnergyInSodiumIodideDetector = 0.;
	m_fEnergyOutSodiumIodideDetector = 0.;
	m_fEnergyOtherSodiumIodideDetector = 0.;

	m_iGeDetectorId = 0;
	m_fTotalEnergyDepositedGeDetector = 0.;
	m_iTrackIdGeDetector = 0;
	m_pParticleTypeGeDetector->clear();
	m_fTimeGeDetector = 0.;
	m_fEnergyInGeDetector = 0.;
	m_fEnergyOutGeDetector = 0.;
	m_fEnergyOtherGeDetector = 0.;

	m_pPrimaryParticleType->clear();
	m_fPrimaryEnergy = 0.;
	m_fPrimaryX = 0.;
	m_fPrimaryY = 0.;
	m_fPrimaryZ = 0.;	
	m_fPrimaryDirectionX = 0.;
	m_fPrimaryDirectionY = 0.;
	m_fPrimaryDirectionZ = 0.;	
}

