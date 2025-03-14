
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
#include "../algorithm/helperFunctions.h"
using namespace std;


void analyze_files(const std::string backgroundFileName, const std::string signalFileName) {
    SetPlotStyle();
    TH1F* sig_h_LRJ_Et = new TH1F("sig_h_LRJ_Et", "LRJ Et Distribution;E_{T} [GeV];Normalized # of Large R Jets", 50, 0, 1000);
    TH1F* sig_h_LRJ_eta = new TH1F("sig_h_LRJ_eta", "LRJ Eta Distribution;#eta;Counts", 50, -5, 5);
    TH1F* sig_h_LRJ_phi = new TH1F("sig_h_LRJ_phi", "LRJ Phi Distribution;#phi;Counts", 32, -3.2, 3.2);

    TH1F* back_h_LRJ_Et = new TH1F("back_h_LRJ_Et", "LRJ Et Distribution;E_{T} [GeV];Normalized # of Large R Jets", 50, 0, 1000);
    TH1F* back_h_LRJ_eta = new TH1F("back_h_LRJ_eta", "LRJ Eta Distribution;#eta;Counts", 50, -5, 5);
    TH1F* back_h_LRJ_phi = new TH1F("back_h_LRJ_phi", "LRJ Phi Distribution;#phi;Counts", 32, -3.2, 3.2);

    std::vector<double> sig_LRJ_Et;
    std::vector<double> back_LRJ_Et;

    // Variables to store data
    std::vector<double> backgroundEtValues, backgroundEtaValues, backgroundPhiValues;
    std::vector<double> signalEtValues, signalEtaValues, signalPhiValues;

    // Open input file

    int current_event = -1;
    std::string sig_line;
    std::string back_line;
    // Bools determining which file being processed

    std::ifstream sig_infile(signalFileName);
    std::ifstream back_infile(backgroundFileName);
    if (!sig_infile.is_open() || !back_infile.is_open()) {
        std::cerr << "Error: Could not open file " << signalFileName << std::endl;
        return;
    }

    while (std::getline(sig_infile, sig_line) && std::getline(back_infile, back_line)) {
        // Skip event header lines
        if (sig_line.find("Event") != std::string::npos && back_line.find("Event") != std::string::npos) {
            current_event++;
            // Extract the event number
            std::stringstream ss(sig_line);
            std::string dummy, event_label;
            ss >> dummy >> event_label >> current_event;
            std::cout << "Processing Event: " << current_event << std::endl;
            continue;
        }

        // Extract the binary word
        std::stringstream sig_ss(sig_line);
        std::string sig_index, sig_binary, sig_hex_word;
        sig_ss >> sig_index >> sig_binary >> sig_hex_word;

        // Split binary into Et, Phi, and Eta
        size_t sig_first_pipe = sig_binary.find('|');
        size_t sig_second_pipe = sig_binary.rfind('|');

        std::string sig_et_bin = sig_binary.substr(0, sig_first_pipe);
        std::string sig_eta_bin = sig_binary.substr(sig_first_pipe + 1, sig_second_pipe - sig_first_pipe - 1);
        std::string sig_phi_bin = sig_binary.substr(sig_second_pipe + 1);

        // Convert to bitsets
        std::bitset<13> sig_et_bits(sig_et_bin);
        std::bitset<8> sig_phi_bits(sig_phi_bin);
        std::bitset<11> sig_eta_bits(sig_eta_bin);

        // Undigitize the values
        double sig_undigitized_et = undigitize_et(sig_et_bits);
        double sig_undigitized_phi = undigitize_phi(sig_phi_bits);
        double sig_undigitized_eta = undigitize_eta(sig_eta_bits);
        sig_h_LRJ_Et->Fill(sig_undigitized_et);
        sig_LRJ_Et.push_back(sig_undigitized_et);
        sig_h_LRJ_eta->Fill(sig_undigitized_eta);
        sig_h_LRJ_phi->Fill(sig_undigitized_phi);

        std::stringstream back_ss(back_line);
        std::string back_index, back_binary, back_hex_word;
        back_ss >> back_index >> back_binary >> back_hex_word;

        // Split binary into Et, Phi, and Eta
        size_t back_first_pipe = back_binary.find('|');
        size_t back_second_pipe = back_binary.rfind('|');

        std::string back_et_bin = back_binary.substr(0, back_first_pipe);
        std::string back_eta_bin = back_binary.substr(back_first_pipe + 1, back_second_pipe - back_first_pipe - 1);
        std::string back_phi_bin = back_binary.substr(back_second_pipe + 1);

        // Convert to bitsets
        std::bitset<13> back_et_bits(back_et_bin);
        std::bitset<8> back_phi_bits(back_phi_bin);
        std::bitset<11> back_eta_bits(back_eta_bin);

        // Undigitize the values
        double back_undigitized_et = undigitize_et(back_et_bits);
        double back_undigitized_phi = undigitize_phi(back_phi_bits);
        double back_undigitized_eta = undigitize_eta(back_eta_bits);
        back_h_LRJ_Et->Fill(back_undigitized_et);
        back_LRJ_Et.push_back(sig_undigitized_et);
        back_h_LRJ_eta->Fill(back_undigitized_phi);
        back_h_LRJ_phi->Fill(back_undigitized_eta);

    }

    vector<double> roc_curve_points_x;
    vector<double> roc_curve_points_y;

    vector<double> efficiency_curve_points_x;
    vector<double> efficiency_curve_points_y;
    double det_cutoff = 1.0;
    for (double et_cutoff = det_cutoff; et_cutoff < 500.0; et_cutoff += det_cutoff) {

        int numTruePositive = 0;
        for (int i = 0; i < sig_LRJ_Et.size(); i++) {
            if (useMax_){
                if (energyMax(sig_LRJ_Et.at(i),
                                    et_cutoff)) {
                numTruePositive++;
                }
            }
            else {
                if (energyMin(sig_LRJ_Et.at(i),
                                    et_cutoff)) {
                numTruePositive++;
                }
            }
            
        }
        double truePositiveRate = ((double)numTruePositive)/((double)sig_LRJ_Et.size());
        roc_curve_points_x.emplace_back(truePositiveRate);

        efficiency_curve_points_x.emplace_back(et_cutoff);
        efficiency_curve_points_y.emplace_back(truePositiveRate);

        int numFalsePositive = 0;
        for (int i = 0; i < back_LRJ_Et.size(); i++) {
            if (useMax_){
                if (energyMax(back_LRJ_Et.at(i),
                                    et_cutoff)) {
                numFalsePositive++;
                }
            }
            else{
                if (energyMin(back_LRJ_Et.at(i),
                                    et_cutoff)) {
                numFalsePositive++;
                }
            }
            
        }
        double falsePositiveRate = ((double)numFalsePositive)/((double) back_LRJ_Et.size());
        double backgroundRejection = 1.0 / falsePositiveRate;

        roc_curve_points_y.emplace_back(backgroundRejection);
    } 

    // Save histograms
    TCanvas c;
    TString outputFileDir = "overlayLargeRJetHistograms/";
    TLegend *leg = new TLegend(0.6,0.6,0.95,0.95);
    leg->SetTextSize(0.05);
    leg->AddEntry(sig_h_LRJ_Et, "Signal", "l");
    leg->AddEntry(back_h_LRJ_Et, "Background", "l");
    sig_h_LRJ_Et->Scale(1.0 / sig_h_LRJ_Et->Integral());
    back_h_LRJ_Et->Scale(1.0 / back_h_LRJ_Et->Integral());
    sig_h_LRJ_Et->SetLineColor(kRed);
    back_h_LRJ_Et->SetLineColor(kBlue);
    sig_h_LRJ_Et->Draw("HIST");
    back_h_LRJ_Et->Draw("HIST SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "LRJ_Et.pdf");

    sig_h_LRJ_eta->SetLineColor(kRed);
    back_h_LRJ_eta->SetLineColor(kBlue);
    sig_h_LRJ_eta->Draw();
    back_h_LRJ_eta->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "LRJ_eta.pdf");

    sig_h_LRJ_phi->SetLineColor(kRed);
    back_h_LRJ_phi->SetLineColor(kBlue);
    sig_h_LRJ_phi->Draw();
    back_h_LRJ_phi->Draw("SAME");
    leg->Draw();
    c.SaveAs(outputFileDir + "LRJ_phi.pdf");


    // Create ROC Curve Graph
    TGraph* gr = new TGraph(roc_curve_points_x.size(), &roc_curve_points_x[0], &roc_curve_points_y[0]);
    gr->SetTitle("ROC Curve;True Positive Rate;Background Rejection (1/FPR)");

    TCanvas* roc_plot_canvas = new TCanvas("roc_plot_canvas", "ROC Curve", 710, 10, 700, 500);

    gr->SetMarkerColor(kBlue);

    gr->GetYaxis()->SetRangeUser(0, 20);
    gr->SetMarkerSize(0.5);
    gr->Draw();

    // Save the ROC plot as a PDF
    roc_plot_canvas->SaveAs(outputFileDir + "roc_curve.pdf");

    // Create Efficiency Curve Graph
    TGraph* gr2 = new TGraph(efficiency_curve_points_x.size(), &efficiency_curve_points_x[0], &efficiency_curve_points_y[0]);
    gr2->SetTitle("Efficiency Curve;E_{T} Minimum;True Positive Rate");

    TCanvas* eff_plot_canvas = new TCanvas("eff_plot_canvas", "Efficiency Curve", 0, 10, 700, 500);

    gr2->SetMarkerColor(kBlue);

    gr2->GetYaxis()->SetRangeUser(0, 1);
    gr2->SetMarkerSize(0.5);
    gr2->Draw();

    // Save the Efficiency plot as a PDF
    eff_plot_canvas->SaveAs(outputFileDir + "efficiency_curve.pdf");
}


