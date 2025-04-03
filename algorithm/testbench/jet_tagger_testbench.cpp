#include "/home/larsonma/LargeRadiusJets/algorithm/jet_tagger_top.h"
//#include "/home/larsonma/LargeRadiusJets/algorithm/helperFunctions.h"
#include <string>
#include <iostream>
#include <array>
#include <vector>

int main() {
    std::cout << "Starting test bench..." << std::endl;
    std::array<std::array<double, 3>, nTotalSeeds_ > seedValues; // 3 is # of datatypes (Et, eta, phi)
    std::array<std::array<double, 3>, maxObjectsConsidered_ > inputObjectValues;
   // std::vector<std::array<double, 3> > seedValues;
    //std::vector<std::array<double, 3> > inputObjectValues
    //std::vector<std::array<double, 3> > outputJetValues;
    std::array<std::bitset<et_bit_length_ >, nSeeds_ > outputJetEtValues;
    std::array<std::bitset<eta_bit_length_ >, nSeeds_ > outputJetEtaValues;
    std::array<std::bitset<phi_bit_length_ >, nSeeds_> outputJetPhiValues;
    const bool signalBool = true;
    const std::string fileName = signalBool ? "mc21_14TeV_hh_bbbb_vbf_novhh" : "mc21_14TeV_jj_JZ3";
    const std::string seedFile = memPrintsPath_ + "gFex/" + fileName + "_gfex_smallrj.dat";
    const std::string inputObjectFile = memPrintsPath_ + "CaloTopo_422/" + fileName + "_topo422.dat";
    
    // Call the function under test

    const std::string outputJetsFile = memPrintsPath_ + "largeRJets/" + fileName + "_largeR.dat";
    std::ofstream outFile(outputJetsFile);
    if (!outFile) {
        std::cerr << "Error: Could not open file " << outputJetsFile << std::endl;
        return 0;
    }
    //std::array<std::array<double, 3>, maxObjectsConsidered_ > seedValuesArray; // 3 is # of datatypes (Et, eta, phi)
        //std::array<std::array<double, 3>, maxObjectsConsidered_ > inputObjectValuesArray;
        //for (unsigned int i = 0; i < maxObjectsConsidered_; i++){ // FIXME add option to use 
        //    seedValuesArray[]
        //}
    for (unsigned int iEvt = 0; iEvt < maxEvent_; iEvt++){
        outFile << "Event : " << std::dec << iEvt << std::endl;
        extract_values_from_file<nTotalSeeds_ >(seedFile, seedValues, iEvt);
        std::cout << "after extract 1" << std::endl;
        extract_values_from_file<maxObjectsConsidered_ >(inputObjectFile, inputObjectValues, iEvt);
        std::cout << "after extract 2" << std::endl;
        jet_tagger_top(seedValues, inputObjectValues, outputJetEtValues, outputJetEtaValues, outputJetPhiValues);
        std::cout << "after extract sim_tagger_top_function" << std::endl;
        for (unsigned int iOutput = 0; iOutput < outputJetEtValues.size(); iOutput++){
            //ap_uint<et_bit_length_> et_apuint(outputJetEtValues[iOutput].to_string());
            //ap_uint<eta_bit_length_> eta_apuint(outputJetEtaValues[iOutput].to_string());
            //ap_uint<phi_bit_length_> phi_apuint(outputJetPhiValues[iOutput].to_string());
            unsigned long long et_value = outputJetEtValues[iOutput].to_ullong();  // Convert to unsigned long long
            unsigned long long eta_value = outputJetEtaValues[iOutput].to_ullong();
            unsigned long long phi_value = outputJetPhiValues[iOutput].to_ullong();
    
            // Convert to ap_uint
            ap_uint<et_bit_length_> et_apuint(et_value);
            ap_uint<eta_bit_length_> eta_apuint(eta_value);
            ap_uint<phi_bit_length_> phi_apuint(phi_value);
            int combined_value = ( et_apuint << (eta_bit_length_ + phi_bit_length_)) | (eta_apuint << phi_bit_length_) | phi_apuint;
            // Convert to hexadecimal (for the last field)
            std::stringstream hex_stream;
            hex_stream << std::setw(8) << std::setfill('0') << std::hex << combined_value;
            std::string hexValue = hex_stream.str();
            // Output in the required format
            
            outFile << "0x" << std::setw(2) << std::setfill('0') << std::hex << iOutput
            << " " << outputJetEtValues[iOutput].to_string() << "|" << outputJetEtaValues[iOutput].to_string() << "|" << outputJetPhiValues[iOutput].to_string()
            << " 0x" << hexValue << std::endl; 
        }
    }   
    
    
    std::cout << "Test bench completed." << std::endl;
    return 0;
}
