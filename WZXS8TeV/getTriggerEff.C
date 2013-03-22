// scp lxplus.cern.ch:/afs/cern.ch/user/d/dmeister/public/LatinoTrees/triggW/NewVals.txt           www/AuxiliaryFilesWZXS8TeV/.
// scp lxplus.cern.ch:/afs/cern.ch/user/d/dmeister/public/LatinoTrees/triggW/triggerEffCombiner.py www/AuxiliaryFilesWZXS8TeV/.


const TString path = "/nfs/fanae/user/piedra/www/AuxiliaryFilesWZXS8TeV/";


void getTriggerEff()
{
  TFile* file = new TFile(path + "triggerEfficiencies.root", "recreate");

  TH2F* DoubleElLead  = MakeTH2("DoubleElLead");
  TH2F* DoubleMuLead  = MakeTH2("DoubleMuLead");
  TH2F* DoubleElTrail = MakeTH2("DoubleElTrail");
  TH2F* DoubleMuTrail = MakeTH2("DoubleMuTrail");

  DoubleElLead ->Write();
  DoubleMuLead ->Write();
  DoubleElTrail->Write();
  DoubleMuTrail->Write();

  file->Close();
}


//------------------------------------------------------------------------------
// MakeTH2
//------------------------------------------------------------------------------
TH2F* MakeTH2(TString trigger)
{
  Double_t hptmax = 100.;

  std::vector<Double_t> etav;
  std::vector<Double_t> ptv;
  
  TString name;

  Double_t leta, heta, lpt, hpt, eff;

  ifstream in;

  in.open(path + "NewVals.txt");

  while (1)
    {
      in >> name >> leta >> heta >> lpt >> hpt >> eff;
      
      if (!in.good()) break;

      if (name != trigger) continue;

      if (hpt > hptmax) hpt = hptmax;

      if (etav.size() == 0)
	{
	  etav.push_back(leta);
	  etav.push_back(heta);
	}
      else if (heta > etav.back())
	{
	  etav.push_back(heta);
	}

      if (ptv.size() == 0)
	{
	  ptv.push_back(lpt);
	  ptv.push_back(hpt);
	}
      else if (hpt > ptv.back())
	{
	  ptv.push_back(hpt);
	}
    }

  in.close();

  Double_t* x = &ptv[0];
  Double_t* y = &etav[0];

  TH2F* h2 = new TH2F(trigger, trigger, ptv.size()-1, x, etav.size()-1, y);


  // Now fill the histogram
  //----------------------------------------------------------------------------
  in.open(path + "NewVals.txt");

  while (1)
    {
      in >> name >> leta >> heta >> lpt >> hpt >> eff;

      if (!in.good()) break;

      if (name != trigger) continue;

      if (hpt > hptmax) hpt = hptmax;

      Double_t eta = 0.5 * (heta + leta);
      Double_t pt  = 0.5 * (hpt  + lpt);

      h2->SetBinContent(h2->FindBin(pt,eta), eff); 
    }

  in.close();

  return h2;
}

