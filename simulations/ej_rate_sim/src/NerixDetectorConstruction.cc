#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Cons.hh>
#include <G4Polycone.hh>
#include <G4Polyhedra.hh>
#include <G4SubtractionSolid.hh>
#include <G4UnionSolid.hh>
#include <G4QuadrangularFacet.hh>
#include <G4TessellatedSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4PVParameterised.hh>
#include <G4OpBoundaryProcess.hh>
#include <G4SDManager.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
#include <G4VisAttributes.hh>
#include <G4UnitsTable.hh>
#include <G4Colour.hh>
#include <globals.hh>

#include <vector>
#include <numeric>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

using std::vector;
using std::string;
using std::ifstream;
using std::stringstream;


#include "NerixLXeSensitiveDetector.hh"
#include "NerixLiquidScintillatorSensitiveDetector.hh"
#include "NerixGeDetectorSensitiveDetector.hh"
#include "NerixPmtSensitiveDetector.hh"

#include "NerixDetectorConstruction.hh"
#include "lscint_settings.hh"


map<G4String, G4double> NerixDetectorConstruction::m_hGeometryParameters;

NerixDetectorConstruction::NerixDetectorConstruction():
	m_pLabLogicalVolume(0), m_pLXeLogicalVolume(0),
	m_pLabPhysicalVolume(0), m_pLXePhysicalVolume(0)
{
	m_pRotationXPlus90 = new G4RotationMatrix();
	m_pRotationXPlus90->rotateX(90*deg);

	m_pRotationXMinus90 = new G4RotationMatrix();
	m_pRotationXMinus90->rotateX(-90*deg);

	m_pRotationX180 = new G4RotationMatrix();
	m_pRotationX180->rotateX(180*deg);

	m_pRotationYPlus90 = new G4RotationMatrix();
	m_pRotationYPlus90->rotateY(90*deg);

	m_pRotationYMinus90 = new G4RotationMatrix();
	m_pRotationYMinus90->rotateY(-90*deg);

	m_pRotationZPlus90 = new G4RotationMatrix();
	m_pRotationZPlus90->rotateZ(90*deg);

	m_pRotationZMinus90 = new G4RotationMatrix();
	m_pRotationZMinus90->rotateZ(-90*deg);

	m_pRotationZPlus45 = new G4RotationMatrix();
	m_pRotationZPlus45->rotateZ(45*deg);

	m_pRotationXPlus90ZMinus90 = new G4RotationMatrix();
	m_pRotationXPlus90ZMinus90->rotateX(90*deg);
	m_pRotationXPlus90ZMinus90->rotateZ(-90*deg);

	m_pRotationXMinus90ZPlus90 = new G4RotationMatrix();
	m_pRotationXMinus90ZPlus90->rotateX(-90*deg);
	m_pRotationXMinus90ZPlus90->rotateZ(90*deg);

	m_pRotationYPlus45XPlus90 = new G4RotationMatrix();
	m_pRotationYPlus45XPlus90->rotateY(45*deg);
	m_pRotationYPlus45XPlus90->rotateX(90*deg);

	m_pRotationXPlus45YPlus90 = new G4RotationMatrix();
	m_pRotationXPlus45YPlus90->rotateX(45*deg);
	m_pRotationXPlus45YPlus90->rotateY(90*deg);

	m_pRotationLabToSimulation = new G4RotationMatrix();
}

NerixDetectorConstruction::~NerixDetectorConstruction()
{
}

G4VPhysicalVolume*
NerixDetectorConstruction::Construct()
{
	DefineMaterials();

	DefineGeometryParameters();

	ConstructLaboratory();

    //ConstructSource();

    ConstructMinitron();

	//ConstructFrame();

	//ConstructCryostat();

	//ConstructChamber();

	//ConstructXenon();

	//ConstructTeflon();

	//ConstructGrids();

	//ConstructPlates();

	//ConstructPmts();

    ConstructLiquidScintillators();

    //ConstructGeDetectors();

    //ConstructLeadCollimators();

	PrintGeometryInformation();

	return m_pLabPhysicalVolume;
}

void
NerixDetectorConstruction::UpdateMaterialConstantProperty(const G4String &hMaterialName, const G4String &hProperty, G4double dPropertyValue)
{
	G4Material *pMaterial = G4Material::GetMaterial(hMaterialName);

	if(pMaterial)
	{
		G4MaterialPropertiesTable *pPropertiesTable = pMaterial->GetMaterialPropertiesTable();

		if(!pPropertiesTable)
		{
			pPropertiesTable = new G4MaterialPropertiesTable();
			pMaterial->SetMaterialPropertiesTable(pPropertiesTable);
		}

		pPropertiesTable->AddConstProperty(hProperty.c_str(), dPropertyValue);
	}
	else
		G4Exception("NerixDetectorConstruction::UpdateMaterialProperty()", "G4Material::GetMaterial()", FatalException, "Could not find material!");
}

void
NerixDetectorConstruction::UpdateMaterialProperty(const G4String &hMaterialName, const G4String &hProperty, const vector<G4double> &hPhotonEnergies, const vector<G4double> &hPropertyValues)
{
	G4Material *pMaterial = G4Material::GetMaterial(hMaterialName);

	if(pMaterial)
	{
		G4MaterialPropertiesTable *pPropertiesTable = pMaterial->GetMaterialPropertiesTable();

		if(!pPropertiesTable)
		{
			pPropertiesTable = new G4MaterialPropertiesTable();
			pMaterial->SetMaterialPropertiesTable(pPropertiesTable);
		}

		pPropertiesTable->AddProperty(hProperty.c_str(), (G4double *) hPhotonEnergies.data(), (G4double *) hPropertyValues.data(), hPhotonEnergies.size());
	}
	else
		G4Exception("NerixDetectorConstruction::UpdateMaterialProperty()", "G4Material::GetMaterial()", FatalException, "Could not find material!");
}

void
NerixDetectorConstruction::UpdateMaterialPropertyFromString(const G4String &hString)
{
	stringstream hStream(hString);

	string hMaterialName, hAction, hProperty, hEnergyUnit, hPropertyUnit;
	vector<G4double> hPhotonEnergies, hPropertyValues;
	G4double dPropertyValue;

	// get material, action, and property
	hStream >> hMaterialName >> hAction >> hProperty;

	G4cout << "material: " << hMaterialName << ", ";
	G4cout << "action: " << hAction << ", ";
	G4cout << "property: " << hProperty << G4endl;

	string hValues = hStream.str().substr(hStream.tellg());
	
	G4cout << hValues << G4endl;

	if(hAction == "AddProperty")
	{
		stringstream hTmp(hValues);

		// parse photon energy values
		hTmp.seekg(hTmp.str().find_first_of("{")+1);
		hPhotonEnergies.clear();
		G4double dPhotonEnergy;
		while(hTmp.peek() != '}')
		{
			if(hTmp.peek() == ',')
				hTmp.ignore();
			hTmp >> dPhotonEnergy;
			hPhotonEnergies.push_back(dPhotonEnergy);
		}
		hTmp.ignore();
		hTmp >> hEnergyUnit;

		G4double dConversionFactor = G4UnitDefinition::GetValueOf(hEnergyUnit);
		vector<G4double>::iterator pIt;
		for(pIt = hPhotonEnergies.begin(); pIt != hPhotonEnergies.end(); pIt++)
			*pIt *= dConversionFactor;

		// parse property values
		hTmp.seekg(hTmp.str().find_first_of("{", hTmp.tellg())+1);
		hPropertyValues.clear();
		while(hTmp.peek() != '}')
		{
			if(hTmp.peek() == ',')
				hTmp.ignore();
			hTmp >> dPropertyValue;
			hPropertyValues.push_back(dPropertyValue);
		}
		hTmp.ignore();
		hTmp >> hPropertyUnit;

		if(hPropertyUnit != "")
		{
			G4double dConversionFactor = G4UnitDefinition::GetValueOf(hPropertyUnit);
			vector<G4double>::iterator pIt;
			for(pIt = hPropertyValues.begin(); pIt != hPropertyValues.end(); pIt++)
				*pIt *= dConversionFactor;
		}

		if(hPhotonEnergies.size() != hPropertyValues.size())
			G4Exception("NerixDetectorConstruction::UpdateMaterialPropertiesFromString()", "", FatalException, "Photon energy values count is different than property values count!");

		UpdateMaterialProperty(hMaterialName, hProperty, hPhotonEnergies, hPropertyValues);
	}
	else if(hAction == "AddConstProperty")
	{
		stringstream hTmp(hValues);

		hTmp >> dPropertyValue >> hPropertyUnit;

		if(hPropertyUnit.substr(0,2) == "1/")
			dPropertyValue *= 1./G4UnitDefinition::GetValueOf(hPropertyUnit.substr(2)); 
		else
			dPropertyValue *= G4UnitDefinition::GetValueOf(hPropertyUnit); 

		UpdateMaterialConstantProperty(hMaterialName, hProperty, dPropertyValue);
	}
}

void
NerixDetectorConstruction::UpdateMaterialPropertiesFromFile(const G4String &hFilename)
{
	ifstream hMaterialPropertiesFile(hFilename.c_str(), std::ios::binary);
	string hPropertyString;

	while(hMaterialPropertiesFile.good())
	{
		getline(hMaterialPropertiesFile, hPropertyString);
		if(!hPropertyString.empty())
			UpdateMaterialPropertyFromString(hPropertyString);
	}

	hMaterialPropertiesFile.close();
}

G4double
NerixDetectorConstruction::GetGeometryParameter(const char *szParameter)
{
	return m_hGeometryParameters[szParameter];
}

void
NerixDetectorConstruction::DefineMaterials()
{
	//================================== elements ===================================
	G4Element *Xe = new G4Element("Xenon",     "Xe", 54., 131.293*g/mole);
	G4Element *H  = new G4Element("Hydrogen",  "H",  1.,  1.0079*g/mole);
	G4Element *C  = new G4Element("Carbon",    "C",  6.,  12.011*g/mole);
	G4Element *N  = new G4Element("Nitrogen",  "N",  7.,  14.007*g/mole);
	G4Element *O  = new G4Element("Oxygen",    "O",  8.,  15.999*g/mole);
	G4Element *F  = new G4Element("Fluorine",  "F",  9.,  18.998*g/mole);
	G4Element *Al = new G4Element("Aluminium", "Al", 13., 26.982*g/mole);
	G4Element *Si = new G4Element("Silicon",   "Si", 14., 28.086*g/mole);
	G4Element *Cr = new G4Element("Chromium",  "Cr", 24., 51.996*g/mole);
	G4Element *Mn = new G4Element("Manganese", "Mn", 25., 54.938*g/mole);
	G4Element *Fe = new G4Element("Iron",      "Fe", 26., 55.850*g/mole);
	G4Element *Ni = new G4Element("Nickel",    "Ni", 28., 58.693*g/mole);
	G4Element *Ge = new G4Element("Germanium", "Ge", 32., 72.631*g/mole);
	G4Element *I  = new G4Element("Iodine",    "I",  53., 126.90*g/mole);
	G4Element *Pb = new G4Element("Lead",      "Pb", 82., 207.20*g/mole);
	
	//================================== materials ================================== 
	G4NistManager* pNistManager = G4NistManager::Instance();

	//------------------------------------- air -------------------------------------
	G4Material *Air = pNistManager->FindOrBuildMaterial("G4_AIR");

	//----------------------------------- vacuum ------------------------------------
	G4Material *Vacuum = new G4Material("Vacuum", 1.e-20*g/cm3, 2, kStateGas);
	Vacuum->AddElement(N, 0.755);
	Vacuum->AddElement(O, 0.245);
	
	//-------------------------------- liquid xenon ---------------------------------
	G4Material *LXe = new G4Material("LXe", 2.9172*g/cm3, 1, kStateLiquid, 168.15*kelvin, 1.5*atmosphere);
	LXe->AddElement(Xe, 1);

	const G4int iNbEntries = 3;

	G4double pdLXePhotonMomentum[]   = {6.91*eV, 6.98*eV, 7.05*eV};
	G4double pdLXeScintillation[]    = {0.1,     1.0,     0.1};
	G4double pdLXeRefractiveIndex[]  = {1.63,    1.61,    1.58};
	G4double pdLXeAbsorbtionLength[] = {100.*cm, 100.*cm, 100.*cm};
	G4double pdLXeScatteringLength[] = {30.*cm,  30.*cm,  30.*cm};

	const G4double dFieldQuenchingFactor = 1.00;

	G4MaterialPropertiesTable *pLXePropertiesTable = new G4MaterialPropertiesTable();

	pLXePropertiesTable->AddProperty("FASTCOMPONENT", pdLXePhotonMomentum, pdLXeScintillation, iNbEntries);
	pLXePropertiesTable->AddProperty("SLOWCOMPONENT", pdLXePhotonMomentum, pdLXeScintillation, iNbEntries);
	pLXePropertiesTable->AddProperty("RINDEX", pdLXePhotonMomentum, pdLXeRefractiveIndex, iNbEntries);
	pLXePropertiesTable->AddProperty("ABSLENGTH", pdLXePhotonMomentum, pdLXeAbsorbtionLength, iNbEntries);
	pLXePropertiesTable->AddProperty("RAYLEIGH", pdLXePhotonMomentum, pdLXeScatteringLength, iNbEntries);

//    pLXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", dFieldQuenchingFactor*1./(21.6*eV));
	//pLXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 100000./keV);
//    pLXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 1000000./keV);
    pLXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 0./keV);
	pLXePropertiesTable->AddConstProperty("RESOLUTIONSCALE", 1.0);
	pLXePropertiesTable->AddConstProperty("FASTTIMECONSTANT", 3.*ns);
	pLXePropertiesTable->AddConstProperty("SLOWTIMECONSTANT", 27.*ns);
	pLXePropertiesTable->AddConstProperty("YIELDRATIO", 1.0);

	LXe->SetMaterialPropertiesTable(pLXePropertiesTable);

	//-------------------------------- gaseous xenon --------------------------------
	G4Material *GXe = new G4Material("GXe", 0.005887*g/cm3, 1, kStateGas, 173.15*kelvin, 1.5*atmosphere);
	GXe->AddElement(Xe, 1);

	G4double pdGXePhotonMomentum[]   = {6.91*eV, 6.98*eV, 7.05*eV};
	G4double pdGXeScintillation[]    = {0.1,     1.0,     0.1};
	G4double pdGXeRefractiveIndex[]  = {1.00,    1.00,    1.00};
	G4double pdGXeAbsorbtionLength[] = {100*m,   100*m,   100*m};

	G4MaterialPropertiesTable *pGXePropertiesTable = new G4MaterialPropertiesTable();

	pGXePropertiesTable->AddProperty("FASTCOMPONENT", pdGXePhotonMomentum, pdGXeScintillation, iNbEntries);
	pGXePropertiesTable->AddProperty("SLOWCOMPONENT", pdGXePhotonMomentum, pdGXeScintillation, iNbEntries);
	pGXePropertiesTable->AddProperty("RINDEX", pdGXePhotonMomentum, pdGXeRefractiveIndex, iNbEntries);
	pGXePropertiesTable->AddProperty("ABSLENGTH", pdGXePhotonMomentum, pdGXeAbsorbtionLength, iNbEntries);

//    pGXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 1./(21.6*eV));
//    pGXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 100000./keV);
//	pGXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 400000./keV);
    pGXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 0./keV); //setting scintyield to zero...need to take out if looking for optical photons
	pGXePropertiesTable->AddConstProperty("RESOLUTIONSCALE", 1.0);
	pGXePropertiesTable->AddConstProperty("FASTTIMECONSTANT", 3.*ns);
	pGXePropertiesTable->AddConstProperty("SLOWTIMECONSTANT", 27.*ns);
	pGXePropertiesTable->AddConstProperty("YIELDRATIO", 1.0);

	GXe->SetMaterialPropertiesTable(pGXePropertiesTable);	

	//----------------------------------- quartz ------------------------------------
	// ref: http://www.sciner.com/Opticsland/FS.htm
	G4Material *Quartz = new G4Material("Quartz", 2.201*g/cm3, 2, kStateSolid, 168.15*kelvin, 1.5*atmosphere);
	Quartz->AddElement(Si, 1);
	Quartz->AddElement(O, 2);

	G4double pdQuartzPhotonMomentum[]   = {6.91*eV, 6.98*eV, 7.05*eV};
	G4double pdQuartzRefractiveIndex[]  = {1.50,    1.56,    1.60};
	G4double pdQuartzAbsorbtionLength[] = {30*m,    30*m,    30*m};

	G4MaterialPropertiesTable *pQuartzPropertiesTable = new G4MaterialPropertiesTable();

	pQuartzPropertiesTable->AddProperty("RINDEX", pdQuartzPhotonMomentum, pdQuartzRefractiveIndex, iNbEntries);
	pQuartzPropertiesTable->AddProperty("ABSLENGTH", pdQuartzPhotonMomentum, pdQuartzAbsorbtionLength, iNbEntries);

	Quartz->SetMaterialPropertiesTable(pQuartzPropertiesTable);

	//------------------------------- stainless steel -------------------------------
	G4Material *SS304LSteel = new G4Material("SS304LSteel", 8.00*g/cm3, 5, kStateSolid);
	SS304LSteel->AddElement(Fe, 0.65);
	SS304LSteel->AddElement(Cr, 0.20);
	SS304LSteel->AddElement(Ni, 0.12);
	SS304LSteel->AddElement(Mn, 0.02);
	SS304LSteel->AddElement(Si, 0.01);

	G4double pdSteelPhotonMomentum[] = {6.91*eV, 6.98*eV, 7.05*eV};
	G4double pdSteelReflectivity[]   = {0.20,    0.20,     0.20};
	G4MaterialPropertiesTable *pSteelPropertiesTable = new G4MaterialPropertiesTable();

	pSteelPropertiesTable->AddProperty("REFLECTIVITY", pdSteelPhotonMomentum, pdSteelReflectivity, iNbEntries);
	SS304LSteel->SetMaterialPropertiesTable(pSteelPropertiesTable);

	//---------------------------- grid stainless steel -----------------------------
	G4Material *GridSteel = new G4Material("GridSteel", 8.00*g/cm3, 5, kStateSolid);
	GridSteel->AddElement(Fe, 0.65);
	GridSteel->AddElement(Cr, 0.20);
	GridSteel->AddElement(Ni, 0.12);
	GridSteel->AddElement(Mn, 0.02);
	GridSteel->AddElement(Si, 0.01);

	G4double pdGridSteelPhotonMomentum[] = {6.91*eV, 6.98*eV, 7.05*eV};
//    G4double pdGridSteelReflectivity[]   = {0.90,    0.90,     0.90};
	G4double pdGridSteelReflectivity[]   = {0.20,    0.20,     0.20};
	G4MaterialPropertiesTable *pGridSteelPropertiesTable = new G4MaterialPropertiesTable();

	pGridSteelPropertiesTable->AddProperty("REFLECTIVITY", pdGridSteelPhotonMomentum, pdGridSteelReflectivity, iNbEntries);
	GridSteel->SetMaterialPropertiesTable(pGridSteelPropertiesTable);

	//---------------------------------- aluminium ----------------------------------
	G4Material* Aluminium = pNistManager->FindOrBuildMaterial("G4_Al");

	G4double pdAluminiumPhotonMomentum[] = {6.91*eV, 6.98*eV, 7.05*eV};
	G4double pdAluminiumReflectivity[]   = {0.90,    0.90,    0.90};
	G4MaterialPropertiesTable *pAluminiumPropertiesTable = new G4MaterialPropertiesTable();

	pAluminiumPropertiesTable->AddProperty("REFLECTIVITY", pdAluminiumPhotonMomentum, pdAluminiumReflectivity, iNbEntries);
	Aluminium->SetMaterialPropertiesTable(pAluminiumPropertiesTable);

	//---------------------------- photocathode aluminium ---------------------------
	G4Material *PhotoCathodeAluminium = new G4Material("PhotoCathodeAluminium", 8.00*g/cm3, 1, kStateSolid);
	PhotoCathodeAluminium->AddElement(Al, 1);

	G4double pdPhotoCathodePhotonMomentum[]   = {6.91*eV, 6.98*eV, 7.05*eV};
	G4double pdPhotoCathodeRefractiveIndex[]  = {1.50,    1.56,    1.60};
	G4double pdPhotoCathodeAbsorbtionLength[] = {1.*nm,   1.*nm,   1.*nm};

	G4MaterialPropertiesTable *pPhotoCathodePropertiesTable = new G4MaterialPropertiesTable();

	pPhotoCathodePropertiesTable->AddProperty("RINDEX", pdPhotoCathodePhotonMomentum, pdPhotoCathodeRefractiveIndex, iNbEntries);
	pPhotoCathodePropertiesTable->AddProperty("ABSLENGTH", pdPhotoCathodePhotonMomentum, pdPhotoCathodeAbsorbtionLength, iNbEntries);

	PhotoCathodeAluminium->SetMaterialPropertiesTable(pPhotoCathodePropertiesTable);

	//------------------------------------ cirlex -----------------------------------
	G4Material *Cirlex = new G4Material("Cirlex", 1.43*g/cm3, 4, kStateSolid);
	Cirlex->AddElement(C, 22);
	Cirlex->AddElement(H, 10);
	Cirlex->AddElement(N, 2);
	Cirlex->AddElement(O, 5);

	//------------------------------------ EJ301 ------------------------------------
	G4Material* EJ301 = new G4Material("EJ301", 0.874*g/cm3, 2);
	EJ301->AddElement(H, 54.8*perCent);
	EJ301->AddElement(C, 45.2*perCent);

	//------------------------------------ HPGe -------------------------------------
	G4Material *HPGe = new G4Material("HPGe", 5.323*g/cm3, 1);
	HPGe->AddElement(Ge, 1);

	//------------------------------------ teflon -----------------------------------
	G4Material* Teflon = new G4Material("Teflon", 2.2*g/cm3, 2, kStateSolid, 168.15*kelvin);
	Teflon->AddElement(C, 0.240183);
	Teflon->AddElement(F, 0.759817);

	G4double pdTeflonPhotonMomentum[]  = {6.91*eV, 6.98*eV, 7.05*eV};
	G4double pdTeflonRefractiveIndex[] = {1.63,    1.61,    1.58};
	G4double pdTeflonReflectivity[]    = {0.95,    0.95,    0.95};
	G4double pdTeflonSpecularLobe[]    = {0.01,    0.01,    0.01};
	G4double pdTeflonSpecularSpike[]   = {0.01,    0.01,    0.01};
	G4double pdTeflonBackscatter[]     = {0.01,    0.01,    0.01};
	G4double pdTeflonEfficiency[]      = {1.0,     1.0,     1.0};
	G4MaterialPropertiesTable *pTeflonPropertiesTable = new G4MaterialPropertiesTable();

	pTeflonPropertiesTable->AddProperty("RINDEX", pdTeflonPhotonMomentum, pdTeflonRefractiveIndex, iNbEntries);
	pTeflonPropertiesTable->AddProperty("REFLECTIVITY", pdTeflonPhotonMomentum, pdTeflonReflectivity, iNbEntries);
	pTeflonPropertiesTable->AddProperty("SPECULARLOBECONSTANT", pdTeflonPhotonMomentum, pdTeflonSpecularLobe, iNbEntries);
	pTeflonPropertiesTable->AddProperty("SPECULARSPIKECONSTANT", pdTeflonPhotonMomentum, pdTeflonSpecularSpike, iNbEntries);
	pTeflonPropertiesTable->AddProperty("BACKSCATTERCONSTANT", pdTeflonPhotonMomentum, pdTeflonBackscatter, iNbEntries);
	pTeflonPropertiesTable->AddProperty("EFFICIENCY", pdTeflonPhotonMomentum, pdTeflonEfficiency, iNbEntries);
	Teflon->SetMaterialPropertiesTable(pTeflonPropertiesTable);

	//------------------------------------- lead ------------------------------------
	G4Material *Lead = new G4Material("Lead", 11.34*g/cm3, 1);
	Lead->AddElement(Pb, 1); 

	//------------------------------------- wood ------------------------------------
	G4Material *Wood = new G4Material("Wood", 0.9*g/cm3, 3);
	Wood->AddElement(H , 4);
	Wood->AddElement(O , 1);
	Wood->AddElement(C , 2);

	//---------------------------------- mineral oil---------------------------------
	G4Material *MineralOil = new G4Material("MineralOil", 0.77*g/cm3, 2);
	MineralOil->AddElement(H , 2);
	MineralOil->AddElement(C , 1);

	//----------------------------- source stainless steel --------------------------
	G4Material *SourceSteel = new G4Material("SourceSteel", 8.00*g/cm3, 5, kStateSolid);
	SourceSteel->AddElement(Fe, 0.65);
	SourceSteel->AddElement(Cr, 0.20);
	SourceSteel->AddElement(Ni, 0.12);
	SourceSteel->AddElement(Mn, 0.02);
	SourceSteel->AddElement(Si, 0.01);

	G4double pdSourceSteelPhotonMomentum[] = {6.91*eV, 6.98*eV, 7.05*eV};
	G4double pdSourceSteelReflectivity[]   = {0.20,    0.20,     0.20};
	G4MaterialPropertiesTable *pSourceSteelPropertiesTable = new G4MaterialPropertiesTable();

	pSourceSteelPropertiesTable->AddProperty("REFLECTIVITY", pdSourceSteelPhotonMomentum, pdSourceSteelReflectivity, iNbEntries);
	SourceSteel->SetMaterialPropertiesTable(pSourceSteelPropertiesTable);
}

