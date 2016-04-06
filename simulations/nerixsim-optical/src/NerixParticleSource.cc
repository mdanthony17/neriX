#include <G4PrimaryParticle.hh>
#include <G4Event.hh>
#include <G4TransportationManager.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4IonTable.hh>
#include <G4Ions.hh>
#include <G4TrackingManager.hh>
#include <G4Track.hh>
#include <Randomize.hh>

#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <numeric>

#include "NerixParticleSource.hh"

using std::ifstream;
using std::stringstream;
using std::set;
using std::map;
using std::vector;

NerixParticleSource::NerixParticleSource()
{
	m_iNumberOfParticlesToBeGenerated = 1;
	m_pParticleDefinition = 0;
	G4ThreeVector hZero(0., 0., 0.);

	m_hParticleMomentumDirection = G4ParticleMomentum(1., 0., 0.);
	m_dParticleEnergy = 1.0*MeV;
	m_hParticlePosition = hZero;
	m_dParticleTime = 0.0;
	m_hParticlePolarization = hZero;
	m_dParticleCharge = 0.0;

	m_hSourcePosType = "Volume";
	m_hShape = "NULL";
	m_dHalfz = 0.;
	m_dRadius = 0.;
	m_hCenterCoords = hZero;
	m_bConfine = false;
	m_hVolumeNames.clear();

	m_hAngDistType = "iso";
	m_dMinTheta = 0.;
	m_dMaxTheta = pi;
	m_dMinPhi = 0.;
	m_dMaxPhi = twopi;

	m_hEnergyDisType = "Mono";
	m_dMonoEnergy = 1*MeV;
	m_hEnergyFile = "";
	m_hEnergyAngleFile = "";

	m_iVerbosityLevel = 0;

	m_hEnergyUnitFactor["eV"] = MeV/eV;
	m_hEnergyUnitFactor["keV"] = MeV/keV;
	m_hEnergyUnitFactor["MeV"] = MeV/MeV;
	m_hEnergyUnitFactor["GeV"] = MeV/GeV;

	m_hAngleUnitFactor["rad"] = rad/rad;
	m_hAngleUnitFactor["deg"] = rad/deg;

	m_pMessenger = new NerixParticleSourceMessenger(this);
	m_pNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
}

NerixParticleSource::~NerixParticleSource()
{
	delete m_pMessenger;
}

void
NerixParticleSource::SetParticleDefinition(G4ParticleDefinition *pParticleDefinition)
{
	m_pParticleDefinition = pParticleDefinition;
	m_dParticleCharge = m_pParticleDefinition->GetPDGCharge();
}

void
NerixParticleSource::SetEnergyFile(G4String hEnergyFile)
{
	m_hEnergyFile = hEnergyFile;

	ReadEnergySpectrum();
}

void
NerixParticleSource::SetEnergyAngleFile(G4String hEnergyAngleFile)
{
	m_hEnergyAngleFile = hEnergyAngleFile;

	ReadEnergyAngleSpectrum();
}

