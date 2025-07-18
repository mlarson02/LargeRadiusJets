from pathlib import Path
#import vitis
import os
import shutil
import math
import ROOT
import xml.etree.ElementTree as ET
import subprocess

# Base/default constants
base_constants = {
    "sortSeeds_": "true",
    "iterative_": "false",
    "nTotalSeeds_": 200,
    "nSeedsInput_": 6,
    "nSeedsOutput_": 2,
    "maxObjectsConsidered_": 1024,
    "inputEnergyCut_": 1,
    "useInputEnergyCut_": "false", 
    "et_granularity_": 0.25,
    "r2Cut_": 1.0,
    "rMergeCut_": 2.5,
    "et_bit_length_": 13,
    "eta_bit_length_": 8,
    "phi_bit_length_": 6,
    "io_bit_length_": 5, 
    "deltaRBits_": 8,
    "phi_min_": -3.2,
    "phi_max_": 3.2,
    "pi_digitized_in_phi_": 31,
    "eta_min_": -5.0,
    "eta_max_": 5.0,
    "eta_granularity_": 0.0390625,
    "phi_granularity_": 0.1,
    "et_min_": 0,
    "et_max_": 2048,
    "useMax_": "false",
    "max_R2lut_size_": 2048,
    "max_Rlut_size_": 2048
}

def calculate_lutR2_max_size(r2Cut, eta_bit_length, phi_bit_length, eta_granularity, phi_granularity):
    last_one_index = 0
    idx = 0
    #print("r2cut used in calculating max lut size: ", r2Cut)
    for etaIt in range(1 << eta_bit_length):
        for phiIt in range(1 << (phi_bit_length - 1)):
            etaSquared = (etaIt * eta_granularity) ** 2
            phiSquared = (phiIt * phi_granularity) ** 2
            #print("etaSquared:", etaSquared)
            #print("phiSquared:", phiSquared)
            
            deltaR2 = etaSquared + phiSquared
            #print("deltaR2:", deltaR2)

            if deltaR2 < r2Cut:
                last_one_index = idx
            idx += 1
            #print("last_one_index:", last_one_index)
    lut_max_size = last_one_index + 1
    #print("lut_max_size:", lut_max_size)
    return lut_max_size

def calculate_lutR_max_size(rCut, eta_bit_length, phi_bit_length, eta_granularity, phi_granularity):
    last_one_index = 0
    idx = 0
    #print("r2cut used in calculating max lut size: ", r2Cut)
    for etaIt in range(1 << eta_bit_length):
        for phiIt in range(1 << (phi_bit_length - 1)):
            etaSquared = (etaIt * eta_granularity) ** 2
            phiSquared = (phiIt * phi_granularity) ** 2
            #print("etaSquared:", etaSquared)
            #print("phiSquared:", phiSquared)
            
            deltaR = math.sqrt(etaSquared + phiSquared)
            #print("deltaR2:", deltaR2)

            if deltaR < rCut:
                last_one_index = idx
            idx += 1
            #print("last_one_index:", last_one_index)
    
    lut_max_size = last_one_index + 1
    #print("lut_max_size:", lut_max_size)
    return lut_max_size

def compute_derived(constants):
    constants["phi_range_"] = constants["phi_max_"] - constants["phi_min_"]
    return constants

def run_lut_generator_via_root(source_file: str):
    ROOT.gSystem.Load("libCore")  # Make sure libCore is loaded
    ROOT.gROOT.ProcessLine(f'.L {source_file}')
    ROOT.gROOT.ProcessLine('main();')  # Assuming your main is called "main"

def extract_hls_report(xml_path):
    if not os.path.exists(xml_path):
        raise FileNotFoundError(f"Report file not found: {xml_path}")

    tree = ET.parse(xml_path)
    root = tree.getroot()

    resources = {}
    latency = {}

    # Extract Resource Utilization
    for summary in root.findall(".//SummaryOfResources"):
        resources['LUT'] = int(summary.findtext('LUT', default='0'))
        resources['FF'] = int(summary.findtext('FF', default='0'))
        resources['BRAM_18K'] = int(summary.findtext('BRAM_18K', default='0'))
        resources['DSP48E'] = int(summary.findtext('DSP48E', default='0'))

    # Extract Latency
    for timing in root.findall(".//SummaryOfTiming/Latency"):
        latency['BestCaseCycles'] = int(timing.findtext('Best-case', default='0'))
        latency['WorstCaseCycles'] = int(timing.findtext('Worst-case', default='0'))

    return resources, latency


import os