void
NerixDetectorConstruction::DefineGeometryParameters()
{
	// floor
	m_hGeometryParameters["FloorToCryostatFlange"] = 980.*mm;

	// source
	m_hGeometryParameters["SourcePositionX"] = 0.*mm;
	m_hGeometryParameters["SourcePositionY"] = 440.*mm;
	m_hGeometryParameters["SourcePositionZ"] = 0.*mm;

	m_hGeometryParameters["SourceRadius"] = 25.*mm;
	m_hGeometryParameters["SourceLength"] = 70.*mm;

	// minitron
	m_hGeometryParameters["MinitronPositionX"] = 0.*mm;
	m_hGeometryParameters["MinitronPositionY"] = 400.*mm;

	m_hGeometryParameters["MinitronBaseWidth"]     = 270.*mm;
	m_hGeometryParameters["MinitronBaseLength"]    = 430.*mm;
	m_hGeometryParameters["MinitronBaseHeight"]    = 130.*mm;
	m_hGeometryParameters["MinitronBaseToFloor"]   = 400.*mm;
	m_hGeometryParameters["MinitronBasePositionX"] = 0.*mm;
	m_hGeometryParameters["MinitronBasePositionY"] = 475.*mm;

	//m_hGeometryParameters["MinitronTubeRadius"]    = 25.4*mm;
	//m_hGeometryParameters["MinitronTubeThickness"] = 1.6*mm;
	m_hGeometryParameters["MinitronTubeRadius"]    = 36.3*mm;
	m_hGeometryParameters["MinitronTubeThickness"] = 1.7*mm;
	m_hGeometryParameters["MinitronTubeHeight"]    = 340.*mm;

	m_hGeometryParameters["MinitronHolderRadius"]          = 17.46*mm;
	m_hGeometryParameters["MinitronHolderHeight"]          = 255.02*mm;
	m_hGeometryParameters["MinitronHolderCutRadius"]       = 14.8*mm;
	m_hGeometryParameters["MinitronHolderCutDepth"]        = 114.935*mm;
	m_hGeometryParameters["MinitronHolderBottomToTubeTop"] = 310.*mm;

	m_hGeometryParameters["MinitronVacuumRadius"] = 12.7*mm;
	m_hGeometryParameters["MinitronVacuumHeight"] = 104.*mm;

	// frame
	m_hGeometryParameters["FrameWidth"]                 = 700.*mm;
	m_hGeometryParameters["FrameHeight"]                = 858.*mm;
	m_hGeometryParameters["FrameThickness"]             = 5.*mm;
	m_hGeometryParameters["FrameCutWidth"]              = 600.*mm;
	m_hGeometryParameters["FrameCutHeight"]             = 758.*mm;
	m_hGeometryParameters["FrameTableThickness"]        = 12.*mm;
	m_hGeometryParameters["FrameTableToCryostatFlange"] = 0.*mm;

	// cryostat
	m_hGeometryParameters["CryostatFlangeRadius"]                = 145.034*mm; // ISO-LF NW250
	m_hGeometryParameters["CryostatFlangeThickness"]             = 12.*mm;
	m_hGeometryParameters["CryostatWallRadius"]                  = 123.825*mm;
	m_hGeometryParameters["CryostatWallHeight"]                  = 467.*mm;
	m_hGeometryParameters["CryostatWallThickness"]               = 3.429*mm;
	m_hGeometryParameters["CryostatFlangeGasketThickness"]       = 4.01*mm;
	m_hGeometryParameters["CryostatFlangeToChamberMatingFlange"] = 73.5*mm;

	// chamber
	m_hGeometryParameters["ChamberFlangeRadius"]    = 101.22*mm;
	m_hGeometryParameters["ChamberFlangeThickness"] = 22.35*mm;
	
	m_hGeometryParameters["ChamberTopWallRadius"]    = 73.152*mm;
	m_hGeometryParameters["ChamberTopWallHeight"]    = 228.6*mm;
	m_hGeometryParameters["ChamberTopWallThickness"] = 3.048*mm;

	m_hGeometryParameters["ChamberMiddlePlateThickness"] = 1.524*mm;

	m_hGeometryParameters["ChamberWallRadius"]      = 40.132*mm;
	m_hGeometryParameters["ChamberWallHeight"]      = 109.22*mm;
	m_hGeometryParameters["ChamberWallThickness"]   = 3.048*mm;

	m_hGeometryParameters["ChamberWallBulgeRadius"]         = 17.399*mm;
	m_hGeometryParameters["ChamberWallBulgeHeight"]         = 52.451*mm;
	m_hGeometryParameters["ChamberWallBulgeThickness"]      = 1.651*mm;
	m_hGeometryParameters["ChamberWallBulgeRotationAngleZ"] = 190.*deg;

	m_hGeometryParameters["ChamberBufferWidth"]      = 50.8*mm;
	m_hGeometryParameters["ChamberBufferLength"]     = 65.2018*mm; // not exact
	m_hGeometryParameters["ChamberBufferHeight"]     = 103.3272*mm;
	m_hGeometryParameters["ChamberBufferStepHeight"] = 17.399*mm;
	m_hGeometryParameters["ChamberBufferStepLength"] = 25.7238*mm;
	m_hGeometryParameters["ChamberBufferThickness"]  = 2.1082*mm;

	m_hGeometryParameters["ChamberBottomPlateThickness"] = 1.524*mm;

	// lxe
	m_hGeometryParameters["LXeHeight"] = 107.11*mm;
	m_hGeometryParameters["GXeGapHeight"] = 2.5*mm;

	// grid options
	m_hGeometryParameters["UseTopGrid"] = 0;
	m_hGeometryParameters["UseScreeningGrid"] = 1;

	// teflon
	m_hGeometryParameters["TeflonStackInnerRadius"] = 21.8821*mm;
	m_hGeometryParameters["TeflonStackOuterRadius"] = 32.0675*mm;

	m_hGeometryParameters["TeflonTopPmtHolderRadius"]           = 42.7609*mm;
	m_hGeometryParameters["TeflonTopPmtHolderHeight"]           = 33.6042*mm;
	m_hGeometryParameters["TeflonTopPmtHolderSquareCutWidth"]   = 55.8546*mm;
	m_hGeometryParameters["TeflonTopPmtHolderSquareCutDepth"]   = 25.7058*mm;
	m_hGeometryParameters["TeflonTopPmtHolderTopStepWidth"]     = 13.7541*mm;
	m_hGeometryParameters["TeflonTopPmtHolderTopStepHeight"]    = 2.032*mm;
	m_hGeometryParameters["TeflonTopPmtHolderBottomStepWidth"]  = 4.6355*mm;
	m_hGeometryParameters["TeflonTopPmtHolderBottomStepHeight"] = 1.778*mm;

	m_hGeometryParameters["TeflonTopGridSpacerHeight"]           = 10.9474*mm;
	m_hGeometryParameters["TeflonTopGridSpacerInnerStepWidth"]   = 7.332*mm;
	m_hGeometryParameters["TeflonTopGridSpacerInnerStepHeight"]  = 4.064*mm;
	m_hGeometryParameters["TeflonTopGridSpacerTopStepWidth"]     = 2.853*mm;
	m_hGeometryParameters["TeflonTopGridSpacerTopStepHeight"]    = 2.032*mm;
	m_hGeometryParameters["TeflonTopGridSpacerBottomStepWidth"]  = 4.843*mm;
	m_hGeometryParameters["TeflonTopGridSpacerBottomStepHeight"] = 1.778*mm;

	m_hGeometryParameters["TeflonAnodeSpacerHeight"]          = 11.2014*mm;
	m_hGeometryParameters["TeflonAnodeSpacerInnerStepWidth"]  = 7.2644*mm;
	m_hGeometryParameters["TeflonAnodeSpacerInnerStepHeight"] = 4.064*mm;
	m_hGeometryParameters["TeflonAnodeSpacerGapHeight"]       = 5.08*mm;

	m_hGeometryParameters["TeflonDriftRegionSpacerHeight"]           = 25.8572*mm;
	m_hGeometryParameters["TeflonDriftRegionSpacerInnerStepWidth"]   = 7.2517*mm;
	m_hGeometryParameters["TeflonDriftRegionSpacerInnerStepHeight"]  = 2.032*mm;
	m_hGeometryParameters["TeflonDriftRegionSpacerTopStepWidth"]     = 3.6195*mm;
	m_hGeometryParameters["TeflonDriftRegionSpacerTopStepHeight"]    = 1.778*mm;
	m_hGeometryParameters["TeflonDriftRegionSpacerBottomStepWidth"]  = 4.064*mm;
	m_hGeometryParameters["TeflonDriftRegionSpacerBottomStepHeight"] = 18.7198*mm;

	m_hGeometryParameters["TeflonScreeningGridSpacerHeight"]          = 8.9154*mm;
	m_hGeometryParameters["TeflonScreeningGridSpacerInnerStepWidth"]  = 7.239*mm;
	m_hGeometryParameters["TeflonScreeningGridSpacerInnerStepHeight"] = 2.032*mm;
	m_hGeometryParameters["TeflonScreeningGridSpacerTopStepWidth"]    = 7.6962*mm;
	m_hGeometryParameters["TeflonScreeningGridSpacerTopStepHeight"]   = 1.778*mm;

	m_hGeometryParameters["TeflonBottomPmtHolderRadius"]          = 36.195*mm;
	m_hGeometryParameters["TeflonBottomPmtHolderHeight"]          = 38.9382*mm;
	m_hGeometryParameters["TeflonBottomPmtHolderInnerStepWidth"]  = 7.3914*mm;
	m_hGeometryParameters["TeflonBottomPmtHolderInnerStepHeight"] = 34.1122*mm;
	m_hGeometryParameters["TeflonBottomPmtHolderTopStepWidth"]    = 11.8237*mm;
	m_hGeometryParameters["TeflonBottomPmtHolderTopStepHeight"]   = 1.7526*mm;

	// grids
	m_hGeometryParameters["GridRingRadius"]    = 28.4988*mm;
	m_hGeometryParameters["GridRingWidth"]     = 3.9878*mm;
	m_hGeometryParameters["GridRingThickness"] = 2.0066*mm;

	m_hGeometryParameters["TopGridWireDiameter"]       = 125.*um;
	m_hGeometryParameters["TopGridWirePitch"]          = 3.*mm;
	m_hGeometryParameters["AnodeGridWireDiameter"]     = 120.*um;
	m_hGeometryParameters["AnodeGridWirePitch"]        = 3.*mm;
	m_hGeometryParameters["BottomGridWireDiameter"]    = 120.*um;
	m_hGeometryParameters["BottomGridWirePitch"]       = 3.*mm;
	m_hGeometryParameters["CathodeGridWireDiameter"]   = 25.*um;
	m_hGeometryParameters["CathodeGridWirePitch"]       = 3.*mm;
	m_hGeometryParameters["ScreeningGridWireDiameter"] = 125.*um;
	m_hGeometryParameters["ScreeningGridWirePitch"]    = 3.*mm;

	// plates
	m_hGeometryParameters["TopPlateRadius"]    = 43.0022*mm;
	m_hGeometryParameters["TopPlateThickness"] = 3.175*mm;

	m_hGeometryParameters["TopPmtPlateRadius"]     = 43.0022*mm;
	m_hGeometryParameters["TopPmtPlateThickness"]  = 3.175*mm;
//    m_hGeometryParameters["TopPlateToTopPmtPlate"] = 20.701*mm; // run_00 value
	m_hGeometryParameters["TopPlateToTopPmtPlate"] = 12.7*mm;

	m_hGeometryParameters["BottomPlateRadius"]      = 36.83*mm;
	m_hGeometryParameters["BottomPlateInnerRadius"] = 29.21*mm;
	m_hGeometryParameters["BottomPlateThickness"]   = 3.175*mm;

	m_hGeometryParameters["BottomPmtPlateRadius"]        = 37.465*mm;
	m_hGeometryParameters["BottomPmtPlateThickness"]     = 3.175*mm;
	m_hGeometryParameters["BottomPlateToBottomPmtPlate"] = 20.701*mm;

	// pmts
	m_hGeometryParameters["NbPmts"] = 5;

	m_hGeometryParameters["MultiAnodePmts"] = 1;
	m_hGeometryParameters["NbPmtAnodes"] = 4;

	m_hGeometryParameters["TopPmtWindowWidth"]           = 24.0*mm;
	m_hGeometryParameters["TopPmtWindowThickness"]       = 1.5*mm;
	m_hGeometryParameters["TopPmtCasingWidth"]           = 25.4*mm;
	m_hGeometryParameters["TopPmtCasingHeight"]          = 27.0*mm;
	m_hGeometryParameters["TopPmtCasingThickness"]       = 0.5*mm;
	m_hGeometryParameters["TopPmtSealWidth"]             = 24.4*mm;
	m_hGeometryParameters["TopPmtSealThickness"]         = 0.5*mm;
	m_hGeometryParameters["TopPmtPhotoCathodeWidth"]     = 22.0*mm;
	m_hGeometryParameters["TopPmtPhotoCathodeThickness"] = 0.5*mm;
	m_hGeometryParameters["TopPmtBaseThickness"]         = 1.5*mm;
	m_hGeometryParameters["TopPmtToPmtBase"]             = 2.00*mm;
	m_hGeometryParameters["TopPmtTeflonSpacerThickness"] = 7.0*mm;
	m_hGeometryParameters["TopPmtXSpacing"]              = 3.174*mm;
	m_hGeometryParameters["TopPmtYSpacing"]              = 2.032*mm;

	m_hGeometryParameters["BottomPmtWindowRadius"]         = 25.25*mm;
	m_hGeometryParameters["BottomPmtWindowThickness"]      = 1.5*mm;
	m_hGeometryParameters["BottomPmtCasingRadius"]         = 27.85*mm;
	m_hGeometryParameters["BottomPmtCasingHeight"]         = 31.*mm;
	m_hGeometryParameters["BottomPmtCasingThickness"]      = 0.5*mm;
	m_hGeometryParameters["BottomPmtSealRadius"]           = 25.25*mm;
	m_hGeometryParameters["BottomPmtSealThickness"]        = 0.5*mm;
	m_hGeometryParameters["BottomPmtPhotoCathodeRadius"]    = 22.5*mm;
	m_hGeometryParameters["BottomPmtPhotoCathodeThickness"] = 0.5*mm;

	m_hGeometryParameters["BottomPmtBaseRadius"]            = 30.52*mm;
	m_hGeometryParameters["BottomPmtBaseInnerRadius"]       = 7.76*mm;
	m_hGeometryParameters["BottomPmtBaseThickness"]         = 4.7625*mm;
	m_hGeometryParameters["BottomPmtToPmtBase"]             = 10.00*mm;
	m_hGeometryParameters["BottomPmtTeflonSpacerRadius"]    = 18.82*mm;
	m_hGeometryParameters["BottomPmtTeflonSpacerThickness"] = 6.35*mm;

	// liquid scintillators
	m_hGeometryParameters["LiquidScintillatorCasingRadius"] = (12.7+1.66)*mm;
	m_hGeometryParameters["LiquidScintillatorCasingHeight"] = (26.9+1.66)*mm;
	//m_hGeometryParameters["LiquidScintillatorRadius"] = 38.1*mm;
    m_hGeometryParameters["LiquidScintillatorRadius"] = 12.7*mm;
	m_hGeometryParameters["LiquidScintillatorHeight"] = 26.9*mm; // need to get measurement

	m_hGeometryParameters["NbLiquidScintillators"] = nb_lscints;
	
	m_hGeometryParameters["LiquidScintillator0PositionX"] = 1800.0*mm;
	m_hGeometryParameters["LiquidScintillator0PositionY"] = 400.0*mm;
	m_hGeometryParameters["LiquidScintillator0PositionZ"] = 0.0*mm;
	

	m_hGeometryParameters["NbRecoilEnergies"] = 1;
	//m_hGeometryParameters["RecoilEnergy0"] = recoil_energy;
    //m_hGeometryParameters["RecoilEnergy1"] = 11.0;
    //remember to change ge detector to uncommented below

	// 60 kV
//    m_hGeometryParameters["NbRecoilEnergies"] = 1;
//    m_hGeometryParameters["RecoilEnergy0"] = 3.0;
//    m_hGeometryParameters["RecoilEnergy1"] = 5.1;
//    m_hGeometryParameters["RecoilEnergy2"] = 8.6;
//    m_hGeometryParameters["RecoilEnergy3"] = 11.0;
//    m_hGeometryParameters["RecoilEnergy4"] = 14.9;
//    m_hGeometryParameters["RecoilEnergy5"] = 55.8;

	// 80 kV
//    m_hGeometryParameters["NbRecoilEnergies"] = 4;
//    m_hGeometryParameters["RecoilEnergy0"] = 4.0;
//    m_hGeometryParameters["RecoilEnergy1"] = 6.6;
//    m_hGeometryParameters["RecoilEnergy2"] = 11.0;
//    m_hGeometryParameters["RecoilEnergy3"] = 14.9;

	/*
	m_hGeometryParameters["Recoil3keVrLiquidScintillator0PositionX"] = -391.0*mm;
	m_hGeometryParameters["Recoil3keVrLiquidScintillator0PositionY"] = -921.0*mm;
	m_hGeometryParameters["Recoil3keVrLiquidScintillator0PositionZ"] = 0.*mm;
	m_hGeometryParameters["Recoil3keVrLiquidScintillator1PositionX"] = 391.0*mm;
	m_hGeometryParameters["Recoil3keVrLiquidScintillator1PositionY"] = -921.0*mm;
	m_hGeometryParameters["Recoil3keVrLiquidScintillator1PositionZ"] = 0.*mm;
	*/
	
	
	// germanium detectors
	m_hGeometryParameters["GeDetectorCasingRadius"] = 25.4*mm;
	m_hGeometryParameters["GeDetectorCasingHeight"] = 50.8*mm;
	m_hGeometryParameters["GeDetectorRadius"] = 12.616*mm;
	m_hGeometryParameters["GeDetectorHeight"] = 10.0*mm;

	m_hGeometryParameters["NbGeDetectors"] = 0;

	m_hGeometryParameters["NbRecoilEnergies"] = 1;
	//m_hGeometryParameters["RecoilEnergy0"] = 32.0;

	m_hGeometryParameters["Recoil32keVeeGeDetector0PositionX"] = 0.*mm;
	m_hGeometryParameters["Recoil32keVeeGeDetector0PositionY"] = -620.*mm;
	m_hGeometryParameters["Recoil32keVeeGeDetector0PositionZ"] = 180.*mm;

	m_hGeometryParameters["LabToSimulationCoordinatesRotationAngleZ"] = 0.*deg;

	// lead collimators
	m_hGeometryParameters["SourceLeadCollimatorWidth"]      = 200.0*mm;
	m_hGeometryParameters["SourceLeadCollimatorHeight"]     = 100.0*mm;
	m_hGeometryParameters["SourceLeadCollimatorDepth"]      = 50.0*mm;
	m_hGeometryParameters["SourceLeadCollimatorHoleRadius"] = 1.4605*mm;
	m_hGeometryParameters["SourceLeadCollimatorPositionX"]  = 0.*mm;
//    m_hGeometryParameters["SourceLeadCollimatorPositionY"]  = 191.5*mm; // <-- real position!!
//    m_hGeometryParameters["SourceLeadCollimatorPositionY"]  = 156.5*mm; // <-- antonio question
	m_hGeometryParameters["SourceLeadCollimatorPositionY"]  = 156.5*mm; // <-- far position
	m_hGeometryParameters["SourceLeadCollimatorPositionZ"]  = 0.*mm;

	// derived quantities
	m_hGeometryParameters["CryostatFlangeToLiquidLevel"] = GetGeometryParameter("CryostatFlangeToChamberMatingFlange")
		+ 2.*GetGeometryParameter("ChamberFlangeThickness") + GetGeometryParameter("ChamberTopWallHeight")
		+ GetGeometryParameter("ChamberWallHeight") - GetGeometryParameter("ChamberBottomPlateThickness")
		- GetGeometryParameter("LXeHeight");

	// set the rotation for lab to simulation coordinates
	m_pRotationLabToSimulation->rotateZ(-GetGeometryParameter("LabToSimulationCoordinatesRotationAngleZ"));
}

void
NerixDetectorConstruction::ConstructLaboratory()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< laboratory >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dLabHalfX = 5.0*m; //originally all 1.5
	const G4double dLabHalfY = 5.0*m;
	const G4double dLabHalfZ = 5.0*m;

	G4Box *pLabBox = new G4Box("LabBox", dLabHalfX, dLabHalfY, dLabHalfZ);
	
	G4Material *Air = G4Material::GetMaterial("G4_AIR");

	m_pLabLogicalVolume = new G4LogicalVolume(pLabBox, Air, "LabLogicalVolume", 0, 0, 0);

	m_pLabPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), m_pLabLogicalVolume, "Lab", 0, false, 0);

	m_pLabLogicalVolume->SetVisAttributes(G4VisAttributes::Invisible);

	m_pMotherLogicalVolume = m_pLabLogicalVolume;
}

void
NerixDetectorConstruction::ConstructSource()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< source >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dSourcePositionX = GetGeometryParameter("SourcePositionX");
	const G4double dSourcePositionY = GetGeometryParameter("SourcePositionY");
	const G4double dSourcePositionZ = GetGeometryParameter("SourcePositionZ");

	const G4double dSourceRadius = GetGeometryParameter("SourceRadius");
	const G4double dSourceLength = GetGeometryParameter("SourceLength");

	const G4double dLabToSimulationCoordinatesRotationAngleZ = GetGeometryParameter("LabToSimulationCoordinatesRotationAngleZ");

	G4Material *Vacuum = G4Material::GetMaterial("Vacuum");	

	//=================================== source ====================================
	const G4double dSourceHalfZ = 0.5*dSourceLength;

	G4Tubs *pSourceTube = new G4Tubs("SourceTube", 0., dSourceRadius, dSourceHalfZ, 0.*deg, 360.*deg);

	m_pSourceLogicalVolume = new G4LogicalVolume(pSourceTube, Vacuum, "SourceLogicalVolume", 0, 0, 0);

	G4ThreeVector hSourcePos(dSourcePositionX, dSourcePositionY, dSourcePositionZ);
	hSourcePos.rotateZ(dLabToSimulationCoordinatesRotationAngleZ);

	G4RotationMatrix *pRotation = new G4RotationMatrix();
	pRotation->rotateZ(-dLabToSimulationCoordinatesRotationAngleZ);
	pRotation->rotateX(90.*deg);

	m_pSourcePhysicalVolume = new G4PVPlacement(pRotation, hSourcePos,
		m_pSourceLogicalVolume, "Source", m_pMotherLogicalVolume, false, 0);

	//================================== attributes =================================
	G4Colour hVacuumColor(1.000, 1.000, 1.000);

	G4VisAttributes *pSourceVisAtt = new G4VisAttributes(hVacuumColor);
	pSourceVisAtt->SetVisibility(true);
	m_pSourceLogicalVolume->SetVisAttributes(pSourceVisAtt);
}

void
NerixDetectorConstruction::ConstructMinitron()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< minitron >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dFloorToCryostatFlange = GetGeometryParameter("FloorToCryostatFlange");
	const G4double dCryostatFlangeToLiquidLevel = GetGeometryParameter("CryostatFlangeToLiquidLevel");

	const G4double dMinitronPositionX = GetGeometryParameter("MinitronPositionX");
	const G4double dMinitronPositionY = GetGeometryParameter("MinitronPositionY");

	const G4double dMinitronBaseWidth = GetGeometryParameter("MinitronBaseWidth");
	const G4double dMinitronBaseLength = GetGeometryParameter("MinitronBaseLength");
	const G4double dMinitronBaseHeight = GetGeometryParameter("MinitronBaseHeight");
	const G4double dMinitronBaseToFloor = GetGeometryParameter("MinitronBaseToFloor");
	const G4double dMinitronBasePositionX = GetGeometryParameter("MinitronBasePositionX");
	const G4double dMinitronBasePositionY = GetGeometryParameter("MinitronBasePositionY");

	const G4double dMinitronTubeRadius = GetGeometryParameter("MinitronTubeRadius");
	const G4double dMinitronTubeHeight = GetGeometryParameter("MinitronTubeHeight");
	const G4double dMinitronTubeThickness = GetGeometryParameter("MinitronTubeThickness");

	const G4double dMinitronHolderRadius = GetGeometryParameter("MinitronHolderRadius");
	const G4double dMinitronHolderHeight = GetGeometryParameter("MinitronHolderHeight");
	const G4double dMinitronHolderCutRadius = GetGeometryParameter("MinitronHolderCutRadius");
	const G4double dMinitronHolderCutDepth = GetGeometryParameter("MinitronHolderCutDepth");
	const G4double dMinitronHolderBottomToTubeTop = GetGeometryParameter("MinitronHolderBottomToTubeTop");

	const G4double dMinitronVacuumRadius = GetGeometryParameter("MinitronVacuumRadius");
	const G4double dMinitronVacuumHeight = GetGeometryParameter("MinitronVacuumHeight");

	const G4double dLabToSimulationCoordinatesRotationAngleZ = GetGeometryParameter("LabToSimulationCoordinatesRotationAngleZ");

	G4Material *Wood = G4Material::GetMaterial("Wood");
	G4Material *SS304LSteel = G4Material::GetMaterial("SS304LSteel");
	G4Material *MineralOil = G4Material::GetMaterial("MineralOil");
	G4Material *Teflon = G4Material::GetMaterial("Teflon");	
	G4Material *Vacuum = G4Material::GetMaterial("Vacuum");	

	//===================================== base ====================================
	const G4double dMinitronBaseHalfX = 0.5*dMinitronBaseLength;
	const G4double dMinitronBaseHalfY = 0.5*dMinitronBaseWidth;
	const G4double dMinitronBaseHalfZ = 0.5*dMinitronBaseHeight;
	const G4double dMinitronBaseOffsetZ = dCryostatFlangeToLiquidLevel-dFloorToCryostatFlange+dMinitronBaseToFloor+dMinitronBaseHalfZ;

	G4Box *pMinitronBaseBox = new G4Box("MinitronBaseBox", dMinitronBaseHalfX, dMinitronBaseHalfY, dMinitronBaseHalfZ);

	m_pMinitronBaseLogicalVolume = new G4LogicalVolume(pMinitronBaseBox, Wood,
		"MinitronBaseLogicalVolume", 0, 0, 0);

	G4ThreeVector hMinitronBasePos(dMinitronBasePositionX, dMinitronBasePositionY, dMinitronBaseOffsetZ);
	hMinitronBasePos.rotateZ(dLabToSimulationCoordinatesRotationAngleZ);

	m_pMinitronBasePhysicalVolume = new G4PVPlacement(m_pRotationLabToSimulation, hMinitronBasePos,
		m_pMinitronBaseLogicalVolume, "MinitronBase", m_pMotherLogicalVolume, false, 0);

	//===================================== tube ====================================
	const G4double dMinitronTubeHalfZ = 0.5*dMinitronTubeHeight;
	const G4double dMinitronTubeOffsetZ = dMinitronBaseOffsetZ+dMinitronBaseHalfZ+dMinitronTubeHalfZ;

	G4Tubs *pMinitronTubeTube = new G4Tubs("MinitronTubeTube", 0.,
		dMinitronTubeRadius, dMinitronTubeHalfZ, 0.*deg, 360.*deg);

	m_pMinitronTubeLogicalVolume = new G4LogicalVolume(pMinitronTubeTube, SS304LSteel,
		"MinitronTubeLogicalVolume", 0, 0, 0);

	G4ThreeVector hMinitronTubePos(dMinitronPositionX, dMinitronPositionY, dMinitronTubeOffsetZ);
	hMinitronTubePos.rotateZ(dLabToSimulationCoordinatesRotationAngleZ);

	m_pMinitronTubePhysicalVolume = new G4PVPlacement(m_pRotationLabToSimulation, hMinitronTubePos,
		m_pMinitronTubeLogicalVolume, "MinitronTube", m_pMotherLogicalVolume, false, 0);

	//================================== insulation =================================
	const G4double dMinitronInsulationRadius = dMinitronTubeRadius-dMinitronTubeThickness;
	const G4double dMinitronInsulationHalfZ = 0.5*(dMinitronTubeHeight-dMinitronTubeThickness);
	const G4double dMinitronInsulationOffsetZ = 0.5*dMinitronTubeThickness;

	G4Tubs *pMinitronInsulationTube = new G4Tubs("MinitronInsulationTube", 0.,
		dMinitronInsulationRadius, dMinitronInsulationHalfZ, 0.*deg, 360.*deg);

	m_pMinitronInsulationLogicalVolume = new G4LogicalVolume(pMinitronInsulationTube, MineralOil,
		"MinitronInsulationLogicalVolume", 0, 0, 0);

	m_pMinitronInsulationPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dMinitronInsulationOffsetZ),
		m_pMinitronInsulationLogicalVolume, "MinitronInsulation", m_pMinitronTubeLogicalVolume, false, 0);

	//==================================== holder ===================================
	const G4double dMinitronHolderHalfZ = 0.5*dMinitronHolderHeight;
	const G4double dMinitronHolderOffsetZ = dMinitronInsulationHalfZ+dMinitronHolderHalfZ-dMinitronHolderBottomToTubeTop;

	const G4double dMinitronHolderCutHalfZ = dMinitronHolderCutDepth;
	const G4double dMinitronHolderCutOffsetZ = -dMinitronHolderHalfZ;

	G4Tubs *pMinitronHolderTube = new G4Tubs("MinitronHolderTube", 0.,
		dMinitronHolderRadius, dMinitronHolderHalfZ, 0.*deg, 360.*deg);

	G4Tubs *pMinitronHolderCutTube = new G4Tubs("MinitronHolderCutTube", 0.,
		dMinitronHolderCutRadius, dMinitronHolderCutHalfZ, 0.*deg, 360.*deg);

	G4SubtractionSolid *pMinitronHolderWithCut = new G4SubtractionSolid("MinitronHolderWithCut",
		pMinitronHolderTube, pMinitronHolderCutTube, 0, G4ThreeVector(0., 0., dMinitronHolderCutOffsetZ));

	m_pMinitronHolderLogicalVolume = new G4LogicalVolume(pMinitronHolderWithCut, Teflon,
		"MinitronHolderLogicalVolume", 0, 0, 0);

	m_pMinitronHolderPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dMinitronHolderOffsetZ),
		m_pMinitronHolderLogicalVolume, "MinitronHolder", m_pMinitronInsulationLogicalVolume, false, 0);

	//==================================== vacuum ===================================
	const G4double dMinitronVacuumHalfZ = 0.5*dMinitronVacuumHeight;
	const G4double dMinitronVacuumOffsetZ = dMinitronInsulationHalfZ+dMinitronVacuumHalfZ-dMinitronHolderBottomToTubeTop;

	G4Tubs *pMinitronVacuumTube = new G4Tubs("MinitronVacuumTube", 0.,
		dMinitronVacuumRadius, dMinitronVacuumHalfZ, 0.*deg, 360.*deg);

	m_pMinitronVacuumLogicalVolume = new G4LogicalVolume(pMinitronVacuumTube, Vacuum,
		"MinitronVacuumLogicalVolume", 0, 0, 0);

	m_pMinitronVacuumPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dMinitronVacuumOffsetZ),
		m_pMinitronVacuumLogicalVolume, "MinitronVacuum", m_pMinitronInsulationLogicalVolume, false, 0);

	//================================== attributes =================================
	G4Colour hWoodColor(0.824, 0.680, 0.051);
	G4Colour hStainlessSteelColor(0.500, 0.500, 0.500, 0.1);
	G4Colour hMineralOilColor(0., 0., 0.);
	G4Colour hTeflonColor(1., 1., 1., 1.);
	G4Colour hVacuumColor(0.500, 0., 0.);

	G4VisAttributes *pMinitronBaseVisAtt = new G4VisAttributes(hWoodColor);
	pMinitronBaseVisAtt->SetVisibility(true);
	m_pMinitronBaseLogicalVolume->SetVisAttributes(pMinitronBaseVisAtt);

	G4VisAttributes *pMinitronTubeVisAtt = new G4VisAttributes(hStainlessSteelColor);
	pMinitronTubeVisAtt->SetVisibility(true);
	m_pMinitronTubeLogicalVolume->SetVisAttributes(pMinitronTubeVisAtt);

	G4VisAttributes *pMinitronInsulationVisAtt = new G4VisAttributes(hMineralOilColor);
	pMinitronInsulationVisAtt->SetVisibility(true);
	m_pMinitronInsulationLogicalVolume->SetVisAttributes(pMinitronInsulationVisAtt);

	G4VisAttributes *pMinitronHolderVisAtt = new G4VisAttributes(hTeflonColor);
	pMinitronHolderVisAtt->SetVisibility(true);
	m_pMinitronHolderLogicalVolume->SetVisAttributes(pMinitronHolderVisAtt);

	G4VisAttributes *pMinitronVacuumVisAtt = new G4VisAttributes(hVacuumColor);
	pMinitronVacuumVisAtt->SetVisibility(true);
	m_pMinitronVacuumLogicalVolume->SetVisAttributes(pMinitronVacuumVisAtt);
}