G4bool
NerixParticleSource::ReadEnergySpectrum()
{
	m_hEnergyBins.clear();
	m_hEnergySpectrum.clear();
	m_hEnergySpectrumCdf.clear();

	// read the energy spectrum from the file
	ifstream hIn(m_hEnergyFile.c_str());

	if(hIn.fail())
	{
		G4cout << "Error: cannot open energy spectrum file " << m_hEnergyFile << "!" << G4endl;
		return false;
	}

	if(m_iVerbosityLevel >= 1)
		G4cout << "Source energy spectrum from file: " << m_hEnergyFile << G4endl;

	// read the header
	G4String hEnergyUnit;
	while(!hIn.eof())
	{
		G4String hHeader;
		hIn >> hHeader;

		if(hHeader == "unit:")
			hIn >> hEnergyUnit;
		else if(hHeader == "spectrum:")
			break;
		else
		{
			G4cout << "Error: unknown tag in spectrum file!" << G4endl;
			return false;
		}
	}

	G4double dFactor = m_hEnergyUnitFactor[hEnergyUnit];

	G4double dBinEnergy = 0., dProbability = 0.;
	while(!hIn.eof())
	{
		hIn >> dBinEnergy >> dProbability;

		if(hIn.good())
		{
			if(m_iVerbosityLevel >= 2)
				G4cout << std::setprecision(3) << std::scientific << dBinEnergy << "  " << dProbability << G4endl;
			
			m_hEnergyBins.push_back(dBinEnergy*dFactor);
			m_hEnergySpectrum.push_back(dProbability);
		}
	}

	hIn.close();

	vector<G4double>::iterator pIt;
	G4double dCumulativeProbability = 0.;
	G4double dTotalProbability = std::accumulate(m_hEnergySpectrum.begin(), m_hEnergySpectrum.end(), 0.);
	for(pIt = m_hEnergySpectrum.begin(); pIt != m_hEnergySpectrum.end(); pIt++)
	{
		dCumulativeProbability += *pIt;
		m_hEnergySpectrumCdf.push_back(dCumulativeProbability/dTotalProbability);
	}

	m_iNbEnergyBins = m_hEnergyBins.size();
	m_dEnergyBinWidth = (m_hEnergyBins.back()-m_hEnergyBins.front())/(m_iNbEnergyBins-1);

	return true;
}

G4bool
NerixParticleSource::ReadEnergyAngleSpectrum()
{
	m_hEnergyBins.clear();
	m_hAngleBins.clear();
	m_hEnergyAngleSpectrum.clear();
	m_hEnergyAngleSpectrumCdf.clear();

	// read the energy angle spectrum from the file
	ifstream hIn(m_hEnergyAngleFile.c_str());

	if(hIn.fail())
	{
		G4cout << "Error: cannot open energy angle spectrum file " << m_hEnergyAngleFile << "!" << G4endl;
		return false;
	}

	if(m_iVerbosityLevel >= 1)
		G4cout << "Source energy angle spectrum from file: " << m_hEnergyAngleFile << G4endl;

	// read the header
	while(!hIn.eof())
	{
		G4String hHeader;
		hIn >> hHeader;

		if(hHeader[0] == '#')
		{
			while(1) if(hIn.get() == '\n') break;
		}
		else if(hHeader == "energies:")
		{
			hIn >> hHeader;

			G4String hEnergyUnit;
			if(hHeader == "unit:")
				hIn >> hEnergyUnit;
			else
			{
				G4cout << "Error: unknown tag in spectrum file!" << G4endl;
				return false;
			}

			G4double dEnergyFactor = m_hEnergyUnitFactor[hEnergyUnit];

			hIn >> m_iNbEnergyBins;

			G4double dBinEnergy = 0.;
			for(int iBin = 0; iBin < m_iNbEnergyBins; iBin++)
			{
				hIn >> dBinEnergy;

				if(hIn.good())
					m_hEnergyBins.push_back(dBinEnergy*dEnergyFactor);
			}

			m_dEnergyBinWidth = (m_hEnergyBins.back()-m_hEnergyBins.front())/(m_iNbEnergyBins-1);
		}
		else if(hHeader == "angles:")
		{
			hIn >> hHeader;

			G4String hAngleUnit;
			if(hHeader == "unit:")
				hIn >> hAngleUnit;
			else
			{
				G4cout << "Error: unknown tag in spectrum file!" << G4endl;
				return false;
			}

			G4double dAngleFactor = m_hAngleUnitFactor[hAngleUnit];

			hIn >> m_iNbAngleBins;

			G4double dBinAngle = 0.;
			for(int iBin = 0; iBin < m_iNbAngleBins; iBin++)
			{
				hIn >> dBinAngle;

				if(hIn.good())
					m_hAngleBins.push_back(dBinAngle*dAngleFactor);
			}

			m_dAngleBinWidth = (m_hAngleBins.back()-m_hAngleBins.front())/(m_iNbAngleBins-1);
		}
		else if(hHeader == "spectrum:")
			break;
		else
		{
			G4cout << "Error: unknown tag in spectrum file!" << G4endl;
			return false;
		}
	}

	G4double dProbability = 0.;

	while(!hIn.eof())
	{
		hIn >> dProbability;

		if(hIn.good())
			m_hEnergyAngleSpectrum.push_back(dProbability);
	}

	hIn.close();

	vector<G4double>::iterator pIt;
	G4double dCumulativeProbability = 0.;
	G4double dTotalProbability = std::accumulate(m_hEnergyAngleSpectrum.begin(), m_hEnergyAngleSpectrum.end(), 0.);
	for(pIt = m_hEnergyAngleSpectrum.begin(); pIt != m_hEnergyAngleSpectrum.end(); pIt++)
	{
		dCumulativeProbability += *pIt;
		m_hEnergyAngleSpectrumCdf.push_back(dCumulativeProbability/dTotalProbability);
	}

	return true;
}

