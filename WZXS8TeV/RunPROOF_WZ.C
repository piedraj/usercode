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
//Double_t G_Event_Lumi   = 12103.3;  // pb
Double_t G_Event_Lumi   = 19468.3;  // pb
TProof*  proof          = 0;
TString  dataPath       = "/hadoop";


void RunPROOF_WZ(TString  sample  = "DoubleMu",
		 Long64_t nEvents = -1,
		 Bool_t   update  = true)
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
  gROOT->LoadMacro("../DatasetManager/DatasetManager.C+");

  if (sample.Contains("DoubleElectron") || sample.Contains("DoubleMu")) {

    gPAFOptions->dataFiles = GetRealDataFiles("MC_Summer12_53X/WH", sample.Data());
  }
  else {

    DatasetManager* dm = new DatasetManager("Summer12_53X", "WH");

    if (update) dm->RedownloadFiles();

    dm->LoadDataset(sample);

    gPAFOptions->dataFiles = dm->GetFiles();

    G_Event_Weight = dm->GetCrossSection() * G_Event_Lumi / dm->GetEventsInTheSample();

    cout << endl;
    cout << "      x-section = " << dm->GetCrossSection()      << endl;
    cout << "     luminosity = " << G_Event_Lumi               << endl;
    cout << "        nevents = " << dm->GetEventsInTheSample() << endl;
    cout << " base file name = " << dm->GetBaseFileName()      << endl;
    cout << "         weight = " << G_Event_Weight             << endl;
    cout << endl;
  }


  // Output file name
  //----------------------------------------------------------------------------
  TString outputDir = "../WZXS8TeV/results/Summer12_53X/WH";

  gSystem->mkdir(outputDir, kTRUE);

  TString outputFile = outputDir + "/" + sample + ".root";
  
  gPAFOptions->outputFile = outputFile;


  // Parameters for the analysis
  //----------------------------------------------------------------------------
  gPAFOptions->inputParameters = new InputParameters();

  gPAFOptions->inputParameters->SetNamedString("directory",  outputDir.Data());
  gPAFOptions->inputParameters->SetNamedString("sample",     sample.Data());
  gPAFOptions->inputParameters->SetNamedDouble("xs_weight",  G_Event_Weight);
  gPAFOptions->inputParameters->SetNamedDouble("luminosity", G_Event_Lumi);


  // Number of events (Long64_t)
  //----------------------------------------------------------------------------
  gPAFOptions->nEvents = nEvents;

  
  // Name of analysis class
  //----------------------------------------------------------------------------
  gPAFOptions->myAnalysis = "AnalysisWZ";


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


//------------------------------------------------------------------------------
// GetRealDataFiles
//------------------------------------------------------------------------------
vector<TString> GetRealDataFiles(const char* relativepath,
				 const char* filebasename)
{
  vector<TString> theFiles;

  TString basefile(filebasename);

  TString fullpath = dataPath + "/" + relativepath;

  TString command("ls ");

  command += 
    fullpath + "/Tree_" + basefile + "A_892.root " +
    fullpath + "/Tree_" + basefile + "A_892_[0-9].root " +
    fullpath + "/Tree_" + basefile + "A_892_[0-9][0-9].root " +
    fullpath + "/Tree_" + basefile + "B_4404.root " +
    fullpath + "/Tree_" + basefile + "B_4404_[0-9].root " +
    fullpath + "/Tree_" + basefile + "B_4404_[0-9][0-9].root " +
    fullpath + "/Tree_" + basefile + "C_6807.root " +
    fullpath + "/Tree_" + basefile + "C_6807_[0-9].root " +
    fullpath + "/Tree_" + basefile + "C_6807_[0-9][0-9].root " +
    fullpath + "/Tree_" + basefile + "C_91.root " +
    fullpath + "/Tree_" + basefile + "C_ReReco11Dec_134.root " +
    fullpath + "/Tree_" + basefile + "D_7274.root " +
    fullpath + "/Tree_" + basefile + "D_7274_[0-9].root " +
    fullpath + "/Tree_" + basefile + "D_7274_[0-9][0-9].root";

  command += " 2> /dev/null";

  TString result;

  FILE* pipe = gSystem->OpenPipe(command, "r");

  if (!pipe) {
    cerr << "ERROR: in RunPROOF_WZ::GetRealDataFiles. Cannot run command \""
	 << command << "\"" << endl;
  }
  else {
    TString line;
    while (line.Gets(pipe)) {
      if (result != "")
	result += "\n";
      result += line;
    }
  
    gSystem->ClosePipe(pipe);
  }

  if (result != "") {
    TObjArray* filesfound = result.Tokenize(TString('\n'));
    if (!filesfound)
      cerr << "ERROR: Could not parse output while finding files" << endl;
    else {
      for (int i=0; i<filesfound->GetEntries(); i++) {
	
	theFiles.push_back(filesfound->At(i)->GetName());
      }
      filesfound->Clear();

      delete filesfound;
    }
  }

  if (theFiles.size() == 0)
    cerr << "ERROR: Could not find data!" << endl;

  return theFiles;
}
