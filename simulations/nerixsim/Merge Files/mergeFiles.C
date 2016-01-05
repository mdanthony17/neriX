#include <string>
#include <iostream>

#include "TList.h"
#include "TFile.h"
#include "TTree.h"

//run using the following command:
//echo rCoincidenceAnalysis55keV.root 55keVAnalysisp1.root 55keVAnalysisp2.root 55keVAnalysisp3.root 55keVAnalysisp4.root 55keVAnalysisp5.root | root -b -l mergeFiles.C+


// Change this line if your TTree has a different name
const char *TreeName = "t1";

void mergeFiles()
{
    using namespace std;
    
    string outfile;
    cin >> outfile;
    
    TList tree_list;
    std::string filename;
    
    Int_t total_events = 0;
    
    while(cin >> filename) {
        TFile *f = new TFile(filename.c_str());
        
        if(TTree *tree = (TTree *)f->Get(TreeName)) {
            
            cout << "Adding file: " << filename << endl;
            tree_list.Add(tree);
            
            total_events += (Int_t )tree->GetEntries();
            
        } else {
            cout << "File has no TTree named TMBTree" << endl;
        }
    }
    
    cout << "Opening output file: " << outfile << endl;
    TFile output(outfile.c_str(), "RECREATE");
    
    cout << "Merging trees...patience..." << endl;
    TTree::MergeTrees(&tree_list);
    output.Write();
    output.Close();
    
    cout << "Total Events: " << total_events << endl;
}

