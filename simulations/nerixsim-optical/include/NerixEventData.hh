#ifndef __NERIXEVENTDATA_H__
#define __NERIXEVENTDATA_H__

#include <string>
#include <vector>

using std::string;
using std::vector;

class NerixEventData
{
public:
	 NerixEventData();
	~NerixEventData();

public:
	void Clear();

public:
	int m_iEventId;								// the event ID
	vector<unsigned long> *m_pEngineStatus;		// status of the random number engine

	vector<string> *m_pVolumes;					// volumes traversed (if primary is a neutron)

	int m_iNbPmtHits;							// number of top pmt hits
	vector<int> *m_pPmtHits;					// number of photon hits per pmt
	vector<vector<float> > *m_pPmtHitsAngles;	// angles of photon hits on each pmt
	float m_fTotalEnergyDeposited;				// total energy deposited in the ScintSD
	int m_iNbSteps;								// number of energy depositing steps
	vector<int> *m_pTrackId;					// id of the particle
	vector<int> *m_pParentId;					// id of the parent particle
	vector<string> *m_pParticleType;			// type of particle
	vector<string> *m_pParentType;				// type of particle
	vector<string> *m_pCreatorProcess;			// interaction
	vector<string> *m_pDepositingProcess;		// energy depositing process
	vector<float> *m_pX;						// position of the step
	vector<float> *m_pY;
	vector<float> *m_pZ;
	vector<float> *m_pEnergyDeposited; 			// energy deposited in the step
	vector<float> *m_pKineticEnergy;			// particle kinetic energy after the step			
	vector<float> *m_pTime;						// time of the step

	int m_iLiquidScintillatorId;						//
	float m_fTotalEnergyDepositedLiquidScintillator;	//
	int m_iTrackIdLiquidScintillator;					//
	vector<string> *m_pParticleTypeLiquidScintillator;	//
	float m_fTimeLiquidScintillator;					//
	float m_fEnergyInLiquidScintillator;				//
	float m_fEnergyOutLiquidScintillator;				//
	float m_fEnergyOtherLiquidScintillator;				//

	int m_iSodiumIodideDetectorId;						//
	float m_fTotalEnergyDepositedSodiumIodideDetector;	//
	int m_iTrackIdSodiumIodideDetector;					//
	vector<string> *m_pParticleTypeSodiumIodideDetector;//
	float m_fTimeSodiumIodideDetector;					//
	float m_fEnergyInSodiumIodideDetector;				//
	float m_fEnergyOutSodiumIodideDetector;				//
	float m_fEnergyOtherSodiumIodideDetector;			//

	int m_iGeDetectorId;						        //
	float m_fTotalEnergyDepositedGeDetector;	        //
	int m_iTrackIdGeDetector;					        //
	vector<string> *m_pParticleTypeGeDetector;          //
	float m_fTimeGeDetector;				         	//
	float m_fEnergyInGeDetector;				        //
	float m_fEnergyOutGeDetector;				        //
	float m_fEnergyOtherGeDetector;			            //

	vector<string> *m_pPrimaryParticleType;		// type of particle
	float m_fPrimaryEnergy;						// energy of the primary particle
	float m_fPrimaryX;							// position of the primary particle
	float m_fPrimaryY;
	float m_fPrimaryZ;	
	float m_fPrimaryDirectionX;							// position of the primary particle
	float m_fPrimaryDirectionY;
	float m_fPrimaryDirectionZ;	
};

#endif // __NERIXEVENTDATA_H__

