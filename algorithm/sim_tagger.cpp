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
#include <cmath>
#include "sim_tagger.h"
#include "helperFunctions.h"
#include <ap_int.h>

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
void extract_values_from_file(const std::string& fileName, std::vector<std::array<double, 3> >& values, unsigned int eventToProcess) {

    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }

    // Event iteration used for indexing
    int iEvt = -1; 
    //unsigned int lineIt = -1;

    std::string line;
    std::vector<std::array<double, 3> > valuesForEvent; 
    while (std::getline(inFile, line)) {
        //lineIt++; 
        int eventNumber;
        
        // Skip event header lines
        if (line.find("Event") != std::string::npos) {
            std::stringstream ss0(line);
            std::string temp;
            
            
            ss0 >> temp >> temp >> eventNumber;  // "Event :" is skipped, eventNumber is read
            //std::cout << "Event found: " << eventNumber << "and eventToProcess: " << eventToProcess << std::endl;
            if (iEvt >= 0 && iEvt == eventToProcess){
                //std::cout << "iEvt: " << iEvt << "\n";
                //std::cout << "event being processed: " << iEvt << "\n";
                values = valuesForEvent;
            }
            iEvt++;
            valuesForEvent.clear();
            continue;
        }
        if (iEvt > eventToProcess) break;
        if (eventNumber == eventToProcess){
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

            //try {
            // Convert binary to bitsets
            std::bitset<et_bit_length_> et_bits(et_bin);
            std::bitset<eta_bit_length_> eta_bits(eta_bin);
            std::bitset<phi_bit_length_> phi_bits(phi_bin);

            // Convert bitsets to floating-point values
            //std::cout << "Et bits: " << et_bits
            //          << ", Phi bits: " << phi_bits
            //          << ", Eta bits: " << eta_bits << std::endl;
            double et = undigitize_et(et_bits);
            double eta = undigitize_eta(eta_bits); // FIXME use digitized values in calculations, don't convert to double
            double phi = undigitize_phi(phi_bits);
            //std::cout << "Et: " << et
            //          << ", Phi: " << phi
            //          << ", Eta: " << eta << std::endl;

            // Store in vector
            //std::cout << "et " << et << "\n";
            valuesForEvent.push_back({et, eta, phi});
            //} 
            /*catch (const std::exception& e) {
                std::cerr << "Error processing line: " << line << " -> " << e.what() << std::endl;
            }*/
        }
        
    }
    if (maxEvent_ == 1000){
        values = valuesForEvent;
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
void process_event(const std::string& fileName, unsigned int eventToProcess, std::ofstream& outFile){ // FIXME use event loop outside of this, that way function is called each event 
    // Open input file
    const std::string gFexFile = memPrintsPath_ + "gFex/" + fileName + "_gfex_smallrj.dat";
    const std::string topoFile = memPrintsPath_ + "CaloTopo_422/" + fileName + "_topo422.dat";
    
    //std::cout << "gFex File: " << gFexFile << " and topo file: " << topoFile << "\n";

    std::vector<std::array<double, 3> > seedValues;
    std::vector<std::array<double, 3> > inputObjectValues;
    std::vector<std::array<double, 3> > outputJetValues;

    extract_values_from_file(gFexFile, seedValues, eventToProcess);
    extract_values_from_file(topoFile, inputObjectValues, eventToProcess);

    outFile << "Event : " << std::dec << eventToProcess << std::endl;
    //std::cout << "processing iEvt: " << eventToProcess << "\n";
    // perform sorting by Et here
    // Sort the vector of arrays for the specific event (iEvt)
    /*std::sort(seedValues.begin(), seedValues.end(), // FIXME possibly replace with insertion sort (note: O(n^2)) - consider instead looping through and only considering input objects with minimum energy
                [](const std::array<double, 3>& a, const std::array<double, 3>& b) {
                    return a[0] > b[0]; // Sort by highest Et (first element of the array)
                });*/

    
    for (unsigned int iSeed = 0; iSeed < nSeeds_; ++iSeed){
        //std::cout << "size of seed values: " << seedValues[iSeed].size() << "\n";
        double outputJetEt = seedValues[iSeed][0]; // reset outputjet values for each seed, to values of seed
        double outputJetEta = seedValues[iSeed][1]; 
        double outputJetPhi = seedValues[iSeed][2]; 
        //std::cout << "outputJetEt: " << outputJetEt << " and outputJetEta: " << outputJetEta << " and outputJetPhi: " << outputJetPhi << "\n";
        //std::cout << "seed Et, Eta, phi: " << seedValues[iSeed][0] << " , " << seedValues[iSeed][1] << " , " << seedValues[iSeed][2] << "\n";
        //std::cout << "inputObjectValues.size(): " << inputObjectValues.size() << "\n";
        for (unsigned int iInput = 0; iInput < inputObjectValues.size() && iInput < maxObjectsConsidered_; ++iInput){
            //std::cout << "topo Et, Eta, phi: " << inputObjectValues[iInput][0] << " , " << inputObjectValues[iInput][1] << " , " << inputObjectValues[iInput][2] << "\n";
            double dR2 = calcDeltaR2(seedValues[iSeed][1], seedValues[iSeed][2], inputObjectValues[iInput][1], inputObjectValues[iInput][2]);
            //std::cout << "deltaR2: " << dR2 << "\n";
            if (dR2 <= r2Cut_){
                //std::cout << "iInput: "<< iInput << "and inputObjectValues.size(): " <<  inputObjectValues.size() << "\n";
                outputJetEt += inputObjectValues[iInput][0];
                // Erase the element at iInput from inputObjectValues
                //std::cout << "iINput: " << iInput << " and inputObjectValues.begin() + iInput: " << inputObjectValues.begin() + iInput << " and inputObjectValues.size(): " <<inputObjectValues.size() << "\n";
                
                inputObjectValues.erase(inputObjectValues.begin() + iInput);

                // Adjust the loop index since the vector size has decreased
                --iInput;
            }
        }
        //std::cout << "outputjetEt : " << outputJetEt << " original seed Et: " << seedValues[iSeed][0] << "\n";
        //std::cout << "outputJetEta: " << outputJetEta << " outputJetPhi: " << outputJetPhi << "\n";
        //std::cout << "outputJetEt: " << outputJetEt << " outputJetEta: " << outputJetEta << "outputJetPhi: " << outputJetPhi << "\n";
        // Convert to binary format (assuming fixed lengths)
        std::pair<std::string, unsigned int> et_bin = digitize<et_bit_length_>(outputJetEt, et_min_, et_max_); // Example: 13 bits for Et
        std::pair<std::string, unsigned int> eta_bin = digitize<eta_bit_length_>(outputJetEta, eta_min_, eta_max_); // Example: 11 bits for eta
        std::pair<std::string, unsigned int> phi_bin = digitize<phi_bit_length_>(outputJetPhi, phi_min_, phi_max_); // Example: 8 bits for phi

        //std::cout << "et_int: " << et_bin.second << " eta_int: " << eta_bin.second << " phi_int: " << phi_bin.second << "\n";
        int combined_value = (et_bin.second << (eta_bit_length_ + phi_bit_length_)) | (eta_bin.second << phi_bit_length_) | phi_bin.second;
        // Convert to hexadecimal (for the last field)
        std::stringstream hex_stream;
        hex_stream << std::setw(8) << std::setfill('0') << std::hex << combined_value;
        std::string hexValue = hex_stream.str();
        // Output in the required format
        outFile << "0x" << std::setw(2) << std::setfill('0') << std::hex << iSeed
        << " " << et_bin.first << "|" << eta_bin.first << "|" << phi_bin.first
        << " 0x" << hexValue << std::endl; // FIXME can't use in hls
        //char formattedString[64];  // Allocate enough space
        //ap_uint<et_bit_length_ > et_value = et_bin.first - '0';
        //ap_uint<et_bit_length_ > et_value = (int)strtol(et_bin.first, NULL, 2);
        //ap_uint<eta_bit_length_ > eta_value = eta_bin.first - '0';
        //ap_uint<eta_bit_length_ > eta_value = (int)strtol(eta_bin.first, NULL, 2);
        //ap_uint<phi_bit_length_ > phi_value = phi_bin.first - '0';
        //ap_uint<phi_bit_length_ > phi_value = (int)strtol(phi_bin.first, NULL, 2);
        //format_output(formattedString, sizeof(formattedString), iSeed, et_value, eta_value, phi_value, hexValue);
        //outFile << formattedString << std::endl; // FIXME don't write out file for actual implementation, only C sim, use precompiler directive
    }
    //seedValues.clear();
    //inputObjectValues.clear();
    //outputJetValues.clear();

}

// Calls main function with global fileName_ variable 
void callEventLoop(const char* fileName){
    //redirect_output_to_file();
    const std::string outputJetsFile = memPrintsPath_ + "largeRJets/" + fileName + "_largeR.dat";
    std::ofstream outFile(outputJetsFile);
    if (!outFile) {
        std::cerr << "Error: Could not open file " << outputJetsFile << std::endl;
        return;
    }
    for (unsigned int iEvt = 0; iEvt < maxEvent_; iEvt++){
        process_event(fileName, iEvt, outFile);
    }   
    //fclose(stdout);
    return;
}