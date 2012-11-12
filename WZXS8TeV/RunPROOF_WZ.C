////////////////////////////////////////////////////////////////////////////////
//
//    FILE: RunPROOF_WZ.C
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


void RunPROOF_WZ(TString  sample   = "WZTo3LNu",
		 TString  selector = "AnalysisWZ",
		 TString  folder   = ".",
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
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/" + folder + "/Tree_DoubleElectronA_892_*.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/" + folder + "/Tree_DoubleElectronB_4404_*.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/" + folder + "/Tree_DoubleElectronC_6807_*.root");
  }
  else if (sample.Contains("Data_DoubleMu")) {
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/" + folder + "/Tree_DoubleMuA_892_*.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/" + folder + "/Tree_DoubleMuB_4404_*.root");
    gPAFOptions->dataFiles.push_back(dataPath + "/MC_Summer12_53X/" + folder + "/Tree_DoubleMuC_6807_*.root");
  }
  else {

    gROOT->LoadMacro("../DatasetManager/DatasetManager.C+");

    DatasetManager* dm = new DatasetManager("Summer12_53X", folder);

    // Use this if you know that the information on the google doc table has
    // changed and you need to update the information
    dm->RedownloadFiles();

    dm->LoadDataset(sample);  // Load information about a given dataset
    
    G_Event_Weight = dm->GetCrossSection() * G_Event_Lumi / dm->GetEventsInTheSample();

    cout << endl;
    cout << "      x-section = " << dm->GetCrossSection()      << endl;
    cout << "     luminosity = " << G_Event_Lumi               << endl;
    cout << "        nevents = " << dm->GetEventsInTheSample() << endl;
    cout << " base file name = " << dm->GetBaseFileName()      << endl;
    cout << "         weight = " << G_Event_Weight             << endl;
    cout << endl;

    gPAFOptions->dataFiles = dm->GetFiles();
  }


  // Output file name
  //----------------------------------------------------------------------------
  TString outputDir = "../WZXS8TeV/rootfiles/Summer12_53X/" + folder;

  gSystem->mkdir(outputDir, kTRUE);

  TString outputFile = outputDir + "/" + sample + ".root";
  
  gPAFOptions->outputFile = outputFile;


  // Parameters for the analysis
  //----------------------------------------------------------------------------
  gPAFOptions->inputParameters = new InputParameters();

  gPAFOptions->inputParameters->SetNamedString("folder",     folder.Data());
  gPAFOptions->inputParameters->SetNamedString("sample",     sample.Data());
  gPAFOptions->inputParameters->SetNamedDouble("weight",     G_Event_Weight);
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