void
NerixDetectorConstruction::ConstructFrame()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< frame >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dCryostatFlangeToLiquidLevel = GetGeometryParameter("CryostatFlangeToLiquidLevel");

	const G4double dFrameWidth = GetGeometryParameter("FrameWidth");
	const G4double dFrameHeight = GetGeometryParameter("FrameHeight");
	const G4double dFrameThickness = GetGeometryParameter("FrameThickness");
	const G4double dFrameCutWidth = GetGeometryParameter("FrameCutWidth");
	const G4double dFrameCutHeight = GetGeometryParameter("FrameCutHeight");
	const G4double dFrameTableThickness = GetGeometryParameter("FrameTableThickness");
	const G4double dFrameTableToCryostatFlange = GetGeometryParameter("FrameTableToCryostatFlange");

	const G4double dLabToSimulationCoordinatesRotationAngleZ = GetGeometryParameter("LabToSimulationCoordinatesRotationAngleZ");

	G4Material *Aluminium = G4Material::GetMaterial("G4_Al");

	//==================================== frame ====================================
	const G4double dFrameTableHalfX = 0.5*dFrameWidth;
	const G4double dFrameTableHalfY = dFrameTableHalfX;
	const G4double dFrameTableHalfZ = 0.5*dFrameTableThickness;

	const G4double dFrameTableCutRadius = 0.5*(dFrameCutWidth);

	const G4double dFrameTableOffsetZ = dCryostatFlangeToLiquidLevel+dFrameTableToCryostatFlange;

	const G4double dFrameUpDownSideHalfX = 0.5*dFrameWidth;
	const G4double dFrameUpDownSideHalfY = dFrameUpDownSideHalfX;
	const G4double dFrameUpDownSideHalfZ = 0.5*dFrameThickness;

	const G4double dFrameUpDownSideCutHalfX = 0.5*(dFrameCutWidth);
	const G4double dFrameUpDownSideCutHalfY = dFrameUpDownSideCutHalfX;
	const G4double dFrameUpDownSideCutHalfZ = dFrameThickness;

	const G4double dFrameUpSideOffsetZ = dFrameTableOffsetZ-dFrameTableHalfZ-0.5*dFrameThickness;
	const G4double dFrameDownSideOffsetZ = dFrameUpSideOffsetZ-dFrameHeight+dFrameThickness;

	const G4double dFrameLeftRightSideHalfX = 0.5*dFrameThickness;
	const G4double dFrameLeftRightSideHalfY = 0.5*dFrameWidth;
	const G4double dFrameLeftRightSideHalfZ = 0.5*(dFrameHeight-2.*dFrameThickness);

	const G4double dFrameLeftRightSideCutHalfX = dFrameThickness;
	const G4double dFrameLeftRightSideCutHalfY = 0.5*dFrameCutWidth;
	const G4double dFrameLeftRightSideCutHalfZ = 0.5*dFrameCutHeight;

	const G4double dFrameLeftSideOffsetX = -0.5*(dFrameWidth-dFrameThickness);
	const G4double dFrameRightSideOffsetX = -dFrameLeftSideOffsetX;

	const G4double dFrameLeftRightSideOffsetZ = dFrameUpSideOffsetZ-dFrameUpDownSideHalfZ-dFrameLeftRightSideHalfZ;

	const G4double dFrameFrontBackSideHalfX = 0.5*(dFrameWidth-2.*dFrameThickness);
	const G4double dFrameFrontBackSideHalfY = 0.5*dFrameThickness;
	const G4double dFrameFrontBackSideHalfZ = 0.5*(dFrameHeight-2.*dFrameThickness);

	const G4double dFrameFrontBackSideCutHalfX = 0.5*dFrameCutWidth;
	const G4double dFrameFrontBackSideCutHalfY = dFrameThickness;
	const G4double dFrameFrontBackSideCutHalfZ = 0.5*dFrameCutHeight;

	const G4double dFrameFrontSideOffsetY = -0.5*(dFrameWidth-dFrameThickness);
	const G4double dFrameBackSideOffsetY = -dFrameFrontSideOffsetY;

	const G4double dFrameFrontBackSideOffsetZ = dFrameUpSideOffsetZ-dFrameUpDownSideHalfZ-dFrameFrontBackSideHalfZ;

	//------------------------------------ table ------------------------------------
	G4Box *pFrameTableBox = new G4Box("FrameTableBox", dFrameTableHalfX, dFrameTableHalfY, dFrameTableHalfZ);

	G4Tubs *pFrameTableCutTube = new G4Tubs("FrameTableCutTube", 0.,
		dFrameTableCutRadius, dFrameTableThickness, 0.*deg, 360.*deg);

	G4SubtractionSolid *pFrameTableWithCut = new G4SubtractionSolid("FrameTableWithCut",
		pFrameTableBox, pFrameTableCutTube);

	m_pFrameTableLogicalVolume = new G4LogicalVolume(pFrameTableWithCut, Aluminium,
		"FrameTableLogicalVolume", 0, 0, 0);

	m_pFrameTablePhysicalVolume = new G4PVPlacement(m_pRotationLabToSimulation, G4ThreeVector(0., 0., dFrameTableOffsetZ),
		m_pFrameTableLogicalVolume, "FrameTable", m_pMotherLogicalVolume, false, 0);

	//--------------------------------- up/down side --------------------------------
	G4Box *pFrameUpDownSideBox = new G4Box("FrameUpDownSideBox",
		dFrameUpDownSideHalfX, dFrameUpDownSideHalfY, dFrameUpDownSideHalfZ);

	G4Box *pFrameUpDownSideCutBox = new G4Box("FrameUpDownSideCutBox",
		dFrameUpDownSideCutHalfX, dFrameUpDownSideCutHalfY, dFrameUpDownSideCutHalfZ);

	G4SubtractionSolid *pFrameUpDownSideWithCut = new G4SubtractionSolid("FrameUpDownSideWithCut",
		pFrameUpDownSideBox, pFrameUpDownSideCutBox);

	m_pFrameUpDownSideLogicalVolume = new G4LogicalVolume(pFrameUpDownSideWithCut, Aluminium,
		"FrameUpDownSideLogicalVolume", 0, 0, 0);

	m_hFrameSidePhysicalVolumes.push_back(new G4PVPlacement(m_pRotationLabToSimulation, G4ThreeVector(0., 0., dFrameUpSideOffsetZ),
		m_pFrameUpDownSideLogicalVolume, "FrameUpSide", m_pMotherLogicalVolume, false, 0));

	m_hFrameSidePhysicalVolumes.push_back(new G4PVPlacement(m_pRotationLabToSimulation, G4ThreeVector(0., 0., dFrameDownSideOffsetZ),
		m_pFrameUpDownSideLogicalVolume, "FrameDownSide", m_pMotherLogicalVolume, false, 0));

	//------------------------------- left/right side -------------------------------
	G4Box *pFrameLeftRightSideBox = new G4Box("FrameLeftRightSideBox",
		dFrameLeftRightSideHalfX, dFrameLeftRightSideHalfY, dFrameLeftRightSideHalfZ);

	G4Box *pFrameLeftRightSideCutBox = new G4Box("FrameLeftRightSideCutBox",
		dFrameLeftRightSideCutHalfX, dFrameLeftRightSideCutHalfY, dFrameLeftRightSideCutHalfZ);

	G4SubtractionSolid *pFrameLeftRightSideWithCut = new G4SubtractionSolid("FrameLeftRightSideWithCut",
		pFrameLeftRightSideBox, pFrameLeftRightSideCutBox);

	m_pFrameLeftRightSideLogicalVolume = new G4LogicalVolume(pFrameLeftRightSideWithCut, Aluminium,
		"FrameLeftRightSideLogicalVolume", 0, 0, 0);

	G4ThreeVector hFrameLeftSidePos(dFrameLeftSideOffsetX, 0., dFrameLeftRightSideOffsetZ);
	hFrameLeftSidePos.rotateZ(dLabToSimulationCoordinatesRotationAngleZ);

	G4ThreeVector hFrameRightSidePos(dFrameRightSideOffsetX, 0., dFrameLeftRightSideOffsetZ);
	hFrameRightSidePos.rotateZ(dLabToSimulationCoordinatesRotationAngleZ);

	m_hFrameSidePhysicalVolumes.push_back(new G4PVPlacement(m_pRotationLabToSimulation,
		hFrameLeftSidePos, m_pFrameLeftRightSideLogicalVolume, "FrameLeftSide", m_pMotherLogicalVolume, false, 0));

	m_hFrameSidePhysicalVolumes.push_back(new G4PVPlacement(m_pRotationLabToSimulation,
		hFrameRightSidePos, m_pFrameLeftRightSideLogicalVolume, "FrameRightSide", m_pMotherLogicalVolume, false, 0));

	//------------------------------- front/back side -------------------------------
	G4Box *pFrameFrontBackSideBox = new G4Box("FrameFrontBackSideBox",
		dFrameFrontBackSideHalfX, dFrameFrontBackSideHalfY, dFrameFrontBackSideHalfZ);

	G4Box *pFrameFrontBackSideCutBox = new G4Box("FrameFrontBackSideCutBox",
		dFrameFrontBackSideCutHalfX, dFrameFrontBackSideCutHalfY, dFrameFrontBackSideCutHalfZ);

	G4SubtractionSolid *pFrameFrontBackSideWithCut = new G4SubtractionSolid("FrameFrontBackSideWithCut",
		pFrameFrontBackSideBox, pFrameFrontBackSideCutBox);

	m_pFrameFrontBackSideLogicalVolume = new G4LogicalVolume(pFrameFrontBackSideWithCut, Aluminium,
		"FrameFrontBackSideLogicalVolume", 0, 0, 0);

	G4ThreeVector hFrameFrontSidePos(0., dFrameFrontSideOffsetY, dFrameFrontBackSideOffsetZ);
	hFrameFrontSidePos.rotateZ(dLabToSimulationCoordinatesRotationAngleZ);

	G4ThreeVector hFrameBackSidePos(0., dFrameBackSideOffsetY, dFrameFrontBackSideOffsetZ);
	hFrameBackSidePos.rotateZ(dLabToSimulationCoordinatesRotationAngleZ);

	m_hFrameSidePhysicalVolumes.push_back(new G4PVPlacement(m_pRotationLabToSimulation,
		hFrameFrontSidePos, m_pFrameFrontBackSideLogicalVolume, "FrameFrontSide", m_pMotherLogicalVolume, false, 0));

	m_hFrameSidePhysicalVolumes.push_back(new G4PVPlacement(m_pRotationLabToSimulation,
		hFrameBackSidePos, m_pFrameFrontBackSideLogicalVolume, "FrameBackSide", m_pMotherLogicalVolume, false, 0));

	//================================== attributes =================================
	G4Colour hAluminiumColor(0.750, 0.750, 0.750);

	G4VisAttributes *pFrameVisAtt = new G4VisAttributes(hAluminiumColor);
	pFrameVisAtt->SetVisibility(true);
	m_pFrameTableLogicalVolume->SetVisAttributes(pFrameVisAtt);
	m_pFrameUpDownSideLogicalVolume->SetVisAttributes(pFrameVisAtt);
	m_pFrameLeftRightSideLogicalVolume->SetVisAttributes(pFrameVisAtt);
	m_pFrameFrontBackSideLogicalVolume->SetVisAttributes(pFrameVisAtt);
}

void
NerixDetectorConstruction::ConstructCryostat()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< cryostat >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dCryostatFlangeRadius = GetGeometryParameter("CryostatFlangeRadius");
	const G4double dCryostatFlangeThickness = GetGeometryParameter("CryostatFlangeThickness");
	const G4double dCryostatWallRadius = GetGeometryParameter("CryostatWallRadius");
	const G4double dCryostatWallHeight = GetGeometryParameter("CryostatWallHeight");
	const G4double dCryostatWallThickness = GetGeometryParameter("CryostatWallThickness");
	const G4double dCryostatFlangeGasketThickness = GetGeometryParameter("CryostatFlangeGasketThickness");
	const G4double dCryostatFlangeToLiquidLevel = GetGeometryParameter("CryostatFlangeToLiquidLevel");

	G4Material *SS304LSteel = G4Material::GetMaterial("SS304LSteel");
	G4Material *Aluminium = G4Material::GetMaterial("G4_Al");
	G4Material *Vacuum = G4Material::GetMaterial("Vacuum");

	//================================= top flange ==================================
	const G4double dCryostatTopFlangeRadius = dCryostatFlangeRadius;
	const G4double dCryostatTopFlangeHalfZ = 0.5*dCryostatFlangeThickness;
	const G4double dCryostatTopFlangeOffsetZ = dCryostatFlangeToLiquidLevel+dCryostatTopFlangeHalfZ;

	G4Tubs *pCryostatTopFlangeTube = new G4Tubs("CryostatTopFlangeTube", 0.,
		dCryostatTopFlangeRadius, dCryostatTopFlangeHalfZ, 0.*deg, 360.*deg);

	m_pCryostatTopFlangeLogicalVolume = new G4LogicalVolume(pCryostatTopFlangeTube, SS304LSteel,
		"CryostatTopFlangeLogicalVolume", 0, 0, 0);

	m_pCryostatTopFlangePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dCryostatTopFlangeOffsetZ),
		m_pCryostatTopFlangeLogicalVolume, "CryostatTopFlange", m_pMotherLogicalVolume, false, 0);

	//================================== side wall ==================================
	const G4double dCryostatWallOffsetZ = dCryostatTopFlangeOffsetZ-dCryostatTopFlangeHalfZ-dCryostatFlangeGasketThickness-dCryostatWallHeight;
	const G4double dCryostatWallFlangeRadius = dCryostatFlangeRadius;
	const G4double dCryostatWallFlangeThickness = dCryostatFlangeThickness;

	const G4int iNbZPlanes = 6;

	const G4double pdZPlanes[]
		= {0, dCryostatWallFlangeThickness, dCryostatWallFlangeThickness,
			dCryostatWallHeight-dCryostatWallFlangeThickness,
			dCryostatWallHeight-dCryostatWallFlangeThickness,
			dCryostatWallHeight};

	const G4double pdInnerRadii[]
		= {dCryostatWallRadius, dCryostatWallRadius, dCryostatWallRadius,
			dCryostatWallRadius, dCryostatWallRadius, dCryostatWallRadius};

	const G4double pdOuterRadii[]
		= {dCryostatWallFlangeRadius, dCryostatWallFlangeRadius,
			dCryostatWallRadius+dCryostatWallThickness, dCryostatWallRadius+dCryostatWallThickness,
			dCryostatWallFlangeRadius, dCryostatWallFlangeRadius};

	G4Polycone *pCryostatWallPolycone = new G4Polycone("CryostatWallPolycone",
			0.*deg, 360.*deg, iNbZPlanes, pdZPlanes, pdInnerRadii, pdOuterRadii);

	m_pCryostatWallLogicalVolume = new G4LogicalVolume(pCryostatWallPolycone, SS304LSteel,
		"CryostatWallLogicalVolume", 0, 0, 0);

	m_pCryostatWallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dCryostatWallOffsetZ),
		m_pCryostatWallLogicalVolume, "CryostatWall", m_pMotherLogicalVolume, false, 0);

	//================================ bottom flange ================================
	const G4double dCryostatBottomFlangeRadius = dCryostatFlangeRadius;
	const G4double dCryostatBottomFlangeHalfZ = 0.5*dCryostatFlangeThickness;
	const G4double dCryostatBottomFlangeOffsetZ = dCryostatTopFlangeOffsetZ-dCryostatTopFlangeHalfZ-2.*dCryostatFlangeGasketThickness-dCryostatWallHeight-dCryostatBottomFlangeHalfZ; // ???

	G4Tubs *pCryostatBottomFlangeTube = new G4Tubs("CryostatBottomFlangeTube", 0.,
		dCryostatBottomFlangeRadius, dCryostatBottomFlangeHalfZ, 0.*deg, 360.*deg);

	m_pCryostatBottomFlangeLogicalVolume = new G4LogicalVolume(pCryostatBottomFlangeTube, SS304LSteel,
		"CryostatBottomFlangeLogicalVolume", 0, 0, 0);

	m_pCryostatBottomFlangePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dCryostatBottomFlangeOffsetZ),
		m_pCryostatBottomFlangeLogicalVolume, "CryostatBottomFlange", m_pMotherLogicalVolume, false, 0);

	//=================================== gaskets ===================================
	const G4double dCryostatFlangeGasketRadius = dCryostatFlangeRadius;
	const G4double dCryostatFlangeGasketHalfZ = 0.5*dCryostatFlangeGasketThickness;
	const G4double dCryostatTopFlangeGasketOffsetZ = dCryostatTopFlangeOffsetZ-dCryostatTopFlangeHalfZ-dCryostatFlangeGasketHalfZ;
	const G4double dCryostatBottomFlangeGasketOffsetZ = dCryostatWallOffsetZ-dCryostatFlangeGasketHalfZ;

	G4Tubs *pCryostatFlangeGasketTube = new G4Tubs("CryostatFlangeGasketTube", dCryostatWallRadius,
		dCryostatFlangeGasketRadius, dCryostatFlangeGasketHalfZ, 0.*deg, 360.*deg);

	m_pCryostatFlangeGasketLogicalVolume = new G4LogicalVolume(pCryostatFlangeGasketTube, Aluminium,
		"CryostatFlangeGasketLogicalVolume", 0, 0, 0);

	m_hCryostatFlangeGasketPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(0., 0., dCryostatTopFlangeGasketOffsetZ),
		m_pCryostatFlangeGasketLogicalVolume, "CryostatTopFlangeGasket", m_pMotherLogicalVolume, false, 0));

	m_hCryostatFlangeGasketPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(0., 0., dCryostatBottomFlangeGasketOffsetZ),
		m_pCryostatFlangeGasketLogicalVolume, "CryostatBottomFlangeGasket", m_pMotherLogicalVolume, false, 1));
	
	//==================================== vacuum ===================================
	const G4double dCryostatVacuumRadius = dCryostatWallRadius;
	const G4double dCryostatVacuumHalfZ = 0.5*(dCryostatWallHeight+2.*dCryostatFlangeGasketThickness);
	const G4double dCryostatVacuumOffsetZ = dCryostatTopFlangeOffsetZ-dCryostatTopFlangeHalfZ-dCryostatVacuumHalfZ;

	G4Tubs *pCryostatVacuumTube = new G4Tubs("CryostatVacuumTube", 0.,
		dCryostatVacuumRadius, dCryostatVacuumHalfZ, 0.*deg, 360.*deg);

	m_pCryostatVacuumLogicalVolume = new G4LogicalVolume(pCryostatVacuumTube, Vacuum,
		"CryostatVacuumLogicalVolume", 0, 0, 0);

	m_pCryostatVacuumPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dCryostatVacuumOffsetZ),
		m_pCryostatVacuumLogicalVolume, "CryostatVacuum", m_pMotherLogicalVolume, false, 0);

	//================================== attributes =================================
	G4Colour hStainlessSteelColor(0.500, 0.500, 0.500, 0.1);
	G4Colour hAluminiumColor(0.750, 0.750, 0.750);

	G4VisAttributes *pCryostatVisAtt = new G4VisAttributes(hStainlessSteelColor);
	pCryostatVisAtt->SetVisibility(true);
	m_pCryostatTopFlangeLogicalVolume->SetVisAttributes(pCryostatVisAtt);
	m_pCryostatWallLogicalVolume->SetVisAttributes(pCryostatVisAtt);
	m_pCryostatBottomFlangeLogicalVolume->SetVisAttributes(pCryostatVisAtt);

	G4VisAttributes *pCryostatFlangeGasketVisAtt = new G4VisAttributes(hAluminiumColor);
	pCryostatFlangeGasketVisAtt->SetVisibility(true);
	m_pCryostatFlangeGasketLogicalVolume->SetVisAttributes(pCryostatFlangeGasketVisAtt);

	m_pCryostatVacuumLogicalVolume->SetVisAttributes(G4VisAttributes::Invisible);
}

void
NerixDetectorConstruction::ConstructChamber()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< chamber >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dCryostatFlangeToChamberMatingFlange = GetGeometryParameter("CryostatFlangeToChamberMatingFlange");
	const G4double dCryostatWallHeight = GetGeometryParameter("CryostatWallHeight");
	const G4double dCryostatFlangeGasketThickness = GetGeometryParameter("CryostatFlangeGasketThickness");

	const G4double dChamberWallRadius = GetGeometryParameter("ChamberWallRadius");
	const G4double dChamberWallHeight = GetGeometryParameter("ChamberWallHeight");
	const G4double dChamberWallThickness = GetGeometryParameter("ChamberWallThickness");

	const G4double dChamberWallBulgeRadius = GetGeometryParameter("ChamberWallBulgeRadius");
	const G4double dChamberWallBulgeHeight = GetGeometryParameter("ChamberWallBulgeHeight");
	const G4double dChamberWallBulgeThickness = GetGeometryParameter("ChamberWallBulgeThickness");
	const G4double dChamberWallBulgeRotationAngleZ = GetGeometryParameter("ChamberWallBulgeRotationAngleZ");

	const G4double dChamberBufferWidth = GetGeometryParameter("ChamberBufferWidth");
	const G4double dChamberBufferLength = GetGeometryParameter("ChamberBufferLength");
	const G4double dChamberBufferHeight = GetGeometryParameter("ChamberBufferHeight");
	const G4double dChamberBufferStepHeight = GetGeometryParameter("ChamberBufferStepHeight");
	const G4double dChamberBufferStepLength = GetGeometryParameter("ChamberBufferStepLength");
	const G4double dChamberBufferThickness = GetGeometryParameter("ChamberBufferThickness");

	const G4double dChamberFlangeRadius = GetGeometryParameter("ChamberFlangeRadius");
	const G4double dChamberFlangeThickness = GetGeometryParameter("ChamberFlangeThickness");

	const G4double dChamberTopWallRadius = GetGeometryParameter("ChamberTopWallRadius");
	const G4double dChamberTopWallHeight = GetGeometryParameter("ChamberTopWallHeight");
	const G4double dChamberTopWallThickness = GetGeometryParameter("ChamberTopWallThickness");

	const G4double dChamberMiddlePlateThickness = GetGeometryParameter("ChamberMiddlePlateThickness");
	const G4double dChamberBottomPlateThickness = GetGeometryParameter("ChamberBottomPlateThickness");

	G4Material *SS304LSteel = G4Material::GetMaterial("SS304LSteel");

	const G4double dCryostatVacuumHalfZ = 0.5*(dCryostatWallHeight+2.*dCryostatFlangeGasketThickness);

	//================================ mating flange ================================
	const G4double dChamberMatingFlangeRadius = dChamberFlangeRadius;
	const G4double dChamberMatingFlangeHalfZ = 0.5*dChamberFlangeThickness;
	const G4double dChamberMatingFlangeOffsetZ = dCryostatVacuumHalfZ-dCryostatFlangeToChamberMatingFlange-dChamberMatingFlangeHalfZ;

	G4Tubs *pChamberMatingFlangeTube = new G4Tubs("ChamberMatingFlangeTube", 0.,
		dChamberMatingFlangeRadius, dChamberMatingFlangeHalfZ, 0.*deg, 360.*deg);

	m_pChamberMatingFlangeLogicalVolume = new G4LogicalVolume(pChamberMatingFlangeTube, SS304LSteel,
		"ChamberMatingFlangeLogicalVolume", 0, 0, 0);

	m_pChamberMatingFlangePhysicalVolume = new G4PVPlacement(m_pRotationLabToSimulation, G4ThreeVector(0., 0., dChamberMatingFlangeOffsetZ),
		m_pChamberMatingFlangeLogicalVolume, "ChamberMatingFlange", m_pCryostatVacuumLogicalVolume, false, 0);

	//================================= top flange ==================================
	const G4double dChamberTopFlangeRadius = dChamberFlangeRadius;
	const G4double dChamberTopFlangeHalfZ = 0.5*dChamberFlangeThickness;
	const G4double dChamberTopFlangeOffsetZ = dChamberMatingFlangeOffsetZ-dChamberMatingFlangeHalfZ-dChamberTopFlangeHalfZ;

	G4Tubs *pChamberTopFlangeTube = new G4Tubs("ChamberTopFlangeTube", dChamberTopWallRadius,
		dChamberTopFlangeRadius, dChamberTopFlangeHalfZ, 0.*deg, 360.*deg);

	m_pChamberTopFlangeLogicalVolume = new G4LogicalVolume(pChamberTopFlangeTube, SS304LSteel,
		"ChamberTopFlangeLogicalVolume", 0, 0, 0);

	m_pChamberTopFlangePhysicalVolume = new G4PVPlacement(m_pRotationLabToSimulation, G4ThreeVector(0., 0., dChamberTopFlangeOffsetZ),
		m_pChamberTopFlangeLogicalVolume, "ChamberTopFlange", m_pCryostatVacuumLogicalVolume, false, 0);
	
	//================================== top wall ===================================
	const G4double dChamberTopWallHalfZ = 0.5*dChamberTopWallHeight;
	const G4double dChamberTopWallOffsetZ = dChamberTopFlangeOffsetZ-dChamberTopFlangeHalfZ-dChamberTopWallHalfZ;

	const G4double dChamberTopWallBufferCutHalfX = dChamberTopWallRadius;
	const G4double dChamberTopWallBufferCutHalfY = 0.5*dChamberBufferWidth;
	const G4double dChamberTopWallBufferCutHalfZ = dChamberBufferStepHeight;

	G4Tubs *pChamberTopWallTube = new G4Tubs("ChamberTopWallTube", dChamberTopWallRadius,
		dChamberTopWallRadius+dChamberTopWallThickness, dChamberTopWallHalfZ, 0.*deg, 360.*deg);

	G4Box *pChamberTopWallBufferCutBox = new G4Box("ChamberTopWallBufferCutBox",
		dChamberTopWallBufferCutHalfX, dChamberTopWallBufferCutHalfY, dChamberTopWallBufferCutHalfZ);

	G4SubtractionSolid *pChamberTopWallWithCut1 = new G4SubtractionSolid("ChamberTopWallWithCut1",
		pChamberTopWallTube, pChamberTopWallBufferCutBox, 0,
		G4ThreeVector(dChamberTopWallRadius, 0., -dChamberTopWallHalfZ));

	m_pChamberTopWallLogicalVolume = new G4LogicalVolume(pChamberTopWallWithCut1, SS304LSteel,
		"ChamberTopWallLogicalVolume", 0, 0, 0);

	m_pChamberTopWallPhysicalVolume = new G4PVPlacement(m_pRotationLabToSimulation, G4ThreeVector(0., 0., dChamberTopWallOffsetZ),
		m_pChamberTopWallLogicalVolume, "ChamberTopWall", m_pCryostatVacuumLogicalVolume, false, 0);

	//================================= middle plate ================================
	const G4double dChamberMiddlePlateHalfZ = 0.5*dChamberMiddlePlateThickness;
	const G4double dChamberMiddlePlateOffsetZ = dChamberTopWallOffsetZ-dChamberTopWallHalfZ+dChamberMiddlePlateHalfZ;
	
	const G4double dChamberWallBulgeCenterRadius = dChamberWallRadius-dChamberWallThickness;
	const G4double dChamberWallBulgeOffsetX = dChamberWallBulgeCenterRadius*cos(dChamberWallBulgeRotationAngleZ);
	const G4double dChamberWallBulgeOffsetY = dChamberWallBulgeCenterRadius*sin(dChamberWallBulgeRotationAngleZ);

	const G4double dChamberMiddlePlateBufferCutHalfX = dChamberTopWallRadius;
	const G4double dChamberMiddlePlateBufferCutHalfY = 0.5*dChamberBufferWidth;
	const G4double dChamberMiddlePlateBufferCutHalfZ = dChamberMiddlePlateThickness;

	G4Tubs *pChamberMiddlePlateTube = new G4Tubs("ChamberMiddlePlateTube", 0., dChamberTopWallRadius,
		dChamberMiddlePlateHalfZ, 0.*deg, 360.*deg);

	G4Tubs *pChamberMiddlePlateCutTube = new G4Tubs("ChamberMiddlePlateCutTube", 0., dChamberWallRadius,
		dChamberMiddlePlateThickness, 0.*deg, 360.*deg);

	G4SubtractionSolid *pChamberMiddlePlateWithCut1 = new G4SubtractionSolid("ChamberMiddlePlateWithCut1",
		pChamberMiddlePlateTube, pChamberMiddlePlateCutTube);

	G4Tubs *pChamberMiddlePlateBulgeCutTube = new G4Tubs("ChamberMiddlePlateBulgeCutTube", 0.,
		dChamberWallBulgeRadius, dChamberMiddlePlateThickness, 0.*deg, 360.*deg);

	G4SubtractionSolid *pChamberMiddlePlateWithCut2 = new G4SubtractionSolid("ChamberMiddlePlateWithCut2",
		pChamberMiddlePlateWithCut1, pChamberMiddlePlateBulgeCutTube, 0,
		G4ThreeVector(dChamberWallBulgeOffsetX, dChamberWallBulgeOffsetY, 0.));

	G4Box *pChamberMiddlePlateBufferCutBox = new G4Box("ChamberMiddlePlateBufferCutBox",
		dChamberMiddlePlateBufferCutHalfX, dChamberMiddlePlateBufferCutHalfY, dChamberMiddlePlateBufferCutHalfZ);

	G4SubtractionSolid *pChamberMiddlePlateWithCut3 = new G4SubtractionSolid("ChamberMiddlePlateWithCut3",
		pChamberMiddlePlateWithCut2, pChamberMiddlePlateBufferCutBox, 0,
		G4ThreeVector(dChamberWallRadius, 0., 0.));

	m_pChamberMiddlePlateLogicalVolume = new G4LogicalVolume(pChamberMiddlePlateWithCut3, SS304LSteel,
		"ChamberMiddlePlateLogicalVolume", 0, 0, 0);

	m_pChamberMiddlePlatePhysicalVolume = new G4PVPlacement(m_pRotationLabToSimulation, G4ThreeVector(0., 0., dChamberMiddlePlateOffsetZ),
		m_pChamberMiddlePlateLogicalVolume, "ChamberMiddlePlate", m_pCryostatVacuumLogicalVolume, false, 0);

	//================================== side wall ==================================
	const G4double dChamberWallHalfZ = 0.5*dChamberWallHeight;
	const G4double dChamberWallOffsetZ = dChamberMiddlePlateOffsetZ-dChamberMiddlePlateHalfZ-dChamberWallHalfZ;

	const G4double dChamberWallBulgeHalfZ = 0.5*dChamberWallBulgeHeight;

	const G4double dChamberWallBufferCutHalfX = dChamberWallRadius;
	const G4double dChamberWallBufferCutHalfY = 0.5*dChamberBufferWidth;
	const G4double dChamberWallBufferCutHalfZ = dChamberBufferHeight-dChamberBufferStepHeight;

	const G4double dChamberWallBulgeOffsetZ = dChamberWallHalfZ-dChamberWallBulgeHalfZ;

	G4Tubs *pChamberWallTube =  new G4Tubs("ChamberWallTube", dChamberWallRadius,
			dChamberWallRadius+dChamberWallThickness, dChamberWallHalfZ, 0.*deg, 360.*deg);

	G4Tubs *pChamberWallBulgeCutTube = new G4Tubs("ChamberWallBulgeCutBox",
		0., dChamberWallBulgeRadius, dChamberWallBulgeHeight, 0.*deg, 360.*deg);

	G4SubtractionSolid *pChamberWallWithCut1 = new G4SubtractionSolid("ChamberWallWithCut1",
		pChamberWallTube, pChamberWallBulgeCutTube, 0,
		G4ThreeVector(dChamberWallBulgeOffsetX, dChamberWallBulgeOffsetY, dChamberWallHalfZ));

	G4Box *pChamberWallBufferCutBox = new G4Box("ChamberWallBufferCutBox", dChamberWallBufferCutHalfX,
		dChamberWallBufferCutHalfY, dChamberWallBufferCutHalfZ);

	G4SubtractionSolid *pChamberWallWithCut2 = new G4SubtractionSolid("ChamberWallWithCut2",
		pChamberWallWithCut1, pChamberWallBufferCutBox, 0,
		G4ThreeVector(dChamberWallRadius, 0., dChamberWallHalfZ));

	G4Tubs *pChamberWallBulgeTube =  new G4Tubs("ChamberWallBulgeTube", dChamberWallBulgeRadius,
			dChamberWallBulgeRadius+dChamberWallBulgeThickness, dChamberWallBulgeHalfZ, -90.*deg, 180.*deg);

	G4RotationMatrix *pChamberWallBulgeRotation = new G4RotationMatrix();
	pChamberWallBulgeRotation->rotateZ(-dChamberWallBulgeRotationAngleZ);

	G4UnionSolid *pChamberWallWithWeld1 = new G4UnionSolid("ChamberWallWithWeld1",
		pChamberWallWithCut2, pChamberWallBulgeTube, pChamberWallBulgeRotation,
		G4ThreeVector(dChamberWallBulgeOffsetX, dChamberWallBulgeOffsetY, dChamberWallBulgeOffsetZ));

	m_pChamberWallLogicalVolume = new G4LogicalVolume(pChamberWallWithWeld1, SS304LSteel,
		"ChamberWallLogicalVolume", 0, 0, 0);

	m_pChamberWallPhysicalVolume = new G4PVPlacement(m_pRotationLabToSimulation, G4ThreeVector(0., 0., dChamberWallOffsetZ),
		m_pChamberWallLogicalVolume, "ChamberWall", m_pCryostatVacuumLogicalVolume, false, 0);

	//================================== bulge plate ================================
	const G4double dChamberWallBulgePlateThickness = dChamberWallBulgeThickness;
	const G4double dChamberWallBulgePlateHalfZ = 0.5*dChamberWallBulgeThickness;
	const G4double dChamberWallBulgePlateOffsetZ = dChamberWallOffsetZ+dChamberWallHalfZ-dChamberWallBulgeHeight+dChamberWallBulgePlateHalfZ;
	
	G4Tubs *pChamberWallBulgePlateTube = new G4Tubs("ChamberWallBulgePlateTube", 0., dChamberWallBulgeRadius,
		dChamberWallBulgePlateHalfZ, 0.*deg, 360.*deg);

	G4Tubs *pChamberWallBulgePlateCutTube = new G4Tubs("ChamberWallBulgePlateCutTube", 0., dChamberWallRadius,
		dChamberWallBulgePlateThickness, 0.*deg, 360.*deg);

	G4SubtractionSolid *pChamberWallBulgePlateWithCut1 = new G4SubtractionSolid("ChamberWallBulgePlateWithCut1",
		pChamberWallBulgePlateTube, pChamberWallBulgePlateCutTube, 0, G4ThreeVector(-dChamberWallBulgeCenterRadius, 0., 0.));

	m_pChamberWallBulgePlateLogicalVolume = new G4LogicalVolume(pChamberWallBulgePlateWithCut1, SS304LSteel,
		"ChamberWallBulgePlateLogicalVolume", 0, 0, 0);

	m_pChamberWallBulgePlatePhysicalVolume = new G4PVPlacement(pChamberWallBulgeRotation,
		G4ThreeVector(dChamberWallBulgeOffsetX, dChamberWallBulgeOffsetY, dChamberWallBulgePlateOffsetZ),
		m_pChamberWallBulgePlateLogicalVolume, "ChamberWallBulgePlate", m_pCryostatVacuumLogicalVolume, false, 0);

	//==================================== buffer ===================================
	const G4double dChamberBufferHalfX = 0.5*dChamberBufferLength;
	const G4double dChamberBufferHalfY = 0.5*dChamberBufferWidth;
	const G4double dChamberBufferHalfZ = 0.5*dChamberBufferHeight;
	const G4double dChamberBufferOffsetX = sqrt(dChamberWallRadius*dChamberWallRadius-dChamberBufferHalfY*dChamberBufferHalfY)+dChamberBufferHalfX;
	const G4double dChamberBufferOffsetZ = dChamberWallOffsetZ+dChamberWallHalfZ-dChamberBufferHalfZ+dChamberBufferStepHeight;

	const G4double dChamberBufferInnerCutHalfX = dChamberBufferLength-dChamberBufferThickness;
	const G4double dChamberBufferInnerCutHalfY = dChamberBufferHalfY-dChamberBufferThickness;
	const G4double dChamberBufferInnerCutHalfZ = dChamberBufferHalfZ-dChamberBufferThickness;

	const G4double dChamberBufferStepCutHalfX = dChamberBufferStepLength;
	const G4double dChamberBufferStepCutHalfY = dChamberBufferWidth;
	const G4double dChamberBufferStepCutHalfZ = dChamberBufferStepHeight-dChamberMiddlePlateThickness;

	G4Box *pChamberBufferBox = new G4Box("ChamberBufferBox", dChamberBufferHalfX,
		dChamberBufferHalfY, dChamberBufferHalfZ);

	G4Box *pChamberBufferInnerCutBox = new G4Box("ChamberBufferInnerCutBox", dChamberBufferInnerCutHalfX,
		dChamberBufferInnerCutHalfY, dChamberBufferInnerCutHalfZ);

	G4SubtractionSolid *pChamberBufferWithCut1 = new G4SubtractionSolid("ChamberBufferWithCut1",
		pChamberBufferBox, pChamberBufferInnerCutBox, 0, G4ThreeVector(-dChamberBufferHalfX, 0., 0.));

	G4Tubs *pChamberBufferCutTube = new G4Tubs("ChamberBufferCutTube", 0., dChamberWallRadius,
		dChamberBufferHeight, 0.*deg, 360.*deg);

	G4SubtractionSolid *pChamberBufferWithCut2 = new G4SubtractionSolid("ChamberBufferWithCut2",
		pChamberBufferWithCut1, pChamberBufferCutTube, 0, G4ThreeVector(-dChamberBufferOffsetX, 0., 0.));

	G4Box *pChamberBufferStepCutBox = new G4Box("ChamberBufferStepCutBox", dChamberBufferStepCutHalfX,
		dChamberBufferStepCutHalfY, dChamberBufferStepCutHalfZ);

	G4SubtractionSolid *pChamberBufferWithCut3 = new G4SubtractionSolid("ChamberBufferWithCut3",
		pChamberBufferWithCut2, pChamberBufferStepCutBox, 0,
		G4ThreeVector(-dChamberBufferHalfX, 0., dChamberBufferHalfZ));

	m_pChamberBufferLogicalVolume = new G4LogicalVolume(pChamberBufferWithCut3, SS304LSteel,
		"ChamberBufferLogicalVolume", 0, 0, 0);

	m_pChamberBufferPhysicalVolume = new G4PVPlacement(0,
		G4ThreeVector(dChamberBufferOffsetX, 0., dChamberBufferOffsetZ),
		m_pChamberBufferLogicalVolume, "ChamberBuffer", m_pCryostatVacuumLogicalVolume, false, 0);

	//=============================== buffer partition ==============================
	const G4double dChamberBufferPartitionHalfX = 0.5*dChamberBufferThickness;
	const G4double dChamberBufferPartitionHalfY = dChamberBufferHalfY-dChamberBufferThickness;
	const G4double dChamberBufferPartitionHalfZ = dChamberBufferHalfZ-dChamberBufferThickness;
	const G4double dChamberBufferPartitionOffsetX = dChamberBufferOffsetX-dChamberBufferHalfX+dChamberBufferStepLength+dChamberBufferPartitionHalfX;
	const G4double dChamberBufferPartitionOffsetZ = dChamberBufferOffsetZ;

	G4Box *pChamberBufferPartitionBox = new G4Box("ChamberBufferPartitionBox",
		dChamberBufferPartitionHalfX, dChamberBufferPartitionHalfY, dChamberBufferPartitionHalfZ);

	m_pChamberBufferPartitionLogicalVolume = new G4LogicalVolume(pChamberBufferPartitionBox, SS304LSteel,
		"ChamberBufferPartitionLogicalVolume", 0, 0, 0);

	m_pChamberBufferPartitionPhysicalVolume = new G4PVPlacement(0,
		G4ThreeVector(dChamberBufferPartitionOffsetX, 0., dChamberBufferPartitionOffsetZ),
		m_pChamberBufferPartitionLogicalVolume, "ChamberBufferPartition", m_pCryostatVacuumLogicalVolume, false, 0);

	//================================= bottom plate ================================
	const G4double dChamberBottomPlateHalfZ = 0.5*dChamberBottomPlateThickness;
	const G4double dChamberBottomPlateOffsetZ = dChamberWallOffsetZ-dChamberWallHalfZ+dChamberBottomPlateHalfZ;
	
	G4Tubs *pChamberBottomPlateTube = new G4Tubs("ChamberBottomPlateTube", 0., dChamberWallRadius,
		dChamberBottomPlateHalfZ, 0.*deg, 360.*deg);

	m_pChamberBottomPlateLogicalVolume = new G4LogicalVolume(pChamberBottomPlateTube, SS304LSteel,
		"ChamberBottomPlateLogicalVolume", 0, 0, 0);

	m_pChamberBottomPlatePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dChamberBottomPlateOffsetZ),
		m_pChamberBottomPlateLogicalVolume, "ChamberBottomPlate", m_pCryostatVacuumLogicalVolume, false, 0);

	//================================== attributes =================================
	G4Colour hStainlessSteelColor(0.500, 0.500, 0.500, 0.1);

	G4VisAttributes *pChamberVisAtt = new G4VisAttributes(hStainlessSteelColor);
	pChamberVisAtt->SetVisibility(true);
	m_pChamberMatingFlangeLogicalVolume->SetVisAttributes(pChamberVisAtt);
	m_pChamberTopFlangeLogicalVolume->SetVisAttributes(pChamberVisAtt);
	m_pChamberTopWallLogicalVolume->SetVisAttributes(pChamberVisAtt);
	m_pChamberMiddlePlateLogicalVolume->SetVisAttributes(pChamberVisAtt);
	m_pChamberWallLogicalVolume->SetVisAttributes(pChamberVisAtt);
	m_pChamberWallBulgePlateLogicalVolume->SetVisAttributes(pChamberVisAtt);
	m_pChamberBottomPlateLogicalVolume->SetVisAttributes(pChamberVisAtt);
	m_pChamberBufferLogicalVolume->SetVisAttributes(pChamberVisAtt);
	m_pChamberBufferPartitionLogicalVolume->SetVisAttributes(pChamberVisAtt);
}

