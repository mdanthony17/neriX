#include <G4SDManager.hh>
#include <G4VProcess.hh>
#include <G4Run.hh>
#include <G4Event.hh>
#include <G4HCofThisEvent.hh>

#include <numeric>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TParameter.h>

#include "NerixDetectorConstruction.hh"
#include "NerixLXeHit.hh"
#include "NerixLiquidScintillatorHit.hh"
#include "NerixGeDetectorHit.hh"
#include "NerixPmtHit.hh"
#include "NerixPrimaryGeneratorAction.hh"
#include "NerixEventData.hh"
//#include "NerixVectorDict.hh"

#include "NerixAnalysisManager.hh"

NerixAnalysisManager::NerixAnalysisManager(NerixPrimaryGeneratorAction *pPrimaryGeneratorAction)
{
	m_iLXeHitsCollectionID = -1;
	m_iLiquidScintillatorHitsCollectionID = -1;
	m_iGeDetectorHitsCollectionID = -1;
	m_iPmtHitsCollectionID = -1;

	m_hDataFilename = "events.root";

	m_pPrimaryGeneratorAction = pPrimaryGeneratorAction;

	m_pEventData = new NerixEventData();
}

NerixAnalysisManager::~NerixAnalysisManager()
{
}

void
NerixAnalysisManager::BeginOfRun(const G4Run *pRun)
{
	m_pTreeFile = new TFile(m_hDataFilename.c_str(), "RECREATE", "File containing event data for Nerix");
	m_pTree = new TTree("t1", "Tree containing event data for Nerix");

	gROOT->ProcessLine("#include <vector>");
	gROOT->ProcessLine("vector<vector<float>> x;");

	m_pTree->Branch("eventid", &m_pEventData->m_iEventId, "eventid/I");
	m_pTree->Branch("enginestatus", "vector<unsigned long>", &m_pEventData->m_pEngineStatus);

	m_pTree->Branch("npmthits", &m_pEventData->m_iNbPmtHits, "npmthits/I");
	m_pTree->Branch("pmthits", "vector<int>", &m_pEventData->m_pPmtHits);
	m_pTree->Branch("pmthitsangles", "vector<vector<float>>", &m_pEventData->m_pPmtHitsAngles);
	
	m_pTree->Branch("volumes", "vector<string>", &m_pEventData->m_pVolumes);

	//m_pTree->Branch("etot", &m_pEventData->m_fTotalEnergyDeposited, "etot/F");
	//m_pTree->Branch("nsteps", &m_pEventData->m_iNbSteps, "nsteps/I");
	//m_pTree->Branch("trackid", "vector<int>", &m_pEventData->m_pTrackId);
	//m_pTree->Branch("parentid", "vector<int>", &m_pEventData->m_pParentId);
	//m_pTree->Branch("type", "vector<string>", &m_pEventData->m_pParticleType);
	//m_pTree->Branch("parenttype", "vector<string>", &m_pEventData->m_pParentType);
	//m_pTree->Branch("creaproc", "vector<string>", &m_pEventData->m_pCreatorProcess);
	//m_pTree->Branch("edproc", "vector<string>", &m_pEventData->m_pDepositingProcess);
	//m_pTree->Branch("xp", "vector<float>", &m_pEventData->m_pX);
	//m_pTree->Branch("yp", "vector<float>", &m_pEventData->m_pY);
	//m_pTree->Branch("zp", "vector<float>", &m_pEventData->m_pZ);
	//m_pTree->Branch("ed", "vector<float>", &m_pEventData->m_pEnergyDeposited);
	//m_pTree->Branch("ke", "vector<float>", &m_pEventData->m_pKineticEnergy);
	//m_pTree->Branch("time", "vector<float>", &m_pEventData->m_pTime);

	//m_pTree->Branch("liqsciid", &m_pEventData->m_iLiquidScintillatorId, "liqsciid/I");
	//m_pTree->Branch("etotliqsci", &m_pEventData->m_fTotalEnergyDepositedLiquidScintillator, "etotliqsci/F");
	//m_pTree->Branch("trackidliqsci", &m_pEventData->m_iTrackIdLiquidScintillator, "trackidliqsci/I");
	//m_pTree->Branch("typeliqsci", "vector<string>", &m_pEventData->m_pParticleTypeLiquidScintillator);
	//m_pTree->Branch("timeliqsci", &m_pEventData->m_fTimeLiquidScintillator, "timeliqsci/F");
	//m_pTree->Branch("einliqsci", &m_pEventData->m_fEnergyInLiquidScintillator, "einliqsci/F");
	//m_pTree->Branch("eoutliqsci", &m_pEventData->m_fEnergyOutLiquidScintillator, "eoutliqsci/F");
	//m_pTree->Branch("eotherliqsci", &m_pEventData->m_fEnergyOtherLiquidScintillator, "eotherliqsci/F");

	//m_pTree->Branch("geid", &m_pEventData->m_iGeDetectorId, "geid/I");
	//m_pTree->Branch("etotge", &m_pEventData->m_fTotalEnergyDepositedGeDetector, "etotge/F");
	//m_pTree->Branch("trackidge", &m_pEventData->m_iTrackIdGeDetector, "trackidge/I");
	//m_pTree->Branch("typege", "vector<string>", &m_pEventData->m_pParticleTypeGeDetector);
	//m_pTree->Branch("timege", &m_pEventData->m_fTimeGeDetector, "timege/F");
	//m_pTree->Branch("einge", &m_pEventData->m_fEnergyInGeDetector, "einge/F");
	//m_pTree->Branch("eoutge", &m_pEventData->m_fEnergyOutGeDetector, "eoutge/F");
	//m_pTree->Branch("eotherge", &m_pEventData->m_fEnergyOtherGeDetector, "eotherge/F");

	m_pTree->Branch("type_pri", "vector<string>", &m_pEventData->m_pPrimaryParticleType);
	//m_pTree->Branch("e_pri", &m_pEventData->m_fPrimaryEnergy, "e_pri/F");
	m_pTree->Branch("xp_pri", &m_pEventData->m_fPrimaryX, "xp_pri/F");
	m_pTree->Branch("yp_pri", &m_pEventData->m_fPrimaryY, "yp_pri/F");
	m_pTree->Branch("zp_pri", &m_pEventData->m_fPrimaryZ, "zp_pri/F");
	m_pTree->Branch("ux_pri", &m_pEventData->m_fPrimaryDirectionX, "ux_pri/F");
	m_pTree->Branch("uy_pri", &m_pEventData->m_fPrimaryDirectionY, "uy_pri/F");
	m_pTree->Branch("uz_pri", &m_pEventData->m_fPrimaryDirectionZ, "uz_pri/F");

	m_pTree->SetMaxTreeSize((int64_t) 100e9);
	m_pTree->AutoSave();

	m_pNbEventsToSimulateParameter = new TParameter<int>("nbevents", m_iNbEventsToSimulate);
	m_pNbEventsToSimulateParameter->Write();
}

