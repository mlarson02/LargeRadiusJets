
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
    


void analyze_files(std::vector<std::string > backgroundFileNames, std::vector<std::string > signalFileNames) {
    SetPlotStyle();
    TH1F* sig_h_LRJ_nmio = new TH1F("sig_h_LRJ_nmio", "Merged IOs per LRJs;Num. Merged Input Objects;Normalized # of Large R Jets / 2 IOs", 20, 0, 40);
    TH1F* sig_h_LRJ_Et = new TH1F("sig_h_LRJ_Et", "LRJ Et Distribution;E_{T} [GeV];Normalized # of Large R Jets / 10 GeV", 75, 0, 750);
    TH1F* sig_h_leading_LRJ_Et = new TH1F("sig_h_leading_LRJ_Et", "Leading LRJ Et Distribution;E_{T} [GeV];Normalized # of Leading Large R Jets / 10 GeV", 75, 0, 750);
    TH1F* sig_h_subleading_LRJ_Et = new TH1F("sig_h_subleading_LRJ_Et", "Subleading LRJ Et Distribution;E_{T} [GeV];Normalized # of Subleading Large R Jets / 10 GeV", 75, 0, 750);
    TH1F* sig_h_LRJ_E = new TH1F("sig_h_LRJ_E", "LRJ Et Distribution;Energy [GeV];Normalized # of Large R Jets / 10 GeV", 100, 0, 1000);
    TH1F* sig_h_LRJ_eta = new TH1F("sig_h_LRJ_eta", "LRJ Eta Distribution;#eta;Counts", 50, -5, 5);
    TH1F* sig_h_LRJ_phi = new TH1F("sig_h_LRJ_phi", "LRJ Phi Distribution;#phi;Counts", 32, -3.2, 3.2);

    TH1F* back_h_LRJ_nmio = new TH1F("back_h_LRJ_nmio", "Merged IOs per LRJs;Num. Merged Input Objects;Normalized # of Large R Jets / 2 IOs", 20, 0, 40);
    TH1F* back_h_LRJ_Et = new TH1F("back_h_LRJ_Et", "LRJ Et Distribution;E_{T} [GeV];Normalized # of Large R Jets / 10 GeV", 75, 0, 750);
    TH1F* back_h_leading_LRJ_Et = new TH1F("back_h_leading_LRJ_Et", "Leading LRJ Et Distribution;E_{T} [GeV];Normalized # of Leading Large R Jets / 10 GeV", 75, 0, 750);
    TH1F* back_h_subleading_LRJ_Et = new TH1F("back_h_subleading_LRJ_Et", "Subleading LRJ Et Distribution;E_{T} [GeV];Normalized # of Subleading Large R Jets / 10 GeV", 75, 0, 750);
    TH1F* back_h_LRJ_E = new TH1F("back_h_LRJ_E", "LRJ Et Distribution;Energy [GeV];Normalized # of Large R Jets / 10 GeV", 100, 0, 1000);
    TH1F* back_h_LRJ_eta = new TH1F("back_h_LRJ_eta", "LRJ Eta Distribution;#eta;Counts", 50, -5, 5);
    TH1F* back_h_LRJ_phi = new TH1F("back_h_LRJ_phi", "LRJ Phi Distribution;#phi;Counts", 32, -3.2, 3.2);

    gSystem->RedirectOutput("output2.log", "w");
    const int num_processed_events = 3300;

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

    vector<vector<double> > efficiency_curve_points_x(backgroundFileNames.size());
    vector<vector<double> > efficiency_curve_points_y(backgroundFileNames.size());
    vector<double > tprMinMaxCut(backgroundFileNames.size());
    vector<double > fprMinMaxCut(backgroundFileNames.size());
    // Bools determining which file being processed

    for (int fileIt = 0; fileIt < backgroundFileNames.size(); ++fileIt){
        //if (fileIt > 0) break;
        vector<vector<double> > sig_LRJ_Et(num_processed_events);
        vector<vector<double> > back_LRJ_Et(num_processed_events);
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
            back_h_LRJ_eta->Fill(back_undigitized_phi);
            back_h_LRJ_phi->Fill(back_undigitized_eta);

        }

        /*for (int i = 0; i < nSeeds_; i++){ // loop not needed for just 2 seeds considered
            if (i == 0){
                
            }
            if (i == 1){
                
            }
            
        } */

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
                if(sig_LRJ_Et[i][0] >= et_cutoff && sig_LRJ_Et[i][1] >= et_cutoff){
                    numTruePositive++;
                }
                    //if (energyMin(sig_LRJ_Et.at(i),
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
                if(back_LRJ_Et[i][0] >= et_cutoff && back_LRJ_Et[i][1] >= et_cutoff){
                    numFalsePositive++;
                }
                /*if (useMax_){
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
                }*/
                
            }

            double falsePositiveRate = ((double)numFalsePositive)/((double) num_processed_events);
            if (falsePositiveRate == 0) falsePositiveRate = 0.1; 
            double backgroundRejection = 1.0 / falsePositiveRate;
            //std::cout << " backgroundRejectioN: "  << backgroundRejection << " for et_cutoff: " << et_cutoff << " and file: " << algorithmConfigurations[fileIt] << "\n";
            roc_curve_points_y[fileIt].emplace_back(backgroundRejection);
            //std::cout << "signal to background: " << std::fixed << std::setprecision(16) << backgroundRejection * truePositiveRate << "\n";
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
        TString outputFileDir = "overlayLargeRJetHistogramsjFexNoSeedEnergy/";
        TString modifiedOutputFileDir = "overlayLargeRJetHistogramsjFexNoSeedEnergy_" + algorithmConfigurations[fileIt] + "/";
        gSystem->mkdir(modifiedOutputFileDir);
        //gStyle->SetOptTitle(1); // Enable title
        TLegend *leg = new TLegend(0.8,0.8,0.95,0.95);
        leg->SetTextSize(0.025);
        leg->AddEntry(sig_h_LRJ_Et, "Signal", "l");
        leg->AddEntry(back_h_LRJ_Et, "Background", "l");
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

        std::cout << "Leading signal mean: " << sig_h_leading_LRJ_Et->GetMean() << " and leading background mean: " << back_h_leading_LRJ_Et->GetMean() << "\n";
        sig_h_leading_LRJ_Et->Scale(1.0 / sig_h_leading_LRJ_Et->Integral());
        back_h_leading_LRJ_Et->Scale(1.0 / back_h_leading_LRJ_Et->Integral());
        sig_h_leading_LRJ_Et->SetLineColor(kRed);
        back_h_leading_LRJ_Et->SetLineColor(kBlue);
        sig_h_leading_LRJ_Et->Draw("HIST");
        back_h_leading_LRJ_Et->Draw("HIST SAME");
        
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

    } 

    TString outputFileDir = "overlayLargeRJetHistogramsjFexNoSeedEnergy/";

    // Create ROC Curve Canvas with 2 pads: top (ROC) and bottom (S/B)
    TLegend* legend1 = new TLegend(0.45, 0.45, 0.95, 0.95);
    legend1->SetTextSize(0.035); // Adjust text size

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
        gr->GetYaxis()->SetRangeUser(0, 10);
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
        std::cout << "----------------------------------------------" << "\n";
        std::cout << "----------------- SUMMARY --------------------" << "\n";
        std::cout << "maxSignalToBackgroundRatio for " << legendMap[algorithmConfigurations[fileIt]].c_str() << " is: " << std::fixed << std::setprecision(8) << *maxIt << "\n";
        size_t maxIndex = std::distance(maxSignalToBackgroundRatio[fileIt].begin(), maxIt);
        std::cout << "corresponding Et cut value: " << maxIndex * 1.0 + 1.0 << "\n";
        std::cout << "and corresponding back rej rate: " << roc_curve_points_y[fileIt][maxIndex] << " and tpr: " << roc_curve_points_x[fileIt][maxIndex] << "\n";
        std::cout << "applying cut between 100, 200 GeV yields tpr: " << tprMinMaxCut[fileIt] << " and background rejection rate: " << 1/(fprMinMaxCut[fileIt]) << " and sig to back ratio: " << tprMinMaxCut[fileIt] / (fprMinMaxCut[fileIt]) << "\n";
        std::cout << "----------------------------------------------" << "\n";

        // Save after final iteration
        if (fileIt == backgroundFileNames.size() - 1) {
            roc_plot_canvas->cd();
            pad1->cd();
            legend1->Draw();

            pad2->cd();
            TH1F* h_frame = pad2->DrawFrame(0, 0, 1, 2.5, ";Signal Efficiency;Sig. / Back. Ratio");
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
    std::vector<std::string > signalFilenames = {"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p21_maxObj128_sig_noecut_ecutVal4.dat",
                                               "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p44_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p69_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p96_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj256_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p21_maxObj256_sig_noecut_ecutVal4.dat",
                                               "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p44_maxObj256_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p69_maxObj256_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p96_maxObj256_sig_noecut_ecutVal4.dat"
                                                
                                                /*"/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut0p64_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut0p64_maxObj128_sig_ecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p44_maxObj128_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj256_sig_ecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj256_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p0_maxObj512_sig_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMerged/mc21_14TeV_hh_bbbb_vbf_novhh_largeRnSeeds2_r2Cut1p44_maxObj512_sig_noecut_ecutVal4.dat"*/
                                                };

    std::vector<std::string > backgroundFilenames = { "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p0_maxObj128_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p21_maxObj128_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p44_maxObj128_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p69_maxObj128_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p96_maxObj128_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p0_maxObj256_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p21_maxObj256_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p44_maxObj256_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p69_maxObj256_back_noecut_ecutVal4.dat",
                                                "/home/larsonma/LargeRadiusJets/data/MemPrints/largeRJetsNoSeedEnergyMergedjFexSeeds/mc21_14TeV_jj_JZ3_largeRnSeeds2_r2Cut1p96_maxObj256_back_noecut_ecutVal4.dat"
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

    analyze_files(backgroundFilenames, signalFilenames);
    gSystem->Exit(0);
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