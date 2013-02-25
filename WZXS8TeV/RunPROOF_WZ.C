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


enum {RAW, PPF, PPP};


Double_t G_Event_Weight = 1;
Double_t G_Event_Lumi   = 19468.3;  // pb
TProof*  proof          = 0;
TString  dataPath       = "";


void RunPROOF_WZ(TString  sample       = "DoubleMu",
		 Int_t    mode         = RAW,
		 Int_t    closure_test = 0,
		 Long64_t nEvents      = -1,
		 Bool_t   update       = true)
{
  dataPath = GuessLocalBasePath();


  // PROOF mode
  //----------------------------------------------------------------------------
  //  gPAFOptions->proofMode = kSequential;
  //  gPAFOptions->proofMode = kLite;
  gPAFOptions->proofMode = kCluster;
  //  gPAFOptions->proofMode = kPoD;
  gPAFOptions->NSlots    = 30;


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

  if (closure_test) outputDir += "/closure_test";

  gSystem->mkdir(outputDir, kTRUE);

  if (mode == PPF) sample += "_PPF";
  if (mode == PPP) sample += "_PPP";

  TString outputFile = outputDir + "/" + sample + ".root";
  
  gPAFOptions->outputFile = outputFile;


  // Parameters for the analysis
  //----------------------------------------------------------------------------
  gPAFOptions->inputParameters = new InputParameters();

  gPAFOptions->inputParameters->SetNamedString("directory",    outputDir.Data());
  gPAFOptions->inputParameters->SetNamedString("sample",       sample.Data());
  gPAFOptions->inputParameters->SetNamedDouble("xs_weight",    G_Event_Weight);
  gPAFOptions->inputParameters->SetNamedDouble("luminosity",   G_Event_Lumi);
  gPAFOptions->inputParameters->SetNamedInt   ("mode",         mode);
  gPAFOptions->inputParameters->SetNamedInt   ("closure_test", closure_test);


  // Number of events (Long64_t)
  //----------------------------------------------------------------------------
  if (nEvents == -1 && gPAFOptions->proofMode == kSequential)
    {
      TChain* chain = new TChain("Tree", "Tree");
  
      for (UInt_t i=0; i<gPAFOptions->dataFiles.size(); i++)
	chain->Add(gPAFOptions->dataFiles[i]);

      nEvents = chain->GetEntries();

      printf("\n [RunPROOF_WZ] The number of events is %ld\n\n", nEvents);
    }

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
    fullpath + "/Tree_" + basefile + "C_7032.root " +
    fullpath + "/Tree_" + basefile + "C_7032_[0-9].root " +
    fullpath + "/Tree_" + basefile + "C_7032_[0-9][0-9].root " +
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


//------------------------------------------------------------------------------
// GuessLocalBasePath
//------------------------------------------------------------------------------
TString GuessLocalBasePath()
{
  TString host = gSystem->HostName();

  TString localBasePath = "/pool/ciencias";

  if (host.Contains("ifca"))
    {
      localBasePath = TString("/gpfs/csic_projects/tier3data");
    }

  return localBasePath;
}