void
NerixParticleSource::ConfineSourceToVolume(G4String hVolumeList)
{
	stringstream hStream;
	hStream.str(hVolumeList);
	G4String hVolumeName;

	// store all the volume names
	while(!hStream.eof())
	{
		hStream >> hVolumeName;
		m_hVolumeNames.insert(hVolumeName);
	}

	// checks if the selected volumes exist and store all volumes that match
	G4PhysicalVolumeStore *PVStore = G4PhysicalVolumeStore::GetInstance();
	G4bool bFoundAll = true;

	set<G4String> hActualVolumeNames;
	for(set<G4String>::iterator pIt = m_hVolumeNames.begin(); pIt != m_hVolumeNames.end(); pIt++)
	{
		G4String hRequiredVolumeName = *pIt;
		G4bool bMatch = false;

		if((bMatch = (hRequiredVolumeName.last('*')) != std::string::npos))
			hRequiredVolumeName = hRequiredVolumeName.strip(G4String::trailing, '*');

		G4bool bFoundOne = false;
		for(G4int iIndex = 0; iIndex < (G4int) PVStore->size(); iIndex++)
		{
			G4String hName = (*PVStore)[iIndex]->GetName();

			if((bMatch && (hName.substr(0, hRequiredVolumeName.size())) == hRequiredVolumeName) || hName == hRequiredVolumeName)
			{
				hActualVolumeNames.insert(hName);
				bFoundOne = true;
			}
		}

		bFoundAll = bFoundAll && bFoundOne;
	}

	if(bFoundAll)
	{
		m_hVolumeNames = hActualVolumeNames;
		m_bConfine = true;

		if(m_iVerbosityLevel >= 1)
			G4cout << "Source confined to volumes: " << hVolumeList << G4endl;

		if(m_iVerbosityLevel >= 2)
		{
			G4cout << "Volume list: " << G4endl;

			for(set<G4String>::iterator pIt = m_hVolumeNames.begin(); pIt != m_hVolumeNames.end(); pIt++)
				G4cout << *pIt << G4endl;
		}
	}
	else if(m_hVolumeNames.empty())
		m_bConfine = false;
	else
	{
		G4cout << " **** Error: One or more volumes do not exist **** " << G4endl;
		G4cout << " Ignoring confine condition" << G4endl;
		m_hVolumeNames.clear();
		m_bConfine = false;
	}
}

void
NerixParticleSource::GeneratePointSource()
{
	// Generates Points given the point source.
	if(m_hSourcePosType == "Point")
		m_hParticlePosition = m_hCenterCoords;
	else if(m_iVerbosityLevel >= 1)
		G4cout << "Error SourcePosType is not set to Point" << G4endl;
}

