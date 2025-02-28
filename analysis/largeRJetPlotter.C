#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"

void SetPlotStyle();
void mySmallText(Double_t x, Double_t y, Color_t color, char* text);

double deltaPhi(double phi1, double phi2) {
    double dphi = phi1 - phi2;
    if (dphi > TMath::Pi()) dphi -= 2 * TMath::Pi();
    if (dphi <= -TMath::Pi()) dphi += 2 * TMath::Pi();
    return dphi;
}

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


void styleHistogram(TH1F* hist, int color, const std::string& xTitle, const std::string& yTitle) {
    // Apply styling to the histogram
    hist->SetLineColor(color);
    hist->SetLineWidth(2);
    hist->SetTitle("");
    hist->GetXaxis()->SetTitle(xTitle.c_str());
    hist->GetYaxis()->SetTitle(yTitle.c_str());
    hist->GetXaxis()->SetTitleSize(0.04);
    hist->GetYaxis()->SetTitleSize(0.04);
    hist->GetXaxis()->SetLabelSize(0.03);
    hist->GetYaxis()->SetLabelSize(0.03);
}

void analyze_existing_histograms(const std::string& inputFileName, const std::string& outputPrefix) {
    //SetPlotStyle();
    // Open input ROOT file
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

void callPlot(const bool signalBool, const bool overlay = false) {
    // Usage: callPlot(true)
    std::string inputFile;
    if (signalBool) inputFile = "outputRootFiles/mc21_14TeV_hh_bbbb_vbf_novhh.root";
    else inputFile = "outputRootFiles/mc21_14TeV_jj_JZ3.root";
    std::string outputPrefix;
    if (signalBool) outputPrefix = "signalHistograms/";
    else outputPrefix = "backgroundHistograms/"; 

    analyze_existing_histograms(inputFile, outputPrefix);
}



// Declare helper functions 
void SetPlotStyle() {
  // from ATLAS plot style macro

  // use plain black on white colors
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetStatColor(0);
  gStyle->SetHistLineColor(1);

  gStyle->SetPalette(1);

  // set the paper & margin sizes
  gStyle->SetPaperSize(20, 26);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  gStyle->SetTitleXOffset(1.4);
  gStyle->SetTitleYOffset(1.4);

  // use large fonts
  gStyle->SetTextFont(42);
  gStyle->SetTextSize(0.07);
  gStyle->SetLabelFont(42, "x");
  gStyle->SetTitleFont(42, "x");
  gStyle->SetLabelFont(42, "y");
  gStyle->SetTitleFont(42, "y");
  gStyle->SetLabelFont(42, "z");
  gStyle->SetTitleFont(42, "z");
  gStyle->SetLabelSize(0.05, "x");
  gStyle->SetTitleSize(0.05, "x");
  gStyle->SetLabelSize(0.05, "y");
  gStyle->SetTitleSize(0.05, "y");
  gStyle->SetLabelSize(0.05, "z");
  gStyle->SetTitleSize(0.05, "z");

  // use bold lines and markers
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);
  gStyle->SetHistLineWidth(2.);
  gStyle->SetLineStyleString(2, "[12 12]");

  // get rid of error bar caps
  gStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
}

void mySmallText(Double_t x, Double_t y, Color_t color, char* text) {
  Double_t tsize = 0.044;
  TLatex l;
  l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x, y, text);
}
