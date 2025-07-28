#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "analysisHelperFunctions.h"

#define WRITE_LUT false // for disabling firmware (ap_uint), LUT declarations

double deltaPhi(double phi1, double phi2) {
    double dphi = phi1 - phi2;
    if (dphi > TMath::Pi()) dphi -= 2 * TMath::Pi();
    if (dphi <= -TMath::Pi()) dphi += 2 * TMath::Pi();
    return dphi;
}


// Function to compute the mean of a vector
float computeMean(const std::vector<float>& vec) {
    if (vec.empty()) return 0.0f;
    float sum = 0.0f;
    for (float value : vec) {
        sum += value;
    }
    return sum / vec.size();
}

// Function to compute the standard deviation of a vector
float computeStandardDeviation(const std::vector<float>& vec, float mean) {
    if (vec.size() < 2) return 0.0f;
    float sum = 0.0f;
    for (float value : vec) {
        float diff = value - mean;
        sum += diff * diff;
    }
    return std::sqrt(sum / (vec.size() - 1));  // Using (n-1) for sample standard deviation
}

// Function to compute the standard error of the mean
float computeStandardError(const std::vector<float>& vec, float stdDev) {
    if (vec.size() == 0) return 0.0f;
    return stdDev / std::sqrt(vec.size());
}