void
NerixParticleSource::GeneratePointsInVolume()
{
	G4ThreeVector RandPos;
	G4double x = 0., y = 0., z = 0.;

	if(m_hSourcePosType != "Volume" && m_iVerbosityLevel >= 1)
		G4cout << "Error SourcePosType not Volume" << G4endl;

	if(m_hShape == "Sphere")
	{
		x = m_dRadius * 2.;
		y = m_dRadius * 2.;
		z = m_dRadius * 2.;
		while(((x * x) + (y * y) + (z * z)) > (m_dRadius * m_dRadius))
		{
			x = G4UniformRand();
			y = G4UniformRand();
			z = G4UniformRand();

			x = (x * 2. * m_dRadius) - m_dRadius;
			y = (y * 2. * m_dRadius) - m_dRadius;
			z = (z * 2. * m_dRadius) - m_dRadius;
		}
	}

	else if(m_hShape == "Cylinder")
	{
		x = m_dRadius * 2.;
		y = m_dRadius * 2.;
		while(((x * x) + (y * y)) > (m_dRadius * m_dRadius))
		{
			x = G4UniformRand();
			y = G4UniformRand();
			z = G4UniformRand();
			x = (x * 2. * m_dRadius) - m_dRadius;
			y = (y * 2. * m_dRadius) - m_dRadius;
			z = (z * 2. * m_dHalfz) - m_dHalfz;
		}
	}

	else
		G4cout << "Error: Volume Shape Does Not Exist" << G4endl;

	RandPos.setX(x);
	RandPos.setY(y);
	RandPos.setZ(z);
	m_hParticlePosition = m_hCenterCoords + RandPos;

}

G4bool
NerixParticleSource::IsSourceConfined()
{
	// Method to check point is within the volume specified
	if(m_bConfine == false)
		G4cout << "Error: Confine is false" << G4endl;
	G4ThreeVector null(0., 0., 0.);
	G4ThreeVector *ptr;

	ptr = &null;

	// Check m_hParticlePosition is within a volume in our list
	G4VPhysicalVolume *theVolume;

	theVolume = m_pNavigator->LocateGlobalPointAndSetup(m_hParticlePosition, ptr, true);
	G4String theVolName = theVolume->GetName();

	set<G4String>::iterator pIt;
	if((pIt = m_hVolumeNames.find(theVolName)) != m_hVolumeNames.end())
	{
		if(m_iVerbosityLevel >= 1)
			G4cout << "Particle is in volume " << *pIt << G4endl;
		return (true);
	}
	else
		return (false);
}

void
NerixParticleSource::GenerateIsotropicFlux()
{
	G4double rndm, rndm2;
	G4double px, py, pz;

	G4double sintheta, sinphi, costheta, cosphi;

	rndm = G4UniformRand();
	costheta = std::cos(m_dMinTheta) - rndm * (std::cos(m_dMinTheta) - std::cos(m_dMaxTheta));
	sintheta = std::sqrt(1. - costheta * costheta);

	rndm2 = G4UniformRand();
	m_dPhi = m_dMinPhi + (m_dMaxPhi - m_dMinPhi) * rndm2;
	sinphi = std::sin(m_dPhi);
	cosphi = std::cos(m_dPhi);

	px = -sintheta * cosphi;
	py = -sintheta * sinphi;
	pz = -costheta;

	G4double ResMag = std::sqrt((px * px) + (py * py) + (pz * pz));

	px = px / ResMag;
	py = py / ResMag;
	pz = pz / ResMag;

	m_hParticleMomentumDirection.setX(px);
	m_hParticleMomentumDirection.setY(py);
	m_hParticleMomentumDirection.setZ(pz);

	// m_hParticleMomentumDirection now holds unit momentum vector.
	if(m_iVerbosityLevel >= 2)
		G4cout << "Generating isotropic vector: " <<
			m_hParticleMomentumDirection << G4endl;
}

void
NerixParticleSource::GenerateMonoEnergetic()
{
	m_dParticleEnergy = m_dMonoEnergy;
}

