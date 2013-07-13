#!/bin/bash


INPUT_DIR=/nfs/fanae/user/piedra/work/WZ
OUTPUT_DIR=/nfs/fanae/user/piedra/svn/notes/AN-13-093/trunk


# Make directories
#-------------------------------------------------------------------------------
mkdir -p $OUTPUT_DIR/figures/scale_factors
mkdir -p $OUTPUT_DIR/tables/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/MET30/WPlus/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/MET30/WMinus/.


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


# ZJetsRegion figures
#-------------------------------------------------------------------------------
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/WInclusive/hInvMass2Lep_EEE_ZJetsRegion_WInclusive.pdf        $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/WInclusive/hInvMass2Lep_EEM_ZJetsRegion_WInclusive.pdf        $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/WInclusive/hInvMass2Lep_MME_ZJetsRegion_WInclusive.pdf        $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/WInclusive/hInvMass2Lep_MMM_ZJetsRegion_WInclusive.pdf        $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/WInclusive/hInvMass2Lep_combined_ZJetsRegion_WInclusive.pdf   $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/WInclusive/hPtLeadingJet_EEE_ZJetsRegion_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/WInclusive/hPtLeadingJet_EEM_ZJetsRegion_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/WInclusive/hPtLeadingJet_MME_ZJetsRegion_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/WInclusive/hPtLeadingJet_MMM_ZJetsRegion_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/ZJetsRegion/WInclusive/hPtLeadingJet_combined_ZJetsRegion_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.


# InvMass3Lep100 figures
#-------------------------------------------------------------------------------
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtZ_EEE_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtZ_EEM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtZ_MME_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtZ_MMM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtZ_combined_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtW_EEE_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtW_EEM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtW_MME_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtW_MMM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtW_combined_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtWLepton_EEE_InvMass3Lep100_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtWLepton_EEM_InvMass3Lep100_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtWLepton_MME_InvMass3Lep100_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtWLepton_MMM_InvMass3Lep100_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtWLepton_combined_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtLeadingJet_EEE_InvMass3Lep100_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtLeadingJet_EEM_InvMass3Lep100_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtLeadingJet_MME_InvMass3Lep100_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtLeadingJet_MMM_InvMass3Lep100_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtLeadingJet_combined_InvMass3Lep100_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMET_EEE_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMET_EEM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMET_MME_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMET_MMM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMET_combined_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hInvMass2Lep_EEE_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hInvMass2Lep_EEM_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hInvMass2Lep_MME_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hInvMass2Lep_MMM_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hInvMass2Lep_combined_InvMass3Lep100_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMtW_EEE_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMtW_EEM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMtW_MME_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMtW_MMM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMtW_combined_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hNJet30_EEE_InvMass3Lep100_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hNJet30_EEM_InvMass3Lep100_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hNJet30_MME_InvMass3Lep100_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hNJet30_MMM_InvMass3Lep100_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hNJet30_combined_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hDPhiWLeptonMET_EEE_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hDPhiWLeptonMET_EEM_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hDPhiWLeptonMET_MME_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hDPhiWLeptonMET_MMM_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/InvMass3Lep100/WInclusive/hDPhiWLeptonMET_combined_InvMass3Lep100_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.


# HasZ figures
#-------------------------------------------------------------------------------
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtZ_EEE_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtZ_EEM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtZ_MME_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtZ_MMM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtZ_combined_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtW_EEE_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtW_EEM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtW_MME_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtW_MMM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtW_combined_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtWLepton_EEE_HasZ_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtWLepton_EEM_HasZ_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtWLepton_MME_HasZ_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtWLepton_MMM_HasZ_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtWLepton_combined_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtLeadingJet_EEE_HasZ_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtLeadingJet_EEM_HasZ_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtLeadingJet_MME_HasZ_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtLeadingJet_MMM_HasZ_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hPtLeadingJet_combined_HasZ_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hMET_EEE_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hMET_EEM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hMET_MME_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hMET_MMM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hMET_combined_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hInvMass2Lep_EEE_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hInvMass2Lep_EEM_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hInvMass2Lep_MME_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hInvMass2Lep_MMM_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hInvMass2Lep_combined_HasZ_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hMtW_EEE_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hMtW_EEM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hMtW_MME_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hMtW_MMM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hMtW_combined_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hNJet30_EEE_HasZ_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hNJet30_EEM_HasZ_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hNJet30_MME_HasZ_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hNJet30_MMM_HasZ_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hNJet30_combined_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hDPhiWLeptonMET_EEE_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hDPhiWLeptonMET_EEM_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hDPhiWLeptonMET_MME_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hDPhiWLeptonMET_MMM_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/HasZ/WInclusive/hDPhiWLeptonMET_combined_HasZ_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.


