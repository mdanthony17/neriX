#ifndef __NERIXDETECTORCONSTRUCTION_H__
#define __NERIXDETECTORCONSTRUCTION_H__

#include <globals.hh>

#include <vector>
#include <map>

using std::vector;
using std::map;

class G4LogicalVolume;
class G4VPhysicalVolume;

#include <G4VUserDetectorConstruction.hh>

class NerixDetectorConstruction: public G4VUserDetectorConstruction
{
public:
	NerixDetectorConstruction();
	~NerixDetectorConstruction();

	G4VPhysicalVolume* Construct();

	void UpdateMaterialConstantProperty(const G4String &hMaterial, const G4String &hProperty, G4double dPropertyValue);
	void UpdateMaterialProperty(const G4String &hMaterial, const G4String &hProperty, const vector<G4double> &hPhotonEnergies, const vector<G4double> &hPropertyValues);

	void UpdateMaterialPropertyFromString(const G4String &hString);
	void UpdateMaterialPropertiesFromFile(const G4String &hFilename);

	static G4double GetGeometryParameter(const char *szParameter);

private:
	void DefineMaterials();
	void DefineGeometryParameters();

	void ConstructLaboratory();
	void ConstructSource();
	void ConstructMinitron();
	void ConstructFrame();
	void ConstructCryostat();
	void ConstructChamber();
	void ConstructXenon();
	void ConstructTeflon();
	void ConstructGrids();
	void ConstructPlates();
	void ConstructPmts();
	void ConstructLiquidScintillators();
	void ConstructGeDetectors();
	void ConstructLeadCollimators();
	void ConstructLeadBrickCollimator();

	void PrintGeometryInformation();

private:
	// rotation matrices
	G4RotationMatrix *m_pRotationXPlus90;
	G4RotationMatrix *m_pRotationXMinus90;
	G4RotationMatrix *m_pRotationX180;
	G4RotationMatrix *m_pRotationYPlus90;
	G4RotationMatrix *m_pRotationYMinus90;
	G4RotationMatrix *m_pRotationZPlus90;
	G4RotationMatrix *m_pRotationZMinus90;
	G4RotationMatrix *m_pRotationZPlus45;

	G4RotationMatrix *m_pRotationXPlus90ZMinus90;
	G4RotationMatrix *m_pRotationXMinus90ZPlus90;

	G4RotationMatrix *m_pRotationYPlus45XPlus90;
	G4RotationMatrix *m_pRotationXPlus45YPlus90;

	G4RotationMatrix *m_pRotationLabToSimulation;

	// logical volumes
	G4LogicalVolume *m_pMotherLogicalVolume;

	G4LogicalVolume *m_pLabLogicalVolume;

	G4LogicalVolume *m_pSourceLogicalVolume;

	G4LogicalVolume *m_pMinitronBaseLogicalVolume;
	G4LogicalVolume *m_pMinitronTubeLogicalVolume;
	G4LogicalVolume *m_pMinitronInsulationLogicalVolume;
	G4LogicalVolume *m_pMinitronHolderLogicalVolume;
	G4LogicalVolume *m_pMinitronVacuumLogicalVolume;

	G4LogicalVolume *m_pFrameTableLogicalVolume;
	G4LogicalVolume *m_pFrameUpDownSideLogicalVolume;
	G4LogicalVolume *m_pFrameLeftRightSideLogicalVolume;
	G4LogicalVolume *m_pFrameFrontBackSideLogicalVolume;

	G4LogicalVolume *m_pCryostatTopFlangeLogicalVolume;
	G4LogicalVolume *m_pCryostatWallLogicalVolume;
	G4LogicalVolume *m_pCryostatBottomFlangeLogicalVolume;
	G4LogicalVolume *m_pCryostatFlangeGasketLogicalVolume;
	G4LogicalVolume *m_pCryostatVacuumLogicalVolume;