void
NerixAnalysisManager::EndOfRun(const G4Run *pRun)
{
	m_pTreeFile->Write();
	m_pTreeFile->Close();
}

void
NerixAnalysisManager::BeginOfEvent(const G4Event *pEvent)
{
	G4SDManager *pSDManager = G4SDManager::GetSDMpointer();

	if(m_iLXeHitsCollectionID == -1)
		m_iLXeHitsCollectionID = pSDManager->GetCollectionID("LXeHitsCollection");

	if(m_iLiquidScintillatorHitsCollectionID == -1)
		m_iLiquidScintillatorHitsCollectionID = pSDManager->GetCollectionID("LiquidScintillatorHitsCollection");

    if(m_iGeDetectorHitsCollectionID == -1)
        m_iGeDetectorHitsCollectionID = pSDManager->GetCollectionID("GeDetectorHitsCollection");

	if(m_iPmtHitsCollectionID == -1)
		m_iPmtHitsCollectionID = pSDManager->GetCollectionID("PmtHitsCollection");

	*(m_pEventData->m_pEngineStatus) = m_pPrimaryGeneratorAction->GetEngineStatus();

	m_dEnergyInLiquidScintillator = 0.;
	m_hNeutronVolumes.clear();

	m_hGammaVolumes.clear();
}

