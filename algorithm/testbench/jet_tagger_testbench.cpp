#include "/home/larsonma/LargeRadiusJets/algorithm/process_event.h"
#include "/home/larsonma/LargeRadiusJets/algorithm/fileRead.h"
//#include "/home/larsonma/LargeRadiusJets/algorithm/helperFunctions.h"
#include <string>
#include <iostream>
#include <array>
#include <vector>

int main() {

    std::cout << "io_low_: " << io_low_ << " io_high_ " << io_high_ << "\n";
    std::cout << "et_low_: " << et_low_ << " et_high_ " << et_high_ << "\n";
    std::cout << "eta_low_: " << eta_low_ << " eta_high_ " << eta_high_ << "\n";
    std::cout << "phi_low_: " << phi_low_ << " phi_high_ " << phi_high_ << "\n";
    input seedValues[nTotalSeeds_]; // 3 is # of datatypes (Et, eta, phi)
    input inputObjectValues[maxObjectsConsidered_];

    input outputJetValues[nSeeds_];
    unsigned int totalOutputJetMergedIO[nSeeds_]; 
    std::vector<unsigned int > allOutputJetMergedIOLeading; 
    std::vector<unsigned int > allOutputJetMergedIOSubLeading; 
    //const bool signalBool = true;
    //const std::string fileName = signalBool ? "mc21_14TeV_hh_bbbb_vbf_novhh" : "mc21_14TeV_jj_JZ3";
    const std::string seedFile = memPrintsPath_ + "gFex/" + fileName_ + "_gfex_smallrj.dat";
    const std::string inputObjectFile = memPrintsPath_ + "CaloTopo_422/" + fileName_ + "_topo422.dat";
    
    // Call the function under test
    //std::cout << "signalbool : " << signalBool_ << "\n";
    std::string outputJetsFile = memPrintsPath_ + "largeRJetsNoSeedEnergyMerged/" + fileName_ + "_largeR";
    outputJetsFile += kFileSuffix; 
    outputJetsFile += ".dat";
    //std::cout << "outputJetsFile : " << outputJetsFile << "\n";
    std::ofstream outFile(outputJetsFile);
    if (!outFile) {
        std::cerr << "Error: Could not open file " << outputJetsFile << std::endl;
        return 0;
    }

    for (unsigned int iEvt = 0; iEvt < maxEvent_; iEvt++){
        //if (iEvt > 49) break;
        std::cout << "processing event: " << iEvt << "\n";
        for (unsigned int i = 0; i < nSeeds_; ++i) {
            outputJetValues[i] = 0;
        }
        outFile << "Event : " << std::dec << iEvt << std::endl;
        extract_values_from_file<nTotalSeeds_ >(seedFile, seedValues, iEvt);
        //for (unsigned int i = 0; i < nTotalSeeds_; i++){
        //    std::cout << "seedValues.et: " << seedValues[i].et << " seedValues.phi: " << seedValues[i].phi << " and seedValues.eta: " << seedValues[i].eta << "\n";
        //}
        extract_values_from_file<maxObjectsConsidered_ >(inputObjectFile, inputObjectValues, iEvt);
        sortByEt(seedValues);
        for (unsigned int i = 0; i < nTotalSeeds_; i++){
            std::cout << "seedValues et: " << seedValues[i].range(et_high_, et_low_) << "\n";
        }
        process_event(seedValues, inputObjectValues, outputJetValues);
        for (unsigned int iOutput = 0; iOutput < nSeeds_; iOutput++){
            unsigned long long numio_value = outputJetValues[iOutput].range(io_high_, io_low_).to_uint64();
            unsigned long long et_value = outputJetValues[iOutput].range(et_high_, et_low_).to_uint64();  // Convert to unsigned long long
            unsigned long long eta_value = outputJetValues[iOutput].range(eta_high_, eta_low_).to_uint64();
            unsigned long long phi_value = outputJetValues[iOutput].range(phi_high_, phi_low_).to_uint64();
            std::cout << "OUTPUT DATA: " << "\n";
            std::cout << "Et: " << outputJetValues[iOutput].range(et_high_, et_low_) << "\n";
            std::cout << "Et 64: " << outputJetValues[iOutput].range(et_high_, et_low_).to_uint64() << "\n";
            std::cout << "eta: " << outputJetValues[iOutput].range(eta_high_, eta_low_) << "\n";
            std::cout << "eta 64: " << eta_value << "\n";
            //std::cout << "et_value : " << et_value << "\n";
     
            // Convert to ap_uint
            std::bitset<io_bit_length_> io_bitset(numio_value); 
            std::bitset<et_bit_length_ > et_bitset(et_value);
            std::bitset<eta_bit_length_ > eta_bitset(eta_value);
            std::bitset<phi_bit_length_ > phi_bitset(phi_value);
            ap_uint<io_bit_length_> io_apuint(numio_value);
            ap_uint<et_bit_length_> et_apuint(et_value);
            //std::cout << "et_apuint : " << et_apuint << "\n";
            ap_uint<eta_bit_length_> eta_apuint(eta_value);
            ap_uint<phi_bit_length_> phi_apuint(phi_value);
            ap_uint<total_bits_> combined_value = (ap_uint<total_bits_>(io_apuint) << (et_bit_length_ + eta_bit_length_ + phi_bit_length_ ) | ap_uint<et_bit_length_ + eta_bit_length_ + phi_bit_length_ >(et_apuint) << (eta_bit_length_ + phi_bit_length_)) | (ap_uint<et_bit_length_ + eta_bit_length_ + phi_bit_length_ >(eta_apuint) << phi_bit_length_) | phi_apuint;
            // Convert to hexadecimal (for the last field)
            std::stringstream hex_stream;
            hex_stream << std::setw(8) << std::setfill('0') << std::hex << combined_value;
            std::string hexValue = hex_stream.str();

            //std::cout << "et_bitset.to_string() : " << et_bitset.to_string() << "\n";
            // Output in the required format
            outFile << "0x" << std::setw(2) << std::setfill('0') << std::hex << iOutput
            << " " << io_bitset.to_string() << "|" << et_bitset.to_string() << "|" << eta_bitset.to_string() << "|" << phi_bitset.to_string()
            << " 0x" << hexValue << std::endl; 

            totalOutputJetMergedIO[iOutput] += numio_value; 
            if (iOutput == 0) allOutputJetMergedIOLeading.push_back(numio_value);
            if (iOutput == 1) allOutputJetMergedIOSubLeading.push_back(numio_value);
        }
    }   

    std::cout << "average merged IO for leading seed: " << float(totalOutputJetMergedIO[0] / maxEvent_) << " and for sub-leading seed: " << float(totalOutputJetMergedIO[1] / maxEvent_) << "\n";
    unsigned int maxValueLeading = *std::max_element(allOutputJetMergedIOLeading.begin(), allOutputJetMergedIOLeading.end());
    unsigned int maxValueSubLeading = *std::max_element(allOutputJetMergedIOSubLeading.begin(), allOutputJetMergedIOSubLeading.end());
    std::cout << "max number of merged input objects (leading): " << maxValueLeading << " and for sub-leading: " << maxValueSubLeading << " for n input objects: " << maxObjectsConsidered_ << "\n";
    
    //std::cout << "Test bench completed." << std::endl;
    return 0;
}
