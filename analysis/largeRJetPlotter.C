#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "analysisHelperFunctions.h"

#define WRITE_LUT false // for disabling firmware (ap_uint), LUT declarations

template <bool signalBackgroundOverlay> 
void analyze_existing_histograms(const std::string& signalInputFileName, const std::string& backgroundInputFileName, const std::string& outputPrefix, const bool signalBool);

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




// Declare helper functions 


// Function to sort and get highest Et gFex jets
std::pair<int, int> getTwoHighestIndices(const std::vector<double>& etValues) {
    if (etValues.size() < 2) return {-1, -1}; // If not enough jets, return invalid indices
    int maxIdx1 = 0, maxIdx2 = -1;
    for (size_t i = 1; i < etValues.size(); i++) {
        if (etValues[i] > etValues[maxIdx1]) {
            maxIdx2 = maxIdx1;
            maxIdx1 = i;
        } else if (maxIdx2 == -1 || etValues[i] > etValues[maxIdx2]) {
            maxIdx2 = i;
        }
    }
    return {maxIdx1, maxIdx2};
}

template <>
void analyze_existing_histograms<false>(const std::string& signalInputFileName, const std::string& backgroundInputFileName, const std::string& outputPrefix, const bool signalBool) {
    SetPlotStyle();
    // Open input ROOT file
    std::string inputFileName; 
    if (signalBool) inputFileName = signalInputFileName;
    else inputFileName = backgroundInputFileName;
    TFile* inputFile = TFile::Open(inputFileName.c_str(), "READ");
    if (!inputFile || inputFile->IsZombie()) {
        std::cerr << "Error: Could not open file " << inputFileName << std::endl;
        return;
    }

    // Get trees
    TTree* topoTree = (TTree*)inputFile->Get("topoTree");
    TTree* gFexTree = (TTree*)inputFile->Get("gFexTree");
    TTree* jFexTree = (TTree*)inputFile->Get("jFexTree");

    if (!topoTree || !gFexTree || !jFexTree) {
        std::cerr << "Error: Could not retrieve one or more trees." << std::endl;
        inputFile->Close();
        return;
    }

    // Define variables
    std::vector<double>* topoEtValues = nullptr;
    std::vector<double>* topoEtaValues = nullptr;
    std::vector<double>* topoPhiValues = nullptr;

    std::vector<double>* gFexEtValues = nullptr;
    std::vector<double>* gFexEtaValues = nullptr;
    std::vector<double>* gFexPhiValues = nullptr;

    std::vector<double>* jFexEtValues = nullptr;
    std::vector<double>* jFexEtaValues = nullptr;
    std::vector<double>* jFexPhiValues = nullptr;

    // Histograms
    TH1F* h_topo_Et = new TH1F("h_topo_Et", "Topo Et Distribution;Et (GeV);Counts", 100, 0, 400);
    TH1F* h_gFex_Et = new TH1F("h_gFex_Et", "gFex Et Distribution;Et (GeV);Counts", 100, 0, 400);
    TH1F* h_jFex_Et = new TH1F("h_jFex_Et", "jFex Et Distribution;Et (GeV);Counts", 100, 0, 400);

    TH1F* h_topo_eta = new TH1F("h_topo_eta", "Topo Eta Distribution;Eta;Counts", 50, -5, 5);
    TH1F* h_gFex_eta = new TH1F("h_gFex_eta", "gFex Eta Distribution;Eta;Counts", 50, -5, 5);
    TH1F* h_jFex_eta = new TH1F("h_jFex_eta", "jFex Eta Distribution;Eta;Counts", 50, -5, 5);

    TH1F* h_gFex_leading_Et = new TH1F("h_gFex_leading_Et", "Leading gFEX Jet Et;Et (GeV);Counts", 100, 0, 400);
    TH1F* h_gFex_subleading_Et = new TH1F("h_gFex_subleading_Et", "Sub-leading gFEX Jet Et;Et (GeV);Counts", 100, 0, 400);

    TH1F* h_gFex_leading_eta = new TH1F("h_gFex_leading_eta", "Leading gFEX Jet Eta;#eta;Counts", 50, -5, 5);
    TH1F* h_gFex_subleading_eta = new TH1F("h_gFex_subleading_eta", "Sub-leading gFEX Jet Eta;#eta;Counts", 50, -5, 5);

    TH1F* h_gFex_leading_phi = new TH1F("h_gFex_leading_phi", "Leading gFEX Jet Phi;#phi;Counts", 64, -3.2, 3.2);
    TH1F* h_gFex_subleading_phi = new TH1F("h_gFex_subleading_phi", "Sub-leading gFEX Jet Phi;#phi;Counts", 64, -3.2, 3.2);

    TH1F* h_gFex_leading_subleading_deltaR = new TH1F("h_gFex_leading_subleading_deltaR", "Leading, subleading gFex SRJ DeltaR Dist.; #Delta R gFex Lead, Sublead SRJ; Normalized Events  / 0.08", 100, 0, 8);

    TH1F* h_topo_phi = new TH1F("h_topo_phi", "Topo Phi Distribution;Phi;Counts", 64, -3.2, 3.2);
    TH1F* h_gFex_phi = new TH1F("h_gFex_phi", "gFex Phi Distribution;Phi;Counts", 64, -3.2, 3.2);
    TH1F* h_jFex_phi = new TH1F("h_jFex_phi", "jFex Phi Distribution;Phi;Counts", 64, -3.2, 3.2);

    TH1F* h_gFex_multiplicity = new TH1F("h_gFex_multiplicity", "gFex SmallR Jet Multiplicity;Number of gFex SmallR Jets;Counts", 20, 0, 20);
    TH1F* h_topo_multiplicity = new TH1F("h_topo_multiplicity", "Topo422 Cluster Multiplicity;Number of Topo422 Clusters;Counts", 60, 400, 1000);
    TH1F* h_jFex_multiplicity = new TH1F("h_jFex_multiplicity", "jFex SmallR Jet Multiplicity;Number of jFex SmallR Jets;Counts", 50, 0, 100);

    // gFex jet deltaR^2 histogram
    TH1F* h_deltaR2_jet1 = new TH1F("h_deltaR2_jet1", "#DeltaR^{2} (gFex Highest Et Jet vs. topo422)", 50, 0, 10);
    TH1F* h_deltaR2_jet2 = new TH1F("h_deltaR2_jet2", "#DeltaR^{2} (gFex 2nd Highest Et Jet vs. topo422)", 50, 0, 10);
    TH1F* h_deltaR_jet1 = new TH1F("h_deltaR_jet1", "#DeltaR (gFex Highest Et Jet vs. topo422)", 25, 0, 5);
    TH1F* h_deltaR_jet2 = new TH1F("h_deltaR_jet2", "#DeltaR (gFex 2nd Highest Et Jet vs. topo422)", 25, 0, 5);

    // Process trees
    topoTree->SetBranchAddress("Et", &topoEtValues);
    topoTree->SetBranchAddress("Eta", &topoEtaValues);
    topoTree->SetBranchAddress("Phi", &topoPhiValues);

    gFexTree->SetBranchAddress("Et", &gFexEtValues);
    gFexTree->SetBranchAddress("Eta", &gFexEtaValues);
    gFexTree->SetBranchAddress("Phi", &gFexPhiValues);

    jFexTree->SetBranchAddress("Et", &jFexEtValues);
    jFexTree->SetBranchAddress("Eta", &jFexEtaValues);
    jFexTree->SetBranchAddress("Phi", &jFexPhiValues);

    int nEntries = gFexTree->GetEntries();
    std::cout << "gfex nentries : " << nEntries << "\n";
    for (int i = 0; i < nEntries; i++) {
        gFexTree->GetEntry(i);
        h_gFex_multiplicity->Fill(gFexEtValues->size());
        for (size_t j = 0; j < gFexEtValues->size(); j++) {
            h_gFex_Et->Fill(gFexEtValues->at(j));
            h_gFex_eta->Fill(gFexEtaValues->at(j));
            h_gFex_phi->Fill(gFexPhiValues->at(j));
        }

        // Find two highest Et jets
        auto [idx1, idx2] = getTwoHighestIndices(*gFexEtValues);

        // Leading jet
        h_gFex_leading_Et->Fill(gFexEtValues->at(idx1));
        h_gFex_leading_eta->Fill(gFexEtaValues->at(idx1));
        h_gFex_leading_phi->Fill(gFexPhiValues->at(idx1));

        // Sub-leading jet
        h_gFex_subleading_Et->Fill(gFexEtValues->at(idx2));
        h_gFex_subleading_eta->Fill(gFexEtaValues->at(idx2));
        h_gFex_subleading_phi->Fill(gFexPhiValues->at(idx2));
        std::cout << "idx1: " << idx1 << " idx2: " << idx2 << "\n";
        std::cout << "gfex 1 eta: " << gFexEtaValues->at(idx1) << " gfex 1 phi: " << gFexPhiValues->at(idx1) << "\n";
        std::cout << "gfex 2 eta: " << gFexEtaValues->at(idx2) << " gfex 2 phi: " << gFexPhiValues->at(idx2) << "\n";
        std::cout << "deltaR^2: " << sqrt(calcDeltaR2(gFexEtaValues->at(idx1), gFexPhiValues->at(idx1), gFexEtaValues->at(idx2), gFexPhiValues->at(idx2))) << "\n";
        h_gFex_leading_subleading_deltaR->Fill(sqrt(calcDeltaR2(gFexEtaValues->at(idx1), gFexPhiValues->at(idx1), gFexEtaValues->at(idx2), gFexPhiValues->at(idx2))));

        topoTree->GetEntry(i);
        //std::cout << "topoEtValues->size(): " << topoEtValues->size() << "\n";
        h_topo_multiplicity->Fill(topoEtValues->size());
        for (size_t k = 0; k < topoEtValues->size(); k++) {
            h_topo_Et->Fill(topoEtValues->at(k));
            h_topo_eta->Fill(topoEtaValues->at(k));
            h_topo_phi->Fill(topoPhiValues->at(k));
            double dr2_1 = ((gFexEtaValues->at(idx1)) - topoEtaValues->at(k))*((gFexEtaValues->at(idx1)) - topoEtaValues->at(k)) + deltaPhi(gFexPhiValues->at(idx1), topoPhiValues->at(k))*deltaPhi(gFexPhiValues->at(idx1), topoPhiValues->at(k));
            h_deltaR2_jet1->Fill(dr2_1);
            h_deltaR_jet1->Fill(sqrt(dr2_1));
            double dr2_2 = ((gFexEtaValues->at(idx2)) - topoEtaValues->at(k))*((gFexEtaValues->at(idx2)) - topoEtaValues->at(k)) + deltaPhi(gFexPhiValues->at(idx2), topoPhiValues->at(k))*deltaPhi(gFexPhiValues->at(idx2), topoPhiValues->at(k));
            h_deltaR2_jet2->Fill(dr2_2);
            h_deltaR_jet2->Fill(sqrt(dr2_2));
        }
            
    }

    for (int i = 0; i < jFexTree->GetEntries(); i++) {
        jFexTree->GetEntry(i);
        //std::cout << "jFexEtValues->size(): " << jFexEtValues->size() << "\n";
        h_jFex_multiplicity->Fill(jFexEtValues->size());
        for (float et : *jFexEtValues) h_jFex_Et->Fill(et);
        for (float eta : *jFexEtaValues) h_jFex_eta->Fill(eta);
        for (float phi : *jFexPhiValues) h_jFex_phi->Fill(phi);
    }

    // Save histograms
    TCanvas c;
    TString outputFileDir = outputPrefix;

    h_topo_Et->Draw();
    c.SaveAs(outputFileDir + "topo_Et.pdf");
    h_gFex_Et->Draw();
    c.SaveAs(outputFileDir + "gFex_Et.pdf");
    h_jFex_Et->Draw();
    c.SaveAs(outputFileDir + "jFex_Et.pdf");

    h_topo_eta->Draw();
    c.SaveAs(outputFileDir + "topo_eta.pdf");
    h_gFex_eta->Draw();
    c.SaveAs(outputFileDir + "gFex_eta.pdf");
    h_jFex_eta->Draw();
    c.SaveAs(outputFileDir + "jFex_eta.pdf");

    h_topo_phi->Draw();
    c.SaveAs(outputFileDir + "topo_phi.pdf");
    h_gFex_phi->Draw();
    c.SaveAs(outputFileDir + "gFex_phi.pdf");
    h_jFex_phi->Draw();
    c.SaveAs(outputFileDir + "jFex_phi.pdf");
    
    h_gFex_leading_Et->Draw();
    c.SaveAs(outputFileDir + "gFex_leading_Et.pdf");    
    h_gFex_leading_eta->Draw();
    c.SaveAs(outputFileDir + "gFex_leading_eta.pdf");   
    h_gFex_leading_phi->Draw();
    c.SaveAs(outputFileDir + "gFex_leading_phi.pdf");   

    h_gFex_subleading_Et->Draw();
    c.SaveAs(outputFileDir + "gFex_subleading_Et.pdf");   
    h_gFex_subleading_eta->Draw();
    c.SaveAs(outputFileDir + "gFex_subleading_eta.pdf");   
    h_gFex_subleading_phi->Draw();
    c.SaveAs(outputFileDir + "gFex_subleading_phi.pdf");   

    
    h_gFex_leading_subleading_deltaR->Scale(1.0 / h_gFex_leading_subleading_deltaR->Integral());
    h_gFex_leading_subleading_deltaR->Draw("HIST");
    c.SaveAs(outputFileDir + "gFex_leading_subleading_deltaR.pdf");
    

    h_gFex_multiplicity->Draw();
    c.SaveAs(outputFileDir + "gFex_multiplicity.pdf");
    h_topo_multiplicity->Draw();
    c.SaveAs(outputFileDir + "topo_multiplicity.pdf");
    h_jFex_multiplicity->Draw();
    c.SaveAs(outputFileDir + "jFex_multiplicity.pdf");

    h_deltaR2_jet1->Scale(1.0 / h_deltaR2_jet1->Integral());
    h_deltaR2_jet1->SetXTitle("#Delta R^{2}");   // X-axis title
    h_deltaR2_jet1->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    h_deltaR2_jet1->Draw();

    c.SaveAs(outputFileDir + "deltaR2_jet1.pdf");
    h_deltaR2_jet2->Scale(1.0 / h_deltaR2_jet2->Integral());
    h_deltaR2_jet2->SetXTitle("#Delta R^{2}");   // X-axis title
    h_deltaR2_jet2->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    h_deltaR2_jet2->Draw();
    c.SaveAs(outputFileDir + "deltaR2_jet2.pdf");

    h_deltaR_jet1->Scale(1.0 / h_deltaR_jet1->Integral());
    h_deltaR_jet1->SetXTitle("#Delta R");   // X-axis title
    h_deltaR_jet1->SetYTitle("Normalized Num. of Topo422 Clusters / 0.2");  // Y-axis title
    h_deltaR_jet1->Draw();
    c.SaveAs(outputFileDir + "deltaR_jet1.pdf");
    h_deltaR_jet2->Scale(1.0 / h_deltaR_jet2->Integral());
    h_deltaR_jet2->SetXTitle("#Delta R");   // X-axis title
    h_deltaR_jet2->SetYTitle("Normalized Num. of Topo422 Clusters / 0.2");  // Y-axis title
    h_deltaR_jet2->Draw();
    c.SaveAs(outputFileDir + "deltaR_jet2.pdf");


    TCanvas cLog;
    // Log scale plots
    cLog.SetLogx();
    cLog.SetLogy();

    h_topo_Et->Draw();
    cLog.SaveAs(outputFileDir + "log_topo_Et.pdf");
    h_gFex_Et->Draw();
    cLog.SaveAs(outputFileDir + "log_gFex_Et.pdf");
    h_jFex_Et->Draw();
    cLog.SaveAs(outputFileDir + "log_jFex_Et.pdf");

    h_gFex_leading_Et->Draw();
    c.SaveAs(outputFileDir + "log_gFex_leading_Et.pdf");    

    h_gFex_subleading_Et->Draw();
    c.SaveAs(outputFileDir + "log_gFex_subleading_Et.pdf"); 

    inputFile->Close();
}