void
NerixDetectorConstruction::ConstructXenon()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< xenon >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dCryostatWallHeight = GetGeometryParameter("CryostatWallHeight");
	const G4double dCryostatFlangeGasketThickness = GetGeometryParameter("CryostatFlangeGasketThickness");
	const G4double dCryostatFlangeToChamberMatingFlange = GetGeometryParameter("CryostatFlangeToChamberMatingFlange");
	const G4double dChamberFlangeThickness = GetGeometryParameter("ChamberFlangeThickness");

	const G4double dChamberWallRadius = GetGeometryParameter("ChamberWallRadius");
	const G4double dChamberWallHeight = GetGeometryParameter("ChamberWallHeight");
	const G4double dChamberWallThickness = GetGeometryParameter("ChamberWallThickness");

	const G4double dChamberTopWallRadius = GetGeometryParameter("ChamberTopWallRadius");
	const G4double dChamberTopWallHeight = GetGeometryParameter("ChamberTopWallHeight");
	const G4double dChamberBottomPlateThickness = GetGeometryParameter("ChamberBottomPlateThickness");
	const G4double dChamberMiddlePlateThickness = GetGeometryParameter("ChamberMiddlePlateThickness");

	const G4double dChamberWallBulgeRadius = GetGeometryParameter("ChamberWallBulgeRadius");
	const G4double dChamberWallBulgeHeight = GetGeometryParameter("ChamberWallBulgeHeight");
	const G4double dChamberWallBulgeThickness = GetGeometryParameter("ChamberWallBulgeThickness");

	const G4double dChamberWallBulgeRotationAngleZ = GetGeometryParameter("ChamberWallBulgeRotationAngleZ");

	const G4double dChamberBufferWidth = GetGeometryParameter("ChamberBufferWidth");
	const G4double dChamberBufferLength = GetGeometryParameter("ChamberBufferLength");
	const G4double dChamberBufferHeight = GetGeometryParameter("ChamberBufferHeight");
	const G4double dChamberBufferStepLength = GetGeometryParameter("ChamberBufferStepLength");
	const G4double dChamberBufferStepHeight = GetGeometryParameter("ChamberBufferStepHeight");
	const G4double dChamberBufferThickness = GetGeometryParameter("ChamberBufferThickness");

	const G4double dLXeHeight = GetGeometryParameter("LXeHeight");

	G4Material *LXe = G4Material::GetMaterial("LXe");
	G4Material *GXe = G4Material::GetMaterial("GXe");

	const G4double dCryostatVacuumHalfZ = 0.5*(dCryostatWallHeight+2.*dCryostatFlangeGasketThickness);

	//================================ liquid xenon =================================
	const G4double dLXeRadius = dChamberWallRadius;
	const G4double dLXeHalfZ = 0.5*dLXeHeight;
	const G4double dLXeOffsetZ = dCryostatVacuumHalfZ-dCryostatFlangeToChamberMatingFlange-2.*dChamberFlangeThickness-dChamberTopWallHeight-dChamberWallHeight+dLXeHalfZ+dChamberBottomPlateThickness;

	const G4double dLXeBulgeRadius = dChamberWallBulgeRadius;
	const G4double dLXeBulgeHalfZ = 0.5*(dLXeHeight-(dChamberWallHeight-dChamberWallBulgeHeight)-dChamberWallBulgeThickness+dChamberWallThickness);
	const G4double dLXeBulgeCenterRadius = dChamberWallRadius-dChamberWallThickness;
	const G4double dLXeBulgeOffsetX = dLXeBulgeCenterRadius*cos(dChamberWallBulgeRotationAngleZ);
	const G4double dLXeBulgeOffsetY = dLXeBulgeCenterRadius*sin(dChamberWallBulgeRotationAngleZ);
	const G4double dLXeBulgeOffsetZ = dLXeHalfZ-dLXeBulgeHalfZ;

	const G4double dLXeSideBoxHalfX = dChamberBufferStepLength;
	const G4double dLXeSideBoxHalfY = 0.5*dChamberBufferWidth-dChamberBufferThickness;
	const G4double dLXeSideBoxHalfZ = 0.5*(dLXeHeight-(dChamberWallHeight-dChamberBufferHeight+dChamberBufferStepHeight)-dChamberBufferThickness+dChamberWallThickness);

	const G4double dLXeSideBoxOffsetX = sqrt(dChamberWallRadius*dChamberWallRadius-0.5*dChamberBufferWidth*0.5*dChamberBufferWidth);
	const G4double dLXeSideBoxOffsetZ = dLXeHalfZ-dLXeSideBoxHalfZ;

	G4Tubs *pLXeTube = new G4Tubs("LXeTube", 0., dLXeRadius, dLXeHalfZ, 0.*deg, 360.*deg);

	G4Tubs *pLXeBulgeTube = new G4Tubs("LXeBulgeTube", 0., dLXeBulgeRadius, dLXeBulgeHalfZ, 0.*deg, 360.*deg);

	G4UnionSolid *pLXeWithAddition1 = new G4UnionSolid("LXeWithAddition1",
		pLXeTube, pLXeBulgeTube, 0, G4ThreeVector(dLXeBulgeOffsetX, dLXeBulgeOffsetY, dLXeBulgeOffsetZ));

	G4Box *pLXeSideBox = new G4Box("LXeSideBox", dLXeSideBoxHalfX, dLXeSideBoxHalfY, dLXeSideBoxHalfZ);

	G4UnionSolid *pLXeWithAddition2 = new G4UnionSolid("LXeWithAddition2",
		pLXeWithAddition1, pLXeSideBox, 0, G4ThreeVector(dLXeSideBoxOffsetX, 0., dLXeSideBoxOffsetZ));

	m_pLXeLogicalVolume = new G4LogicalVolume(pLXeWithAddition2, LXe, "LXeLogicalVolume", 0, 0, 0);

	m_pLXePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dLXeOffsetZ),
		m_pLXeLogicalVolume, "LXe", m_pCryostatVacuumLogicalVolume, false, 0);

	//============================== lxe chamber buffer =============================
	const G4double dLXeChamberBufferHalfX = 0.5*(dChamberBufferLength-dChamberBufferStepLength-dChamberBufferThickness);
	const G4double dLXeChamberBufferHalfY = 0.5*dChamberBufferWidth-dChamberBufferThickness;
	const G4double dLXeChamberBufferHalfZ = 0.5*(dLXeHeight-(dChamberWallHeight-dChamberBufferHeight+dChamberBufferStepHeight)-dChamberBufferThickness+dChamberWallThickness);
	const G4double dLXeChamberBufferOffsetX = sqrt(dChamberWallRadius*dChamberWallRadius-0.5*dChamberBufferWidth*0.5*dChamberBufferWidth)+dChamberBufferStepLength+dChamberBufferThickness+dLXeChamberBufferHalfX;
	const G4double dLXeChamberBufferOffsetZ = dLXeOffsetZ+dLXeHalfZ-dLXeSideBoxHalfZ;

	G4Box *pLXeChamberBufferBox = new G4Box("LXeChamberBufferBox", dLXeChamberBufferHalfX,
		dLXeChamberBufferHalfY, dLXeChamberBufferHalfZ);

	m_pLXeChamberBufferLogicalVolume = new G4LogicalVolume(pLXeChamberBufferBox, LXe,
		"LXeChamberBufferLogicalVolume", 0, 0, 0);

	m_pLXeChamberBufferPhysicalVolume = new G4PVPlacement(0,
		G4ThreeVector(dLXeChamberBufferOffsetX, 0., dLXeChamberBufferOffsetZ),
		m_pLXeChamberBufferLogicalVolume, "LXeChamberBuffer", m_pCryostatVacuumLogicalVolume, false, 0);

	//================================ gaseous xenon ================================
	const G4double dGXeRadius = dChamberTopWallRadius;
	const G4double dGXeHalfZ = 0.5*(dChamberTopWallHeight+dChamberFlangeThickness-dChamberMiddlePlateThickness);
	const G4double dGXeOffsetZ = dCryostatVacuumHalfZ-dCryostatFlangeToChamberMatingFlange-dChamberFlangeThickness-dGXeHalfZ;

	const G4double dGXeBottomTubeRadius = dChamberWallRadius;
	const G4double dGXeBottomTubeHalfZ = dChamberWallHeight+dChamberMiddlePlateThickness-dChamberBottomPlateThickness-dLXeHeight;

	const G4double dGXeBulgeRadius = dChamberWallBulgeRadius;
	const G4double dGXeBulgeHalfZ = dChamberWallHeight+dChamberMiddlePlateThickness-dChamberBottomPlateThickness-dLXeHeight;
	const G4double dGXeBulgeCenterRadius = dChamberWallRadius-dChamberWallThickness;
	const G4double dGXeBulgeOffsetX = dGXeBulgeCenterRadius*cos(dChamberWallBulgeRotationAngleZ);
	const G4double dGXeBulgeOffsetY = dGXeBulgeCenterRadius*sin(dChamberWallBulgeRotationAngleZ);

//    const G4double dGXeChamberTopWallMiddlePlateAdditionHalfX = 0.5*(dChamberWidth-2.*dChamberCornerCutWidth);	
//    const G4double dGXeChamberTopWallMiddlePlateAdditionHalfY = 0.5*dChamberWidth;
//    const G4double dGXeChamberTopWallMiddlePlateAdditionHalfZ = dChamberMiddlePlateThickness;

	G4Tubs *pGXeTube = new G4Tubs("GXeTube", 0., dGXeRadius, dGXeHalfZ, 0.*deg, 360.*deg);

	G4Tubs *pGXeBottomTube = new G4Tubs("GXeBottomTube", 0., dGXeBottomTubeRadius,
		dGXeBottomTubeHalfZ, 0.*deg, 360.*deg);

	G4UnionSolid *pGXeWithAddition1 = new G4UnionSolid("GXeWithAddition1",
		pGXeTube, pGXeBottomTube, 0, G4ThreeVector(0., 0., -dGXeHalfZ));

	G4Tubs *pGXeBulgeTube = new G4Tubs("GXeBulgeTube", 0., dGXeBulgeRadius,
		dGXeBulgeHalfZ, 0.*deg, 360.*deg);

	G4UnionSolid *pGXeWithAddition2 = new G4UnionSolid("GXeWithAddition2",
		pGXeWithAddition1, pGXeBulgeTube, 0,
		G4ThreeVector(dGXeBulgeOffsetX, dGXeBulgeOffsetY, -dGXeHalfZ));

	m_pGXeLogicalVolume = new G4LogicalVolume(pGXeWithAddition2, GXe, "GXeLogicalVolume", 0, 0, 0);

	m_pGXePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dGXeOffsetZ),
		m_pGXeLogicalVolume, "GXe", m_pCryostatVacuumLogicalVolume, false, 0);

	//============================== gxe chamber wall ===============================
//    const G4double dGXeChamberWallHalfX = 0.5*(dChamberWidth-2.*dChamberCornerCutWidth);
//    const G4double dGXeChamberWallHalfY = 0.5*dChamberWidth;
//    const G4double dGXeChamberWallHalfZ = 0.5*(dChamberWallHeight+dChamberBottomCutHeight-dChamberWallThickness-dLXeHeight);
//    const G4double dGXeChamberWallOffsetZ = 0.5*dChamberWallHeight-dGXeChamberWallHalfZ;

//    const G4double dGXePmtHoleBoxHalfX = 0.5*dPmtCasingWidth;
//    const G4double dGXePmtHoleBoxHalfY = dGXePmtHoleBoxHalfX;
//    const G4double dGXePmtHoleBoxHalfZ = dPmtCasingHeight+dPmtWindowThickness+dMiddleToPmtWindow-dLiquidLevelToMiddle;
//    const G4double dGXePmtHoleBoxOffsetZ = -dGXeChamberWallHalfZ;

//    G4Box *pGXeChamberWallBox1 = new G4Box("GXeChamberWallBox1", dGXeChamberWallHalfX, dGXeChamberWallHalfY, dGXeChamberWallHalfZ);
//    G4Box *pGXeChamberWallBox2 = new G4Box("GXeChamberWallBox2", dGXeChamberWallHalfY, dGXeChamberWallHalfX, dGXeChamberWallHalfZ);

//    G4UnionSolid *pGXeChamberWallPlus = new G4UnionSolid("GXeChamberWallPlus",
//        pGXeChamberWallBox1, pGXeChamberWallBox2, 0, G4ThreeVector(0., 0., 0.));

//    G4Box *pGXePmtHoleBox = new G4Box("GXePmtHoleBox", dGXePmtHoleBoxHalfX, dGXePmtHoleBoxHalfY, dGXePmtHoleBoxHalfZ);

//    G4SubtractionSolid *pGXeChamberWallWithPmtHole = new G4SubtractionSolid("GXeChamberWallWithPmtHole",
//        pGXeChamberWallPlus, pGXePmtHoleBox, 0, G4ThreeVector(0., 0., dGXePmtHoleBoxOffsetZ));

//    m_pGXeChamberWallLogicalVolume = new G4LogicalVolume(pGXeChamberWallWithPmtHole, GXe,
//        "GXeChamberWallLogicalVolume", 0, 0, 0);

//    m_pGXeChamberWallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dGXeChamberWallOffsetZ),
//        m_pGXeChamberWallLogicalVolume, "GXeChamberWall", m_pChamberWallLogicalVolume, false, 0);

	//============================== gxe chamber buffer =============================
//    const G4double dGXeChamberBufferHalfX = 0.5*dChamberBufferWidth;
//    const G4double dGXeChamberBufferHalfY = dGXeChamberBufferHalfX;
//    const G4double dGXeChamberBufferHalfZ = 0.5*(dChamberWallHeight-2.*dChamberWallThickness-dLXeHeight+dChamberBottomCutHeight);
//    const G4double dGXeChamberBufferOffsetX = 0.5*dChamberWallThickness;
//    const G4double dGXeChamberBufferOffsetY = -0.5*dChamberWallThickness;
//    const G4double dGXeChamberBufferOffsetZ = 0.5*dChamberWallHeight-dGXeChamberBufferHalfZ-dChamberWallThickness;

//    G4Box *pGXeChamberBufferBox = new G4Box("GXeChamberBufferBox", dGXeChamberBufferHalfX,
//        dGXeChamberBufferHalfY, dGXeChamberBufferHalfZ);

//    m_pGXeChamberBufferLogicalVolume = new G4LogicalVolume(pGXeChamberBufferBox, GXe,
//        "GXeChamberBufferLogicalVolume", 0, 0, 0);

//    m_pGXeChamberBufferPhysicalVolume = new G4PVPlacement(0,
//        G4ThreeVector(dGXeChamberBufferOffsetX, dGXeChamberBufferOffsetY, dGXeChamberBufferOffsetZ),
//        m_pGXeChamberBufferLogicalVolume, "GXeChamberBuffer", m_pChamberBufferLogicalVolume, false, 0);

	//============================== xenon sensitivity ==============================
	G4SDManager *pSDManager = G4SDManager::GetSDMpointer();

	NerixLXeSensitiveDetector *pLXeSD = new NerixLXeSensitiveDetector("Nerix/LXeSD");
	pSDManager->AddNewDetector(pLXeSD);
	m_pLXeLogicalVolume->SetSensitiveDetector(pLXeSD);

	//================================== attributes =================================
	G4Colour hLXeColor(0.435, 0.694, 0.757, 1.00);
	G4Colour hGXeColor(0.239, 0.918, 0.878, 0.50);

	G4VisAttributes *pLXeVisAtt = new G4VisAttributes(hLXeColor);
	pLXeVisAtt->SetVisibility(true);
	m_pLXeLogicalVolume->SetVisAttributes(pLXeVisAtt);
	m_pLXeChamberBufferLogicalVolume->SetVisAttributes(pLXeVisAtt);

	G4VisAttributes *pGXeVisAtt = new G4VisAttributes(hGXeColor);
	pGXeVisAtt->SetVisibility(true);
	m_pGXeLogicalVolume->SetVisAttributes(pGXeVisAtt);
//    m_pGXeChamberWallLogicalVolume->SetVisAttributes(pGXeVisAtt);
//    m_pGXeChamberBufferLogicalVolume->SetVisAttributes(pGXeVisAtt);
}

void
NerixDetectorConstruction::ConstructTeflon()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< teflon >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dLXeHeight = GetGeometryParameter("LXeHeight");
	const G4double dGXeGapHeight = GetGeometryParameter("GXeGapHeight");

	const G4double dChamberTopWallHeight = GetGeometryParameter("ChamberTopWallHeight");
	const G4double dChamberWallHeight = GetGeometryParameter("ChamberWallHeight");
	const G4double dChamberFlangeThickness = GetGeometryParameter("ChamberFlangeThickness");
	const G4double dChamberMiddlePlateThickness = GetGeometryParameter("ChamberMiddlePlateThickness");
	const G4double dChamberBottomPlateThickness = GetGeometryParameter("ChamberBottomPlateThickness");

	const G4bool bUseTopGrid = (G4bool) GetGeometryParameter("UseTopGrid");
	const G4bool bUseScreeningGrid = (G4bool) GetGeometryParameter("UseScreeningGrid");

//    const G4double dLiquidLevelToMiddle = GetGeometryParameter("LiquidLevelToMiddle");
//    const G4double dChamberBottomCutHeight = GetGeometryParameter("ChamberBottomCutHeight");
	const G4double dTeflonStackInnerRadius = GetGeometryParameter("TeflonStackInnerRadius");
	const G4double dTeflonStackOuterRadius = GetGeometryParameter("TeflonStackOuterRadius");

	const G4double dTeflonTopPmtHolderRadius = GetGeometryParameter("TeflonTopPmtHolderRadius");
	const G4double dTeflonTopPmtHolderHeight = GetGeometryParameter("TeflonTopPmtHolderHeight");
	const G4double dTeflonTopPmtHolderSquareCutWidth = GetGeometryParameter("TeflonTopPmtHolderSquareCutWidth");
	const G4double dTeflonTopPmtHolderSquareCutDepth = GetGeometryParameter("TeflonTopPmtHolderSquareCutDepth");
	const G4double dTeflonTopPmtHolderTopStepWidth = GetGeometryParameter("TeflonTopPmtHolderTopStepWidth");
	const G4double dTeflonTopPmtHolderTopStepHeight = GetGeometryParameter("TeflonTopPmtHolderTopStepHeight");
	const G4double dTeflonTopPmtHolderBottomStepWidth = GetGeometryParameter("TeflonTopPmtHolderBottomStepWidth");
	const G4double dTeflonTopPmtHolderBottomStepHeight = GetGeometryParameter("TeflonTopPmtHolderBottomStepHeight");

	const G4double dTeflonTopGridSpacerHeight = GetGeometryParameter("TeflonTopGridSpacerHeight");
	const G4double dTeflonTopGridSpacerInnerStepWidth = GetGeometryParameter("TeflonTopGridSpacerInnerStepWidth");
	const G4double dTeflonTopGridSpacerInnerStepHeight = GetGeometryParameter("TeflonTopGridSpacerInnerStepHeight");
	const G4double dTeflonTopGridSpacerTopStepWidth = GetGeometryParameter("TeflonTopGridSpacerTopStepWidth");
	const G4double dTeflonTopGridSpacerTopStepHeight = GetGeometryParameter("TeflonTopGridSpacerTopStepHeight");
	const G4double dTeflonTopGridSpacerBottomStepWidth = GetGeometryParameter("TeflonTopGridSpacerBottomStepWidth");
	const G4double dTeflonTopGridSpacerBottomStepHeight = GetGeometryParameter("TeflonTopGridSpacerBottomStepHeight");
	
	const G4double dTeflonAnodeSpacerHeight = GetGeometryParameter("TeflonAnodeSpacerHeight");
	const G4double dTeflonAnodeSpacerInnerStepWidth = GetGeometryParameter("TeflonAnodeSpacerInnerStepWidth");
	const G4double dTeflonAnodeSpacerInnerStepHeight = GetGeometryParameter("TeflonAnodeSpacerInnerStepHeight");
	const G4double dTeflonAnodeSpacerGapHeight = GetGeometryParameter("TeflonAnodeSpacerGapHeight");
	
	const G4double dTeflonDriftRegionSpacerHeight = GetGeometryParameter("TeflonDriftRegionSpacerHeight");
	const G4double dTeflonDriftRegionSpacerInnerStepWidth = GetGeometryParameter("TeflonDriftRegionSpacerInnerStepWidth");
	const G4double dTeflonDriftRegionSpacerInnerStepHeight = GetGeometryParameter("TeflonDriftRegionSpacerInnerStepHeight");
	const G4double dTeflonDriftRegionSpacerTopStepWidth = GetGeometryParameter("TeflonDriftRegionSpacerTopStepWidth");
	const G4double dTeflonDriftRegionSpacerTopStepHeight = GetGeometryParameter("TeflonDriftRegionSpacerTopStepHeight");
	const G4double dTeflonDriftRegionSpacerBottomStepWidth = GetGeometryParameter("TeflonDriftRegionSpacerBottomStepWidth");
	const G4double dTeflonDriftRegionSpacerBottomStepHeight = GetGeometryParameter("TeflonDriftRegionSpacerBottomStepHeight");
	
	const G4double dTeflonScreeningGridSpacerHeight = GetGeometryParameter("TeflonScreeningGridSpacerHeight");
	const G4double dTeflonScreeningGridSpacerInnerStepWidth = GetGeometryParameter("TeflonScreeningGridSpacerInnerStepWidth");
	const G4double dTeflonScreeningGridSpacerInnerStepHeight = GetGeometryParameter("TeflonScreeningGridSpacerInnerStepHeight");
	const G4double dTeflonScreeningGridSpacerTopStepWidth = GetGeometryParameter("TeflonScreeningGridSpacerTopStepWidth");
	const G4double dTeflonScreeningGridSpacerTopStepHeight = GetGeometryParameter("TeflonScreeningGridSpacerTopStepHeight");
	
	const G4double dTeflonBottomPmtHolderRadius = GetGeometryParameter("TeflonBottomPmtHolderRadius");
	const G4double dTeflonBottomPmtHolderHeight = GetGeometryParameter("TeflonBottomPmtHolderHeight");
	const G4double dTeflonBottomPmtHolderInnerStepWidth = GetGeometryParameter("TeflonBottomPmtHolderInnerStepWidth");
	const G4double dTeflonBottomPmtHolderInnerStepHeight = GetGeometryParameter("TeflonBottomPmtHolderInnerStepHeight");
	const G4double dTeflonBottomPmtHolderTopStepWidth = GetGeometryParameter("TeflonBottomPmtHolderTopStepWidth");
	const G4double dTeflonBottomPmtHolderTopStepHeight = GetGeometryParameter("TeflonBottomPmtHolderTopStepHeight");
	
	G4Material *Teflon = G4Material::GetMaterial("Teflon");	

	//================================ top pmt holder ===============================
	const G4double dTeflonTopPmtHolderHalfZ = 0.5*dTeflonTopPmtHolderHeight;
	const G4double dTeflonTopPmtHolderOffsetZ = (bUseTopGrid)?(-0.5*(dChamberTopWallHeight+dChamberFlangeThickness-dChamberMiddlePlateThickness)-dChamberMiddlePlateThickness-dChamberWallHeight+dLXeHeight+dChamberBottomPlateThickness+dGXeGapHeight+dTeflonAnodeSpacerInnerStepHeight+dTeflonTopGridSpacerHeight-dTeflonTopGridSpacerBottomStepHeight-dTeflonTopGridSpacerTopStepHeight+dTeflonTopPmtHolderHalfZ-dTeflonTopPmtHolderTopStepHeight-dTeflonTopPmtHolderBottomStepHeight):(-0.5*(dChamberTopWallHeight+dChamberFlangeThickness-dChamberMiddlePlateThickness)-dChamberMiddlePlateThickness-dChamberWallHeight+dLXeHeight+dChamberBottomPlateThickness+dGXeGapHeight+dTeflonAnodeSpacerInnerStepHeight+dTeflonTopPmtHolderHalfZ-dTeflonTopPmtHolderTopStepHeight-dTeflonTopPmtHolderBottomStepHeight);

	const G4double dTeflonTopPmtHolderSquareCutHalfX = 0.5*dTeflonTopPmtHolderSquareCutWidth;
	const G4double dTeflonTopPmtHolderSquareCutHalfY = dTeflonTopPmtHolderSquareCutHalfX;
	const G4double dTeflonTopPmtHolderSquareCutHalfZ = dTeflonTopPmtHolderSquareCutDepth;

	const G4double dTeflonTopPmtHolderConicalCutRadius = dTeflonTopPmtHolderSquareCutHalfX;
	const G4double dTeflonTopPmtHolderConicalCutHalfZ = 0.5*dTeflonTopPmtHolderConicalCutRadius;
	const G4double dTeflonTopPmtHolderConicalCutOffsetZ = dTeflonTopPmtHolderHalfZ-dTeflonTopPmtHolderSquareCutDepth-dTeflonTopPmtHolderConicalCutHalfZ;

	const G4double dTeflonTopPmtHolderCenterCutRadius = dTeflonStackInnerRadius;
	const G4double dTeflonTopPmtHolderCenterCutHalfZ = dTeflonTopPmtHolderHeight;

	const G4double dTeflonTopPmtHolderTopStepCutInnerRadius = dTeflonTopPmtHolderRadius-dTeflonTopPmtHolderTopStepWidth;
	const G4double dTeflonTopPmtHolderTopStepCutOuterRadius = 2.*dTeflonTopPmtHolderRadius;
	const G4double dTeflonTopPmtHolderTopStepCutHalfZ = dTeflonTopPmtHolderTopStepHeight+dTeflonTopPmtHolderBottomStepHeight;

	const G4double dTeflonTopPmtHolderBottomStepCutInnerRadius = dTeflonTopPmtHolderTopStepCutInnerRadius-dTeflonTopPmtHolderBottomStepWidth;
	const G4double dTeflonTopPmtHolderBottomStepCutOuterRadius = dTeflonTopPmtHolderRadius;
	const G4double dTeflonTopPmtHolderBottomStepCutHalfZ = dTeflonTopPmtHolderBottomStepHeight;

	G4Tubs *pTeflonTopPmtHolderTube = new G4Tubs("TeflonTopPmtHolderTube",
		0., dTeflonTopPmtHolderRadius, dTeflonTopPmtHolderHalfZ, 0.*deg, 360.*deg);

	G4Box *pTeflonTopPmtHolderSquareCutBox = new G4Box("TeflonTopPmtHolderSquareCutBox",
		dTeflonTopPmtHolderSquareCutHalfX, dTeflonTopPmtHolderSquareCutHalfY, dTeflonTopPmtHolderSquareCutHalfZ);

	G4SubtractionSolid *pTeflonTopPmtHolderWithCut1 = new G4SubtractionSolid("TeflonTopPmtHolderWithCut1",
		pTeflonTopPmtHolderTube, pTeflonTopPmtHolderSquareCutBox, 0, G4ThreeVector(0., 0., dTeflonTopPmtHolderHalfZ));

	G4Cons *pTeflonTopPmtHolderConicalCutCone = new G4Cons("TeflonTopPmtHolderConicalCutBox",
		0., 0., 0., dTeflonTopPmtHolderConicalCutRadius, dTeflonTopPmtHolderConicalCutHalfZ, 0.*deg, 360.*deg);

	G4SubtractionSolid *pTeflonTopPmtHolderWithCut2 = new G4SubtractionSolid("TeflonTopPmtHolderWithCut2",
		pTeflonTopPmtHolderWithCut1, pTeflonTopPmtHolderConicalCutCone, 0, G4ThreeVector(0., 0., dTeflonTopPmtHolderConicalCutOffsetZ));

	G4Tubs *pTeflonTopPmtHolderCenterCutTube = new G4Tubs("TeflonTopPmtHolderCenterCutTube",
		0., dTeflonTopPmtHolderCenterCutRadius, dTeflonTopPmtHolderCenterCutHalfZ, 0.*deg, 360.*deg);

	G4SubtractionSolid *pTeflonTopPmtHolderWithCut3 = new G4SubtractionSolid("TeflonTopPmtHolderWithCut3",
		pTeflonTopPmtHolderWithCut2, pTeflonTopPmtHolderCenterCutTube, 0, G4ThreeVector(0., 0., 0.));

	G4Tubs *pTeflonTopPmtHolderTopStepCutTube = new G4Tubs("TeflonTopPmtHolderTopStepCutTube",
		dTeflonTopPmtHolderTopStepCutInnerRadius, dTeflonTopPmtHolderTopStepCutOuterRadius,
		dTeflonTopPmtHolderTopStepCutHalfZ, 0.*deg, 360.*deg);

	G4SubtractionSolid *pTeflonTopPmtHolderWithCut4 = new G4SubtractionSolid("TeflonTopPmtHolderWithCut4",
		pTeflonTopPmtHolderWithCut3, pTeflonTopPmtHolderTopStepCutTube, 0, G4ThreeVector(0., 0., -dTeflonTopPmtHolderHalfZ));

	G4Tubs *pTeflonTopPmtHolderBottomStepCutTube = new G4Tubs("TeflonTopPmtHolderBottomStepCutTube",
		dTeflonTopPmtHolderBottomStepCutInnerRadius, dTeflonTopPmtHolderBottomStepCutOuterRadius,
		dTeflonTopPmtHolderBottomStepCutHalfZ, 0.*deg, 360.*deg);

	G4SubtractionSolid *pTeflonTopPmtHolderWithCut5 = new G4SubtractionSolid("TeflonTopPmtHolderWithCut5",
		pTeflonTopPmtHolderWithCut4, pTeflonTopPmtHolderBottomStepCutTube, 0, G4ThreeVector(0., 0., -dTeflonTopPmtHolderHalfZ));

