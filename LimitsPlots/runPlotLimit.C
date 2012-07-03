void runPlotLimit()
{
  gInterpreter->LoadMacro("PlotLimit.C+");

  PlotLimit("inputs/ana_v7_Full2011_limits_nj_shape.txt", "limit7TeV", "4.9 fb^{-1} (7 TeV)",  90, 600, 0, 0);
  PlotLimit("inputs/ana_v7_Full2011_limits_nj_shape.txt", "limit7TeV", "4.9 fb^{-1} (7 TeV)", 100, 300, 0, 0);
  PlotLimit("inputs/ana_v7_Full2011_limits_nj_shape.txt", "limit7TeV", "4.9 fb^{-1} (7 TeV)", 105, 600, 1, 0);
  PlotLimit("inputs/ana_v7_Full2011_limits_nj_shape.txt", "limit7TeV", "4.9 fb^{-1} (7 TeV)", 105, 300, 1, 0);
  PlotLimit("inputs/ana_v7_Full2011_limits_nj_shape.txt", "limit7TeV", "4.9 fb^{-1} (7 TeV)", 105, 600, 1, 1);
  PlotLimit("inputs/ana_v7_Full2011_limits_nj_shape.txt", "limit7TeV", "4.9 fb^{-1} (7 TeV)", 105, 300, 1, 1);

  PlotLimit("inputs/ana_ICHEP_limits_nj_cut_8TeV.txt","limit8TeV", "5.1 fb^{-1} (8 TeV)",  90, 600, 0, 0);
  PlotLimit("inputs/ana_ICHEP_limits_nj_cut_8TeV.txt","limit8TeV", "5.1 fb^{-1} (8 TeV)", 100, 300, 0, 0);
  PlotLimit("inputs/ana_ICHEP_limits_nj_cut_8TeV.txt","limit8TeV", "5.1 fb^{-1} (8 TeV)", 105, 600, 1, 0);
  PlotLimit("inputs/ana_ICHEP_limits_nj_cut_8TeV.txt","limit8TeV", "5.1 fb^{-1} (8 TeV)", 105, 300, 1, 0);
  PlotLimit("inputs/ana_ICHEP_limits_nj_cut_8TeV.txt","limit8TeV", "5.1 fb^{-1} (8 TeV)", 105, 600, 1, 1);
  PlotLimit("inputs/ana_ICHEP_limits_nj_cut_8TeV.txt","limit8TeV", "5.1 fb^{-1} (8 TeV)", 105, 300, 1, 1);

  PlotLimit("inputs/ana_ICHEP_limits_nj_shape7teV_cut8TeV.txt", "combined", "4.9 fb^{-1} (7 TeV) + 5.1 fb^{-1} (8 TeV)",  90, 600, 0, 0);
  PlotLimit("inputs/ana_ICHEP_limits_nj_shape7teV_cut8TeV.txt", "combined", "4.9 fb^{-1} (7 TeV) + 5.1 fb^{-1} (8 TeV)", 100, 300, 0, 0);
  PlotLimit("inputs/ana_ICHEP_limits_nj_shape7teV_cut8TeV.txt", "combined", "4.9 fb^{-1} (7 TeV) + 5.1 fb^{-1} (8 TeV)", 105, 600, 1, 0);
  PlotLimit("inputs/ana_ICHEP_limits_nj_shape7teV_cut8TeV.txt", "combined", "4.9 fb^{-1} (7 TeV) + 5.1 fb^{-1} (8 TeV)", 105, 300, 1, 0);
  PlotLimit("inputs/ana_ICHEP_limits_nj_shape7teV_cut8TeV.txt", "combined", "4.9 fb^{-1} (7 TeV) + 5.1 fb^{-1} (8 TeV)", 105, 600, 1, 1);
  PlotLimit("inputs/ana_ICHEP_limits_nj_shape7teV_cut8TeV.txt", "combined", "4.9 fb^{-1} (7 TeV) + 5.1 fb^{-1} (8 TeV)", 105, 300, 1, 1);
}