void
NerixParticleSource::GenerateEnergyFromSpectrum()
{
	G4double dRandom = G4UniformRand();

	vector<G4double>::iterator pIt = std::lower_bound(m_hEnergySpectrumCdf.begin(), m_hEnergySpectrumCdf.end(), dRandom);
	int iBin = pIt-m_hEnergySpectrumCdf.begin();

	G4double dE = m_hEnergyBins[iBin];
	G4double dP0 = (iBin != 0)?(m_hEnergySpectrumCdf[iBin-1]):(0.);
	G4double dP1 = m_hEnergySpectrumCdf[iBin];

	m_dParticleEnergy = (dE-m_dEnergyBinWidth + ((dRandom-dP0)/(dP1-dP0))*m_dEnergyBinWidth)*MeV;
}

void
NerixParticleSource::GenerateEnergyAngleFromSpectrum()
{
	G4bool bWithinThetaRange = false;
	int iBin, iAngleBin, iEnergyBin;
	G4double dEnergy, dAngle;

	// theta and energy
	while(!bWithinThetaRange)
	{
		G4double dRandom = G4UniformRand();

		vector<G4double>::iterator pIt = std::lower_bound(m_hEnergyAngleSpectrumCdf.begin(), m_hEnergyAngleSpectrumCdf.end(), dRandom);
		iBin = pIt-m_hEnergyAngleSpectrumCdf.begin();
		iAngleBin = iBin/m_iNbEnergyBins;

		dAngle = (m_hAngleBins[iAngleBin]-0.5*m_dAngleBinWidth + m_dAngleBinWidth*G4UniformRand())*rad;

		if(dAngle < m_dMinTheta || dAngle > m_dMaxTheta)
			continue;
		bWithinThetaRange = true;

		iEnergyBin = iBin-iAngleBin*m_iNbEnergyBins;

		G4double dE = m_hEnergyBins[iEnergyBin];
		G4double dP0 = (iBin != 0)?(m_hEnergyAngleSpectrumCdf[iBin-1]):(0.);
		G4double dP1 = m_hEnergyAngleSpectrumCdf[iBin];
		
		dEnergy = (dE-0.5*m_dEnergyBinWidth + ((dRandom-dP0)/(dP1-dP0))*m_dEnergyBinWidth)*MeV;
	}
	m_dTheta = dAngle;
	m_dParticleEnergy = dEnergy;

	G4double dCosTheta = std::cos(m_dTheta);
	G4double dSinTheta = std::sin(m_dTheta);

	// phi angle
	G4double dRandom = G4UniformRand();
	m_dPhi = m_dMinPhi + (m_dMaxPhi - m_dMinPhi) * dRandom;
	G4double dSinPhi = std::sin(m_dPhi);
	G4double dCosPhi = std::cos(m_dPhi);

	G4ThreeVector hDirection(dSinTheta*dCosPhi, dSinTheta*dSinPhi, dCosTheta);

	m_hParticleMomentumDirection = hDirection.unit();
}

