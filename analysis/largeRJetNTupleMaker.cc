#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>
#include <vector>
#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "analysisHelperFunctions.h"
/*
double undigitize_phi(const std::bitset<8>& phi_bits) {
    return -3.2 + phi_bits.to_ulong() * (6.4 / 256.0);
}

double undigitize_eta(const std::bitset<eta_bit_length_>& eta_bits) {
    return -5.0 + eta_bits.to_ulong() * (10.0 / 2048.0);
}

double undigitize_et(const std::bitset<et_bit_length_>& et_bits) {
    return et_bits.to_ulong() * 0.25;
}
*/
void analyze_file(const std::vector<std::string> fileNames, const bool signalBool) {
    
    TString outputFileName; 
    if (signalBool) outputFileName = "outputRootFiles/mc21_14TeV_hh_bbbb_vbf_novhh.root";
    else outputFileName = "outputRootFiles/mc21_14TeV_jj_JZ3.root";

    // Create ROOT output file
    TFile* outputFile = new TFile(outputFileName, "RECREATE");

    // Create a TTree
    TTree* topoTree = new TTree("topoTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* caloTopoTree = new TTree("caloTopoTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gFexTree = new TTree("gFexTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* jFexTree = new TTree("jFexTree", "Tree storing event-wise Et, Eta, Phi");

    // Variables to store data
    int topoEventNumber, caloTopoEventNumber, gFexEventNumber, jFexEventNumber;
    std::vector<double> topoEtValues, topoEtaValues, topoPhiValues;
    std::vector<double> caloTopoEtValues, caloTopoEtaValues, caloTopoPhiValues;
    std::vector<double> gFexEtValues, gFexEtaValues, gFexPhiValues;
    std::vector<double> jFexEtValues, jFexEtaValues, jFexPhiValues;

    // Create branches
    topoTree->Branch("eventNumber", &topoEventNumber, "eventNumber/I");
    topoTree->Branch("Et", &topoEtValues);
    topoTree->Branch("Eta", &topoEtaValues);
    topoTree->Branch("Phi", &topoPhiValues);

    caloTopoTree->Branch("eventNumber", &caloTopoEventNumber, "eventNumber/I");
    caloTopoTree->Branch("Et", &caloTopoEtValues);
    caloTopoTree->Branch("Eta", &caloTopoEtaValues);
    caloTopoTree->Branch("Phi", &caloTopoPhiValues);

    gFexTree->Branch("eventNumber", &gFexEventNumber, "eventNumber/I");
    gFexTree->Branch("Et", &gFexEtValues);
    gFexTree->Branch("Eta", &gFexEtaValues);
    gFexTree->Branch("Phi", &gFexPhiValues);

    jFexTree->Branch("eventNumber", &jFexEventNumber, "eventNumber/I");
    jFexTree->Branch("Et", &jFexEtValues);
    jFexTree->Branch("Eta", &jFexEtaValues);
    jFexTree->Branch("Phi", &jFexPhiValues);

    // Open input file

    for (std::string fileName : fileNames){
        int current_event = -1;
        std::string line;
        // Bools determining which file being processed
        bool topo = false;
        bool gFex = false;
        bool jFex = false;
        bool caloTopo = false;
        if (fileName.find("topo422") != std::string::npos) topo = true;
        if (fileName.find("calotopo") != std::string::npos) caloTopo = true;
        if (fileName.find("gfex") != std::string::npos) gFex = true;
        if (fileName.find("jfex") != std::string::npos) jFex = true;
        std::ifstream infile(fileName);
        if (!infile.is_open()) {
            std::cerr << "Error: Could not open file " << fileName << std::endl;
            return;
        }

        while (std::getline(infile, line)) {
            // Skip event header lines
            if (line.find("Event") != std::string::npos) {
                
                if (current_event != -1) {
                    if (topo){
                        topoEventNumber = current_event;
                        topoTree->Fill();
                        topoEtValues.clear();
                        topoEtaValues.clear();
                        topoPhiValues.clear();
                    }
                    if (caloTopo){
                        caloTopoEventNumber = current_event;
                        caloTopoTree->Fill();
                        caloTopoEtValues.clear();
                        caloTopoEtaValues.clear();
                        caloTopoPhiValues.clear();
                    }
                    if (gFex){
                        topoEventNumber = current_event;
                        gFexTree->Fill();
                        gFexEtValues.clear();
                        gFexEtaValues.clear();
                        gFexPhiValues.clear();
                    }
                    if (jFex){
                        topoEventNumber = current_event;
                        jFexTree->Fill();
                        jFexEtValues.clear();
                        jFexEtaValues.clear();
                        jFexPhiValues.clear();
                    }
                }
                current_event++;
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
            std::bitset<et_bit_length_> et_bits(et_bin);
            std::bitset<phi_bit_length_> phi_bits(phi_bin);
            std::bitset<eta_bit_length_> eta_bits(eta_bin);
    
            // Undigitize the values
            double undigitized_et = undigitize_et(et_bits);
            double undigitized_phi = undigitize_phi(phi_bits);
            //std::cout << std::dec << "eta_bits: " << eta_bits << "\n";
            double undigitized_eta = undigitize_eta(eta_bits);
            //std::cout << std::dec << "undig eta: " << undigitized_eta << "\n";

            if (topo){
                // Store values in vectors
                topoEtValues.push_back(undigitized_et);
                topoEtaValues.push_back(undigitized_eta);
                topoPhiValues.push_back(undigitized_phi);
            }
            if (caloTopo){
                // Store values in vectors
                caloTopoEtValues.push_back(undigitized_et);
                caloTopoEtaValues.push_back(undigitized_eta);
                caloTopoPhiValues.push_back(undigitized_phi);
            }
            if (gFex){
                gFexEtValues.push_back(undigitized_et);
                gFexEtaValues.push_back(undigitized_eta);
                gFexPhiValues.push_back(undigitized_phi);
            }
            if (jFex){
                jFexEtValues.push_back(undigitized_et);
                jFexEtaValues.push_back(undigitized_eta);
                jFexPhiValues.push_back(undigitized_phi);
            }
    
            
        }

        // Fill the last event
        if (topo) topoTree->Fill();
        if (caloTopo) caloTopoTree->Fill();
        else if (gFex) gFexTree->Fill();
        else if (jFex) jFexTree->Fill();
        infile.close();

    }
    // Write and close the file
    topoTree->Write();
    caloTopoTree->Write();
    gFexTree->Write();
    jFexTree->Write();
    outputFile->Close();
}

void callNTupleMaker(const bool signalBool) {
    // usage: callNTupleMaker(true), callNTupleMaker(false)

    std::vector<std::string > fileNames; 
    std::string fileName;

    
    fileName = "/data/larsonma/LargeRadiusJets/MemPrints/CaloTopo_422/";
    if (signalBool) fileName += "mc21_14TeV_hh_bbbb_vbf_novhh_topo422.dat";
    else fileName += "mc21_14TeV_jj_JZ3_topo422.dat";
    fileNames.push_back(fileName);

    fileName = "/data/larsonma/LargeRadiusJets/MemPrints/CaloTopoTowers/";
    if (signalBool) fileName += "mc21_14TeV_hh_bbbb_vbf_novhh_calotopotowers.dat";
    else fileName += "mc21_14TeV_jj_JZ3_calotopotowers.dat";
    fileNames.push_back(fileName);

    fileName = "/data/larsonma/LargeRadiusJets/MemPrints/gFex/";
    if (signalBool) fileName += "mc21_14TeV_hh_bbbb_vbf_novhh_gfex_smallrj.dat";
    else fileName += "mc21_14TeV_jj_JZ3_gfex_smallrj.dat";
    fileNames.push_back(fileName);

    fileName = "/data/larsonma/LargeRadiusJets/MemPrints/jFex/";
    if (signalBool) fileName += "mc21_14TeV_hh_bbbb_vbf_novhh_jfex_smallrj.dat";
    else fileName += "mc21_14TeV_jj_JZ3_jfex_smallrj.dat";
    fileNames.push_back(fileName);

    analyze_file(fileNames, signalBool);
    return;
}
