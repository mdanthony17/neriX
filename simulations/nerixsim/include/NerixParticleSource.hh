#ifndef __NERIXPARTICLESOURCE_H__
#define __NERIXPARTICLESOURCE_H__

#include <G4VPrimaryGenerator.hh>
#include <G4Navigator.hh>
#include <G4ParticleMomentum.hh>
#include <G4ParticleDefinition.hh>
#include <G4Track.hh>

#include <set>
#include <map>
#include <vector>

#include "NerixParticleSourceMessenger.hh"

class NerixParticleSource: public G4VPrimaryGenerator
{
public:
	NerixParticleSource();
	~NerixParticleSource();

public:
	void GeneratePrimaryVertex(G4Event *pEvent);
	void GeneratePrimaryVertexFromTrack(G4Track *pTrack, G4Event *pEvent);

	void SetPosDisType(G4String hSourcePosType) { m_hSourcePosType = hSourcePosType; }
	void SetPosDisShape(G4String hShape) { m_hShape = hShape; }
	void SetCenterCoords(G4ThreeVector hCenterCoords) { m_hCenterCoords = hCenterCoords; }
	void SetHalfZ(G4double dHalfz) { m_dHalfz = dHalfz; }
	void SetRadius(G4double dRadius) { m_dRadius = dRadius; }

	void SetAngDistType(G4String hAngDistType) { m_hAngDistType = hAngDistType; }
	void SetThetaRange(G4double dMinTheta, G4double dMaxTheta) { m_dMinTheta = dMinTheta; m_dMaxTheta = dMaxTheta; }
	void SetPhiRange(G4double dMinPhi, G4double dMaxPhi) { m_dMinPhi = dMinPhi; m_dMaxPhi = dMaxPhi; }
	void SetParticleMomentumDirection(G4ParticleMomentum hMomentum) { m_hParticleMomentumDirection = hMomentum.unit(); }

	void SetEnergyDisType(G4String hEnergyDisType) { m_hEnergyDisType = hEnergyDisType; }
	void SetEnergyFile(G4String hEnergyFile);
	void SetEnergyAngleFile(G4String hEnergyAngleFile);
	void SetMonoEnergy(G4double dMonoEnergy) { m_dMonoEnergy = dMonoEnergy; }

	void SetNumberOfParticlesToBeGenerated(G4int iNumParticles) { m_iNumberOfParticlesToBeGenerated = iNumParticles; }

	void SetParticleDefinition(G4ParticleDefinition *pParticleDefinition);
	inline void SetParticleCharge(G4double dCharge) { m_dParticleCharge = dCharge; }

	void SetVerbosity(G4int iVerbosityLevel) { m_iVerbosityLevel = iVerbosityLevel; }

	const G4String &GetParticleType() { return m_pParticleDefinition->GetParticleName(); }
	G4double GetParticleEnergy() { return m_dParticleEnergy; }
	const G4ThreeVector &GetParticlePosition() { return m_hParticlePosition; }
	const G4ThreeVector &GetParticleMomentumDirection() { return m_hParticleMomentumDirection; }

	G4bool ReadEnergySpectrum();
	G4bool ReadEnergyAngleSpectrum();
	void GeneratePointSource();
	void GeneratePointsInVolume();
	G4bool IsSourceConfined();
	void ConfineSourceToVolume(G4String);

	void GenerateIsotropicFlux();

	void GenerateMonoEnergetic();
	void GenerateEnergyFromSpectrum();
	void GenerateEnergyAngleFromSpectrum();

private:
	G4String m_hSourcePosType;
	G4String m_hShape;
	G4ThreeVector m_hCenterCoords;
	G4double m_dHalfz;
	G4double m_dRadius;
	G4bool m_bConfine;
	std::set<G4String> m_hVolumeNames;
	G4String m_hAngDistType;
	G4double m_dMinTheta, m_dMaxTheta, m_dMinPhi, m_dMaxPhi;
	G4double m_dTheta, m_dPhi;
	G4String m_hEnergyDisType;
	G4String m_hEnergyFile;
	G4String m_hEnergyAngleFile;
	G4double m_dMonoEnergy;

	G4int m_iNumberOfParticlesToBeGenerated;
	G4ParticleDefinition *m_pParticleDefinition;
	G4ParticleMomentum m_hParticleMomentumDirection;
	G4double m_dParticleEnergy;
	G4double m_dParticleCharge;
	G4ThreeVector m_hParticlePosition;
	G4double m_dParticleTime;
	G4ThreeVector m_hParticlePolarization;

	G4int m_iVerbosityLevel;

	std::map<G4String, G4double> m_hEnergyUnitFactor;
	std::map<G4String, G4double> m_hAngleUnitFactor;

	G4int m_iNbEnergyBins;
	G4double m_dEnergyBinWidth;
	std::vector<G4double> m_hEnergyBins;

	G4int m_iNbAngleBins;
	G4double m_dAngleBinWidth;
	std::vector<G4double> m_hAngleBins;

	std::vector<G4double> m_hEnergySpectrum;
	std::vector<G4double> m_hEnergySpectrumCdf;

	std::vector<G4double> m_hEnergyAngleSpectrum;
	std::vector<G4double> m_hEnergyAngleSpectrumCdf;

	NerixParticleSourceMessenger *m_pMessenger;
	G4Navigator *m_pNavigator;
};

#endif // __NERIXPARTICLESOURCE_H__