# MET30 figures
#-------------------------------------------------------------------------------
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtZ_EEE_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtZ_EEM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtZ_MME_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtZ_MMM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtZ_combined_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtW_EEE_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtW_EEM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtW_MME_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtW_MMM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtW_combined_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtWLepton_EEE_MET30_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtWLepton_EEM_MET30_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtWLepton_MME_MET30_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtWLepton_MMM_MET30_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtWLepton_combined_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtLeadingJet_EEE_MET30_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtLeadingJet_EEM_MET30_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtLeadingJet_MME_MET30_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtLeadingJet_MMM_MET30_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hPtLeadingJet_combined_MET30_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hMET_EEE_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hMET_EEM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hMET_MME_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hMET_MMM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hMET_combined_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hInvMass2Lep_EEE_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hInvMass2Lep_EEM_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hInvMass2Lep_MME_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hInvMass2Lep_MMM_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hInvMass2Lep_combined_MET30_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hMtW_EEE_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hMtW_EEM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hMtW_MME_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hMtW_MMM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hMtW_combined_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hNJet30_EEE_MET30_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hNJet30_EEM_MET30_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hNJet30_MME_MET30_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hNJet30_MMM_MET30_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hNJet30_combined_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hDPhiWLeptonMET_EEE_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hDPhiWLeptonMET_EEM_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hDPhiWLeptonMET_MME_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hDPhiWLeptonMET_MMM_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/hDPhiWLeptonMET_combined_MET30_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.


# TighterCuts figures
#-------------------------------------------------------------------------------
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtZ_EEE_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtZ_EEM_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtZ_MME_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtZ_MMM_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtZ_combined_TighterCuts_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtW_EEE_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtW_EEM_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtW_MME_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtW_MMM_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtW_combined_TighterCuts_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtWLepton_EEE_TighterCuts_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtWLepton_EEM_TighterCuts_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtWLepton_MME_TighterCuts_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtWLepton_MMM_TighterCuts_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtWLepton_combined_TighterCuts_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtLeadingJet_EEE_TighterCuts_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtLeadingJet_EEM_TighterCuts_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtLeadingJet_MME_TighterCuts_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtLeadingJet_MMM_TighterCuts_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hPtLeadingJet_combined_TighterCuts_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hMET_EEE_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hMET_EEM_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hMET_MME_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hMET_MMM_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hMET_combined_TighterCuts_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hInvMass2Lep_EEE_TighterCuts_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hInvMass2Lep_EEM_TighterCuts_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hInvMass2Lep_MME_TighterCuts_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hInvMass2Lep_MMM_TighterCuts_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hInvMass2Lep_combined_TighterCuts_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hMtW_EEE_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hMtW_EEM_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hMtW_MME_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hMtW_MMM_TighterCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hMtW_combined_TighterCuts_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hNJet30_EEE_TighterCuts_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hNJet30_EEM_TighterCuts_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hNJet30_MME_TighterCuts_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hNJet30_MMM_TighterCuts_WInclusive.pdf               $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hNJet30_combined_TighterCuts_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hDPhiWLeptonMET_EEE_TighterCuts_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hDPhiWLeptonMET_EEM_TighterCuts_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hDPhiWLeptonMET_MME_TighterCuts_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hDPhiWLeptonMET_MMM_TighterCuts_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/hDPhiWLeptonMET_combined_TighterCuts_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.


# Yields
#-------------------------------------------------------------------------------
cp $INPUT_DIR/tex/EEE_WInclusive_ppf.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/EEM_WInclusive_ppf.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/MME_WInclusive_ppf.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/MMM_WInclusive_ppf.tex  $OUTPUT_DIR/tables/.

cp $INPUT_DIR/tex/EEE_WInclusive_mc.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/EEM_WInclusive_mc.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/MME_WInclusive_mc.tex  $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/MMM_WInclusive_mc.tex  $OUTPUT_DIR/tables/.


# Final results
#-------------------------------------------------------------------------------
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WInclusive/ratioNLO_MET30_WInclusive.pdf              $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/TighterCuts/WInclusive/ratioNLO_TighterCuts_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/TighterCuts/WInclusive/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WPlus/ratioNLO_MET30_WPlus.pdf                        $OUTPUT_DIR/figures/analysis/PPF/MET30/WPlus/.
cp $INPUT_DIR/pdf/analysis/PPF/MET30/WMinus/ratioNLO_MET30_WMinus.pdf                      $OUTPUT_DIR/figures/analysis/PPF/MET30/WMinus/.
cp $INPUT_DIR/tex/xs_MET30_WInclusive_ppf.tex                                              $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/ratioWMinusWPlus_MET30_ppf.tex                                           $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/ratioWPlusWMinus_MET30_ppf.tex                                           $OUTPUT_DIR/tables/.
cp $INPUT_DIR/tex/systematics_MET30_WInclusive_ppf.tex                                     $OUTPUT_DIR/tables/.

