#include "TStyle.h"


//------------------------------------------------------------------------------
// tdrGrid
//------------------------------------------------------------------------------
void tdrGrid(bool gridOn)
{
  tdrStyle->SetPadGridX(gridOn);
  tdrStyle->SetPadGridY(gridOn);
}


//------------------------------------------------------------------------------
// fixOverlay
//------------------------------------------------------------------------------
void fixOverlay()
{
  gPad->RedrawAxis();
}


//------------------------------------------------------------------------------
// setTDRStyle
// http://cmsdoc.cern.ch/cms/cpt/tdr/tdrstyle.C
//------------------------------------------------------------------------------
void setTDRStyle()
{
  TStyle* tdrStyle = new TStyle("tdrStyle", "Style for P-TDR");


  // Canvas
  //----------------------------------------------------------------------------
  tdrStyle->SetCanvasBorderMode(     0);
  tdrStyle->SetCanvasColor     (kWhite);
  tdrStyle->SetCanvasDefH      (   600);
  tdrStyle->SetCanvasDefW      (   600);
  tdrStyle->SetCanvasDefX      (     0);
  tdrStyle->SetCanvasDefY      (     0);


  // Pad
  //----------------------------------------------------------------------------
  tdrStyle->SetPadBorderMode(     0);
  tdrStyle->SetPadColor     (kWhite);
  tdrStyle->SetPadGridX     ( false);
  tdrStyle->SetPadGridY     ( false);
  tdrStyle->SetGridColor    (     0);
  tdrStyle->SetGridStyle    (     3);
  tdrStyle->SetGridWidth    (     1);


  // Frame
  //----------------------------------------------------------------------------
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor (0);
  tdrStyle->SetFrameFillStyle (0);
  tdrStyle->SetFrameLineColor (1);
  tdrStyle->SetFrameLineStyle (1);
  tdrStyle->SetFrameLineWidth (1);


  // Hist
  //----------------------------------------------------------------------------
  tdrStyle->SetHistLineColor(          1);
  tdrStyle->SetHistLineStyle(          0);
  tdrStyle->SetHistLineWidth(          1);
  tdrStyle->SetEndErrorSize (          2);
  tdrStyle->SetErrorX       (        0.0);
  tdrStyle->SetMarkerStyle  (kFullCircle);


  // Function
  //----------------------------------------------------------------------------
  tdrStyle->SetOptFit   (     1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(     2);
  tdrStyle->SetFuncStyle(     1);
  tdrStyle->SetFuncWidth(     1);


  // Date
  //----------------------------------------------------------------------------
  tdrStyle->SetOptDate(0);


  // Statistics box
  //----------------------------------------------------------------------------
  tdrStyle->SetOptFile       (     0);
  tdrStyle->SetOptStat       (     0);
  tdrStyle->SetStatColor     (kWhite);
  tdrStyle->SetStatFont      (    42);
  tdrStyle->SetStatFontSize  ( 0.025);
  tdrStyle->SetStatTextColor (     1);
  tdrStyle->SetStatFormat    ("6.4g");
  tdrStyle->SetStatBorderSize(     1);
  tdrStyle->SetStatH         (  0.10);
  tdrStyle->SetStatW         (  0.15);


  // Margins
  //----------------------------------------------------------------------------
  tdrStyle->SetPadTopMargin   (0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin  (0.16);
  tdrStyle->SetPadRightMargin (0.02);


  // Global title
  //----------------------------------------------------------------------------
  tdrStyle->SetOptTitle      (   0);
  tdrStyle->SetTitleFont     (  42);
  tdrStyle->SetTitleColor    (   1);
  tdrStyle->SetTitleTextColor(   1);
  tdrStyle->SetTitleFillColor(  10);
  tdrStyle->SetTitleFontSize (0.05);


  // Axis titles
  //----------------------------------------------------------------------------
  tdrStyle->SetTitleColor  (   1, "XYZ");
  tdrStyle->SetTitleFont   (  42, "XYZ");
  tdrStyle->SetTitleSize   (0.06, "XYZ");
  tdrStyle->SetTitleXOffset(       0.90);
  tdrStyle->SetTitleYOffset(       1.25);

  
  // Axis labels
  //----------------------------------------------------------------------------
  tdrStyle->SetLabelColor (    1, "XYZ");
  tdrStyle->SetLabelFont  (   42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize  (0.050, "XYZ");


  // Axis
  //----------------------------------------------------------------------------
  tdrStyle->SetAxisColor    (   1, "XYZ");
  tdrStyle->SetTickLength   (0.03, "XYZ");
  tdrStyle->SetNdivisions   ( 510, "XYZ");
  tdrStyle->SetStripDecimals(      kTRUE);
  tdrStyle->SetPadTickX     (          1);  // Tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY     (          1);  // Tick marks on the opposite side of the frame


  // Postscript
  //----------------------------------------------------------------------------
  tdrStyle->SetPaperSize(20., 20.);

  tdrStyle->cd();
}