void
NerixAnalysisManager::EndOfEvent(const G4Event *pEvent)
{
	G4HCofThisEvent* pHCofThisEvent = pEvent->GetHCofThisEvent();
	NerixLXeHitsCollection* pLXeHitsCollection = 0;
	NerixLiquidScintillatorHitsCollection* pLiquidScintillatorHitsCollection = 0;
	NerixGeDetectorHitsCollection* pGeDetectorHitsCollection = 0;
	NerixPmtHitsCollection* pPmtHitsCollection = 0;

	G4int iNbLXeHits = 0, iNbPmtHits = 0;
	G4int iNbLiquidScintillatorHits = 0, iNbGeDetectorHits = 0;
	
	if(pHCofThisEvent)
	{
		if(m_iLXeHitsCollectionID != -1)
		{
			pLXeHitsCollection = (NerixLXeHitsCollection *)(pHCofThisEvent->GetHC(m_iLXeHitsCollectionID));
			iNbLXeHits = (pLXeHitsCollection)?(pLXeHitsCollection->entries()):(0);
		}

		
		if(m_iLiquidScintillatorHitsCollectionID != -1)
		{
			pLiquidScintillatorHitsCollection = (NerixLiquidScintillatorHitsCollection *)(pHCofThisEvent->GetHC(m_iLiquidScintillatorHitsCollectionID));
			iNbLiquidScintillatorHits = (pLiquidScintillatorHitsCollection)?(pLiquidScintillatorHitsCollection->entries()):(0);
		}
		
		if(m_iGeDetectorHitsCollectionID != -1)
		{
			pGeDetectorHitsCollection = (NerixGeDetectorHitsCollection *)(pHCofThisEvent->GetHC(m_iGeDetectorHitsCollectionID));
			iNbGeDetectorHits = (pGeDetectorHitsCollection)?(pGeDetectorHitsCollection->entries()):(0);
		}

		if(m_iPmtHitsCollectionID != -1)
		{
			pPmtHitsCollection = (NerixPmtHitsCollection *)(pHCofThisEvent->GetHC(m_iPmtHitsCollectionID));
			iNbPmtHits = (pPmtHitsCollection)?(pPmtHitsCollection->entries()):(0);
		}
	}

	if(iNbLXeHits || iNbLiquidScintillatorHits || iNbGeDetectorHits || iNbPmtHits)
	{
		m_pEventData->m_iEventId = pEvent->GetEventID();

		m_pEventData->m_pPrimaryParticleType->push_back(m_pPrimaryGeneratorAction->GetParticleTypeOfPrimary());
		m_pEventData->m_fPrimaryEnergy = m_pPrimaryGeneratorAction->GetEnergyOfPrimary()/keV;
		m_pEventData->m_fPrimaryX = m_pPrimaryGeneratorAction->GetPositionOfPrimary().x()/mm;
		m_pEventData->m_fPrimaryY = m_pPrimaryGeneratorAction->GetPositionOfPrimary().y()/mm;
		m_pEventData->m_fPrimaryZ = m_pPrimaryGeneratorAction->GetPositionOfPrimary().z()/mm;
		m_pEventData->m_fPrimaryDirectionX = m_pPrimaryGeneratorAction->GetDirectionOfPrimary().x();
		m_pEventData->m_fPrimaryDirectionY = m_pPrimaryGeneratorAction->GetDirectionOfPrimary().y();
		m_pEventData->m_fPrimaryDirectionZ = m_pPrimaryGeneratorAction->GetDirectionOfPrimary().z();

		if(iNbLiquidScintillatorHits)
			m_pEventData->m_pVolumes->assign(m_hNeutronVolumes.begin(), m_hNeutronVolumes.end());
		else if(iNbGeDetectorHits)
			m_pEventData->m_pVolumes->assign(m_hGammaVolumes.begin(), m_hGammaVolumes.end());

		// LXe Hits
		G4int iNbSteps = 0;
		G4float fTotalEnergyDeposited = 0.;

		for(G4int i=0; i<iNbLXeHits; i++)
		{
			NerixLXeHit *pHit = (*pLXeHitsCollection)[i];

			if(pHit->GetParticleType() != "opticalphoton")
			{
				m_pEventData->m_pTrackId->push_back(pHit->GetTrackId());
				m_pEventData->m_pParentId->push_back(pHit->GetParentId());

				m_pEventData->m_pParticleType->push_back(pHit->GetParticleType());
				m_pEventData->m_pParentType->push_back(pHit->GetParentType());
				m_pEventData->m_pCreatorProcess->push_back(pHit->GetCreatorProcess());
				m_pEventData->m_pDepositingProcess->push_back(pHit->GetDepositingProcess());

				m_pEventData->m_pX->push_back(pHit->GetPosition().x()/mm);
				m_pEventData->m_pY->push_back(pHit->GetPosition().y()/mm);
				m_pEventData->m_pZ->push_back(pHit->GetPosition().z()/mm);

				fTotalEnergyDeposited += pHit->GetEnergyDeposited()/keV;
				m_pEventData->m_pEnergyDeposited->push_back(pHit->GetEnergyDeposited()/keV);

				m_pEventData->m_pKineticEnergy->push_back(pHit->GetKineticEnergy()/keV);
				m_pEventData->m_pTime->push_back(pHit->GetTime()/ns);

				iNbSteps++;
			}
		};

		m_pEventData->m_iNbSteps = iNbSteps;
		m_pEventData->m_fTotalEnergyDeposited = fTotalEnergyDeposited;

		// Liquid Scintillator Hits
		G4int iLiquidScintillatorId = -1;
		G4int iTrackIdLiquidScintillator = -1;
		G4String hParticleTypeLiquidScintillator;
		G4float fTotalEnergyDepositedLiquidScintillator = 0.;
		G4float fTimeLiquidScintillator = 0.;
		G4float fEnergyOutLiquidScintillator = 0.;
		G4float fEnergyOtherLiquidScintillator = 0.;

		for(G4int i=0; i<iNbLiquidScintillatorHits; i++)
		{
			NerixLiquidScintillatorHit *pHit = (*pLiquidScintillatorHitsCollection)[i];

			if(iLiquidScintillatorId == -1)
				iLiquidScintillatorId = pHit->GetLiquidScintillatorId();

			if(pHit->GetLiquidScintillatorId() == iLiquidScintillatorId)
			{
				if(iTrackIdLiquidScintillator == -1)
				{
					iTrackIdLiquidScintillator = pHit->GetTrackId();
					hParticleTypeLiquidScintillator = pHit->GetParticleType();
					fTimeLiquidScintillator = pHit->GetTime()/ns;
				}

				if((pHit->GetParticleType() == hParticleTypeLiquidScintillator) && (pHit->GetDepositingProcess() == "Transportation"))
					fEnergyOutLiquidScintillator = pHit->GetKineticEnergy()/keV;

				if(iTrackIdLiquidScintillator != -1)
					fTotalEnergyDepositedLiquidScintillator += pHit->GetEnergyDeposited()/keV;
			}
			else
			{
				fEnergyOtherLiquidScintillator += pHit->GetEnergyDeposited()/keV;
			}
		}

		m_pEventData->m_iLiquidScintillatorId = iLiquidScintillatorId;
		m_pEventData->m_fTotalEnergyDepositedLiquidScintillator = fTotalEnergyDepositedLiquidScintillator;
		m_pEventData->m_iTrackIdLiquidScintillator = iTrackIdLiquidScintillator;
		m_pEventData->m_pParticleTypeLiquidScintillator->push_back(hParticleTypeLiquidScintillator);
		m_pEventData->m_fTimeLiquidScintillator = fTimeLiquidScintillator;
		m_pEventData->m_fEnergyInLiquidScintillator = (float) m_dEnergyInLiquidScintillator;
		m_pEventData->m_fEnergyOutLiquidScintillator = fEnergyOutLiquidScintillator;
		m_pEventData->m_fEnergyOtherLiquidScintillator = fEnergyOtherLiquidScintillator;

		// Germanium Detector Hits
		G4int iGeDetectorId = -1;
		G4int iTrackIdGeDetector = -1;
		G4String hParticleTypeGeDetector;
		G4float fTotalEnergyDepositedGeDetector = 0.;
		G4float fTimeGeDetector = 0.;
		G4float fEnergyOutGeDetector = 0.;
		G4float fEnergyOtherGeDetector = 0.;

		for(G4int i=0; i<iNbGeDetectorHits; i++)
		{
			NerixGeDetectorHit *pHit = (*pGeDetectorHitsCollection)[i];

			if(iGeDetectorId == -1)
				iGeDetectorId = pHit->GetGeDetectorId();

			if(pHit->GetGeDetectorId() == iGeDetectorId)
			{
				if(iTrackIdGeDetector == -1)
				{
					iTrackIdGeDetector = pHit->GetTrackId();
					hParticleTypeGeDetector = pHit->GetParticleType();
					fTimeGeDetector = pHit->GetTime()/ns;
				}

				if((pHit->GetParticleType() == hParticleTypeGeDetector) && (pHit->GetDepositingProcess() == "Transportation"))
					fEnergyOutGeDetector = pHit->GetKineticEnergy()/keV;

				if(iTrackIdGeDetector != -1)
					fTotalEnergyDepositedGeDetector += pHit->GetEnergyDeposited()/keV;
			}
			else
			{
				fEnergyOtherGeDetector += pHit->GetEnergyDeposited()/keV;
			}
		}

		m_pEventData->m_iGeDetectorId = iGeDetectorId;
		m_pEventData->m_fTotalEnergyDepositedGeDetector = fTotalEnergyDepositedGeDetector;
		m_pEventData->m_iTrackIdGeDetector = iTrackIdGeDetector;
		m_pEventData->m_pParticleTypeGeDetector->push_back(hParticleTypeGeDetector);
		m_pEventData->m_fTimeGeDetector = fTimeGeDetector;
		m_pEventData->m_fEnergyInGeDetector = (float) m_dEnergyInGeDetector;
		m_pEventData->m_fEnergyOutGeDetector = fEnergyOutGeDetector;
		m_pEventData->m_fEnergyOtherGeDetector = fEnergyOtherGeDetector;

		G4int iNbPmts = (G4int) NerixDetectorConstruction::GetGeometryParameter("NbPmts");
		G4bool bMultiAnodePmts = (G4bool) NerixDetectorConstruction::GetGeometryParameter("MultiAnodePmts");
		G4int iNbPmtAnodes = (G4int) NerixDetectorConstruction::GetGeometryParameter("NbPmtAnodes");

		if(!bMultiAnodePmts)
		{
			m_pEventData->m_pPmtHits->resize(iNbPmts, 0);
			for(G4int i=0; i<iNbPmts; i++)
				m_pEventData->m_pPmtHitsAngles->push_back(vector<float>());

			// Pmt hits
			for(G4int i=0; i<iNbPmtHits; i++)
			{
				G4int iPmtNb = (*pPmtHitsCollection)[i]->GetPmtNb();
                G4double dAngle = (*pPmtHitsCollection)[i]->GetAngle();
				(*(m_pEventData->m_pPmtHits))[iPmtNb]++;
				(*(m_pEventData->m_pPmtHitsAngles))[iPmtNb].push_back(dAngle);
			}

			m_pEventData->m_iNbPmtHits = accumulate(m_pEventData->m_pPmtHits->begin(), m_pEventData->m_pPmtHits->begin()+iNbPmts, 0);
		}
		else
		{
			G4int iNbPmtsWithMultiAnodes = iNbPmtAnodes*(iNbPmts-1)+1;
			m_pEventData->m_pPmtHits->resize(iNbPmtsWithMultiAnodes, 0);
			
			for(G4int i=0; i<iNbPmtsWithMultiAnodes; i++)
				m_pEventData->m_pPmtHitsAngles->push_back(vector<float>());

			// Pmt hits
			for(G4int i=0; i<iNbPmtHits; i++)
			{
				G4int iPmtNb = (*pPmtHitsCollection)[i]->GetPmtNb();
				G4int iQuadrantNb = (*pPmtHitsCollection)[i]->GetQuadrantNb();
				G4double dAngle = (*pPmtHitsCollection)[i]->GetAngle();
// 		               G4cout << "hPosition: " << hPosition << "  iPmtNb: " << iPmtNb << G4endl;
				(*(m_pEventData->m_pPmtHits))[iNbPmtAnodes*iPmtNb+iQuadrantNb]++;
				(*(m_pEventData->m_pPmtHitsAngles))[iNbPmtAnodes*iPmtNb+iQuadrantNb].push_back(dAngle);
			}

			m_pEventData->m_iNbPmtHits = accumulate(m_pEventData->m_pPmtHits->begin(), m_pEventData->m_pPmtHits->begin()+iNbPmtsWithMultiAnodes, 0);
		}

//      if(((fTotalEnergyDeposited > 0.) && ((fTotalEnergyDepositedLiquidScintillator > 0.) || (fTotalEnergyDepositedSodiumIodideDetector > 0.) || (fTotalEnergyDepositedGeDetector > 0.))) || (iNbPmtHits > 0))
//      if(iNbPmtHits > 0)
//      if(fTotalEnergyDeposited > 0.)
		
//		Write output data to tree only if energy deposited in LXe AND HPGe		
/*
		if (m_pEventData->m_fTotalEnergyDeposited > 0. && m_pEventData->m_fTotalEnergyDepositedGeDetector > 0.)
		{
			m_pTree->Fill();
		}
*/
		m_pTree->Fill();

		m_pEventData->Clear();
	}
}

