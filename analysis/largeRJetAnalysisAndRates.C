
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <array>
#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include <cmath>
#include <TMath.h>
#include <cstdio>
#include "analysisHelperFunctions.h"
//#include "../algorithm/constants.h"
using namespace std;

int colors[] = {
            
            
            kRed,          // strong red
            kGreen+2,         // deep blue
            kBlue,      // vivid green
            kMagenta,      // purple-pink
            kOrange+7,     // light orange

            kCyan+1,       // bright cyan
            kViolet+1,     // light violet
            kAzure+2,      // soft blue
            kPink+3,       // light pink
            kSpring+5      // light green-yellow
        };

std::map<std::string, std::string> legendMap = {
    {"Seeds2_r2Cut0p64_maxObj128_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 0.64, N_{IO} = 128, No IO E_{cut}"},

    {"Seeds2_r2Cut0p81_maxObj128_1p5back",
        "R^{2}_{cut} = 0.81, N_{IO} = 128, rMergeCut = 1.5"},
    
    {"Seeds2_r2Cut0p81_maxObj256_1p5back",
        "R^{2}_{cut} = 0.81, N_{IO} = 256, rMergeCut = 1.5"},

    {"Seeds2_r2Cut1p0_maxObj128_0p01back",
        "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut Disabled"},

    {"Seeds2_r2Cut1p0_maxObj64_0p001back",
        "R^{2}_{cut} = 1.0, N_{IO} = 64"},

    {"Seeds2_r2Cut1p0_maxObj128_0p001back",
        "R^{2}_{cut} = 1.0, N_{IO} = 128"},

    {"Seeds2_r2Cut1p0_maxObj256_0p001back",
        "R^{2}_{cut} = 1.0, N_{IO} = 256"},


    {"Seeds2_r2Cut1p0_maxObj128_0p001back_SeedPosRecalcWeighted",
    "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut Disabled"},

    {"Seeds2_r2Cut1p0_maxObj128_1p5back_SeedPosRecalcWeighted",
    "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut  = 1.5"},

    {"Seeds2_r2Cut1p0_maxObj128_2p0back_SeedPosRecalcWeighted",
    "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut  = 2.5"},

    {"Seeds2_r2Cut1p0_maxObj128_2p5back_SeedPosRecalcWeighted",
    "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut  = 2.5"},

    {"Seeds2_r2Cut1p0_maxObj128_3p0back_SeedPosRecalcWeighted",
    "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut  = 3.5"},

    {"Seeds2_r2Cut1p0_maxObj128_3p5back_SeedPosRecalcWeighted",
    "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut  = 3.5"},

    {"Seeds2_r2Cut1p0_maxObj256_0p001back_SeedPosRecalcWeighted",
    "R^{2}_{cut} = 1.0, N_{IO} = 256, rMergeCut Disabled"},

    {"Seeds2_r2Cut1p0_maxObj256_1p5back_SeedPosRecalcWeighted",
    "R^{2}_{cut} = 1.0, N_{IO} = 256, rMergeCut  = 1.5"},

    {"Seeds2_r2Cut1p0_maxObj256_2p5back_SeedPosRecalcWeighted",
    "R^{2}_{cut} = 1.0, N_{IO} = 256, rMergeCut  = 2.5"},

    {"Seeds2_r2Cut1p0_maxObj256_3p5back_SeedPosRecalcWeighted",
    "R^{2}_{cut} = 1.0, N_{IO} = 256, rMergeCut  = 3.5"},

    {"Seeds2_r2Cut1p0_maxObj128_1p25back",
        "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut = 1.25"},

    {"Seeds2_r2Cut1p0_maxObj128_1p5back",
        "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut = 1.5"},

    {"Seeds2_r2Cut1p0_maxObj128_1p75back",
        "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut = 1.75"},
    
    {"Seeds2_r2Cut1p0_maxObj256_1p5back",
        "R^{2}_{cut} = 1.0, N_{IO} = 256, rMergeCut = 1.5"},

    {"Seeds2_r2Cut1p44_maxObj128_1p5back",
        "R^{2}_{cut} = 1.44, N_{IO} = 128, rMergeCut = 1.5"},
    
    {"Seeds2_r2Cut1p44_maxObj256_1p5back",
        "R^{2}_{cut} = 1.44, N_{IO} = 256, rMergeCut = 1.5"},

    {"Seeds2_r2Cut0p81_maxObj128_2p0back",
        "R^{2}_{cut} = 0.81, N_{IO} = 128, rMergeCut = 2.0"},
    
    {"Seeds2_r2Cut0p81_maxObj256_2p0back",
        "R^{2}_{cut} = 0.81, N_{IO} = 256, rMergeCut = 2.0"},

    {"Seeds2_r2Cut1p0_maxObj128_2p0back",
        "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut = 2.0"},
    
    {"Seeds2_r2Cut1p0_maxObj256_2p0back",
        "R^{2}_{cut} = 1.0, N_{IO} = 256, rMergeCut = 2.0"},

    {"Seeds2_r2Cut1p44_maxObj128_2p0back",
        "R^{2}_{cut} = 1.44, N_{IO} = 128, rMergeCut = 2.0"},
    
    {"Seeds2_r2Cut1p44_maxObj256_2p0back",
        "R^{2}_{cut} = 1.44, N_{IO} = 256, rMergeCut = 2.0"},

    {"Seeds2_r2Cut0p81_maxObj128_2p5back",
        "R^{2}_{cut} = 0.81, N_{IO} = 128, rMergeCut = 2.5"},
    
    {"Seeds2_r2Cut0p81_maxObj256_2p5back",
        "R^{2}_{cut} = 0.81, N_{IO} = 256, rMergeCut = 2.5"},

    {"Seeds2_r2Cut1p0_maxObj128_2p5back",
        "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut = 2.5"},
    
    {"Seeds2_r2Cut1p0_maxObj256_2p5back",
        "R^{2}_{cut} = 1.0, N_{IO} = 256, rMergeCut = 2.5"},

    {"Seeds2_r2Cut1p44_maxObj128_2p5back",
        "R^{2}_{cut} = 1.44, N_{IO} = 128, rMergeCut = 2.5"},
    
    {"Seeds2_r2Cut1p44_maxObj256_2p5back",
        "R^{2}_{cut} = 1.44, N_{IO} = 256, rMergeCut = 2.5"},

    {"Seeds2_r2Cut0p81_maxObj128_3p0back",
        "R^{2}_{cut} = 0.81, N_{IO} = 128, rMergeCut = 3.0"},
    
    {"Seeds2_r2Cut0p81_maxObj256_3p0back",
        "R^{2}_{cut} = 0.81, N_{IO} = 256, rMergeCut = 3.0"},

    {"Seeds2_r2Cut1p0_maxObj128_3p0back",
        "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut = 3.0"},
    
    {"Seeds2_r2Cut1p0_maxObj256_3p0back",
        "R^{2}_{cut} = 1.0, N_{IO} = 256, rMergeCut = 3.0"},

    {"Seeds2_r2Cut1p44_maxObj128_3p0back",
        "R^{2}_{cut} = 1.44, N_{IO} = 128, rMergeCut = 3.0"},
    
    {"Seeds2_r2Cut1p44_maxObj256_3p0back",
        "R^{2}_{cut} = 1.44, N_{IO} = 256, rMergeCut = 3.0"},

    {"Seeds2_r2Cut0p81_maxObj128_3p5back",
        "R^{2}_{cut} = 0.81, N_{IO} = 128, rMergeCut = 3.5"},
    
    {"Seeds2_r2Cut0p81_maxObj256_3p5back",
        "R^{2}_{cut} = 0.81, N_{IO} = 256, rMergeCut = 3.5"},

    {"Seeds2_r2Cut1p0_maxObj128_3p5back",
        "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut = 3.5"},
    
    {"Seeds2_r2Cut1p0_maxObj256_3p5back",
        "R^{2}_{cut} = 1.0, N_{IO} = 256, rMergeCut = 3.5"},

    {"Seeds2_r2Cut1p44_maxObj128_3p5back",
        "R^{2}_{cut} = 1.44, N_{IO} = 128, rMergeCut = 3.5"},
    
    {"Seeds2_r2Cut1p44_maxObj256_3p5back",
        "R^{2}_{cut} = 1.44, N_{IO} = 256, rMergeCut = 3.5"},

    {"Seeds2_r2Cut0p81_maxObj128_4p0back",
        "R^{2}_{cut} = 0.81, N_{IO} = 128, rMergeCut = 4.0"},
    
    {"Seeds2_r2Cut0p81_maxObj256_4p0back",
        "R^{2}_{cut} = 0.81, N_{IO} = 256, rMergeCut = 4.0"},

    {"Seeds2_r2Cut1p0_maxObj128_4p0back",
        "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut = 4.0"},
    
    {"Seeds2_r2Cut1p0_maxObj256_4p0back",
        "R^{2}_{cut} = 1.0, N_{IO} = 256, rMergeCut = 4.0"},

    {"Seeds2_r2Cut1p44_maxObj128_4p0back",
        "R^{2}_{cut} = 1.44, N_{IO} = 128, rMergeCut = 4.0"},
    
    {"Seeds2_r2Cut1p44_maxObj256_4p0back",
        "R^{2}_{cut} = 1.44, N_{IO} = 256, rMergeCut = 4.0"},

    {"Seeds2_r2Cut0p81_maxObj128_4p5back",
        "R^{2}_{cut} = 0.81, N_{IO} = 128, rMergeCut = 4.5"},
    
    {"Seeds2_r2Cut0p81_maxObj256_4p5back",
        "R^{2}_{cut} = 0.81, N_{IO} = 256, rMergeCut = 4.5"},

    {"Seeds2_r2Cut1p0_maxObj128_4p5back",
        "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut = 4.5"},
    
    {"Seeds2_r2Cut1p0_maxObj256_4p5back",
        "R^{2}_{cut} = 1.0, N_{IO} = 256, rMergeCut = 4.5"},

    {"Seeds2_r2Cut1p44_maxObj128_4p5back",
        "R^{2}_{cut} = 1.44, N_{IO} = 128, rMergeCut = 4.5"},
    
    {"Seeds2_r2Cut1p44_maxObj256_4p5back",
        "R^{2}_{cut} = 1.44, N_{IO} = 256, rMergeCut = 4.5"},

    {"Seeds2_r2Cut0p81_maxObj128_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 0.81, N_{IO} = 128, No IO E_{cut}"},

    {"Seeds2_r2Cut0p81_maxObj256_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 0.81, N_{IO} = 256, No IO E_{cut}"},

    {"Seeds2_r2Cut1p21_maxObj256_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 1.21, N_{IO} = 256, No IO E_{cut}"},

    {"Seeds2_r2Cut1p44_maxObj256_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 1.44, N_{IO} = 256, No IO E_{cut}"},

    {"Seeds2_r2Cut1p69_maxObj256_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 1.69, N_{IO} = 256, No IO E_{cut}"},

    {"Seeds2_r2Cut1p96_maxObj256_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 1.96, N_{IO} = 256, No IO E_{cut}"},

    {"Seeds2_r2Cut1p21_maxObj128_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 1.21, N_{IO} = 128, No IO E_{cut}"},

    {"Seeds2_r2Cut1p44_maxObj128_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 1.44, N_{IO} = 128, No IO E_{cut}"},

    {"Seeds2_r2Cut1p69_maxObj128_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 1.69, N_{IO} = 128, No IO E_{cut}"},

    {"Seeds2_r2Cut1p96_maxObj128_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 1.96, N_{IO} = 128, No IO E_{cut}"},

    {"Seeds2_r2Cut0p64_maxObj128_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 0.64, N_{IO} = 128, No IO E_{cut}"},

    {"Seeds2_r2Cut0p64_maxObj128_back_ecut_ecutVal4", 
    "R^{2}_{cut} = 0.64, N_{IO} = 128, IO E_{cut}"},

    {"Seeds2_r2Cut1p0_maxObj128_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 1.0, N_{IO} = 128, No IO E_{cut}"},

    {"Seeds2_r2Cut1p44_maxObj128_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 1.44, N_{IO} = 128, No IO E_{cut}"},

    {"Seeds2_r2Cut1p0_maxObj256_back_ecut_ecutVal4", 
        "R^{2}_{cut} = 1.0, N_{IO} = 256, IO E_{cut}"},
        
    {"Seeds2_r2Cut1p0_maxObj256_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 1.0, N_{IO} = 256, No IO E_{cut}"},

    {"Seeds2_r2Cut1p0_maxObj512_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 1.0, N_{IO} = 512, No IO E_{cut}"},

    {"Seeds2_r2Cut1p44_maxObj512_back_noecut_ecutVal4", 
    "R^{2}_{cut} = 1.44, N_{IO} = 512, No IO E_{cut}"}
};

void analyze_files(std::vector<std::string > signalRootFileNames, std::vector<std::string > backgroundRootFileNames, bool overlayThreeFiles) {
    SetPlotStyle();
    
    // Vectors to hold per-file histograms
    std::vector<TH1F*> sig_num100, sig_denom100;
    std::vector<TH1F*> sig_num250, sig_denom250;
    std::vector<TH1F*> sig_num400, sig_denom400;

    std::vector<TH1F*> back_num100, back_denom100;
    std::vector<TH1F*> back_num200, back_denom200;
    std::vector<TH1F*> back_num300, back_denom300;

    // Efficiency histograms per file (only keep these)
    std::vector<TH1F*> back_eff_100, back_eff_200, back_eff_300;
    std::vector<TH1F*> sig_eff_100,  sig_eff_250,  sig_eff_400;

    // ROC and efficiency curve declarations
    vector<vector<double> > roc_curve_points_x(backgroundRootFileNames.size());
    vector<vector<double> > roc_curve_points_y(backgroundRootFileNames.size());
    vector<vector<double> > roc_curve_points_x2(backgroundRootFileNames.size());
    vector<vector<double> > roc_curve_points_y2(backgroundRootFileNames.size());
    vector<vector<double> > maxSignalToBackgroundRatio(backgroundRootFileNames.size());
    vector<vector<double> > maxSignalToBackgroundRatioTPR0p1(backgroundRootFileNames.size());

    vector<vector<double> > maxSignalToBackgroundRatio2(backgroundRootFileNames.size());
    vector<vector<double> > maxSignalToBackgroundRatio2TPR0p1(backgroundRootFileNames.size());

    vector<vector<double> > efficiency_curve_points_x(backgroundRootFileNames.size());
    vector<vector<double> > efficiency_curve_points_y(backgroundRootFileNames.size());
    vector<vector<double> > efficiency_curve_points_y2(backgroundRootFileNames.size());
    vector<double > tprMinMaxCut(backgroundRootFileNames.size());
    vector<double > fprMinMaxCut(backgroundRootFileNames.size());
    std::cout << "Processing : " << backgroundRootFileNames.size() << " files. " << "\n";
    std::vector<std::string > algorithmConfigurations;
    for (int fileIt = 0; fileIt < backgroundRootFileNames.size(); ++fileIt){
        unsigned int nInputObjectsAlgorithmConfiguration;
        std::regex re("_IOs_(\\d+)_");
        std::smatch match;

        if (std::regex_search(signalRootFileNames[fileIt], match, re)) {
            nInputObjectsAlgorithmConfiguration = std::stoi(match[1]);  // Convert to integer
            std::cout << "Extracted number of input objects for this algorithm configuration: " << nInputObjectsAlgorithmConfiguration << std::endl;
        } else {
            std::cout << "No match found." << std::endl;
        }

        // Open input ROOT file
        TFile* signalInputFile = TFile::Open(signalRootFileNames[fileIt].c_str(), "READ");
        TFile* backgroundInputFile = TFile::Open(backgroundRootFileNames[fileIt].c_str(), "READ");
        if ((!signalInputFile || signalInputFile->IsZombie()) || (!backgroundInputFile || backgroundInputFile->IsZombie())) {
            std::cerr << "Error: Could not open file " << signalRootFileNames[fileIt] << std::endl;
            return;
        }
        TTree* jetTaggerLRJsSignal = (TTree*)signalInputFile->Get("jetTaggerLRJsTree");
        TTree* jetTaggerLeadingLRJsSignal = (TTree*)signalInputFile->Get("jetTaggerLeadingLRJsTree");
        TTree* jetTaggerSubleadingLRJsSignal = (TTree*)signalInputFile->Get("jetTaggerSubleadingLRJsTree");
        TTree* eventInfoTreeSignal = (TTree*)signalInputFile->Get("eventInfoTree");
        TTree* truthbTreeSignal = (TTree*)signalInputFile->Get("truthbTree");
        TTree* truthHiggsTreeSignal = (TTree*)signalInputFile->Get("truthHiggsTree");
        TTree* caloTopoTowerTreeSignal = (TTree*)signalInputFile->Get("caloTopoTowerTree");
        TTree* topo422TreeSignal = (TTree*)signalInputFile->Get("topo422Tree");
        TTree* gepBasicClustersTreeSignal = (TTree*)signalInputFile->Get("gepBasicClustersTree");
        TTree* gFexSRJTreeSignal = (TTree*)signalInputFile->Get("gFexSRJTree");
        TTree* gFexLeadingSRJTreeSignal = (TTree*)signalInputFile->Get("gFexLeadingSRJTree");
        TTree* gFexSubleadingSRJTreeSignal = (TTree*)signalInputFile->Get("gFexSubleadingSRJTree");
        TTree* gFexLRJTreeSignal = (TTree*)signalInputFile->Get("gFexLRJTree");
        TTree* gFexLeadingLRJTreeSignal = (TTree*)signalInputFile->Get("gFexLeadingLRJTree");
        TTree* gFexSubleadingLRJTreeSignal = (TTree*)signalInputFile->Get("gFexSubleadingLRJTree");
        TTree* inTimeAntiKt4TruthJetsTreeSignal = (TTree*)signalInputFile->Get("inTimeAntiKt4TruthJetsTree");
        TTree* leadingInTimeAntiKt4TruthJetsTreeSignal = (TTree*)signalInputFile->Get("leadingInTimeAntiKt4TruthJetsTree");
        TTree* subleadingInTimeAntiKt4TruthJetsTreeSignal = (TTree*)signalInputFile->Get("subleadingInTimeAntiKt4TruthJetsTree");
        TTree* jFexSRJTreeSignal = (TTree*)signalInputFile->Get("jFexSRJTree");
        TTree* jFexLeadingSRJTreeSignal = (TTree*)signalInputFile->Get("jFexLeadingSRJTree");
        TTree* jFexSubleadingSRJTreeSignal = (TTree*)signalInputFile->Get("jFexSubleadingSRJTree");
        TTree* jFexLRJTreeSignal = (TTree*)signalInputFile->Get("jFexLRJTree");
        TTree* jFexLeadingLRJTreeSignal = (TTree*)signalInputFile->Get("jFexLeadingLRJTree");
        TTree* jFexSubleadingLRJTreeSignal = (TTree*)signalInputFile->Get("jFexSubleadingLRJTree");
        TTree* hltAntiKt4EMTopoJetsTreeSignal = (TTree*)signalInputFile->Get("hltAntiKt4EMTopoJetsTree");
        TTree* leadingHltAntiKt4EMTopoJetsTreeSignal = (TTree*)signalInputFile->Get("leadingHltAntiKt4EMTopoJetsTree");
        TTree* subleadingHltAntiKt4EMTopoJetsTreeSignal = (TTree*)signalInputFile->Get("subleadingHltAntiKt4EMTopoJetsTree");
        TTree* recoAntiKt10UFOCSSKJetsSignal = (TTree*)signalInputFile->Get("recoAntiKt10UFOCSSKJets");
        TTree* leadingRecoAntiKt10UFOCSSKJetsSignal = (TTree*)signalInputFile->Get("leadingRecoAntiKt10UFOCSSKJets");
        TTree* subleadingRecoAntiKt10UFOCSSKJetsSignal = (TTree*)signalInputFile->Get("subleadingRecoAntiKt10UFOCSSKJets");
        TTree* truthAntiKt4TruthDressedWZJetsSignal = (TTree*)signalInputFile->Get("truthAntiKt4TruthDressedWZJets");
        TTree* leadingTruthAntiKt4TruthDressedWZJetsSignal = (TTree*)signalInputFile->Get("leadingTruthAntiKt4TruthDressedWZJets");
        TTree* subleadingTruthAntiKt4TruthDressedWZJetsSignal = (TTree*)signalInputFile->Get("subleadingTruthAntiKt4TruthDressedWZJets");

        TTree* jetTaggerLRJsBack = (TTree*)backgroundInputFile->Get("jetTaggerLRJsTree");
        TTree* jetTaggerLeadingLRJsBack = (TTree*)backgroundInputFile->Get("jetTaggerLeadingLRJsTree");
        TTree* jetTaggerSubleadingLRJsBack = (TTree*)backgroundInputFile->Get("jetTaggerSubleadingLRJsTree");
        TTree* eventInfoTreeBack = (TTree*)backgroundInputFile->Get("eventInfoTree");
        TTree* caloTopoTowerTreeBack = (TTree*)backgroundInputFile->Get("caloTopoTowerTree");
        TTree* topo422TreeBack = (TTree*)backgroundInputFile->Get("topo422Tree");
        TTree* gepBasicClustersTreeBack = (TTree*)backgroundInputFile->Get("gepBasicClustersTree");
        TTree* gFexSRJTreeBack = (TTree*)backgroundInputFile->Get("gFexSRJTree");
        TTree* gFexLeadingSRJTreeBack = (TTree*)backgroundInputFile->Get("gFexLeadingSRJTree");
        TTree* gFexSubleadingSRJTreeBack = (TTree*)backgroundInputFile->Get("gFexSubleadingSRJTree");
        TTree* gFexLRJTreeBack = (TTree*)backgroundInputFile->Get("gFexLRJTree");
        TTree* gFexLeadingLRJTreeBack = (TTree*)backgroundInputFile->Get("gFexLeadingLRJTree");
        TTree* gFexSubleadingLRJTreeBack = (TTree*)backgroundInputFile->Get("gFexSubleadingLRJTree");
        TTree* inTimeAntiKt4TruthJetsTreeBack = (TTree*)backgroundInputFile->Get("inTimeAntiKt4TruthJetsTree");
        TTree* leadingInTimeAntiKt4TruthJetsTreeBack = (TTree*)backgroundInputFile->Get("leadingInTimeAntiKt4TruthJetsTree");
        TTree* subleadingInTimeAntiKt4TruthJetsTreeBack = (TTree*)backgroundInputFile->Get("subleadingInTimeAntiKt4TruthJetsTree");
        TTree* jFexSRJTreeBack = (TTree*)backgroundInputFile->Get("jFexSRJTree");
        TTree* jFexLeadingSRJTreeBack = (TTree*)backgroundInputFile->Get("jFexLeadingSRJTree");
        TTree* jFexSubleadingSRJTreeBack = (TTree*)backgroundInputFile->Get("jFexSubleadingSRJTree");
        TTree* jFexLRJTreeBack = (TTree*)backgroundInputFile->Get("jFexLRJTree");
        TTree* jFexLeadingLRJTreeBack = (TTree*)backgroundInputFile->Get("jFexLeadingLRJTree");
        TTree* jFexSubleadingLRJTreeBack = (TTree*)backgroundInputFile->Get("jFexSubleadingLRJTree");
        TTree* hltAntiKt4EMTopoJetsTreeBack = (TTree*)backgroundInputFile->Get("hltAntiKt4EMTopoJetsTree");
        TTree* leadingHltAntiKt4EMTopoJetsTreeBack = (TTree*)backgroundInputFile->Get("leadingHltAntiKt4EMTopoJetsTree");
        TTree* subleadingHltAntiKt4EMTopoJetsTreeBack = (TTree*)backgroundInputFile->Get("subleadingHltAntiKt4EMTopoJetsTree");
        TTree* recoAntiKt10UFOCSSKJetsBack = (TTree*)backgroundInputFile->Get("recoAntiKt10UFOCSSKJets");
        TTree* leadingRecoAntiKt10UFOCSSKJetsBack = (TTree*)backgroundInputFile->Get("leadingRecoAntiKt10UFOCSSKJets");
        TTree* subleadingRecoAntiKt10UFOCSSKJetsBack = (TTree*)backgroundInputFile->Get("subleadingRecoAntiKt10UFOCSSKJets");
        TTree* truthAntiKt4TruthDressedWZJetsBack = (TTree*)backgroundInputFile->Get("truthAntiKt4TruthDressedWZJets");
        TTree* leadingTruthAntiKt4TruthDressedWZJetsBack = (TTree*)backgroundInputFile->Get("leadingTruthAntiKt4TruthDressedWZJets");
        TTree* subleadingTruthAntiKt4TruthDressedWZJetsBack = (TTree*)backgroundInputFile->Get("subleadingTruthAntiKt4TruthDressedWZJets");

        std::vector<double>* mcEventWeightsValuesSignal = nullptr;
        double* sumOfWeightsForSampleValuesSignal = nullptr;
        std::vector<double>* eventWeightsValuesSignal = nullptr;
        int* sampleJZSliceValuesSignal = nullptr;
        std::vector<double>* jetTaggerLRJDiamValuesSignal = nullptr;
        std::vector<unsigned int>* jetTaggerLRJMergedIndicesValuesSignal = nullptr;
        std::vector<double>* jetTaggerLRJEtValuesSignal = nullptr;
        std::vector<double>* jetTaggerLRJEtaValuesSignal = nullptr;
        std::vector<double>* jetTaggerLRJPhiValuesSignal = nullptr;
        std::vector<double>* jetTaggerLeadingLRJDiamValuesSignal = nullptr;
        std::vector<unsigned int>* jetTaggerLeadingLRJMergedIndicesValuesSignal = nullptr;
        std::vector<double>* jetTaggerLeadingLRJEtValuesSignal = nullptr;
        std::vector<double>* jetTaggerLeadingLRJEtaValuesSignal = nullptr;
        std::vector<double>* jetTaggerLeadingLRJPhiValuesSignal = nullptr;
        std::vector<double>* jetTaggerSubleadingLRJDiamValuesSignal = nullptr;
        std::vector<unsigned int>* jetTaggerSubleadingLRJMergedIndicesValuesSignal = nullptr;
        std::vector<double>* jetTaggerSubleadingLRJEtValuesSignal = nullptr;
        std::vector<double>* jetTaggerSubleadingLRJEtaValuesSignal = nullptr;
        std::vector<double>* jetTaggerSubleadingLRJPhiValuesSignal = nullptr;
        std::vector<unsigned int>* higgsIndexValuesSignal = nullptr;
        std::vector<unsigned int>* indexOfHiggsValuesSignal = nullptr;
        std::vector<double>* truthbquarksEtValuesSignal = nullptr;
        std::vector<double>* truthbquarksEnergyValuesSignal = nullptr;
        std::vector<double>* truthbquarkspTValuesSignal = nullptr;
        std::vector<double>* truthbquarkspxValuesSignal = nullptr;
        std::vector<double>* truthbquarkspyValuesSignal = nullptr;
        std::vector<double>* truthbquarkspzValuesSignal = nullptr;
        std::vector<double>* truthbquarksEtaValuesSignal = nullptr;
        std::vector<double>* truthbquarksPhiValuesSignal = nullptr;
        std::vector<double>* truthHiggsEtValuesSignal = nullptr;
        std::vector<double>* truthHiggsEnergyValuesSignal = nullptr;
        std::vector<double>* truthHiggspTValuesSignal = nullptr;
        std::vector<double>* truthHiggspxValuesSignal = nullptr;
        std::vector<double>* truthHiggspyValuesSignal = nullptr;
        std::vector<double>* truthHiggspzValuesSignal = nullptr;
        std::vector<double>* truthHiggsEtaValuesSignal = nullptr;
        std::vector<double>* truthHiggsPhiValuesSignal = nullptr;
        std::vector<double>* truthHiggsInvMassValuesSignal = nullptr;
        std::vector<double>* caloTopoTowerEtValuesSignal = nullptr;
        std::vector<double>* caloTopoTowerEtaValuesSignal = nullptr;
        std::vector<double>* caloTopoTowerPhiValuesSignal = nullptr;
        std::vector<double>* topo422EtValuesSignal = nullptr;
        std::vector<double>* topo422EtaValuesSignal = nullptr;
        std::vector<double>* topo422PhiValuesSignal = nullptr;
        std::vector<double>* gepBasicClustersEtValuesSignal = nullptr;
        std::vector<double>* gepBasicClustersEtaValuesSignal = nullptr;
        std::vector<double>* gepBasicClustersPhiValuesSignal = nullptr;
        std::vector<unsigned int>* gFexSRJEtIndexValuesSignal = nullptr;
        std::vector<double>* gFexSRJEtValuesSignal = nullptr;
        std::vector<double>* gFexSRJEtaValuesSignal = nullptr;
        std::vector<double>* gFexSRJPhiValuesSignal = nullptr;
        std::vector<double>* gFexSRJLeadingEtValuesSignal = nullptr;
        std::vector<double>* gFexSRJLeadingEtaValuesSignal = nullptr;
        std::vector<double>* gFexSRJLeadingPhiValuesSignal = nullptr;
        std::vector<double>* gFexSRJSubleadingEtValuesSignal = nullptr;
        std::vector<double>* gFexSRJSubleadingEtaValuesSignal = nullptr;
        std::vector<double>* gFexSRJSubleadingPhiValuesSignal = nullptr;
        std::vector<unsigned int>* gFexLRJEtIndexValuesSignal = nullptr;
        std::vector<double>* gFexLRJEtValuesSignal = nullptr;
        std::vector<double>* gFexLRJEtaValuesSignal = nullptr;
        std::vector<double>* gFexLRJPhiValuesSignal = nullptr;
        std::vector<double>* gFexLRJLeadingEtValuesSignal = nullptr;
        std::vector<double>* gFexLRJLeadingEtaValuesSignal = nullptr;
        std::vector<double>* gFexLRJLeadingPhiValuesSignal = nullptr;
        std::vector<double>* gFexLRJSubleadingEtValuesSignal = nullptr;
        std::vector<double>* gFexLRJSubleadingEtaValuesSignal = nullptr;
        std::vector<double>* gFexLRJSubleadingPhiValuesSignal = nullptr;
        std::vector<unsigned int>* jFexSRJEtIndexValuesSignal = nullptr;
        std::vector<double>* jFexSRJEtValuesSignal = nullptr;
        std::vector<double>* jFexSRJEtaValuesSignal = nullptr;
        std::vector<double>* jFexSRJPhiValuesSignal = nullptr;
        std::vector<double>* jFexSRJLeadingEtValuesSignal = nullptr;
        std::vector<double>* jFexSRJLeadingEtaValuesSignal = nullptr;
        std::vector<double>* jFexSRJLeadingPhiValuesSignal = nullptr;
        std::vector<double>* jFexSRJSubleadingEtValuesSignal = nullptr;
        std::vector<double>* jFexSRJSubleadingEtaValuesSignal = nullptr;
        std::vector<double>* jFexSRJSubleadingPhiValuesSignal = nullptr;
        std::vector<unsigned int>* jFexLRJEtIndexValuesSignal = nullptr;
        std::vector<double>* jFexLRJEtValuesSignal = nullptr;
        std::vector<double>* jFexLRJEtaValuesSignal = nullptr;
        std::vector<double>* jFexLRJPhiValuesSignal = nullptr;
        std::vector<double>* jFexLRJLeadingEtValuesSignal = nullptr;
        std::vector<double>* jFexLRJLeadingEtaValuesSignal = nullptr;
        std::vector<double>* jFexLRJLeadingPhiValuesSignal = nullptr;
        std::vector<double>* jFexLRJSubleadingEtValuesSignal = nullptr;
        std::vector<double>* jFexLRJSubleadingEtaValuesSignal = nullptr;
        std::vector<double>* jFexLRJSubleadingPhiValuesSignal = nullptr;
        std::vector<unsigned int>* hltAntiKt4SRJEtIndexValuesSignal = nullptr;
        std::vector<double>* hltAntiKt4SRJEtValuesSignal = nullptr;
        std::vector<double>* hltAntiKt4SRJEtaValuesSignal = nullptr;
        std::vector<double>* hltAntiKt4SRJPhiValuesSignal = nullptr;
        std::vector<double>* hltAntiKt4SRJLeadingEtValuesSignal = nullptr;
        std::vector<double>* hltAntiKt4SRJLeadingEtaValuesSignal = nullptr;
        std::vector<double>* hltAntiKt4SRJLeadingPhiValuesSignal = nullptr;
        std::vector<double>* hltAntiKt4SRJSubleadingEtValuesSignal = nullptr;
        std::vector<double>* hltAntiKt4SRJSubleadingEtaValuesSignal = nullptr;
        std::vector<double>* hltAntiKt4SRJSubleadingPhiValuesSignal = nullptr;
        std::vector<unsigned int>* recoAntiKt10LRJEtIndexValuesSignal = nullptr;
        std::vector<double>* recoAntiKt10LRJEtValuesSignal = nullptr;
        std::vector<double>* recoAntiKt10LRJEtaValuesSignal = nullptr;
        std::vector<double>* recoAntiKt10LRJPhiValuesSignal = nullptr;
        std::vector<double>* recoAntiKt10LRJMassValuesSignal = nullptr;
        std::vector<double>* recoAntiKt10LRJLeadingEtValuesSignal = nullptr;
        std::vector<double>* recoAntiKt10LRJLeadingEtaValuesSignal = nullptr;
        std::vector<double>* recoAntiKt10LRJLeadingPhiValuesSignal = nullptr;
        std::vector<double>* recoAntiKt10LRJLeadingMassValuesSignal = nullptr;
        std::vector<double>* recoAntiKt10LRJSubleadingEtValuesSignal = nullptr;
        std::vector<double>* recoAntiKt10LRJSubleadingEtaValuesSignal = nullptr;
        std::vector<double>* recoAntiKt10LRJSubleadingPhiValuesSignal = nullptr;
        std::vector<double>* recoAntiKt10LRJSubleadingMassValuesSignal = nullptr;
        std::vector<unsigned int>* truthAntiKt4WZSRJEtIndexValuesSignal = nullptr;
        std::vector<double>* truthAntiKt4WZSRJEtValuesSignal = nullptr;
        std::vector<double>* truthAntiKt4WZSRJEtaValuesSignal = nullptr;
        std::vector<double>* truthAntiKt4WZSRJPhiValuesSignal = nullptr;
        std::vector<double>* truthAntiKt4WZSRJMassValuesSignal = nullptr;
        std::vector<double>* truthAntiKt4WZSRJLeadingEtValuesSignal = nullptr;
        std::vector<double>* truthAntiKt4WZSRJLeadingEtaValuesSignal = nullptr;
        std::vector<double>* truthAntiKt4WZSRJLeadingPhiValuesSignal = nullptr;
        std::vector<double>* truthAntiKt4WZSRJLeadingMassValuesSignal = nullptr;
        std::vector<double>* truthAntiKt4WZSRJSubleadingEtValuesSignal = nullptr;
        std::vector<double>* truthAntiKt4WZSRJSubleadingEtaValuesSignal = nullptr;
        std::vector<double>* truthAntiKt4WZSRJSubleadingPhiValuesSignal = nullptr;
        std::vector<double>* truthAntiKt4WZSRJSubleadingMassValuesSignal = nullptr;
        std::vector<unsigned int>* inTimeAntiKt4TruthSRJEtIndexValuesSignal = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJEtValuesSignal = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJEtaValuesSignal = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJPhiValuesSignal = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJLeadingEtValuesSignal = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJLeadingEtaValuesSignal = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJLeadingPhiValuesSignal = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJSubleadingEtValuesSignal = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJSubleadingEtaValuesSignal = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJSubleadingPhiValuesSignal = nullptr;

        std::vector<unsigned int>* higgsIndexValuesBack = nullptr;
        std::vector<unsigned int>* indexOfHiggsValuesBack = nullptr;
        std::vector<double>* truthbquarksEtValuesBack = nullptr;
        std::vector<double>* truthbquarksEnergyValuesBack = nullptr;
        std::vector<double>* truthbquarkspTValuesBack = nullptr;
        std::vector<double>* truthbquarkspxValuesBack = nullptr;
        std::vector<double>* truthbquarkspyValuesBack = nullptr;
        std::vector<double>* truthbquarkspzValuesBack = nullptr;
        std::vector<double>* truthbquarksEtaValuesBack = nullptr;
        std::vector<double>* truthbquarksPhiValuesBack = nullptr;
        std::vector<double>* truthHiggsEtValuesBack = nullptr;
        std::vector<double>* truthHiggsEnergyValuesBack = nullptr;
        std::vector<double>* truthHiggspTValuesBack = nullptr;
        std::vector<double>* truthHiggspxValuesBack = nullptr;
        std::vector<double>* truthHiggspyValuesBack = nullptr;
        std::vector<double>* truthHiggspzValuesBack = nullptr;
        std::vector<double>* truthHiggsEtaValuesBack = nullptr;
        std::vector<double>* truthHiggsPhiValuesBack = nullptr;
        std::vector<double>* truthHiggsInvMassValuesBack = nullptr;
        std::vector<double>* mcEventWeightsValuesBack = nullptr;
        double sumOfWeightsForSampleValuesBack = 0.0;
        std::vector<double>* eventWeightsValuesBack = nullptr;
        int sampleJZSliceValuesBack = -1;
        std::vector<double>* jetTaggerLRJDiamValuesBack = nullptr;
        std::vector<unsigned int>* jetTaggerLRJMergedIndicesValuesBack = nullptr;
        std::vector<double>* jetTaggerLRJEtValuesBack = nullptr;
        std::vector<double>* jetTaggerLRJEtaValuesBack = nullptr;
        std::vector<double>* jetTaggerLRJPhiValuesBack = nullptr;
        std::vector<double>* jetTaggerLeadingLRJDiamValuesBack = nullptr;
        std::vector<double>* jetTaggerLeadingLRJMergedIndicesValuesBack = nullptr;
        std::vector<double>* jetTaggerLeadingLRJEtValuesBack = nullptr;
        std::vector<double>* jetTaggerLeadingLRJEtaValuesBack = nullptr;
        std::vector<double>* jetTaggerLeadingLRJPhiValuesBack = nullptr;
        std::vector<double>* jetTaggerSubleadingLRJDiamValuesBack = nullptr;
        std::vector<double>* jetTaggerSubleadingLRJMergedIndicesValuesBack = nullptr;
        std::vector<double>* jetTaggerSubleadingLRJEtValuesBack = nullptr;
        std::vector<double>* jetTaggerSubleadingLRJEtaValuesBack = nullptr;
        std::vector<double>* jetTaggerSubleadingLRJPhiValuesBack = nullptr;
        std::vector<double>* caloTopoTowerEtValuesBack = nullptr;
        std::vector<double>* caloTopoTowerEtaValuesBack = nullptr;
        std::vector<double>* caloTopoTowerPhiValuesBack = nullptr;
        std::vector<double>* topo422EtValuesBack = nullptr;
        std::vector<double>* topo422EtaValuesBack = nullptr;
        std::vector<double>* topo422PhiValuesBack = nullptr;
        std::vector<double>* gepBasicClustersEtValuesBack = nullptr;
        std::vector<double>* gepBasicClustersEtaValuesBack = nullptr;
        std::vector<double>* gepBasicClustersPhiValuesBack = nullptr;
        std::vector<unsigned int>* gFexSRJEtIndexValuesBack = nullptr;
        std::vector<double>* gFexSRJEtValuesBack = nullptr;
        std::vector<double>* gFexSRJEtaValuesBack = nullptr;
        std::vector<double>* gFexSRJPhiValuesBack = nullptr;
        std::vector<double>* gFexSRJLeadingEtValuesBack = nullptr;
        std::vector<double>* gFexSRJLeadingEtaValuesBack = nullptr;
        std::vector<double>* gFexSRJLeadingPhiValuesBack = nullptr;
        std::vector<double>* gFexSRJSubleadingEtValuesBack = nullptr;
        std::vector<double>* gFexSRJSubleadingEtaValuesBack = nullptr;
        std::vector<double>* gFexSRJSubleadingPhiValuesBack = nullptr;
        std::vector<unsigned int>* gFexLRJEtIndexValuesBack = nullptr;
        std::vector<double>* gFexLRJEtValuesBack = nullptr;
        std::vector<double>* gFexLRJEtaValuesBack = nullptr;
        std::vector<double>* gFexLRJPhiValuesBack = nullptr;
        std::vector<double>* gFexLRJLeadingEtValuesBack = nullptr;
        std::vector<double>* gFexLRJLeadingEtaValuesBack = nullptr;
        std::vector<double>* gFexLRJLeadingPhiValuesBack = nullptr;
        std::vector<double>* gFexLRJSubleadingEtValuesBack = nullptr;
        std::vector<double>* gFexLRJSubleadingEtaValuesBack = nullptr;
        std::vector<double>* gFexLRJSubleadingPhiValuesBack = nullptr;
        std::vector<unsigned int>* jFexSRJEtIndexValuesBack = nullptr;
        std::vector<double>* jFexSRJEtValuesBack = nullptr;
        std::vector<double>* jFexSRJEtaValuesBack = nullptr;
        std::vector<double>* jFexSRJPhiValuesBack = nullptr;
        std::vector<double>* jFexSRJLeadingEtValuesBack = nullptr;
        std::vector<double>* jFexSRJLeadingEtaValuesBack = nullptr;
        std::vector<double>* jFexSRJLeadingPhiValuesBack = nullptr;
        std::vector<double>* jFexSRJSubleadingEtValuesBack = nullptr;
        std::vector<double>* jFexSRJSubleadingEtaValuesBack = nullptr;
        std::vector<double>* jFexSRJSubleadingPhiValuesBack = nullptr;
        std::vector<unsigned int>* jFexLRJEtIndexValuesBack = nullptr;
        std::vector<double>* jFexLRJEtValuesBack = nullptr;
        std::vector<double>* jFexLRJEtaValuesBack = nullptr;
        std::vector<double>* jFexLRJPhiValuesBack = nullptr;
        std::vector<double>* jFexLRJLeadingEtValuesBack = nullptr;
        std::vector<double>* jFexLRJLeadingEtaValuesBack = nullptr;
        std::vector<double>* jFexLRJLeadingPhiValuesBack = nullptr;
        std::vector<double>* jFexLRJSubleadingEtValuesBack = nullptr;
        std::vector<double>* jFexLRJSubleadingEtaValuesBack = nullptr;
        std::vector<double>* jFexLRJSubleadingPhiValuesBack = nullptr;
        std::vector<unsigned int>* hltAntiKt4SRJEtIndexValuesBack = nullptr;
        std::vector<double>* hltAntiKt4SRJEtValuesBack = nullptr;
        std::vector<double>* hltAntiKt4SRJEtaValuesBack = nullptr;
        std::vector<double>* hltAntiKt4SRJPhiValuesBack = nullptr;
        std::vector<double>* hltAntiKt4SRJLeadingEtValuesBack = nullptr;
        std::vector<double>* hltAntiKt4SRJLeadingEtaValuesBack = nullptr;
        std::vector<double>* hltAntiKt4SRJLeadingPhiValuesBack = nullptr;
        std::vector<double>* hltAntiKt4SRJSubleadingEtValuesBack = nullptr;
        std::vector<double>* hltAntiKt4SRJSubleadingEtaValuesBack = nullptr;
        std::vector<double>* hltAntiKt4SRJSubleadingPhiValuesBack = nullptr;
        std::vector<unsigned int>* recoAntiKt10LRJEtIndexValuesBack = nullptr;
        std::vector<double>* recoAntiKt10LRJEtValuesBack = nullptr;
        std::vector<double>* recoAntiKt10LRJEtaValuesBack = nullptr;
        std::vector<double>* recoAntiKt10LRJPhiValuesBack = nullptr;
        std::vector<double>* recoAntiKt10LRJMassValuesBack = nullptr;
        std::vector<double>* recoAntiKt10LRJLeadingEtValuesBack = nullptr;
        std::vector<double>* recoAntiKt10LRJLeadingEtaValuesBack = nullptr;
        std::vector<double>* recoAntiKt10LRJLeadingPhiValuesBack = nullptr;
        std::vector<double>* recoAntiKt10LRJLeadingMassValuesBack = nullptr;
        std::vector<double>* recoAntiKt10LRJSubleadingEtValuesBack = nullptr;
        std::vector<double>* recoAntiKt10LRJSubleadingEtaValuesBack = nullptr;
        std::vector<double>* recoAntiKt10LRJSubleadingPhiValuesBack = nullptr;
        std::vector<double>* recoAntiKt10LRJSubleadingMassValuesBack = nullptr;
        std::vector<unsigned int>* truthAntiKt4WZSRJEtIndexValuesBack = nullptr;
        std::vector<double>* truthAntiKt4WZSRJEtValuesBack = nullptr;
        std::vector<double>* truthAntiKt4WZSRJEtaValuesBack = nullptr;
        std::vector<double>* truthAntiKt4WZSRJPhiValuesBack = nullptr;
        std::vector<double>* truthAntiKt4WZSRJLeadingEtValuesBack = nullptr;
        std::vector<double>* truthAntiKt4WZSRJLeadingEtaValuesBack = nullptr;
        std::vector<double>* truthAntiKt4WZSRJLeadingPhiValuesBack = nullptr;
        std::vector<double>* truthAntiKt4WZSRJSubleadingEtValuesBack = nullptr;
        std::vector<double>* truthAntiKt4WZSRJSubleadingEtaValuesBack = nullptr;
        std::vector<double>* truthAntiKt4WZSRJSubleadingPhiValuesBack = nullptr;
        std::vector<unsigned int>* inTimeAntiKt4TruthSRJEtIndexValuesBack = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJEtValuesBack = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJEtaValuesBack = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJPhiValuesBack = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJLeadingEtValuesBack = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJLeadingEtaValuesBack = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJLeadingPhiValuesBack = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJSubleadingEtValuesBack = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJSubleadingEtaValuesBack = nullptr;
        std::vector<double>* inTimeAntiKt4TruthSRJSubleadingPhiValuesBack = nullptr;

        // === eventInfoTreeSignal ===
        eventInfoTreeSignal->SetBranchAddress("mcEventWeight", &mcEventWeightsValuesSignal);
        eventInfoTreeSignal->SetBranchAddress("sumOfWeightsForSample", &sumOfWeightsForSampleValuesSignal);
        eventInfoTreeSignal->SetBranchAddress("eventWeights", &eventWeightsValuesSignal);
        eventInfoTreeSignal->SetBranchAddress("sampleJZSlice", &sampleJZSliceValuesSignal);

        // === jetTaggerLRJsSignal ===
        jetTaggerLRJsSignal->SetBranchAddress("Diam", &jetTaggerLRJDiamValuesSignal);
        jetTaggerLRJsSignal->SetBranchAddress("MergedIndices", &jetTaggerLRJMergedIndicesValuesSignal);
        jetTaggerLRJsSignal->SetBranchAddress("Et", &jetTaggerLRJEtValuesSignal);
        jetTaggerLRJsSignal->SetBranchAddress("Eta", &jetTaggerLRJEtaValuesSignal);
        jetTaggerLRJsSignal->SetBranchAddress("Phi", &jetTaggerLRJPhiValuesSignal);

        // === jetTaggerLeadingLRJsSignal ===
        jetTaggerLeadingLRJsSignal->SetBranchAddress("Diam", &jetTaggerLeadingLRJDiamValuesSignal);
        jetTaggerLeadingLRJsSignal->SetBranchAddress("MergedIndices", &jetTaggerLeadingLRJMergedIndicesValuesSignal);
        jetTaggerLeadingLRJsSignal->SetBranchAddress("Et", &jetTaggerLeadingLRJEtValuesSignal);
        jetTaggerLeadingLRJsSignal->SetBranchAddress("Eta", &jetTaggerLeadingLRJEtaValuesSignal);
        jetTaggerLeadingLRJsSignal->SetBranchAddress("Phi", &jetTaggerLeadingLRJPhiValuesSignal);

        // === jetTaggerSubleadingLRJsSignal ===
        jetTaggerSubleadingLRJsSignal->SetBranchAddress("Diam", &jetTaggerSubleadingLRJDiamValuesSignal);
        jetTaggerSubleadingLRJsSignal->SetBranchAddress("MergedIndices", &jetTaggerSubleadingLRJMergedIndicesValuesSignal);
        jetTaggerSubleadingLRJsSignal->SetBranchAddress("Et", &jetTaggerSubleadingLRJEtValuesSignal);
        jetTaggerSubleadingLRJsSignal->SetBranchAddress("Eta", &jetTaggerSubleadingLRJEtaValuesSignal);
        jetTaggerSubleadingLRJsSignal->SetBranchAddress("Phi", &jetTaggerSubleadingLRJPhiValuesSignal);

        // === truthbTreeSignal ===
        truthbTreeSignal->SetBranchAddress("higgsIndex", &higgsIndexValuesSignal);
        truthbTreeSignal->SetBranchAddress("Et", &truthbquarksEtValuesSignal);
        truthbTreeSignal->SetBranchAddress("Energy", &truthbquarksEnergyValuesSignal);
        truthbTreeSignal->SetBranchAddress("pT", &truthbquarkspTValuesSignal);
        truthbTreeSignal->SetBranchAddress("px", &truthbquarkspxValuesSignal);
        truthbTreeSignal->SetBranchAddress("py", &truthbquarkspyValuesSignal);
        truthbTreeSignal->SetBranchAddress("pz", &truthbquarkspzValuesSignal);
        truthbTreeSignal->SetBranchAddress("Eta", &truthbquarksEtaValuesSignal);
        truthbTreeSignal->SetBranchAddress("Phi", &truthbquarksPhiValuesSignal);

        // === truthHiggsTreeSignal ===
        truthHiggsTreeSignal->SetBranchAddress("indexOfHiggs", &indexOfHiggsValuesSignal);
        truthHiggsTreeSignal->SetBranchAddress("invMass", &truthHiggsInvMassValuesSignal);
        truthHiggsTreeSignal->SetBranchAddress("Et", &truthHiggsEtValuesSignal);
        truthHiggsTreeSignal->SetBranchAddress("Energy", &truthHiggsEnergyValuesSignal);
        truthHiggsTreeSignal->SetBranchAddress("pT", &truthHiggspTValuesSignal);
        truthHiggsTreeSignal->SetBranchAddress("px", &truthHiggspxValuesSignal);
        truthHiggsTreeSignal->SetBranchAddress("py", &truthHiggspyValuesSignal);
        truthHiggsTreeSignal->SetBranchAddress("pz", &truthHiggspzValuesSignal);
        truthHiggsTreeSignal->SetBranchAddress("Eta", &truthHiggsEtaValuesSignal);
        truthHiggsTreeSignal->SetBranchAddress("Phi", &truthHiggsPhiValuesSignal);

        // === caloTopoTowerTreeSignal ===
        caloTopoTowerTreeSignal->SetBranchAddress("Et", &caloTopoTowerEtValuesSignal);
        caloTopoTowerTreeSignal->SetBranchAddress("Eta", &caloTopoTowerEtaValuesSignal);
        caloTopoTowerTreeSignal->SetBranchAddress("Phi", &caloTopoTowerPhiValuesSignal);

        // === topo422TreeSignal ===
        topo422TreeSignal->SetBranchAddress("Et", &topo422EtValuesSignal);
        topo422TreeSignal->SetBranchAddress("Eta", &topo422EtaValuesSignal);
        topo422TreeSignal->SetBranchAddress("Phi", &topo422PhiValuesSignal);

        // === gepBasicClustersTree ===
        gepBasicClustersTreeSignal->SetBranchAddress("Et", &gepBasicClustersEtValuesSignal);
        gepBasicClustersTreeSignal->SetBranchAddress("Eta", &gepBasicClustersEtaValuesSignal);
        gepBasicClustersTreeSignal->SetBranchAddress("Phi", &gepBasicClustersPhiValuesSignal);

        // === gFexSRJTreeSignal ===
        gFexSRJTreeSignal->SetBranchAddress("EtIndex", &gFexSRJEtIndexValuesSignal);
        gFexSRJTreeSignal->SetBranchAddress("Et", &gFexSRJEtValuesSignal);
        gFexSRJTreeSignal->SetBranchAddress("Eta", &gFexSRJEtaValuesSignal);
        gFexSRJTreeSignal->SetBranchAddress("Phi", &gFexSRJPhiValuesSignal);

        // === gFexLeadingSRJTreeSignal ===
        gFexLeadingSRJTreeSignal->SetBranchAddress("Et", &gFexSRJLeadingEtValuesSignal);
        gFexLeadingSRJTreeSignal->SetBranchAddress("Eta", &gFexSRJLeadingEtaValuesSignal);
        gFexLeadingSRJTreeSignal->SetBranchAddress("Phi", &gFexSRJLeadingPhiValuesSignal);

        // === gFexSubleadingSRJTreeSignal ===
        gFexSubleadingSRJTreeSignal->SetBranchAddress("Et", &gFexSRJSubleadingEtValuesSignal);
        gFexSubleadingSRJTreeSignal->SetBranchAddress("Eta", &gFexSRJSubleadingEtaValuesSignal);
        gFexSubleadingSRJTreeSignal->SetBranchAddress("Phi", &gFexSRJSubleadingPhiValuesSignal);

        // === gFexLRJTreeSignal ===
        gFexLRJTreeSignal->SetBranchAddress("EtIndex", &gFexLRJEtIndexValuesSignal);
        gFexLRJTreeSignal->SetBranchAddress("Et", &gFexLRJEtValuesSignal);
        gFexLRJTreeSignal->SetBranchAddress("Eta", &gFexLRJEtaValuesSignal);
        gFexLRJTreeSignal->SetBranchAddress("Phi", &gFexLRJPhiValuesSignal);

        // === gFexLeadingLRJTreeSignal ===
        gFexLeadingLRJTreeSignal->SetBranchAddress("Et", &gFexLRJLeadingEtValuesSignal);
        gFexLeadingLRJTreeSignal->SetBranchAddress("Eta", &gFexLRJLeadingEtaValuesSignal);
        gFexLeadingLRJTreeSignal->SetBranchAddress("Phi", &gFexLRJLeadingPhiValuesSignal);

        // === gFexSubleadingLRJTreeSignal ===
        gFexSubleadingLRJTreeSignal->SetBranchAddress("Et", &gFexLRJSubleadingEtValuesSignal);
        gFexSubleadingLRJTreeSignal->SetBranchAddress("Eta", &gFexLRJSubleadingEtaValuesSignal);
        gFexSubleadingLRJTreeSignal->SetBranchAddress("Phi", &gFexLRJSubleadingPhiValuesSignal);

        // === jFexSRJTreeSignal ===
        jFexSRJTreeSignal->SetBranchAddress("EtIndex", &jFexSRJEtIndexValuesSignal);
        jFexSRJTreeSignal->SetBranchAddress("Et", &jFexSRJEtValuesSignal);
        jFexSRJTreeSignal->SetBranchAddress("Eta", &jFexSRJEtaValuesSignal);
        jFexSRJTreeSignal->SetBranchAddress("Phi", &jFexSRJPhiValuesSignal);

        // === jFexLeadingSRJTreeSignal ===
        jFexLeadingSRJTreeSignal->SetBranchAddress("Et", &jFexSRJLeadingEtValuesSignal);
        jFexLeadingSRJTreeSignal->SetBranchAddress("Eta", &jFexSRJLeadingEtaValuesSignal);
        jFexLeadingSRJTreeSignal->SetBranchAddress("Phi", &jFexSRJLeadingPhiValuesSignal);

        // === jFexSubleadingSRJTreeSignal ===
        jFexSubleadingSRJTreeSignal->SetBranchAddress("Et", &jFexSRJSubleadingEtValuesSignal);
        jFexSubleadingSRJTreeSignal->SetBranchAddress("Eta", &jFexSRJSubleadingEtaValuesSignal);
        jFexSubleadingSRJTreeSignal->SetBranchAddress("Phi", &jFexSRJSubleadingPhiValuesSignal);

        // === jFexLRJTreeSignal ===
        jFexLRJTreeSignal->SetBranchAddress("EtIndex", &jFexLRJEtIndexValuesSignal);
        jFexLRJTreeSignal->SetBranchAddress("Et", &jFexLRJEtValuesSignal);
        jFexLRJTreeSignal->SetBranchAddress("Eta", &jFexLRJEtaValuesSignal);
        jFexLRJTreeSignal->SetBranchAddress("Phi", &jFexLRJPhiValuesSignal);

        // === jFexLeadingLRJTreeSignal ===
        jFexLeadingLRJTreeSignal->SetBranchAddress("Et", &jFexLRJLeadingEtValuesSignal);
        jFexLeadingLRJTreeSignal->SetBranchAddress("Eta", &jFexLRJLeadingEtaValuesSignal);
        jFexLeadingLRJTreeSignal->SetBranchAddress("Phi", &jFexLRJLeadingPhiValuesSignal);

        // === jFexSubleadingLRJTreeSignal ===
        jFexSubleadingLRJTreeSignal->SetBranchAddress("Et", &jFexLRJSubleadingEtValuesSignal);
        jFexSubleadingLRJTreeSignal->SetBranchAddress("Eta", &jFexLRJSubleadingEtaValuesSignal);
        jFexSubleadingLRJTreeSignal->SetBranchAddress("Phi", &jFexLRJSubleadingPhiValuesSignal);

        // === hltAntiKt4EMTopoJetsTreeSignal ===
        hltAntiKt4EMTopoJetsTreeSignal->SetBranchAddress("EtIndex", &hltAntiKt4SRJEtIndexValuesSignal);
        hltAntiKt4EMTopoJetsTreeSignal->SetBranchAddress("Et", &hltAntiKt4SRJEtValuesSignal);
        hltAntiKt4EMTopoJetsTreeSignal->SetBranchAddress("Eta", &hltAntiKt4SRJEtaValuesSignal);
        hltAntiKt4EMTopoJetsTreeSignal->SetBranchAddress("Phi", &hltAntiKt4SRJPhiValuesSignal);

        // === leadingHltAntiKt4EMTopoJetsTreeSignal ===
        leadingHltAntiKt4EMTopoJetsTreeSignal->SetBranchAddress("Et", &hltAntiKt4SRJLeadingEtValuesSignal);
        leadingHltAntiKt4EMTopoJetsTreeSignal->SetBranchAddress("Eta", &hltAntiKt4SRJLeadingEtaValuesSignal);
        leadingHltAntiKt4EMTopoJetsTreeSignal->SetBranchAddress("Phi", &hltAntiKt4SRJLeadingPhiValuesSignal);

        // === subleadingHltAntiKt4EMTopoJetsTreeSignal ===
        subleadingHltAntiKt4EMTopoJetsTreeSignal->SetBranchAddress("Et", &hltAntiKt4SRJSubleadingEtValuesSignal);
        subleadingHltAntiKt4EMTopoJetsTreeSignal->SetBranchAddress("Eta", &hltAntiKt4SRJSubleadingEtaValuesSignal);
        subleadingHltAntiKt4EMTopoJetsTreeSignal->SetBranchAddress("Phi", &hltAntiKt4SRJSubleadingPhiValuesSignal);

        // === recoAntiKt10UFOCSSKJetsSignal ===
        recoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("EtIndex", &recoAntiKt10LRJEtIndexValuesSignal);
        recoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Et", &recoAntiKt10LRJEtValuesSignal);
        recoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Eta", &recoAntiKt10LRJEtaValuesSignal);
        recoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Phi", &recoAntiKt10LRJPhiValuesSignal);
        recoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Phi", &recoAntiKt10LRJMassValuesSignal);

        // === leadingRecoAntiKt10UFOCSSKJetsSignal ===
        leadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Et", &recoAntiKt10LRJLeadingEtValuesSignal);
        leadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Eta", &recoAntiKt10LRJLeadingEtaValuesSignal);
        leadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Phi", &recoAntiKt10LRJLeadingPhiValuesSignal);
        leadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Mass", &recoAntiKt10LRJLeadingMassValuesSignal);

        // === subleadingRecoAntiKt10UFOCSSKJetsSignal ===
        subleadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Et", &recoAntiKt10LRJSubleadingEtValuesSignal);
        subleadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Eta", &recoAntiKt10LRJSubleadingEtaValuesSignal);
        subleadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Phi", &recoAntiKt10LRJSubleadingPhiValuesSignal);
        subleadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Mass", &recoAntiKt10LRJSubleadingMassValuesSignal);

        // === truthAntiKt4TruthDressedWZJetsSignal ===
        truthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("EtIndex", &truthAntiKt4WZSRJEtIndexValuesSignal);
        truthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Et", &truthAntiKt4WZSRJEtValuesSignal);
        truthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Eta", &truthAntiKt4WZSRJEtaValuesSignal);
        truthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Phi", &truthAntiKt4WZSRJPhiValuesSignal);
        truthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Mass", &truthAntiKt4WZSRJMassValuesSignal);

        // === leadingTruthAntiKt4TruthDressedWZJetsSignal ===
        leadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Et", &truthAntiKt4WZSRJLeadingEtValuesSignal);
        leadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Eta", &truthAntiKt4WZSRJLeadingEtaValuesSignal);
        leadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Phi", &truthAntiKt4WZSRJLeadingPhiValuesSignal);
        leadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Mass", &truthAntiKt4WZSRJLeadingMassValuesSignal);

        // === subleadingTruthAntiKt4TruthDressedWZJetsSignal ===
        subleadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Et", &truthAntiKt4WZSRJSubleadingEtValuesSignal);
        subleadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Eta", &truthAntiKt4WZSRJSubleadingEtaValuesSignal);
        subleadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Phi", &truthAntiKt4WZSRJSubleadingPhiValuesSignal);
        subleadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Mass", &truthAntiKt4WZSRJSubleadingMassValuesSignal);

        // === inTimeAntiKt4TruthJetsTreeSignal ===
        inTimeAntiKt4TruthJetsTreeSignal->SetBranchAddress("EtIndex", &inTimeAntiKt4TruthSRJEtIndexValuesSignal);
        inTimeAntiKt4TruthJetsTreeSignal->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJEtValuesSignal);
        inTimeAntiKt4TruthJetsTreeSignal->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJEtaValuesSignal);
        inTimeAntiKt4TruthJetsTreeSignal->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJPhiValuesSignal);

        // === leadingInTimeAntiKt4TruthJetsTreeSignal ===
        leadingInTimeAntiKt4TruthJetsTreeSignal->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJLeadingEtValuesSignal);
        leadingInTimeAntiKt4TruthJetsTreeSignal->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJLeadingEtaValuesSignal);
        leadingInTimeAntiKt4TruthJetsTreeSignal->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJLeadingPhiValuesSignal);

        // === subleadingInTimeAntiKt4TruthJetsTreeSignal ===
        subleadingInTimeAntiKt4TruthJetsTreeSignal->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJSubleadingEtValuesSignal);
        subleadingInTimeAntiKt4TruthJetsTreeSignal->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJSubleadingEtaValuesSignal);
        subleadingInTimeAntiKt4TruthJetsTreeSignal->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJSubleadingPhiValuesSignal);

        // BACKGROUND TREES
        // === eventInfoTreeBack ===
        eventInfoTreeBack->SetBranchAddress("mcEventWeight", &mcEventWeightsValuesBack);
        eventInfoTreeBack->SetBranchAddress("sumOfWeightsForSample", &sumOfWeightsForSampleValuesBack);
        eventInfoTreeBack->SetBranchAddress("eventWeights", &eventWeightsValuesBack);
        eventInfoTreeBack->SetBranchAddress("sampleJZSlice", &sampleJZSliceValuesBack);

        // === jetTaggerLRJsBack ===
        jetTaggerLRJsBack->SetBranchAddress("Diam", &jetTaggerLRJDiamValuesBack);
        jetTaggerLRJsBack->SetBranchAddress("MergedIndices", &jetTaggerLRJMergedIndicesValuesBack);
        jetTaggerLRJsBack->SetBranchAddress("Et", &jetTaggerLRJEtValuesBack);
        jetTaggerLRJsBack->SetBranchAddress("Eta", &jetTaggerLRJEtaValuesBack);
        jetTaggerLRJsBack->SetBranchAddress("Phi", &jetTaggerLRJPhiValuesBack);

        // === jetTaggerLeadingLRJsBack ===
        jetTaggerLeadingLRJsBack->SetBranchAddress("Diam", &jetTaggerLeadingLRJDiamValuesBack);
        jetTaggerLeadingLRJsBack->SetBranchAddress("MergedIndices", &jetTaggerLeadingLRJMergedIndicesValuesBack);
        jetTaggerLeadingLRJsBack->SetBranchAddress("Et", &jetTaggerLeadingLRJEtValuesBack);
        jetTaggerLeadingLRJsBack->SetBranchAddress("Eta", &jetTaggerLeadingLRJEtaValuesBack);
        jetTaggerLeadingLRJsBack->SetBranchAddress("Phi", &jetTaggerLeadingLRJPhiValuesBack);

        // === jetTaggerSubleadingLRJsBack ===
        jetTaggerSubleadingLRJsBack->SetBranchAddress("Diam", &jetTaggerSubleadingLRJDiamValuesBack);
        jetTaggerSubleadingLRJsBack->SetBranchAddress("MergedIndices", &jetTaggerSubleadingLRJMergedIndicesValuesBack);
        jetTaggerSubleadingLRJsBack->SetBranchAddress("Et", &jetTaggerSubleadingLRJEtValuesBack);
        jetTaggerSubleadingLRJsBack->SetBranchAddress("Eta", &jetTaggerSubleadingLRJEtaValuesBack);
        jetTaggerSubleadingLRJsBack->SetBranchAddress("Phi", &jetTaggerSubleadingLRJPhiValuesBack);

        // === caloTopoTowerTreeBack ===
        caloTopoTowerTreeBack->SetBranchAddress("Et", &caloTopoTowerEtValuesBack);
        caloTopoTowerTreeBack->SetBranchAddress("Eta", &caloTopoTowerEtaValuesBack);
        caloTopoTowerTreeBack->SetBranchAddress("Phi", &caloTopoTowerPhiValuesBack);

        // === topo422TreeBack ===
        topo422TreeBack->SetBranchAddress("Et", &topo422EtValuesBack);
        topo422TreeBack->SetBranchAddress("Eta", &topo422EtaValuesBack);
        topo422TreeBack->SetBranchAddress("Phi", &topo422PhiValuesBack);

        // === gepBasicClustersTreeBack ===
        gepBasicClustersTreeBack->SetBranchAddress("Et", &gepBasicClustersEtValuesBack);
        gepBasicClustersTreeBack->SetBranchAddress("Eta", &gepBasicClustersEtaValuesBack);
        gepBasicClustersTreeBack->SetBranchAddress("Phi", &gepBasicClustersPhiValuesBack);

        // === gFexSRJTreeBack ===
        gFexSRJTreeBack->SetBranchAddress("EtIndex", &gFexSRJEtIndexValuesBack);
        gFexSRJTreeBack->SetBranchAddress("Et", &gFexSRJEtValuesBack);
        gFexSRJTreeBack->SetBranchAddress("Eta", &gFexSRJEtaValuesBack);
        gFexSRJTreeBack->SetBranchAddress("Phi", &gFexSRJPhiValuesBack);

        // === gFexLeadingSRJTreeBack ===
        gFexLeadingSRJTreeBack->SetBranchAddress("Et", &gFexSRJLeadingEtValuesBack);
        gFexLeadingSRJTreeBack->SetBranchAddress("Eta", &gFexSRJLeadingEtaValuesBack);
        gFexLeadingSRJTreeBack->SetBranchAddress("Phi", &gFexSRJLeadingPhiValuesBack);

        // === gFexSubleadingSRJTreeBack ===
        gFexSubleadingSRJTreeBack->SetBranchAddress("Et", &gFexSRJSubleadingEtValuesBack);
        gFexSubleadingSRJTreeBack->SetBranchAddress("Eta", &gFexSRJSubleadingEtaValuesBack);
        gFexSubleadingSRJTreeBack->SetBranchAddress("Phi", &gFexSRJSubleadingPhiValuesBack);

        // === gFexLRJTreeBack ===
        gFexLRJTreeBack->SetBranchAddress("EtIndex", &gFexLRJEtIndexValuesBack);
        gFexLRJTreeBack->SetBranchAddress("Et", &gFexLRJEtValuesBack);
        gFexLRJTreeBack->SetBranchAddress("Eta", &gFexLRJEtaValuesBack);
        gFexLRJTreeBack->SetBranchAddress("Phi", &gFexLRJPhiValuesBack);

        // === gFexLeadingLRJTreeBack ===
        gFexLeadingLRJTreeBack->SetBranchAddress("Et", &gFexLRJLeadingEtValuesBack);
        gFexLeadingLRJTreeBack->SetBranchAddress("Eta", &gFexLRJLeadingEtaValuesBack);
        gFexLeadingLRJTreeBack->SetBranchAddress("Phi", &gFexLRJLeadingPhiValuesBack);

        // === gFexSubleadingLRJTreeBack ===
        gFexSubleadingLRJTreeBack->SetBranchAddress("Et", &gFexLRJSubleadingEtValuesBack);
        gFexSubleadingLRJTreeBack->SetBranchAddress("Eta", &gFexLRJSubleadingEtaValuesBack);
        gFexSubleadingLRJTreeBack->SetBranchAddress("Phi", &gFexLRJSubleadingPhiValuesBack);

        // === jFexSRJTreeBack ===
        jFexSRJTreeBack->SetBranchAddress("EtIndex", &jFexSRJEtIndexValuesBack);
        jFexSRJTreeBack->SetBranchAddress("Et", &jFexSRJEtValuesBack);
        jFexSRJTreeBack->SetBranchAddress("Eta", &jFexSRJEtaValuesBack);
        jFexSRJTreeBack->SetBranchAddress("Phi", &jFexSRJPhiValuesBack);

        // === jFexLeadingSRJTreeBack ===
        jFexLeadingSRJTreeBack->SetBranchAddress("Et", &jFexSRJLeadingEtValuesBack);
        jFexLeadingSRJTreeBack->SetBranchAddress("Eta", &jFexSRJLeadingEtaValuesBack);
        jFexLeadingSRJTreeBack->SetBranchAddress("Phi", &jFexSRJLeadingPhiValuesBack);

        // === jFexSubleadingSRJTreeBack ===
        jFexSubleadingSRJTreeBack->SetBranchAddress("Et", &jFexSRJSubleadingEtValuesBack);
        jFexSubleadingSRJTreeBack->SetBranchAddress("Eta", &jFexSRJSubleadingEtaValuesBack);
        jFexSubleadingSRJTreeBack->SetBranchAddress("Phi", &jFexSRJSubleadingPhiValuesBack);

        // === jFexLRJTreeBack ===
        jFexLRJTreeBack->SetBranchAddress("EtIndex", &jFexLRJEtIndexValuesBack);
        jFexLRJTreeBack->SetBranchAddress("Et", &jFexLRJEtValuesBack);
        jFexLRJTreeBack->SetBranchAddress("Eta", &jFexLRJEtaValuesBack);
        jFexLRJTreeBack->SetBranchAddress("Phi", &jFexLRJPhiValuesBack);

        // === jFexLeadingLRJTreeBack ===
        jFexLeadingLRJTreeBack->SetBranchAddress("Et", &jFexLRJLeadingEtValuesBack);
        jFexLeadingLRJTreeBack->SetBranchAddress("Eta", &jFexLRJLeadingEtaValuesBack);
        jFexLeadingLRJTreeBack->SetBranchAddress("Phi", &jFexLRJLeadingPhiValuesBack);

        // === jFexSubleadingLRJTreeBack ===
        jFexSubleadingLRJTreeBack->SetBranchAddress("Et", &jFexLRJSubleadingEtValuesBack);
        jFexSubleadingLRJTreeBack->SetBranchAddress("Eta", &jFexLRJSubleadingEtaValuesBack);
        jFexSubleadingLRJTreeBack->SetBranchAddress("Phi", &jFexLRJSubleadingPhiValuesBack);

        // === hltAntiKt4EMTopoJetsTreeBack ===
        hltAntiKt4EMTopoJetsTreeBack->SetBranchAddress("EtIndex", &hltAntiKt4SRJEtIndexValuesBack);
        hltAntiKt4EMTopoJetsTreeBack->SetBranchAddress("Et", &hltAntiKt4SRJEtValuesBack);
        hltAntiKt4EMTopoJetsTreeBack->SetBranchAddress("Eta", &hltAntiKt4SRJEtaValuesBack);
        hltAntiKt4EMTopoJetsTreeBack->SetBranchAddress("Phi", &hltAntiKt4SRJPhiValuesBack);

        // === leadingHltAntiKt4EMTopoJetsTreeBack ===
        leadingHltAntiKt4EMTopoJetsTreeBack->SetBranchAddress("Et", &hltAntiKt4SRJLeadingEtValuesBack);
        leadingHltAntiKt4EMTopoJetsTreeBack->SetBranchAddress("Eta", &hltAntiKt4SRJLeadingEtaValuesBack);
        leadingHltAntiKt4EMTopoJetsTreeBack->SetBranchAddress("Phi", &hltAntiKt4SRJLeadingPhiValuesBack);

        // === subleadingHltAntiKt4EMTopoJetsTreeBack ===
        subleadingHltAntiKt4EMTopoJetsTreeBack->SetBranchAddress("Et", &hltAntiKt4SRJSubleadingEtValuesBack);
        subleadingHltAntiKt4EMTopoJetsTreeBack->SetBranchAddress("Eta", &hltAntiKt4SRJSubleadingEtaValuesBack);
        subleadingHltAntiKt4EMTopoJetsTreeBack->SetBranchAddress("Phi", &hltAntiKt4SRJSubleadingPhiValuesBack);

        // === recoAntiKt10UFOCSSKJetsBack ===
        recoAntiKt10UFOCSSKJetsBack->SetBranchAddress("EtIndex", &recoAntiKt10LRJEtIndexValuesBack);
        recoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Et", &recoAntiKt10LRJEtValuesBack);
        recoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Eta", &recoAntiKt10LRJEtaValuesBack);
        recoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Phi", &recoAntiKt10LRJPhiValuesBack);
        recoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Mass", &recoAntiKt10LRJMassValuesBack);

        // === leadingRecoAntiKt10UFOCSSKJetsBack ===
        leadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Et", &recoAntiKt10LRJLeadingEtValuesBack);
        leadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Eta", &recoAntiKt10LRJLeadingEtaValuesBack);
        leadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Phi", &recoAntiKt10LRJLeadingPhiValuesBack);
        leadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Mass", &recoAntiKt10LRJLeadingMassValuesBack);

        // === subleadingRecoAntiKt10UFOCSSKJetsBack ===
        subleadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Et", &recoAntiKt10LRJSubleadingEtValuesBack);
        subleadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Eta", &recoAntiKt10LRJSubleadingEtaValuesBack);
        subleadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Phi", &recoAntiKt10LRJSubleadingPhiValuesBack);
        subleadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Mass", &recoAntiKt10LRJSubleadingMassValuesBack);

        // === truthAntiKt4TruthDressedWZJetsBack ===
        truthAntiKt4TruthDressedWZJetsBack->SetBranchAddress("EtIndex", &truthAntiKt4WZSRJEtIndexValuesBack);
        truthAntiKt4TruthDressedWZJetsBack->SetBranchAddress("Et", &truthAntiKt4WZSRJEtValuesBack);
        truthAntiKt4TruthDressedWZJetsBack->SetBranchAddress("Eta", &truthAntiKt4WZSRJEtaValuesBack);
        truthAntiKt4TruthDressedWZJetsBack->SetBranchAddress("Phi", &truthAntiKt4WZSRJPhiValuesBack);

        // === leadingTruthAntiKt4TruthDressedWZJetsBack ===
        leadingTruthAntiKt4TruthDressedWZJetsBack->SetBranchAddress("Et", &truthAntiKt4WZSRJLeadingEtValuesBack);
        leadingTruthAntiKt4TruthDressedWZJetsBack->SetBranchAddress("Eta", &truthAntiKt4WZSRJLeadingEtaValuesBack);
        leadingTruthAntiKt4TruthDressedWZJetsBack->SetBranchAddress("Phi", &truthAntiKt4WZSRJLeadingPhiValuesBack);

        // === subleadingTruthAntiKt4TruthDressedWZJetsBack ===
        subleadingTruthAntiKt4TruthDressedWZJetsBack->SetBranchAddress("Et", &truthAntiKt4WZSRJSubleadingEtValuesBack);
        subleadingTruthAntiKt4TruthDressedWZJetsBack->SetBranchAddress("Eta", &truthAntiKt4WZSRJSubleadingEtaValuesBack);
        subleadingTruthAntiKt4TruthDressedWZJetsBack->SetBranchAddress("Phi", &truthAntiKt4WZSRJSubleadingPhiValuesBack);

        // === inTimeAntiKt4TruthJetsTreeBack ===
        inTimeAntiKt4TruthJetsTreeBack->SetBranchAddress("EtIndex", &inTimeAntiKt4TruthSRJEtIndexValuesBack);
        inTimeAntiKt4TruthJetsTreeBack->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJEtValuesBack);
        inTimeAntiKt4TruthJetsTreeBack->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJEtaValuesBack);
        inTimeAntiKt4TruthJetsTreeBack->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJPhiValuesBack);

        // === leadingInTimeAntiKt4TruthJetsTreeBack ===
        leadingInTimeAntiKt4TruthJetsTreeBack->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJLeadingEtValuesBack);
        leadingInTimeAntiKt4TruthJetsTreeBack->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJLeadingEtaValuesBack);
        leadingInTimeAntiKt4TruthJetsTreeBack->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJLeadingPhiValuesBack);

        // === subleadingInTimeAntiKt4TruthJetsTreeBack ===
        subleadingInTimeAntiKt4TruthJetsTreeBack->SetBranchAddress("Et", &inTimeAntiKt4TruthSRJSubleadingEtValuesBack);
        subleadingInTimeAntiKt4TruthJetsTreeBack->SetBranchAddress("Eta", &inTimeAntiKt4TruthSRJSubleadingEtaValuesBack);
        subleadingInTimeAntiKt4TruthJetsTreeBack->SetBranchAddress("Phi", &inTimeAntiKt4TruthSRJSubleadingPhiValuesBack);

        //gSystem->RedirectOutput("output5.log", "w");
        const int num_processed_events_background = eventInfoTreeBack->GetEntries();
        const int num_processed_events_signal = eventInfoTreeSignal->GetEntries();
        std::cout << "num_processed_events_background: " << num_processed_events_background << "\n";
        std::cout << "num_processed_events_signal: " <<  num_processed_events_signal << "\n";

        // Variables to store data
        std::vector<double> backgroundEtValues, backgroundEtaValues, backgroundPhiValues;
        std::vector<double> signalEtValues, signalEtaValues, signalPhiValues;

        // Open input file

        int current_event = -1;
        std::string sig_line;
        std::string back_line;
        
        // Bools determining which file being processed


        // Histograms for making overlaid plots between multiple files.
        // make unique names so ROOT doesn't overwrite
        auto h_sig_num100 = new TH1F(Form("sig_h_offlineLRJ_Et_num100_%d", fileIt),
            "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)",
            100, 0, 1000);
        auto h_sig_denom100 = new TH1F(Form("sig_h_offlineLRJ_Et_denom100_%d", fileIt),
            "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)",
            100, 0, 1000);

        auto h_sig_num250 = new TH1F(Form("sig_h_offlineLRJ_Et_num250_%d", fileIt),
            "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)",
            100, 0, 1000);
        auto h_sig_denom250 = new TH1F(Form("sig_h_offlineLRJ_Et_denom250_%d", fileIt),
            "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)",
            100, 0, 1000);

        auto h_sig_num400 = new TH1F(Form("sig_h_offlineLRJ_Et_num400_%d", fileIt),
            "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)",
            100, 0, 1000);
        auto h_sig_denom400 = new TH1F(Form("sig_h_offlineLRJ_Et_denom400_%d", fileIt),
            "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)",
            100, 0, 1000);

        auto h_back_num100 = new TH1F(Form("back_h_offlineLRJ_Et_num100_%d", fileIt),
            "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)",
            45, 50, 500);
        auto h_back_denom100 = new TH1F(Form("back_h_offlineLRJ_Et_denom100_%d", fileIt),
            "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)",
            45, 50, 500);

        auto h_back_num200 = new TH1F(Form("back_h_offlineLRJ_Et_num200_%d", fileIt),
            "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)",
            45, 50, 500);
        auto h_back_denom200 = new TH1F(Form("back_h_offlineLRJ_Et_denom200_%d", fileIt),
            "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)",
            45, 50, 500);

        auto h_back_num300 = new TH1F(Form("back_h_offlineLRJ_Et_num300_%d", fileIt),
            "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)",
            45, 50, 500);
        auto h_back_denom300 = new TH1F(Form("back_h_offlineLRJ_Et_denom300_%d", fileIt),
            "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)",
            45, 50, 500);

        // detach from any TFile so they survive after closing
        h_sig_num100->SetDirectory(nullptr);
        h_sig_denom100->SetDirectory(nullptr);
        h_sig_num250->SetDirectory(nullptr);
        h_sig_denom250->SetDirectory(nullptr);
        h_sig_num400->SetDirectory(nullptr);
        h_sig_denom400->SetDirectory(nullptr);

        h_back_num100->SetDirectory(nullptr);
        h_back_denom100->SetDirectory(nullptr);
        h_back_num200->SetDirectory(nullptr);
        h_back_denom200->SetDirectory(nullptr);
        h_back_num300->SetDirectory(nullptr);
        h_back_denom300->SetDirectory(nullptr);

        // push into vectors
        sig_num100.push_back(h_sig_num100);
        sig_denom100.push_back(h_sig_denom100);
        sig_num250.push_back(h_sig_num250);
        sig_denom250.push_back(h_sig_denom250);
        sig_num400.push_back(h_sig_num400);
        sig_denom400.push_back(h_sig_denom400);

        back_num100.push_back(h_back_num100);
        back_denom100.push_back(h_back_denom100);
        back_num200.push_back(h_back_num200);
        back_denom200.push_back(h_back_denom200);
        back_num300.push_back(h_back_num300);
        back_denom300.push_back(h_back_denom300);


        // Arrays to store JZ slice copies of histograms:
        TH1F* back_h_leading_LRJ_Et_arr[nJZSlices_];
        TH1F* back_h_subleading_LRJ_Et_arr[nJZSlices_];
        TH1F* back_h_gFEX_leading_LRJ_Et_arr[nJZSlices_];
        TH1F* back_h_gFEX_subleading_LRJ_Et_arr[nJZSlices_];
        TH1F* back_h_jFEX_leading_LRJ_Et_arr[nJZSlices_];
        TH1F* back_h_jFEX_subleading_LRJ_Et_arr[nJZSlices_];
        TH1F* back_h_leading_offlineLRJ_Et_arr[nJZSlices_];
        TH1F* back_h_subleading_offlineLRJ_Et_arr[nJZSlices_];
        TH1F* back_h_leading_truthSRJs_Et_arr[nJZSlices_];
        TH1F* back_h_subleading_truthSRJs_Et_arr[nJZSlices_];

        for (int i = 0; i < nJZSlices_; ++i) {
            back_h_leading_LRJ_Et_arr[i] = new TH1F(
                Form("back_h_leading_LRJ_Et_%d", i),
                "Leading LRJ Et Distribution; Leading JetTagger LRJ E_{T} [GeV]; Events / 10 GeV",
                210, 0, 2100);

            back_h_subleading_LRJ_Et_arr[i] = new TH1F(
                Form("back_h_subleading_LRJ_Et_%d", i),
                "Subleading LRJ Et Distribution;Subleading JetTagger LRJ E_{T} [GeV]; Events / 10 GeV",
                210, 0, 2100);

            back_h_gFEX_leading_LRJ_Et_arr[i] = new TH1F(
                Form("back_h_gFEX_leading_LRJ_Et_%d", i),
                "Leading gFEX LRJ Et Distribution;Leading gFEX LRJ E_{T} [GeV]; Events / 10 GeV",
                85, 0, 850);

            back_h_gFEX_subleading_LRJ_Et_arr[i] = new TH1F(
                Form("back_h_gFEX_subleading_LRJ_Et_%d", i),
                "Subleading gFEX LRJ Et Distribution;Subleading gFEX LRJ E_{T} [GeV]; Events / 10 GeV",
                85, 0, 850);

            back_h_jFEX_leading_LRJ_Et_arr[i] = new TH1F(
                Form("back_h_jFEX_leading_LRJ_Et_%d", i),
                "Leading jFEX LRJ Et Distribution;Leading jFEX LRJ E_{T} [GeV]; Events / 10 GeV",
                170, 0, 1700);

            back_h_jFEX_subleading_LRJ_Et_arr[i] = new TH1F(
                Form("back_h_jFEX_subleading_LRJ_Et_%d", i),
                "Subleading jFEX LRJ Et Distribution;Subleading jFEX LRJ E_{T} [GeV]; Events / 10 GeV",
                170, 0, 1700);

            back_h_leading_offlineLRJ_Et_arr[i] = new TH1F(
                Form("back_h_leading_offlineLRJ_Et_%d", i),
                "Leading Offline LRJ Et Distribution;Leading Offline LRJ E_{T} [GeV]; Events / 25 GeV",
                160, 0, 4000);

            back_h_subleading_offlineLRJ_Et_arr[i] = new TH1F(
                Form("back_h_subleading_offlineLRJ_Et_%d", i),
                "Subleading Offline LRJ Et Distribution;Subleading Offline LRJ E_{T} [GeV]; Events / 25 GeV",
                160, 0, 4000);

            back_h_leading_truthSRJs_Et_arr[i] = new TH1F(
                Form("back_h_leading_truthSRJs_Et_%d", i),
                "Leading Offline LRJ Et Distribution;Leading Truth SRJ E_{T} [GeV]; Events / 25 GeV",
                160, 0, 4000);

            back_h_subleading_truthSRJs_Et_arr[i] = new TH1F(
                Form("back_h_subleading_truthSRJs_Et_%d", i),
                "Subleading Offline LRJ Et Distribution;E_{T} [GeV];Subleading Truth SRJs / 25 GeV",
                160, 0, 4000);
        }

        // Variable binning: 0–400 GeV in 10 GeV steps, 400–800 GeV in 25 GeV steps
        std::vector<Double_t> rateVsEffBins;

        // 0 → 400 GeV in 10 GeV steps
        for (int i = 0; i <= 40; ++i) rateVsEffBins.push_back(i * 10.0);

        // 425 → 800 GeV in 25 GeV steps
        //for (int i = 17; i <= 32; ++i) rateVsEffBins.push_back(400.0 + (i - 16) * 50.0);
        // (above loop adds 425, 450, …, 800; total continuous sequence)

        
        // 450 → 800 GeV in 50 GeV steps
        for (int i = 1; i <= 8; ++i)
            rateVsEffBins.push_back(400.0 + i * 50.0);  // 450, 500, ..., 800

        // ---------------- Histograms / Profiles ----------------
        const double ptMin = 25., ptMax = 500.;
        const int    nPtBins = 19;   // 20 GeV per bin like the screenshot

        TProfile* prof_AvgDR_vs_HpT =
        new TProfile("prof_AvgDR_vs_HpT",
                    "Average #DeltaR(b,b̄) vs Higgs p_{T};Higgs p_{T} [GeV];Average #DeltaR(b,#bar{b})",
                    nPtBins, ptMin, ptMax);

        TH2F* h2_DR_vs_HpT =
        new TH2F("h2_DR_vs_HpT",
                "#DeltaR(b,b̄) vs Higgs p_{T};Higgs p_{T} [GeV];#DeltaR(b,#bar{b})",
                nPtBins, ptMin, ptMax,
                30, 0.0, 6.0);    // y-range wide enough for low-pt cases

        // style
        prof_AvgDR_vs_HpT->SetMarkerStyle(20);
        prof_AvgDR_vs_HpT->SetMarkerSize(0.9);
        prof_AvgDR_vs_HpT->SetLineColor(kBlack);
        prof_AvgDR_vs_HpT->SetMarkerColor(kBlack);

        TH1F* sig_h_LRJ_substruct = new TH1F("sig_h_LRJ_substruct", "LRJ 'Diameter';LRJ 'Diameter';% of LRJs / ~0.03", 32, 0, 1);
        TH2F *sigDiamvsEt = new TH2F("sigDiamvsEt", "Sum of Topo422 E_{T} in Each Bin; LRJ E_{T} [GeV];LRJ 'Diameter'", 
                            20, 0, 800,   // 100 bins from -5 to 5 on eta axis
                            32, 0, 1.0);  // 64 bins from -3.2 to 3.2 on phi axis


        TH2F *sigOfflineLeadingLRJMassvsEt = new TH2F("sigOfflineLeadingLRJMassvsEt", "Sum of Topo422 E_{T} in Each Bin; Offline Leading LRJ E_{T} [GeV];Offline Leading LRJ Mass [GeV]", 
                            35, 0, 700,   // 100 bins from -5 to 5 on eta axis
                            25, 0, 250);  // 64 bins from -3.2 to 3.2 on phi axis

        TH2F *backOfflineLeadingLRJMassvsEt = new TH2F("backOfflineLeadingLRJMassvsEt", "Sum of Topo422 E_{T} in Each Bin; Offline Leading LRJ E_{T} [GeV];Offline Leading LRJ Mass [GeV]", 
                            35, 0, 700,   // 100 bins from -5 to 5 on eta axis
                            25, 0, 250);  // 64 bins from -3.2 to 3.2 on phi axis


        TH2F *sigOfflineLeadingLRJMassvsSubjetMult = new TH2F("sigOfflineLeadingLRJMassvsSubjetMult", "Sum of Topo422 E_{T} in Each Bin; Number of Subjets; Offline Leading LRJ Mass [GeV]", 
                            4, 0, 4, // x axis
                            25, 0, 250 ); //y axis
        TH2F *backOfflineLeadingLRJMassvsSubjetMult = new TH2F("backOfflineLeadingLRJMassvsSubjetMult", "Sum of Topo422 E_{T} in Each Bin; Number of Subjets; Offline Leading LRJ Mass [GeV]", 
                            4, 0, 4, // x axis
                            25, 0, 250 ); //y axis

        TH2F *sigOfflineLeadingLRJEtvsSubjetMult = new TH2F("sigOfflineLeadingLRJEtvsSubjetMult", "Sum of Topo422 E_{T} in Each Bin; Number of Subjets; Offline Leading LRJ E_{T} [GeV]", 
                            4, 0, 4, // x axis
                            30, 0, 1500 ); //y axis
        TH2F *backOfflineLeadingLRJEtvsSubjetMult = new TH2F("backOfflineLeadingLRJEtvsSubjetMult", "Sum of Topo422 E_{T} in Each Bin; Number of Subjets; Offline Leading LRJ E_{T} [GeV]", 
                            4, 0, 4, // x axis
                            35, 0, 700 ); //y axis

        TH1F* sigOfflineLeadingLRJMass = new TH1F("sigOfflineLeadingLRJMass", "LRJ Et Distribution;Offline Leading LRJ Mass [GeV];% of Leading LRJs / 10 GeV", 25, 0, 250);
        TH1F* backOfflineLeadingLRJMass = new TH1F("backOfflineLeadingLRJMass", "LRJ Et Distribution;Offline Leading LRJ Mass [GeV];% of Leading LRJs / 10 GeV", 25, 0, 250);

        TH1F* sig_h_LeadingOfflineLRJ_SubjetMultiplicity = new TH1F("sig_h_LeadingOfflineLRJ_SubjetMultiplicity", "LRJ Et Distribution;Number of Subjets;% of Leading LRJs", 4, 0, 4);
        TH1F* back_h_LeadingOfflineLRJ_SubjetMultiplicity = new TH1F("back_h_LeadingOfflineLRJ_SubjetMultiplicity", "LRJ Et Distribution;Number of Subjets;% of Leading LRJs", 4, 0, 4);

        TH1F* sig_h_LeadingOfflineLRJ_SubjetEt = new TH1F("sig_h_LeadingOfflineLRJ_SubjetEt", "LRJ Et Distribution;Subjet E_{T}; (# of Leading Offline LRJs Subjets / Event) / 20 GeV", 20, 0, 400);
        TH1F* back_h_LeadingOfflineLRJ_SubjetEt = new TH1F("back_h_LeadingOfflineLRJ_SubjetEt", "LRJ Et Distribution;Subjet E_{T}; (# of Leading Offline LRJs Subjets / Event) / 20 GeV", 20, 0, 400);

        TH1F* sig_h_LRJ_Et = new TH1F("sig_h_LRJ_Et", "LRJ Et Distribution;E_{T} [GeV];% of LRJs / 50 GeV", 16, 0, 800);
        TH1F* sig_h_leading_LRJ_Et = new TH1F("sig_h_leading_LRJ_Et", "Leading LRJ Et Distribution;E_{T} [GeV];% of Leading LRJs / 25 GeV", rateVsEffBins.size() - 1, rateVsEffBins.data());
        TH1F* sig_h_subleading_LRJ_Et = new TH1F("sig_h_subleading_LRJ_Et", "Subleading LRJ Et Distribution;E_{T} [GeV];% of Subleading LRJs / 25 GeV", rateVsEffBins.size() - 1, rateVsEffBins.data());

        TH1F* sig_h_gFEX_leading_LRJ_Et = new TH1F("sig_h_gFEX_leading_LRJ_Et", "Leading LRJ Et Distribution;E_{T} [GeV];% of Leading LRJs / 25 GeV", rateVsEffBins.size() - 1, rateVsEffBins.data());
        TH1F* sig_h_gFEX_subleading_LRJ_Et = new TH1F("sig_h_gFEX_subleading_LRJ_Et", "Subleading LRJ Et Distribution;E_{T} [GeV];% of Subleading LRJs / 25 GeV", rateVsEffBins.size() - 1, rateVsEffBins.data());

        TH1F* sig_h_jFEX_leading_LRJ_Et = new TH1F("sig_h_jFEX_leading_LRJ_Et", "Leading LRJ Et Distribution;E_{T} [GeV];% of Leading LRJs / 25 GeV", rateVsEffBins.size() - 1, rateVsEffBins.data());
        TH1F* sig_h_jFEX_subleading_LRJ_Et = new TH1F("sig_h_jFEX_subleading_LRJ_Et", "Subleading LRJ Et Distribution;E_{T} [GeV];% of Subleading LRJs / 25 GeV", rateVsEffBins.size() - 1, rateVsEffBins.data());

        // with deltaR metric
        TH2F *sigOfflineLeadingLRJEtvsPsi_R = new TH2F("sigOfflineLeadingLRJEtvsPsi_R", "Sum of Topo422 E_{T} in Each Bin; JetTagger Leading LRJ E_{T} [GeV]; #Psi_{R}", 
                            35, 0, 700, // x axis
                            20, 0, 1 ); //y axis
        TH2F *sigOfflineLeadingLRJPsi_RvsSubleadingPsi_R = new TH2F("sigOfflineLeadingLRJPsi_RvsSubleadingPsi_R", "Sum of Topo422 E_{T} in Each Bin; #Psi_{R} [Subleading Jet]; #Psi_{R} [Leading Jet]", 
                            20, 0, 1, // x axis
                            20, 0, 1 ); //y axis
        TH2F *backOfflineLeadingLRJEtvsPsi_R = new TH2F("backOfflineLeadingLRJEtvsPsi_R", "Sum of Topo422 E_{T} in Each Bin; JetTagger Leading LRJ E_{T} [GeV]; #Psi_{R}", 
                            35, 0, 700, // x axis
                            20, 0, 1 ); //y axis
        TH2F *backOfflineLeadingLRJPsi_RvsSubleadingPsi_R = new TH2F("backOfflineLeadingLRJPsi_RvsSubleadingPsi_R", "Sum of Topo422 E_{T} in Each Bin; #Psi_{R} [Subleading Jet]; #Psi_{R} [Leading Jet]", 
                            20, 0, 1, // x axis
                            20, 0, 1 ); //y axis

        TH2F *sigOfflineSubleadingLRJEtvsPsi_R = new TH2F("sigOfflineSubleadingLRJEtvsPsi_R", "Sum of Topo422 E_{T} in Each Bin; JetTagger Subleading LRJ E_{T} [GeV]; #Psi_{R}", 
                            35, 0, 700, // x axis
                            20, 0, 1 ); //y axis
        TH2F *backOfflineSubleadingLRJEtvsPsi_R = new TH2F("backOfflineSubleadingLRJEtvsPsi_R", "Sum of Topo422 E_{T} in Each Bin; JetTagger Subleading LRJ E_{T} [GeV]; #Psi_{R}", 
                            35, 0, 700, // x axis
                            20, 0, 1 ); //y axis
        
        TH2F *sigOfflineLeadingLRJEtvsPsi_R_squared = new TH2F("sigOfflineLeadingLRJEtvsPsi_R_squared", "Sum of Topo422 E_{T} in Each Bin; JetTagger Leading LRJ E_{T} [GeV]; #Psi_{R, Leading} #times #Psi_{R, Subleading}", 
                            35, 0, 700, // x axis
                            20, 0, 0.4); //y axis
        TH2F *backOfflineLeadingLRJEtvsPsi_R_squared = new TH2F("backOfflineLeadingLRJEtvsPsi_R_squared", "Sum of Topo422 E_{T} in Each Bin; JetTagger Leading LRJ E_{T} [GeV]; #Psi_{R, Leading} #times #Psi_{R, Subleading}", 
                            35, 0, 700, // x axis
                            20, 0, 0.6); //y axis                    
        TH2F *sigOfflineSubleadingLRJEtvsPsi_R_squared = new TH2F("sigOfflineSubleadingLRJEtvsPsi_R_squared", "Sum of Topo422 E_{T} in Each Bin; JetTagger Subleading LRJ E_{T} [GeV]; #Psi_{R, Leading} #times #Psi_{R, Subleading}", 
                            35, 0, 700, // x axis
                            20, 0, 0.4); //y axis
        TH2F *backOfflineSubleadingLRJEtvsPsi_R_squared = new TH2F("backOfflineSubleadingLRJEtvsPsi_R_squared", "Sum of Topo422 E_{T} in Each Bin; JetTagger Subleading LRJ E_{T} [GeV]; #Psi_{R, Leading} #times #Psi_{R, Subleading}", 
                            35, 0, 700, // x axis
                            20, 0, 0.6); //y axis

        // with deltaR^2 Metric
        TH2F *sigOfflineLeadingLRJEtvsPsi_R2 = new TH2F("sigOfflineLeadingLRJEtvsPsi_R2", "Sum of Topo422 E_{T} in Each Bin; JetTagger Leading LRJ E_{T} [GeV]; #Psi_{R^{2}}", 
                            35, 0, 700, // x axis
                            20, 0, 1 ); //y axis
        TH2F *sigOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2 = new TH2F("sigOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2", "Sum of Topo422 E_{T} in Each Bin; #Psi_{R^{2}} [Subleading Jet]; #Psi_{R^{2}} [Leading Jet]", 
                            35, 0, 0.7, // x axis
                            35, 0, 0.7); //y axis
        TH2F *backOfflineLeadingLRJEtvsPsi_R2 = new TH2F("backOfflineLeadingLRJEtvsPsi_R2", "Sum of Topo422 E_{T} in Each Bin; JetTagger Leading LRJ E_{T} [GeV]; #Psi_{R^{2}}", 
                            35, 0, 700, // x axis
                            20, 0, 1 ); //y axis
        TH2F *backOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2 = new TH2F("backOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2", "Sum of Topo422 E_{T} in Each Bin; #Psi_{R^{2}} [Subleading Jet]; #Psi_{R^{2}} [Leading Jet]", 
                            35, 0, 0.7, // x axis
                            35, 0, 0.7); //y axis

        TH2F *sigOfflineSubleadingLRJEtvsPsi_R2 = new TH2F("sigOfflineSubleadingLRJEtvsPsi_R2", "Sum of Topo422 E_{T} in Each Bin; JetTagger Subleading LRJ E_{T} [GeV]; #Psi_{R^{2}}", 
                            35, 0, 700, // x axis
                            20, 0, 1 ); //y axis
        TH2F *backOfflineSubleadingLRJEtvsPsi_R2 = new TH2F("backOfflineSubleadingLRJEtvsPsi_R2", "Sum of Topo422 E_{T} in Each Bin; JetTagger Subleading LRJ E_{T} [GeV]; #Psi_{R^{2}}", 
                            35, 0, 700, // x axis
                            20, 0, 1 ); //y axis
        
        TH2F *sigOfflineLeadingLRJEtvsPsi_R2_squared = new TH2F("sigOfflineLeadingLRJEtvsPsi_R2_squared", "Sum of Topo422 E_{T} in Each Bin; JetTagger Leading LRJ E_{T} [GeV]; #Psi_{R^{2}, Leading} #times #Psi_{R^{2}, Subleading}", 
                            35, 0, 700, // x axis
                            30, 0, 0.3); //y axis
        TH2F *backOfflineLeadingLRJEtvsPsi_R2_squared = new TH2F("backOfflineLeadingLRJEtvsPsi_R2_squared", "Sum of Topo422 E_{T} in Each Bin; JetTagger Leading LRJ E_{T} [GeV]; #Psi_{R^{2}, Leading} #times #Psi_{R^{2}, Subleading}", 
                            35, 0, 700, // x axis
                            30, 0, 0.3); //y axis                    
        TH2F *sigOfflineSubleadingLRJEtvsPsi_R2_squared = new TH2F("sigOfflineSubleadingLRJEtvsPsi_R2_squared", "Sum of Topo422 E_{T} in Each Bin; JetTagger Subleading LRJ E_{T} [GeV]; #Psi_{R^{2}, Leading} #times #Psi_{R^{2}, Subleading}", 
                            35, 0, 700, // x axis
                            30, 0, 0.3); //y axis
        TH2F *backOfflineSubleadingLRJEtvsPsi_R2_squared = new TH2F("backOfflineSubleadingLRJEtvsPsi_R2_squared", "Sum of Topo422 E_{T} in Each Bin; JetTagger Subleading LRJ E_{T} [GeV]; #Psi_{R^{2}, Leading} #times #Psi_{R^{2}, Subleading}", 
                            35, 0, 700, // x axis
                            30, 0, 0.3); //y axis
        TH2F *sigOfflineLeadingLRJEtvsPsi_12 = new TH2F("sigOfflineLeadingLRJEtvsPsi_12", "Sum of Topo422 E_{T} in Each Bin; JetTagger Leading LRJ E_{T} [GeV]; #frac{#Psi_{R, Leading}}{#Psi_{R, Subleading}}", 
                            35, 0, 700, // x axis
                            40, 0, 4); //y axis
        TH2F *backOfflineLeadingLRJEtvsPsi_12 = new TH2F("backOfflineLeadingLRJEtvsPsi_12", "Sum of Topo422 E_{T} in Each Bin; JetTagger Leading LRJ E_{T} [GeV]; #frac{#Psi_{R, Leading}}{#Psi_{R, Subleading}}", 
                            35, 0, 700, // x axis
                            40, 0, 4); //y axis

        TH2F *sigOfflineSubleadingLRJEtvsPsi_12 = new TH2F("sigOfflineSubleadingLRJEtvsPsi_12", "Sum of Topo422 E_{T} in Each Bin; JetTagger Subleading LRJ E_{T} [GeV]; #frac{#Psi_{R, Leading}}{#Psi_{R, Subleading}}", 
                            35, 0, 700, // x axis
                            40, 0, 4); //y axis
        TH2F *backOfflineSubleadingLRJEtvsPsi_12 = new TH2F("backOfflineSubleadingLRJEtvsPsi_12", "Sum of Topo422 E_{T} in Each Bin; JetTagger Subleading LRJ E_{T} [GeV]; #frac{#Psi_{R, Leading}}{#Psi_{R, Subleading}}", 
                            35, 0, 700, // x axis
                            40, 0, 4); //y axis

        TH1F* sig_h_leading_LRJ_psi_R = new TH1F("sig_h_leading_LRJ_psi_R", "Leading LRJ Et Distribution;#Psi_{R};% of Leading LRJs / 0.05", 20, 0, 1);
        TH1F* sig_h_LRJ_psi_R_squared = new TH1F("sig_h_LRJ_psi_R_squared", "Leading LRJ Et Distribution;#Psi_{R, Leading} #times #Psi_{R, Subleading};% of Events / 0.02", 20, 0, 0.4);
        TH1F* sig_h_LRJ_psi_R_12 = new TH1F("sig_h_LRJ_psi_R_12", "Leading LRJ Et Distribution; #frac{#Psi_{R, Leading}}{#Psi_{R, Subleading}};% of Events / 0.2", 25, 0, 5);
        TH1F* sig_h_subleading_LRJ_psi_R = new TH1F("sig_h_subleading_LRJ_psi_R", "Subleading LRJ Et Distribution;#Psi_{R};% of Subleading LRJs / 0.05 ", 20, 0, 1);
        TH1F* back_h_leading_LRJ_psi_R = new TH1F("back_h_leading_LRJ_psi_R", "Leading LRJ Et Distribution;#Psi_{R};% of Leading LRJs / 0.05", 20, 0, 1);
        TH1F* back_h_LRJ_psi_R_squared = new TH1F("back_h_LRJ_psi_R_squared", "Leading LRJ Et Distribution;#Psi_{R, Leading} #times #Psi_{R, Subleading};% of Events / 0.02", 20, 0, 0.4);
        TH1F* back_h_LRJ_psi_R_12 = new TH1F("back_h_LRJ_psi_R_12", "Leading LRJ Et Distribution; #frac{#Psi_{R, Leading}}{#Psi_{R, Subleading}};% of Events / 0.2", 25, 0, 5);
        TH1F* back_h_subleading_LRJ_psi_R = new TH1F("back_h_subleading_LRJ_psi_R", "Subleading LRJ Et Distribution;#Psi_{R};% of Subleading LRJs / 0.05", 20, 0, 1);

        TH1F* sig_h_leading_LRJ_psi_R2 = new TH1F("sig_h_leading_LRJ_psi_R2", "Leading LRJ Et Distribution;#Psi_{R^{2}};% of Leading LRJs / 0.05", 20, 0, 1);
        TH1F* sig_h_LRJ_psi_R2_squared = new TH1F("sig_h_LRJ_psi_R2_squared", "Leading LRJ Et Distribution;#Psi_{R^{2}, Leading} #times #Psi_{R^{2}, Subleading};% of Events / 0.01", 20, 0, 0.2);
        TH1F* sig_h_LRJ_psi_R2_12 = new TH1F("sig_h_LRJ_psi_R2_12", "Leading LRJ Et Distribution; #frac{#Psi_{R^{2}, Leading}}{#Psi_{R^{2}, Subleading}};% of Events / 0.2", 25, 0, 5);
        TH1F* sig_h_subleading_LRJ_psi_R2 = new TH1F("sig_h_subleading_LRJ_psi_R2", "Subleading LRJ Et Distribution;#Psi_{R^{2}};% of Subleading LRJs / 0.05 ", 20, 0, 1);
        TH1F* back_h_leading_LRJ_psi_R2 = new TH1F("back_h_leading_LRJ_psi_R2", "Leading LRJ Et Distribution;#Psi_{R^{2}};% of Leading LRJs / 0.05", 20, 0, 1);
        TH1F* back_h_LRJ_psi_R2_squared = new TH1F("back_h_LRJ_psi_R2_squared", "Leading LRJ Et Distribution;#Psi_{R^{2}, Leading} #times #Psi_{R^{2}, Subleading};% of Events / 0.01", 20, 0, 0.2);
        TH1F* back_h_LRJ_psi_R2_12 = new TH1F("back_h_LRJ_psi_R2_12", "Leading LRJ Et Distribution; #frac{#Psi_{R^{2}, Leading}}{#Psi_{R^{2}, Subleading}};% of Events / 0.2", 25, 0, 5);
        TH1F* back_h_subleading_LRJ_psi_R2 = new TH1F("back_h_subleading_LRJ_psi_R2", "Subleading LRJ Et Distribution;#Psi_{R^{2}};% of Subleading LRJs / 0.05", 20, 0, 1);

        TH1F* sig_h_LRJ1_deltaEt_digitized_double = new TH1F("sig_h_LRJ1_deltaEt_digitized_double", "LRJ Et Distribution;#Delta E_{T} (Digitized - Full-precision) [GeV];% of LRJs / 1 GeV", 40, -20, 20);
        TH1F* sig_h_LRJ2_deltaEt_digitized_double = new TH1F("sig_h_LRJ2_deltaEt_digitized_double", "LRJ Et Distribution;#Delta E_{T} (Digitized - Full-precision) [GeV];% of LRJs / 1 GeV", 40, -20, 20);

        TH1F* back_h_LRJ1_deltaEt_digitized_double = new TH1F("back_h_LRJ1_deltaEt_digitized_double", "LRJ Et Distribution;#Delta E_{T} (Digitized - Full-precision) [GeV];% of LRJs / 1 GeV", 40, -20, 20);
        TH1F* back_h_LRJ2_deltaEt_digitized_double = new TH1F("back_h_LRJ2_deltaEt_digitized_double", "LRJ Et Distribution;#Delta E_{T} (Digitized - Full-precision) [GeV];% of LRJs / 1 GeV", 40, -20, 20);


        TH1F* sig_h_offlineLRJ_Et = new TH1F("sig_h_offlineLRJ_Et", "LRJ Et Distribution;E_{T} [GeV];% of Offline LRJs / 50 GeV", 16, 0, 800);
        TH1F* sig_h_leading_offlineLRJ_Et = new TH1F("sig_h_leading_offlineLRJ_Et", "Leading LRJ Et Distribution;E_{T} [GeV];% of Leading Offline LRJs / 50 GeV", 24, 0, 1200);
        TH1F* sig_h_subleading_offlineLRJ_Et = new TH1F("sig_h_subleading_offlineLRJ_Et", "Subleading LRJ Et Distribution;E_{T} [GeV];% of Subleading Offline LRJs / 50 GeV", 16, 0, 800);

        TH1F* sig_h_LRJ_E = new TH1F("sig_h_LRJ_E", "LRJ Et Distribution;Energy [GeV];% of LRJs / 10 GeV", 100, 0, 1000);
        TH1F* sig_h_LRJ_eta = new TH1F("sig_h_LRJ_eta", "LRJ Eta Distribution;#eta;Counts", 50, -5, 5);
        TH1F* sig_h_LRJ_phi = new TH1F("sig_h_LRJ_phi", "LRJ Phi Distribution;#phi;Counts", 32, -3.2, 3.2);


        TH1F* sig_h_leading_LRJ_gFexLRJ_deltaEt = new TH1F("sig_h_leading_LRJ_gFexLRJ_deltaEt", "#Delta E_{T} Leading gFex LRJ, Output LRJ, ;#Delta E_{T} (gFex - JetTagger) [GeV];% of Leading LRJs / 10 GeV", 50, -150, 150);
        TH1F* sig_h_leading_LRJ_offlineLRJ_deltaEt = new TH1F("sig_h_leading_LRJ_offlineLRJ_deltaEt", "#Delta E_{T} Leading Offline LRJ, Output LRJ, ;#Delta E_{T} (Offline - JetTagger) [GeV];% of Leading LRJs / 10 GeV", 50, -150, 350);

        TH1F* sig_h_leading_offlineLRJ_gFexLRJ_deltaEt = new TH1F("sig_h_leading_offlineLRJ_gFexLRJ_deltaEt", "#Delta E_{T} Leading gFex LRJ, Output LRJ, ;#Delta E_{T} (gFex - JetTagger) [GeV];% of Leading LRJs / 10 GeV", 50, -150, 350);
        TH1F* sig_h_leading_offlineLRJ_jFexLRJ_deltaEt = new TH1F("sig_h_leading_offlineLRJ_jFexLRJ_deltaEt", "#Delta E_{T} Leading gFex LRJ, Output LRJ, ;#Delta E_{T} (gFex - JetTagger) [GeV];% of Leading LRJs / 10 GeV", 50, -150, 350);

        TH1F* sig_h_leading_LRJ_gFexLRJ_Et_resolution = new TH1F("sig_h_leading_LRJ_gFexLRJ_Et_resolution", "#Delta E_{T} Leading gFex LRJ, Output LRJ, ;#Delta E_{T} (gFex - JetTagger) / E_{T, Offline} [GeV];% of Leading LRJs / 0.1", 40, -2, 2);
        TH1F* sig_h_leading_LRJ_offlineLRJ_Et_resolution = new TH1F("sig_h_leading_LRJ_offlineLRJ_Et_resolution", "#Delta E_{T} Leading Offline LRJ, Output LRJ, ;#Delta E_{T} (Offline - JetTagger) / E_{T, Offline} [GeV];% of Leading LRJs / 0.1", 20, -1, 1);

        TH1F* sig_h_leading_offlineLRJ_gFexLRJ_Et_resolution = new TH1F("sig_h_leading_offlineLRJ_gFexLRJ_Et_resolution", "#Delta E_{T} Leading gFEX LRJ, Output LRJ, ;#Delta E_{T} (Offline - gFEX) / E_{T, Offline} [GeV];% of Leading LRJs / 0.1", 50, -3.5, 1.5);
        TH1F* sig_h_leading_offlineLRJ_jFexLRJ_Et_resolution = new TH1F("sig_h_leading_offlineLRJ_jFexLRJ_Et_resolution", "#Delta E_{T} Leading jFEX LRJ, Output LRJ, ;#Delta E_{T} (Offline - jFEX) / E_{T, Offline} [GeV];% of Leading LRJs / 0.1", 40, -3, 1);

        TH1F* sig_h_leading_LRJ_gFexLRJ_deltaR = new TH1F("sig_h_leading_LRJ_gFexLRJ_deltaR", "#Delta R Leading gFex LRJ, Output LRJ, ;#Delta R (gFex, JetTagger);% of Leading LRJs / 0.1", 50, 0, 5);
        TH1F* sig_h_leading_LRJ_offlineLRJ_deltaR = new TH1F("sig_h_leading_LRJ_offlineLRJ_deltaR", "#Delta R Leading Offline LRJ, Output LRJ, ;#Delta R (Offline, JetTagger);% of Leading LRJs / 0.1", 50, 0, 5);

        TH1F* sig_h_first_LRJ_jFexSRJ_deltaR = new TH1F("sig_h_first_LRJ_jFexSRJ_deltaR", "#Delta R Leading jFex SRJ, Output LRJ, ;#Delta R (Lead. jFex SRJ, 1st JetTagger LRJ);% of Leading LRJs / 0.1", 50, 0, 5);
        TH1F* sig_h_second_LRJ_jFexSRJ_deltaR = new TH1F("sig_h_second_LRJ_jFexSRJ_deltaR", "#Delta R Subleading jFex SRJ, Output LRJ, ;#Delta R (Sublead. jFex SRJ, 2nd JetTagger LRJ);% of Leading LRJs / 0.1", 50, 0, 5);

        TH1F* sig_h_lead_sublead_LRJ_deltaR = new TH1F("sig_h_lead_sublead_LRJ_deltaR", "#Delta R Leading, Subleading LRJ, ;#Delta R (Lead., Sublead. Output LRJ);% of Leading LRJs / 0.1", 50, 0, 5);

        // Signal trigger efficiencies (gFex, JetTagger)

        TH1F* sig_h_offlineLRJ_Et_num50 = new TH1F("sig_h_offlineLRJ_Et_num50", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom50 = new TH1F("sig_h_offlineLRJ_Et_denom50", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num50_Dijet = new TH1F("sig_h_offlineLRJ_Et_num50_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom50_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom50_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num50_1Subjet = new TH1F("sig_h_offlineLRJ_Et_num50_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom50_1Subjet = new TH1F("sig_h_offlineLRJ_Et_denom50_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num50_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_num50_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom50_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_denom50_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num50_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_num50_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom50_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_denom50_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num50_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num50_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom50_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom50_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num50_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num50_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom50_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom50_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num50_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_num50_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom50_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom50_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_Avg_b_Et_num50 = new TH1F("sig_h_Avg_b_Et_num50", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_Avg_b_Et_denom50 = new TH1F("sig_h_Avg_b_Et_denom50", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num100 = new TH1F("sig_h_offlineLRJ_Et_num100", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom100 = new TH1F("sig_h_offlineLRJ_Et_denom100", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num100_Dijet = new TH1F("sig_h_offlineLRJ_Et_num100_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom100_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom100_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num100_1Subjet = new TH1F("sig_h_offlineLRJ_Et_num100_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom100_1Subjet = new TH1F("sig_h_offlineLRJ_Et_denom100_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num100_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_num100_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom100_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_denom100_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num100_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_num100_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom100_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_denom100_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num100_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num100_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom100_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom100_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num100_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num100_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom100_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom100_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num100_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_num100_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom100_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom100_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_Avg_b_Et_num100 = new TH1F("sig_h_Avg_b_Et_num100", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_Avg_b_Et_denom100 = new TH1F("sig_h_Avg_b_Et_denom100", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num150 = new TH1F("sig_h_offlineLRJ_Et_num150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom150 = new TH1F("sig_h_offlineLRJ_Et_denom150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num150_Dijet = new TH1F("sig_h_offlineLRJ_Et_num150_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom150_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom150_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num150_1Subjet = new TH1F("sig_h_offlineLRJ_Et_num150_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom150_1Subjet = new TH1F("sig_h_offlineLRJ_Et_denom150_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num150_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_num150_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom150_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_denom150_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num150_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_num150_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom150_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_denom150_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num150_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num150_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom150_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom150_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num150_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num150_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom150_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom150_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num150_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_num150_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom150_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom150_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_Avg_b_Et_num150 = new TH1F("sig_h_Avg_b_Et_num150", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_Avg_b_Et_denom150 = new TH1F("sig_h_Avg_b_Et_denom150", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num200 = new TH1F("sig_h_offlineLRJ_Et_num200", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom200 = new TH1F("sig_h_offlineLRJ_Et_denom200", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num200_Dijet = new TH1F("sig_h_offlineLRJ_Et_num200_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom200_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom200_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num200_1Subjet = new TH1F("sig_h_offlineLRJ_Et_num200_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom200_1Subjet = new TH1F("sig_h_offlineLRJ_Et_denom200_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num200_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_num200_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom200_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_denom200_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num200_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_num200_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom200_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_denom200_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num200_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num200_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom200_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom200_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num200_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num200_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom200_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom200_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num200_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_num200_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom200_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom200_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_Avg_b_Et_num200 = new TH1F("sig_h_Avg_b_Et_num200", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_Avg_b_Et_denom200 = new TH1F("sig_h_Avg_b_Et_denom200", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num250 = new TH1F("sig_h_offlineLRJ_Et_num250", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom250 = new TH1F("sig_h_offlineLRJ_Et_denom250", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num250_Dijet = new TH1F("sig_h_offlineLRJ_Et_num250_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom250_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom250_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num250_1Subjet = new TH1F("sig_h_offlineLRJ_Et_num250_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom250_1Subjet = new TH1F("sig_h_offlineLRJ_Et_denom250_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num250_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_num250_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom250_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_denom250_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num250_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_num250_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom250_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_denom250_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num250_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num250_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom250_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom250_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num250_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num250_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom250_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom250_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num250_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_num250_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom250_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom250_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_Avg_b_Et_num250 = new TH1F("sig_h_Avg_b_Et_num250", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_Avg_b_Et_denom250 = new TH1F("sig_h_Avg_b_Et_denom250", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num300 = new TH1F("sig_h_offlineLRJ_Et_num300", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom300 = new TH1F("sig_h_offlineLRJ_Et_denom300", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num300_Dijet = new TH1F("sig_h_offlineLRJ_Et_num300_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom300_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom300_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num300_1Subjet = new TH1F("sig_h_offlineLRJ_Et_num300_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom300_1Subjet = new TH1F("sig_h_offlineLRJ_Et_denom300_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num300_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_num300_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom300_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_denom300_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        
        TH1F* sig_h_offlineLRJ_Et_num300_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_num300_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom300_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_denom300_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num300_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num300_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom300_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom300_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num300_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num300_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom300_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom300_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num300_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_num300_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom300_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom300_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_Avg_b_Et_num300 = new TH1F("sig_h_Avg_b_Et_num300", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_Avg_b_Et_denom300 = new TH1F("sig_h_Avg_b_Et_denom300", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num350 = new TH1F("sig_h_offlineLRJ_Et_num350", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom350 = new TH1F("sig_h_offlineLRJ_Et_denom350", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num350_Dijet = new TH1F("sig_h_offlineLRJ_Et_num350_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom350_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom350_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num350_1Subjet = new TH1F("sig_h_offlineLRJ_Et_num350_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom350_1Subjet = new TH1F("sig_h_offlineLRJ_Et_denom350_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num350_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_num350_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom350_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_denom350_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num350_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_num350_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom350_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_denom350_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num350_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num350_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom350_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom350_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num350_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num350_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom350_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom350_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num350_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_num350_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom350_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom350_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_Avg_b_Et_num350 = new TH1F("sig_h_Avg_b_Et_num350", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_Avg_b_Et_denom350 = new TH1F("sig_h_Avg_b_Et_denom350", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num400 = new TH1F("sig_h_offlineLRJ_Et_num400", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom400 = new TH1F("sig_h_offlineLRJ_Et_denom400", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num400_Dijet = new TH1F("sig_h_offlineLRJ_Et_num400_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom400_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom400_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num400_1Subjet = new TH1F("sig_h_offlineLRJ_Et_num400_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom400_1Subjet = new TH1F("sig_h_offlineLRJ_Et_denom400_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num400_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_num400_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom400_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_denom400_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num400_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_num400_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom400_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_denom400_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num400_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num400_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom400_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom400_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num400_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num400_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom400_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom400_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num400_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_num400_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom400_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom400_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_Avg_b_Et_num400 = new TH1F("sig_h_Avg_b_Et_num400", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_Avg_b_Et_denom400 = new TH1F("sig_h_Avg_b_Et_denom400", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num450 = new TH1F("sig_h_offlineLRJ_Et_num450", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom450 = new TH1F("sig_h_offlineLRJ_Et_denom450", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num450_Dijet = new TH1F("sig_h_offlineLRJ_Et_num45_Dijet0", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom450_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom450_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num450_1Subjet = new TH1F("sig_h_offlineLRJ_Et_num450_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom450_1Subjet = new TH1F("sig_h_offlineLRJ_Et_denom450_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num450_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_num450_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom450_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_denom450_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num450_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_num450_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom450_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_denom450_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num450_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num450_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom450_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom450_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num450_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num450_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom450_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom450_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num450_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_num450_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom450_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom450_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_Avg_b_Et_num450 = new TH1F("sig_h_Avg_b_Et_num450", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_Avg_b_Et_denom450 = new TH1F("sig_h_Avg_b_Et_denom450", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num500 = new TH1F("sig_h_offlineLRJ_Et_num500", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom500 = new TH1F("sig_h_offlineLRJ_Et_denom500", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num500_Dijet = new TH1F("sig_h_offlineLRJ_Et_num500_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom500_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom500_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num500_1Subjet = new TH1F("sig_h_offlineLRJ_Et_num500_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom500_1Subjet = new TH1F("sig_h_offlineLRJ_Et_denom500_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num500_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_num500_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom500_GrEq2Subjets = new TH1F("sig_h_offlineLRJ_Et_denom500_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num500_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_num500_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom500_mass100to150 = new TH1F("sig_h_offlineLRJ_Et_denom500_mass100to150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num500_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num500_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom500_gFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom500_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num500_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_num500_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom500_jFexLRJ = new TH1F("sig_h_offlineLRJ_Et_denom500_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_offlineLRJ_Et_num500_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_num500_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_offlineLRJ_Et_denom500_gFexLRJ_Dijet = new TH1F("sig_h_offlineLRJ_Et_denom500_gFexLRJ_Dijet", "LRJ Et Distribution;Offline Subleading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        TH1F* sig_h_Avg_b_Et_num500 = new TH1F("sig_h_Avg_b_Et_num500", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);
        TH1F* sig_h_Avg_b_Et_denom500 = new TH1F("sig_h_Avg_b_Et_denom500", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency (Signal)", 100, 0, 1000);

        // Background trigger efficiencies (gFex, JetTagger)

        TH1F* back_h_offlineLRJ_Et_num50 = new TH1F("back_h_offlineLRJ_Et_num50", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom50 = new TH1F("back_h_offlineLRJ_Et_denom50", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num50_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_num50_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom50_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom50_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num50_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_num50_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom50_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom50_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num100 = new TH1F("back_h_offlineLRJ_Et_num100", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom100 = new TH1F("back_h_offlineLRJ_Et_denom100", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num100_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_num100_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom100_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom100_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num100_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_num100_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom100_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom100_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num100_1Subjet = new TH1F("back_h_offlineLRJ_Et_num100_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom100_1Subjet = new TH1F("back_h_offlineLRJ_Et_denom100_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num100_GrEq2Subjets = new TH1F("back_h_offlineLRJ_Et_num100_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom100_GrEq2Subjets = new TH1F("back_h_offlineLRJ_Et_denom100_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num150 = new TH1F("back_h_offlineLRJ_Et_num150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom150 = new TH1F("back_h_offlineLRJ_Et_denom150", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num150_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_num150_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom150_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom150_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num150_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_num150_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom150_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom150_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num200 = new TH1F("back_h_offlineLRJ_Et_num200", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom200 = new TH1F("back_h_offlineLRJ_Et_denom200", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num200_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_num200_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom200_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom200_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num200_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_num200_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom200_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom200_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num200_1Subjet = new TH1F("back_h_offlineLRJ_Et_num200_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom200_1Subjet = new TH1F("back_h_offlineLRJ_Et_denom200_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num200_GrEq2Subjets = new TH1F("back_h_offlineLRJ_Et_num200_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom200_GrEq2Subjets = new TH1F("back_h_offlineLRJ_Et_denom200_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num250 = new TH1F("back_h_offlineLRJ_Et_num250", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom250 = new TH1F("back_h_offlineLRJ_Et_denom250", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num250_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_num250_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom250_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom250_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num250_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_num250_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom250_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom250_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num300 = new TH1F("back_h_offlineLRJ_Et_num300", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom300 = new TH1F("back_h_offlineLRJ_Et_denom300", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num300_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_num300_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom300_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom300_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num300_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_num300_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom300_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom300_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num300_1Subjet = new TH1F("back_h_offlineLRJ_Et_num300_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom300_1Subjet = new TH1F("back_h_offlineLRJ_Et_denom300_1Subjet", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num300_GrEq2Subjets = new TH1F("back_h_offlineLRJ_Et_num300_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom300_GrEq2Subjets = new TH1F("back_h_offlineLRJ_Et_denom300_GrEq2Subjets", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num350 = new TH1F("back_h_offlineLRJ_Et_num350", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom350 = new TH1F("back_h_offlineLRJ_Et_denom350", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num350_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_num350_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom350_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom350_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num350_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_num350_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom350_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom350_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num400 = new TH1F("back_h_offlineLRJ_Et_num400", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom400 = new TH1F("back_h_offlineLRJ_Et_denom400", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num400_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_num400_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom400_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom400_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num400_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_num400_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom400_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom400_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num450 = new TH1F("back_h_offlineLRJ_Et_num450", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom450 = new TH1F("back_h_offlineLRJ_Et_denom450", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num450_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_num450_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom450_gFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom450_gFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* back_h_offlineLRJ_Et_num450_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_num450_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);
        TH1F* back_h_offlineLRJ_Et_denom450_jFexLRJ = new TH1F("back_h_offlineLRJ_Et_denom450_jFexLRJ", "LRJ Et Distribution;Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)", 45, 50, 500);

        TH1F* sig_h_Mjj = new TH1F("sig_h_Mjj", "Invariant Mass of 2 LRJs;Invariant Mass of 2 LRJs [GeV]; % Events / 20 GeV", 75, 0, 1500);

        TH2F *sigTopo422Highest128SeedPositionsEv0 = new TH2F("sigTopo422Highest128SeedPositionsEv0", "Sum of Topo422 E_{T} in Each Bin; #eta;#phi", 
                            100, -5, 5,   // 100 bins from -5 to 5 on eta axis
                            64, -3.2, 3.2);  // 64 bins from -3.2 to 3.2 on phi axis

        std::vector<std::pair<double, double > > jFexSeedPositionsSigEv0;
        std::vector<std::pair<double, double > > jFexAdditionalSRJPositionsSigEv0;
        std::vector<std::pair<double, double > > newSeedPositionsSigEv0;
        std::vector<std::pair<double, double  > > gFexLRJPositionSigEv0;
        std::vector<std::pair<double, double  > > offlineLRJPositionSigEv0;
        std::vector<double > sigLRJEtEv0;
        std::vector<double > siggFexLRJEtEv0;
        std::vector<double > sigOfflineLRJEtEv0;

        TH2F *sigTopo422Highest128SeedPositionsEv1 = new TH2F("sigTopo422Highest128SeedPositionsEv1", "Sum of Topo422 E_{T} in Each Bin; #eta;#phi", 
                            100, -5, 5,   // 100 bins from -5 to 5 on eta axis
                            64, -3.2, 3.2);  // 64 bins from -3.2 to 3.2 on phi axis

        std::vector<std::pair<double, double  > > jFexSeedPositionsSigEv1;
        std::vector<std::pair<double, double  > > jFexAdditionalSRJPositionsSigEv1;
        std::vector<std::pair<double, double  > > newSeedPositionsSigEv1;
        std::vector<std::pair<double, double  > > gFexLRJPositionSigEv1;
        std::vector<std::pair<double, double  > > offlineLRJPositionSigEv1;
        std::vector<double > sigLRJEtEv1;
        std::vector<double > siggFexLRJEtEv1;
        std::vector<double > sigOfflineLRJEtEv1;

        TH2F *sigTopo422Highest128SeedPositionsEv2 = new TH2F("sigTopo422Highest128SeedPositionsEv2", "Sum of Topo422 E_{T} in Each Bin; #eta;#phi", 
                            100, -5, 5,   // 100 bins from -5 to 5 on eta axis
                            64, -3.2, 3.2);  // 64 bins from -3.2 to 3.2 on phi axis
        
        std::vector<std::pair<double, double  > > jFexSeedPositionsSigEv2;
        std::vector<std::pair<double, double  > > jFexAdditionalSRJPositionsSigEv2;
        std::vector<std::pair<double, double  > > newSeedPositionsSigEv2;
        std::vector<std::pair<double, double  > > gFexLRJPositionSigEv2;
        std::vector<std::pair<double, double  > > offlineLRJPositionSigEv2;
        std::vector<double > sigLRJEtEv2;
        std::vector<double > siggFexLRJEtEv2;
        std::vector<double > sigOfflineLRJEtEv2;

        TH2F *sigTopo422Highest128SeedPositionsEv3 = new TH2F("sigTopo422Highest128SeedPositionsEv3", "Sum of Topo422 E_{T} in Each Bin; #eta;#phi", 
                            100, -5, 5,   // 100 bins from -5 to 5 on eta axis
                            64, -3.2, 3.2);  // 64 bins from -3.2 to 3.2 on phi axis

        std::vector<std::pair<double, double  > > jFexSeedPositionsSigEv3;
        std::vector<std::pair<double, double  > > jFexAdditionalSRJPositionsSigEv3;
        std::vector<std::pair<double, double  > > newSeedPositionsSigEv3;
        std::vector<std::pair<double, double  > > gFexLRJPositionSigEv3;
        std::vector<std::pair<double, double  > > offlineLRJPositionSigEv3;
        std::vector<double > sigLRJEtEv3;
        std::vector<double > siggFexLRJEtEv3;
        std::vector<double > sigOfflineLRJEtEv3;

        TH2F *sigTopo422Highest128SeedPositionsEv4 = new TH2F("sigTopo422Highest128SeedPositionsEv4", "Sum of Topo422 E_{T} in Each Bin; #eta;#phi", 
                            100, -5, 5,   // 100 bins from -5 to 5 on eta axis
                            64, -3.2, 3.2);  // 64 bins from -3.2 to 3.2 on phi axis

        std::vector<std::pair<double, double  > > jFexSeedPositionsSigEv4;
        std::vector<std::pair<double, double  > > jFexAdditionalSRJPositionsSigEv4;
        std::vector<std::pair<double, double  > > newSeedPositionsSigEv4;
        std::vector<std::pair<double, double  > > gFexLRJPositionSigEv4;
        std::vector<std::pair<double, double  > > offlineLRJPositionSigEv4;
        std::vector<double > sigLRJEtEv4;
        std::vector<double > siggFexLRJEtEv4;
        std::vector<double > sigOfflineLRJEtEv4;

        TH1F* back_h_LRJ_substruct = new TH1F("back_h_LRJ_substruct", "LRJ 'Diameter';LRJ 'Diameter';% of LRJs / ~0.03", 32, 0, 1);
        TH2F *backDiamvsEt = new TH2F("backDiamvsEt", "Sum of Topo422 E_{T} in Each Bin; LRJ E_{T} [GeV];LRJ 'Diameter'", 
                            20, 0, 800,   // 100 bins from -5 to 5 on eta axis
                            32, 0, 1.0);  // 64 bins from -3.2 to 3.2 on phi axis
        TH1F* back_h_LRJ_Et = new TH1F("back_h_LRJ_Et", "LRJ Et Distribution;E_{T} [GeV];% of LRJs / 50 GeV", 16, 0, 800);
        TH1F* back_h_leading_LRJ_Et = new TH1F("back_h_leading_LRJ_Et", "Leading LRJ Et Distribution;E_{T} [GeV];% of Leading LRJs / 25 GeV", rateVsEffBins.size() - 1, rateVsEffBins.data());
        TH1F* back_h_subleading_LRJ_Et = new TH1F("back_h_subleading_LRJ_Et", "Subleading LRJ Et Distribution;E_{T} [GeV];% of Subleading LRJs / 25 GeV", rateVsEffBins.size() - 1, rateVsEffBins.data());

        TH1F* back_h_gFEX_leading_LRJ_Et = new TH1F("back_h_gFEX_leading_LRJ_Et", "Leading LRJ Et Distribution;E_{T} [GeV];% of Leading LRJs / 25 GeV", rateVsEffBins.size() - 1, rateVsEffBins.data());
        TH1F* back_h_gFEX_subleading_LRJ_Et = new TH1F("back_h_gFEX_subleading_LRJ_Et", "Subleading LRJ Et Distribution;E_{T} [GeV];% of Subleading LRJs / 25 GeV", rateVsEffBins.size() - 1, rateVsEffBins.data());

        TH1F* back_h_jFEX_leading_LRJ_Et = new TH1F("back_h_jFEX_leading_LRJ_Et", "Leading LRJ Et Distribution;E_{T} [GeV];% of Leading LRJs / 25 GeV", rateVsEffBins.size() - 1, rateVsEffBins.data());
        TH1F* back_h_jFEX_subleading_LRJ_Et = new TH1F("back_h_jFEX_subleading_LRJ_Et", "Subleading LRJ Et Distribution;E_{T} [GeV];% of Subleading LRJs / 25 GeV", rateVsEffBins.size() - 1, rateVsEffBins.data());

        TH1F* back_h_offlineLRJ_Et = new TH1F("back_h_offlineLRJ_Et", "LRJ Et Distribution;E_{T} [GeV];% of Offline LRJs / 50 GeV", 16, 0, 800);
        TH1F* back_h_leading_offlineLRJ_Et = new TH1F("back_h_leading_offlineLRJ_Et", "Leading LRJ Et Distribution;E_{T} [GeV];% of Leading Offline LRJs / 50 GeV", 24, 0, 1200);
        TH1F* back_h_subleading_offlineLRJ_Et = new TH1F("back_h_subleading_offlineLRJ_Et", "Subleading LRJ Et Distribution;E_{T} [GeV];% of Subleading Offline LRJs / 50 GeV", 16, 0, 800);
        TH1F* back_h_LRJ_E = new TH1F("back_h_LRJ_E", "LRJ Et Distribution;Energy [GeV];% of LRJs / 10 GeV", 100, 0, 1000);
        TH1F* back_h_LRJ_eta = new TH1F("back_h_LRJ_eta", "LRJ Eta Distribution;#eta;Counts", 50, -5, 5);
        TH1F* back_h_LRJ_phi = new TH1F("back_h_LRJ_phi", "LRJ Phi Distribution;#phi;Counts", 32, -3.2, 3.2);

        TH1F* back_h_leading_LRJ_gFexLRJ_deltaEt = new TH1F("back_h_leading_LRJ_gFexLRJ_deltaEt", "#Delta E_{T} Leading gFex LRJ, Output LRJ, ;#Delta E_{T} (gFex - JetTagger) [GeV];% of Leading LRJs / 10 GeV", 50, -350, 150);
        TH1F* back_h_leading_LRJ_offlineLRJ_deltaEt = new TH1F("back_h_leading_LRJ_offlineLRJ_deltaEt", "#Delta E_{T} Leading Offline LRJ, Output LRJ, ;#Delta E_{T} (Offline - JetTagger) [GeV];% of Leading LRJs / 10 GeV",  50, -150, 350);

        TH1F* back_h_leading_offlineLRJ_gFexLRJ_deltaEt = new TH1F("back_h_leading_offlineLRJ_gFexLRJ_deltaEt", "#Delta E_{T} Leading gFex LRJ, Output LRJ, ;#Delta E_{T} (Offline - gFEX) [GeV];% of Leading LRJs / 10 GeV", 50, -150, 350);
        TH1F* back_h_leading_offlineLRJ_jFexLRJ_deltaEt = new TH1F("back_h_leading_offlineLRJ_jFexLRJ_deltaEt", "#Delta E_{T} Leading gFex LRJ, Output LRJ, ;#Delta E_{T} (Offline - gFEX) [GeV];% of Leading LRJs / 10 GeV", 50, -150, 350);

        TH1F* back_h_leading_LRJ_gFexLRJ_Et_resolution = new TH1F("back_h_leading_LRJ_gFexLRJ_Et_resolution", "#Delta E_{T} Leading gFex LRJ, Output LRJ, ;#Delta E_{T} (gFEX - JetTagger) / E_{T, gFEX} [GeV];% of Leading LRJs / 0.1", 40, -2, 2);
        TH1F* back_h_leading_LRJ_offlineLRJ_Et_resolution = new TH1F("back_h_leading_LRJ_offlineLRJ_Et_resolution", "#Delta E_{T} Leading Offline LRJ, Output LRJ, ;#Delta E_{T} (Offline - JetTagger) / E_{T, Offline} [GeV];% of Leading LRJs / 0.1", 20, -1, 1);

        TH1F* back_h_leading_offlineLRJ_gFexLRJ_Et_resolution = new TH1F("back_h_leading_offlineLRJ_gFexLRJ_Et_resolution", "#Delta E_{T} Leading gFex LRJ, Output LRJ, ;#Delta E_{T} (Offline - gFEX) / E_{T, Offline} [GeV];% of Leading LRJs / 0.1", 50, -3.5, 1.5);
        TH1F* back_h_leading_offlineLRJ_jFexLRJ_Et_resolution = new TH1F("back_h_leading_offlineLRJ_jFexLRJ_Et_resolution", "#Delta E_{T} Leading gFex LRJ, Output LRJ, ;#Delta E_{T} (Offline - jFEX) / E_{T, Offline} [GeV];% of Leading LRJs / 0.1", 40, -3, 1);

        TH1F* back_h_leading_LRJ_gFexLRJ_deltaR = new TH1F("back_h_leading_LRJ_gFexLRJ_deltaR", "#Delta R Leading gFex LRJ, Output LRJ, ;#Delta R (gFex, JetTagger);% of Leading LRJs / 0.1", 50, 0, 5);
        TH1F* back_h_leading_LRJ_offlineLRJ_deltaR = new TH1F("back_h_leading_LRJ_offlineLRJ_deltaR", "#Delta R Leading Offline LRJ, Output LRJ, ;#Delta R (Offline, JetTagger);% of Leading LRJs / 0.1", 50, 0, 5);

        TH1F* back_h_first_LRJ_jFexSRJ_deltaR = new TH1F("back_h_first_LRJ_jFexSRJ_deltaR", "#Delta R Leading jFex SRJ, Output LRJ, ;#Delta R (Lead. jFex SRJ, JetTagger LRJ);% of Leading LRJs / 0.1", 50, 0, 5);
        TH1F* back_h_second_LRJ_jFexSRJ_deltaR = new TH1F("back_h_second_LRJ_jFexSRJ_deltaR", "#Delta R Subleading jFex SRJ, Output LRJ, ;#Delta R (Sublead. jFex SRJ, JetTagger LRJ);% of Leading LRJs / 0.1", 50, 0, 5);

        TH1F* back_h_lead_sublead_LRJ_deltaR = new TH1F("back_h_lead_sublead_LRJ_deltaR", "#Delta R Leading, Subleading LRJ, ;#Delta R (Lead., Sublead. Output LRJ);% of Leading LRJs / 0.1", 50, 0, 5);

        TH1F* back_h_Mjj = new TH1F("back_h_Mjj", "Invariant Mass of 2 LRJs;Invariant Mass of 2 LRJs [GeV]; % Events / 20 GeV", 75, 0, 1500);

        TH2F *backTopo422Highest128SeedPositionsEv0 = new TH2F("backTopo422Highest128SeedPositionsEv0", "Sum of Topo422 E_{T} in Each Bin; #eta;#phi", 
                            100, -5, 5,   // 100 bins from -5 to 5 on eta axis
                            64, -3.2, 3.2);  // 64 bins from -3.2 to 3.2 on phi axis

        std::vector<std::pair<double, double > > jFexSeedPositionsBackEv0;
        std::vector<std::pair<double, double > > jFexAdditionalSRJPositionsBackEv0;
        std::vector<std::pair<double, double > > newSeedPositionsBackEv0;
        std::vector<double > backLRJEtEv0;

        TH2F *backTopo422Highest128SeedPositionsEv1 = new TH2F("backTopo422Highest128SeedPositionsEv1", "Sum of Topo422 E_{T} in Each Bin; #eta;#phi", 
                            100, -5, 5,   // 100 bins from -5 to 5 on eta axis
                            64, -3.2, 3.2);  // 64 bins from -3.2 to 3.2 on phi axis

        std::vector<std::pair<double, double  > > jFexSeedPositionsBackEv1;
        std::vector<std::pair<double, double  > > jFexAdditionalSRJPositionsBackEv1;
        std::vector<std::pair<double, double  > > newSeedPositionsBackEv1;
        std::vector<double > backLRJEtEv1;

        TH2F *backTopo422Highest128SeedPositionsEv2 = new TH2F("backTopo422Highest128SeedPositionsEv2", "Sum of Topo422 E_{T} in Each Bin; #eta;#phi", 
                            100, -5, 5,   // 100 bins from -5 to 5 on eta axis
                            64, -3.2, 3.2);  // 64 bins from -3.2 to 3.2 on phi axis
        
        std::vector<std::pair<double, double  > > jFexSeedPositionsBackEv2;
        std::vector<std::pair<double, double  > > jFexAdditionalSRJPositionsBackEv2;
        std::vector<std::pair<double, double  > > newSeedPositionsBackEv2;
        std::vector<double > backLRJEtEv2;

        TH2F *backTopo422Highest128SeedPositionsEv3 = new TH2F("backTopo422Highest128SeedPositionsEv3", "Sum of Topo422 E_{T} in Each Bin; #eta;#phi", 
                            100, -5, 5,   // 100 bins from -5 to 5 on eta axis
                            64, -3.2, 3.2);  // 64 bins from -3.2 to 3.2 on phi axis

        std::vector<std::pair<double, double  > > jFexSeedPositionsBackEv3;
        std::vector<std::pair<double, double  > > jFexAdditionalSRJPositionsBackEv3;
        std::vector<std::pair<double, double  > > newSeedPositionsBackEv3;
        std::vector<double > backLRJEtEv3;

        TH2F *backTopo422Highest128SeedPositionsEv4 = new TH2F("backTopo422Highest128SeedPositionsEv4", "Sum of Topo422 E_{T} in Each Bin; #eta;#phi", 
                            100, -5, 5,   // 100 bins from -5 to 5 on eta axis
                            64, -3.2, 3.2);  // 64 bins from -3.2 to 3.2 on phi axis

        std::vector<std::pair<double, double  > > jFexSeedPositionsBackEv4;
        std::vector<std::pair<double, double  > > jFexAdditionalSRJPositionsBackEv4;
        std::vector<std::pair<double, double  > > newSeedPositionsBackEv4;
        std::vector<double > backLRJEtEv4;

        //if (fileIt > 0) break;
        std::cout << "-------------------------------------" << "\n";
        std::cout << "processing next file" << "\n";
        std::cout << "backgroundFileName: " << backgroundRootFileNames[fileIt] << "\n";
        vector<vector<double> > sig_LRJ_Et(num_processed_events_signal);
        vector<vector<double> > back_LRJ_Et(num_processed_events_background);

        vector<vector<double> > sig_leading_LRJ_Et(num_processed_events_signal);
        vector<vector<double> > back_leading_LRJ_Et(num_processed_events_background);

        vector<vector<double> > sig_subleading_LRJ_Et(num_processed_events_signal);
        vector<vector<double> > back_subleading_LRJ_Et(num_processed_events_background);

        vector<vector<double> > sig_LRJ_Eta(num_processed_events_signal);
        vector<vector<double> > back_LRJ_Eta(num_processed_events_background);

        vector<vector<double> > sig_LRJ_Phi(num_processed_events_signal);
        vector<vector<double> > back_LRJ_Phi(num_processed_events_background);
        //std::cout << "fileIt : " << fileIt << "\n";
        // Find position of "largeR"
        std::string key = "16130_";
        size_t pos = backgroundRootFileNames[fileIt].find(key);

        if (pos != std::string::npos) {
            // Start after "largeR"
            size_t start = pos + key.length();
            
            // Remove .dat by finding its position
            size_t end =  backgroundRootFileNames[fileIt].rfind(".root");

            if (end != std::string::npos && end > start) {
                algorithmConfigurations.push_back( backgroundRootFileNames[fileIt].substr(start, end - start));
                //std::cout << "Extracted string: " << backgroundRootFileNames[fileIt].substr(start, end - start) << std::endl;
            } else {
                std::cerr << ".root not found or too early in string.\n";
            }
        } else {
            std::cerr << "\"16130_\" not found in string.\n";
        }

        /// FILEIT LOOP HERE
    
        std::ifstream sig_infile(signalRootFileNames[fileIt]);
        std::ifstream back_infile(backgroundRootFileNames[fileIt]);
        //std::cout << "processing: " << signalRootFileNames[fileIt] << " and: " << backgroundRootFileNames[fileIt] << "\n";
        if (!sig_infile.is_open()) { 
            std::cerr << "Error: Could not open file " << signalRootFileNames[fileIt] << std::endl;
            return;
        }
        if (!back_infile.is_open()){
            std::cerr << "Error: Could not open file " << backgroundRootFileNames[fileIt] << std::endl;
            return;
        }

        for(unsigned int iEvt = 0; iEvt < num_processed_events_signal; iEvt ++ ){
            jetTaggerLRJsSignal->GetEntry(iEvt);
            jetTaggerLeadingLRJsSignal->GetEntry(iEvt);
            jetTaggerSubleadingLRJsSignal->GetEntry(iEvt);
            gFexLeadingLRJTreeSignal->GetEntry(iEvt);
            gFexSubleadingLRJTreeSignal->GetEntry(iEvt);
            jFexLeadingLRJTreeSignal->GetEntry(iEvt);
            jFexSubleadingLRJTreeSignal->GetEntry(iEvt);


            sig_h_leading_LRJ_Et->Fill(jetTaggerLeadingLRJEtValuesSignal->at(0));
            sig_h_subleading_LRJ_Et->Fill(jetTaggerSubleadingLRJEtValuesSignal->at(0));

            sig_h_gFEX_leading_LRJ_Et->Fill(gFexLRJLeadingEtValuesSignal->at(0)); 
            sig_h_gFEX_subleading_LRJ_Et->Fill(gFexLRJSubleadingEtValuesSignal->at(0)); 

            if(jFexLRJLeadingEtValuesSignal->size() > 0) sig_h_jFEX_leading_LRJ_Et->Fill(jFexLRJLeadingEtValuesSignal->at(0)); 
            if(jFexLRJSubleadingEtValuesSignal->size() > 0) sig_h_jFEX_subleading_LRJ_Et->Fill(jFexLRJSubleadingEtValuesSignal->at(0));
            
            for(unsigned int iSigLRJ = 0; iSigLRJ < jetTaggerLRJDiamValuesSignal->size(); iSigLRJ++){
                sig_h_LRJ_substruct->Fill(jetTaggerLRJDiamValuesSignal->at(iSigLRJ)); 
                sigDiamvsEt->Fill(jetTaggerLRJEtValuesSignal->at(iSigLRJ), jetTaggerLRJDiamValuesSignal->at(iSigLRJ));
                sig_h_LRJ_Et->Fill(jetTaggerLRJEtValuesSignal->at(iSigLRJ));
                sig_h_LRJ_E->Fill(jetTaggerLRJEtValuesSignal->at(iSigLRJ) * cosh(jetTaggerLRJEtaValuesSignal->at(iSigLRJ)));
                sig_LRJ_Et[iEvt].push_back(jetTaggerLRJEtValuesSignal->at(iSigLRJ));
                sig_LRJ_Eta[iEvt].push_back(jetTaggerLRJEtaValuesSignal->at(iSigLRJ));
                sig_LRJ_Phi[iEvt].push_back(jetTaggerLRJPhiValuesSignal->at(iSigLRJ));
                sig_h_LRJ_eta->Fill(jetTaggerLRJEtaValuesSignal->at(iSigLRJ));
                sig_h_LRJ_phi->Fill(jetTaggerLRJPhiValuesSignal->at(iSigLRJ));
            }

            
        }

        for(unsigned int iEvt = 0; iEvt < num_processed_events_background; iEvt ++ ){ 
            jetTaggerLRJsBack->GetEntry(iEvt);
            jetTaggerLeadingLRJsBack->GetEntry(iEvt);
            jetTaggerSubleadingLRJsBack->GetEntry(iEvt);
            gFexLeadingLRJTreeBack->GetEntry(iEvt);
            gFexSubleadingLRJTreeBack->GetEntry(iEvt);
            jFexLeadingLRJTreeBack->GetEntry(iEvt);
            jFexSubleadingLRJTreeBack->GetEntry(iEvt);

            eventInfoTreeBack->GetEntry(iEvt);

            double backgroundEventWeight = eventWeightsValuesBack->at(0);

            back_h_leading_LRJ_Et->Fill(jetTaggerLeadingLRJEtValuesBack->at(0), backgroundEventWeight); 
            back_h_subleading_LRJ_Et->Fill(jetTaggerSubleadingLRJEtValuesBack->at(0), backgroundEventWeight);

            back_h_gFEX_leading_LRJ_Et->Fill(gFexLRJLeadingEtValuesBack->at(0), backgroundEventWeight); 
            back_h_gFEX_subleading_LRJ_Et->Fill(gFexLRJSubleadingEtValuesBack->at(0), backgroundEventWeight); 

            if(jFexLRJLeadingEtValuesBack->size() > 0) back_h_jFEX_leading_LRJ_Et->Fill(jFexLRJLeadingEtValuesBack->at(0), backgroundEventWeight); 
            if(jFexLRJSubleadingEtValuesBack->size() > 0) back_h_jFEX_subleading_LRJ_Et->Fill(jFexLRJSubleadingEtValuesBack->at(0), backgroundEventWeight); 

            for(unsigned int iBackLRJ = 0; iBackLRJ < jetTaggerLRJDiamValuesBack->size(); iBackLRJ++){
                back_h_LRJ_substruct->Fill(jetTaggerLRJDiamValuesBack->at(iBackLRJ), backgroundEventWeight);
                backDiamvsEt->Fill(jetTaggerLRJEtValuesBack->at(iBackLRJ), jetTaggerLRJDiamValuesBack->at(iBackLRJ), backgroundEventWeight);
                back_h_LRJ_Et->Fill(jetTaggerLRJEtValuesBack->at(iBackLRJ), backgroundEventWeight);
                back_h_LRJ_E->Fill(jetTaggerLRJEtValuesBack->at(iBackLRJ) * cosh(jetTaggerLRJEtaValuesBack->at(iBackLRJ)), backgroundEventWeight);
                back_LRJ_Et[iEvt].push_back(jetTaggerLRJEtValuesBack->at(iBackLRJ));
                back_LRJ_Eta[iEvt].push_back(jetTaggerLRJEtaValuesBack->at(iBackLRJ));
                back_LRJ_Phi[iEvt].push_back(jetTaggerLRJPhiValuesBack->at(iBackLRJ));
                back_h_LRJ_eta->Fill(jetTaggerLRJPhiValuesBack->at(iBackLRJ), backgroundEventWeight);
                back_h_LRJ_phi->Fill(jetTaggerLRJEtaValuesBack->at(iBackLRJ), backgroundEventWeight);
            }
        }
            
        // ---- helpers --------------------------------------------------------------
        auto SetAxes = [](TAxis* ax, const char* title) {
        ax->SetTitle(title);
        ax->SetTitleFont(42);
        ax->SetLabelFont(42);
        ax->SetTitleSize(0.05);
        ax->SetLabelSize(0.05);
        ax->SetTitleColor(kBlack);
        ax->SetLabelColor(kBlack);
        ax->SetNdivisions(510);
        ax->SetTitleOffset(1.25);
        };

        TString rateVsEffFileDir = "rateVsEff_" + algorithmConfigurations[fileIt] + "/";

        gSystem->mkdir(rateVsEffFileDir);

        // ---- LEADING --------------------------------------------------------------
        auto out = MakeRateVsEff(sig_h_leading_LRJ_Et, back_h_leading_LRJ_Et);

        // Titles & colors for TH1s
        SetAxes(out.hEff_vsThr ->GetXaxis(), "Leading JetTagger LRJ E_{T} threshold [GeV]");
        SetAxes(out.hEff_vsThr ->GetYaxis(), "Signal (hh#rightarrow4b) Efficiency");
        SetAxes(out.hRate_vsThr->GetXaxis(), "Leading JetTagger LRJ E_{T} threshold [GeV]");
        SetAxes(out.hRate_vsThr->GetYaxis(), "Estimated Background Rate [Hz]");

        // Titles & colors for TGraph
        SetAxes(out.gRate_vsEff->GetXaxis(), "Signal (hh#rightarrow4b) Efficiency");
        SetAxes(out.gRate_vsEff->GetYaxis(), "Estimated Background Rate [Hz]");

        // Optional: vs-threshold views
        auto c1 = new TCanvas("c1","vs threshold",1200,500);
        c1->Divide(2,1);
        c1->cd(1);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        out.hEff_vsThr->Draw("HIST");  // draw as histogram
        c1->cd(2);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        gPad->SetLogy();
        out.hRate_vsThr->Draw("HIST");
        c1->SaveAs(rateVsEffFileDir + "threshold_views.pdf");

        // Main plot: rate vs efficiency (graph)
        auto c2 = new TCanvas("c2","Rate vs Eff",700,600);
        c2->SetLeftMargin(0.16); c2->SetBottomMargin(0.16); c2->SetTicks(1,1);
        c2->SetLogy();
        out.gRate_vsEff->Draw("AP");   // A=axes, P=points
        c2->SaveAs(rateVsEffFileDir + "rate_vs_eff.pdf");

        // ---- SUBLEADING -----------------------------------------------------------
        auto out_subleading = MakeRateVsEff(sig_h_subleading_LRJ_Et, back_h_subleading_LRJ_Et);

        // TH1s
        SetAxes(out_subleading.hEff_vsThr ->GetXaxis(), "Subleading JetTagger LRJ E_{T} threshold [GeV]");
        SetAxes(out_subleading.hEff_vsThr ->GetYaxis(), "Signal (hh#rightarrow4b) Efficiency");
        SetAxes(out_subleading.hRate_vsThr->GetXaxis(), "Subleading JetTagger LRJ E_{T} threshold [GeV]");
        SetAxes(out_subleading.hRate_vsThr->GetYaxis(), "Estimated Background Rate [Hz]");

        // Graph
        SetAxes(out_subleading.gRate_vsEff->GetXaxis(), "Signal (hh#rightarrow4b) Efficiency");
        SetAxes(out_subleading.gRate_vsEff->GetYaxis(), "Estimated Background Rate [Hz]");

        // Optional: vs-threshold views
        auto c1_subleading = new TCanvas("c1_subleading","vs threshold",1200,500);
        c1_subleading->Divide(2,1);
        c1_subleading->cd(1);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        out_subleading.hEff_vsThr->Draw("HIST");
        c1_subleading->cd(2);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        gPad->SetLogy();
        out_subleading.hRate_vsThr->Draw("HIST");
        c1_subleading->SaveAs(rateVsEffFileDir + "threshold_views_subleading.pdf");

        // Main plot
        auto c2_subleading = new TCanvas("c2_subleading","Rate vs Eff",700,600);
        c2_subleading->SetLeftMargin(0.16); c2_subleading->SetBottomMargin(0.16); c2_subleading->SetTicks(1,1);
        c2_subleading->SetLogy();
        out_subleading.gRate_vsEff->Draw("AP");
        c2_subleading->SaveAs(rateVsEffFileDir + "rate_vs_eff_subleading.pdf");



        // ---- gFEX LEADING --------------------------------------------------------------
        auto out_gFEX_leading = MakeRateVsEff(sig_h_gFEX_leading_LRJ_Et, back_h_gFEX_leading_LRJ_Et);

        // Titles & colors for TH1s
        SetAxes(out_gFEX_leading.hEff_vsThr ->GetXaxis(), "Leading gFEX LRJ E_{T} threshold [GeV]");
        SetAxes(out_gFEX_leading.hEff_vsThr ->GetYaxis(), "Signal (hh#rightarrow4b) Efficiency");
        SetAxes(out_gFEX_leading.hRate_vsThr->GetXaxis(), "Leading gFEX LRJ E_{T} threshold [GeV]");
        SetAxes(out_gFEX_leading.hRate_vsThr->GetYaxis(), "Estimated Background Rate [Hz]");

        // Titles & colors for TGraph
        SetAxes(out_gFEX_leading.gRate_vsEff->GetXaxis(), "Signal (hh#rightarrow4b) Efficiency");
        SetAxes(out_gFEX_leading.gRate_vsEff->GetYaxis(), "Estimated Background Rate [Hz]");

        // Optional: vs-threshold views
        auto c1_gFEX_leading = new TCanvas("c1_gFEX_leading","vs threshold",1200,500);
        c1_gFEX_leading->Divide(2,1);
        c1_gFEX_leading->cd(1);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        out_gFEX_leading.hEff_vsThr->Draw("HIST");  // draw as histogram
        c1_gFEX_leading->cd(2);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        gPad->SetLogy();
        out_gFEX_leading.hRate_vsThr->Draw("HIST");
        c1_gFEX_leading->SaveAs(rateVsEffFileDir + "threshold_views_gfex_leading.pdf");

        // Main plot: rate vs efficiency (graph)
        auto c2_gFEX_leading = new TCanvas("c2_gFEX_leading","Rate vs Eff",700,600);
        c2_gFEX_leading->SetLeftMargin(0.16); c2_gFEX_leading->SetBottomMargin(0.16); c2_gFEX_leading->SetTicks(1,1);
        c2_gFEX_leading->SetLogy();
        out_gFEX_leading.gRate_vsEff->Draw("AP");   // A=axes, P=points
        c2_gFEX_leading->SaveAs(rateVsEffFileDir + "rate_vs_eff_gfex_leading.pdf");


        // ---- jFEX LEADING --------------------------------------------------------------
        auto out_jFEX_leading = MakeRateVsEff(sig_h_jFEX_leading_LRJ_Et, back_h_jFEX_leading_LRJ_Et);

        // Titles & colors for TH1s
        SetAxes(out_jFEX_leading.hEff_vsThr ->GetXaxis(), "Leading jFEX LRJ E_{T} threshold [GeV]");
        SetAxes(out_jFEX_leading.hEff_vsThr ->GetYaxis(), "Signal (hh#rightarrow4b) Efficiency");
        SetAxes(out_jFEX_leading.hRate_vsThr->GetXaxis(), "Leading jFEX LRJ E_{T} threshold [GeV]");
        SetAxes(out_jFEX_leading.hRate_vsThr->GetYaxis(), "Estimated Background Rate [Hz]");

        // Titles & colors for TGraph
        SetAxes(out_jFEX_leading.gRate_vsEff->GetXaxis(), "Signal (hh#rightarrow4b) Efficiency");
        SetAxes(out_jFEX_leading.gRate_vsEff->GetYaxis(), "Estimated Background Rate [Hz]");

        // Optional: vs-threshold views
        auto c1_jFEX_leading = new TCanvas("c1_jFEX_leading","vs threshold",1200,500);
        c1_jFEX_leading->Divide(2,1);
        c1_jFEX_leading->cd(1);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        out_jFEX_leading.hEff_vsThr->Draw("HIST");  // draw as histogram
        c1_jFEX_leading->cd(2);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        gPad->SetLogy();
        out_jFEX_leading.hRate_vsThr->Draw("HIST");
        c1_jFEX_leading->SaveAs(rateVsEffFileDir + "threshold_views_jFEX_leading.pdf");

        // Main plot: rate vs efficiency (graph)
        auto c2_jFEX_leading = new TCanvas("c2_jFEX_leading","Rate vs Eff",700,600);
        c2_jFEX_leading->SetLeftMargin(0.16); c2_jFEX_leading->SetBottomMargin(0.16); c2_jFEX_leading->SetTicks(1,1);
        c2_jFEX_leading->SetLogy();
        out_jFEX_leading.gRate_vsEff->Draw("AP");   // A=axes, P=points
        c2_jFEX_leading->SaveAs(rateVsEffFileDir + "rate_vs_eff_jFEX_leading.pdf");


        // ---- gFEX SUBLEADING --------------------------------------------------------------
        auto out_gFEX_subleading = MakeRateVsEff(sig_h_gFEX_subleading_LRJ_Et, back_h_gFEX_subleading_LRJ_Et);

        // Titles & colors for TH1s
        SetAxes(out_gFEX_subleading.hEff_vsThr ->GetXaxis(), "Subleading gFEX LRJ E_{T} threshold [GeV]");
        SetAxes(out_gFEX_subleading.hEff_vsThr ->GetYaxis(), "Signal (hh#rightarrow4b) Efficiency");
        SetAxes(out_gFEX_subleading.hRate_vsThr->GetXaxis(), "Subleading gFEX LRJ E_{T} threshold [GeV]");
        SetAxes(out_gFEX_subleading.hRate_vsThr->GetYaxis(), "Estimated Background Rate [Hz]");

        // Titles & colors for TGraph
        SetAxes(out_gFEX_subleading.gRate_vsEff->GetXaxis(), "Signal (hh#rightarrow4b) Efficiency");
        SetAxes(out_gFEX_subleading.gRate_vsEff->GetYaxis(), "Estimated Background Rate [Hz]");

        // Optional: vs-threshold views
        auto c1_gFEX_subleading = new TCanvas("c1_gFEX_subleading","vs threshold",1200,500);
        c1_gFEX_subleading->Divide(2,1);
        c1_gFEX_subleading->cd(1);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        out_gFEX_subleading.hEff_vsThr->Draw("HIST");  // draw as histogram
        c1_gFEX_subleading->cd(2);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        gPad->SetLogy();
        out_gFEX_subleading.hRate_vsThr->Draw("HIST");
        c1_gFEX_subleading->SaveAs(rateVsEffFileDir + "threshold_views_gfex_subleading.pdf");

        // Main plot: rate vs efficiency (graph)
        auto c2_gFEX_subleading = new TCanvas("c2_gFEX_subleading","Rate vs Eff",700,600);
        c2_gFEX_subleading->SetLeftMargin(0.16); c2_gFEX_subleading->SetBottomMargin(0.16); c2_gFEX_subleading->SetTicks(1,1);
        c2_gFEX_subleading->SetLogy();
        out_gFEX_subleading.gRate_vsEff->Draw("AP");   // A=axes, P=points
        c2_gFEX_subleading->SaveAs(rateVsEffFileDir + "rate_vs_eff_gfex_subleading.pdf");

        // ---- jFEX SUBLEADING --------------------------------------------------------------
        auto out_jFEX_subleading = MakeRateVsEff(sig_h_jFEX_subleading_LRJ_Et, back_h_jFEX_subleading_LRJ_Et);

        // Titles & colors for TH1s
        SetAxes(out_jFEX_subleading.hEff_vsThr ->GetXaxis(), "Subleading jFEX LRJ E_{T} threshold [GeV]");
        SetAxes(out_jFEX_subleading.hEff_vsThr ->GetYaxis(), "Signal (hh#rightarrow4b) Efficiency");
        SetAxes(out_jFEX_subleading.hRate_vsThr->GetXaxis(), "Subleading jFEX LRJ E_{T} threshold [GeV]");
        SetAxes(out_jFEX_subleading.hRate_vsThr->GetYaxis(), "Estimated Background Rate [Hz]");

        // Titles & colors for TGraph
        SetAxes(out_jFEX_subleading.gRate_vsEff->GetXaxis(), "Signal (hh#rightarrow4b) Efficiency");
        SetAxes(out_jFEX_subleading.gRate_vsEff->GetYaxis(), "Estimated Background Rate [Hz]");

        // Optional: vs-threshold views
        auto c1_jFEX_subleading = new TCanvas("c1_jFEX_subleading","vs threshold",1200,500);
        c1_jFEX_subleading->Divide(2,1);
        c1_jFEX_subleading->cd(1);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        out_jFEX_subleading.hEff_vsThr->Draw("HIST");  // draw as histogram
        c1_jFEX_subleading->cd(2);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        gPad->SetLogy();
        out_jFEX_subleading.hRate_vsThr->Draw("HIST");
        c1_jFEX_subleading->SaveAs(rateVsEffFileDir + "threshold_views_jFEX_subleading.pdf");

        // Main plot: rate vs efficiency (graph)
        auto c2_jFEX_subleading = new TCanvas("c2_jFEX_subleading","Rate vs Eff",700,600);
        c2_jFEX_subleading->SetLeftMargin(0.16); c2_jFEX_subleading->SetBottomMargin(0.16); c2_jFEX_subleading->SetTicks(1,1);
        c2_jFEX_subleading->SetLogy();
        out_jFEX_subleading.gRate_vsEff->Draw("AP");   // A=axes, P=points
        c2_jFEX_subleading->SaveAs(rateVsEffFileDir + "rate_vs_eff_jFEX_subleading.pdf");


        // ******************** OVERLAYS (LEADING only): JetTagger vs gFEX vs jFEX ********************
        // Colors/markers per algorithm
        const Int_t cols[3]    = { kBlack, kRed+1, kBlue+1 };
        const Int_t mstyles[3] = { 20, 21, 22 };
        const char* labels[3]  = { "JetTagger LRJ", "gFEX LRJ", "jFEX LRJ" };

        // Convenience handles
        TH1*   effH [3] = { out.hEff_vsThr,        out_gFEX_leading.hEff_vsThr,        out_jFEX_leading.hEff_vsThr };
        TH1*   rateH[3] = { out.hRate_vsThr,       out_gFEX_leading.hRate_vsThr,       out_jFEX_leading.hRate_vsThr };
        TGraph* rocG[3] = { out.gRate_vsEff,       out_gFEX_leading.gRate_vsEff,       out_jFEX_leading.gRate_vsEff };

        // ---------- Overlay: Efficiency/Rate vs Threshold ----------
        auto cLeadThrOverlay = new TCanvas("cLeadThrOverlay","Leading: thresholds overlay",1200,500);
        cLeadThrOverlay->Divide(2,1);

        // Left pad: efficiency vs threshold
        cLeadThrOverlay->cd(1);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);

        for (int i=0;i<3;++i) {
            effH[i]->SetLineColor(cols[i]);
            effH[i]->SetLineWidth(2);
        }
        // draw first to create axes with titles already set on JetTagger hist
        effH[0]->GetXaxis()->SetTitle("Leading LRJ E_{T} Threshold [GeV]");
        effH[0]->Draw("HIST");
        effH[1]->Draw("HIST SAME");
        effH[2]->Draw("HIST SAME");

        // legend (eff)
        auto legEff = new TLegend(0.55,0.72,0.88,0.88);
        legEff->SetBorderSize(0); legEff->SetFillStyle(0); legEff->SetTextSize(0.04);
        legEff->AddEntry(effH[0], labels[0], "l");
        legEff->AddEntry(effH[1], labels[1], "l");
        legEff->AddEntry(effH[2], labels[2], "l");
        legEff->Draw();

        // Right pad: rate vs threshold
        cLeadThrOverlay->cd(2);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        gPad->SetLogy();

        for (int i=0;i<3;++i) {
            rateH[i]->SetLineColor(cols[i]);
            rateH[i]->SetLineWidth(2);
        }
        rateH[0]->GetXaxis()->SetTitle("Leading LRJ E_{T} Threshold [GeV]");
        rateH[0]->Draw("HIST");
        rateH[1]->Draw("HIST SAME");
        rateH[2]->Draw("HIST SAME");

        // legend (rate)
        auto legRate = new TLegend(0.55,0.72,0.88,0.88);
        legRate->SetBorderSize(0); legRate->SetFillStyle(0); legRate->SetTextSize(0.04);
        legRate->AddEntry(rateH[0], labels[0], "l");
        legRate->AddEntry(rateH[1], labels[1], "l");
        legRate->AddEntry(rateH[2], labels[2], "l");
        legRate->Draw();

        cLeadThrOverlay->SaveAs(rateVsEffFileDir + "leading_threshold_overlays.pdf");

        // ---------- Overlay: Rate vs Efficiency (ROC-like) ----------
        auto cLeadRocOverlay = new TCanvas("cLeadRocOverlay","Leading: rate vs eff overlay",700,600);
        cLeadRocOverlay->SetLeftMargin(0.16); cLeadRocOverlay->SetBottomMargin(0.16);
        cLeadRocOverlay->SetTicks(1,1);
        cLeadRocOverlay->SetLogy();

        // style graphs
        for (int i=0;i<3;++i) {
            rocG[i]->SetMarkerStyle(mstyles[i]);
            rocG[i]->SetMarkerSize(1.1);
            rocG[i]->SetMarkerColor(cols[i]);
            rocG[i]->SetLineColor(cols[i]);
            rocG[i]->SetLineWidth(2);
        }

        // draw first one with axes, others with SAME
        rocG[0]->Draw("AP");     // axes + points
        rocG[1]->Draw("P SAME");
        rocG[2]->Draw("P SAME");

        // legend (roc)
        auto legROC = new TLegend(0.53,0.37,0.86,0.53);
        legROC->SetBorderSize(0); legROC->SetFillStyle(0); legROC->SetTextSize(0.04);
        legROC->AddEntry(rocG[0], labels[0], "lp");
        legROC->AddEntry(rocG[1], labels[1], "lp");
        legROC->AddEntry(rocG[2], labels[2], "lp");
        legROC->Draw();

        cLeadRocOverlay->SaveAs(rateVsEffFileDir + "leading_rate_vs_eff_overlay.pdf");

        // ******************** OVERLAYS (SUBLEADING): JetTagger vs gFEX vs jFEX ********************
        // Convenience handles (SUBLEADING)
        TH1*    effH_sub [3] = { out_subleading.hEff_vsThr,        out_gFEX_subleading.hEff_vsThr,        out_jFEX_subleading.hEff_vsThr };
        TH1*    rateH_sub[3] = { out_subleading.hRate_vsThr,       out_gFEX_subleading.hRate_vsThr,       out_jFEX_subleading.hRate_vsThr };
        TGraph* rocG_sub [3] = { out_subleading.gRate_vsEff,       out_gFEX_subleading.gRate_vsEff,       out_jFEX_subleading.gRate_vsEff };

        // ---------- Overlay: Efficiency/Rate vs Threshold ----------
        auto cSubLeadThrOverlay = new TCanvas("cSubLeadThrOverlay","Subleading: thresholds overlay",1200,500);
        cSubLeadThrOverlay->Divide(2,1);

        // Left pad: efficiency vs threshold
        cSubLeadThrOverlay->cd(1);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);

        for (int i=0;i<3;++i) {
        effH_sub[i]->SetLineColor(cols[i]);
        effH_sub[i]->SetLineWidth(2);
        }
        // draw first to set axes; override X title to emphasize subleading
        effH_sub[0]->GetXaxis()->SetTitle("Subleading LRJ E_{T} Threshold [GeV]");
        effH_sub[0]->GetYaxis()->SetTitle("Signal (hh#rightarrow4b) Efficiency");
        effH_sub[0]->Draw("HIST");
        effH_sub[1]->Draw("HIST SAME");
        effH_sub[2]->Draw("HIST SAME");

        // legend (eff)
        auto legEff_sub = new TLegend(0.55,0.72,0.88,0.88);
        legEff_sub->SetBorderSize(0); legEff_sub->SetFillStyle(0); legEff_sub->SetTextSize(0.04);
        legEff_sub->AddEntry(effH_sub[0], labels[0], "l");
        legEff_sub->AddEntry(effH_sub[1], labels[1], "l");
        legEff_sub->AddEntry(effH_sub[2], labels[2], "l");
        legEff_sub->Draw();

        // Right pad: rate vs threshold
        cSubLeadThrOverlay->cd(2);
        gPad->SetLeftMargin(0.16); gPad->SetBottomMargin(0.16); gPad->SetTicks(1,1);
        gPad->SetLogy();

        for (int i=0;i<3;++i) {
        rateH_sub[i]->SetLineColor(cols[i]);
        rateH_sub[i]->SetLineWidth(2);
        }
        rateH_sub[0]->GetXaxis()->SetTitle("Subleading LRJ E_{T} Threshold [GeV]");
        rateH_sub[0]->GetYaxis()->SetTitle("Estimated Background Rate [Hz]");
        rateH_sub[0]->Draw("HIST");
        rateH_sub[1]->Draw("HIST SAME");
        rateH_sub[2]->Draw("HIST SAME");

        // legend (rate)
        auto legRate_sub = new TLegend(0.55,0.72,0.88,0.88);
        legRate_sub->SetBorderSize(0); legRate_sub->SetFillStyle(0); legRate_sub->SetTextSize(0.04);
        legRate_sub->AddEntry(rateH_sub[0], labels[0], "l");
        legRate_sub->AddEntry(rateH_sub[1], labels[1], "l");
        legRate_sub->AddEntry(rateH_sub[2], labels[2], "l");
        legRate_sub->Draw();

        cSubLeadThrOverlay->SaveAs(rateVsEffFileDir + "subleading_threshold_overlays.pdf");

        // ---------- Overlay: Rate vs Efficiency (ROC-like) ----------
        auto cSubLeadRocOverlay = new TCanvas("cSubLeadRocOverlay","Subleading: rate vs eff overlay",700,600);
        cSubLeadRocOverlay->SetLeftMargin(0.16); cSubLeadRocOverlay->SetBottomMargin(0.16);
        cSubLeadRocOverlay->SetTicks(1,1);
        cSubLeadRocOverlay->SetLogy();

        // style graphs
        for (int i=0;i<3;++i) {
        rocG_sub[i]->SetMarkerStyle(mstyles[i]);
        rocG_sub[i]->SetMarkerSize(1.1);
        rocG_sub[i]->SetMarkerColor(cols[i]);
        rocG_sub[i]->SetLineColor(cols[i]);
        rocG_sub[i]->SetLineWidth(2);
        }

        // draw first one with axes, others with SAME; set axes titles for clarity
        rocG_sub[0]->Draw("AP");
        rocG_sub[0]->GetXaxis()->SetTitle("Signal (hh#rightarrow4b) Efficiency");
        rocG_sub[0]->GetYaxis()->SetTitle("Estimated Background Rate [Hz]");
        rocG_sub[1]->Draw("P SAME");
        rocG_sub[2]->Draw("P SAME");

        // legend (roc)
        auto legROC_sub = new TLegend(0.53,0.37,0.86,0.53);
        legROC_sub->SetBorderSize(0); legROC_sub->SetFillStyle(0); legROC_sub->SetTextSize(0.04);
        legROC_sub->AddEntry(rocG_sub[0], labels[0], "lp");
        legROC_sub->AddEntry(rocG_sub[1], labels[1], "lp");
        legROC_sub->AddEntry(rocG_sub[2], labels[2], "lp");
        legROC_sub->Draw();

        cSubLeadRocOverlay->SaveAs(rateVsEffFileDir + "subleading_rate_vs_eff_overlay.pdf");

        // ================== COMBINED OVERLAY: Rate vs Efficiency (all 6) ==================
        // Curves: (JetTagger, gFEX, jFEX) × (Leading, Subleading)
        // Uses: out, out_subleading, out_gFEX_leading, out_gFEX_subleading, out_jFEX_leading, out_jFEX_subleading

        // ---- Style palette ----
        // Colors per algorithm
        const Int_t algoCols[3] = { kBlack, kRed+1, kBlue+1 }; // JetTagger, gFEX, jFEX

        // Marker styles: leading vs subleading per algorithm
        const Int_t mLead [3] = { 20, 21, 22 };  // solid markers
        const Int_t mSub  [3] = { 24, 25, 26 };  // open markers

        // Line styles: leading=solid, subleading=dashed
        const Int_t lLead = 1;
        const Int_t lSub  = 2;

        // Labels
        const char* algoNames[3] = { "JetTagger LRJ", "gFEX LRJ", "jFEX LRJ" };

        // Convenience handles (rate vs eff graphs)
        TGraph* gLead[3] = { out.gRate_vsEff,            out_gFEX_leading.gRate_vsEff,      out_jFEX_leading.gRate_vsEff      };
        TGraph* gSub [3] = { out_subleading.gRate_vsEff, out_gFEX_subleading.gRate_vsEff,   out_jFEX_subleading.gRate_vsEff   };

        // ---- Canvas ----
        auto cAllROC = new TCanvas("cAllROC","Rate vs Efficiency: Leading & Subleading (All Algos)",800,650);
        cAllROC->SetLeftMargin(0.16); 
        cAllROC->SetBottomMargin(0.16);
        cAllROC->SetTicks(1,1);
        cAllROC->SetLogy();

        // ---- Style & draw ----
        // We’ll draw the first curve with axes ("AP"), then all others as "P SAME"
        bool firstDrawn = false;

        // (A) Leading first (solid)
        for (int i=0; i<3; ++i) {
            if (!gLead[i]) continue;
            gLead[i]->SetMarkerStyle(mLead[i]);
            gLead[i]->SetMarkerSize(1.1);
            gLead[i]->SetMarkerColor(algoCols[i]);
            gLead[i]->SetLineColor(algoCols[i]);
            gLead[i]->SetLineStyle(lLead);
            gLead[i]->SetLineWidth(2);

            if (!firstDrawn) {
                gLead[i]->Draw("AP"); // axes + points for the very first
                gLead[i]->GetXaxis()->SetTitle("Signal (hh#rightarrow4b) Efficiency");
                gLead[i]->GetYaxis()->SetTitle("Estimated Background Rate [Hz]");
                // Optional axis cosmetics
                gLead[i]->GetXaxis()->SetTitleOffset(1.2);
                gLead[i]->GetYaxis()->SetTitleOffset(1.6);

                // (optional) Y-range on log axis for clarity — adjust as needed
                gLead[i]->SetMinimum(1e-1); // show down to 0.1 Hz
                // gLead[i]->SetMaximum(1e5); // uncomment/adjust if you want an explicit ceiling

                firstDrawn = true;
            } else {
                gLead[i]->Draw("P SAME");
            }
        }

        // (B) Subleading next (dashed)
        for (int i=0; i<3; ++i) {
        if (!gSub[i]) continue;
        gSub[i]->SetMarkerStyle(mSub[i]);
        gSub[i]->SetMarkerSize(1.1);
        gSub[i]->SetMarkerColor(algoCols[i]);
        gSub[i]->SetLineColor(algoCols[i]);
        gSub[i]->SetLineStyle(lSub);
        gSub[i]->SetLineWidth(2);
        gSub[i]->Draw("P SAME");
        }

        // ---- Legend ----
        // Two-column legend grouping by algorithm, showing leading vs subleading
        auto legAll = new TLegend(0.38, 0.20, 0.78, 0.42); // adjust position if needed
        legAll->SetBorderSize(0);
        legAll->SetFillStyle(0);
        legAll->SetTextSize(0.035);

        // Add algorithm-specific entries
        for (int i=0; i<3; ++i) {
        // Leading entry
        auto eL = legAll->AddEntry(gLead[i], Form("%s (leading)",   algoNames[i]), "lp");
        // Subleading entry
        auto eS = legAll->AddEntry(gSub[i],  Form("%s (subleading)",algoNames[i]), "lp");
        }
        legAll->Draw();

        // (optional) Extra label (e.g., experiment/run label)
        // TLatex lat; lat.SetNDC(); lat.SetTextSize(0.04); 
        // lat.DrawLatex(0.18,0.92,"#bf{ATLAS} Internal   #scale[0.9]{Run-3, #sqrt{s}=13.6 TeV}");

        // ---- Save ----
        gPad->RedrawAxis();
        cAllROC->SaveAs(rateVsEffFileDir + "all_algos_leading_subleading_rate_vs_eff_overlay.pdf");


        for (int i = 0; i < num_processed_events_signal; i++) {
            jetTaggerLRJsSignal->GetEntry(i);
            jetTaggerLeadingLRJsSignal->GetEntry(i);
            jetTaggerSubleadingLRJsSignal->GetEntry(i);
            subleadingRecoAntiKt10UFOCSSKJetsSignal->GetEntry(i);
            leadingRecoAntiKt10UFOCSSKJetsSignal->GetEntry(i);
            truthAntiKt4TruthDressedWZJetsSignal->GetEntry(i);
            gFexLeadingLRJTreeSignal->GetEntry(i);
            jFexLeadingLRJTreeSignal->GetEntry(i);
            jFexLeadingSRJTreeSignal->GetEntry(i);
            jFexSubleadingSRJTreeSignal->GetEntry(i);
            gFexSubleadingLRJTreeSignal->GetEntry(i);
            topo422TreeSignal->GetEntry(i);
            gepBasicClustersTreeSignal->GetEntry(i);
            truthbTreeSignal->GetEntry(i);
            truthHiggsTreeSignal->GetEntry(i);
            jFexSRJTreeSignal->GetEntry(i);
            truthbTreeSignal->GetEntry(i);

            unsigned int highestEtIndexLRJSig = -1;
            if(sig_LRJ_Et[i][0] >= sig_LRJ_Et[i][1]) highestEtIndexLRJSig = 0;
            else highestEtIndexLRJSig = 1;

            double jFEXSRJLeadingEta;
            double jFEXSRJLeadingPhi;
            if(jFexSRJLeadingEtaValuesSignal->size() > 0){
                jFEXSRJLeadingEta = jFexSRJLeadingEtaValuesSignal->at(0);
            
                jFEXSRJLeadingPhi = jFexSRJLeadingPhiValuesSignal->at(0);
            }
            else{
                jFEXSRJLeadingEta = 0.0;
                jFEXSRJLeadingPhi = 0.0;
            }
             
            double jFEXSRJSubleadingEta; 
            double jFEXSRJSubleadingPhi;
            if(jFexSRJSubleadingEtaValuesSignal->size() > 0){ // Account for the case when there aren't any jFEX subleading jets FIXME do this in the ntupler potentially?
                jFEXSRJSubleadingEta = jFexSRJSubleadingEtaValuesSignal->at(0);
                jFEXSRJSubleadingPhi = jFexSRJSubleadingPhiValuesSignal->at(0);
            }   
            else{
                jFEXSRJSubleadingEta = 0.0;
                jFEXSRJSubleadingPhi = 0.0;
            }

            double psi_R_SigJet1 = 0.0;
            double psi_R_SigJet2 = 0.0;
            double psi_R2_SigJet1 = 0.0;
            double psi_R2_SigJet2 = 0.0;

            // Loop through and compute psi_R for each large-R jet
            for(auto& idxSigJet1 : *jetTaggerLeadingLRJMergedIndicesValuesSignal){ // FIXME need to add in all the pSI_R and psi_R^2 computations!!
                psi_R_SigJet1 += (1.0/jetTaggerLeadingLRJEtValuesSignal->at(0)) * gepBasicClustersEtValuesSignal->at(idxSigJet1) * sqrt(calcDeltaR2(jetTaggerLeadingLRJEtaValuesSignal->at(0), jetTaggerLeadingLRJPhiValuesSignal->at(0), gepBasicClustersEtaValuesSignal->at(idxSigJet1), gepBasicClustersPhiValuesSignal->at(idxSigJet1)));
                psi_R2_SigJet1 += (1.0/jetTaggerLeadingLRJEtValuesSignal->at(0)) * gepBasicClustersEtValuesSignal->at(idxSigJet1) * calcDeltaR2(jFexSRJLeadingEtaValuesSignal->at(0), jFexSRJLeadingPhiValuesSignal->at(0), gepBasicClustersEtaValuesSignal->at(idxSigJet1), gepBasicClustersPhiValuesSignal->at(idxSigJet1));
                //std::cout << "psi_R_SigJet1: " << psi_R_SigJet1 << "\n";
            }
            for(auto& idxSigJet2 : *jetTaggerSubleadingLRJMergedIndicesValuesSignal){
                psi_R_SigJet2 += (1.0/jetTaggerSubleadingLRJEtValuesSignal->at(0)) * gepBasicClustersEtValuesSignal->at(idxSigJet2) * sqrt(calcDeltaR2(jetTaggerSubleadingLRJEtaValuesSignal->at(0), jetTaggerSubleadingLRJPhiValuesSignal->at(0), gepBasicClustersEtaValuesSignal->at(idxSigJet2), gepBasicClustersPhiValuesSignal->at(idxSigJet2)));
                psi_R2_SigJet2 += (1.0/jetTaggerSubleadingLRJEtValuesSignal->at(0)) * gepBasicClustersEtValuesSignal->at(idxSigJet2) * calcDeltaR2(jFexSRJSubleadingEtaValuesSignal->at(0), jFexSRJSubleadingPhiValuesSignal->at(0), gepBasicClustersEtaValuesSignal->at(idxSigJet2), gepBasicClustersPhiValuesSignal->at(idxSigJet2));
            }

            sig_h_LRJ_psi_R_squared->Fill(psi_R_SigJet1 * psi_R_SigJet2);
            sigOfflineLeadingLRJEtvsPsi_R_squared->Fill(std::max(sig_LRJ_Et[i][0], sig_LRJ_Et[i][1]), psi_R_SigJet1 * psi_R_SigJet2);
            sigOfflineSubleadingLRJEtvsPsi_R_squared->Fill(std::min(sig_LRJ_Et[i][0], sig_LRJ_Et[i][1]), psi_R_SigJet1 * psi_R_SigJet2);
            sig_h_LRJ_psi_R2_squared->Fill(psi_R2_SigJet1 * psi_R2_SigJet2);
            sigOfflineLeadingLRJEtvsPsi_R2_squared->Fill(std::max(sig_LRJ_Et[i][0], sig_LRJ_Et[i][1]), psi_R2_SigJet1 * psi_R2_SigJet2);
            sigOfflineSubleadingLRJEtvsPsi_R2_squared->Fill(std::min(sig_LRJ_Et[i][0], sig_LRJ_Et[i][1]), psi_R2_SigJet1 * psi_R2_SigJet2);

            // Fill psi_R for leading, subleading, signal, background
            sig_h_leading_LRJ_psi_R->Fill(psi_R_SigJet1);
            sig_h_subleading_LRJ_psi_R->Fill(psi_R_SigJet2);
            sigOfflineLeadingLRJEtvsPsi_R->Fill(jetTaggerLeadingLRJEtValuesSignal->at(0), psi_R_SigJet1);
            sigOfflineSubleadingLRJEtvsPsi_R->Fill(jetTaggerSubleadingLRJEtValuesBack->at(0), psi_R_SigJet2);

            // Fill with regular deltaR metric
            sig_h_leading_LRJ_psi_R->Fill(psi_R_SigJet1);
            sig_h_subleading_LRJ_psi_R->Fill(psi_R_SigJet2);
            sig_h_LRJ_psi_R_12->Fill(psi_R_SigJet1 / psi_R_SigJet2);
            sigOfflineLeadingLRJEtvsPsi_12->Fill(jetTaggerLeadingLRJEtValuesSignal->at(0), psi_R_SigJet1 / psi_R_SigJet2);
            sigOfflineSubleadingLRJEtvsPsi_12->Fill(jetTaggerSubleadingLRJEtValuesSignal->at(0), psi_R_SigJet1 / psi_R_SigJet2);
            sigOfflineLeadingLRJEtvsPsi_R->Fill(jetTaggerLeadingLRJEtValuesSignal->at(0), psi_R_SigJet1);
            sigOfflineSubleadingLRJEtvsPsi_R->Fill(jetTaggerSubleadingLRJEtValuesBack->at(0), psi_R_SigJet2);
            sigOfflineLeadingLRJPsi_RvsSubleadingPsi_R->Fill(psi_R_SigJet2, psi_R_SigJet1); // subleading filled to x-axis

            // Fill with deltaR^2 metric
            //std::cout << "psi_R2_SigJet1: " << psi_R2_SigJet1 << "\n";
            //std::cout << "psi_R2_SigJet2 : " << psi_R2_SigJet2 << "\n";
            sig_h_leading_LRJ_psi_R2->Fill(psi_R2_SigJet1);
            sig_h_subleading_LRJ_psi_R2->Fill(psi_R2_SigJet2);
            sig_h_LRJ_psi_R2_12->Fill(psi_R2_SigJet1 / psi_R2_SigJet2);
            sigOfflineLeadingLRJEtvsPsi_R2->Fill(jetTaggerLeadingLRJEtValuesSignal->at(0), psi_R2_SigJet1);
            sigOfflineSubleadingLRJEtvsPsi_R2->Fill(jetTaggerSubleadingLRJEtValuesBack->at(0), psi_R2_SigJet2);
            sigOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2->Fill(psi_R2_SigJet2, psi_R2_SigJet1); 
            //std::cout << "SIGNAL final psi1_R: " << psi_R_SigJet1 << " and final psi_R_SigJet2 : " << psi_R_SigJet2 << "\n";
            //std::cout << "filling 1 sig" << "\n";

            // Fill deltaEt for digitized, undigitized algorithm configuration (can also serve to compare for using seed position recalculation algorithm version)
            sig_h_LRJ1_deltaEt_digitized_double->Fill((sig_LRJ_Et[i][0] - jetTaggerLeadingLRJEtValuesSignal->at(0))); // FIXME this is no longer useful....
            sig_h_LRJ2_deltaEt_digitized_double->Fill((sig_LRJ_Et[i][1] - jetTaggerSubleadingLRJEtValuesSignal->at(0)));

            // Map: Higgs index -> list of b-quark indices
            std::unordered_map<int, std::vector<int>> higgsToB;
            higgsToB.reserve(higgsIndexValuesSignal->size());
            for (size_t ib = 0; ib < higgsIndexValuesSignal->size(); ++ib) {
                higgsToB[ (*higgsIndexValuesSignal)[ib] ].push_back((int)ib);
            }

            // For each Higgs, take the first two associated b's
            for (size_t ih = 0; ih < indexOfHiggsValuesSignal->size(); ++ih) {
                const int hid = (*indexOfHiggsValuesSignal)[ih];

                auto it = higgsToB.find(hid);
                if (it == higgsToB.end() || it->second.size() < 2) continue;

                const int ib1 = it->second[0];
                const int ib2 = it->second[1];

                const double eta1 = (*truthbquarksEtaValuesSignal)[ib1];
                const double phi1 = (*truthbquarksPhiValuesSignal)[ib1];
                const double eta2 = (*truthbquarksEtaValuesSignal)[ib2];
                const double phi2 = (*truthbquarksPhiValuesSignal)[ib2];

                const double dr2 = calcDeltaR2(eta1, phi1, eta2, phi2);
                const double dR  = std::sqrt(dr2);

                const double hpT = (*truthHiggspTValuesSignal)[ih];

                prof_AvgDR_vs_HpT->Fill(hpT, dR);
                h2_DR_vs_HpT     ->Fill(hpT, dR);
            }

            // FIXME don't want to skip over all these events??
            if(recoAntiKt10LRJLeadingEtValuesSignal->size() == 0) continue;

            // Mass vs. Et for Leading Offline LRJ 
            sigOfflineLeadingLRJMassvsEt->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0), recoAntiKt10LRJLeadingMassValuesSignal->at(0));
            sigOfflineLeadingLRJMass->Fill(recoAntiKt10LRJLeadingMassValuesSignal->at(0));
            sig_h_leading_offlineLRJ_Et->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            // ------------------- SIGNAL -------------------
            // Subjet plots 
            const auto& etSig  = *truthAntiKt4WZSRJEtValuesSignal;
            const auto& etaSig = *truthAntiKt4WZSRJEtaValuesSignal;
            const auto& phiSig = *truthAntiKt4WZSRJPhiValuesSignal;

            const double lrjEtaSig = recoAntiKt10LRJLeadingEtaValuesSignal->at(0);
            const double lrjPhiSig = recoAntiKt10LRJLeadingPhiValuesSignal->at(0);

            // ---- Tunable parameters ----
            const double subjetEtCutoff           = 25.0;  // as before
            const double drMatchToLRJ             = 1.0;   // match truth subjets to leading large-R jet
            const double drMinBetweenSubjets      = 0.4;   // NEW: minimum deltaR separation between counted subjets

            // ---- Utility: select non-overlapping subjets (greedy by Et desc) ----
            auto selectNonOverlappingSubjets = [&](const std::vector<double>& et,
                                                const std::vector<double>& eta,
                                                const std::vector<double>& phi,
                                                double lrjEta, double lrjPhi,
                                                TH1* etHist)
            {
                const size_t n = et.size();

                // 1) First, collect candidates within drMatchToLRJ to the leading LRJ (for plotting) 
                //    and build a list of indices that ALSO pass the Et cut (for counting).
                std::vector<size_t> candidates; candidates.reserve(n);
                for (size_t i = 0; i < n; ++i) {
                    const double dR = std::sqrt(calcDeltaR2(eta[i], phi[i], lrjEta, lrjPhi));
                    if (dR < drMatchToLRJ) {
                        if (etHist) etHist->Fill(et[i]);          // unchanged behavior for plotting
                        if (et[i] >= subjetEtCutoff) {
                            candidates.push_back(i);              // for counting (subject to overlap removal)
                        }
                    }
                }

                // 2) Sort candidates by Et descending so we keep the highest-Et subjets when enforcing separation
                std::sort(candidates.begin(), candidates.end(),
                        [&](size_t a, size_t b){ return et[a] > et[b]; });

                // 3) Greedily select non-overlapping subjets (ΔR > drMinBetweenSubjets)
                std::vector<size_t> selected; selected.reserve(candidates.size());
                for (size_t idx : candidates) {
                    bool overlaps = false;
                    for (size_t jdx : selected) {
                        const double dR = std::sqrt(calcDeltaR2(eta[idx], phi[idx], eta[jdx], phi[jdx]));
                        if (dR <= drMinBetweenSubjets) { overlaps = true; break; }
                    }
                    if (!overlaps) selected.push_back(idx);
                }

                return selected.size(); // this is the multiplicity you want to Fill(...)
            };
            
            const unsigned int signalSubjetCounter =
                selectNonOverlappingSubjets(etSig, etaSig, phiSig, lrjEtaSig, lrjPhiSig,
                                            sig_h_LeadingOfflineLRJ_SubjetEt);

            sig_h_LeadingOfflineLRJ_SubjetMultiplicity->Fill(signalSubjetCounter);
            sigOfflineLeadingLRJEtvsSubjetMult->Fill(signalSubjetCounter, recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sigOfflineLeadingLRJMassvsSubjetMult->Fill(signalSubjetCounter, recoAntiKt10LRJLeadingMassValuesSignal->at(0));

            // Delta R, delta Et for leading gFEX, jFEX SRJ (seeds), Offline LRJs
            sig_h_leading_LRJ_gFexLRJ_deltaEt->Fill(gFexLRJLeadingEtValuesSignal->at(0) - jetTaggerLeadingLRJEtValuesSignal->at(0));
            sig_h_leading_offlineLRJ_gFexLRJ_deltaEt->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0) - gFexLRJLeadingEtValuesSignal->at(0));
            sig_h_leading_offlineLRJ_jFexLRJ_deltaEt->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0) - jFexLRJLeadingEtValuesSignal->at(0));
            sig_h_leading_LRJ_offlineLRJ_deltaEt->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0) - jetTaggerLeadingLRJEtValuesSignal->at(0));   

            // Fill Et "resolution"]
            //std::cout << "(gFexLRJLeadingEtValuesSignal->at(0) - jetTaggerLeadingLRJEtValuesSignal->at(0)) /gFexLRJLeadingEtValuesSignal->at(0): " << (gFexLRJLeadingEtValuesSignal->at(0) - jetTaggerLeadingLRJEtValuesSignal->at(0)) /gFexLRJLeadingEtValuesSignal->at(0) << "\n";
            sig_h_leading_LRJ_gFexLRJ_Et_resolution->Fill((gFexLRJLeadingEtValuesSignal->at(0) - jetTaggerLeadingLRJEtValuesSignal->at(0)) /gFexLRJLeadingEtValuesSignal->at(0));
            sig_h_leading_offlineLRJ_gFexLRJ_Et_resolution->Fill((recoAntiKt10LRJLeadingEtValuesSignal->at(0) - gFexLRJLeadingEtValuesSignal->at(0))/recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_leading_offlineLRJ_jFexLRJ_Et_resolution->Fill((recoAntiKt10LRJLeadingEtValuesSignal->at(0) - jFexLRJLeadingEtValuesSignal->at(0))/recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_leading_LRJ_offlineLRJ_Et_resolution->Fill((recoAntiKt10LRJLeadingEtValuesSignal->at(0) - jetTaggerLeadingLRJEtValuesSignal->at(0)) / recoAntiKt10LRJLeadingEtValuesSignal->at(0)); 

            sig_h_lead_sublead_LRJ_deltaR->Fill(sqrt(calcDeltaR2(sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0], sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1])));
            sig_h_leading_LRJ_gFexLRJ_deltaR->Fill(sqrt(calcDeltaR2(gFexLRJLeadingEtaValuesSignal->at(0), gFexLRJLeadingPhiValuesSignal->at(0), sig_LRJ_Eta[i][highestEtIndexLRJSig], sig_LRJ_Phi[i][highestEtIndexLRJSig])));
            sig_h_first_LRJ_jFexSRJ_deltaR->Fill(sqrt(calcDeltaR2(jFEXSRJLeadingEta, jFEXSRJLeadingPhi, sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0])));
            sig_h_second_LRJ_jFexSRJ_deltaR->Fill(sqrt(calcDeltaR2(jFEXSRJSubleadingEta, jFEXSRJSubleadingPhi, sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1])));
            sig_h_leading_LRJ_offlineLRJ_deltaR->Fill(sqrt(calcDeltaR2(recoAntiKt10LRJLeadingEtaValuesSignal->at(0), recoAntiKt10LRJLeadingPhiValuesSignal->at(0), sig_LRJ_Eta[i][highestEtIndexLRJSig], sig_LRJ_Phi[i][highestEtIndexLRJSig])));

            // Dijet efficiencies (signal)
            if(recoAntiKt10LRJSubleadingEtValuesSignal->size() > 0){
                sig_h_subleading_offlineLRJ_Et->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                if(sig_LRJ_Et[i][0] >= 50.0 && sig_LRJ_Et[i][1] >= 50.0){
                    sig_h_offlineLRJ_Et_num50_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom50_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 100.0 && sig_LRJ_Et[i][1] >= 100.0){
                    sig_h_offlineLRJ_Et_num100_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom100_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 150.0 && sig_LRJ_Et[i][1] >= 150.0){
                    sig_h_offlineLRJ_Et_num150_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom150_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 200.0 && sig_LRJ_Et[i][1] >= 200.0){
                    sig_h_offlineLRJ_Et_num200_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom200_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 250.0 && sig_LRJ_Et[i][1] >= 250.0){
                    sig_h_offlineLRJ_Et_num250_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom250_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 300.0 && sig_LRJ_Et[i][1] >= 300.0){
                    sig_h_offlineLRJ_Et_num300_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom300_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 350.0 && sig_LRJ_Et[i][1] >= 350.0){
                    sig_h_offlineLRJ_Et_num350_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom350_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 400.0 && sig_LRJ_Et[i][1] >= 400.0){
                    sig_h_offlineLRJ_Et_num400_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom400_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 450.0 && sig_LRJ_Et[i][1] >= 450.0){
                    sig_h_offlineLRJ_Et_num450_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom450_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 500.0 && sig_LRJ_Et[i][1] >= 500.0){
                    sig_h_offlineLRJ_Et_num500_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom500_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
            }
            
            // Subjet requirement efficiencies
            if(signalSubjetCounter == 1){
                if(sig_LRJ_Et[i][0] >= 50.0 || sig_LRJ_Et[i][1] >= 50.0){
                    sig_h_offlineLRJ_Et_num50_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom50_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 100.0 || sig_LRJ_Et[i][1] >= 100.0){
                    sig_h_offlineLRJ_Et_num100_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom100_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 150.0 || sig_LRJ_Et[i][1] >= 150.0){
                    sig_h_offlineLRJ_Et_num150_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom150_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 200.0 || sig_LRJ_Et[i][1] >= 200.0){
                    sig_h_offlineLRJ_Et_num200_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom200_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 250.0 || sig_LRJ_Et[i][1] >= 250.0){
                    sig_h_offlineLRJ_Et_num250_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom250_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 300.0 || sig_LRJ_Et[i][1] >= 300.0){
                    sig_h_offlineLRJ_Et_num300_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom300_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 350.0 || sig_LRJ_Et[i][1] >= 350.0){
                    sig_h_offlineLRJ_Et_num350_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom350_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 400.0 || sig_LRJ_Et[i][1] >= 400.0){
                    sig_h_offlineLRJ_Et_num400_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom400_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 450.0 || sig_LRJ_Et[i][1] >= 450.0){
                    sig_h_offlineLRJ_Et_num450_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom450_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 500.0 || sig_LRJ_Et[i][1] >= 500.0){
                    sig_h_offlineLRJ_Et_num500_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom500_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }

            if(signalSubjetCounter >= 2){
                if(sig_LRJ_Et[i][0] >= 50.0 || sig_LRJ_Et[i][1] >= 50.0){
                    sig_h_offlineLRJ_Et_num50_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom50_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 100.0 || sig_LRJ_Et[i][1] >= 100.0){
                    sig_h_offlineLRJ_Et_num100_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom100_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 150.0 || sig_LRJ_Et[i][1] >= 150.0){
                    sig_h_offlineLRJ_Et_num150_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom150_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 200.0 || sig_LRJ_Et[i][1] >= 200.0){
                    sig_h_offlineLRJ_Et_num200_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom200_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 250.0 || sig_LRJ_Et[i][1] >= 250.0){
                    sig_h_offlineLRJ_Et_num250_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom250_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 300.0 || sig_LRJ_Et[i][1] >= 300.0){
                    sig_h_offlineLRJ_Et_num300_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom300_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 350.0 || sig_LRJ_Et[i][1] >= 350.0){
                    sig_h_offlineLRJ_Et_num350_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom350_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 400.0 || sig_LRJ_Et[i][1] >= 400.0){
                    sig_h_offlineLRJ_Et_num400_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom400_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 450.0 || sig_LRJ_Et[i][1] >= 450.0){
                    sig_h_offlineLRJ_Et_num450_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom450_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 500.0 || sig_LRJ_Et[i][1] >= 500.0){
                    sig_h_offlineLRJ_Et_num500_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom500_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            // Mass window efficiencies
            if(recoAntiKt10LRJLeadingMassValuesSignal->at(0) >= 100.0 && recoAntiKt10LRJLeadingMassValuesSignal->at(0) <= 150.0){
                if(sig_LRJ_Et[i][0] >= 50.0 || sig_LRJ_Et[i][1] >= 50.0){
                    sig_h_offlineLRJ_Et_num50_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom50_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 100.0 || sig_LRJ_Et[i][1] >= 100.0){
                    sig_h_offlineLRJ_Et_num100_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom100_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 150.0 || sig_LRJ_Et[i][1] >= 150.0){
                    sig_h_offlineLRJ_Et_num150_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom150_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 200.0 || sig_LRJ_Et[i][1] >= 200.0){
                    sig_h_offlineLRJ_Et_num200_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom200_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 250.0 || sig_LRJ_Et[i][1] >= 250.0){
                    sig_h_offlineLRJ_Et_num250_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom250_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 300.0 || sig_LRJ_Et[i][1] >= 300.0){
                    sig_h_offlineLRJ_Et_num300_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom300_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 350.0 || sig_LRJ_Et[i][1] >= 350.0){
                    sig_h_offlineLRJ_Et_num350_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom350_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 400.0 || sig_LRJ_Et[i][1] >= 400.0){
                    sig_h_offlineLRJ_Et_num400_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom400_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 450.0 || sig_LRJ_Et[i][1] >= 450.0){
                    sig_h_offlineLRJ_Et_num450_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom450_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

                if(sig_LRJ_Et[i][0] >= 500.0 || sig_LRJ_Et[i][1] >= 500.0){
                    sig_h_offlineLRJ_Et_num500_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom500_mass100to150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }

            if(sig_LRJ_Et[i][0] >= 50.0 || sig_LRJ_Et[i][1] >= 50.0){
                sig_h_offlineLRJ_Et_num50->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                sig_h_Avg_b_Et_num50->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
            sig_h_offlineLRJ_Et_denom50->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_Avg_b_Et_denom50->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);

            if(sig_LRJ_Et[i][0] >= 100.0 || sig_LRJ_Et[i][1] >= 100.0){
                sig_h_offlineLRJ_Et_num100->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                h_sig_num100->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                sig_h_Avg_b_Et_num100->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
            sig_h_offlineLRJ_Et_denom100->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            h_sig_denom100->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_Avg_b_Et_denom100->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);

            if(sig_LRJ_Et[i][0] >= 150.0 || sig_LRJ_Et[i][1] >= 150.0){
                sig_h_offlineLRJ_Et_num150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                sig_h_Avg_b_Et_num150->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
            sig_h_offlineLRJ_Et_denom150->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_Avg_b_Et_denom150->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);

            if(sig_LRJ_Et[i][0] >= 200.0 || sig_LRJ_Et[i][1] >= 200.0){
                sig_h_offlineLRJ_Et_num200->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                sig_h_Avg_b_Et_num200->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
            sig_h_offlineLRJ_Et_denom200->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_Avg_b_Et_denom200->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);

            if(sig_LRJ_Et[i][0] >= 250.0 || sig_LRJ_Et[i][1] >= 250.0){
                sig_h_offlineLRJ_Et_num250->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                h_sig_num250->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                sig_h_Avg_b_Et_num250->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
            h_sig_denom250->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_offlineLRJ_Et_denom250->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_Avg_b_Et_denom250->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);

            if(sig_LRJ_Et[i][0] >= 300.0 || sig_LRJ_Et[i][1] >= 300.0){
                sig_h_offlineLRJ_Et_num300->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                sig_h_Avg_b_Et_num300->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
            sig_h_offlineLRJ_Et_denom300->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_Avg_b_Et_denom300->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);

            if(sig_LRJ_Et[i][0] >= 350.0 || sig_LRJ_Et[i][1] >= 350.0){
                sig_h_offlineLRJ_Et_num350->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                sig_h_Avg_b_Et_num350->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
            sig_h_offlineLRJ_Et_denom350->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_Avg_b_Et_denom350->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);

            if(sig_LRJ_Et[i][0] >= 400.0 || sig_LRJ_Et[i][1] >= 400.0){
                sig_h_offlineLRJ_Et_num400->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                h_sig_num400->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                sig_h_Avg_b_Et_num400->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
            h_sig_denom400->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_offlineLRJ_Et_denom400->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_Avg_b_Et_denom400->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);

            if(sig_LRJ_Et[i][0] >= 450.0 || sig_LRJ_Et[i][1] >= 450.0){
                sig_h_offlineLRJ_Et_num450->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                sig_h_Avg_b_Et_num450->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
            sig_h_offlineLRJ_Et_denom450->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_Avg_b_Et_denom450->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);

            if(sig_LRJ_Et[i][0] >= 500.0 || sig_LRJ_Et[i][1] >= 500.0){
                sig_h_offlineLRJ_Et_num500->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                sig_h_Avg_b_Et_num500->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
            sig_h_offlineLRJ_Et_denom500->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_Avg_b_Et_denom500->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);

            // Fill jFEX Signal Trigger Efficiencies (first 1 jet)
            if(jFexLRJLeadingEtValuesSignal->at(0) >= 50.0){
                sig_h_offlineLRJ_Et_num50_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom50_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(jFexLRJLeadingEtValuesSignal->at(0) >= 100.0){
                sig_h_offlineLRJ_Et_num100_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom100_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(jFexLRJLeadingEtValuesSignal->at(0) >= 150.0){
                sig_h_offlineLRJ_Et_num150_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom150_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(jFexLRJLeadingEtValuesSignal->at(0) >= 200.0){
                sig_h_offlineLRJ_Et_num200_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom200_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(jFexLRJLeadingEtValuesSignal->at(0) >= 250.0){
                sig_h_offlineLRJ_Et_num250_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom250_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(jFexLRJLeadingEtValuesSignal->at(0) >= 300.0){
                sig_h_offlineLRJ_Et_num300_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom300_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(jFexLRJLeadingEtValuesSignal->at(0) >= 350.0){
                sig_h_offlineLRJ_Et_num350_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom350_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(jFexLRJLeadingEtValuesSignal->at(0) >= 400.0){
                sig_h_offlineLRJ_Et_num400_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom400_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(jFexLRJLeadingEtValuesSignal->at(0) >= 450.0){
                sig_h_offlineLRJ_Et_num450_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom450_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(jFexLRJLeadingEtValuesSignal->at(0) >= 500.0){
                sig_h_offlineLRJ_Et_num500_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom500_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            // Fill gFEX Signal Trigger Efficiencies (first 1 jet)
            if(gFexLRJLeadingEtValuesSignal->at(0) >= 50.0){
                sig_h_offlineLRJ_Et_num50_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom50_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(gFexLRJLeadingEtValuesSignal->at(0) >= 100.0){
                sig_h_offlineLRJ_Et_num100_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom100_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(gFexLRJLeadingEtValuesSignal->at(0) >= 150.0){
                sig_h_offlineLRJ_Et_num150_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom150_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(gFexLRJLeadingEtValuesSignal->at(0) >= 200.0){
                sig_h_offlineLRJ_Et_num200_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom200_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(gFexLRJLeadingEtValuesSignal->at(0) >= 250.0){
                sig_h_offlineLRJ_Et_num250_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom250_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(gFexLRJLeadingEtValuesSignal->at(0) >= 300.0){
                sig_h_offlineLRJ_Et_num300_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom300_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(gFexLRJLeadingEtValuesSignal->at(0) >= 350.0){
                sig_h_offlineLRJ_Et_num350_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom350_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(gFexLRJLeadingEtValuesSignal->at(0) >= 400.0){
                sig_h_offlineLRJ_Et_num400_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom400_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(gFexLRJLeadingEtValuesSignal->at(0) >= 450.0){
                sig_h_offlineLRJ_Et_num450_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom450_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            if(gFexLRJLeadingEtValuesSignal->at(0) >= 500.0){
                sig_h_offlineLRJ_Et_num500_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            }
            sig_h_offlineLRJ_Et_denom500_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));

            // next dijet for signal
            if(recoAntiKt10LRJSubleadingEtValuesSignal->size() > 0){
                if(gFexLRJSubleadingEtValuesSignal->at(0) >= 50.0){
                    sig_h_offlineLRJ_Et_num50_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom50_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(gFexLRJSubleadingEtValuesSignal->at(0) >= 100.0){
                    sig_h_offlineLRJ_Et_num100_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom100_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(gFexLRJSubleadingEtValuesSignal->at(0) >= 150.0){
                    sig_h_offlineLRJ_Et_num150_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom150_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(gFexLRJSubleadingEtValuesSignal->at(0) >= 200.0){
                    sig_h_offlineLRJ_Et_num200_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom200_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(gFexLRJSubleadingEtValuesSignal->at(0) >= 250.0){
                    sig_h_offlineLRJ_Et_num250_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom250_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(gFexLRJSubleadingEtValuesSignal->at(0) >= 300.0){
                    sig_h_offlineLRJ_Et_num300_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom300_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(gFexLRJSubleadingEtValuesSignal->at(0) >= 350.0){
                    sig_h_offlineLRJ_Et_num350_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom350_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(gFexLRJSubleadingEtValuesSignal->at(0) >= 400.0){
                    sig_h_offlineLRJ_Et_num400_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom400_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(gFexLRJSubleadingEtValuesSignal->at(0) >= 450.0){
                    sig_h_offlineLRJ_Et_num450_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom450_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));

                if(gFexLRJSubleadingEtValuesSignal->at(0) >= 500.0){
                    sig_h_offlineLRJ_Et_num500_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                }
                sig_h_offlineLRJ_Et_denom500_gFexLRJ_Dijet->Fill(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
            }

            if(i == evDisplay0Sig_){
                if(recoAntiKt10LRJSubleadingEtValuesSignal->size() > 0 && jFexSRJEtaValuesSignal->size() >= 6){
                    displayEv0Sig_ = true;
                    jFexSeedPositionsSigEv0.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(0), jFexSRJPhiValuesSignal->at(0)));
                    jFexSeedPositionsSigEv0.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(1), jFexSRJPhiValuesSignal->at(1)));
                    jFexAdditionalSRJPositionsSigEv0.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(2), jFexSRJPhiValuesSignal->at(2)));
                    jFexAdditionalSRJPositionsSigEv0.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(3), jFexSRJPhiValuesSignal->at(3)));
                    jFexAdditionalSRJPositionsSigEv0.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(4), jFexSRJPhiValuesSignal->at(4)));
                    jFexAdditionalSRJPositionsSigEv0.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(5), jFexSRJPhiValuesSignal->at(5)));
                    if(sig_LRJ_Et[i][0] > sig_LRJ_Et[i][1]){
                        newSeedPositionsSigEv0.push_back(std::make_pair(sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0]));
                        newSeedPositionsSigEv0.push_back(std::make_pair(sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1]));
                        sigLRJEtEv0.push_back(sig_LRJ_Et[i][0]);
                        sigLRJEtEv0.push_back(sig_LRJ_Et[i][1]);
                    }
                    else{
                        newSeedPositionsSigEv0.push_back(std::make_pair(sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1]));
                        newSeedPositionsSigEv0.push_back(std::make_pair(sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0]));
                        sigLRJEtEv0.push_back(sig_LRJ_Et[i][1]);
                        sigLRJEtEv0.push_back(sig_LRJ_Et[i][0]);
                    }
                    gFexLRJPositionSigEv0.push_back(std::make_pair(gFexLRJLeadingEtaValuesSignal->at(0), gFexLRJLeadingPhiValuesSignal->at(0)));
                    gFexLRJPositionSigEv0.push_back(std::make_pair(gFexLRJSubleadingEtaValuesSignal->at(0), gFexLRJSubleadingPhiValuesSignal->at(0)));
                    siggFexLRJEtEv0.push_back(gFexLRJLeadingEtValuesSignal->at(0));
                    siggFexLRJEtEv0.push_back(gFexLRJSubleadingEtValuesSignal->at(0));
                    offlineLRJPositionSigEv0.push_back(std::make_pair(recoAntiKt10LRJLeadingEtaValuesSignal->at(0), recoAntiKt10LRJLeadingPhiValuesSignal->at(0)));
                    offlineLRJPositionSigEv0.push_back(std::make_pair(recoAntiKt10LRJSubleadingEtaValuesSignal->at(0), recoAntiKt10LRJSubleadingPhiValuesSignal->at(0)));
                    sigOfflineLRJEtEv0.push_back(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                    sigOfflineLRJEtEv0.push_back(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                    std::cout << "Event 0 Sig LRJ 1 Et, Eta, Phi: " << sig_LRJ_Et[i][0] << " , " << sig_LRJ_Eta[i][0] << " , " << sig_LRJ_Phi[i][0] << "\n";
                    std::cout << "Event 0 Sig LRJ 2 Et, Eta, Phi: " << sig_LRJ_Et[i][1] << " , " << sig_LRJ_Eta[i][1] << " , " << sig_LRJ_Phi[i][1] << "\n";
                }
            }
            if(i == evDisplay1Sig_){
                if(recoAntiKt10LRJSubleadingEtValuesSignal->size() > 0 && jFexSRJEtaValuesSignal->size() >= 6){
                    displayEv1Sig_ = true;
                    jFexSeedPositionsSigEv1.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(0), jFexSRJPhiValuesSignal->at(0)));
                    jFexSeedPositionsSigEv1.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(1), jFexSRJPhiValuesSignal->at(1)));
                    jFexAdditionalSRJPositionsSigEv1.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(2), jFexSRJPhiValuesSignal->at(2)));
                    jFexAdditionalSRJPositionsSigEv1.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(3), jFexSRJPhiValuesSignal->at(3)));
                    jFexAdditionalSRJPositionsSigEv1.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(4), jFexSRJPhiValuesSignal->at(4)));
                    jFexAdditionalSRJPositionsSigEv1.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(5), jFexSRJPhiValuesSignal->at(5)));
                    if(sig_LRJ_Et[i][0] > sig_LRJ_Et[i][1]){
                        newSeedPositionsSigEv1.push_back(std::make_pair(sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0]));
                        newSeedPositionsSigEv1.push_back(std::make_pair(sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1]));
                        sigLRJEtEv1.push_back(sig_LRJ_Et[i][0]);
                        sigLRJEtEv1.push_back(sig_LRJ_Et[i][1]);
                    }
                    else{
                        newSeedPositionsSigEv1.push_back(std::make_pair(sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1]));
                        newSeedPositionsSigEv1.push_back(std::make_pair(sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0]));
                        sigLRJEtEv1.push_back(sig_LRJ_Et[i][1]);
                        sigLRJEtEv1.push_back(sig_LRJ_Et[i][0]);
                    }
                    gFexLRJPositionSigEv1.push_back(std::make_pair(gFexLRJLeadingEtaValuesSignal->at(0), gFexLRJLeadingPhiValuesSignal->at(0)));
                    gFexLRJPositionSigEv1.push_back(std::make_pair(gFexLRJSubleadingEtaValuesSignal->at(0), gFexLRJSubleadingPhiValuesSignal->at(0)));
                    siggFexLRJEtEv1.push_back(gFexLRJLeadingEtValuesSignal->at(0));
                    siggFexLRJEtEv1.push_back(gFexLRJSubleadingEtValuesSignal->at(0));
                    
                    offlineLRJPositionSigEv1.push_back(std::make_pair(recoAntiKt10LRJLeadingEtaValuesSignal->at(0), recoAntiKt10LRJLeadingPhiValuesSignal->at(0)));
                    offlineLRJPositionSigEv1.push_back(std::make_pair(recoAntiKt10LRJSubleadingEtaValuesSignal->at(0), recoAntiKt10LRJSubleadingPhiValuesSignal->at(0)));
                    sigOfflineLRJEtEv1.push_back(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                    sigOfflineLRJEtEv1.push_back(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                    std::cout << "Event 1 Sig LRJ 1 Et, Eta, Phi: " << sig_LRJ_Et[i][0] << " , " << sig_LRJ_Eta[i][0] << " , " << sig_LRJ_Phi[i][0] << "\n";
                    std::cout << "Event 1 Sig LRJ 2 Et, Eta, Phi: " << sig_LRJ_Et[i][1] << " , " << sig_LRJ_Eta[i][1] << " , " << sig_LRJ_Phi[i][1] << "\n";
                
                }
            }
            if(i == evDisplay2Sig_){
                if(recoAntiKt10LRJSubleadingEtValuesSignal->size() > 0 && jFexSRJEtaValuesSignal->size() >= 6){
                    displayEv2Sig_ = true;
                    jFexSeedPositionsSigEv2.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(0), jFexSRJPhiValuesSignal->at(0)));
                    jFexSeedPositionsSigEv2.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(1), jFexSRJPhiValuesSignal->at(1)));
                    jFexAdditionalSRJPositionsSigEv2.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(2), jFexSRJPhiValuesSignal->at(2)));
                    jFexAdditionalSRJPositionsSigEv2.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(3), jFexSRJPhiValuesSignal->at(3)));
                    jFexAdditionalSRJPositionsSigEv2.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(4), jFexSRJPhiValuesSignal->at(4)));
                    jFexAdditionalSRJPositionsSigEv2.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(5), jFexSRJPhiValuesSignal->at(5)));
                    if(sig_LRJ_Et[i][0] > sig_LRJ_Et[i][1]){
                        newSeedPositionsSigEv2.push_back(std::make_pair(sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0]));
                        newSeedPositionsSigEv2.push_back(std::make_pair(sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1]));
                        sigLRJEtEv2.push_back(sig_LRJ_Et[i][0]);
                        sigLRJEtEv2.push_back(sig_LRJ_Et[i][1]);
                    }
                    else{
                        newSeedPositionsSigEv2.push_back(std::make_pair(sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1]));
                        newSeedPositionsSigEv2.push_back(std::make_pair(sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0]));
                        sigLRJEtEv2.push_back(sig_LRJ_Et[i][1]);
                        sigLRJEtEv2.push_back(sig_LRJ_Et[i][0]);
                    }
                    gFexLRJPositionSigEv2.push_back(std::make_pair(gFexLRJLeadingEtaValuesSignal->at(0), gFexLRJLeadingPhiValuesSignal->at(0)));
                    gFexLRJPositionSigEv2.push_back(std::make_pair(gFexLRJSubleadingEtaValuesSignal->at(0), gFexLRJSubleadingPhiValuesSignal->at(0)));
                    siggFexLRJEtEv2.push_back(gFexLRJLeadingEtValuesSignal->at(0));
                    siggFexLRJEtEv2.push_back(gFexLRJSubleadingEtValuesSignal->at(0));
                    offlineLRJPositionSigEv2.push_back(std::make_pair(recoAntiKt10LRJLeadingEtaValuesSignal->at(0), recoAntiKt10LRJLeadingPhiValuesSignal->at(0)));
                    offlineLRJPositionSigEv2.push_back(std::make_pair(recoAntiKt10LRJSubleadingEtaValuesSignal->at(0), recoAntiKt10LRJSubleadingPhiValuesSignal->at(0)));
                    sigOfflineLRJEtEv2.push_back(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                    sigOfflineLRJEtEv2.push_back(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                    std::cout << "Event 2 Sig LRJ 1 Et, Eta, Phi: " << sig_LRJ_Et[i][0] << " , " << sig_LRJ_Eta[i][0] << " , " << sig_LRJ_Phi[i][0] << "\n";
                    std::cout << "Event 2 Sig LRJ 2 Et, Eta, Phi: " << sig_LRJ_Et[i][1] << " , " << sig_LRJ_Eta[i][1] << " , " << sig_LRJ_Phi[i][1] << "\n";
                }
                
            }
            if(i == evDisplay3Sig_){
                if(recoAntiKt10LRJSubleadingEtValuesSignal->size() > 0 && jFexSRJEtaValuesSignal->size() >= 6){
                    displayEv3Sig_ = true;
                    jFexSeedPositionsSigEv3.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(0), jFexSRJPhiValuesSignal->at(0)));
                    jFexSeedPositionsSigEv3.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(1), jFexSRJPhiValuesSignal->at(1)));
                    jFexAdditionalSRJPositionsSigEv3.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(2), jFexSRJPhiValuesSignal->at(2)));
                    jFexAdditionalSRJPositionsSigEv3.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(3), jFexSRJPhiValuesSignal->at(3)));
                    jFexAdditionalSRJPositionsSigEv3.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(4), jFexSRJPhiValuesSignal->at(4)));
                    jFexAdditionalSRJPositionsSigEv3.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(5), jFexSRJPhiValuesSignal->at(5)));
                    if(sig_LRJ_Et[i][0] > sig_LRJ_Et[i][1]){
                        newSeedPositionsSigEv3.push_back(std::make_pair(sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0]));
                        newSeedPositionsSigEv3.push_back(std::make_pair(sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1]));
                        sigLRJEtEv3.push_back(sig_LRJ_Et[i][0]);
                        sigLRJEtEv3.push_back(sig_LRJ_Et[i][1]);
                    }
                    else{
                        newSeedPositionsSigEv3.push_back(std::make_pair(sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1]));
                        newSeedPositionsSigEv3.push_back(std::make_pair(sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0]));
                        sigLRJEtEv3.push_back(sig_LRJ_Et[i][1]);
                        sigLRJEtEv3.push_back(sig_LRJ_Et[i][0]);
                    }
                    gFexLRJPositionSigEv3.push_back(std::make_pair(gFexLRJLeadingEtaValuesSignal->at(0), gFexLRJLeadingPhiValuesSignal->at(0)));
                    gFexLRJPositionSigEv3.push_back(std::make_pair(gFexLRJSubleadingEtaValuesSignal->at(0), gFexLRJSubleadingPhiValuesSignal->at(0)));
                    siggFexLRJEtEv3.push_back(gFexLRJLeadingEtValuesSignal->at(0));
                    siggFexLRJEtEv3.push_back(gFexLRJSubleadingEtValuesSignal->at(0));
                    offlineLRJPositionSigEv3.push_back(std::make_pair(recoAntiKt10LRJLeadingEtaValuesSignal->at(0), recoAntiKt10LRJLeadingPhiValuesSignal->at(0)));
                    offlineLRJPositionSigEv3.push_back(std::make_pair(recoAntiKt10LRJSubleadingEtaValuesSignal->at(0), recoAntiKt10LRJSubleadingPhiValuesSignal->at(0)));
                    sigOfflineLRJEtEv3.push_back(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                    sigOfflineLRJEtEv3.push_back(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                    std::cout << "Event 3 Sig LRJ 1 Et, Eta, Phi: " << sig_LRJ_Et[i][0] << " , " << sig_LRJ_Eta[i][0] << " , " << sig_LRJ_Phi[i][0] << "\n";
                    std::cout << "Event 3 Sig LRJ 2 Et, Eta, Phi: " << sig_LRJ_Et[i][1] << " , " << sig_LRJ_Eta[i][1] << " , " << sig_LRJ_Phi[i][1] << "\n";
                }
                
            }
            if(i == evDisplay4Sig_){
                if(recoAntiKt10LRJSubleadingEtValuesSignal->size() > 0 && jFexSRJEtaValuesSignal->size() >= 6){
                    displayEv4Sig_ = true;
                    jFexSeedPositionsSigEv4.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(0), jFexSRJPhiValuesSignal->at(0)));
                    jFexSeedPositionsSigEv4.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(1), jFexSRJPhiValuesSignal->at(1)));
                    jFexAdditionalSRJPositionsSigEv4.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(2), jFexSRJPhiValuesSignal->at(2)));
                    jFexAdditionalSRJPositionsSigEv4.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(3), jFexSRJPhiValuesSignal->at(3)));
                    jFexAdditionalSRJPositionsSigEv4.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(4), jFexSRJPhiValuesSignal->at(4)));
                    jFexAdditionalSRJPositionsSigEv4.push_back(std::make_pair(jFexSRJEtaValuesSignal->at(5), jFexSRJPhiValuesSignal->at(5)));
                    if(sig_LRJ_Et[i][0] > sig_LRJ_Et[i][1]){
                        newSeedPositionsSigEv4.push_back(std::make_pair(sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0]));
                        newSeedPositionsSigEv4.push_back(std::make_pair(sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1]));
                        sigLRJEtEv4.push_back(sig_LRJ_Et[i][0]);
                        sigLRJEtEv4.push_back(sig_LRJ_Et[i][1]);
                    }
                    else{
                        newSeedPositionsSigEv4.push_back(std::make_pair(sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1]));
                        newSeedPositionsSigEv4.push_back(std::make_pair(sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0]));
                        sigLRJEtEv4.push_back(sig_LRJ_Et[i][1]);
                        sigLRJEtEv4.push_back(sig_LRJ_Et[i][0]);
                    }
                    gFexLRJPositionSigEv4.push_back(std::make_pair(gFexLRJLeadingEtaValuesSignal->at(0), gFexLRJLeadingPhiValuesSignal->at(0)));
                    gFexLRJPositionSigEv4.push_back(std::make_pair(gFexLRJSubleadingEtaValuesSignal->at(0), gFexLRJSubleadingPhiValuesSignal->at(0)));
                    siggFexLRJEtEv4.push_back(gFexLRJLeadingEtValuesSignal->at(0));
                    siggFexLRJEtEv4.push_back(gFexLRJSubleadingEtValuesSignal->at(0));
                    offlineLRJPositionSigEv4.push_back(std::make_pair(recoAntiKt10LRJLeadingEtaValuesSignal->at(0), recoAntiKt10LRJLeadingPhiValuesSignal->at(0)));
                    offlineLRJPositionSigEv4.push_back(std::make_pair(recoAntiKt10LRJSubleadingEtaValuesSignal->at(0), recoAntiKt10LRJSubleadingPhiValuesSignal->at(0)));
                    sigOfflineLRJEtEv4.push_back(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                    sigOfflineLRJEtEv4.push_back(recoAntiKt10LRJSubleadingEtValuesSignal->at(0));
                    std::cout << "Event 4 Sig LRJ 1 Et, Eta, Phi: " << sig_LRJ_Et[i][0] << " , " << sig_LRJ_Eta[i][0] << " , " << sig_LRJ_Phi[i][0] << "\n";
                    std::cout << "Event 4 Sig LRJ 2 Et, Eta, Phi: " << sig_LRJ_Et[i][1] << " , " << sig_LRJ_Eta[i][1] << " , " << sig_LRJ_Phi[i][1] << "\n";
                }
            }
            if(i <= evDisplayMaxSig_){
                for(unsigned int iTopo = 0; iTopo < nInputObjectsAlgorithmConfiguration; iTopo++){
                    if(i == evDisplay0Sig_){
                        sigTopo422Highest128SeedPositionsEv0->Fill(gepBasicClustersEtaValuesSignal->at(iTopo), gepBasicClustersPhiValuesSignal->at(iTopo), gepBasicClustersEtValuesSignal->at(iTopo));
                    } 
                    if(i == evDisplay1Sig_){
                        sigTopo422Highest128SeedPositionsEv1->Fill(gepBasicClustersEtaValuesSignal->at(iTopo), gepBasicClustersPhiValuesSignal->at(iTopo), gepBasicClustersEtValuesSignal->at(iTopo));
                    } 
                    if(i == evDisplay2Sig_){
                        sigTopo422Highest128SeedPositionsEv2->Fill(gepBasicClustersEtaValuesSignal->at(iTopo), gepBasicClustersPhiValuesSignal->at(iTopo), gepBasicClustersEtValuesSignal->at(iTopo));
                    } 
                    if(i == evDisplay3Sig_){
                        sigTopo422Highest128SeedPositionsEv3->Fill(gepBasicClustersEtaValuesSignal->at(iTopo), gepBasicClustersPhiValuesSignal->at(iTopo), gepBasicClustersEtValuesSignal->at(iTopo));
                    } 
                    if(i == evDisplay4Sig_){
                        sigTopo422Highest128SeedPositionsEv4->Fill(gepBasicClustersEtaValuesSignal->at(iTopo), gepBasicClustersPhiValuesSignal->at(iTopo), gepBasicClustersEtValuesSignal->at(iTopo));
                    } 
                }
            }

            // try computing invariant mass, signal, background using jet tagger jets
            double sigDeltaEta = sig_LRJ_Eta[i][0] - sig_LRJ_Eta[i][1];
            double sigDeltaPhi = std::abs(sig_LRJ_Phi[i][0] - sig_LRJ_Phi[i][1]);
            // Ensure Δφ is within [-π, π] range
            if (sigDeltaPhi > M_PI) {
                sigDeltaPhi = 2 * M_PI - sigDeltaPhi;
            }

            double sigCoshDeltaEta = cosh(sigDeltaEta);
            double sigCosDeltaPhi = cos(sigDeltaPhi);
            double sigMjj = sqrt(2.0 * sig_LRJ_Et[i][0] * sig_LRJ_Et[i][1] * sigCoshDeltaEta - sigCosDeltaPhi);
            //std::cout << "sigMjj: " << sigMjj << "\n";

            sig_h_Mjj->Fill(sigMjj);
        } // loop through signal events

        for (int i = 0; i < num_processed_events_background; i++) {
            jetTaggerLRJsBack->GetEntry(i);
            jetTaggerLeadingLRJsBack->GetEntry(i);
            jetTaggerSubleadingLRJsBack->GetEntry(i);
            eventInfoTreeBack->GetEntry(i);
            truthAntiKt4TruthDressedWZJetsBack->GetEntry(i);
            leadingTruthAntiKt4TruthDressedWZJetsBack->GetEntry(i);
            subleadingTruthAntiKt4TruthDressedWZJetsBack->GetEntry(i);
            leadingRecoAntiKt10UFOCSSKJetsBack->GetEntry(i);
            subleadingRecoAntiKt10UFOCSSKJetsBack->GetEntry(i);
            jFexLeadingLRJTreeBack->GetEntry(i);
            gFexLeadingLRJTreeBack->GetEntry(i);
            jFexSubleadingLRJTreeBack->GetEntry(i);
            gFexSubleadingLRJTreeBack->GetEntry(i);
            jFexLeadingSRJTreeBack->GetEntry(i);
            jFexSubleadingSRJTreeBack->GetEntry(i);
            jFexSRJTreeBack->GetEntry(i);
            topo422TreeBack->GetEntry(i);
            gepBasicClustersTreeBack->GetEntry(i);

            double backgroundEventWeight = eventWeightsValuesBack->at(0);

            back_h_leading_LRJ_Et_arr[sampleJZSliceValuesBack]->Fill(jetTaggerLeadingLRJEtValuesBack->at(0), backgroundEventWeight);
            back_h_subleading_LRJ_Et_arr[sampleJZSliceValuesBack]->Fill(jetTaggerSubleadingLRJEtValuesBack->at(0), backgroundEventWeight);
            back_h_gFEX_leading_LRJ_Et_arr[sampleJZSliceValuesBack]->Fill(gFexLRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            back_h_gFEX_subleading_LRJ_Et_arr[sampleJZSliceValuesBack]->Fill(gFexLRJSubleadingEtValuesBack->at(0), backgroundEventWeight);
            if(jFexLRJLeadingEtValuesBack->size() > 0) back_h_jFEX_leading_LRJ_Et_arr[sampleJZSliceValuesBack]->Fill(jFexLRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            if(jFexLRJSubleadingEtValuesBack->size() > 0) back_h_jFEX_subleading_LRJ_Et_arr[sampleJZSliceValuesBack]->Fill(jFexLRJSubleadingEtValuesBack->at(0), backgroundEventWeight);
            if(recoAntiKt10LRJLeadingEtValuesBack->size() > 0) back_h_leading_offlineLRJ_Et_arr[sampleJZSliceValuesBack]->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            if(recoAntiKt10LRJSubleadingEtValuesBack->size() > 0) back_h_subleading_offlineLRJ_Et_arr[sampleJZSliceValuesBack]->Fill(recoAntiKt10LRJSubleadingEtValuesBack->at(0), backgroundEventWeight);
            if(truthAntiKt4WZSRJLeadingEtValuesBack->size() > 0) back_h_leading_truthSRJs_Et_arr[sampleJZSliceValuesBack]->Fill(truthAntiKt4WZSRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            if(truthAntiKt4WZSRJSubleadingEtValuesBack->size() > 0) back_h_subleading_truthSRJs_Et_arr[sampleJZSliceValuesBack]->Fill(truthAntiKt4WZSRJSubleadingEtValuesBack->at(0), backgroundEventWeight);
            unsigned int highestEtIndexLRJBack = -1;
            
            if(back_LRJ_Et[i][0] >= back_LRJ_Et[i][1]) highestEtIndexLRJBack = 0;
            else highestEtIndexLRJBack = 1;

            double psi_R_BackJet1 = 0.0;
            double psi_R_BackJet2 = 0.0;
            double psi_R2_BackJet1 = 0.0;
            double psi_R2_BackJet2 = 0.0;
            
            for(auto& idxBackJet1 : *jetTaggerLeadingLRJMergedIndicesValuesBack){
                psi_R_BackJet1 += (1.0/jetTaggerLeadingLRJEtValuesBack->at(0)) * gepBasicClustersEtValuesBack->at(idxBackJet1) * sqrt(calcDeltaR2(jetTaggerLeadingLRJEtaValuesBack->at(0), jetTaggerLeadingLRJPhiValuesBack->at(0), gepBasicClustersEtaValuesBack->at(idxBackJet1), gepBasicClustersPhiValuesBack->at(idxBackJet1)));
                psi_R2_BackJet1 += (1.0/jetTaggerLeadingLRJEtValuesBack->at(0)) * gepBasicClustersEtValuesBack->at(idxBackJet1) * calcDeltaR2(jFexSRJLeadingEtaValuesBack->at(0), jFexSRJLeadingPhiValuesBack->at(0), gepBasicClustersEtaValuesBack->at(idxBackJet1), gepBasicClustersPhiValuesBack->at(idxBackJet1));
            }
            for(auto& idxBackJet2 : *jetTaggerSubleadingLRJMergedIndicesValuesBack){
                psi_R_BackJet2 += (1.0/jetTaggerSubleadingLRJEtValuesBack->at(0)) * gepBasicClustersEtValuesBack->at(idxBackJet2) * sqrt(calcDeltaR2(jetTaggerSubleadingLRJEtaValuesBack->at(0), jetTaggerSubleadingLRJPhiValuesBack->at(0), gepBasicClustersEtaValuesBack->at(idxBackJet2), gepBasicClustersPhiValuesBack->at(idxBackJet2)));
                psi_R2_BackJet2 += (1.0/jetTaggerSubleadingLRJEtValuesBack->at(0)) * gepBasicClustersEtValuesBack->at(idxBackJet2) * calcDeltaR2(jFexSRJSubleadingEtaValuesBack->at(0), jFexSRJSubleadingPhiValuesBack->at(0), gepBasicClustersEtaValuesBack->at(idxBackJet2), gepBasicClustersPhiValuesBack->at(idxBackJet2));
            }

            back_h_LRJ_psi_R_squared->Fill(psi_R_BackJet1 * psi_R_BackJet2, backgroundEventWeight);
            backOfflineLeadingLRJEtvsPsi_R_squared->Fill(std::max(back_LRJ_Et[i][0], back_LRJ_Et[i][1]), psi_R_BackJet1 * psi_R_BackJet2, backgroundEventWeight);
            backOfflineSubleadingLRJEtvsPsi_R_squared->Fill(std::min(back_LRJ_Et[i][0], back_LRJ_Et[i][1]), psi_R_BackJet1 * psi_R_BackJet2, backgroundEventWeight);
            back_h_LRJ_psi_R2_squared->Fill(psi_R2_BackJet1 * psi_R2_BackJet2, backgroundEventWeight);
            backOfflineLeadingLRJEtvsPsi_R2_squared->Fill(std::max(back_LRJ_Et[i][0], back_LRJ_Et[i][1]), psi_R2_BackJet1 * psi_R2_BackJet2, backgroundEventWeight);
            backOfflineSubleadingLRJEtvsPsi_R2_squared->Fill(std::min(back_LRJ_Et[i][0], back_LRJ_Et[i][1]), psi_R2_BackJet1 * psi_R2_BackJet2, backgroundEventWeight);
            back_h_leading_LRJ_psi_R->Fill(psi_R_BackJet1, backgroundEventWeight);
            back_h_subleading_LRJ_psi_R->Fill(psi_R_BackJet2, backgroundEventWeight);
            backOfflineLeadingLRJEtvsPsi_R->Fill(jetTaggerLeadingLRJEtValuesBack->at(0), psi_R_BackJet1, backgroundEventWeight);
            backOfflineSubleadingLRJEtvsPsi_R->Fill(jetTaggerSubleadingLRJEtValuesBack->at(0), psi_R_BackJet2, backgroundEventWeight);
            back_h_LRJ_psi_R2_squared->Fill(psi_R2_BackJet1 * psi_R2_BackJet2, backgroundEventWeight);
            backOfflineLeadingLRJEtvsPsi_R2_squared->Fill(jetTaggerLeadingLRJEtValuesBack->at(0), psi_R2_BackJet1 * psi_R2_BackJet2, backgroundEventWeight);
            backOfflineSubleadingLRJEtvsPsi_R2_squared->Fill(jetTaggerSubleadingLRJEtValuesBack->at(0), psi_R2_BackJet1 * psi_R2_BackJet2, backgroundEventWeight);

            // Fill with regular deltaR metric
            back_h_leading_LRJ_psi_R->Fill(psi_R_BackJet1, backgroundEventWeight);
            back_h_subleading_LRJ_psi_R->Fill(psi_R_BackJet2, backgroundEventWeight);
            back_h_LRJ_psi_R_12->Fill(psi_R_BackJet1 / psi_R_BackJet2, backgroundEventWeight);
            backOfflineLeadingLRJEtvsPsi_12->Fill(jetTaggerLeadingLRJEtValuesBack->at(0), psi_R_BackJet1 / psi_R_BackJet2, backgroundEventWeight);
            backOfflineSubleadingLRJEtvsPsi_12->Fill(jetTaggerSubleadingLRJEtValuesBack->at(0), psi_R_BackJet1 / psi_R_BackJet2, backgroundEventWeight);
            
            backOfflineLeadingLRJEtvsPsi_R->Fill(jetTaggerLeadingLRJEtValuesBack->at(0), psi_R_BackJet1, backgroundEventWeight);
            backOfflineSubleadingLRJEtvsPsi_R->Fill(jetTaggerSubleadingLRJEtValuesBack->at(0), psi_R_BackJet2, backgroundEventWeight);
            backOfflineLeadingLRJPsi_RvsSubleadingPsi_R->Fill(psi_R_BackJet2, psi_R_BackJet1, backgroundEventWeight); // subleading filled to x-axis
            // Fill with deltaR^2 metric
            back_h_leading_LRJ_psi_R2->Fill(psi_R2_BackJet1, backgroundEventWeight);
            back_h_subleading_LRJ_psi_R2->Fill(psi_R2_BackJet2, backgroundEventWeight);
            back_h_LRJ_psi_R2_12->Fill(psi_R2_BackJet1 / psi_R2_BackJet2, backgroundEventWeight);
            backOfflineLeadingLRJEtvsPsi_R2->Fill(jetTaggerLeadingLRJEtValuesBack->at(0), psi_R2_BackJet1, backgroundEventWeight);
            backOfflineSubleadingLRJEtvsPsi_R2->Fill(jetTaggerSubleadingLRJEtValuesBack->at(0), psi_R2_BackJet2, backgroundEventWeight);
            backOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2->Fill(psi_R2_BackJet2, psi_R2_BackJet1, backgroundEventWeight); // subleading filled to x-axis


            back_h_LRJ1_deltaEt_digitized_double->Fill((back_LRJ_Et[i][0] - jetTaggerLeadingLRJEtValuesBack->at(0)), backgroundEventWeight);
            back_h_LRJ2_deltaEt_digitized_double->Fill((back_LRJ_Et[i][1] - jetTaggerSubleadingLRJEtValuesBack->at(0)), backgroundEventWeight);

            if(recoAntiKt10LRJLeadingEtValuesBack->size() == 0) continue;
            // Mass vs. Et for Leading Offline LRJ 
            backOfflineLeadingLRJMassvsEt->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), recoAntiKt10LRJLeadingMassValuesBack->at(0), backgroundEventWeight);
            backOfflineLeadingLRJMass->Fill(recoAntiKt10LRJLeadingMassValuesBack->at(0), backgroundEventWeight);
            back_h_leading_offlineLRJ_Et->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            
            if(recoAntiKt10LRJSubleadingEtValuesBack->size() > 0) back_h_subleading_offlineLRJ_Et->Fill(recoAntiKt10LRJSubleadingEtValuesBack->at(0), backgroundEventWeight);
            
            // ------------------- BACKGROUND -------------------
            const auto& etBkg  = *truthAntiKt4WZSRJEtValuesBack;
            const auto& etaBkg = *truthAntiKt4WZSRJEtaValuesBack;
            const auto& phiBkg = *truthAntiKt4WZSRJPhiValuesBack;

            const double lrjEtaBkg = recoAntiKt10LRJLeadingEtaValuesBack->at(0);
            const double lrjPhiBkg = recoAntiKt10LRJLeadingPhiValuesBack->at(0);

            // ---- Tunable parameters ----
            const double subjetEtCutoff           = 25.0;  // as before
            const double drMatchToLRJ             = 1.0;   // match truth subjets to leading large-R jet
            const double drMinBetweenSubjets      = 0.4;   // NEW: minimum deltaR separation between counted subjets

// ---- Utility: select non-overlapping subjets (greedy by Et desc) ----
            auto selectNonOverlappingSubjets = [&](const std::vector<double>& et,
                                                const std::vector<double>& eta,
                                                const std::vector<double>& phi,
                                                double lrjEta, double lrjPhi,
                                                TH1* etHist)
            {
                const size_t n = et.size();

                // 1) First, collect candidates within drMatchToLRJ to the leading LRJ (for plotting) 
                //    and build a list of indices that ALSO pass the Et cut (for counting).
                std::vector<size_t> candidates; candidates.reserve(n);
                for (size_t i = 0; i < n; ++i) {
                    const double dR = std::sqrt(calcDeltaR2(eta[i], phi[i], lrjEta, lrjPhi));
                    if (dR < drMatchToLRJ) {
                        if (etHist) etHist->Fill(et[i]);          // unchanged behavior for plotting
                        if (et[i] >= subjetEtCutoff) {
                            candidates.push_back(i);              // for counting (subject to overlap removal)
                        }
                    }
                }

                // 2) Sort candidates by Et descending so we keep the highest-Et subjets when enforcing separation
                std::sort(candidates.begin(), candidates.end(),
                        [&](size_t a, size_t b){ return et[a] > et[b]; });

                // 3) Greedily select non-overlapping subjets (ΔR > drMinBetweenSubjets)
                std::vector<size_t> selected; selected.reserve(candidates.size());
                for (size_t idx : candidates) {
                    bool overlaps = false;
                    for (size_t jdx : selected) {
                        const double dR = std::sqrt(calcDeltaR2(eta[idx], phi[idx], eta[jdx], phi[jdx]));
                        if (dR <= drMinBetweenSubjets) { overlaps = true; break; }
                    }
                    if (!overlaps) selected.push_back(idx);
                }

                return selected.size(); // this is the multiplicity you want to Fill(...)
            };
            
            const unsigned int backgroundSubjetCounter =
                selectNonOverlappingSubjets(etBkg, etaBkg, phiBkg, lrjEtaBkg, lrjPhiBkg,
                                            back_h_LeadingOfflineLRJ_SubjetEt);

            back_h_LeadingOfflineLRJ_SubjetMultiplicity->Fill(backgroundSubjetCounter, backgroundEventWeight);
            backOfflineLeadingLRJEtvsSubjetMult->Fill(backgroundSubjetCounter, recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            backOfflineLeadingLRJMassvsSubjetMult->Fill(backgroundSubjetCounter, recoAntiKt10LRJLeadingMassValuesBack->at(0), backgroundEventWeight);

            // Delta R, delta Et for leading gFEX, jFEX SRJ (seeds), Offline LRJs
            back_h_leading_LRJ_gFexLRJ_deltaEt->Fill(gFexLRJLeadingEtValuesBack->at(0) - jetTaggerLeadingLRJEtValuesBack->at(0), backgroundEventWeight);
            back_h_leading_offlineLRJ_gFexLRJ_deltaEt->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0) - gFexLRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            back_h_leading_offlineLRJ_jFexLRJ_deltaEt->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0) - jFexLRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            back_h_leading_LRJ_offlineLRJ_deltaEt->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0) - jetTaggerLeadingLRJEtValuesBack->at(0), backgroundEventWeight);
            // Fill "resolution"
            back_h_leading_LRJ_gFexLRJ_Et_resolution->Fill(((gFexLRJLeadingEtValuesBack->at(0) - jetTaggerLeadingLRJEtValuesBack->at(0))/ gFexLRJLeadingEtValuesBack->at(0)), backgroundEventWeight);
            back_h_leading_offlineLRJ_gFexLRJ_Et_resolution->Fill((recoAntiKt10LRJLeadingEtValuesBack->at(0) - gFexLRJLeadingEtValuesBack->at(0))/recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            back_h_leading_offlineLRJ_jFexLRJ_Et_resolution->Fill((recoAntiKt10LRJLeadingEtValuesBack->at(0) - jFexLRJLeadingEtValuesBack->at(0))/recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            back_h_leading_LRJ_offlineLRJ_Et_resolution->Fill(((recoAntiKt10LRJLeadingEtValuesBack->at(0) - jetTaggerLeadingLRJEtValuesBack->at(0))/ recoAntiKt10LRJLeadingEtValuesBack->at(0)), backgroundEventWeight);

            back_h_leading_LRJ_gFexLRJ_deltaR->Fill(sqrt(calcDeltaR2(gFexLRJLeadingEtaValuesBack->at(0), gFexLRJLeadingPhiValuesBack->at(0), back_LRJ_Eta[i][highestEtIndexLRJBack], back_LRJ_Phi[i][highestEtIndexLRJBack])), backgroundEventWeight);
            
            back_h_leading_LRJ_offlineLRJ_deltaR->Fill(sqrt(calcDeltaR2(recoAntiKt10LRJLeadingEtaValuesBack->at(0), recoAntiKt10LRJLeadingPhiValuesBack->at(0), back_LRJ_Eta[i][highestEtIndexLRJBack], back_LRJ_Phi[i][highestEtIndexLRJBack])), backgroundEventWeight);
            back_h_first_LRJ_jFexSRJ_deltaR->Fill(sqrt(calcDeltaR2(jFexSRJLeadingEtaValuesBack->at(0), jFexSRJLeadingPhiValuesBack->at(0), back_LRJ_Eta[i][0], back_LRJ_Phi[i][0])));
            back_h_second_LRJ_jFexSRJ_deltaR->Fill(sqrt(calcDeltaR2(jFexSRJSubleadingEtaValuesBack->at(0), jFexSRJSubleadingPhiValuesBack->at(0), back_LRJ_Eta[i][1], back_LRJ_Phi[i][1])), backgroundEventWeight);
            back_h_lead_sublead_LRJ_deltaR->Fill(sqrt(calcDeltaR2(back_LRJ_Eta[i][0], back_LRJ_Phi[i][0], back_LRJ_Eta[i][1], back_LRJ_Phi[i][1])), backgroundEventWeight);

            // Dijet efficiencies (background)
            if(backgroundSubjetCounter == 1){
                if(back_LRJ_Et[i][0] >= 100.0 || back_LRJ_Et[i][1] >= 100.0){
                    back_h_offlineLRJ_Et_num100_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
                }
                back_h_offlineLRJ_Et_denom100_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

                if(back_LRJ_Et[i][0] >= 200.0 || back_LRJ_Et[i][1] >= 200.0){
                    back_h_offlineLRJ_Et_num200_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
                }
                back_h_offlineLRJ_Et_denom200_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

                if(back_LRJ_Et[i][0] >= 300.0 || back_LRJ_Et[i][1] >= 300.0){
                    back_h_offlineLRJ_Et_num300_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
                }
                back_h_offlineLRJ_Et_denom300_1Subjet->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }

            if(backgroundSubjetCounter >= 2){
                if(back_LRJ_Et[i][0] >= 100.0 || back_LRJ_Et[i][1] >= 100.0){
                    back_h_offlineLRJ_Et_num100_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
                }
                back_h_offlineLRJ_Et_denom100_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

                if(back_LRJ_Et[i][0] >= 200.0 || back_LRJ_Et[i][1] >= 200.0){
                    back_h_offlineLRJ_Et_num200_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
                }
                back_h_offlineLRJ_Et_denom200_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);


                if(back_LRJ_Et[i][0] >= 300.0 || back_LRJ_Et[i][1] >= 300.0){
                    back_h_offlineLRJ_Et_num300_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
                }
                back_h_offlineLRJ_Et_denom300_GrEq2Subjets->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }

            // Fill background jFEX trigger efficiencies (1 jet) 
            if(jFexLRJLeadingEtValuesBack->at(0) >= 50.0){
                back_h_offlineLRJ_Et_num50_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom50_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(jFexLRJLeadingEtValuesBack->at(0) >= 100.0){
                back_h_offlineLRJ_Et_num100_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom100_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(jFexLRJLeadingEtValuesBack->at(0) >= 150.0){
                back_h_offlineLRJ_Et_num150_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom150_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(jFexLRJLeadingEtValuesBack->at(0) >= 200.0){
                back_h_offlineLRJ_Et_num200_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom200_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(jFexLRJLeadingEtValuesBack->at(0) >= 250.0){
                back_h_offlineLRJ_Et_num250_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom250_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(jFexLRJLeadingEtValuesBack->at(0) >= 300.0){
                back_h_offlineLRJ_Et_num300_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom300_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(jFexLRJLeadingEtValuesBack->at(0) >= 350.0){
                back_h_offlineLRJ_Et_num350_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom350_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(jFexLRJLeadingEtValuesBack->at(0) >= 400.0){
                back_h_offlineLRJ_Et_num400_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom400_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(jFexLRJLeadingEtValuesBack->at(0) >= 450.0){
                back_h_offlineLRJ_Et_num450_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom450_jFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            // Fill background gFex trigger efficiencies
            if(gFexLRJLeadingEtValuesBack->at(0) >= 50.0){
                back_h_offlineLRJ_Et_num50_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom50_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(gFexLRJLeadingEtValuesBack->at(0) >= 100.0){
                back_h_offlineLRJ_Et_num100_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom100_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(gFexLRJLeadingEtValuesBack->at(0) >= 150.0){
                back_h_offlineLRJ_Et_num150_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom150_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(gFexLRJLeadingEtValuesBack->at(0) >= 200.0){
                back_h_offlineLRJ_Et_num200_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom200_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(gFexLRJLeadingEtValuesBack->at(0) >= 250.0){
                back_h_offlineLRJ_Et_num250_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom250_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(gFexLRJLeadingEtValuesBack->at(0) >= 300.0){
                back_h_offlineLRJ_Et_num300_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom300_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(gFexLRJLeadingEtValuesBack->at(0) >= 350.0){
                back_h_offlineLRJ_Et_num350_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom350_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(gFexLRJLeadingEtValuesBack->at(0) >= 400.0){
                back_h_offlineLRJ_Et_num400_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom400_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(gFexLRJLeadingEtValuesBack->at(0) >= 450.0){
                back_h_offlineLRJ_Et_num450_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom450_gFexLRJ->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            // Fill background JetTagger trigger efficiencies
            if(back_LRJ_Et[i][0] >= 50.0 || back_LRJ_Et[i][1] >= 50.0){
                back_h_offlineLRJ_Et_num50->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom50->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(back_LRJ_Et[i][0] >= 100.0 || back_LRJ_Et[i][1] >= 100.0){
                h_back_num100->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
                back_h_offlineLRJ_Et_num100->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            h_back_denom100->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            back_h_offlineLRJ_Et_denom100->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(back_LRJ_Et[i][0] >= 150.0 || back_LRJ_Et[i][1] >= 150.0){
                back_h_offlineLRJ_Et_num150->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom150->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(back_LRJ_Et[i][0] >= 200.0 || back_LRJ_Et[i][1] >= 200.0){
                h_back_num200->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
                back_h_offlineLRJ_Et_num200->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            h_back_denom200->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            back_h_offlineLRJ_Et_denom200->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(back_LRJ_Et[i][0] >= 250.0 || back_LRJ_Et[i][1] >= 250.0){
                back_h_offlineLRJ_Et_num250->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom250->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(back_LRJ_Et[i][0] >= 300.0 || back_LRJ_Et[i][1] >= 300.0){
                h_back_num300->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
                back_h_offlineLRJ_Et_num300->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            h_back_denom300->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            back_h_offlineLRJ_Et_denom300->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(back_LRJ_Et[i][0] >= 350.0 || back_LRJ_Et[i][1] >= 350.0){
                back_h_offlineLRJ_Et_num350->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom350->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(back_LRJ_Et[i][0] >= 400.0 || back_LRJ_Et[i][1] >= 400.0){
                back_h_offlineLRJ_Et_num400->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom400->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            if(back_LRJ_Et[i][0] >= 450.0 || back_LRJ_Et[i][1] >= 450.0){
                back_h_offlineLRJ_Et_num450->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);
            }
            back_h_offlineLRJ_Et_denom450->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0), backgroundEventWeight);

            
            if(i == evDisplay0Back_){
                if(jFexSRJEtaValuesBack->size() >= 6){
                    displayEv0Back_ = true;
                    jFexSeedPositionsBackEv0.push_back(std::make_pair(jFexSRJEtaValuesBack->at(0), jFexSRJPhiValuesBack->at(0)));
                    jFexSeedPositionsBackEv0.push_back(std::make_pair(jFexSRJEtaValuesBack->at(1), jFexSRJPhiValuesBack->at(1)));
                    jFexAdditionalSRJPositionsBackEv0.push_back(std::make_pair(jFexSRJEtaValuesBack->at(2), jFexSRJPhiValuesBack->at(2)));
                    jFexAdditionalSRJPositionsBackEv0.push_back(std::make_pair(jFexSRJEtaValuesBack->at(3), jFexSRJPhiValuesBack->at(3)));
                    jFexAdditionalSRJPositionsBackEv0.push_back(std::make_pair(jFexSRJEtaValuesBack->at(4), jFexSRJPhiValuesBack->at(4)));
                    jFexAdditionalSRJPositionsBackEv0.push_back(std::make_pair(jFexSRJEtaValuesBack->at(5), jFexSRJPhiValuesBack->at(5)));
                    newSeedPositionsBackEv0.push_back(std::make_pair(back_LRJ_Eta[i][0], back_LRJ_Phi[i][0]));
                    newSeedPositionsBackEv0.push_back(std::make_pair(back_LRJ_Eta[i][1], back_LRJ_Phi[i][1]));
                    backLRJEtEv0.push_back(back_LRJ_Et[i][0]);
                    backLRJEtEv0.push_back(back_LRJ_Et[i][1]);
                }
                
            }
            if(i == evDisplay1Back_){
                if(jFexSRJEtaValuesBack->size() >= 6){
                    displayEv1Back_ = true;
                    jFexSeedPositionsBackEv1.push_back(std::make_pair(jFexSRJEtaValuesBack->at(0), jFexSRJPhiValuesBack->at(0)));
                    jFexSeedPositionsBackEv1.push_back(std::make_pair(jFexSRJEtaValuesBack->at(1), jFexSRJPhiValuesBack->at(1)));
                    jFexAdditionalSRJPositionsBackEv1.push_back(std::make_pair(jFexSRJEtaValuesBack->at(2), jFexSRJPhiValuesBack->at(2)));
                    jFexAdditionalSRJPositionsBackEv1.push_back(std::make_pair(jFexSRJEtaValuesBack->at(3), jFexSRJPhiValuesBack->at(3)));
                    jFexAdditionalSRJPositionsBackEv1.push_back(std::make_pair(jFexSRJEtaValuesBack->at(4), jFexSRJPhiValuesBack->at(4)));
                    jFexAdditionalSRJPositionsBackEv1.push_back(std::make_pair(jFexSRJEtaValuesBack->at(5), jFexSRJPhiValuesBack->at(5)));
                    newSeedPositionsBackEv1.push_back(std::make_pair(back_LRJ_Eta[i][0], back_LRJ_Phi[i][0]));
                    newSeedPositionsBackEv1.push_back(std::make_pair(back_LRJ_Eta[i][1], back_LRJ_Phi[i][1]));
                    backLRJEtEv1.push_back(back_LRJ_Et[i][0]);
                    backLRJEtEv1.push_back(back_LRJ_Et[i][1]);
                }
                
            }
            if(i == evDisplay2Back_){
                if(jFexSRJEtaValuesBack->size() >= 6){
                    displayEv2Back_ = true;
                    jFexSeedPositionsBackEv2.push_back(std::make_pair(jFexSRJEtaValuesBack->at(0), jFexSRJPhiValuesBack->at(0)));
                    jFexSeedPositionsBackEv2.push_back(std::make_pair(jFexSRJEtaValuesBack->at(1), jFexSRJPhiValuesBack->at(1)));
                    jFexAdditionalSRJPositionsBackEv2.push_back(std::make_pair(jFexSRJEtaValuesBack->at(2), jFexSRJPhiValuesBack->at(2)));
                    jFexAdditionalSRJPositionsBackEv2.push_back(std::make_pair(jFexSRJEtaValuesBack->at(3), jFexSRJPhiValuesBack->at(3)));
                    jFexAdditionalSRJPositionsBackEv2.push_back(std::make_pair(jFexSRJEtaValuesBack->at(4), jFexSRJPhiValuesBack->at(4)));
                    jFexAdditionalSRJPositionsBackEv2.push_back(std::make_pair(jFexSRJEtaValuesBack->at(5), jFexSRJPhiValuesBack->at(5)));
                    newSeedPositionsBackEv2.push_back(std::make_pair(back_LRJ_Eta[i][0], back_LRJ_Phi[i][0]));
                    newSeedPositionsBackEv2.push_back(std::make_pair(back_LRJ_Eta[i][1], back_LRJ_Phi[i][1]));
                    backLRJEtEv2.push_back(back_LRJ_Et[i][0]);
                    backLRJEtEv2.push_back(back_LRJ_Et[i][1]);
                }
                
            }
            if(i == evDisplay3Back_){
                if(jFexSRJEtaValuesBack->size() >= 6){
                    displayEv3Back_ = true;
                    jFexSeedPositionsBackEv3.push_back(std::make_pair(jFexSRJEtaValuesBack->at(0), jFexSRJPhiValuesBack->at(0)));
                    jFexSeedPositionsBackEv3.push_back(std::make_pair(jFexSRJEtaValuesBack->at(1), jFexSRJPhiValuesBack->at(1)));
                    jFexAdditionalSRJPositionsBackEv3.push_back(std::make_pair(jFexSRJEtaValuesBack->at(2), jFexSRJPhiValuesBack->at(2)));
                    jFexAdditionalSRJPositionsBackEv3.push_back(std::make_pair(jFexSRJEtaValuesBack->at(3), jFexSRJPhiValuesBack->at(3)));
                    jFexAdditionalSRJPositionsBackEv3.push_back(std::make_pair(jFexSRJEtaValuesBack->at(4), jFexSRJPhiValuesBack->at(4)));
                    jFexAdditionalSRJPositionsBackEv3.push_back(std::make_pair(jFexSRJEtaValuesBack->at(5), jFexSRJPhiValuesBack->at(5)));
                    newSeedPositionsBackEv3.push_back(std::make_pair(back_LRJ_Eta[i][0], back_LRJ_Phi[i][0]));
                    newSeedPositionsBackEv3.push_back(std::make_pair(back_LRJ_Eta[i][1], back_LRJ_Phi[i][1]));
                    backLRJEtEv3.push_back(back_LRJ_Et[i][0]);
                    backLRJEtEv3.push_back(back_LRJ_Et[i][1]);
                }
                
            }
            if(i == evDisplay4Back_){
                if(jFexSRJEtaValuesBack->size() >= 6){
                    displayEv4Back_ = true; 
                    jFexSeedPositionsBackEv4.push_back(std::make_pair(jFexSRJEtaValuesBack->at(0), jFexSRJPhiValuesBack->at(0)));
                    jFexSeedPositionsBackEv4.push_back(std::make_pair(jFexSRJEtaValuesBack->at(1), jFexSRJPhiValuesBack->at(1)));
                    jFexAdditionalSRJPositionsBackEv4.push_back(std::make_pair(jFexSRJEtaValuesBack->at(2), jFexSRJPhiValuesBack->at(2)));
                    jFexAdditionalSRJPositionsBackEv4.push_back(std::make_pair(jFexSRJEtaValuesBack->at(3), jFexSRJPhiValuesBack->at(3)));
                    jFexAdditionalSRJPositionsBackEv4.push_back(std::make_pair(jFexSRJEtaValuesBack->at(4), jFexSRJPhiValuesBack->at(4)));
                    jFexAdditionalSRJPositionsBackEv4.push_back(std::make_pair(jFexSRJEtaValuesBack->at(5), jFexSRJPhiValuesBack->at(5)));
                    newSeedPositionsBackEv4.push_back(std::make_pair(back_LRJ_Eta[i][0], back_LRJ_Phi[i][0]));
                    newSeedPositionsBackEv4.push_back(std::make_pair(back_LRJ_Eta[i][1], back_LRJ_Phi[i][1]));
                    backLRJEtEv4.push_back(back_LRJ_Et[i][0]);
                    backLRJEtEv4.push_back(back_LRJ_Et[i][1]);
                }
            }
            if(i <= evDisplayMaxBack_){
                for(unsigned int iTopo = 0; iTopo < nInputObjectsAlgorithmConfiguration; iTopo++){
                    if(i == evDisplay0Back_){
                        backTopo422Highest128SeedPositionsEv0->Fill(gepBasicClustersEtaValuesBack->at(iTopo), gepBasicClustersPhiValuesBack->at(iTopo), gepBasicClustersEtValuesBack->at(iTopo));
                    } 
                    if(i == evDisplay1Back_){
                        backTopo422Highest128SeedPositionsEv1->Fill(gepBasicClustersEtaValuesBack->at(iTopo), gepBasicClustersPhiValuesBack->at(iTopo), gepBasicClustersEtValuesBack->at(iTopo));
                    } 
                    if(i == evDisplay2Back_){
                        backTopo422Highest128SeedPositionsEv2->Fill(gepBasicClustersEtaValuesBack->at(iTopo), gepBasicClustersPhiValuesBack->at(iTopo), gepBasicClustersEtValuesBack->at(iTopo));
                    } 
                    if(i == evDisplay3Back_){
                        backTopo422Highest128SeedPositionsEv3->Fill(gepBasicClustersEtaValuesBack->at(iTopo), gepBasicClustersPhiValuesBack->at(iTopo), gepBasicClustersEtValuesBack->at(iTopo));
                    } 
                    if(i == evDisplay4Back_){
                        backTopo422Highest128SeedPositionsEv4->Fill(gepBasicClustersEtaValuesBack->at(iTopo), gepBasicClustersPhiValuesBack->at(iTopo), gepBasicClustersEtValuesBack->at(iTopo));
                    } 
                }
            }

            // Compute invariant mass using JetTagger LRJs for background
            double backDeltaEta = back_LRJ_Eta[i][0] - back_LRJ_Eta[i][1];
            double backDeltaPhi = std::abs(back_LRJ_Phi[i][0] - back_LRJ_Phi[i][1]);
            // Ensure Δφ is within [-π, π] range
            if (backDeltaPhi > M_PI) {
                backDeltaPhi = 2 * M_PI - backDeltaPhi;
            }

            double backCoshDeltaEta = cosh(backDeltaEta);
            double backCosDeltaPhi = cos(backDeltaPhi);

            double backMjj = sqrt(2.0 * back_LRJ_Et[i][0] * back_LRJ_Et[i][1] * backCoshDeltaEta - backCosDeltaPhi);

            back_h_Mjj->Fill(backMjj, backgroundEventWeight);

        }

        double det_cutoff = 1.0;
        for (double et_cutoff = det_cutoff; et_cutoff < 500.0; et_cutoff += det_cutoff) {
            


            //std::cout << "back_LRJ_Et.size(): " << back_LRJ_Et.size() << " and sig_LRJ_Et.size(): " << sig_LRJ_Et.size() << "\n";
            int numTruePositive = 0;
            int numTruePositive2 = 0;
            
            for (int i = 0; i < num_processed_events_signal; i++) {
                //std::cout << "i: " << i << "\n";
                //std::cout << "i:  " << i << "\n";
                //for (int j = 0; j < sig_LRJ_Et[current_event]; j++){
                //   if ()
                //}
                //std::cout << "sig_LRJ_Et[i][0]: " << sig_LRJ_Et[i][0] << " and i " << i << "\n";
                if(sig_LRJ_Et[i][0] >= et_cutoff || sig_LRJ_Et[i][1] >= et_cutoff){
                    numTruePositive++;
                }
                if(sig_LRJ_Et[i][0] >= et_cutoff && sig_LRJ_Et[i][1] >= et_cutoff){
                    numTruePositive2++;
                }
            }
        

            double truePositiveRate = ((double)numTruePositive)/(num_processed_events_signal);
            double truePositiveRate2 = ((double)numTruePositive2)/(num_processed_events_signal);
            //std::cout << "truePositiveRate: " << truePositiveRate << " for et_cutoff: " << et_cutoff << "\n";
            roc_curve_points_x[fileIt].emplace_back(truePositiveRate);
            roc_curve_points_x2[fileIt].emplace_back(truePositiveRate2);

            efficiency_curve_points_x[fileIt].emplace_back(et_cutoff);
            efficiency_curve_points_y[fileIt].emplace_back(truePositiveRate);
            efficiency_curve_points_y2[fileIt].emplace_back(truePositiveRate2);

            int numFalsePositive = 0;
            int numFalsePositive2 = 0;
            for (int i = 0; i < num_processed_events_background; i++) {
                //std::cout << "back_LRJ_Et[i][0]: " << back_LRJ_Et[i][0] << " and i : " << i << "\n";
                if(back_LRJ_Et[i][0] >= et_cutoff || back_LRJ_Et[i][1] >= et_cutoff){
                    numFalsePositive++;
                }
                if(back_LRJ_Et[i][0] >= et_cutoff && back_LRJ_Et[i][1] >= et_cutoff){
                    numFalsePositive2++;
                }
            }

            double falsePositiveRate = ((double)numFalsePositive)/((double) num_processed_events_background);
            double falsePositiveRate2 = ((double)numFalsePositive2)/((double) num_processed_events_background);
            if (falsePositiveRate == 0){
                //std::cout << "FPR IS 0!!" << "\n";
                falsePositiveRate = 1.0/num_processed_events_background; // /10
                //std::cout << "FPR WHEN FPR IS 0: " << falsePositiveRate << "\n";
            } 
            if (falsePositiveRate2 == 0){
                //std::cout << "FPR2 IS 0!!" << "\n";
                falsePositiveRate2 = 1.0/num_processed_events_background; 
            } 
            double backgroundRejection = 1.0 / falsePositiveRate;
            double backgroundRejection2 = 1.0 / falsePositiveRate2;
            //std::cout << " backgroundRejectioN: "  << backgroundRejection << " for et_cutoff: " << et_cutoff << " and file: " << algorithmConfigurations[fileIt] << "\n";
            roc_curve_points_y[fileIt].emplace_back(backgroundRejection);
            roc_curve_points_y2[fileIt].emplace_back(backgroundRejection2);
            //std::cout << "signal to background: " << std::fixed << std::setprecision(16) << backgroundRejection * truePositiveRate << "\n";
            if (truePositiveRate > 0.1) maxSignalToBackgroundRatioTPR0p1[fileIt].push_back(backgroundRejection * truePositiveRate);
            maxSignalToBackgroundRatio[fileIt].push_back(backgroundRejection * truePositiveRate);

            if (truePositiveRate2 > 0.1) maxSignalToBackgroundRatio2TPR0p1[fileIt].push_back(backgroundRejection2 * truePositiveRate2);
            maxSignalToBackgroundRatio2[fileIt].push_back(backgroundRejection2 * truePositiveRate2);
        }
        int numTruePositiveMinMaxCut = 0;
        int numFalsePositiveMinMaxCut = 0;
        for (int i = 0; i < num_processed_events_signal; i++) {
            if ((sig_LRJ_Et[i][0] > 100 && sig_LRJ_Et[i][0] < 200) && (sig_LRJ_Et[i][1] > 100 && sig_LRJ_Et[i][1] < 200)){
                numTruePositiveMinMaxCut++;
            }
        }
        for (int i = 0; i < num_processed_events_background; i++) {
            if ((back_LRJ_Et[i][0] > 100 && back_LRJ_Et[i][0] < 200) && (back_LRJ_Et[i][1] > 100 && back_LRJ_Et[i][1] < 200)){
                numFalsePositiveMinMaxCut++;
            }
        }
        tprMinMaxCut[fileIt] = double(numTruePositiveMinMaxCut)/ double(num_processed_events_signal );
        fprMinMaxCut[fileIt] = double(numFalsePositiveMinMaxCut)/ double(num_processed_events_background);

        // Start drawing and saving histograms and plots
        TString outputFileDir = "overlayLargeRJetHistograms/";
        
        TString modifiedOutputFileDir = "overlayLargeRJetHistograms_" + algorithmConfigurations[fileIt] + "/";

        gSystem->mkdir(modifiedOutputFileDir);

        TCanvas cAvgDR_vs_HpT("cAvgDR_vs_HpT","Avg dR vs HpT", 900, 700);
        prof_AvgDR_vs_HpT->SetMinimum(0.0);
        prof_AvgDR_vs_HpT->SetMaximum(5.2);
        prof_AvgDR_vs_HpT->Draw("E1");

        // ΔR=1 guide (optional)
        TLine lDR1(ptMin, 1.0, ptMax, 1.0); lDR1.SetLineColor(kBlue+1); lDR1.SetLineStyle(2); lDR1.Draw("same");

        // 2 mH_ / pT^H curve
        
        TF1 f_opening("f_opening", Form("%g*2.0/x", mH_), ptMin, ptMax);
        f_opening.SetLineColor(kRed);
        f_opening.SetLineStyle(2);
        f_opening.Draw("same");

        TLegend legdRvsHiggspTProfile(0.54, 0.72, 0.86, 0.90);
        legdRvsHiggspTProfile.SetBorderSize(0); legdRvsHiggspTProfile.SetFillStyle(0);
        legdRvsHiggspTProfile.SetTextSize(0.04);
        legdRvsHiggspTProfile.AddEntry(prof_AvgDR_vs_HpT, "Average #DeltaR", "lep");
        legdRvsHiggspTProfile.AddEntry(&lDR1, "#DeltaR = 1.0", "l");
        legdRvsHiggspTProfile.AddEntry(&f_opening, "2 m_{H} / p_{T}^{H}", "l");
        legdRvsHiggspTProfile.Draw();

        cAvgDR_vs_HpT.SaveAs(modifiedOutputFileDir + "avg_dR_vs_HpT_profile.pdf");


        TCanvas c2_TH2F("c2_TH2F","dR vs HpT 2D", 900, 700);
        h2_DR_vs_HpT->Draw("COLZ");
        // Legend in top-right for the fit and normalization info
        TLegend leg_TH2f(0.52, 0.80, 0.82, 0.93);
        TF1 f_opening2("f_opening2", Form("%g*2.0/x", mH_), ptMin, ptMax);
        f_opening2.SetLineColor(kBlack);
        f_opening2.SetLineStyle(3); // dotted
        f_opening2.SetLineWidth(2);
        f_opening2.Draw("same");
        leg_TH2f.SetBorderSize(0);
        leg_TH2f.SetTextSize(0.06);
        leg_TH2f.SetFillStyle(0);
        leg_TH2f.AddEntry(&f_opening2, "2 m_{H} / p_{T}^{H}", "l");
        //leg_TH2f.AddEntry(h2_DR_vs_HpT, "Normalized counts", "f");
        leg_TH2f.Draw();
        h2_DR_vs_HpT->Scale(1.0 / h2_DR_vs_HpT->Integral());
        c2_TH2F.SaveAs(modifiedOutputFileDir + "dR_vs_HpT_2D.pdf");

    
        //gSystem->RedirectOutput(0); // back to normal
        // Save histograms
        TCanvas c;
        TCanvas cSigEffb;
        TCanvas cSigEffLRJ;
        TCanvas cSigEffLRJ_Mass100to150;
        TCanvas cSigEffLRJ_1Subjet;
        TCanvas cBackEffLRJ_1Subjet;
        TCanvas cSigEffLRJ_Dijet;
        TCanvas cSigEffLRJ_GrEq2Subjets;
        TCanvas cBackEffLRJ_GrEq2Subjets;
        TCanvas cBackEffLRJ;
        TCanvas cSigEffLRJ_gFex;
        TCanvas cSigEffLRJ_jFex;
        TCanvas cSigEffLRJ_gFex_Dijet;
        TCanvas cBackEffLRJ_gFex;
        TCanvas cBackEffLRJ_jFex;
        TCanvas cSigEffLRJ_gFex_LRJ_overlay;
        TCanvas cSigEffLRJ_jFex_LRJ_overlay;
        TCanvas cSigEffLRJ_gFex_LRJ_Dijet_overlay;
        TCanvas cSigEffLRJ_Mass100to150_LRJ_overlay;
        TCanvas cSigEffLRJ_Overlay_1Subjet_GrEq2Subjets;
        TCanvas cBackEffLRJ_Overlay_1Subjet_GrEq2Subjets;
        TCanvas cBackEffLRJ_gFex_LRJ_overlay;
        TCanvas cBackEffLRJ_jFex_LRJ_overlay;
        
        //gStyle->SetOptTitle(1); // Enable title
        TLegend *leg = new TLegend(0.7,0.8,0.85,0.95);
        leg->SetTextSize(0.025);

        leg->AddEntry(sig_h_LRJ_Et, "Signal", "l");
        leg->AddEntry(back_h_LRJ_Et, "Background", "l");

        TLegend *leg_diam = new TLegend(0.4,0.8,0.55,0.95);
        leg_diam->SetTextSize(0.025);

        leg_diam->AddEntry(sig_h_LRJ_Et, "Signal", "l");
        leg_diam->AddEntry(back_h_LRJ_Et, "Background", "l");

        TLegend *legEffb = new TLegend(0.8,0.2,0.95,0.35);
        legEffb->SetTextSize(0.025);

        TLegend *sigLegEffLRJ = new TLegend(0.7,0.2,1.0,0.525);
        sigLegEffLRJ->SetTextSize(0.03);

        TLegend *sigLegEffLRJ_mass100to150 = new TLegend(0.7,0.2,1.0,0.525);
        sigLegEffLRJ_mass100to150->SetTextSize(0.03);

        TLegend *sigLegEffLRJ_1Subjet = new TLegend(0.7,0.2,1.0,0.525);
        sigLegEffLRJ_1Subjet->SetTextSize(0.03);

        TLegend *backLegEffLRJ_1Subjet = new TLegend(0.7,0.2,1.0,0.525);
        backLegEffLRJ_1Subjet->SetTextSize(0.03);

        TLegend *sigLegEffLRJ_Dijet = new TLegend(0.6,0.2,0.9,0.525);
        sigLegEffLRJ_Dijet->SetTextSize(0.03);

        TLegend *sigLegEffLRJ_GrEq2Subjets = new TLegend(0.7,0.2,1.0,0.525);
        sigLegEffLRJ_GrEq2Subjets->SetTextSize(0.03);

        TLegend *backLegEffLRJ_GrEq2Subjets = new TLegend(0.7,0.2,1.0,0.525);
        backLegEffLRJ_GrEq2Subjets->SetTextSize(0.03);

        TLegend *backLegEffLRJ = new TLegend(0.7,0.2,1.0,0.525);
        backLegEffLRJ->SetTextSize(0.03);

        TLegend *sigLegEffLRJ_gFex = new TLegend(0.7,0.2,1.0,0.525);
        sigLegEffLRJ_gFex->SetTextSize(0.03);

        TLegend *sigLegEffLRJ_jFex = new TLegend(0.7,0.2,1.0,0.525);
        sigLegEffLRJ_jFex->SetTextSize(0.03);

        TLegend *sigLegEffLRJ_gFex_Dijet = new TLegend(0.7,0.2,1.0,0.525);
        sigLegEffLRJ_gFex_Dijet->SetTextSize(0.03);

        TLegend *backLegEffLRJ_gFex = new TLegend(0.7,0.2,1.0,0.525);
        backLegEffLRJ_gFex->SetTextSize(0.03);

        TLegend *backLegEffLRJ_jFex = new TLegend(0.7,0.2,1.0,0.525);
        backLegEffLRJ_jFex->SetTextSize(0.03);

        c.cd();

        std::cout << "--------------------------------" << "\n";
        std::cout << "for: " << algorithmConfigurations[fileIt] << "\n";
        std::cout << "Signal mean: " << sig_h_LRJ_Et->GetMean() << " and background mean: " << back_h_LRJ_Et->GetMean() << "\n";
        sig_h_LRJ_Et->Scale(1.0 / sig_h_LRJ_Et->Integral());
        back_h_LRJ_Et->Scale(1.0 / back_h_LRJ_Et->Integral());
        sig_h_LRJ_Et->SetLineColor(kRed);
        back_h_LRJ_Et->SetLineColor(kBlue);
        sig_h_LRJ_Et->Draw("HIST");
        back_h_LRJ_Et->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "LRJ_Et.pdf");
        //gStyle->SetOptTitle(0); // disable title

        sig_h_leading_LRJ_psi_R->Scale(1.0 / sig_h_leading_LRJ_psi_R->Integral());
        back_h_leading_LRJ_psi_R->Scale(1.0 / back_h_leading_LRJ_psi_R->Integral());
        sig_h_leading_LRJ_psi_R->SetLineColor(kRed);
        back_h_leading_LRJ_psi_R->SetLineColor(kBlue);
        
        back_h_leading_LRJ_psi_R->Draw("HIST");
        sig_h_leading_LRJ_psi_R->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_LRJ_psi_R_JetTagger.pdf");

        sig_h_LRJ_psi_R_12->Scale(1.0 / sig_h_LRJ_psi_R_12->Integral());
        back_h_LRJ_psi_R_12->Scale(1.0 / back_h_LRJ_psi_R_12->Integral());
        sig_h_LRJ_psi_R_12->SetLineColor(kRed);
        back_h_LRJ_psi_R_12->SetLineColor(kBlue);
        
        back_h_LRJ_psi_R_12->Draw("HIST");
        sig_h_LRJ_psi_R_12->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "LRJ_psi_R_Leading_over_Subleading.pdf");

        sig_h_LRJ_psi_R_squared->Scale(1.0 / sig_h_LRJ_psi_R_squared->Integral());
        back_h_LRJ_psi_R_squared->Scale(1.0 / back_h_LRJ_psi_R_squared->Integral());
        sig_h_LRJ_psi_R_squared->SetLineColor(kRed);
        back_h_LRJ_psi_R_squared->SetLineColor(kBlue);
        
        back_h_LRJ_psi_R_squared->Draw("HIST");
        sig_h_LRJ_psi_R_squared->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_LRJ_psi_R_squared_JetTagger.pdf");

        sig_h_subleading_LRJ_psi_R->Scale(1.0 / sig_h_subleading_LRJ_psi_R->Integral());
        back_h_subleading_LRJ_psi_R->Scale(1.0 / back_h_subleading_LRJ_psi_R->Integral());
        sig_h_subleading_LRJ_psi_R->SetLineColor(kRed);
        back_h_subleading_LRJ_psi_R->SetLineColor(kBlue);
        back_h_subleading_LRJ_psi_R->Draw("HIST");
        sig_h_subleading_LRJ_psi_R->Draw("HIST SAME");
        
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "subleading_LRJ_psi_R_JetTagger.pdf");

        sig_h_LRJ1_deltaEt_digitized_double->Scale(1.0 / sig_h_LRJ1_deltaEt_digitized_double->Integral());
        back_h_LRJ1_deltaEt_digitized_double->Scale(1.0 / back_h_LRJ1_deltaEt_digitized_double->Integral());
        sig_h_LRJ1_deltaEt_digitized_double->SetLineColor(kRed);
        back_h_LRJ1_deltaEt_digitized_double->SetLineColor(kBlue);
        sig_h_LRJ1_deltaEt_digitized_double->Draw("HIST");
        back_h_LRJ1_deltaEt_digitized_double->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "LRJ1_JetTagger_deltaEt_digitized_floatingpoint.pdf");

        sig_h_LRJ2_deltaEt_digitized_double->Scale(1.0 / sig_h_LRJ2_deltaEt_digitized_double->Integral());
        back_h_LRJ2_deltaEt_digitized_double->Scale(1.0 / back_h_LRJ2_deltaEt_digitized_double->Integral());
        sig_h_LRJ2_deltaEt_digitized_double->SetLineColor(kRed);
        back_h_LRJ2_deltaEt_digitized_double->SetLineColor(kBlue);
        sig_h_LRJ2_deltaEt_digitized_double->Draw("HIST");
        back_h_LRJ2_deltaEt_digitized_double->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "LRJ2_JetTagger_deltaEt_digitized_floatingpoint.pdf");

        sig_h_Mjj->Scale(1.0 / sig_h_Mjj->Integral());
        back_h_Mjj->Scale(1.0 / back_h_Mjj->Integral());
        sig_h_Mjj->SetLineColor(kRed);
        back_h_Mjj->SetLineColor(kBlue);
        back_h_Mjj->Draw("HIST");
        sig_h_Mjj->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "Mjj.pdf");

        sig_h_LeadingOfflineLRJ_SubjetMultiplicity->SetLineColor(kRed);
        back_h_LeadingOfflineLRJ_SubjetMultiplicity->SetLineColor(kBlue);
        back_h_LeadingOfflineLRJ_SubjetMultiplicity->Scale(1.0 / back_h_LeadingOfflineLRJ_SubjetMultiplicity->Integral());
        back_h_LeadingOfflineLRJ_SubjetMultiplicity->Draw("HIST");
        sig_h_LeadingOfflineLRJ_SubjetMultiplicity->Scale(1.0 / sig_h_LeadingOfflineLRJ_SubjetMultiplicity->Integral());
        sig_h_LeadingOfflineLRJ_SubjetMultiplicity->Draw("HIST SAME");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "LeadingOfflineLRJ_SubjetMultiplicity.pdf");


        
        

        sig_h_LeadingOfflineLRJ_SubjetEt->SetLineColor(kRed);
        back_h_LeadingOfflineLRJ_SubjetEt->SetLineColor(kBlue);
        back_h_LeadingOfflineLRJ_SubjetEt->Scale(1.0 / num_processed_events_background);
        back_h_LeadingOfflineLRJ_SubjetEt->Draw("HIST");
        sig_h_LeadingOfflineLRJ_SubjetEt->Scale(1.0 / num_processed_events_signal);
        sig_h_LeadingOfflineLRJ_SubjetEt->Draw("HIST SAME");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "LeadingOfflineLRJ_SubjetEt.pdf");



        sig_h_leading_LRJ_gFexLRJ_deltaEt->Scale(1.0 / sig_h_leading_LRJ_gFexLRJ_deltaEt->Integral());
        back_h_leading_LRJ_gFexLRJ_deltaEt->Scale(1.0 / back_h_leading_LRJ_gFexLRJ_deltaEt->Integral());
        sig_h_leading_LRJ_gFexLRJ_deltaEt->SetLineColor(kRed);
        back_h_leading_LRJ_gFexLRJ_deltaEt->SetLineColor(kBlue);
        back_h_leading_LRJ_gFexLRJ_deltaEt->Draw("HIST");
        sig_h_leading_LRJ_gFexLRJ_deltaEt->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_LRJ_gFexLRJ_deltaEt.pdf");

        sig_h_leading_offlineLRJ_gFexLRJ_deltaEt->Scale(1.0 / sig_h_leading_offlineLRJ_gFexLRJ_deltaEt->Integral());
        back_h_leading_offlineLRJ_gFexLRJ_deltaEt->Scale(1.0 / back_h_leading_offlineLRJ_gFexLRJ_deltaEt->Integral());
        sig_h_leading_offlineLRJ_gFexLRJ_deltaEt->SetLineColor(kRed);
        back_h_leading_offlineLRJ_gFexLRJ_deltaEt->SetLineColor(kBlue);
        back_h_leading_offlineLRJ_gFexLRJ_deltaEt->Draw("HIST");
        sig_h_leading_offlineLRJ_gFexLRJ_deltaEt->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_offlineLRJ_gFexLRJ_deltaEt.pdf");

        sig_h_leading_offlineLRJ_jFexLRJ_deltaEt->Scale(1.0 / sig_h_leading_offlineLRJ_jFexLRJ_deltaEt->Integral());
        back_h_leading_offlineLRJ_jFexLRJ_deltaEt->Scale(1.0 / back_h_leading_offlineLRJ_jFexLRJ_deltaEt->Integral());
        sig_h_leading_offlineLRJ_jFexLRJ_deltaEt->SetLineColor(kRed);
        back_h_leading_offlineLRJ_jFexLRJ_deltaEt->SetLineColor(kBlue);
        back_h_leading_offlineLRJ_jFexLRJ_deltaEt->Draw("HIST");
        sig_h_leading_offlineLRJ_jFexLRJ_deltaEt->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_offlineLRJ_jFexLRJ_deltaEt.pdf");

        sig_h_leading_LRJ_offlineLRJ_deltaEt->Scale(1.0 / sig_h_leading_LRJ_offlineLRJ_deltaEt->Integral());
        back_h_leading_LRJ_offlineLRJ_deltaEt->Scale(1.0 / back_h_leading_LRJ_offlineLRJ_deltaEt->Integral());
        sig_h_leading_LRJ_offlineLRJ_deltaEt->SetLineColor(kRed);
        back_h_leading_LRJ_offlineLRJ_deltaEt->SetLineColor(kBlue);
        back_h_leading_LRJ_offlineLRJ_deltaEt->Draw("HIST");
        sig_h_leading_LRJ_offlineLRJ_deltaEt->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_LRJ_offlineLRJ_deltaEt.pdf");
        
        // Resolution plots
        sig_h_leading_LRJ_gFexLRJ_Et_resolution->Scale(1.0 / sig_h_leading_LRJ_gFexLRJ_Et_resolution->Integral());
        back_h_leading_LRJ_gFexLRJ_Et_resolution->Scale(1.0 / back_h_leading_LRJ_gFexLRJ_Et_resolution->Integral());
        sig_h_leading_LRJ_gFexLRJ_Et_resolution->SetLineColor(kRed);
        back_h_leading_LRJ_gFexLRJ_Et_resolution->SetLineColor(kBlue);
        sig_h_leading_LRJ_gFexLRJ_Et_resolution->Draw("HIST");
        back_h_leading_LRJ_gFexLRJ_Et_resolution->Draw("HIST SAME");
        
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_LRJ_gFexLRJ_Et_resolution.pdf");

        sig_h_leading_offlineLRJ_gFexLRJ_Et_resolution->Scale(1.0 / sig_h_leading_offlineLRJ_gFexLRJ_Et_resolution->Integral());
        back_h_leading_offlineLRJ_gFexLRJ_Et_resolution->Scale(1.0 / back_h_leading_offlineLRJ_gFexLRJ_Et_resolution->Integral());
        sig_h_leading_offlineLRJ_gFexLRJ_Et_resolution->SetLineColor(kRed);
        back_h_leading_offlineLRJ_gFexLRJ_Et_resolution->SetLineColor(kBlue);
        sig_h_leading_offlineLRJ_gFexLRJ_Et_resolution->Draw("HIST");
        back_h_leading_offlineLRJ_gFexLRJ_Et_resolution->Draw("HIST SAME");
        
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_offlineLRJ_gFexLRJ_Et_resolution.pdf");

        sig_h_leading_offlineLRJ_jFexLRJ_Et_resolution->Scale(1.0 / sig_h_leading_offlineLRJ_jFexLRJ_Et_resolution->Integral());
        back_h_leading_offlineLRJ_jFexLRJ_Et_resolution->Scale(1.0 / back_h_leading_offlineLRJ_jFexLRJ_Et_resolution->Integral());
        sig_h_leading_offlineLRJ_jFexLRJ_Et_resolution->SetLineColor(kRed);
        back_h_leading_offlineLRJ_jFexLRJ_Et_resolution->SetLineColor(kBlue);
        sig_h_leading_offlineLRJ_jFexLRJ_Et_resolution->Draw("HIST");
        back_h_leading_offlineLRJ_jFexLRJ_Et_resolution->Draw("HIST SAME");
        
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_offlineLRJ_jFexLRJ_Et_resolution.pdf");

        sig_h_leading_LRJ_offlineLRJ_Et_resolution->Scale(1.0 / sig_h_leading_LRJ_offlineLRJ_Et_resolution->Integral());
        back_h_leading_LRJ_offlineLRJ_Et_resolution->Scale(1.0 / back_h_leading_LRJ_offlineLRJ_Et_resolution->Integral());
        sig_h_leading_LRJ_offlineLRJ_Et_resolution->SetLineColor(kRed);
        back_h_leading_LRJ_offlineLRJ_Et_resolution->SetLineColor(kBlue);
        sig_h_leading_LRJ_offlineLRJ_Et_resolution->Draw("HIST");
        back_h_leading_LRJ_offlineLRJ_Et_resolution->Draw("HIST SAME");
        
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_LRJ_offlineLRJ_Et_resolution.pdf");



        // === Overlay: gFEX (dashed) vs JetTagger (solid), same colors ===
        TCanvas c_overlay("c_overlay", "DeltaEt Overlay", 800, 650);
        c_overlay.SetTicks(1,1);

        // Give the big titles some breathing room (leave your global sizes as-is)
        c_overlay.SetLeftMargin(0.20);    // was 0.16
        c_overlay.SetBottomMargin(0.20);  // was 0.16
        c_overlay.SetRightMargin(0.08);   // a hair more room for legend
        c_overlay.SetTopMargin(0.06);

        c_overlay.cd();

        // 1) Normalize (guard against zero integrals)
        auto safe_norm = [](TH1* h) {
            if (!h) return;
            const double I = h->Integral();
            if (I > 0) h->Scale(1.0 / I);
        };
        safe_norm(sig_h_leading_offlineLRJ_gFexLRJ_deltaEt);
        safe_norm(back_h_leading_offlineLRJ_gFexLRJ_deltaEt);
        safe_norm(sig_h_leading_LRJ_offlineLRJ_deltaEt);
        safe_norm(back_h_leading_LRJ_offlineLRJ_deltaEt);

        // 2) Styling: colors fixed, styles differ by source
        // gFEX = dashed
        sig_h_leading_offlineLRJ_gFexLRJ_deltaEt->SetLineColor(kRed);
        sig_h_leading_offlineLRJ_gFexLRJ_deltaEt->SetLineStyle(kDashed);
        sig_h_leading_offlineLRJ_gFexLRJ_deltaEt->SetLineWidth(2);

        back_h_leading_offlineLRJ_gFexLRJ_deltaEt->SetLineColor(kBlue);
        back_h_leading_offlineLRJ_gFexLRJ_deltaEt->SetLineStyle(kDashed);
        back_h_leading_offlineLRJ_gFexLRJ_deltaEt->SetLineWidth(2);

        // JetTagger = solid
        sig_h_leading_LRJ_offlineLRJ_deltaEt->SetLineColor(kRed);
        sig_h_leading_LRJ_offlineLRJ_deltaEt->SetLineStyle(kSolid);
        sig_h_leading_LRJ_offlineLRJ_deltaEt->SetLineWidth(2);

        back_h_leading_LRJ_offlineLRJ_deltaEt->SetLineColor(kBlue);
        back_h_leading_LRJ_offlineLRJ_deltaEt->SetLineStyle(kSolid);
        back_h_leading_LRJ_offlineLRJ_deltaEt->SetLineWidth(2);

        // 3) Axis/frame: use the gFEX background as a frame clone
        TH1F* frame = (TH1F*)back_h_leading_offlineLRJ_gFexLRJ_deltaEt->Clone("frame_deltaEt");
        frame->Reset("ICES");
        frame->SetTitle(";#Delta E_{T} (Offline - JetTagger, gFEX) [GeV];% of Leading LRJs / 10 GeV");

        // Compute a common max
        double ymax = 0.0;
        auto updmax = [&](TH1* h){ if(h) ymax = std::max(ymax, h->GetMaximum()); };
        updmax(sig_h_leading_offlineLRJ_gFexLRJ_deltaEt);
        updmax(back_h_leading_offlineLRJ_gFexLRJ_deltaEt);
        updmax(sig_h_leading_LRJ_offlineLRJ_deltaEt);
        updmax(back_h_leading_LRJ_offlineLRJ_deltaEt);
        if (ymax <= 0) ymax = 1.0;

        frame->SetMaximum(1.1 * ymax);
        frame->SetMinimum(0.0);
        frame->GetXaxis()->SetLabelSize(0.045);
        frame->GetXaxis()->SetTitleSize(0.055);
        frame->GetYaxis()->SetLabelSize(0.045);
        frame->GetYaxis()->SetTitleSize(0.055);
        frame->Draw("AXIS");   // draw just axes so "HIST" fills them

        // 4) Draw all four histos
        back_h_leading_offlineLRJ_gFexLRJ_deltaEt->Draw("HIST SAME");
        sig_h_leading_offlineLRJ_gFexLRJ_deltaEt->Draw("HIST SAME");
        back_h_leading_LRJ_offlineLRJ_deltaEt->Draw("HIST SAME");
        sig_h_leading_LRJ_offlineLRJ_deltaEt->Draw("HIST SAME");

        // 5) Legend: gFEX (dashed) vs JetTagger (solid)
        TLegend* leg_overlay = new TLegend(0.56, 0.62, 0.86, 0.87);
        leg_overlay->SetBorderSize(0);
        leg_overlay->SetFillStyle(0);
        leg_overlay->SetTextSize(0.032);
        leg_overlay->AddEntry(sig_h_leading_offlineLRJ_gFexLRJ_deltaEt,  "Signal (gFEX)",      "l");
        leg_overlay->AddEntry(back_h_leading_offlineLRJ_gFexLRJ_deltaEt, "Background (gFEX)",  "l");
        leg_overlay->AddEntry(sig_h_leading_LRJ_offlineLRJ_deltaEt,      "Signal (JetTagger)", "l");
        leg_overlay->AddEntry(back_h_leading_LRJ_offlineLRJ_deltaEt,     "Background (JetTagger)", "l");
        leg_overlay->Draw();

        c_overlay.Modified(); c_overlay.Update();
        c_overlay.SaveAs(modifiedOutputFileDir + "leading_deltaEt_overlay_gFEX_vs_JetTagger.pdf");


        // === Overlay: jFEX (dashed) vs JetTagger (solid), same colors ===
        TCanvas c_overlay_jFEX("c_overlay_jFEX", "DeltaEt Overlay", 800, 650);
        c_overlay_jFEX.SetTicks(1,1);

        // Give the big titles some breathing room (leave your global sizes as-is)
        c_overlay_jFEX.SetLeftMargin(0.20);    // was 0.16
        c_overlay_jFEX.SetBottomMargin(0.20);  // was 0.16
        c_overlay_jFEX.SetRightMargin(0.08);   // a hair more room for legend
        c_overlay_jFEX.SetTopMargin(0.06);

        c_overlay_jFEX.cd();

        // 1) Normalize (guard against zero integrals)
        safe_norm(sig_h_leading_offlineLRJ_jFexLRJ_deltaEt);
        safe_norm(back_h_leading_offlineLRJ_jFexLRJ_deltaEt);
        safe_norm(sig_h_leading_LRJ_offlineLRJ_deltaEt);
        safe_norm(back_h_leading_LRJ_offlineLRJ_deltaEt);

        // 2) Styling: colors fixed, styles differ by source
        // jFEX = dashed
        sig_h_leading_offlineLRJ_jFexLRJ_deltaEt->SetLineColor(kRed);
        sig_h_leading_offlineLRJ_jFexLRJ_deltaEt->SetLineStyle(kDashed);
        sig_h_leading_offlineLRJ_jFexLRJ_deltaEt->SetLineWidth(2);

        back_h_leading_offlineLRJ_jFexLRJ_deltaEt->SetLineColor(kBlue);
        back_h_leading_offlineLRJ_jFexLRJ_deltaEt->SetLineStyle(kDashed);
        back_h_leading_offlineLRJ_jFexLRJ_deltaEt->SetLineWidth(2);

        // JetTagger = solid
        sig_h_leading_LRJ_offlineLRJ_deltaEt->SetLineColor(kRed);
        sig_h_leading_LRJ_offlineLRJ_deltaEt->SetLineStyle(kSolid);
        sig_h_leading_LRJ_offlineLRJ_deltaEt->SetLineWidth(2);

        back_h_leading_LRJ_offlineLRJ_deltaEt->SetLineColor(kBlue);
        back_h_leading_LRJ_offlineLRJ_deltaEt->SetLineStyle(kSolid);
        back_h_leading_LRJ_offlineLRJ_deltaEt->SetLineWidth(2);

        // 3) Axis/frame: use the jFEX background as a frame clone
        TH1F* frame_jFEX = (TH1F*)back_h_leading_offlineLRJ_jFexLRJ_deltaEt->Clone("frame_deltaEt");
        frame_jFEX->Reset("ICES");
        frame_jFEX->SetTitle(";#Delta E_{T} (Offline - JetTagger, jFEX) [GeV];% of Leading LRJs / 10 GeV");

        // Compute a common max
        double ymax_jFEX = 0.0;
        auto updmax_jFEX = [&](TH1* h){ if(h) ymax_jFEX = std::max(ymax_jFEX, h->GetMaximum()); };
        updmax_jFEX(sig_h_leading_offlineLRJ_jFexLRJ_deltaEt);
        updmax_jFEX(back_h_leading_offlineLRJ_jFexLRJ_deltaEt);
        updmax_jFEX(sig_h_leading_LRJ_offlineLRJ_deltaEt);
        updmax_jFEX(back_h_leading_LRJ_offlineLRJ_deltaEt);
        if (ymax_jFEX <= 0) ymax_jFEX = 1.0;

        frame_jFEX->SetMaximum(1.1 * ymax_jFEX);
        frame_jFEX->SetMinimum(0.0);
        frame_jFEX->GetXaxis()->SetLabelSize(0.045);
        frame_jFEX->GetXaxis()->SetTitleSize(0.055);
        frame_jFEX->GetYaxis()->SetLabelSize(0.045);
        frame_jFEX->GetYaxis()->SetTitleSize(0.055);
        frame_jFEX->Draw("AXIS");   // draw just axes so "HIST" fills them

        // 4) Draw all four histos
        back_h_leading_offlineLRJ_jFexLRJ_deltaEt->Draw("HIST SAME");
        sig_h_leading_offlineLRJ_jFexLRJ_deltaEt->Draw("HIST SAME");
        back_h_leading_LRJ_offlineLRJ_deltaEt->Draw("HIST SAME");
        sig_h_leading_LRJ_offlineLRJ_deltaEt->Draw("HIST SAME");

        // 5) Legend: jFEX (dashed) vs JetTagger (solid)
        TLegend* leg_overlay_jFEX = new TLegend(0.56, 0.62, 0.86, 0.87);
        leg_overlay_jFEX->SetBorderSize(0);
        leg_overlay_jFEX->SetFillStyle(0);
        leg_overlay_jFEX->SetTextSize(0.032);
        leg_overlay_jFEX->AddEntry(sig_h_leading_offlineLRJ_jFexLRJ_deltaEt,  "Signal (jFEX)",      "l");
        leg_overlay_jFEX->AddEntry(back_h_leading_offlineLRJ_jFexLRJ_deltaEt, "Background (jFEX)",  "l");
        leg_overlay_jFEX->AddEntry(sig_h_leading_LRJ_offlineLRJ_deltaEt,      "Signal (JetTagger)", "l");
        leg_overlay_jFEX->AddEntry(back_h_leading_LRJ_offlineLRJ_deltaEt,     "Background (JetTagger)", "l");
        leg_overlay_jFEX->Draw();

        c_overlay_jFEX.Modified(); c_overlay_jFEX.Update();
        c_overlay_jFEX.SaveAs(modifiedOutputFileDir + "leading_deltaEt_overlay_jFEX_vs_JetTagger.pdf");




        c.cd();

        sig_h_leading_LRJ_gFexLRJ_deltaR->Scale(1.0 / sig_h_leading_LRJ_gFexLRJ_deltaR->Integral());
        back_h_leading_LRJ_gFexLRJ_deltaR->Scale(1.0 / back_h_leading_LRJ_gFexLRJ_deltaR->Integral());
        sig_h_leading_LRJ_gFexLRJ_deltaR->SetLineColor(kRed);
        back_h_leading_LRJ_gFexLRJ_deltaR->SetLineColor(kBlue);
        sig_h_leading_LRJ_gFexLRJ_deltaR->Draw("HIST");
        back_h_leading_LRJ_gFexLRJ_deltaR->Draw("HIST SAME");
        
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_LRJ_gFexLRJ_deltaR.pdf");

        sig_h_leading_LRJ_offlineLRJ_deltaR->Scale(1.0 / sig_h_leading_LRJ_offlineLRJ_deltaR->Integral());
        back_h_leading_LRJ_offlineLRJ_deltaR->Scale(1.0 / back_h_leading_LRJ_offlineLRJ_deltaR->Integral());
        sig_h_leading_LRJ_offlineLRJ_deltaR->SetLineColor(kRed);
        back_h_leading_LRJ_offlineLRJ_deltaR->SetLineColor(kBlue);
        sig_h_leading_LRJ_offlineLRJ_deltaR->Draw("HIST");
        back_h_leading_LRJ_offlineLRJ_deltaR->Draw("HIST SAME");
        
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_LRJ_offlineLRJ_deltaR.pdf");

        sig_h_first_LRJ_jFexSRJ_deltaR->Scale(1.0 / sig_h_first_LRJ_jFexSRJ_deltaR->Integral());
        back_h_first_LRJ_jFexSRJ_deltaR->Scale(1.0 / back_h_first_LRJ_jFexSRJ_deltaR->Integral());
        sig_h_first_LRJ_jFexSRJ_deltaR->SetLineColor(kRed);
        back_h_first_LRJ_jFexSRJ_deltaR->SetLineColor(kBlue);
        sig_h_first_LRJ_jFexSRJ_deltaR->Draw("HIST");
        back_h_first_LRJ_jFexSRJ_deltaR->Draw("HIST SAME");
        
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "first_LRJ_jFexSRJ_deltaR.pdf");

        sig_h_second_LRJ_jFexSRJ_deltaR->Scale(1.0 / sig_h_second_LRJ_jFexSRJ_deltaR->Integral());
        back_h_second_LRJ_jFexSRJ_deltaR->Scale(1.0 / back_h_second_LRJ_jFexSRJ_deltaR->Integral());
        sig_h_second_LRJ_jFexSRJ_deltaR->SetLineColor(kRed);
        back_h_second_LRJ_jFexSRJ_deltaR->SetLineColor(kBlue);
        sig_h_second_LRJ_jFexSRJ_deltaR->Draw("HIST");
        back_h_second_LRJ_jFexSRJ_deltaR->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "second_LRJ_jFexSRJ_deltaR.pdf");

        sig_h_lead_sublead_LRJ_deltaR->Scale(1.0 / sig_h_lead_sublead_LRJ_deltaR->Integral());
        back_h_lead_sublead_LRJ_deltaR->Scale(1.0 / back_h_lead_sublead_LRJ_deltaR->Integral());
        sig_h_lead_sublead_LRJ_deltaR->SetLineColor(kRed);
        back_h_lead_sublead_LRJ_deltaR->SetLineColor(kBlue);
        back_h_lead_sublead_LRJ_deltaR->Draw("HIST");
        sig_h_lead_sublead_LRJ_deltaR->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "lead_sublead_LRJ_deltaR.pdf");

        TString overlayedJZSliceDir = "overlayJZSliceHistograms_" + algorithmConfigurations[fileIt] + "/";

        gSystem->mkdir(overlayedJZSliceDir); 

        back_h_leading_LRJ_Et_arr[0]->Draw("HIST");
        c.SaveAs(overlayedJZSliceDir + "JZ0_jettagger_leading_LRJ_Et.pdf");

        back_h_leading_LRJ_Et_arr[4]->Draw("HIST");
        c.SaveAs(overlayedJZSliceDir + "JZ4_jettagger_leading_LRJ_Et.pdf");

        back_h_leading_LRJ_Et_arr[9]->Draw("HIST");
        c.SaveAs(overlayedJZSliceDir + "JZ9_jettagger_leading_LRJ_Et.pdf");

        OverlayAndSave(back_h_leading_LRJ_Et_arr,  nJZSlices_, "c_leadLRJ", overlayedJZSliceDir + "overlay_leading_LRJ_Et.pdf");
        OverlayAndSave(back_h_subleading_LRJ_Et_arr, nJZSlices_, "c_subleadLRJ", overlayedJZSliceDir + "overlay_subleading_LRJ_Et.pdf");

        OverlayAndSave(back_h_gFEX_leading_LRJ_Et_arr, nJZSlices_, "c_gFEX_leadLRJ", overlayedJZSliceDir + "overlay_gFEX_leading_LRJ_Et.pdf");
        OverlayAndSave(back_h_gFEX_subleading_LRJ_Et_arr, nJZSlices_, "c_gFEX_subLRJ", overlayedJZSliceDir + "overlay_gFEX_subleading_LRJ_Et.pdf");

        OverlayAndSave(back_h_jFEX_leading_LRJ_Et_arr, nJZSlices_, "c_jFEX_leadLRJ",   overlayedJZSliceDir + "overlay_jFEX_leading_LRJ_Et.pdf");
        OverlayAndSave(back_h_jFEX_subleading_LRJ_Et_arr, nJZSlices_, "c_jFEX_subLRJ", overlayedJZSliceDir + "overlay_jFEX_subleading_LRJ_Et.pdf");

        OverlayAndSave(back_h_leading_offlineLRJ_Et_arr, nJZSlices_, "c_off_leadLRJ", overlayedJZSliceDir + "overlay_offline_leading_LRJ_Et.pdf");
        OverlayAndSave(back_h_subleading_offlineLRJ_Et_arr,nJZSlices_, "c_off_subLRJ", overlayedJZSliceDir + "overlay_offline_subleading_LRJ_Et.pdf");

        OverlayAndSave(back_h_leading_truthSRJs_Et_arr, nJZSlices_, "c_truth_leadSRJ", overlayedJZSliceDir + "overlay_truth_leading_SRJs_Et.pdf");
        OverlayAndSave(back_h_subleading_truthSRJs_Et_arr, nJZSlices_, "c_truth_subSRJ", overlayedJZSliceDir + "overlay_truth_subleading_SRJs_Et.pdf");

        TCanvas cLog;
        cLog.SetLogy();
        cLog.cd();
        back_h_leading_LRJ_Et->Draw("HIST");
        cLog.SaveAs(modifiedOutputFileDir + "back_leading_LRJ_Et_events.pdf");

        back_h_gFEX_leading_LRJ_Et->Draw("HIST");
        cLog.SaveAs(modifiedOutputFileDir + "back_h_gFEX_leading_LRJ_Et_events.pdf");

        back_h_jFEX_leading_LRJ_Et->Draw("HIST");
        cLog.SaveAs(modifiedOutputFileDir + "back_h_jFEX_leading_LRJ_Et_events.pdf");

        back_h_leading_offlineLRJ_Et->Draw("HIST");
        cLog.SaveAs(modifiedOutputFileDir + "back_leading_offline_LRJ_Et_events.pdf");


        std::cout << "Leading signal mean: " << sig_h_leading_LRJ_Et->GetMean() << " and leading background mean: " << back_h_leading_LRJ_Et->GetMean() << "\n";
        sig_h_leading_LRJ_Et->Scale(1.0 / sig_h_leading_LRJ_Et->Integral());
        back_h_leading_LRJ_Et->Scale(1.0 / back_h_leading_LRJ_Et->Integral());
        sig_h_leading_LRJ_Et->SetLineColor(kRed);
        back_h_leading_LRJ_Et->SetLineColor(kBlue);
        back_h_leading_LRJ_Et->Draw("HIST");
        sig_h_leading_LRJ_Et->Draw("HIST SAME");
        
        leg->Draw();
        cLog.SaveAs(modifiedOutputFileDir + "leading_LRJ_Et.pdf");


        

        sig_h_gFEX_leading_LRJ_Et->Scale(1.0 / sig_h_gFEX_leading_LRJ_Et->Integral());
        back_h_gFEX_leading_LRJ_Et->Scale(1.0 / back_h_gFEX_leading_LRJ_Et->Integral());
        sig_h_gFEX_leading_LRJ_Et->SetLineColor(kRed);
        back_h_gFEX_leading_LRJ_Et->SetLineColor(kBlue);
        back_h_gFEX_leading_LRJ_Et->Draw("HIST");
        sig_h_gFEX_leading_LRJ_Et->Draw("HIST SAME");
        
        leg->Draw();
        cLog.SaveAs(modifiedOutputFileDir + "leading_gFEX_LRJ_Et.pdf");

        sig_h_jFEX_leading_LRJ_Et->Scale(1.0 / sig_h_jFEX_leading_LRJ_Et->Integral());
        back_h_jFEX_leading_LRJ_Et->Scale(1.0 / back_h_jFEX_leading_LRJ_Et->Integral());
        sig_h_jFEX_leading_LRJ_Et->SetLineColor(kRed);
        back_h_jFEX_leading_LRJ_Et->SetLineColor(kBlue);
        back_h_jFEX_leading_LRJ_Et->Draw("HIST");
        sig_h_jFEX_leading_LRJ_Et->Draw("HIST SAME");
        
        leg->Draw();
        cLog.SaveAs(modifiedOutputFileDir + "leading_jFEX_LRJ_Et.pdf");

        std::cout << "Subleading signal mean: " << sig_h_subleading_LRJ_Et->GetMean() << " and subleading background mean: " << back_h_subleading_LRJ_Et->GetMean() << "\n";
        sig_h_subleading_LRJ_Et->Scale(1.0 / sig_h_subleading_LRJ_Et->Integral());
        back_h_subleading_LRJ_Et->Scale(1.0 / back_h_subleading_LRJ_Et->Integral());
        sig_h_subleading_LRJ_Et->SetLineColor(kRed);
        back_h_subleading_LRJ_Et->SetLineColor(kBlue);
        back_h_subleading_LRJ_Et->Draw("HIST");
        sig_h_subleading_LRJ_Et->Draw("HIST SAME");
        
        leg->Draw();
        cLog.SaveAs(modifiedOutputFileDir + "subleading_LRJ_Et.pdf");

        sig_h_gFEX_subleading_LRJ_Et->Scale(1.0 / sig_h_gFEX_subleading_LRJ_Et->Integral());
        back_h_gFEX_subleading_LRJ_Et->Scale(1.0 / back_h_gFEX_subleading_LRJ_Et->Integral());
        sig_h_gFEX_subleading_LRJ_Et->SetLineColor(kRed);
        back_h_gFEX_subleading_LRJ_Et->SetLineColor(kBlue);
        back_h_gFEX_subleading_LRJ_Et->Draw("HIST");
        sig_h_gFEX_subleading_LRJ_Et->Draw("HIST SAME");
        
        leg->Draw();
        cLog.SaveAs(modifiedOutputFileDir + "subleading_gFEX_LRJ_Et.pdf");

        sig_h_jFEX_subleading_LRJ_Et->Scale(1.0 / sig_h_jFEX_subleading_LRJ_Et->Integral());
        back_h_jFEX_subleading_LRJ_Et->Scale(1.0 / back_h_jFEX_subleading_LRJ_Et->Integral());
        sig_h_jFEX_subleading_LRJ_Et->SetLineColor(kRed);
        back_h_jFEX_subleading_LRJ_Et->SetLineColor(kBlue);
        back_h_jFEX_subleading_LRJ_Et->Draw("HIST");
        sig_h_jFEX_subleading_LRJ_Et->Draw("HIST SAME");
        
        leg->Draw();
        cLog.SaveAs(modifiedOutputFileDir + "subleading_jFEX_LRJ_Et.pdf");


        sig_h_LRJ_E->Scale(1.0 / sig_h_LRJ_E->Integral());
        back_h_LRJ_E->Scale(1.0 / back_h_LRJ_E->Integral());
        sig_h_LRJ_E->SetLineColor(kRed);
        back_h_LRJ_E->SetLineColor(kBlue);
        sig_h_LRJ_E->Draw("HIST");
        back_h_LRJ_E->Draw("HIST SAME");
        
        leg->Draw();
        cLog.SaveAs(modifiedOutputFileDir + "LRJ_Energy.pdf");

        sig_h_leading_offlineLRJ_Et->Scale(1.0 / sig_h_leading_offlineLRJ_Et->Integral());
        back_h_leading_offlineLRJ_Et->Scale(1.0 / back_h_leading_offlineLRJ_Et->Integral());
        sig_h_leading_offlineLRJ_Et->SetLineColor(kRed);
        back_h_leading_offlineLRJ_Et->SetLineColor(kBlue);
        
        back_h_leading_offlineLRJ_Et->Draw("HIST");
        sig_h_leading_offlineLRJ_Et->Draw("HIST SAME");
        
        leg->Draw();

        cLog.SaveAs(modifiedOutputFileDir + "leading_offline_LRJ_Et.pdf");

        sig_h_subleading_offlineLRJ_Et->Scale(1.0 / sig_h_subleading_offlineLRJ_Et->Integral());
        back_h_subleading_offlineLRJ_Et->Scale(1.0 / back_h_subleading_offlineLRJ_Et->Integral());
        sig_h_subleading_offlineLRJ_Et->SetLineColor(kRed);
        back_h_subleading_offlineLRJ_Et->SetLineColor(kBlue);
        
        back_h_subleading_offlineLRJ_Et->Draw("HIST");
        sig_h_subleading_offlineLRJ_Et->Draw("HIST SAME");
        
        leg->Draw();

        cLog.SaveAs(modifiedOutputFileDir + "subleading_offline_LRJ_Et.pdf");

        c.cd();

        sigDiamvsEt->Draw("COLZ");
        c.SaveAs(modifiedOutputFileDir + "EtvsDiamSignal.pdf");

        backDiamvsEt->Draw("COLZ");
        c.SaveAs(modifiedOutputFileDir + "EtvsDiamBack.pdf");


        

        sigOfflineLeadingLRJEtvsPsi_R->Draw("COLZ");
        sigOfflineLeadingLRJEtvsPsi_R->Scale(1.0 / sigOfflineLeadingLRJEtvsPsi_R->Integral());
        c.SaveAs(modifiedOutputFileDir + "sigOfflineLeadingLRJEtvsPsi_R.pdf");

        sigOfflineSubleadingLRJEtvsPsi_R->Draw("COLZ");
        sigOfflineSubleadingLRJEtvsPsi_R->Scale(1.0 / sigOfflineSubleadingLRJEtvsPsi_R->Integral());
        c.SaveAs(modifiedOutputFileDir + "sigOfflineSubleadingLRJEtvsPsi_R.pdf");

        backOfflineLeadingLRJEtvsPsi_R->Draw("COLZ");
        backOfflineLeadingLRJEtvsPsi_R->Scale(1.0 / backOfflineLeadingLRJEtvsPsi_R->Integral());
        c.SaveAs(modifiedOutputFileDir + "backOfflineLeadingLRJEtvsPsi_R.pdf");

        backOfflineSubleadingLRJEtvsPsi_R->Draw("COLZ");
        backOfflineSubleadingLRJEtvsPsi_R->Scale(1.0 / backOfflineSubleadingLRJEtvsPsi_R->Integral());
        c.SaveAs(modifiedOutputFileDir + "backOfflineSubleadingLRJEtvsPsi_R.pdf");

        sigOfflineLeadingLRJEtvsPsi_R_squared->Draw("COLZ");
        sigOfflineLeadingLRJEtvsPsi_R_squared->Scale(1.0 / sigOfflineLeadingLRJEtvsPsi_R_squared->Integral());
        c.SaveAs(modifiedOutputFileDir + "sigOfflineLeadingLRJEtvsPsi_R_squared.pdf");

        sigOfflineSubleadingLRJEtvsPsi_R_squared->Draw("COLZ");
        sigOfflineSubleadingLRJEtvsPsi_R_squared->Scale(1.0 / sigOfflineSubleadingLRJEtvsPsi_R_squared->Integral());
        c.SaveAs(modifiedOutputFileDir + "sigOfflineSubleadingLRJEtvsPsi_R_squared.pdf");

        TCanvas cLogZ;
        cLogZ.SetLogz();

        cLogZ.cd();

        backOfflineLeadingLRJEtvsPsi_R_squared->Draw("COLZ");
        //backOfflineLeadingLRJEtvsPsi_R_squared->Scale(1.0 / backOfflineLeadingLRJEtvsPsi_R_squared->Integral());
        cLogZ.SaveAs(modifiedOutputFileDir + "backOfflineLeadingLRJEtvsPsi_R_squared.pdf");

        backOfflineSubleadingLRJEtvsPsi_R_squared->Draw("COLZ");
        //backOfflineSubleadingLRJEtvsPsi_R_squared->Scale(1.0 / backOfflineSubleadingLRJEtvsPsi_R_squared->Integral());
        cLogZ.SaveAs(modifiedOutputFileDir + "backOfflineSubleadingLRJEtvsPsi_R_squared.pdf");

        c.cd();
        
        sigOfflineLeadingLRJPsi_RvsSubleadingPsi_R->Draw("COLZ");
        sigOfflineLeadingLRJPsi_RvsSubleadingPsi_R->Scale(1.0 / sigOfflineLeadingLRJPsi_RvsSubleadingPsi_R->Integral());
        c.SaveAs(modifiedOutputFileDir + "sigOfflineLeadingLRJPsi_RvsSubleadingPsi_R.pdf");

        cLogZ.cd();

        backOfflineLeadingLRJEtvsPsi_12->Draw("COLZ");
        //backOfflineLeadingLRJEtvsPsi_12->Scale(1.0 / backOfflineLeadingLRJEtvsPsi_12->Integral());
        cLogZ.SaveAs(modifiedOutputFileDir + "backOfflineLeadingLRJEtvsPsi_12.pdf");

        backOfflineSubleadingLRJEtvsPsi_12->Draw("COLZ");
        //backOfflineSubleadingLRJEtvsPsi_12->Scale(1.0 / backOfflineSubleadingLRJEtvsPsi_12->Integral());
        cLogZ.SaveAs(modifiedOutputFileDir + "backOfflineSubleadingLRJEtvsPsi_12.pdf");

        backOfflineLeadingLRJPsi_RvsSubleadingPsi_R->Draw("COLZ");
        //backOfflineLeadingLRJPsi_RvsSubleadingPsi_R->Scale(1.0 / backOfflineLeadingLRJPsi_RvsSubleadingPsi_R->Integral());
        cLogZ.SaveAs(modifiedOutputFileDir + "backOfflineLeadingLRJPsi_RvsSubleadingPsi_R.pdf");

        backOfflineLeadingLRJEtvsPsi_R->Draw("COLZ");
        //backOfflineLeadingLRJEtvsPsi_R->Scale(1.0 / backOfflineLeadingLRJEtvsPsi_R->Integral());
        cLogZ.SaveAs(modifiedOutputFileDir + "backOfflineLeadingLRJEtvsPsi_R.pdf");

        backOfflineSubleadingLRJEtvsPsi_R->Draw("COLZ");
        //backOfflineSubleadingLRJEtvsPsi_R->Scale(1.0 / backOfflineSubleadingLRJEtvsPsi_R->Integral());
        cLogZ.SaveAs(modifiedOutputFileDir + "backOfflineSubleadingLRJEtvsPsi_R.pdf");

        c.cd();

        // Computed with deltaR^2 metric
        sigOfflineLeadingLRJEtvsPsi_R2->Draw("COLZ");
        sigOfflineLeadingLRJEtvsPsi_R2->Scale(1.0 / sigOfflineLeadingLRJEtvsPsi_R2->Integral());
        c.SaveAs(modifiedOutputFileDir + "sigOfflineLeadingLRJEtvsPsi_R2.pdf");

        sigOfflineSubleadingLRJEtvsPsi_R2->Draw("COLZ");
        sigOfflineSubleadingLRJEtvsPsi_R2->Scale(1.0 / sigOfflineSubleadingLRJEtvsPsi_R2->Integral());
        c.SaveAs(modifiedOutputFileDir + "sigOfflineSubleadingLRJEtvsPsi_R2.pdf");

        sigOfflineLeadingLRJEtvsPsi_R2_squared->Draw("COLZ");
        sigOfflineLeadingLRJEtvsPsi_R2_squared->Scale(1.0 / sigOfflineLeadingLRJEtvsPsi_R2_squared->Integral());
        c.SaveAs(modifiedOutputFileDir + "sigOfflineLeadingLRJEtvsPsi_R2_squared.pdf");
        
        cLogZ.cd();
        sigOfflineLeadingLRJEtvsPsi_12->Draw("COLZ");
        sigOfflineLeadingLRJEtvsPsi_12->Scale(1.0 / sigOfflineLeadingLRJEtvsPsi_12->Integral());
        cLogZ.SaveAs(modifiedOutputFileDir + "sigOfflineLeadingLRJEtvsPsi_12.pdf");
        
        sigOfflineSubleadingLRJEtvsPsi_12->Draw("COLZ");
        sigOfflineSubleadingLRJEtvsPsi_12->Scale(1.0 / sigOfflineSubleadingLRJEtvsPsi_12->Integral());
        cLogZ.SaveAs(modifiedOutputFileDir + "sigOfflineSubleadingLRJEtvsPsi_12.pdf");

        c.cd();
        sigOfflineSubleadingLRJEtvsPsi_R2_squared->Draw("COLZ");
        sigOfflineSubleadingLRJEtvsPsi_R2_squared->Scale(1.0 / sigOfflineSubleadingLRJEtvsPsi_R2_squared->Integral());
        c.SaveAs(modifiedOutputFileDir + "sigOfflineSubleadingLRJEtvsPsi_R2_squared.pdf");

        backOfflineLeadingLRJEtvsPsi_R2_squared->Draw("COLZ");
        backOfflineLeadingLRJEtvsPsi_R2_squared->Scale(1.0 / backOfflineLeadingLRJEtvsPsi_R2_squared->Integral());
        c.SaveAs(modifiedOutputFileDir + "backOfflineLeadingLRJEtvsPsi_R2_squared.pdf");

        backOfflineSubleadingLRJEtvsPsi_R2_squared->Draw("COLZ");
        backOfflineSubleadingLRJEtvsPsi_R2_squared->Scale(1.0 / backOfflineSubleadingLRJEtvsPsi_R2_squared->Integral());
        c.SaveAs(modifiedOutputFileDir + "backOfflineSubleadingLRJEtvsPsi_R2_squared.pdf");

        sigOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2->Scale(1.0 / sigOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2->Integral());
        sigOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2->Draw("COLZ");
        c.SaveAs(modifiedOutputFileDir + "sigOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2.pdf");

        backOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2->Draw("COLZ");
        backOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2->Scale(1.0 / backOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2->Integral());
        c.SaveAs(modifiedOutputFileDir + "backOfflineLeadingLRJPsi_R2vsSubleadingPsi_R2.pdf");

        backOfflineLeadingLRJEtvsPsi_R2->Draw("COLZ");
        backOfflineLeadingLRJEtvsPsi_R2->Scale(1.0 / backOfflineLeadingLRJEtvsPsi_R2->Integral());
        c.SaveAs(modifiedOutputFileDir + "backOfflineLeadingLRJEtvsPsi_R2.pdf");

        backOfflineSubleadingLRJEtvsPsi_R2->Draw("COLZ");
        backOfflineSubleadingLRJEtvsPsi_R2->Scale(1.0 / backOfflineSubleadingLRJEtvsPsi_R2->Integral());
        c.SaveAs(modifiedOutputFileDir + "backOfflineSubleadingLRJEtvsPsi_R2.pdf");

        
        cLogZ.cd();
        sigOfflineLeadingLRJMassvsEt->Scale(1.0 / sigOfflineLeadingLRJMassvsEt->Integral());
        sigOfflineLeadingLRJMassvsEt->Draw("COLZ");
        cLogZ.SaveAs(modifiedOutputFileDir + "sigOfflineLeadingLRJMassvsEt.pdf");


        backOfflineLeadingLRJMassvsEt->Draw("COLZ");
        backOfflineLeadingLRJMassvsEt->Scale(1.0 / backOfflineLeadingLRJMassvsEt->Integral());
        cLogZ.SaveAs(modifiedOutputFileDir + "backOfflineLeadingLRJMassvsEt.pdf");

        c.cd();
        sigOfflineLeadingLRJMassvsSubjetMult->Draw("COLZ");
        c.SaveAs(modifiedOutputFileDir + "sigOfflineLeadingLRJMassvsSubjetMult.pdf");

        backOfflineLeadingLRJMassvsSubjetMult->Draw("COLZ");
        c.SaveAs(modifiedOutputFileDir + "backOfflineLeadingLRJMassvsSubjetMult.pdf");

        sigOfflineLeadingLRJEtvsSubjetMult->Draw("COLZ");
        c.SaveAs(modifiedOutputFileDir + "sigOfflineLeadingLRJEtvsSubjetMult.pdf");

        backOfflineLeadingLRJEtvsSubjetMult->Draw("COLZ");
        c.SaveAs(modifiedOutputFileDir + "backOfflineLeadingLRJEtvsSubjetMult.pdf");

        sigOfflineLeadingLRJMass->Scale(1.0 / sigOfflineLeadingLRJMass->Integral());
        backOfflineLeadingLRJMass->Scale(1.0 / backOfflineLeadingLRJMass->Integral());
        sigOfflineLeadingLRJMass->SetLineColor(kRed);
        backOfflineLeadingLRJMass->SetLineColor(kBlue);
        backOfflineLeadingLRJMass->Draw("HIST");
        sigOfflineLeadingLRJMass->Draw("HIST SAME");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leadingOfflineLRJMass.pdf");


        sig_h_LRJ_substruct->Scale(1.0 / sig_h_LRJ_substruct->Integral());
        back_h_LRJ_substruct->Scale(1.0 / back_h_LRJ_substruct->Integral());
        sig_h_LRJ_substruct->SetLineColor(kRed);
        back_h_LRJ_substruct->SetLineColor(kBlue);
        
        
        sig_h_LRJ_substruct->Draw("HIST");
        back_h_LRJ_substruct->Draw("HIST SAME");
        leg_diam->Draw();
        c.SaveAs(modifiedOutputFileDir + "LRJ_diam.pdf");

        sig_h_LRJ_eta->SetLineColor(kRed);
        back_h_LRJ_eta->SetLineColor(kBlue);
        sig_h_LRJ_eta->Draw();
        back_h_LRJ_eta->Draw("SAME");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "LRJ_eta.pdf");

        sig_h_LRJ_phi->SetLineColor(kRed);
        back_h_LRJ_phi->SetLineColor(kBlue);
        sig_h_LRJ_phi->Draw();
        back_h_LRJ_phi->Draw("SAME");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "LRJ_phi.pdf");

        // Build overlaid efficiency histograms (background first)
        auto be100 = (TH1F*)h_back_num100->Clone(Form("back_eff_offlineLRJ_Et_100_%d", fileIt));
        be100->SetDirectory(nullptr);
        be100->SetTitle("Emulated Trigger Efficiency (Background);Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)");
        be100->Divide(h_back_num100, h_back_denom100 , 1.0, 1.0);//, "B"); // binomial errors
        be100->SetAxisRange(0.0, 1.1, "Y");
        back_eff_100.push_back(be100);

        auto be200 = (TH1F*)h_back_num200->Clone(Form("back_eff_offlineLRJ_Et_200_%d", fileIt));
        be200->SetDirectory(nullptr);
        be200->SetTitle("Emulated Trigger Efficiency (Background);Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)");
        be200->Divide(h_back_num200, h_back_denom200, 1.0, 1.0);//, "B");
        be200->SetAxisRange(0.0, 1.1, "Y");
        back_eff_200.push_back(be200);

        auto be300 = (TH1F*)h_back_num300->Clone(Form("back_eff_offlineLRJ_Et_300_%d", fileIt));
        be300->SetDirectory(nullptr);
        be300->SetTitle("Emulated Trigger Efficiency (Background);Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)");
        be300->Divide(h_back_num300, h_back_denom300, 1.0, 1.0);//, "B");
        be300->SetAxisRange(0.0, 1.1, "Y");
        back_eff_300.push_back(be300);

        // Signal
        auto se100 = (TH1F*)h_sig_num100->Clone(Form("sig_eff_offlineLRJ_Et_100_%d", fileIt));
        se100->SetDirectory(nullptr);
        se100->SetTitle("Emulated Trigger Efficiency (Signal);Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)");
        se100->Divide(h_sig_num100, h_sig_denom100, 1.0, 1.0);//, "B");
        se100->SetAxisRange(0.0, 1.1, "Y");
        sig_eff_100.push_back(se100);

        auto se250 = (TH1F*)h_sig_num250->Clone(Form("sig_eff_offlineLRJ_Et_250_%d", fileIt));
        se250->SetDirectory(nullptr);
        se250->SetTitle("Emulated Trigger Efficiency (Signal);Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)");
        se250->Divide(h_sig_num250, h_sig_denom250, 1.0, 1.0);//, "B");
        se250->SetAxisRange(0.0, 1.1, "Y");
        sig_eff_250.push_back(se250);

        auto se400 = (TH1F*)h_sig_num400->Clone(Form("sig_eff_offlineLRJ_Et_400_%d", fileIt));
        se400->SetDirectory(nullptr);
        se400->SetTitle("Emulated Trigger Efficiency (Signal);Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)");
        se400->Divide(h_sig_num400, h_sig_denom400, 1.0, 1.0);//, "B");
        se400->SetAxisRange(0.0, 1.1, "Y");
        sig_eff_400.push_back(se400);


        // Draw signal JetTagger efficiency curves by dividing numerator, denominator TH1Fs

        TH1F* sig_eff_offlineLRJ50 = (TH1F*)sig_h_offlineLRJ_Et_num50->Clone();
        sig_eff_offlineLRJ50->SetName("eff_LRJ50");
        sig_eff_offlineLRJ50->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ50->Divide(sig_h_offlineLRJ_Et_num50, sig_h_offlineLRJ_Et_denom50, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ50->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ50->Draw();
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ50.pdf");

        TH1F* sig_eff_offlineLRJ50_mass100to150 = (TH1F*)sig_h_offlineLRJ_Et_num50_mass100to150->Clone();
        sig_eff_offlineLRJ50_mass100to150->SetName("eff_LRJ50_mass100to150");
        sig_eff_offlineLRJ50_mass100to150->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ50_mass100to150->Divide(sig_h_offlineLRJ_Et_num50_mass100to150, sig_h_offlineLRJ_Et_denom50_mass100to150, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ50_mass100to150->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ50_mass100to150->Draw();
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ50_mass100to150.pdf");

        TH1F* sig_eff_b_Et50 = (TH1F*)sig_h_Avg_b_Et_num50->Clone();
        sig_eff_b_Et50->SetName("eff_b50");
        sig_eff_b_Et50->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_b_Et50->Divide(sig_h_Avg_b_Et_num50, sig_h_Avg_b_Et_denom50, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et50->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et50->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_b_Et50.pdf");

        TH1F* sig_eff_offlineLRJ100 = (TH1F*)sig_h_offlineLRJ_Et_num100->Clone();
        sig_eff_offlineLRJ100->SetName("eff_LRJ100");
        sig_eff_offlineLRJ100->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ100->Divide(sig_h_offlineLRJ_Et_num100, sig_h_offlineLRJ_Et_denom100, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ100->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ100->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ100.pdf");

        TH1F* sig_eff_offlineLRJ100_mass100to150 = (TH1F*)sig_h_offlineLRJ_Et_num100_mass100to150->Clone();
        sig_eff_offlineLRJ100_mass100to150->SetName("eff_LRJ100_mass100to150");
        sig_eff_offlineLRJ100_mass100to150->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ100_mass100to150->Divide(sig_h_offlineLRJ_Et_num100_mass100to150, sig_h_offlineLRJ_Et_denom100_mass100to150, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ100_mass100to150->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ100_mass100to150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ100_mass100to150.pdf");

        TH1F* sig_eff_b_Et100 = (TH1F*)sig_h_Avg_b_Et_num100->Clone();
        sig_eff_b_Et100->SetName("eff_b100");
        sig_eff_b_Et100->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_b_Et100->Divide(sig_h_Avg_b_Et_num100, sig_h_Avg_b_Et_denom100, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et100->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et100->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_b_Et100.pdf");

        TH1F* sig_eff_offlineLRJ150 = (TH1F*)sig_h_offlineLRJ_Et_num150->Clone();
        sig_eff_offlineLRJ150->SetName("eff_LRJ150");
        sig_eff_offlineLRJ150->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ150->Divide(sig_h_offlineLRJ_Et_num150, sig_h_offlineLRJ_Et_denom150, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ150->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ150.pdf");

        TH1F* sig_eff_offlineLRJ150_mass100to150 = (TH1F*)sig_h_offlineLRJ_Et_num150_mass100to150->Clone();
        sig_eff_offlineLRJ150_mass100to150->SetName("eff_LRJ150_mass100to150");
        sig_eff_offlineLRJ150_mass100to150->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ150_mass100to150->Divide(sig_h_offlineLRJ_Et_num150_mass100to150, sig_h_offlineLRJ_Et_denom150_mass100to150, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ150_mass100to150->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ150_mass100to150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ150_mass100to150.pdf");

        TH1F* sig_eff_b_Et150 = (TH1F*)sig_h_Avg_b_Et_num150->Clone();
        sig_eff_b_Et150->SetName("eff_b150");
        sig_eff_b_Et150->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_b_Et150->Divide(sig_h_Avg_b_Et_num150, sig_h_Avg_b_Et_denom150, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et150->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_b_Et150.pdf");

        TH1F* sig_eff_offlineLRJ200 = (TH1F*)sig_h_offlineLRJ_Et_num200->Clone();
        sig_eff_offlineLRJ200->SetName("eff_LRJ200");
        sig_eff_offlineLRJ200->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ200->Divide(sig_h_offlineLRJ_Et_num200, sig_h_offlineLRJ_Et_denom200, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ200->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ200->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ200.pdf");

        TH1F* sig_eff_offlineLRJ200_mass100to150 = (TH1F*)sig_h_offlineLRJ_Et_num200_mass100to150->Clone();
        sig_eff_offlineLRJ200_mass100to150->SetName("eff_LRJ200_mass100to150");
        sig_eff_offlineLRJ200_mass100to150->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ200_mass100to150->Divide(sig_h_offlineLRJ_Et_num200_mass100to150, sig_h_offlineLRJ_Et_denom200_mass100to150, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ200_mass100to150->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ200_mass100to150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ200_mass100to150.pdf");

        TH1F* sig_eff_b_Et200 = (TH1F*)sig_h_Avg_b_Et_num200->Clone();
        sig_eff_b_Et200->SetName("eff_b200");
        sig_eff_b_Et200->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_b_Et200->Divide(sig_h_Avg_b_Et_num200, sig_h_Avg_b_Et_denom200, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et200->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et200->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_b_Et200.pdf");

        TH1F* sig_eff_offlineLRJ250 = (TH1F*)sig_h_offlineLRJ_Et_num250->Clone();
        sig_eff_offlineLRJ250->SetName("eff_LRJ250");
        sig_eff_offlineLRJ250->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ250->Divide(sig_h_offlineLRJ_Et_num250, sig_h_offlineLRJ_Et_denom250, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ250->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ250->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ250.pdf");

        TH1F* sig_eff_offlineLRJ250_mass100to150 = (TH1F*)sig_h_offlineLRJ_Et_num250_mass100to150->Clone();
        sig_eff_offlineLRJ250_mass100to150->SetName("eff_LRJ250_mass100to150");
        sig_eff_offlineLRJ250_mass100to150->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ250_mass100to150->Divide(sig_h_offlineLRJ_Et_num250_mass100to150, sig_h_offlineLRJ_Et_denom250_mass100to150, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ250_mass100to150->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ250_mass100to150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ250_mass100to150.pdf");

        TH1F* sig_eff_b_Et250 = (TH1F*)sig_h_Avg_b_Et_num250->Clone();
        sig_eff_b_Et250->SetName("eff_b250");
        sig_eff_b_Et250->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_b_Et250->Divide(sig_h_Avg_b_Et_num250, sig_h_Avg_b_Et_denom250, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et250->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et250->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_b_Et250.pdf");

        TH1F* sig_eff_offlineLRJ300 = (TH1F*)sig_h_offlineLRJ_Et_num300->Clone();
        sig_eff_offlineLRJ300->SetName("eff_LRJ300");
        sig_eff_offlineLRJ300->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ300->Divide(sig_h_offlineLRJ_Et_num300, sig_h_offlineLRJ_Et_denom300, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ300->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ300->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ300.pdf");

        TH1F* sig_eff_offlineLRJ300_mass100to150 = (TH1F*)sig_h_offlineLRJ_Et_num300_mass100to150->Clone();
        sig_eff_offlineLRJ300_mass100to150->SetName("eff_LRJ300_mass100to150");
        sig_eff_offlineLRJ300_mass100to150->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ300_mass100to150->Divide(sig_h_offlineLRJ_Et_num300_mass100to150, sig_h_offlineLRJ_Et_denom300_mass100to150, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ300_mass100to150->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ300_mass100to150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ300_mass100to150.pdf");

        TH1F* sig_eff_b_Et300 = (TH1F*)sig_h_Avg_b_Et_num300->Clone();
        sig_eff_b_Et300->SetName("eff_b300");
        sig_eff_b_Et300->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_b_Et300->Divide(sig_h_Avg_b_Et_num300, sig_h_Avg_b_Et_denom300, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et300->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et300->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_b_Et300.pdf");

        TH1F* sig_eff_offlineLRJ350 = (TH1F*)sig_h_offlineLRJ_Et_num350->Clone();
        sig_eff_offlineLRJ350->SetName("eff_LRJ350");
        sig_eff_offlineLRJ350->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ350->Divide(sig_h_offlineLRJ_Et_num350, sig_h_offlineLRJ_Et_denom350, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ350->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ350->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ350.pdf");

        TH1F* sig_eff_offlineLRJ350_mass100to150 = (TH1F*)sig_h_offlineLRJ_Et_num350_mass100to150->Clone();
        sig_eff_offlineLRJ350_mass100to150->SetName("eff_LRJ350_mass100to150");
        sig_eff_offlineLRJ350_mass100to150->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ350_mass100to150->Divide(sig_h_offlineLRJ_Et_num350_mass100to150, sig_h_offlineLRJ_Et_denom350_mass100to150, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ350_mass100to150->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ350_mass100to150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ350_mass100to150.pdf");

        TH1F* sig_eff_b_Et350 = (TH1F*)sig_h_Avg_b_Et_num350->Clone();
        sig_eff_b_Et350->SetName("eff_b350");
        sig_eff_b_Et350->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_b_Et350->Divide(sig_h_Avg_b_Et_num350, sig_h_Avg_b_Et_denom350, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et350->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et350->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_b_Et350.pdf");

        TH1F* sig_eff_offlineLRJ400 = (TH1F*)sig_h_offlineLRJ_Et_num400->Clone();
        sig_eff_offlineLRJ400->SetName("eff_LRJ400");
        sig_eff_offlineLRJ400->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ400->Divide(sig_h_offlineLRJ_Et_num400, sig_h_offlineLRJ_Et_denom400, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ400->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ400->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ400.pdf");

        TH1F* sig_eff_offlineLRJ400_mass100to150 = (TH1F*)sig_h_offlineLRJ_Et_num400_mass100to150->Clone();
        sig_eff_offlineLRJ400_mass100to150->SetName("eff_LRJ400_mass100to150");
        sig_eff_offlineLRJ400_mass100to150->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ400_mass100to150->Divide(sig_h_offlineLRJ_Et_num400_mass100to150, sig_h_offlineLRJ_Et_denom400_mass100to150, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ400_mass100to150->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ400_mass100to150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ400_mass100to150.pdf");

        TH1F* sig_eff_b_Et400 = (TH1F*)sig_h_Avg_b_Et_num400->Clone();
        sig_eff_b_Et400->SetName("eff_b400");
        sig_eff_b_Et400->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_b_Et400->Divide(sig_h_Avg_b_Et_num400, sig_h_Avg_b_Et_denom400, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et400->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et400->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_b_Et400.pdf");

        TH1F* sig_eff_offlineLRJ450 = (TH1F*)sig_h_offlineLRJ_Et_num450->Clone();
        sig_eff_offlineLRJ450->SetName("eff_LRJ450");
        sig_eff_offlineLRJ450->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ450->Divide(sig_h_offlineLRJ_Et_num450, sig_h_offlineLRJ_Et_denom450, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ450->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ450->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ450.pdf");

        TH1F* sig_eff_offlineLRJ450_mass100to150 = (TH1F*)sig_h_offlineLRJ_Et_num450_mass100to150->Clone();
        sig_eff_offlineLRJ450_mass100to150->SetName("eff_LRJ450_mass100to150");
        sig_eff_offlineLRJ450_mass100to150->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ450_mass100to150->Divide(sig_h_offlineLRJ_Et_num450_mass100to150, sig_h_offlineLRJ_Et_denom450_mass100to150, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ450_mass100to150->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ450_mass100to150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ450_mass100to150.pdf");

        TH1F* sig_eff_b_Et450 = (TH1F*)sig_h_Avg_b_Et_num450->Clone();
        sig_eff_b_Et450->SetName("eff_b450");
        sig_eff_b_Et450->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_b_Et450->Divide(sig_h_Avg_b_Et_num450, sig_h_Avg_b_Et_denom450, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et450->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et450->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_b_Et450.pdf");

        TH1F* sig_eff_offlineLRJ500 = (TH1F*)sig_h_offlineLRJ_Et_num500->Clone();
        sig_eff_offlineLRJ500->SetName("eff_LRJ500");
        sig_eff_offlineLRJ500->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ500->Divide(sig_h_offlineLRJ_Et_num500, sig_h_offlineLRJ_Et_denom500, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ500->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ500->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ500.pdf");

        TH1F* sig_eff_offlineLRJ500_mass100to150 = (TH1F*)sig_h_offlineLRJ_Et_num500_mass100to150->Clone();
        sig_eff_offlineLRJ500_mass100to150->SetName("eff_LRJ500_mass100to150");
        sig_eff_offlineLRJ500_mass100to150->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ500_mass100to150->Divide(sig_h_offlineLRJ_Et_num500_mass100to150, sig_h_offlineLRJ_Et_denom500_mass100to150, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ500_mass100to150->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ500_mass100to150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ500_mass100to150.pdf");

        TH1F* sig_eff_b_Et500 = (TH1F*)sig_h_Avg_b_Et_num500->Clone();
        sig_eff_b_Et500->SetName("eff_b500");
        sig_eff_b_Et500->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_b_Et500->Divide(sig_h_Avg_b_Et_num500, sig_h_Avg_b_Et_denom500, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et500->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et500->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_b_Et500.pdf");

        // Draw signal gFEX efficiency curves - first 1 jet

        TH1F* sig_eff_offlineLRJ50_gFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num50_gFexLRJ->Clone();
        sig_eff_offlineLRJ50_gFexLRJ->SetName("eff_LRJ50");
        sig_eff_offlineLRJ50_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ50_gFexLRJ->Divide(sig_h_offlineLRJ_Et_num50_gFexLRJ, sig_h_offlineLRJ_Et_denom50_gFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ50_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ50_gFexLRJ->Draw();
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ50_gFexLRJ.pdf");

        TH1F* sig_eff_offlineLRJ100_gFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num100_gFexLRJ->Clone();
        sig_eff_offlineLRJ100_gFexLRJ->SetName("eff_LRJ100");
        sig_eff_offlineLRJ100_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ100_gFexLRJ->Divide(sig_h_offlineLRJ_Et_num100_gFexLRJ, sig_h_offlineLRJ_Et_denom100_gFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ100_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ100_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ100_gFexLRJ.pdf");

        TH1F* sig_eff_offlineLRJ150_gFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num150_gFexLRJ->Clone();
        sig_eff_offlineLRJ150_gFexLRJ->SetName("eff_LRJ150");
        sig_eff_offlineLRJ150_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ150_gFexLRJ->Divide(sig_h_offlineLRJ_Et_num150_gFexLRJ, sig_h_offlineLRJ_Et_denom150_gFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ150_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ150_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ150_gFexLRJ.pdf");

        TH1F* sig_eff_offlineLRJ200_gFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num200_gFexLRJ->Clone();
        sig_eff_offlineLRJ200_gFexLRJ->SetName("eff_LRJ200_gFexLRJ");
        sig_eff_offlineLRJ200_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ200_gFexLRJ->Divide(sig_h_offlineLRJ_Et_num200_gFexLRJ, sig_h_offlineLRJ_Et_denom200_gFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ200_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ200_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ200_gFexLRJ.pdf");

        TH1F* sig_eff_offlineLRJ250_gFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num250_gFexLRJ->Clone();
        sig_eff_offlineLRJ250_gFexLRJ->SetName("eff_LRJ250_gFexLRJ");
        sig_eff_offlineLRJ250_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ250_gFexLRJ->Divide(sig_h_offlineLRJ_Et_num250_gFexLRJ, sig_h_offlineLRJ_Et_denom250_gFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ250_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ250_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ250_gFexLRJ.pdf");


        TH1F* sig_eff_offlineLRJ300_gFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num300_gFexLRJ->Clone();
        sig_eff_offlineLRJ300_gFexLRJ->SetName("eff_LRJ300");
        sig_eff_offlineLRJ300_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ300_gFexLRJ->Divide(sig_h_offlineLRJ_Et_num300_gFexLRJ, sig_h_offlineLRJ_Et_denom300_gFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ300_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ300_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ300_gFexLRJ.pdf");


        TH1F* sig_eff_offlineLRJ350_gFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num350_gFexLRJ->Clone();
        sig_eff_offlineLRJ350_gFexLRJ->SetName("eff_LRJ350");
        sig_eff_offlineLRJ350_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ350_gFexLRJ->Divide(sig_h_offlineLRJ_Et_num350_gFexLRJ, sig_h_offlineLRJ_Et_denom350_gFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ350_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ350_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ350_gFexLRJ.pdf");


        TH1F* sig_eff_offlineLRJ400_gFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num400_gFexLRJ->Clone();
        sig_eff_offlineLRJ400_gFexLRJ->SetName("eff_LRJ400");
        sig_eff_offlineLRJ400_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ400_gFexLRJ->Divide(sig_h_offlineLRJ_Et_num400_gFexLRJ, sig_h_offlineLRJ_Et_denom400_gFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ400_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ400_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ400_gFexLRJ.pdf");


        TH1F* sig_eff_offlineLRJ450_gFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num450_gFexLRJ->Clone();
        sig_eff_offlineLRJ450_gFexLRJ->SetName("eff_LRJ450");
        sig_eff_offlineLRJ450_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ450_gFexLRJ->Divide(sig_h_offlineLRJ_Et_num450_gFexLRJ, sig_h_offlineLRJ_Et_denom450_gFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ450_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ450_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ450_gFexLRJ.pdf");


        TH1F* sig_eff_offlineLRJ500_gFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num500_gFexLRJ->Clone();
        sig_eff_offlineLRJ500_gFexLRJ->SetName("eff_LRJ500");
        sig_eff_offlineLRJ500_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ500_gFexLRJ->Divide(sig_h_offlineLRJ_Et_num500_gFexLRJ, sig_h_offlineLRJ_Et_denom500_gFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ500_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ500_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ500_gFexLRJ.pdf");

        // Next jFEX (1 jet) signal

        TH1F* sig_eff_offlineLRJ50_jFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num50_jFexLRJ->Clone();
        sig_eff_offlineLRJ50_jFexLRJ->SetName("eff_LRJ50");
        sig_eff_offlineLRJ50_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ50_jFexLRJ->Divide(sig_h_offlineLRJ_Et_num50_jFexLRJ, sig_h_offlineLRJ_Et_denom50_jFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ50_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ50_jFexLRJ->Draw();
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ50_jFexLRJ.pdf");

        TH1F* sig_eff_offlineLRJ100_jFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num100_jFexLRJ->Clone();
        sig_eff_offlineLRJ100_jFexLRJ->SetName("eff_LRJ100");
        sig_eff_offlineLRJ100_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ100_jFexLRJ->Divide(sig_h_offlineLRJ_Et_num100_jFexLRJ, sig_h_offlineLRJ_Et_denom100_jFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ100_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ100_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ100_jFexLRJ.pdf");

        TH1F* sig_eff_offlineLRJ150_jFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num150_jFexLRJ->Clone();
        sig_eff_offlineLRJ150_jFexLRJ->SetName("eff_LRJ150");
        sig_eff_offlineLRJ150_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ150_jFexLRJ->Divide(sig_h_offlineLRJ_Et_num150_jFexLRJ, sig_h_offlineLRJ_Et_denom150_jFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ150_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ150_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ150_jFexLRJ.pdf");

        TH1F* sig_eff_offlineLRJ200_jFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num200_jFexLRJ->Clone();
        sig_eff_offlineLRJ200_jFexLRJ->SetName("eff_LRJ200_jFexLRJ");
        sig_eff_offlineLRJ200_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ200_jFexLRJ->Divide(sig_h_offlineLRJ_Et_num200_jFexLRJ, sig_h_offlineLRJ_Et_denom200_jFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ200_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ200_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ200_jFexLRJ.pdf");

        TH1F* sig_eff_offlineLRJ250_jFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num250_jFexLRJ->Clone();
        sig_eff_offlineLRJ250_jFexLRJ->SetName("eff_LRJ250_jFexLRJ");
        sig_eff_offlineLRJ250_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ250_jFexLRJ->Divide(sig_h_offlineLRJ_Et_num250_jFexLRJ, sig_h_offlineLRJ_Et_denom250_jFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ250_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ250_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ250_jFexLRJ.pdf");


        TH1F* sig_eff_offlineLRJ300_jFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num300_jFexLRJ->Clone();
        sig_eff_offlineLRJ300_jFexLRJ->SetName("eff_LRJ300");
        sig_eff_offlineLRJ300_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ300_jFexLRJ->Divide(sig_h_offlineLRJ_Et_num300_jFexLRJ, sig_h_offlineLRJ_Et_denom300_jFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ300_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ300_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ300_jFexLRJ.pdf");


        TH1F* sig_eff_offlineLRJ350_jFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num350_jFexLRJ->Clone();
        sig_eff_offlineLRJ350_jFexLRJ->SetName("eff_LRJ350");
        sig_eff_offlineLRJ350_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ350_jFexLRJ->Divide(sig_h_offlineLRJ_Et_num350_jFexLRJ, sig_h_offlineLRJ_Et_denom350_jFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ350_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ350_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ350_jFexLRJ.pdf");


        TH1F* sig_eff_offlineLRJ400_jFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num400_jFexLRJ->Clone();
        sig_eff_offlineLRJ400_jFexLRJ->SetName("eff_LRJ400");
        sig_eff_offlineLRJ400_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ400_jFexLRJ->Divide(sig_h_offlineLRJ_Et_num400_jFexLRJ, sig_h_offlineLRJ_Et_denom400_jFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ400_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ400_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ400_jFexLRJ.pdf");


        TH1F* sig_eff_offlineLRJ450_jFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num450_jFexLRJ->Clone();
        sig_eff_offlineLRJ450_jFexLRJ->SetName("eff_LRJ450");
        sig_eff_offlineLRJ450_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ450_jFexLRJ->Divide(sig_h_offlineLRJ_Et_num450_jFexLRJ, sig_h_offlineLRJ_Et_denom450_jFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ450_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ450_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ450_jFexLRJ.pdf");


        TH1F* sig_eff_offlineLRJ500_jFexLRJ = (TH1F*)sig_h_offlineLRJ_Et_num500_jFexLRJ->Clone();
        sig_eff_offlineLRJ500_jFexLRJ->SetName("eff_LRJ500");
        sig_eff_offlineLRJ500_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ500_jFexLRJ->Divide(sig_h_offlineLRJ_Et_num500_jFexLRJ, sig_h_offlineLRJ_Et_denom500_jFexLRJ, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ500_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ500_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ500_jFexLRJ.pdf");

        // Next dijet

        TH1F* sig_eff_offlineLRJ50_gFexLRJ_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num50_gFexLRJ_Dijet->Clone();
        sig_eff_offlineLRJ50_gFexLRJ_Dijet->SetName("eff_LRJ50");
        sig_eff_offlineLRJ50_gFexLRJ_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ50_gFexLRJ_Dijet->Divide(sig_h_offlineLRJ_Et_num50_gFexLRJ_Dijet, sig_h_offlineLRJ_Et_denom50_gFexLRJ_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ50_gFexLRJ_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ50_gFexLRJ_Dijet->Draw();
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ50_gFexLRJ_Dijet.pdf");

        TH1F* sig_eff_offlineLRJ100_gFexLRJ_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num100_gFexLRJ_Dijet->Clone();
        sig_eff_offlineLRJ100_gFexLRJ_Dijet->SetName("eff_LRJ100");
        sig_eff_offlineLRJ100_gFexLRJ_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ100_gFexLRJ_Dijet->Divide(sig_h_offlineLRJ_Et_num100_gFexLRJ_Dijet, sig_h_offlineLRJ_Et_denom100_gFexLRJ_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ100_gFexLRJ_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ100_gFexLRJ_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ100_gFexLRJ_Dijet.pdf");

        TH1F* sig_eff_offlineLRJ150_gFexLRJ_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num150_gFexLRJ_Dijet->Clone();
        sig_eff_offlineLRJ150_gFexLRJ_Dijet->SetName("eff_LRJ150");
        sig_eff_offlineLRJ150_gFexLRJ_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ150_gFexLRJ_Dijet->Divide(sig_h_offlineLRJ_Et_num150_gFexLRJ_Dijet, sig_h_offlineLRJ_Et_denom150_gFexLRJ_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ150_gFexLRJ_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ150_gFexLRJ_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ150_gFexLRJ_Dijet.pdf");

        TH1F* sig_eff_offlineLRJ200_gFexLRJ_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num200_gFexLRJ_Dijet->Clone();
        sig_eff_offlineLRJ200_gFexLRJ_Dijet->SetName("eff_LRJ200_gFexLRJ_Dijet");
        sig_eff_offlineLRJ200_gFexLRJ_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ200_gFexLRJ_Dijet->Divide(sig_h_offlineLRJ_Et_num200_gFexLRJ_Dijet, sig_h_offlineLRJ_Et_denom200_gFexLRJ_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ200_gFexLRJ_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ200_gFexLRJ_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ200_gFexLRJ_Dijet.pdf");

        TH1F* sig_eff_offlineLRJ250_gFexLRJ_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num250_gFexLRJ_Dijet->Clone();
        sig_eff_offlineLRJ250_gFexLRJ_Dijet->SetName("eff_LRJ250_gFexLRJ_Dijet");
        sig_eff_offlineLRJ250_gFexLRJ_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ250_gFexLRJ_Dijet->Divide(sig_h_offlineLRJ_Et_num250_gFexLRJ_Dijet, sig_h_offlineLRJ_Et_denom250_gFexLRJ_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ250_gFexLRJ_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ250_gFexLRJ_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ250_gFexLRJ_Dijet.pdf");


        TH1F* sig_eff_offlineLRJ300_gFexLRJ_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num300_gFexLRJ_Dijet->Clone();
        sig_eff_offlineLRJ300_gFexLRJ_Dijet->SetName("eff_LRJ300");
        sig_eff_offlineLRJ300_gFexLRJ_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ300_gFexLRJ_Dijet->Divide(sig_h_offlineLRJ_Et_num300_gFexLRJ_Dijet, sig_h_offlineLRJ_Et_denom300_gFexLRJ_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ300_gFexLRJ_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ300_gFexLRJ_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ300_gFexLRJ_Dijet.pdf");


        TH1F* sig_eff_offlineLRJ350_gFexLRJ_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num350_gFexLRJ_Dijet->Clone();
        sig_eff_offlineLRJ350_gFexLRJ_Dijet->SetName("eff_LRJ350");
        sig_eff_offlineLRJ350_gFexLRJ_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ350_gFexLRJ_Dijet->Divide(sig_h_offlineLRJ_Et_num350_gFexLRJ_Dijet, sig_h_offlineLRJ_Et_denom350_gFexLRJ_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ350_gFexLRJ_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ350_gFexLRJ_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ350_gFexLRJ_Dijet.pdf");


        TH1F* sig_eff_offlineLRJ400_gFexLRJ_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num400_gFexLRJ_Dijet->Clone();
        sig_eff_offlineLRJ400_gFexLRJ_Dijet->SetName("eff_LRJ400");
        sig_eff_offlineLRJ400_gFexLRJ_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ400_gFexLRJ_Dijet->Divide(sig_h_offlineLRJ_Et_num400_gFexLRJ_Dijet, sig_h_offlineLRJ_Et_denom400_gFexLRJ_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ400_gFexLRJ_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ400_gFexLRJ_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ400_gFexLRJ_Dijet.pdf");


        TH1F* sig_eff_offlineLRJ450_gFexLRJ_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num450_gFexLRJ_Dijet->Clone();
        sig_eff_offlineLRJ450_gFexLRJ_Dijet->SetName("eff_LRJ450");
        sig_eff_offlineLRJ450_gFexLRJ_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ450_gFexLRJ_Dijet->Divide(sig_h_offlineLRJ_Et_num450_gFexLRJ_Dijet, sig_h_offlineLRJ_Et_denom450_gFexLRJ_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ450_gFexLRJ_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ450_gFexLRJ_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ450_gFexLRJ_Dijet.pdf");


        TH1F* sig_eff_offlineLRJ500_gFexLRJ_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num500_gFexLRJ_Dijet->Clone();
        sig_eff_offlineLRJ500_gFexLRJ_Dijet->SetName("eff_LRJ500");
        sig_eff_offlineLRJ500_gFexLRJ_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ500_gFexLRJ_Dijet->Divide(sig_h_offlineLRJ_Et_num500_gFexLRJ_Dijet, sig_h_offlineLRJ_Et_denom500_gFexLRJ_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ500_gFexLRJ_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ500_gFexLRJ_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ500_gFexLRJ_Dijet.pdf");



        // Draw background gFex trigger efficiencies (and create histograms with actual efficiency curves from numerators, denominators)
        TH1F* back_eff_offlineLRJ50_gFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num50_gFexLRJ->Clone();
        back_eff_offlineLRJ50_gFexLRJ->SetName("eff_LRJ50");
        back_eff_offlineLRJ50_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ50_gFexLRJ->Divide(back_h_offlineLRJ_Et_num50_gFexLRJ, back_h_offlineLRJ_Et_denom50_gFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ50_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ50_gFexLRJ->Draw();
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ50_gFexLRJ.pdf");

        TH1F* back_eff_offlineLRJ100_gFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num100_gFexLRJ->Clone();
        back_eff_offlineLRJ100_gFexLRJ->SetName("eff_LRJ100");
        back_eff_offlineLRJ100_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ100_gFexLRJ->Divide(back_h_offlineLRJ_Et_num100_gFexLRJ, back_h_offlineLRJ_Et_denom100_gFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ100_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ100_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ100_gFexLRJ.pdf");

        TH1F* back_eff_offlineLRJ150_gFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num150_gFexLRJ->Clone();
        back_eff_offlineLRJ150_gFexLRJ->SetName("eff_LRJ150");
        back_eff_offlineLRJ150_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ150_gFexLRJ->Divide(back_h_offlineLRJ_Et_num150_gFexLRJ, back_h_offlineLRJ_Et_denom150_gFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ150_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ150_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ150_gFexLRJ.pdf");

        TH1F* back_eff_offlineLRJ200_gFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num200_gFexLRJ->Clone();
        back_eff_offlineLRJ200_gFexLRJ->SetName("eff_LRJ200_gFexLRJ");
        back_eff_offlineLRJ200_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ200_gFexLRJ->Divide(back_h_offlineLRJ_Et_num200_gFexLRJ, back_h_offlineLRJ_Et_denom200_gFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ200_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ200_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ200_gFexLRJ.pdf");

        TH1F* back_eff_offlineLRJ250_gFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num250_gFexLRJ->Clone();
        back_eff_offlineLRJ250_gFexLRJ->SetName("eff_LRJ250_gFexLRJ");
        back_eff_offlineLRJ250_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ250_gFexLRJ->Divide(back_h_offlineLRJ_Et_num250_gFexLRJ, back_h_offlineLRJ_Et_denom250_gFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ250_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ250_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ250_gFexLRJ.pdf");


        TH1F* back_eff_offlineLRJ300_gFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num300_gFexLRJ->Clone();
        back_eff_offlineLRJ300_gFexLRJ->SetName("eff_LRJ300");
        back_eff_offlineLRJ300_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ300_gFexLRJ->Divide(back_h_offlineLRJ_Et_num300_gFexLRJ, back_h_offlineLRJ_Et_denom300_gFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ300_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ300_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ300_gFexLRJ.pdf");


        TH1F* back_eff_offlineLRJ350_gFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num350_gFexLRJ->Clone();
        back_eff_offlineLRJ350_gFexLRJ->SetName("eff_LRJ350");
        back_eff_offlineLRJ350_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ350_gFexLRJ->Divide(back_h_offlineLRJ_Et_num350_gFexLRJ, back_h_offlineLRJ_Et_denom350_gFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ350_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ350_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ350_gFexLRJ.pdf");


        TH1F* back_eff_offlineLRJ400_gFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num400_gFexLRJ->Clone();
        back_eff_offlineLRJ400_gFexLRJ->SetName("eff_LRJ400");
        back_eff_offlineLRJ400_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ400_gFexLRJ->Divide(back_h_offlineLRJ_Et_num400_gFexLRJ, back_h_offlineLRJ_Et_denom400_gFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ400_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ400_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ400_gFexLRJ.pdf");


        TH1F* back_eff_offlineLRJ450_gFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num450_gFexLRJ->Clone();
        back_eff_offlineLRJ450_gFexLRJ->SetName("eff_LRJ450");
        back_eff_offlineLRJ450_gFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ450_gFexLRJ->Divide(back_h_offlineLRJ_Et_num450_gFexLRJ, back_h_offlineLRJ_Et_denom450_gFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ450_gFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ450_gFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ450_gFexLRJ.pdf");


        // Draw background jFex trigger efficiencies (and create histograms with actual efficiency curves from numerators, denominators)
        TH1F* back_eff_offlineLRJ50_jFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num50_jFexLRJ->Clone();
        back_eff_offlineLRJ50_jFexLRJ->SetName("eff_LRJ50");
        back_eff_offlineLRJ50_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ50_jFexLRJ->Divide(back_h_offlineLRJ_Et_num50_jFexLRJ, back_h_offlineLRJ_Et_denom50_jFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ50_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ50_jFexLRJ->Draw();
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ50_jFexLRJ.pdf");

        TH1F* back_eff_offlineLRJ100_jFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num100_jFexLRJ->Clone();
        back_eff_offlineLRJ100_jFexLRJ->SetName("eff_LRJ100");
        back_eff_offlineLRJ100_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ100_jFexLRJ->Divide(back_h_offlineLRJ_Et_num100_jFexLRJ, back_h_offlineLRJ_Et_denom100_jFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ100_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ100_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ100_jFexLRJ.pdf");

        TH1F* back_eff_offlineLRJ150_jFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num150_jFexLRJ->Clone();
        back_eff_offlineLRJ150_jFexLRJ->SetName("eff_LRJ150");
        back_eff_offlineLRJ150_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ150_jFexLRJ->Divide(back_h_offlineLRJ_Et_num150_jFexLRJ, back_h_offlineLRJ_Et_denom150_jFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ150_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ150_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ150_jFexLRJ.pdf");

        TH1F* back_eff_offlineLRJ200_jFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num200_jFexLRJ->Clone();
        back_eff_offlineLRJ200_jFexLRJ->SetName("eff_LRJ200_jFexLRJ");
        back_eff_offlineLRJ200_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ200_jFexLRJ->Divide(back_h_offlineLRJ_Et_num200_jFexLRJ, back_h_offlineLRJ_Et_denom200_jFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ200_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ200_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ200_jFexLRJ.pdf");

        TH1F* back_eff_offlineLRJ250_jFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num250_jFexLRJ->Clone();
        back_eff_offlineLRJ250_jFexLRJ->SetName("eff_LRJ250_jFexLRJ");
        back_eff_offlineLRJ250_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ250_jFexLRJ->Divide(back_h_offlineLRJ_Et_num250_jFexLRJ, back_h_offlineLRJ_Et_denom250_jFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ250_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ250_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ250_jFexLRJ.pdf");


        TH1F* back_eff_offlineLRJ300_jFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num300_jFexLRJ->Clone();
        back_eff_offlineLRJ300_jFexLRJ->SetName("eff_LRJ300");
        back_eff_offlineLRJ300_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ300_jFexLRJ->Divide(back_h_offlineLRJ_Et_num300_jFexLRJ, back_h_offlineLRJ_Et_denom300_jFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ300_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ300_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ300_jFexLRJ.pdf");


        TH1F* back_eff_offlineLRJ350_jFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num350_jFexLRJ->Clone();
        back_eff_offlineLRJ350_jFexLRJ->SetName("eff_LRJ350");
        back_eff_offlineLRJ350_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ350_jFexLRJ->Divide(back_h_offlineLRJ_Et_num350_jFexLRJ, back_h_offlineLRJ_Et_denom350_jFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ350_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ350_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ350_jFexLRJ.pdf");


        TH1F* back_eff_offlineLRJ400_jFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num400_jFexLRJ->Clone();
        back_eff_offlineLRJ400_jFexLRJ->SetName("eff_LRJ400");
        back_eff_offlineLRJ400_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ400_jFexLRJ->Divide(back_h_offlineLRJ_Et_num400_jFexLRJ, back_h_offlineLRJ_Et_denom400_jFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ400_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ400_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ400_jFexLRJ.pdf");


        TH1F* back_eff_offlineLRJ450_jFexLRJ = (TH1F*)back_h_offlineLRJ_Et_num450_jFexLRJ->Clone();
        back_eff_offlineLRJ450_jFexLRJ->SetName("eff_LRJ450");
        back_eff_offlineLRJ450_jFexLRJ->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ450_jFexLRJ->Divide(back_h_offlineLRJ_Et_num450_jFexLRJ, back_h_offlineLRJ_Et_denom450_jFexLRJ, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ450_jFexLRJ->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ450_jFexLRJ->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ450_jFexLRJ.pdf");

        // Draw background jet tagger efficiency curves

        TH1F* back_eff_offlineLRJ50 = (TH1F*)back_h_offlineLRJ_Et_num50->Clone();
        back_eff_offlineLRJ50->SetName("eff_LRJ50");
        back_eff_offlineLRJ50->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ50->Divide(back_h_offlineLRJ_Et_num50, back_h_offlineLRJ_Et_denom50, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ50->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ50->Draw();
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ50.pdf");

        TH1F* back_eff_offlineLRJ100 = (TH1F*)back_h_offlineLRJ_Et_num100->Clone();
        back_eff_offlineLRJ100->SetName("eff_LRJ100");
        back_eff_offlineLRJ100->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ100->Divide(back_h_offlineLRJ_Et_num100, back_h_offlineLRJ_Et_denom100, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ100->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ100->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ100.pdf");

        TH1F* back_eff_offlineLRJ150 = (TH1F*)back_h_offlineLRJ_Et_num150->Clone();
        back_eff_offlineLRJ150->SetName("eff_LRJ150");
        back_eff_offlineLRJ150->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ150->Divide(back_h_offlineLRJ_Et_num150, back_h_offlineLRJ_Et_denom150, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ150->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ150.pdf");

        TH1F* back_eff_offlineLRJ200 = (TH1F*)back_h_offlineLRJ_Et_num200->Clone();
        back_eff_offlineLRJ200->SetName("eff_LRJ200");
        back_eff_offlineLRJ200->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ200->Divide(back_h_offlineLRJ_Et_num200, back_h_offlineLRJ_Et_denom200, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ200->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ200->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ200.pdf");

        TH1F* back_eff_offlineLRJ250 = (TH1F*)back_h_offlineLRJ_Et_num250->Clone();
        back_eff_offlineLRJ250->SetName("eff_LRJ250");
        back_eff_offlineLRJ250->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ250->Divide(back_h_offlineLRJ_Et_num250, back_h_offlineLRJ_Et_denom250, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ250->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ250->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ250.pdf");

        TH1F* back_eff_offlineLRJ300 = (TH1F*)back_h_offlineLRJ_Et_num300->Clone();
        back_eff_offlineLRJ300->SetName("eff_LRJ300");
        back_eff_offlineLRJ300->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ300->Divide(back_h_offlineLRJ_Et_num300, back_h_offlineLRJ_Et_denom300, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ300->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ300->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ300.pdf");

        TH1F* back_eff_offlineLRJ350 = (TH1F*)back_h_offlineLRJ_Et_num350->Clone();
        back_eff_offlineLRJ350->SetName("eff_LRJ350");
        back_eff_offlineLRJ350->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ350->Divide(back_h_offlineLRJ_Et_num350, back_h_offlineLRJ_Et_denom350, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ350->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ350->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ350.pdf");

        TH1F* back_eff_offlineLRJ400 = (TH1F*)back_h_offlineLRJ_Et_num400->Clone();
        back_eff_offlineLRJ400->SetName("eff_LRJ400");
        back_eff_offlineLRJ400->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ400->Divide(back_h_offlineLRJ_Et_num400, back_h_offlineLRJ_Et_denom400, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ400->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ400->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ400.pdf");

        TH1F* back_eff_offlineLRJ450 = (TH1F*)back_h_offlineLRJ_Et_num450->Clone();
        back_eff_offlineLRJ450->SetName("eff_LRJ450");
        back_eff_offlineLRJ450->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ450->Divide(back_h_offlineLRJ_Et_num450, back_h_offlineLRJ_Et_denom450, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ450->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ450->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ450.pdf");

        // Dijet trigger efficiencies (no further event selections)

        TH1F* sig_eff_offlineLRJ50_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num50_Dijet->Clone();
        sig_eff_offlineLRJ50_Dijet->SetName("eff_LRJ50_Dijet");
        sig_eff_offlineLRJ50_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ50_Dijet->Divide(sig_h_offlineLRJ_Et_num50_Dijet, sig_h_offlineLRJ_Et_denom50_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ50_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ50_Dijet->Draw();
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ50_Dijet.pdf");

        TH1F* sig_eff_offlineLRJ100_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num100_Dijet->Clone();
        sig_eff_offlineLRJ100_Dijet->SetName("eff_LRJ100_Dijet");
        sig_eff_offlineLRJ100_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ100_Dijet->Divide(sig_h_offlineLRJ_Et_num100_Dijet, sig_h_offlineLRJ_Et_denom100_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ100_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ100_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ100_Dijet.pdf");

        TH1F* sig_eff_offlineLRJ150_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num150_Dijet->Clone();
        sig_eff_offlineLRJ150_Dijet->SetName("eff_LRJ150_Dijet");
        sig_eff_offlineLRJ150_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ150_Dijet->Divide(sig_h_offlineLRJ_Et_num150_Dijet, sig_h_offlineLRJ_Et_denom150_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ150_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ150_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ150_Dijet.pdf");

        TH1F* sig_eff_offlineLRJ200_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num200_Dijet->Clone();
        sig_eff_offlineLRJ200_Dijet->SetName("eff_LRJ200_Dijet");
        sig_eff_offlineLRJ200_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ200_Dijet->Divide(sig_h_offlineLRJ_Et_num200_Dijet, sig_h_offlineLRJ_Et_denom200_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ200_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ200_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ200_Dijet.pdf");

        TH1F* sig_eff_offlineLRJ250_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num250_Dijet->Clone();
        sig_eff_offlineLRJ250_Dijet->SetName("eff_LRJ250_Dijet");
        sig_eff_offlineLRJ250_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ250_Dijet->Divide(sig_h_offlineLRJ_Et_num250_Dijet, sig_h_offlineLRJ_Et_denom250_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ250_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ250_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ250_Dijet.pdf");

        TH1F* sig_eff_offlineLRJ300_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num300_Dijet->Clone();
        sig_eff_offlineLRJ300_Dijet->SetName("eff_LRJ300_Dijet");
        sig_eff_offlineLRJ300_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ300_Dijet->Divide(sig_h_offlineLRJ_Et_num300_Dijet, sig_h_offlineLRJ_Et_denom300_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ300_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ300_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ300_Dijet.pdf");

        TH1F* sig_eff_offlineLRJ350_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num350_Dijet->Clone();
        sig_eff_offlineLRJ350_Dijet->SetName("eff_LRJ350_Dijet");
        sig_eff_offlineLRJ350_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ350_Dijet->Divide(sig_h_offlineLRJ_Et_num350_Dijet, sig_h_offlineLRJ_Et_denom350_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ350_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ350_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ350_Dijet.pdf");

        TH1F* sig_eff_offlineLRJ400_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num400_Dijet->Clone();
        sig_eff_offlineLRJ400_Dijet->SetName("eff_LRJ400_Dijet");
        sig_eff_offlineLRJ400_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ400_Dijet->Divide(sig_h_offlineLRJ_Et_num400_Dijet, sig_h_offlineLRJ_Et_denom400_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ400_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ400_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ400_Dijet.pdf");

        TH1F* sig_eff_offlineLRJ450_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num450_Dijet->Clone();
        sig_eff_offlineLRJ450_Dijet->SetName("eff_LRJ450_Dijet");
        sig_eff_offlineLRJ450_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ450_Dijet->Divide(sig_h_offlineLRJ_Et_num450_Dijet, sig_h_offlineLRJ_Et_denom450_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ450_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ450_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ450_Dijet.pdf");

        TH1F* sig_eff_offlineLRJ500_Dijet = (TH1F*)sig_h_offlineLRJ_Et_num500_Dijet->Clone();
        sig_eff_offlineLRJ500_Dijet->SetName("eff_LRJ500_Dijet");
        sig_eff_offlineLRJ500_Dijet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ500_Dijet->Divide(sig_h_offlineLRJ_Et_num500_Dijet, sig_h_offlineLRJ_Et_denom500_Dijet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ500_Dijet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ500_Dijet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ500_Dijet.pdf");

        // Subjet requirement efficiencies (1 subjet)

        TH1F* sig_eff_offlineLRJ50_1Subjet = (TH1F*)sig_h_offlineLRJ_Et_num50_1Subjet->Clone();
        sig_eff_offlineLRJ50_1Subjet->SetName("eff_LRJ50_1Subjet");
        sig_eff_offlineLRJ50_1Subjet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ50_1Subjet->Divide(sig_h_offlineLRJ_Et_num50_1Subjet, sig_h_offlineLRJ_Et_denom50_1Subjet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ50_1Subjet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ50_1Subjet->Draw();
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ50_1Subjet.pdf");

        TH1F* sig_eff_offlineLRJ100_1Subjet = (TH1F*)sig_h_offlineLRJ_Et_num100_1Subjet->Clone();
        sig_eff_offlineLRJ100_1Subjet->SetName("eff_LRJ100_1Subjet");
        sig_eff_offlineLRJ100_1Subjet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ100_1Subjet->Divide(sig_h_offlineLRJ_Et_num100_1Subjet, sig_h_offlineLRJ_Et_denom100_1Subjet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ100_1Subjet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ100_1Subjet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ100_1Subjet.pdf");

        TH1F* sig_eff_offlineLRJ150_1Subjet = (TH1F*)sig_h_offlineLRJ_Et_num150_1Subjet->Clone();
        sig_eff_offlineLRJ150_1Subjet->SetName("eff_LRJ150_1Subjet");
        sig_eff_offlineLRJ150_1Subjet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ150_1Subjet->Divide(sig_h_offlineLRJ_Et_num150_1Subjet, sig_h_offlineLRJ_Et_denom150_1Subjet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ150_1Subjet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ150_1Subjet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ150_1Subjet.pdf");

        TH1F* sig_eff_offlineLRJ200_1Subjet = (TH1F*)sig_h_offlineLRJ_Et_num200_1Subjet->Clone();
        sig_eff_offlineLRJ200_1Subjet->SetName("eff_LRJ200_1Subjet");
        sig_eff_offlineLRJ200_1Subjet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ200_1Subjet->Divide(sig_h_offlineLRJ_Et_num200_1Subjet, sig_h_offlineLRJ_Et_denom200_1Subjet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ200_1Subjet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ200_1Subjet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ200_1Subjet.pdf");

        TH1F* sig_eff_offlineLRJ250_1Subjet = (TH1F*)sig_h_offlineLRJ_Et_num250_1Subjet->Clone();
        sig_eff_offlineLRJ250_1Subjet->SetName("eff_LRJ250_1Subjet");
        sig_eff_offlineLRJ250_1Subjet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ250_1Subjet->Divide(sig_h_offlineLRJ_Et_num250_1Subjet, sig_h_offlineLRJ_Et_denom250_1Subjet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ250_1Subjet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ250_1Subjet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ250_1Subjet.pdf");

        TH1F* sig_eff_offlineLRJ300_1Subjet = (TH1F*)sig_h_offlineLRJ_Et_num300_1Subjet->Clone();
        sig_eff_offlineLRJ300_1Subjet->SetName("eff_LRJ300_1Subjet");
        sig_eff_offlineLRJ300_1Subjet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ300_1Subjet->Divide(sig_h_offlineLRJ_Et_num300_1Subjet, sig_h_offlineLRJ_Et_denom300_1Subjet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ300_1Subjet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ300_1Subjet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ300_1Subjet.pdf");

        TH1F* sig_eff_offlineLRJ350_1Subjet = (TH1F*)sig_h_offlineLRJ_Et_num350_1Subjet->Clone();
        sig_eff_offlineLRJ350_1Subjet->SetName("eff_LRJ350_1Subjet");
        sig_eff_offlineLRJ350_1Subjet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ350_1Subjet->Divide(sig_h_offlineLRJ_Et_num350_1Subjet, sig_h_offlineLRJ_Et_denom350_1Subjet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ350_1Subjet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ350_1Subjet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ350_1Subjet.pdf");

        TH1F* sig_eff_offlineLRJ400_1Subjet = (TH1F*)sig_h_offlineLRJ_Et_num400_1Subjet->Clone();
        sig_eff_offlineLRJ400_1Subjet->SetName("eff_LRJ400_1Subjet");
        sig_eff_offlineLRJ400_1Subjet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ400_1Subjet->Divide(sig_h_offlineLRJ_Et_num400_1Subjet, sig_h_offlineLRJ_Et_denom400_1Subjet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ400_1Subjet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ400_1Subjet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ400_1Subjet.pdf");

        TH1F* sig_eff_offlineLRJ450_1Subjet = (TH1F*)sig_h_offlineLRJ_Et_num450_1Subjet->Clone();
        sig_eff_offlineLRJ450_1Subjet->SetName("eff_LRJ450_1Subjet");
        sig_eff_offlineLRJ450_1Subjet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ450_1Subjet->Divide(sig_h_offlineLRJ_Et_num450_1Subjet, sig_h_offlineLRJ_Et_denom450_1Subjet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ450_1Subjet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ450_1Subjet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ450_1Subjet.pdf");

        TH1F* sig_eff_offlineLRJ500_1Subjet = (TH1F*)sig_h_offlineLRJ_Et_num500_1Subjet->Clone();
        sig_eff_offlineLRJ500_1Subjet->SetName("eff_LRJ500_1Subjet");
        sig_eff_offlineLRJ500_1Subjet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ500_1Subjet->Divide(sig_h_offlineLRJ_Et_num500_1Subjet, sig_h_offlineLRJ_Et_denom500_1Subjet, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ500_1Subjet->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ500_1Subjet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ500_1Subjet.pdf");

        // Background subjet = 1 efficiencies

                TH1F* back_eff_offlineLRJ100_1Subjet = (TH1F*)back_h_offlineLRJ_Et_num100_1Subjet->Clone();
        back_eff_offlineLRJ100_1Subjet->SetName("eff_LRJ100_1Subjet");
        back_eff_offlineLRJ100_1Subjet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ100_1Subjet->Divide(back_h_offlineLRJ_Et_num100_1Subjet, back_h_offlineLRJ_Et_denom100_1Subjet, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ100_1Subjet->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ100_1Subjet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ100_1Subjet.pdf");

        TH1F* back_eff_offlineLRJ200_1Subjet = (TH1F*)back_h_offlineLRJ_Et_num200_1Subjet->Clone();
        back_eff_offlineLRJ200_1Subjet->SetName("eff_LRJ200_1Subjet");
        back_eff_offlineLRJ200_1Subjet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ200_1Subjet->Divide(back_h_offlineLRJ_Et_num200_1Subjet, back_h_offlineLRJ_Et_denom200_1Subjet, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ200_1Subjet->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ200_1Subjet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ200_1Subjet.pdf");

        TH1F* back_eff_offlineLRJ300_1Subjet = (TH1F*)back_h_offlineLRJ_Et_num300_1Subjet->Clone();
        back_eff_offlineLRJ300_1Subjet->SetName("eff_LRJ300_1Subjet");
        back_eff_offlineLRJ300_1Subjet->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ300_1Subjet->Divide(back_h_offlineLRJ_Et_num300_1Subjet, back_h_offlineLRJ_Et_denom300_1Subjet, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ300_1Subjet->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ300_1Subjet->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ300_1Subjet.pdf");

        // Subjet requirement efficiencies (greater than or equal to 2 subjets)

        TH1F* sig_eff_offlineLRJ50_GrEq2Subjets = (TH1F*)sig_h_offlineLRJ_Et_num50_GrEq2Subjets->Clone();
        sig_eff_offlineLRJ50_GrEq2Subjets->SetName("eff_LRJ50_GrEq2Subjets");
        sig_eff_offlineLRJ50_GrEq2Subjets->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ50_GrEq2Subjets->Divide(sig_h_offlineLRJ_Et_num50_GrEq2Subjets, sig_h_offlineLRJ_Et_denom50_GrEq2Subjets, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ50_GrEq2Subjets->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ50_GrEq2Subjets->Draw();
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ50_GrEq2Subjets.pdf");

        TH1F* sig_eff_offlineLRJ100_GrEq2Subjets = (TH1F*)sig_h_offlineLRJ_Et_num100_GrEq2Subjets->Clone();
        sig_eff_offlineLRJ100_GrEq2Subjets->SetName("eff_LRJ100_GrEq2Subjets");
        sig_eff_offlineLRJ100_GrEq2Subjets->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ100_GrEq2Subjets->Divide(sig_h_offlineLRJ_Et_num100_GrEq2Subjets, sig_h_offlineLRJ_Et_denom100_GrEq2Subjets, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ100_GrEq2Subjets->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ100_GrEq2Subjets->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ100_GrEq2Subjets.pdf");

        TH1F* sig_eff_offlineLRJ150_GrEq2Subjets = (TH1F*)sig_h_offlineLRJ_Et_num150_GrEq2Subjets->Clone();
        sig_eff_offlineLRJ150_GrEq2Subjets->SetName("eff_LRJ150_GrEq2Subjets");
        sig_eff_offlineLRJ150_GrEq2Subjets->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ150_GrEq2Subjets->Divide(sig_h_offlineLRJ_Et_num150_GrEq2Subjets, sig_h_offlineLRJ_Et_denom150_GrEq2Subjets, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ150_GrEq2Subjets->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ150_GrEq2Subjets->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ150_GrEq2Subjets.pdf");

        TH1F* sig_eff_offlineLRJ200_GrEq2Subjets = (TH1F*)sig_h_offlineLRJ_Et_num200_GrEq2Subjets->Clone();
        sig_eff_offlineLRJ200_GrEq2Subjets->SetName("eff_LRJ200_GrEq2Subjets");
        sig_eff_offlineLRJ200_GrEq2Subjets->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ200_GrEq2Subjets->Divide(sig_h_offlineLRJ_Et_num200_GrEq2Subjets, sig_h_offlineLRJ_Et_denom200_GrEq2Subjets, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ200_GrEq2Subjets->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ200_GrEq2Subjets->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ200_GrEq2Subjets.pdf");

        TH1F* sig_eff_offlineLRJ250_GrEq2Subjets = (TH1F*)sig_h_offlineLRJ_Et_num250_GrEq2Subjets->Clone();
        sig_eff_offlineLRJ250_GrEq2Subjets->SetName("eff_LRJ250_GrEq2Subjets");
        sig_eff_offlineLRJ250_GrEq2Subjets->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ250_GrEq2Subjets->Divide(sig_h_offlineLRJ_Et_num250_GrEq2Subjets, sig_h_offlineLRJ_Et_denom250_GrEq2Subjets, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ250_GrEq2Subjets->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ250_GrEq2Subjets->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ250_GrEq2Subjets.pdf");

        TH1F* sig_eff_offlineLRJ300_GrEq2Subjets = (TH1F*)sig_h_offlineLRJ_Et_num300_GrEq2Subjets->Clone();
        sig_eff_offlineLRJ300_GrEq2Subjets->SetName("eff_LRJ300_GrEq2Subjets");
        sig_eff_offlineLRJ300_GrEq2Subjets->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ300_GrEq2Subjets->Divide(sig_h_offlineLRJ_Et_num300_GrEq2Subjets, sig_h_offlineLRJ_Et_denom300_GrEq2Subjets, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ300_GrEq2Subjets->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ300_GrEq2Subjets->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ300_GrEq2Subjets.pdf");

        TH1F* sig_eff_offlineLRJ350_GrEq2Subjets = (TH1F*)sig_h_offlineLRJ_Et_num350_GrEq2Subjets->Clone();
        sig_eff_offlineLRJ350_GrEq2Subjets->SetName("eff_LRJ350_GrEq2Subjets");
        sig_eff_offlineLRJ350_GrEq2Subjets->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ350_GrEq2Subjets->Divide(sig_h_offlineLRJ_Et_num350_GrEq2Subjets, sig_h_offlineLRJ_Et_denom350_GrEq2Subjets, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ350_GrEq2Subjets->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ350_GrEq2Subjets->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ350_GrEq2Subjets.pdf");

        TH1F* sig_eff_offlineLRJ400_GrEq2Subjets = (TH1F*)sig_h_offlineLRJ_Et_num400_GrEq2Subjets->Clone();
        sig_eff_offlineLRJ400_GrEq2Subjets->SetName("eff_LRJ400_GrEq2Subjets");
        sig_eff_offlineLRJ400_GrEq2Subjets->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ400_GrEq2Subjets->Divide(sig_h_offlineLRJ_Et_num400_GrEq2Subjets, sig_h_offlineLRJ_Et_denom400_GrEq2Subjets, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ400_GrEq2Subjets->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ400_GrEq2Subjets->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ400_GrEq2Subjets.pdf");

        TH1F* sig_eff_offlineLRJ450_GrEq2Subjets = (TH1F*)sig_h_offlineLRJ_Et_num450_GrEq2Subjets->Clone();
        sig_eff_offlineLRJ450_GrEq2Subjets->SetName("eff_LRJ450_GrEq2Subjets");
        sig_eff_offlineLRJ450_GrEq2Subjets->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ450_GrEq2Subjets->Divide(sig_h_offlineLRJ_Et_num450_GrEq2Subjets, sig_h_offlineLRJ_Et_denom450_GrEq2Subjets, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ450_GrEq2Subjets->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ450_GrEq2Subjets->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ450_GrEq2Subjets.pdf");

        TH1F* sig_eff_offlineLRJ500_GrEq2Subjets = (TH1F*)sig_h_offlineLRJ_Et_num500_GrEq2Subjets->Clone();
        sig_eff_offlineLRJ500_GrEq2Subjets->SetName("eff_LRJ500_GrEq2Subjets");
        sig_eff_offlineLRJ500_GrEq2Subjets->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Signal)");
        sig_eff_offlineLRJ500_GrEq2Subjets->Divide(sig_h_offlineLRJ_Et_num500_GrEq2Subjets, sig_h_offlineLRJ_Et_denom500_GrEq2Subjets, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ500_GrEq2Subjets->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ500_GrEq2Subjets->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "sig_eff_offlineLRJ500_GrEq2Subjets.pdf");

        // background greater than or equal to 2 Subjet Efficiencies
        TH1F* back_eff_offlineLRJ100_GrEq2Subjets = (TH1F*)back_h_offlineLRJ_Et_num100_GrEq2Subjets->Clone();
        back_eff_offlineLRJ100_GrEq2Subjets->SetName("back_eff_LRJ100_GrEq2Subjets");
        back_eff_offlineLRJ100_GrEq2Subjets->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ100_GrEq2Subjets->Divide(back_h_offlineLRJ_Et_num100_GrEq2Subjets, back_h_offlineLRJ_Et_denom100_GrEq2Subjets, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ100_GrEq2Subjets->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ100_GrEq2Subjets->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ100_GrEq2Subjets.pdf");


        TH1F* back_eff_offlineLRJ200_GrEq2Subjets = (TH1F*)back_h_offlineLRJ_Et_num200_GrEq2Subjets->Clone();
        back_eff_offlineLRJ200_GrEq2Subjets->SetName("back_eff_LRJ200_GrEq2Subjets");
        back_eff_offlineLRJ200_GrEq2Subjets->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ200_GrEq2Subjets->Divide(back_h_offlineLRJ_Et_num200_GrEq2Subjets, back_h_offlineLRJ_Et_denom200_GrEq2Subjets, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ200_GrEq2Subjets->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ200_GrEq2Subjets->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ200_GrEq2Subjets.pdf");


        TH1F* back_eff_offlineLRJ300_GrEq2Subjets = (TH1F*)back_h_offlineLRJ_Et_num300_GrEq2Subjets->Clone();
        back_eff_offlineLRJ300_GrEq2Subjets->SetName("back_eff_LRJ300_GrEq2Subjets");
        back_eff_offlineLRJ300_GrEq2Subjets->GetYaxis()->SetTitle("Emulated Trigger Efficiency (Background)");
        back_eff_offlineLRJ300_GrEq2Subjets->Divide(back_h_offlineLRJ_Et_num300_GrEq2Subjets, back_h_offlineLRJ_Et_denom300_GrEq2Subjets, 1.0, 1.0);//, "B");
        
        back_eff_offlineLRJ300_GrEq2Subjets->SetAxisRange(0, 1.1, "Y");

        back_eff_offlineLRJ300_GrEq2Subjets->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "back_eff_offlineLRJ300_GrEq2Subjets.pdf");






        if(displayEv0Sig_){
            sigTopo422Highest128SeedPositionsEv0->GetZaxis()->SetTitle("E_{T} [GeV]");
            sigTopo422Highest128SeedPositionsEv0->Draw("COLZ");

            TEllipse *sigCircle0_Ev0 = new TEllipse(jFexSeedPositionsSigEv0[0].first, jFexSeedPositionsSigEv0[0].second, 1.0, 1.0); // R in both x and y
            sigCircle0_Ev0->SetLineColor(kRed);
            sigCircle0_Ev0->SetLineWidth(2);
            sigCircle0_Ev0->SetFillStyle(0); // no fill
            sigCircle0_Ev0->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle0_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle1_Ev0 = new TEllipse(jFexSeedPositionsSigEv0[1].first, jFexSeedPositionsSigEv0[1].second, 1.0, 1.0); // R in both x and y
            sigCircle1_Ev0->SetLineColor(kRed);
            sigCircle1_Ev0->SetLineWidth(2);
            sigCircle1_Ev0->SetFillStyle(0); // no fill
            sigCircle1_Ev0->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle1_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle2_Ev0 = new TEllipse(jFexAdditionalSRJPositionsSigEv0[0].first, jFexAdditionalSRJPositionsSigEv0[0].second, 1.0, 1.0); // R in both x and y
            sigCircle2_Ev0->SetLineColor(kAzure+2);
            sigCircle2_Ev0->SetLineWidth(2);
            sigCircle2_Ev0->SetFillStyle(0); // no fill
            sigCircle2_Ev0->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle2_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle3_Ev0 = new TEllipse(jFexAdditionalSRJPositionsSigEv0[1].first, jFexAdditionalSRJPositionsSigEv0[1].second, 1.0, 1.0); // R in both x and y
            sigCircle3_Ev0->SetLineColor(kAzure+2);
            sigCircle3_Ev0->SetLineWidth(2);
            sigCircle3_Ev0->SetFillStyle(0); // no fill
            sigCircle3_Ev0->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle3_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle4_Ev0 = new TEllipse(jFexAdditionalSRJPositionsSigEv0[2].first, jFexAdditionalSRJPositionsSigEv0[2].second, 1.0, 1.0); // R in both x and y
            sigCircle4_Ev0->SetLineColor(kAzure+2);
            sigCircle4_Ev0->SetLineWidth(2);
            sigCircle4_Ev0->SetFillStyle(0); // no fill
            sigCircle4_Ev0->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle4_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle5_Ev0 = new TEllipse(jFexAdditionalSRJPositionsSigEv0[3].first, jFexAdditionalSRJPositionsSigEv0[3].second, 1.0, 1.0); // R in both x and y
            sigCircle5_Ev0->SetLineColor(kAzure+2);
            sigCircle5_Ev0->SetLineWidth(2);
            sigCircle5_Ev0->SetFillStyle(0); // no fill
            sigCircle5_Ev0->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle5_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle6_Ev0 = new TEllipse(newSeedPositionsSigEv0[0].first, newSeedPositionsSigEv0[0].second, 1.0, 1.0); // R in both x and y
            sigCircle6_Ev0->SetLineColor(kGreen+2);
            sigCircle6_Ev0->SetLineWidth(2);
            sigCircle6_Ev0->SetFillStyle(0); // no fill
            sigCircle6_Ev0->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle6_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle7_Ev0 = new TEllipse(newSeedPositionsSigEv0[1].first, newSeedPositionsSigEv0[1].second, 1.0, 1.0); // R in both x and y
            sigCircle7_Ev0->SetLineColor(kGreen+2);
            sigCircle7_Ev0->SetLineWidth(2);
            sigCircle7_Ev0->SetFillStyle(0); // no fill
            sigCircle7_Ev0->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle7_Ev0->Draw("same");    // overlay on the existing plot

            // Build the label text (1st LRJ)
            TString sigLabel1Ev0 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsSigEv0[0].first, newSeedPositionsSigEv0[0].second, sigLRJEtEv0[0]);

            // Option 1: Use TLatex directly
            TLatex sigLat1Ev0;
            sigLat1Ev0.SetTextSize(0.025);     // Adjust size
            sigLat1Ev0.SetTextColor(kBlack); // Match sigCircle color
            sigLat1Ev0.DrawLatex(newSeedPositionsSigEv0[0].first, newSeedPositionsSigEv0[0].second + 0.6, sigLabel1Ev0); // Slightly above the sigCircle

            // Build the label text (2nd LRJ)
            TString sigLabel2Ev0 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsSigEv0[1].first, newSeedPositionsSigEv0[1].second, sigLRJEtEv0[1]);

            // Option 1: Use TLatex directly
            TLatex sigLat2Ev0;
            sigLat2Ev0.SetTextSize(0.025);     // Adjust size
            sigLat2Ev0.SetTextColor(kBlack); // Match sigCircle color
            sigLat2Ev0.DrawLatex(newSeedPositionsSigEv0[1].first, newSeedPositionsSigEv0[1].second + 0.6, sigLabel2Ev0); // Slightly above the sigCircle\

            c.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev0.pdf");   
        }
        
        
        
        
        if(displayEv1Sig_){
            sigTopo422Highest128SeedPositionsEv1->GetZaxis()->SetTitle("E_{T} [GeV]");
            sigTopo422Highest128SeedPositionsEv1->Draw("COLZ");

            TEllipse *sigCircle0_Ev1 = new TEllipse(jFexSeedPositionsSigEv1[0].first, jFexSeedPositionsSigEv1[0].second, 1.0, 1.0); // R in both x and y
            sigCircle0_Ev1->SetLineColor(kRed);
            sigCircle0_Ev1->SetLineWidth(2);
            sigCircle0_Ev1->SetFillStyle(0); // no fill
            sigCircle0_Ev1->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle0_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle1_Ev1 = new TEllipse(jFexSeedPositionsSigEv1[1].first, jFexSeedPositionsSigEv1[1].second, 1.0, 1.0); // R in both x and y
            sigCircle1_Ev1->SetLineColor(kRed);
            sigCircle1_Ev1->SetLineWidth(2);
            sigCircle1_Ev1->SetFillStyle(0); // no fill
            sigCircle1_Ev1->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle1_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle2_Ev1 = new TEllipse(jFexAdditionalSRJPositionsSigEv1[0].first, jFexAdditionalSRJPositionsSigEv1[0].second, 1.0, 1.0); // R in both x and y
            sigCircle2_Ev1->SetLineColor(kAzure+2);
            sigCircle2_Ev1->SetLineWidth(2);
            sigCircle2_Ev1->SetFillStyle(0); // no fill
            sigCircle2_Ev1->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle2_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle3_Ev1 = new TEllipse(jFexAdditionalSRJPositionsSigEv1[1].first, jFexAdditionalSRJPositionsSigEv1[1].second, 1.0, 1.0); // R in both x and y
            sigCircle3_Ev1->SetLineColor(kAzure+2);
            sigCircle3_Ev1->SetLineWidth(2);
            sigCircle3_Ev1->SetFillStyle(0); // no fill
            sigCircle3_Ev1->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle3_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle4_Ev1 = new TEllipse(jFexAdditionalSRJPositionsSigEv1[2].first, jFexAdditionalSRJPositionsSigEv1[2].second, 1.0, 1.0); // R in both x and y
            sigCircle4_Ev1->SetLineColor(kAzure+2);
            sigCircle4_Ev1->SetLineWidth(2);
            sigCircle4_Ev1->SetFillStyle(0); // no fill
            sigCircle4_Ev1->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle4_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle5_Ev1 = new TEllipse(jFexAdditionalSRJPositionsSigEv1[3].first, jFexAdditionalSRJPositionsSigEv1[3].second, 1.0, 1.0); // R in both x and y
            sigCircle5_Ev1->SetLineColor(kAzure+2);
            sigCircle5_Ev1->SetLineWidth(2);
            sigCircle5_Ev1->SetFillStyle(0); // no fill
            sigCircle5_Ev1->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle5_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle6_Ev1 = new TEllipse(newSeedPositionsSigEv1[0].first, newSeedPositionsSigEv1[0].second, 1.0, 1.0); // R in both x and y
            sigCircle6_Ev1->SetLineColor(kGreen+2);
            sigCircle6_Ev1->SetLineWidth(2);
            sigCircle6_Ev1->SetFillStyle(0); // no fill
            sigCircle6_Ev1->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle6_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle7_Ev1 = new TEllipse(newSeedPositionsSigEv1[1].first, newSeedPositionsSigEv1[1].second, 1.0, 1.0); // R in both x and y
            sigCircle7_Ev1->SetLineColor(kGreen+2);
            sigCircle7_Ev1->SetLineWidth(2);
            sigCircle7_Ev1->SetFillStyle(0); // no fill
            sigCircle7_Ev1->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle7_Ev1->Draw("same");    // overlay on the existing plot

            // Build the label text (1st LRJ)
            TString sigLabel1Ev1 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsSigEv1[0].first, newSeedPositionsSigEv1[0].second, sigLRJEtEv1[0]);

            // Option 1: Use TLatex directly
            TLatex sigLat1Ev1;
            sigLat1Ev1.SetTextSize(0.025);     // Adjust size
            sigLat1Ev1.SetTextColor(kBlack); // Match sigCircle color
            sigLat1Ev1.DrawLatex(newSeedPositionsSigEv1[0].first, newSeedPositionsSigEv1[0].second + 0.6, sigLabel1Ev1); // Slightly above the sigCircle

            // Build the label text (2nd LRJ)
            TString sigLabel2Ev1 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsSigEv1[1].first, newSeedPositionsSigEv1[1].second, sigLRJEtEv1[1]);

            // Option 1: Use TLatex directly
            TLatex sigLat2Ev1;
            sigLat2Ev1.SetTextSize(0.025);     // Adjust size
            sigLat2Ev1.SetTextColor(kBlack); // Match sigCircle color
            sigLat2Ev1.DrawLatex(newSeedPositionsSigEv1[1].first, newSeedPositionsSigEv1[1].second + 0.6, sigLabel2Ev1); // Slightly above the sigCircle\

            c.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev1.pdf");   
        }
        
        
        if(displayEv2Sig_){
            sigTopo422Highest128SeedPositionsEv2->GetZaxis()->SetTitle("E_{T} [GeV]");
            sigTopo422Highest128SeedPositionsEv2->Draw("COLZ");


            TEllipse *sigCircle0_Ev2 = new TEllipse(jFexSeedPositionsSigEv2[0].first, jFexSeedPositionsSigEv2[0].second, 1.0, 1.0); // R in both x and y
            sigCircle0_Ev2->SetLineColor(kRed);
            sigCircle0_Ev2->SetLineWidth(2);
            sigCircle0_Ev2->SetFillStyle(0); // no fill
            sigCircle0_Ev2->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle0_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle1_Ev2 = new TEllipse(jFexSeedPositionsSigEv2[1].first, jFexSeedPositionsSigEv2[1].second, 1.0, 1.0); // R in both x and y
            sigCircle1_Ev2->SetLineColor(kRed);
            sigCircle1_Ev2->SetLineWidth(2);
            sigCircle1_Ev2->SetFillStyle(0); // no fill
            sigCircle1_Ev2->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle1_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle2_Ev2 = new TEllipse(jFexAdditionalSRJPositionsSigEv2[0].first, jFexAdditionalSRJPositionsSigEv2[0].second, 1.0, 1.0); // R in both x and y
            sigCircle2_Ev2->SetLineColor(kAzure+2);
            sigCircle2_Ev2->SetLineWidth(2);
            sigCircle2_Ev2->SetFillStyle(0); // no fill
            sigCircle2_Ev2->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle2_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle3_Ev2 = new TEllipse(jFexAdditionalSRJPositionsSigEv2[1].first, jFexAdditionalSRJPositionsSigEv2[1].second, 1.0, 1.0); // R in both x and y
            sigCircle3_Ev2->SetLineColor(kAzure+2);
            sigCircle3_Ev2->SetLineWidth(2);
            sigCircle3_Ev2->SetFillStyle(0); // no fill
            sigCircle3_Ev2->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle3_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle4_Ev2 = new TEllipse(jFexAdditionalSRJPositionsSigEv2[2].first, jFexAdditionalSRJPositionsSigEv2[2].second, 1.0, 1.0); // R in both x and y
            sigCircle4_Ev2->SetLineColor(kAzure+2);
            sigCircle4_Ev2->SetLineWidth(2);
            sigCircle4_Ev2->SetFillStyle(0); // no fill
            sigCircle4_Ev2->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle4_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle5_Ev2 = new TEllipse(jFexAdditionalSRJPositionsSigEv2[3].first, jFexAdditionalSRJPositionsSigEv2[3].second, 1.0, 1.0); // R in both x and y
            sigCircle5_Ev2->SetLineColor(kAzure+2);
            sigCircle5_Ev2->SetLineWidth(2);
            sigCircle5_Ev2->SetFillStyle(0); // no fill
            sigCircle5_Ev2->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle5_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle6_Ev2 = new TEllipse(newSeedPositionsSigEv2[0].first, newSeedPositionsSigEv2[0].second, 1.0, 1.0); // R in both x and y
            sigCircle6_Ev2->SetLineColor(kGreen+2);
            sigCircle6_Ev2->SetLineWidth(2);
            sigCircle6_Ev2->SetFillStyle(0); // no fill
            sigCircle6_Ev2->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle6_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle7_Ev2 = new TEllipse(newSeedPositionsSigEv2[1].first, newSeedPositionsSigEv2[1].second, 1.0, 1.0); // R in both x and y
            sigCircle7_Ev2->SetLineColor(kGreen+2);
            sigCircle7_Ev2->SetLineWidth(2);
            sigCircle7_Ev2->SetFillStyle(0); // no fill
            sigCircle7_Ev2->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle7_Ev2->Draw("same");    // overlay on the existing plot

            // Build the label text (1st LRJ)
            TString sigLabel1Ev2 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsSigEv2[0].first, newSeedPositionsSigEv2[0].second, sigLRJEtEv2[0]);

            // Option 1: Use TLatex directly
            TLatex sigLat1Ev2;
            sigLat1Ev2.SetTextSize(0.025);     // Adjust size
            sigLat1Ev2.SetTextColor(kBlack); // Match sigCircle color
            sigLat1Ev2.DrawLatex(newSeedPositionsSigEv2[0].first, newSeedPositionsSigEv2[0].second + 0.6, sigLabel1Ev2); // Slightly above the sigCircle

            // Build the label text (2nd LRJ)
            TString sigLabel2Ev2 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsSigEv2[1].first, newSeedPositionsSigEv2[1].second, sigLRJEtEv2[1]);

            // Option 1: Use TLatex directly
            TLatex sigLat2Ev2;
            sigLat2Ev2.SetTextSize(0.025);     // Adjust size
            sigLat2Ev2.SetTextColor(kBlack); // Match sigCircle color
            sigLat2Ev2.DrawLatex(newSeedPositionsSigEv2[1].first, newSeedPositionsSigEv2[1].second + 0.6, sigLabel2Ev2); // Slightly above the sigCircle\

            c.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev2.pdf");   
        }
        
        if(displayEv3Sig_){
            sigTopo422Highest128SeedPositionsEv3->GetZaxis()->SetTitle("E_{T} [GeV]");
            sigTopo422Highest128SeedPositionsEv3->Draw("COLZ");


            TEllipse *sigCircle0_Ev3 = new TEllipse(jFexSeedPositionsSigEv3[0].first, jFexSeedPositionsSigEv3[0].second, 1.0, 1.0); // R in both x and y
            sigCircle0_Ev3->SetLineColor(kRed);
            sigCircle0_Ev3->SetLineWidth(2);
            sigCircle0_Ev3->SetFillStyle(0); // no fill
            sigCircle0_Ev3->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle0_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle1_Ev3 = new TEllipse(jFexSeedPositionsSigEv3[1].first, jFexSeedPositionsSigEv3[1].second, 1.0, 1.0); // R in both x and y
            sigCircle1_Ev3->SetLineColor(kRed);
            sigCircle1_Ev3->SetLineWidth(2);
            sigCircle1_Ev3->SetFillStyle(0); // no fill
            sigCircle1_Ev3->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle1_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle2_Ev3 = new TEllipse(jFexAdditionalSRJPositionsSigEv3[0].first, jFexAdditionalSRJPositionsSigEv3[0].second, 1.0, 1.0); // R in both x and y
            sigCircle2_Ev3->SetLineColor(kAzure+2);
            sigCircle2_Ev3->SetLineWidth(2);
            sigCircle2_Ev3->SetFillStyle(0); // no fill
            sigCircle2_Ev3->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle2_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle3_Ev3 = new TEllipse(jFexAdditionalSRJPositionsSigEv3[1].first, jFexAdditionalSRJPositionsSigEv3[1].second, 1.0, 1.0); // R in both x and y
            sigCircle3_Ev3->SetLineColor(kAzure+2);
            sigCircle3_Ev3->SetLineWidth(2);
            sigCircle3_Ev3->SetFillStyle(0); // no fill
            sigCircle3_Ev3->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle3_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle4_Ev3 = new TEllipse(jFexAdditionalSRJPositionsSigEv3[2].first, jFexAdditionalSRJPositionsSigEv3[2].second, 1.0, 1.0); // R in both x and y
            sigCircle4_Ev3->SetLineColor(kAzure+2);
            sigCircle4_Ev3->SetLineWidth(2);
            sigCircle4_Ev3->SetFillStyle(0); // no fill
            sigCircle4_Ev3->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle4_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle5_Ev3 = new TEllipse(jFexAdditionalSRJPositionsSigEv3[3].first, jFexAdditionalSRJPositionsSigEv3[3].second, 1.0, 1.0); // R in both x and y
            sigCircle5_Ev3->SetLineColor(kAzure+2);
            sigCircle5_Ev3->SetLineWidth(2);
            sigCircle5_Ev3->SetFillStyle(0); // no fill
            sigCircle5_Ev3->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle5_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle6_Ev3 = new TEllipse(newSeedPositionsSigEv3[0].first, newSeedPositionsSigEv3[0].second, 1.0, 1.0); // R in both x and y
            sigCircle6_Ev3->SetLineColor(kGreen+2);
            sigCircle6_Ev3->SetLineWidth(2);
            sigCircle6_Ev3->SetFillStyle(0); // no fill
            sigCircle6_Ev3->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle6_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle7_Ev3 = new TEllipse(newSeedPositionsSigEv3[1].first, newSeedPositionsSigEv3[1].second, 1.0, 1.0); // R in both x and y
            sigCircle7_Ev3->SetLineColor(kGreen+2);
            sigCircle7_Ev3->SetLineWidth(2);
            sigCircle7_Ev3->SetFillStyle(0); // no fill
            sigCircle7_Ev3->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle7_Ev3->Draw("same");    // overlay on the existing plot

            // Build the label text (1st LRJ)
            TString sigLabel1Ev3 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsSigEv3[0].first, newSeedPositionsSigEv3[0].second, sigLRJEtEv3[0]);

            // Option 1: Use TLatex directly
            TLatex sigLat1Ev3;
            sigLat1Ev3.SetTextSize(0.025);     // Adjust size
            sigLat1Ev3.SetTextColor(kBlack); // Match sigCircle color
            sigLat1Ev3.DrawLatex(newSeedPositionsSigEv3[0].first, newSeedPositionsSigEv3[0].second + 0.6, sigLabel1Ev3); // Slightly above the sigCircle

            // Build the label text (2nd LRJ)
            TString sigLabel2Ev3 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsSigEv3[1].first, newSeedPositionsSigEv3[1].second, sigLRJEtEv3[1]);

            // Option 1: Use TLatex directly
            TLatex sigLat2Ev3;
            sigLat2Ev3.SetTextSize(0.025);     // Adjust size
            sigLat2Ev3.SetTextColor(kBlack); // Match sigCircle color
            sigLat2Ev3.DrawLatex(newSeedPositionsSigEv3[1].first, newSeedPositionsSigEv3[1].second + 0.6, sigLabel2Ev3); // Slightly above the sigCircle

            c.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev3.pdf");   
        }
        
        
        if(displayEv4Sig_){
            sigTopo422Highest128SeedPositionsEv4->GetZaxis()->SetTitle("E_{T} [GeV]");
            sigTopo422Highest128SeedPositionsEv4->Draw("COLZ");


            TEllipse *sigCircle0_Ev4 = new TEllipse(jFexSeedPositionsSigEv4[0].first, jFexSeedPositionsSigEv4[0].second, 1.0, 1.0); // R in both x and y
            sigCircle0_Ev4->SetLineColor(kRed);
            sigCircle0_Ev4->SetLineWidth(2);
            sigCircle0_Ev4->SetFillStyle(0); // no fill
            sigCircle0_Ev4->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle0_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle1_Ev4 = new TEllipse(jFexSeedPositionsSigEv4[1].first, jFexSeedPositionsSigEv4[1].second, 1.0, 1.0); // R in both x and y
            sigCircle1_Ev4->SetLineColor(kRed);
            sigCircle1_Ev4->SetLineWidth(2);
            sigCircle1_Ev4->SetFillStyle(0); // no fill
            sigCircle1_Ev4->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle1_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle2_Ev4 = new TEllipse(jFexAdditionalSRJPositionsSigEv4[0].first, jFexAdditionalSRJPositionsSigEv4[0].second, 1.0, 1.0); // R in both x and y
            sigCircle2_Ev4->SetLineColor(kAzure+2);
            sigCircle2_Ev4->SetLineWidth(2);
            sigCircle2_Ev4->SetFillStyle(0); // no fill
            sigCircle2_Ev4->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle2_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle3_Ev4 = new TEllipse(jFexAdditionalSRJPositionsSigEv4[1].first, jFexAdditionalSRJPositionsSigEv4[1].second, 1.0, 1.0); // R in both x and y
            sigCircle3_Ev4->SetLineColor(kAzure+2);
            sigCircle3_Ev4->SetLineWidth(2);
            sigCircle3_Ev4->SetFillStyle(0); // no fill
            sigCircle3_Ev4->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle3_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle4_Ev4 = new TEllipse(jFexAdditionalSRJPositionsSigEv4[2].first, jFexAdditionalSRJPositionsSigEv4[2].second, 1.0, 1.0); // R in both x and y
            sigCircle4_Ev4->SetLineColor(kAzure+2);
            sigCircle4_Ev4->SetLineWidth(2);
            sigCircle4_Ev4->SetFillStyle(0); // no fill
            sigCircle4_Ev4->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle4_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle5_Ev4 = new TEllipse(jFexAdditionalSRJPositionsSigEv4[3].first, jFexAdditionalSRJPositionsSigEv4[3].second, 1.0, 1.0); // R in both x and y
            sigCircle5_Ev4->SetLineColor(kAzure+2);
            sigCircle5_Ev4->SetLineWidth(2);
            sigCircle5_Ev4->SetFillStyle(0); // no fill
            sigCircle5_Ev4->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle5_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle6_Ev4 = new TEllipse(newSeedPositionsSigEv4[0].first, newSeedPositionsSigEv4[0].second, 1.0, 1.0); // R in both x and y
            sigCircle6_Ev4->SetLineColor(kGreen+2);
            sigCircle6_Ev4->SetLineWidth(2);
            sigCircle6_Ev4->SetFillStyle(0); // no fill
            sigCircle6_Ev4->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle6_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *sigCircle7_Ev4 = new TEllipse(newSeedPositionsSigEv4[1].first, newSeedPositionsSigEv4[1].second, 1.0, 1.0); // R in both x and y
            sigCircle7_Ev4->SetLineColor(kGreen+2);
            sigCircle7_Ev4->SetLineWidth(2);
            sigCircle7_Ev4->SetFillStyle(0); // no fill
            sigCircle7_Ev4->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            sigCircle7_Ev4->Draw("same");    // overlay on the existing plot

            // Build the label text (1st LRJ)
            TString sigLabel1Ev4 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsSigEv4[0].first, newSeedPositionsSigEv4[0].second, sigLRJEtEv4[0]);

            // Option 1: Use TLatex directly
            TLatex sigLat1Ev4;
            sigLat1Ev4.SetTextSize(0.025);     // Adjust size
            sigLat1Ev4.SetTextColor(kBlack); // Match sigCircle color
            sigLat1Ev4.DrawLatex(newSeedPositionsSigEv4[0].first, newSeedPositionsSigEv4[0].second + 0.6, sigLabel1Ev4); // Slightly above the sigCircle

            // Build the label text (2nd LRJ)
            TString sigLabel2Ev4 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsSigEv4[1].first, newSeedPositionsSigEv4[1].second, sigLRJEtEv4[1]);

            // Option 1: Use TLatex directly
            TLatex sigLat2Ev4;
            sigLat2Ev4.SetTextSize(0.025);     // Adjust size
            sigLat2Ev4.SetTextColor(kBlack); // Match sigCircle color
            sigLat2Ev4.DrawLatex(newSeedPositionsSigEv4[1].first, newSeedPositionsSigEv4[1].second + 0.6, sigLabel2Ev4); // Slightly above the sigCircle

            c.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev4.pdf");   
        }
        
        
        // Guard: fetch ET values if available
        auto getET = [](const std::vector<double>& v, size_t i)->double {
            return (i < v.size() ? v[i] : 0.0);
        };

        TCanvas cEventDisplays;
        cEventDisplays.cd();

        if (displayEv0Sig_) {
            
            // ---------------- MAIN EVENT DISPLAY (NO LEGEND HERE) ----------------
            // (No right-margin fiddling so the plot doesn't warp.)
            sigTopo422Highest128SeedPositionsEv0->GetZaxis()->SetTitle("E_{T} [GeV]");
            sigTopo422Highest128SeedPositionsEv0->Draw("COLZ");

            // gFEX LRJs
            TEllipse *sigCircle0_Ev0_gFEX_Offline_JetTagger =
                new TEllipse(gFexLRJPositionSigEv0[0].first, gFexLRJPositionSigEv0[0].second, 0.9, 0.9);
            sigCircle0_Ev0_gFEX_Offline_JetTagger->SetLineColor(colors[2]);
            sigCircle0_Ev0_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle0_Ev0_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle0_Ev0_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle0_Ev0_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle1_Ev0_gFEX_Offline_JetTagger =
                new TEllipse(gFexLRJPositionSigEv0[1].first, gFexLRJPositionSigEv0[1].second, 0.9, 0.9);
            sigCircle1_Ev0_gFEX_Offline_JetTagger->SetLineColor(colors[2]);
            sigCircle1_Ev0_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle1_Ev0_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle1_Ev0_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle1_Ev0_gFEX_Offline_JetTagger->Draw("same");

            // JetTagger LRJs
            TEllipse *sigCircle2_Ev0_gFEX_Offline_JetTagger =
                new TEllipse(newSeedPositionsSigEv0[0].first, newSeedPositionsSigEv0[0].second, 1.0, 1.0);
            sigCircle2_Ev0_gFEX_Offline_JetTagger->SetLineColor(colors[3]);
            sigCircle2_Ev0_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle2_Ev0_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle2_Ev0_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle2_Ev0_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle3_Ev0_gFEX_Offline_JetTagger =
                new TEllipse(newSeedPositionsSigEv0[1].first, newSeedPositionsSigEv0[1].second, 1.0, 1.0);
            sigCircle3_Ev0_gFEX_Offline_JetTagger->SetLineColor(colors[3]);
            sigCircle3_Ev0_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle3_Ev0_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle3_Ev0_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle3_Ev0_gFEX_Offline_JetTagger->Draw("same");

            // Offline LRJs
            TEllipse *sigCircle4_Ev0_gFEX_Offline_JetTagger =
                new TEllipse(offlineLRJPositionSigEv0[0].first, offlineLRJPositionSigEv0[0].second, 1.0, 1.0);
            sigCircle4_Ev0_gFEX_Offline_JetTagger->SetLineColor(colors[4]);
            sigCircle4_Ev0_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle4_Ev0_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle4_Ev0_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle4_Ev0_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle5_Ev0_gFEX_Offline_JetTagger =
                new TEllipse(offlineLRJPositionSigEv0[1].first, offlineLRJPositionSigEv0[1].second, 1.0, 1.0);
            sigCircle5_Ev0_gFEX_Offline_JetTagger->SetLineColor(colors[4]);
            sigCircle5_Ev0_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle5_Ev0_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle5_Ev0_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle5_Ev0_gFEX_Offline_JetTagger->Draw("same");

            // Save the event display (no legend)
            
            cEventDisplays.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev0_gFEX_Offline_JetTagger.pdf");

            // ---------------- LEGEND-ONLY PDF ON A SEPARATE CANVAS ----------------
            const double gfexET_lead_Ev0 = getET(siggFexLRJEtEv0,    0);
            const double gfexET_sub_Ev0  = getET(siggFexLRJEtEv0,    1);
            const double jetET_lead_Ev0  = getET(sigLRJEtEv0,        0);
            const double jetET_sub_Ev0   = getET(sigLRJEtEv0,        1);
            const double offET_lead_Ev0  = getET(sigOfflineLRJEtEv0, 0);
            const double offET_sub_Ev0   = getET(sigOfflineLRJEtEv0, 1);

            TString lab_gfex_lead_Ev0 = Form("gFEX Leading  (E_{T}=%.1f GeV)",        gfexET_lead_Ev0);
            TString lab_gfex_sub_Ev0  = Form("gFEX Subleading (E_{T}=%.1f GeV)",      gfexET_sub_Ev0);
            TString lab_jet_lead_Ev0  = Form("JetTagger Leading  (E_{T}=%.1f GeV)",   jetET_lead_Ev0);
            TString lab_jet_sub_Ev0   = Form("JetTagger Subleading (E_{T}=%.1f GeV)", jetET_sub_Ev0);
            TString lab_off_lead_Ev0  = Form("Offline Leading  (E_{T}=%.1f GeV)",     offET_lead_Ev0);
            TString lab_off_sub_Ev0   = Form("Offline Subleading (E_{T}=%.1f GeV)",   offET_sub_Ev0);

            TCanvas cLeg_Ev0("cLeg_Ev0", "Legend Ev0", 600, 700);
            cLeg_Ev0.cd();
            cLeg_Ev0.SetBorderMode(0);
            cLeg_Ev0.SetFrameBorderMode(0);

            // Proxy lines with exact colors/styles to match circles
            TLine* gfex_lead_proxy = new TLine(0,0,1,0);  gfex_lead_proxy->SetLineColor(colors[2]); gfex_lead_proxy->SetLineStyle(1); gfex_lead_proxy->SetLineWidth(2);
            TLine* gfex_sub_proxy  = new TLine(0,0,1,0);  gfex_sub_proxy ->SetLineColor(colors[2]); gfex_sub_proxy ->SetLineStyle(2); gfex_sub_proxy ->SetLineWidth(2);

            TLine* jet_lead_proxy  = new TLine(0,0,1,0);  jet_lead_proxy ->SetLineColor(colors[3]); jet_lead_proxy ->SetLineStyle(1); jet_lead_proxy ->SetLineWidth(2);
            TLine* jet_sub_proxy   = new TLine(0,0,1,0);  jet_sub_proxy  ->SetLineColor(colors[3]); jet_sub_proxy  ->SetLineStyle(2); jet_sub_proxy  ->SetLineWidth(2);

            TLine* off_lead_proxy  = new TLine(0,0,1,0);  off_lead_proxy ->SetLineColor(colors[4]); off_lead_proxy ->SetLineStyle(1); off_lead_proxy ->SetLineWidth(2);
            TLine* off_sub_proxy   = new TLine(0,0,1,0);  off_sub_proxy  ->SetLineColor(colors[4]); off_sub_proxy  ->SetLineStyle(2); off_sub_proxy  ->SetLineWidth(2);

            TLegend* leg_only_Ev0 = new TLegend(0.08, 0.10, 0.92, 0.90);
            leg_only_Ev0->SetBorderSize(0);
            leg_only_Ev0->SetFillStyle(0);
            leg_only_Ev0->SetTextSize(0.04);
            leg_only_Ev0->SetHeader("LRJ positions", "C");

            leg_only_Ev0->AddEntry(gfex_lead_proxy, lab_gfex_lead_Ev0, "l");
            leg_only_Ev0->AddEntry(gfex_sub_proxy,  lab_gfex_sub_Ev0,  "l");
            leg_only_Ev0->AddEntry(jet_lead_proxy,  lab_jet_lead_Ev0,  "l");
            leg_only_Ev0->AddEntry(jet_sub_proxy,   lab_jet_sub_Ev0,   "l");
            leg_only_Ev0->AddEntry(off_lead_proxy,  lab_off_lead_Ev0,  "l");
            leg_only_Ev0->AddEntry(off_sub_proxy,   lab_off_sub_Ev0,   "l");

            leg_only_Ev0->Draw();

            cLeg_Ev0.Modified(); cLeg_Ev0.Update();
            cLeg_Ev0.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev0_gFEX_Offline_JetTagger_LEGEND.pdf");
        }

        if (displayEv1Sig_) {
            cEventDisplays.cd();
            
            // ---------------- MAIN EVENT DISPLAY (NO LEGEND HERE) ----------------
            // (No right-margin fiddling so the plot doesn't warp.)
            sigTopo422Highest128SeedPositionsEv1->GetZaxis()->SetTitle("E_{T} [GeV]");
            sigTopo422Highest128SeedPositionsEv1->Draw("COLZ");

            // gFEX LRJs
            TEllipse *sigCircle0_Ev1_gFEX_Offline_JetTagger =
                new TEllipse(gFexLRJPositionSigEv1[0].first, gFexLRJPositionSigEv1[0].second, 0.9, 0.9);
            sigCircle0_Ev1_gFEX_Offline_JetTagger->SetLineColor(colors[2]);
            sigCircle0_Ev1_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle0_Ev1_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle0_Ev1_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle0_Ev1_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle1_Ev1_gFEX_Offline_JetTagger =
                new TEllipse(gFexLRJPositionSigEv1[1].first, gFexLRJPositionSigEv1[1].second, 0.9, 0.9);
            sigCircle1_Ev1_gFEX_Offline_JetTagger->SetLineColor(colors[2]);
            sigCircle1_Ev1_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle1_Ev1_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle1_Ev1_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle1_Ev1_gFEX_Offline_JetTagger->Draw("same");

            // JetTagger LRJs
            TEllipse *sigCircle2_Ev1_gFEX_Offline_JetTagger =
                new TEllipse(newSeedPositionsSigEv1[0].first, newSeedPositionsSigEv1[0].second, 1.0, 1.0);
            sigCircle2_Ev1_gFEX_Offline_JetTagger->SetLineColor(colors[3]);
            sigCircle2_Ev1_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle2_Ev1_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle2_Ev1_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle2_Ev1_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle3_Ev1_gFEX_Offline_JetTagger =
                new TEllipse(newSeedPositionsSigEv1[1].first, newSeedPositionsSigEv1[1].second, 1.0, 1.0);
            sigCircle3_Ev1_gFEX_Offline_JetTagger->SetLineColor(colors[3]);
            sigCircle3_Ev1_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle3_Ev1_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle3_Ev1_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle3_Ev1_gFEX_Offline_JetTagger->Draw("same");

            // Offline LRJs
            TEllipse *sigCircle4_Ev1_gFEX_Offline_JetTagger =
                new TEllipse(offlineLRJPositionSigEv1[0].first, offlineLRJPositionSigEv1[0].second, 1.0, 1.0);
            sigCircle4_Ev1_gFEX_Offline_JetTagger->SetLineColor(colors[4]);
            sigCircle4_Ev1_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle4_Ev1_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle4_Ev1_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle4_Ev1_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle5_Ev1_gFEX_Offline_JetTagger =
                new TEllipse(offlineLRJPositionSigEv1[1].first, offlineLRJPositionSigEv1[1].second, 1.0, 1.0);
            sigCircle5_Ev1_gFEX_Offline_JetTagger->SetLineColor(colors[4]);
            sigCircle5_Ev1_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle5_Ev1_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle5_Ev1_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle5_Ev1_gFEX_Offline_JetTagger->Draw("same");

            // Save the event display (no legend)
            
            cEventDisplays.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev1_gFEX_Offline_JetTagger.pdf");

            // ---------------- LEGEND-ONLY PDF ON A SEPARATE CANVAS ----------------
            const double gfexET_lead_Ev1 = getET(siggFexLRJEtEv1,    0);
            const double gfexET_sub_Ev1  = getET(siggFexLRJEtEv1,    1);
            const double jetET_lead_Ev1  = getET(sigLRJEtEv1,        0);
            const double jetET_sub_Ev1   = getET(sigLRJEtEv1,        1);
            const double offET_lead_Ev1  = getET(sigOfflineLRJEtEv1, 0);
            const double offET_sub_Ev1   = getET(sigOfflineLRJEtEv1, 1);

            TString lab_gfex_lead_Ev1 = Form("gFEX Leading  (E_{T}=%.1f GeV)",        gfexET_lead_Ev1);
            TString lab_gfex_sub_Ev1  = Form("gFEX Subleading (E_{T}=%.1f GeV)",      gfexET_sub_Ev1);
            TString lab_jet_lead_Ev1  = Form("JetTagger Leading  (E_{T}=%.1f GeV)",   jetET_lead_Ev1);
            TString lab_jet_sub_Ev1   = Form("JetTagger Subleading (E_{T}=%.1f GeV)", jetET_sub_Ev1);
            TString lab_off_lead_Ev1  = Form("Offline Leading  (E_{T}=%.1f GeV)",     offET_lead_Ev1);
            TString lab_off_sub_Ev1   = Form("Offline Subleading (E_{T}=%.1f GeV)",   offET_sub_Ev1);

            TCanvas cLeg_Ev1("cLeg_Ev1", "Legend Ev1", 600, 700);
            cLeg_Ev1.cd();
            cLeg_Ev1.SetBorderMode(0);
            cLeg_Ev1.SetFrameBorderMode(0);

            // Proxy lines with exact colors/styles to match circles
            TLine* gfex_lead_proxy = new TLine(0,0,1,0);  gfex_lead_proxy->SetLineColor(colors[2]); gfex_lead_proxy->SetLineStyle(1); gfex_lead_proxy->SetLineWidth(2);
            TLine* gfex_sub_proxy  = new TLine(0,0,1,0);  gfex_sub_proxy ->SetLineColor(colors[2]); gfex_sub_proxy ->SetLineStyle(2); gfex_sub_proxy ->SetLineWidth(2);

            TLine* jet_lead_proxy  = new TLine(0,0,1,0);  jet_lead_proxy ->SetLineColor(colors[3]); jet_lead_proxy ->SetLineStyle(1); jet_lead_proxy ->SetLineWidth(2);
            TLine* jet_sub_proxy   = new TLine(0,0,1,0);  jet_sub_proxy  ->SetLineColor(colors[3]); jet_sub_proxy  ->SetLineStyle(2); jet_sub_proxy  ->SetLineWidth(2);

            TLine* off_lead_proxy  = new TLine(0,0,1,0);  off_lead_proxy ->SetLineColor(colors[4]); off_lead_proxy ->SetLineStyle(1); off_lead_proxy ->SetLineWidth(2);
            TLine* off_sub_proxy   = new TLine(0,0,1,0);  off_sub_proxy  ->SetLineColor(colors[4]); off_sub_proxy  ->SetLineStyle(2); off_sub_proxy  ->SetLineWidth(2);

            TLegend* leg_only_Ev1 = new TLegend(0.08, 0.10, 0.92, 0.90);
            leg_only_Ev1->SetBorderSize(0);
            leg_only_Ev1->SetFillStyle(0);
            leg_only_Ev1->SetTextSize(0.04);
            leg_only_Ev1->SetHeader("LRJ positions", "C");

            leg_only_Ev1->AddEntry(gfex_lead_proxy, lab_gfex_lead_Ev1, "l");
            leg_only_Ev1->AddEntry(gfex_sub_proxy,  lab_gfex_sub_Ev1,  "l");
            leg_only_Ev1->AddEntry(jet_lead_proxy,  lab_jet_lead_Ev1,  "l");
            leg_only_Ev1->AddEntry(jet_sub_proxy,   lab_jet_sub_Ev1,   "l");
            leg_only_Ev1->AddEntry(off_lead_proxy,  lab_off_lead_Ev1,  "l");
            leg_only_Ev1->AddEntry(off_sub_proxy,   lab_off_sub_Ev1,   "l");

            leg_only_Ev1->Draw();

            cLeg_Ev1.Modified(); cLeg_Ev1.Update();
            cLeg_Ev1.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev1_gFEX_Offline_JetTagger_LEGEND.pdf");
        }

        if (displayEv2Sig_) {
            cEventDisplays.cd();
            
            // ---------------- MAIN EVENT DISPLAY (NO LEGEND HERE) ----------------
            // (No right-margin fiddling so the plot doesn't warp.)
            sigTopo422Highest128SeedPositionsEv2->GetZaxis()->SetTitle("E_{T} [GeV]");
            sigTopo422Highest128SeedPositionsEv2->Draw("COLZ");

            // gFEX LRJs
            TEllipse *sigCircle0_Ev2_gFEX_Offline_JetTagger =
                new TEllipse(gFexLRJPositionSigEv2[0].first, gFexLRJPositionSigEv2[0].second, 0.9, 0.9);
            sigCircle0_Ev2_gFEX_Offline_JetTagger->SetLineColor(colors[2]);
            sigCircle0_Ev2_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle0_Ev2_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle0_Ev2_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle0_Ev2_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle1_Ev2_gFEX_Offline_JetTagger =
                new TEllipse(gFexLRJPositionSigEv2[1].first, gFexLRJPositionSigEv2[1].second, 0.9, 0.9);
            sigCircle1_Ev2_gFEX_Offline_JetTagger->SetLineColor(colors[2]);
            sigCircle1_Ev2_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle1_Ev2_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle1_Ev2_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle1_Ev2_gFEX_Offline_JetTagger->Draw("same");

            // JetTagger LRJs
            TEllipse *sigCircle2_Ev2_gFEX_Offline_JetTagger =
                new TEllipse(newSeedPositionsSigEv2[0].first, newSeedPositionsSigEv2[0].second, 1.0, 1.0);
            sigCircle2_Ev2_gFEX_Offline_JetTagger->SetLineColor(colors[3]);
            sigCircle2_Ev2_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle2_Ev2_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle2_Ev2_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle2_Ev2_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle3_Ev2_gFEX_Offline_JetTagger =
                new TEllipse(newSeedPositionsSigEv2[1].first, newSeedPositionsSigEv2[1].second, 1.0, 1.0);
            sigCircle3_Ev2_gFEX_Offline_JetTagger->SetLineColor(colors[3]);
            sigCircle3_Ev2_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle3_Ev2_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle3_Ev2_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle3_Ev2_gFEX_Offline_JetTagger->Draw("same");

            // Offline LRJs
            TEllipse *sigCircle4_Ev2_gFEX_Offline_JetTagger =
                new TEllipse(offlineLRJPositionSigEv2[0].first, offlineLRJPositionSigEv2[0].second, 1.0, 1.0);
            sigCircle4_Ev2_gFEX_Offline_JetTagger->SetLineColor(colors[4]);
            sigCircle4_Ev2_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle4_Ev2_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle4_Ev2_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle4_Ev2_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle5_Ev2_gFEX_Offline_JetTagger =
                new TEllipse(offlineLRJPositionSigEv2[1].first, offlineLRJPositionSigEv2[1].second, 1.0, 1.0);
            sigCircle5_Ev2_gFEX_Offline_JetTagger->SetLineColor(colors[4]);
            sigCircle5_Ev2_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle5_Ev2_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle5_Ev2_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle5_Ev2_gFEX_Offline_JetTagger->Draw("same");

            // Save the event display (no legend)
            
            cEventDisplays.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev2_gFEX_Offline_JetTagger.pdf");

            // ---------------- LEGEND-ONLY PDF ON A SEPARATE CANVAS ----------------
            const double gfexET_lead_Ev2 = getET(siggFexLRJEtEv2,    0);
            const double gfexET_sub_Ev2  = getET(siggFexLRJEtEv2,    1);
            const double jetET_lead_Ev2  = getET(sigLRJEtEv2,        0);
            const double jetET_sub_Ev2   = getET(sigLRJEtEv2,        1);
            const double offET_lead_Ev2  = getET(sigOfflineLRJEtEv2, 0);
            const double offET_sub_Ev2   = getET(sigOfflineLRJEtEv2, 1);

            TString lab_gfex_lead_Ev2 = Form("gFEX Leading  (E_{T}=%.1f GeV)",        gfexET_lead_Ev2);
            TString lab_gfex_sub_Ev2  = Form("gFEX Subleading (E_{T}=%.1f GeV)",      gfexET_sub_Ev2);
            TString lab_jet_lead_Ev2  = Form("JetTagger Leading  (E_{T}=%.1f GeV)",   jetET_lead_Ev2);
            TString lab_jet_sub_Ev2   = Form("JetTagger Subleading (E_{T}=%.1f GeV)", jetET_sub_Ev2);
            TString lab_off_lead_Ev2  = Form("Offline Leading  (E_{T}=%.1f GeV)",     offET_lead_Ev2);
            TString lab_off_sub_Ev2   = Form("Offline Subleading (E_{T}=%.1f GeV)",   offET_sub_Ev2);

            TCanvas cLeg_Ev2("cLeg_Ev2", "Legend Ev2", 600, 700);
            cLeg_Ev2.cd();
            cLeg_Ev2.SetBorderMode(0);
            cLeg_Ev2.SetFrameBorderMode(0);

            // Proxy lines with exact colors/styles to match circles
            TLine* gfex_lead_proxy = new TLine(0,0,1,0);  gfex_lead_proxy->SetLineColor(colors[2]); gfex_lead_proxy->SetLineStyle(1); gfex_lead_proxy->SetLineWidth(2);
            TLine* gfex_sub_proxy  = new TLine(0,0,1,0);  gfex_sub_proxy ->SetLineColor(colors[2]); gfex_sub_proxy ->SetLineStyle(2); gfex_sub_proxy ->SetLineWidth(2);

            TLine* jet_lead_proxy  = new TLine(0,0,1,0);  jet_lead_proxy ->SetLineColor(colors[3]); jet_lead_proxy ->SetLineStyle(1); jet_lead_proxy ->SetLineWidth(2);
            TLine* jet_sub_proxy   = new TLine(0,0,1,0);  jet_sub_proxy  ->SetLineColor(colors[3]); jet_sub_proxy  ->SetLineStyle(2); jet_sub_proxy  ->SetLineWidth(2);

            TLine* off_lead_proxy  = new TLine(0,0,1,0);  off_lead_proxy ->SetLineColor(colors[4]); off_lead_proxy ->SetLineStyle(1); off_lead_proxy ->SetLineWidth(2);
            TLine* off_sub_proxy   = new TLine(0,0,1,0);  off_sub_proxy  ->SetLineColor(colors[4]); off_sub_proxy  ->SetLineStyle(2); off_sub_proxy  ->SetLineWidth(2);

            TLegend* leg_only_Ev2 = new TLegend(0.08, 0.10, 0.92, 0.90);
            leg_only_Ev2->SetBorderSize(0);
            leg_only_Ev2->SetFillStyle(0);
            leg_only_Ev2->SetTextSize(0.04);
            leg_only_Ev2->SetHeader("LRJ positions", "C");

            leg_only_Ev2->AddEntry(gfex_lead_proxy, lab_gfex_lead_Ev2, "l");
            leg_only_Ev2->AddEntry(gfex_sub_proxy,  lab_gfex_sub_Ev2,  "l");
            leg_only_Ev2->AddEntry(jet_lead_proxy,  lab_jet_lead_Ev2,  "l");
            leg_only_Ev2->AddEntry(jet_sub_proxy,   lab_jet_sub_Ev2,   "l");
            leg_only_Ev2->AddEntry(off_lead_proxy,  lab_off_lead_Ev2,  "l");
            leg_only_Ev2->AddEntry(off_sub_proxy,   lab_off_sub_Ev2,   "l");

            leg_only_Ev2->Draw();

            cLeg_Ev2.Modified(); cLeg_Ev2.Update();
            cLeg_Ev2.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev2_gFEX_Offline_JetTagger_LEGEND.pdf");
        }

        if (displayEv3Sig_) {
            cEventDisplays.cd();
            
            // ---------------- MAIN EVENT DISPLAY (NO LEGEND HERE) ----------------
            // (No right-margin fiddling so the plot doesn't warp.)
            sigTopo422Highest128SeedPositionsEv3->GetZaxis()->SetTitle("E_{T} [GeV]");
            sigTopo422Highest128SeedPositionsEv3->Draw("COLZ");

            // gFEX LRJs
            TEllipse *sigCircle0_Ev3_gFEX_Offline_JetTagger =
                new TEllipse(gFexLRJPositionSigEv3[0].first, gFexLRJPositionSigEv3[0].second, 0.9, 0.9);
            sigCircle0_Ev3_gFEX_Offline_JetTagger->SetLineColor(colors[2]);
            sigCircle0_Ev3_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle0_Ev3_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle0_Ev3_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle0_Ev3_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle1_Ev3_gFEX_Offline_JetTagger =
                new TEllipse(gFexLRJPositionSigEv3[1].first, gFexLRJPositionSigEv3[1].second, 0.9, 0.9);
            sigCircle1_Ev3_gFEX_Offline_JetTagger->SetLineColor(colors[2]);
            sigCircle1_Ev3_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle1_Ev3_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle1_Ev3_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle1_Ev3_gFEX_Offline_JetTagger->Draw("same");

            // JetTagger LRJs
            TEllipse *sigCircle2_Ev3_gFEX_Offline_JetTagger =
                new TEllipse(newSeedPositionsSigEv3[0].first, newSeedPositionsSigEv3[0].second, 1.0, 1.0);
            sigCircle2_Ev3_gFEX_Offline_JetTagger->SetLineColor(colors[3]);
            sigCircle2_Ev3_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle2_Ev3_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle2_Ev3_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle2_Ev3_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle3_Ev3_gFEX_Offline_JetTagger =
                new TEllipse(newSeedPositionsSigEv3[1].first, newSeedPositionsSigEv3[1].second, 1.0, 1.0);
            sigCircle3_Ev3_gFEX_Offline_JetTagger->SetLineColor(colors[3]);
            sigCircle3_Ev3_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle3_Ev3_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle3_Ev3_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle3_Ev3_gFEX_Offline_JetTagger->Draw("same");

            // Offline LRJs
            TEllipse *sigCircle4_Ev3_gFEX_Offline_JetTagger =
                new TEllipse(offlineLRJPositionSigEv3[0].first, offlineLRJPositionSigEv3[0].second, 1.0, 1.0);
            sigCircle4_Ev3_gFEX_Offline_JetTagger->SetLineColor(colors[4]);
            sigCircle4_Ev3_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle4_Ev3_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle4_Ev3_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle4_Ev3_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle5_Ev3_gFEX_Offline_JetTagger =
                new TEllipse(offlineLRJPositionSigEv3[1].first, offlineLRJPositionSigEv3[1].second, 1.0, 1.0);
            sigCircle5_Ev3_gFEX_Offline_JetTagger->SetLineColor(colors[4]);
            sigCircle5_Ev3_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle5_Ev3_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle5_Ev3_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle5_Ev3_gFEX_Offline_JetTagger->Draw("same");

            // Save the event display (no legend)
            
            cEventDisplays.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev3_gFEX_Offline_JetTagger.pdf");

            // ---------------- LEGEND-ONLY PDF ON A SEPARATE CANVAS ----------------
            const double gfexET_lead_Ev3 = getET(siggFexLRJEtEv3,    0);
            const double gfexET_sub_Ev3  = getET(siggFexLRJEtEv3,    1);
            const double jetET_lead_Ev3  = getET(sigLRJEtEv3,        0);
            const double jetET_sub_Ev3   = getET(sigLRJEtEv3,        1);
            const double offET_lead_Ev3  = getET(sigOfflineLRJEtEv3, 0);
            const double offET_sub_Ev3   = getET(sigOfflineLRJEtEv3, 1);

            TString lab_gfex_lead_Ev3 = Form("gFEX Leading  (E_{T}=%.1f GeV)",        gfexET_lead_Ev3);
            TString lab_gfex_sub_Ev3  = Form("gFEX Subleading (E_{T}=%.1f GeV)",      gfexET_sub_Ev3);
            TString lab_jet_lead_Ev3  = Form("JetTagger Leading  (E_{T}=%.1f GeV)",   jetET_lead_Ev3);
            TString lab_jet_sub_Ev3   = Form("JetTagger Subleading (E_{T}=%.1f GeV)", jetET_sub_Ev3);
            TString lab_off_lead_Ev3  = Form("Offline Leading  (E_{T}=%.1f GeV)",     offET_lead_Ev3);
            TString lab_off_sub_Ev3   = Form("Offline Subleading (E_{T}=%.1f GeV)",   offET_sub_Ev3);

            TCanvas cLeg_Ev3("cLeg_Ev3", "Legend Ev3", 600, 700);
            cLeg_Ev3.cd();
            cLeg_Ev3.SetBorderMode(0);
            cLeg_Ev3.SetFrameBorderMode(0);

            // Proxy lines with exact colors/styles to match circles
            TLine* gfex_lead_proxy = new TLine(0,0,1,0);  gfex_lead_proxy->SetLineColor(colors[2]); gfex_lead_proxy->SetLineStyle(1); gfex_lead_proxy->SetLineWidth(2);
            TLine* gfex_sub_proxy  = new TLine(0,0,1,0);  gfex_sub_proxy ->SetLineColor(colors[2]); gfex_sub_proxy ->SetLineStyle(2); gfex_sub_proxy ->SetLineWidth(2);

            TLine* jet_lead_proxy  = new TLine(0,0,1,0);  jet_lead_proxy ->SetLineColor(colors[3]); jet_lead_proxy ->SetLineStyle(1); jet_lead_proxy ->SetLineWidth(2);
            TLine* jet_sub_proxy   = new TLine(0,0,1,0);  jet_sub_proxy  ->SetLineColor(colors[3]); jet_sub_proxy  ->SetLineStyle(2); jet_sub_proxy  ->SetLineWidth(2);

            TLine* off_lead_proxy  = new TLine(0,0,1,0);  off_lead_proxy ->SetLineColor(colors[4]); off_lead_proxy ->SetLineStyle(1); off_lead_proxy ->SetLineWidth(2);
            TLine* off_sub_proxy   = new TLine(0,0,1,0);  off_sub_proxy  ->SetLineColor(colors[4]); off_sub_proxy  ->SetLineStyle(2); off_sub_proxy  ->SetLineWidth(2);

            TLegend* leg_only_Ev3 = new TLegend(0.08, 0.10, 0.92, 0.90);
            leg_only_Ev3->SetBorderSize(0);
            leg_only_Ev3->SetFillStyle(0);
            leg_only_Ev3->SetTextSize(0.04);
            leg_only_Ev3->SetHeader("LRJ positions", "C");

            leg_only_Ev3->AddEntry(gfex_lead_proxy, lab_gfex_lead_Ev3, "l");
            leg_only_Ev3->AddEntry(gfex_sub_proxy,  lab_gfex_sub_Ev3,  "l");
            leg_only_Ev3->AddEntry(jet_lead_proxy,  lab_jet_lead_Ev3,  "l");
            leg_only_Ev3->AddEntry(jet_sub_proxy,   lab_jet_sub_Ev3,   "l");
            leg_only_Ev3->AddEntry(off_lead_proxy,  lab_off_lead_Ev3,  "l");
            leg_only_Ev3->AddEntry(off_sub_proxy,   lab_off_sub_Ev3,   "l");

            leg_only_Ev3->Draw();

            cLeg_Ev3.Modified(); cLeg_Ev3.Update();
            cLeg_Ev3.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev3_gFEX_Offline_JetTagger_LEGEND.pdf");
        }

        if (displayEv4Sig_) {
            cEventDisplays.cd();
            
            // ---------------- MAIN EVENT DISPLAY (NO LEGEND HERE) ----------------
            // (No right-margin fiddling so the plot doesn't warp.)
            sigTopo422Highest128SeedPositionsEv4->GetZaxis()->SetTitle("E_{T} [GeV]");
            sigTopo422Highest128SeedPositionsEv4->Draw("COLZ");

            // gFEX LRJs
            TEllipse *sigCircle0_Ev4_gFEX_Offline_JetTagger =
                new TEllipse(gFexLRJPositionSigEv4[0].first, gFexLRJPositionSigEv4[0].second, 0.9, 0.9);
            sigCircle0_Ev4_gFEX_Offline_JetTagger->SetLineColor(colors[2]);
            sigCircle0_Ev4_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle0_Ev4_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle0_Ev4_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle0_Ev4_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle1_Ev4_gFEX_Offline_JetTagger =
                new TEllipse(gFexLRJPositionSigEv4[1].first, gFexLRJPositionSigEv4[1].second, 0.9, 0.9);
            sigCircle1_Ev4_gFEX_Offline_JetTagger->SetLineColor(colors[2]);
            sigCircle1_Ev4_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle1_Ev4_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle1_Ev4_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle1_Ev4_gFEX_Offline_JetTagger->Draw("same");

            // JetTagger LRJs
            TEllipse *sigCircle2_Ev4_gFEX_Offline_JetTagger =
                new TEllipse(newSeedPositionsSigEv4[0].first, newSeedPositionsSigEv4[0].second, 1.0, 1.0);
            sigCircle2_Ev4_gFEX_Offline_JetTagger->SetLineColor(colors[3]);
            sigCircle2_Ev4_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle2_Ev4_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle2_Ev4_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle2_Ev4_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle3_Ev4_gFEX_Offline_JetTagger =
                new TEllipse(newSeedPositionsSigEv4[1].first, newSeedPositionsSigEv4[1].second, 1.0, 1.0);
            sigCircle3_Ev4_gFEX_Offline_JetTagger->SetLineColor(colors[3]);
            sigCircle3_Ev4_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle3_Ev4_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle3_Ev4_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle3_Ev4_gFEX_Offline_JetTagger->Draw("same");

            // Offline LRJs
            TEllipse *sigCircle4_Ev4_gFEX_Offline_JetTagger =
                new TEllipse(offlineLRJPositionSigEv4[0].first, offlineLRJPositionSigEv4[0].second, 1.0, 1.0);
            sigCircle4_Ev4_gFEX_Offline_JetTagger->SetLineColor(colors[4]);
            sigCircle4_Ev4_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle4_Ev4_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle4_Ev4_gFEX_Offline_JetTagger->SetLineStyle(1);  // leading = solid
            sigCircle4_Ev4_gFEX_Offline_JetTagger->Draw("same");

            TEllipse *sigCircle5_Ev4_gFEX_Offline_JetTagger =
                new TEllipse(offlineLRJPositionSigEv4[1].first, offlineLRJPositionSigEv4[1].second, 1.0, 1.0);
            sigCircle5_Ev4_gFEX_Offline_JetTagger->SetLineColor(colors[4]);
            sigCircle5_Ev4_gFEX_Offline_JetTagger->SetLineWidth(2);
            sigCircle5_Ev4_gFEX_Offline_JetTagger->SetFillStyle(0);
            sigCircle5_Ev4_gFEX_Offline_JetTagger->SetLineStyle(2);  // subleading = dashed
            sigCircle5_Ev4_gFEX_Offline_JetTagger->Draw("same");

            // Save the event display (no legend)
            
            cEventDisplays.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev4_gFEX_Offline_JetTagger.pdf");

            // ---------------- LEGEND-ONLY PDF ON A SEPARATE CANVAS ----------------
            const double gfexET_lead_Ev4 = getET(siggFexLRJEtEv4,    0);
            const double gfexET_sub_Ev4  = getET(siggFexLRJEtEv4,    1);
            const double jetET_lead_Ev4  = getET(sigLRJEtEv4,        0);
            const double jetET_sub_Ev4   = getET(sigLRJEtEv4,        1);
            const double offET_lead_Ev4  = getET(sigOfflineLRJEtEv4, 0);
            const double offET_sub_Ev4   = getET(sigOfflineLRJEtEv4, 1);

            TString lab_gfex_lead_Ev4 = Form("gFEX Leading  (E_{T}=%.1f GeV)",        gfexET_lead_Ev4);
            TString lab_gfex_sub_Ev4  = Form("gFEX Subleading (E_{T}=%.1f GeV)",      gfexET_sub_Ev4);
            TString lab_jet_lead_Ev4  = Form("JetTagger Leading  (E_{T}=%.1f GeV)",   jetET_lead_Ev4);
            TString lab_jet_sub_Ev4   = Form("JetTagger Subleading (E_{T}=%.1f GeV)", jetET_sub_Ev4);
            TString lab_off_lead_Ev4  = Form("Offline Leading  (E_{T}=%.1f GeV)",     offET_lead_Ev4);
            TString lab_off_sub_Ev4   = Form("Offline Subleading (E_{T}=%.1f GeV)",   offET_sub_Ev4);

            TCanvas cLeg_Ev4("cLeg_Ev4", "Legend Ev4", 600, 700);
            cLeg_Ev4.cd();
            cLeg_Ev4.SetBorderMode(0);
            cLeg_Ev4.SetFrameBorderMode(0);

            // Proxy lines with exact colors/styles to match circles
            TLine* gfex_lead_proxy = new TLine(0,0,1,0);  gfex_lead_proxy->SetLineColor(colors[2]); gfex_lead_proxy->SetLineStyle(1); gfex_lead_proxy->SetLineWidth(2);
            TLine* gfex_sub_proxy  = new TLine(0,0,1,0);  gfex_sub_proxy ->SetLineColor(colors[2]); gfex_sub_proxy ->SetLineStyle(2); gfex_sub_proxy ->SetLineWidth(2);

            TLine* jet_lead_proxy  = new TLine(0,0,1,0);  jet_lead_proxy ->SetLineColor(colors[3]); jet_lead_proxy ->SetLineStyle(1); jet_lead_proxy ->SetLineWidth(2);
            TLine* jet_sub_proxy   = new TLine(0,0,1,0);  jet_sub_proxy  ->SetLineColor(colors[3]); jet_sub_proxy  ->SetLineStyle(2); jet_sub_proxy  ->SetLineWidth(2);

            TLine* off_lead_proxy  = new TLine(0,0,1,0);  off_lead_proxy ->SetLineColor(colors[4]); off_lead_proxy ->SetLineStyle(1); off_lead_proxy ->SetLineWidth(2);
            TLine* off_sub_proxy   = new TLine(0,0,1,0);  off_sub_proxy  ->SetLineColor(colors[4]); off_sub_proxy  ->SetLineStyle(2); off_sub_proxy  ->SetLineWidth(2);

            TLegend* leg_only_Ev4 = new TLegend(0.08, 0.10, 0.92, 0.90);
            leg_only_Ev4->SetBorderSize(0);
            leg_only_Ev4->SetFillStyle(0);
            leg_only_Ev4->SetTextSize(0.04);
            leg_only_Ev4->SetHeader("LRJ positions", "C");

            leg_only_Ev4->AddEntry(gfex_lead_proxy, lab_gfex_lead_Ev4, "l");
            leg_only_Ev4->AddEntry(gfex_sub_proxy,  lab_gfex_sub_Ev4,  "l");
            leg_only_Ev4->AddEntry(jet_lead_proxy,  lab_jet_lead_Ev4,  "l");
            leg_only_Ev4->AddEntry(jet_sub_proxy,   lab_jet_sub_Ev4,   "l");
            leg_only_Ev4->AddEntry(off_lead_proxy,  lab_off_lead_Ev4,  "l");
            leg_only_Ev4->AddEntry(off_sub_proxy,   lab_off_sub_Ev4,   "l");

            leg_only_Ev4->Draw();

            cLeg_Ev4.Modified(); cLeg_Ev4.Update();
            cLeg_Ev4.SaveAs(modifiedOutputFileDir + "sigTopo422_highest128Et_EtaPhi_Ev4_gFEX_Offline_JetTagger_LEGEND.pdf");
        }


        











        jFexSeedPositionsSigEv0.clear();
        jFexAdditionalSRJPositionsSigEv0.clear();
        newSeedPositionsSigEv0.clear();
        sigLRJEtEv0.clear();
        gFexLRJPositionSigEv0.clear();
        offlineLRJPositionSigEv0.clear();
        siggFexLRJEtEv0.clear();
        sigOfflineLRJEtEv0.clear();
        jFexSeedPositionsSigEv1.clear();
        jFexAdditionalSRJPositionsSigEv1.clear();
        newSeedPositionsSigEv1.clear();
        sigLRJEtEv1.clear();
        gFexLRJPositionSigEv1.clear();
        offlineLRJPositionSigEv1.clear();
        siggFexLRJEtEv1.clear();
        sigOfflineLRJEtEv1.clear();
        jFexSeedPositionsSigEv2.clear();
        jFexAdditionalSRJPositionsSigEv2.clear();
        newSeedPositionsSigEv2.clear();
        sigLRJEtEv2.clear();
        gFexLRJPositionSigEv2.clear();
        offlineLRJPositionSigEv2.clear();
        siggFexLRJEtEv2.clear();
        sigOfflineLRJEtEv2.clear();
        jFexSeedPositionsSigEv3.clear();
        jFexAdditionalSRJPositionsSigEv3.clear();
        newSeedPositionsSigEv3.clear();
        sigLRJEtEv3.clear();
        gFexLRJPositionSigEv3.clear();
        offlineLRJPositionSigEv3.clear();
        siggFexLRJEtEv3.clear();
        sigOfflineLRJEtEv3.clear();
        jFexSeedPositionsSigEv4.clear();
        jFexAdditionalSRJPositionsSigEv4.clear();
        newSeedPositionsSigEv4.clear();
        sigLRJEtEv4.clear();
        gFexLRJPositionSigEv4.clear();
        offlineLRJPositionSigEv4.clear();
        siggFexLRJEtEv4.clear();
        sigOfflineLRJEtEv4.clear();
        

        // Draw background event displays //FIXME turn into a loop to reduce duplicate code
        if(displayEv0Back_){
            backTopo422Highest128SeedPositionsEv0->GetZaxis()->SetTitle("E_{T} [GeV]");
            backTopo422Highest128SeedPositionsEv0->Draw("COLZ");


            TEllipse *backCircle0_Ev0 = new TEllipse(jFexSeedPositionsBackEv0[0].first, jFexSeedPositionsBackEv0[0].second, 1.0, 1.0); // R in both x and y
            backCircle0_Ev0->SetLineColor(kRed);
            backCircle0_Ev0->SetLineWidth(2);
            backCircle0_Ev0->SetFillStyle(0); // no fill
            backCircle0_Ev0->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle0_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle1_Ev0 = new TEllipse(jFexSeedPositionsBackEv0[1].first, jFexSeedPositionsBackEv0[1].second, 1.0, 1.0); // R in both x and y
            backCircle1_Ev0->SetLineColor(kRed);
            backCircle1_Ev0->SetLineWidth(2);
            backCircle1_Ev0->SetFillStyle(0); // no fill
            backCircle1_Ev0->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle1_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle2_Ev0 = new TEllipse(jFexAdditionalSRJPositionsBackEv0[0].first, jFexAdditionalSRJPositionsBackEv0[0].second, 1.0, 1.0); // R in both x and y
            backCircle2_Ev0->SetLineColor(kAzure+2);
            backCircle2_Ev0->SetLineWidth(2);
            backCircle2_Ev0->SetFillStyle(0); // no fill
            backCircle2_Ev0->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle2_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle3_Ev0 = new TEllipse(jFexAdditionalSRJPositionsBackEv0[1].first, jFexAdditionalSRJPositionsBackEv0[1].second, 1.0, 1.0); // R in both x and y
            backCircle3_Ev0->SetLineColor(kAzure+2);
            backCircle3_Ev0->SetLineWidth(2);
            backCircle3_Ev0->SetFillStyle(0); // no fill
            backCircle3_Ev0->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle3_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle4_Ev0 = new TEllipse(jFexAdditionalSRJPositionsBackEv0[2].first, jFexAdditionalSRJPositionsBackEv0[2].second, 1.0, 1.0); // R in both x and y
            backCircle4_Ev0->SetLineColor(kAzure+2);
            backCircle4_Ev0->SetLineWidth(2);
            backCircle4_Ev0->SetFillStyle(0); // no fill
            backCircle4_Ev0->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle4_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle5_Ev0 = new TEllipse(jFexAdditionalSRJPositionsBackEv0[3].first, jFexAdditionalSRJPositionsBackEv0[3].second, 1.0, 1.0); // R in both x and y
            backCircle5_Ev0->SetLineColor(kAzure+2);
            backCircle5_Ev0->SetLineWidth(2);
            backCircle5_Ev0->SetFillStyle(0); // no fill
            backCircle5_Ev0->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle5_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle6_Ev0 = new TEllipse(newSeedPositionsBackEv0[0].first, newSeedPositionsBackEv0[0].second, 1.0, 1.0); // R in both x and y
            backCircle6_Ev0->SetLineColor(kGreen+2);
            backCircle6_Ev0->SetLineWidth(2);
            backCircle6_Ev0->SetFillStyle(0); // no fill
            backCircle6_Ev0->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle6_Ev0->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle7_Ev0 = new TEllipse(newSeedPositionsBackEv0[1].first, newSeedPositionsBackEv0[1].second, 1.0, 1.0); // R in both x and y
            backCircle7_Ev0->SetLineColor(kGreen+2);
            backCircle7_Ev0->SetLineWidth(2);
            backCircle7_Ev0->SetFillStyle(0); // no fill
            backCircle7_Ev0->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle7_Ev0->Draw("same");    // overlay on the existing plot

            // Build the label text (1st LRJ)
            TString backLabel1Ev0 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsBackEv0[0].first, newSeedPositionsBackEv0[0].second, backLRJEtEv0[0]);

            // Option 1: Use TLatex directly
            TLatex backLat1Ev0;
            backLat1Ev0.SetTextSize(0.025);     // Adjust size
            backLat1Ev0.SetTextColor(kBlack); // Match backCircle color
            backLat1Ev0.DrawLatex(newSeedPositionsBackEv0[0].first, newSeedPositionsBackEv0[0].second + 0.6, backLabel1Ev0); // Slightly above the backCircle

            // Build the label text (2nd LRJ)
            TString backLabel2Ev0 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsBackEv0[1].first, newSeedPositionsBackEv0[1].second, backLRJEtEv0[1]);

            // Option 1: Use TLatex directly
            TLatex backLat2Ev0;
            backLat2Ev0.SetTextSize(0.025);     // Adjust size
            backLat2Ev0.SetTextColor(kBlack); // Match backCircle color
            backLat2Ev0.DrawLatex(newSeedPositionsBackEv0[1].first, newSeedPositionsBackEv0[1].second + 0.6, backLabel2Ev0); // Slightly above the backCircle\

            c.SaveAs(modifiedOutputFileDir + "backTopo422_highest128Et_EtaPhi_Ev0.pdf");   

            jFexSeedPositionsBackEv0.clear();
            jFexAdditionalSRJPositionsBackEv0.clear();
            newSeedPositionsBackEv0.clear();
            backLRJEtEv0.clear();
        }
        
        if(displayEv1Back_){
            backTopo422Highest128SeedPositionsEv1->GetZaxis()->SetTitle("E_{T} [GeV]");
            backTopo422Highest128SeedPositionsEv1->Draw("COLZ");


            TEllipse *backCircle0_Ev1 = new TEllipse(jFexSeedPositionsBackEv1[0].first, jFexSeedPositionsBackEv1[0].second, 1.0, 1.0); // R in both x and y
            backCircle0_Ev1->SetLineColor(kRed);
            backCircle0_Ev1->SetLineWidth(2);
            backCircle0_Ev1->SetFillStyle(0); // no fill
            backCircle0_Ev1->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle0_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle1_Ev1 = new TEllipse(jFexSeedPositionsBackEv1[1].first, jFexSeedPositionsBackEv1[1].second, 1.0, 1.0); // R in both x and y
            backCircle1_Ev1->SetLineColor(kRed);
            backCircle1_Ev1->SetLineWidth(2);
            backCircle1_Ev1->SetFillStyle(0); // no fill
            backCircle1_Ev1->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle1_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle2_Ev1 = new TEllipse(jFexAdditionalSRJPositionsBackEv1[0].first, jFexAdditionalSRJPositionsBackEv1[0].second, 1.0, 1.0); // R in both x and y
            backCircle2_Ev1->SetLineColor(kAzure+2);
            backCircle2_Ev1->SetLineWidth(2);
            backCircle2_Ev1->SetFillStyle(0); // no fill
            backCircle2_Ev1->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle2_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle3_Ev1 = new TEllipse(jFexAdditionalSRJPositionsBackEv1[1].first, jFexAdditionalSRJPositionsBackEv1[1].second, 1.0, 1.0); // R in both x and y
            backCircle3_Ev1->SetLineColor(kAzure+2);
            backCircle3_Ev1->SetLineWidth(2);
            backCircle3_Ev1->SetFillStyle(0); // no fill
            backCircle3_Ev1->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle3_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle4_Ev1 = new TEllipse(jFexAdditionalSRJPositionsBackEv1[2].first, jFexAdditionalSRJPositionsBackEv1[2].second, 1.0, 1.0); // R in both x and y
            backCircle4_Ev1->SetLineColor(kAzure+2);
            backCircle4_Ev1->SetLineWidth(2);
            backCircle4_Ev1->SetFillStyle(0); // no fill
            backCircle4_Ev1->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle4_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle5_Ev1 = new TEllipse(jFexAdditionalSRJPositionsBackEv1[3].first, jFexAdditionalSRJPositionsBackEv1[3].second, 1.0, 1.0); // R in both x and y
            backCircle5_Ev1->SetLineColor(kAzure+2);
            backCircle5_Ev1->SetLineWidth(2);
            backCircle5_Ev1->SetFillStyle(0); // no fill
            backCircle5_Ev1->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle5_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle6_Ev1 = new TEllipse(newSeedPositionsBackEv1[0].first, newSeedPositionsBackEv1[0].second, 1.0, 1.0); // R in both x and y
            backCircle6_Ev1->SetLineColor(kGreen+2);
            backCircle6_Ev1->SetLineWidth(2);
            backCircle6_Ev1->SetFillStyle(0); // no fill
            backCircle6_Ev1->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle6_Ev1->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle7_Ev1 = new TEllipse(newSeedPositionsBackEv1[1].first, newSeedPositionsBackEv1[1].second, 1.0, 1.0); // R in both x and y
            backCircle7_Ev1->SetLineColor(kGreen+2);
            backCircle7_Ev1->SetLineWidth(2);
            backCircle7_Ev1->SetFillStyle(0); // no fill
            backCircle7_Ev1->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle7_Ev1->Draw("same");    // overlay on the existing plot

            // Build the label text (1st LRJ)
            TString backLabel1Ev1 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsBackEv1[0].first, newSeedPositionsBackEv1[0].second, backLRJEtEv1[0]);

            // Option 1: Use TLatex directly
            TLatex backLat1Ev1;
            backLat1Ev1.SetTextSize(0.025);     // Adjust size
            backLat1Ev1.SetTextColor(kBlack); // Match backCircle color
            backLat1Ev1.DrawLatex(newSeedPositionsBackEv1[0].first, newSeedPositionsBackEv1[0].second + 0.6, backLabel1Ev1); // Slightly above the backCircle

            // Build the label text (2nd LRJ)
            TString backLabel2Ev1 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsBackEv1[1].first, newSeedPositionsBackEv1[1].second, backLRJEtEv1[1]);

            // Option 1: Use TLatex directly
            TLatex backLat2Ev1;
            backLat2Ev1.SetTextSize(0.025);     // Adjust size
            backLat2Ev1.SetTextColor(kBlack); // Match backCircle color
            backLat2Ev1.DrawLatex(newSeedPositionsBackEv1[1].first, newSeedPositionsBackEv1[1].second + 0.6, backLabel2Ev1); // Slightly above the backCircle\

            c.SaveAs(modifiedOutputFileDir + "backTopo422_highest128Et_EtaPhi_Ev1.pdf");   
            
            
            jFexSeedPositionsBackEv1.clear();
            jFexAdditionalSRJPositionsBackEv1.clear();
            newSeedPositionsBackEv1.clear();
            backLRJEtEv1.clear();
        }
        
        if(displayEv2Back_){
            backTopo422Highest128SeedPositionsEv2->GetZaxis()->SetTitle("E_{T} [GeV]");
            backTopo422Highest128SeedPositionsEv2->Draw("COLZ");


            TEllipse *backCircle0_Ev2 = new TEllipse(jFexSeedPositionsBackEv2[0].first, jFexSeedPositionsBackEv2[0].second, 1.0, 1.0); // R in both x and y
            backCircle0_Ev2->SetLineColor(kRed);
            backCircle0_Ev2->SetLineWidth(2);
            backCircle0_Ev2->SetFillStyle(0); // no fill
            backCircle0_Ev2->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle0_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle1_Ev2 = new TEllipse(jFexSeedPositionsBackEv2[1].first, jFexSeedPositionsBackEv2[1].second, 1.0, 1.0); // R in both x and y
            backCircle1_Ev2->SetLineColor(kRed);
            backCircle1_Ev2->SetLineWidth(2);
            backCircle1_Ev2->SetFillStyle(0); // no fill
            backCircle1_Ev2->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle1_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle2_Ev2 = new TEllipse(jFexAdditionalSRJPositionsBackEv2[0].first, jFexAdditionalSRJPositionsBackEv2[0].second, 1.0, 1.0); // R in both x and y
            backCircle2_Ev2->SetLineColor(kAzure+2);
            backCircle2_Ev2->SetLineWidth(2);
            backCircle2_Ev2->SetFillStyle(0); // no fill
            backCircle2_Ev2->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle2_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle3_Ev2 = new TEllipse(jFexAdditionalSRJPositionsBackEv2[1].first, jFexAdditionalSRJPositionsBackEv2[1].second, 1.0, 1.0); // R in both x and y
            backCircle3_Ev2->SetLineColor(kAzure+2);
            backCircle3_Ev2->SetLineWidth(2);
            backCircle3_Ev2->SetFillStyle(0); // no fill
            backCircle3_Ev2->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle3_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle4_Ev2 = new TEllipse(jFexAdditionalSRJPositionsBackEv2[2].first, jFexAdditionalSRJPositionsBackEv2[2].second, 1.0, 1.0); // R in both x and y
            backCircle4_Ev2->SetLineColor(kAzure+2);
            backCircle4_Ev2->SetLineWidth(2);
            backCircle4_Ev2->SetFillStyle(0); // no fill
            backCircle4_Ev2->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle4_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle5_Ev2 = new TEllipse(jFexAdditionalSRJPositionsBackEv2[3].first, jFexAdditionalSRJPositionsBackEv2[3].second, 1.0, 1.0); // R in both x and y
            backCircle5_Ev2->SetLineColor(kAzure+2);
            backCircle5_Ev2->SetLineWidth(2);
            backCircle5_Ev2->SetFillStyle(0); // no fill
            backCircle5_Ev2->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle5_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle6_Ev2 = new TEllipse(newSeedPositionsBackEv2[0].first, newSeedPositionsBackEv2[0].second, 1.0, 1.0); // R in both x and y
            backCircle6_Ev2->SetLineColor(kGreen+2);
            backCircle6_Ev2->SetLineWidth(2);
            backCircle6_Ev2->SetFillStyle(0); // no fill
            backCircle6_Ev2->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle6_Ev2->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle7_Ev2 = new TEllipse(newSeedPositionsBackEv2[1].first, newSeedPositionsBackEv2[1].second, 1.0, 1.0); // R in both x and y
            backCircle7_Ev2->SetLineColor(kGreen+2);
            backCircle7_Ev2->SetLineWidth(2);
            backCircle7_Ev2->SetFillStyle(0); // no fill
            backCircle7_Ev2->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle7_Ev2->Draw("same");    // overlay on the existing plot

            // Build the label text (1st LRJ)
            TString backLabel1Ev2 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsBackEv2[0].first, newSeedPositionsBackEv2[0].second, backLRJEtEv2[0]);

            // Option 1: Use TLatex directly
            TLatex backLat1Ev2;
            backLat1Ev2.SetTextSize(0.025);     // Adjust size
            backLat1Ev2.SetTextColor(kBlack); // Match backCircle color
            backLat1Ev2.DrawLatex(newSeedPositionsBackEv2[0].first, newSeedPositionsBackEv2[0].second + 0.6, backLabel1Ev2); // Slightly above the backCircle

            // Build the label text (2nd LRJ)
            TString backLabel2Ev2 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsBackEv2[1].first, newSeedPositionsBackEv2[1].second, backLRJEtEv2[1]);

            // Option 1: Use TLatex directly
            TLatex backLat2Ev2;
            backLat2Ev2.SetTextSize(0.025);     // Adjust size
            backLat2Ev2.SetTextColor(kBlack); // Match backCircle color
            backLat2Ev2.DrawLatex(newSeedPositionsBackEv2[1].first, newSeedPositionsBackEv2[1].second + 0.6, backLabel2Ev2); // Slightly above the backCircle\

            c.SaveAs(modifiedOutputFileDir + "backTopo422_highest128Et_EtaPhi_Ev2.pdf");   
            
            
            jFexSeedPositionsBackEv2.clear();
            jFexAdditionalSRJPositionsBackEv2.clear();
            newSeedPositionsBackEv2.clear();
            backLRJEtEv2.clear();
        }
        

        if(displayEv3Back_){
            backTopo422Highest128SeedPositionsEv3->GetZaxis()->SetTitle("E_{T} [GeV]");
            backTopo422Highest128SeedPositionsEv3->Draw("COLZ");


            TEllipse *backCircle0_Ev3 = new TEllipse(jFexSeedPositionsBackEv3[0].first, jFexSeedPositionsBackEv3[0].second, 1.0, 1.0); // R in both x and y
            backCircle0_Ev3->SetLineColor(kRed);
            backCircle0_Ev3->SetLineWidth(2);
            backCircle0_Ev3->SetFillStyle(0); // no fill
            backCircle0_Ev3->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle0_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle1_Ev3 = new TEllipse(jFexSeedPositionsBackEv3[1].first, jFexSeedPositionsBackEv3[1].second, 1.0, 1.0); // R in both x and y
            backCircle1_Ev3->SetLineColor(kRed);
            backCircle1_Ev3->SetLineWidth(2);
            backCircle1_Ev3->SetFillStyle(0); // no fill
            backCircle1_Ev3->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle1_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle2_Ev3 = new TEllipse(jFexAdditionalSRJPositionsBackEv3[0].first, jFexAdditionalSRJPositionsBackEv3[0].second, 1.0, 1.0); // R in both x and y
            backCircle2_Ev3->SetLineColor(kAzure+2);
            backCircle2_Ev3->SetLineWidth(2);
            backCircle2_Ev3->SetFillStyle(0); // no fill
            backCircle2_Ev3->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle2_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle3_Ev3 = new TEllipse(jFexAdditionalSRJPositionsBackEv3[1].first, jFexAdditionalSRJPositionsBackEv3[1].second, 1.0, 1.0); // R in both x and y
            backCircle3_Ev3->SetLineColor(kAzure+2);
            backCircle3_Ev3->SetLineWidth(2);
            backCircle3_Ev3->SetFillStyle(0); // no fill
            backCircle3_Ev3->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle3_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle4_Ev3 = new TEllipse(jFexAdditionalSRJPositionsBackEv3[2].first, jFexAdditionalSRJPositionsBackEv3[2].second, 1.0, 1.0); // R in both x and y
            backCircle4_Ev3->SetLineColor(kAzure+2);
            backCircle4_Ev3->SetLineWidth(2);
            backCircle4_Ev3->SetFillStyle(0); // no fill
            backCircle4_Ev3->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle4_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle5_Ev3 = new TEllipse(jFexAdditionalSRJPositionsBackEv3[3].first, jFexAdditionalSRJPositionsBackEv3[3].second, 1.0, 1.0); // R in both x and y
            backCircle5_Ev3->SetLineColor(kAzure+2);
            backCircle5_Ev3->SetLineWidth(2);
            backCircle5_Ev3->SetFillStyle(0); // no fill
            backCircle5_Ev3->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle5_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle6_Ev3 = new TEllipse(newSeedPositionsBackEv3[0].first, newSeedPositionsBackEv3[0].second, 1.0, 1.0); // R in both x and y
            backCircle6_Ev3->SetLineColor(kGreen+2);
            backCircle6_Ev3->SetLineWidth(2);
            backCircle6_Ev3->SetFillStyle(0); // no fill
            backCircle6_Ev3->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle6_Ev3->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle7_Ev3 = new TEllipse(newSeedPositionsBackEv3[1].first, newSeedPositionsBackEv3[1].second, 1.0, 1.0); // R in both x and y
            backCircle7_Ev3->SetLineColor(kGreen+2);
            backCircle7_Ev3->SetLineWidth(2);
            backCircle7_Ev3->SetFillStyle(0); // no fill
            backCircle7_Ev3->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle7_Ev3->Draw("same");    // overlay on the existing plot

            // Build the label text (1st LRJ)
            TString backLabel1Ev3 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsBackEv3[0].first, newSeedPositionsBackEv3[0].second, backLRJEtEv3[0]);

            // Option 1: Use TLatex directly
            TLatex backLat1Ev3;
            backLat1Ev3.SetTextSize(0.025);     // Adjust size
            backLat1Ev3.SetTextColor(kBlack); // Match backCircle color
            backLat1Ev3.DrawLatex(newSeedPositionsBackEv3[0].first, newSeedPositionsBackEv3[0].second + 0.6, backLabel1Ev3); // Slightly above the backCircle

            // Build the label text (2nd LRJ)
            TString backLabel2Ev3 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsBackEv3[1].first, newSeedPositionsBackEv3[1].second, backLRJEtEv3[1]);

            // Option 1: Use TLatex directly
            TLatex backLat2Ev3;
            backLat2Ev3.SetTextSize(0.025);     // Adjust size
            backLat2Ev3.SetTextColor(kBlack); // Match backCircle color
            backLat2Ev3.DrawLatex(newSeedPositionsBackEv3[1].first, newSeedPositionsBackEv3[1].second + 0.6, backLabel2Ev3); // Slightly above the backCircle

            c.SaveAs(modifiedOutputFileDir + "backTopo422_highest128Et_EtaPhi_Ev3.pdf");   
            
            
            jFexSeedPositionsBackEv3.clear();
            jFexAdditionalSRJPositionsBackEv3.clear();
            newSeedPositionsBackEv3.clear();
            backLRJEtEv3.clear();
        }

        

        if(displayEv4Back_){
            backTopo422Highest128SeedPositionsEv4->GetZaxis()->SetTitle("E_{T} [GeV]");
            backTopo422Highest128SeedPositionsEv4->Draw("COLZ");


            TEllipse *backCircle0_Ev4 = new TEllipse(jFexSeedPositionsBackEv4[0].first, jFexSeedPositionsBackEv4[0].second, 1.0, 1.0); // R in both x and y
            backCircle0_Ev4->SetLineColor(kRed);
            backCircle0_Ev4->SetLineWidth(2);
            backCircle0_Ev4->SetFillStyle(0); // no fill
            backCircle0_Ev4->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle0_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle1_Ev4 = new TEllipse(jFexSeedPositionsBackEv4[1].first, jFexSeedPositionsBackEv4[1].second, 1.0, 1.0); // R in both x and y
            backCircle1_Ev4->SetLineColor(kRed);
            backCircle1_Ev4->SetLineWidth(2);
            backCircle1_Ev4->SetFillStyle(0); // no fill
            backCircle1_Ev4->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle1_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle2_Ev4 = new TEllipse(jFexAdditionalSRJPositionsBackEv4[0].first, jFexAdditionalSRJPositionsBackEv4[0].second, 1.0, 1.0); // R in both x and y
            backCircle2_Ev4->SetLineColor(kAzure+2);
            backCircle2_Ev4->SetLineWidth(2);
            backCircle2_Ev4->SetFillStyle(0); // no fill
            backCircle2_Ev4->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle2_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle3_Ev4 = new TEllipse(jFexAdditionalSRJPositionsBackEv4[1].first, jFexAdditionalSRJPositionsBackEv4[1].second, 1.0, 1.0); // R in both x and y
            backCircle3_Ev4->SetLineColor(kAzure+2);
            backCircle3_Ev4->SetLineWidth(2);
            backCircle3_Ev4->SetFillStyle(0); // no fill
            backCircle3_Ev4->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle3_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle4_Ev4 = new TEllipse(jFexAdditionalSRJPositionsBackEv4[2].first, jFexAdditionalSRJPositionsBackEv4[2].second, 1.0, 1.0); // R in both x and y
            backCircle4_Ev4->SetLineColor(kAzure+2);
            backCircle4_Ev4->SetLineWidth(2);
            backCircle4_Ev4->SetFillStyle(0); // no fill
            backCircle4_Ev4->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle4_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle5_Ev4 = new TEllipse(jFexAdditionalSRJPositionsBackEv4[3].first, jFexAdditionalSRJPositionsBackEv4[3].second, 1.0, 1.0); // R in both x and y
            backCircle5_Ev4->SetLineColor(kAzure+2);
            backCircle5_Ev4->SetLineWidth(2);
            backCircle5_Ev4->SetFillStyle(0); // no fill
            backCircle5_Ev4->SetLineStyle(2);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle5_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle6_Ev4 = new TEllipse(newSeedPositionsBackEv4[0].first, newSeedPositionsBackEv4[0].second, 1.0, 1.0); // R in both x and y
            backCircle6_Ev4->SetLineColor(kGreen+2);
            backCircle6_Ev4->SetLineWidth(2);
            backCircle6_Ev4->SetFillStyle(0); // no fill
            backCircle6_Ev4->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle6_Ev4->Draw("same");    // overlay on the existing plot

            TEllipse *backCircle7_Ev4 = new TEllipse(newSeedPositionsBackEv4[1].first, newSeedPositionsBackEv4[1].second, 1.0, 1.0); // R in both x and y
            backCircle7_Ev4->SetLineColor(kGreen+2);
            backCircle7_Ev4->SetLineWidth(2);
            backCircle7_Ev4->SetFillStyle(0); // no fill
            backCircle7_Ev4->SetLineStyle(1);  // dashed (1=solid, 2=dashed, 3=dotted, etc.)
            backCircle7_Ev4->Draw("same");    // overlay on the existing plot

            // Build the label text (1st LRJ)
            TString backLabel1Ev4 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsBackEv4[0].first, newSeedPositionsBackEv4[0].second, backLRJEtEv4[0]);

            // Option 1: Use TLatex directly
            TLatex backLat1Ev4;
            backLat1Ev4.SetTextSize(0.025);     // Adjust size
            backLat1Ev4.SetTextColor(kBlack); // Match backCircle color
            backLat1Ev4.DrawLatex(newSeedPositionsBackEv4[0].first, newSeedPositionsBackEv4[0].second + 0.6, backLabel1Ev4); // Slightly above the backCircle

            // Build the label text (2nd LRJ)
            TString backLabel2Ev4 = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", newSeedPositionsBackEv4[1].first, newSeedPositionsBackEv4[1].second, backLRJEtEv4[1]);

            // Option 1: Use TLatex directly
            TLatex backLat2Ev4;
            backLat2Ev4.SetTextSize(0.025);     // Adjust size
            backLat2Ev4.SetTextColor(kBlack); // Match backCircle color
            backLat2Ev4.DrawLatex(newSeedPositionsBackEv4[1].first, newSeedPositionsBackEv4[1].second + 0.6, backLabel2Ev4); // Slightly above the backCircle

            c.SaveAs(modifiedOutputFileDir + "backTopo422_highest128Et_EtaPhi_Ev4.pdf");   
            
            jFexSeedPositionsBackEv4.clear();
            jFexAdditionalSRJPositionsBackEv4.clear();
            newSeedPositionsBackEv4.clear();
            backLRJEtEv4.clear();
        }

        cSigEffLRJ.cd();

        // eff. vs. offline leading lrj Et overlay
        sig_eff_offlineLRJ50->SetLineColor(colors[0]);
        sig_eff_offlineLRJ50->SetMarkerColor(colors[0]);
        sigLegEffLRJ->AddEntry(sig_eff_offlineLRJ50, "Lead. LRJ E_{T} > 50 GeV", "l");
        sig_eff_offlineLRJ100->SetLineColor(colors[1]);
        sig_eff_offlineLRJ100->SetMarkerColor(colors[1]);
        sigLegEffLRJ->AddEntry(sig_eff_offlineLRJ100, "Lead. LRJ E_{T} > 100 GeV", "l");
        sig_eff_offlineLRJ150->SetLineColor(colors[2]);
        sig_eff_offlineLRJ150->SetMarkerColor(colors[2]);
        sigLegEffLRJ->AddEntry(sig_eff_offlineLRJ150, "Lead. LRJ E_{T} > 150 GeV", "l");
        sig_eff_offlineLRJ200->SetLineColor(colors[3]);
        sig_eff_offlineLRJ200->SetMarkerColor(colors[3]);
        sigLegEffLRJ->AddEntry(sig_eff_offlineLRJ200, "Lead. LRJ E_{T} > 200 GeV", "l");
        sig_eff_offlineLRJ250->SetLineColor(colors[4]);
        sig_eff_offlineLRJ250->SetMarkerColor(colors[4]);
        sigLegEffLRJ->AddEntry(sig_eff_offlineLRJ250, "Lead. LRJ E_{T} > 250 GeV", "l");
        sig_eff_offlineLRJ300->SetLineColor(colors[5]);
        sig_eff_offlineLRJ300->SetMarkerColor(colors[5]);
        sigLegEffLRJ->AddEntry(sig_eff_offlineLRJ300, "Lead. LRJ E_{T} > 300 GeV", "l");
        sig_eff_offlineLRJ350->SetLineColor(colors[6]);
        sig_eff_offlineLRJ350->SetMarkerColor(colors[6]);
        sigLegEffLRJ->AddEntry(sig_eff_offlineLRJ350, "Lead. LRJ E_{T} > 350 GeV", "l");
        sig_eff_offlineLRJ400->SetLineColor(colors[7]);
        sig_eff_offlineLRJ400->SetMarkerColor(colors[7]);
        sigLegEffLRJ->AddEntry(sig_eff_offlineLRJ400, "Lead. LRJ E_{T} > 400 GeV", "l");
        sig_eff_offlineLRJ450->SetLineColor(colors[8]);
        sig_eff_offlineLRJ450->SetMarkerColor(colors[8]);
        sigLegEffLRJ->AddEntry(sig_eff_offlineLRJ450, "Lead. LRJ E_{T} > 450 GeV", "l");
        sig_eff_offlineLRJ500->SetLineColor(colors[9]);
        sig_eff_offlineLRJ500->SetMarkerColor(colors[9]);
        sigLegEffLRJ->AddEntry(sig_eff_offlineLRJ500, "Lead. LRJ E_{T} > 500 GeV", "l");
        sig_eff_offlineLRJ50->Draw("P");
        sig_eff_offlineLRJ100->Draw("P SAME");
        sig_eff_offlineLRJ150->Draw("P SAME");
        sig_eff_offlineLRJ200->Draw("P SAME");
        sig_eff_offlineLRJ250->Draw("P SAME");
        sig_eff_offlineLRJ300->Draw("P SAME");
        sig_eff_offlineLRJ350->Draw("P SAME");
        sig_eff_offlineLRJ400->Draw("P SAME");
        sig_eff_offlineLRJ450->Draw("P SAME");
        sig_eff_offlineLRJ500->Draw("P SAME");
        sigLegEffLRJ->Draw();
        cSigEffLRJ.SaveAs(modifiedOutputFileDir + "sig_overlayed_eff_LRJ.pdf");


        cSigEffLRJ_Dijet.cd();

        // eff. vs. offline leading lrj Et overlay
        sig_eff_offlineLRJ50_Dijet->SetLineColor(colors[0]);
        sig_eff_offlineLRJ50_Dijet->SetMarkerColor(colors[0]);
        sigLegEffLRJ_Dijet->AddEntry(sig_eff_offlineLRJ50_Dijet, "Sublead. LRJ E_{T} > 50 GeV", "l");
        sig_eff_offlineLRJ100_Dijet->SetLineColor(colors[1]);
        sig_eff_offlineLRJ100_Dijet->SetMarkerColor(colors[1]);
        sigLegEffLRJ_Dijet->AddEntry(sig_eff_offlineLRJ100_Dijet, "Sublead. LRJ E_{T} > 100 GeV", "l");
        sig_eff_offlineLRJ150_Dijet->SetLineColor(colors[2]);
        sig_eff_offlineLRJ150_Dijet->SetMarkerColor(colors[2]);
        sigLegEffLRJ_Dijet->AddEntry(sig_eff_offlineLRJ150_Dijet, "Sublead. LRJ E_{T} > 150 GeV", "l");
        sig_eff_offlineLRJ200_Dijet->SetLineColor(colors[3]);
        sig_eff_offlineLRJ200_Dijet->SetMarkerColor(colors[3]);
        sigLegEffLRJ_Dijet->AddEntry(sig_eff_offlineLRJ200_Dijet, "Sublead. LRJ E_{T} > 200 GeV", "l");
        sig_eff_offlineLRJ250_Dijet->SetLineColor(colors[4]);
        sig_eff_offlineLRJ250_Dijet->SetMarkerColor(colors[4]);
        sigLegEffLRJ_Dijet->AddEntry(sig_eff_offlineLRJ250_Dijet, "Sublead. LRJ E_{T} > 250 GeV", "l");
        sig_eff_offlineLRJ300_Dijet->SetLineColor(colors[5]);
        sig_eff_offlineLRJ300_Dijet->SetMarkerColor(colors[5]);
        sigLegEffLRJ_Dijet->AddEntry(sig_eff_offlineLRJ300_Dijet, "Sublead. LRJ E_{T} > 300 GeV", "l");
        sig_eff_offlineLRJ350_Dijet->SetLineColor(colors[6]);
        sig_eff_offlineLRJ350_Dijet->SetMarkerColor(colors[6]);
        sigLegEffLRJ_Dijet->AddEntry(sig_eff_offlineLRJ350_Dijet, "Sublead. LRJ E_{T} > 350 GeV", "l");
        sig_eff_offlineLRJ400_Dijet->SetLineColor(colors[7]);
        sig_eff_offlineLRJ400_Dijet->SetMarkerColor(colors[7]);
        sigLegEffLRJ_Dijet->AddEntry(sig_eff_offlineLRJ400_Dijet, "Sublead. LRJ E_{T} > 400 GeV", "l");
        sig_eff_offlineLRJ450_Dijet->SetLineColor(colors[8]);
        sig_eff_offlineLRJ450_Dijet->SetMarkerColor(colors[8]);
        sigLegEffLRJ_Dijet->AddEntry(sig_eff_offlineLRJ450_Dijet, "Sublead. LRJ E_{T} > 450 GeV", "l");
        sig_eff_offlineLRJ500_Dijet->SetLineColor(colors[9]);
        sig_eff_offlineLRJ500_Dijet->SetMarkerColor(colors[9]);
        sigLegEffLRJ_Dijet->AddEntry(sig_eff_offlineLRJ500_Dijet, "Sublead. LRJ E_{T} > 500 GeV", "l");
        sig_eff_offlineLRJ50_Dijet->Draw("P");
        sig_eff_offlineLRJ100_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ150_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ200_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ250_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ300_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ350_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ400_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ450_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ500_Dijet->Draw("P SAME");
        sigLegEffLRJ_Dijet->Draw();
        cSigEffLRJ_Dijet.SaveAs(modifiedOutputFileDir + "sig_overlayed_eff_LRJ_Dijet.pdf");


        cSigEffLRJ_1Subjet.cd();

        // eff. vs. offline leading lrj Et overlay
        sig_eff_offlineLRJ50_1Subjet->SetLineColor(colors[0]);
        sig_eff_offlineLRJ50_1Subjet->SetMarkerColor(colors[0]);
        sigLegEffLRJ_1Subjet->AddEntry(sig_eff_offlineLRJ50_1Subjet, "Lead. LRJ E_{T} > 50 GeV [1 Subjet]", "l");
        sig_eff_offlineLRJ100_1Subjet->SetLineColor(colors[1]);
        sig_eff_offlineLRJ100_1Subjet->SetMarkerColor(colors[1]);
        sigLegEffLRJ_1Subjet->AddEntry(sig_eff_offlineLRJ100_1Subjet, "Lead. LRJ E_{T} > 100 GeV [1 Subjet]", "l");
        sig_eff_offlineLRJ150_1Subjet->SetLineColor(colors[2]);
        sig_eff_offlineLRJ150_1Subjet->SetMarkerColor(colors[2]);
        sigLegEffLRJ_1Subjet->AddEntry(sig_eff_offlineLRJ150_1Subjet, "Lead. LRJ E_{T} > 150 GeV [1 Subjet]", "l");
        sig_eff_offlineLRJ200_1Subjet->SetLineColor(colors[3]);
        sig_eff_offlineLRJ200_1Subjet->SetMarkerColor(colors[3]);
        sigLegEffLRJ_1Subjet->AddEntry(sig_eff_offlineLRJ200_1Subjet, "Lead. LRJ E_{T} > 200 GeV [1 Subjet]", "l");
        sig_eff_offlineLRJ250_1Subjet->SetLineColor(colors[4]);
        sig_eff_offlineLRJ250_1Subjet->SetMarkerColor(colors[4]);
        sigLegEffLRJ_1Subjet->AddEntry(sig_eff_offlineLRJ250_1Subjet, "Lead. LRJ E_{T} > 250 GeV [1 Subjet]", "l");
        sig_eff_offlineLRJ300_1Subjet->SetLineColor(colors[5]);
        sig_eff_offlineLRJ300_1Subjet->SetMarkerColor(colors[5]);
        sigLegEffLRJ_1Subjet->AddEntry(sig_eff_offlineLRJ300_1Subjet, "Lead. LRJ E_{T} > 300 GeV [1 Subjet]", "l");
        sig_eff_offlineLRJ350_1Subjet->SetLineColor(colors[6]);
        sig_eff_offlineLRJ350_1Subjet->SetMarkerColor(colors[6]);
        sigLegEffLRJ_1Subjet->AddEntry(sig_eff_offlineLRJ350_1Subjet, "Lead. LRJ E_{T} > 350 GeV [1 Subjet]", "l");
        sig_eff_offlineLRJ400_1Subjet->SetLineColor(colors[7]);
        sig_eff_offlineLRJ400_1Subjet->SetMarkerColor(colors[7]);
        sigLegEffLRJ_1Subjet->AddEntry(sig_eff_offlineLRJ400_1Subjet, "Lead. LRJ E_{T} > 400 GeV [1 Subjet]", "l");
        sig_eff_offlineLRJ450_1Subjet->SetLineColor(colors[8]);
        sig_eff_offlineLRJ450_1Subjet->SetMarkerColor(colors[8]);
        sigLegEffLRJ_1Subjet->AddEntry(sig_eff_offlineLRJ450_1Subjet, "Lead. LRJ E_{T} > 450 GeV [1 Subjet]", "l");
        sig_eff_offlineLRJ500_1Subjet->SetLineColor(colors[9]);
        sig_eff_offlineLRJ500_1Subjet->SetMarkerColor(colors[9]);
        sigLegEffLRJ_1Subjet->AddEntry(sig_eff_offlineLRJ500_1Subjet, "Lead. LRJ E_{T} > 500 GeV [1 Subjet]", "l");
        sig_eff_offlineLRJ50_1Subjet->Draw("P");
        sig_eff_offlineLRJ100_1Subjet->Draw("P SAME");
        sig_eff_offlineLRJ150_1Subjet->Draw("P SAME");
        sig_eff_offlineLRJ200_1Subjet->Draw("P SAME");
        sig_eff_offlineLRJ250_1Subjet->Draw("P SAME");
        sig_eff_offlineLRJ300_1Subjet->Draw("P SAME");
        sig_eff_offlineLRJ350_1Subjet->Draw("P SAME");
        sig_eff_offlineLRJ400_1Subjet->Draw("P SAME");
        sig_eff_offlineLRJ450_1Subjet->Draw("P SAME");
        sig_eff_offlineLRJ500_1Subjet->Draw("P SAME");
        sigLegEffLRJ_1Subjet->Draw();
        cSigEffLRJ_1Subjet.SaveAs(modifiedOutputFileDir + "sig_overlayed_eff_LRJ_1Subjet.pdf");

        cBackEffLRJ_1Subjet.cd();

        // eff. vs. offline leading lrj Et overlay
        back_eff_offlineLRJ100_1Subjet->SetLineColor(colors[1]);
        back_eff_offlineLRJ100_1Subjet->SetMarkerColor(colors[1]);
        backLegEffLRJ_1Subjet->AddEntry(back_eff_offlineLRJ100_1Subjet, "Lead. LRJ E_{T} > 100 GeV [1 Subjet]", "l");
        back_eff_offlineLRJ200_1Subjet->SetLineColor(colors[3]);
        back_eff_offlineLRJ200_1Subjet->SetMarkerColor(colors[3]);
        backLegEffLRJ_1Subjet->AddEntry(back_eff_offlineLRJ200_1Subjet, "Lead. LRJ E_{T} > 200 GeV [1 Subjet]", "l");
        back_eff_offlineLRJ300_1Subjet->SetLineColor(colors[5]);
        back_eff_offlineLRJ300_1Subjet->SetMarkerColor(colors[5]);
        backLegEffLRJ_1Subjet->AddEntry(back_eff_offlineLRJ300_1Subjet, "Lead. LRJ E_{T} > 300 GeV [1 Subjet]", "l");
        back_eff_offlineLRJ100_1Subjet->Draw("P");
        back_eff_offlineLRJ200_1Subjet->Draw("P SAME");
        back_eff_offlineLRJ300_1Subjet->Draw("P SAME");
        backLegEffLRJ_1Subjet->Draw();
        cBackEffLRJ_1Subjet.SaveAs(modifiedOutputFileDir + "back_overlayed_eff_LRJ_1Subjet.pdf");


        cSigEffLRJ_GrEq2Subjets.cd();

        // eff. vs. offline leading lrj Et overlay
        sig_eff_offlineLRJ50_GrEq2Subjets->SetLineColor(colors[0]);
        sig_eff_offlineLRJ50_GrEq2Subjets->SetMarkerColor(colors[0]);
        sigLegEffLRJ_GrEq2Subjets->AddEntry(sig_eff_offlineLRJ50_GrEq2Subjets, "Lead. LRJ E_{T} > 50 GeV [#geq 2 Subjet]", "l");
        sig_eff_offlineLRJ100_GrEq2Subjets->SetLineColor(colors[1]);
        sig_eff_offlineLRJ100_GrEq2Subjets->SetMarkerColor(colors[1]);
        sigLegEffLRJ_GrEq2Subjets->AddEntry(sig_eff_offlineLRJ100_GrEq2Subjets, "Lead. LRJ E_{T} > 100 GeV [#geq 2 Subjet]", "l");
        sig_eff_offlineLRJ150_GrEq2Subjets->SetLineColor(colors[2]);
        sig_eff_offlineLRJ150_GrEq2Subjets->SetMarkerColor(colors[2]);
        sigLegEffLRJ_GrEq2Subjets->AddEntry(sig_eff_offlineLRJ150_GrEq2Subjets, "Lead. LRJ E_{T} > 150 GeV [#geq 2 Subjet]", "l");
        sig_eff_offlineLRJ200_GrEq2Subjets->SetLineColor(colors[3]);
        sig_eff_offlineLRJ200_GrEq2Subjets->SetMarkerColor(colors[3]);
        sigLegEffLRJ_GrEq2Subjets->AddEntry(sig_eff_offlineLRJ200_GrEq2Subjets, "Lead. LRJ E_{T} > 200 GeV [#geq 2 Subjet]", "l");
        sig_eff_offlineLRJ250_GrEq2Subjets->SetLineColor(colors[4]);
        sig_eff_offlineLRJ250_GrEq2Subjets->SetMarkerColor(colors[4]);
        sigLegEffLRJ_GrEq2Subjets->AddEntry(sig_eff_offlineLRJ250_GrEq2Subjets, "Lead. LRJ E_{T} > 250 GeV [#geq 2 Subjet]", "l");
        sig_eff_offlineLRJ300_GrEq2Subjets->SetLineColor(colors[5]);
        sig_eff_offlineLRJ300_GrEq2Subjets->SetMarkerColor(colors[5]);
        sigLegEffLRJ_GrEq2Subjets->AddEntry(sig_eff_offlineLRJ300_GrEq2Subjets, "Lead. LRJ E_{T} > 300 GeV [#geq 2 Subjet]", "l");
        sig_eff_offlineLRJ350_GrEq2Subjets->SetLineColor(colors[6]);
        sig_eff_offlineLRJ350_GrEq2Subjets->SetMarkerColor(colors[6]);
        sigLegEffLRJ_GrEq2Subjets->AddEntry(sig_eff_offlineLRJ350_GrEq2Subjets, "Lead. LRJ E_{T} > 350 GeV [#geq 2 Subjet]", "l");
        sig_eff_offlineLRJ400_GrEq2Subjets->SetLineColor(colors[7]);
        sig_eff_offlineLRJ400_GrEq2Subjets->SetMarkerColor(colors[7]);
        sigLegEffLRJ_GrEq2Subjets->AddEntry(sig_eff_offlineLRJ400_GrEq2Subjets, "Lead. LRJ E_{T} > 400 GeV [#geq 2 Subjet]", "l");
        sig_eff_offlineLRJ450_GrEq2Subjets->SetLineColor(colors[8]);
        sig_eff_offlineLRJ450_GrEq2Subjets->SetMarkerColor(colors[8]);
        sigLegEffLRJ_GrEq2Subjets->AddEntry(sig_eff_offlineLRJ450_GrEq2Subjets, "Lead. LRJ E_{T} > 450 GeV [#geq 2 Subjet]", "l");
        sig_eff_offlineLRJ500_GrEq2Subjets->SetLineColor(colors[9]);
        sig_eff_offlineLRJ500_GrEq2Subjets->SetMarkerColor(colors[9]);
        sigLegEffLRJ_GrEq2Subjets->AddEntry(sig_eff_offlineLRJ500_GrEq2Subjets, "Lead. LRJ E_{T} > 500 GeV [#geq 2 Subjets]", "l");
        sig_eff_offlineLRJ50_GrEq2Subjets->Draw("P");
        sig_eff_offlineLRJ100_GrEq2Subjets->Draw("P SAME");
        sig_eff_offlineLRJ150_GrEq2Subjets->Draw("P SAME");
        sig_eff_offlineLRJ200_GrEq2Subjets->Draw("P SAME");
        sig_eff_offlineLRJ250_GrEq2Subjets->Draw("P SAME");
        sig_eff_offlineLRJ300_GrEq2Subjets->Draw("P SAME");
        sig_eff_offlineLRJ350_GrEq2Subjets->Draw("P SAME");
        sig_eff_offlineLRJ400_GrEq2Subjets->Draw("P SAME");
        sig_eff_offlineLRJ450_GrEq2Subjets->Draw("P SAME");
        sig_eff_offlineLRJ500_GrEq2Subjets->Draw("P SAME");
        sigLegEffLRJ_GrEq2Subjets->Draw();
        cSigEffLRJ_GrEq2Subjets.SaveAs(modifiedOutputFileDir + "sig_overlayed_eff_LRJ_GrEq2Subjets.pdf");

        cBackEffLRJ_GrEq2Subjets.cd();

        // eff. vs. offline leading lrj Et overlay
        back_eff_offlineLRJ100_GrEq2Subjets->SetLineColor(colors[1]);
        back_eff_offlineLRJ100_GrEq2Subjets->SetMarkerColor(colors[1]);
        backLegEffLRJ_GrEq2Subjets->AddEntry(back_eff_offlineLRJ100_GrEq2Subjets, "Lead. LRJ E_{T} > 100 GeV [#geq 2 Subjet]", "l");
        back_eff_offlineLRJ200_GrEq2Subjets->SetLineColor(colors[3]);
        back_eff_offlineLRJ200_GrEq2Subjets->SetMarkerColor(colors[3]);
        backLegEffLRJ_GrEq2Subjets->AddEntry(back_eff_offlineLRJ200_GrEq2Subjets, "Lead. LRJ E_{T} > 200 GeV [#geq 2 Subjet]", "l");
        back_eff_offlineLRJ300_GrEq2Subjets->SetLineColor(colors[5]);
        back_eff_offlineLRJ300_GrEq2Subjets->SetMarkerColor(colors[5]);
        backLegEffLRJ_GrEq2Subjets->AddEntry(back_eff_offlineLRJ300_GrEq2Subjets, "Lead. LRJ E_{T} > 300 GeV [#geq 2 Subjet]", "l");
        back_eff_offlineLRJ100_GrEq2Subjets->Draw("P");
        back_eff_offlineLRJ200_GrEq2Subjets->Draw("P SAME");
        back_eff_offlineLRJ300_GrEq2Subjets->Draw("P SAME");
        backLegEffLRJ_GrEq2Subjets->Draw();
        cBackEffLRJ_GrEq2Subjets.SaveAs(modifiedOutputFileDir + "back_overlayed_eff_LRJ_GrEq2Subjets.pdf");


        cSigEffLRJ_Mass100to150.cd();

        // eff. vs. offline leading lrj Et overlay
        sig_eff_offlineLRJ50_mass100to150->SetLineColor(colors[0]);
        sig_eff_offlineLRJ50_mass100to150->SetMarkerColor(colors[0]);
        sigLegEffLRJ_mass100to150->AddEntry(sig_eff_offlineLRJ50_mass100to150, "Lead. LRJ E_{T} > 50 GeV", "l");
        sig_eff_offlineLRJ100_mass100to150->SetLineColor(colors[1]);
        sig_eff_offlineLRJ100_mass100to150->SetMarkerColor(colors[1]);
        sigLegEffLRJ_mass100to150->AddEntry(sig_eff_offlineLRJ100_mass100to150, "Lead. LRJ E_{T} > 100 GeV", "l");
        sig_eff_offlineLRJ150_mass100to150->SetLineColor(colors[2]);
        sig_eff_offlineLRJ150_mass100to150->SetMarkerColor(colors[2]);
        sigLegEffLRJ_mass100to150->AddEntry(sig_eff_offlineLRJ150_mass100to150, "Lead. LRJ E_{T} > 150 GeV", "l");
        sig_eff_offlineLRJ200_mass100to150->SetLineColor(colors[3]);
        sig_eff_offlineLRJ200_mass100to150->SetMarkerColor(colors[3]);
        sigLegEffLRJ_mass100to150->AddEntry(sig_eff_offlineLRJ200_mass100to150, "Lead. LRJ E_{T} > 200 GeV", "l");
        sig_eff_offlineLRJ250_mass100to150->SetLineColor(colors[4]);
        sig_eff_offlineLRJ250_mass100to150->SetMarkerColor(colors[4]);
        sigLegEffLRJ_mass100to150->AddEntry(sig_eff_offlineLRJ250_mass100to150, "Lead. LRJ E_{T} > 250 GeV", "l");
        sig_eff_offlineLRJ300_mass100to150->SetLineColor(colors[5]);
        sig_eff_offlineLRJ300_mass100to150->SetMarkerColor(colors[5]);
        sigLegEffLRJ_mass100to150->AddEntry(sig_eff_offlineLRJ300_mass100to150, "Lead. LRJ E_{T} > 300 GeV", "l");
        sig_eff_offlineLRJ350_mass100to150->SetLineColor(colors[6]);
        sig_eff_offlineLRJ350_mass100to150->SetMarkerColor(colors[6]);
        sigLegEffLRJ_mass100to150->AddEntry(sig_eff_offlineLRJ350_mass100to150, "Lead. LRJ E_{T} > 350 GeV", "l");
        sig_eff_offlineLRJ400_mass100to150->SetLineColor(colors[7]);
        sig_eff_offlineLRJ400_mass100to150->SetMarkerColor(colors[7]);
        sigLegEffLRJ_mass100to150->AddEntry(sig_eff_offlineLRJ400_mass100to150, "Lead. LRJ E_{T} > 400 GeV", "l");
        sig_eff_offlineLRJ450_mass100to150->SetLineColor(colors[8]);
        sig_eff_offlineLRJ450_mass100to150->SetMarkerColor(colors[8]);
        sigLegEffLRJ_mass100to150->AddEntry(sig_eff_offlineLRJ450_mass100to150, "Lead. LRJ E_{T} > 450 GeV", "l");
        sig_eff_offlineLRJ500_mass100to150->SetLineColor(colors[9]);
        sig_eff_offlineLRJ500_mass100to150->SetMarkerColor(colors[9]);
        sigLegEffLRJ_mass100to150->AddEntry(sig_eff_offlineLRJ500_mass100to150, "Lead. LRJ E_{T} > 500 GeV", "l");
        sig_eff_offlineLRJ50_mass100to150->Draw("P");
        sig_eff_offlineLRJ100_mass100to150->Draw("P SAME");
        sig_eff_offlineLRJ150_mass100to150->Draw("P SAME");
        sig_eff_offlineLRJ200_mass100to150->Draw("P SAME");
        sig_eff_offlineLRJ250_mass100to150->Draw("P SAME");
        sig_eff_offlineLRJ300_mass100to150->Draw("P SAME");
        sig_eff_offlineLRJ350_mass100to150->Draw("P SAME");
        sig_eff_offlineLRJ400_mass100to150->Draw("P SAME");
        sig_eff_offlineLRJ450_mass100to150->Draw("P SAME");
        sig_eff_offlineLRJ500_mass100to150->Draw("P SAME");
        sigLegEffLRJ_mass100to150->Draw();
        cSigEffLRJ_Mass100to150.SaveAs(modifiedOutputFileDir + "sig_overlayed_eff_LRJ_mass100to150.pdf");


        cBackEffLRJ.cd();

        back_eff_offlineLRJ50->SetLineColor(colors[0]);
        back_eff_offlineLRJ50->SetMarkerColor(colors[0]);
        backLegEffLRJ->AddEntry(back_eff_offlineLRJ50, "Lead. LRJ E_{T} > 50 GeV", "l");
        back_eff_offlineLRJ100->SetLineColor(colors[1]);
        back_eff_offlineLRJ100->SetMarkerColor(colors[1]);
        backLegEffLRJ->AddEntry(back_eff_offlineLRJ100, "Lead. LRJ E_{T} > 100 GeV", "l");
        back_eff_offlineLRJ150->SetLineColor(colors[2]);
        back_eff_offlineLRJ150->SetMarkerColor(colors[2]);
        backLegEffLRJ->AddEntry(back_eff_offlineLRJ150, "Lead. LRJ E_{T} > 150 GeV", "l");
        back_eff_offlineLRJ200->SetLineColor(colors[3]);
        back_eff_offlineLRJ200->SetMarkerColor(colors[3]);
        backLegEffLRJ->AddEntry(back_eff_offlineLRJ200, "Lead. LRJ E_{T} > 200 GeV", "l");
        back_eff_offlineLRJ250->SetLineColor(colors[4]);
        back_eff_offlineLRJ250->SetMarkerColor(colors[4]);
        backLegEffLRJ->AddEntry(back_eff_offlineLRJ250, "Lead. LRJ E_{T} > 250 GeV", "l");
        back_eff_offlineLRJ300->SetLineColor(colors[5]);
        back_eff_offlineLRJ300->SetMarkerColor(colors[5]);
        backLegEffLRJ->AddEntry(back_eff_offlineLRJ300, "Lead. LRJ E_{T} > 300 GeV", "l");
        back_eff_offlineLRJ350->SetLineColor(colors[6]);
        back_eff_offlineLRJ350->SetMarkerColor(colors[6]);
        backLegEffLRJ->AddEntry(back_eff_offlineLRJ350, "Lead. LRJ E_{T} > 350 GeV", "l");
        back_eff_offlineLRJ400->SetLineColor(colors[7]);
        back_eff_offlineLRJ400->SetMarkerColor(colors[7]);
        backLegEffLRJ->AddEntry(back_eff_offlineLRJ400, "Lead. LRJ E_{T} > 400 GeV", "l");
        back_eff_offlineLRJ450->SetLineColor(colors[8]);
        back_eff_offlineLRJ450->SetMarkerColor(colors[8]);
        backLegEffLRJ->AddEntry(back_eff_offlineLRJ450, "Lead. LRJ E_{T} > 450 GeV", "l");
        back_eff_offlineLRJ50->Draw("P");
        back_eff_offlineLRJ100->Draw("P SAME");
        back_eff_offlineLRJ150->Draw("P SAME");
        back_eff_offlineLRJ200->Draw("P SAME");
        back_eff_offlineLRJ250->Draw("P SAME");
        back_eff_offlineLRJ300->Draw("P SAME");
        back_eff_offlineLRJ350->Draw("P SAME");
        back_eff_offlineLRJ400->Draw("P SAME");
        back_eff_offlineLRJ450->Draw("P SAME");
        backLegEffLRJ->Draw();
        cBackEffLRJ.SaveAs(modifiedOutputFileDir + "back_overlayed_eff_LRJ.pdf");


        cSigEffLRJ_gFex_Dijet.cd();
        // eff. vs. offline leading lrj Et overlay
        sig_eff_offlineLRJ50_gFexLRJ_Dijet->SetLineColor(colors[0]);
        sig_eff_offlineLRJ50_gFexLRJ_Dijet->SetMarkerColor(colors[0]);
        sigLegEffLRJ_gFex_Dijet->AddEntry(sig_eff_offlineLRJ50_gFexLRJ_Dijet, "Sublead. LRJ E_{T} > 50 GeV", "p");
        sig_eff_offlineLRJ100_gFexLRJ_Dijet->SetLineColor(colors[1]);
        sig_eff_offlineLRJ100_gFexLRJ_Dijet->SetMarkerColor(colors[1]);
        sigLegEffLRJ_gFex_Dijet->AddEntry(sig_eff_offlineLRJ100_gFexLRJ_Dijet, "Sublead. LRJ E_{T} > 100 GeV", "p");
        sig_eff_offlineLRJ150_gFexLRJ_Dijet->SetLineColor(colors[2]);
        sig_eff_offlineLRJ150_gFexLRJ_Dijet->SetMarkerColor(colors[2]);
        sigLegEffLRJ_gFex_Dijet->AddEntry(sig_eff_offlineLRJ150_gFexLRJ_Dijet, "Sublead. LRJ E_{T} > 150 GeV", "p");
        sig_eff_offlineLRJ200_gFexLRJ_Dijet->SetLineColor(colors[3]);
        sig_eff_offlineLRJ200_gFexLRJ_Dijet->SetMarkerColor(colors[3]);
        sigLegEffLRJ_gFex_Dijet->AddEntry(sig_eff_offlineLRJ200_gFexLRJ_Dijet, "Sublead. LRJ E_{T} > 200 GeV", "p");
        sig_eff_offlineLRJ250_gFexLRJ_Dijet->SetLineColor(colors[4]);
        sig_eff_offlineLRJ250_gFexLRJ_Dijet->SetMarkerColor(colors[4]);
        sigLegEffLRJ_gFex_Dijet->AddEntry(sig_eff_offlineLRJ250_gFexLRJ_Dijet, "Sublead. LRJ E_{T} > 250 GeV", "p");
        sig_eff_offlineLRJ300_gFexLRJ_Dijet->SetLineColor(colors[5]);
        sig_eff_offlineLRJ300_gFexLRJ_Dijet->SetMarkerColor(colors[5]);
        sigLegEffLRJ_gFex_Dijet->AddEntry(sig_eff_offlineLRJ300_gFexLRJ_Dijet, "Sublead. LRJ E_{T} > 300 GeV", "p");
        sig_eff_offlineLRJ350_gFexLRJ_Dijet->SetLineColor(colors[6]);
        sig_eff_offlineLRJ350_gFexLRJ_Dijet->SetMarkerColor(colors[6]);
        sigLegEffLRJ_gFex_Dijet->AddEntry(sig_eff_offlineLRJ350_gFexLRJ_Dijet, "Sublead. LRJ E_{T} > 350 GeV", "p");
        sig_eff_offlineLRJ400_gFexLRJ_Dijet->SetLineColor(colors[7]);
        sig_eff_offlineLRJ400_gFexLRJ_Dijet->SetMarkerColor(colors[7]);
        sigLegEffLRJ_gFex_Dijet->AddEntry(sig_eff_offlineLRJ400_gFexLRJ_Dijet, "Sublead. LRJ E_{T} > 400 GeV", "p");
        sig_eff_offlineLRJ450_gFexLRJ_Dijet->SetLineColor(colors[8]);
        sig_eff_offlineLRJ450_gFexLRJ_Dijet->SetMarkerColor(colors[8]);
        sigLegEffLRJ_gFex_Dijet->AddEntry(sig_eff_offlineLRJ450_gFexLRJ_Dijet, "Sublead. LRJ E_{T} > 450 GeV", "p");
        sig_eff_offlineLRJ500_gFexLRJ_Dijet->SetLineColor(colors[9]);
        sig_eff_offlineLRJ500_gFexLRJ_Dijet->SetMarkerColor(colors[9]);
        sigLegEffLRJ_gFex_Dijet->AddEntry(sig_eff_offlineLRJ500_gFexLRJ_Dijet, "Sublead. LRJ E_{T} > 500 GeV", "p");
        sig_eff_offlineLRJ50_gFexLRJ_Dijet->Draw("P");
        sig_eff_offlineLRJ100_gFexLRJ_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ150_gFexLRJ_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ200_gFexLRJ_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ250_gFexLRJ_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ300_gFexLRJ_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ350_gFexLRJ_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ400_gFexLRJ_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ450_gFexLRJ_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ500_gFexLRJ_Dijet->Draw("P SAME");
        sigLegEffLRJ_gFex_Dijet->Draw();
        cSigEffLRJ_gFex_Dijet.SaveAs(modifiedOutputFileDir + "sig_overlayed_eff_LRJ_gFexLRJ_Dijet.pdf");


        cSigEffLRJ_gFex.cd();
        // eff. vs. offline leading lrj Et overlay
        sig_eff_offlineLRJ50_gFexLRJ->SetLineColor(colors[0]);
        sig_eff_offlineLRJ50_gFexLRJ->SetMarkerColor(colors[0]);
        sigLegEffLRJ_gFex->AddEntry(sig_eff_offlineLRJ50_gFexLRJ, "Lead. LRJ E_{T} > 50 GeV", "l");
        sig_eff_offlineLRJ100_gFexLRJ->SetLineColor(colors[1]);
        sig_eff_offlineLRJ100_gFexLRJ->SetMarkerColor(colors[1]);
        sigLegEffLRJ_gFex->AddEntry(sig_eff_offlineLRJ100_gFexLRJ, "Lead. LRJ E_{T} > 100 GeV", "l");
        sig_eff_offlineLRJ150_gFexLRJ->SetLineColor(colors[2]);
        sig_eff_offlineLRJ150_gFexLRJ->SetMarkerColor(colors[2]);
        sigLegEffLRJ_gFex->AddEntry(sig_eff_offlineLRJ150_gFexLRJ, "Lead. LRJ E_{T} > 150 GeV", "l");
        sig_eff_offlineLRJ200_gFexLRJ->SetLineColor(colors[3]);
        sig_eff_offlineLRJ200_gFexLRJ->SetMarkerColor(colors[3]);
        sigLegEffLRJ_gFex->AddEntry(sig_eff_offlineLRJ200_gFexLRJ, "Lead. LRJ E_{T} > 200 GeV", "l");
        sig_eff_offlineLRJ250_gFexLRJ->SetLineColor(colors[4]);
        sig_eff_offlineLRJ250_gFexLRJ->SetMarkerColor(colors[4]);
        sigLegEffLRJ_gFex->AddEntry(sig_eff_offlineLRJ250_gFexLRJ, "Lead. LRJ E_{T} > 250 GeV", "l");
        sig_eff_offlineLRJ300_gFexLRJ->SetLineColor(colors[5]);
        sig_eff_offlineLRJ300_gFexLRJ->SetMarkerColor(colors[5]);
        sigLegEffLRJ_gFex->AddEntry(sig_eff_offlineLRJ300_gFexLRJ, "Lead. LRJ E_{T} > 300 GeV", "l");
        sig_eff_offlineLRJ350_gFexLRJ->SetLineColor(colors[6]);
        sig_eff_offlineLRJ350_gFexLRJ->SetMarkerColor(colors[6]);
        sigLegEffLRJ_gFex->AddEntry(sig_eff_offlineLRJ350_gFexLRJ, "Lead. LRJ E_{T} > 350 GeV", "l");
        sig_eff_offlineLRJ400_gFexLRJ->SetLineColor(colors[7]);
        sig_eff_offlineLRJ400_gFexLRJ->SetMarkerColor(colors[7]);
        sigLegEffLRJ_gFex->AddEntry(sig_eff_offlineLRJ400_gFexLRJ, "Lead. LRJ E_{T} > 400 GeV", "l");
        sig_eff_offlineLRJ450_gFexLRJ->SetLineColor(colors[8]);
        sig_eff_offlineLRJ450_gFexLRJ->SetMarkerColor(colors[8]);
        sigLegEffLRJ_gFex->AddEntry(sig_eff_offlineLRJ450_gFexLRJ, "Lead. LRJ E_{T} > 450 GeV", "l");
        sig_eff_offlineLRJ500_gFexLRJ->SetLineColor(colors[9]);
        sig_eff_offlineLRJ500_gFexLRJ->SetMarkerColor(colors[9]);
        sigLegEffLRJ_gFex->AddEntry(sig_eff_offlineLRJ500_gFexLRJ, "Lead. LRJ E_{T} > 500 GeV", "l");
        sig_eff_offlineLRJ50_gFexLRJ->Draw("P");
        sig_eff_offlineLRJ100_gFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ150_gFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ200_gFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ250_gFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ300_gFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ350_gFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ400_gFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ450_gFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ500_gFexLRJ->Draw("P SAME");
        sigLegEffLRJ_gFex->Draw();
        cSigEffLRJ_gFex.SaveAs(modifiedOutputFileDir + "sig_overlayed_eff_LRJ_gFexLRJ.pdf");

        cSigEffLRJ_jFex.cd();
        // eff. vs. offline leading lrj Et overlay
        sig_eff_offlineLRJ50_jFexLRJ->SetLineColor(colors[0]);
        sig_eff_offlineLRJ50_jFexLRJ->SetMarkerColor(colors[0]);
        sigLegEffLRJ_jFex->AddEntry(sig_eff_offlineLRJ50_jFexLRJ, "Lead. LRJ E_{T} > 50 GeV", "l");
        sig_eff_offlineLRJ100_jFexLRJ->SetLineColor(colors[1]);
        sig_eff_offlineLRJ100_jFexLRJ->SetMarkerColor(colors[1]);
        sigLegEffLRJ_jFex->AddEntry(sig_eff_offlineLRJ100_jFexLRJ, "Lead. LRJ E_{T} > 100 GeV", "l");
        sig_eff_offlineLRJ150_jFexLRJ->SetLineColor(colors[2]);
        sig_eff_offlineLRJ150_jFexLRJ->SetMarkerColor(colors[2]);
        sigLegEffLRJ_jFex->AddEntry(sig_eff_offlineLRJ150_jFexLRJ, "Lead. LRJ E_{T} > 150 GeV", "l");
        sig_eff_offlineLRJ200_jFexLRJ->SetLineColor(colors[3]);
        sig_eff_offlineLRJ200_jFexLRJ->SetMarkerColor(colors[3]);
        sigLegEffLRJ_jFex->AddEntry(sig_eff_offlineLRJ200_jFexLRJ, "Lead. LRJ E_{T} > 200 GeV", "l");
        sig_eff_offlineLRJ250_jFexLRJ->SetLineColor(colors[4]);
        sig_eff_offlineLRJ250_jFexLRJ->SetMarkerColor(colors[4]);
        sigLegEffLRJ_jFex->AddEntry(sig_eff_offlineLRJ250_jFexLRJ, "Lead. LRJ E_{T} > 250 GeV", "l");
        sig_eff_offlineLRJ300_jFexLRJ->SetLineColor(colors[5]);
        sig_eff_offlineLRJ300_jFexLRJ->SetMarkerColor(colors[5]);
        sigLegEffLRJ_jFex->AddEntry(sig_eff_offlineLRJ300_jFexLRJ, "Lead. LRJ E_{T} > 300 GeV", "l");
        sig_eff_offlineLRJ350_jFexLRJ->SetLineColor(colors[6]);
        sig_eff_offlineLRJ350_jFexLRJ->SetMarkerColor(colors[6]);
        sigLegEffLRJ_jFex->AddEntry(sig_eff_offlineLRJ350_jFexLRJ, "Lead. LRJ E_{T} > 350 GeV", "l");
        sig_eff_offlineLRJ400_jFexLRJ->SetLineColor(colors[7]);
        sig_eff_offlineLRJ400_jFexLRJ->SetMarkerColor(colors[7]);
        sigLegEffLRJ_jFex->AddEntry(sig_eff_offlineLRJ400_jFexLRJ, "Lead. LRJ E_{T} > 400 GeV", "l");
        sig_eff_offlineLRJ450_jFexLRJ->SetLineColor(colors[8]);
        sig_eff_offlineLRJ450_jFexLRJ->SetMarkerColor(colors[8]);
        sigLegEffLRJ_jFex->AddEntry(sig_eff_offlineLRJ450_jFexLRJ, "Lead. LRJ E_{T} > 450 GeV", "l");
        sig_eff_offlineLRJ500_jFexLRJ->SetLineColor(colors[9]);
        sig_eff_offlineLRJ500_jFexLRJ->SetMarkerColor(colors[9]);
        sigLegEffLRJ_jFex->AddEntry(sig_eff_offlineLRJ500_jFexLRJ, "Lead. LRJ E_{T} > 500 GeV", "l");
        sig_eff_offlineLRJ50_jFexLRJ->Draw("P");
        sig_eff_offlineLRJ100_jFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ150_jFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ200_jFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ250_jFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ300_jFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ350_jFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ400_jFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ450_jFexLRJ->Draw("P SAME");
        sig_eff_offlineLRJ500_jFexLRJ->Draw("P SAME");
        sigLegEffLRJ_jFex->Draw();
        cSigEffLRJ_jFex.SaveAs(modifiedOutputFileDir + "sig_overlayed_eff_LRJ_jFexLRJ.pdf");


        cBackEffLRJ_gFex.cd();
        // eff. vs. offline leading lrj Et overlay
        back_eff_offlineLRJ50_gFexLRJ->SetLineColor(colors[0]);
        back_eff_offlineLRJ50_gFexLRJ->SetMarkerColor(colors[0]);
        backLegEffLRJ_gFex->AddEntry(back_eff_offlineLRJ50_gFexLRJ, "Lead. LRJ E_{T} > 50 GeV", "l");
        back_eff_offlineLRJ100_gFexLRJ->SetLineColor(colors[1]);
        back_eff_offlineLRJ100_gFexLRJ->SetMarkerColor(colors[1]);
        backLegEffLRJ_gFex->AddEntry(back_eff_offlineLRJ100_gFexLRJ, "Lead. LRJ E_{T} > 100 GeV", "l");
        back_eff_offlineLRJ150_gFexLRJ->SetLineColor(colors[2]);
        back_eff_offlineLRJ150_gFexLRJ->SetMarkerColor(colors[2]);
        backLegEffLRJ_gFex->AddEntry(back_eff_offlineLRJ150_gFexLRJ, "Lead. LRJ E_{T} > 150 GeV", "l");
        back_eff_offlineLRJ200_gFexLRJ->SetLineColor(colors[3]);
        back_eff_offlineLRJ200_gFexLRJ->SetMarkerColor(colors[3]);
        backLegEffLRJ_gFex->AddEntry(back_eff_offlineLRJ200_gFexLRJ, "Lead. LRJ E_{T} > 200 GeV", "l");
        back_eff_offlineLRJ250_gFexLRJ->SetLineColor(colors[4]);
        back_eff_offlineLRJ250_gFexLRJ->SetMarkerColor(colors[4]);
        backLegEffLRJ_gFex->AddEntry(back_eff_offlineLRJ250_gFexLRJ, "Lead. LRJ E_{T} > 250 GeV", "l");
        back_eff_offlineLRJ300_gFexLRJ->SetLineColor(colors[5]);
        back_eff_offlineLRJ300_gFexLRJ->SetMarkerColor(colors[5]);
        backLegEffLRJ_gFex->AddEntry(back_eff_offlineLRJ300_gFexLRJ, "Lead. LRJ E_{T} > 300 GeV", "l");
        back_eff_offlineLRJ350_gFexLRJ->SetLineColor(colors[6]);
        back_eff_offlineLRJ350_gFexLRJ->SetMarkerColor(colors[6]);
        backLegEffLRJ_gFex->AddEntry(back_eff_offlineLRJ350_gFexLRJ, "Lead. LRJ E_{T} > 350 GeV", "l");
        back_eff_offlineLRJ400_gFexLRJ->SetLineColor(colors[7]);
        back_eff_offlineLRJ400_gFexLRJ->SetMarkerColor(colors[7]);
        backLegEffLRJ_gFex->AddEntry(back_eff_offlineLRJ400_gFexLRJ, "Lead. LRJ E_{T} > 400 GeV", "l");
        back_eff_offlineLRJ450_gFexLRJ->SetLineColor(colors[8]);
        back_eff_offlineLRJ450_gFexLRJ->SetMarkerColor(colors[8]);
        backLegEffLRJ_gFex->AddEntry(back_eff_offlineLRJ450_gFexLRJ, "Lead. LRJ E_{T} > 450 GeV", "l");
        back_eff_offlineLRJ50_gFexLRJ->Draw("P");
        back_eff_offlineLRJ100_gFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ150_gFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ200_gFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ250_gFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ300_gFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ350_gFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ400_gFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ450_gFexLRJ->Draw("P SAME");
        backLegEffLRJ_gFex->Draw();
        cBackEffLRJ_gFex.SaveAs(modifiedOutputFileDir + "back_overlayed_eff_LRJ_gFexLRJ.pdf");

        cBackEffLRJ_jFex.cd();
        // eff. vs. offline leading lrj Et overlay
        back_eff_offlineLRJ50_jFexLRJ->SetLineColor(colors[0]);
        back_eff_offlineLRJ50_jFexLRJ->SetMarkerColor(colors[0]);
        backLegEffLRJ_jFex->AddEntry(back_eff_offlineLRJ50_jFexLRJ, "Lead. LRJ E_{T} > 50 GeV", "l");
        back_eff_offlineLRJ100_jFexLRJ->SetLineColor(colors[1]);
        back_eff_offlineLRJ100_jFexLRJ->SetMarkerColor(colors[1]);
        backLegEffLRJ_jFex->AddEntry(back_eff_offlineLRJ100_jFexLRJ, "Lead. LRJ E_{T} > 100 GeV", "l");
        back_eff_offlineLRJ150_jFexLRJ->SetLineColor(colors[2]);
        back_eff_offlineLRJ150_jFexLRJ->SetMarkerColor(colors[2]);
        backLegEffLRJ_jFex->AddEntry(back_eff_offlineLRJ150_jFexLRJ, "Lead. LRJ E_{T} > 150 GeV", "l");
        back_eff_offlineLRJ200_jFexLRJ->SetLineColor(colors[3]);
        back_eff_offlineLRJ200_jFexLRJ->SetMarkerColor(colors[3]);
        backLegEffLRJ_jFex->AddEntry(back_eff_offlineLRJ200_jFexLRJ, "Lead. LRJ E_{T} > 200 GeV", "l");
        back_eff_offlineLRJ250_jFexLRJ->SetLineColor(colors[4]);
        back_eff_offlineLRJ250_jFexLRJ->SetMarkerColor(colors[4]);
        backLegEffLRJ_jFex->AddEntry(back_eff_offlineLRJ250_jFexLRJ, "Lead. LRJ E_{T} > 250 GeV", "l");
        back_eff_offlineLRJ300_jFexLRJ->SetLineColor(colors[5]);
        back_eff_offlineLRJ300_jFexLRJ->SetMarkerColor(colors[5]);
        backLegEffLRJ_jFex->AddEntry(back_eff_offlineLRJ300_jFexLRJ, "Lead. LRJ E_{T} > 300 GeV", "l");
        back_eff_offlineLRJ350_jFexLRJ->SetLineColor(colors[6]);
        back_eff_offlineLRJ350_jFexLRJ->SetMarkerColor(colors[6]);
        backLegEffLRJ_jFex->AddEntry(back_eff_offlineLRJ350_jFexLRJ, "Lead. LRJ E_{T} > 350 GeV", "l");
        back_eff_offlineLRJ400_jFexLRJ->SetLineColor(colors[7]);
        back_eff_offlineLRJ400_jFexLRJ->SetMarkerColor(colors[7]);
        backLegEffLRJ_jFex->AddEntry(back_eff_offlineLRJ400_jFexLRJ, "Lead. LRJ E_{T} > 400 GeV", "l");
        back_eff_offlineLRJ450_jFexLRJ->SetLineColor(colors[8]);
        back_eff_offlineLRJ450_jFexLRJ->SetMarkerColor(colors[8]);
        backLegEffLRJ_jFex->AddEntry(back_eff_offlineLRJ450_jFexLRJ, "Lead. LRJ E_{T} > 450 GeV", "l");
        back_eff_offlineLRJ50_jFexLRJ->Draw("P");
        back_eff_offlineLRJ100_jFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ150_jFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ200_jFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ250_jFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ300_jFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ350_jFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ400_jFexLRJ->Draw("P SAME");
        back_eff_offlineLRJ450_jFexLRJ->Draw("P SAME");
        backLegEffLRJ_jFex->Draw();
        cBackEffLRJ_jFex.SaveAs(modifiedOutputFileDir + "back_overlayed_eff_LRJ_jFexLRJ.pdf");

        cSigEffLRJ_gFex_LRJ_overlay.cd();

        // Choose the same colors you used before for 100/200/400
        const int col100 = colors[1]; // matches your 100 GeV color
        const int col150 = colors[2]; // matches your 150 GeV color
        const int col200 = colors[3]; // matches your 200 GeV color
        const int col250 = colors[4]; // matches your 250 GeV color
        const int col300 = colors[5]; // matches your 300 GeV color
        const int col400 = colors[7]; // matches your 400 GeV color
        const int col450 = colors[8]; // matches your 450 GeV color

        // Marker styles: full circle for offline, open circle for gFEX
        const int mFull = 20; // kFullCircle
        const int mOpen = 24; // kOpenCircle

        // ---- 100 GeV ----
        sig_eff_offlineLRJ100->SetLineColor(col100);
        sig_eff_offlineLRJ100->SetMarkerColor(col100);
        sig_eff_offlineLRJ100->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ100->SetMarkerSize(1.1);

        sig_eff_offlineLRJ100_gFexLRJ->SetLineColor(col100);
        sig_eff_offlineLRJ100_gFexLRJ->SetMarkerColor(col100);
        sig_eff_offlineLRJ100_gFexLRJ->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ100_gFexLRJ->SetMarkerSize(1.1);

        // ---- 250 GeV ----
        sig_eff_offlineLRJ250->SetLineColor(col250);
        sig_eff_offlineLRJ250->SetMarkerColor(col250);
        sig_eff_offlineLRJ250->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ250->SetMarkerSize(1.1);

        sig_eff_offlineLRJ250_gFexLRJ->SetLineColor(col250);
        sig_eff_offlineLRJ250_gFexLRJ->SetMarkerColor(col250);
        sig_eff_offlineLRJ250_gFexLRJ->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ250_gFexLRJ->SetMarkerSize(1.1);

        // ---- 400 GeV ----
        sig_eff_offlineLRJ400->SetLineColor(col400);
        sig_eff_offlineLRJ400->SetMarkerColor(col400);
        sig_eff_offlineLRJ400->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ400->SetMarkerSize(1.1);

        sig_eff_offlineLRJ400_gFexLRJ->SetLineColor(col400);
        sig_eff_offlineLRJ400_gFexLRJ->SetMarkerColor(col400);
        sig_eff_offlineLRJ400_gFexLRJ->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ400_gFexLRJ->SetMarkerSize(1.1);

        // Draw: use one graph with "AP" to set axes; then overlay the rest
        // (If these are TGraph/TGraphAsymmErrors, "AP" is appropriate; otherwise "P" is fine.)
        sig_eff_offlineLRJ100->Draw("P");              // sets axes
        sig_eff_offlineLRJ100_gFexLRJ->Draw("P SAME");

        sig_eff_offlineLRJ250->Draw("P SAME");
        sig_eff_offlineLRJ250_gFexLRJ->Draw("P SAME");

        sig_eff_offlineLRJ400->Draw("P SAME");
        sig_eff_offlineLRJ400_gFexLRJ->Draw("P SAME");

        // Legend
        
        TLegend *sigLegEffLRJ_gFex_LRJ_overlay = new TLegend(0.615,0.2,0.99,0.525);
        sigLegEffLRJ_gFex_LRJ_overlay->SetTextSize(0.0275);

        // Group entries by threshold, showing offline (full) and gFEX (open) with same color
        sigLegEffLRJ_gFex_LRJ_overlay->AddEntry(sig_eff_offlineLRJ100,           "Lead. Jet E_{T} > 100 GeV (JetTagger)", "p");
        sigLegEffLRJ_gFex_LRJ_overlay->AddEntry(sig_eff_offlineLRJ100_gFexLRJ,   "Lead. Jet E_{T} > 100 GeV (gFEX LRJ)",    "p");

        sigLegEffLRJ_gFex_LRJ_overlay->AddEntry(sig_eff_offlineLRJ250,           "Lead. Jet E_{T} > 250 GeV (JetTagger)", "p");
        sigLegEffLRJ_gFex_LRJ_overlay->AddEntry(sig_eff_offlineLRJ250_gFexLRJ,   "Lead. Jet E_{T} > 250 GeV (gFEX LRJ)",    "p");

        sigLegEffLRJ_gFex_LRJ_overlay->AddEntry(sig_eff_offlineLRJ400,           "Lead. Jet E_{T} > 400 GeV (JetTagger)", "p");
        sigLegEffLRJ_gFex_LRJ_overlay->AddEntry(sig_eff_offlineLRJ400_gFexLRJ,   "Lead. Jet E_{T} > 400 GeV (gFEX LRJ)",    "p");

        sigLegEffLRJ_gFex_LRJ_overlay->Draw();

        // Save to a new file/canvas
        cSigEffLRJ_gFex_LRJ_overlay.SaveAs(modifiedOutputFileDir + "sig_overlayed_eff_LRJ_off_vs_gFEX_100_250_400.pdf");


        // JFEX
        cSigEffLRJ_jFex_LRJ_overlay.cd();
        // ---- 100 GeV ----
        sig_eff_offlineLRJ100->SetLineColor(col100);
        sig_eff_offlineLRJ100->SetMarkerColor(col100);
        sig_eff_offlineLRJ100->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ100->SetMarkerSize(1.1);

        sig_eff_offlineLRJ100_jFexLRJ->SetLineColor(col100);
        sig_eff_offlineLRJ100_jFexLRJ->SetMarkerColor(col100);
        sig_eff_offlineLRJ100_jFexLRJ->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ100_jFexLRJ->SetMarkerSize(1.1);

        // ---- 250 GeV ----
        sig_eff_offlineLRJ250->SetLineColor(col250);
        sig_eff_offlineLRJ250->SetMarkerColor(col250);
        sig_eff_offlineLRJ250->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ250->SetMarkerSize(1.1);

        sig_eff_offlineLRJ250_jFexLRJ->SetLineColor(col250);
        sig_eff_offlineLRJ250_jFexLRJ->SetMarkerColor(col250);
        sig_eff_offlineLRJ250_jFexLRJ->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ250_jFexLRJ->SetMarkerSize(1.1);

        // ---- 400 GeV ----
        sig_eff_offlineLRJ400->SetLineColor(col400);
        sig_eff_offlineLRJ400->SetMarkerColor(col400);
        sig_eff_offlineLRJ400->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ400->SetMarkerSize(1.1);

        sig_eff_offlineLRJ400_jFexLRJ->SetLineColor(col400);
        sig_eff_offlineLRJ400_jFexLRJ->SetMarkerColor(col400);
        sig_eff_offlineLRJ400_jFexLRJ->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ400_jFexLRJ->SetMarkerSize(1.1);

        // Draw: use one graph with "AP" to set axes; then overlay the rest
        // (If these are TGraph/TGraphAsymmErrors, "AP" is appropriate; otherwise "P" is fine.)
        sig_eff_offlineLRJ100->Draw("P");              // sets axes
        sig_eff_offlineLRJ100_jFexLRJ->Draw("P SAME");

        sig_eff_offlineLRJ250->Draw("P SAME");
        sig_eff_offlineLRJ250_jFexLRJ->Draw("P SAME");

        sig_eff_offlineLRJ400->Draw("P SAME");
        sig_eff_offlineLRJ400_jFexLRJ->Draw("P SAME");

        // Legend
        
        TLegend *sigLegEffLRJ_jFex_LRJ_overlay = new TLegend(0.615,0.2,0.99,0.525);
        sigLegEffLRJ_jFex_LRJ_overlay->SetTextSize(0.0275);

        // Group entries by threshold, showing offline (full) and jFex (open) with same color
        sigLegEffLRJ_jFex_LRJ_overlay->AddEntry(sig_eff_offlineLRJ100,           "Lead. Jet E_{T} > 100 GeV (JetTagger)", "p");
        sigLegEffLRJ_jFex_LRJ_overlay->AddEntry(sig_eff_offlineLRJ100_jFexLRJ,   "Lead. Jet E_{T} > 100 GeV (jFEX LRJ)",    "p");

        sigLegEffLRJ_jFex_LRJ_overlay->AddEntry(sig_eff_offlineLRJ250,           "Lead. Jet E_{T} > 250 GeV (JetTagger)", "p");
        sigLegEffLRJ_jFex_LRJ_overlay->AddEntry(sig_eff_offlineLRJ250_jFexLRJ,   "Lead. Jet E_{T} > 250 GeV (jFEX LRJ)",    "p");

        sigLegEffLRJ_jFex_LRJ_overlay->AddEntry(sig_eff_offlineLRJ400,           "Lead. Jet E_{T} > 400 GeV (JetTagger)", "p");
        sigLegEffLRJ_jFex_LRJ_overlay->AddEntry(sig_eff_offlineLRJ400_jFexLRJ,   "Lead. Jet E_{T} > 400 GeV (jFEX LRJ)",    "p");

        sigLegEffLRJ_jFex_LRJ_overlay->Draw();

        // Save to a new file/canvas
        cSigEffLRJ_jFex_LRJ_overlay.SaveAs(modifiedOutputFileDir + "sig_overlayed_eff_LRJ_off_vs_jFex_100_250_400.pdf");



        cSigEffLRJ_gFex_LRJ_Dijet_overlay.cd();
        // ---- 100 GeV ----
        sig_eff_offlineLRJ100_Dijet->SetLineColor(col100);
        sig_eff_offlineLRJ100_Dijet->SetMarkerColor(col100);
        sig_eff_offlineLRJ100_Dijet->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ100_Dijet->SetMarkerSize(1.1);

        sig_eff_offlineLRJ100_gFexLRJ_Dijet->SetLineColor(col100);
        sig_eff_offlineLRJ100_gFexLRJ_Dijet->SetMarkerColor(col100);
        sig_eff_offlineLRJ100_gFexLRJ_Dijet->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ100_gFexLRJ_Dijet->SetMarkerSize(1.1);

        // ---- 250 GeV ----
        sig_eff_offlineLRJ250_Dijet->SetLineColor(col250);
        sig_eff_offlineLRJ250_Dijet->SetMarkerColor(col250);
        sig_eff_offlineLRJ250_Dijet->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ250_Dijet->SetMarkerSize(1.1);

        sig_eff_offlineLRJ250_gFexLRJ_Dijet->SetLineColor(col250);
        sig_eff_offlineLRJ250_gFexLRJ_Dijet->SetMarkerColor(col250);
        sig_eff_offlineLRJ250_gFexLRJ_Dijet->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ250_gFexLRJ_Dijet->SetMarkerSize(1.1);

        // ---- 400 GeV ----
        sig_eff_offlineLRJ400_Dijet->SetLineColor(col400);
        sig_eff_offlineLRJ400_Dijet->SetMarkerColor(col400);
        sig_eff_offlineLRJ400_Dijet->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ400_Dijet->SetMarkerSize(1.1);

        sig_eff_offlineLRJ400_gFexLRJ_Dijet->SetLineColor(col400);
        sig_eff_offlineLRJ400_gFexLRJ_Dijet->SetMarkerColor(col400);
        sig_eff_offlineLRJ400_gFexLRJ_Dijet->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ400_gFexLRJ_Dijet->SetMarkerSize(1.1);

        // Draw: use one graph with "AP" to set axes; then overlay the rest
        // (If these are TGraph/TGraphAsymmErrors, "AP" is appropriate; otherwise "P" is fine.)
        sig_eff_offlineLRJ100_Dijet->Draw("P");              // sets axes
        sig_eff_offlineLRJ100_gFexLRJ_Dijet->Draw("P SAME");

        sig_eff_offlineLRJ250_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ250_gFexLRJ_Dijet->Draw("P SAME");

        sig_eff_offlineLRJ400_Dijet->Draw("P SAME");
        sig_eff_offlineLRJ400_gFexLRJ_Dijet->Draw("P SAME");

        // Legend
        
        TLegend *sigLegEffLRJ_gFex_LRJ_Dijet_overlay = new TLegend(0.515,0.2,0.94,0.525);
        sigLegEffLRJ_gFex_LRJ_Dijet_overlay->SetTextSize(0.0275);

        // Group entries by threshold, showing offline (full) and gFEX (open) with same color
        sigLegEffLRJ_gFex_LRJ_Dijet_overlay->AddEntry(sig_eff_offlineLRJ100_Dijet,           "Sublead. Jet E_{T} > 100 GeV (JetTagger)", "p");
        sigLegEffLRJ_gFex_LRJ_Dijet_overlay->AddEntry(sig_eff_offlineLRJ100_gFexLRJ_Dijet,   "Sublead. Jet E_{T} > 100 GeV (gFEX LRJ)",    "p");

        sigLegEffLRJ_gFex_LRJ_Dijet_overlay->AddEntry(sig_eff_offlineLRJ250_Dijet,           "Sublead. Jet E_{T} > 250 GeV (JetTagger)", "p");
        sigLegEffLRJ_gFex_LRJ_Dijet_overlay->AddEntry(sig_eff_offlineLRJ250_gFexLRJ_Dijet,   "Sublead. Jet E_{T} > 250 GeV (gFEX LRJ)",    "p");

        sigLegEffLRJ_gFex_LRJ_Dijet_overlay->AddEntry(sig_eff_offlineLRJ400_Dijet,           "Sublead. Jet E_{T} > 400 GeV (JetTagger)", "p");
        sigLegEffLRJ_gFex_LRJ_Dijet_overlay->AddEntry(sig_eff_offlineLRJ400_gFexLRJ_Dijet,   "Sublead. Jet E_{T} > 400 GeV (gFEX LRJ)",    "p");

        //sigLegEffLRJ_gFex_LRJ_Dijet_overlay->Draw(); // TOGGLE DRAW OR DON'T DRAW

        // Save to a new file/canvas
        cSigEffLRJ_gFex_LRJ_Dijet_overlay.SaveAs(modifiedOutputFileDir + "sig_overlayed_eff_LRJ_off_vs_gFEX_Dijet_150_250_400.pdf");









        cSigEffLRJ_Mass100to150_LRJ_overlay.cd();
        // ---- 150 GeV ----
        sig_eff_offlineLRJ150->SetLineColor(col150);
        sig_eff_offlineLRJ150->SetMarkerColor(col150);
        sig_eff_offlineLRJ150->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ150->SetMarkerSize(1.1);

        sig_eff_offlineLRJ150_mass100to150->SetLineColor(col150);
        sig_eff_offlineLRJ150_mass100to150->SetMarkerColor(col150);
        sig_eff_offlineLRJ150_mass100to150->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ150_mass100to150->SetMarkerSize(1.1);

        // ---- 300 GeV ----
        sig_eff_offlineLRJ300->SetLineColor(col300);
        sig_eff_offlineLRJ300->SetMarkerColor(col300);
        sig_eff_offlineLRJ300->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ300->SetMarkerSize(1.1);

        sig_eff_offlineLRJ300_mass100to150->SetLineColor(col300);
        sig_eff_offlineLRJ300_mass100to150->SetMarkerColor(col300);
        sig_eff_offlineLRJ300_mass100to150->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ300_mass100to150->SetMarkerSize(1.1);

        // ---- 450 GeV ----
        sig_eff_offlineLRJ450->SetLineColor(col450);
        sig_eff_offlineLRJ450->SetMarkerColor(col450);
        sig_eff_offlineLRJ450->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ450->SetMarkerSize(1.1);

        sig_eff_offlineLRJ450_mass100to150->SetLineColor(col450);
        sig_eff_offlineLRJ450_mass100to150->SetMarkerColor(col450);
        sig_eff_offlineLRJ450_mass100to150->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ450_mass100to150->SetMarkerSize(1.1);

        // Draw: use one graph with "AP" to set axes; then overlay the rest
        // (If these are TGraph/TGraphAsymmErrors, "AP" is appropriate; otherwise "P" is fine.)
        sig_eff_offlineLRJ150->Draw("P");              // sets axes
        sig_eff_offlineLRJ150_mass100to150->Draw("P SAME");

        sig_eff_offlineLRJ300->Draw("P SAME");
        sig_eff_offlineLRJ300_mass100to150->Draw("P SAME");

        sig_eff_offlineLRJ450->Draw("P SAME");
        sig_eff_offlineLRJ450_mass100to150->Draw("P SAME");

        // Legend
        
        TLegend *sigLegEffLRJ_mass100to150_LRJ_overlay = new TLegend(0.56,0.2,0.99,0.525);
        sigLegEffLRJ_mass100to150_LRJ_overlay->SetTextSize(0.0275);

        // Group entries by threshold, showing offline (full) and gFEX (open) with same color
        sigLegEffLRJ_mass100to150_LRJ_overlay->AddEntry(sig_eff_offlineLRJ150,           "Lead. Jet E_{T} > 150 GeV", "p");
        sigLegEffLRJ_mass100to150_LRJ_overlay->AddEntry(sig_eff_offlineLRJ150_mass100to150,   "Lead. Jet E_{T} > 150, 100 < m_{off.} < 150 GeV",    "p");

        sigLegEffLRJ_mass100to150_LRJ_overlay->AddEntry(sig_eff_offlineLRJ300,           "Lead. Jet E_{T} > 300 GeV", "p");
        sigLegEffLRJ_mass100to150_LRJ_overlay->AddEntry(sig_eff_offlineLRJ300_mass100to150,   "Lead. Jet E_{T} > 300, 100 < m_{off.} < 150 GeV",    "p");

        sigLegEffLRJ_mass100to150_LRJ_overlay->AddEntry(sig_eff_offlineLRJ450,           "Lead. Jet E_{T} > 450 GeV", "p");
        sigLegEffLRJ_mass100to150_LRJ_overlay->AddEntry(sig_eff_offlineLRJ450_mass100to150,   "Lead. Jet E_{T} > 450, 100 < m_{off.} < 150 GeV ",    "p");

        sigLegEffLRJ_mass100to150_LRJ_overlay->Draw(); // DRAW OR DON'T DRAW

        // Save to a new file/canvas
        cSigEffLRJ_Mass100to150_LRJ_overlay.SaveAs(modifiedOutputFileDir + "sig_overlayed_eff_LRJ_off_vs_LRJ_mass100to150_150_300_450.pdf");


        cSigEffLRJ_Overlay_1Subjet_GrEq2Subjets.cd();
        // ---- 150 GeV ----
        sig_eff_offlineLRJ100_1Subjet->SetLineColor(col100);
        sig_eff_offlineLRJ100_1Subjet->SetMarkerColor(col100);
        sig_eff_offlineLRJ100_1Subjet->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ100_1Subjet->SetMarkerSize(1.1);

        sig_eff_offlineLRJ100_GrEq2Subjets->SetLineColor(col100);
        sig_eff_offlineLRJ100_GrEq2Subjets->SetMarkerColor(col100);
        sig_eff_offlineLRJ100_GrEq2Subjets->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ100_GrEq2Subjets->SetMarkerSize(1.1);

        // ---- 250 GeV ----
        sig_eff_offlineLRJ250_1Subjet->SetLineColor(col250);
        sig_eff_offlineLRJ250_1Subjet->SetMarkerColor(col250);
        sig_eff_offlineLRJ250_1Subjet->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ250_1Subjet->SetMarkerSize(1.1);

        sig_eff_offlineLRJ250_GrEq2Subjets->SetLineColor(col250);
        sig_eff_offlineLRJ250_GrEq2Subjets->SetMarkerColor(col250);
        sig_eff_offlineLRJ250_GrEq2Subjets->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ250_GrEq2Subjets->SetMarkerSize(1.1);

        // ---- 400 GeV ----
        sig_eff_offlineLRJ400_1Subjet->SetLineColor(col400);
        sig_eff_offlineLRJ400_1Subjet->SetMarkerColor(col400);
        sig_eff_offlineLRJ400_1Subjet->SetMarkerStyle(mFull);
        sig_eff_offlineLRJ400_1Subjet->SetMarkerSize(1.1);

        sig_eff_offlineLRJ400_GrEq2Subjets->SetLineColor(col400);
        sig_eff_offlineLRJ400_GrEq2Subjets->SetMarkerColor(col400);
        sig_eff_offlineLRJ400_GrEq2Subjets->SetMarkerStyle(mOpen);
        sig_eff_offlineLRJ400_GrEq2Subjets->SetMarkerSize(1.1);

        // Draw: use one graph with "AP" to set axes; then overlay the rest
        // (If these are TGraph/TGraphAsymmErrors, "AP" is appropriate; otherwise "P" is fine.)
        sig_eff_offlineLRJ100_1Subjet->Draw("P");              // sets axes
        sig_eff_offlineLRJ100_GrEq2Subjets->Draw("P SAME");

        sig_eff_offlineLRJ250_1Subjet->Draw("P SAME");
        sig_eff_offlineLRJ250_GrEq2Subjets->Draw("P SAME");

        sig_eff_offlineLRJ400_1Subjet->Draw("P SAME");
        sig_eff_offlineLRJ400_GrEq2Subjets->Draw("P SAME");

        // Legend
        
        TLegend *sigLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay = new TLegend(0.56,0.2,0.99,0.525);
        sigLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->SetTextSize(0.0275);

        // Group entries by threshold, showing offline (full) and gFEX (open) with same color
        sigLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->AddEntry(sig_eff_offlineLRJ100_1Subjet,           "Lead. Jet E_{T} > 100 GeV [1 Subjet]", "p");
        sigLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->AddEntry(sig_eff_offlineLRJ100_GrEq2Subjets,   "Lead. Jet E_{T} > 100 GeV [#geq 2 Subjets]",    "p");

        sigLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->AddEntry(sig_eff_offlineLRJ250_1Subjet,           "Lead. Jet E_{T} > 250 GeV [1 Subjet]", "p");
        sigLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->AddEntry(sig_eff_offlineLRJ250_GrEq2Subjets,   "Lead. Jet E_{T} > 250 GeV [#geq 2 Subjets]",    "p");

        sigLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->AddEntry(sig_eff_offlineLRJ400_1Subjet,           "Lead. Jet E_{T} > 400 GeV [1 Subjet]", "p");
        sigLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->AddEntry(sig_eff_offlineLRJ400_GrEq2Subjets,   "Lead. Jet E_{T} > 400 GeV [#geq 2 Subjets]",    "p");

        sigLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->Draw(); // DRAW OR DON'T DRAW

        // Save to a new file/canvas
        cSigEffLRJ_Overlay_1Subjet_GrEq2Subjets.SaveAs(modifiedOutputFileDir + "sig_overlayed_eff_LRJ_off_vs_1Subjet_GrEq2Subjets_100_250_400.pdf");



        cBackEffLRJ_Overlay_1Subjet_GrEq2Subjets.cd();
        // ---- 100 GeV ----
        back_eff_offlineLRJ100_1Subjet->SetLineColor(col100);
        back_eff_offlineLRJ100_1Subjet->SetMarkerColor(col100);
        back_eff_offlineLRJ100_1Subjet->SetMarkerStyle(mFull);
        back_eff_offlineLRJ100_1Subjet->SetMarkerSize(1.1);

        back_eff_offlineLRJ100_GrEq2Subjets->SetLineColor(col100);
        back_eff_offlineLRJ100_GrEq2Subjets->SetMarkerColor(col100);
        back_eff_offlineLRJ100_GrEq2Subjets->SetMarkerStyle(mOpen);
        back_eff_offlineLRJ100_GrEq2Subjets->SetMarkerSize(1.1);

        // ---- 200 GeV ----
        back_eff_offlineLRJ200_1Subjet->SetLineColor(col200);
        back_eff_offlineLRJ200_1Subjet->SetMarkerColor(col200);
        back_eff_offlineLRJ200_1Subjet->SetMarkerStyle(mFull);
        back_eff_offlineLRJ200_1Subjet->SetMarkerSize(1.1);

        back_eff_offlineLRJ200_GrEq2Subjets->SetLineColor(col200);
        back_eff_offlineLRJ200_GrEq2Subjets->SetMarkerColor(col200);
        back_eff_offlineLRJ200_GrEq2Subjets->SetMarkerStyle(mOpen);
        back_eff_offlineLRJ200_GrEq2Subjets->SetMarkerSize(1.1);

        // ---- 300 GeV ----
        back_eff_offlineLRJ300_1Subjet->SetLineColor(col300);
        back_eff_offlineLRJ300_1Subjet->SetMarkerColor(col300);
        back_eff_offlineLRJ300_1Subjet->SetMarkerStyle(mFull);
        back_eff_offlineLRJ300_1Subjet->SetMarkerSize(1.1);

        back_eff_offlineLRJ300_GrEq2Subjets->SetLineColor(col300);
        back_eff_offlineLRJ300_GrEq2Subjets->SetMarkerColor(col300);
        back_eff_offlineLRJ300_GrEq2Subjets->SetMarkerStyle(mOpen);
        back_eff_offlineLRJ300_GrEq2Subjets->SetMarkerSize(1.1);

        // Draw: use one graph with "AP" to set axes; then overlay the rest
        // (If these are TGraph/TGraphAsymmErrors, "AP" is appropriate; otherwise "P" is fine.)
        back_eff_offlineLRJ100_1Subjet->Draw("P");              // sets axes
        back_eff_offlineLRJ100_GrEq2Subjets->Draw("P SAME");

        back_eff_offlineLRJ200_1Subjet->Draw("P SAME");
        back_eff_offlineLRJ200_GrEq2Subjets->Draw("P SAME");

        back_eff_offlineLRJ300_1Subjet->Draw("P SAME");
        back_eff_offlineLRJ300_GrEq2Subjets->Draw("P SAME");

        // Legend
        
        TLegend *backLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay = new TLegend(0.56,0.2,0.99,0.525);
        backLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->SetTextSize(0.0275);

        // Group entries by threshold, showing offline (full) and gFEX (open) with same color
        backLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->AddEntry(back_eff_offlineLRJ100_1Subjet,           "Lead. Jet E_{T} > 100 GeV [1 Subjet]", "p");
        backLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->AddEntry(back_eff_offlineLRJ100_GrEq2Subjets,   "Lead. Jet E_{T} > 100 GeV [#geq 2 Subjets]",    "p");

        backLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->AddEntry(back_eff_offlineLRJ200_1Subjet,           "Lead. Jet E_{T} > 200 GeV [1 Subjet]", "p");
        backLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->AddEntry(back_eff_offlineLRJ200_GrEq2Subjets,   "Lead. Jet E_{T} > 200 GeV [#geq 2 Subjets]",    "p");

        backLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->AddEntry(back_eff_offlineLRJ300_1Subjet,           "Lead. Jet E_{T} > 300 GeV [1 Subjet]", "p");
        backLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->AddEntry(back_eff_offlineLRJ300_GrEq2Subjets,   "Lead. Jet E_{T} > 300 GeV [#geq 2 Subjets]",    "p");

        backLegEffLRJ_1Subjet_GrEq2Subjets_LRJ_overlay->Draw(); // DRAW OR DON'T DRAW

        // Save to a new file/canvas
        cBackEffLRJ_Overlay_1Subjet_GrEq2Subjets.SaveAs(modifiedOutputFileDir + "back_overlayed_eff_LRJ_off_vs_1Subjet_GrEq2Subjets_100_200_300.pdf");







        cBackEffLRJ_gFex_LRJ_overlay.cd();

        // ---- 100 GeV ----
        back_eff_offlineLRJ100->SetLineColor(col100);
        back_eff_offlineLRJ100->SetMarkerColor(col100);
        back_eff_offlineLRJ100->SetMarkerStyle(mFull);
        back_eff_offlineLRJ100->SetMarkerSize(1.1);

        back_eff_offlineLRJ100_gFexLRJ->SetLineColor(col100);
        back_eff_offlineLRJ100_gFexLRJ->SetMarkerColor(col100);
        back_eff_offlineLRJ100_gFexLRJ->SetMarkerStyle(mOpen);
        back_eff_offlineLRJ100_gFexLRJ->SetMarkerSize(1.1);

        // ---- 200 GeV ----
        back_eff_offlineLRJ200->SetLineColor(col200);
        back_eff_offlineLRJ200->SetMarkerColor(col200);
        back_eff_offlineLRJ200->SetMarkerStyle(mFull);
        back_eff_offlineLRJ200->SetMarkerSize(1.1);

        back_eff_offlineLRJ200_gFexLRJ->SetLineColor(col200);
        back_eff_offlineLRJ200_gFexLRJ->SetMarkerColor(col200);
        back_eff_offlineLRJ200_gFexLRJ->SetMarkerStyle(mOpen);
        back_eff_offlineLRJ200_gFexLRJ->SetMarkerSize(1.1);

        // ---- 300 GeV ----
        back_eff_offlineLRJ300->SetLineColor(col300);
        back_eff_offlineLRJ300->SetMarkerColor(col300);
        back_eff_offlineLRJ300->SetMarkerStyle(mFull);
        back_eff_offlineLRJ300->SetMarkerSize(1.1);

        back_eff_offlineLRJ300_gFexLRJ->SetLineColor(col300);
        back_eff_offlineLRJ300_gFexLRJ->SetMarkerColor(col300);
        back_eff_offlineLRJ300_gFexLRJ->SetMarkerStyle(mOpen);
        back_eff_offlineLRJ300_gFexLRJ->SetMarkerSize(1.1);

        // Draw: use one graph with "AP" to set axes; then overlay the rest
        // (If these are TGraph/TGraphAsymmErrors, "AP" is appropriate; otherwise "P" is fine.)
        back_eff_offlineLRJ100->Draw("P");              // sets axes
        back_eff_offlineLRJ100_gFexLRJ->Draw("P SAME");

        back_eff_offlineLRJ200->Draw("P SAME");
        back_eff_offlineLRJ200_gFexLRJ->Draw("P SAME");

        back_eff_offlineLRJ300->Draw("P SAME");
        back_eff_offlineLRJ300_gFexLRJ->Draw("P SAME");

        // Legend
        
        TLegend *backLegEffLRJ_gFex_LRJ_overlay = new TLegend(0.615,0.2,0.99,0.525);
        backLegEffLRJ_gFex_LRJ_overlay->SetTextSize(0.0275);

        // Group entries by threshold, showing offline (full) and gFEX (open) with same color
        backLegEffLRJ_gFex_LRJ_overlay->AddEntry(back_eff_offlineLRJ100,           "Lead. Jet E_{T} > 100 GeV (JetTagger)", "p");
        backLegEffLRJ_gFex_LRJ_overlay->AddEntry(back_eff_offlineLRJ100_gFexLRJ,   "Lead. Jet E_{T} > 100 GeV (gFEX LRJ)",    "p");

        backLegEffLRJ_gFex_LRJ_overlay->AddEntry(back_eff_offlineLRJ200,           "Lead. Jet E_{T} > 200 GeV (JetTagger)", "p");
        backLegEffLRJ_gFex_LRJ_overlay->AddEntry(back_eff_offlineLRJ200_gFexLRJ,   "Lead. Jet E_{T} > 200 GeV (gFEX LRJ)",    "p");

        backLegEffLRJ_gFex_LRJ_overlay->AddEntry(back_eff_offlineLRJ300,           "Lead. Jet E_{T} > 300 GeV (JetTagger)", "p");
        backLegEffLRJ_gFex_LRJ_overlay->AddEntry(back_eff_offlineLRJ300_gFexLRJ,   "Lead. Jet E_{T} > 300 GeV (gFEX LRJ)",    "p");

        //backLegEffLRJ_gFex_LRJ_overlay->Draw();

        // Save to a new file/canvas
        cBackEffLRJ_gFex_LRJ_overlay.SaveAs(modifiedOutputFileDir + "back_overlayed_eff_LRJ_off_vs_gFEX_100_200_300.pdf");

        // jFEX
        cBackEffLRJ_jFex_LRJ_overlay.cd();

        // ---- 100 GeV ----
        back_eff_offlineLRJ100->SetLineColor(col100);
        back_eff_offlineLRJ100->SetMarkerColor(col100);
        back_eff_offlineLRJ100->SetMarkerStyle(mFull);
        back_eff_offlineLRJ100->SetMarkerSize(1.1);

        back_eff_offlineLRJ100_jFexLRJ->SetLineColor(col100);
        back_eff_offlineLRJ100_jFexLRJ->SetMarkerColor(col100);
        back_eff_offlineLRJ100_jFexLRJ->SetMarkerStyle(mOpen);
        back_eff_offlineLRJ100_jFexLRJ->SetMarkerSize(1.1);

        // ---- 200 GeV ----
        back_eff_offlineLRJ200->SetLineColor(col200);
        back_eff_offlineLRJ200->SetMarkerColor(col200);
        back_eff_offlineLRJ200->SetMarkerStyle(mFull);
        back_eff_offlineLRJ200->SetMarkerSize(1.1);

        back_eff_offlineLRJ200_jFexLRJ->SetLineColor(col200);
        back_eff_offlineLRJ200_jFexLRJ->SetMarkerColor(col200);
        back_eff_offlineLRJ200_jFexLRJ->SetMarkerStyle(mOpen);
        back_eff_offlineLRJ200_jFexLRJ->SetMarkerSize(1.1);

        // ---- 300 GeV ----
        back_eff_offlineLRJ300->SetLineColor(col300);
        back_eff_offlineLRJ300->SetMarkerColor(col300);
        back_eff_offlineLRJ300->SetMarkerStyle(mFull);
        back_eff_offlineLRJ300->SetMarkerSize(1.1);

        back_eff_offlineLRJ300_jFexLRJ->SetLineColor(col300);
        back_eff_offlineLRJ300_jFexLRJ->SetMarkerColor(col300);
        back_eff_offlineLRJ300_jFexLRJ->SetMarkerStyle(mOpen);
        back_eff_offlineLRJ300_jFexLRJ->SetMarkerSize(1.1);

        // Draw: use one graph with "AP" to set axes; then overlay the rest
        // (If these are TGraph/TGraphAsymmErrors, "AP" is appropriate; otherwise "P" is fine.)
        back_eff_offlineLRJ100->Draw("P");              // sets axes
        back_eff_offlineLRJ100_jFexLRJ->Draw("P SAME");

        back_eff_offlineLRJ200->Draw("P SAME");
        back_eff_offlineLRJ200_jFexLRJ->Draw("P SAME");

        back_eff_offlineLRJ300->Draw("P SAME");
        back_eff_offlineLRJ300_jFexLRJ->Draw("P SAME");

        // Legend
        
        TLegend *backLegEffLRJ_jFex_LRJ_overlay = new TLegend(0.615,0.2,0.99,0.525);
        backLegEffLRJ_jFex_LRJ_overlay->SetTextSize(0.0275);

        // Group entries by threshold, showing offline (full) and jFex (open) with same color
        backLegEffLRJ_jFex_LRJ_overlay->AddEntry(back_eff_offlineLRJ100,           "Lead. Jet E_{T} > 100 GeV (JetTagger)", "p");
        backLegEffLRJ_jFex_LRJ_overlay->AddEntry(back_eff_offlineLRJ100_jFexLRJ,   "Lead. Jet E_{T} > 100 GeV (jFEX LRJ)",    "p");

        backLegEffLRJ_jFex_LRJ_overlay->AddEntry(back_eff_offlineLRJ200,           "Lead. Jet E_{T} > 200 GeV (JetTagger)", "p");
        backLegEffLRJ_jFex_LRJ_overlay->AddEntry(back_eff_offlineLRJ200_jFexLRJ,   "Lead. Jet E_{T} > 200 GeV (jFEX LRJ)",    "p");

        backLegEffLRJ_jFex_LRJ_overlay->AddEntry(back_eff_offlineLRJ300,           "Lead. Jet E_{T} > 300 GeV (JetTagger)", "p");
        backLegEffLRJ_jFex_LRJ_overlay->AddEntry(back_eff_offlineLRJ300_jFexLRJ,   "Lead. Jet E_{T} > 300 GeV (jFEX LRJ)",    "p");

        //backLegEffLRJ_jFex_LRJ_overlay->Draw();

        // Save to a new file/canvas
        cBackEffLRJ_jFex_LRJ_overlay.SaveAs(modifiedOutputFileDir + "back_overlayed_eff_LRJ_off_vs_jFex_100_200_300.pdf");


        cSigEffb.cd();
        // eff. vs. avg b Et overlay
        sig_eff_b_Et50->SetLineColor(colors[0]);
        sig_eff_b_Et50->SetMarkerColor(colors[0]);
        legEffb->AddEntry(sig_eff_b_Et50, "Avg. b E_{T} > 50 GeV", "l");
        sig_eff_b_Et100->SetLineColor(colors[1]);
        sig_eff_b_Et100->SetMarkerColor(colors[1]);
        legEffb->AddEntry(sig_eff_b_Et100, "Avg. b E_{T} > 100 GeV", "l");
        sig_eff_b_Et150->SetLineColor(colors[2]);
        sig_eff_b_Et150->SetMarkerColor(colors[2]);
        legEffb->AddEntry(sig_eff_b_Et150, "Avg. b E_{T} > 150 GeV", "l");
        sig_eff_b_Et200->SetLineColor(colors[3]);
        sig_eff_b_Et200->SetMarkerColor(colors[3]);
        legEffb->AddEntry(sig_eff_b_Et200, "Avg. b E_{T} > 200 GeV", "l");
        sig_eff_b_Et250->SetLineColor(colors[4]);
        sig_eff_b_Et250->SetMarkerColor(colors[4]);
        legEffb->AddEntry(sig_eff_b_Et250, "Avg. b E_{T} > 250 GeV", "l");
        sig_eff_b_Et50->Draw("P");
        sig_eff_b_Et100->Draw("P SAME");
        sig_eff_b_Et150->Draw("P SAME");
        sig_eff_b_Et200->Draw("P SAME");
        sig_eff_b_Et250->Draw("P SAME");
        legEffb->Draw();
        cSigEffb.SaveAs(modifiedOutputFileDir + "overlayed_eff_b_Et.pdf");

    } 

    TString outputFileDir = "overlayLargeRJetHistograms/";

    if (overlayThreeFiles){
        // -----------------------------
        // Colors for thresholds (pick from your palette)
        // -----------------------------
        const int bcol100 = colors[1];
        const int bcol200 = colors[3];
        const int bcol300 = colors[5];

        // -----------------------------
        // Marker style by file index
        // -----------------------------
        std::vector<int> fileMarkerStyles_b = {20, 24, 25}; // filled circle, open circle, open square
        const double bmSize = 1.1;

        // -----------------------------
        // Prepare canvas + axes frame
        // -----------------------------
        TCanvas cBack("cBack","Background efficiency overlays", 900, 700);

        // Take x-range from the first available histogram (background binning is 50–500 in your code)
        double bxmin = 0, bxmax = 0;
        auto pickBXRange = [&](TH1* h){
        if (h && bxmax<=bxmin) { bxmin = h->GetXaxis()->GetXmin(); bxmax = h->GetXaxis()->GetXmax(); }
        };

        for (auto* h : { (TH1*) (back_eff_100.empty()?nullptr:back_eff_100[0]),
                        (TH1*) (back_eff_200.empty()?nullptr:back_eff_200[0]),
                        (TH1*) (back_eff_300.empty()?nullptr:back_eff_300[0]) }) pickBXRange(h);

        if (bxmax<=bxmin) { bxmin = 50; bxmax = 500; } // fallback to your background binning

        TH1F* bframe = (TH1F*)gPad->DrawFrame(bxmin, 0.0, bxmax, 1.05);
        bframe->SetTitle("Emulated Trigger Efficiency (Background);Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Background)");
        bframe->GetYaxis()->CenterTitle(true);

        // -----------------------------
        // Legends
        // -----------------------------
        TLegend* bLegFiles  = new TLegend(0.54, 0.20, 0.91, 0.48); // file names (marker shapes)
        TLegend* bLegThresh = new TLegend(0.57, 0.50, 0.94, 0.70); // threshold colors
        bLegFiles->SetTextSize(0.028);
        bLegThresh->SetTextSize(0.028);

        // -----------------------------
        // Draw all files × thresholds
        // -----------------------------
        auto styleAndDrawB = [&](TH1F* h, int color, int mstyle) {
            if (!h) return;
            h->SetLineColor(color);
            h->SetMarkerColor(color);
            h->SetMarkerStyle(mstyle);
            h->SetMarkerSize(bmSize);
            h->SetLineWidth(2);
            h->Draw("P SAME");  // or "E1 P SAME" for error bars
        };

        const int nBFiles = (int)std::max({back_eff_100.size(), back_eff_200.size(), back_eff_300.size()});
        for (int fileIt = 0; fileIt < nBFiles; ++fileIt) {
        const int mstyle = fileMarkerStyles_b[fileIt % (int)fileMarkerStyles_b.size()];

        if (fileIt < (int)back_eff_100.size()) styleAndDrawB(back_eff_100[fileIt], bcol100, mstyle);
        if (fileIt < (int)back_eff_200.size()) styleAndDrawB(back_eff_200[fileIt], bcol200, mstyle);
        if (fileIt < (int)back_eff_300.size()) styleAndDrawB(back_eff_300[fileIt], bcol300, mstyle);

        // Use one object to represent the file in the legend (carries the marker)
        TH1F* rep =
            (fileIt < (int)back_eff_100.size()) ? back_eff_100[fileIt] :
            (fileIt < (int)back_eff_200.size()) ? back_eff_200[fileIt] :
            (fileIt < (int)back_eff_300.size()) ? back_eff_300[fileIt] : nullptr;

        if (rep) {
            const std::string label =
            legendMap.count(algorithmConfigurations[fileIt])
            ? legendMap[algorithmConfigurations[fileIt]]
            : backgroundRootFileNames[fileIt];
            auto e = bLegFiles->AddEntry((TObject*)nullptr, label.c_str(), "p");
            e->SetMarkerStyle(mstyle);   // 20, 24, 25 ...
            e->SetMarkerSize(bmSize);
            e->SetMarkerColor(kBlack);   // force black marker in legend
            e->SetLineColor(kBlack);     // (harmless for "p")
        }
        }

        // Threshold color key (line entries so color shows independent of marker shape)
        if (!back_eff_100.empty()) { auto h = back_eff_100.front(); h->SetLineColor(bcol100); bLegThresh->AddEntry(h, "E_{T} > 100 GeV", "l"); }
        if (!back_eff_200.empty()) { auto h = back_eff_200.front(); h->SetLineColor(bcol200); bLegThresh->AddEntry(h, "E_{T} > 200 GeV", "l"); }
        if (!back_eff_300.empty()) { auto h = back_eff_300.front(); h->SetLineColor(bcol300); bLegThresh->AddEntry(h, "E_{T} > 300 GeV", "l"); }

        // Draw legends
        //bLegFiles->Draw();
        //bLegThresh->Draw();

        // Save
        cBack.SaveAs(outputFileDir + "back_eff_overlay_all_files_100_200_300.pdf");


        // -----------------------------
        // Colors for thresholds
        // -----------------------------
        const int col100 = colors[1];  // match your palette
        const int col250 = colors[4];
        const int col400 = colors[7];

        // -----------------------------
        // Marker style by file index
        // (extend if you have >3 files)
        // -----------------------------
        std::vector<int> fileMarkerStyles = {20, 24, 25}; // filled circle, open circle, open square
        const double mSize = 1.1;

        // -----------------------------
        // Prepare canvas + axes frame
        // -----------------------------
        TCanvas cSig("cSig","Signal efficiency overlays", 900, 700);

        // Take x-range from the first available histogram
        double xmin = 0, xmax = 0;
        auto pickXRange = [&](TH1* h){
        if (h && xmax<=xmin) { xmin = h->GetXaxis()->GetXmin(); xmax = h->GetXaxis()->GetXmax(); }
        };

        for (auto* h : { (TH1*) (sig_eff_100.empty()?nullptr:sig_eff_100[0]),
                        (TH1*) (sig_eff_250.empty()?nullptr:sig_eff_250[0]),
                        (TH1*) (sig_eff_400.empty()?nullptr:sig_eff_400[0]) }) pickXRange(h);

        if (xmax<=xmin) { xmin = 0; xmax = 1000; } // fallback

        TH1F* frame = (TH1F*)gPad->DrawFrame(xmin, 0.0, xmax, 1.05);
        frame->SetTitle("Emulated Trigger Efficiency (Signal);Offline Leading LRJ E_{T} [GeV];Emulated Trigger Efficiency (Signal)");
        frame->GetYaxis()->CenterTitle(true);

        // -----------------------------
        // Legends
        // -----------------------------
        TLegend* legFiles   = new TLegend(0.54, 0.20, 0.91, 0.48); // shows file (marker shapes)
        TLegend* legThresh  = new TLegend(0.57, 0.50, 0.94, 0.70); // shows threshold colors
        legFiles->SetFillStyle(0);  legFiles->SetBorderSize(0);  legFiles->SetTextSize(0.028);
        legThresh->SetFillStyle(0); legThresh->SetBorderSize(0); legThresh->SetTextSize(0.028);

        // -----------------------------
        // Draw all files × thresholds
        // -----------------------------
        // Helper to style and draw one eff hist
        auto styleAndDraw = [&](TH1F* h, int color, int mstyle) {
            if (!h) return;
            h->SetLineColor(color);
            h->SetMarkerColor(color);
            h->SetMarkerStyle(mstyle);
            h->SetMarkerSize(mSize);
            h->SetLineWidth(2);
            h->Draw("P SAME");  // use "E1 P SAME" if you want error bars visible
        };

        const int nFiles = (int)std::max({sig_eff_100.size(), sig_eff_250.size(), sig_eff_400.size()});
        for (int fileIt = 0; fileIt < nFiles; ++fileIt) {
            const int mstyle = fileMarkerStyles[fileIt % (int)fileMarkerStyles.size()];

            // Style + draw the three thresholds for this file
            if (fileIt < (int)sig_eff_100.size())  styleAndDraw(sig_eff_100[fileIt],  col100, mstyle);
            if (fileIt < (int)sig_eff_250.size())  styleAndDraw(sig_eff_250[fileIt],  col250, mstyle);
            if (fileIt < (int)sig_eff_400.size())  styleAndDraw(sig_eff_400[fileIt],  col400, mstyle);

            // Legend entry per file, using *one* of that file's objects so the marker matches.
            // We use the 100 GeV object for the entry (purely to carry the marker style).
            TH1F* rep = (fileIt < (int)sig_eff_100.size()) ? sig_eff_100[fileIt]
                        : (fileIt < (int)sig_eff_250.size()) ? sig_eff_250[fileIt]
                        : (fileIt < (int)sig_eff_400.size()) ? sig_eff_400[fileIt]
                        : nullptr;

            if (rep) {
                const std::string label =
                legendMap.count(algorithmConfigurations[fileIt])
                ? legendMap[algorithmConfigurations[fileIt]]
                : backgroundRootFileNames[fileIt];
                
                // inside the background loop, after you choose mstyle and build `label`:

                // 2) add a legend entry that is independent of the histogram colors
                auto e = legFiles->AddEntry((TObject*)nullptr, label.c_str(), "p");
                e->SetMarkerStyle(mstyle);   // 20, 24, 25 ...
                e->SetMarkerSize(bmSize);
                e->SetMarkerColor(kBlack);   // force black marker in legend
                e->SetLineColor(kBlack);     // (harmless for "p")

            }
        }

        // Threshold color key (use first file’s objects if available,
        // and draw as line entries so the legend shows color, not the file’s marker)
        if (!sig_eff_100.empty()) { auto h = sig_eff_100.front(); h->SetLineColor(col100); legThresh->AddEntry(h, "E_{T} > 100 GeV", "l"); }
        if (!sig_eff_250.empty()) { auto h = sig_eff_250.front(); h->SetLineColor(col250); legThresh->AddEntry(h, "E_{T} > 250 GeV", "l"); }
        if (!sig_eff_400.empty()) { auto h = sig_eff_400.front(); h->SetLineColor(col400); legThresh->AddEntry(h, "E_{T} > 400 GeV", "l"); }

        // Draw legends
        legFiles->Draw();
        legThresh->Draw();

        // Save
        cSig.SaveAs(outputFileDir + "sig_eff_overlay_all_files_100_250_400.pdf");
    }
    


    // Create ROC Curve Canvas with 2 pads: top (ROC) and bottom (S/B)
    TLegend* legend1 = new TLegend(0.55, 0.45, 0.85, 0.95);
    legend1->SetTextSize(0.035); // Adjust text size

    TLegend* legend1_2 = new TLegend(0.55, 0.45, 0.85, 0.95);
    legend1_2->SetTextSize(0.035); // Adjust text size

    TCanvas* roc_plot_canvas = new TCanvas("roc_plot_canvas", "ROC Curve with S/B", 710, 10, 700, 700);
    TCanvas* roc_plot_canvas2 = new TCanvas("roc_plot_canvas2", "ROC Curve with S/B", 710, 10, 700, 700);
    TPad* pad1 = new TPad("pad1", "Main ROC Curve", 0.0, 0.3, 1.0, 1.0);
    TPad* pad1_2 = new TPad("pad1_2", "Main ROC Curve", 0.0, 0.3, 1.0, 1.0);
    TPad* pad2 = new TPad("pad2", "S/B Subplot",    0.0, 0.0, 1.0, 0.3);
    TPad* pad2_2 = new TPad("pad2_2", "S/B Subplot",    0.0, 0.0, 1.0, 0.3);
    roc_plot_canvas->cd();
    pad1->SetBottomMargin(0.02);
    pad2->SetTopMargin(0.05);
    pad2->SetBottomMargin(0.3);
    pad2->Draw();
    pad1->Draw();
    

    roc_plot_canvas2->cd();
    pad1_2->SetBottomMargin(0.02);
    pad1_2->Draw();
    pad2_2->SetTopMargin(0.05);
    pad2_2->SetBottomMargin(0.3);
    pad2_2->Draw();

    TLegend* legend2 = new TLegend(0.55, 0.50, 0.85, 0.95);
    legend2->SetTextSize(0.035); // Adjust text size
    TLegend* legend2_2 = new TLegend(0.55, 0.50, 0.85, 0.95);
    legend2_2->SetTextSize(0.035); // Adjust text size
    TCanvas* eff_plot_canvas = new TCanvas("eff_plot_canvas", "Efficiency Curve", 0, 10, 700, 500);
    TCanvas* eff_plot_canvas2 = new TCanvas("eff_plot_canvas2", "Efficiency Curve", 0, 10, 700, 500);

    std::vector<TGraph*> roc_graphs;
    std::vector<TGraph*> roc_graphs2;
    std::vector<TGraph*> sbGraphs;  // S/B subplot graphs
    std::vector<TGraph*> sbGraphs2;  // S/B subplot graphs

    for (int fileIt = 0; fileIt < backgroundRootFileNames.size(); ++fileIt) {
        std::vector<double>& x = roc_curve_points_x[fileIt];
        std::vector<double>& y = roc_curve_points_y[fileIt];
        std::vector<double>& x2 = roc_curve_points_x2[fileIt];
        std::vector<double>& y2 = roc_curve_points_y2[fileIt];

        std::vector<int> goodColors = {1, 2, 4, 6, 7, 8, 9, 28, 30, 46};  // skip 0 (white), 10, etc.
        int colorIndex = goodColors[fileIt % goodColors.size()];

        // ----------------------------------------
        // ROC Curve (top pad)
        pad1->cd();
        pad1->SetLogy();
        TGraph* gr = new TGraph(x.size(), &x[0], &y[0]);
        gr->SetTitle("ROC Curve;Signal Efficiency;Background Rejection (1/FPR)");
        gr->SetMarkerColor(colorIndex);
        gr->SetLineColor(colorIndex);
        gr->GetXaxis()->SetRangeUser(0.0, 1.0);
        gr->GetYaxis()->SetRangeUser(0, 1000);
        gr->SetMarkerSize(0.25);
        gr->GetXaxis()->SetLabelSize(0);  // remove x-axis labels
        gr->GetXaxis()->SetTitleSize(0);  // remove x-axis title
        //legend1->AddEntry(gr, legendMap[algorithmConfigurations[fileIt]].c_str(), "l");
        roc_graphs.push_back(gr);

        if (fileIt == 0) gr->Draw("AL");
        else             gr->Draw("L SAME");
        gr->GetXaxis()->SetLimits(0.0, 1.0);

        // Save ONLY the top ROC (what's in pad1)
        pad1->Modified();
        pad1->Update();
        pad1->SaveAs(outputFileDir + "roc_curve_only.pdf");

        pad1_2->cd();
        pad1_2->SetLogy();
        TGraph* gr2 = new TGraph(x2.size(), &x2[0], &y2[0]);
        gr2->SetTitle("ROC Curve;Signal Efficiency;Background Rejection (1/FPR)");
        gr2->SetMarkerColor(colorIndex);
        gr2->SetLineColor(colorIndex);
        gr2->GetXaxis()->SetRangeUser(0.0, 1.0);
        gr2->GetYaxis()->SetRangeUser(0, 1000);
        gr2->SetMarkerSize(0.25);
        gr2->GetXaxis()->SetLabelSize(0);  // remove x-axis labels
        gr2->GetXaxis()->SetTitleSize(0);  // remove x-axis title
        legend1_2->AddEntry(gr2, legendMap[algorithmConfigurations[fileIt]].c_str(), "l");
        roc_graphs2.push_back(gr2);

        if (fileIt == 0) gr2->Draw("AL");
        else             gr2->Draw("L SAME");
        gr2->GetXaxis()->SetLimits(0.0, 1.0);

        // Save ONLY the other top ROC (what's in pad1_2)
        pad1_2->Modified();
        pad1_2->Update();
        pad1_2->SaveAs(outputFileDir + "roc_curve_only_2LRJs.pdf");

        // ----------------------------------------
        // S/B subplot calculation
        std::vector<double> sb_values;
        for (size_t i = 0; i < x.size(); ++i) {
            double tpr = x[i];
            double fpr = (y[i] > 0) ? 1.0 / y[i] : 1e-4;
            sb_values.push_back(tpr / fpr);
        }
        TGraph* sbGraph = new TGraph(x.size(), &x[0], &sb_values[0]);
        sbGraph->SetLineColor(colorIndex);
        sbGraph->SetLineStyle(1);
        sbGraphs.push_back(sbGraph);

        std::vector<double> sb_values2;
        for (size_t i = 0; i < x2.size(); ++i) {
            double tpr = x2[i];
            double fpr = (y2[i] > 0) ? 1.0 / y2[i] : 1e-4;
            sb_values2.push_back(tpr / fpr);
        }
        TGraph* sbGraph2 = new TGraph(x2.size(), &x2[0], &sb_values2[0]);
        sbGraph2->SetLineColor(colorIndex);
        sbGraph2->SetLineStyle(1);
        sbGraphs2.push_back(sbGraph2);

        // ----------------------------------------
        // Efficiency Curve
        eff_plot_canvas->cd();
        TGraph* gr_eff = new TGraph(efficiency_curve_points_x[fileIt].size(),
            &efficiency_curve_points_x[fileIt][0],
            &efficiency_curve_points_y[fileIt][0]);
        gr_eff->SetTitle("Efficiency Curve;E_{T} Minimum (1 LRJ);Signal Efficiency");
        gr_eff->SetMarkerColor(colorIndex);
        gr_eff->SetLineColor(colorIndex);
        gr_eff->GetYaxis()->SetRangeUser(0.0, 1);
        gr_eff->SetMarkerSize(0.25);
        legend2->AddEntry(gr_eff, legendMap[algorithmConfigurations[fileIt]].c_str(), "l");

        if (fileIt == 0) gr_eff->Draw("AL");
        else             gr_eff->Draw("L SAME");

        eff_plot_canvas2->cd();
        TGraph* gr_eff2 = new TGraph(efficiency_curve_points_x[fileIt].size(),
            &efficiency_curve_points_x[fileIt][0],
            &efficiency_curve_points_y2[fileIt][0]);
        gr_eff2->SetTitle("Efficiency Curve;E_{T} Minimum (2 LRJs);Signal Efficiency");
        gr_eff2->SetMarkerColor(colorIndex);
        gr_eff2->SetLineColor(colorIndex);
        gr_eff2->GetYaxis()->SetRangeUser(0.0, 1);
        gr_eff2->SetMarkerSize(0.25);
        legend2_2->AddEntry(gr_eff2, legendMap[algorithmConfigurations[fileIt]].c_str(), "l");

        if (fileIt == 0) gr_eff2->Draw("AL");
        else             gr_eff2->Draw("L SAME");

        //std::cout << "algorithmConfigurations[fileIt]: " << algorithmConfigurations[fileIt] << "\n";
        //std::cout << "legendMap[algorithmConfigurations[fileIt]].c_str(): " << legendMap[algorithmConfigurations[fileIt]].c_str() << "\n";

        // ----------------------------------------
        // Print max signal-to-background summary
        auto maxIt = std::max_element(maxSignalToBackgroundRatio[fileIt].begin(), maxSignalToBackgroundRatio[fileIt].end());
        auto maxIt0p1 = std::max_element(maxSignalToBackgroundRatioTPR0p1[fileIt].begin(), maxSignalToBackgroundRatioTPR0p1[fileIt].end());

        auto maxIt2 = std::max_element(maxSignalToBackgroundRatio2[fileIt].begin(), maxSignalToBackgroundRatio2[fileIt].end());
        auto maxIt0p1_2 = std::max_element(maxSignalToBackgroundRatio2TPR0p1[fileIt].begin(), maxSignalToBackgroundRatio2TPR0p1[fileIt].end());
        std::cout << "----------------------------------------------" << "\n";
        std::cout << "----------------- SUMMARY --------------------" << "\n";
        std::cout << "maxSignalToBackgroundRatio for " << legendMap[algorithmConfigurations[fileIt]].c_str() << " is: " << std::fixed << std::setprecision(8) << *maxIt << "\n";
        size_t maxIndex = std::distance(maxSignalToBackgroundRatio[fileIt].begin(), maxIt);
        std::cout << "corresponding Et cut value: " << maxIndex * 1.0 + 1.0 << "\n";
        std::cout << "and corresponding back rej rate: " << roc_curve_points_y[fileIt][maxIndex] << " and tpr: " << roc_curve_points_x[fileIt][maxIndex] << "\n";
        std::cout << "----" << "\n";
        std::cout << "maxSignalToBackgroundRatio (with tpr > 0.1) for " << legendMap[algorithmConfigurations[fileIt]].c_str() << " is: " << std::fixed << std::setprecision(8) << *maxIt0p1 << "\n";
        size_t maxIndex0p1 = std::distance(maxSignalToBackgroundRatioTPR0p1[fileIt].begin(), maxIt0p1);
        std::cout << "corresponding Et cut value: " << maxIndex0p1 * 1.0 + 1.0 << "\n";
        std::cout << "and corresponding back rej rate: " << roc_curve_points_y[fileIt][maxIndex0p1] << " and tpr: " << roc_curve_points_x[fileIt][maxIndex0p1] << "\n";
        std::cout << "----" << "\n";
        std::cout << "applying cut between 100, 200 GeV yields tpr: " << tprMinMaxCut[fileIt] << " and background rejection rate: " << 1/(fprMinMaxCut[fileIt]) << " and sig to back ratio: " << tprMinMaxCut[fileIt] / (fprMinMaxCut[fileIt]) << "\n";
        std::cout << "----" << "\n";
        size_t maxIndex0p1_2 = std::distance(maxSignalToBackgroundRatio2TPR0p1[fileIt].begin(), maxIt0p1_2);
        std::cout << "maxSignalToBackgroundRatio (2 LRJs > min Et) (with tpr > 0.1) for " << legendMap[algorithmConfigurations[fileIt]].c_str() << " is: " << std::fixed << std::setprecision(8) << *maxIt0p1_2 << "\n";
         std::cout << "corresponding Et cut value: " << maxIndex0p1_2 * 1.0 + 1.0 << "\n";
        std::cout << "----" << "\n";
        size_t maxIndex2 = std::distance(maxSignalToBackgroundRatio2[fileIt].begin(), maxIt2);
        std::cout << "maxSignalToBackgroundRatio (2 LRJs > min Et) for " << legendMap[algorithmConfigurations[fileIt]].c_str() << " is: " << std::fixed << std::setprecision(8) << *maxIt2 << "\n";
        std::cout << "corresponding Et cut value: " << maxIndex2 * 1.0 + 1.0 << "\n";
        std::cout << "----------------------------------------------" << "\n";

        // Save after final iteration
        if (fileIt == backgroundRootFileNames.size() - 1) {
            roc_plot_canvas->cd();
            pad1->cd();
            legend1->Draw();

            pad1_2->cd();
            legend1_2->Draw();

            pad2->cd();
            pad2->SetLogy(); 
            TH1F* h_frame = pad2->DrawFrame(0, 0.5, 1, 1000, ";Signal Efficiency;TPR / FPR");
            h_frame->SetTitle(";Signal Efficiency; TPR / FPR");
            //h_frame->GetYaxis()->SetTitle("Sig. / Back. Ratio");
            h_frame->GetYaxis()->SetTitleSize(0.08);
            h_frame->GetYaxis()->SetLabelSize(0.08);

            h_frame->GetXaxis()->SetTitleSize(0.10);
            h_frame->GetXaxis()->SetLabelSize(0.08);
            h_frame->GetYaxis()->CenterTitle(true);
            

            for (auto sbg : sbGraphs) sbg->Draw("L SAME");

            // After you build sbGraphs, before drawing:
            std::cout << "sbGraphs size = " << sbGraphs.size() << "\n";
            int gi = 0;
            for (auto g : sbGraphs) {
                if (!g) { std::cout << "graph["<<gi<<"] is null\n"; ++gi; continue; }
                std::cout << "graph["<<gi<<"] N=" << g->GetN() << "\n";
                for (int i = 0; i < g->GetN(); ++i) {
                    double x,y; g->GetPoint(i,x,y);
                    if (!std::isfinite(x) || !std::isfinite(y)) {
                        std::cout << "  non-finite at " << i << " -> ("<<x<<","<<y<<")\n";
                    }
                }
                ++gi;
            }

            roc_plot_canvas->SaveAs(outputFileDir + "roc_curve_with_sb_ratio.pdf");

            roc_plot_canvas2->cd();
            pad2_2->cd();
            pad2_2->SetLogy();
            TH1F* h_frame2 = pad2_2->DrawFrame(0, 0.5, 1, 1000, ";Signal Efficiency;Sig. / Back. Ratio");
            h_frame2->SetTitle(";Signal Efficiency;Sig. / Back. Ratio");
            //h_frame->GetYaxis()->SetTitle("Sig. / Back. Ratio");
            h_frame2->GetYaxis()->SetTitleSize(0.08);
            h_frame2->GetYaxis()->SetLabelSize(0.08);

            h_frame2->GetXaxis()->SetTitleSize(0.10);
            h_frame2->GetXaxis()->SetLabelSize(0.08);
            h_frame2->GetYaxis()->CenterTitle(true);
            

            for (auto sbg2 : sbGraphs2) sbg2->Draw("L SAME");

            roc_plot_canvas2->SaveAs(outputFileDir + "roc_curve_with_sb_ratio_2LRJs.pdf");

            // Use the vectors and legends you already filled on the top pads:
            SaveStandaloneROC("roc_only",
                            roc_graphs,
                            legend1,
                            outputFileDir + "roc_curve_only.pdf",
                            /*ymin=*/1.0, /*ymax=*/1e4);

            SaveStandaloneROC("roc_only_2LRJs",
                            roc_graphs2,
                            legend1_2,
                            outputFileDir + "roc_curve_only_2LRJs.pdf",
                            /*ymin=*/1.0, /*ymax=*/1e4);

            eff_plot_canvas->cd();
            legend2->Draw();
            eff_plot_canvas->SaveAs(outputFileDir + "efficiency_curve.pdf");

            eff_plot_canvas2->cd();
            legend2_2->Draw();
            eff_plot_canvas2->SaveAs(outputFileDir + "efficiency_curve_2LRJs.pdf");
        }
    }// Loop through files to create ROC, efficiency curves

    
    } // loop through files?

void callAnalyzer(bool overlayThreeFiles = false){
    //const std::string signalLargeRJetDataFileName = "/eos/home-m/mlarson/LargeRadiusJets/MemPrints/largeRJets/mc21_14TeV_hh_bbbb_vbf_novhh_largeR.dat";
    //const std::string backgroundLargeRJetDataFileName = "/eos/home-m/mlarson/LargeRadiusJets/MemPrints/largeRJets/mc21_14TeV_jj_JZ3_largeR.dat";
    constexpr bool vbfBool = true;
    std::vector<std::string > signalRootFileNames;
    std::vector<std::string > backgroundRootFileNames;
    std::string signalRootFileName; // FIXME allow for processing of multiple root files of different configurations! 
    if (vbfBool) signalRootFileName = "/home/larsonma/LargeRadiusJets/data/outputNTuples/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.root";
    else signalRootFileName = "/home/larsonma/LargeRadiusJets/data/daodNtuples/mc21_14TeV_HHbbbb_HLLHC_DAOD_NTUPLE_GEP.root";
    signalRootFileNames.push_back(signalRootFileName);
    std::string backgroundRootFileName = "/home/larsonma/LargeRadiusJets/data/outputNTuples/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.root";
    backgroundRootFileNames.push_back(backgroundRootFileName);
    analyze_files(signalRootFileNames, backgroundRootFileNames, overlayThreeFiles);
    gSystem->Exit(0);
}