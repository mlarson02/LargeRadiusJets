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
const std::string memPrintsPath_ = "/home/larsonma/LargeRadiusJets/data/MemPrints/";
const std::string lutPath_ = "/home/larsonma/LargeRadiusJets/data/LUTs/deltaR2Cut.dat";
const std::string kFileSuffix = "nSeeds2_r2Cut0p64_maxObj128_back_noecut_ecutVal4";
constexpr bool signalBool_ = false;

const unsigned int maxEvent_ = signalBool_ ? 3900 : 5000;
const std::string fileName_ = signalBool_ ? "mc21_14TeV_hh_bbbb_vbf_novhh" : "mc21_14TeV_jj_JZ3";


void sortByEt(input seedValues[nTotalSeeds_]) {
    for (int i = 0; i < nTotalSeeds_ - 1; ++i) {
        for (int j = 0; j < nTotalSeeds_ - i - 1; ++j) {
            ap_uint<et_bit_length_> et1 = seedValues[j].range(et_high_, et_low_);
            ap_uint<et_bit_length_> et2 = seedValues[j + 1].range(et_high_, et_low_);
            if (et1 < et2) { // Descending sort
                input temp = seedValues[j];
                seedValues[j] = seedValues[j + 1];
                seedValues[j + 1] = temp;
            }
        }
    }
}

// read values from .dat files for a provided event
template <unsigned int arraySize >
inline void extract_values_from_file(const std::string& fileName, input (&values)[arraySize], unsigned int eventToProcess) {

    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }

    int iEvt = -1; 
    std::string line;
    input valuesForEvent[arraySize]; 
    int lastEvent = 0; 
    int objectIt = -1;
    int eventNumber = -1;
    while (std::getline(inFile, line)) {
        if (line.find("Event") != std::string::npos) {
            std::stringstream ss0(line);
            std::string temp;
            ss0 >> temp >> temp >> eventNumber;
            if (iEvt >= 0 && iEvt == eventToProcess){
                for (unsigned int i = 0; i < arraySize; ++i) {
                    values[i] = valuesForEvent[i];
                }
            }
            iEvt++;
            input zero = 0;
            std::fill(std::begin(valuesForEvent), std::end(valuesForEvent), zero);
            continue;
        }
        if (iEvt > eventToProcess) break;
        if (eventNumber == eventToProcess){
            if (lastEvent != eventToProcess) objectIt = -1;
            lastEvent = eventToProcess;
            objectIt++; 
            
            std::stringstream ss(line);
            std::string index, bin, hex_word;
            ss >> index >> bin >> hex_word;

            size_t first_pipe = bin.find('|');
            size_t second_pipe = bin.rfind('|');
            if (first_pipe == std::string::npos || second_pipe == std::string::npos || first_pipe == second_pipe) {
                std::cerr << "Error: Malformed line -> " << line << std::endl;
                continue;
            }

            std::string et_bin  = bin.substr(0, first_pipe);
            std::string eta_bin = bin.substr(first_pipe + 1, second_pipe - first_pipe - 1);
            std::string phi_bin = bin.substr(second_pipe + 1);

            // Prepend 5 zero bits (as MSB) to represent num_io = 0
            std::string num_io_bin = "00000";

            // Final bitstring in MSB to LSB order: num_io | et | eta | phi
            std::string full_bin = phi_bin + eta_bin + et_bin + num_io_bin;

            //std::cout << "full_bin: " << full_bin << std::endl;

            input fullInput = ap_uint<input::width>(std::stoull(full_bin, nullptr, 2));


            if (objectIt >= arraySize) continue;
            valuesForEvent[objectIt]  = fullInput;
        }
    }
    if (eventToProcess == maxEvent_){
        for (unsigned int i = 0; i < arraySize; ++i) {
            values[i] = valuesForEvent[i];
        }
    }

    inFile.close();
    return;
}

#endif