void
NerixAnalysisManager::Step(const G4Step *pStep)
{
	G4Track *pTrack = pStep->GetTrack();

	if(pTrack->GetDefinition()->GetParticleName() == "neutron")
	{
		if(pStep->GetPostStepPoint()->GetPhysicalVolume())
		{
			G4String hPreVolumeName = pStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
			G4String hPostVolumeName = pStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
			G4String hProcessName = pStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

			if(hProcessName != "Transportation")
			{
				if(m_hNeutronVolumes.empty() || (hPostVolumeName != m_hNeutronVolumes.back()))
					m_hNeutronVolumes.push_back(hPostVolumeName);
			}
			else if((hPreVolumeName.substr(0, 24) == "LiquidScintillatorCasing") && (hPostVolumeName == "LiquidScintillator"))
				m_dEnergyInLiquidScintillator = pTrack->GetKineticEnergy()/keV;
		}
	}

	if(pTrack->GetDefinition()->GetParticleName() == "gamma")
	{
		if(pStep->GetPostStepPoint()->GetPhysicalVolume())
		{
			G4String hPreVolumeName = pStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
			G4String hPostVolumeName = pStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
			G4String hProcessName = pStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

			if(hProcessName != "Transportation")
			{
				if(m_hGammaVolumes.empty() || (hPostVolumeName != m_hGammaVolumes.back()))
					m_hGammaVolumes.push_back(hPostVolumeName);
			}
			else if((hPreVolumeName.substr(0, 16) == "GeDetectorCasing") && (hPostVolumeName == "GeDetector"))
				m_dEnergyInGeDetector = pTrack->GetKineticEnergy()/keV;
		}
	}
}

G4bool
NerixAnalysisManager::FilterEvent(NerixEventData *pEventData)
{
	return true;
}

