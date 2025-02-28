#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>
#include <vector>
#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"

const unsigned int maxEvent = 999;

double undigitize_phi(const std::bitset<8>& phi_bits) {
    return -3.2 + phi_bits.to_ulong() * (6.4 / 256.0);
}

double undigitize_eta(const std::bitset<11>& eta_bits) {
    return -5.0 + eta_bits.to_ulong() * (10.0 / 2048.0);
}

double undigitize_et(const std::bitset<13>& et_bits) {
    return et_bits.to_ulong() * 0.25;
}
void callEventLoop(const std::string& fileName){
    process_file(fileName);
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <bitset>

std::vector<std::array<double, 3>> extract_values_from_file(const std::string& fileName, unsigned int event) {
    std::vector<std::array<double, 3>> values;

    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return values;
    }

    unsigned int eventIt = 0; 

    std::string line;
    while (std::getline(inFile, line)) {
        // Skip event header lines
        if (line.find("Event") != std::string::npos && eventIt == (event + 1)) {
            break;
        }
        if (line.find("Event") != std::string::npos) {
            eventIt++;
            continue;
        }
        

        std::stringstream ss(line);
        std::string index, binary, hex_word;
        ss >> index >> binary >> hex_word;

        // Validate line format
        size_t first_pipe = binary.find('|');
        size_t second_pipe = binary.rfind('|');
        if (first_pipe == std::string::npos || second_pipe == std::string::npos || first_pipe == second_pipe) {
            std::cerr << "Error: Malformed line -> " << line << std::endl;
            continue;
        }

        // Extract binary substrings
        std::string et_bin = binary.substr(0, first_pipe);
        std::string eta_bin = binary.substr(first_pipe + 1, second_pipe - first_pipe - 1);
        std::string phi_bin = binary.substr(second_pipe + 1);

        try {
            // Convert binary to bitsets
            std::bitset<13> et_bits(et_bin);
            std::bitset<11> eta_bits(eta_bin);
            std::bitset<8> phi_bits(phi_bin);

            // Convert bitsets to floating-point values
            double et = undigitize_et(et_bits);
            double eta = undigitize_eta(eta_bits);
            double phi = undigitize_phi(phi_bits);

            // Store in vector
            values.push_back({et, eta, phi});
        } catch (const std::exception& e) {
            std::cerr << "Error processing line: " << line << " -> " << e.what() << std::endl;
        }
    }

    inFile.close();
    return values;
}


/*
template<unsigned int event> 
void process_event(const std::string& fileName);

// Templated event loop so that event is constant able to be used to access file (potentially this is unneeded)
template<unsigned int event>
void eventLoop(const std::string& fileName){
    process_event<event>(fileName);
    eventLoop<event + 1>(fileName);
}





// Specialized final eventLoop function (no longer recursively calls eventLoop)
template<> 
void eventLoop<maxEvent>(const std::string& fileName){
    process_event<maxEvent>(fileName);
}

template<unsigned int event> 
*/
void process_file(const std::string& fileName){ // FIXME use event loop outside of this, that way function is called each event 
    // Open input file
    const std::string eosPath = "/eos/user/m/mlarson/LargeRadiusJets/MemPrints/";
    const std::string gFexFile = eosPath + "gFex/" + fileName + "_gfex_smallrj.dat";
    const std::string topoFile = eosPath + "CaloTopo_422/" + fileName + "_topo422.dat";
    std::cout << "gFex File: " << gFexFile << " and topo file: " << topoFile << "\n";
    std::ifstream gFexInFile(gFexFile);
    std::ifstream topoInFile(topoFile);
    if (!gFexInFile.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }
    if (!topoInFile.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }

    std::string gfex_line;
    int current_event = -1; // Tracks the event being processed
    int eventIt = 0;

    while (std::getline(gFexInFile, gfex_line)) { // FIXME find more efficient way of processing current event (if possible) ?? 
        // Skip non-event header lines
        if (gfex_line.find("Event") != std::string::npos) {
            // Extract the event number
            std::stringstream ss(gfex_line);
            std::string dummy, event_label;
            ss >> dummy >> event_label >> current_event;

            std::cout << "Found Event: " << current_event << std::endl;
            
            // Check if this is the target event
            if (current_event == event) {
                std::cout << "Processing Target Event: " << current_event << std::endl;
            } else {
                // Skip further processing for this event
                continue;
            }
        }

        // If processing a specific event, extract binary word
        if (current_event == event) {
            std::stringstream ss(gfex_line);
            std::string index, binary, hex_word;
            ss >> index >> binary >> hex_word;

            std::cout << "index: " << index << " binary : " << binary << " hex_word: " << hex_word << "\n";

            std::cout << "test 1" << "\n";

            // Split binary into Et, Phi, and Eta
            size_t first_pipe = binary.find('|');
            size_t second_pipe = binary.rfind('|');

            std::cout << "test 1.1" << "\n";

            std::string et_bin = binary.substr(0, first_pipe);
            std::string eta_bin = binary.substr(first_pipe + 1, second_pipe - first_pipe - 1);
            std::string phi_bin = binary.substr(second_pipe + 1);

            std::cout << "test 1.2" << "\n";

            std::cout << "et_bin: " << et_bin << " eta_bin: " << eta_bin << " phi_bin: " << phi_bin << "\n";

            // Convert to bitsets
            std::bitset<13> et_bits(et_bin);
            std::bitset<8> phi_bits(phi_bin);
            std::bitset<11> eta_bits(eta_bin);

            std::cout << "test 1.3" << "\n";

            // Undigitize the values
            double undigitized_et = undigitize_et(et_bits);
            double undigitized_phi = undigitize_phi(phi_bits);
            double undigitized_eta = undigitize_eta(eta_bits);

            std::cout << "Et: " << undigitized_et
                      << ", Phi: " << undigitized_phi
                      << ", Eta: " << undigitized_eta << std::endl;
        }
    }

    std::cout << "Finished processing." << std::endl;

    gFexInFile.close();
    topoInFile.close();

}

const std::string fileName = "mc21_14TeV_hh_bbbb_vbf_novhh"; 

callEventLoop(fileName);