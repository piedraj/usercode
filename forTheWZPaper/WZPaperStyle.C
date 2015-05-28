const Font_t _cmsTextFont   = 61;
const Font_t _extraTextFont = 52;
const Font_t _lumiTextFont  = 42;


void WZPaperStyle()
{
  TStyle* WZPaperStyle = new TStyle("WZPaperStyle", "WZPaperStyle");

  gStyle = WZPaperStyle;


  //----------------------------------------------------------------------------
  // Canvas
  //----------------------------------------------------------------------------
  WZPaperStyle->SetCanvasBorderMode(  0);
  WZPaperStyle->SetCanvasBorderSize( 10);
  WZPaperStyle->SetCanvasColor     (  0);
  WZPaperStyle->SetCanvasDefH      (600);
  WZPaperStyle->SetCanvasDefW      (550);
  WZPaperStyle->SetCanvasDefX      ( 10);
  WZPaperStyle->SetCanvasDefY      ( 10);


  //----------------------------------------------------------------------------
  // Pad
  //----------------------------------------------------------------------------
  WZPaperStyle->SetPadBorderMode  (   0);
  WZPaperStyle->SetPadBorderSize  (  10);
  WZPaperStyle->SetPadColor       (   0);
  WZPaperStyle->SetPadBottomMargin(0.20);
  WZPaperStyle->SetPadTopMargin   (0.08);
  WZPaperStyle->SetPadLeftMargin  (0.18);
  WZPaperStyle->SetPadRightMargin (0.05);


  //----------------------------------------------------------------------------
  // Frame
  //----------------------------------------------------------------------------
  WZPaperStyle->SetFrameBorderMode( 0);
  WZPaperStyle->SetFrameBorderSize(10);
  WZPaperStyle->SetFrameFillStyle ( 0);
  WZPaperStyle->SetFrameFillColor ( 0);
  WZPaperStyle->SetFrameLineColor ( 1);
  WZPaperStyle->SetFrameLineStyle ( 0);
  WZPaperStyle->SetFrameLineWidth ( 2);


  //----------------------------------------------------------------------------
  // Hist
  //----------------------------------------------------------------------------
  WZPaperStyle->SetHistFillColor(0);
  WZPaperStyle->SetHistFillStyle(1);
  WZPaperStyle->SetHistLineColor(1);
  WZPaperStyle->SetHistLineStyle(0);
  WZPaperStyle->SetHistLineWidth(1);


  //----------------------------------------------------------------------------
  // Axis
  //----------------------------------------------------------------------------
  WZPaperStyle->SetLabelFont  (   42, "xyz");
  WZPaperStyle->SetLabelOffset(0.015, "xyz");
  WZPaperStyle->SetLabelSize  (0.040, "xyz");
  WZPaperStyle->SetNdivisions (  505, "xyz");
  WZPaperStyle->SetTitleFont  (   42, "xyz");
  WZPaperStyle->SetTitleSize  (0.040, "xyz");
  WZPaperStyle->SetTitleOffset( 1.15,   "x");
  WZPaperStyle->SetTitleOffset( 1.15,   "y");
  WZPaperStyle->SetPadTickX   (           1);
  WZPaperStyle->SetPadTickY   (           1);


  //----------------------------------------------------------------------------
  // Title
  //----------------------------------------------------------------------------
  WZPaperStyle->SetTitleAlign     (   33);
  WZPaperStyle->SetTitleBorderSize(    0);
  WZPaperStyle->SetTitleFillColor (   10);
  WZPaperStyle->SetTitleFont      (   42);
  WZPaperStyle->SetTitleFontSize  (0.045);
  WZPaperStyle->SetTitleX         (0.960);
  WZPaperStyle->SetTitleY         (0.990);


  //----------------------------------------------------------------------------
  // Stat
  //----------------------------------------------------------------------------
  WZPaperStyle->SetOptStat       (   0);
  WZPaperStyle->SetStatBorderSize(   0);
  WZPaperStyle->SetStatColor     (  10);
  WZPaperStyle->SetStatFont      (  42);
  WZPaperStyle->SetStatX         (0.94);
  WZPaperStyle->SetStatY         (0.91);
}
