#include <string>
#include <sstream>
#include <unistd.h>

#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4UIterminal.hh>
#include <G4UItcsh.hh>
#include <G4VisExecutive.hh>

#include "NerixDetectorConstruction.hh"
#include "NerixPhysicsList.hh"
#include "NerixPrimaryGeneratorAction.hh"
#include "NerixAnalysisManager.hh"
#include "NerixRunAction.hh"
#include "NerixEventAction.hh"
#include "NerixSteppingAction.hh"

void usage();

int
main(int argc, char **argv)
{
    //XInitThreads()
	// switches
	int c = 0;

	std::stringstream hStream;
	
	bool bInteractive = false;
	bool bVisualize = false;
	bool bVrmlVisualize = false;
	bool bOpenGlVisualize = false;

	bool bMacroFile = false;
	std::string hMacroFilename, hDataFilename, hMaterialPropertiesFilename;
	std::vector<unsigned long> hInitialEngineStatus;
	int iNbEventsToSimulate = 0;

	// parse switches
	while((c = getopt(argc,argv,"v:f:o:n:s:m:i")) != -1)
	{
		switch(c)
		{
			case 'v':
				bVisualize = true;
				hStream.str(optarg);
				if(hStream.str() == "vrml")
					bVrmlVisualize = true;
				else if(hStream.str() == "opengl")
					bOpenGlVisualize = true;
				hStream.clear();
				break;

			case 'f':
				bMacroFile = true;
				hMacroFilename = optarg;
				break;

			case 'o':
				hDataFilename = optarg;
				break;

			case 'n':
				hStream.str(optarg);
				hStream >> iNbEventsToSimulate;
				hStream.clear();
				break;

			case 's':
				hStream.str(optarg);
				hInitialEngineStatus.resize(4);
				hStream >> hInitialEngineStatus[0]; hStream.ignore();
				hStream >> hInitialEngineStatus[1]; hStream.ignore();
				hStream >> hInitialEngineStatus[2]; hStream.ignore();
				hStream >> hInitialEngineStatus[3];
				hStream.clear();
				break;

			case 'm':
				hMaterialPropertiesFilename = optarg;
				break;

			case 'i':
				bInteractive = true;
				break;

			default:
				usage();
		}
	}

	// create the run manager
	G4RunManager *pRunManager = new G4RunManager;

	// set user-defined initialization classes
	NerixDetectorConstruction *pDetectorConstruction = new NerixDetectorConstruction();
	pRunManager->SetUserInitialization(pDetectorConstruction);
	pRunManager->SetUserInitialization(new NerixPhysicsList);

	G4VisManager* pVisManager = new G4VisExecutive;
	pVisManager->Initialize();

	// create the primary generator action
	NerixPrimaryGeneratorAction *pPrimaryGeneratorAction = new NerixPrimaryGeneratorAction();

	// create an analysis manager object
	NerixAnalysisManager *pAnalysisManager = new NerixAnalysisManager(pPrimaryGeneratorAction);
	if(!hDataFilename.empty())
		pAnalysisManager->SetDataFilename(hDataFilename);
	if(iNbEventsToSimulate)
		pAnalysisManager->SetNbEventsToSimulate(iNbEventsToSimulate);
	if(!hInitialEngineStatus.empty())
		pPrimaryGeneratorAction->SetInitialEngineStatus(hInitialEngineStatus);

	// set user-defined action classes
	pRunManager->SetUserAction(pPrimaryGeneratorAction);
	pRunManager->SetUserAction(new NerixRunAction(pAnalysisManager));
	pRunManager->SetUserAction(new NerixEventAction(pAnalysisManager));
	pRunManager->SetUserAction(new NerixSteppingAction(pAnalysisManager));

	pRunManager->Initialize();

	// geometry was initialized, we can update material properties
	if(!hMaterialPropertiesFilename.empty())
		pDetectorConstruction->UpdateMaterialPropertiesFromFile(hMaterialPropertiesFilename);

	G4UImanager* pUImanager = G4UImanager::GetUIpointer();

	G4UIsession * pUIsession = 0;
	if(bInteractive)
	{
		pUIsession = new G4UIterminal(new G4UItcsh);
	}

	std::string hCommand;

	if(bVisualize)
	{
		pUImanager->ApplyCommand("/vis/scene/create");
		if(bVrmlVisualize)
			pUImanager->ApplyCommand("/vis/open VRML2FILE");
		if(bOpenGlVisualize)
			pUImanager->ApplyCommand("/vis/open OGLSX");
		pUImanager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 0 0 deg");
		pUImanager->ApplyCommand("/vis/viewer/zoom 1.0");
		//pUImanager->ApplyCommand("/tracking/storeTrajectory 1");
		pUImanager->ApplyCommand("/vis/scene/add/trajectories");
	}

	if(bMacroFile)
	{
		hCommand = "/control/execute " + hMacroFilename;
		pUImanager->ApplyCommand(hCommand);
	}

	if(iNbEventsToSimulate)
	{
		hStream.str("");
		hStream.clear();
		hStream << "/run/beamOn " << iNbEventsToSimulate;
		pUImanager->ApplyCommand(hStream.str());
	}

	if(bInteractive)
	{
		pUIsession->SessionStart();

		delete pUIsession;
	}

	delete pAnalysisManager;

	if(bVisualize)
		delete pVisManager;
	delete pRunManager;
	return 0;
}

void
usage()
{
	exit(0);
}

