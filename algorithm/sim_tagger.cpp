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

#include "helperFunctions.h"

/* // old version for each event
void extract_values_from_file(const std::string& fileName, unsigned int event, std::vector<std::array<double, 3>>& values) {

    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }

    unsigned int eventIt = 0; 

    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << "eventIt: " << eventIt << "\n";
        // Skip event header lines
        if (line.find("Event") != std::string::npos && eventIt == (event + 1)) {
            break;
        }
        if (line.find("Event") != std::string::npos) {
            eventIt++;
            continue;
        }

        if (line.find("Event") != std::string::npos) {
            std::stringstream ss(line);
            std::string dummy, event_label;
            ss >> dummy >> event_label >> current_event;

            if current_event == event{
                continue;
            }

            std::cout << "Found Event: " << current_event << std::endl;
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
            //std::cout << "Et: " << et
            //          << ", Phi: " << phi
            //          << ", Eta: " << eta << std::endl;

            // Store in vector
            values.push_back({et, eta, phi});
        } catch (const std::exception& e) {
            std::cerr << "Error processing line: " << line << " -> " << e.what() << std::endl;
        }
    }

    inFile.close();
    return;
}
*/
// new version for all events
void extract_values_from_file(const std::string& fileName, std::vector<std::vector<std::array<double, 3> > >& values) {

    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }

    // Event iteration used for indexing
    int iEvt = -1; 
    //unsigned int lineIt = -1;

    std::string line;
    std::vector<std::array<double, 3> > valuesForEachEvent; 
    while (std::getline(inFile, line)) {
        //lineIt++; 

        // Skip event header lines
        if (line.find("Event") != std::string::npos) {
            if (iEvt >= 0){
                values.push_back(valuesForEachEvent);
                valuesForEachEvent.clear();
            }
            iEvt++;
            continue;
        }
        if (iEvt > maxEvent_) break;
        
        std::stringstream ss(line);
        std::string index, bin, hex_word;
        ss >> index >> bin >> hex_word;

        //std::cout << "index: " << index << " binary : " << bin << " hex_word: " << hex_word << "\n";

        // Validate line format
        size_t first_pipe = bin.find('|');
        size_t second_pipe = bin.rfind('|');
        if (first_pipe == std::string::npos || second_pipe == std::string::npos || first_pipe == second_pipe) {
            std::cerr << "Error: Malformed line -> " << line << std::endl;
            continue;
        }

        // Extract binary substrings
        std::string et_bin = bin.substr(0, first_pipe);
        std::string eta_bin = bin.substr(first_pipe + 1, second_pipe - first_pipe - 1);
        std::string phi_bin = bin.substr(second_pipe + 1);

        try {
            // Convert binary to bitsets
            std::bitset<et_bit_length_> et_bits(et_bin);
            std::bitset<eta_bit_length_> eta_bits(eta_bin);
            std::bitset<phi_bit_length_> phi_bits(phi_bin);

            // Convert bitsets to floating-point values
            std::cout << "Et bits: " << et_bits
                      << ", Phi bits: " << phi_bits
                      << ", Eta bits: " << eta_bits << std::endl;
            double et = undigitize_et(et_bits);
            double eta = undigitize_eta(eta_bits);
            double phi = undigitize_phi(phi_bits);
            std::cout << "Et: " << et
                      << ", Phi: " << phi
                      << ", Eta: " << eta << std::endl;

            // Store in vector
            valuesForEachEvent.push_back({et, eta, phi});
        } catch (const std::exception& e) {
            std::cerr << "Error processing line: " << line << " -> " << e.what() << std::endl;
        }
    }

    inFile.close();
    return;
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