void callAnalyzer(){
    const std::string signalLargeRJetDataFileName = "/eos/home-m/mlarson/LargeRadiusJets/MemPrints/largeRJets/mc21_14TeV_hh_bbbb_vbf_novhh_largeR.dat";
    const std::string backgroundLargeRJetDataFileName = "/eos/home-m/mlarson/LargeRadiusJets/MemPrints/largeRJets/mc21_14TeV_jj_JZ3_largeR.dat";
    analyze_files(backgroundLargeRJetDataFileName, signalLargeRJetDataFileName);
}





/*


bool AcceptanceAlgorithm(double eta, double phi, double pt, double m, double et_cutoff) {
    return pt >= et_cutoff;
}

const double det_cutoff = 1.0;
void GenerateRocCurve(double R) {
    // Positive Data Set: 973 Jets AntiKt4lctopoCaloCalJets from Zprime_tthad_mu200
    int numMembersPositive = 0;

    ifstream testdatafile;
    testdatafile.open("SavedDataSets/PositiveDataSet.bin");
    testdatafile.read(reinterpret_cast<char*>(&numMembersPositive), sizeof(int));
    
    vector<double> p_test_etas(numMembersPositive, 0);
    vector<double> p_test_phis(numMembersPositive, 0);
    vector<double> p_test_pts(numMembersPositive, 0);
    vector<double> p_test_ms(numMembersPositive, 0);

    testdatafile.read(reinterpret_cast<char*>(p_test_etas.data()), numMembersPositive*sizeof(double));
    testdatafile.read(reinterpret_cast<char*>(p_test_phis.data()), numMembersPositive*sizeof(double));
    testdatafile.read(reinterpret_cast<char*>(p_test_pts.data()), numMembersPositive*sizeof(double));
    testdatafile.read(reinterpret_cast<char*>(p_test_ms.data()), numMembersPositive*sizeof(double));
    testdatafile.close();

    vector<double>* positive_result_jets_etas;
    vector<double>* positive_result_jets_phis;
    vector<double>* positive_result_jets_pts;
    vector<double>* positive_result_jets_m;
    antiKtAlgorithm(R,
                    &p_test_etas, &p_test_phis, &p_test_pts, &p_test_ms,
                    positive_result_jets_etas, positive_result_jets_phis, positive_result_jets_pts, positive_result_jets_m);


    // Negative Data Set: 1116 Jets AntiKt4lctopoCaloCalJets from JZ3WithSW_mu200
    int numMembersNegative = 0;

    testdatafile.open("SavedDataSets/NegativeDataSet.bin");
    testdatafile.read(reinterpret_cast<char*>(&numMembersNegative), sizeof(int));
    
    vector<double> n_test_etas(numMembersNegative, 0);
    vector<double> n_test_phis(numMembersNegative, 0);
    vector<double> n_test_pts(numMembersNegative, 0);
    vector<double> n_test_ms(numMembersNegative, 0);

    testdatafile.read(reinterpret_cast<char*>(n_test_etas.data()), numMembersNegative*sizeof(double));
    testdatafile.read(reinterpret_cast<char*>(n_test_phis.data()), numMembersNegative*sizeof(double));
    testdatafile.read(reinterpret_cast<char*>(n_test_pts.data()), numMembersNegative*sizeof(double));
    testdatafile.read(reinterpret_cast<char*>(n_test_ms.data()), numMembersNegative*sizeof(double));
    testdatafile.close();

    vector<double>* negative_result_jets_etas;
    vector<double>* negative_result_jets_phis;
    vector<double>* negative_result_jets_pts;
    vector<double>* negative_result_jets_m;
    antiKtAlgorithm(R,
                    &n_test_etas, &n_test_phis, &n_test_pts, &n_test_ms,
                    negative_result_jets_etas, negative_result_jets_phis, negative_result_jets_pts, negative_result_jets_m);

    int numClusteredPositive = positive_result_jets_etas->size();
    int numClusteredNegative = negative_result_jets_etas->size();

    cout << "pos: " << numMembersPositive << " -> " << numClusteredPositive << endl;
    cout << "neg: " << numMembersNegative << " -> " << numClusteredNegative << endl;

    vector<double> roc_curve_points_x;
    vector<double> roc_curve_points_y;

    vector<double> efficiency_curve_points_x;
    vector<double> efficiency_curve_points_y;
    for (double et_cutoff = det_cutoff; et_cutoff < 5000.0; et_cutoff += det_cutoff) {

        int numTruePositive = 0;
        for (int i = 0; i < numClusteredPositive; i++) {
            if (AcceptanceAlgorithm(positive_result_jets_etas->at(i),
                                    positive_result_jets_phis->at(i),
                                    positive_result_jets_pts->at(i),
                                    positive_result_jets_m->at(i),
                                    et_cutoff)) {
                numTruePositive++;
            }
        }
        double truePositiveRate = ((double)numTruePositive)/((double)numClusteredPositive);
        roc_curve_points_x.emplace_back(truePositiveRate);

        efficiency_curve_points_x.emplace_back(et_cutoff);
        efficiency_curve_points_y.emplace_back(truePositiveRate);

        int numFalsePositive = 0;
        for (int i = 0; i < numClusteredNegative; i++) {
            if (AcceptanceAlgorithm(negative_result_jets_etas->at(i),
                                    negative_result_jets_phis->at(i),
                                    negative_result_jets_pts->at(i),
                                    negative_result_jets_m->at(i),
                                    et_cutoff)) {
                numFalsePositive++;
            }
        }
        double falsePositiveRate = ((double)numFalsePositive)/((double)numClusteredNegative);
        double backgroundRejection = 1.0 / falsePositiveRate;

        roc_curve_points_y.emplace_back(backgroundRejection);
    }

    // Create ROC Cruve Graph
    TGraph* gr = new TGraph(roc_curve_points_x.size(), &roc_curve_points_x[0], &roc_curve_points_y[0]);
    gr->SetTitle("ROC Curve;True Positive Rate;Background Rejection (1/FPR)");
    
    TCanvas* roc_plot_canvas = new TCanvas("roc_plot_canvas", "ROC Curve", 710, 10, 700, 500);
    
    gr->SetMarkerColor(kBlue);
    gr->SetMarkerStyle(20);

    gr->GetYaxis()->SetRangeUser(0, 10);

    gr->Draw("APL");

    // Create Eff Cruve Graph
    TGraph* gr2 = new TGraph(efficiency_curve_points_x.size(), &efficiency_curve_points_x[0], &efficiency_curve_points_y[0]);
    gr2->SetTitle("Efficiency Curve;P_t Cutoff;True Positive Rate");
    
    TCanvas* eff_plot_canvas = new TCanvas("eff_plot_canvas", "Efficiency Curve", 0, 10, 700, 500);
    
    gr2->SetMarkerColor(kBlue);
    gr2->SetMarkerStyle(20);

    gr2->GetYaxis()->SetRangeUser(0, 1);

    gr2->Draw("APL");
}*/