
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

std::map<std::string, std::string> legendMap = {
    {"Seeds2_r2Cut0p64_maxObj128_back_noecut_ecutVal4", 
        "R^{2}_{cut} = 0.64, N_{IO} = 128, No IO E_{cut}"},

    {"Seeds2_r2Cut0p81_maxObj128_1p5back",
        "R^{2}_{cut} = 0.81, N_{IO} = 128, rMergeCut = 1.5"},
    
    {"Seeds2_r2Cut0p81_maxObj256_1p5back",
        "R^{2}_{cut} = 0.81, N_{IO} = 256, rMergeCut = 1.5"},

    {"Seeds2_r2Cut1p0_maxObj128_1p5back",
        "R^{2}_{cut} = 1.0, N_{IO} = 128, rMergeCut = 1.5"},
    
    {"Seeds2_r2Cut1p0_maxObj256_1p5back",
        "R^{2}_{cut} = 1.0, N_{IO} = 256, rMergeCut = 1.5"},

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
    


void analyze_files(std::vector<std::string > backgroundFileNames, std::vector<std::string > signalFileNames, std::string signalRootFileName, std::string backgroundRootFileName) {
    SetPlotStyle();
    TH1F* sig_h_LRJ_nmio = new TH1F("sig_h_LRJ_nmio", "Merged IOs per LRJs;Num. Merged Input Objects;Normalized # of Large R Jets / 2 IOs", 20, 0, 40);
    TH1F* sig_h_LRJ_Et = new TH1F("sig_h_LRJ_Et", "LRJ Et Distribution;E_{T} [GeV];Normalized # of Large R Jets / 10 GeV", 75, 0, 750);
    TH1F* sig_h_leading_LRJ_Et = new TH1F("sig_h_leading_LRJ_Et", "Leading LRJ Et Distribution;E_{T} [GeV];Normalized # of Lead Large R Jets / 10 GeV", 75, 0, 750);
    TH1F* sig_h_subleading_LRJ_Et = new TH1F("sig_h_subleading_LRJ_Et", "Subleading LRJ Et Distribution;E_{T} [GeV];Norm. # of Sublead Large R Jets / 10 GeV", 75, 0, 750);
    TH1F* sig_h_LRJ_E = new TH1F("sig_h_LRJ_E", "LRJ Et Distribution;Energy [GeV];Normalized # of Large R Jets / 10 GeV", 100, 0, 1000);
    TH1F* sig_h_LRJ_eta = new TH1F("sig_h_LRJ_eta", "LRJ Eta Distribution;#eta;Counts", 50, -5, 5);
    TH1F* sig_h_LRJ_phi = new TH1F("sig_h_LRJ_phi", "LRJ Phi Distribution;#phi;Counts", 32, -3.2, 3.2);

    TH1F* sig_h_leading_LRJ_gFexLRJ_deltaEt = new TH1F("sig_h_leading_LRJ_gFexLRJ_deltaEt", "#Delta E_{T} Leading gFex LRJ, Output LRJ, ;#Delta E_{T} (gFex - JetTagger) [GeV];Normalized # of Leading LRJs / 10 GeV", 50, -350, 150);
    TH1F* sig_h_leading_LRJ_offlineLRJ_deltaEt = new TH1F("sig_h_leading_LRJ_offlineLRJ_deltaEt", "#Delta E_{T} Leading Offline LRJ, Output LRJ, ;#Delta E_{T} (Offline - JetTagger) [GeV];Normalized # of Leading LRJs / 10 GeV", 40, -200, 200);

    TH1F* sig_h_leading_LRJ_gFexLRJ_deltaR = new TH1F("sig_h_leading_LRJ_gFexLRJ_deltaR", "#Delta R Leading gFex LRJ, Output LRJ, ;#Delta R (gFex, JetTagger);Normalized # of Leading LRJs / 0.2", 75, 0, 15);
    TH1F* sig_h_leading_LRJ_offlineLRJ_deltaR = new TH1F("sig_h_leading_LRJ_offlineLRJ_deltaR", "#Delta R Leading Offline LRJ, Output LRJ, ;#Delta R (Offline, JetTagger);Normalized # of Leading LRJs / 0.2", 75, 0, 15);

    TH1F* sig_h_first_LRJ_jFexSRJ_deltaR = new TH1F("sig_h_first_LRJ_jFexSRJ_deltaR", "#Delta R Leading jFex SRJ, Output LRJ, ;#Delta R (Lead. jFex SRJ, 1st JetTagger LRJ);Normalized # of Events / 0.1", 100, 0, 10);
    TH1F* sig_h_second_LRJ_jFexSRJ_deltaR = new TH1F("sig_h_second_LRJ_jFexSRJ_deltaR", "#Delta R Subleading jFex SRJ, Output LRJ, ;#Delta R (Sublead. jFex SRJ, 2nd JetTagger LRJ);Normalized # of Events / 0.1", 100, 0, 10);

    TH1F* sig_h_lead_sublead_LRJ_deltaR = new TH1F("sig_h_lead_sublead_LRJ_deltaR", "#Delta R Leading, Subleading LRJ, ;#Delta R (Lead., Sublead. Output LRJ);Normalized # of Events / 0.2", 75, 0, 15);

    TH1F* sig_h_offlineLRJ_Et_num50 = new TH1F("sig_h_offlineLRJ_Et_num50", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_offlineLRJ_Et_denom50 = new TH1F("sig_h_offlineLRJ_Et_denom50", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_num50 = new TH1F("sig_h_Avg_b_Et_num50", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_denom50 = new TH1F("sig_h_Avg_b_Et_denom50", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);

    TH1F* sig_h_offlineLRJ_Et_num100 = new TH1F("sig_h_offlineLRJ_Et_num100", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_offlineLRJ_Et_denom100 = new TH1F("sig_h_offlineLRJ_Et_denom100", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_num100 = new TH1F("sig_h_Avg_b_Et_num100", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_denom100 = new TH1F("sig_h_Avg_b_Et_denom100", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);

    TH1F* sig_h_offlineLRJ_Et_num150 = new TH1F("sig_h_offlineLRJ_Et_num150", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_offlineLRJ_Et_denom150 = new TH1F("sig_h_offlineLRJ_Et_denom150", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_num150 = new TH1F("sig_h_Avg_b_Et_num150", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_denom150 = new TH1F("sig_h_Avg_b_Et_denom150", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);

    TH1F* sig_h_offlineLRJ_Et_num200 = new TH1F("sig_h_offlineLRJ_Et_num200", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_offlineLRJ_Et_denom200 = new TH1F("sig_h_offlineLRJ_Et_denom200", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_num200 = new TH1F("sig_h_Avg_b_Et_num200", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_denom200 = new TH1F("sig_h_Avg_b_Et_denom200", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);

    TH1F* sig_h_offlineLRJ_Et_num250 = new TH1F("sig_h_offlineLRJ_Et_num250", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_offlineLRJ_Et_denom250 = new TH1F("sig_h_offlineLRJ_Et_denom250", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_num250 = new TH1F("sig_h_Avg_b_Et_num250", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_denom250 = new TH1F("sig_h_Avg_b_Et_denom250", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    
    TH1F* sig_h_offlineLRJ_Et_num300 = new TH1F("sig_h_offlineLRJ_Et_num300", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_offlineLRJ_Et_denom300 = new TH1F("sig_h_offlineLRJ_Et_denom300", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_num300 = new TH1F("sig_h_Avg_b_Et_num300", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_denom300 = new TH1F("sig_h_Avg_b_Et_denom300", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);

    TH1F* sig_h_offlineLRJ_Et_num350 = new TH1F("sig_h_offlineLRJ_Et_num350", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_offlineLRJ_Et_denom350 = new TH1F("sig_h_offlineLRJ_Et_denom350", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_num350 = new TH1F("sig_h_Avg_b_Et_num350", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_denom350 = new TH1F("sig_h_Avg_b_Et_denom350", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);

    TH1F* sig_h_offlineLRJ_Et_num400 = new TH1F("sig_h_offlineLRJ_Et_num400", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_offlineLRJ_Et_denom400 = new TH1F("sig_h_offlineLRJ_Et_denom400", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_num400 = new TH1F("sig_h_Avg_b_Et_num400", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_denom400 = new TH1F("sig_h_Avg_b_Et_denom400", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);

    TH1F* sig_h_offlineLRJ_Et_num450 = new TH1F("sig_h_offlineLRJ_Et_num450", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_offlineLRJ_Et_denom450 = new TH1F("sig_h_offlineLRJ_Et_denom450", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_num450 = new TH1F("sig_h_Avg_b_Et_num450", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_denom450 = new TH1F("sig_h_Avg_b_Et_denom450", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);

    TH1F* sig_h_offlineLRJ_Et_num500 = new TH1F("sig_h_offlineLRJ_Et_num500", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_offlineLRJ_Et_denom500 = new TH1F("sig_h_offlineLRJ_Et_denom500", "LRJ Et Distribution;Offline LRJ leading Jet E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_num500 = new TH1F("sig_h_Avg_b_Et_num500", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);
    TH1F* sig_h_Avg_b_Et_denom500 = new TH1F("sig_h_Avg_b_Et_denom500", "LRJ Et Distribution;Avg. b E_{T};Emulated Trigger Efficiency", 70, 0, 700);

    TH1F* back_h_LRJ_nmio = new TH1F("back_h_LRJ_nmio", "Merged IOs per LRJs;Num. Merged Input Objects;Normalized # of Large R Jets / 2 IOs", 20, 0, 40);
    TH1F* back_h_LRJ_Et = new TH1F("back_h_LRJ_Et", "LRJ Et Distribution;E_{T} [GeV];Normalized # of Large R Jets / 10 GeV", 75, 0, 750);
    TH1F* back_h_leading_LRJ_Et = new TH1F("back_h_leading_LRJ_Et", "Leading LRJ Et Distribution;E_{T} [GeV];Normalized # of Lead Large R Jets / 10 GeV", 75, 0, 750);
    TH1F* back_h_subleading_LRJ_Et = new TH1F("back_h_subleading_LRJ_Et", "Subleading LRJ Et Distribution;E_{T} [GeV];Norm. # of Sublead Large R Jets / 10 GeV", 75, 0, 750);
    TH1F* back_h_LRJ_E = new TH1F("back_h_LRJ_E", "LRJ Et Distribution;Energy [GeV];Normalized # of Large R Jets / 10 GeV", 100, 0, 1000);
    TH1F* back_h_LRJ_eta = new TH1F("back_h_LRJ_eta", "LRJ Eta Distribution;#eta;Counts", 50, -5, 5);
    TH1F* back_h_LRJ_phi = new TH1F("back_h_LRJ_phi", "LRJ Phi Distribution;#phi;Counts", 32, -3.2, 3.2);

    TH1F* back_h_leading_LRJ_gFexLRJ_deltaEt = new TH1F("back_h_leading_LRJ_gFexLRJ_deltaEt", "#Delta E_{T} Leading gFex LRJ, Output LRJ, ;#Delta E_{T} (gFex - JetTagger) [GeV];Normalized # of Leading LRJs / 10 GeV", 50, -350, 150);
    TH1F* back_h_leading_LRJ_offlineLRJ_deltaEt = new TH1F("back_h_leading_LRJ_offlineLRJ_deltaEt", "#Delta E_{T} Leading Offline LRJ, Output LRJ, ;#Delta E_{T} (Offline - JetTagger) [GeV];Normalized # of Leading LRJs / 10 GeV", 40, -200, 200);

    TH1F* back_h_leading_LRJ_gFexLRJ_deltaR = new TH1F("back_h_leading_LRJ_gFexLRJ_deltaR", "#Delta R Leading gFex LRJ, Output LRJ, ;#Delta R (gFex, JetTagger);Normalized # of Leading LRJs / 0.2", 75, 0, 15);
    TH1F* back_h_leading_LRJ_offlineLRJ_deltaR = new TH1F("back_h_leading_LRJ_offlineLRJ_deltaR", "#Delta R Leading Offline LRJ, Output LRJ, ;#Delta R (Offline, JetTagger);Normalized # of Leading LRJs / 0.2", 75, 0, 15);

    TH1F* back_h_first_LRJ_jFexSRJ_deltaR = new TH1F("back_h_first_LRJ_jFexSRJ_deltaR", "#Delta R Leading jFex SRJ, Output LRJ, ;#Delta R (Lead. jFex SRJ, JetTagger LRJ);Normalized # of Events / 0.1", 100, 0, 10);
    TH1F* back_h_second_LRJ_jFexSRJ_deltaR = new TH1F("back_h_second_LRJ_jFexSRJ_deltaR", "#Delta R Subleading jFex SRJ, Output LRJ, ;#Delta R (Sublead. jFex SRJ, JetTagger LRJ);Normalized # of Events / 0.1", 100, 0, 10);

    TH1F* back_h_lead_sublead_LRJ_deltaR = new TH1F("back_h_lead_sublead_LRJ_deltaR", "#Delta R Leading, Subleading LRJ, ;#Delta R (Lead., Sublead. Output LRJ);Normalized # of Events / 0.2", 75, 0, 15);

    // Open input ROOT file
    TFile* signalInputFile = TFile::Open(signalRootFileName.c_str(), "READ");
    TFile* backgroundInputFile = TFile::Open(backgroundRootFileName.c_str(), "READ");
    if ((!signalInputFile || signalInputFile->IsZombie()) || (!backgroundInputFile || backgroundInputFile->IsZombie())) {
        std::cerr << "Error: Could not open file " << signalRootFileName << std::endl;
        return;
    }

    TTree* truthbTreeSignal = (TTree*)signalInputFile->Get("truthbTree");
    TTree* truthHiggsTreeSignal = (TTree*)signalInputFile->Get("truthHiggsTree");
    TTree* caloTopoTowerTreeSignal = (TTree*)signalInputFile->Get("caloTopoTowerTree");
    TTree* topo422TreeSignal = (TTree*)signalInputFile->Get("topo422Tree");
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

    TTree* caloTopoTowerTreeBack = (TTree*)backgroundInputFile->Get("caloTopoTowerTree");
    TTree* topo422TreeBack = (TTree*)backgroundInputFile->Get("topo422Tree");
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
    std::vector<double>* recoAntiKt10LRJLeadingEtValuesSignal = nullptr;
    std::vector<double>* recoAntiKt10LRJLeadingEtaValuesSignal = nullptr;
    std::vector<double>* recoAntiKt10LRJLeadingPhiValuesSignal = nullptr;
    std::vector<double>* recoAntiKt10LRJSubleadingEtValuesSignal = nullptr;
    std::vector<double>* recoAntiKt10LRJSubleadingEtaValuesSignal = nullptr;
    std::vector<double>* recoAntiKt10LRJSubleadingPhiValuesSignal = nullptr;
    std::vector<unsigned int>* truthAntiKt4WZSRJEtIndexValuesSignal = nullptr;
    std::vector<double>* truthAntiKt4WZSRJEtValuesSignal = nullptr;
    std::vector<double>* truthAntiKt4WZSRJEtaValuesSignal = nullptr;
    std::vector<double>* truthAntiKt4WZSRJPhiValuesSignal = nullptr;
    std::vector<double>* truthAntiKt4WZSRJLeadingEtValuesSignal = nullptr;
    std::vector<double>* truthAntiKt4WZSRJLeadingEtaValuesSignal = nullptr;
    std::vector<double>* truthAntiKt4WZSRJLeadingPhiValuesSignal = nullptr;
    std::vector<double>* truthAntiKt4WZSRJSubleadingEtValuesSignal = nullptr;
    std::vector<double>* truthAntiKt4WZSRJSubleadingEtaValuesSignal = nullptr;
    std::vector<double>* truthAntiKt4WZSRJSubleadingPhiValuesSignal = nullptr;
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
    std::vector<double>* caloTopoTowerEtValuesBack = nullptr;
    std::vector<double>* caloTopoTowerEtaValuesBack = nullptr;
    std::vector<double>* caloTopoTowerPhiValuesBack = nullptr;
    std::vector<double>* topo422EtValuesBack = nullptr;
    std::vector<double>* topo422EtaValuesBack = nullptr;
    std::vector<double>* topo422PhiValuesBack = nullptr;
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
    std::vector<double>* recoAntiKt10LRJLeadingEtValuesBack = nullptr;
    std::vector<double>* recoAntiKt10LRJLeadingEtaValuesBack = nullptr;
    std::vector<double>* recoAntiKt10LRJLeadingPhiValuesBack = nullptr;
    std::vector<double>* recoAntiKt10LRJSubleadingEtValuesBack = nullptr;
    std::vector<double>* recoAntiKt10LRJSubleadingEtaValuesBack = nullptr;
    std::vector<double>* recoAntiKt10LRJSubleadingPhiValuesBack = nullptr;
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

    // === leadingRecoAntiKt10UFOCSSKJetsSignal ===
    leadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Et", &recoAntiKt10LRJLeadingEtValuesSignal);
    leadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Eta", &recoAntiKt10LRJLeadingEtaValuesSignal);
    leadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Phi", &recoAntiKt10LRJLeadingPhiValuesSignal);

    // === subleadingRecoAntiKt10UFOCSSKJetsSignal ===
    subleadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Et", &recoAntiKt10LRJSubleadingEtValuesSignal);
    subleadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Eta", &recoAntiKt10LRJSubleadingEtaValuesSignal);
    subleadingRecoAntiKt10UFOCSSKJetsSignal->SetBranchAddress("Phi", &recoAntiKt10LRJSubleadingPhiValuesSignal);

    // === truthAntiKt4TruthDressedWZJetsSignal ===
    truthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("EtIndex", &truthAntiKt4WZSRJEtIndexValuesSignal);
    truthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Et", &truthAntiKt4WZSRJEtValuesSignal);
    truthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Eta", &truthAntiKt4WZSRJEtaValuesSignal);
    truthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Phi", &truthAntiKt4WZSRJPhiValuesSignal);

    // === leadingTruthAntiKt4TruthDressedWZJetsSignal ===
    leadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Et", &truthAntiKt4WZSRJLeadingEtValuesSignal);
    leadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Eta", &truthAntiKt4WZSRJLeadingEtaValuesSignal);
    leadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Phi", &truthAntiKt4WZSRJLeadingPhiValuesSignal);

    // === subleadingTruthAntiKt4TruthDressedWZJetsSignal ===
    subleadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Et", &truthAntiKt4WZSRJSubleadingEtValuesSignal);
    subleadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Eta", &truthAntiKt4WZSRJSubleadingEtaValuesSignal);
    subleadingTruthAntiKt4TruthDressedWZJetsSignal->SetBranchAddress("Phi", &truthAntiKt4WZSRJSubleadingPhiValuesSignal);

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

    // === caloTopoTowerTreeBack ===
    caloTopoTowerTreeBack->SetBranchAddress("Et", &caloTopoTowerEtValuesBack);
    caloTopoTowerTreeBack->SetBranchAddress("Eta", &caloTopoTowerEtaValuesBack);
    caloTopoTowerTreeBack->SetBranchAddress("Phi", &caloTopoTowerPhiValuesBack);

    // === topo422TreeBack ===
    topo422TreeBack->SetBranchAddress("Et", &topo422EtValuesBack);
    topo422TreeBack->SetBranchAddress("Eta", &topo422EtaValuesBack);
    topo422TreeBack->SetBranchAddress("Phi", &topo422PhiValuesBack);

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

    // === leadingRecoAntiKt10UFOCSSKJetsBack ===
    leadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Et", &recoAntiKt10LRJLeadingEtValuesBack);
    leadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Eta", &recoAntiKt10LRJLeadingEtaValuesBack);
    leadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Phi", &recoAntiKt10LRJLeadingPhiValuesBack);

    // === subleadingRecoAntiKt10UFOCSSKJetsBack ===
    subleadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Et", &recoAntiKt10LRJSubleadingEtValuesBack);
    subleadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Eta", &recoAntiKt10LRJSubleadingEtaValuesBack);
    subleadingRecoAntiKt10UFOCSSKJetsBack->SetBranchAddress("Phi", &recoAntiKt10LRJSubleadingPhiValuesBack);

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



    //gSystem->RedirectOutput("output3.log", "w");
    const int num_processed_events = 10000;

    // Variables to store data
    std::vector<double> backgroundEtValues, backgroundEtaValues, backgroundPhiValues;
    std::vector<double> signalEtValues, signalEtaValues, signalPhiValues;

    // Open input file

    int current_event = -1;
    std::string sig_line;
    std::string back_line;
    std::vector<std::string > algorithmConfigurations;

    vector<vector<double> > roc_curve_points_x(backgroundFileNames.size());
    vector<vector<double> > roc_curve_points_y(backgroundFileNames.size());
    vector<vector<double> > maxSignalToBackgroundRatio(backgroundFileNames.size());
    vector<vector<double> > maxSignalToBackgroundRatioTPR0p1(backgroundFileNames.size());

    vector<vector<double> > efficiency_curve_points_x(backgroundFileNames.size());
    vector<vector<double> > efficiency_curve_points_y(backgroundFileNames.size());
    vector<double > tprMinMaxCut(backgroundFileNames.size());
    vector<double > fprMinMaxCut(backgroundFileNames.size());
    // Bools determining which file being processed

    for (int fileIt = 0; fileIt < backgroundFileNames.size(); ++fileIt){
        //if (fileIt > 0) break;
        vector<vector<double> > sig_LRJ_Et(num_processed_events);
        vector<vector<double> > back_LRJ_Et(num_processed_events);

        vector<vector<double> > sig_LRJ_Eta(num_processed_events);
        vector<vector<double> > back_LRJ_Eta(num_processed_events);

        vector<vector<double> > sig_LRJ_Phi(num_processed_events);
        vector<vector<double> > back_LRJ_Phi(num_processed_events);
        //std::cout << "fileIt : " << fileIt << "\n";
        // Find position of "largeR"
        std::string key = "largeRn";
        size_t pos = backgroundFileNames[fileIt].find(key);

        if (pos != std::string::npos) {
            // Start after "largeR"
            size_t start = pos + key.length();
            
            // Remove .dat by finding its position
            size_t end =  backgroundFileNames[fileIt].rfind(".dat");

            if (end != std::string::npos && end > start) {
                algorithmConfigurations.push_back( backgroundFileNames[fileIt].substr(start, end - start));
                //std::cout << "Extracted string: " << backgroundFileNames[fileIt].substr(start, end - start) << std::endl;
            } else {
                std::cerr << ".dat not found or too early in string.\n";
            }
        } else {
            std::cerr << "\"largeR\" not found in string.\n";
        }

        /// FILEIT LOOP HERE
    
        std::ifstream sig_infile(signalFileNames[fileIt]);
        std::ifstream back_infile(backgroundFileNames[fileIt]);
        //std::cout << "processing: " << signalFileNames[fileIt] << " and: " << backgroundFileNames[fileIt] << "\n";
        if (!sig_infile.is_open()) { 
            std::cerr << "Error: Could not open file " << signalFileNames[fileIt] << std::endl;
            return;
        }
        if (!back_infile.is_open()){
            std::cerr << "Error: Could not open file " << backgroundFileNames[fileIt] << std::endl;
            return;
        }

        std::vector<double > sigEtValues;
        std::vector<double > backEtValues;
        while (std::getline(sig_infile, sig_line) && std::getline(back_infile, back_line)) {
            // Skip event header lines
            
            if (sig_line.find("Event") != std::string::npos && back_line.find("Event") != std::string::npos) {
                current_event++;
                // Extract the event number
                std::stringstream ss(sig_line);
                std::string dummy, event_label;
                ss >> dummy >> event_label >> current_event;
                if (current_event != 0){
                    std::sort(sigEtValues.begin(), sigEtValues.end(), std::greater<double>());
                    std::sort(backEtValues.begin(), backEtValues.end(), std::greater<double>());
                    //std::cout << "sigEtValues[0]: " << sigEtValues[0] << " and sigEtValues[1]: " << sigEtValues[1] << "\n";
                    back_h_leading_LRJ_Et->Fill(backEtValues[0]);
                    sig_h_leading_LRJ_Et->Fill(sigEtValues[0]);
                    back_h_subleading_LRJ_Et->Fill(backEtValues[1]);
                    sig_h_subleading_LRJ_Et->Fill(sigEtValues[1]);
                    sigEtValues.clear();
                    backEtValues.clear();
                }
                
                //std::cout << "Processing Event: " << current_event << std::endl;
                continue;
            }
            if (current_event >= num_processed_events) break;
            //std::cout << "current_event: " << current_event << "\n";
            // Extract the binary word
            std::stringstream sig_ss(sig_line);
            std::string sig_index, sig_binary, sig_hex_word;
            sig_ss >> sig_index >> sig_binary >> sig_hex_word;

            // Find positions of pipe separators
            size_t first_pipe = sig_binary.find('|');
            size_t second_pipe = sig_binary.find('|', first_pipe + 1);
            size_t third_pipe = sig_binary.find('|', second_pipe + 1);

            // Extract binary substrings
            std::string nmio_bin = sig_binary.substr(0, first_pipe);
            std::string et_bin   = sig_binary.substr(first_pipe + 1, second_pipe - first_pipe - 1);
            std::string eta_bin  = sig_binary.substr(second_pipe + 1, third_pipe - second_pipe - 1);
            std::string phi_bin  = sig_binary.substr(third_pipe + 1);

            //std::cout << nmio_bin << "|" << et_bin << "|" << eta_bin << "|" << phi_bin << "\n";

            // Convert to bitsets
            std::bitset<io_bit_length_>   nmio_bits(nmio_bin);
            std::bitset<et_bit_length_>   et_bits(et_bin);
            std::bitset<eta_bit_length_>  eta_bits(eta_bin);
            std::bitset<phi_bit_length_>  phi_bits(phi_bin);

            // Undigitize values
            double sig_undigitized_nmio = undigitize_nmio(nmio_bits);
            double sig_undigitized_et   = undigitize_et(et_bits);
            double sig_undigitized_eta  = undigitize_eta(eta_bits);
            double sig_undigitized_phi  = undigitize_phi(phi_bits);

            //std::cout << "et_bits : " << et_bits << " and undigitized et : " << sig_undigitized_et << "\n";

            sig_h_LRJ_nmio->Fill(sig_undigitized_nmio);
            sig_h_LRJ_Et->Fill(sig_undigitized_et);
            sigEtValues.push_back(sig_undigitized_et);
            sig_h_LRJ_E->Fill(sig_undigitized_et * cosh(sig_undigitized_eta));
            //std::cout << "current_event: " << current_event << "\n";
            sig_LRJ_Et[current_event].push_back(sig_undigitized_et);
            sig_LRJ_Eta[current_event].push_back(sig_undigitized_eta);
            sig_LRJ_Phi[current_event].push_back(sig_undigitized_phi);
            sig_h_LRJ_eta->Fill(sig_undigitized_eta);
            sig_h_LRJ_phi->Fill(sig_undigitized_phi);

            // Extract the binary word
            std::stringstream back_ss(back_line);
            std::string back_index, back_binary, back_hex_word;
            back_ss >> back_index >> back_binary >> back_hex_word;

            // Find positions of pipe separators
            size_t back_first_pipe  = back_binary.find('|');
            size_t back_second_pipe = back_binary.find('|', back_first_pipe + 1);
            size_t back_third_pipe  = back_binary.find('|', back_second_pipe + 1);

            // Extract binary substrings
            std::string back_nmio_bin = back_binary.substr(0, back_first_pipe);
            std::string back_et_bin   = back_binary.substr(back_first_pipe + 1, back_second_pipe - back_first_pipe - 1);
            std::string back_eta_bin  = back_binary.substr(back_second_pipe + 1, back_third_pipe - back_second_pipe - 1);
            std::string back_phi_bin  = back_binary.substr(back_third_pipe + 1);

            // Convert to bitsets
            std::bitset<io_bit_length_>   back_nmio_bits(back_nmio_bin);
            //std::cout << "nmio_bits: " << back_nmio_bits << "\n";
            std::bitset<et_bit_length_>   back_et_bits(back_et_bin);
            std::bitset<eta_bit_length_>  back_eta_bits(back_eta_bin);
            std::bitset<phi_bit_length_>  back_phi_bits(back_phi_bin);

            // Undigitize the values
            double back_undigitized_nmio = undigitize_nmio(back_nmio_bits);
            //std::cout << "back_undigitized_nmio: " << back_undigitized_nmio << "\n";
            double back_undigitized_et   = undigitize_et(back_et_bits);
            //std::cout << "et_bits : " << back_et_bits << " and undigitized et : " << back_undigitized_et << "\n";
            double back_undigitized_eta  = undigitize_eta(back_eta_bits);
            double back_undigitized_phi  = undigitize_phi(back_phi_bits);
            back_h_LRJ_nmio->Fill(back_undigitized_nmio);
            back_h_LRJ_Et->Fill(back_undigitized_et);
            backEtValues.push_back(back_undigitized_et);
            back_h_LRJ_E->Fill(back_undigitized_et * cosh(back_undigitized_eta));
            back_LRJ_Et[current_event].push_back(back_undigitized_et);
            back_LRJ_Eta[current_event].push_back(back_undigitized_eta);
            back_LRJ_Phi[current_event].push_back(back_undigitized_phi);
            back_h_LRJ_eta->Fill(back_undigitized_phi);
            back_h_LRJ_phi->Fill(back_undigitized_eta);

        }

        /*for (int i = 0; i < nSeeds_; i++){ // loop not needed for just 2 seeds considered
            if (i == 0){
                
            }
            if (i == 1){
                
            }
            
        } */
       // === gFexLeadingLRJTreeSignal ===
        for (int i = 0; i < num_processed_events; i++) {
            leadingRecoAntiKt10UFOCSSKJetsSignal->GetEntry(i);
            leadingRecoAntiKt10UFOCSSKJetsBack->GetEntry(i);
            gFexLeadingLRJTreeSignal->GetEntry(i);
            gFexLeadingLRJTreeBack->GetEntry(i);
            jFexLeadingSRJTreeSignal->GetEntry(i);
            jFexSubleadingSRJTreeSignal->GetEntry(i);
            jFexLeadingSRJTreeBack->GetEntry(i);
            jFexSubleadingSRJTreeBack->GetEntry(i);
            truthbTreeSignal->GetEntry(i);
            unsigned int highestEtIndexLRJSig = -1;
            unsigned int highestEtIndexLRJBack = -1;
            if(sig_LRJ_Et[i][0] >= sig_LRJ_Et[i][1]) highestEtIndexLRJSig = 0;
            else highestEtIndexLRJSig = 1;

            if(back_LRJ_Et[i][0] >= back_LRJ_Et[i][1]) highestEtIndexLRJBack = 0;
            else highestEtIndexLRJBack = 1;

            if(recoAntiKt10LRJLeadingEtValuesSignal->size() == 0) continue;
            

            sig_h_leading_LRJ_gFexLRJ_deltaEt->Fill(gFexLRJLeadingEtValuesSignal->at(0) - std::max(sig_LRJ_Et[i][0], sig_LRJ_Et[i][1]));
            back_h_leading_LRJ_gFexLRJ_deltaEt->Fill(gFexLRJLeadingEtValuesBack->at(0) - std::max(back_LRJ_Et[i][0], back_LRJ_Et[i][1]));
            
            
            sig_h_leading_LRJ_gFexLRJ_deltaR->Fill(calcDeltaR2(gFexLRJLeadingEtaValuesSignal->at(0), gFexLRJLeadingPhiValuesSignal->at(0), sig_LRJ_Eta[i][highestEtIndexLRJSig], sig_LRJ_Phi[i][highestEtIndexLRJSig]));
            back_h_leading_LRJ_gFexLRJ_deltaR->Fill(calcDeltaR2(gFexLRJLeadingEtaValuesBack->at(0), gFexLRJLeadingPhiValuesBack->at(0), back_LRJ_Eta[i][highestEtIndexLRJBack], back_LRJ_Phi[i][highestEtIndexLRJBack]));

            sig_h_first_LRJ_jFexSRJ_deltaR->Fill(calcDeltaR2(jFexSRJLeadingEtaValuesSignal->at(0), jFexSRJLeadingPhiValuesSignal->at(0), sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0]));
            sig_h_second_LRJ_jFexSRJ_deltaR->Fill(calcDeltaR2(jFexSRJSubleadingEtaValuesSignal->at(0), jFexSRJSubleadingPhiValuesSignal->at(0), sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1]));

            sig_h_leading_LRJ_offlineLRJ_deltaEt->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0) - std::max(sig_LRJ_Et[i][0], sig_LRJ_Et[i][1]));
            back_h_leading_LRJ_offlineLRJ_deltaEt->Fill(recoAntiKt10LRJLeadingEtValuesBack->at(0) - std::max(back_LRJ_Et[i][0], back_LRJ_Et[i][1]));

            sig_h_leading_LRJ_offlineLRJ_deltaR->Fill(calcDeltaR2(recoAntiKt10LRJLeadingEtaValuesSignal->at(0), recoAntiKt10LRJLeadingPhiValuesSignal->at(0), sig_LRJ_Eta[i][highestEtIndexLRJSig], sig_LRJ_Phi[i][highestEtIndexLRJSig]));
            back_h_leading_LRJ_offlineLRJ_deltaR->Fill(calcDeltaR2(recoAntiKt10LRJLeadingEtaValuesBack->at(0), recoAntiKt10LRJLeadingPhiValuesBack->at(0), back_LRJ_Eta[i][highestEtIndexLRJBack], back_LRJ_Phi[i][highestEtIndexLRJBack]));

            back_h_first_LRJ_jFexSRJ_deltaR->Fill(calcDeltaR2(jFexSRJLeadingEtaValuesBack->at(0), jFexSRJLeadingPhiValuesBack->at(0), back_LRJ_Eta[i][0], back_LRJ_Phi[i][0]));
            back_h_second_LRJ_jFexSRJ_deltaR->Fill(calcDeltaR2(jFexSRJSubleadingEtaValuesBack->at(0), jFexSRJSubleadingPhiValuesBack->at(0), back_LRJ_Eta[i][1], back_LRJ_Phi[i][1]));
            
            sig_h_lead_sublead_LRJ_deltaR->Fill(calcDeltaR2(sig_LRJ_Eta[i][0], sig_LRJ_Phi[i][0], sig_LRJ_Eta[i][1], sig_LRJ_Phi[i][1]));
            back_h_lead_sublead_LRJ_deltaR->Fill(calcDeltaR2(back_LRJ_Eta[i][0], back_LRJ_Phi[i][0], back_LRJ_Eta[i][1], back_LRJ_Phi[i][1]));

            if(sig_LRJ_Et[i][0] >= 50.0 || sig_LRJ_Et[i][1] >= 50.0){
                sig_h_offlineLRJ_Et_num50->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                sig_h_Avg_b_Et_num50->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
            sig_h_offlineLRJ_Et_denom50->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
            sig_h_Avg_b_Et_denom50->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);

            if(sig_LRJ_Et[i][0] >= 100.0 || sig_LRJ_Et[i][1] >= 100.0){
                sig_h_offlineLRJ_Et_num100->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
                sig_h_Avg_b_Et_num100->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
            sig_h_offlineLRJ_Et_denom100->Fill(recoAntiKt10LRJLeadingEtValuesSignal->at(0));
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
                sig_h_Avg_b_Et_num250->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
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
                sig_h_Avg_b_Et_num400->Fill((truthbquarksEtValuesSignal->at(0) + truthbquarksEtValuesSignal->at(1))/2);
            }
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
            
        }




        double det_cutoff = 1.0;
        for (double et_cutoff = det_cutoff; et_cutoff < 500.0; et_cutoff += det_cutoff) {
            


            //std::cout << "back_LRJ_Et.size(): " << back_LRJ_Et.size() << " and sig_LRJ_Et.size(): " << sig_LRJ_Et.size() << "\n";
            int numTruePositive = 0;
            
            for (int i = 0; i < num_processed_events; i++) {
                //std::cout << "i: " << i << "\n";
                //std::cout << "i:  " << i << "\n";
                //for (int j = 0; j < sig_LRJ_Et[current_event]; j++){
                //   if ()
                //}
                //std::cout << "sig_LRJ_Et[i][0]: " << sig_LRJ_Et[i][0] << " and i " << i << "\n";
                if(sig_LRJ_Et[i][0] >= et_cutoff || sig_LRJ_Et[i][1] >= et_cutoff){
                    numTruePositive++;
                }
                    //if (energyMin(sig_LRJ_Et->at(i),
                    //                    et_cutoff)) {
                    //numTruePositive++;
                    //}

                
                
            }
            double truePositiveRate = ((double)numTruePositive)/(num_processed_events);
            //std::cout << "truePositiveRate: " << truePositiveRate << " for et_cutoff: " << et_cutoff << "\n";
            roc_curve_points_x[fileIt].emplace_back(truePositiveRate);

            efficiency_curve_points_x[fileIt].emplace_back(et_cutoff);
            efficiency_curve_points_y[fileIt].emplace_back(truePositiveRate);

            int numFalsePositive = 0;
            for (int i = 0; i < num_processed_events; i++) {
                //std::cout << "back_LRJ_Et[i][0]: " << back_LRJ_Et[i][0] << " and i : " << i << "\n";
                if(back_LRJ_Et[i][0] >= et_cutoff || back_LRJ_Et[i][1] >= et_cutoff){
                    numFalsePositive++;
                }
                /*if (useMax_){
                    if (energyMax(back_LRJ_Et->at(i),
                                        et_cutoff)) {
                    numFalsePositive++;
                    }
                }
                else{
                    if (energyMin(back_LRJ_Et->at(i),
                                        et_cutoff)) {
                    numFalsePositive++;
                    }
                }*/
                
            }

            double falsePositiveRate = ((double)numFalsePositive)/((double) num_processed_events);
            if (falsePositiveRate == 0) falsePositiveRate = 0.1; 
            double backgroundRejection = 1.0 / falsePositiveRate;
            //std::cout << " backgroundRejectioN: "  << backgroundRejection << " for et_cutoff: " << et_cutoff << " and file: " << algorithmConfigurations[fileIt] << "\n";
            roc_curve_points_y[fileIt].emplace_back(backgroundRejection);
            //std::cout << "signal to background: " << std::fixed << std::setprecision(16) << backgroundRejection * truePositiveRate << "\n";
            if (truePositiveRate > 0.1) maxSignalToBackgroundRatioTPR0p1[fileIt].push_back(backgroundRejection * truePositiveRate);
            maxSignalToBackgroundRatio[fileIt].push_back(backgroundRejection * truePositiveRate);
        }
        int numTruePositiveMinMaxCut = 0;
        int numFalsePositiveMinMaxCut = 0;
        for (int i = 0; i < num_processed_events; i++) {
            if ((sig_LRJ_Et[i][0] > 100 && sig_LRJ_Et[i][0] < 200) && (sig_LRJ_Et[i][1] > 100 && sig_LRJ_Et[i][1] < 200)){
                numTruePositiveMinMaxCut++;
            }
        }
        for (int i = 0; i < num_processed_events; i++) {
            if ((back_LRJ_Et[i][0] > 100 && back_LRJ_Et[i][0] < 200) && (back_LRJ_Et[i][1] > 100 && back_LRJ_Et[i][1] < 200)){
                numFalsePositiveMinMaxCut++;
            }
        }
        tprMinMaxCut[fileIt] = double(numTruePositiveMinMaxCut)/ double(num_processed_events );
        fprMinMaxCut[fileIt] = double(numFalsePositiveMinMaxCut)/ double(num_processed_events);



    

        // Save histograms
        TCanvas c;
        TCanvas cSigEffb;
        TCanvas cSigEffLRJ;
        TString outputFileDir = "overlayLargeRJetHistogramsrMergeScan/";
        TString modifiedOutputFileDir = "overlayLargeRJetHistogramsrMergeScan_" + algorithmConfigurations[fileIt] + "/";
        gSystem->mkdir(modifiedOutputFileDir);
        //gStyle->SetOptTitle(1); // Enable title
        TLegend *leg = new TLegend(0.8,0.8,0.95,0.95);
        leg->SetTextSize(0.025);

        leg->AddEntry(sig_h_LRJ_Et, "Signal", "l");
        leg->AddEntry(back_h_LRJ_Et, "Background", "l");

        TLegend *legEffb = new TLegend(0.8,0.2,0.95,0.35);
        legEffb->SetTextSize(0.025);

        TLegend *legEffLRJ = new TLegend(0.7,0.2,1.0,0.525);
        legEffLRJ->SetTextSize(0.03);

        c.cd();

        //std::string title = "Normalized E_{T} Distribution for " + algorithmConfigurations[fileIt];
        //back_h_LRJ_Et->SetTitle(title.c_str());
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

        sig_h_leading_LRJ_gFexLRJ_deltaEt->Scale(1.0 / sig_h_leading_LRJ_gFexLRJ_deltaEt->Integral());
        back_h_leading_LRJ_gFexLRJ_deltaEt->Scale(1.0 / back_h_leading_LRJ_gFexLRJ_deltaEt->Integral());
        sig_h_leading_LRJ_gFexLRJ_deltaEt->SetLineColor(kRed);
        back_h_leading_LRJ_gFexLRJ_deltaEt->SetLineColor(kBlue);
        back_h_leading_LRJ_gFexLRJ_deltaEt->Draw("HIST");
        sig_h_leading_LRJ_gFexLRJ_deltaEt->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_LRJ_gFexLRJ_deltaEt.pdf");

        sig_h_leading_LRJ_offlineLRJ_deltaEt->Scale(1.0 / sig_h_leading_LRJ_offlineLRJ_deltaEt->Integral());
        back_h_leading_LRJ_offlineLRJ_deltaEt->Scale(1.0 / back_h_leading_LRJ_offlineLRJ_deltaEt->Integral());
        sig_h_leading_LRJ_offlineLRJ_deltaEt->SetLineColor(kRed);
        back_h_leading_LRJ_offlineLRJ_deltaEt->SetLineColor(kBlue);
        back_h_leading_LRJ_offlineLRJ_deltaEt->Draw("HIST");
        sig_h_leading_LRJ_offlineLRJ_deltaEt->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_LRJ_offlineLRJ_deltaEt.pdf");

        sig_h_leading_LRJ_gFexLRJ_deltaR->Scale(1.0 / sig_h_leading_LRJ_gFexLRJ_deltaR->Integral());
        back_h_leading_LRJ_gFexLRJ_deltaR->Scale(1.0 / back_h_leading_LRJ_gFexLRJ_deltaR->Integral());
        sig_h_leading_LRJ_gFexLRJ_deltaR->SetLineColor(kRed);
        back_h_leading_LRJ_gFexLRJ_deltaR->SetLineColor(kBlue);
        back_h_leading_LRJ_gFexLRJ_deltaR->Draw("HIST");
        sig_h_leading_LRJ_gFexLRJ_deltaR->Draw("HIST SAME");
        
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
        back_h_first_LRJ_jFexSRJ_deltaR->Draw("HIST");
        sig_h_first_LRJ_jFexSRJ_deltaR->Draw("HIST SAME");
        
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

        std::cout << "Leading signal mean: " << sig_h_leading_LRJ_Et->GetMean() << " and leading background mean: " << back_h_leading_LRJ_Et->GetMean() << "\n";
        sig_h_leading_LRJ_Et->Scale(1.0 / sig_h_leading_LRJ_Et->Integral());
        back_h_leading_LRJ_Et->Scale(1.0 / back_h_leading_LRJ_Et->Integral());
        sig_h_leading_LRJ_Et->SetLineColor(kRed);
        back_h_leading_LRJ_Et->SetLineColor(kBlue);
        back_h_leading_LRJ_Et->Draw("HIST");
        sig_h_leading_LRJ_Et->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "leading_LRJ_Et.pdf");

        std::cout << "Subleading signal mean: " << sig_h_subleading_LRJ_Et->GetMean() << " and subleading background mean: " << back_h_subleading_LRJ_Et->GetMean() << "\n";
        sig_h_subleading_LRJ_Et->Scale(1.0 / sig_h_subleading_LRJ_Et->Integral());
        back_h_subleading_LRJ_Et->Scale(1.0 / back_h_subleading_LRJ_Et->Integral());
        sig_h_subleading_LRJ_Et->SetLineColor(kRed);
        back_h_subleading_LRJ_Et->SetLineColor(kBlue);
        
        sig_h_subleading_LRJ_Et->Draw("HIST");
        back_h_subleading_LRJ_Et->Draw("HIST SAME");
        
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "subleading_LRJ_Et.pdf");


        sig_h_LRJ_E->Scale(1.0 / sig_h_LRJ_E->Integral());
        back_h_LRJ_E->Scale(1.0 / back_h_LRJ_E->Integral());
        sig_h_LRJ_E->SetLineColor(kRed);
        back_h_LRJ_E->SetLineColor(kBlue);
        sig_h_LRJ_E->Draw("HIST");
        back_h_LRJ_E->Draw("HIST SAME");
        
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "LRJ_Energy.pdf");


        sig_h_LRJ_nmio->Scale(1.0 / sig_h_LRJ_nmio->Integral());
        back_h_LRJ_nmio->Scale(1.0 / back_h_LRJ_nmio->Integral());
        sig_h_LRJ_nmio->SetLineColor(kRed);
        back_h_LRJ_nmio->SetLineColor(kBlue);
        
        back_h_LRJ_nmio->Draw("HIST");
        sig_h_LRJ_nmio->Draw("HIST SAME");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "LRJ_nmio.pdf");

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

        TH1F* sig_eff_offlineLRJ50 = (TH1F*)sig_h_offlineLRJ_Et_num50->Clone();
        sig_eff_offlineLRJ50->SetName("eff_LRJ50");
        sig_eff_offlineLRJ50->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_offlineLRJ50->Divide(sig_h_offlineLRJ_Et_num50, sig_h_offlineLRJ_Et_denom50, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ50->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ50->Draw();
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_offlineLRJ50.pdf");

        TH1F* sig_eff_b_Et50 = (TH1F*)sig_h_Avg_b_Et_num50->Clone();
        sig_eff_b_Et50->SetName("eff_b50");
        sig_eff_b_Et50->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_b_Et50->Divide(sig_h_Avg_b_Et_num50, sig_h_Avg_b_Et_denom50, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et50->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et50->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_b_Et50.pdf");

        TH1F* sig_eff_offlineLRJ100 = (TH1F*)sig_h_offlineLRJ_Et_num100->Clone();
        sig_eff_offlineLRJ100->SetName("eff_LRJ100");
        sig_eff_offlineLRJ100->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_offlineLRJ100->Divide(sig_h_offlineLRJ_Et_num100, sig_h_offlineLRJ_Et_denom100, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ100->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ100->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_offlineLRJ100.pdf");

        TH1F* sig_eff_b_Et100 = (TH1F*)sig_h_Avg_b_Et_num100->Clone();
        sig_eff_b_Et100->SetName("eff_b100");
        sig_eff_b_Et100->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_b_Et100->Divide(sig_h_Avg_b_Et_num100, sig_h_Avg_b_Et_denom100, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et100->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et100->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_b_Et100.pdf");

        TH1F* sig_eff_offlineLRJ150 = (TH1F*)sig_h_offlineLRJ_Et_num150->Clone();
        sig_eff_offlineLRJ150->SetName("eff_LRJ150");
        sig_eff_offlineLRJ150->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_offlineLRJ150->Divide(sig_h_offlineLRJ_Et_num150, sig_h_offlineLRJ_Et_denom150, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ150->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_offlineLRJ150.pdf");

        TH1F* sig_eff_b_Et150 = (TH1F*)sig_h_Avg_b_Et_num150->Clone();
        sig_eff_b_Et150->SetName("eff_b150");
        sig_eff_b_Et150->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_b_Et150->Divide(sig_h_Avg_b_Et_num150, sig_h_Avg_b_Et_denom150, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et150->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et150->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_b_Et150.pdf");

        TH1F* sig_eff_offlineLRJ200 = (TH1F*)sig_h_offlineLRJ_Et_num200->Clone();
        sig_eff_offlineLRJ200->SetName("eff_LRJ200");
        sig_eff_offlineLRJ200->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_offlineLRJ200->Divide(sig_h_offlineLRJ_Et_num200, sig_h_offlineLRJ_Et_denom200, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ200->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ200->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_offlineLRJ200.pdf");

        TH1F* sig_eff_b_Et200 = (TH1F*)sig_h_Avg_b_Et_num200->Clone();
        sig_eff_b_Et200->SetName("eff_b200");
        sig_eff_b_Et200->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_b_Et200->Divide(sig_h_Avg_b_Et_num200, sig_h_Avg_b_Et_denom200, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et200->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et200->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_b_Et200.pdf");

        TH1F* sig_eff_offlineLRJ250 = (TH1F*)sig_h_offlineLRJ_Et_num250->Clone();
        sig_eff_offlineLRJ250->SetName("eff_LRJ250");
        sig_eff_offlineLRJ250->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_offlineLRJ250->Divide(sig_h_offlineLRJ_Et_num250, sig_h_offlineLRJ_Et_denom250, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ250->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ250->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_offlineLRJ250.pdf");

        TH1F* sig_eff_b_Et250 = (TH1F*)sig_h_Avg_b_Et_num250->Clone();
        sig_eff_b_Et250->SetName("eff_b250");
        sig_eff_b_Et250->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_b_Et250->Divide(sig_h_Avg_b_Et_num250, sig_h_Avg_b_Et_denom250, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et250->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et250->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_b_Et250.pdf");


        TH1F* sig_eff_offlineLRJ300 = (TH1F*)sig_h_offlineLRJ_Et_num300->Clone();
        sig_eff_offlineLRJ300->SetName("eff_LRJ300");
        sig_eff_offlineLRJ300->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_offlineLRJ300->Divide(sig_h_offlineLRJ_Et_num300, sig_h_offlineLRJ_Et_denom300, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ300->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ300->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_offlineLRJ300.pdf");

        TH1F* sig_eff_b_Et300 = (TH1F*)sig_h_Avg_b_Et_num300->Clone();
        sig_eff_b_Et300->SetName("eff_b300");
        sig_eff_b_Et300->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_b_Et300->Divide(sig_h_Avg_b_Et_num300, sig_h_Avg_b_Et_denom300, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et300->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et300->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_b_Et300.pdf");

        TH1F* sig_eff_offlineLRJ350 = (TH1F*)sig_h_offlineLRJ_Et_num350->Clone();
        sig_eff_offlineLRJ350->SetName("eff_LRJ350");
        sig_eff_offlineLRJ350->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_offlineLRJ350->Divide(sig_h_offlineLRJ_Et_num350, sig_h_offlineLRJ_Et_denom350, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ350->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ350->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_offlineLRJ350.pdf");

        TH1F* sig_eff_b_Et350 = (TH1F*)sig_h_Avg_b_Et_num350->Clone();
        sig_eff_b_Et350->SetName("eff_b350");
        sig_eff_b_Et350->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_b_Et350->Divide(sig_h_Avg_b_Et_num350, sig_h_Avg_b_Et_denom350, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et350->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et350->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_b_Et350.pdf");

        TH1F* sig_eff_offlineLRJ400 = (TH1F*)sig_h_offlineLRJ_Et_num400->Clone();
        sig_eff_offlineLRJ400->SetName("eff_LRJ400");
        sig_eff_offlineLRJ400->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_offlineLRJ400->Divide(sig_h_offlineLRJ_Et_num400, sig_h_offlineLRJ_Et_denom400, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ400->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ400->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_offlineLRJ400.pdf");

        TH1F* sig_eff_b_Et400 = (TH1F*)sig_h_Avg_b_Et_num400->Clone();
        sig_eff_b_Et400->SetName("eff_b400");
        sig_eff_b_Et400->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_b_Et400->Divide(sig_h_Avg_b_Et_num400, sig_h_Avg_b_Et_denom400, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et400->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et400->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_b_Et400.pdf");


        TH1F* sig_eff_offlineLRJ450 = (TH1F*)sig_h_offlineLRJ_Et_num450->Clone();
        sig_eff_offlineLRJ450->SetName("eff_LRJ450");
        sig_eff_offlineLRJ450->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_offlineLRJ450->Divide(sig_h_offlineLRJ_Et_num450, sig_h_offlineLRJ_Et_denom450, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ450->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ450->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_offlineLRJ450.pdf");

        TH1F* sig_eff_b_Et450 = (TH1F*)sig_h_Avg_b_Et_num450->Clone();
        sig_eff_b_Et450->SetName("eff_b450");
        sig_eff_b_Et450->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_b_Et450->Divide(sig_h_Avg_b_Et_num450, sig_h_Avg_b_Et_denom450, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et450->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et450->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_b_Et450.pdf");


        TH1F* sig_eff_offlineLRJ500 = (TH1F*)sig_h_offlineLRJ_Et_num500->Clone();
        sig_eff_offlineLRJ500->SetName("eff_LRJ500");
        sig_eff_offlineLRJ500->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_offlineLRJ500->Divide(sig_h_offlineLRJ_Et_num500, sig_h_offlineLRJ_Et_denom500, 1.0, 1.0);//, "B");
        
        sig_eff_offlineLRJ500->SetAxisRange(0, 1.1, "Y");

        sig_eff_offlineLRJ500->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_offlineLRJ500.pdf");

        TH1F* sig_eff_b_Et500 = (TH1F*)sig_h_Avg_b_Et_num500->Clone();
        sig_eff_b_Et500->SetName("eff_b500");
        sig_eff_b_Et500->GetYaxis()->SetTitle("Emulated Trigger Efficiency");
        sig_eff_b_Et500->Divide(sig_h_Avg_b_Et_num500, sig_h_Avg_b_Et_denom500, 1.0, 1.0);//, "B");
        
        sig_eff_b_Et500->SetAxisRange(0, 1.1, "Y");

        sig_eff_b_Et500->Draw("P");
        leg->Draw();
        c.SaveAs(modifiedOutputFileDir + "eff_b_Et500.pdf");


        cSigEffLRJ.cd();
        int colors[] = {
            kRed,          // strong red
            kBlue,         // deep blue
            kGreen+2,      // vivid green
            kMagenta,      // purple-pink
            kOrange+7,     // light orange

            kCyan+1,       // bright cyan
            kViolet+1,     // light violet
            kAzure+2,      // soft blue
            kPink+3,       // light pink
            kSpring+5      // light green-yellow
        };

        // eff. vs. offline leading lrj Et overlay
        sig_eff_offlineLRJ50->SetLineColor(colors[0]);
        sig_eff_offlineLRJ50->SetMarkerColor(colors[0]);
        legEffLRJ->AddEntry(sig_eff_offlineLRJ50, "Lead. LRJ E_{T} > 50 GeV", "l");
        sig_eff_offlineLRJ100->SetLineColor(colors[1]);
        sig_eff_offlineLRJ100->SetMarkerColor(colors[1]);
        legEffLRJ->AddEntry(sig_eff_offlineLRJ100, "Lead. LRJ E_{T} > 100 GeV", "l");
        sig_eff_offlineLRJ150->SetLineColor(colors[2]);
        sig_eff_offlineLRJ150->SetMarkerColor(colors[2]);
        legEffLRJ->AddEntry(sig_eff_offlineLRJ150, "Lead. LRJ E_{T} > 150 GeV", "l");
        sig_eff_offlineLRJ200->SetLineColor(colors[3]);
        sig_eff_offlineLRJ200->SetMarkerColor(colors[3]);
        legEffLRJ->AddEntry(sig_eff_offlineLRJ200, "Lead. LRJ E_{T} > 200 GeV", "l");
        sig_eff_offlineLRJ250->SetLineColor(colors[4]);
        sig_eff_offlineLRJ250->SetMarkerColor(colors[4]);
        legEffLRJ->AddEntry(sig_eff_offlineLRJ250, "Lead. LRJ E_{T} > 250 GeV", "l");
        sig_eff_offlineLRJ300->SetLineColor(colors[5]);
        sig_eff_offlineLRJ300->SetMarkerColor(colors[5]);
        legEffLRJ->AddEntry(sig_eff_offlineLRJ300, "Lead. LRJ E_{T} > 300 GeV", "l");
        sig_eff_offlineLRJ350->SetLineColor(colors[6]);
        sig_eff_offlineLRJ350->SetMarkerColor(colors[6]);
        legEffLRJ->AddEntry(sig_eff_offlineLRJ350, "Lead. LRJ E_{T} > 350 GeV", "l");
        sig_eff_offlineLRJ400->SetLineColor(colors[7]);
        sig_eff_offlineLRJ400->SetMarkerColor(colors[7]);
        legEffLRJ->AddEntry(sig_eff_offlineLRJ400, "Lead. LRJ E_{T} > 400 GeV", "l");
        sig_eff_offlineLRJ450->SetLineColor(colors[8]);
        sig_eff_offlineLRJ450->SetMarkerColor(colors[8]);
        legEffLRJ->AddEntry(sig_eff_offlineLRJ450, "Lead. LRJ E_{T} > 450 GeV", "l");
        sig_eff_offlineLRJ500->SetLineColor(colors[9]);
        sig_eff_offlineLRJ500->SetMarkerColor(colors[9]);
        legEffLRJ->AddEntry(sig_eff_offlineLRJ500, "Lead. LRJ E_{T} > 500 GeV", "l");
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
        legEffLRJ->Draw();
        cSigEffLRJ.SaveAs(modifiedOutputFileDir + "overlayed_eff_LRJ.pdf");

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

    TString outputFileDir = "overlayLargeRJetHistogramsrMergeScan/";

    // Create ROC Curve Canvas with 2 pads: top (ROC) and bottom (S/B)
    TLegend* legend1 = new TLegend(0.45, 0.45, 0.9, 0.95);
    legend1->SetTextSize(0.0325); // Adjust text size

    TCanvas* roc_plot_canvas = new TCanvas("roc_plot_canvas", "ROC Curve with S/B", 710, 10, 700, 700);
    TPad* pad1 = new TPad("pad1", "Main ROC Curve", 0.0, 0.3, 1.0, 1.0);
    TPad* pad2 = new TPad("pad2", "S/B Subplot",    0.0, 0.0, 1.0, 0.3);
    pad1->SetBottomMargin(0.02);
    pad2->SetTopMargin(0.05);
    pad2->SetBottomMargin(0.3);
    pad1->Draw();
    pad2->Draw();

    TLegend* legend2 = new TLegend(0.50, 0.50, 0.95, 0.95);
    legend2->SetTextSize(0.035); // Adjust text size
    TCanvas* eff_plot_canvas = new TCanvas("eff_plot_canvas", "Efficiency Curve", 0, 10, 700, 500);

    std::vector<TGraph*> roc_graphs;
    std::vector<TGraph*> sbGraphs;  // S/B subplot graphs

    for (int fileIt = 0; fileIt < backgroundFileNames.size(); ++fileIt) {
        std::vector<double>& x = roc_curve_points_x[fileIt];
        std::vector<double>& y = roc_curve_points_y[fileIt];

        std::vector<int> goodColors = {1, 2, 4, 6, 7, 8, 9, 28, 30, 46};  // skip 0 (white), 10, etc.
        int colorIndex = goodColors[fileIt % goodColors.size()];

        // ----------------------------------------
        // ROC Curve (top pad)
        pad1->cd();
        TGraph* gr = new TGraph(x.size(), &x[0], &y[0]);
        gr->SetTitle("ROC Curve;Signal Efficiency;Background Rejection (1/FPR)");
        gr->SetMarkerColor(colorIndex);
        gr->SetLineColor(colorIndex);
        gr->GetXaxis()->SetRangeUser(0, 1);
        gr->GetYaxis()->SetRangeUser(0, 50);
        gr->SetMarkerSize(0.25);
        gr->GetXaxis()->SetLabelSize(0);  // remove x-axis labels
        gr->GetXaxis()->SetTitleSize(0);  // remove x-axis title
        legend1->AddEntry(gr, legendMap[algorithmConfigurations[fileIt]].c_str(), "l");
        roc_graphs.push_back(gr);

        if (fileIt == 0) gr->Draw("AL");
        else             gr->Draw("L SAME");

        // ----------------------------------------
        // S/B subplot calculation
        std::vector<double> sb_values;
        for (size_t i = 0; i < x.size(); ++i) {
            double tpr = x[i];
            double fpr = (y[i] > 0) ? 1.0 / y[i] : 1e-10;
            sb_values.push_back(tpr / fpr);
        }
        TGraph* sbGraph = new TGraph(x.size(), &x[0], &sb_values[0]);
        sbGraph->SetLineColor(colorIndex);
        sbGraph->SetLineStyle(1);
        sbGraphs.push_back(sbGraph);

        // ----------------------------------------
        // Efficiency Curve
        eff_plot_canvas->cd();
        TGraph* gr2 = new TGraph(efficiency_curve_points_x[fileIt].size(),
            &efficiency_curve_points_x[fileIt][0],
            &efficiency_curve_points_y[fileIt][0]);
        gr2->SetTitle("Efficiency Curve;E_{T} Minimum;Signal Efficiency");
        gr2->SetMarkerColor(colorIndex);
        gr2->SetLineColor(colorIndex);
        gr2->GetYaxis()->SetRangeUser(0.0, 1);
        gr2->SetMarkerSize(0.25);
        legend2->AddEntry(gr2, legendMap[algorithmConfigurations[fileIt]].c_str(), "l");

        if (fileIt == 0) gr2->Draw("AL");
        else             gr2->Draw("L SAME");

        //std::cout << "algorithmConfigurations[fileIt]: " << algorithmConfigurations[fileIt] << "\n";
        //std::cout << "legendMap[algorithmConfigurations[fileIt]].c_str(): " << legendMap[algorithmConfigurations[fileIt]].c_str() << "\n";

        // ----------------------------------------
        // Print max signal-to-background summary
        auto maxIt = std::max_element(maxSignalToBackgroundRatio[fileIt].begin(), maxSignalToBackgroundRatio[fileIt].end());
        auto maxIt0p1 = std::max_element(maxSignalToBackgroundRatioTPR0p1[fileIt].begin(), maxSignalToBackgroundRatioTPR0p1[fileIt].end());
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
        std::cout << "----------------------------------------------" << "\n";

        // Save after final iteration
        if (fileIt == backgroundFileNames.size() - 1) {
            roc_plot_canvas->cd();
            pad1->cd();
            legend1->Draw();

            pad2->cd();
            TH1F* h_frame = pad2->DrawFrame(0, 0, 1, 5, ";Signal Efficiency;Sig. / Back. Ratio");
            h_frame->SetTitle(";Signal Efficiency;Sig. / Back. Ratio");
            //h_frame->GetYaxis()->SetTitle("Sig. / Back. Ratio");
            h_frame->GetYaxis()->SetTitleSize(0.08);
            h_frame->GetYaxis()->SetLabelSize(0.08);

            h_frame->GetXaxis()->SetTitleSize(0.10);
            h_frame->GetXaxis()->SetLabelSize(0.08);
            h_frame->GetYaxis()->CenterTitle(true);
            

            for (auto sbg : sbGraphs) sbg->Draw("L SAME");

            roc_plot_canvas->SaveAs(outputFileDir + "roc_curve_with_sb_ratio.pdf");

            eff_plot_canvas->cd();
            legend2->Draw();
            eff_plot_canvas->SaveAs(outputFileDir + "efficiency_curve.pdf");
        }
    }

    
}


void callAnalyzer(){
    std::vector<std::string > signalFilenames = {"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsrMergeScan/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut0p81_maxObj128_1p5sig.dat",
                                                 "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsrMergeScan/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut0p81_maxObj256_1p5sig.dat",
                                                 "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsrMergeScan/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj128_1p5sig.dat",
                                                 "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsrMergeScan/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj256_1p5sig.dat"
        //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut0p81_maxObj128_sig_noecut_ecutVal4.dat",
                                                 //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj128_sig_noecut_ecutVal4.dat",
                                                //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p21_maxObj128_sig_noecut_ecutVal4.dat",
                                                //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsjFexSeedCalc/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut0p81_maxObj128_sig_noecut_ecutVal4.dat"
                                                //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut0p81_maxObj256_sig_noecut_ecutVal4.dat",
                                                // "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj256_sig_noecut_ecutVal4.dat",
                                                //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p21_maxObj256_sig_noecut_ecutVal4.dat",
                                                //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p44_maxObj256_sig_noecut_ecutVal4.dat"
        /*"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p21_maxObj128_sig_noecut_ecutVal4.dat",
                                               "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p44_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p69_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p96_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj256_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p21_maxObj256_sig_noecut_ecutVal4.dat",
                                               "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p44_maxObj256_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p69_maxObj256_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p96_maxObj256_sig_noecut_ecutVal4.dat"*/
                                                
                                                /*"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut0p64_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut0p64_maxObj128_sig_ecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p44_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj256_sig_ecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj256_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj512_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p44_maxObj512_sig_noecut_ecutVal4.dat"*/
                                                };

    std::vector<std::string > backgroundFilenames = { "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsrMergeScan/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut0p81_maxObj128_1p5back.dat",
                                                      "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsrMergeScan/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut0p81_maxObj256_1p5back.dat",
                                                      "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsrMergeScan/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p0_maxObj128_1p5back.dat",
                                                      "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsrMergeScan/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p0_maxObj256_1p5back.dat"
        //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut0p81_maxObj128_back_noecut_ecutVal4.dat",
                                                      //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p0_maxObj128_back_noecut_ecutVal4.dat",
                                                      //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p21_maxObj128_back_noecut_ecutVal4.dat",
                                                      //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsjFexSeedCalc/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut0p81_maxObj128_back_noecut_ecutVal4.dat"
                                                      //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut0p81_maxObj256_back_noecut_ecutVal4.dat",
                                                      //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p0_maxObj256_back_noecut_ecutVal4.dat",
                                                      //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p21_maxObj256_back_noecut_ecutVal4.dat",
                                                      //"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p44_maxObj256_back_noecut_ecutVal4.dat"
        
        
        
        /*"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p0_maxObj128_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p21_maxObj128_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p44_maxObj128_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p69_maxObj128_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p96_maxObj128_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p0_maxObj256_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p21_maxObj256_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p44_maxObj256_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p69_maxObj256_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p96_maxObj256_back_noecut_ecutVal4.dat"*/
                                                    /*"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut0p64_maxObj128_back_noecut_ecutVal4.dat",
                                                    "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut0p64_maxObj128_back_ecut_ecutVal4.dat",
                                                    "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p0_maxObj128_back_noecut_ecutVal4.dat",
                                                    "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p44_maxObj128_back_noecut_ecutVal4.dat",
                                                    "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p0_maxObj256_back_ecut_ecutVal4.dat",
                                                    "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p0_maxObj256_back_noecut_ecutVal4.dat",
                                                    "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p0_maxObj512_back_noecut_ecutVal4.dat",
                                                    "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p44_maxObj512_back_noecut_ecutVal4.dat"*/
                                                    };

    //const std::string signalLargeRJetDataFileName = "/eos/home-m/mlarson/LargeRadiusJets/MemPrints/largeRJets/mc21_14TeV_hh_bbbb_vbf_novhh_largeR.dat";
    //const std::string backgroundLargeRJetDataFileName = "/eos/home-m/mlarson/LargeRadiusJets/MemPrints/largeRJets/mc21_14TeV_jj_JZ3_largeR.dat";
    constexpr bool vbfBool = true;
    std::string signalRootFileName;
    if (vbfBool) signalRootFileName = "/home/larsonma/LargeRadiusJets/data/daodNtuples/mc21_14TeV_hh_bbbb_vbf_novhh_DAOD_NTUPLE.root";
    else signalRootFileName = "/home/larsonma/LargeRadiusJets/data/daodNtuples/mc21_14TeV_HHbbbb_HLLHC_DAOD_NTUPLE.root";
    std::string backgroundRootFileName = "/home/larsonma/LargeRadiusJets/data/daodNtuples/mc21_14TeV_jj_JZ3_DAOD_NTUPLE.root";

    analyze_files(backgroundFilenames, signalFilenames, signalRootFileName, backgroundRootFileName);
    //gSystem->Exit(0);
}