//    G4Box *pTeflonCubeVerticalCutBox = new G4Box("TeflonCubeVerticalCutBox",
//        dTeflonCubeVerticalCutHalfX, dTeflonCubeVerticalCutHalfY, dTeflonCubeVerticalCutHalfZ);

//    G4SubtractionSolid *pTeflonCubeWithCut5 = new G4SubtractionSolid("TeflonCubeWithCut5",
//        pTeflonCubeWithCut4, pTeflonCubeVerticalCutBox, 0, G4ThreeVector(0., 0., dTeflonCubeVerticalCutOffsetZ));
//    G4SubtractionSolid *pTeflonCubeWithCut6 = new G4SubtractionSolid("TeflonCubeWithCut6",
//        pTeflonCubeWithCut5, pTeflonCubeVerticalCutBox, 0, G4ThreeVector(0., 0., -dTeflonCubeVerticalCutOffsetZ));

//    G4Box *pTeflonCubeVerticalThroughCutBox = new G4Box("TeflonCubeVerticalThroughCutBox",
//        dTeflonCubeVerticalThroughCutHalfX, dTeflonCubeVerticalThroughCutHalfY, dTeflonCubeVerticalThroughCutHalfZ);

//    G4SubtractionSolid *pTeflonCubeWithCut7 = new G4SubtractionSolid("TeflonCubeWithCut7",
//        pTeflonCubeWithCut6, pTeflonCubeVerticalThroughCutBox, 0, G4ThreeVector(0., 0., 0.));

//    G4Box *pTeflonCubeHorizontalThroughCutBox = new G4Box("TeflonCubeHorizontalThroughCutBox",
//        dTeflonCubeHorizontalThroughCutHalfX, dTeflonCubeHorizontalThroughCutHalfY,
//        dTeflonCubeHorizontalThroughCutHalfZ);

//    G4SubtractionSolid *pTeflonCubeWithCut8 = new G4SubtractionSolid("TeflonCubeWithCut8",
//        pTeflonCubeWithCut7, pTeflonCubeHorizontalThroughCutBox, 0, G4ThreeVector(0., dTeflonCubeHalfY, 0.));
//    G4SubtractionSolid *pTeflonCubeWithCut9 = new G4SubtractionSolid("TeflonCubeWithCut9",
//        pTeflonCubeWithCut8, pTeflonCubeHorizontalThroughCutBox, m_pRotationZPlus90, G4ThreeVector(dTeflonCubeHalfX, 0., 0.));
//    G4SubtractionSolid *pTeflonCubeWithCut10 = new G4SubtractionSolid("TeflonCubeWithCut10",
//        pTeflonCubeWithCut9, pTeflonCubeHorizontalThroughCutBox, 0, G4ThreeVector(0., -dTeflonCubeHalfY, 0.));
//    G4SubtractionSolid *pTeflonCubeWithCut11 = new G4SubtractionSolid("TeflonCubeWithCut11",
//        pTeflonCubeWithCut10, pTeflonCubeHorizontalThroughCutBox, m_pRotationZPlus90, G4ThreeVector(-dTeflonCubeHalfX, 0., 0.));

	m_pTeflonTopPmtHolderLogicalVolume = new G4LogicalVolume(pTeflonTopPmtHolderWithCut5, Teflon,
			"TeflonTopPmtHolderLogicalVolume", 0, 0, 0);

	m_pTeflonTopPmtHolderPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dTeflonTopPmtHolderOffsetZ),
		m_pTeflonTopPmtHolderLogicalVolume, "TeflonTopPmtHolder", m_pGXeLogicalVolume, false, 0);

	//=============================== top grid spacer ===============================
	const G4double dTeflonTopGridSpacerOuterRadius = dTeflonStackOuterRadius;
	const G4double dTeflonTopGridSpacerInnerRadius = dTeflonStackInnerRadius;
	const G4double dTeflonTopGridSpacerOffsetZ = -0.5*(dChamberTopWallHeight+dChamberFlangeThickness-dChamberMiddlePlateThickness)-dChamberMiddlePlateThickness-dChamberWallHeight+dLXeHeight+dChamberBottomPlateThickness+dGXeGapHeight+dTeflonAnodeSpacerInnerStepHeight+dTeflonTopGridSpacerHeight-dTeflonTopGridSpacerBottomStepHeight-dTeflonTopGridSpacerTopStepHeight;

	const G4int iTeflonTopGridSpacerNbZPlanes = 8;

	const G4double pdTeflonTopGridSpacerZPlanes[]
		= {0,
			-dTeflonTopGridSpacerInnerStepHeight,
			-dTeflonTopGridSpacerInnerStepHeight,
			-dTeflonTopGridSpacerHeight+dTeflonTopGridSpacerBottomStepHeight+dTeflonTopGridSpacerTopStepHeight,
			-dTeflonTopGridSpacerHeight+dTeflonTopGridSpacerBottomStepHeight+dTeflonTopGridSpacerTopStepHeight,
			-dTeflonTopGridSpacerHeight+dTeflonTopGridSpacerBottomStepHeight,
			-dTeflonTopGridSpacerHeight+dTeflonTopGridSpacerBottomStepHeight,
			-dTeflonTopGridSpacerHeight
			};

	const G4double pdTeflonTopGridSpacerInnerRadii[]
		= {dTeflonTopGridSpacerInnerRadius+dTeflonTopGridSpacerInnerStepWidth,
			dTeflonTopGridSpacerInnerRadius+dTeflonTopGridSpacerInnerStepWidth,
			dTeflonTopGridSpacerInnerRadius,
			dTeflonTopGridSpacerInnerRadius,
			dTeflonTopGridSpacerInnerRadius,
			dTeflonTopGridSpacerInnerRadius,
			dTeflonTopGridSpacerInnerRadius,
			dTeflonTopGridSpacerInnerRadius
			};

	const G4double pdTeflonTopGridSpacerOuterRadii[]
		= {dTeflonTopGridSpacerOuterRadius,
			dTeflonTopGridSpacerOuterRadius,
			dTeflonTopGridSpacerOuterRadius,
			dTeflonTopGridSpacerOuterRadius,
			dTeflonTopGridSpacerOuterRadius-dTeflonTopGridSpacerTopStepWidth,
			dTeflonTopGridSpacerOuterRadius-dTeflonTopGridSpacerTopStepWidth,
			dTeflonTopGridSpacerOuterRadius-dTeflonTopGridSpacerTopStepWidth-dTeflonTopGridSpacerBottomStepWidth,
			dTeflonTopGridSpacerOuterRadius-dTeflonTopGridSpacerTopStepWidth-dTeflonTopGridSpacerBottomStepWidth
			};

	if(bUseTopGrid)
	{
		G4Polycone *pTeflonTopGridSpacerPolycone = new G4Polycone("TeflonTopGridSpacerPolycone",
				0.*deg, 360.*deg, iTeflonTopGridSpacerNbZPlanes, pdTeflonTopGridSpacerZPlanes,
				pdTeflonTopGridSpacerInnerRadii, pdTeflonTopGridSpacerOuterRadii);

		m_pTeflonTopGridSpacerLogicalVolume = new G4LogicalVolume(pTeflonTopGridSpacerPolycone, Teflon,
			"TeflonTopGridSpacerLogicalVolume", 0, 0, 0);

		m_pTeflonTopGridSpacerPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dTeflonTopGridSpacerOffsetZ),
			m_pTeflonTopGridSpacerLogicalVolume, "TeflonTopGridSpacer", m_pGXeLogicalVolume, false, 0);
	}

	//================================= anode spacer ================================
	const G4double dTeflonAnodeSpacerOuterRadius = dTeflonStackOuterRadius;
	const G4double dTeflonAnodeSpacerInnerRadius = dTeflonStackInnerRadius;
	const G4double dTeflonAnodeSpacerAboveLevelHalfZ = 0.5*(dTeflonAnodeSpacerInnerStepHeight+dGXeGapHeight);
	const G4double dTeflonAnodeSpacerAboveLevelOffsetZ = -0.5*(dChamberTopWallHeight+dChamberFlangeThickness-dChamberMiddlePlateThickness)-dChamberMiddlePlateThickness-dChamberWallHeight+dLXeHeight+dChamberBottomPlateThickness+dTeflonAnodeSpacerAboveLevelHalfZ;
	const G4double dTeflonAnodeSpacerBelowLevelHalfZ = 0.5*(dTeflonAnodeSpacerHeight-dTeflonAnodeSpacerInnerStepHeight-dGXeGapHeight);
	const G4double dTeflonAnodeSpacerBelowLevelOffsetZ = 0.5*dLXeHeight-dTeflonAnodeSpacerBelowLevelHalfZ;
	
	G4Tubs *pTeflonAnodeSpacerAboveLevelTube = new G4Tubs("TeflonAnodeSpacerAboveLevelTube",
		dTeflonAnodeSpacerInnerRadius, dTeflonAnodeSpacerOuterRadius,
		dTeflonAnodeSpacerAboveLevelHalfZ, 0.*deg, 360.*deg);

	G4Tubs *pTeflonAnodeSpacerAboveLevelCut = new G4Tubs("TeflonAnodeSpacerAboveLevelCut",
		0., dTeflonAnodeSpacerInnerRadius+dTeflonAnodeSpacerInnerStepWidth,
		dTeflonAnodeSpacerInnerStepHeight, 0.*deg, 360.*deg);

	G4SubtractionSolid *pTeflonAnoderSpacerAboveLevelWithCut1 = new G4SubtractionSolid("TeflonAnoderSpacerAboveLevelWithCut1",
		pTeflonAnodeSpacerAboveLevelTube, pTeflonAnodeSpacerAboveLevelCut, 0,
		G4ThreeVector(0., 0., dTeflonAnodeSpacerAboveLevelHalfZ));

	G4Tubs *pTeflonAnodeSpacerBelowLevelTube = new G4Tubs("TeflonAnodeSpacerBelowLevelTube",
		dTeflonAnodeSpacerInnerRadius, dTeflonAnodeSpacerOuterRadius,
		dTeflonAnodeSpacerBelowLevelHalfZ, 0.*deg, 360.*deg);

	G4Tubs *pTeflonAnodeSpacerBelowLevelCut = new G4Tubs("TeflonAnodeSpacerBelowLevelCut",
		0., dTeflonAnodeSpacerInnerRadius+dTeflonAnodeSpacerInnerStepWidth,
		dTeflonAnodeSpacerHeight-dTeflonAnodeSpacerInnerStepHeight-dTeflonAnodeSpacerGapHeight, 0.*deg, 360.*deg);

	G4SubtractionSolid *pTeflonAnoderSpacerBelowLevelWithCut1 = new G4SubtractionSolid("TeflonAnoderSpacerBelowLevelWithCut1",
		pTeflonAnodeSpacerBelowLevelTube, pTeflonAnodeSpacerBelowLevelCut, 0,
		G4ThreeVector(0., 0., -dTeflonAnodeSpacerBelowLevelHalfZ));

	m_pTeflonAnodeSpacerAboveLevelLogicalVolume = new G4LogicalVolume(pTeflonAnoderSpacerAboveLevelWithCut1, Teflon,
		"TeflonAnodeSpacerAboveLevelLogicalVolume", 0, 0, 0);

	m_pTeflonAnodeSpacerAboveLevelPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dTeflonAnodeSpacerAboveLevelOffsetZ),
		m_pTeflonAnodeSpacerAboveLevelLogicalVolume, "TeflonAnodeSpacerAboveLevel", m_pGXeLogicalVolume, false, 0);

	m_pTeflonAnodeSpacerBelowLevelLogicalVolume = new G4LogicalVolume(pTeflonAnoderSpacerBelowLevelWithCut1, Teflon,
		"TeflonAnodeSpacerBelowLevelLogicalVolume", 0, 0, 0);

	m_pTeflonAnodeSpacerBelowLevelPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dTeflonAnodeSpacerBelowLevelOffsetZ),
		m_pTeflonAnodeSpacerBelowLevelLogicalVolume, "TeflonAnodeSpacerBelowLevel", m_pLXeLogicalVolume, false, 0);

	//============================= drift region spacer =============================
	const G4double dTeflonDriftRegionSpacerOuterRadius = dTeflonStackOuterRadius;
	const G4double dTeflonDriftRegionSpacerInnerRadius = dTeflonStackInnerRadius;
	const G4double dTeflonDriftRegionSpacerOffsetZ = 0.5*dLXeHeight-dTeflonAnodeSpacerGapHeight+dGXeGapHeight;

	const G4int iTeflonDriftRegionSpacerNbZPlanes = 8;

	const G4double pdTeflonDriftRegionSpacerZPlanes[]
		= {0,
			-dTeflonDriftRegionSpacerTopStepHeight,
			-dTeflonDriftRegionSpacerTopStepHeight,
			-dTeflonDriftRegionSpacerTopStepHeight-dTeflonDriftRegionSpacerBottomStepHeight,
			-dTeflonDriftRegionSpacerTopStepHeight-dTeflonDriftRegionSpacerBottomStepHeight,
			-dTeflonDriftRegionSpacerHeight+dTeflonDriftRegionSpacerInnerStepHeight,
			-dTeflonDriftRegionSpacerHeight+dTeflonDriftRegionSpacerInnerStepHeight,
			-dTeflonDriftRegionSpacerHeight
			};

	const G4double pdTeflonDriftRegionSpacerInnerRadii[]
		= {dTeflonDriftRegionSpacerInnerRadius,
			dTeflonDriftRegionSpacerInnerRadius,
			dTeflonDriftRegionSpacerInnerRadius,
			dTeflonDriftRegionSpacerInnerRadius,
			dTeflonDriftRegionSpacerInnerRadius,
			dTeflonDriftRegionSpacerInnerRadius,
			dTeflonDriftRegionSpacerInnerRadius+dTeflonDriftRegionSpacerInnerStepWidth,
			dTeflonDriftRegionSpacerInnerRadius+dTeflonDriftRegionSpacerInnerStepWidth
			};

	const G4double pdTeflonDriftRegionSpacerOuterRadii[]
		= {dTeflonDriftRegionSpacerOuterRadius-dTeflonDriftRegionSpacerBottomStepWidth-dTeflonDriftRegionSpacerTopStepWidth,
			dTeflonDriftRegionSpacerOuterRadius-dTeflonDriftRegionSpacerBottomStepWidth-dTeflonDriftRegionSpacerTopStepWidth,
			dTeflonDriftRegionSpacerOuterRadius-dTeflonDriftRegionSpacerBottomStepWidth,
			dTeflonDriftRegionSpacerOuterRadius-dTeflonDriftRegionSpacerBottomStepWidth,
			dTeflonDriftRegionSpacerOuterRadius,
			dTeflonDriftRegionSpacerOuterRadius,
			dTeflonDriftRegionSpacerOuterRadius,
			dTeflonDriftRegionSpacerOuterRadius
			};

	G4Polycone *pTeflonDriftRegionSpacerPolycone = new G4Polycone("TeflonDriftRegionSpacerPolycone",
			0.*deg, 360.*deg, iTeflonDriftRegionSpacerNbZPlanes, pdTeflonDriftRegionSpacerZPlanes,
			pdTeflonDriftRegionSpacerInnerRadii, pdTeflonDriftRegionSpacerOuterRadii);

	m_pTeflonDriftRegionSpacerLogicalVolume = new G4LogicalVolume(pTeflonDriftRegionSpacerPolycone, Teflon,
		"TeflonDriftRegionSpacerLogicalVolume", 0, 0, 0);

	m_pTeflonDriftRegionSpacerPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dTeflonDriftRegionSpacerOffsetZ),
		m_pTeflonDriftRegionSpacerLogicalVolume, "TeflonDriftRegionSpacer", m_pLXeLogicalVolume, false, 0);

	//========================== drift region ring holder ===========================
	const G4double dTeflonDriftRegionRingHolderOuterRadius = dTeflonDriftRegionSpacerOuterRadius;
	const G4double dTeflonDriftRegionRingHolderInnerRadius = dTeflonDriftRegionSpacerOuterRadius-dTeflonDriftRegionSpacerBottomStepWidth;
	const G4double dTeflonDriftRegionRingHolderHalfZ = 0.5*dTeflonDriftRegionSpacerBottomStepHeight;
	const G4double dTeflonDriftRegionRingHolderOffsetZ = 0.5*dLXeHeight-dTeflonAnodeSpacerGapHeight+dGXeGapHeight-dTeflonDriftRegionSpacerTopStepHeight-dTeflonDriftRegionRingHolderHalfZ;

	G4Tubs *pTeflonDriftRegionRingHolderTube = new G4Tubs("TeflonDriftRegionRingHolderTube",
		dTeflonDriftRegionRingHolderInnerRadius, dTeflonDriftRegionRingHolderOuterRadius,
		dTeflonDriftRegionRingHolderHalfZ, 0.*deg, 360.*deg);

	m_pTeflonDriftRegionRingHolderLogicalVolume = new G4LogicalVolume(pTeflonDriftRegionRingHolderTube, Teflon,
		"TeflonDriftRegionRingHolderLogicalVolume", 0, 0, 0);

	m_pTeflonDriftRegionRingHolderPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dTeflonDriftRegionRingHolderOffsetZ),
		m_pTeflonDriftRegionRingHolderLogicalVolume, "TeflonDriftRegionRingHolder", m_pLXeLogicalVolume, false, 0);

	//============================ screening grid spacer ============================
	const G4double dTeflonScreeningGridSpacerOuterRadius = dTeflonStackOuterRadius;
	const G4double dTeflonScreeningGridSpacerInnerRadius = dTeflonStackInnerRadius;
	const G4double dTeflonScreeningGridSpacerOffsetZ = dTeflonDriftRegionSpacerOffsetZ-dTeflonDriftRegionSpacerHeight+dTeflonScreeningGridSpacerTopStepHeight;

	const G4int iTeflonScreeningGridSpacerNbZPlanes = 6;

	const G4double pdTeflonScreeningGridSpacerZPlanes[]
		= {0,
			-dTeflonScreeningGridSpacerTopStepHeight,
			-dTeflonScreeningGridSpacerTopStepHeight,
			-dTeflonScreeningGridSpacerHeight+dTeflonScreeningGridSpacerInnerStepHeight,
			-dTeflonScreeningGridSpacerHeight+dTeflonScreeningGridSpacerInnerStepHeight,
			-dTeflonScreeningGridSpacerHeight
			};

	const G4double pdTeflonScreeningGridSpacerInnerRadii[]
		= {dTeflonScreeningGridSpacerInnerRadius,
			dTeflonScreeningGridSpacerInnerRadius,
			dTeflonScreeningGridSpacerInnerRadius,
			dTeflonScreeningGridSpacerInnerRadius,
			dTeflonScreeningGridSpacerInnerRadius+dTeflonScreeningGridSpacerInnerStepWidth,
			dTeflonScreeningGridSpacerInnerRadius+dTeflonScreeningGridSpacerInnerStepWidth
			};

	const G4double pdTeflonScreeningGridSpacerOuterRadii[]
		= {dTeflonScreeningGridSpacerOuterRadius-dTeflonScreeningGridSpacerTopStepWidth,
			dTeflonScreeningGridSpacerOuterRadius-dTeflonScreeningGridSpacerTopStepWidth,
			dTeflonScreeningGridSpacerOuterRadius,
			dTeflonScreeningGridSpacerOuterRadius,
			dTeflonScreeningGridSpacerOuterRadius,
			dTeflonScreeningGridSpacerOuterRadius
			};

	if(bUseScreeningGrid)
	{
		G4Polycone *pTeflonScreeningGridSpacerPolycone = new G4Polycone("TeflonScreeningGridSpacerPolycone",
				0.*deg, 360.*deg, iTeflonScreeningGridSpacerNbZPlanes, pdTeflonScreeningGridSpacerZPlanes,
				pdTeflonScreeningGridSpacerInnerRadii, pdTeflonScreeningGridSpacerOuterRadii);

		m_pTeflonScreeningGridSpacerLogicalVolume = new G4LogicalVolume(pTeflonScreeningGridSpacerPolycone, Teflon,
			"TeflonScreeningGridSpacerLogicalVolume", 0, 0, 0);

		m_pTeflonScreeningGridSpacerPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dTeflonScreeningGridSpacerOffsetZ),
			m_pTeflonScreeningGridSpacerLogicalVolume, "TeflonScreeningGridSpacer", m_pLXeLogicalVolume, false, 0);
	}

	//============================== bottom pmt holder ==============================
	const G4double dTeflonBottomPmtHolderOuterRadius = dTeflonBottomPmtHolderRadius;
	const G4double dTeflonBottomPmtHolderInnerRadius = dTeflonStackInnerRadius;
	const G4double dTeflonBottomPmtHolderOffsetZ = (bUseScreeningGrid)?(dTeflonScreeningGridSpacerOffsetZ-dTeflonScreeningGridSpacerHeight+dTeflonScreeningGridSpacerTopStepHeight):(dTeflonScreeningGridSpacerOffsetZ);

	const G4int iTeflonBottomPmtHolderNbZPlanes = 6;

	const G4double pdTeflonBottomPmtHolderZPlanes[]
		= {0,
			-dTeflonBottomPmtHolderTopStepHeight,
			-dTeflonBottomPmtHolderTopStepHeight,
			-dTeflonBottomPmtHolderHeight+dTeflonBottomPmtHolderInnerStepHeight,
			-dTeflonBottomPmtHolderHeight+dTeflonBottomPmtHolderInnerStepHeight,
			-dTeflonBottomPmtHolderHeight
			};

	const G4double pdTeflonBottomPmtHolderInnerRadii[]
		= {dTeflonBottomPmtHolderInnerRadius,
			dTeflonBottomPmtHolderInnerRadius,
			dTeflonBottomPmtHolderInnerRadius,
			dTeflonBottomPmtHolderInnerRadius,
			dTeflonBottomPmtHolderInnerRadius+dTeflonBottomPmtHolderInnerStepWidth,
			dTeflonBottomPmtHolderInnerRadius+dTeflonBottomPmtHolderInnerStepWidth
			};

	const G4double pdTeflonBottomPmtHolderOuterRadii[]
		= {dTeflonBottomPmtHolderOuterRadius-dTeflonBottomPmtHolderTopStepWidth,
			dTeflonBottomPmtHolderOuterRadius-dTeflonBottomPmtHolderTopStepWidth,
			dTeflonBottomPmtHolderOuterRadius,
			dTeflonBottomPmtHolderOuterRadius,
			dTeflonBottomPmtHolderOuterRadius,
			dTeflonBottomPmtHolderOuterRadius
			};

	G4Polycone *pTeflonBottomPmtHolderPolycone = new G4Polycone("TeflonBottomPmtHolderPolycone",
			0.*deg, 360.*deg, iTeflonBottomPmtHolderNbZPlanes, pdTeflonBottomPmtHolderZPlanes,
			pdTeflonBottomPmtHolderInnerRadii, pdTeflonBottomPmtHolderOuterRadii);

	m_pTeflonBottomPmtHolderLogicalVolume = new G4LogicalVolume(pTeflonBottomPmtHolderPolycone, Teflon,
		"TeflonBottomPmtHolderLogicalVolume", 0, 0, 0);

	m_pTeflonBottomPmtHolderPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dTeflonBottomPmtHolderOffsetZ),
		m_pTeflonBottomPmtHolderLogicalVolume, "TeflonBottomPmtHolder", m_pLXeLogicalVolume, false, 0);

	//=============================== optical surfaces ==============================
	G4double dSigmaAlpha = 0.1;
	G4OpticalSurface *pTeflonOpticalSurface = new G4OpticalSurface("TeflonOpticalSurface", unified,
		ground, dielectric_metal, dSigmaAlpha);

	pTeflonOpticalSurface->SetMaterialPropertiesTable(Teflon->GetMaterialPropertiesTable());
	
	new G4LogicalBorderSurface("TeflonTopPmtHolderBorderSurface",
		m_pGXePhysicalVolume, m_pTeflonTopPmtHolderPhysicalVolume, pTeflonOpticalSurface);
	if(bUseTopGrid)
		new G4LogicalBorderSurface("TeflonTopGridSpacerBorderSurface",
			m_pGXePhysicalVolume, m_pTeflonTopGridSpacerPhysicalVolume, pTeflonOpticalSurface);
	new G4LogicalBorderSurface("TeflonAnodeSpacerAboveLevelBorderSurface",
		m_pGXePhysicalVolume, m_pTeflonAnodeSpacerAboveLevelPhysicalVolume, pTeflonOpticalSurface);
	new G4LogicalBorderSurface("TeflonAnodeSpacerBelowLevelBorderSurface",
		m_pLXePhysicalVolume, m_pTeflonAnodeSpacerBelowLevelPhysicalVolume, pTeflonOpticalSurface);
	new G4LogicalBorderSurface("TeflonDriftRegionSpacerBorderSurface",
		m_pLXePhysicalVolume, m_pTeflonDriftRegionSpacerPhysicalVolume, pTeflonOpticalSurface);
	new G4LogicalBorderSurface("TeflonDriftRegionRingHolderBorderSurface",
		m_pLXePhysicalVolume, m_pTeflonDriftRegionRingHolderPhysicalVolume, pTeflonOpticalSurface);
	if(bUseScreeningGrid)
		new G4LogicalBorderSurface("TeflonScreeningGridSpacerBorderSurface",
			m_pLXePhysicalVolume, m_pTeflonScreeningGridSpacerPhysicalVolume, pTeflonOpticalSurface);
	new G4LogicalBorderSurface("TeflonBottomPmtHolderBorderSurface",
		m_pLXePhysicalVolume, m_pTeflonBottomPmtHolderPhysicalVolume, pTeflonOpticalSurface);

	//================================== attributes =================================
	G4Colour hTeflonColor(1., 1., 1., 1.);

	G4VisAttributes *pTeflonVisAtt = new G4VisAttributes(hTeflonColor);
	pTeflonVisAtt->SetVisibility(true);
	m_pTeflonTopPmtHolderLogicalVolume->SetVisAttributes(pTeflonVisAtt);
	if(bUseTopGrid)
		m_pTeflonTopGridSpacerLogicalVolume->SetVisAttributes(pTeflonVisAtt);
	m_pTeflonAnodeSpacerAboveLevelLogicalVolume->SetVisAttributes(pTeflonVisAtt);
	m_pTeflonAnodeSpacerBelowLevelLogicalVolume->SetVisAttributes(pTeflonVisAtt);
	m_pTeflonDriftRegionSpacerLogicalVolume->SetVisAttributes(pTeflonVisAtt);
	m_pTeflonDriftRegionRingHolderLogicalVolume->SetVisAttributes(pTeflonVisAtt);
	if(bUseScreeningGrid)
		m_pTeflonScreeningGridSpacerLogicalVolume->SetVisAttributes(pTeflonVisAtt);
	m_pTeflonBottomPmtHolderLogicalVolume->SetVisAttributes(pTeflonVisAtt);
}