	G4LogicalVolume *m_pChamberMatingFlangeLogicalVolume;
	G4LogicalVolume *m_pChamberTopFlangeLogicalVolume;
	G4LogicalVolume *m_pChamberTopWallLogicalVolume;
	G4LogicalVolume *m_pChamberMiddlePlateLogicalVolume;
	G4LogicalVolume *m_pChamberWallLogicalVolume;
	G4LogicalVolume *m_pChamberWallBulgePlateLogicalVolume;
	G4LogicalVolume *m_pChamberBufferLogicalVolume;
	G4LogicalVolume *m_pChamberBufferPartitionLogicalVolume;
	G4LogicalVolume *m_pChamberBottomPlateLogicalVolume;

	G4LogicalVolume *m_pGXeLogicalVolume;
	G4LogicalVolume *m_pGXeChamberBufferLogicalVolume;
	G4LogicalVolume *m_pLXeLogicalVolume;
	G4LogicalVolume *m_pLXeChamberBufferLogicalVolume;

	G4LogicalVolume *m_pTeflonTopPmtHolderLogicalVolume;
	G4LogicalVolume *m_pTeflonTopGridSpacerLogicalVolume;
	G4LogicalVolume *m_pTeflonAnodeSpacerAboveLevelLogicalVolume;
	G4LogicalVolume *m_pTeflonAnodeSpacerBelowLevelLogicalVolume;
	G4LogicalVolume *m_pTeflonDriftRegionSpacerLogicalVolume;
	G4LogicalVolume *m_pTeflonDriftRegionRingHolderLogicalVolume;
	G4LogicalVolume *m_pTeflonScreeningGridSpacerLogicalVolume;
	G4LogicalVolume *m_pTeflonBottomPmtHolderLogicalVolume;

	G4LogicalVolume *m_pGridRingLogicalVolume;
	G4LogicalVolume *m_pTopGridHexagonalCellLogicalVolume;
	G4LogicalVolume *m_pAnodeGridHexagonalCellLogicalVolume;
	G4LogicalVolume *m_pBottomGridHexagonalCellLogicalVolume;
	G4LogicalVolume *m_pCathodeGridHexagonalCellLogicalVolume;
	G4LogicalVolume *m_pScreeningGridHexagonalCellLogicalVolume;

	G4LogicalVolume *m_pTopPlateLogicalVolume;
	G4LogicalVolume *m_pTopPmtPlateLogicalVolume;
	G4LogicalVolume *m_pBottomPlateLogicalVolume;
	G4LogicalVolume *m_pBottomPmtPlateLogicalVolume;

	G4LogicalVolume *m_pTopPmtWindowLogicalVolume;
	G4LogicalVolume *m_pTopPmtCasingLogicalVolume;
	G4LogicalVolume *m_pTopPmtInteriorLogicalVolume;
	G4LogicalVolume *m_pTopPmtSealLogicalVolume;
	G4LogicalVolume *m_pTopPmtPhotoCathodeLogicalVolume;
	G4LogicalVolume *m_pTopPmtBaseLogicalVolume;
	G4LogicalVolume *m_pTopPmtTeflonSpacerLogicalVolume;

	G4LogicalVolume *m_pBottomPmtWindowLogicalVolume;
	G4LogicalVolume *m_pBottomPmtCasingLogicalVolume;
	G4LogicalVolume *m_pBottomPmtInteriorLogicalVolume;
	G4LogicalVolume *m_pBottomPmtSealLogicalVolume;
	G4LogicalVolume *m_pBottomPmtPhotoCathodeLogicalVolume;
	G4LogicalVolume *m_pBottomPmtBaseLogicalVolume;
	G4LogicalVolume *m_pBottomPmtTeflonSpacerLogicalVolume;

	G4LogicalVolume *m_pLiquidScintillatorCasingLogicalVolume;
	G4LogicalVolume *m_pLiquidScintillatorLogicalVolume;
	//Ge detector -- added objects by MarcW, Aug 7, 2014	
	G4LogicalVolume	*m_pGeDetectorOuterCasingLogicalVolume;
	G4LogicalVolume	*m_pGeDetectorOuterCasingVacuumLogicalVolume;
	G4LogicalVolume	*m_pGeDetectorInnerCasingLogicalVolume;
	G4LogicalVolume	*m_pGeDetectorInnerCasingVacuumLogicalVolume;
	G4LogicalVolume	*m_pGeDetectorCrystalsLogicalVolume;
	G4LogicalVolume	*m_pGeDetectorCrystalsHoleLogicalVolume;
	G4LogicalVolume *m_pGeDetectorCasingLogicalVolume;
	G4LogicalVolume *m_pGeDetectorLogicalVolume;
	
