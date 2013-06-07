void mcfmEfficiencies()
{
  Float_t eff_cuts = 36.49 + 39.84;
  Float_t eff      = 94.40 + 94.64;
  Float_t ratio    = eff_cuts / eff;

  Float_t eff_cuts_scale_x2 = 36.11 + 39.13;
  Float_t eff_scale_x2      = 94.64 + 94.74;
  Float_t ratio_scale_x2    = eff_cuts_scale_x2 / eff_scale_x2;

  Float_t eff_cuts_scale_x05 = 36.53 + 39.48;
  Float_t eff_scale_x05      = 94.40 + 94.72;
  Float_t ratio_scale_x05    = eff_cuts_scale_x05 / eff_scale_x05;

  printf("\n");
  printf("           ratio = %.3f\n", ratio);
  printf("  ratio_scale_x2 = %.3f\n", ratio_scale_x2);
  printf(" ratio_scale_x05 = %.3f\n", ratio_scale_x05);
  printf("\n");
  printf("  delta_scale_x2 = %.1f%s\n", 1e2 * fabs(ratio_scale_x2 - ratio) / ratio, "%");
  printf(" delta_scale_x05 = %.1f%s\n", 1e2 * fabs(ratio_scale_x05 - ratio) / ratio, "%");
  printf("\n");
}


/*
--------------------------------------------------------------------------------
./mcfm myInputs/WPlus/inputWPlusZ_cuts.DAT
./mcfm myInputs/WMinus/inputWMinusZ_cuts.DAT
--------------------------------------------------------------------------------
Value of final tota integral is       14.783 +/-     0.033 fb

 Total number of shots       :      4997120
 Total no. failing cuts      :      3422606
 Number failing jet cuts     :            0
 Number failing process cuts :      2740642

   Jet efficiency : 100.00%
   Cut efficiency :  36.49%
 Total efficiency :  36.49%


Value of final tota integral is        8.573 +/-     0.016 fb

 Total number of shots       :      4997120
 Total no. failing cuts      :      3250907
 Number failing jet cuts     :            0
 Number failing process cuts :      2636870

   Jet efficiency : 100.00%
   Cut efficiency :  39.84%
 Total efficiency :  39.84%


--------------------------------------------------------------------------------
./mcfm myInputs/WPlus/inputWPlusZ.DAT
./mcfm myInputs/WMinus/inputWMinusZ.DAT
--------------------------------------------------------------------------------
Value of final tota integral is       48.127 +/-     0.041 fb

 Total number of shots       :      4997120
 Total no. failing cuts      :       332771
 Number failing jet cuts     :            0
 Number failing process cuts :       276789

   Jet efficiency : 100.00%
   Cut efficiency :  94.40%
 Total efficiency :  94.40%


Value of final tota integral is       27.904 +/-     0.022 fb

 Total number of shots       :      4997120
 Total no. failing cuts      :       322663
 Number failing jet cuts     :            0
 Number failing process cuts :       264730

   Jet efficiency : 100.00%
   Cut efficiency :  94.64%
 Total efficiency :  94.64%


--------------------------------------------------------------------------------
./mcfm myInputs/WPlus/inputWPlusZ_cuts_scale_x2.DAT
./mcfm myInputs/WMinus/inputWMinusZ_cuts_scale_x2.DAT
--------------------------------------------------------------------------------
Value of final tota integral is       14.186 +/-     0.030 fb

 Total number of shots       :      4997120
 Total no. failing cuts      :      3442320
 Number failing jet cuts     :            0
 Number failing process cuts :      2751075

   Jet efficiency : 100.00%
   Cut efficiency :  36.11%
 Total efficiency :  36.11%


Value of final tota integral is        8.215 +/-     0.015 fb

 Total number of shots       :      4997120
 Total no. failing cuts      :      3285049
 Number failing jet cuts     :            0
 Number failing process cuts :      2663004

   Jet efficiency : 100.00%
   Cut efficiency :  39.13%
 Total efficiency :  39.13%


--------------------------------------------------------------------------------
./mcfm myInputs/WPlus/inputWPlusZ_scale_x2.DAT
./mcfm myInputs/WMinus/inputWMinusZ_scale_x2.DAT
--------------------------------------------------------------------------------
Value of final tota integral is       46.205 +/-     0.039 fb

 Total number of shots       :      4997120
 Total no. failing cuts      :       322951
 Number failing jet cuts     :            0
 Number failing process cuts :       264770

   Jet efficiency : 100.00%
   Cut efficiency :  94.64%
 Total efficiency :  94.64%


Value of final tota integral is       26.763 +/-     0.021 fb

 Total number of shots       :      4997120
 Total no. failing cuts      :       315972
 Number failing jet cuts     :            0
 Number failing process cuts :       259807

   Jet efficiency : 100.00%
   Cut efficiency :  94.74%
 Total efficiency :  94.74%


--------------------------------------------------------------------------------
./mcfm myInputs/WPlus/inputWPlusZ_cuts_scale_x05.DAT
./mcfm myInputs/WMinus/inputWMinusZ_cuts_scale_x05.DAT
--------------------------------------------------------------------------------
Value of final tota integral is       15.610 +/-     0.040 fb

 Total number of shots       :      4997120
 Total no. failing cuts      :      3427920
 Number failing jet cuts     :            0
 Number failing process cuts :      2726767

   Jet efficiency : 100.00%
   Cut efficiency :  36.53%
 Total efficiency :  36.53%


Value of final tota integral is        9.102 +/-     0.017 fb

 Total number of shots       :      4997120
 Total no. failing cuts      :      3264961
 Number failing jet cuts     :            0
 Number failing process cuts :      2655451

   Jet efficiency : 100.00%
   Cut efficiency :  39.48%
 Total efficiency :  39.48%


--------------------------------------------------------------------------------
./mcfm myInputs/WPlus/inputWPlusZ_scale_x05.DAT
./mcfm myInputs/WMinus/inputWMinusZ_scale_x05.DAT
--------------------------------------------------------------------------------
Value of final tota integral is       50.638 +/-     0.042 fb

 Total number of shots       :      4997120
 Total no. failing cuts      :       333909
 Number failing jet cuts     :            0
 Number failing process cuts :       276883

   Jet efficiency : 100.00%
   Cut efficiency :  94.40%
 Total efficiency :  94.40%


Value of final tota integral is       29.405 +/-     0.023 fb

 Total number of shots       :      4997120
 Total no. failing cuts      :       317655
 Number failing jet cuts     :            0
 Number failing process cuts :       261021

   Jet efficiency : 100.00%
   Cut efficiency :  94.72%
 Total efficiency :  94.72%
*/
