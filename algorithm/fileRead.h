
#ifndef FILE_READ_H  // Check if the macro is defined
#define FILE_READ_H  // Define the macro
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <array>
#include <ap_int.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

// Define constants used by testbench

//const std::string memPrintsPath_ = "/eos/user/m/mlarson/LargeRadiusJets/MemPrints/"; // FIXME define pre-compile statement if running on lxplus or millerlabml01
//const std::string fileName_ = "mc21_14TeV_hh_bbbb_vbf_novhh"; 
const std::string memPrintsPath_ = "/home/larsonma/LargeRadiusJets/data/MemPrints/";
const unsigned int maxEvent_ = 1000;
const std::string fileName_ = signalBool_ ? "mc21_14TeV_hh_bbbb_vbf_novhh" : "mc21_14TeV_jj_JZ3";
constexpr bool signalBool_ = false;


// read values from .dat files for a provided event
template <unsigned int arraySize >
inline void extract_values_from_file(const std::string& fileName, std::array<std::array<double, 3>, arraySize >& values, unsigned int eventToProcess) {

    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }

    // Event iteration used for indexing
    int iEvt = -1; 
    //unsigned int lineIt = -1;
    std::string line;
    std::array<std::array<double, 3>, arraySize> valuesForEvent; 
    int lastEvent = 0; 
    int objectIt = -1;
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
                /*for (int i = 0; i < values.size(); i++){
                    std::cout << "values Et: " << values[i][0] << " values Eta: " << values[i][1] << " values Phi: " << values[i][2] << "\n";
                }*/
            }
            iEvt++;
            std::array<double, 3> zeroArray = {0.0, 0.0, 0.0};
            std::fill(std::begin(valuesForEvent), std::end(valuesForEvent), zeroArray);
            continue;
        }
        if (iEvt > eventToProcess) break;
        //std::cout << "eventNumber: " << eventNumber << " and eventToProcess: " << eventToProcess << "\n";
        if (eventNumber == eventToProcess){
            if (lastEvent != eventToProcess) objectIt = -1;
            lastEvent = eventToProcess;
            objectIt++; 
            
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
            /*std::cout << "Et: " << et
                      << ", Phi: " << phi
                      << ", Eta: " << eta << std::endl;*/

            // Store in vector
            //std::cout << "et " << et << "\n";
            valuesForEvent[objectIt]  = {et, eta, phi};
            //} 
            /*catch (const std::exception& e) {
                std::cerr << "Error processing line: " << line << " -> " << e.what() << std::endl;
            }*/
        }
        
    }
    if (eventToProcess == maxEvent_){
        values = valuesForEvent;
    }

    inFile.close();
    return;
}
#endif