	//--------------------------------------------------
	//Lead brick collimators -- added objects by MarcW, Sep 18, 2014
	G4LogicalVolume *m_pCollimatorLeadBrickLogicalVolume_1;
	G4LogicalVolume *m_pCollimatorLeadBrickLogicalVolume_2;
	//--------------------------------------------------	

	G4LogicalVolume *m_pSourceLeadCollimatorLogicalVolume;

	// physical volumes
	G4VPhysicalVolume *m_pLabPhysicalVolume;

	G4VPhysicalVolume *m_pSourcePhysicalVolume;

	G4VPhysicalVolume *m_pMinitronBasePhysicalVolume;
	G4VPhysicalVolume *m_pMinitronTubePhysicalVolume;
	G4VPhysicalVolume *m_pMinitronInsulationPhysicalVolume;
	G4VPhysicalVolume *m_pMinitronHolderPhysicalVolume;
	G4VPhysicalVolume *m_pMinitronVacuumPhysicalVolume;

	G4VPhysicalVolume *m_pFrameTablePhysicalVolume;
	vector<G4VPhysicalVolume *> m_hFrameSidePhysicalVolumes;

	G4VPhysicalVolume *m_pCryostatTopFlangePhysicalVolume;
	G4VPhysicalVolume *m_pCryostatWallPhysicalVolume;
	G4VPhysicalVolume *m_pCryostatBottomFlangePhysicalVolume;
	vector<G4VPhysicalVolume *> m_hCryostatFlangeGasketPhysicalVolumes;
	G4VPhysicalVolume *m_pCryostatVacuumPhysicalVolume;

	G4VPhysicalVolume *m_pChamberMatingFlangePhysicalVolume;
	G4VPhysicalVolume *m_pChamberTopFlangePhysicalVolume;
	G4VPhysicalVolume *m_pChamberTopWallPhysicalVolume;
	G4VPhysicalVolume *m_pChamberMiddlePlatePhysicalVolume;
	G4VPhysicalVolume *m_pChamberWallPhysicalVolume;
	G4VPhysicalVolume *m_pChamberWallBulgePlatePhysicalVolume;
	G4VPhysicalVolume *m_pChamberBufferPhysicalVolume;
	G4VPhysicalVolume *m_pChamberBufferPartitionPhysicalVolume;
	G4VPhysicalVolume *m_pChamberBottomPlatePhysicalVolume;

	G4VPhysicalVolume *m_pGXePhysicalVolume;
	G4VPhysicalVolume *m_pGXeChamberBufferPhysicalVolume;
	G4VPhysicalVolume *m_pLXePhysicalVolume;
	G4VPhysicalVolume *m_pLXeChamberBufferPhysicalVolume;

	G4VPhysicalVolume *m_pTeflonTopPmtHolderPhysicalVolume;
	G4VPhysicalVolume *m_pTeflonTopGridSpacerPhysicalVolume;
	G4VPhysicalVolume *m_pTeflonAnodeSpacerAboveLevelPhysicalVolume;
	G4VPhysicalVolume *m_pTeflonAnodeSpacerBelowLevelPhysicalVolume;
	G4VPhysicalVolume *m_pTeflonDriftRegionSpacerPhysicalVolume;
	G4VPhysicalVolume *m_pTeflonDriftRegionRingHolderPhysicalVolume;
	G4VPhysicalVolume *m_pTeflonScreeningGridSpacerPhysicalVolume;
	G4VPhysicalVolume *m_pTeflonBottomPmtHolderPhysicalVolume;

