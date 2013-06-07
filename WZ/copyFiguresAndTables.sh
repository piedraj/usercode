#!/bin/bash


INPUT_DIR=/nfs/fanae/user/piedra/work/WZ
OUTPUT_DIR=/nfs/fanae/user/piedra/svn/notes/AN-13-093/trunk


# Make directories
#-------------------------------------------------------------------------------
mkdir -p $OUTPUT_DIR/figures/scale_factors
#mkdir -p $OUTPUT_DIR/figures/analysis/MC/Exactly3Leptons/.
mkdir -p $OUTPUT_DIR/figures/analysis/MC/MET30/.
#mkdir -p $OUTPUT_DIR/figures/analysis/PPF/Exactly3Leptons/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/MET30/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/ClosureTest/.
mkdir -p $OUTPUT_DIR/tables/.


# Auxiliary figures
#-------------------------------------------------------------------------------
cp $INPUT_DIR/pdf/scale_factors/ElecSF.pdf          $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/MuonSF.pdf          $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/DoubleElLead.pdf    $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/DoubleElTrail.pdf   $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/DoubleMuLead.pdf    $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/DoubleMuTrail.pdf   $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/ElecFR_Jet35.pdf    $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/MuonFR_Jet15.pdf    $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/ElecPR.pdf          $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/MuonPR.pdf          $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/hLeptonWeight.pdf   $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/hTriggerWeight.pdf  $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/hTotalWeight.pdf    $OUTPUT_DIR/figures/scale_factors/.


# Closure test figures
#-------------------------------------------------------------------------------
cp $INPUT_DIR/pdf/analysis/PPF/ClosureTest/hInvMass2Lep_EEE_ClosureTest.pdf   $OUTPUT_DIR/figures/analysis/PPF/ClosureTest/.
cp $INPUT_DIR/pdf/analysis/PPF/ClosureTest/hInvMass2Lep_EEM_ClosureTest.pdf   $OUTPUT_DIR/figures/analysis/PPF/ClosureTest/.
cp $INPUT_DIR/pdf/analysis/PPF/ClosureTest/hInvMass2Lep_MME_ClosureTest.pdf   $OUTPUT_DIR/figures/analysis/PPF/ClosureTest/.
cp $INPUT_DIR/pdf/analysis/PPF/ClosureTest/hInvMass2Lep_MMM_ClosureTest.pdf   $OUTPUT_DIR/figures/analysis/PPF/ClosureTest/.
cp $INPUT_DIR/pdf/analysis/PPF/ClosureTest/hPtLeadingJet_EEE_ClosureTest.pdf  $OUTPUT_DIR/figures/analysis/PPF/ClosureTest/.
cp $INPUT_DIR/pdf/analysis/PPF/ClosureTest/hPtLeadingJet_EEM_ClosureTest.pdf  $OUTPUT_DIR/figures/analysis/PPF/ClosureTest/.
cp $INPUT_DIR/pdf/analysis/PPF/ClosureTest/hPtLeadingJet_MME_ClosureTest.pdf  $OUTPUT_DIR/figures/analysis/PPF/ClosureTest/.
cp $INPUT_DIR/pdf/analysis/PPF/ClosureTest/hPtLeadingJet_MMM_ClosureTest.pdf  $OUTPUT_DIR/figures/analysis/PPF/ClosureTest/.