void
NerixDetectorConstruction::ConstructGrids()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< grids >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dLXeHeight = GetGeometryParameter("LXeHeight");
	const G4double dGXeGapHeight = GetGeometryParameter("GXeGapHeight");

	const G4double dChamberTopWallHeight = GetGeometryParameter("ChamberTopWallHeight");
	const G4double dChamberWallHeight = GetGeometryParameter("ChamberWallHeight");
	const G4double dChamberFlangeThickness = GetGeometryParameter("ChamberFlangeThickness");
	const G4double dChamberMiddlePlateThickness = GetGeometryParameter("ChamberMiddlePlateThickness");
	const G4double dChamberBottomPlateThickness = GetGeometryParameter("ChamberBottomPlateThickness");

	const G4bool bUseTopGrid = (G4bool) GetGeometryParameter("UseTopGrid");
	const G4bool bUseScreeningGrid = (G4bool) GetGeometryParameter("UseScreeningGrid");

	const G4double dTeflonTopGridSpacerHeight = GetGeometryParameter("TeflonTopGridSpacerHeight");
	const G4double dTeflonTopGridSpacerInnerStepHeight = GetGeometryParameter("TeflonTopGridSpacerInnerStepHeight");
	const G4double dTeflonTopGridSpacerTopStepHeight = GetGeometryParameter("TeflonTopGridSpacerTopStepHeight");
	const G4double dTeflonTopGridSpacerBottomStepHeight = GetGeometryParameter("TeflonTopGridSpacerBottomStepHeight");
	const G4double dTeflonAnodeSpacerInnerStepHeight = GetGeometryParameter("TeflonAnodeSpacerInnerStepHeight");
	const G4double dTeflonAnodeSpacerGapHeight = GetGeometryParameter("TeflonAnodeSpacerGapHeight");
	const G4double dTeflonDriftRegionSpacerHeight = GetGeometryParameter("TeflonDriftRegionSpacerHeight");
	const G4double dTeflonScreeningGridSpacerHeight = GetGeometryParameter("TeflonScreeningGridSpacerHeight");
	const G4double dTeflonScreeningGridSpacerTopStepHeight = GetGeometryParameter("TeflonScreeningGridSpacerTopStepHeight");

	const G4double dGridRingRadius = GetGeometryParameter("GridRingRadius");
	const G4double dGridRingWidth = GetGeometryParameter("GridRingWidth");
	const G4double dGridRingThickness = GetGeometryParameter("GridRingThickness");

	const G4double dTopGridWireDiameter = GetGeometryParameter("TopGridWireDiameter");
	const G4double dTopGridWirePitch = GetGeometryParameter("TopGridWirePitch");
	const G4double dAnodeGridWireDiameter = GetGeometryParameter("AnodeGridWireDiameter");
	const G4double dAnodeGridWirePitch = GetGeometryParameter("AnodeGridWirePitch");
	const G4double dBottomGridWireDiameter = GetGeometryParameter("BottomGridWireDiameter");
	const G4double dBottomGridWirePitch = GetGeometryParameter("BottomGridWirePitch");
	const G4double dCathodeGridWireDiameter = GetGeometryParameter("CathodeGridWireDiameter");
	const G4double dCathodeGridWirePitch = GetGeometryParameter("CathodeGridWirePitch");
	const G4double dScreeningGridWireDiameter = GetGeometryParameter("ScreeningGridWireDiameter");
	const G4double dScreeningGridWirePitch = GetGeometryParameter("ScreeningGridWirePitch");

	G4Material *GridSteel = G4Material::GetMaterial("GridSteel");

	//==================================== grids ====================================
	const G4double dGridRingInnerRadius = dGridRingRadius-dGridRingWidth;
	const G4double dGridRingHalfZ = 0.5*dGridRingThickness;

	G4Tubs *pGridRingTube = new G4Tubs("GridRingTube", dGridRingInnerRadius, dGridRingRadius,
		dGridRingHalfZ, 0.*deg, 360.*deg);

	m_pGridRingLogicalVolume = new G4LogicalVolume(pGridRingTube, GridSteel, "GridRingLogicalVolume", 0, 0, 0);

	//---------------------------------- top grid -----------------------------------
	const G4double dTopGridRingOffsetZ = -0.5*(dChamberTopWallHeight+dChamberFlangeThickness-dChamberMiddlePlateThickness)-dChamberMiddlePlateThickness-dChamberWallHeight+dLXeHeight+dChamberBottomPlateThickness+dGXeGapHeight+dTeflonAnodeSpacerInnerStepHeight+(dTeflonTopGridSpacerHeight-dTeflonTopGridSpacerBottomStepHeight-dTeflonTopGridSpacerTopStepHeight-dTeflonTopGridSpacerInnerStepHeight)+dGridRingHalfZ;

	const G4double dTopGridHalfZ = 0.5*dTopGridWireDiameter;
	const G4double dTopGridOffsetZ = dTopGridRingOffsetZ-dGridRingHalfZ+dTopGridHalfZ;

	int iVolumeId;
	stringstream hVolumeName;
	int iNbRings;

	if(bUseTopGrid)
	{
		m_pTopGridRingPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dTopGridRingOffsetZ),
			m_pGridRingLogicalVolume, "TopGridRing", m_pGXeLogicalVolume, false, 0);

		const G4double pdTopGridZPlanes[] = {-0.5*dTopGridWireDiameter, 0.5*dTopGridWireDiameter};

		const G4double pdTopGridInnerRadii[] = {0.5*(dTopGridWirePitch-dTopGridWireDiameter), 0.5*(dTopGridWirePitch-dTopGridWireDiameter)};

		const G4double pdTopGridOuterRadii[] = {0.5*(dTopGridWirePitch), 0.5*(dTopGridWirePitch)};

		G4Polyhedra *pTopGridHexagonalCell = new G4Polyhedra("TopGridHexagonalCell", 0.*deg, 360.*deg, 6, 2,
			pdTopGridZPlanes, pdTopGridInnerRadii, pdTopGridOuterRadii);

		m_pTopGridHexagonalCellLogicalVolume = new G4LogicalVolume(pTopGridHexagonalCell, GridSteel, "TopGridHexagonalCellLogicalVolume", 0, 0, 0);

		iVolumeId = 1;
		iNbRings = 12;
		for(int iNa = -iNbRings+1; iNa < iNbRings; iNa++)
		{
			for(int iNb = -iNbRings+1; iNb < iNbRings; iNb++)
			{
				G4ThreeVector hPos(iNb*(0.5*sqrt(3.)*dTopGridWirePitch), iNa*(dTopGridWirePitch)+iNb*(0.5*dTopGridWirePitch), dTopGridOffsetZ);

				if((sqrt(pow(hPos.x()+0.5*dTopGridWirePitch,2)+pow(hPos.y(),2)) > dGridRingInnerRadius)
					|| (sqrt(pow(hPos.x()-0.5*dTopGridWirePitch,2)+pow(hPos.y(),2)) > dGridRingInnerRadius)
					|| (sqrt(pow(hPos.x()+0.5/sqrt(3.)*dTopGridWirePitch,2)+pow(hPos.y()+0.5*dTopGridWirePitch,2)) > dGridRingInnerRadius)
					|| (sqrt(pow(hPos.x()-0.5/sqrt(3.)*dTopGridWirePitch,2)+pow(hPos.y()+0.5*dTopGridWirePitch,2)) > dGridRingInnerRadius)
					|| (sqrt(pow(hPos.x()+0.5/sqrt(3.)*dTopGridWirePitch,2)+pow(hPos.y()-0.5*dTopGridWirePitch,2)) > dGridRingInnerRadius)
					|| (sqrt(pow(hPos.x()-0.5/sqrt(3.)*dTopGridWirePitch,2)+pow(hPos.y()-0.5*dTopGridWirePitch,2)) > dGridRingInnerRadius))
					continue;

				hVolumeName << "TopGridHexagonalCell" << iVolumeId++;

				m_hTopGridPhysicalVolumes.push_back(new G4PVPlacement(0, hPos,
					m_pTopGridHexagonalCellLogicalVolume, hVolumeName.str(), m_pGXeLogicalVolume, false, 0));

				hVolumeName.str(""); hVolumeName.clear();
			}
		}
	}

	//------------------------------------ anode ------------------------------------
	const G4double dAnodeGridRingOffsetZ = -0.5*(dChamberTopWallHeight+dChamberFlangeThickness-dChamberMiddlePlateThickness)-dChamberMiddlePlateThickness-dChamberWallHeight+dLXeHeight+dChamberBottomPlateThickness+dGXeGapHeight+dGridRingHalfZ;

	const G4double dAnodeGridHalfZ = 0.5*dAnodeGridWireDiameter;
	const G4double dAnodeGridOffsetZ = dAnodeGridRingOffsetZ-dGridRingHalfZ+dAnodeGridHalfZ;

	m_pAnodeGridRingPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dAnodeGridRingOffsetZ),
		m_pGridRingLogicalVolume, "AnodeGridRing", m_pGXeLogicalVolume, false, 0);

	const G4double pdAnodeGridZPlanes[] = {-0.5*dAnodeGridWireDiameter, 0.5*dAnodeGridWireDiameter};

	const G4double pdAnodeGridInnerRadii[] = {0.5*(dAnodeGridWirePitch-dAnodeGridWireDiameter), 0.5*(dAnodeGridWirePitch-dAnodeGridWireDiameter)};

	const G4double pdAnodeGridOuterRadii[] = {0.5*dAnodeGridWirePitch, 0.5*dAnodeGridWirePitch};

	G4Polyhedra *pAnodeGridHexagonalCell = new G4Polyhedra("AnodeGridHexagonalCell", 0.*deg, 360.*deg, 6, 2,
		pdAnodeGridZPlanes, pdAnodeGridInnerRadii, pdAnodeGridOuterRadii);

	m_pAnodeGridHexagonalCellLogicalVolume = new G4LogicalVolume(pAnodeGridHexagonalCell, GridSteel, "AnodeGridHexagonalCellLogicalVolume", 0, 0, 0);

	iVolumeId = 1;
	iNbRings = 12;
	for(int iNa = -iNbRings+1; iNa < iNbRings; iNa++)
	{
		for(int iNb = -iNbRings+1; iNb < iNbRings; iNb++)
		{
			G4ThreeVector hPos(iNb*(0.5*sqrt(3.)*dAnodeGridWirePitch), iNa*(dAnodeGridWirePitch)+iNb*(0.5*dAnodeGridWirePitch), dAnodeGridOffsetZ);

			if((sqrt(pow(hPos.x()+0.5*dAnodeGridWirePitch,2)+pow(hPos.y(),2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()-0.5*dAnodeGridWirePitch,2)+pow(hPos.y(),2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()+0.5/sqrt(3.)*dAnodeGridWirePitch,2)+pow(hPos.y()+0.5*dAnodeGridWirePitch,2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()-0.5/sqrt(3.)*dAnodeGridWirePitch,2)+pow(hPos.y()+0.5*dAnodeGridWirePitch,2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()+0.5/sqrt(3.)*dAnodeGridWirePitch,2)+pow(hPos.y()-0.5*dAnodeGridWirePitch,2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()-0.5/sqrt(3.)*dAnodeGridWirePitch,2)+pow(hPos.y()-0.5*dAnodeGridWirePitch,2)) > dGridRingInnerRadius))
				continue;

			hVolumeName << "AnodeGridHexagonalCell" << iVolumeId++;

			m_hAnodeGridPhysicalVolumes.push_back(new G4PVPlacement(0, hPos,
				m_pAnodeGridHexagonalCellLogicalVolume, hVolumeName.str(), m_pGXeLogicalVolume, false, 0));

			hVolumeName.str(""); hVolumeName.clear();
		}
	}

	//--------------------------------- bottom grid ---------------------------------
	const G4double dBottomGridRingOffsetZ = 0.5*dLXeHeight-dGridRingHalfZ-dTeflonAnodeSpacerGapHeight+dGXeGapHeight;

	const G4double dBottomGridHalfZ = 0.5*dBottomGridWireDiameter;
	const G4double dBottomGridOffsetZ = dBottomGridRingOffsetZ+dGridRingHalfZ-dBottomGridHalfZ;

	m_pBottomGridRingPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dBottomGridRingOffsetZ),
		m_pGridRingLogicalVolume, "BottomGridRing", m_pLXeLogicalVolume, false, 0);

	const G4double pdBottomGridZPlanes[] = {-0.5*dBottomGridWireDiameter, 0.5*dBottomGridWireDiameter};

	const G4double pdBottomGridInnerRadii[] = {0.5*(dBottomGridWirePitch-dBottomGridWireDiameter), 0.5*(dBottomGridWirePitch-dBottomGridWireDiameter)};

	const G4double pdBottomGridOuterRadii[] = {0.5*dBottomGridWirePitch, 0.5*dBottomGridWirePitch};

	G4Polyhedra *pBottomGridHexagonalCell = new G4Polyhedra("BottomGridHexagonalCell", 0.*deg, 360.*deg, 6, 2,
		pdBottomGridZPlanes, pdBottomGridInnerRadii, pdBottomGridOuterRadii);

	m_pBottomGridHexagonalCellLogicalVolume = new G4LogicalVolume(pBottomGridHexagonalCell, GridSteel, "BottomGridHexagonalCellLogicalVolume", 0, 0, 0);

	iVolumeId = 1;
	iNbRings = 12;
	for(int iNa = -iNbRings+1; iNa < iNbRings; iNa++)
	{
		for(int iNb = -iNbRings+1; iNb < iNbRings; iNb++)
		{
			G4ThreeVector hPos(iNb*(0.5*sqrt(3.)*dBottomGridWirePitch), iNa*(dBottomGridWirePitch)+iNb*(0.5*dBottomGridWirePitch), dBottomGridOffsetZ);

			if((sqrt(pow(hPos.x()+0.5*dBottomGridWirePitch,2)+pow(hPos.y(),2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()-0.5*dBottomGridWirePitch,2)+pow(hPos.y(),2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()+0.5/sqrt(3.)*dBottomGridWirePitch,2)+pow(hPos.y()+0.5*dBottomGridWirePitch,2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()-0.5/sqrt(3.)*dBottomGridWirePitch,2)+pow(hPos.y()+0.5*dBottomGridWirePitch,2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()+0.5/sqrt(3.)*dBottomGridWirePitch,2)+pow(hPos.y()-0.5*dBottomGridWirePitch,2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()-0.5/sqrt(3.)*dBottomGridWirePitch,2)+pow(hPos.y()-0.5*dBottomGridWirePitch,2)) > dGridRingInnerRadius))
				continue;

			hVolumeName << "BottomGridHexagonalCell" << iVolumeId++;

			m_hBottomGridPhysicalVolumes.push_back(new G4PVPlacement(0, hPos,
				m_pBottomGridHexagonalCellLogicalVolume, hVolumeName.str(), m_pLXeLogicalVolume, false, 0));

			hVolumeName.str(""); hVolumeName.clear();
		}
	}

	//----------------------------------- cathode -----------------------------------
	const G4double dCathodeGridRingOffsetZ = 0.5*dLXeHeight-dTeflonAnodeSpacerGapHeight+dGXeGapHeight-dTeflonDriftRegionSpacerHeight+dGridRingHalfZ;

	const G4double dCathodeGridHalfZ = 0.5*dCathodeGridWireDiameter;
	const G4double dCathodeGridOffsetZ = dCathodeGridRingOffsetZ+dGridRingHalfZ-dCathodeGridHalfZ;

	m_pCathodeGridRingPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dCathodeGridRingOffsetZ),
		m_pGridRingLogicalVolume, "CathodeGridRing", m_pLXeLogicalVolume, false, 0);

	const G4double pdCathodeGridZPlanes[] = {-0.5*dCathodeGridWireDiameter, 0.5*dCathodeGridWireDiameter};

	const G4double pdCathodeGridInnerRadii[] = {0.5*(dCathodeGridWirePitch-dCathodeGridWireDiameter), 0.5*(dCathodeGridWirePitch-dCathodeGridWireDiameter)};

	const G4double pdCathodeGridOuterRadii[] = {0.5*dCathodeGridWirePitch, 0.5*dCathodeGridWirePitch};

	G4Polyhedra *pCathodeGridHexagonalCell = new G4Polyhedra("CathodeGridHexagonalCell", 0.*deg, 360.*deg, 6, 2,
		pdCathodeGridZPlanes, pdCathodeGridInnerRadii, pdCathodeGridOuterRadii);

	m_pCathodeGridHexagonalCellLogicalVolume = new G4LogicalVolume(pCathodeGridHexagonalCell, GridSteel, "CathodeGridHexagonalCellLogicalVolume", 0, 0, 0);

	iVolumeId = 1;
	iNbRings = 12;
	for(int iNa = -iNbRings+1; iNa < iNbRings; iNa++)
	{
		for(int iNb = -iNbRings+1; iNb < iNbRings; iNb++)
		{
			G4ThreeVector hPos(iNb*(0.5*sqrt(3.)*dCathodeGridWirePitch), iNa*(dCathodeGridWirePitch)+iNb*(0.5*dCathodeGridWirePitch), dCathodeGridOffsetZ);

			if((sqrt(pow(hPos.x()+0.5*dCathodeGridWirePitch,2)+pow(hPos.y(),2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()-0.5*dCathodeGridWirePitch,2)+pow(hPos.y(),2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()+0.5/sqrt(3.)*dCathodeGridWirePitch,2)+pow(hPos.y()+0.5*dCathodeGridWirePitch,2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()-0.5/sqrt(3.)*dCathodeGridWirePitch,2)+pow(hPos.y()+0.5*dCathodeGridWirePitch,2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()+0.5/sqrt(3.)*dCathodeGridWirePitch,2)+pow(hPos.y()-0.5*dCathodeGridWirePitch,2)) > dGridRingInnerRadius)
				|| (sqrt(pow(hPos.x()-0.5/sqrt(3.)*dCathodeGridWirePitch,2)+pow(hPos.y()-0.5*dCathodeGridWirePitch,2)) > dGridRingInnerRadius))
				continue;

			hVolumeName << "CathodeGridHexagonalCell" << iVolumeId++;

			m_hCathodeGridPhysicalVolumes.push_back(new G4PVPlacement(0, hPos,
				m_pCathodeGridHexagonalCellLogicalVolume, hVolumeName.str(), m_pLXeLogicalVolume, false, 0));

			hVolumeName.str(""); hVolumeName.clear();
		}
	}

	//-------------------------------- screening grid -------------------------------
	const G4double dScreeningGridRingOffsetZ = 0.5*dLXeHeight-dTeflonAnodeSpacerGapHeight+dGXeGapHeight-dTeflonDriftRegionSpacerHeight-dTeflonScreeningGridSpacerHeight+dTeflonScreeningGridSpacerTopStepHeight+dGridRingHalfZ;

	const G4double dScreeningGridHalfZ = 0.5*dScreeningGridWireDiameter;
	const G4double dScreeningGridOffsetZ = dScreeningGridRingOffsetZ+dGridRingHalfZ-dScreeningGridHalfZ;

	if(bUseScreeningGrid)
	{
		m_pScreeningGridRingPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dScreeningGridRingOffsetZ),
			m_pGridRingLogicalVolume, "ScreeningGridRing", m_pLXeLogicalVolume, false, 0);

		const G4double pdScreeningGridZPlanes[] = {-0.5*dScreeningGridWireDiameter, 0.5*dScreeningGridWireDiameter};

		const G4double pdScreeningGridInnerRadii[] = {0.5*(dScreeningGridWirePitch-dScreeningGridWireDiameter), 0.5*(dScreeningGridWirePitch-dScreeningGridWireDiameter)};

		const G4double pdScreeningGridOuterRadii[] = {0.5*dScreeningGridWirePitch, 0.5*dScreeningGridWirePitch};

		G4Polyhedra *pScreeningGridHexagonalCell = new G4Polyhedra("ScreeningGridHexagonalCell", 0.*deg, 360.*deg, 6, 2,
			pdScreeningGridZPlanes, pdScreeningGridInnerRadii, pdScreeningGridOuterRadii);

		m_pScreeningGridHexagonalCellLogicalVolume = new G4LogicalVolume(pScreeningGridHexagonalCell, GridSteel, "ScreeningGridHexagonalCellLogicalVolume", 0, 0, 0);

		iVolumeId = 1;
		iNbRings = 12;
		for(int iNa = -iNbRings+1; iNa < iNbRings; iNa++)
		{
			for(int iNb = -iNbRings+1; iNb < iNbRings; iNb++)
			{
				G4ThreeVector hPos(iNb*(0.5*sqrt(3.)*dScreeningGridWirePitch), iNa*(dScreeningGridWirePitch)+iNb*(0.5*dScreeningGridWirePitch), dScreeningGridOffsetZ);

				if((sqrt(pow(hPos.x()+0.5*dScreeningGridWirePitch,2)+pow(hPos.y(),2)) > dGridRingInnerRadius)
					|| (sqrt(pow(hPos.x()-0.5*dScreeningGridWirePitch,2)+pow(hPos.y(),2)) > dGridRingInnerRadius)
					|| (sqrt(pow(hPos.x()+0.5/sqrt(3.)*dScreeningGridWirePitch,2)+pow(hPos.y()+0.5*dScreeningGridWirePitch,2)) > dGridRingInnerRadius)
					|| (sqrt(pow(hPos.x()-0.5/sqrt(3.)*dScreeningGridWirePitch,2)+pow(hPos.y()+0.5*dScreeningGridWirePitch,2)) > dGridRingInnerRadius)
					|| (sqrt(pow(hPos.x()+0.5/sqrt(3.)*dScreeningGridWirePitch,2)+pow(hPos.y()-0.5*dScreeningGridWirePitch,2)) > dGridRingInnerRadius)
					|| (sqrt(pow(hPos.x()-0.5/sqrt(3.)*dScreeningGridWirePitch,2)+pow(hPos.y()-0.5*dScreeningGridWirePitch,2)) > dGridRingInnerRadius))
					continue;

				hVolumeName << "ScreeningGridHexagonalCell" << iVolumeId++;

				m_hScreeningGridPhysicalVolumes.push_back(new G4PVPlacement(0, hPos,
					m_pScreeningGridHexagonalCellLogicalVolume, hVolumeName.str(), m_pLXeLogicalVolume, false, 0));

				hVolumeName.str(""); hVolumeName.clear();
			}
		}
	}

	//=============================== optical surfaces ==============================
	G4OpticalSurface *pGridSteelOpticalSurface = new G4OpticalSurface("GridSteelOpticalSurface", unified, polished, dielectric_metal, 0.);

	pGridSteelOpticalSurface->SetMaterialPropertiesTable(GridSteel->GetMaterialPropertiesTable());
	
	vector<G4VPhysicalVolume *>::iterator pPhysicalVolume;

	int iSurfaceId;
	stringstream hSurfaceName;

	if(bUseTopGrid)
	{	
		iSurfaceId = 0;
		for(pPhysicalVolume = m_hTopGridPhysicalVolumes.begin(); pPhysicalVolume != m_hTopGridPhysicalVolumes.end(); pPhysicalVolume++)
		{
			hSurfaceName << "TopGridBorderSurface" << iSurfaceId++;		
			new G4LogicalBorderSurface(hSurfaceName.str(), m_pGXePhysicalVolume, *pPhysicalVolume, pGridSteelOpticalSurface);
			hSurfaceName.str(""); hSurfaceName.clear();
		}
	}

	iSurfaceId = 0;
	for(pPhysicalVolume = m_hAnodeGridPhysicalVolumes.begin(); pPhysicalVolume != m_hAnodeGridPhysicalVolumes.end(); pPhysicalVolume++)
	{
		hSurfaceName << "AnodeGridBorderSurface" << iSurfaceId++;		
		new G4LogicalBorderSurface(hSurfaceName.str(), m_pGXePhysicalVolume, *pPhysicalVolume, pGridSteelOpticalSurface);
		hSurfaceName.str(""); hSurfaceName.clear();
	}

	iSurfaceId = 0;
	for(pPhysicalVolume = m_hBottomGridPhysicalVolumes.begin(); pPhysicalVolume != m_hBottomGridPhysicalVolumes.end(); pPhysicalVolume++)
	{
		hSurfaceName << "BottomGridBorderSurface" << iSurfaceId++;		
		new G4LogicalBorderSurface(hSurfaceName.str(), m_pLXePhysicalVolume, *pPhysicalVolume, pGridSteelOpticalSurface);
		hSurfaceName.str(""); hSurfaceName.clear();
	}

	iSurfaceId = 0;
	for(pPhysicalVolume = m_hCathodeGridPhysicalVolumes.begin(); pPhysicalVolume != m_hCathodeGridPhysicalVolumes.end(); pPhysicalVolume++)
	{
		hSurfaceName << "CathodeGridBorderSurface" << iSurfaceId++;		
		new G4LogicalBorderSurface(hSurfaceName.str(), m_pLXePhysicalVolume, *pPhysicalVolume, pGridSteelOpticalSurface);
		hSurfaceName.str(""); hSurfaceName.clear();
	}

	if(bUseScreeningGrid)
	{	
		iSurfaceId = 0;
		for(pPhysicalVolume = m_hScreeningGridPhysicalVolumes.begin(); pPhysicalVolume != m_hScreeningGridPhysicalVolumes.end(); pPhysicalVolume++)
		{
			hSurfaceName << "ScreeningGridBorderSurface" << iSurfaceId++;		
			new G4LogicalBorderSurface(hSurfaceName.str(), m_pLXePhysicalVolume, *pPhysicalVolume, pGridSteelOpticalSurface);
			hSurfaceName.str(""); hSurfaceName.clear();
		}
	}

	//================================== attributes =================================
	G4Colour hGridRingColor(0.04, 0.72, 0.0, 1.);
	G4VisAttributes *pGridRingVisAtt = new G4VisAttributes(hGridRingColor);
	pGridRingVisAtt->SetVisibility(true);
	m_pGridRingLogicalVolume->SetVisAttributes(pGridRingVisAtt);

	G4Colour hGridColor(0.42, 0.72, 0.0, 1.);
	G4VisAttributes *pGridVisAtt = new G4VisAttributes(hGridColor);
	pGridVisAtt->SetVisibility(true);
	if(bUseTopGrid)
		m_pTopGridHexagonalCellLogicalVolume->SetVisAttributes(pGridVisAtt);
	m_pAnodeGridHexagonalCellLogicalVolume->SetVisAttributes(pGridVisAtt);
	m_pBottomGridHexagonalCellLogicalVolume->SetVisAttributes(pGridVisAtt);
	m_pCathodeGridHexagonalCellLogicalVolume->SetVisAttributes(pGridVisAtt);
	if(bUseScreeningGrid)
		m_pScreeningGridHexagonalCellLogicalVolume->SetVisAttributes(pGridVisAtt);
}