// Main functon
void process_file(const std::string& fileName){ // FIXME use event loop outside of this, that way function is called each event 
    // Open input file
    const std::string eosPath = "/eos/user/m/mlarson/LargeRadiusJets/MemPrints/";
    const std::string gFexFile = eosPath + "gFex/" + fileName + "_gfex_smallrj.dat";
    const std::string topoFile = eosPath + "CaloTopo_422/" + fileName + "_topo422.dat";
    const std::string outputJetsFile = eosPath + "largeRJets/" + fileName + "_largeR.dat";
    std::cout << "gFex File: " << gFexFile << " and topo file: " << topoFile << "\n";

    std::ofstream outFile(outputJetsFile);
    if (!outFile) {
        std::cerr << "Error: Could not open file " << outputJetsFile << std::endl;
        return;
    }

    std::vector<std::vector<std::array<double, 3> > > gFexValues;
    std::vector<std::vector<std::array<double, 3> > > topoValues;
    std::vector<std::vector<std::array<double, 3> > > outputJetValues;

    extract_values_from_file(gFexFile, gFexValues);
    extract_values_from_file(topoFile, topoValues);

    for (unsigned int iEvt = 0; iEvt < maxEvent_; iEvt++){
        outFile << "Event : " << iEvt << std::endl;
        std::cout << "processing iEvt: " << iEvt << "\n";
        if (iterative_){
            if (sortSeeds_){
                // perform sorting by Et here
                std::sort(gFexValues[iEvt].begin(), gFexValues[iEvt].end(),
                    [](const std::array<double, 3>& a, const std::array<double, 3>& b) {
                        return a[0] > b[0]; // Descending order
                    });
                for (unsigned int igFex = 0; igFex < nSeeds_; ++igFex){
                    for (unsigned int iTopo = 0; iTopo < maxObjectsConsidered_; ++iTopo){
                        
                    }
                }
            }
            else {
                for (unsigned int igFex = 0; igFex < nSeeds_; ++igFex){
                    for (unsigned int iTopo = 0; iTopo < maxObjectsConsidered_; ++iTopo){
                        
                    }
                }
            }
        }
        else {
            
            if (sortSeeds_){
                // perform sorting by Et here
                if (iEvt < gFexValues.size()) {
                    // Sort the vector of arrays for the specific event (iEvt)
                    std::sort(gFexValues[iEvt].begin(), gFexValues[iEvt].end(),
                              [](const std::array<double, 3>& a, const std::array<double, 3>& b) {
                                  return a[0] > b[0]; // Sort by highest Et (first element of the array)
                              });
                } else {
                    std::cerr << "Error: iEvt out of bounds. iEvt = " << iEvt << ", gFexValues.size() = " << gFexValues.size() << std::endl;
                }
                
                for (unsigned int igFex = 0; igFex < nSeeds_; ++igFex){
                    double outputJetEt = gFexValues[iEvt][igFex][0]; // reset outputjet values for each seed, to values of seed
                    double outputJetEta = gFexValues[iEvt][igFex][1]; 
                    double outputJetPhi = gFexValues[iEvt][igFex][2]; 
                    //std::cout << "seed Et, Eta, phi: " << gFexValues[iEvt][igFex][0] << " , " << gFexValues[iEvt][igFex][1] << " , " << gFexValues[iEvt][igFex][2] << "\n";
                    for (unsigned int iTopo = 0; iTopo < topoValues[iEvt].size() && iTopo < maxObjectsConsidered_; ++iTopo){
                        //std::cout << "topo Et, Eta, phi: " << topoValues[iEvt][iTopo][0] << " , " << topoValues[iEvt][iTopo][1] << " , " << topoValues[iEvt][iTopo][2] << "\n";
                        double dR2 = calcDeltaR2(gFexValues[iEvt][igFex][1], gFexValues[iEvt][igFex][2], topoValues[iEvt][iTopo][1], topoValues[iEvt][iTopo][2]);
                        //std::cout << "deltaR2: " << dR2 << "\n";
                        if (dR2 <= r2Cut_){
                            outputJetEt += topoValues[iEvt][iTopo][0];
                            // Erase the element at iTopo from topoValues
                            topoValues[iEvt].erase(topoValues[iEvt].begin() + iTopo);

                            // Adjust the loop index since the vector size has decreased
                            --iTopo;
                        }
                    }
                    std::cout << "outputjetEt : " << outputJetEt << " original seed Et: " << gFexValues[iEvt][igFex][0] << "\n";
                    std::cout << "outputJetEta: " << outputJetEta << " outputJetPhi: " << outputJetPhi << "\n";

                    // Convert to binary format (assuming fixed lengths)
                    std::pair<std::string, unsigned int> et_bin = digitize<et_bit_length_>(outputJetEt, et_min_, et_max_); // Example: 13 bits for Et
                    std::pair<std::string, unsigned int> eta_bin = digitize<eta_bit_length_>(outputJetEta, eta_min_, eta_max_); // Example: 11 bits for eta
                    std::pair<std::string, unsigned int> phi_bin = digitize<phi_bit_length_>(outputJetPhi, phi_min_, phi_max_); // Example: 8 bits for phi

                    std::cout << "et_int: " << et_bin.second << " eta_int: " << eta_bin.second << " phi_int: " << phi_bin.second << "\n";

                    int combined_value = (et_bin.second << (eta_bit_length_ + phi_bit_length_)) | (eta_bin.second << phi_bit_length_) | phi_bin.second;
                    
                    // Convert to hexadecimal (for the last field)
                    std::stringstream hex_stream;
                    hex_stream << std::setw(8) << std::setfill('0') << std::hex << combined_value;
                    std::string hexValue = hex_stream.str();

                    // Output in the required format
                    outFile << "0x" << std::setw(2) << std::setfill('0') << std::hex << igFex
                    << " " << et_bin.first << "|" << eta_bin.first << "|" << phi_bin.first
                    << " 0x" << hexValue << std::endl;
                }
            }
            else {
                for (unsigned int igFex = 0; igFex < nSeeds_; ++igFex){
                    for (unsigned int iTopo = 0; iTopo < maxObjectsConsidered_; ++iTopo){
                        
                    }
                }
            }
        }
            


    }

}

// Calls main function with global fileName_ variable 
void callEventLoop(const std::string& fileName = fileName_){
    redirect_output_to_file();
    process_file(fileName);
    fclose(stdout);
    return;
}