def write_file_read_header(file_path, file_suffix, signal_bool):
    # Remove existing constants.h if it exists
    if os.path.exists(file_path):
        os.remove(file_path)
    # Base content up to lutPath_
    header_content = """#ifndef FILE_READ_H  // Check if the macro is defined
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
"""

    # Add your file suffix line *after* lutPath_
    suffix_line = f'const std::string kFileSuffix = "{file_suffix}";\n'
    # Now create the signalBool_ line dynamically
    signal_bool_line = f'constexpr bool signalBool_ = {"true" if signal_bool else "false"};\n'

    # Now continue with the rest of the file content
    remaining_content = """
const unsigned int maxEvent_ = signalBool_ ? 3300 : 5000;
const std::string fileName_ = signalBool_ ? "mc21_14TeV_hh_bbbb_vbf_novhh" : "mc21_14TeV_jj_JZ3";


void sortByEt(input seedValues[nTotalSeeds_], input sortedSeedValues[nSeedsInput_]) {
    //std::cout << "SORTING BY ET!" << std::endl;
    for (int i = 0; i < nTotalSeeds_ - 1; ++i) {
        //std::cout << "i: " << i << std::endl;
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
    for (int j = 0; j < nSeedsInput_; ++j){
        //std::cout << "seedValues[j]: " << std::hex << seedValues[j] << std::endl;
        //std::cout << "after sort j: " << std::dec << j << std::endl;
        sortedSeedValues[j] = seedValues[j];
        //std::cout << "sortedSeedValues[j] : " << std::hex << sortedSeedValues[j] << std::endl;
        //std::cout << "AFTER ASSIGNING VALUES j: " << std::dec << j << std::endl;
    }
    //std::cout << "exiting???? " << std::endl;
    return;
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
"""

    # Now write everything to file
    with open(file_path, 'w') as f:
        f.write(header_content)
        f.write(suffix_line)
        f.write(signal_bool_line)
        f.write(remaining_content)

    print(f"File {file_path} written successfully with the file suffix.")




def write_constants_h(constants: dict, output_file: str, unroll: int, ii: int):
    # Remove existing constants.h if it exists
    if os.path.exists(output_file):
        os.remove(output_file)

    with open(output_file, "w") as f:
        f.write("#ifndef CONSTANTS_H\n")
        f.write("#define CONSTANTS_H\n")
        f.write("// Constants used by SW & FW implementation\n\n")

        # Write UNROLLFACTOR and PIPELINEII
        f.write(f"#define UNROLLFACTOR {unroll}\n")
        f.write(f"#define PIPELINEII {ii}\n\n")


        # Write all constants
        for key, value in constants.items():
            if isinstance(value, bool) or value in ["true", "false"]:
                f.write(f"#define {key} {'true' if str(value).lower() == 'true' else 'false'}\n")
            elif isinstance(value, int):
                f.write(f"constexpr unsigned int {key} = {value};\n")
            elif isinstance(value, float):
                f.write(f"constexpr double {key} = {value};\n")
            else:
                f.write(f"constexpr auto {key} = {value};\n")

        # Write the extra constants, LUT include, and struct
        f.write('''


const unsigned int lut_size_ = (1 << (eta_bit_length_ + phi_bit_length_));
#if !WRITE_LUT

constexpr unsigned int total_bits_ = io_bit_length_ + et_bit_length_ + eta_bit_length_ + phi_bit_length_;
typedef ap_uint<total_bits_> input;

constexpr unsigned int io_low_ = 0; 
constexpr unsigned int io_high_ = io_bit_length_ - 1; 
constexpr unsigned int et_low_ = io_high_ + 1;
constexpr unsigned int et_high_ = et_low_ + et_bit_length_ - 1; 
constexpr unsigned int eta_low_ = et_high_ + 1; 
constexpr unsigned int eta_high_ = eta_low_ + eta_bit_length_ - 1; 
constexpr unsigned int phi_low_ = eta_high_ + 1; 
constexpr unsigned int phi_high_ = phi_low_ + phi_bit_length_ - 1; 

constexpr unsigned int nSeedsDeltaR_ = nSeedsInput_ - nSeedsOutput_;

static const bool lut_[max_R2lut_size_] =
#include "../data/LUTs/deltaR2Cut.h"
;

static const ap_uint<deltaRBits_ > lutR_[max_Rlut_size_] = 
#include "../data/LUTs/deltaR.h"
;

#endif
        ''')

        f.write("\n#endif // CONSTANTS_H\n")


