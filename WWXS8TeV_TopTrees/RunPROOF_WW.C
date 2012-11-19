////////////////////////////////////////////////////////////////////////////////
//
//    FILE: RunPROOF_WW.C
// AUTHORS: I. Gonzalez, A.Y. Rodriguez, J. Piedra
//    DATE: 2012
//
// CONTENT: Main macro to run over MiniTrees using PROOF in PROOF-Lite,
//          PROOF-Cluster or Sequential mode
//
////////////////////////////////////////////////////////////////////////////////

#include "scripts/PAFUtils.C"


Double_t G_Event_Weight = 1;
Double_t G_Event_Lumi   = 12103.3;  // pb
TProof*  proof          = 0;


void RunPROOF_WW(TString  sample   = "WZTo3LNu",
		 TString  selector = "AnalysisWW",
		 Long64_t nEvents  = -1)
{
  // PROOF mode
  //----------------------------------------------------------------------------
  //  gPAFOptions->proofMode = kSequential;
  //  gPAFOptions->proofMode = kLite;
  //  gPAFOptions->proofMode = kCluster;
  gPAFOptions->proofMode = kPoD;
  gPAFOptions->NSlots    = 20;


  // PROOF start
  //----------------------------------------------------------------------------
  proof = InitProof();

  if (!proof && gPAFOptions->proofMode != kSequential) {
    cerr << " ERROR: I could not initialise a PROOF session." << endl;
    return;
  }


  // Tree type
  //----------------------------------------------------------------------------
  gPAFOptions->treeType = kMiniTrees;

  
  // Read input files
  //----------------------------------------------------------------------------
  TString dataPath = "/hadoop";

  if (sample.Contains("Data_DoubleElectron")) {
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleElectronA_892_0.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleElectronB_4404_0.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleElectronB_4404_1.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleElectronC_6807_0.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleElectronC_6807_1.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleElectronC_6807_2.root");
  }								  
  else if (sample.Contains("Data_DoubleMu")) {			  
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleMuA_892_0.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleMuB_4404_0.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleMuB_4404_1.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleMuB_4404_2.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleMuB_4404_3.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleMuC_6807_0.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleMuC_6807_1.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleMuC_6807_2.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleMuC_6807_3.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_DoubleMuC_6807_4.root");
  }								  
  else if (sample.Contains("Data_MuEG")) {			  
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_MuEGA_892_0.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_MuEGB_4404_0.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_MuEGB_4404_1.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_MuEGB_4404_2.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_MuEGC_6807_0.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_MuEGC_6807_1.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_MuEGC_6807_2.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/Tree_MuEGC_6807_3.root");
  }
  else {

    gROOT->LoadMacro("../DatasetManager/DatasetManager.C+");

    DatasetManager* dm = new DatasetManager("Summer12_53X");

    // Use this if you know that the information on the google doc table has
    // changed and you need to update the information
    dm->RedownloadFiles();

    dm->LoadDataset(sample);  // Load information about a given dataset
    
    G_Event_Weight = dm->GetCrossSection() * G_Event_Lumi / dm->GetEventsInTheSample();

    cout << endl;
    cout << "             xs = " << dm->GetCrossSection()      << endl;
    cout << "     luminosity = " << G_Event_Lumi               << endl;
    cout << "        nevents = " << dm->GetEventsInTheSample() << endl;
    cout << " base file name = " << dm->GetBaseFileName()      << endl;
    cout << "      xs weight = " << G_Event_Weight             << endl;
    cout << endl;

    gPAFOptions->dataFiles = dm->GetFiles();
  }


  // Output file name
  //----------------------------------------------------------------------------
  TString outputDir = "../WWXS8TeV_TopTrees/rootfiles/Summer12_53X/";

  gSystem->mkdir(outputDir, kTRUE);

  TString outputFile = outputDir + "/" + sample + ".root";
  
  gPAFOptions->outputFile = outputFile;


  // Parameters for the analysis
  //----------------------------------------------------------------------------
  gPAFOptions->inputParameters = new InputParameters();

  gPAFOptions->inputParameters->SetNamedString("sample",     sample.Data());
  gPAFOptions->inputParameters->SetNamedDouble("xs_weight",  G_Event_Weight);
  gPAFOptions->inputParameters->SetNamedDouble("luminosity", G_Event_Lumi);


  // Number of events (Long64_t)
  //----------------------------------------------------------------------------
  gPAFOptions->nEvents = nEvents;

  
  // Name of analysis class
  //----------------------------------------------------------------------------
  gPAFOptions->myAnalysis = selector.Data();


  // Additional packages to be uploaded to PROOF
  //----------------------------------------------------------------------------
  if (gPAFOptions->treeType == kMiniTrees)
    gPAFOptions->packages.push_back("CMSAnalysisSelectorMiniTrees");
  else if (gPAFOptions->treeType == kTESCO)
    gPAFOptions->packages.push_back("CMSAnalysisSelectorTESCO");

  gPAFOptions->packages.push_back("PUWeight");


  // Control output and checks
  //----------------------------------------------------------------------------
  // If true (default) the output file is reopened so the objects in the file
  // can be interactively accessed. The objects are also listed.

  gPAFOptions->reopenOutputFile = false;


  // Run the analysis
  //----------------------------------------------------------------------------
  if (!RunAnalysis())
    cerr << " ERROR: There was a problem running the analysis" << endl;
}
