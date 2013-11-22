#!/bin/bash


OUTPUT_DIR=/nfs/fanae/user/piedra/svn/notes/AN-13-093/trunk


# Make directories
#-------------------------------------------------------------------------------
mkdir -p $OUTPUT_DIR/figures/scale_factors
mkdir -p $OUTPUT_DIR/tables/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/MET30/WPlus/.
mkdir -p $OUTPUT_DIR/figures/analysis/PPF/MET30/WMinus/.


# Auxiliary figures
#-------------------------------------------------------------------------------
cp pdf/scale_factors/ElecSF.pdf          $OUTPUT_DIR/figures/scale_factors/.
cp pdf/scale_factors/MuonSF.pdf          $OUTPUT_DIR/figures/scale_factors/.
cp pdf/scale_factors/DoubleElLead.pdf    $OUTPUT_DIR/figures/scale_factors/.
cp pdf/scale_factors/DoubleElTrail.pdf   $OUTPUT_DIR/figures/scale_factors/.
cp pdf/scale_factors/DoubleMuLead.pdf    $OUTPUT_DIR/figures/scale_factors/.
cp pdf/scale_factors/DoubleMuTrail.pdf   $OUTPUT_DIR/figures/scale_factors/.
cp pdf/scale_factors/ElecFR_Jet35.pdf    $OUTPUT_DIR/figures/scale_factors/.
cp pdf/scale_factors/MuonFR_Jet20.pdf    $OUTPUT_DIR/figures/scale_factors/.
cp pdf/scale_factors/ElecPR.pdf          $OUTPUT_DIR/figures/scale_factors/.
cp pdf/scale_factors/MuonPR.pdf          $OUTPUT_DIR/figures/scale_factors/.
cp pdf/scale_factors/hLeptonWeight.pdf   $OUTPUT_DIR/figures/scale_factors/.
cp pdf/scale_factors/hTriggerWeight.pdf  $OUTPUT_DIR/figures/scale_factors/.
cp pdf/scale_factors/hTotalWeight.pdf    $OUTPUT_DIR/figures/scale_factors/.


# ZJetsRegion figures
#-------------------------------------------------------------------------------
cp pdf/analysis/PPF/ZJetsRegion/WInclusive/hInvMass2Lep_EEE_ZJetsRegion_WInclusive.pdf        $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp pdf/analysis/PPF/ZJetsRegion/WInclusive/hInvMass2Lep_EEM_ZJetsRegion_WInclusive.pdf        $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp pdf/analysis/PPF/ZJetsRegion/WInclusive/hInvMass2Lep_MME_ZJetsRegion_WInclusive.pdf        $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp pdf/analysis/PPF/ZJetsRegion/WInclusive/hInvMass2Lep_MMM_ZJetsRegion_WInclusive.pdf        $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp pdf/analysis/PPF/ZJetsRegion/WInclusive/hInvMass2Lep_combined_ZJetsRegion_WInclusive.pdf   $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp pdf/analysis/PPF/ZJetsRegion/WInclusive/hPtLeadingJet_EEE_ZJetsRegion_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp pdf/analysis/PPF/ZJetsRegion/WInclusive/hPtLeadingJet_EEM_ZJetsRegion_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp pdf/analysis/PPF/ZJetsRegion/WInclusive/hPtLeadingJet_MME_ZJetsRegion_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp pdf/analysis/PPF/ZJetsRegion/WInclusive/hPtLeadingJet_MMM_ZJetsRegion_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.
cp pdf/analysis/PPF/ZJetsRegion/WInclusive/hPtLeadingJet_combined_ZJetsRegion_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/ZJetsRegion/WInclusive/.


# InvMass3Lep100 figures
#-------------------------------------------------------------------------------
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtZ_EEE_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtZ_EEM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtZ_MME_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtZ_MMM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtZ_combined_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtW_EEE_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtW_EEM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtW_MME_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtW_MMM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtW_combined_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtWLepton_EEE_InvMass3Lep100_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtWLepton_EEM_InvMass3Lep100_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtWLepton_MME_InvMass3Lep100_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtWLepton_MMM_InvMass3Lep100_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtWLepton_combined_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtLeadingJet_EEE_InvMass3Lep100_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtLeadingJet_EEM_InvMass3Lep100_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtLeadingJet_MME_InvMass3Lep100_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtLeadingJet_MMM_InvMass3Lep100_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hPtLeadingJet_combined_InvMass3Lep100_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMET_EEE_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMET_EEM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMET_MME_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMET_MMM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMET_combined_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hInvMass2Lep_EEE_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hInvMass2Lep_EEM_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hInvMass2Lep_MME_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hInvMass2Lep_MMM_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hInvMass2Lep_combined_InvMass3Lep100_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMtW_EEE_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMtW_EEM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMtW_MME_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMtW_MMM_InvMass3Lep100_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hMtW_combined_InvMass3Lep100_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hNJetAbove30_EEE_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hNJetAbove30_EEM_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hNJetAbove30_MME_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hNJetAbove30_MMM_InvMass3Lep100_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hNJetAbove30_combined_InvMass3Lep100_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hDPhiWLeptonMET_EEE_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hDPhiWLeptonMET_EEM_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hDPhiWLeptonMET_MME_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hDPhiWLeptonMET_MMM_InvMass3Lep100_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.
cp pdf/analysis/PPF/InvMass3Lep100/WInclusive/hDPhiWLeptonMET_combined_InvMass3Lep100_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/InvMass3Lep100/WInclusive/.


