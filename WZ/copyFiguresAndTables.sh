#!/bin/bash


INPUT_DIR=/nfs/fanae/user/piedra/work/WZ
OUTPUT_DIR=/nfs/fanae/user/piedra/svn/notes/AN-13-093/trunk


# Make directories
#-------------------------------------------------------------------------------
mkdir -p $OUTPUT_DIR/figures/scale_factors
mkdir -p $OUTPUT_DIR/tables/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/MET30/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/.


# Auxiliary figures
#-------------------------------------------------------------------------------
cp $INPUT_DIR/pdf/scale_factors/ElecSF.pdf          $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/MuonSF.pdf          $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/DoubleElLead.pdf    $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/DoubleElTrail.pdf   $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/DoubleMuLead.pdf    $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/DoubleMuTrail.pdf   $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/ElecFR_Jet35.pdf    $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/MuonFR_Jet20.pdf    $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/ElecPR.pdf          $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/MuonPR.pdf          $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/hLeptonWeight.pdf   $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/hTriggerWeight.pdf  $OUTPUT_DIR/figures/scale_factors/.
cp $INPUT_DIR/pdf/scale_factors/hTotalWeight.pdf    $OUTPUT_DIR/figures/scale_factors/.


# Z+jets region figures
#-------------------------------------------------------------------------------
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/hInvMass2Lep_EEE_ZJetsRegion_WInclusive.pdf   $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/hInvMass2Lep_EEM_ZJetsRegion_WInclusive.pdf   $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/hInvMass2Lep_MME_ZJetsRegion_WInclusive.pdf   $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/hInvMass2Lep_MMM_ZJetsRegion_WInclusive.pdf   $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/hPtLeadingJet_EEE_ZJetsRegion_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/hPtLeadingJet_EEM_ZJetsRegion_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/hPtLeadingJet_MME_ZJetsRegion_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/hPtLeadingJet_MMM_ZJetsRegion_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/.


# Data-driven figures
#-------------------------------------------------------------------------------
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hPtZ_EEE_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hPtZ_EEM_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hPtZ_MME_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hPtZ_MMM_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hPtWLepton_EEE_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hPtWLepton_EEM_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hPtWLepton_MME_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hPtWLepton_MMM_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hPtLeadingJet_EEE_InvMass3Lep100_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hPtLeadingJet_EEM_InvMass3Lep100_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hPtLeadingJet_MME_InvMass3Lep100_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hPtLeadingJet_MMM_InvMass3Lep100_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hMET_EEE_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hMET_EEM_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hMET_MME_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hMET_MMM_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hInvMass2Lep_EEE_InvMass3Lep100_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hInvMass2Lep_EEM_InvMass3Lep100_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hInvMass2Lep_MME_InvMass3Lep100_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hInvMass2Lep_MMM_InvMass3Lep100_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hMtW_EEE_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hMtW_EEM_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hMtW_MME_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hMtW_MMM_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hNJet30_EEE_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hNJet30_EEM_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hNJet30_MME_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hNJet30_MMM_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hDPhiWLeptonMET_EEE_InvMass3Lep100_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hDPhiWLeptonMET_EEM_InvMass3Lep100_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hDPhiWLeptonMET_MME_InvMass3Lep100_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/hDPhiWLeptonMET_MMM_InvMass3Lep100_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/.

cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hPtZ_EEE_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hPtZ_EEM_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hPtZ_MME_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hPtZ_MMM_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hPtWLepton_EEE_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hPtWLepton_EEM_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hPtWLepton_MME_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hPtWLepton_MMM_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hPtLeadingJet_EEE_HasZ_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hPtLeadingJet_EEM_HasZ_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hPtLeadingJet_MME_HasZ_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hPtLeadingJet_MMM_HasZ_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hMET_EEE_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hMET_EEM_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hMET_MME_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hMET_MMM_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hInvMass2Lep_EEE_HasZ_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hInvMass2Lep_EEM_HasZ_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hInvMass2Lep_MME_HasZ_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hInvMass2Lep_MMM_HasZ_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hMtW_EEE_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hMtW_EEM_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hMtW_MME_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hMtW_MMM_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hNJet30_EEE_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hNJet30_EEM_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hNJet30_MME_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hNJet30_MMM_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hDPhiWLeptonMET_EEE_HasZ_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hDPhiWLeptonMET_EEM_HasZ_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hDPhiWLeptonMET_MME_HasZ_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/HasZ/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/hDPhiWLeptonMET_MMM_HasZ_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/HasZ/.

cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtZ_EEE_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtZ_EEM_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtZ_MME_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtZ_MMM_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtWLepton_EEE_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtWLepton_EEM_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtWLepton_MME_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtWLepton_MMM_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtLeadingJet_EEE_MET30_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtLeadingJet_EEM_MET30_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtLeadingJet_MME_MET30_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hPtLeadingJet_MMM_MET30_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hMET_EEE_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hMET_EEM_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hMET_MME_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hMET_MMM_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hInvMass2Lep_EEE_MET30_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hInvMass2Lep_EEM_MET30_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hInvMass2Lep_MME_MET30_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hInvMass2Lep_MMM_MET30_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hMtW_EEE_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hMtW_EEM_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hMtW_MME_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hMtW_MMM_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hNJet30_EEE_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hNJet30_EEM_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hNJet30_MME_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hNJet30_MMM_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hDPhiWLeptonMET_EEE_MET30_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hDPhiWLeptonMET_EEM_MET30_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hDPhiWLeptonMET_MME_MET30_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/hDPhiWLeptonMET_MMM_MET30_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/MET30/.


# Yields
#-------------------------------------------------------------------------------
cp $INPUT_DIR/tex/EEE_WInclusive_ppf.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/EEM_WInclusive_ppf.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/MME_WInclusive_ppf.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/MMM_WInclusive_ppf.tex  $OUTPUT_DIR/tables/.


# Final results
#-------------------------------------------------------------------------------
cp $INPUT_DIR/pdf/analysis/PPF/MET30/xs_MET30_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/xs_MET30_WPlus.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/xs_MET30_WMinus.pdf      $OUTPUT_DIR/figures/analysis/PPF/MET30/.
cp $INPUT_DIR/tex/xs_MET30_WInclusive_ppf.tex                 $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/ratioWMinusWPlus_MET30_ppf.tex              $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/ratioWPlusWMinus_MET30_ppf.tex              $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/systematics_MET30_WInclusive.tex            $OUTPUT_DIR/tables/.

