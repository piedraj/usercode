void CmsPaperStyle()
{
  TStyle* CmsPaperStyle = new TStyle("CmsPaperStyle", "CmsPaperStyle");

  gStyle = CmsPaperStyle;


  //----------------------------------------------------------------------------
  // Canvas
  //----------------------------------------------------------------------------
  CmsPaperStyle->SetCanvasBorderMode(  0);
  CmsPaperStyle->SetCanvasBorderSize( 10);
  CmsPaperStyle->SetCanvasColor     (  0);
  CmsPaperStyle->SetCanvasDefH      (600);
  CmsPaperStyle->SetCanvasDefW      (550);
  CmsPaperStyle->SetCanvasDefX      ( 10);
  CmsPaperStyle->SetCanvasDefY      ( 10);


  //----------------------------------------------------------------------------
  // Pad
  //----------------------------------------------------------------------------
  CmsPaperStyle->SetPadBorderMode  (   0);
  CmsPaperStyle->SetPadBorderSize  (  10);
  CmsPaperStyle->SetPadColor       (   0);
  CmsPaperStyle->SetPadBottomMargin(0.20);
  CmsPaperStyle->SetPadTopMargin   (0.08);
  CmsPaperStyle->SetPadLeftMargin  (0.18);
  CmsPaperStyle->SetPadRightMargin (0.05);


  //----------------------------------------------------------------------------
  // Frame
  //----------------------------------------------------------------------------
  CmsPaperStyle->SetFrameBorderMode( 0);
  CmsPaperStyle->SetFrameBorderSize(10);
  CmsPaperStyle->SetFrameFillStyle ( 0);
  CmsPaperStyle->SetFrameFillColor ( 0);
  CmsPaperStyle->SetFrameLineColor ( 1);
  CmsPaperStyle->SetFrameLineStyle ( 0);
  CmsPaperStyle->SetFrameLineWidth ( 2);


  //----------------------------------------------------------------------------
  // Hist
  //----------------------------------------------------------------------------
  CmsPaperStyle->SetHistFillColor(0);
  CmsPaperStyle->SetHistFillStyle(1);
  CmsPaperStyle->SetHistLineColor(1);
  CmsPaperStyle->SetHistLineStyle(0);
  CmsPaperStyle->SetHistLineWidth(1);


  //----------------------------------------------------------------------------
  // Axis
  //----------------------------------------------------------------------------
  CmsPaperStyle->SetLabelFont  (   42, "xyz");
  CmsPaperStyle->SetLabelOffset(0.015, "xyz");
  CmsPaperStyle->SetLabelSize  (0.040, "xyz");
  CmsPaperStyle->SetNdivisions (  505, "xyz");
  CmsPaperStyle->SetTitleFont  (   42, "xyz");
  CmsPaperStyle->SetTitleSize  (0.040, "xyz");
  CmsPaperStyle->SetTitleOffset( 1.15,   "x");
  CmsPaperStyle->SetTitleOffset( 1.15,   "y");
  CmsPaperStyle->SetPadTickX   (           1);
  CmsPaperStyle->SetPadTickY   (           1);


  //----------------------------------------------------------------------------
  // Title
  //----------------------------------------------------------------------------
  CmsPaperStyle->SetTitleAlign     (   33);
  CmsPaperStyle->SetTitleBorderSize(    0);
  CmsPaperStyle->SetTitleFillColor (   10);
  CmsPaperStyle->SetTitleFont      (   42);
  CmsPaperStyle->SetTitleFontSize  (0.045);
  CmsPaperStyle->SetTitleX         (0.960);
  CmsPaperStyle->SetTitleY         (0.990);


  //----------------------------------------------------------------------------
  // Stat
  //----------------------------------------------------------------------------
  CmsPaperStyle->SetOptStat       (   0);
  CmsPaperStyle->SetStatBorderSize(   0);
  CmsPaperStyle->SetStatColor     (  10);
  CmsPaperStyle->SetStatFont      (  42);
  CmsPaperStyle->SetStatX         (0.94);
  CmsPaperStyle->SetStatY         (0.91);
}
