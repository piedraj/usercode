void makeclass()
{
  TString path = "/pool/ciencias/LatinosSkims/MuData8TeVRun2012ABC/";

  TChain* chain = new TChain("Tree", "Tree");

  chain->Add(path + "Tree_DoubleMuA_810_FR.root");
  chain->Add(path + "Tree_DoubleMuA_ECALrecovery_82pbinv_FR.root");
  chain->Add(path + "Tree_DoubleMuB_4404pbinv_FR.root");
  chain->Add(path + "Tree_DoubleMuC_6312pbinv_FR.root");
  chain->Add(path + "Tree_DoubleMuC_91pbinv_FR.root");
  chain->Add(path + "Tree_DoubleMuC_ReReco_495pbinv_FR.root");
  chain->Add(path + "Tree_DoubleMuD_7274pbinv_MuFR.root");

  chain->MakeClass("MuonFakeRate");
}
