{
  gInterpreter->ExecuteMacro("./draw/ChargeRatioStyle.C");
  //  gInterpreter->ExecuteMacro("./draw/setTDRStyle.C");
  gInterpreter->LoadMacro   ("./draw/draw.C+");
  gStyle      ->SetOptStat  (0);
  gStyle      ->SetPalette  (1);
}
