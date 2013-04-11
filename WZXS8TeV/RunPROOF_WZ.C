#include "scripts/PAFUtils.C"


Double_t luminosity    = 19602.0;  // pb
Double_t pu_luminosity = 19468.3;  // pb
Double_t xs_weight     = 1;
TProof*  proof         = 0;
TString  dataPath      = "";


enum {RAW, PPF};

enum {
  noSyst,
  metSyst,
  muonUpSyst,
  muonDownSyst,
  electronUpSyst,
  electronDownSyst
};


void RunPROOF_WZ(TString  sample     = "WZTo3LNu",
		 Int_t    mode       = RAW,
		 Int_t    systematic = noSyst,
		 Long64_t nEvents    = -1,
		 Bool_t   update     = true)
{
  GuessDataPath();

  Int_t runAtOviedo = (dataPath.Contains("ifca")) ? 0 : 1;


  // Reset PROOF
  //----------------------------------------------------------------------------
  gSystem->Exec("./resetproof.sh -a");
  gSystem->Exec("rm -f *.d");
  gSystem->Exec("rm -f *.so");

  if (runAtOviedo) gSystem->Exec("qdel all");
  else             gSystem->Exec("endproof");


  // PROOF mode
  //----------------------------------------------------------------------------
  gPAFOptions->NSlots = 30;

  if (runAtOviedo) gPAFOptions->proofMode = kPoD;
  else             gPAFOptions->proofMode = kCluster;           

  //  gPAFOptions->proofMode = kSequential;
  //  gPAFOptions->proofMode = kLite;


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

  if (sample.Contains("DoubleElectron") ||
      sample.Contains("DoubleMu") ||
      sample.Contains("MuEG"))
    {
      gPAFOptions->dataFiles = GetRealDataFiles("MC_Summer12_53X/WH/CalibratedE", sample.Data());
    }
  else
    {
      DatasetManager* dm = new DatasetManager("Summer12_53X", "WH");

      if (update) dm->RedownloadFiles();

      dm->LoadDataset(sample);

      gPAFOptions->dataFiles = dm->GetFiles();

      xs_weight = dm->GetCrossSection() * luminosity / dm->GetEventsInTheSample();

      cout << endl;
      cout << "             xs = " << dm->GetCrossSection()      << " pb"   << endl;
      cout << "     luminosity = " << luminosity                 << " pb-1" << endl;
      cout << "        nevents = " << dm->GetEventsInTheSample() << endl;
      cout << " base file name = " << dm->GetBaseFileName()      << endl;
      cout << "         weight = " << xs_weight                  << endl;
      cout << endl;
    }


  // Output file name
  //----------------------------------------------------------------------------
  TString outputDir = "../WZXS8TeV/results/Summer12_53X/WH";

  if      (systematic == noSyst)           outputDir += "/analysis";
  else if (systematic == metSyst)          outputDir += "/systematics/met";
  else if (systematic == muonUpSyst)       outputDir += "/systematics/muonUp";
  else if (systematic == muonDownSyst)     outputDir += "/systematics/muonDown";
  else if (systematic == electronUpSyst)   outputDir += "/systematics/electronUp";
  else if (systematic == electronDownSyst) outputDir += "/systematics/electronDown";

  gSystem->mkdir(outputDir, kTRUE);

  if (mode == PPF) sample += "_PPF";

  TString outputFile = outputDir + "/" + sample + ".root";
  
  gPAFOptions->outputFile = outputFile;


  // Parameters for the analysis
  //----------------------------------------------------------------------------
  gPAFOptions->inputParameters = new InputParameters();

  gPAFOptions->inputParameters->SetNamedString("directory",     outputDir.Data());
  gPAFOptions->inputParameters->SetNamedString("sample",        sample.Data());
  gPAFOptions->inputParameters->SetNamedDouble("luminosity",    luminosity);
  gPAFOptions->inputParameters->SetNamedDouble("pu_luminosity", pu_luminosity);
  gPAFOptions->inputParameters->SetNamedDouble("xs_weight",     xs_weight);
  gPAFOptions->inputParameters->SetNamedInt   ("mode",          mode);
  gPAFOptions->inputParameters->SetNamedInt   ("systematic",    systematic);
  gPAFOptions->inputParameters->SetNamedInt   ("runAtOviedo",   runAtOviedo);


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
// GuessDataPath
//------------------------------------------------------------------------------
void GuessDataPath()
{
  TString host = gSystem->HostName();

  dataPath = "/pool/ciencias";

  if (host.Contains("ifca"))
    {
      dataPath = TString("/gpfs/csic_projects/tier3data");
    }
}