void
NerixParticleSource::GeneratePrimaryVertex(G4Event * evt)
{

	if(m_pParticleDefinition == 0)
	{
		G4cout << "No particle has been defined!" << G4endl;
		return;
	}

	// Position
	G4bool srcconf = false;
	G4int LoopCount = 0;

	while(srcconf == false)
	{
		if(m_hSourcePosType == "Point")
			GeneratePointSource();
		else if(m_hSourcePosType == "Volume")
			GeneratePointsInVolume();
		else
		{
			G4cout << "Error: SourcePosType undefined" << G4endl;
			G4cout << "Generating point source" << G4endl;
			GeneratePointSource();
		}

		if(m_bConfine == true)
		{
			srcconf = IsSourceConfined();
			// if source in confined srcconf = true terminating the loop
			// if source isnt confined srcconf = false and loop continues
		}
		else if(m_bConfine == false)
			srcconf = true;		// terminate loop

		LoopCount++;
		if(LoopCount == 1000000)
		{
			G4cout << "*************************************" << G4endl;
			G4cout << "LoopCount = 1000000" << G4endl;
			G4cout << "Either the source distribution >> confinement" << G4endl;
			G4cout << "or any confining volume may not overlap with" << G4endl;
			G4cout << "the source distribution or any confining volumes" << G4endl;
			G4cout << "may not exist" << G4endl;
			G4cout << "If you have set confine then this will be ignored" << G4endl;
			G4cout << "for this event." << G4endl;
			G4cout << "*************************************" << G4endl;
			srcconf = true;		//Avoids an infinite loop
		}
	}

	// Angular stuff
	if(m_hEnergyDisType != "EnergyAngle")
	{
		if(m_hAngDistType == "iso" || m_hAngDistType == "flat")
			GenerateIsotropicFlux();
		else if(m_hAngDistType == "direction")
			SetParticleMomentumDirection(m_hParticleMomentumDirection);
		else
			G4cout << "Error: AngDistType has unusual value" << G4endl;
	}

	// Energy stuff
	if(m_hEnergyDisType == "Mono")
		GenerateMonoEnergetic();
	else if(m_hEnergyDisType == "Spectrum")
		GenerateEnergyFromSpectrum();
	else if(m_hEnergyDisType == "EnergyAngle")
		GenerateEnergyAngleFromSpectrum();
	else
		G4cout << "Error: EnergyDisType has unusual value" << G4endl;

	// create a new vertex
	G4PrimaryVertex *vertex = new G4PrimaryVertex(m_hParticlePosition, m_dParticleTime);

	if(m_iVerbosityLevel >= 2)
		G4cout << "Creating primaries and assigning to vertex" << G4endl;
	// create new primaries and set them to the vertex
	G4double mass = m_pParticleDefinition->GetPDGMass();
	G4double energy = m_dParticleEnergy + mass;
	G4double pmom = std::sqrt(energy * energy - mass * mass);
	G4double px = pmom * m_hParticleMomentumDirection.x();
	G4double py = pmom * m_hParticleMomentumDirection.y();
	G4double pz = pmom * m_hParticleMomentumDirection.z();

	if(m_iVerbosityLevel >= 1)
	{
		G4cout << "Particle name: " << m_pParticleDefinition->GetParticleName() << G4endl;
		G4cout << "       Energy: " << m_dParticleEnergy << G4endl;
		G4cout << "     Position: " << m_hParticlePosition << G4endl;
		G4cout << "    Direction: " << m_hParticleMomentumDirection << G4endl;
		G4cout << " NumberOfParticlesToBeGenerated: " << m_iNumberOfParticlesToBeGenerated << G4endl;
	}

	for(G4int i = 0; i < m_iNumberOfParticlesToBeGenerated; i++)
	{
		G4PrimaryParticle *particle = new G4PrimaryParticle(m_pParticleDefinition, px, py, pz);
		particle->SetMass(mass);
		particle->SetCharge(m_dParticleCharge);
		particle->SetPolarization(m_hParticlePolarization.x(), m_hParticlePolarization.y(), m_hParticlePolarization.z());
		vertex->SetPrimary(particle);
	}
	evt->AddPrimaryVertex(vertex);
	if(m_iVerbosityLevel > 1)
		G4cout << " Primary Vetex generated " << G4endl;
}

void
NerixParticleSource::GeneratePrimaryVertexFromTrack(G4Track *pTrack, G4Event *pEvent)
{
	G4double dPX = pTrack->GetMomentum().x();
	G4double dPY = pTrack->GetMomentum().y();
	G4double dPZ = pTrack->GetMomentum().z();

	G4PrimaryVertex *pVertex = new G4PrimaryVertex(pTrack->GetPosition(), m_dParticleTime);

	G4PrimaryParticle *pPrimary = new G4PrimaryParticle(pTrack->GetDefinition(), dPX, dPY, dPZ);
	pPrimary->SetMass(pTrack->GetDefinition()->GetPDGMass());
	pPrimary->SetCharge(pTrack->GetDefinition()->GetPDGCharge());

	pVertex->SetPrimary(pPrimary);

	pEvent->AddPrimaryVertex(pVertex);
}