	G4VPhysicalVolume *m_pTopGridRingPhysicalVolume;
	vector<G4VPhysicalVolume *> m_hTopGridPhysicalVolumes;
	G4VPhysicalVolume *m_pAnodeGridRingPhysicalVolume;
	vector<G4VPhysicalVolume *> m_hAnodeGridPhysicalVolumes;
	G4VPhysicalVolume *m_pBottomGridRingPhysicalVolume;
	vector<G4VPhysicalVolume *> m_hBottomGridPhysicalVolumes;
	G4VPhysicalVolume *m_pCathodeGridRingPhysicalVolume;
	vector<G4VPhysicalVolume *> m_hCathodeGridPhysicalVolumes;
	G4VPhysicalVolume *m_pScreeningGridRingPhysicalVolume;
	vector<G4VPhysicalVolume *> m_hScreeningGridPhysicalVolumes;

	G4VPhysicalVolume *m_pTopPlatePhysicalVolume;
	G4VPhysicalVolume *m_pTopPmtPlatePhysicalVolume;
	G4VPhysicalVolume *m_pBottomPlatePhysicalVolume;
	G4VPhysicalVolume *m_pBottomPmtPlatePhysicalVolume;

	vector<G4VPhysicalVolume *> m_hTopPmtWindowPhysicalVolumes;
	vector<G4VPhysicalVolume *> m_hTopPmtCasingPhysicalVolumes;
	G4VPhysicalVolume *m_pTopPmtInteriorPhysicalVolume;
	G4VPhysicalVolume *m_pTopPmtSealPhysicalVolume;
	G4VPhysicalVolume *m_pTopPmtPhotoCathodePhysicalVolume;
	vector<G4VPhysicalVolume *> m_hTopPmtBasePhysicalVolumes;
	vector<G4VPhysicalVolume *> m_hTopPmtTeflonSpacerPhysicalVolumes;

	G4VPhysicalVolume *m_pBottomPmtWindowPhysicalVolume;
	G4VPhysicalVolume *m_pBottomPmtCasingPhysicalVolume;
	G4VPhysicalVolume *m_pBottomPmtInteriorPhysicalVolume;
	G4VPhysicalVolume *m_pBottomPmtSealPhysicalVolume;
	G4VPhysicalVolume *m_pBottomPmtPhotoCathodePhysicalVolume;
	G4VPhysicalVolume *m_pBottomPmtBasePhysicalVolume;
	G4VPhysicalVolume *m_pBottomPmtTeflonSpacerPhysicalVolume;
	//Ge detector -- added objects by MarcW, Aug 7, 2014
	vector<G4VPhysicalVolume *> m_hGeDetectorOuterCasingPhysicalVolumes;
	G4VPhysicalVolume *m_pGeDetectorOuterCasingVacuumPhysicalVolume;
	G4VPhysicalVolume *m_pGeDetectorInnerCasingPhysicalVolume;
	G4VPhysicalVolume *m_pGeDetectorInnerCasingVacuumPhysicalVolume;
	G4VPhysicalVolume *m_pGeDetectorCrystalsPhysicalVolume;
	G4VPhysicalVolume *m_pGeDetectorCrystalsHolePhysicalVolume;
	//---------------------------------------------------
	
	//--------------------------------------------------
	//Lead brick collimators -- added objects by MarcW, Sep 18, 2014
	G4VPhysicalVolume *m_pCollimatorLeadBrickPhysicalVolume_1;
	G4VPhysicalVolume *m_pCollimatorLeadBrickPhysicalVolume_2;
	//--------------------------------------------------
	
	vector<G4VPhysicalVolume *> m_hLiquidScintillatorCasingPhysicalVolumes;
	G4VPhysicalVolume *m_pLiquidScintillatorPhysicalVolume;

	vector<G4VPhysicalVolume *> m_hGeDetectorCasingPhysicalVolumes;
	G4VPhysicalVolume *m_pGeDetectorPhysicalVolume;

	G4VPhysicalVolume *m_pSourceLeadCollimatorPhysicalVolume;

	static map<G4String, G4double> m_hGeometryParameters;
};

#endif // __NERIXDETECTORCONSTRUCTION_H__
