{
  gInterpreter->ExecuteMacro("HiggsPaperStyle.C");
  gInterpreter->LoadMacro   ("DrawFunctions.C+");
  gStyle      ->SetOptStat  (0);
  gStyle      ->SetPalette  (1);
}