void
NerixDetectorConstruction::ConstructPlates()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< plates >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dLXeHeight = GetGeometryParameter("LXeHeight");
	const G4double dGXeGapHeight = GetGeometryParameter("GXeGapHeight");

	const G4double dChamberTopWallHeight = GetGeometryParameter("ChamberTopWallHeight");
	const G4double dChamberFlangeThickness = GetGeometryParameter("ChamberFlangeThickness");
	const G4double dChamberMiddlePlateThickness = GetGeometryParameter("ChamberMiddlePlateThickness");
	const G4double dChamberWallHeight = GetGeometryParameter("ChamberWallHeight");
	const G4double dChamberBottomPlateThickness = GetGeometryParameter("ChamberBottomPlateThickness");

	const G4bool bUseTopGrid = (G4bool) GetGeometryParameter("UseTopGrid");
	const G4bool bUseScreeningGrid = (G4bool) GetGeometryParameter("UseScreeningGrid");

	const G4double dTeflonTopPmtHolderHeight = GetGeometryParameter("TeflonTopPmtHolderHeight");
	const G4double dTeflonTopPmtHolderSquareCutWidth = GetGeometryParameter("TeflonTopPmtHolderSquareCutWidth");
	const G4double dTeflonTopPmtHolderSquareCutDepth = GetGeometryParameter("TeflonTopPmtHolderSquareCutDepth");
	const G4double dTeflonTopPmtHolderTopStepHeight = GetGeometryParameter("TeflonTopPmtHolderTopStepHeight");
	const G4double dTeflonTopPmtHolderBottomStepHeight = GetGeometryParameter("TeflonTopPmtHolderBottomStepHeight");

	const G4double dTeflonTopGridSpacerHeight = GetGeometryParameter("TeflonTopGridSpacerHeight");
	const G4double dTeflonTopGridSpacerTopStepHeight = GetGeometryParameter("TeflonTopGridSpacerTopStepHeight");
	const G4double dTeflonTopGridSpacerBottomStepHeight = GetGeometryParameter("TeflonTopGridSpacerBottomStepHeight");

	const G4double dTeflonAnodeSpacerInnerStepHeight = GetGeometryParameter("TeflonAnodeSpacerInnerStepHeight");
	const G4double dTeflonAnodeSpacerGapHeight = GetGeometryParameter("TeflonAnodeSpacerGapHeight");

	const G4double dTeflonDriftRegionSpacerHeight = GetGeometryParameter("TeflonDriftRegionSpacerHeight");

	const G4double dTeflonScreeningGridSpacerHeight = GetGeometryParameter("TeflonScreeningGridSpacerHeight");
	const G4double dTeflonScreeningGridSpacerTopStepHeight = GetGeometryParameter("TeflonScreeningGridSpacerTopStepHeight");

	const G4double dTeflonBottomPmtHolderHeight = GetGeometryParameter("TeflonBottomPmtHolderHeight");
	const G4double dTeflonBottomPmtHolderTopStepHeight = GetGeometryParameter("TeflonBottomPmtHolderTopStepHeight");

	const G4double dTopPlateRadius = GetGeometryParameter("TopPlateRadius");
	const G4double dTopPlateThickness = GetGeometryParameter("TopPlateThickness");
	const G4double dTopPmtPlateRadius = GetGeometryParameter("TopPmtPlateRadius");
	const G4double dTopPmtPlateThickness = GetGeometryParameter("TopPmtPlateThickness");
	const G4double dTopPlateToTopPmtPlate = GetGeometryParameter("TopPlateToTopPmtPlate");

	const G4double dBottomPlateRadius = GetGeometryParameter("BottomPlateRadius");
	const G4double dBottomPlateInnerRadius = GetGeometryParameter("BottomPlateInnerRadius");
	const G4double dBottomPlateThickness = GetGeometryParameter("BottomPlateThickness");
	const G4double dBottomPmtPlateRadius = GetGeometryParameter("BottomPmtPlateRadius");
	const G4double dBottomPmtPlateThickness = GetGeometryParameter("BottomPmtPlateThickness");
	const G4double dBottomPlateToBottomPmtPlate = GetGeometryParameter("BottomPlateToBottomPmtPlate");

	G4Material *SS304LSteel = G4Material::GetMaterial("SS304LSteel");

	//================================== top plate ==================================
	const G4double dTopPlateHalfZ = 0.5*dTopPlateThickness;
	const G4double dTeflonTopPmtHolderOffsetZ = (bUseTopGrid)?(-0.5*(dChamberTopWallHeight+dChamberFlangeThickness-dChamberMiddlePlateThickness)-dChamberMiddlePlateThickness-dChamberWallHeight+dLXeHeight+dChamberBottomPlateThickness+dGXeGapHeight+dTeflonAnodeSpacerInnerStepHeight+dTeflonTopGridSpacerHeight-dTeflonTopGridSpacerBottomStepHeight-dTeflonTopGridSpacerTopStepHeight+0.5*dTeflonTopPmtHolderHeight-dTeflonTopPmtHolderTopStepHeight-dTeflonTopPmtHolderBottomStepHeight):(-0.5*(dChamberTopWallHeight+dChamberFlangeThickness-dChamberMiddlePlateThickness)-dChamberMiddlePlateThickness-dChamberWallHeight+dLXeHeight+dChamberBottomPlateThickness+dGXeGapHeight+dTeflonAnodeSpacerInnerStepHeight+0.5*dTeflonTopPmtHolderHeight-dTeflonTopPmtHolderTopStepHeight-dTeflonTopPmtHolderBottomStepHeight);
	const G4double dTopPlateOffsetZ = dTeflonTopPmtHolderOffsetZ+0.5*dTeflonTopPmtHolderHeight+dTopPlateHalfZ;

	const G4double dTopPlateSquareCutHalfX = 0.5*dTeflonTopPmtHolderSquareCutWidth;
	const G4double dTopPlateSquareCutHalfY = dTopPlateSquareCutHalfX;
	const G4double dTopPlateSquareCutHalfZ = dTeflonTopPmtHolderSquareCutDepth;

	G4Tubs *pTopPlateTube = new G4Tubs("TopPlateTube", 0., dTopPlateRadius, dTopPlateHalfZ, 0.*deg, 360.*deg);

	G4Box *pTopPlateSquareCutBox = new G4Box("TopPlateSquareCutBox",
		dTopPlateSquareCutHalfX, dTopPlateSquareCutHalfY, dTopPlateSquareCutHalfZ);

	G4SubtractionSolid *pTopPlateWithCut1 = new G4SubtractionSolid("TopPlateWithCut1",
		pTopPlateTube, pTopPlateSquareCutBox, 0, G4ThreeVector(0., 0., dTopPlateHalfZ));

	m_pTopPlateLogicalVolume = new G4LogicalVolume(pTopPlateWithCut1, SS304LSteel, "TopPlateLogicalVolume", 0, 0, 0);

	m_pTopPlatePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dTopPlateOffsetZ),
		m_pTopPlateLogicalVolume, "TopPlate", m_pGXeLogicalVolume, false, 0);

	//================================ top pmt plate ================================
	const G4double dTopPmtPlateHalfZ = 0.5*dTopPmtPlateThickness;
	const G4double dTopPmtPlateOffsetZ = dTopPlateOffsetZ+dTopPlateHalfZ+dTopPlateToTopPmtPlate+dTopPmtPlateHalfZ;

	G4Tubs *pTopPmtPlateTube = new G4Tubs("TopPmtPlateTube", 0., dTopPmtPlateRadius, dTopPmtPlateHalfZ, 0.*deg, 360.*deg);

	m_pTopPmtPlateLogicalVolume = new G4LogicalVolume(pTopPmtPlateTube, SS304LSteel, "TopPmtPlateLogicalVolume", 0, 0, 0);

	m_pTopPmtPlatePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dTopPmtPlateOffsetZ),
		m_pTopPmtPlateLogicalVolume, "TopPmtPlate", m_pGXeLogicalVolume, false, 0);

	//================================ bottom plate =================================
	const G4double dBottomPlateHalfZ = 0.5*dBottomPlateThickness;
	const G4double dBottomPlateOffsetZ = (bUseScreeningGrid)?(0.5*dLXeHeight-(dTeflonAnodeSpacerGapHeight-dGXeGapHeight)-dTeflonDriftRegionSpacerHeight-(dTeflonScreeningGridSpacerHeight-dTeflonScreeningGridSpacerTopStepHeight)-(dTeflonBottomPmtHolderHeight-dTeflonBottomPmtHolderTopStepHeight)-dBottomPlateHalfZ):(0.5*dLXeHeight-(dTeflonAnodeSpacerGapHeight-dGXeGapHeight)-dTeflonDriftRegionSpacerHeight-(dTeflonBottomPmtHolderHeight-dTeflonBottomPmtHolderTopStepHeight)-dBottomPlateHalfZ);

	G4Tubs *pBottomPlateTube = new G4Tubs("BottomPlateTube", dBottomPlateInnerRadius, dBottomPlateRadius, dBottomPlateHalfZ, 0.*deg, 360.*deg);

	m_pBottomPlateLogicalVolume = new G4LogicalVolume(pBottomPlateTube, SS304LSteel, "BottomPlateLogicalVolume", 0, 0, 0);

	m_pBottomPlatePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dBottomPlateOffsetZ),
		m_pBottomPlateLogicalVolume, "BottomPlate", m_pLXeLogicalVolume, false, 0);

	//============================== bottom pmt plate ===============================
	const G4double dBottomPmtPlateHalfZ = 0.5*dBottomPmtPlateThickness;
	const G4double dBottomPmtPlateOffsetZ = dBottomPlateOffsetZ-dBottomPlateHalfZ-dBottomPlateToBottomPmtPlate-dBottomPmtPlateHalfZ;

	G4Tubs *pBottomPmtPlateTube = new G4Tubs("BottomPmtPlateTube", 0., dBottomPmtPlateRadius, dBottomPmtPlateHalfZ, 0.*deg, 360.*deg);

	m_pBottomPmtPlateLogicalVolume = new G4LogicalVolume(pBottomPmtPlateTube, SS304LSteel, "BottomPmtPlateLogicalVolume", 0, 0, 0);

	m_pBottomPmtPlatePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dBottomPmtPlateOffsetZ),
		m_pBottomPmtPlateLogicalVolume, "BottomPmtPlate", m_pLXeLogicalVolume, false, 0);

	//=============================== optical surfaces ==============================
	G4OpticalSurface *pSS304LSteelOpticalSurface = new G4OpticalSurface("SS304LSteelOpticalSurface", unified, polished, dielectric_metal, 0.);

	pSS304LSteelOpticalSurface->SetMaterialPropertiesTable(SS304LSteel->GetMaterialPropertiesTable());
	
	new G4LogicalBorderSurface("TopPlateBorderSurface", m_pGXePhysicalVolume, m_pTopPlatePhysicalVolume, pSS304LSteelOpticalSurface);
	new G4LogicalBorderSurface("TopPmtPlateBorderSurface", m_pGXePhysicalVolume, m_pTopPmtPlatePhysicalVolume, pSS304LSteelOpticalSurface);
	new G4LogicalBorderSurface("BottomPlateBorderSurface", m_pLXePhysicalVolume, m_pBottomPlatePhysicalVolume, pSS304LSteelOpticalSurface);
	new G4LogicalBorderSurface("BottomPmtPlateBorderSurface", m_pLXePhysicalVolume, m_pBottomPmtPlatePhysicalVolume, pSS304LSteelOpticalSurface);

	//================================== attributes =================================
	G4Colour hPlateColor(0.75, 0.21, 0.93, 1.);
	G4VisAttributes *pPlateVisAtt = new G4VisAttributes(hPlateColor);
	pPlateVisAtt->SetVisibility(true);
	m_pTopPlateLogicalVolume->SetVisAttributes(pPlateVisAtt);
	m_pTopPmtPlateLogicalVolume->SetVisAttributes(pPlateVisAtt);
	m_pBottomPlateLogicalVolume->SetVisAttributes(pPlateVisAtt);
	m_pBottomPmtPlateLogicalVolume->SetVisAttributes(pPlateVisAtt);
}

void
NerixDetectorConstruction::ConstructPmts()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< pmts >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dLXeHeight = GetGeometryParameter("LXeHeight");
	const G4double dGXeGapHeight = GetGeometryParameter("GXeGapHeight");

	const G4double dChamberTopWallHeight = GetGeometryParameter("ChamberTopWallHeight");
	const G4double dChamberWallHeight = GetGeometryParameter("ChamberWallHeight");
	const G4double dChamberFlangeThickness = GetGeometryParameter("ChamberFlangeThickness");
	const G4double dChamberMiddlePlateThickness = GetGeometryParameter("ChamberMiddlePlateThickness");
	const G4double dChamberBottomPlateThickness = GetGeometryParameter("ChamberBottomPlateThickness");

	const G4bool bUseTopGrid = (G4bool) GetGeometryParameter("UseTopGrid");
	const G4bool bUseScreeningGrid = (G4bool) GetGeometryParameter("UseScreeningGrid");

	const G4double dTeflonTopPmtHolderHeight = GetGeometryParameter("TeflonTopPmtHolderHeight");
	const G4double dTeflonTopPmtHolderTopStepHeight = GetGeometryParameter("TeflonTopPmtHolderTopStepHeight");
	const G4double dTeflonTopPmtHolderBottomStepHeight = GetGeometryParameter("TeflonTopPmtHolderBottomStepHeight");
	const G4double dTeflonTopGridSpacerHeight = GetGeometryParameter("TeflonTopGridSpacerHeight");
	const G4double dTeflonTopGridSpacerBottomStepHeight = GetGeometryParameter("TeflonTopGridSpacerBottomStepHeight");
	const G4double dTeflonTopGridSpacerTopStepHeight = GetGeometryParameter("TeflonTopGridSpacerTopStepHeight");
	const G4double dTeflonAnodeSpacerGapHeight = GetGeometryParameter("TeflonAnodeSpacerGapHeight");
	const G4double dTeflonAnodeSpacerInnerStepHeight = GetGeometryParameter("TeflonAnodeSpacerInnerStepHeight");
	const G4double dTeflonDriftRegionSpacerHeight = GetGeometryParameter("TeflonDriftRegionSpacerHeight");
	const G4double dTeflonScreeningGridSpacerHeight = GetGeometryParameter("TeflonScreeningGridSpacerHeight");
	const G4double dTeflonScreeningGridSpacerTopStepHeight = GetGeometryParameter("TeflonScreeningGridSpacerTopStepHeight");
	const G4double dTeflonBottomPmtHolderHeight = GetGeometryParameter("TeflonBottomPmtHolderHeight");
	const G4double dTeflonBottomPmtHolderTopStepHeight = GetGeometryParameter("TeflonBottomPmtHolderTopStepHeight");

	const G4double dTopPlateThickness = GetGeometryParameter("TopPlateThickness");
	const G4double dTopPlateToTopPmtPlate = GetGeometryParameter("TopPlateToTopPmtPlate");
	const G4double dTopPmtPlateThickness = GetGeometryParameter("TopPmtPlateThickness");
	const G4double dBottomPlateThickness = GetGeometryParameter("BottomPlateThickness");
	const G4double dBottomPlateToBottomPmtPlate = GetGeometryParameter("BottomPlateToBottomPmtPlate");

	const G4double dTopPmtWindowWidth = GetGeometryParameter("TopPmtWindowWidth");
	const G4double dTopPmtWindowThickness = GetGeometryParameter("TopPmtWindowThickness");

	const G4double dTopPmtCasingWidth = GetGeometryParameter("TopPmtCasingWidth");
	const G4double dTopPmtCasingHeight = GetGeometryParameter("TopPmtCasingHeight");
	const G4double dTopPmtCasingThickness = GetGeometryParameter("TopPmtCasingThickness");

	const G4double dTopPmtSealWidth = GetGeometryParameter("TopPmtSealWidth");
	const G4double dTopPmtSealThickness = GetGeometryParameter("TopPmtSealThickness");

	const G4double dTopPmtPhotoCathodeWidth = GetGeometryParameter("TopPmtPhotoCathodeWidth");
	const G4double dTopPmtPhotoCathodeThickness = GetGeometryParameter("TopPmtPhotoCathodeThickness");

	const G4double dTopPmtBaseThickness = GetGeometryParameter("TopPmtBaseThickness");
	const G4double dTopPmtToPmtBase = GetGeometryParameter("TopPmtToPmtBase");

	const G4double dTopPmtTeflonSpacerThickness = GetGeometryParameter("TopPmtTeflonSpacerThickness");

	const G4double dTopPmtXSpacing = GetGeometryParameter("TopPmtXSpacing");
	const G4double dTopPmtYSpacing = GetGeometryParameter("TopPmtYSpacing");

	const G4double dBottomPmtWindowRadius = GetGeometryParameter("BottomPmtWindowRadius");
	const G4double dBottomPmtWindowThickness = GetGeometryParameter("BottomPmtWindowThickness");

	const G4double dBottomPmtCasingRadius = GetGeometryParameter("BottomPmtCasingRadius");
	const G4double dBottomPmtCasingHeight = GetGeometryParameter("BottomPmtCasingHeight");
	const G4double dBottomPmtCasingThickness = GetGeometryParameter("BottomPmtCasingThickness");

	const G4double dBottomPmtSealRadius = GetGeometryParameter("BottomPmtSealRadius");
	const G4double dBottomPmtSealThickness = GetGeometryParameter("BottomPmtSealThickness");

	const G4double dBottomPmtPhotoCathodeRadius = GetGeometryParameter("BottomPmtPhotoCathodeRadius");
	const G4double dBottomPmtPhotoCathodeThickness = GetGeometryParameter("BottomPmtPhotoCathodeThickness");

	const G4double dBottomPmtBaseRadius = GetGeometryParameter("BottomPmtBaseRadius");
	const G4double dBottomPmtBaseInnerRadius = GetGeometryParameter("BottomPmtBaseInnerRadius");
	const G4double dBottomPmtBaseThickness = GetGeometryParameter("BottomPmtBaseThickness");
	const G4double dBottomPmtToPmtBase = GetGeometryParameter("BottomPmtToPmtBase");

	const G4double dBottomPmtTeflonSpacerRadius = GetGeometryParameter("BottomPmtTeflonSpacerRadius");
	const G4double dBottomPmtTeflonSpacerThickness = GetGeometryParameter("BottomPmtTeflonSpacerThickness");

	G4Material *Quartz = G4Material::GetMaterial("Quartz");
	G4Material *SS304LSteel = G4Material::GetMaterial("SS304LSteel");
	G4Material *Vacuum = G4Material::GetMaterial("Vacuum");
	G4Material *PhotoCathodeAluminium = G4Material::GetMaterial("PhotoCathodeAluminium");
	G4Material *Cirlex = G4Material::GetMaterial("Cirlex");
	G4Material *Teflon = G4Material::GetMaterial("Teflon");	
	G4Material *Aluminium = G4Material::GetMaterial("G4_Al");	

	//================================== top pmts ===================================
	const G4double dTopPmtPlateOffsetZ = (bUseTopGrid)?(-0.5*(dChamberTopWallHeight+dChamberFlangeThickness-dChamberMiddlePlateThickness)-dChamberMiddlePlateThickness-dChamberWallHeight+dLXeHeight+dChamberBottomPlateThickness+dGXeGapHeight+dTeflonAnodeSpacerInnerStepHeight+dTeflonTopGridSpacerHeight-dTeflonTopGridSpacerBottomStepHeight-dTeflonTopGridSpacerTopStepHeight+dTeflonTopPmtHolderHeight-dTeflonTopPmtHolderTopStepHeight-dTeflonTopPmtHolderBottomStepHeight+dTopPlateThickness+dTopPlateToTopPmtPlate+0.5*dTopPmtPlateThickness):(-0.5*(dChamberTopWallHeight+dChamberFlangeThickness-dChamberMiddlePlateThickness)-dChamberMiddlePlateThickness-dChamberWallHeight+dLXeHeight+dChamberBottomPlateThickness+dGXeGapHeight+dTeflonAnodeSpacerInnerStepHeight+dTeflonTopPmtHolderHeight-dTeflonTopPmtHolderTopStepHeight-dTeflonTopPmtHolderBottomStepHeight+dTopPlateThickness+dTopPlateToTopPmtPlate+0.5*dTopPmtPlateThickness);

	const G4double dTopPmtWindowHalfX = 0.5*dTopPmtWindowWidth;
	const G4double dTopPmtWindowHalfY = dTopPmtWindowHalfX;
	const G4double dTopPmtWindowHalfZ = 0.5*dTopPmtWindowThickness;

	const G4double dTopPmtWindowOffsetZ = dTopPmtPlateOffsetZ-0.5*dTopPmtPlateThickness-dTopPmtTeflonSpacerThickness-dTopPmtBaseThickness-dTopPmtToPmtBase-dTopPmtCasingHeight-dTopPmtWindowHalfZ;
	const G4double dTopPmt1WindowOffsetX = -dTopPmtWindowHalfX-0.5*dTopPmtXSpacing;
	const G4double dTopPmt1WindowOffsetY = +dTopPmtWindowHalfY+0.5*dTopPmtYSpacing;
	const G4double dTopPmt2WindowOffsetX = +dTopPmtWindowHalfX+0.5*dTopPmtXSpacing;
	const G4double dTopPmt2WindowOffsetY = +dTopPmtWindowHalfY+0.5*dTopPmtYSpacing;
	const G4double dTopPmt3WindowOffsetX = -dTopPmtWindowHalfX-0.5*dTopPmtXSpacing;
	const G4double dTopPmt3WindowOffsetY = -dTopPmtWindowHalfY-0.5*dTopPmtYSpacing;
	const G4double dTopPmt4WindowOffsetX = +dTopPmtWindowHalfX+0.5*dTopPmtXSpacing;
	const G4double dTopPmt4WindowOffsetY = -dTopPmtWindowHalfY-0.5*dTopPmtYSpacing;

	const G4double dTopPmtCasingHalfX = 0.5*dTopPmtCasingWidth;
	const G4double dTopPmtCasingHalfY = dTopPmtCasingHalfX;
	const G4double dTopPmtCasingHalfZ = 0.5*dTopPmtCasingHeight;
 
	const G4double dTopPmtCasingOffsetZ = dTopPmtWindowOffsetZ+dTopPmtWindowHalfZ+dTopPmtCasingHalfZ;
	const G4double dTopPmt1CasingOffsetX = dTopPmt1WindowOffsetX;
	const G4double dTopPmt1CasingOffsetY = dTopPmt1WindowOffsetY;
	const G4double dTopPmt2CasingOffsetX = dTopPmt2WindowOffsetX;
	const G4double dTopPmt2CasingOffsetY = dTopPmt2WindowOffsetY;
	const G4double dTopPmt3CasingOffsetX = dTopPmt3WindowOffsetX;
	const G4double dTopPmt3CasingOffsetY = dTopPmt3WindowOffsetY;
	const G4double dTopPmt4CasingOffsetX = dTopPmt4WindowOffsetX;
	const G4double dTopPmt4CasingOffsetY = dTopPmt4WindowOffsetY;

	const G4double dTopPmtInteriorHalfX = dTopPmtCasingHalfX-dTopPmtCasingThickness;
	const G4double dTopPmtInteriorHalfY = dTopPmtCasingHalfY-dTopPmtCasingThickness;
	const G4double dTopPmtInteriorHalfZ = dTopPmtCasingHalfZ-dTopPmtCasingThickness;

	const G4double dTopPmtSealHalfX = 0.5*dTopPmtSealWidth;
	const G4double dTopPmtSealHalfY = dTopPmtSealHalfX;
	const G4double dTopPmtSealHalfZ = 0.5*dTopPmtSealThickness;

	const G4double dTopPmtSealOffsetZ = dTopPmtCasingHalfZ-dTopPmtSealHalfZ;	

	const G4double dTopPmtPhotoCathodeHalfX = 0.5*dTopPmtPhotoCathodeWidth;
	const G4double dTopPmtPhotoCathodeHalfY = dTopPmtPhotoCathodeHalfX;
	const G4double dTopPmtPhotoCathodeHalfZ = 0.5*dTopPmtPhotoCathodeThickness;

	const G4double dTopPmtBaseHalfX = 0.5*dTopPmtCasingWidth;
	const G4double dTopPmtBaseHalfY = dTopPmtBaseHalfX;
	const G4double dTopPmtBaseHalfZ = 0.5*dTopPmtBaseThickness;

	const G4double dTopPmtBaseOffsetZ = dTopPmtCasingOffsetZ+dTopPmtCasingHalfZ+dTopPmtToPmtBase+dTopPmtBaseHalfZ;
	const G4double dTopPmt1BaseOffsetX = dTopPmt1WindowOffsetX;
	const G4double dTopPmt1BaseOffsetY = dTopPmt1WindowOffsetY;
	const G4double dTopPmt2BaseOffsetX = dTopPmt2WindowOffsetX;
	const G4double dTopPmt2BaseOffsetY = dTopPmt2WindowOffsetY;
	const G4double dTopPmt3BaseOffsetX = dTopPmt3WindowOffsetX;
	const G4double dTopPmt3BaseOffsetY = dTopPmt3WindowOffsetY;
	const G4double dTopPmt4BaseOffsetX = dTopPmt4WindowOffsetX;
	const G4double dTopPmt4BaseOffsetY = dTopPmt4WindowOffsetY;

	const G4double dTopPmtTeflonSpacerHalfX = 0.5*dTopPmtCasingWidth;
	const G4double dTopPmtTeflonSpacerHalfY = dTopPmtTeflonSpacerHalfX;
	const G4double dTopPmtTeflonSpacerHalfZ = 0.5*dTopPmtTeflonSpacerThickness;

	const G4double dTopPmtTeflonSpacerOffsetZ = dTopPmtBaseOffsetZ+dTopPmtBaseHalfZ+dTopPmtTeflonSpacerHalfZ;
	const G4double dTopPmt1TeflonSpacerOffsetX = dTopPmt1WindowOffsetX;
	const G4double dTopPmt1TeflonSpacerOffsetY = dTopPmt1WindowOffsetY;
	const G4double dTopPmt2TeflonSpacerOffsetX = dTopPmt2WindowOffsetX;
	const G4double dTopPmt2TeflonSpacerOffsetY = dTopPmt2WindowOffsetY;
	const G4double dTopPmt3TeflonSpacerOffsetX = dTopPmt3WindowOffsetX;
	const G4double dTopPmt3TeflonSpacerOffsetY = dTopPmt3WindowOffsetY;
	const G4double dTopPmt4TeflonSpacerOffsetX = dTopPmt4WindowOffsetX;
	const G4double dTopPmt4TeflonSpacerOffsetY = dTopPmt4WindowOffsetY;

	G4Box *pTopPmtWindowBox = new G4Box("TopPmtWindowBox", dTopPmtWindowHalfX, dTopPmtWindowHalfY, dTopPmtWindowHalfZ);
	G4Box *pTopPmtCasingBox = new G4Box("TopPmtCasingBox", dTopPmtCasingHalfX, dTopPmtCasingHalfY, dTopPmtCasingHalfZ);
	G4Box *pTopPmtInteriorBox = new G4Box("TopPmtInteriorBox", dTopPmtInteriorHalfX, dTopPmtInteriorHalfY, dTopPmtInteriorHalfZ);
	G4Box *pTopPmtSealBox = new G4Box("TopPmtSealBox", dTopPmtSealHalfX, dTopPmtSealHalfY, dTopPmtSealHalfZ);
	G4Box *pTopPmtPhotoCathodeBox = new G4Box("TopPmtPhotoCathodeBox", dTopPmtPhotoCathodeHalfX, dTopPmtPhotoCathodeHalfY, dTopPmtPhotoCathodeHalfZ);
	G4Box *pTopPmtBaseBox = new G4Box("TopPmtBaseBox", dTopPmtBaseHalfX, dTopPmtBaseHalfY, dTopPmtBaseHalfZ);
	G4Box *pTopPmtTeflonSpacerBox = new G4Box("TopPmtTeflonSpacerBox", dTopPmtTeflonSpacerHalfX, dTopPmtTeflonSpacerHalfY, dTopPmtTeflonSpacerHalfZ);

	m_pTopPmtWindowLogicalVolume = new G4LogicalVolume(pTopPmtWindowBox, Quartz, "TopPmtWindowLogicalVolume", 0, 0, 0);
	m_pTopPmtCasingLogicalVolume = new G4LogicalVolume(pTopPmtCasingBox, SS304LSteel, "TopPmtCasingLogicalVolume", 0, 0, 0);
	m_pTopPmtInteriorLogicalVolume = new G4LogicalVolume(pTopPmtInteriorBox, Vacuum, "TopPmtInteriorLogicalVolume", 0, 0, 0);
	m_pTopPmtSealLogicalVolume = new G4LogicalVolume(pTopPmtSealBox, Aluminium, "TopPmtSealLogicalVolume", 0, 0, 0);
	m_pTopPmtPhotoCathodeLogicalVolume = new G4LogicalVolume(pTopPmtPhotoCathodeBox, PhotoCathodeAluminium, "TopPmtPhotoCathodeLogicalVolume", 0, 0, 0);
	m_pTopPmtBaseLogicalVolume = new G4LogicalVolume(pTopPmtBaseBox, Cirlex, "TopPmtBaseLogicalVolume", 0, 0, 0);
	m_pTopPmtTeflonSpacerLogicalVolume = new G4LogicalVolume(pTopPmtTeflonSpacerBox, Teflon, "TopPmtTeflonSpacerLogicalVolume", 0, 0, 0);

	m_pTopPmtInteriorPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
		m_pTopPmtInteriorLogicalVolume, "TopPmtInterior", m_pTopPmtCasingLogicalVolume, false, 0);
	m_pTopPmtSealPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., -dTopPmtSealOffsetZ),
		m_pTopPmtSealLogicalVolume, "TopPmtSeal", m_pTopPmtCasingLogicalVolume, false, 0);
	m_pTopPmtPhotoCathodePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., 0),
		m_pTopPmtPhotoCathodeLogicalVolume, "TopPmtPhotoCathode", m_pTopPmtSealLogicalVolume, false, 0);

	//------------------------------------ pmt 1 ------------------------------------
	m_hTopPmtWindowPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt1WindowOffsetX, dTopPmt1WindowOffsetY, dTopPmtWindowOffsetZ),
		m_pTopPmtWindowLogicalVolume, "TopPmtWindowNo1", m_pGXeLogicalVolume, false, 0));

	m_hTopPmtCasingPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt1CasingOffsetX, dTopPmt1CasingOffsetY, dTopPmtCasingOffsetZ),
		m_pTopPmtCasingLogicalVolume, "TopPmtCasingNo1", m_pGXeLogicalVolume, false, 0));

	m_hTopPmtBasePhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt1BaseOffsetX, dTopPmt1BaseOffsetY, dTopPmtBaseOffsetZ),
		m_pTopPmtBaseLogicalVolume, "TopPmtBaseNo1", m_pGXeLogicalVolume, false, 0));

	m_hTopPmtTeflonSpacerPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt1TeflonSpacerOffsetX, dTopPmt1TeflonSpacerOffsetY, dTopPmtTeflonSpacerOffsetZ),
		m_pTopPmtTeflonSpacerLogicalVolume, "TopPmtTeflonSpacerNo1", m_pGXeLogicalVolume, false, 0));

	//------------------------------------ pmt 2 ------------------------------------
	m_hTopPmtWindowPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt2WindowOffsetX, dTopPmt2WindowOffsetY, dTopPmtWindowOffsetZ),
		m_pTopPmtWindowLogicalVolume, "TopPmtWindowNo2", m_pGXeLogicalVolume, false, 1));

	m_hTopPmtCasingPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt2CasingOffsetX, dTopPmt2CasingOffsetY, dTopPmtCasingOffsetZ),
		m_pTopPmtCasingLogicalVolume, "TopPmtCasingNo2", m_pGXeLogicalVolume, false, 1));

	m_hTopPmtBasePhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt2BaseOffsetX, dTopPmt2BaseOffsetY, dTopPmtBaseOffsetZ),
		m_pTopPmtBaseLogicalVolume, "TopPmtBaseNo2", m_pGXeLogicalVolume, false, 1));

	m_hTopPmtTeflonSpacerPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt2TeflonSpacerOffsetX, dTopPmt2TeflonSpacerOffsetY, dTopPmtTeflonSpacerOffsetZ),
		m_pTopPmtTeflonSpacerLogicalVolume, "TopPmtTeflonSpacerNo2", m_pGXeLogicalVolume, false, 1));

	//------------------------------------ pmt 3 ------------------------------------
	m_hTopPmtWindowPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt3WindowOffsetX, dTopPmt3WindowOffsetY, dTopPmtWindowOffsetZ),
		m_pTopPmtWindowLogicalVolume, "TopPmtWindowNo3", m_pGXeLogicalVolume, false, 2));

	m_hTopPmtCasingPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt3CasingOffsetX, dTopPmt3CasingOffsetY, dTopPmtCasingOffsetZ),
		m_pTopPmtCasingLogicalVolume, "TopPmtCasingNo3", m_pGXeLogicalVolume, false, 2));

	m_hTopPmtBasePhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt3BaseOffsetX, dTopPmt3BaseOffsetY, dTopPmtBaseOffsetZ),
		m_pTopPmtBaseLogicalVolume, "TopPmtBaseNo3", m_pGXeLogicalVolume, false, 2));

	m_hTopPmtTeflonSpacerPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt3TeflonSpacerOffsetX, dTopPmt3TeflonSpacerOffsetY, dTopPmtTeflonSpacerOffsetZ),
		m_pTopPmtTeflonSpacerLogicalVolume, "TopPmtTeflonSpacerNo3", m_pGXeLogicalVolume, false, 2));

	//------------------------------------ pmt 4 ------------------------------------
	m_hTopPmtWindowPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt4WindowOffsetX, dTopPmt4WindowOffsetY, dTopPmtWindowOffsetZ),
		m_pTopPmtWindowLogicalVolume, "TopPmtWindowNo4", m_pGXeLogicalVolume, false, 3));

	m_hTopPmtCasingPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt4CasingOffsetX, dTopPmt4CasingOffsetY, dTopPmtCasingOffsetZ),
		m_pTopPmtCasingLogicalVolume, "TopPmtCasingNo4", m_pGXeLogicalVolume, false, 3));

	m_hTopPmtBasePhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt4BaseOffsetX, dTopPmt4BaseOffsetY, dTopPmtBaseOffsetZ),
		m_pTopPmtBaseLogicalVolume, "TopPmtBaseNo4", m_pGXeLogicalVolume, false, 3));

	m_hTopPmtTeflonSpacerPhysicalVolumes.push_back(new G4PVPlacement(0,
		G4ThreeVector(dTopPmt4TeflonSpacerOffsetX, dTopPmt4TeflonSpacerOffsetY, dTopPmtTeflonSpacerOffsetZ),
		m_pTopPmtTeflonSpacerLogicalVolume, "TopPmtTeflonSpacerNo4", m_pGXeLogicalVolume, false, 3));

	//================================= bottom pmt ==================================
	const G4double dBottomPmtTeflonSpacerHalfZ = 0.5*dBottomPmtTeflonSpacerThickness;
	const G4double dBottomPmtTeflonSpacerOffsetZ = (bUseScreeningGrid)?(0.5*dLXeHeight-(dTeflonAnodeSpacerGapHeight-dGXeGapHeight)-dTeflonDriftRegionSpacerHeight-(dTeflonScreeningGridSpacerHeight-dTeflonScreeningGridSpacerTopStepHeight)-(dTeflonBottomPmtHolderHeight-dTeflonBottomPmtHolderTopStepHeight)-dBottomPlateThickness-dBottomPlateToBottomPmtPlate+dBottomPmtTeflonSpacerHalfZ):(0.5*dLXeHeight-(dTeflonAnodeSpacerGapHeight-dGXeGapHeight)-dTeflonDriftRegionSpacerHeight-(dTeflonBottomPmtHolderHeight-dTeflonBottomPmtHolderTopStepHeight)-dBottomPlateThickness-dBottomPlateToBottomPmtPlate+dBottomPmtTeflonSpacerHalfZ);

	const G4double dBottomPmtBaseHalfZ = 0.5*dBottomPmtBaseThickness;
	const G4double dBottomPmtBaseOffsetZ = dBottomPmtTeflonSpacerOffsetZ+dBottomPmtTeflonSpacerHalfZ+dBottomPmtBaseHalfZ;

	const G4double dBottomPmtCasingHalfZ = 0.5*dBottomPmtCasingHeight;
	const G4double dBottomPmtCasingOffsetZ = dBottomPmtBaseOffsetZ+dBottomPmtBaseHalfZ+dBottomPmtToPmtBase+dBottomPmtCasingHalfZ;

	const G4double dBottomPmtInteriorRadius = dBottomPmtCasingRadius-dBottomPmtCasingThickness;
	const G4double dBottomPmtInteriorHalfZ = dBottomPmtCasingHalfZ-dBottomPmtCasingThickness;

	const G4double dBottomPmtSealHalfZ = 0.5*dBottomPmtSealThickness;
	const G4double dBottomPmtSealOffsetZ = dBottomPmtCasingHalfZ-dBottomPmtSealHalfZ;

	const G4double dBottomPmtPhotoCathodeHalfZ = 0.5*dBottomPmtPhotoCathodeThickness;

	const G4double dBottomPmtWindowHalfZ = 0.5*dBottomPmtWindowThickness;
	const G4double dBottomPmtWindowOffsetZ = dBottomPmtCasingOffsetZ+dBottomPmtCasingHalfZ+dBottomPmtWindowHalfZ;

	//--------------------------- bottom pmt teflon spacer --------------------------
	G4Tubs *pBottomPmtTeflonSpacerTube = new G4Tubs("BottomPmtTeflonSpacerTube", 0., dBottomPmtTeflonSpacerRadius, dBottomPmtTeflonSpacerHalfZ, 0.*deg, 360.*deg);

	m_pBottomPmtTeflonSpacerLogicalVolume = new G4LogicalVolume(pBottomPmtTeflonSpacerTube, Teflon, "BottomPmtTeflonSpacerLogicalVolume", 0, 0, 0);

	m_pBottomPmtTeflonSpacerPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dBottomPmtTeflonSpacerOffsetZ),
		m_pBottomPmtTeflonSpacerLogicalVolume, "BottomPmtTeflonSpacer", m_pLXeLogicalVolume, false, 4);

	//------------------------------- bottom pmt base -------------------------------
	G4Tubs *pBottomPmtBaseTube = new G4Tubs("BottomPmtBaseTube", dBottomPmtBaseInnerRadius, dBottomPmtBaseRadius, dBottomPmtBaseHalfZ, 0.*deg, 360.*deg);

	m_pBottomPmtBaseLogicalVolume = new G4LogicalVolume(pBottomPmtBaseTube, Teflon, "BottomPmtBaseLogicalVolume", 0, 0, 0);

	m_pBottomPmtBasePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dBottomPmtBaseOffsetZ),
		m_pBottomPmtBaseLogicalVolume, "BottomPmtBase", m_pLXeLogicalVolume, false, 4);

	//--------------------------------- bottom pmt ----------------------------------
	G4Tubs *pBottomPmtCasingTube = new G4Tubs("BottomPmtCasingTube", 0., dBottomPmtCasingRadius, dBottomPmtCasingHalfZ, 0.*deg, 360.*deg);
	G4Tubs *pBottomPmtInteriorTube = new G4Tubs("BottomPmtInteriorTube", 0., dBottomPmtInteriorRadius, dBottomPmtInteriorHalfZ, 0.*deg, 360.*deg);
	G4Tubs *pBottomPmtSealTube = new G4Tubs("BottomPmtSealTube", 0., dBottomPmtSealRadius, dBottomPmtSealHalfZ, 0.*deg, 360.*deg);
	G4Tubs *pBottomPmtPhotoCathodeTube = new G4Tubs("BottomPmtPhotoCathodeTube", 0., dBottomPmtPhotoCathodeRadius, dBottomPmtPhotoCathodeHalfZ, 0.*deg, 360.*deg);
	G4Tubs *pBottomPmtWindowTube = new G4Tubs("BottomPmtWindowTube", 0., dBottomPmtWindowRadius, dBottomPmtWindowHalfZ, 0.*deg, 360.*deg);

	m_pBottomPmtCasingLogicalVolume = new G4LogicalVolume(pBottomPmtCasingTube, SS304LSteel, "BottomPmtCasingLogicalVolume", 0, 0, 0);
	m_pBottomPmtInteriorLogicalVolume = new G4LogicalVolume(pBottomPmtInteriorTube, Vacuum, "BottomPmtInteriorLogicalVolume", 0, 0, 0);
	m_pBottomPmtSealLogicalVolume = new G4LogicalVolume(pBottomPmtSealTube, Aluminium, "BottomPmtSealLogicalVolume", 0, 0, 0);
	m_pBottomPmtPhotoCathodeLogicalVolume = new G4LogicalVolume(pBottomPmtPhotoCathodeTube, PhotoCathodeAluminium, "BottomPmtPhotoCathodeLogicalVolume", 0, 0, 0);
	m_pBottomPmtWindowLogicalVolume = new G4LogicalVolume(pBottomPmtWindowTube, Quartz, "BottomPmtWindowLogicalVolume", 0, 0, 0);

	m_pBottomPmtCasingPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dBottomPmtCasingOffsetZ),
		m_pBottomPmtCasingLogicalVolume, "BottomPmtCasing", m_pLXeLogicalVolume, false, 4);
	m_pBottomPmtInteriorPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
		m_pBottomPmtInteriorLogicalVolume, "BottomPmtInterior", m_pBottomPmtCasingLogicalVolume, false, 4);
	m_pBottomPmtSealPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dBottomPmtSealOffsetZ),
		m_pBottomPmtSealLogicalVolume, "BottomPmtSeal", m_pBottomPmtCasingLogicalVolume, false, 4);
	m_pBottomPmtPhotoCathodePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
		m_pBottomPmtPhotoCathodeLogicalVolume, "BottomPmtPhotoCathode", m_pBottomPmtSealLogicalVolume, false, 4);
	m_pBottomPmtWindowPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., dBottomPmtWindowOffsetZ),
		m_pBottomPmtWindowLogicalVolume, "BottomPmtWindow", m_pLXeLogicalVolume, false, 4);

	//=============================== pmt sensitivity ===============================
	G4SDManager *pSDManager = G4SDManager::GetSDMpointer();

	NerixPmtSensitiveDetector *pPmtSD = new NerixPmtSensitiveDetector("Nerix/PmtSD");
	pSDManager->AddNewDetector(pPmtSD);
	m_pTopPmtPhotoCathodeLogicalVolume->SetSensitiveDetector(pPmtSD);
	m_pBottomPmtPhotoCathodeLogicalVolume->SetSensitiveDetector(pPmtSD);

	//=============================== optical surfaces ==============================
	G4OpticalSurface *pSS304LSteelOpticalSurface = new G4OpticalSurface("SS304LSteelOpticalSurface", unified, polished, dielectric_metal, 0.);

	pSS304LSteelOpticalSurface->SetMaterialPropertiesTable(G4Material::GetMaterial("SS304LSteel")->GetMaterialPropertiesTable());
	
	new G4LogicalBorderSurface("GXeTopPmtCasingBorderSurface0", m_pGXePhysicalVolume, m_hTopPmtCasingPhysicalVolumes[0], pSS304LSteelOpticalSurface);
	new G4LogicalBorderSurface("GXeTopPmtCasingBorderSurface1", m_pGXePhysicalVolume, m_hTopPmtCasingPhysicalVolumes[1], pSS304LSteelOpticalSurface);
	new G4LogicalBorderSurface("GXeTopPmtCasingBorderSurface2", m_pGXePhysicalVolume, m_hTopPmtCasingPhysicalVolumes[2], pSS304LSteelOpticalSurface);
	new G4LogicalBorderSurface("GXeTopPmtCasingBorderSurface3", m_pGXePhysicalVolume, m_hTopPmtCasingPhysicalVolumes[3], pSS304LSteelOpticalSurface);
	new G4LogicalBorderSurface("LXeBottomPmtCasingBorderSurface", m_pLXePhysicalVolume, m_pBottomPmtCasingPhysicalVolume, pSS304LSteelOpticalSurface);

	new G4LogicalBorderSurface("TopPmtWindowPmtCasingBorderSurface0", m_hTopPmtWindowPhysicalVolumes[0], m_hTopPmtCasingPhysicalVolumes[0], pSS304LSteelOpticalSurface);
	new G4LogicalBorderSurface("TopPmtWindowPmtCasingBorderSurface1", m_hTopPmtWindowPhysicalVolumes[1], m_hTopPmtCasingPhysicalVolumes[1], pSS304LSteelOpticalSurface);
	new G4LogicalBorderSurface("TopPmtWindowPmtCasingBorderSurface2", m_hTopPmtWindowPhysicalVolumes[2], m_hTopPmtCasingPhysicalVolumes[2], pSS304LSteelOpticalSurface);
	new G4LogicalBorderSurface("TopPmtWindowPmtCasingBorderSurface3", m_hTopPmtWindowPhysicalVolumes[3], m_hTopPmtCasingPhysicalVolumes[3], pSS304LSteelOpticalSurface);
	new G4LogicalBorderSurface("BottomPmtWindowPmtCasingBorderSurface", m_pBottomPmtWindowPhysicalVolume, m_pBottomPmtCasingPhysicalVolume, pSS304LSteelOpticalSurface);

	G4OpticalSurface *pAluminiumOpticalSurface = new G4OpticalSurface("AluminiumOpticalSurface", unified, polished, dielectric_metal, 0.);

	pAluminiumOpticalSurface->SetMaterialPropertiesTable(G4Material::GetMaterial("G4_Al")->GetMaterialPropertiesTable());

	new G4LogicalBorderSurface("GXePmtSealBorderSurface", m_pGXePhysicalVolume, m_pTopPmtSealPhysicalVolume, pAluminiumOpticalSurface);
	new G4LogicalBorderSurface("LXeBottomPmtSealBorderSurface", m_pLXePhysicalVolume, m_pBottomPmtSealPhysicalVolume, pAluminiumOpticalSurface);

	new G4LogicalBorderSurface("TopPmtWindowPmtSealBorderSurface0", m_hTopPmtWindowPhysicalVolumes[0], m_pTopPmtSealPhysicalVolume, pAluminiumOpticalSurface);
	new G4LogicalBorderSurface("TopPmtWindowPmtSealBorderSurface1", m_hTopPmtWindowPhysicalVolumes[1], m_pTopPmtSealPhysicalVolume, pAluminiumOpticalSurface);
	new G4LogicalBorderSurface("TopPmtWindowPmtSealBorderSurface2", m_hTopPmtWindowPhysicalVolumes[2], m_pTopPmtSealPhysicalVolume, pAluminiumOpticalSurface);
	new G4LogicalBorderSurface("TopPmtWindowPmtSealBorderSurface3", m_hTopPmtWindowPhysicalVolumes[3], m_pTopPmtSealPhysicalVolume, pAluminiumOpticalSurface);
	new G4LogicalBorderSurface("BottomPmtWindowPmtSealBorderSurface", m_pBottomPmtWindowPhysicalVolume, m_pBottomPmtSealPhysicalVolume, pAluminiumOpticalSurface);

	G4double dSigmaAlpha = 0.1;
	G4OpticalSurface *pTeflonOpticalSurface = new G4OpticalSurface("TeflonOpticalSurface", unified,
		ground, dielectric_metal, dSigmaAlpha);

	pTeflonOpticalSurface->SetMaterialPropertiesTable(Teflon->GetMaterialPropertiesTable());

	new G4LogicalBorderSurface("GXeTopPmtTeflonSpacerBorderSurface0", m_pGXePhysicalVolume, m_hTopPmtTeflonSpacerPhysicalVolumes[0], pTeflonOpticalSurface);
	new G4LogicalBorderSurface("GXeTopPmtTeflonSpacerBorderSurface1", m_pGXePhysicalVolume, m_hTopPmtTeflonSpacerPhysicalVolumes[1], pTeflonOpticalSurface);
	new G4LogicalBorderSurface("GXeTopPmtTeflonSpacerBorderSurface2", m_pGXePhysicalVolume, m_hTopPmtTeflonSpacerPhysicalVolumes[2], pTeflonOpticalSurface);
	new G4LogicalBorderSurface("GXeTopPmtTeflonSpacerBorderSurface3", m_pGXePhysicalVolume, m_hTopPmtTeflonSpacerPhysicalVolumes[3], pTeflonOpticalSurface);
	new G4LogicalBorderSurface("LXeBottomPmtTeflonSpacerBorderSurface", m_pLXePhysicalVolume, m_pBottomPmtTeflonSpacerPhysicalVolume, pTeflonOpticalSurface);

	new G4LogicalBorderSurface("LXeBottomPmtBaseBorderSurface", m_pLXePhysicalVolume, m_pBottomPmtBasePhysicalVolume, pTeflonOpticalSurface);

	//================================== attributes =================================
	G4Colour hPmtWindowColor(1., 0.757, 0.024);
	G4VisAttributes *pPmtWindowVisAtt = new G4VisAttributes(hPmtWindowColor);
	pPmtWindowVisAtt->SetVisibility(true);
	m_pTopPmtWindowLogicalVolume->SetVisAttributes(pPmtWindowVisAtt);
	m_pBottomPmtWindowLogicalVolume->SetVisAttributes(pPmtWindowVisAtt);

	G4Colour hPmtCasingColor(1., 0.486, 0.027);
	G4VisAttributes *pPmtCasingVisAtt = new G4VisAttributes(hPmtCasingColor);
	pPmtCasingVisAtt->SetVisibility(true);
	m_pTopPmtCasingLogicalVolume->SetVisAttributes(pPmtCasingVisAtt);
	m_pBottomPmtCasingLogicalVolume->SetVisAttributes(pPmtCasingVisAtt);

	G4VisAttributes *pPmtInteriorVisAtt = new G4VisAttributes();
	pPmtInteriorVisAtt->SetVisibility(true);
	m_pTopPmtInteriorLogicalVolume->SetVisAttributes(pPmtInteriorVisAtt);	
	m_pBottomPmtInteriorLogicalVolume->SetVisAttributes(pPmtInteriorVisAtt);	

	G4Colour hPmtSealColor(0.800, 0.800, 0.800);
	G4VisAttributes *pPmtSealVisAtt = new G4VisAttributes(hPmtSealColor);
	pPmtSealVisAtt->SetVisibility(true);
	m_pTopPmtSealLogicalVolume->SetVisAttributes(pPmtSealVisAtt);
	m_pBottomPmtSealLogicalVolume->SetVisAttributes(pPmtSealVisAtt);

	G4Colour hPmtPhotoCathodeColor(1., 0.082, 0.011);
	G4VisAttributes *pPmtPhotoCathodeVisAtt = new G4VisAttributes(hPmtPhotoCathodeColor);
	pPmtPhotoCathodeVisAtt->SetVisibility(true);
	m_pTopPmtPhotoCathodeLogicalVolume->SetVisAttributes(pPmtPhotoCathodeVisAtt);
	m_pBottomPmtPhotoCathodeLogicalVolume->SetVisAttributes(pPmtPhotoCathodeVisAtt);

	G4Colour hPmtBaseColor(0.788, 0.188, 0.780);
	G4VisAttributes *pPmtBaseVisAtt = new G4VisAttributes(hPmtBaseColor);
	pPmtBaseVisAtt->SetVisibility(true);
	m_pTopPmtBaseLogicalVolume->SetVisAttributes(pPmtBaseVisAtt);

	G4Colour hTeflonColor(1., 1., 1., 1.);
	G4VisAttributes *pTeflonVisAtt = new G4VisAttributes(hTeflonColor);
	pTeflonVisAtt->SetVisibility(true);
	m_pTopPmtTeflonSpacerLogicalVolume->SetVisAttributes(pTeflonVisAtt);
	m_pBottomPmtBaseLogicalVolume->SetVisAttributes(pTeflonVisAtt);
	m_pBottomPmtTeflonSpacerLogicalVolume->SetVisAttributes(pTeflonVisAtt);
}