template <>
void analyze_existing_histograms<true>(const std::string& signalInputFileName, const std::string& backgroundInputFileName, const std::string& outputPrefix, const bool signalBool) {
    unsigned int nTopo = 0;
    unsigned int nTopoGreater1GeV = 0;
    double totalTopoEnergy = 0.0;
    double topoEnergyClustersGreater1GeV = 0.0;
    SetPlotStyle();
    // Open input ROOT file
    TFile* signalInputFile = TFile::Open(signalInputFileName.c_str(), "READ");
    TFile* backgroundInputFile = TFile::Open(backgroundInputFileName.c_str(), "READ");
    if ((!signalInputFile || signalInputFile->IsZombie()) && (!backgroundInputFile || backgroundInputFile->IsZombie())) {
        std::cerr << "Error: Could not open file " << signalInputFileName << std::endl;
        return;
    }

    // Get trees
    TTree* sigTopoTree = (TTree*)signalInputFile->Get("topoTree");
    TTree* sigGFexTree = (TTree*)signalInputFile->Get("gFexTree");
    TTree* sigJFexTree = (TTree*)signalInputFile->Get("jFexTree");

    TTree* backTopoTree = (TTree*)backgroundInputFile->Get("topoTree");
    TTree* backGFexTree = (TTree*)backgroundInputFile->Get("gFexTree");
    TTree* backJFexTree = (TTree*)backgroundInputFile->Get("jFexTree");

    if (!sigTopoTree || !sigGFexTree || !sigJFexTree || !backTopoTree || !backGFexTree || !backJFexTree) {
        std::cerr << "Error: Could not retrieve one or more trees." << std::endl;
        signalInputFile->Close();
        backgroundInputFile->Close();
        return;
    }

    // Define variables
    std::vector<double>* sigTopoEtValues = nullptr;
    std::vector<double>* sigTopoEtaValues = nullptr;
    std::vector<double>* sigTopoPhiValues = nullptr;

    std::vector<double> sigTopoEtValues_top128;
    std::vector<double> sigTopoEtaValues_top128;
    std::vector<double> sigTopoPhiValues_top128;

    std::vector<double>* sigGFexEtValues = nullptr;
    std::vector<double>* sigGFexEtaValues = nullptr;
    std::vector<double>* sigGFexPhiValues = nullptr;

    std::vector<double>* sigJFexEtValues = nullptr;
    std::vector<double>* sigJFexEtaValues = nullptr;
    std::vector<double>* sigJFexPhiValues = nullptr;

    std::vector<double>* backTopoEtValues = nullptr;
    std::vector<double>* backTopoEtaValues = nullptr;
    std::vector<double>* backTopoPhiValues = nullptr;

    std::vector<double> backTopoEtValues_top128;
    std::vector<double> backTopoEtaValues_top128;
    std::vector<double> backTopoPhiValues_top128;

    std::vector<double>* backGFexEtValues = nullptr;
    std::vector<double>* backGFexEtaValues = nullptr;
    std::vector<double>* backGFexPhiValues = nullptr;

    std::vector<double>* backJFexEtValues = nullptr;
    std::vector<double>* backJFexEtaValues = nullptr;
    std::vector<double>* backJFexPhiValues = nullptr;

    // Histograms
    TH1F* sig_h_topo_Et = new TH1F("sig_h_topo_Et", "Topo Et Distribution;Et (GeV);Topo422 Clusters / 20 GeV", 20, 0, 400);
    TH1F* sig_h_gFex_Et = new TH1F("sig_h_gFex_Et", "gFex Et Distribution;Et (GeV);Counts", 30, 0, 300);
    TH1F* sig_h_jFex_Et = new TH1F("sig_h_jFex_Et", "jFex Et Distribution;Et (GeV);Counts", 30, 0, 300);

    TH1F* sig_h_topo_eta = new TH1F("sig_h_topo_eta", "Topo Eta Distribution;Eta;Counts", 50, -5, 5);
    TH1F* sig_h_gFex_eta = new TH1F("sig_h_gFex_eta", "gFex Eta Distribution;Eta;Counts", 50, -5, 5);
    TH1F* sig_h_jFex_eta = new TH1F("sig_h_jFex_eta", "jFex Eta Distribution;Eta;Counts", 50, -5, 5);

    TH1F* sig_h_topo_phi = new TH1F("sig_h_topo_phi", "Topo Phi Distribution;Phi;Counts", 64, -3.2, 3.2);
    TH1F* sig_h_gFex_phi = new TH1F("sig_h_gFex_phi", "gFex Phi Distribution;Phi;Counts", 64, -3.2, 3.2);
    TH1F* sig_h_jFex_phi = new TH1F("sig_h_jFex_phi", "jFex Phi Distribution;Phi;Counts", 64, -3.2, 3.2);

    TH1F* sig_h_gFex_multiplicity = new TH1F("sig_h_gFex_multiplicity", "gFex SmallR Jet Multiplicity;Number of gFex SmallR Jets;Counts", 20, 0, 20);
    TH1F* sig_h_topo_multiplicity = new TH1F("sig_h_topo_multiplicity", "Topo422 Cluster Multiplicity;Number of Topo422 Clusters;Counts", 60, 400, 1000);
    TH1F* sig_h_jFex_multiplicity = new TH1F("sig_h_jFex_multiplicity", "jFex SmallR Jet Multiplicity;Number of jFex SmallR Jets;Counts", 50, 0, 100);

    TH1F* sig_h_gFex_leading_Et = new TH1F("sig_h_gFex_leading_Et", "Leading gFEX Jet Et;Et (GeV);Counts", 100, 0, 400);
    TH1F* sig_h_gFex_subleading_Et = new TH1F("sig_h_gFex_subleading_Et", "Sub-leading gFEX Jet Et;Et (GeV);Counts", 100, 0, 400);

    TH1F* sig_h_gFex_leading_eta = new TH1F("sig_h_gFex_leading_eta", "Leading gFEX Jet Eta;#eta;Counts", 50, -5, 5);
    TH1F* sig_h_gFex_subleading_eta = new TH1F("sig_h_gFex_subleading_eta", "Sub-leading gFEX Jet Eta;#eta;Counts", 50, -5, 5);

    TH1F* sig_h_gFex_leading_phi = new TH1F("sig_h_gFex_leading_phi", "Leading gFEX Jet Phi;#phi;Counts", 64, -3.2, 3.2);
    TH1F* sig_h_gFex_subleading_phi = new TH1F("sig_h_gFex_subleading_phi", "Sub-leading gFEX Jet Phi;#phi;Counts", 64, -3.2, 3.2);

    TH1F* sig_h_gFex_leading_subleading_deltaR = new TH1F("sig_h_gFex_leading_subleading_deltaR", "Leading, subleading gFex SRJ DeltaR Dist.; #Delta R gFex Lead, Sublead SRJ; Normalized Events / 0.08", 100, 0, 8);

    TH1F* sig_h_jFex_leading_Et = new TH1F("sig_h_jFex_leading_Et", "Leading jFEX Jet Et;Et (GeV);Counts", 100, 0, 800);
    TH1F* sig_h_jFex_subleading_Et = new TH1F("sig_h_jFex_subleading_Et", "Sub-leading jFEX Jet Et;Et (GeV);Counts", 100, 0, 500);

    TH1F* sig_h_jFex_leading_eta = new TH1F("sig_h_jFex_leading_eta", "Leading jFEX Jet Eta;#eta;Counts", 50, -5, 5);
    TH1F* sig_h_jFex_subleading_eta = new TH1F("sig_h_jFex_subleading_eta", "Sub-leading jFEX Jet Eta;#eta;Counts", 50, -5, 5);

    TH1F* sig_h_jFex_leading_phi = new TH1F("sig_h_jFex_leading_phi", "Leading jFEX Jet Phi;#phi;Counts", 64, -3.2, 3.2);
    TH1F* sig_h_jFex_subleading_phi = new TH1F("sig_h_jFex_subleading_phi", "Sub-leading jFEX Jet Phi;#phi;Counts", 64, -3.2, 3.2);

    TH1F* sig_h_jFex_leading_subleading_deltaR = new TH1F("sig_h_jFex_leading_subleading_deltaR", "Leading, subleading jFex SRJ DeltaR Dist.; #Delta R jFex Lead, Sublead SRJ; Normalized Events / 0.08", 100, 0, 8);

    TH1F* sig_h_topo_Et_top128  = new TH1F("sig_h_topo_Et_top128", "128 Highest Et Topo Cluster Et;Et (GeV);Counts", 20, 0, 400);
    TH1F* sig_h_topo_eta_top128 = new TH1F("sig_h_topo_eta_top128", "128 Highest Et Topo Cluster Eta;#eta;Counts", 50, -5, 5);
    TH1F* sig_h_topo_phi_top128 = new TH1F("sig_h_topo_phi_top128", "128 Highest Et Topo Cluster Phi;#phi;Counts", 64, -3.2, 3.2);

    // gFex jet deltaR^2 histogram
    TH1F* sig_h_deltaR2_gFex_SRJ1 = new TH1F("sig_h_deltaR2_gFex_SRJ1", "#DeltaR^{2} (gFex Highest Et Jet vs. topo422)", 25, 0, 10);
    TH1F* sig_h_deltaR2_gFex_SRJ2 = new TH1F("sig_h_deltaR2_gFex_SRJ2", "#DeltaR^{2} (gFex 2nd Highest Et Jet vs. topo422)", 25, 0, 10);
    TH1F* sig_h_deltaR_gFex_SRJ1 = new TH1F("sig_h_deltaR_gFex_SRJ1", "#DeltaR (gFex Highest Et Jet vs. topo422)", 25, 0, 5);
    TH1F* sig_h_deltaR_gFex_SRJ2 = new TH1F("sig_h_deltaR_gFex_SRJ2", "#DeltaR (gFex 2nd Highest Et Jet vs. topo422)", 25, 0, 5);

    TH1F* sig_h_deltaR2_gFex_SRJ1_topo128 = new TH1F("sig_h_deltaR2_gFex_SRJ1_topo128", "#DeltaR^{2} (gFex Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 10);
    TH1F* sig_h_deltaR2_gFex_SRJ2_topo128 = new TH1F("sig_h_deltaR2_gFex_SRJ2_topo128", "#DeltaR^{2} (gFex 2nd Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 10);
    TH1F* sig_h_deltaR_gFex_SRJ1_topo128 = new TH1F("sig_h_deltaR_gFex_SRJ1_topo128", "#DeltaR (gFex Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 5);
    TH1F* sig_h_deltaR_gFex_SRJ2_topo128 = new TH1F("sig_h_deltaR_gFex_SRJ2_topo128", "#DeltaR (gFex 2nd Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 5);

    TH1F* sig_h_deltaR2_jFex_SRJ1 = new TH1F("sig_h_deltaR2_jFex_SRJ1", "#DeltaR^{2} (gFex Highest Et Jet vs. topo422)", 25, 0, 10);
    TH1F* sig_h_deltaR2_jFex_SRJ2 = new TH1F("sig_h_deltaR2_jFex_SRJ2", "#DeltaR^{2} (gFex 2nd Highest Et Jet vs. topo422)", 25, 0, 10);
    TH1F* sig_h_deltaR_jFex_SRJ1 = new TH1F("sig_h_deltaR_jFex_SRJ1", "#DeltaR (gFex Highest Et Jet vs. topo422)", 25, 0, 5);
    TH1F* sig_h_deltaR_jFex_SRJ2 = new TH1F("sig_h_deltaR_jFex_SRJ2", "#DeltaR (gFex 2nd Highest Et Jet vs. topo422)", 25, 0, 5);

    TH1F* sig_h_deltaR2_jFex_SRJ1_topo128 = new TH1F("sig_h_deltaR2_jFex_SRJ1_topo128", "#DeltaR^{2} (gFex Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 10);
    TH1F* sig_h_deltaR2_jFex_SRJ2_topo128 = new TH1F("sig_h_deltaR2_jFex_SRJ2_topo128", "#DeltaR^{2} (gFex 2nd Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 10);
    TH1F* sig_h_deltaR_jFex_SRJ1_topo128 = new TH1F("sig_h_deltaR_jFex_SRJ1_topo128", "#DeltaR (gFex Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 5);
    TH1F* sig_h_deltaR_jFex_SRJ2_topo128 = new TH1F("sig_h_deltaR_jFex_SRJ2_topo128", "#DeltaR (gFex 2nd Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 5);

    TH1F* back_h_topo_Et = new TH1F("h_topo_Et", "Topo Et Distribution;Et (GeV);Topo422 Clusters / 20 GeV", 20, 0, 400);
    TH1F* back_h_gFex_Et = new TH1F("h_gFex_Et", "gFex Et Distribution;Et (GeV);Counts", 30, 0, 300);
    TH1F* back_h_jFex_Et = new TH1F("h_jFex_Et", "jFex Et Distribution;Et (GeV);Counts", 30, 0, 300);

    TH1F* back_h_topo_eta = new TH1F("h_topo_eta", "Topo Eta Distribution;Eta;Counts", 50, -5, 5);
    TH1F* back_h_gFex_eta = new TH1F("h_gFex_eta", "gFex Eta Distribution;Eta;Counts", 50, -5, 5);
    TH1F* back_h_jFex_eta = new TH1F("h_jFex_eta", "jFex Eta Distribution;Eta;Counts", 50, -5, 5);

    TH1F* back_h_topo_phi = new TH1F("h_topo_phi", "Topo Phi Distribution;Phi;Counts", 64, -3.2, 3.2);
    TH1F* back_h_gFex_phi = new TH1F("h_gFex_phi", "gFex Phi Distribution;Phi;Counts", 64, -3.2, 3.2);
    TH1F* back_h_jFex_phi = new TH1F("h_jFex_phi", "jFex Phi Distribution;Phi;Counts", 64, -3.2, 3.2);

    TH1F* back_h_gFex_multiplicity = new TH1F("h_gFex_multiplicity", "gFex SmallR Jet Multiplicity;Number of gFex SmallR Jets;Counts", 20, 0, 20);
    TH1F* back_h_topo_multiplicity = new TH1F("h_topo_multiplicity", "Topo422 Cluster Multiplicity;Number of Topo422 Clusters;Counts", 60, 400, 1000);
    TH1F* back_h_jFex_multiplicity = new TH1F("h_jFex_multiplicity", "jFex SmallR Jet Multiplicity;Number of jFex SmallR Jets;Counts", 50, 0, 100);

    TH1F* back_h_gFex_leading_Et = new TH1F("back_h_gFex_leading_Et", "Leading gFEX Jet Et;Et (GeV);Counts", 100, 0, 400);
    TH1F* back_h_gFex_subleading_Et = new TH1F("back_h_gFex_subleading_Et", "Sub-leading gFEX Jet Et;Et (GeV);Counts", 100, 0, 400);

    TH1F* back_h_gFex_leading_eta = new TH1F("back_h_gFex_leading_eta", "Leading gFEX Jet Eta;#eta;Counts", 50, -5, 5);
    TH1F* back_h_gFex_subleading_eta = new TH1F("back_h_gFex_subleading_eta", "Sub-leading gFEX Jet Eta;#eta;Counts", 50, -5, 5);

    TH1F* back_h_gFex_leading_phi = new TH1F("back_h_gFex_leading_phi", "Leading gFEX Jet Phi;#phi;Counts", 64, -3.2, 3.2);
    TH1F* back_h_gFex_subleading_phi = new TH1F("back_h_gFex_subleading_phi", "Sub-leading gFEX Jet Phi;#phi;Counts", 64, -3.2, 3.2);

    TH1F* back_h_gFex_leading_subleading_deltaR = new TH1F("back_h_gFex_leading_subleading_deltaR", "Leading, subleading gFex SRJ DeltaR Dist.; #Delta R gFex Lead, Sublead SRJ; Normalized Events / 0.08", 100, 0, 8);

    TH1F* back_h_jFex_leading_Et = new TH1F("back_h_jFex_leading_Et", "Leading jFEX Jet Et;Et (GeV);Counts", 100, 0, 500);
    TH1F* back_h_jFex_subleading_Et = new TH1F("back_h_jFex_subleading_Et", "Sub-leading jFEX Jet Et;Et (GeV);Counts", 100, 0, 500);

    TH1F* back_h_jFex_leading_eta = new TH1F("back_h_jFex_leading_eta", "Leading jFEX Jet Eta;#eta;Counts", 50, -5, 5);
    TH1F* back_h_jFex_subleading_eta = new TH1F("back_h_jFex_subleading_eta", "Sub-leading jFEX Jet Eta;#eta;Counts", 50, -5, 5);

    TH1F* back_h_jFex_leading_phi = new TH1F("back_h_jFex_leading_phi", "Leading jFEX Jet Phi;#phi;Counts", 64, -3.2, 3.2);
    TH1F* back_h_jFex_subleading_phi = new TH1F("back_h_jFex_subleading_phi", "Sub-leading jFEX Jet Phi;#phi;Counts", 64, -3.2, 3.2);

    TH1F* back_h_jFex_leading_subleading_deltaR = new TH1F("back_h_jFex_leading_subleading_deltaR", "Leading, subleading jFex SRJ DeltaR Dist.; #Delta R jFex Lead, Sublead SRJ; Normalized Events / 0.08", 100, 0, 8);

    TH1F* back_h_topo_Et_top128  = new TH1F("back_h_topo_Et_top128", "128 Highest Et Topo Cluster Et;Et (GeV);Counts", 20, 0, 400);
    TH1F* back_h_topo_eta_top128 = new TH1F("back_h_topo_eta_top128", "128 Highest Et Topo Cluster Eta;#eta;Counts", 50, -5, 5);
    TH1F* back_h_topo_phi_top128 = new TH1F("back_h_topo_phi_top128", "128 Highest Et Topo Cluster Phi;#phi;Counts", 64, -3.2, 3.2);

    // gFex, jFex jet deltaR^2 histogram
    TH1F* back_h_deltaR2_gFex_SRJ1 = new TH1F("back_h_deltaR2_gFex_SRJ1", "#DeltaR^{2} (gFex Highest Et Jet vs. topo422)", 25, 0, 10);
    TH1F* back_h_deltaR2_gFex_SRJ2 = new TH1F("back_h_deltaR2_gFex_SRJ2", "#DeltaR^{2} (gFex 2nd Highest Et Jet vs. topo422)", 25, 0, 10);
    TH1F* back_h_deltaR_gFex_SRJ1 = new TH1F("back_h_deltaR_gFex_SRJ1", "#DeltaR (gFex Highest Et Jet vs. topo422)", 25, 0, 5);
    TH1F* back_h_deltaR_gFex_SRJ2 = new TH1F("back_h_deltaR_gFex_SRJ2", "#DeltaR (gFex 2nd Highest Et Jet vs. topo422)", 25, 0, 5);

    TH1F* back_h_deltaR2_jFex_SRJ1 = new TH1F("back_h_deltaR2_jFex_SRJ1", "#DeltaR^{2} (jFex Highest Et Jet vs. topo422)", 25, 0, 10);
    TH1F* back_h_deltaR2_jFex_SRJ2 = new TH1F("back_h_deltaR2_jFex_SRJ2", "#DeltaR^{2} (jFex 2nd Highest Et Jet vs. topo422)", 25, 0, 10);
    TH1F* back_h_deltaR_jFex_SRJ1 = new TH1F("back_h_deltaR_jFex_SRJ1", "#DeltaR (jFex Highest Et Jet vs. topo422)", 25, 0, 5);
    TH1F* back_h_deltaR_jFex_SRJ2 = new TH1F("back_h_deltaR_jFex_SRJ2", "#DeltaR (jFex 2nd Highest Et Jet vs. topo422)", 25, 0, 5);

    TH1F* back_h_deltaR2_gFex_SRJ1_topo128 = new TH1F("back_h_deltaR2_gFex_SRJ1_topo128", "#DeltaR^{2} (gFex Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 10);
    TH1F* back_h_deltaR2_gFex_SRJ2_topo128 = new TH1F("back_h_deltaR2_gFex_SRJ2_topo128", "#DeltaR^{2} (gFex 2nd Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 10);
    TH1F* back_h_deltaR_gFex_SRJ1_topo128 = new TH1F("back_h_deltaR_gFex_SRJ1_topo128", "#DeltaR (gFex Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 5);
    TH1F* back_h_deltaR_gFex_SRJ2_topo128 = new TH1F("back_h_deltaR_gFex_SRJ2_topo128", "#DeltaR (gFex 2nd Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 5);

    TH1F* back_h_deltaR2_jFex_SRJ1_topo128 = new TH1F("back_h_deltaR2_jFex_SRJ1_topo128", "#DeltaR^{2} (gFex Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 10);
    TH1F* back_h_deltaR2_jFex_SRJ2_topo128 = new TH1F("back_h_deltaR2_jFex_SRJ2_topo128", "#DeltaR^{2} (gFex 2nd Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 10);
    TH1F* back_h_deltaR_jFex_SRJ1_topo128 = new TH1F("back_h_deltaR_jFex_SRJ1_topo128", "#DeltaR (gFex Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 5);
    TH1F* back_h_deltaR_jFex_SRJ2_topo128 = new TH1F("back_h_deltaR_jFex_SRJ2_topo128", "#DeltaR (gFex 2nd Highest Et Jet vs. 128 highest Et topo422)", 25, 0, 5);

    TH1F* sigdR2gFexLeadingTopoByEnergy = new TH1F("sigdR2gFexLeadingTopoByEnergy", "#Delta R^{2} Leading gFex SRJ, topo422 / 8 GeV", 20, 0, 400);
    TH1F* backdR2gFexLeadingTopoByEnergy = new TH1F("backdR2gFexLeadingTopoByEnergy", "#Delta R^{2} Leading gFex SRJ, topo422 / 8 GeV", 20, 0, 400);

    TH1F* sigdR2gFexSubLeadingTopoByEnergy = new TH1F("dR2gFexSubLeadingTopoByEnergy", "#Delta R^{2} Subleading gFex SRJ, topo422 / 8 GeV", 20, 0, 400);
    TH1F* backdR2gFexSubLeadingTopoByEnergy = new TH1F("backdR2gFexSubLeadingTopoByEnergy", "#Delta R^{2} Subleading gFex SRJ, topo422 / 8 GeV", 20, 0, 400);
    
    TH2F *sigdR2gFexLeadingTopoByEnergy2D = new TH2F("sigdR2gFexLeadingTopoByEnergy2D", "#Delta R^{2} Leading gFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading gFex SRJ, topo422", 
                        50, 0, 500,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *backdR2gFexLeadingTopoByEnergy2D = new TH2F("backdR2gFexLeadingTopoByEnergy2D", "#Delta R^{2} Leading gFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading gFex SRJ, topo422", 
                        50, 0, 500,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *sigdR2gFexSubLeadingTopoByEnergy2D = new TH2F("sigdR2gFexSubLeadingTopoByEnergy2D", "#Delta R^{2} Subleading gFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading gFex SRJ, topo422", 
                        50, 0, 500,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *backdR2gFexSubLeadingTopoByEnergy2D = new TH2F("backdR2gFexSubLeadingTopoByEnergy2D", "#Delta R^{2} Subleading gFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading gFex SRJ, topo422", 
                        50, 0, 500,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis

    TH2F *sigdR2gFexLeadingTopoByEnergy2D0to50GeV = new TH2F("sigdR2gFexLeadingTopoByEnergy2D0to50GeV", "#Delta R^{2} Leading gFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading gFex SRJ, topo422", 
                        100, 0, 50,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *backdR2gFexLeadingTopoByEnergy2D0to50GeV = new TH2F("backdR2gFexLeadingTopoByEnergy2D0to50GeV", "#Delta R^{2} Leading gFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading gFex SRJ, topo422", 
                        100, 0, 50,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *sigdR2gFexSubLeadingTopoByEnergy2D0to50GeV = new TH2F("sigdR2gFexSubLeadingTopoByEnergy2D0to50GeV", "#Delta R^{2} Subleading gFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading gFex SRJ, topo422", 
                        100, 0, 50,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *backdR2gFexSubLeadingTopoByEnergy2D0to50GeV = new TH2F("backdR2gFexSubLeadingTopoByEnergy2D0to50GeV", "#Delta R^{2} Subleading gFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading gFex SRJ, topo422", 
                        100, 0, 50,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis

    TH2F *sigdR2gFexLeadingTopoByEnergy2D50to550GeV = new TH2F("sigdR2gFexLeadingTopoByEnergy2D50to550GeV", "#Delta R^{2} Leading gFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading gFex SRJ, topo422", 
                        50, 50, 550,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *backdR2gFexLeadingTopoByEnergy2D50to550GeV = new TH2F("backdR2gFexLeadingTopoByEnergy2D50to550GeV", "#Delta R^{2} Leading gFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading gFex SRJ, topo422", 
                        50, 50, 550,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *sigdR2gFexSubLeadingTopoByEnergy2D50to550GeV = new TH2F("sigdR2gFexSubLeadingTopoByEnergy2D50to550GeV", "#Delta R^{2} Subleading gFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading gFex SRJ, topo422", 
                        50, 50, 550,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *backdR2gFexSubLeadingTopoByEnergy2D50to550GeV = new TH2F("backdR2gFexSubLeadingTopoByEnergy2D50to550GeV", "#Delta R^{2} Subleading gFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading gFex SRJ, topo422", 
                        50, 50, 550,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis


    TH2F *sigdR2jFexLeadingTopoByEnergy2D = new TH2F("sigdR2jFexLeadingTopoByEnergy2D", "#Delta R^{2} Leading jFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading jFex SRJ, topo422", 
                        50, 0, 500,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *backdR2jFexLeadingTopoByEnergy2D = new TH2F("backdR2jFexLeadingTopoByEnergy2D", "#Delta R^{2} Leading jFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading jFex SRJ, topo422", 
                        50, 0, 500,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *sigdR2jFexSubLeadingTopoByEnergy2D = new TH2F("sigdR2jFexSubLeadingTopoByEnergy2D", "#Delta R^{2} Subleading jFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading jFex SRJ, topo422", 
                        50, 0, 500,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *backdR2jFexSubLeadingTopoByEnergy2D = new TH2F("backdR2jFexSubLeadingTopoByEnergy2D", "#Delta R^{2} Subleading jFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading jFex SRJ, topo422", 
                        50, 0, 500,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis

    TH2F *sigdR2jFexLeadingTopoByEnergy2D0to50GeV = new TH2F("sigdR2jFexLeadingTopoByEnergy2D0to50GeV", "#Delta R^{2} Leading jFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading jFex SRJ, topo422", 
                        100, 0, 50,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *backdR2jFexLeadingTopoByEnergy2D0to50GeV = new TH2F("backdR2jFexLeadingTopoByEnergy2D0to50GeV", "#Delta R^{2} Leading jFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading jFex SRJ, topo422", 
                        100, 0, 50,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *sigdR2jFexSubLeadingTopoByEnergy2D0to50GeV = new TH2F("sigdR2jFexSubLeadingTopoByEnergy2D0to50GeV", "#Delta R^{2} Subleading jFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading jFex SRJ, topo422", 
                        100, 0, 50,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *backdR2jFexSubLeadingTopoByEnergy2D0to50GeV = new TH2F("backdR2jFexSubLeadingTopoByEnergy2D0to50GeV", "#Delta R^{2} Subleading jFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading jFex SRJ, topo422", 
                        100, 0, 50,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis

    TH2F *sigdR2jFexLeadingTopoByEnergy2D50to550GeV = new TH2F("sigdR2jFexLeadingTopoByEnergy2D50to550GeV", "#Delta R^{2} Leading jFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading jFex SRJ, topo422", 
                        50, 50, 550,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *backdR2jFexLeadingTopoByEnergy2D50to550GeV = new TH2F("backdR2jFexLeadingTopoByEnergy2D50to550GeV", "#Delta R^{2} Leading jFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading jFex SRJ, topo422", 
                        50, 50, 550,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *sigdR2jFexSubLeadingTopoByEnergy2D50to550GeV = new TH2F("sigdR2jFexSubLeadingTopoByEnergy2D50to550GeV", "#Delta R^{2} Subleading jFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading jFex SRJ, topo422", 
                        50, 50, 550,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    TH2F *backdR2jFexSubLeadingTopoByEnergy2D50to550GeV = new TH2F("backdR2jFexSubLeadingTopoByEnergy2D50to550GeV", "#Delta R^{2} Subleading jFex SRJ, topo422; Topo422 Energy [GeV];#Delta R^{2} Leading jFex SRJ, topo422", 
                        50, 50, 550,   // 50 bins from -5 to 5 on X axis
                        50, 0, 2.5);  // 50 bins from -5 to 5 on Y axis
    

    // Process trees
    sigTopoTree->SetBranchAddress("Et", &sigTopoEtValues);
    sigTopoTree->SetBranchAddress("Eta", &sigTopoEtaValues);
    sigTopoTree->SetBranchAddress("Phi", &sigTopoPhiValues);

    sigGFexTree->SetBranchAddress("Et", &sigGFexEtValues);
    sigGFexTree->SetBranchAddress("Eta", &sigGFexEtaValues);
    sigGFexTree->SetBranchAddress("Phi", &sigGFexPhiValues);

    sigJFexTree->SetBranchAddress("Et", &sigJFexEtValues);
    sigJFexTree->SetBranchAddress("Eta", &sigJFexEtaValues);
    sigJFexTree->SetBranchAddress("Phi", &sigJFexPhiValues);

    backTopoTree->SetBranchAddress("Et", &backTopoEtValues);
    backTopoTree->SetBranchAddress("Eta", &backTopoEtaValues);
    backTopoTree->SetBranchAddress("Phi", &backTopoPhiValues);

    backGFexTree->SetBranchAddress("Et", &backGFexEtValues);
    backGFexTree->SetBranchAddress("Eta", &backGFexEtaValues);
    backGFexTree->SetBranchAddress("Phi", &backGFexPhiValues);

    backJFexTree->SetBranchAddress("Et", &backJFexEtValues);
    backJFexTree->SetBranchAddress("Eta", &backJFexEtaValues);
    backJFexTree->SetBranchAddress("Phi", &backJFexPhiValues);
    const unsigned int nEnergyBins = 20;

    std::vector<std::vector<float>> sigdR2LeadinggFexSRJTopo422Energy(nEnergyBins);
    std::vector<std::vector<float>> backdR2LeadinggFexSRJTopo422Energy(nEnergyBins);
    std::vector<std::vector<float>> sigdR2SubLeadinggFexSRJTopo422Energy(nEnergyBins);
    std::vector<std::vector<float>> backdR2SubLeadinggFexSRJTopo422Energy(nEnergyBins);
    std::vector<double> sigTopo128EnergyValues1;
    std::vector<double> backTopo128EnergyValues1;
    std::vector<double> sigTopo128EnergyValues2;
    std::vector<double> backTopo128EnergyValues2;
    std::vector<double> sigdR2LeadinggFexSRJTopo128;
    std::vector<double> backdR2LeadinggFexSRJTopo128;
    std::vector<double> sigdR2SubLeadinggFexSRJTopo128;
    std::vector<double> backdR2SubLeadinggFexSRJTopo128;

    int sigNEntries = sigGFexTree->GetEntries();
    std::cout << " sigNEntries : "<< sigNEntries << "\n";
    for (int i = 0; i < sigNEntries; i++) {
        sigGFexTree->GetEntry(i);
        sigJFexTree->GetEntry(i);
        sig_h_gFex_multiplicity->Fill(sigGFexEtValues->size());
        for (size_t j = 0; j < sigGFexEtValues->size(); j++) {
            sig_h_gFex_Et->Fill(sigGFexEtValues->at(j));
            sig_h_gFex_eta->Fill(sigGFexEtaValues->at(j));
            sig_h_gFex_phi->Fill(sigGFexPhiValues->at(j));
        }

        // Find two highest Et jets
        auto [idx1, idx2] = getTwoHighestIndices(*sigGFexEtValues);
        auto [idx1_jFex, idx2_jFex] = getTwoHighestIndices(*sigJFexEtValues);

        // Leading jet
        sig_h_gFex_leading_Et->Fill(sigGFexEtValues->at(idx1));
        sig_h_gFex_leading_eta->Fill(sigGFexEtaValues->at(idx1));
        sig_h_gFex_leading_phi->Fill(sigGFexPhiValues->at(idx1));

        // Sub-leading jet
        sig_h_gFex_subleading_Et->Fill(sigGFexEtValues->at(idx2));
        sig_h_gFex_subleading_eta->Fill(sigGFexEtaValues->at(idx2));
        sig_h_gFex_subleading_phi->Fill(sigGFexPhiValues->at(idx2));
        sig_h_gFex_leading_subleading_deltaR->Fill(sqrt(calcDeltaR2(sigGFexEtaValues->at(idx1), sigGFexPhiValues->at(idx1), sigGFexEtaValues->at(idx2), sigGFexPhiValues->at(idx2))));

        sigTopoTree->GetEntry(i);
        //std::cout << "sigTopoEtValues->size(): " << sigTopoEtValues->size() << "\n";
        sig_h_topo_multiplicity->Fill(sigTopoEtValues->size());
        for (size_t k = 0; k < sigTopoEtValues->size(); k++) {
            nTopo += 1;
            //std::cout << "sigTopoEtValues->at(k): " << sigTopoEtValues->at(k) << " for k: " << k << "\n";
            totalTopoEnergy += sigTopoEtValues->at(k);
            if (sigTopoEtValues->at(k) > 1.0){
                nTopoGreater1GeV += 1;
                topoEnergyClustersGreater1GeV += sigTopoEtValues->at(k);
            } 
            
            sig_h_topo_Et->Fill(sigTopoEtValues->at(k));
            sig_h_topo_eta->Fill(sigTopoEtaValues->at(k)); // FIXME rename all GFex, Topo variables to something like input object, seed
            sig_h_topo_phi->Fill(sigTopoPhiValues->at(k));
            double dr2_1 = ((sigGFexEtaValues->at(idx1)) - sigTopoEtaValues->at(k))*((sigGFexEtaValues->at(idx1)) - sigTopoEtaValues->at(k)) + deltaPhi(sigGFexPhiValues->at(idx1), sigTopoPhiValues->at(k))*deltaPhi(sigGFexPhiValues->at(idx1), sigTopoPhiValues->at(k));
            sig_h_deltaR2_gFex_SRJ1->Fill(dr2_1);
            sig_h_deltaR_gFex_SRJ1->Fill(sqrt(dr2_1));
            double dr2_2 = ((sigGFexEtaValues->at(idx2)) - sigTopoEtaValues->at(k))*((sigGFexEtaValues->at(idx2)) - sigTopoEtaValues->at(k)) + deltaPhi(sigGFexPhiValues->at(idx2), sigTopoPhiValues->at(k))*deltaPhi(sigGFexPhiValues->at(idx2), sigTopoPhiValues->at(k));
            sig_h_deltaR2_gFex_SRJ2->Fill(dr2_2);
            sig_h_deltaR_gFex_SRJ2->Fill(sqrt(dr2_2));
            double dr2_1_jFex = ((sigJFexEtaValues->at(idx1_jFex)) - sigTopoEtaValues->at(k))*((sigJFexEtaValues->at(idx1_jFex)) - sigTopoEtaValues->at(k)) + deltaPhi(sigJFexPhiValues->at(idx1_jFex), sigTopoPhiValues->at(k))*deltaPhi(sigJFexPhiValues->at(idx1_jFex), sigTopoPhiValues->at(k));
            sig_h_deltaR2_jFex_SRJ1->Fill(dr2_1_jFex);
            sig_h_deltaR_jFex_SRJ1->Fill(sqrt(dr2_1_jFex));
            double dr2_2_jFex = ((sigJFexEtaValues->at(idx2_jFex)) - sigTopoEtaValues->at(k))*((sigJFexEtaValues->at(idx2_jFex)) - sigTopoEtaValues->at(k)) + deltaPhi(sigJFexPhiValues->at(idx2_jFex), sigTopoPhiValues->at(k))*deltaPhi(sigJFexPhiValues->at(idx2_jFex), sigTopoPhiValues->at(k));
            sig_h_deltaR2_jFex_SRJ2->Fill(dr2_2_jFex);
            sig_h_deltaR_jFex_SRJ2->Fill(sqrt(dr2_2_jFex));
            if (k < 128){
                sig_h_topo_Et_top128->Fill(sigTopoEtValues->at(k));
                sig_h_topo_eta_top128->Fill(sigTopoEtaValues->at(k));
                sig_h_topo_phi_top128->Fill(sigTopoPhiValues->at(k));

                sig_h_deltaR2_gFex_SRJ1_topo128->Fill(dr2_1);
                sig_h_deltaR_gFex_SRJ1_topo128->Fill(sqrt(dr2_1));
                sig_h_deltaR2_gFex_SRJ2_topo128->Fill(dr2_2);
                sig_h_deltaR_gFex_SRJ2_topo128->Fill(sqrt(dr2_2));

                sig_h_deltaR2_jFex_SRJ1_topo128->Fill(dr2_1_jFex);
                sig_h_deltaR_jFex_SRJ1_topo128->Fill(sqrt(dr2_1_jFex));
                sig_h_deltaR2_jFex_SRJ2_topo128->Fill(dr2_2_jFex);
                sig_h_deltaR_jFex_SRJ2_topo128->Fill(sqrt(dr2_2_jFex));

                if (dr2_1 < 2.5){
                    sigTopo128EnergyValues1.push_back(sigTopoEtValues->at(k));
                    sigdR2LeadinggFexSRJTopo128.push_back(dr2_1);
                    sigdR2gFexLeadingTopoByEnergy2D->Fill(sigTopoEtValues->at(k), dr2_1);
                    sigdR2jFexLeadingTopoByEnergy2D->Fill(sigTopoEtValues->at(k), dr2_1_jFex);
                    if (sigTopoEtValues->at(k) <= 50.0){
                        sigdR2gFexLeadingTopoByEnergy2D0to50GeV->Fill(sigTopoEtValues->at(k), dr2_1);
                        sigdR2jFexLeadingTopoByEnergy2D0to50GeV->Fill(sigTopoEtValues->at(k), dr2_1_jFex);
                    }
                    if ((sigTopoEtValues->at(k) > 50.0) && (sigTopoEtValues->at(k) <= 550.0)){
                        sigdR2gFexLeadingTopoByEnergy2D50to550GeV->Fill(sigTopoEtValues->at(k), dr2_1);
                        sigdR2jFexLeadingTopoByEnergy2D50to550GeV->Fill(sigTopoEtValues->at(k), dr2_1_jFex);
                    }
                    
                }
                if (dr2_2 < 2.5){
                    sigTopo128EnergyValues2.push_back(sigTopoEtValues->at(k));
                    sigdR2SubLeadinggFexSRJTopo128.push_back(dr2_2);
                    sigdR2gFexSubLeadingTopoByEnergy2D->Fill(sigTopoEtValues->at(k), dr2_2);
                    sigdR2jFexSubLeadingTopoByEnergy2D->Fill(sigTopoEtValues->at(k), dr2_2_jFex);
                    if (sigTopoEtValues->at(k) <= 50.0){
                        sigdR2gFexSubLeadingTopoByEnergy2D0to50GeV->Fill(sigTopoEtValues->at(k), dr2_2);
                        sigdR2jFexSubLeadingTopoByEnergy2D0to50GeV->Fill(sigTopoEtValues->at(k), dr2_2_jFex);
                    }
                    if ((sigTopoEtValues->at(k) > 50.0) && (sigTopoEtValues->at(k) <= 550.0)){
                        sigdR2gFexSubLeadingTopoByEnergy2D50to550GeV->Fill(sigTopoEtValues->at(k), dr2_2);
                        sigdR2jFexSubLeadingTopoByEnergy2D50to550GeV->Fill(sigTopoEtValues->at(k), dr2_2_jFex);
                    }
                }   
                
                

                //std::cout << "sigTopoEtValues->at(k): " << sigTopoEtValues->at(k) << "\n";
                unsigned int energyBin = sig_h_topo_Et_top128->FindBin(sigTopoEtValues->at(k));
                //std::cout << "energy bin sig: " << energyBin << "\n";
                if (energyBin > nEnergyBins){
                    //std::cout << "SIGNAL ENERGY OUTLIER WITH: " << sigTopoEtValues->at(k) << " [GeV]" << "\n";
                    energyBin = nEnergyBins;
                }
                sigdR2LeadinggFexSRJTopo422Energy[energyBin - 1].push_back(dr2_1);
                sigdR2SubLeadinggFexSRJTopo422Energy[energyBin - 1].push_back(dr2_2);
            }
        }
        
    }

    for (int i = 0; i < sigJFexTree->GetEntries(); i++) {
        sigJFexTree->GetEntry(i);
        //std::cout << "sigJFexEtValues->size(): " << sigJFexEtValues->size() << "\n";
        sig_h_jFex_multiplicity->Fill(sigJFexEtValues->size());
        for (float et : *sigJFexEtValues) sig_h_jFex_Et->Fill(et);
        for (float eta : *sigJFexEtaValues) sig_h_jFex_eta->Fill(eta);
        for (float phi : *sigJFexPhiValues) sig_h_jFex_phi->Fill(phi);

        auto [idx1, idx2] = getTwoHighestIndices(*sigJFexEtValues);

        // Leading jet
        sig_h_jFex_leading_Et->Fill(sigJFexEtValues->at(idx1));
        //if (sigJFexEtValues->at(idx1) > 100.0){
        //   std::cout << "sigJFexEtValues->at(idx1): " << sigJFexEtValues->at(idx1) << "\n";
        //}
        
        sig_h_jFex_leading_eta->Fill(sigJFexEtaValues->at(idx1));
        sig_h_jFex_leading_phi->Fill(sigJFexPhiValues->at(idx1));

        // Sub-leading jet
        sig_h_jFex_subleading_Et->Fill(sigJFexEtValues->at(idx2));
        sig_h_jFex_subleading_eta->Fill(sigJFexEtaValues->at(idx2));
        sig_h_jFex_subleading_phi->Fill(sigJFexPhiValues->at(idx2));

        sig_h_jFex_leading_subleading_deltaR->Fill(sqrt(calcDeltaR2(sigJFexEtaValues->at(idx1), sigJFexPhiValues->at(idx1), sigJFexEtaValues->at(idx2), sigJFexPhiValues->at(idx2))));
    }

    int backNEntries = backGFexTree->GetEntries();
    std::cout << " backNEntries: " << backNEntries << "\n";
    for (int i = 0; i < backNEntries; i++) {
        backGFexTree->GetEntry(i);
        backJFexTree->GetEntry(i);
        back_h_gFex_multiplicity->Fill(backGFexEtValues->size());
        for (size_t j = 0; j < backGFexEtValues->size(); j++) {
            back_h_gFex_Et->Fill(backGFexEtValues->at(j));
            back_h_gFex_eta->Fill(backGFexEtaValues->at(j));
            back_h_gFex_phi->Fill(backGFexPhiValues->at(j));
        }

        // Find two highest Et jets
        auto [idx1, idx2] = getTwoHighestIndices(*backGFexEtValues);

        auto [idx1_jFex, idx2_jFex] = getTwoHighestIndices(*backJFexEtValues);

        // Leading jet
        back_h_gFex_leading_Et->Fill(backGFexEtValues->at(idx1));
        back_h_gFex_leading_eta->Fill(backGFexEtaValues->at(idx1));
        back_h_gFex_leading_phi->Fill(backGFexPhiValues->at(idx1));

        // Sub-leading jet
        back_h_gFex_subleading_Et->Fill(backGFexEtValues->at(idx2));
        back_h_gFex_subleading_eta->Fill(backGFexEtaValues->at(idx2));
        back_h_gFex_subleading_phi->Fill(backGFexPhiValues->at(idx2));

        back_h_gFex_leading_subleading_deltaR->Fill(sqrt(calcDeltaR2(backGFexEtaValues->at(idx1), backGFexPhiValues->at(idx1), backGFexEtaValues->at(idx2), backGFexPhiValues->at(idx2))));
        

        backTopoTree->GetEntry(i);
        //std::cout << "topoEtValues->size(): " << backTopoEtValues->size() << "\n";
        back_h_topo_multiplicity->Fill(backTopoEtValues->size());
        for (size_t k = 0; k < backTopoEtValues->size(); k++) {
            back_h_topo_Et->Fill(backTopoEtValues->at(k));
            back_h_topo_eta->Fill(backTopoEtaValues->at(k));
            back_h_topo_phi->Fill(backTopoPhiValues->at(k));
            double dr2_1 = ((backGFexEtaValues->at(idx1)) - backTopoEtaValues->at(k))*((backGFexEtaValues->at(idx1)) - backTopoEtaValues->at(k)) + deltaPhi(backGFexPhiValues->at(idx1), backTopoPhiValues->at(k))*deltaPhi(backGFexPhiValues->at(idx1), backTopoPhiValues->at(k));
            //std::cout << "background dr2_1: " << dr2_1 << " and dr: " << sqrt(dr2_1) << "\n";
            back_h_deltaR2_gFex_SRJ1->Fill(dr2_1);
            back_h_deltaR_gFex_SRJ1->Fill(sqrt(dr2_1));
            double dr2_2 = ((backGFexEtaValues->at(idx2)) - backTopoEtaValues->at(k))*((backGFexEtaValues->at(idx2)) - backTopoEtaValues->at(k)) + deltaPhi(backGFexPhiValues->at(idx2), backTopoPhiValues->at(k))*deltaPhi(backGFexPhiValues->at(idx2), backTopoPhiValues->at(k));
            back_h_deltaR2_gFex_SRJ2->Fill(dr2_2);
            back_h_deltaR_gFex_SRJ2->Fill(sqrt(dr2_2));


            double dr2_1_jFex = ((backJFexEtaValues->at(idx1_jFex)) - backTopoEtaValues->at(k))*((backJFexEtaValues->at(idx1_jFex)) - backTopoEtaValues->at(k)) + deltaPhi(backJFexPhiValues->at(idx1_jFex), backTopoPhiValues->at(k))*deltaPhi(backJFexPhiValues->at(idx1_jFex), backTopoPhiValues->at(k));
            double dr2_2_jFex = ((backJFexEtaValues->at(idx2_jFex)) - backTopoEtaValues->at(k))*((backJFexEtaValues->at(idx2_jFex)) - backTopoEtaValues->at(k)) + deltaPhi(backJFexPhiValues->at(idx2_jFex), backTopoPhiValues->at(k))*deltaPhi(backJFexPhiValues->at(idx2_jFex), backTopoPhiValues->at(k));
            back_h_deltaR2_jFex_SRJ1->Fill(dr2_1_jFex);
            back_h_deltaR_jFex_SRJ1->Fill(sqrt(dr2_1_jFex));
            back_h_deltaR2_jFex_SRJ2->Fill(dr2_2_jFex);
            back_h_deltaR_jFex_SRJ2->Fill(sqrt(dr2_2_jFex));


            if (k < 128){
                back_h_topo_Et_top128->Fill(backTopoEtValues->at(k));
                back_h_topo_eta_top128->Fill(backTopoEtaValues->at(k));
                back_h_topo_phi_top128->Fill(backTopoPhiValues->at(k));
                back_h_deltaR2_gFex_SRJ1_topo128->Fill(dr2_1);
                back_h_deltaR_gFex_SRJ1_topo128->Fill(sqrt(dr2_1));
                back_h_deltaR2_gFex_SRJ2_topo128->Fill(dr2_2);
                back_h_deltaR_gFex_SRJ2_topo128->Fill(sqrt(dr2_2));

                back_h_deltaR2_jFex_SRJ1_topo128->Fill(dr2_1_jFex);
                back_h_deltaR_jFex_SRJ1_topo128->Fill(sqrt(dr2_1_jFex));
                back_h_deltaR2_jFex_SRJ2_topo128->Fill(dr2_2_jFex);
                back_h_deltaR_jFex_SRJ2_topo128->Fill(sqrt(dr2_2_jFex));

                if (dr2_1 < 5.0){
                    backTopo128EnergyValues1.push_back(backTopoEtValues->at(k));
                    backdR2LeadinggFexSRJTopo128.push_back(dr2_1);
                    backdR2gFexLeadingTopoByEnergy2D->Fill(backTopoEtValues->at(k), dr2_1);
                    backdR2jFexLeadingTopoByEnergy2D->Fill(backTopoEtValues->at(k), dr2_1_jFex);
                    if (backTopoEtValues->at(k) <= 50.0){
                        backdR2gFexLeadingTopoByEnergy2D0to50GeV->Fill(backTopoEtValues->at(k), dr2_1);
                        backdR2jFexLeadingTopoByEnergy2D0to50GeV->Fill(backTopoEtValues->at(k), dr2_1_jFex);
                    }
                    if ((backTopoEtValues->at(k) > 50.0) && (backTopoEtValues->at(k) <= 550.0)){
                        backdR2gFexLeadingTopoByEnergy2D50to550GeV->Fill(backTopoEtValues->at(k), dr2_1);
                        backdR2jFexLeadingTopoByEnergy2D50to550GeV->Fill(backTopoEtValues->at(k), dr2_1_jFex);
                    }
                }
                if (dr2_2 < 5.0){
                    backTopo128EnergyValues2.push_back(backTopoEtValues->at(k));
                    backdR2SubLeadinggFexSRJTopo128.push_back(dr2_1);
                    backdR2gFexSubLeadingTopoByEnergy2D->Fill(backTopoEtValues->at(k), dr2_2);
                    backdR2jFexSubLeadingTopoByEnergy2D->Fill(backTopoEtValues->at(k), dr2_2_jFex);
                    if (backTopoEtValues->at(k) <= 50.0){
                        backdR2gFexSubLeadingTopoByEnergy2D0to50GeV->Fill(backTopoEtValues->at(k), dr2_2);
                        backdR2jFexSubLeadingTopoByEnergy2D0to50GeV->Fill(backTopoEtValues->at(k), dr2_2_jFex);
                    }
                    if ((backTopoEtValues->at(k) > 50.0) && (backTopoEtValues->at(k) <= 550.0)){
                        backdR2gFexSubLeadingTopoByEnergy2D50to550GeV->Fill(backTopoEtValues->at(k), dr2_2);
                        backdR2jFexSubLeadingTopoByEnergy2D50to550GeV->Fill(backTopoEtValues->at(k), dr2_2_jFex);
                    }
                }
                

                unsigned int energyBin = back_h_topo_Et_top128->FindBin(backTopoEtValues->at(k));
                if (energyBin > nEnergyBins){
                    //std::cout << "BACKGROUND ENERGY OUTLIER WITH: " << backTopoEtValues->at(k) << " [GeV]" << "\n";
                    energyBin = nEnergyBins;
                }
                backdR2LeadinggFexSRJTopo422Energy[energyBin - 1].push_back(dr2_1);
                backdR2SubLeadinggFexSRJTopo422Energy[energyBin - 1].push_back(dr2_2);

            }
        }
        
    }

    for (int i = 0; i < backJFexTree->GetEntries(); i++) {
        backJFexTree->GetEntry(i);
        //std::cout << "backJFexEtValues->size(): " << backJFexEtValues->size() << "\n";
        back_h_jFex_multiplicity->Fill(backJFexEtValues->size());
        for (float et : *backJFexEtValues) back_h_jFex_Et->Fill(et);
        for (float eta : *backJFexEtaValues) back_h_jFex_eta->Fill(eta);
        for (float phi : *backJFexPhiValues) back_h_jFex_phi->Fill(phi);

        auto [idx1, idx2] = getTwoHighestIndices(*backJFexEtValues);

        // Leading jet
        back_h_jFex_leading_Et->Fill(backJFexEtValues->at(idx1));
        if (backJFexEtValues->at(idx1) > 100.0){
            std::cout << "backJFexEtValues->at(idx1): " << backJFexEtValues->at(idx1) << "\n";
        }
        back_h_jFex_leading_eta->Fill(backJFexEtaValues->at(idx1));
        back_h_jFex_leading_phi->Fill(backJFexPhiValues->at(idx1));

        // Sub-leading jet
        back_h_jFex_subleading_Et->Fill(backJFexEtValues->at(idx2));
        back_h_jFex_subleading_eta->Fill(backJFexEtaValues->at(idx2));
        back_h_jFex_subleading_phi->Fill(backJFexPhiValues->at(idx2));

        back_h_jFex_leading_subleading_deltaR->Fill(sqrt(calcDeltaR2(backJFexEtaValues->at(idx1), backJFexPhiValues->at(idx1), backJFexEtaValues->at(idx2), backJFexPhiValues->at(idx2))));
    }

    for (int bin = 0; bin < nEnergyBins; ++bin){
        //std::cout << "bin: " << bin << "\n";
        if (bin == (nEnergyBins - 1)){
            std::cout << "last bin mean (sig, leading): " << computeMean(sigdR2LeadinggFexSRJTopo422Energy[bin]);
            std::cout << " sig n entries for last bin: " << sigdR2LeadinggFexSRJTopo422Energy[bin].size() << "\n";

            std::cout << "last bin mean (back, leading): " << computeMean(backdR2LeadinggFexSRJTopo422Energy[bin]);
            std::cout << " back n entries for last bin: " << backdR2LeadinggFexSRJTopo422Energy[bin].size() << "\n";
        } 
        sigdR2gFexLeadingTopoByEnergy->SetBinContent(bin + 1, computeMean(sigdR2LeadinggFexSRJTopo422Energy[bin]));
        backdR2gFexLeadingTopoByEnergy->SetBinContent(bin + 1, computeMean(backdR2LeadinggFexSRJTopo422Energy[bin]));
        sigdR2gFexSubLeadingTopoByEnergy->SetBinContent(bin + 1, computeMean(sigdR2SubLeadinggFexSRJTopo422Energy[bin]));
        backdR2gFexSubLeadingTopoByEnergy->SetBinContent(bin + 1, computeMean(backdR2SubLeadinggFexSRJTopo422Energy[bin]));
        sigdR2gFexLeadingTopoByEnergy->SetBinError(bin + 1, computeStandardError(sigdR2LeadinggFexSRJTopo422Energy[bin], computeStandardDeviation(sigdR2LeadinggFexSRJTopo422Energy[bin], computeMean(sigdR2LeadinggFexSRJTopo422Energy[bin]))));
        backdR2gFexLeadingTopoByEnergy->SetBinError(bin + 1, computeStandardError(backdR2LeadinggFexSRJTopo422Energy[bin], computeStandardDeviation(backdR2LeadinggFexSRJTopo422Energy[bin], computeMean(backdR2LeadinggFexSRJTopo422Energy[bin]))));
        sigdR2gFexSubLeadingTopoByEnergy->SetBinError(bin + 1, computeStandardError(sigdR2SubLeadinggFexSRJTopo422Energy[bin], computeStandardDeviation(sigdR2SubLeadinggFexSRJTopo422Energy[bin], computeMean(sigdR2SubLeadinggFexSRJTopo422Energy[bin]))));
        backdR2gFexSubLeadingTopoByEnergy->SetBinError(bin + 1, computeStandardError(backdR2SubLeadinggFexSRJTopo422Energy[bin], computeStandardDeviation(backdR2SubLeadinggFexSRJTopo422Energy[bin], computeMean(backdR2SubLeadinggFexSRJTopo422Energy[bin]))));
    }

    // Save histograms
    TString outputFileDir = "overlayHistograms/";

    TCanvas c2D = new TCanvas("c2D", "2D Histogram", 1000, 600);  // wider than 800

    
    sigdR2gFexLeadingTopoByEnergy2D->Scale(1.0 / sigdR2gFexLeadingTopoByEnergy2D->Integral());
    // Get number of bins in Y and the bin corresponding to y = 1.0
    int yBinCut1 = sigdR2gFexLeadingTopoByEnergy2D->GetYaxis()->FindBin(1.0);

    // Integrate over all X bins and Y bins from 1 up to (but not including) yBinCut
    int nBinsX1 = sigdR2gFexLeadingTopoByEnergy2D->GetNbinsX();
    int nBinsY1 = sigdR2gFexLeadingTopoByEnergy2D->GetNbinsY();

    double integralBelowCut1 = sigdR2gFexLeadingTopoByEnergy2D->Integral(1, nBinsX1, 1, yBinCut1 - 1);
    double totalIntegral1 = sigdR2gFexLeadingTopoByEnergy2D->Integral();

    double fractionBelowCut1 = integralBelowCut1 / totalIntegral1;
    char* label1 = Form("%.3f", fractionBelowCut1);
    std::cout << "sig gfex leading fraction below deltaR^2: " << fractionBelowCut1 << "\n";
    mySmallText(0.6, 0.6, 0, label1);
    TLine *line = new TLine(sigdR2gFexLeadingTopoByEnergy2D->GetXaxis()->GetXmin(), 1.0, sigdR2gFexLeadingTopoByEnergy2D->GetXaxis()->GetXmax(), 1.0); // xMin/xMax = x-axis limits
    line->SetLineColor(kBlack);
    line->SetLineWidth(2);
    line->SetLineStyle(2);  // dashed
    
    gStyle->SetPalette(1);
    sigdR2gFexLeadingTopoByEnergy2D->Draw("COLZ");  
    gPad->Update();
    TPaletteAxis *palette1 = (TPaletteAxis*)sigdR2gFexLeadingTopoByEnergy2D->GetListOfFunctions()->FindObject("palette");
    //palette1->SetLabelSize(0.15);
    sigdR2gFexLeadingTopoByEnergy2D->GetZaxis()->SetLabelSize(0.03);
    
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "sigdR2gFexLeadingTopoByEnergy2D.pdf");


    gStyle->SetPalette(1);
    sigdR2gFexLeadingTopoByEnergy2D0to50GeV->Scale(1.0 / sigdR2gFexLeadingTopoByEnergy2D0to50GeV->Integral());
    sigdR2gFexLeadingTopoByEnergy2D0to50GeV->Draw("COLZ");  
    gPad->Update();
    sigdR2gFexLeadingTopoByEnergy2D0to50GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "sigdR2gFexLeadingTopoByEnergy2D0to50GeV.pdf");


    gStyle->SetPalette(1);
    backdR2gFexLeadingTopoByEnergy2D0to50GeV->Scale(1.0 / backdR2gFexLeadingTopoByEnergy2D0to50GeV->Integral());
    backdR2gFexLeadingTopoByEnergy2D0to50GeV->Draw("COLZ");  
    gPad->Update();
    backdR2gFexLeadingTopoByEnergy2D0to50GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "backdR2gFexLeadingTopoByEnergy2D0to50GeV.pdf");

    gStyle->SetPalette(1);
    sigdR2gFexLeadingTopoByEnergy2D50to550GeV->Scale(1.0 / sigdR2gFexLeadingTopoByEnergy2D50to550GeV->Integral());
    sigdR2gFexLeadingTopoByEnergy2D50to550GeV->Draw("COLZ");  
    gPad->Update();
    sigdR2gFexLeadingTopoByEnergy2D50to550GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "sigdR2gFexLeadingTopoByEnergy2D50to550GeV.pdf");

    gStyle->SetPalette(1);
    backdR2gFexLeadingTopoByEnergy2D50to550GeV->Scale(1.0 / backdR2gFexLeadingTopoByEnergy2D50to550GeV->Integral());
    backdR2gFexLeadingTopoByEnergy2D50to550GeV->Draw("COLZ");  
    gPad->Update();
    backdR2gFexLeadingTopoByEnergy2D50to550GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "backdR2gFexLeadingTopoByEnergy2D50to550GeV.pdf");


    gStyle->SetPalette(1);
    sigdR2gFexSubLeadingTopoByEnergy2D0to50GeV->Scale(1.0 / sigdR2gFexSubLeadingTopoByEnergy2D0to50GeV->Integral());
    sigdR2gFexSubLeadingTopoByEnergy2D0to50GeV->Draw("COLZ");  
    gPad->Update();
    sigdR2gFexSubLeadingTopoByEnergy2D0to50GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "sigdR2gFexSubLeadingTopoByEnergy2D0to50GeV.pdf");


    gStyle->SetPalette(1);
    backdR2gFexSubLeadingTopoByEnergy2D0to50GeV->Scale(1.0 / backdR2gFexSubLeadingTopoByEnergy2D0to50GeV->Integral());
    backdR2gFexSubLeadingTopoByEnergy2D0to50GeV->Draw("COLZ");  
    gPad->Update();
    backdR2gFexSubLeadingTopoByEnergy2D0to50GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "backdR2gFexSubLeadingTopoByEnergy2D0to50GeV.pdf");

    gStyle->SetPalette(1);
    sigdR2gFexSubLeadingTopoByEnergy2D50to550GeV->Scale(1.0 / sigdR2gFexSubLeadingTopoByEnergy2D50to550GeV->Integral());
    sigdR2gFexSubLeadingTopoByEnergy2D50to550GeV->Draw("COLZ");  
    gPad->Update();
    sigdR2gFexSubLeadingTopoByEnergy2D50to550GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "sigdR2gFexSubLeadingTopoByEnergy2D50to550GeV.pdf");

    gStyle->SetPalette(1);
    backdR2gFexSubLeadingTopoByEnergy2D50to550GeV->Scale(1.0 / backdR2gFexSubLeadingTopoByEnergy2D50to550GeV->Integral());
    backdR2gFexSubLeadingTopoByEnergy2D50to550GeV->Draw("COLZ");  
    gPad->Update();
    backdR2gFexSubLeadingTopoByEnergy2D50to550GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "backdR2gFexSubLeadingTopoByEnergy2D50to550GeV.pdf");

    gStyle->SetPalette(1);
    backdR2gFexLeadingTopoByEnergy2D->Scale(1.0 / backdR2gFexLeadingTopoByEnergy2D->Integral());
    // Get number of bins in Y and the bin corresponding to y = 1.0
    int yBinCut2 = backdR2gFexLeadingTopoByEnergy2D->GetYaxis()->FindBin(1.0);

    // Integrate over all X bins and Y bins from 1 up to (but not including) yBinCut
    int nBinsX2 = backdR2gFexLeadingTopoByEnergy2D->GetNbinsX();
    int nBinsY2 = backdR2gFexLeadingTopoByEnergy2D->GetNbinsY();

    double integralBelowCut2 = backdR2gFexLeadingTopoByEnergy2D->Integral(1, nBinsX2, 1, yBinCut2 - 1);
    double totalIntegral2 = backdR2gFexLeadingTopoByEnergy2D->Integral();

    double fractionBelowCut2 = integralBelowCut2 / totalIntegral2;
    std::cout << "back gfex leading fraction below deltaR^2: " << fractionBelowCut2 << "\n";
    char* label2 = Form("%.3f", fractionBelowCut2);
    mySmallText(0.6, 0.6, 0, label2);
    
    backdR2gFexLeadingTopoByEnergy2D->Draw("COLZ");  
    gPad->Update();
    TPaletteAxis *palette2 = (TPaletteAxis*)backdR2gFexLeadingTopoByEnergy2D->GetListOfFunctions()->FindObject("palette");
    //palette2->SetLabelSize(0.15);
    backdR2gFexLeadingTopoByEnergy2D->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "backdR2gFexLeadingTopoByEnergy2D.pdf");

    gStyle->SetPalette(1);
    sigdR2gFexSubLeadingTopoByEnergy2D->Scale(1.0 / sigdR2gFexSubLeadingTopoByEnergy2D->Integral());
    // Get number of bins in Y and the bin corresponding to y = 1.0
    int yBinCut3 = sigdR2gFexSubLeadingTopoByEnergy2D->GetYaxis()->FindBin(1.0);

    // Integrate over all X bins and Y bins from 1 up to (but not including) yBinCut
    int nBinsX3 = sigdR2gFexSubLeadingTopoByEnergy2D->GetNbinsX();
    int nBinsY3 = sigdR2gFexSubLeadingTopoByEnergy2D->GetNbinsY();

    double integralBelowCut3 = sigdR2gFexSubLeadingTopoByEnergy2D->Integral(1, nBinsX3, 1, yBinCut3 - 1);
    double totalIntegral3 = sigdR2gFexSubLeadingTopoByEnergy2D->Integral();

    double fractionBelowCut3 = integralBelowCut3 / totalIntegral3;
    char* label3 = Form("%.3f", fractionBelowCut3);
    std::cout << "sig gfex subleading fraction below deltaR^2: " << fractionBelowCut3 << "\n";
    mySmallText(0.6, 0.6, 0, label3);
    sigdR2gFexSubLeadingTopoByEnergy2D->Draw("COLZ");  
    gPad->Update();
    TPaletteAxis *palette3 = (TPaletteAxis*)sigdR2gFexSubLeadingTopoByEnergy2D->GetListOfFunctions()->FindObject("palette");
    //palette3->SetLabelSize(0.15);
    sigdR2gFexSubLeadingTopoByEnergy2D->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "sigdR2gFexSubLeadingTopoByEnergy2D.pdf");
    
    backdR2gFexSubLeadingTopoByEnergy2D->Scale(1.0 / backdR2gFexSubLeadingTopoByEnergy2D->Integral());
    // Get number of bins in Y and the bin corresponding to y = 1.0
    int yBinCut4 = backdR2gFexSubLeadingTopoByEnergy2D->GetYaxis()->FindBin(1.0);

    // Integrate over all X bins and Y bins from 1 up to (but not including) yBinCut
    int nBinsX4 = backdR2gFexSubLeadingTopoByEnergy2D->GetNbinsX();
    int nBinsY4 = backdR2gFexSubLeadingTopoByEnergy2D->GetNbinsY();

    double integralBelowCut4 = backdR2gFexSubLeadingTopoByEnergy2D->Integral(1, nBinsX4, 1, yBinCut4 - 1);
    double totalIntegral4 = backdR2gFexSubLeadingTopoByEnergy2D->Integral();

    double fractionBelowCut4 = integralBelowCut4 / totalIntegral4;
    char* label4 = Form("%.3f", fractionBelowCut4);
    std::cout << "back gfex subleading fraction below deltaR^2: " << fractionBelowCut4 << "\n";
    mySmallText(0.6, 0.6, 0, label4);
    gStyle->SetPalette(1);
    backdR2gFexSubLeadingTopoByEnergy2D->Draw("COLZ");  
    gPad->Update();
    TPaletteAxis *palette4 = (TPaletteAxis*)backdR2gFexSubLeadingTopoByEnergy2D->GetListOfFunctions()->FindObject("palette");
    //palette4->SetLabelSize(0.15);
    backdR2gFexSubLeadingTopoByEnergy2D->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "backdR2gFexSubLeadingTopoByEnergy2D.pdf");

    sigdR2jFexLeadingTopoByEnergy2D->Scale(1.0 / sigdR2jFexLeadingTopoByEnergy2D->Integral());
    // Get number of bins in Y and the bin corresponding to y = 1.0
    int yBinCutJ1 = sigdR2jFexLeadingTopoByEnergy2D->GetYaxis()->FindBin(1.0);

    // Integrate over all X bins and Y bins from 1 up to (but not including) yBinCutJ
    int nBinsXJ1 = sigdR2jFexLeadingTopoByEnergy2D->GetNbinsX();
    int nBinsYJ1 = sigdR2jFexLeadingTopoByEnergy2D->GetNbinsY();

    double integralBelowCutJ1 = sigdR2jFexLeadingTopoByEnergy2D->Integral(1, nBinsXJ1, 1, yBinCutJ1 - 1);
    double totalIntegralJ1 = sigdR2jFexLeadingTopoByEnergy2D->Integral();

    double fractionBelowCutJ1 = integralBelowCutJ1 / totalIntegralJ1;
    char* labelJ1 = Form("%.3f", fractionBelowCutJ1);
    std::cout << "sig jfex leading fraction below deltaR^2: " << fractionBelowCutJ1 << "\n";
    mySmallText(0.6, 0.6, 0, labelJ1);


    gStyle->SetPalette(1);
    sigdR2jFexLeadingTopoByEnergy2D->Draw("COLZ");  
    gPad->Update();
    TPaletteAxis *paletteJ1 = (TPaletteAxis*)sigdR2jFexLeadingTopoByEnergy2D->GetListOfFunctions()->FindObject("palette");
    //palette1->SetLabelSize(0.15);
    sigdR2jFexLeadingTopoByEnergy2D->GetZaxis()->SetLabelSize(0.03);

    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "sigdR2jFexLeadingTopoByEnergy2D.pdf");


    gStyle->SetPalette(1);
    sigdR2jFexLeadingTopoByEnergy2D0to50GeV->Scale(1.0 / sigdR2jFexLeadingTopoByEnergy2D0to50GeV->Integral());
    sigdR2jFexLeadingTopoByEnergy2D0to50GeV->Draw("COLZ");  
    gPad->Update();
    sigdR2jFexLeadingTopoByEnergy2D0to50GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "sigdR2jFexLeadingTopoByEnergy2D0to50GeV.pdf");


    gStyle->SetPalette(1);
    backdR2jFexLeadingTopoByEnergy2D0to50GeV->Scale(1.0 / backdR2jFexLeadingTopoByEnergy2D0to50GeV->Integral());
    backdR2jFexLeadingTopoByEnergy2D0to50GeV->Draw("COLZ");  
    gPad->Update();
    backdR2jFexLeadingTopoByEnergy2D0to50GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "backdR2jFexLeadingTopoByEnergy2D0to50GeV.pdf");

    gStyle->SetPalette(1);
    sigdR2jFexLeadingTopoByEnergy2D50to550GeV->Scale(1.0 / sigdR2jFexLeadingTopoByEnergy2D50to550GeV->Integral());
    sigdR2jFexLeadingTopoByEnergy2D50to550GeV->Draw("COLZ");  
    gPad->Update();
    sigdR2jFexLeadingTopoByEnergy2D50to550GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "sigdR2jFexLeadingTopoByEnergy2D50to550GeV.pdf");

    gStyle->SetPalette(1);
    backdR2jFexLeadingTopoByEnergy2D50to550GeV->Scale(1.0 / backdR2jFexLeadingTopoByEnergy2D50to550GeV->Integral());
    backdR2jFexLeadingTopoByEnergy2D50to550GeV->Draw("COLZ");  
    gPad->Update();
    backdR2jFexLeadingTopoByEnergy2D50to550GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "backdR2jFexLeadingTopoByEnergy2D50to550GeV.pdf");

    gStyle->SetPalette(1);
    sigdR2jFexSubLeadingTopoByEnergy2D0to50GeV->Scale(1.0 / sigdR2jFexSubLeadingTopoByEnergy2D0to50GeV->Integral());
    sigdR2jFexSubLeadingTopoByEnergy2D0to50GeV->Draw("COLZ");  
    gPad->Update();
    sigdR2jFexSubLeadingTopoByEnergy2D0to50GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "sigdR2jFexSubLeadingTopoByEnergy2D0to50GeV.pdf");


    gStyle->SetPalette(1);
    backdR2jFexSubLeadingTopoByEnergy2D0to50GeV->Scale(1.0 / backdR2jFexSubLeadingTopoByEnergy2D0to50GeV->Integral());
    backdR2jFexSubLeadingTopoByEnergy2D0to50GeV->Draw("COLZ");  
    gPad->Update();
    backdR2jFexSubLeadingTopoByEnergy2D0to50GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "backdR2jFexSubLeadingTopoByEnergy2D0to50GeV.pdf");

    gStyle->SetPalette(1);
    sigdR2jFexSubLeadingTopoByEnergy2D50to550GeV->Scale(1.0 / sigdR2jFexSubLeadingTopoByEnergy2D50to550GeV->Integral());
    sigdR2jFexSubLeadingTopoByEnergy2D50to550GeV->Draw("COLZ");  
    gPad->Update();
    sigdR2jFexSubLeadingTopoByEnergy2D50to550GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "sigdR2jFexSubLeadingTopoByEnergy2D50to550GeV.pdf");

    gStyle->SetPalette(1);
    backdR2jFexSubLeadingTopoByEnergy2D50to550GeV->Scale(1.0 / backdR2jFexSubLeadingTopoByEnergy2D50to550GeV->Integral());
    backdR2jFexSubLeadingTopoByEnergy2D50to550GeV->Draw("COLZ");  
    gPad->Update();
    backdR2jFexSubLeadingTopoByEnergy2D50to550GeV->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "backdR2jFexSubLeadingTopoByEnergy2D50to550GeV.pdf");

    gStyle->SetPalette(1);
    backdR2jFexLeadingTopoByEnergy2D->Scale(1.0 / backdR2jFexLeadingTopoByEnergy2D->Integral());
    // Get number of bins in Y and the bin corresponding to y = 1.0
    int yBinCutJ2 = backdR2jFexLeadingTopoByEnergy2D->GetYaxis()->FindBin(1.0);

    // Integrate over all X bins and Y bins from 1 up to (but not including) yBinCutJ
    int nBinsXJ2 = backdR2jFexLeadingTopoByEnergy2D->GetNbinsX();
    int nBinsYJ2 = backdR2jFexLeadingTopoByEnergy2D->GetNbinsY();

    double integralBelowCutJ2 = backdR2jFexLeadingTopoByEnergy2D->Integral(1, nBinsXJ2, 1, yBinCutJ2 - 1);
    double totalIntegralJ2 = backdR2jFexLeadingTopoByEnergy2D->Integral();

    double fractionBelowCutJ2 = integralBelowCutJ2 / totalIntegralJ2;
    std::cout << "back jfex leading fraction below deltaR^2: " << fractionBelowCutJ2 << "\n";
    char* labelJ2 = Form("%.3f", fractionBelowCutJ2);
    mySmallText(0.6, 0.6, 0, labelJ2);

    backdR2jFexLeadingTopoByEnergy2D->Draw("COLZ");  
    gPad->Update();
    TPaletteAxis *paletteJ2 = (TPaletteAxis*)backdR2jFexLeadingTopoByEnergy2D->GetListOfFunctions()->FindObject("palette");
    //palette2->SetLabelSize(0.15);
    backdR2jFexLeadingTopoByEnergy2D->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "backdR2jFexLeadingTopoByEnergy2D.pdf");

    gStyle->SetPalette(1);
    sigdR2jFexSubLeadingTopoByEnergy2D->Scale(1.0 / sigdR2jFexSubLeadingTopoByEnergy2D->Integral());
    // Get number of bins in Y and the bin corresponding to y = 1.0
    int yBinCutJ3 = sigdR2jFexSubLeadingTopoByEnergy2D->GetYaxis()->FindBin(1.0);

    // Integrate over all X bins and Y bins from 1 up to (but not including) yBinCutJ
    int nBinsXJ3 = sigdR2jFexSubLeadingTopoByEnergy2D->GetNbinsX();
    int nBinsYJ3 = sigdR2jFexSubLeadingTopoByEnergy2D->GetNbinsY();

    double integralBelowCutJ3 = sigdR2jFexSubLeadingTopoByEnergy2D->Integral(1, nBinsXJ3, 1, yBinCutJ3 - 1);
    double totalIntegralJ3 = sigdR2jFexSubLeadingTopoByEnergy2D->Integral();

    double fractionBelowCutJ3 = integralBelowCutJ3 / totalIntegralJ3;
    char* labelJ3 = Form("%.3f", fractionBelowCutJ3);
    std::cout << "sig jfex subleading fraction below deltaR^2: " << fractionBelowCutJ3 << "\n";
    mySmallText(0.6, 0.6, 0, labelJ3);
    sigdR2jFexSubLeadingTopoByEnergy2D->Draw("COLZ");  
    gPad->Update();
    TPaletteAxis *paletteJ3 = (TPaletteAxis*)sigdR2jFexSubLeadingTopoByEnergy2D->GetListOfFunctions()->FindObject("palette");
    //palette3->SetLabelSize(0.15);
    sigdR2jFexSubLeadingTopoByEnergy2D->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "sigdR2jFexSubLeadingTopoByEnergy2D.pdf");

    backdR2jFexSubLeadingTopoByEnergy2D->Scale(1.0 / backdR2jFexSubLeadingTopoByEnergy2D->Integral());
    // Get number of bins in Y and the bin corresponding to y = 1.0
    int yBinCutJ4 = backdR2jFexSubLeadingTopoByEnergy2D->GetYaxis()->FindBin(1.0);

    // Integrate over all X bins and Y bins from 1 up to (but not including) yBinCutJ
    int nBinsXJ4 = backdR2jFexSubLeadingTopoByEnergy2D->GetNbinsX();
    int nBinsYJ4 = backdR2jFexSubLeadingTopoByEnergy2D->GetNbinsY();

    double integralBelowCutJ4 = backdR2jFexSubLeadingTopoByEnergy2D->Integral(1, nBinsXJ4, 1, yBinCutJ4 - 1);
    double totalIntegralJ4 = backdR2jFexSubLeadingTopoByEnergy2D->Integral();

    double fractionBelowCutJ4 = integralBelowCutJ4 / totalIntegralJ4;
    char* labelJ4 = Form("%.3f", fractionBelowCutJ4);
    std::cout << "back jfex subleading fraction below deltaR^2: " << fractionBelowCutJ4 << "\n";
    mySmallText(0.6, 0.6, 0, labelJ4);
    gStyle->SetPalette(1);
    backdR2jFexSubLeadingTopoByEnergy2D->Draw("COLZ");  
    gPad->Update();
    TPaletteAxis *paletteJ4 = (TPaletteAxis*)backdR2jFexSubLeadingTopoByEnergy2D->GetListOfFunctions()->FindObject("palette");
    //palette4->SetLabelSize(0.15);
    backdR2jFexSubLeadingTopoByEnergy2D->GetZaxis()->SetLabelSize(0.03);
    line->Draw("SAME");
    c2D.SaveAs(outputFileDir + "backdR2jFexSubLeadingTopoByEnergy2D.pdf");

    

    TCanvas c;
    c.cd();
    sigdR2gFexLeadingTopoByEnergy->Draw();
    c.SaveAs(outputFileDir + "sigdR2gFexLeadingTopoByEnergy.pdf");

    
    backdR2gFexLeadingTopoByEnergy->Draw();
    c.SaveAs(outputFileDir + "backdR2gFexLeadingTopoByEnergy.pdf");


    sigdR2gFexSubLeadingTopoByEnergy->Draw();
    c.SaveAs(outputFileDir + "sigdR2gFexSubLeadingTopoByEnergy.pdf");


    backdR2gFexSubLeadingTopoByEnergy->Draw();
    c.SaveAs(outputFileDir + "backdR2gFexSubLeadingTopoByEnergy.pdf");


    sigdR2gFexLeadingTopoByEnergy->SetLineColor(kRed);
    backdR2gFexLeadingTopoByEnergy->SetLineColor(kBlue);
    sigdR2gFexLeadingTopoByEnergy->SetMarkerSize(0.5);  // smaller marker
    backdR2gFexLeadingTopoByEnergy->SetMarkerSize(0.5);  // smaller marker
    sigdR2gFexLeadingTopoByEnergy->SetYTitle("#Delta R^{2} (lead gFex SRJ, 128 highest Et topo422)");   // Y-axis title
    sigdR2gFexLeadingTopoByEnergy->SetXTitle("Topo422 Energy [GeV]");  // X-axis title
    sigdR2gFexLeadingTopoByEnergy->Draw();
    backdR2gFexLeadingTopoByEnergy->Draw("SAME");
    TLegend *leg = new TLegend(0.75,0.75,0.9,0.95);
    leg->SetTextSize(0.03);
    leg->AddEntry(sigdR2gFexLeadingTopoByEnergy, "Signal", "l");
    leg->AddEntry(backdR2gFexLeadingTopoByEnergy, "Background", "l");
    leg->Draw();
    c.SaveAs(outputFileDir + "OverlaydR2gFexLeadingTopoByEnergy.pdf");

    sigdR2gFexSubLeadingTopoByEnergy->SetLineColor(kRed);
    backdR2gFexSubLeadingTopoByEnergy->SetLineColor(kBlue);
    sigdR2gFexSubLeadingTopoByEnergy->SetMarkerSize(0.5);  // smaller marker
    backdR2gFexSubLeadingTopoByEnergy->SetMarkerSize(0.5);  // smaller marker
    sigdR2gFexSubLeadingTopoByEnergy->SetYTitle("#Delta R^{2} (sublead gFex SRJ, 128 highest Et topo)");   // Y-axis title
    sigdR2gFexSubLeadingTopoByEnergy->SetXTitle("Topo422 Energy [GeV]");  // X-axis title
    sigdR2gFexSubLeadingTopoByEnergy->Draw("");
    backdR2gFexSubLeadingTopoByEnergy->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "OverlaydR2gFexSubLeadingTopoByEnergy.pdf");

    TGraph *g1 = new TGraph(sigTopo128EnergyValues1.size(), &sigTopo128EnergyValues1[0], &sigdR2LeadinggFexSRJTopo128[0]);
    g1->SetMarkerStyle(20);
    g1->SetMarkerColor(kRed);
    g1->SetMarkerSize(0.5);
    g1->SetTitle("Unbinned Scatter Plot;Topo422 Energy [GeV];#Delta R^{2} (lead gFex SRJ, topo)");
    g1->Draw("AP");
    c.SaveAs(outputFileDir + "scatter_sigdR2gFexLeadingTopo422ByEnergy.pdf");

    TGraph *g2 = new TGraph(backTopo128EnergyValues1.size(), &backTopo128EnergyValues1[0], &backdR2LeadinggFexSRJTopo128[0]);
    g2->SetMarkerStyle(20);
    g2->SetMarkerColor(kBlue);
    g2->SetMarkerSize(0.5);
    g2->SetTitle("Unbinned Scatter Plot;Topo422 Energy [GeV];#Delta R^{2} (lead gFex SRJ, topo)");
    g2->Draw("AP");
    c.SaveAs(outputFileDir + "scatter_backdR2gFexLeadingTopo422ByEnergy.pdf");

    TGraph *g3 = new TGraph(sigTopo128EnergyValues2.size(), &sigTopo128EnergyValues2[0], &sigdR2SubLeadinggFexSRJTopo128[0]);
    g3->SetMarkerStyle(20);
    g3->SetMarkerColor(kRed);
    g3->SetMarkerSize(0.5);
    g3->SetTitle("Unbinned Scatter Plot;Topo422 Energy [GeV];#Delta R^{2} (sublead gFex SRJ, topo)");
    g3->Draw("AP");
    c.SaveAs(outputFileDir + "scatter_sigdR2gFexSubLeadingTopo422ByEnergy.pdf");

    TGraph *g4 = new TGraph(backTopo128EnergyValues2.size(), &backTopo128EnergyValues2[0], &backdR2SubLeadinggFexSRJTopo128[0]);
    g4->SetMarkerStyle(20);
    g4->SetMarkerColor(kBlue);
    g4->SetMarkerSize(0.5);
    g4->SetTitle("Unbinned Scatter Plot;Topo422 Energy [GeV];#Delta R^{2} (sublead gFex SRJ, topo)");
    g4->Draw("AP");
    c.SaveAs(outputFileDir + "scatter_backdR2gFexSubLeadingTopo422ByEnergy.pdf");

    sig_h_topo_Et->SetLineColor(kRed);
    back_h_topo_Et->SetLineColor(kBlue);
    back_h_topo_Et->Draw();
    sig_h_topo_Et->Draw("SAME");
    
    leg->Draw();
    c.SaveAs(outputFileDir + "topo_Et.pdf");

    sig_h_topo_Et_top128->SetLineColor(kRed);
    back_h_topo_Et_top128->SetLineColor(kBlue);
    sig_h_topo_Et_top128->Scale(1.0 / sig_h_topo_Et_top128->Integral());
    back_h_topo_Et_top128->Scale(1.0 / back_h_topo_Et_top128->Integral());
    back_h_topo_Et_top128->Draw("HIST");
    sig_h_topo_Et_top128->Draw("HIST SAME");
    c.SaveAs(outputFileDir + "topo_Et_top128.pdf");

    sig_h_gFex_leading_Et->SetLineColor(kRed);
    back_h_gFex_leading_Et->SetLineColor(kBlue);
    sig_h_gFex_leading_Et->Scale(1.0 / sig_h_gFex_leading_Et->Integral());
    back_h_gFex_leading_Et->Scale(1.0 / back_h_gFex_leading_Et->Integral());
    sig_h_gFex_leading_Et->Draw("HIST");
    back_h_gFex_leading_Et->Draw("HIST SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "gFex_leading_Et.pdf");

    sig_h_gFex_subleading_Et->SetLineColor(kRed);
    back_h_gFex_subleading_Et->SetLineColor(kBlue);
    sig_h_gFex_subleading_Et->Scale(1.0 / sig_h_gFex_subleading_Et->Integral());
    back_h_gFex_subleading_Et->Scale(1.0 / back_h_gFex_subleading_Et->Integral());
    sig_h_gFex_subleading_Et->Draw("HIST");
    back_h_gFex_subleading_Et->Draw("HIST SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "gFex_subleading_Et.pdf");

    sig_h_jFex_leading_Et->SetLineColor(kRed);
    back_h_jFex_leading_Et->SetLineColor(kBlue);
    sig_h_jFex_leading_Et->Scale(1.0 / sig_h_jFex_leading_Et->Integral());
    back_h_jFex_leading_Et->Scale(1.0 / back_h_jFex_leading_Et->Integral());
    back_h_jFex_leading_Et->Draw("HIST");
    sig_h_jFex_leading_Et->Draw("HIST SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "jFex_leading_Et.pdf");

    sig_h_jFex_subleading_Et->SetLineColor(kRed);
    back_h_jFex_subleading_Et->SetLineColor(kBlue);
    sig_h_jFex_subleading_Et->Scale(1.0 / sig_h_jFex_subleading_Et->Integral());
    back_h_jFex_subleading_Et->Scale(1.0 / back_h_jFex_subleading_Et->Integral());
    sig_h_jFex_subleading_Et->Draw("HIST");
    back_h_jFex_subleading_Et->Draw("HIST SAME");
    
    leg->Draw();
    c.SaveAs(outputFileDir + "jFex_subleading_Et.pdf");

    sig_h_gFex_leading_subleading_deltaR->SetLineColor(kRed);
    back_h_gFex_leading_subleading_deltaR->SetLineColor(kBlue);
    sig_h_gFex_leading_subleading_deltaR->Scale(1.0 / sig_h_gFex_leading_subleading_deltaR->Integral());
    back_h_gFex_leading_subleading_deltaR->Scale(1.0 / back_h_gFex_leading_subleading_deltaR->Integral());
    sig_h_gFex_leading_subleading_deltaR->Draw("HIST");
    back_h_gFex_leading_subleading_deltaR->Draw("HIST SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR_leading_subleading_gFexSRJ.pdf");

    sig_h_jFex_leading_subleading_deltaR->SetLineColor(kRed);
    back_h_jFex_leading_subleading_deltaR->SetLineColor(kBlue);
    sig_h_jFex_leading_subleading_deltaR->Scale(1.0 / sig_h_jFex_leading_subleading_deltaR->Integral());
    back_h_jFex_leading_subleading_deltaR->Scale(1.0 / back_h_jFex_leading_subleading_deltaR->Integral());
    
    back_h_jFex_leading_subleading_deltaR->Draw("HIST");
    sig_h_jFex_leading_subleading_deltaR->Draw("HIST SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR_leading_subleading_jFexSRJ.pdf");

    sig_h_gFex_Et->SetLineColor(kRed);
    back_h_gFex_Et->SetLineColor(kBlue);
    sig_h_gFex_Et->Scale(1.0 / sig_h_gFex_Et->Integral());
    back_h_gFex_Et->Scale(1.0 / back_h_gFex_Et->Integral());
    back_h_gFex_Et->Draw("HIST");
    sig_h_gFex_Et->Draw("HIST SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "gFex_Et.pdf");

    sig_h_jFex_Et->SetLineColor(kRed);
    back_h_jFex_Et->SetLineColor(kBlue);
    back_h_jFex_Et->Draw();
    sig_h_jFex_Et->Draw("SAME");
    
    leg->Draw();
    c.SaveAs(outputFileDir + "jFex_Et.pdf");

    sig_h_topo_eta->SetLineColor(kRed);
    back_h_topo_eta->SetLineColor(kBlue);
    back_h_topo_eta->Draw();
    sig_h_topo_eta->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "topo_eta.pdf");

    sig_h_topo_eta_top128->SetLineColor(kRed);
    back_h_topo_eta_top128->SetLineColor(kBlue);
    back_h_topo_eta_top128->Draw();
    sig_h_topo_eta_top128->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "topo_eta_top128.pdf");

    sig_h_gFex_eta->SetLineColor(kRed);
    back_h_gFex_eta->SetLineColor(kBlue);
    back_h_gFex_eta->Draw();
    sig_h_gFex_eta->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "gFex_eta.pdf");

    sig_h_gFex_leading_eta->SetLineColor(kRed);
    back_h_gFex_leading_eta->SetLineColor(kBlue);
    back_h_gFex_leading_eta->Draw();
    sig_h_gFex_leading_eta->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "gFex_leading_eta.pdf");

    sig_h_gFex_subleading_eta->SetLineColor(kRed);
    back_h_gFex_subleading_eta->SetLineColor(kBlue);
    back_h_gFex_subleading_eta->Draw();
    sig_h_gFex_subleading_eta->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "gFex_subleading_eta.pdf");

    sig_h_jFex_leading_eta->SetLineColor(kRed);
    back_h_jFex_leading_eta->SetLineColor(kBlue);
    back_h_jFex_leading_eta->Draw();
    sig_h_jFex_leading_eta->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "jFex_leading_eta.pdf");

    sig_h_jFex_subleading_eta->SetLineColor(kRed);
    back_h_jFex_subleading_eta->SetLineColor(kBlue);
    back_h_jFex_subleading_eta->Draw();
    sig_h_jFex_subleading_eta->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "jFex_subleading_eta.pdf");

    sig_h_jFex_eta->SetLineColor(kRed);
    back_h_jFex_eta->SetLineColor(kBlue);
    back_h_jFex_eta->Draw();
    sig_h_jFex_eta->Draw("SAME");
    
    leg->Draw();
    c.SaveAs(outputFileDir + "jFex_eta.pdf");

    sig_h_topo_phi->SetLineColor(kRed);
    back_h_topo_phi->SetLineColor(kBlue);
    back_h_topo_phi->Draw();
    sig_h_topo_phi->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "topo_phi.pdf");

    sig_h_topo_phi_top128->SetLineColor(kRed);
    back_h_topo_phi_top128->SetLineColor(kBlue);
    back_h_topo_phi_top128->Draw();
    sig_h_topo_phi_top128->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "topo_phi_top128.pdf");

    sig_h_gFex_phi->SetLineColor(kRed);
    back_h_gFex_phi->SetLineColor(kBlue);
    back_h_gFex_phi->Draw();
    sig_h_gFex_phi->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "gFex_phi.pdf");

    sig_h_gFex_leading_phi->SetLineColor(kRed);
    back_h_gFex_leading_phi->SetLineColor(kBlue);
    back_h_gFex_leading_phi->Draw();
    sig_h_gFex_leading_phi->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "gFex_leading_phi.pdf");

    sig_h_gFex_leading_phi->SetLineColor(kRed);
    back_h_gFex_leading_phi->SetLineColor(kBlue);
    back_h_gFex_leading_phi->Draw();
    sig_h_gFex_leading_phi->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "gFex_subleading_phi.pdf");

    sig_h_jFex_leading_phi->SetLineColor(kRed);
    back_h_jFex_leading_phi->SetLineColor(kBlue);
    back_h_jFex_leading_phi->Draw();
    sig_h_jFex_leading_phi->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "jFex_leading_phi.pdf");

    sig_h_jFex_leading_phi->SetLineColor(kRed);
    back_h_jFex_leading_phi->SetLineColor(kBlue);
    back_h_jFex_leading_phi->Draw();
    sig_h_jFex_leading_phi->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "jFex_subleading_phi.pdf");

    sig_h_jFex_phi->SetLineColor(kRed);
    back_h_jFex_phi->SetLineColor(kBlue);
    back_h_jFex_phi->Draw();
    sig_h_jFex_phi->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "jFex_phi.pdf");

    sig_h_gFex_multiplicity->SetLineColor(kRed);
    back_h_gFex_multiplicity->SetLineColor(kBlue);
    back_h_gFex_multiplicity->Draw();
    sig_h_gFex_multiplicity->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "gFex_multiplicity.pdf");

    sig_h_topo_multiplicity->SetLineColor(kRed);
    back_h_topo_multiplicity->SetLineColor(kBlue);
    sig_h_topo_multiplicity->Scale(1.0 / sig_h_topo_multiplicity->Integral());
    back_h_topo_multiplicity->Scale(1.0 / back_h_topo_multiplicity->Integral());
    sig_h_topo_multiplicity->Draw("HIST");
    back_h_topo_multiplicity->Draw("HIST SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "topo_multiplicity.pdf");

    sig_h_jFex_multiplicity->SetLineColor(kRed);
    back_h_jFex_multiplicity->SetLineColor(kBlue);
    back_h_jFex_multiplicity->Draw();
    sig_h_jFex_multiplicity->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "jFex_multiplicity.pdf");

    std::cout << "maximum jfex mult. signal: " << sig_h_jFex_multiplicity->GetMaximum() << "\n";
    std::cout << "maximum jfex mult. background: " << back_h_jFex_multiplicity->GetMaximum() << "\n";

    
    back_h_deltaR2_gFex_SRJ1->Scale(1.0 / back_h_deltaR2_gFex_SRJ1->Integral());
    back_h_deltaR2_gFex_SRJ1->SetMarkerSize(0.5);
    back_h_deltaR2_gFex_SRJ1->SetXTitle("#Delta R^{2}");   // X-axis title
    back_h_deltaR2_gFex_SRJ1->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    sig_h_deltaR2_gFex_SRJ1->SetLineColor(kRed);
    back_h_deltaR2_gFex_SRJ1->SetLineColor(kBlue);
    sig_h_deltaR2_gFex_SRJ1->SetMarkerSize(0.5);
    back_h_deltaR2_gFex_SRJ1->Draw();
    sig_h_deltaR2_gFex_SRJ1->Scale(1.0 / sig_h_deltaR2_gFex_SRJ1->Integral());
    sig_h_deltaR2_gFex_SRJ1->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR2_gFex_SRJ1.pdf");

    back_h_deltaR2_jFex_SRJ1->Scale(1.0 / back_h_deltaR2_jFex_SRJ1->Integral());
    back_h_deltaR2_jFex_SRJ1->SetMarkerSize(0.5);
    back_h_deltaR2_jFex_SRJ1->SetXTitle("#Delta R^{2}");   // X-axis title
    back_h_deltaR2_jFex_SRJ1->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    sig_h_deltaR2_jFex_SRJ1->SetLineColor(kRed);
    back_h_deltaR2_jFex_SRJ1->SetLineColor(kBlue);
    sig_h_deltaR2_jFex_SRJ1->SetMarkerSize(0.5);
    back_h_deltaR2_jFex_SRJ1->Draw();
    sig_h_deltaR2_jFex_SRJ1->Scale(1.0 / sig_h_deltaR2_jFex_SRJ1->Integral());
    sig_h_deltaR2_jFex_SRJ1->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR2_jFex_SRJ1.pdf");

    back_h_deltaR2_gFex_SRJ2->Scale(1.0 / back_h_deltaR2_gFex_SRJ2->Integral());
    back_h_deltaR2_gFex_SRJ2->SetXTitle("#Delta R^{2}");   // X-axis title
    back_h_deltaR2_gFex_SRJ2->SetMarkerSize(0.5);
    back_h_deltaR2_gFex_SRJ2->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    sig_h_deltaR2_gFex_SRJ2->SetLineColor(kRed);
    back_h_deltaR2_gFex_SRJ2->SetLineColor(kBlue);
    back_h_deltaR2_gFex_SRJ2->Draw();
    sig_h_deltaR2_gFex_SRJ2->SetMarkerSize(0.5);
    sig_h_deltaR2_gFex_SRJ2->Scale(1.0 / sig_h_deltaR2_gFex_SRJ2->Integral());
    sig_h_deltaR2_gFex_SRJ2->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR2_gFex_SRJ2.pdf");
    
    back_h_deltaR2_jFex_SRJ2->Scale(1.0 / back_h_deltaR2_jFex_SRJ2->Integral());
    back_h_deltaR2_jFex_SRJ2->SetXTitle("#Delta R^{2}");   // X-axis title
    back_h_deltaR2_jFex_SRJ2->SetMarkerSize(0.5);
    back_h_deltaR2_jFex_SRJ2->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    sig_h_deltaR2_jFex_SRJ2->SetLineColor(kRed);
    back_h_deltaR2_jFex_SRJ2->SetLineColor(kBlue);
    back_h_deltaR2_jFex_SRJ2->Draw();
    sig_h_deltaR2_jFex_SRJ2->SetMarkerSize(0.5);
    sig_h_deltaR2_jFex_SRJ2->Scale(1.0 / sig_h_deltaR2_jFex_SRJ2->Integral());
    sig_h_deltaR2_jFex_SRJ2->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR2_jFex_SRJ2.pdf");

    back_h_deltaR_gFex_SRJ1->Scale(1.0 / back_h_deltaR_gFex_SRJ1->Integral());
    back_h_deltaR_gFex_SRJ1->SetXTitle("#Delta R");   // X-axis title
    back_h_deltaR_gFex_SRJ1->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    back_h_deltaR_gFex_SRJ1->SetMarkerSize(0.5);
    sig_h_deltaR_gFex_SRJ1->SetLineColor(kRed);
    back_h_deltaR_gFex_SRJ1->SetLineColor(kBlue);
    back_h_deltaR_gFex_SRJ1->Draw();
    sig_h_deltaR_gFex_SRJ1->Scale(1.0 / sig_h_deltaR_gFex_SRJ1->Integral());
    sig_h_deltaR_gFex_SRJ1->SetMarkerSize(0.5);
    sig_h_deltaR_gFex_SRJ1->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR_gFex_SRJ1.pdf");

    back_h_deltaR_jFex_SRJ1->Scale(1.0 / back_h_deltaR_jFex_SRJ1->Integral());
    back_h_deltaR_jFex_SRJ1->SetXTitle("#Delta R");   // X-axis title
    back_h_deltaR_jFex_SRJ1->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    back_h_deltaR_jFex_SRJ1->SetMarkerSize(0.5);
    sig_h_deltaR_jFex_SRJ1->SetLineColor(kRed);
    back_h_deltaR_jFex_SRJ1->SetLineColor(kBlue);
    back_h_deltaR_jFex_SRJ1->Draw();
    sig_h_deltaR_jFex_SRJ1->Scale(1.0 / sig_h_deltaR_jFex_SRJ1->Integral());
    sig_h_deltaR_jFex_SRJ1->SetMarkerSize(0.5);
    sig_h_deltaR_jFex_SRJ1->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR_jFex_SRJ1.pdf");

    back_h_deltaR_gFex_SRJ2->Scale(1.0 / back_h_deltaR_gFex_SRJ2->Integral());
    back_h_deltaR_gFex_SRJ2->SetXTitle("#Delta R");   // X-axis title
    back_h_deltaR_gFex_SRJ2->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    back_h_deltaR_gFex_SRJ2->SetMarkerSize(0.5);
    sig_h_deltaR_gFex_SRJ2->SetLineColor(kRed);
    back_h_deltaR_gFex_SRJ2->SetLineColor(kBlue);
    back_h_deltaR_gFex_SRJ2->Draw();
    sig_h_deltaR_gFex_SRJ2->Scale(1.0 / sig_h_deltaR_gFex_SRJ2->Integral());
    sig_h_deltaR_gFex_SRJ2->SetMarkerSize(0.5);
    sig_h_deltaR_gFex_SRJ2->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR_gFex_SRJ2.pdf");

    back_h_deltaR_jFex_SRJ2->Scale(1.0 / back_h_deltaR_jFex_SRJ2->Integral());
    back_h_deltaR_jFex_SRJ2->SetXTitle("#Delta R");   // X-axis title
    back_h_deltaR_jFex_SRJ2->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    back_h_deltaR_jFex_SRJ2->SetMarkerSize(0.5);
    sig_h_deltaR_jFex_SRJ2->SetLineColor(kRed);
    back_h_deltaR_jFex_SRJ2->SetLineColor(kBlue);
    back_h_deltaR_jFex_SRJ2->Draw();
    sig_h_deltaR_jFex_SRJ2->Scale(1.0 / sig_h_deltaR_jFex_SRJ2->Integral());
    sig_h_deltaR_jFex_SRJ2->SetMarkerSize(0.5);
    sig_h_deltaR_jFex_SRJ2->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR_jFex_SRJ2.pdf");

    back_h_deltaR2_gFex_SRJ1_topo128->Scale(1.0 / back_h_deltaR2_gFex_SRJ1_topo128->Integral());
    back_h_deltaR2_gFex_SRJ1_topo128->SetMarkerSize(0.5);
    back_h_deltaR2_gFex_SRJ1_topo128->SetXTitle("#Delta R^{2}");   // X-axis title
    back_h_deltaR2_gFex_SRJ1_topo128->SetYTitle("Normalized # of Topo422 Clusters (per 0.2)");  // Y-axis title
    sig_h_deltaR2_gFex_SRJ1_topo128->SetLineColor(kRed);
    back_h_deltaR2_gFex_SRJ1_topo128->SetLineColor(kBlue);
    sig_h_deltaR2_gFex_SRJ1_topo128->SetMarkerSize(0.5);
    back_h_deltaR2_gFex_SRJ1_topo128->Draw();
    sig_h_deltaR2_gFex_SRJ1_topo128->Scale(1.0 / sig_h_deltaR2_gFex_SRJ1_topo128->Integral());
    sig_h_deltaR2_gFex_SRJ1_topo128->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR2_gFex_SRJ1_top128.pdf");

    back_h_deltaR2_jFex_SRJ1_topo128->Scale(1.0 / back_h_deltaR2_jFex_SRJ1_topo128->Integral());
    back_h_deltaR2_jFex_SRJ1_topo128->SetMarkerSize(0.5);
    back_h_deltaR2_jFex_SRJ1_topo128->SetXTitle("#Delta R^{2}");   // X-axis title
    back_h_deltaR2_jFex_SRJ1_topo128->SetYTitle("Normalized # of Topo422 Clusters (per 0.2)");  // Y-axis title
    sig_h_deltaR2_jFex_SRJ1_topo128->SetLineColor(kRed);
    back_h_deltaR2_jFex_SRJ1_topo128->SetLineColor(kBlue);
    sig_h_deltaR2_jFex_SRJ1_topo128->SetMarkerSize(0.5);
    back_h_deltaR2_jFex_SRJ1_topo128->Draw();
    sig_h_deltaR2_jFex_SRJ1_topo128->Scale(1.0 / sig_h_deltaR2_jFex_SRJ1_topo128->Integral());
    sig_h_deltaR2_jFex_SRJ1_topo128->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR2_jFex_SRJ1_top128.pdf");
    
    back_h_deltaR2_gFex_SRJ2_topo128->Scale(1.0 / back_h_deltaR2_gFex_SRJ2_topo128->Integral());
    back_h_deltaR2_gFex_SRJ2_topo128->SetXTitle("#Delta R^{2}");   // X-axis title
    back_h_deltaR2_gFex_SRJ2_topo128->SetMarkerSize(0.5);
    back_h_deltaR2_gFex_SRJ2_topo128->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    sig_h_deltaR2_gFex_SRJ2_topo128->SetLineColor(kRed);
    back_h_deltaR2_gFex_SRJ2_topo128->SetLineColor(kBlue);
    back_h_deltaR2_gFex_SRJ2_topo128->Draw();
    sig_h_deltaR2_gFex_SRJ2_topo128->SetMarkerSize(0.5);
    sig_h_deltaR2_gFex_SRJ2_topo128->Scale(1.0 / sig_h_deltaR2_gFex_SRJ2_topo128->Integral());
    sig_h_deltaR2_gFex_SRJ2_topo128->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR2_gFex_SRJ2_top128.pdf");

    back_h_deltaR2_jFex_SRJ2_topo128->Scale(1.0 / back_h_deltaR2_jFex_SRJ2_topo128->Integral());
    back_h_deltaR2_jFex_SRJ2_topo128->SetXTitle("#Delta R^{2}");   // X-axis title
    back_h_deltaR2_jFex_SRJ2_topo128->SetMarkerSize(0.5);
    back_h_deltaR2_jFex_SRJ2_topo128->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    sig_h_deltaR2_jFex_SRJ2_topo128->SetLineColor(kRed);
    back_h_deltaR2_jFex_SRJ2_topo128->SetLineColor(kBlue);
    back_h_deltaR2_jFex_SRJ2_topo128->Draw();
    sig_h_deltaR2_jFex_SRJ2_topo128->SetMarkerSize(0.5);
    sig_h_deltaR2_jFex_SRJ2_topo128->Scale(1.0 / sig_h_deltaR2_jFex_SRJ2_topo128->Integral());
    sig_h_deltaR2_jFex_SRJ2_topo128->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR2_jFex_SRJ2_top128.pdf");

    back_h_deltaR_gFex_SRJ1_topo128->Scale(1.0 / back_h_deltaR_gFex_SRJ1_topo128->Integral());
    back_h_deltaR_gFex_SRJ1_topo128->SetXTitle("#Delta R");   // X-axis title
    back_h_deltaR_gFex_SRJ1_topo128->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    back_h_deltaR_gFex_SRJ1_topo128->SetMarkerSize(0.5);
    sig_h_deltaR_gFex_SRJ1_topo128->SetLineColor(kRed);
    back_h_deltaR_gFex_SRJ1_topo128->SetLineColor(kBlue);
    back_h_deltaR_gFex_SRJ1_topo128->Draw();
    sig_h_deltaR_gFex_SRJ1_topo128->Scale(1.0 / sig_h_deltaR_gFex_SRJ1_topo128->Integral());
    sig_h_deltaR_gFex_SRJ1_topo128->SetMarkerSize(0.5);
    sig_h_deltaR_gFex_SRJ1_topo128->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR_gFex_SRJ1_top128.pdf");

    back_h_deltaR_jFex_SRJ1_topo128->Scale(1.0 / back_h_deltaR_jFex_SRJ1_topo128->Integral());
    back_h_deltaR_jFex_SRJ1_topo128->SetXTitle("#Delta R");   // X-axis title
    back_h_deltaR_jFex_SRJ1_topo128->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    back_h_deltaR_jFex_SRJ1_topo128->SetMarkerSize(0.5);
    sig_h_deltaR_jFex_SRJ1_topo128->SetLineColor(kRed);
    back_h_deltaR_jFex_SRJ1_topo128->SetLineColor(kBlue);
    back_h_deltaR_jFex_SRJ1_topo128->Draw();
    sig_h_deltaR_jFex_SRJ1_topo128->Scale(1.0 / sig_h_deltaR_jFex_SRJ1_topo128->Integral());
    sig_h_deltaR_jFex_SRJ1_topo128->SetMarkerSize(0.5);
    sig_h_deltaR_jFex_SRJ1_topo128->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR_jFex_SRJ1_top128.pdf");

    back_h_deltaR_gFex_SRJ2_topo128->Scale(1.0 / back_h_deltaR_gFex_SRJ2_topo128->Integral());
    back_h_deltaR_gFex_SRJ2_topo128->SetXTitle("#Delta R");   // X-axis title
    back_h_deltaR_gFex_SRJ2_topo128->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    back_h_deltaR_gFex_SRJ2_topo128->SetMarkerSize(0.5);
    sig_h_deltaR_gFex_SRJ2_topo128->SetLineColor(kRed);
    back_h_deltaR_gFex_SRJ2_topo128->SetLineColor(kBlue);
    back_h_deltaR_gFex_SRJ2_topo128->Draw();
    sig_h_deltaR_gFex_SRJ2_topo128->Scale(1.0 / sig_h_deltaR_gFex_SRJ2_topo128->Integral());
    sig_h_deltaR_gFex_SRJ2_topo128->SetMarkerSize(0.5);
    sig_h_deltaR_gFex_SRJ2_topo128->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR_gFex_SRJ2_top128.pdf");

    back_h_deltaR_jFex_SRJ2_topo128->Scale(1.0 / back_h_deltaR_jFex_SRJ2_topo128->Integral());
    back_h_deltaR_jFex_SRJ2_topo128->SetXTitle("#Delta R");   // X-axis title
    back_h_deltaR_jFex_SRJ2_topo128->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    back_h_deltaR_jFex_SRJ2_topo128->SetMarkerSize(0.5);
    sig_h_deltaR_jFex_SRJ2_topo128->SetLineColor(kRed);
    back_h_deltaR_jFex_SRJ2_topo128->SetLineColor(kBlue);
    back_h_deltaR_jFex_SRJ2_topo128->Draw();
    sig_h_deltaR_jFex_SRJ2_topo128->Scale(1.0 / sig_h_deltaR_jFex_SRJ2_topo128->Integral());
    sig_h_deltaR_jFex_SRJ2_topo128->SetMarkerSize(0.5);
    sig_h_deltaR_jFex_SRJ2_topo128->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR_jFex_SRJ2_top128.pdf");

    TCanvas cLog;
    //cLog.SetLogx();
    cLog.SetLogy();

    sig_h_topo_Et->SetLineColor(kRed);
    back_h_topo_Et->SetLineColor(kBlue);
    sig_h_topo_Et->Scale(1.0 / sig_h_topo_Et->Integral());
    back_h_topo_Et->Scale(1.0 / back_h_topo_Et->Integral());
    back_h_topo_Et->Draw("HIST");
    sig_h_topo_Et->Draw("HIST SAME");
    leg->Draw();
    cLog.SaveAs(outputFileDir + "log_topo_Et.pdf");

    sig_h_gFex_Et->SetLineColor(kRed);
    back_h_gFex_Et->SetLineColor(kBlue);
    sig_h_gFex_Et->Scale(1.0 / sig_h_gFex_Et->Integral());
    back_h_gFex_Et->Scale(1.0 / back_h_gFex_Et->Integral());
    back_h_gFex_Et->Draw("HIST");
    sig_h_gFex_Et->Draw("HIST SAME");
    
    leg->Draw();
    cLog.SaveAs(outputFileDir + "log_gFex_Et.pdf");

    sig_h_jFex_Et->SetLineColor(kRed);
    back_h_jFex_Et->SetLineColor(kBlue);
    back_h_jFex_Et->Draw();
    sig_h_jFex_Et->Draw("SAME");
    leg->Draw();
    cLog.SaveAs(outputFileDir + "log_jFex_Et.pdf");

    sig_h_topo_Et_top128->SetLineColor(kRed);
    back_h_topo_Et_top128->SetLineColor(kBlue);
    sig_h_topo_Et_top128->Scale(1.0 / sig_h_topo_Et_top128->Integral());
    back_h_topo_Et_top128->Scale(1.0 / back_h_topo_Et_top128->Integral());
    back_h_topo_Et_top128->Draw("HIST");
    sig_h_topo_Et_top128->Draw("HIST SAME");
    cLog.SaveAs(outputFileDir + "log_topo_Et_top128.pdf");

    std::cout << "nTopo: " << nTopo << " and nTopoGreater1GeV: " << nTopoGreater1GeV << "\n";
    std::cout << "totalTopoEnergy: "  << totalTopoEnergy << " and topoEnergyClustersGreater1GeV: " << topoEnergyClustersGreater1GeV << "\n";

    signalInputFile->Close();
    backgroundInputFile->Close();
    
}

template<bool overlayBool>
void callPlot(const bool signalBool) {
    // Usage: callPlot<true>(true)
    const std::string signalInputFile = "outputRootFiles/mc21_14TeV_hh_bbbb_vbf_novhh.root";
    const std::string backgroundInputFile = "outputRootFiles/mc21_14TeV_jj_JZ3.root";
    std::string outputPrefix;
    if (signalBool) outputPrefix = "signalHistograms/";
    else outputPrefix = "backgroundHistograms/"; 

    analyze_existing_histograms<overlayBool>(signalInputFile, backgroundInputFile, outputPrefix, signalBool);
}
