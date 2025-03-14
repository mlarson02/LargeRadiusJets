#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "../algorithm/helperFunctions.h"

template <bool signalBackgroundOverlay> 
void analyze_existing_histograms(const std::string& signalInputFileName, const std::string& backgroundInputFileName, const std::string& outputPrefix, const bool signalBool);

double deltaPhi(double phi1, double phi2) {
    double dphi = phi1 - phi2;
    if (dphi > TMath::Pi()) dphi -= 2 * TMath::Pi();
    if (dphi <= -TMath::Pi()) dphi += 2 * TMath::Pi();
    return dphi;
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

        topoTree->GetEntry(i);
        std::cout << "topoEtValues->size(): " << topoEtValues->size() << "\n";
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
        std::cout << "jFexEtValues->size(): " << jFexEtValues->size() << "\n";
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
    h_deltaR_jet1->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    h_deltaR_jet1->Draw();
    c.SaveAs(outputFileDir + "deltaR_jet1.pdf");
    h_deltaR_jet2->Scale(1.0 / h_deltaR_jet2->Integral());
    h_deltaR_jet2->SetXTitle("#Delta R");   // X-axis title
    h_deltaR_jet2->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
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

    inputFile->Close();
}

template <>
void analyze_existing_histograms<true>(const std::string& signalInputFileName, const std::string& backgroundInputFileName, const std::string& outputPrefix, const bool signalBool) {
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

    std::vector<double>* sigGFexEtValues = nullptr;
    std::vector<double>* sigGFexEtaValues = nullptr;
    std::vector<double>* sigGFexPhiValues = nullptr;

    std::vector<double>* sigJFexEtValues = nullptr;
    std::vector<double>* sigJFexEtaValues = nullptr;
    std::vector<double>* sigJFexPhiValues = nullptr;

    std::vector<double>* backTopoEtValues = nullptr;
    std::vector<double>* backTopoEtaValues = nullptr;
    std::vector<double>* backTopoPhiValues = nullptr;

    std::vector<double>* backGFexEtValues = nullptr;
    std::vector<double>* backGFexEtaValues = nullptr;
    std::vector<double>* backGFexPhiValues = nullptr;

    std::vector<double>* backJFexEtValues = nullptr;
    std::vector<double>* backJFexEtaValues = nullptr;
    std::vector<double>* backJFexPhiValues = nullptr;

    // Histograms
    TH1F* sig_h_topo_Et = new TH1F("sig_h_topo_Et", "Topo Et Distribution;Et (GeV);Counts", 50, 0, 400);
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

    // gFex jet deltaR^2 histogram
    TH1F* sig_h_deltaR2_jet1 = new TH1F("sig_h_deltaR2_jet1", "#DeltaR^{2} (gFex Highest Et Jet vs. topo422)", 25, 0, 10);
    TH1F* sig_h_deltaR2_jet2 = new TH1F("sig_h_deltaR2_jet2", "#DeltaR^{2} (gFex 2nd Highest Et Jet vs. topo422)", 25, 0, 10);
    TH1F* sig_h_deltaR_jet1 = new TH1F("sig_h_deltaR_jet1", "#DeltaR (gFex Highest Et Jet vs. topo422)", 25, 0, 5);
    TH1F* sig_h_deltaR_jet2 = new TH1F("sig_h_deltaR_jet2", "#DeltaR (gFex 2nd Highest Et Jet vs. topo422)", 25, 0, 5);

    TH1F* back_h_topo_Et = new TH1F("h_topo_Et", "Topo Et Distribution;Et (GeV);Counts", 50, 0, 400);
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

    // gFex jet deltaR^2 histogram
    TH1F* back_h_deltaR2_jet1 = new TH1F("h_deltaR2_jet1", "#DeltaR^{2} (gFex Highest Et Jet vs. topo422)", 25, 0, 10);
    TH1F* back_h_deltaR2_jet2 = new TH1F("h_deltaR2_jet2", "#DeltaR^{2} (gFex 2nd Highest Et Jet vs. topo422)", 25, 0, 10);
    TH1F* back_h_deltaR_jet1 = new TH1F("h_deltaR_jet1", "#DeltaR (gFex Highest Et Jet vs. topo422)", 25, 0, 5);
    TH1F* back_h_deltaR_jet2 = new TH1F("h_deltaR_jet2", "#DeltaR (gFex 2nd Highest Et Jet vs. topo422)", 25, 0, 5);

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


    int sigNEntries = sigGFexTree->GetEntries();
    for (int i = 0; i < sigNEntries; i++) {
        sigGFexTree->GetEntry(i);
        sig_h_gFex_multiplicity->Fill(sigGFexEtValues->size());
        for (size_t j = 0; j < sigGFexEtValues->size(); j++) {
            sig_h_gFex_Et->Fill(sigGFexEtValues->at(j));
            sig_h_gFex_eta->Fill(sigGFexEtaValues->at(j));
            sig_h_gFex_phi->Fill(sigGFexPhiValues->at(j));
        }

        // Find two highest Et jets
        auto [idx1, idx2] = getTwoHighestIndices(*sigGFexEtValues);

        sigTopoTree->GetEntry(i);
        std::cout << "sigTopoEtValues->size(): " << sigTopoEtValues->size() << "\n";
        sig_h_topo_multiplicity->Fill(sigTopoEtValues->size());
        for (size_t k = 0; k < sigTopoEtValues->size(); k++) {
            sig_h_topo_Et->Fill(sigTopoEtValues->at(k));
            sig_h_topo_eta->Fill(sigTopoEtaValues->at(k)); // FIXME rename all GFex, Topo variables to something like input object, seed
            sig_h_topo_phi->Fill(sigTopoPhiValues->at(k));
            double dr2_1 = ((sigGFexEtaValues->at(idx1)) - sigTopoEtaValues->at(k))*((sigGFexEtaValues->at(idx1)) - sigTopoEtaValues->at(k)) + deltaPhi(sigGFexPhiValues->at(idx1), sigTopoPhiValues->at(k))*deltaPhi(sigGFexPhiValues->at(idx1), sigTopoPhiValues->at(k));
            sig_h_deltaR2_jet1->Fill(dr2_1);
            sig_h_deltaR_jet1->Fill(sqrt(dr2_1));
            double dr2_2 = ((sigGFexEtaValues->at(idx2)) - sigTopoEtaValues->at(k))*((sigGFexEtaValues->at(idx2)) - sigTopoEtaValues->at(k)) + deltaPhi(sigGFexPhiValues->at(idx2), sigTopoPhiValues->at(k))*deltaPhi(sigGFexPhiValues->at(idx2), sigTopoPhiValues->at(k));
            sig_h_deltaR2_jet2->Fill(dr2_2);
            sig_h_deltaR_jet2->Fill(sqrt(dr2_2));
        }
            
    }

    for (int i = 0; i < sigJFexTree->GetEntries(); i++) {
        sigJFexTree->GetEntry(i);
        std::cout << "sigJFexEtValues->size(): " << sigJFexEtValues->size() << "\n";
        sig_h_jFex_multiplicity->Fill(sigJFexEtValues->size());
        for (float et : *sigJFexEtValues) sig_h_jFex_Et->Fill(et);
        for (float eta : *sigJFexEtaValues) sig_h_jFex_eta->Fill(eta);
        for (float phi : *sigJFexPhiValues) sig_h_jFex_phi->Fill(phi);
    }


    int backNEntries = backGFexTree->GetEntries();
    for (int i = 0; i < backNEntries; i++) {
        backGFexTree->GetEntry(i);
        back_h_gFex_multiplicity->Fill(backGFexEtValues->size());
        for (size_t j = 0; j < backGFexEtValues->size(); j++) {
            back_h_gFex_Et->Fill(backGFexEtValues->at(j));
            back_h_gFex_eta->Fill(backGFexEtaValues->at(j));
            back_h_gFex_phi->Fill(backGFexPhiValues->at(j));
        }

        // Find two highest Et jets
        auto [idx1, idx2] = getTwoHighestIndices(*backGFexEtValues);

        backTopoTree->GetEntry(i);
        std::cout << "topoEtValues->size(): " << backTopoEtValues->size() << "\n";
        back_h_topo_multiplicity->Fill(backTopoEtValues->size());
        for (size_t k = 0; k < backTopoEtValues->size(); k++) {
            back_h_topo_Et->Fill(backTopoEtValues->at(k));
            back_h_topo_eta->Fill(backTopoEtaValues->at(k));
            back_h_topo_phi->Fill(backTopoPhiValues->at(k));
            double dr2_1 = ((backGFexEtaValues->at(idx1)) - backTopoEtaValues->at(k))*((backGFexEtaValues->at(idx1)) - backTopoEtaValues->at(k)) + deltaPhi(backGFexPhiValues->at(idx1), backTopoPhiValues->at(k))*deltaPhi(backGFexPhiValues->at(idx1), backTopoPhiValues->at(k));
            std::cout << "background dr2_1: " << dr2_1 << " and dr: " << sqrt(dr2_1) << "\n";
            back_h_deltaR2_jet1->Fill(dr2_1);
            back_h_deltaR_jet1->Fill(sqrt(dr2_1));
            double dr2_2 = ((backGFexEtaValues->at(idx2)) - backTopoEtaValues->at(k))*((backGFexEtaValues->at(idx2)) - backTopoEtaValues->at(k)) + deltaPhi(backGFexPhiValues->at(idx2), backTopoPhiValues->at(k))*deltaPhi(backGFexPhiValues->at(idx2), backTopoPhiValues->at(k));
            back_h_deltaR2_jet2->Fill(dr2_2);
            back_h_deltaR_jet2->Fill(sqrt(dr2_2));
        }
            
    }

    for (int i = 0; i < backJFexTree->GetEntries(); i++) {
        backJFexTree->GetEntry(i);
        std::cout << "backJFexEtValues->size(): " << backJFexEtValues->size() << "\n";
        back_h_jFex_multiplicity->Fill(backJFexEtValues->size());
        for (float et : *backJFexEtValues) back_h_jFex_Et->Fill(et);
        for (float eta : *backJFexEtaValues) back_h_jFex_eta->Fill(eta);
        for (float phi : *backJFexPhiValues) back_h_jFex_phi->Fill(phi);
    }

    // Save histograms
    TCanvas c;
    TString outputFileDir = "overlayHistograms/";

    sig_h_topo_Et->SetLineColor(kRed);
    back_h_topo_Et->SetLineColor(kBlue);
    back_h_topo_Et->Draw();
    sig_h_topo_Et->Draw("SAME");
    
    TLegend *leg = new TLegend(0.8,0.8,0.95,0.95);
    leg->SetTextSize(0.03);
    leg->AddEntry(sig_h_topo_Et, "Signal", "l");
    leg->AddEntry(back_h_topo_Et, "Background", "l");
    leg->Draw();
    c.SaveAs(outputFileDir + "topo_Et.pdf");

    sig_h_gFex_Et->SetLineColor(kRed);
    back_h_gFex_Et->SetLineColor(kBlue);
    back_h_gFex_Et->Draw();
    sig_h_gFex_Et->Draw("SAME");
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

    sig_h_gFex_eta->SetLineColor(kRed);
    back_h_gFex_eta->SetLineColor(kBlue);
    back_h_gFex_eta->Draw();
    sig_h_gFex_eta->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "gFex_eta.pdf");

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

    sig_h_gFex_phi->SetLineColor(kRed);
    back_h_gFex_phi->SetLineColor(kBlue);
    back_h_gFex_phi->Draw();
    sig_h_gFex_phi->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "gFex_phi.pdf");

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
    back_h_topo_multiplicity->Draw();
    sig_h_topo_multiplicity->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "topo_multiplicity.pdf");

    sig_h_jFex_multiplicity->SetLineColor(kRed);
    back_h_jFex_multiplicity->SetLineColor(kBlue);
    back_h_jFex_multiplicity->Draw();
    sig_h_jFex_multiplicity->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "jFex_multiplicity.pdf");

    
    back_h_deltaR2_jet1->Scale(1.0 / back_h_deltaR2_jet1->Integral());
    back_h_deltaR2_jet1->SetMarkerSize(0.5);
    back_h_deltaR2_jet1->SetXTitle("#Delta R^{2}");   // X-axis title
    back_h_deltaR2_jet1->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    sig_h_deltaR2_jet1->SetLineColor(kRed);
    back_h_deltaR2_jet1->SetLineColor(kBlue);
    sig_h_deltaR2_jet1->SetMarkerSize(0.5);
    back_h_deltaR2_jet1->Draw();
    sig_h_deltaR2_jet1->Scale(1.0 / sig_h_deltaR2_jet1->Integral());
    sig_h_deltaR2_jet1->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR2_jet1.pdf");

    back_h_deltaR2_jet2->Scale(1.0 / back_h_deltaR2_jet2->Integral());
    back_h_deltaR2_jet2->SetXTitle("#Delta R^{2}");   // X-axis title
    back_h_deltaR2_jet2->SetMarkerSize(0.5);
    back_h_deltaR2_jet2->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    sig_h_deltaR2_jet2->SetLineColor(kRed);
    back_h_deltaR2_jet2->SetLineColor(kBlue);
    back_h_deltaR2_jet2->Draw();
    sig_h_deltaR2_jet2->SetMarkerSize(0.5);
    sig_h_deltaR2_jet2->Scale(1.0 / sig_h_deltaR2_jet2->Integral());
    sig_h_deltaR2_jet2->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR2_jet2.pdf");

    back_h_deltaR_jet1->Scale(1.0 / back_h_deltaR_jet1->Integral());
    back_h_deltaR_jet1->SetXTitle("#Delta R");   // X-axis title
    back_h_deltaR_jet1->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    back_h_deltaR_jet1->SetMarkerSize(0.5);
    sig_h_deltaR_jet1->SetLineColor(kRed);
    back_h_deltaR_jet1->SetLineColor(kBlue);
    back_h_deltaR_jet1->Draw();
    sig_h_deltaR_jet1->Scale(1.0 / sig_h_deltaR_jet1->Integral());
    sig_h_deltaR_jet1->SetMarkerSize(0.5);
    sig_h_deltaR_jet1->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR_jet1.pdf");

    back_h_deltaR_jet2->Scale(1.0 / back_h_deltaR_jet2->Integral());
    back_h_deltaR_jet2->SetXTitle("#Delta R");   // X-axis title
    back_h_deltaR_jet2->SetYTitle("Normalized Num. of Topo422 Clusters (per 0.2)");  // Y-axis title
    back_h_deltaR_jet2->SetMarkerSize(0.5);
    sig_h_deltaR_jet2->SetLineColor(kRed);
    back_h_deltaR_jet2->SetLineColor(kBlue);
    back_h_deltaR_jet2->Draw();
    sig_h_deltaR_jet2->Scale(1.0 / sig_h_deltaR_jet2->Integral());
    sig_h_deltaR_jet2->SetMarkerSize(0.5);
    sig_h_deltaR_jet2->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "deltaR_jet2.pdf");

    TCanvas cLog;
    cLog.SetLogx();
    cLog.SetLogy();

    sig_h_topo_Et->SetLineColor(kRed);
    back_h_topo_Et->SetLineColor(kBlue);
    back_h_topo_Et->Draw();
    sig_h_topo_Et->Draw("SAME");
    leg->Draw();
    cLog.SaveAs(outputFileDir + "log_topo_Et.pdf");

    sig_h_gFex_Et->SetLineColor(kRed);
    back_h_gFex_Et->SetLineColor(kBlue);
    back_h_gFex_Et->Draw();
    sig_h_gFex_Et->Draw("SAME");
    
    leg->Draw();
    cLog.SaveAs(outputFileDir + "log_gFex_Et.pdf");

    sig_h_jFex_Et->SetLineColor(kRed);
    back_h_jFex_Et->SetLineColor(kBlue);
    back_h_jFex_Et->Draw();
    sig_h_jFex_Et->Draw("SAME");
    leg->Draw();
    cLog.SaveAs(outputFileDir + "log_jFex_Et.pdf");

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