void
NerixDetectorConstruction::ConstructLiquidScintillators()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<< liquid scintillators >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dLiquidScintillatorCasingRadius = GetGeometryParameter("LiquidScintillatorCasingRadius");
	const G4double dLiquidScintillatorCasingHeight = GetGeometryParameter("LiquidScintillatorCasingHeight");
	const G4double dLiquidScintillatorRadius = GetGeometryParameter("LiquidScintillatorRadius");
	const G4double dLiquidScintillatorHeight = GetGeometryParameter("LiquidScintillatorHeight");

	const G4int iNbLiquidScintillators = (G4int) GetGeometryParameter("NbLiquidScintillators");
	const G4int iNbRecoilEnergies = (G4int) GetGeometryParameter("NbRecoilEnergies");

	const G4double dLabToSimulationCoordinatesRotationAngleZ = GetGeometryParameter("LabToSimulationCoordinatesRotationAngleZ");

	G4Material *Aluminium = G4Material::GetMaterial("G4_Al");
	G4Material *EJ301 = G4Material::GetMaterial("EJ301");

	//=================================== casing ====================================
	const G4double dLiquidScintillatorCasingHalfZ = 0.5*dLiquidScintillatorCasingHeight;
	const G4double dLiquidScintillatorHalfZ = 0.5*dLiquidScintillatorHeight;

	G4Tubs *pLiquidScintillatorCasingTube = new G4Tubs("LiquidScintillatorCasingTube", 0.,
		dLiquidScintillatorCasingRadius, dLiquidScintillatorCasingHalfZ, 0.*deg, 360.*deg);

	m_pLiquidScintillatorCasingLogicalVolume = new G4LogicalVolume(pLiquidScintillatorCasingTube, Aluminium,
		"LiquidScintillatorCasingLogicalVolume", 0, 0, 0);

	G4Tubs *pLiquidScintillatorTube = new G4Tubs("LiquidScintillatorTube", 0.,
		dLiquidScintillatorRadius, dLiquidScintillatorHalfZ, 0.*deg, 360.*deg);

	m_pLiquidScintillatorLogicalVolume = new G4LogicalVolume(pLiquidScintillatorTube, EJ301,
		"LiquidScintillatorLogicalVolume", 0, 0, 0);

	m_pLiquidScintillatorPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
		m_pLiquidScintillatorLogicalVolume, "LiquidScintillator", m_pLiquidScintillatorCasingLogicalVolume, false, 0);

	stringstream hVolumeName, hParameterName;

	for(int iLiquidScintillatorId = 0; iLiquidScintillatorId < iNbLiquidScintillators; iLiquidScintillatorId++)
	{
		hVolumeName << "LiquidScintillatorCasing" << iLiquidScintillatorId;

		G4ThreeVector hPos;

		hParameterName << "LiquidScintillator" << iLiquidScintillatorId << "PositionX";
		hPos.setX(GetGeometryParameter(hParameterName.str().c_str()));
		//debugging
		//G4cout << hParameterName.str().c_str() << std::endl;
		//G4cout << "$$$$$$$$$$$$$$$$$$$$$ x = " << hPos.getX() << std::endl;
		hParameterName.str(""); hParameterName.clear();

		hParameterName << "LiquidScintillator" << iLiquidScintillatorId << "PositionY";
		hPos.setY(GetGeometryParameter(hParameterName.str().c_str()));
		hParameterName.str(""); hParameterName.clear();

		hParameterName << "LiquidScintillator" << iLiquidScintillatorId << "PositionZ";
		hPos.setZ(GetGeometryParameter(hParameterName.str().c_str()));
		hParameterName.str(""); hParameterName.clear();

		hPos.rotateZ(dLabToSimulationCoordinatesRotationAngleZ);
		
		//moves detector when set manually so problem is in setting positions
		
		//hPos.setX(0); //added to check where problem is
		//hPos.setY(-700*mm);
		//hPos.setZ(0);

		m_hLiquidScintillatorCasingPhysicalVolumes.push_back(new G4PVPlacement(0, hPos,
			m_pLiquidScintillatorCasingLogicalVolume, hVolumeName.str(), m_pMotherLogicalVolume,
			false, iLiquidScintillatorId));

		hVolumeName.str(""); hVolumeName.clear();
	}

	//================================= sensitivity =================================
	G4SDManager *pSDManager = G4SDManager::GetSDMpointer();

	NerixLiquidScintillatorSensitiveDetector *pLiquidScintillatorSD = new NerixLiquidScintillatorSensitiveDetector("Nerix/LiquidScintillatorSD", "LiquidScintillatorHitsCollection");
	pSDManager->AddNewDetector(pLiquidScintillatorSD);
	m_pLiquidScintillatorLogicalVolume->SetSensitiveDetector(pLiquidScintillatorSD);

	//================================== attributes =================================
	G4Colour hAluminiumColor(0.750, 0.750, 0.750);
	G4Colour hEJ301Color(0.000, 0.945, 0.000);

	G4VisAttributes *pLiquidScintillatorCasingVisAtt = new G4VisAttributes(hAluminiumColor);
	pLiquidScintillatorCasingVisAtt->SetVisibility(true);
	m_pLiquidScintillatorCasingLogicalVolume->SetVisAttributes(pLiquidScintillatorCasingVisAtt);

	G4VisAttributes *pLiquidScintillatorVisAtt = new G4VisAttributes(hEJ301Color);
	pLiquidScintillatorVisAtt->SetVisibility(true);
	m_pLiquidScintillatorLogicalVolume->SetVisAttributes(pLiquidScintillatorVisAtt);
}

void
NerixDetectorConstruction::ConstructGeDetectors()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<< germanium detectors >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dGeDetectorCasingRadius = GetGeometryParameter("GeDetectorCasingRadius");
	const G4double dGeDetectorCasingHeight = GetGeometryParameter("GeDetectorCasingHeight");
	const G4double dGeDetectorRadius = GetGeometryParameter("GeDetectorRadius");
	const G4double dGeDetectorHeight = GetGeometryParameter("GeDetectorHeight");

	const G4int iNbGeDetectors = (G4int) GetGeometryParameter("NbGeDetectors");
	const G4int iNbRecoilEnergies = (G4int) GetGeometryParameter("NbRecoilEnergies");

	const G4double dLabToSimulationCoordinatesRotationAngleZ = GetGeometryParameter("LabToSimulationCoordinatesRotationAngleZ");

	G4Material *Aluminium = G4Material::GetMaterial("G4_Al");
	G4Material *HPGe = G4Material::GetMaterial("HPGe");

	//=================================== casing ====================================
	const G4double dGeDetectorCasingHalfZ = 0.5*dGeDetectorCasingHeight;
	const G4double dGeDetectorHalfZ = 0.5*dGeDetectorHeight;

	G4Tubs *pGeDetectorCasingTube = new G4Tubs("GeDetectorCasingTube", 0.,
		dGeDetectorCasingRadius, dGeDetectorCasingHalfZ, 0.*deg, 360.*deg);

	m_pGeDetectorCasingLogicalVolume = new G4LogicalVolume(pGeDetectorCasingTube, Aluminium,
		"GeDetectorCasingLogicalVolume", 0, 0, 0);

	G4Tubs *pGeDetectorTube = new G4Tubs("GeDetectorTube", 0.,
		dGeDetectorRadius, dGeDetectorHalfZ, 0.*deg, 360.*deg);

	m_pGeDetectorLogicalVolume = new G4LogicalVolume(pGeDetectorTube, HPGe,
		"GeDetectorLogicalVolume", 0, 0, 0);

	m_pGeDetectorPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
		m_pGeDetectorLogicalVolume, "GeDetector", m_pGeDetectorCasingLogicalVolume, false, 0);

	stringstream hVolumeName, hParameterName;
	for(int iRecoilEnergyId = 0; iRecoilEnergyId < iNbRecoilEnergies; iRecoilEnergyId++)
	{
		hParameterName << "RecoilEnergy" << iRecoilEnergyId;
		G4double dRecoilEnergy = GetGeometryParameter(hParameterName.str().c_str());
		hParameterName.str(""); hParameterName.clear();
		int iRecoilEnergyKey = (int) dRecoilEnergy;

		for(int iGeDetectorId = 0; iGeDetectorId < iNbGeDetectors; iGeDetectorId++)
		{
			hVolumeName << "GeDetectorCasing" << iGeDetectorId;

			G4ThreeVector hPos;

			hParameterName << "Recoil" << iRecoilEnergyKey << "keVee" << "GeDetector" << iGeDetectorId << "PositionX";
			hPos.setX(GetGeometryParameter(hParameterName.str().c_str()));
			hParameterName.str(""); hParameterName.clear();

			hParameterName << "Recoil" << iRecoilEnergyKey << "keVee" << "GeDetector" << iGeDetectorId << "PositionY";
			hPos.setY(GetGeometryParameter(hParameterName.str().c_str()));
			hParameterName.str(""); hParameterName.clear();

			hParameterName << "Recoil" << iRecoilEnergyKey << "keVee" << "GeDetector" << iGeDetectorId << "PositionZ";
			hPos.setZ(GetGeometryParameter(hParameterName.str().c_str()));
			hParameterName.str(""); hParameterName.clear();

			hPos.rotateZ(dLabToSimulationCoordinatesRotationAngleZ);

			m_hGeDetectorCasingPhysicalVolumes.push_back(new G4PVPlacement(0, hPos,
				m_pGeDetectorCasingLogicalVolume, hVolumeName.str(), m_pMotherLogicalVolume,
				false, 10*iRecoilEnergyId+iGeDetectorId));

			hVolumeName.str(""); hVolumeName.clear();
		}
	}

	//================================= sensitivity =================================
	G4SDManager *pSDManager = G4SDManager::GetSDMpointer();

	NerixGeDetectorSensitiveDetector *pGeDetectorSD = new NerixGeDetectorSensitiveDetector("Nerix/GeDetectorSD", "GeDetectorHitsCollection");
	pSDManager->AddNewDetector(pGeDetectorSD);
	m_pGeDetectorLogicalVolume->SetSensitiveDetector(pGeDetectorSD);

	//================================== attributes =================================
	G4Colour hAluminiumColor(0.750, 0.750, 0.750);
	G4Colour hHPGeColor(0.100, 0.100, 0.100);

	G4VisAttributes *pGeDetectorCasingVisAtt = new G4VisAttributes(hAluminiumColor);
	pGeDetectorCasingVisAtt->SetVisibility(true);
	m_pGeDetectorCasingLogicalVolume->SetVisAttributes(pGeDetectorCasingVisAtt);

	G4VisAttributes *pGeDetectorVisAtt = new G4VisAttributes(hHPGeColor);
	pGeDetectorVisAtt->SetVisibility(true);
	m_pGeDetectorLogicalVolume->SetVisAttributes(pGeDetectorVisAtt);
}

void
NerixDetectorConstruction::ConstructLeadCollimators()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< lead collimators >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	const G4double dSourceLeadCollimatorWidth = GetGeometryParameter("SourceLeadCollimatorWidth");
	const G4double dSourceLeadCollimatorDepth = GetGeometryParameter("SourceLeadCollimatorDepth");
	const G4double dSourceLeadCollimatorHeight = GetGeometryParameter("SourceLeadCollimatorHeight");
	const G4double dSourceLeadCollimatorHoleRadius = GetGeometryParameter("SourceLeadCollimatorHoleRadius");
	const G4double dSourceLeadCollimatorPositionX = GetGeometryParameter("SourceLeadCollimatorPositionX");
	const G4double dSourceLeadCollimatorPositionY = GetGeometryParameter("SourceLeadCollimatorPositionY");
	const G4double dSourceLeadCollimatorPositionZ = GetGeometryParameter("SourceLeadCollimatorPositionZ");

	const G4double dLabToSimulationCoordinatesRotationAngleZ = GetGeometryParameter("LabToSimulationCoordinatesRotationAngleZ");

	G4Material *Lead = G4Material::GetMaterial("Lead");

	//=============================== source collimator =============================
	const G4double dSourceLeadCollimatorHalfX = 0.5*dSourceLeadCollimatorWidth;
	const G4double dSourceLeadCollimatorHalfY = 0.5*dSourceLeadCollimatorDepth;
	const G4double dSourceLeadCollimatorHalfZ = 0.5*dSourceLeadCollimatorHeight;
	const G4double dSourceLeadCollimatorCutHalfX = dSourceLeadCollimatorDepth;
	const G4double dSourceLeadCollimatorCutRadius = dSourceLeadCollimatorHoleRadius;

	G4Box *pSourceLeadCollimatorBox = new G4Box("SourceLeadCollimatorBox", dSourceLeadCollimatorHalfX, dSourceLeadCollimatorHalfY, dSourceLeadCollimatorHalfZ);

	G4Tubs *pSourceLeadCollimatorCutTube = new G4Tubs("SourceLeadCollimatorCutTube", 0.,
		dSourceLeadCollimatorCutRadius, dSourceLeadCollimatorCutHalfX, 0.*deg, 360.*deg);

	G4SubtractionSolid *pSourceLeadCollimatorWithHole = new G4SubtractionSolid("SourceLeadCollimatorWithHole",
		pSourceLeadCollimatorBox, pSourceLeadCollimatorCutTube, m_pRotationXPlus90, G4ThreeVector(0., 0., 0.));

	m_pSourceLeadCollimatorLogicalVolume = new G4LogicalVolume(pSourceLeadCollimatorWithHole, Lead,
		"SourceLeadCollimatorLogicalVolume", 0, 0, 0);

	G4ThreeVector hSourceLeadCollimatorPos(dSourceLeadCollimatorPositionX, dSourceLeadCollimatorPositionY, dSourceLeadCollimatorPositionZ);
	hSourceLeadCollimatorPos.rotateZ(dLabToSimulationCoordinatesRotationAngleZ);

	m_pSourceLeadCollimatorPhysicalVolume = new G4PVPlacement(m_pRotationLabToSimulation, hSourceLeadCollimatorPos,
		m_pSourceLeadCollimatorLogicalVolume, "SourceLeadCollimator", m_pMotherLogicalVolume, false, 0);

	//================================== attributes =================================
	G4Colour hLeadColor(0.500, 0.500, 0.500, 0.1);

	G4VisAttributes *pLeadCollimatorVisAtt = new G4VisAttributes(hLeadColor);
	pLeadCollimatorVisAtt->SetVisibility(true);
	m_pSourceLeadCollimatorLogicalVolume->SetVisAttributes(pLeadCollimatorVisAtt);
}

void
NerixDetectorConstruction::PrintGeometryInformation()
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< xenon >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if(m_pLXeLogicalVolume)
	{
		const G4double dLXeTotalMass = m_pLXeLogicalVolume->GetMass(false, false)/kg
			+m_pLXeChamberBufferLogicalVolume->GetMass(false, false)/kg;
		G4cout
			<< "Total LXe Mass: " << dLXeTotalMass << " kg" << G4endl;
	}
}

