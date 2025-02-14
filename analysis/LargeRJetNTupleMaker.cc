#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>
#include <vector>
#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"

double undigitize_phi(const std::bitset<8>& phi_bits) {
    return -3.2 + phi_bits.to_ulong() * (6.4 / 256.0);
}

double undigitize_eta(const std::bitset<11>& eta_bits) {
    return -5.0 + eta_bits.to_ulong() * (10.0 / 2048.0);
}

double undigitize_et(const std::bitset<13>& et_bits) {
    return et_bits.to_ulong() * 0.25;
}

void analyze_file(const std::string& fileName, const std::string& dataType) {
    // Open input file
    std::ifstream infile(fileName);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }

    // ROOT histograms
    TH1F* h_phi = new TH1F("h_phi", "Phi Distribution;Phi;Counts", 100, -3.2, 3.2);
    TH1F* h_eta = new TH1F("h_eta", "Eta Distribution;Eta;Counts", 100, -5, 5);
    TH1F* h_et = new TH1F("h_et", "Et Distribution;Et (GeV);Counts", 100, 0, 4096);

    std::string line;
    int current_event = -1;
    while (std::getline(infile, line)) {
        // Skip event header lines
        if (line.find("Event") != std::string::npos) {
            // Extract the event number
            std::stringstream ss(line);
            std::string dummy, event_label;
            ss >> dummy >> event_label >> current_event;
            std::cout << "Processing Event: " << current_event << std::endl;
            continue;
        }
        //if (current_event > 9) break;

        // Extract the binary word
        std::stringstream ss(line);
        std::string index, binary, hex_word;
        ss >> index >> binary >> hex_word;

        // Split binary into Et, Phi, and Eta
        size_t first_pipe = binary.find('|');
        size_t second_pipe = binary.rfind('|');

        std::string et_bin = binary.substr(0, first_pipe);
        std::string eta_bin = binary.substr(first_pipe + 1, second_pipe - first_pipe - 1);
        std::string phi_bin = binary.substr(second_pipe + 1);

        // Convert to bitsets
        std::bitset<13> et_bits(et_bin);
        std::bitset<8> phi_bits(phi_bin);
        std::bitset<11> eta_bits(eta_bin);

        // Undigitize the values
        double undigitized_et = undigitize_et(et_bits);
        double undigitized_phi = undigitize_phi(phi_bits);
        //std::cout << std::dec << "eta_bits: " << eta_bits << "\n";
        double undigitized_eta = undigitize_eta(eta_bits);
        //std::cout << std::dec << "undig eta: " << undigitized_eta << "\n";

        // Fill histograms
        h_phi->Fill(undigitized_phi);
        h_eta->Fill(undigitized_eta);
        h_et->Fill(undigitized_et);
    }

    infile.close();

    // Create a canvas and draw the histograms
    TCanvas* canvas = new TCanvas("canvas", "Histograms", 900, 600);
    canvas->Divide(2, 2);
    canvas->cd(1);
    h_phi->Draw();
    canvas->cd(2);
    h_eta->Draw();
    canvas->cd(3);
    h_et->Draw();

    // Save histograms to a ROOT file
    TString outputFile = "outputRootFiles/" + dataType + ".root";
    TFile* outfile = new TFile(outputFile, "RECREATE");
    h_phi->Write();
    h_eta->Write();
    h_et->Write();
    outfile->Close();

    std::cout << "Histograms saved to " << dataType << ".root" << std::endl;
}

void callPlot(const std::string& dataType) {
    // usage: callPlot("topo422")


    //std::string dataType = "topo422"; 
    //std::string dataType = "gFex_smallrj"; 
    //std::string dataType = "jFex_smallrj"; 
    std::string fileName; 
    if(dataType == "topo422") {
        fileName = "/eos/user/m/mlarson/LargeRadiusJets/MemPrints/CaloTopo_422/mc21_14TeV_hh_bbbb_vbf_novhh_topo422.dat";
    }
    else if (dataType == "gFex_smallrj"){
        fileName = "/eos/home-m/mlarson/LargeRadiusJets/MemPrints/gFex/mc21_14TeV_hh_bbbb_vbf_novhh_gfex_smallrj.dat";
    }
    else if (dataType == "jFex_smallrj"){
        fileName = "/eos/home-m/mlarson/LargeRadiusJets/MemPrints/jFex/mc21_14TeV_hh_bbbb_vbf_novhh_jfex_smallrj.dat";
    }
    
    analyze_file(fileName, dataType);
    return;
}