analyzeHistograms(std::string& signalInputFileNameVBF, std::string& signalInputFileNameggF, const std::string& backgroundInputFileName){
    
    
    SetPlotStyle();

    // Open input ROOT file
    TFile* signalInputFileVBF = TFile::Open(signalInputFileNameVBF.c_str(), "READ");
    TFile* signalInputFileggF = TFile::Open(signalInputFileNameggF.c_str(), "READ");
    TFile* backgroundInputFile = TFile::Open(backgroundInputFileName.c_str(), "READ");
    if ((!signalInputFileVBF || signalInputFileVBF->IsZombie()) || (!signalInputFileggF || signalInputFileggF->IsZombie()) || (!backgroundInputFile || backgroundInputFile->IsZombie())) {
        std::cerr << "Error: Could not open file " << signalInputFileNameVBF << std::endl;
        return;
    }


    TTree* truthbTree_VBF = (TTree*)signalInputFileVBF->Get("truthbTree");
    TTree* truthHiggsTree_VBF = (TTree*)signalInputFileVBF->Get("truthHiggsTree");
    TTree* caloTopoTowerTree_VBF = (TTree*)signalInputFileVBF->Get("caloTopoTowerTree");
    TTree* topo422Tree_VBF = (TTree*)signalInputFileVBF->Get("topo422Tree");
    TTree* gFexSRJTree_VBF = (TTree*)signalInputFileVBF->Get("gFexSRJTree");
    TTree* gFexLeadingSRJTree_VBF = (TTree*)signalInputFileVBF->Get("gFexLeadingSRJTree");
    TTree* gFexSubleadingSRJTree_VBF = (TTree*)signalInputFileVBF->Get("gFexSubleadingSRJTree");
    TTree* gFexLRJTree_VBF = (TTree*)signalInputFileVBF->Get("gFexLRJTree");
    TTree* gFexLeadingLRJTree_VBF = (TTree*)signalInputFileVBF->Get("gFexLeadingLRJTree");
    TTree* gFexSubleadingLRJTree_VBF = (TTree*)signalInputFileVBF->Get("gFexSubleadingLRJTree");
    TTree* inTimeAntiKt4TruthJetsTree_VBF = (TTree*)signalInputFileVBF->Get("inTimeAntiKt4TruthJetsTree");
    TTree* leadingInTimeAntiKt4TruthJetsTree_VBF = (TTree*)signalInputFileVBF->Get("leadingInTimeAntiKt4TruthJetsTree");
    TTree* subleadingInTimeAntiKt4TruthJetsTree_VBF = (TTree*)signalInputFileVBF->Get("subleadingInTimeAntiKt4TruthJetsTree");
    TTree* jFexSRJTree_VBF = (TTree*)signalInputFileVBF->Get("jFexSRJTree");
    TTree* jFexLeadingSRJTree_VBF = (TTree*)signalInputFileVBF->Get("jFexLeadingSRJTree");
    TTree* jFexSubleadingSRJTree_VBF = (TTree*)signalInputFileVBF->Get("jFexSubleadingSRJTree");
    TTree* jFexLRJTree_VBF = (TTree*)signalInputFileVBF->Get("jFexLRJTree");
    TTree* jFexLeadingLRJTree_VBF = (TTree*)signalInputFileVBF->Get("jFexLeadingLRJTree");
    TTree* jFexSubleadingLRJTree_VBF = (TTree*)signalInputFileVBF->Get("jFexSubleadingLRJTree");
    TTree* hltAntiKt4EMTopoJetsTree_VBF = (TTree*)signalInputFileVBF->Get("hltAntiKt4EMTopoJetsTree");
    TTree* leadingHltAntiKt4EMTopoJetsTree_VBF = (TTree*)signalInputFileVBF->Get("leadingHltAntiKt4EMTopoJetsTree");
    TTree* subleadingHltAntiKt4EMTopoJetsTree_VBF = (TTree*)signalInputFileVBF->Get("subleadingHltAntiKt4EMTopoJetsTree");
    TTree* recoAntiKt10UFOCSSKJets_VBF = (TTree*)signalInputFileVBF->Get("recoAntiKt10UFOCSSKJets");
    TTree* leadingRecoAntiKt10UFOCSSKJets_VBF = (TTree*)signalInputFileVBF->Get("leadingRecoAntiKt10UFOCSSKJets");
    TTree* subleadingRecoAntiKt10UFOCSSKJets_VBF = (TTree*)signalInputFileVBF->Get("subleadingRecoAntiKt10UFOCSSKJets");
    TTree* truthAntiKt4TruthDressedWZJets_VBF = (TTree*)signalInputFileVBF->Get("truthAntiKt4TruthDressedWZJets");
    TTree* leadingTruthAntiKt4TruthDressedWZJets_VBF = (TTree*)signalInputFileVBF->Get("leadingTruthAntiKt4TruthDressedWZJets");
    TTree* subleadingTruthAntiKt4TruthDressedWZJets_VBF = (TTree*)signalInputFileVBF->Get("subleadingTruthAntiKt4TruthDressedWZJets");

    TTree* truthbTree_ggF = (TTree*)signalInputFileggF->Get("truthbTree");
    TTree* truthHiggsTree_ggF = (TTree*)signalInputFileggF->Get("truthHiggsTree");
    TTree* caloTopoTowerTree_ggF = (TTree*)signalInputFileggF->Get("caloTopoTowerTree");
    TTree* topo422Tree_ggF = (TTree*)signalInputFileggF->Get("topo422Tree");
    TTree* gFexSRJTree_ggF = (TTree*)signalInputFileggF->Get("gFexSRJTree");
    TTree* gFexLeadingSRJTree_ggF = (TTree*)signalInputFileggF->Get("gFexLeadingSRJTree");
    TTree* gFexSubleadingSRJTree_ggF = (TTree*)signalInputFileggF->Get("gFexSubleadingSRJTree");
    TTree* gFexLRJTree_ggF = (TTree*)signalInputFileggF->Get("gFexLRJTree");
    TTree* gFexLeadingLRJTree_ggF = (TTree*)signalInputFileggF->Get("gFexLeadingLRJTree");
    TTree* gFexSubleadingLRJTree_ggF = (TTree*)signalInputFileggF->Get("gFexSubleadingLRJTree");
    TTree* inTimeAntiKt4TruthJetsTree_ggF = (TTree*)signalInputFileggF->Get("inTimeAntiKt4TruthJetsTree");
    TTree* leadingInTimeAntiKt4TruthJetsTree_ggF = (TTree*)signalInputFileggF->Get("leadingInTimeAntiKt4TruthJetsTree");
    TTree* subleadingInTimeAntiKt4TruthJetsTree_ggF = (TTree*)signalInputFileggF->Get("subleadingInTimeAntiKt4TruthJetsTree");
    TTree* jFexSRJTree_ggF = (TTree*)signalInputFileggF->Get("jFexSRJTree");
    TTree* jFexLeadingSRJTree_ggF = (TTree*)signalInputFileggF->Get("jFexLeadingSRJTree");
    TTree* jFexSubleadingSRJTree_ggF = (TTree*)signalInputFileggF->Get("jFexSubleadingSRJTree");
    TTree* jFexLRJTree_ggF = (TTree*)signalInputFileggF->Get("jFexLRJTree");
    TTree* jFexLeadingLRJTree_ggF = (TTree*)signalInputFileggF->Get("jFexLeadingLRJTree");
    TTree* jFexSubleadingLRJTree_ggF = (TTree*)signalInputFileggF->Get("jFexSubleadingLRJTree");
    TTree* hltAntiKt4EMTopoJetsTree_ggF = (TTree*)signalInputFileggF->Get("hltAntiKt4EMTopoJetsTree");
    TTree* leadingHltAntiKt4EMTopoJetsTree_ggF = (TTree*)signalInputFileggF->Get("leadingHltAntiKt4EMTopoJetsTree");
    TTree* subleadingHltAntiKt4EMTopoJetsTree_ggF = (TTree*)signalInputFileggF->Get("subleadingHltAntiKt4EMTopoJetsTree");
    TTree* recoAntiKt10UFOCSSKJets_ggF = (TTree*)signalInputFileggF->Get("recoAntiKt10UFOCSSKJets");
    TTree* leadingRecoAntiKt10UFOCSSKJets_ggF = (TTree*)signalInputFileggF->Get("leadingRecoAntiKt10UFOCSSKJets");
    TTree* subleadingRecoAntiKt10UFOCSSKJets_ggF = (TTree*)signalInputFileggF->Get("subleadingRecoAntiKt10UFOCSSKJets");
    TTree* truthAntiKt4TruthDressedWZJets_ggF = (TTree*)signalInputFileggF->Get("truthAntiKt4TruthDressedWZJets");
    TTree* leadingTruthAntiKt4TruthDressedWZJets_ggF = (TTree*)signalInputFileggF->Get("leadingTruthAntiKt4TruthDressedWZJets");
    TTree* subleadingTruthAntiKt4TruthDressedWZJets_ggF = (TTree*)signalInputFileggF->Get("subleadingTruthAntiKt4TruthDressedWZJets");

    TTree* caloTopoTowerTree_back = (TTree*)backgroundInputFile->Get("caloTopoTowerTree");
    TTree* topo422Tree_back = (TTree*)backgroundInputFile->Get("topo422Tree");
    TTree* gFexSRJTree_back = (TTree*)backgroundInputFile->Get("gFexSRJTree");
    TTree* gFexLeadingSRJTree_back = (TTree*)backgroundInputFile->Get("gFexLeadingSRJTree");
    TTree* gFexSubleadingSRJTree_back = (TTree*)backgroundInputFile->Get("gFexSubleadingSRJTree");
    TTree* gFexLRJTree_back = (TTree*)backgroundInputFile->Get("gFexLRJTree");
    TTree* gFexLeadingLRJTree_back = (TTree*)backgroundInputFile->Get("gFexLeadingLRJTree");
    TTree* gFexSubleadingLRJTree_back = (TTree*)backgroundInputFile->Get("gFexSubleadingLRJTree");
    TTree* inTimeAntiKt4TruthJetsTree_back = (TTree*)backgroundInputFile->Get("inTimeAntiKt4TruthJetsTree");
    TTree* leadingInTimeAntiKt4TruthJetsTree_back = (TTree*)backgroundInputFile->Get("leadingInTimeAntiKt4TruthJetsTree");
    TTree* subleadingInTimeAntiKt4TruthJetsTree_back = (TTree*)backgroundInputFile->Get("subleadingInTimeAntiKt4TruthJetsTree");
    TTree* jFexSRJTree_back = (TTree*)backgroundInputFile->Get("jFexSRJTree");
    TTree* jFexLeadingSRJTree_back = (TTree*)backgroundInputFile->Get("jFexLeadingSRJTree");
    TTree* jFexSubleadingSRJTree_back = (TTree*)backgroundInputFile->Get("jFexSubleadingSRJTree");
    TTree* jFexLRJTree_back = (TTree*)backgroundInputFile->Get("jFexLRJTree");
    TTree* jFexLeadingLRJTree_back = (TTree*)backgroundInputFile->Get("jFexLeadingLRJTree");
    TTree* jFexSubleadingLRJTree_back = (TTree*)backgroundInputFile->Get("jFexSubleadingLRJTree");
    TTree* hltAntiKt4EMTopoJetsTree_back = (TTree*)backgroundInputFile->Get("hltAntiKt4EMTopoJetsTree");
    TTree* leadingHltAntiKt4EMTopoJetsTree_back = (TTree*)backgroundInputFile->Get("leadingHltAntiKt4EMTopoJetsTree");
    TTree* subleadingHltAntiKt4EMTopoJetsTree_back = (TTree*)backgroundInputFile->Get("subleadingHltAntiKt4EMTopoJetsTree");
    TTree* recoAntiKt10UFOCSSKJets_back = (TTree*)backgroundInputFile->Get("recoAntiKt10UFOCSSKJets");
    TTree* leadingRecoAntiKt10UFOCSSKJets_back = (TTree*)backgroundInputFile->Get("leadingRecoAntiKt10UFOCSSKJets");
    TTree* subleadingRecoAntiKt10UFOCSSKJets_back = (TTree*)backgroundInputFile->Get("subleadingRecoAntiKt10UFOCSSKJets");
    TTree* truthAntiKt4TruthDressedWZJets_back = (TTree*)backgroundInputFile->Get("truthAntiKt4TruthDressedWZJets");
    TTree* leadingTruthAntiKt4TruthDressedWZJets_back = (TTree*)backgroundInputFile->Get("leadingTruthAntiKt4TruthDressedWZJets");
    TTree* subleadingTruthAntiKt4TruthDressedWZJets_back = (TTree*)backgroundInputFile->Get("subleadingTruthAntiKt4TruthDressedWZJets");

    std::vector<unsigned int> higgsIndexValues_VBF, indexOfHiggsValues_VBF;
    std::vector<double> truthbquarksEtValues_VBF, truthbquarksEnergyValues_VBF, truthbquarkspTValues_VBF, truthbquarkspxValues_VBF, truthbquarkspyValues_VBF, truthbquarkspzValues_VBF, truthbquarksEtaValues_VBF, truthbquarksPhiValues_VBF;
    std::vector<double> truthHiggsEtValues_VBF, truthHiggsEnergyValues_VBF, truthHiggspTValues_VBF, truthHiggspxValues_VBF, truthHiggspyValues_VBF, truthHiggspzValues_VBF, truthHiggsEtaValues_VBF, truthHiggsPhiValues_VBF, truthHiggsInvMassValues_VBF;
    std::vector<double> caloTopoTowerEtValues_VBF, caloTopoTowerEtaValues_VBF, caloTopoTowerPhiValues_VBF;
    std::vector<double> topo422EtValues_VBF, topo422EtaValues_VBF, topo422PhiValues_VBF;
    std::vector<unsigned int> gFexSRJEtIndexValues_VBF;
    std::vector<double> gFexSRJEtValues_VBF, gFexSRJEtaValues_VBF, gFexSRJPhiValues_VBF;
    std::vector<double> gFexSRJLeadingEtValues_VBF, gFexSRJLeadingEtaValues_VBF, gFexSRJLeadingPhiValues_VBF;
    std::vector<double> gFexSRJSubleadingEtValues_VBF, gFexSRJSubleadingEtaValues_VBF, gFexSRJSubleadingPhiValues_VBF;
    std::vector<unsigned int> gFexLRJEtIndexValues_VBF;
    std::vector<double> gFexLRJEtValues_VBF, gFexLRJEtaValues_VBF, gFexLRJPhiValues_VBF;
    std::vector<double> gFexLRJLeadingEtValues_VBF, gFexLRJLeadingEtaValues_VBF, gFexLRJLeadingPhiValues_VBF;
    std::vector<double> gFexLRJSubleadingEtValues_VBF, gFexLRJSubleadingEtaValues_VBF, gFexLRJSubleadingPhiValues_VBF;
    std::vector<unsigned int> jFexSRJEtIndexValues_VBF;
    std::vector<double> jFexSRJEtValues_VBF, jFexSRJEtaValues_VBF, jFexSRJPhiValues_VBF;
    std::vector<double> jFexSRJLeadingEtValues_VBF, jFexSRJLeadingEtaValues_VBF, jFexSRJLeadingPhiValues_VBF;
    std::vector<double> jFexSRJSubleadingEtValues_VBF, jFexSRJSubleadingEtaValues_VBF, jFexSRJSubleadingPhiValues_VBF;
    std::vector<unsigned int> jFexLRJEtIndexValues_VBF;
    std::vector<double> jFexLRJEtValues_VBF, jFexLRJEtaValues_VBF, jFexLRJPhiValues_VBF;
    std::vector<double> jFexLRJLeadingEtValues_VBF, jFexLRJLeadingEtaValues_VBF, jFexLRJLeadingPhiValues_VBF;
    std::vector<double> jFexLRJSubleadingEtValues_VBF, jFexLRJSubleadingEtaValues_VBF, jFexLRJSubleadingPhiValues_VBF;
    std::vector<unsigned int> hltAntiKt4SRJEtIndexValues_VBF;
    std::vector<double> hltAntiKt4SRJEtValues_VBF, hltAntiKt4SRJEtaValues_VBF, hltAntiKt4SRJPhiValues_VBF;
    std::vector<double> hltAntiKt4SRJLeadingEtValues_VBF, hltAntiKt4SRJLeadingEtaValues_VBF, hltAntiKt4SRJLeadingPhiValues_VBF;
    std::vector<double> hltAntiKt4SRJSubleadingEtValues_VBF, hltAntiKt4SRJSubleadingEtaValues_VBF, hltAntiKt4SRJSubleadingPhiValues_VBF;
    std::vector<unsigned int> recoAntiKt10LRJEtIndexValues_VBF;
    std::vector<double> recoAntiKt10LRJEtValues_VBF, recoAntiKt10LRJEtaValues_VBF, recoAntiKt10LRJPhiValues_VBF;
    std::vector<double> recoAntiKt10LRJLeadingEtValues_VBF, recoAntiKt10LRJLeadingEtaValues_VBF, recoAntiKt10LRJLeadingPhiValues_VBF;
    std::vector<double> recoAntiKt10LRJSubleadingEtValues_VBF, recoAntiKt10LRJSubleadingEtaValues_VBF, recoAntiKt10LRJSubleadingPhiValues_VBF;
    std::vector<unsigned int> truthAntiKt4WZSRJEtIndexValues_VBF;
    std::vector<double> truthAntiKt4WZSRJEtValues_VBF, truthAntiKt4WZSRJEtaValues_VBF, truthAntiKt4WZSRJPhiValues_VBF;
    std::vector<double> truthAntiKt4WZSRJLeadingEtValues_VBF, truthAntiKt4WZSRJLeadingEtaValues_VBF, truthAntiKt4WZSRJLeadingPhiValues_VBF;
    std::vector<double> truthAntiKt4WZSRJSubleadingEtValues_VBF, truthAntiKt4WZSRJSubleadingEtaValues_VBF, truthAntiKt4WZSRJSubleadingPhiValues_VBF;
    std::vector<unsigned int> inTimeAntiKt4TruthSRJEtIndexValues_VBF;
    std::vector<double> inTimeAntiKt4TruthSRJEtValues_VBF, inTimeAntiKt4TruthSRJEtaValues_VBF, inTimeAntiKt4TruthSRJPhiValues_VBF;
    std::vector<double> inTimeAntiKt4TruthSRJLeadingEtValues_VBF, inTimeAntiKt4TruthSRJLeadingEtaValues_VBF, inTimeAntiKt4TruthSRJLeadingPhiValues_VBF;
    std::vector<double> inTimeAntiKt4TruthSRJSubleadingEtValues_VBF, inTimeAntiKt4TruthSRJSubleadingEtaValues_VBF, inTimeAntiKt4TruthSRJSubleadingPhiValues_VBF;

    std::vector<unsigned int> higgsIndexValues_ggF, indexOfHiggsValues_ggF;
    std::vector<double> truthbquarksEtValues_ggF, truthbquarksEnergyValues_ggF, truthbquarkspTValues_ggF, truthbquarkspxValues_ggF, truthbquarkspyValues_ggF, truthbquarkspzValues_ggF, truthbquarksEtaValues_ggF, truthbquarksPhiValues_ggF;
    std::vector<double> truthHiggsEtValues_ggF, truthHiggsEnergyValues_ggF, truthHiggspTValues_ggF, truthHiggspxValues_ggF, truthHiggspyValues_ggF, truthHiggspzValues_ggF, truthHiggsEtaValues_ggF, truthHiggsPhiValues_ggF, truthHiggsInvMassValues_ggF;
    std::vector<double> caloTopoTowerEtValues_ggF, caloTopoTowerEtaValues_ggF, caloTopoTowerPhiValues_ggF;
    std::vector<double> topo422EtValues_ggF, topo422EtaValues_ggF, topo422PhiValues_ggF;
    std::vector<unsigned int> gFexSRJEtIndexValues_ggF;
    std::vector<double> gFexSRJEtValues_ggF, gFexSRJEtaValues_ggF, gFexSRJPhiValues_ggF;
    std::vector<double> gFexSRJLeadingEtValues_ggF, gFexSRJLeadingEtaValues_ggF, gFexSRJLeadingPhiValues_ggF;
    std::vector<double> gFexSRJSubleadingEtValues_ggF, gFexSRJSubleadingEtaValues_ggF, gFexSRJSubleadingPhiValues_ggF;
    std::vector<unsigned int> gFexLRJEtIndexValues_ggF;
    std::vector<double> gFexLRJEtValues_ggF, gFexLRJEtaValues_ggF, gFexLRJPhiValues_ggF;
    std::vector<double> gFexLRJLeadingEtValues_ggF, gFexLRJLeadingEtaValues_ggF, gFexLRJLeadingPhiValues_ggF;
    std::vector<double> gFexLRJSubleadingEtValues_ggF, gFexLRJSubleadingEtaValues_ggF, gFexLRJSubleadingPhiValues_ggF;
    std::vector<unsigned int> jFexSRJEtIndexValues_ggF;
    std::vector<double> jFexSRJEtValues_ggF, jFexSRJEtaValues_ggF, jFexSRJPhiValues_ggF;
    std::vector<double> jFexSRJLeadingEtValues_ggF, jFexSRJLeadingEtaValues_ggF, jFexSRJLeadingPhiValues_ggF;
    std::vector<double> jFexSRJSubleadingEtValues_ggF, jFexSRJSubleadingEtaValues_ggF, jFexSRJSubleadingPhiValues_ggF;
    std::vector<unsigned int> jFexLRJEtIndexValues_ggF;
    std::vector<double> jFexLRJEtValues_ggF, jFexLRJEtaValues_ggF, jFexLRJPhiValues_ggF;
    std::vector<double> jFexLRJLeadingEtValues_ggF, jFexLRJLeadingEtaValues_ggF, jFexLRJLeadingPhiValues_ggF;
    std::vector<double> jFexLRJSubleadingEtValues_ggF, jFexLRJSubleadingEtaValues_ggF, jFexLRJSubleadingPhiValues_ggF;
    std::vector<unsigned int> hltAntiKt4SRJEtIndexValues_ggF;
    std::vector<double> hltAntiKt4SRJEtValues_ggF, hltAntiKt4SRJEtaValues_ggF, hltAntiKt4SRJPhiValues_ggF;
    std::vector<double> hltAntiKt4SRJLeadingEtValues_ggF, hltAntiKt4SRJLeadingEtaValues_ggF, hltAntiKt4SRJLeadingPhiValues_ggF;
    std::vector<double> hltAntiKt4SRJSubleadingEtValues_ggF, hltAntiKt4SRJSubleadingEtaValues_ggF, hltAntiKt4SRJSubleadingPhiValues_ggF;
    std::vector<unsigned int> recoAntiKt10LRJEtIndexValues_ggF;
    std::vector<double> recoAntiKt10LRJEtValues_ggF, recoAntiKt10LRJEtaValues_ggF, recoAntiKt10LRJPhiValues_ggF;
    std::vector<double> recoAntiKt10LRJLeadingEtValues_ggF, recoAntiKt10LRJLeadingEtaValues_ggF, recoAntiKt10LRJLeadingPhiValues_ggF;
    std::vector<double> recoAntiKt10LRJSubleadingEtValues_ggF, recoAntiKt10LRJSubleadingEtaValues_ggF, recoAntiKt10LRJSubleadingPhiValues_ggF;
    std::vector<unsigned int> truthAntiKt4WZSRJEtIndexValues_ggF;
    std::vector<double> truthAntiKt4WZSRJEtValues_ggF, truthAntiKt4WZSRJEtaValues_ggF, truthAntiKt4WZSRJPhiValues_ggF;
    std::vector<double> truthAntiKt4WZSRJLeadingEtValues_ggF, truthAntiKt4WZSRJLeadingEtaValues_ggF, truthAntiKt4WZSRJLeadingPhiValues_ggF;
    std::vector<double> truthAntiKt4WZSRJSubleadingEtValues_ggF, truthAntiKt4WZSRJSubleadingEtaValues_ggF, truthAntiKt4WZSRJSubleadingPhiValues_ggF;
    std::vector<unsigned int> inTimeAntiKt4TruthSRJEtIndexValues_ggF;
    std::vector<double> inTimeAntiKt4TruthSRJEtValues_ggF, inTimeAntiKt4TruthSRJEtaValues_ggF, inTimeAntiKt4TruthSRJPhiValues_ggF;
    std::vector<double> inTimeAntiKt4TruthSRJLeadingEtValues_ggF, inTimeAntiKt4TruthSRJLeadingEtaValues_ggF, inTimeAntiKt4TruthSRJLeadingPhiValues_ggF;
    std::vector<double> inTimeAntiKt4TruthSRJSubleadingEtValues_ggF, inTimeAntiKt4TruthSRJSubleadingEtaValues_ggF, inTimeAntiKt4TruthSRJSubleadingPhiValues_ggF;

    std::vector<double> caloTopoTowerEtValues_back, caloTopoTowerEtaValues_back, caloTopoTowerPhiValues_back;
    std::vector<double> topo422EtValues_back, topo422EtaValues_back, topo422PhiValues_back;
    std::vector<unsigned int> gFexSRJEtIndexValues_back;
    std::vector<double> gFexSRJEtValues_back, gFexSRJEtaValues_back, gFexSRJPhiValues_back;
    std::vector<double> gFexSRJLeadingEtValues_back, gFexSRJLeadingEtaValues_back, gFexSRJLeadingPhiValues_back;
    std::vector<double> gFexSRJSubleadingEtValues_back, gFexSRJSubleadingEtaValues_back, gFexSRJSubleadingPhiValues_back;
    std::vector<unsigned int> gFexLRJEtIndexValues_back;
    std::vector<double> gFexLRJEtValues_back, gFexLRJEtaValues_back, gFexLRJPhiValues_back;
    std::vector<double> gFexLRJLeadingEtValues_back, gFexLRJLeadingEtaValues_back, gFexLRJLeadingPhiValues_back;
    std::vector<double> gFexLRJSubleadingEtValues_back, gFexLRJSubleadingEtaValues_back, gFexLRJSubleadingPhiValues_back;
    std::vector<unsigned int> jFexSRJEtIndexValues_back;
    std::vector<double> jFexSRJEtValues_back, jFexSRJEtaValues_back, jFexSRJPhiValues_back;
    std::vector<double> jFexSRJLeadingEtValues_back, jFexSRJLeadingEtaValues_back, jFexSRJLeadingPhiValues_back;
    std::vector<double> jFexSRJSubleadingEtValues_back, jFexSRJSubleadingEtaValues_back, jFexSRJSubleadingPhiValues_back;
    std::vector<unsigned int> jFexLRJEtIndexValues_back;
    std::vector<double> jFexLRJEtValues_back, jFexLRJEtaValues_back, jFexLRJPhiValues_back;
    std::vector<double> jFexLRJLeadingEtValues_back, jFexLRJLeadingEtaValues_back, jFexLRJLeadingPhiValues_back;
    std::vector<double> jFexLRJSubleadingEtValues_back, jFexLRJSubleadingEtaValues_back, jFexLRJSubleadingPhiValues_back;
    std::vector<unsigned int> hltAntiKt4SRJEtIndexValues_back;
    std::vector<double> hltAntiKt4SRJEtValues_back, hltAntiKt4SRJEtaValues_back, hltAntiKt4SRJPhiValues_back;
    std::vector<double> hltAntiKt4SRJLeadingEtValues_back, hltAntiKt4SRJLeadingEtaValues_back, hltAntiKt4SRJLeadingPhiValues_back;
    std::vector<double> hltAntiKt4SRJSubleadingEtValues_back, hltAntiKt4SRJSubleadingEtaValues_back, hltAntiKt4SRJSubleadingPhiValues_back;
    std::vector<unsigned int> recoAntiKt10LRJEtIndexValues_back;
    std::vector<double> recoAntiKt10LRJEtValues_back, recoAntiKt10LRJEtaValues_back, recoAntiKt10LRJPhiValues_back;
    std::vector<double> recoAntiKt10LRJLeadingEtValues_back, recoAntiKt10LRJLeadingEtaValues_back, recoAntiKt10LRJLeadingPhiValues_back;
    std::vector<double> recoAntiKt10LRJSubleadingEtValues_back, recoAntiKt10LRJSubleadingEtaValues_back, recoAntiKt10LRJSubleadingPhiValues_back;
    std::vector<unsigned int> truthAntiKt4WZSRJEtIndexValues_back;
    std::vector<double> truthAntiKt4WZSRJEtValues_back, truthAntiKt4WZSRJEtaValues_back, truthAntiKt4WZSRJPhiValues_back;
    std::vector<double> truthAntiKt4WZSRJLeadingEtValues_back, truthAntiKt4WZSRJLeadingEtaValues_back, truthAntiKt4WZSRJLeadingPhiValues_back;
    std::vector<double> truthAntiKt4WZSRJSubleadingEtValues_back, truthAntiKt4WZSRJSubleadingEtaValues_back, truthAntiKt4WZSRJSubleadingPhiValues_back;
    std::vector<unsigned int> inTimeAntiKt4TruthSRJEtIndexValues_back;
    std::vector<double> inTimeAntiKt4TruthSRJEtValues_back, inTimeAntiKt4TruthSRJEtaValues_back, inTimeAntiKt4TruthSRJPhiValues_back;
    std::vector<double> inTimeAntiKt4TruthSRJLeadingEtValues_back, inTimeAntiKt4TruthSRJLeadingEtaValues_back, inTimeAntiKt4TruthSRJLeadingPhiValues_back;
    std::vector<double> inTimeAntiKt4TruthSRJSubleadingEtValues_back, inTimeAntiKt4TruthSRJSubleadingEtaValues_back, inTimeAntiKt4TruthSRJSubleadingPhiValues_back;

    // === truthbTree_VBF ===
    truthbTree_VBF->SetBranchAddress("higgsIndex", &higgsIndexValues_VBF);
    truthbTree_VBF->SetBranchAddress("Et", &truthbquarksEtValues_VBF);
    truthbTree_VBF->SetBranchAddress("Energy", &truthbquarksEnergyValues_VBF);
    truthbTree_VBF->SetBranchAddress("pT", &truthbquarkspTValues_VBF);
    truthbTree_VBF->SetBranchAddress("px", &truthbquarkspxValues_VBF);
    truthbTree_VBF->SetBranchAddress("py", &truthbquarkspyValues_VBF);
    truthbTree_VBF->SetBranchAddress("pz", &truthbquarkspzValues_VBF);
    truthbTree_VBF->SetBranchAddress("Eta", &truthbquarksEtaValues_VBF);
    truthbTree_VBF->SetBranchAddress("Phi", &truthbquarksPhiValues_VBF);

    // === truthHiggsTree_VBF ===
    truthHiggsTree_VBF->SetBranchAddress("indexOfHiggs", &indexOfHiggsValues_VBF);
    truthHiggsTree_VBF->SetBranchAddress("invMass", &truthHiggsInvMassValues_VBF);
    truthHiggsTree_VBF->SetBranchAddress("Et", &truthHiggsEtValues_VBF);
    truthHiggsTree_VBF->SetBranchAddress("Energy", &truthHiggsEnergyValues_VBF);
    truthHiggsTree_VBF->SetBranchAddress("pT", &truthHiggspTValues_VBF);
    truthHiggsTree_VBF->SetBranchAddress("px", &truthHiggspxValues_VBF);
    truthHiggsTree_VBF->SetBranchAddress("py", &truthHiggspyValues_VBF);
    truthHiggsTree_VBF->SetBranchAddress("pz", &truthHiggspzValues_VBF);
    truthHiggsTree_VBF->SetBranchAddress("Eta", &truthHiggsEtaValues_VBF);
    truthHiggsTree_VBF->SetBranchAddress("Phi", &truthHiggsPhiValues_VBF);

    // === caloTopoTowerTree_VBF ===
    caloTopoTowerTree_VBF->SetBranchAddress("Et", &caloTopoTowerEtValues_VBF);
    caloTopoTowerTree_VBF->SetBranchAddress("Eta", &caloTopoTowerEtaValues_VBF);
    caloTopoTowerTree_VBF->SetBranchAddress("Phi", &caloTopoTowerPhiValues_VBF);

    // === topo422Tree_VBF ===
    topo422Tree_VBF->SetBranchAddress("Et", &topo422EtValues_VBF);
    topo422Tree_VBF->SetBranchAddress("Eta", &topo422EtaValues_VBF);
    topo422Tree_VBF->SetBranchAddress("Phi", &topo422PhiValues_VBF);

    // === gFexSRJTree_VBF ===
    gFexSRJTree_VBF->SetBranchAddress("EtIndex", &gFexSRJEtIndexValues_VBF);
    gFexSRJTree_VBF->SetBranchAddress("Et", &gFexSRJEtValues_VBF);
    gFexSRJTree_VBF->SetBranchAddress("Eta", &gFexSRJEtaValues_VBF);
    gFexSRJTree_VBF->SetBranchAddress("Phi", &gFexSRJPhiValues_VBF);

    // === gFexLeadingSRJTree_VBF ===
    gFexLeadingSRJTree_VBF->SetBranchAddress("Et", &gFexSRJLeadingEtValues_VBF);
    gFexLeadingSRJTree_VBF->SetBranchAddress("Eta", &gFexSRJLeadingEtaValues_VBF);
    gFexLeadingSRJTree_VBF->SetBranchAddress("Phi", &gFexSRJLeadingPhiValues_VBF);

    // === gFexSubleadingSRJTree_VBF ===
    gFexSubleadingSRJTree_VBF->SetBranchAddress("Et", &gFexSRJSubleadingEtValues_VBF);
    gFexSubleadingSRJTree_VBF->SetBranchAddress("Eta", &gFexSRJSubleadingEtaValues_VBF);
    gFexSubleadingSRJTree_VBF->SetBranchAddress("Phi", &gFexSRJSubleadingPhiValues_VBF);

    // === gFexLRJTree_VBF ===
    gFexLRJTree_VBF->SetBranchAddress("EtIndex", &gFexLRJEtIndexValues_VBF);
    gFexLRJTree_VBF->SetBranchAddress("Et", &gFexLRJEtValues_VBF);
    gFexLRJTree_VBF->SetBranchAddress("Eta", &gFexLRJEtaValues_VBF);
    gFexLRJTree_VBF->SetBranchAddress("Phi", &gFexLRJPhiValues_VBF);

    // === gFexLeadingLRJTree_VBF ===
    gFexLeadingLRJTree_VBF->SetBranchAddress("Et", &gFexLRJLeadingEtValues_VBF);
    gFexLeadingLRJTree_VBF->SetBranchAddress("Eta", &gFexLRJLeadingEtaValues_VBF);
    gFexLeadingLRJTree_VBF->SetBranchAddress("Phi", &gFexLRJLeadingPhiValues_VBF);

    // === gFexSubleadingLRJTree_VBF ===
    gFexSubleadingLRJTree_VBF->SetBranchAddress("Et", &gFexLRJSubleadingEtValues_VBF);
    gFexSubleadingLRJTree_VBF->SetBranchAddress("Eta", &gFexLRJSubleadingEtaValues_VBF);
    gFexSubleadingLRJTree_VBF->SetBranchAddress("Phi", &gFexLRJSubleadingPhiValues_VBF);

    // === jFexSRJTree_VBF ===
    jFexSRJTree_VBF->SetBranchAddress("EtIndex", &jFexSRJEtIndexValues_VBF);
    jFexSRJTree_VBF->SetBranchAddress("Et", &jFexSRJEtValues_VBF);
    jFexSRJTree_VBF->SetBranchAddress("Eta", &jFexSRJEtaValues_VBF);
    jFexSRJTree_VBF->SetBranchAddress("Phi", &jFexSRJPhiValues_VBF);

    // === jFexLeadingSRJTree_VBF ===
    jFexLeadingSRJTree_VBF->SetBranchAddress("Et", &jFexSRJLeadingEtValues_VBF);
    jFexLeadingSRJTree_VBF->SetBranchAddress("Eta", &jFexSRJLeadingEtaValues_VBF);
    jFexLeadingSRJTree_VBF->SetBranchAddress("Phi", &jFexSRJLeadingPhiValues_VBF);

    // === jFexSubleadingSRJTree_VBF ===
    jFexSubleadingSRJTree_VBF->SetBranchAddress("Et", &jFexSRJSubleadingEtValues_VBF);
    jFexSubleadingSRJTree_VBF->SetBranchAddress("Eta", &jFexSRJSubleadingEtaValues_VBF);
    jFexSubleadingSRJTree_VBF->SetBranchAddress("Phi", &jFexSRJSubleadingPhiValues_VBF);

    // === jFexLRJTree_VBF ===
    jFexLRJTree_VBF->SetBranchAddress("EtIndex", &jFexLRJEtIndexValues_VBF);
    jFexLRJTree_VBF->SetBranchAddress("Et", &jFexLRJEtValues_VBF);
    jFexLRJTree_VBF->SetBranchAddress("Eta", &jFexLRJEtaValues_VBF);
    jFexLRJTree_VBF->SetBranchAddress("Phi", &jFexLRJPhiValues_VBF);

    // === jFexLeadingLRJTree_VBF ===
    jFexLeadingLRJTree_VBF->SetBranchAddress("Et", &jFexLRJLeadingEtValues_VBF);
    jFexLeadingLRJTree_VBF->SetBranchAddress("Eta", &jFexLRJLeadingEtaValues_VBF);
    jFexLeadingLRJTree_VBF->SetBranchAddress("Phi", &jFexLRJLeadingPhiValues_VBF);

    // === jFexSubleadingLRJTree_VBF ===
    jFexSubleadingLRJTree_VBF->SetBranchAddress("Et", &jFexLRJSubleadingEtValues_VBF);
    jFexSubleadingLRJTree_VBF->SetBranchAddress("Eta", &jFexLRJSubleadingEtaValues_VBF);
    jFexSubleadingLRJTree_VBF->SetBranchAddress("Phi", &jFexLRJSubleadingPhiValues_VBF);

    // === hltAntiKt4EMTopoJetsTree_VBF ===
    hltAntiKt4EMTopoJetsTree_VBF->SetBranchAddress("EtIndex", &hltAntiKt4SRJEtIndexValues_VBF);
    hltAntiKt4EMTopoJetsTree_VBF->SetBranchAddress("Et", &hltAntiKt4SRJEtValues_VBF);
    hltAntiKt4EMTopoJetsTree_VBF->SetBranchAddress("Eta", &hltAntiKt4SRJEtaValues_VBF);
    hltAntiKt4EMTopoJetsTree_VBF->SetBranchAddress("Phi", &hltAntiKt4SRJPhiValues_VBF);

    // === leadingHltAntiKt4EMTopoJetsTree_VBF ===
    leadingHltAntiKt4EMTopoJetsTree_VBF->SetBranchAddress("Et", &hltAntiKt4SRJLeadingEtValues_VBF);
    leadingHltAntiKt4EMTopoJetsTree_VBF->SetBranchAddress("Eta", &hltAntiKt4SRJLeadingEtaValues_VBF);
    leadingHltAntiKt4EMTopoJetsTree_VBF->SetBranchAddress("Phi", &hltAntiKt4SRJLeadingPhiValues_VBF);

    // === subleadingHltAntiKt4EMTopoJetsTree_VBF ===
    subleadingHltAntiKt4EMTopoJetsTree_VBF->SetBranchAddress("Et", &hltAntiKt4SRJSubleadingEtValues_VBF);
    subleadingHltAntiKt4EMTopoJetsTree_VBF->SetBranchAddress("Eta", &hltAntiKt4SRJSubleadingEtaValues_VBF);
    subleadingHltAntiKt4EMTopoJetsTree_VBF->SetBranchAddress("Phi", &hltAntiKt4SRJSubleadingPhiValues_VBF);

    // === recoAntiKt10UFOCSSKJets_VBF ===
    recoAntiKt10UFOCSSKJets_VBF->SetBranchAddress("EtIndex", &recoAntiKt10LRJEtIndexValues_VBF);
    recoAntiKt10UFOCSSKJets_VBF->SetBranchAddress("Et", &recoAntiKt10LRJEtValues_VBF);
    recoAntiKt10UFOCSSKJets_VBF->SetBranchAddress("Eta", &recoAntiKt10LRJEtaValues_VBF);
    recoAntiKt10UFOCSSKJets_VBF->SetBranchAddress("Phi", &recoAntiKt10LRJPhiValues_VBF);

    // === leadingRecoAntiKt10UFOCSSKJets_VBF ===
    leadingRecoAntiKt10UFOCSSKJets_VBF->SetBranchAddress("Et", &recoAntiKt10LRJLeadingEtValues_VBF);
    leadingRecoAntiKt10UFOCSSKJets_VBF->SetBranchAddress("Eta", &recoAntiKt10LRJLeadingEtaValues_VBF);
    leadingRecoAntiKt10UFOCSSKJets_VBF->SetBranchAddress("Phi", &recoAntiKt10LRJLeadingPhiValues_VBF);

    // === subleadingRecoAntiKt10UFOCSSKJets_VBF ===
    subleadingRecoAntiKt10UFOCSSKJets_VBF->SetBranchAddress("Et", &recoAntiKt10LRJSubleadingEtValues_VBF);
    subleadingRecoAntiKt10UFOCSSKJets_VBF->SetBranchAddress("Eta", &recoAntiKt10LRJSubleadingEtaValues_VBF);
    subleadingRecoAntiKt10UFOCSSKJets_VBF->SetBranchAddress("Phi", &recoAntiKt10LRJSubleadingPhiValues_VBF);

    // === truthAntiKt4TruthDressedWZJets_VBF ===
    truthAntiKt4TruthDressedWZJets_VBF->SetBranchAddress("EtIndex", &truthAntiKt4WZSRJEtIndexValues_VBF);
    truthAntiKt4TruthDressedWZJets_VBF->SetBranchAddress("Et", &truthAntiKt4WZSRJEtValues_VBF);
    truthAntiKt4TruthDressedWZJets_VBF->SetBranchAddress("Eta", &truthAntiKt4WZSRJEtaValues_VBF);
    truthAntiKt4TruthDressedWZJets_VBF->SetBranchAddress("Phi", &truthAntiKt4WZSRJPhiValues_VBF);

    // === leadingTruthAntiKt4TruthDressedWZJets_VBF ===
    leadingTruthAntiKt4TruthDressedWZJets_VBF->SetBranchAddress("Et", &truthAntiKt4WZSRJLeadingEtValues_VBF);
    leadingTruthAntiKt4TruthDressedWZJets_VBF->SetBranchAddress("Eta", &truthAntiKt4WZSRJLeadingEtaValues_VBF);
    leadingTruthAntiKt4TruthDressedWZJets_VBF->SetBranchAddress("Phi", &truthAntiKt4WZSRJLeadingPhiValues_VBF);

    // === subleadingTruthAntiKt4TruthDressedWZJets_VBF ===
    subleadingTruthAntiKt4TruthDressedWZJets_VBF->SetBranchAddress("Et", &truthAntiKt4WZSRJSubleadingEtValues_VBF);
    subleadingTruthAntiKt4TruthDressedWZJets_VBF->SetBranchAddress("Eta", &truthAntiKt4WZSRJSubleadingEtaValues_VBF);
    subleadingTruthAntiKt4TruthDressedWZJets_VBF->SetBranchAddress("Phi", &truthAntiKt4WZSRJSubleadingPhiValues_VBF);

    // === inTimeAntiKt4TruthJetsTree_VBF ===
    inTimeAntiKt4TruthJetsTree_VBF->SetBranchAddress("EtIndex", &inTimeAntiKt4TruthSRJEtIndexValues_VBF);
    inTimeAntiKt4TruthJetsTree_VBF->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJEtValues_VBF);
    inTimeAntiKt4TruthJetsTree_VBF->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJEtaValues_VBF);
    inTimeAntiKt4TruthJetsTree_VBF->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJPhiValues_VBF);

    // === leadingInTimeAntiKt4TruthJetsTree_VBF ===
    leadingInTimeAntiKt4TruthJetsTree_VBF->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJLeadingEtValues_VBF);
    leadingInTimeAntiKt4TruthJetsTree_VBF->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJLeadingEtaValues_VBF);
    leadingInTimeAntiKt4TruthJetsTree_VBF->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJLeadingPhiValues_VBF);

    // === subleadingInTimeAntiKt4TruthJetsTree_VBF ===
    subleadingInTimeAntiKt4TruthJetsTree_VBF->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJSubleadingEtValues_VBF);
    subleadingInTimeAntiKt4TruthJetsTree_VBF->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJSubleadingEtaValues_VBF);
    subleadingInTimeAntiKt4TruthJetsTree_VBF->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJSubleadingPhiValues_VBF);

    // === truthbTree_ggF ===
    truthbTree_ggF->SetBranchAddress("higgsIndex", &higgsIndexValues_ggF);
    truthbTree_ggF->SetBranchAddress("Et", &truthbquarksEtValues_ggF);
    truthbTree_ggF->SetBranchAddress("Energy", &truthbquarksEnergyValues_ggF);
    truthbTree_ggF->SetBranchAddress("pT", &truthbquarkspTValues_ggF);
    truthbTree_ggF->SetBranchAddress("px", &truthbquarkspxValues_ggF);
    truthbTree_ggF->SetBranchAddress("py", &truthbquarkspyValues_ggF);
    truthbTree_ggF->SetBranchAddress("pz", &truthbquarkspzValues_ggF);
    truthbTree_ggF->SetBranchAddress("Eta", &truthbquarksEtaValues_ggF);
    truthbTree_ggF->SetBranchAddress("Phi", &truthbquarksPhiValues_ggF);

    // === truthHiggsTree_ggF ===
    truthHiggsTree_ggF->SetBranchAddress("indexOfHiggs", &indexOfHiggsValues_ggF);
    truthHiggsTree_ggF->SetBranchAddress("invMass", &truthHiggsInvMassValues_ggF);
    truthHiggsTree_ggF->SetBranchAddress("Et", &truthHiggsEtValues_ggF);
    truthHiggsTree_ggF->SetBranchAddress("Energy", &truthHiggsEnergyValues_ggF);
    truthHiggsTree_ggF->SetBranchAddress("pT", &truthHiggspTValues_ggF);
    truthHiggsTree_ggF->SetBranchAddress("px", &truthHiggspxValues_ggF);
    truthHiggsTree_ggF->SetBranchAddress("py", &truthHiggspyValues_ggF);
    truthHiggsTree_ggF->SetBranchAddress("pz", &truthHiggspzValues_ggF);
    truthHiggsTree_ggF->SetBranchAddress("Eta", &truthHiggsEtaValues_ggF);
    truthHiggsTree_ggF->SetBranchAddress("Phi", &truthHiggsPhiValues_ggF);

    // === caloTopoTowerTree_ggF ===
    caloTopoTowerTree_ggF->SetBranchAddress("Et", &caloTopoTowerEtValues_ggF);
    caloTopoTowerTree_ggF->SetBranchAddress("Eta", &caloTopoTowerEtaValues_ggF);
    caloTopoTowerTree_ggF->SetBranchAddress("Phi", &caloTopoTowerPhiValues_ggF);

    // === topo422Tree_ggF ===
    topo422Tree_ggF->SetBranchAddress("Et", &topo422EtValues_ggF);
    topo422Tree_ggF->SetBranchAddress("Eta", &topo422EtaValues_ggF);
    topo422Tree_ggF->SetBranchAddress("Phi", &topo422PhiValues_ggF);

    // === gFexSRJTree_ggF ===
    gFexSRJTree_ggF->SetBranchAddress("EtIndex", &gFexSRJEtIndexValues_ggF);
    gFexSRJTree_ggF->SetBranchAddress("Et", &gFexSRJEtValues_ggF);
    gFexSRJTree_ggF->SetBranchAddress("Eta", &gFexSRJEtaValues_ggF);
    gFexSRJTree_ggF->SetBranchAddress("Phi", &gFexSRJPhiValues_ggF);

    // === gFexLeadingSRJTree_ggF ===
    gFexLeadingSRJTree_ggF->SetBranchAddress("Et", &gFexSRJLeadingEtValues_ggF);
    gFexLeadingSRJTree_ggF->SetBranchAddress("Eta", &gFexSRJLeadingEtaValues_ggF);
    gFexLeadingSRJTree_ggF->SetBranchAddress("Phi", &gFexSRJLeadingPhiValues_ggF);

    // === gFexSubleadingSRJTree_ggF ===
    gFexSubleadingSRJTree_ggF->SetBranchAddress("Et", &gFexSRJSubleadingEtValues_ggF);
    gFexSubleadingSRJTree_ggF->SetBranchAddress("Eta", &gFexSRJSubleadingEtaValues_ggF);
    gFexSubleadingSRJTree_ggF->SetBranchAddress("Phi", &gFexSRJSubleadingPhiValues_ggF);

    // === gFexLRJTree_ggF ===
    gFexLRJTree_ggF->SetBranchAddress("EtIndex", &gFexLRJEtIndexValues_ggF);
    gFexLRJTree_ggF->SetBranchAddress("Et", &gFexLRJEtValues_ggF);
    gFexLRJTree_ggF->SetBranchAddress("Eta", &gFexLRJEtaValues_ggF);
    gFexLRJTree_ggF->SetBranchAddress("Phi", &gFexLRJPhiValues_ggF);

    // === gFexLeadingLRJTree_ggF ===
    gFexLeadingLRJTree_ggF->SetBranchAddress("Et", &gFexLRJLeadingEtValues_ggF);
    gFexLeadingLRJTree_ggF->SetBranchAddress("Eta", &gFexLRJLeadingEtaValues_ggF);
    gFexLeadingLRJTree_ggF->SetBranchAddress("Phi", &gFexLRJLeadingPhiValues_ggF);

    // === gFexSubleadingLRJTree_ggF ===
    gFexSubleadingLRJTree_ggF->SetBranchAddress("Et", &gFexLRJSubleadingEtValues_ggF);
    gFexSubleadingLRJTree_ggF->SetBranchAddress("Eta", &gFexLRJSubleadingEtaValues_ggF);
    gFexSubleadingLRJTree_ggF->SetBranchAddress("Phi", &gFexLRJSubleadingPhiValues_ggF);

    // === jFexSRJTree_ggF ===
    jFexSRJTree_ggF->SetBranchAddress("EtIndex", &jFexSRJEtIndexValues_ggF);
    jFexSRJTree_ggF->SetBranchAddress("Et", &jFexSRJEtValues_ggF);
    jFexSRJTree_ggF->SetBranchAddress("Eta", &jFexSRJEtaValues_ggF);
    jFexSRJTree_ggF->SetBranchAddress("Phi", &jFexSRJPhiValues_ggF);

    // === jFexLeadingSRJTree_ggF ===
    jFexLeadingSRJTree_ggF->SetBranchAddress("Et", &jFexSRJLeadingEtValues_ggF);
    jFexLeadingSRJTree_ggF->SetBranchAddress("Eta", &jFexSRJLeadingEtaValues_ggF);
    jFexLeadingSRJTree_ggF->SetBranchAddress("Phi", &jFexSRJLeadingPhiValues_ggF);

    // === jFexSubleadingSRJTree_ggF ===
    jFexSubleadingSRJTree_ggF->SetBranchAddress("Et", &jFexSRJSubleadingEtValues_ggF);
    jFexSubleadingSRJTree_ggF->SetBranchAddress("Eta", &jFexSRJSubleadingEtaValues_ggF);
    jFexSubleadingSRJTree_ggF->SetBranchAddress("Phi", &jFexSRJSubleadingPhiValues_ggF);

    // === jFexLRJTree_ggF ===
    jFexLRJTree_ggF->SetBranchAddress("EtIndex", &jFexLRJEtIndexValues_ggF);
    jFexLRJTree_ggF->SetBranchAddress("Et", &jFexLRJEtValues_ggF);
    jFexLRJTree_ggF->SetBranchAddress("Eta", &jFexLRJEtaValues_ggF);
    jFexLRJTree_ggF->SetBranchAddress("Phi", &jFexLRJPhiValues_ggF);

    // === jFexLeadingLRJTree_ggF ===
    jFexLeadingLRJTree_ggF->SetBranchAddress("Et", &jFexLRJLeadingEtValues_ggF);
    jFexLeadingLRJTree_ggF->SetBranchAddress("Eta", &jFexLRJLeadingEtaValues_ggF);
    jFexLeadingLRJTree_ggF->SetBranchAddress("Phi", &jFexLRJLeadingPhiValues_ggF);

    // === jFexSubleadingLRJTree_ggF ===
    jFexSubleadingLRJTree_ggF->SetBranchAddress("Et", &jFexLRJSubleadingEtValues_ggF);
    jFexSubleadingLRJTree_ggF->SetBranchAddress("Eta", &jFexLRJSubleadingEtaValues_ggF);
    jFexSubleadingLRJTree_ggF->SetBranchAddress("Phi", &jFexLRJSubleadingPhiValues_ggF);

    // === hltAntiKt4EMTopoJetsTree_ggF ===
    hltAntiKt4EMTopoJetsTree_ggF->SetBranchAddress("EtIndex", &hltAntiKt4SRJEtIndexValues_ggF);
    hltAntiKt4EMTopoJetsTree_ggF->SetBranchAddress("Et", &hltAntiKt4SRJEtValues_ggF);
    hltAntiKt4EMTopoJetsTree_ggF->SetBranchAddress("Eta", &hltAntiKt4SRJEtaValues_ggF);
    hltAntiKt4EMTopoJetsTree_ggF->SetBranchAddress("Phi", &hltAntiKt4SRJPhiValues_ggF);

    // === leadingHltAntiKt4EMTopoJetsTree_ggF ===
    leadingHltAntiKt4EMTopoJetsTree_ggF->SetBranchAddress("Et", &hltAntiKt4SRJLeadingEtValues_ggF);
    leadingHltAntiKt4EMTopoJetsTree_ggF->SetBranchAddress("Eta", &hltAntiKt4SRJLeadingEtaValues_ggF);
    leadingHltAntiKt4EMTopoJetsTree_ggF->SetBranchAddress("Phi", &hltAntiKt4SRJLeadingPhiValues_ggF);

    // === subleadingHltAntiKt4EMTopoJetsTree_ggF ===
    subleadingHltAntiKt4EMTopoJetsTree_ggF->SetBranchAddress("Et", &hltAntiKt4SRJSubleadingEtValues_ggF);
    subleadingHltAntiKt4EMTopoJetsTree_ggF->SetBranchAddress("Eta", &hltAntiKt4SRJSubleadingEtaValues_ggF);
    subleadingHltAntiKt4EMTopoJetsTree_ggF->SetBranchAddress("Phi", &hltAntiKt4SRJSubleadingPhiValues_ggF);

    // === recoAntiKt10UFOCSSKJets_ggF ===
    recoAntiKt10UFOCSSKJets_ggF->SetBranchAddress("EtIndex", &recoAntiKt10LRJEtIndexValues_ggF);
    recoAntiKt10UFOCSSKJets_ggF->SetBranchAddress("Et", &recoAntiKt10LRJEtValues_ggF);
    recoAntiKt10UFOCSSKJets_ggF->SetBranchAddress("Eta", &recoAntiKt10LRJEtaValues_ggF);
    recoAntiKt10UFOCSSKJets_ggF->SetBranchAddress("Phi", &recoAntiKt10LRJPhiValues_ggF);

    // === leadingRecoAntiKt10UFOCSSKJets_ggF ===
    leadingRecoAntiKt10UFOCSSKJets_ggF->SetBranchAddress("Et", &recoAntiKt10LRJLeadingEtValues_ggF);
    leadingRecoAntiKt10UFOCSSKJets_ggF->SetBranchAddress("Eta", &recoAntiKt10LRJLeadingEtaValues_ggF);
    leadingRecoAntiKt10UFOCSSKJets_ggF->SetBranchAddress("Phi", &recoAntiKt10LRJLeadingPhiValues_ggF);

    // === subleadingRecoAntiKt10UFOCSSKJets_ggF ===
    subleadingRecoAntiKt10UFOCSSKJets_ggF->SetBranchAddress("Et", &recoAntiKt10LRJSubleadingEtValues_ggF);
    subleadingRecoAntiKt10UFOCSSKJets_ggF->SetBranchAddress("Eta", &recoAntiKt10LRJSubleadingEtaValues_ggF);
    subleadingRecoAntiKt10UFOCSSKJets_ggF->SetBranchAddress("Phi", &recoAntiKt10LRJSubleadingPhiValues_ggF);

    // === truthAntiKt4TruthDressedWZJets_ggF ===
    truthAntiKt4TruthDressedWZJets_ggF->SetBranchAddress("EtIndex", &truthAntiKt4WZSRJEtIndexValues_ggF);
    truthAntiKt4TruthDressedWZJets_ggF->SetBranchAddress("Et", &truthAntiKt4WZSRJEtValues_ggF);
    truthAntiKt4TruthDressedWZJets_ggF->SetBranchAddress("Eta", &truthAntiKt4WZSRJEtaValues_ggF);
    truthAntiKt4TruthDressedWZJets_ggF->SetBranchAddress("Phi", &truthAntiKt4WZSRJPhiValues_ggF);

    // === leadingTruthAntiKt4TruthDressedWZJets_ggF ===
    leadingTruthAntiKt4TruthDressedWZJets_ggF->SetBranchAddress("Et", &truthAntiKt4WZSRJLeadingEtValues_ggF);
    leadingTruthAntiKt4TruthDressedWZJets_ggF->SetBranchAddress("Eta", &truthAntiKt4WZSRJLeadingEtaValues_ggF);
    leadingTruthAntiKt4TruthDressedWZJets_ggF->SetBranchAddress("Phi", &truthAntiKt4WZSRJLeadingPhiValues_ggF);

    // === subleadingTruthAntiKt4TruthDressedWZJets_ggF ===
    subleadingTruthAntiKt4TruthDressedWZJets_ggF->SetBranchAddress("Et", &truthAntiKt4WZSRJSubleadingEtValues_ggF);
    subleadingTruthAntiKt4TruthDressedWZJets_ggF->SetBranchAddress("Eta", &truthAntiKt4WZSRJSubleadingEtaValues_ggF);
    subleadingTruthAntiKt4TruthDressedWZJets_ggF->SetBranchAddress("Phi", &truthAntiKt4WZSRJSubleadingPhiValues_ggF);

    // === inTimeAntiKt4TruthJetsTree_ggF ===
    inTimeAntiKt4TruthJetsTree_ggF->SetBranchAddress("EtIndex", &inTimeAntiKt4TruthSRJEtIndexValues_ggF);
    inTimeAntiKt4TruthJetsTree_ggF->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJEtValues_ggF);
    inTimeAntiKt4TruthJetsTree_ggF->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJEtaValues_ggF);
    inTimeAntiKt4TruthJetsTree_ggF->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJPhiValues_ggF);

    // === leadingInTimeAntiKt4TruthJetsTree_ggF ===
    leadingInTimeAntiKt4TruthJetsTree_ggF->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJLeadingEtValues_ggF);
    leadingInTimeAntiKt4TruthJetsTree_ggF->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJLeadingEtaValues_ggF);
    leadingInTimeAntiKt4TruthJetsTree_ggF->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJLeadingPhiValues_ggF);

    // === subleadingInTimeAntiKt4TruthJetsTree_ggF ===
    subleadingInTimeAntiKt4TruthJetsTree_ggF->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJSubleadingEtValues_ggF);
    subleadingInTimeAntiKt4TruthJetsTree_ggF->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJSubleadingEtaValues_ggF);
    subleadingInTimeAntiKt4TruthJetsTree_ggF->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJSubleadingPhiValues_ggF);

    // === caloTopoTowerTree_back ===
    caloTopoTowerTree_back->SetBranchAddress("Et", &caloTopoTowerEtValues_back);
    caloTopoTowerTree_back->SetBranchAddress("Eta", &caloTopoTowerEtaValues_back);
    caloTopoTowerTree_back->SetBranchAddress("Phi", &caloTopoTowerPhiValues_back);

    // === topo422Tree_back ===
    topo422Tree_back->SetBranchAddress("Et", &topo422EtValues_back);
    topo422Tree_back->SetBranchAddress("Eta", &topo422EtaValues_back);
    topo422Tree_back->SetBranchAddress("Phi", &topo422PhiValues_back);

    // === gFexSRJTree_back ===
    gFexSRJTree_back->SetBranchAddress("EtIndex", &gFexSRJEtIndexValues_back);
    gFexSRJTree_back->SetBranchAddress("Et", &gFexSRJEtValues_back);
    gFexSRJTree_back->SetBranchAddress("Eta", &gFexSRJEtaValues_back);
    gFexSRJTree_back->SetBranchAddress("Phi", &gFexSRJPhiValues_back);

    // === gFexLeadingSRJTree_back ===
    gFexLeadingSRJTree_back->SetBranchAddress("Et", &gFexSRJLeadingEtValues_back);
    gFexLeadingSRJTree_back->SetBranchAddress("Eta", &gFexSRJLeadingEtaValues_back);
    gFexLeadingSRJTree_back->SetBranchAddress("Phi", &gFexSRJLeadingPhiValues_back);

    // === gFexSubleadingSRJTree_back ===
    gFexSubleadingSRJTree_back->SetBranchAddress("Et", &gFexSRJSubleadingEtValues_back);
    gFexSubleadingSRJTree_back->SetBranchAddress("Eta", &gFexSRJSubleadingEtaValues_back);
    gFexSubleadingSRJTree_back->SetBranchAddress("Phi", &gFexSRJSubleadingPhiValues_back);

    // === gFexLRJTree_back ===
    gFexLRJTree_back->SetBranchAddress("EtIndex", &gFexLRJEtIndexValues_back);
    gFexLRJTree_back->SetBranchAddress("Et", &gFexLRJEtValues_back);
    gFexLRJTree_back->SetBranchAddress("Eta", &gFexLRJEtaValues_back);
    gFexLRJTree_back->SetBranchAddress("Phi", &gFexLRJPhiValues_back);

    // === gFexLeadingLRJTree_back ===
    gFexLeadingLRJTree_back->SetBranchAddress("Et", &gFexLRJLeadingEtValues_back);
    gFexLeadingLRJTree_back->SetBranchAddress("Eta", &gFexLRJLeadingEtaValues_back);
    gFexLeadingLRJTree_back->SetBranchAddress("Phi", &gFexLRJLeadingPhiValues_back);

    // === gFexSubleadingLRJTree_back ===
    gFexSubleadingLRJTree_back->SetBranchAddress("Et", &gFexLRJSubleadingEtValues_back);
    gFexSubleadingLRJTree_back->SetBranchAddress("Eta", &gFexLRJSubleadingEtaValues_back);
    gFexSubleadingLRJTree_back->SetBranchAddress("Phi", &gFexLRJSubleadingPhiValues_back);

    // === jFexSRJTree_back ===
    jFexSRJTree_back->SetBranchAddress("EtIndex", &jFexSRJEtIndexValues_back);
    jFexSRJTree_back->SetBranchAddress("Et", &jFexSRJEtValues_back);
    jFexSRJTree_back->SetBranchAddress("Eta", &jFexSRJEtaValues_back);
    jFexSRJTree_back->SetBranchAddress("Phi", &jFexSRJPhiValues_back);

    // === jFexLeadingSRJTree_back ===
    jFexLeadingSRJTree_back->SetBranchAddress("Et", &jFexSRJLeadingEtValues_back);
    jFexLeadingSRJTree_back->SetBranchAddress("Eta", &jFexSRJLeadingEtaValues_back);
    jFexLeadingSRJTree_back->SetBranchAddress("Phi", &jFexSRJLeadingPhiValues_back);

    // === jFexSubleadingSRJTree_back ===
    jFexSubleadingSRJTree_back->SetBranchAddress("Et", &jFexSRJSubleadingEtValues_back);
    jFexSubleadingSRJTree_back->SetBranchAddress("Eta", &jFexSRJSubleadingEtaValues_back);
    jFexSubleadingSRJTree_back->SetBranchAddress("Phi", &jFexSRJSubleadingPhiValues_back);

    // === jFexLRJTree_back ===
    jFexLRJTree_back->SetBranchAddress("EtIndex", &jFexLRJEtIndexValues_back);
    jFexLRJTree_back->SetBranchAddress("Et", &jFexLRJEtValues_back);
    jFexLRJTree_back->SetBranchAddress("Eta", &jFexLRJEtaValues_back);
    jFexLRJTree_back->SetBranchAddress("Phi", &jFexLRJPhiValues_back);

    // === jFexLeadingLRJTree_back ===
    jFexLeadingLRJTree_back->SetBranchAddress("Et", &jFexLRJLeadingEtValues_back);
    jFexLeadingLRJTree_back->SetBranchAddress("Eta", &jFexLRJLeadingEtaValues_back);
    jFexLeadingLRJTree_back->SetBranchAddress("Phi", &jFexLRJLeadingPhiValues_back);

    // === jFexSubleadingLRJTree_back ===
    jFexSubleadingLRJTree_back->SetBranchAddress("Et", &jFexLRJSubleadingEtValues_back);
    jFexSubleadingLRJTree_back->SetBranchAddress("Eta", &jFexLRJSubleadingEtaValues_back);
    jFexSubleadingLRJTree_back->SetBranchAddress("Phi", &jFexLRJSubleadingPhiValues_back);

    // === hltAntiKt4EMTopoJetsTree_back ===
    hltAntiKt4EMTopoJetsTree_back->SetBranchAddress("EtIndex", &hltAntiKt4SRJEtIndexValues_back);
    hltAntiKt4EMTopoJetsTree_back->SetBranchAddress("Et", &hltAntiKt4SRJEtValues_back);
    hltAntiKt4EMTopoJetsTree_back->SetBranchAddress("Eta", &hltAntiKt4SRJEtaValues_back);
    hltAntiKt4EMTopoJetsTree_back->SetBranchAddress("Phi", &hltAntiKt4SRJPhiValues_back);

    // === leadingHltAntiKt4EMTopoJetsTree_back ===
    leadingHltAntiKt4EMTopoJetsTree_back->SetBranchAddress("Et", &hltAntiKt4SRJLeadingEtValues_back);
    leadingHltAntiKt4EMTopoJetsTree_back->SetBranchAddress("Eta", &hltAntiKt4SRJLeadingEtaValues_back);
    leadingHltAntiKt4EMTopoJetsTree_back->SetBranchAddress("Phi", &hltAntiKt4SRJLeadingPhiValues_back);

    // === subleadingHltAntiKt4EMTopoJetsTree_back ===
    subleadingHltAntiKt4EMTopoJetsTree_back->SetBranchAddress("Et", &hltAntiKt4SRJSubleadingEtValues_back);
    subleadingHltAntiKt4EMTopoJetsTree_back->SetBranchAddress("Eta", &hltAntiKt4SRJSubleadingEtaValues_back);
    subleadingHltAntiKt4EMTopoJetsTree_back->SetBranchAddress("Phi", &hltAntiKt4SRJSubleadingPhiValues_back);

    // === recoAntiKt10UFOCSSKJets_back ===
    recoAntiKt10UFOCSSKJets_back->SetBranchAddress("EtIndex", &recoAntiKt10LRJEtIndexValues_back);
    recoAntiKt10UFOCSSKJets_back->SetBranchAddress("Et", &recoAntiKt10LRJEtValues_back);
    recoAntiKt10UFOCSSKJets_back->SetBranchAddress("Eta", &recoAntiKt10LRJEtaValues_back);
    recoAntiKt10UFOCSSKJets_back->SetBranchAddress("Phi", &recoAntiKt10LRJPhiValues_back);

    // === leadingRecoAntiKt10UFOCSSKJets_back ===
    leadingRecoAntiKt10UFOCSSKJets_back->SetBranchAddress("Et", &recoAntiKt10LRJLeadingEtValues_back);
    leadingRecoAntiKt10UFOCSSKJets_back->SetBranchAddress("Eta", &recoAntiKt10LRJLeadingEtaValues_back);
    leadingRecoAntiKt10UFOCSSKJets_back->SetBranchAddress("Phi", &recoAntiKt10LRJLeadingPhiValues_back);

    // === subleadingRecoAntiKt10UFOCSSKJets_back ===
    subleadingRecoAntiKt10UFOCSSKJets_back->SetBranchAddress("Et", &recoAntiKt10LRJSubleadingEtValues_back);
    subleadingRecoAntiKt10UFOCSSKJets_back->SetBranchAddress("Eta", &recoAntiKt10LRJSubleadingEtaValues_back);
    subleadingRecoAntiKt10UFOCSSKJets_back->SetBranchAddress("Phi", &recoAntiKt10LRJSubleadingPhiValues_back);

    // === truthAntiKt4TruthDressedWZJets_back ===
    truthAntiKt4TruthDressedWZJets_back->SetBranchAddress("EtIndex", &truthAntiKt4WZSRJEtIndexValues_back);
    truthAntiKt4TruthDressedWZJets_back->SetBranchAddress("Et", &truthAntiKt4WZSRJEtValues_back);
    truthAntiKt4TruthDressedWZJets_back->SetBranchAddress("Eta", &truthAntiKt4WZSRJEtaValues_back);
    truthAntiKt4TruthDressedWZJets_back->SetBranchAddress("Phi", &truthAntiKt4WZSRJPhiValues_back);

    // === leadingTruthAntiKt4TruthDressedWZJets_back ===
    leadingTruthAntiKt4TruthDressedWZJets_back->SetBranchAddress("Et", &truthAntiKt4WZSRJLeadingEtValues_back);
    leadingTruthAntiKt4TruthDressedWZJets_back->SetBranchAddress("Eta", &truthAntiKt4WZSRJLeadingEtaValues_back);
    leadingTruthAntiKt4TruthDressedWZJets_back->SetBranchAddress("Phi", &truthAntiKt4WZSRJLeadingPhiValues_back);

    // === subleadingTruthAntiKt4TruthDressedWZJets_back ===
    subleadingTruthAntiKt4TruthDressedWZJets_back->SetBranchAddress("Et", &truthAntiKt4WZSRJSubleadingEtValues_back);
    subleadingTruthAntiKt4TruthDressedWZJets_back->SetBranchAddress("Eta", &truthAntiKt4WZSRJSubleadingEtaValues_back);
    subleadingTruthAntiKt4TruthDressedWZJets_back->SetBranchAddress("Phi", &truthAntiKt4WZSRJSubleadingPhiValues_back);

    // === inTimeAntiKt4TruthJetsTree_back ===
    inTimeAntiKt4TruthJetsTree_back->SetBranchAddress("EtIndex", &inTimeAntiKt4TruthSRJEtIndexValues_back);
    inTimeAntiKt4TruthJetsTree_back->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJEtValues_back);
    inTimeAntiKt4TruthJetsTree_back->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJEtaValues_back);
    inTimeAntiKt4TruthJetsTree_back->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJPhiValues_back);

    // === leadingInTimeAntiKt4TruthJetsTree_back ===
    leadingInTimeAntiKt4TruthJetsTree_back->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJLeadingEtValues_back);
    leadingInTimeAntiKt4TruthJetsTree_back->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJLeadingEtaValues_back);
    leadingInTimeAntiKt4TruthJetsTree_back->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJLeadingPhiValues_back);

    // === subleadingInTimeAntiKt4TruthJetsTree_back ===
    subleadingInTimeAntiKt4TruthJetsTree_back->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJSubleadingEtValues_back);
    subleadingInTimeAntiKt4TruthJetsTree_back->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJSubleadingEtaValues_back);
    subleadingInTimeAntiKt4TruthJetsTree_back->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJSubleadingPhiValues_back);

    // VBF histogram declarations
    TH1F* sig_vbf_h_b_Et = new TH1F("sig_vbf_h_b_Et", "b E_{T} Distribution;E_{T} (GeV);b's / 2 GeV", 200, 0, 400);
    TH1F* sig_vbf_h_higgs_Et = new TH1F("sig_vbf_h_higgs_Et", "Higgs E_{T} Distribution;E_{T} (GeV);Higgs's / 2 GeV", 200, 0, 400);

    TH1F* sig_vbf_h_topo_Et = new TH1F("sig_vbf_h_topo_Et", "Topo E_{T} Distribution;E_{T} (GeV);Topo422 Clusters / 2 GeV", 200, 0, 400);
    TH1F* sig_vbf_h_calotopo_Et = new TH1F("sig_vbf_h_calotopo_Et", "CaloTopoTower E_{T} Distribution;Et (GeV);CaloTopo Towers / 2 GeV", 200, 0, 400);

    TH1F* sig_vbf_h_gFex_SRJ_Et = new TH1F("sig_vbf_h_gFex_SRJ_Et", "gFex SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* sig_vbf_h_lead_gFex_SRJ_Et = new TH1F("sig_vbf_h_lead_gFex_SRJ_Et", "Lead. gFex SRJ E_{T} Distribution;Et (GeV);Counts", 30, 0, 300);

    TH1F* sig_vbf_h_gFex_LRJ_Et = new TH1F("sig_vbf_h_gFex_LRJ_Et", "gFex LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* sig_vbf_h_lead_gFex_LRJ_Et = new TH1F("sig_vbf_h_lead_gFex_LRJ_Et", "Lead. gFex LRJ E_{T} Distribution;Et (GeV);Counts", 30, 0, 300);

    TH1F* sig_vbf_h_jFex_SRJ_Et = new TH1F("sig_vbf_h_jFex_SRJ_Et", "jFex SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* sig_vbf_h_lead_jFex_SRJ_Et = new TH1F("sig_vbf_h_lead_jFex_SRJ_Et", "Lead. jFex SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);

    TH1F* sig_vbf_h_jFex_LRJ_Et = new TH1F("sig_vbf_h_jFex_LRJ_Et", "jFex LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* sig_vbf_h_lead_jFex_LRJ_Et = new TH1F("sig_vbf_h_lead_jFex_LRJ_Et", "Lead. jFex LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    
    TH1F* sig_vbf_h_reco_antikt10UFOCSSKJets_Et = new TH1F("sig_vbf_h_reco_antikt10UFOCSSKJets_Et", "Offline Reco. LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* sig_vbf_h_lead_reco_antikt10UFOCSSKJets_Et = new TH1F("sig_vbf_h_lead_reco_antikt10UFOCSSKJets_Et", "Lead. Offline Reco. LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);

    TH1F* sig_vbf_h_truth_antikt4DressedWZJets_Et = new TH1F("sig_vbf_h_truth_antikt4DressedWZJets_Et", "Truth SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* sig_vbf_h_lead_truth_antikt4DressedWZJets_Et = new TH1F("sig_vbf_h_lead_truth_antikt4DressedWZJets_Et", "Truth SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);

    // ggF histogram declarations
    TH1F* sig_ggf_h_b_Et = new TH1F("sig_ggf_h_b_Et", "b E_{T} Distribution;E_{T} (GeV);b's / 2 GeV", 200, 0, 400);
    TH1F* sig_ggf_h_higgs_Et = new TH1F("sig_ggf_h_higgs_Et", "Higgs E_{T} Distribution;E_{T} (GeV);Higgs's / 2 GeV", 200, 0, 400);

    TH1F* sig_ggf_h_topo_Et = new TH1F("sig_ggf_h_topo_Et", "Topo E_{T} Distribution;E_{T} (GeV);Topo422 Clusters / 2 GeV", 200, 0, 400);
    TH1F* sig_ggf_h_calotopo_Et = new TH1F("sig_ggf_h_calotopo_Et", "CaloTopoTower E_{T} Distribution;Et (GeV);CaloTopo Towers / 2 GeV", 200, 0, 400);

    TH1F* sig_ggf_h_gFex_SRJ_Et = new TH1F("sig_ggf_h_gFex_SRJ_Et", "gFex SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* sig_ggf_h_lead_gFex_SRJ_Et = new TH1F("sig_ggf_h_lead_gFex_SRJ_Et", "Lead. gFex SRJ E_{T} Distribution;Et (GeV);Counts", 30, 0, 300);

    TH1F* sig_ggf_h_gFex_LRJ_Et = new TH1F("sig_ggf_h_gFex_LRJ_Et", "gFex LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* sig_ggf_h_lead_gFex_LRJ_Et = new TH1F("sig_ggf_h_lead_gFex_LRJ_Et", "Lead. gFex LRJ E_{T} Distribution;Et (GeV);Counts", 30, 0, 300);

    TH1F* sig_ggf_h_jFex_SRJ_Et = new TH1F("sig_ggf_h_jFex_SRJ_Et", "jFex SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* sig_ggf_h_lead_jFex_SRJ_Et = new TH1F("sig_ggf_h_lead_jFex_SRJ_Et", "Lead. jFex SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);

    TH1F* sig_ggf_h_jFex_LRJ_Et = new TH1F("sig_ggf_h_jFex_LRJ_Et", "jFex LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* sig_ggf_h_lead_jFex_LRJ_Et = new TH1F("sig_ggf_h_lead_jFex_LRJ_Et", "Lead. jFex LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    
    TH1F* sig_ggf_h_reco_antikt10UFOCSSKJets_Et = new TH1F("sig_ggf_h_reco_antikt10UFOCSSKJets_Et", "Offline Reco. LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* sig_ggf_h_lead_reco_antikt10UFOCSSKJets_Et = new TH1F("sig_ggf_h_lead_reco_antikt10UFOCSSKJets_Et", "Lead. Offline Reco. LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);

    TH1F* sig_ggf_h_truth_antikt4DressedWZJets_Et = new TH1F("sig_ggf_h_truth_antikt4DressedWZJets_Et", "Truth SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* sig_ggf_h_lead_truth_antikt4DressedWZJets_Et = new TH1F("sig_ggf_h_lead_truth_antikt4DressedWZJets_Et", "Truth SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);

    // Background histogram declarations
    TH1F* back_h_topo_Et = new TH1F("back_h_topo_Et", "Topo E_{T} Distribution;E_{T} (GeV);Topo422 Clusters / 2 GeV", 200, 0, 400);
    TH1F* back_h_calotopo_Et = new TH1F("back_h_calotopo_Et", "CaloTopoTower E_{T} Distribution;Et (GeV);CaloTopo Towers / 2 GeV", 200, 0, 400);

    TH1F* back_h_gFex_SRJ_Et = new TH1F("back_h_gFex_SRJ_Et", "gFex SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* back_h_lead_gFex_SRJ_Et = new TH1F("back_h_lead_gFex_SRJ_Et", "Lead. gFex SRJ E_{T} Distribution;Et (GeV);Counts", 30, 0, 300);

    TH1F* back_h_gFex_LRJ_Et = new TH1F("back_h_gFex_LRJ_Et", "gFex LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* back_h_lead_gFex_LRJ_Et = new TH1F("back_h_lead_gFex_LRJ_Et", "Lead. gFex LRJ E_{T} Distribution;Et (GeV);Counts", 30, 0, 300);

    TH1F* back_h_jFex_SRJ_Et = new TH1F("back_h_jFex_SRJ_Et", "jFex SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* back_h_lead_jFex_SRJ_Et = new TH1F("back_h_lead_jFex_SRJ_Et", "Lead. jFex SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);

    TH1F* back_h_jFex_LRJ_Et = new TH1F("back_h_jFex_LRJ_Et", "jFex LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* back_h_lead_jFex_LRJ_Et = new TH1F("back_h_lead_jFex_LRJ_Et", "Lead. jFex LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    
    TH1F* back_h_reco_antikt10UFOCSSKJets_Et = new TH1F("back_h_reco_antikt10UFOCSSKJets_Et", "Offline Reco. LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* back_h_lead_reco_antikt10UFOCSSKJets_Et = new TH1F("back_h_lead_reco_antikt10UFOCSSKJets_Et", "Lead. Offline Reco. LRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);

    TH1F* back_h_truth_antikt4DressedWZJets_Et = new TH1F("back_h_truth_antikt4DressedWZJets_Et", "Truth SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);
    TH1F* back_h_lead_truth_antikt4DressedWZJets_Et = new TH1F("back_h_lead_truth_antikt4DressedWZJets_Et", "Truth SRJ E_{T} Distribution;E_{T} (GeV);Counts", 30, 0, 300);

    unsigned int sigVBFNEntries = truthbTree_VBF->GetEntries();







    unsigned int sigggFNEntries = truthbTree_VBF->GetEntries();












    unsigned int backNEntries = caloTopoTowerTree_back->GetEntries(); 











}




















void callPlot() {
    // Usage: callPlot(true, true)
    std::string signalInputFileVBF = "outputRootFiles/mc21_14TeV_hh_bbbb_vbf_novhh_DAOD_NTUPLE.root";
    std::string signalInputFileggF = "outputRootFiles/mc21_14TeV_HHbbbb_HLLHC_DAOD_NTUPLE.root";
     
    const std::string backgroundInputFile = "outputRootFiles/mc21_14TeV_jj_JZ3_DAOD_NTUPLE.root";


    analyzeHistograms(signalInputFileVBF, signalInputFileggF, backgroundInputFile);
    gSystem->Exit(0);
}
