#include "DrawFunctions.h"


const UInt_t nChannel = 4;

TString sChannel[nChannel] = {"MMM", "EEE", "MME", "EEM"};


Double_t dataMinusMc[nChannel] = {190.3, 55.9, 116.2,  87.5};  // From XS.C
Double_t dataPPF    [nChannel] = {205.3, 22.1,  44.5, 124.0};  // From XS.C

Double_t zjetsMc [nChannel];
Double_t zjetsPPF[nChannel];


void upgrades()
{
  TFile* file = new TFile("upgrades/analysis/ZJets.root");

  TFile* file_PPF = new TFile("upgrades/analysis/ZJets_PPF.root");

  for (UInt_t channel=0; channel<nChannel; channel++) {

    TString hname = "hCounter_" + sChannel[channel] + "_ClosureTest_WInclusive_LLL";

    zjetsMc[channel] = Yield((TH1D*)file->Get(hname));

    zjetsPPF[channel] = Yield((TH1D*)file_PPF->Get(hname));
  }

  printf("\n %30s",  "Z+jets MC");                  for (UInt_t channel=0; channel<nChannel; channel++) {printf(" & %3.0f  ", zjetsMc[channel]);}
  printf("\\\\");
  printf("\n %30s",  "data-driven from Z+jets MC"); for (UInt_t channel=0; channel<nChannel; channel++) {printf(" & %3.0f  ", zjetsPPF[channel]);}
  printf("\\\\");
  printf("\n %30s",  "$\\delta$");                  for (UInt_t channel=0; channel<nChannel; channel++) {printf(" & %3.0f%s", 1e2 * fabs(zjetsMc[channel] - zjetsPPF[channel]) / zjetsMc[channel], "\\%");}
  printf("\\\\\\hline");

  printf("\n %30s",  "Z+jets MC");                  for (UInt_t channel=0; channel<nChannel; channel++) {printf(" & %3.0f  ", zjetsMc[channel]);}
  printf("\\\\");
  printf("\n %30s",  "data-driven from data");      for (UInt_t channel=0; channel<nChannel; channel++) {printf(" & %3.0f  ", dataPPF[channel]);}
  printf("\\\\");
  printf("\n %30s",  "$\\delta$");                  for (UInt_t channel=0; channel<nChannel; channel++) {printf(" & %3.0f%s", 1e2 * fabs(zjetsMc[channel] - dataPPF[channel]) / zjetsMc[channel], "\\%");}
  printf("\\\\\\hline");

  printf("\n %30s",  "data $-$ MC");                for (UInt_t channel=0; channel<nChannel; channel++) {printf(" & %3.0f  ", dataMinusMc[channel]);}
  printf("\\\\");
  printf("\n %30s",  "data-driven from data");      for (UInt_t channel=0; channel<nChannel; channel++) {printf(" & %3.0f  ", dataPPF[channel]);}
  printf("\\\\");
  printf("\n %30s",  "$\\delta$");                  for (UInt_t channel=0; channel<nChannel; channel++) {printf(" & %3.0f%s", 1e2 * fabs(dataMinusMc[channel] - dataPPF[channel]) / dataMinusMc[channel], "\\%");}
  printf("\\\\\\hline\n");
}