# HasZ figures
#-------------------------------------------------------------------------------
cp pdf/analysis/PPF/HasZ/WInclusive/hPtZ_EEE_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtZ_EEM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtZ_MME_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtZ_MMM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtZ_combined_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtW_EEE_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtW_EEM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtW_MME_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtW_MMM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtW_combined_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtWLepton_EEE_HasZ_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtWLepton_EEM_HasZ_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtWLepton_MME_HasZ_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtWLepton_MMM_HasZ_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtWLepton_combined_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtLeadingJet_EEE_HasZ_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtLeadingJet_EEM_HasZ_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtLeadingJet_MME_HasZ_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtLeadingJet_MMM_HasZ_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hPtLeadingJet_combined_HasZ_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hMET_EEE_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hMET_EEM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hMET_MME_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hMET_MMM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hMET_combined_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hInvMass2Lep_EEE_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hInvMass2Lep_EEM_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hInvMass2Lep_MME_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hInvMass2Lep_MMM_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hInvMass2Lep_combined_HasZ_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hMtW_EEE_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hMtW_EEM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hMtW_MME_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hMtW_MMM_HasZ_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hMtW_combined_HasZ_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hNJetAbove30_EEE_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hNJetAbove30_EEM_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hNJetAbove30_MME_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hNJetAbove30_MMM_HasZ_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hNJetAbove30_combined_HasZ_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hDPhiWLeptonMET_EEE_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hDPhiWLeptonMET_EEM_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hDPhiWLeptonMET_MME_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hDPhiWLeptonMET_MMM_HasZ_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.
cp pdf/analysis/PPF/HasZ/WInclusive/hDPhiWLeptonMET_combined_HasZ_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/HasZ/WInclusive/.


# MET30 figures
#-------------------------------------------------------------------------------
cp pdf/analysis/PPF/MET30/WInclusive/hPtZ_EEE_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtZ_EEM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtZ_MME_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtZ_MMM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtZ_combined_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtW_EEE_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtW_EEM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtW_MME_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtW_MMM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtW_combined_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtWLepton_EEE_MET30_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtWLepton_EEM_MET30_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtWLepton_MME_MET30_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtWLepton_MMM_MET30_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtWLepton_combined_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtLeadingJet_EEE_MET30_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtLeadingJet_EEM_MET30_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtLeadingJet_MME_MET30_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtLeadingJet_MMM_MET30_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hPtLeadingJet_combined_MET30_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hMET_EEE_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hMET_EEM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hMET_MME_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hMET_MMM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hMET_combined_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hInvMass2Lep_EEE_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hInvMass2Lep_EEM_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hInvMass2Lep_MME_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hInvMass2Lep_MMM_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hInvMass2Lep_combined_MET30_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hMtW_EEE_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hMtW_EEM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hMtW_MME_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hMtW_MMM_MET30_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hMtW_combined_MET30_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hNJetAbove30_EEE_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hNJetAbove30_EEM_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hNJetAbove30_MME_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hNJetAbove30_MMM_MET30_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hNJetAbove30_combined_MET30_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hDPhiWLeptonMET_EEE_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hDPhiWLeptonMET_EEM_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hDPhiWLeptonMET_MME_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hDPhiWLeptonMET_MMM_MET30_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/MET30/WInclusive/hDPhiWLeptonMET_combined_MET30_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.