# MC figures
#-------------------------------------------------------------------------------
#cp $INPUT_DIR/pdf/analysis/MC/Exactly3Leptons/hInvMass3Lep_EEE_Exactly3Leptons.pdf  $OUTPUT_DIR/figures/analysis/MC/Exactly3Leptons/.
#cp $INPUT_DIR/pdf/analysis/MC/Exactly3Leptons/hInvMass3Lep_EEM_Exactly3Leptons.pdf  $OUTPUT_DIR/figures/analysis/MC/Exactly3Leptons/.
#cp $INPUT_DIR/pdf/analysis/MC/Exactly3Leptons/hInvMass3Lep_MME_Exactly3Leptons.pdf  $OUTPUT_DIR/figures/analysis/MC/Exactly3Leptons/.
#cp $INPUT_DIR/pdf/analysis/MC/Exactly3Leptons/hInvMass3Lep_MMM_Exactly3Leptons.pdf  $OUTPUT_DIR/figures/analysis/MC/Exactly3Leptons/.
cp $INPUT_DIR/pdf/analysis/MC/MET30/hInvMass2Lep_EEE_MET30.pdf                      $OUTPUT_DIR/figures/analysis/MC/MET30/.
cp $INPUT_DIR/pdf/analysis/MC/MET30/hInvMass2Lep_EEM_MET30.pdf                      $OUTPUT_DIR/figures/analysis/MC/MET30/.
cp $INPUT_DIR/pdf/analysis/MC/MET30/hInvMass2Lep_MME_MET30.pdf                      $OUTPUT_DIR/figures/analysis/MC/MET30/.
cp $INPUT_DIR/pdf/analysis/MC/MET30/hInvMass2Lep_MMM_MET30.pdf                      $OUTPUT_DIR/figures/analysis/MC/MET30/.
cp $INPUT_DIR/pdf/analysis/MC/MET30/hNJet30_EEE_MET30.pdf                           $OUTPUT_DIR/figures/analysis/MC/MET30/.
cp $INPUT_DIR/pdf/analysis/MC/MET30/hNJet30_EEM_MET30.pdf                           $OUTPUT_DIR/figures/analysis/MC/MET30/.
cp $INPUT_DIR/pdf/analysis/MC/MET30/hNJet30_MME_MET30.pdf                           $OUTPUT_DIR/figures/analysis/MC/MET30/.
cp $INPUT_DIR/pdf/analysis/MC/MET30/hNJet30_MMM_MET30.pdf                           $OUTPUT_DIR/figures/analysis/MC/MET30/.
cp $INPUT_DIR/pdf/analysis/MC/MET30/hPtZ_EEE_MET30.pdf                              $OUTPUT_DIR/figures/analysis/MC/MET30/.
cp $INPUT_DIR/pdf/analysis/MC/MET30/hPtZ_EEM_MET30.pdf                              $OUTPUT_DIR/figures/analysis/MC/MET30/.
cp $INPUT_DIR/pdf/analysis/MC/MET30/hPtZ_MME_MET30.pdf                              $OUTPUT_DIR/figures/analysis/MC/MET30/.
cp $INPUT_DIR/pdf/analysis/MC/MET30/hPtZ_MMM_MET30.pdf                              $OUTPUT_DIR/figures/analysis/MC/MET30/.


# Data-driven figures
#-------------------------------------------------------------------------------
#cp $INPUT_DIR/pdf/analysis/PPF/Exactly3Leptons/hInvMass3Lep_EEE_Exactly3Leptons.pdf  $OUTPUT_DIR/figures/analysis/PPF/Exactly3Leptons/.
#cp $INPUT_DIR/pdf/analysis/PPF/Exactly3Leptons/hInvMass3Lep_EEM_Exactly3Leptons.pdf  $OUTPUT_DIR/figures/analysis/PPF/Exactly3Leptons/.
#cp $INPUT_DIR/pdf/analysis/PPF/Exactly3Leptons/hInvMass3Lep_MME_Exactly3Leptons.pdf  $OUTPUT_DIR/figures/analysis/PPF/Exactly3Leptons/.
#cp $INPUT_DIR/pdf/analysis/PPF/Exactly3Leptons/hInvMass3Lep_MMM_Exactly3Leptons.pdf  $OUTPUT_DIR/figures/analysis/PPF/Exactly3Leptons/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hInvMass2Lep_EEE_MET30.pdf                      $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hInvMass2Lep_EEM_MET30.pdf                      $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hInvMass2Lep_MME_MET30.pdf                      $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hInvMass2Lep_MMM_MET30.pdf                      $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hNJet30_EEE_MET30.pdf                           $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hNJet30_EEM_MET30.pdf                           $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hNJet30_MME_MET30.pdf                           $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hNJet30_MMM_MET30.pdf                           $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtZ_EEE_MET30.pdf                              $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtZ_EEM_MET30.pdf                              $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtZ_MME_MET30.pdf                              $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtZ_MMM_MET30.pdf                              $OUTPUT_DIR/figures/analysis/PPF/MET30/.


# Yields
#-------------------------------------------------------------------------------
cp $INPUT_DIR/tex/EEE_mc.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/EEM_mc.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/MME_mc.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/MMM_mc.tex  $OUTPUT_DIR/tables/.

cp $INPUT_DIR/tex/EEE_ppf.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/EEM_ppf.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/MME_ppf.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/MMM_ppf.tex  $OUTPUT_DIR/tables/.


# Final results
#-------------------------------------------------------------------------------
cp $INPUT_DIR/pdf/analysis/PPF/MET30/xs_MET30.pdf  $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/tex/xs_MET30_ppf.tex                 $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/systematics_MET30.tex            $OUTPUT_DIR/tables/.