if __name__ == "__main__":
    # Sweep over combinations
    #nSeeds_options = [1, 2, 3, 4, 5, 6, 7, 8]
    nSeeds_options = [2]
    #r2Cut_options = [0.8, 0.9, 1.0, 1.1, 1.2]
    #r2Cut_options = [1.0, 1.21, 1.44, 1.69, 1.96]
    #r2Cut_options = [1.69]
    r2Cut_options = [1.0]
    #maxObjectsConsidered_options = [128, 256, 512, 1024]
    #maxObjectsConsidered_options = [128, 256]
    maxObjectsConsidered_options = [128]
    #sortSeeds_options = [False, True]
    sortSeeds_options = [False] # NOW ALWAYS TRUE UNDER ASSUMPTION THAT GFEX IS DOING SORTING
    signalBool_options = [True, False]
    #inputEnergyCut_options = [False]
    inputEnergyCut_options = [False]
    #inputEnergyCutValues_options = [0.5, 1.0, 1.5, 2.0] # only use these if inputEnergyCutOption is true
    inputEnergyCutValues_options = [4] # note 4 == 1 GeV (0.25 GeV granularity for inputobjectvalues)
    for nSeeds in nSeeds_options:
        for r2Cut in r2Cut_options:
            for maxObjectsConsidered in maxObjectsConsidered_options:
                for sortSeeds in sortSeeds_options:
                    for inputEnergyCutBool in inputEnergyCut_options:
                        for inputEnergyCutValues in inputEnergyCutValues_options:
                            for signalBool in signalBool_options:
                                # Copy base constants
                                constants = base_constants.copy()
                                constants["nSeeds_"] = nSeeds
                                constants["r2Cut_"] = r2Cut
                                constants["maxObjectsConsidered_"] = maxObjectsConsidered
                                constants["sortSeeds_"] = sortSeeds
                                constants["useInputEnergyCut_"] = inputEnergyCutBool
                                if inputEnergyCutBool:
                                    constants["inputEnergyCut_"] = inputEnergyCutValues

                                # Calculate phi and eta granularities from base constants
                                phi_range = base_constants["phi_max_"] - base_constants["phi_min_"]
                                phi_granularity = phi_range / (1 << base_constants["phi_bit_length_"])
                                constants["phi_granularity_"] = phi_granularity

                                eta_range = base_constants["eta_max_"] - base_constants["eta_min_"]
                                eta_granularity = eta_range / (1 << base_constants["eta_bit_length_"])
                                constants["eta_granularity_"] = eta_granularity

                                lut_max_size = calculate_lutR2_max_size(
                                    r2Cut=constants["r2Cut_"],
                                    eta_bit_length=constants["eta_bit_length_"],
                                    phi_bit_length=constants["phi_bit_length_"],
                                    eta_granularity=eta_granularity,
                                    phi_granularity=phi_granularity
                                )

                                lutR_max_size = calculate_lutR_max_size(
                                    rCut=constants["rMergeCut_"],
                                    eta_bit_length=constants["eta_bit_length_"],
                                    phi_bit_length=constants["phi_bit_length_"],
                                    eta_granularity=eta_granularity,
                                    phi_granularity=phi_granularity
                                )

                                constants["max_R2lut_size_"] = lut_max_size  # Add it to constants.h!
                                constants["max_Rlut_size_"] = lutR_max_size  # Add it to constants.h!

                                # Update derived values
                                constants = compute_derived(constants)

                                # Clean up float formatting for r2Cut and inputEnergyCutValues
                                r2Cut_str = str(r2Cut).replace('.', 'p')
                                energyCut_str = str(inputEnergyCutValues).replace('.', 'p')

                                # Bool formatting for sortSeeds and inputEnergyCut
                                signal_str = 'sig' if signalBool else 'back'
                                energyCutBool_str = 'ecut' if inputEnergyCutBool else 'noecut'
                                constsFilename = f"../algorithm/constants.h"

                                # File name based on parameter values
                                file_suffix = (
                                    f"nSeeds{nSeeds}_"
                                    f"r2Cut{r2Cut_str}_"
                                    f"maxObj{maxObjectsConsidered}_"
                                    f"{signal_str}_"
                                    f"{energyCutBool_str}_"
                                    f"ecutVal{energyCut_str}"
                                )

                                unroll = int(maxObjectsConsidered/8)

                                ii = 3

                                # Write to file
                                write_constants_h(constants, constsFilename, unroll, ii)
                                fileReadPath = "/home/larsonma/LargeRadiusJets/algorithm/fileRead.h"  # Path to save the file
                                write_file_read_header(fileReadPath, file_suffix, signalBool)

                                print(f" Wrote {constsFilename}")
                                run_lut_generator_via_root("/home/larsonma/LargeRadiusJets/algorithm/writeDeltaR2LUT.cc")
                                print(f"Launching HLS with project name: {file_suffix}")
                                subprocess.run(["vitis", "-s", "jet_tagger_hls.py", file_suffix, "1"], check=True)
                                xml_report_path = os.path.join('w', file_suffix, file_suffix, 'syn', 'report', 'jet_tagger_top_csynth.xml')
                                print("xml_report_path,", xml_report_path)
                                #resources, latency = extract_hls_report(xml_report_path)
                                #print(f"Run {file_suffix}: Resources {resources}, Latency {latency}")
                            