# SantiagoCuts figures
#-------------------------------------------------------------------------------
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtZ_EEE_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtZ_EEM_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtZ_MME_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtZ_MMM_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtZ_combined_SantiagoCuts_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtW_EEE_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtW_EEM_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtW_MME_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtW_MMM_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtW_combined_SantiagoCuts_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtWLepton_EEE_SantiagoCuts_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtWLepton_EEM_SantiagoCuts_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtWLepton_MME_SantiagoCuts_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtWLepton_MMM_SantiagoCuts_WInclusive.pdf            $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtWLepton_combined_SantiagoCuts_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtLeadingJet_EEE_SantiagoCuts_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtLeadingJet_EEM_SantiagoCuts_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtLeadingJet_MME_SantiagoCuts_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtLeadingJet_MMM_SantiagoCuts_WInclusive.pdf         $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hPtLeadingJet_combined_SantiagoCuts_WInclusive.pdf    $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hMET_EEE_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hMET_EEM_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hMET_MME_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hMET_MMM_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hMET_combined_SantiagoCuts_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hInvMass2Lep_EEE_SantiagoCuts_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hInvMass2Lep_EEM_SantiagoCuts_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hInvMass2Lep_MME_SantiagoCuts_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hInvMass2Lep_MMM_SantiagoCuts_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hInvMass2Lep_combined_SantiagoCuts_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hMtW_EEE_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hMtW_EEM_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hMtW_MME_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hMtW_MMM_SantiagoCuts_WInclusive.pdf                  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hMtW_combined_SantiagoCuts_WInclusive.pdf             $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hNJetAbove30_EEE_SantiagoCuts_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hNJetAbove30_EEM_SantiagoCuts_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hNJetAbove30_MME_SantiagoCuts_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hNJetAbove30_MMM_SantiagoCuts_WInclusive.pdf          $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hNJetAbove30_combined_SantiagoCuts_WInclusive.pdf     $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hDPhiWLeptonMET_EEE_SantiagoCuts_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hDPhiWLeptonMET_EEM_SantiagoCuts_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hDPhiWLeptonMET_MME_SantiagoCuts_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hDPhiWLeptonMET_MMM_SantiagoCuts_WInclusive.pdf       $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/hDPhiWLeptonMET_combined_SantiagoCuts_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.


# Yields
#-------------------------------------------------------------------------------
cp tex/EEE_WInclusive_ppf.tex  $OUTPUT_DIR/tables/.
cp tex/EEM_WInclusive_ppf.tex  $OUTPUT_DIR/tables/.
cp tex/MME_WInclusive_ppf.tex  $OUTPUT_DIR/tables/.
cp tex/MMM_WInclusive_ppf.tex  $OUTPUT_DIR/tables/.

cp tex/EEE_WPlus_ppf.tex  $OUTPUT_DIR/tables/.
cp tex/EEM_WPlus_ppf.tex  $OUTPUT_DIR/tables/.
cp tex/MME_WPlus_ppf.tex  $OUTPUT_DIR/tables/.
cp tex/MMM_WPlus_ppf.tex  $OUTPUT_DIR/tables/.

cp tex/EEE_WMinus_ppf.tex  $OUTPUT_DIR/tables/.
cp tex/EEM_WMinus_ppf.tex  $OUTPUT_DIR/tables/.
cp tex/MME_WMinus_ppf.tex  $OUTPUT_DIR/tables/.
cp tex/MMM_WMinus_ppf.tex  $OUTPUT_DIR/tables/.

cp tex/EEE_WInclusive_mc.tex  $OUTPUT_DIR/tables/.
cp tex/EEM_WInclusive_mc.tex  $OUTPUT_DIR/tables/.
cp tex/MME_WInclusive_mc.tex  $OUTPUT_DIR/tables/.
cp tex/MMM_WInclusive_mc.tex  $OUTPUT_DIR/tables/.


# Final results
#-------------------------------------------------------------------------------
cp pdf/analysis/PPF/MET30/WInclusive/ratioNLO_MET30_WInclusive.pdf                $OUTPUT_DIR/figures/analysis/PPF/MET30/WInclusive/.
cp pdf/analysis/PPF/SantiagoCuts/WInclusive/ratioNLO_SantiagoCuts_WInclusive.pdf  $OUTPUT_DIR/figures/analysis/PPF/SantiagoCuts/WInclusive/.
cp pdf/analysis/PPF/MET30/WPlus/ratioNLO_MET30_WPlus.pdf                          $OUTPUT_DIR/figures/analysis/PPF/MET30/WPlus/.
cp pdf/analysis/PPF/MET30/WMinus/ratioNLO_MET30_WMinus.pdf                        $OUTPUT_DIR/figures/analysis/PPF/MET30/WMinus/.
cp tex/xs_MET30_WInclusive_ppf.tex                                                $OUTPUT_DIR/tables/.
cp tex/ratioWMinusWPlus_MET30_ppf.tex                                             $OUTPUT_DIR/tables/.
cp tex/ratioWPlusWMinus_MET30_ppf.tex                                             $OUTPUT_DIR/tables/.
cp tex/systematics_MET30_WInclusive_ppf.tex                                       $OUTPUT_DIR/tables/.

