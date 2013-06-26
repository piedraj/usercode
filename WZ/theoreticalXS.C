//------------------------------------------------------------------------------
//
// theoreticalXS
//
// MCFM-6.6
//
// 40              [itmx1, number of iterations for pre-conditioning]
// 20000           [ncall1]
// 40              [itmx2, number of iterations for final run]
// 20000           [ncall2]
//
//------------------------------------------------------------------------------
Double_t xsPositive [3];
Double_t xsNegative [3];
Double_t xsInclusive[3];


void theoreticalXS(Bool_t mstw8nlo = false)
{
  if (mstw8nlo)
    {
      printf("\n mstw8nlo\n\n");

      // -1d0
      xsPositive[0] = 13863.552;  // +/-    14.553 fb
      xsNegative[0] =  8041.464;  // +/-     8.051 fb

      // 171.58 GeV
      xsPositive[1] = 13308.880;  // +/-    14.289 fb
      xsNegative[1] =  7718.715;  // +/-     7.873 fb

      // 42.90 GeV
      xsPositive[2] = 14593.260;  // +/-    15.699 fb
      xsNegative[2] =  8478.544;  // +/-     8.445 fb
    }
  else
    {
      printf("\n CT10.00\n\n");

      // -1d0
      xsPositive[0] = 13746.702;  // +/-    14.530 fb
      xsNegative[0] =  7729.026;  // +/-     7.852 fb

      // 171.58 GeV
      xsPositive[1] = 13200.188;  // +/-    13.774 fb
      xsNegative[1] =  7426.924;  // +/-     7.371 fb

      // 42.90 GeV
      xsPositive[2] = 14433.613;  // +/-    15.644 fb
      xsNegative[2] =  8136.993;  // +/-     8.130 fb
    }


  // Do the algebra
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<3; i++)
    {
      xsPositive[i] /= 1e3;
      xsNegative[i] /= 1e3;

      xsInclusive[i] = xsPositive[i] + xsNegative[i];
    }

  PrintABC("xs(W+Z)", " pb", 5, 2, xsPositive [0], xsPositive [1], xsPositive [2]);
  PrintABC("xs(W-Z)", " pb", 5, 2, xsNegative [0], xsNegative [1], xsNegative [2]);
  PrintABC("xs(W Z)", " pb", 5, 2, xsInclusive[0], xsInclusive[1], xsInclusive[2]);

  printf("\n");

  Ratio("xs(W+Z) / xs(W-Z)", xsPositive, xsNegative);
  Ratio("xs(W-Z) / xs(W+Z)", xsNegative, xsPositive);

  printf("\n");
}


//------------------------------------------------------------------------------
// Ratio
//------------------------------------------------------------------------------
void Ratio(TString   name,
	   Double_t* num,
	   Double_t* den)
{
  Double_t ratio[3];
  
  for (UInt_t i=0; i<3; i++) ratio[i] = num[i] / den[i];

  PrintABC(name, "", 6, 3, ratio[0], ratio[1], ratio[2]);
}


//------------------------------------------------------------------------------
// PrintABC
//------------------------------------------------------------------------------
void PrintABC(TString  name,
	      TString  units,
	      Int_t    precision1,
	      Int_t    precision2,
	      Double_t x0,
	      Double_t x1,
	      Double_t x2)
{
  TString title = Form(" %s%s = %s%d.%df %s%d.%df  %s%d.%df%s%s\n",
		       "%", "s",
		       "%", precision1, precision2,
		       "%", precision1, precision2,
		       "%", precision1, precision2,
		       "%", "s");

  printf(Form(title.Data(), name.Data(), x0, x1-x0, x2-x0, units.Data()));
}
