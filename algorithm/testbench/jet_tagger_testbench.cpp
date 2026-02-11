#include "/home/larsonma/LargeRadiusJets/algorithm/jet_tag.h"
#include "/home/larsonma/LargeRadiusJets/algorithm/fileRead.h"
//#include "/home/larsonma/LargeRadiusJets/algorithm/helperFunctions.h"
#include <string>
#include <iostream>
#include <array>
#include <vector>

int main() {

    input inputObjectValues[maxObjectsConsidered_];

    const std::string inputObjectFile = memPrintsPath_ + "GEPConeJetsCellsTowers/" + fileName_ + "_gepconejetscellstowers.dat";
    std::cout << "inputObjectFile: " << inputObjectFile << "\n";
    // Call the function under test
    std::string outputJetsFile = memPrintsPath_ + "largeRJets_ConeJetsCellsTowersOnly/" + fileName_ + "_largeR";
    outputJetsFile += kFileSuffix; 
    outputJetsFile += ".dat";

    std::ofstream outFile(outputJetsFile);
    if (!outFile) {
        std::cerr << "Error: Could not open file " << outputJetsFile << std::endl;
        return 0;
    }
    std::cout << "rMergeConsiderCutDigitized_: " << rMergeConsiderCutDigitized_ << "\n";
    for (unsigned int iEvt = 0; iEvt < maxEvent_; iEvt++){

        outFile << "Event : " << std::dec << iEvt << std::endl;
        //std::cout << " ---------------------------------- " << "\n";
        //std::cout << "processing event: " << std::dec << iEvt << "\n";

        extract_values_from_file<maxObjectsConsidered_ >(inputObjectFile, inputObjectValues, iEvt);

        for (unsigned j = 0; j < maxObjectsConsidered_; ++j) {
            uint32_t w = static_cast<uint32_t>(inputObjectValues[j]);
            //std::cout << "input object value: 0x"
            //        << std::setw(8) << std::setfill('0') << std::uppercase
            //        << std::hex << w
            //        << std::dec << std::setfill(' ') << "\n";
        }
        output outputJetValues[nSeedsOutput_];

        jet_tag(inputObjectValues, outputJetValues); // top function 
        for (unsigned int iOutput = 0; iOutput < nSeedsOutput_; iOutput++){
            unsigned long long num_constituents_value = outputJetValues[iOutput].range(num_constituents_high_, num_constituents_low_).to_uint64();
            unsigned long long psi_R_value = outputJetValues[iOutput].range(psi_R_high_, psi_R_low_).to_uint64();
            unsigned long long et_value = outputJetValues[iOutput].range(et_high_, et_low_).to_uint64();  // Convert to unsigned long long
            unsigned long long eta_value = outputJetValues[iOutput].range(eta_high_, eta_low_).to_uint64();
            unsigned long long phi_value = outputJetValues[iOutput].range(phi_high_, phi_low_).to_uint64();
            /*
            std::cout << "OUTPUT DATA: " << "\n";
            std::cout << "Et: " << outputJetValues[iOutput].range(et_high_, et_low_) << "\n";
            std::cout << "Et 64: " << outputJetValues[iOutput].range(et_high_, et_low_).to_uint64() << "\n";
            std::cout << "eta: " << outputJetValues[iOutput].range(eta_high_, eta_low_) << "\n";
            std::cout << "eta 64: " << eta_value << "\n";*/
            //std::cout << "et_value : " << et_value << "\n";
     
            // Convert to ap_uint
            std::bitset<num_constituents_bit_length_ > mergedio_bitset(num_constituents_value); 
            std::bitset<psi_R_bit_length_ > psi_R_bitset(psi_R_value); 
            std::bitset<et_bit_length_ > et_bitset(et_value);
            std::bitset<eta_bit_length_ > eta_bitset(eta_value);
            std::bitset<phi_bit_length_ > phi_bitset(phi_value);
            //ap_uint<diam_bit_length_> diam_apuint(diam_value);
            //ap_uint<et_bit_length_> et_apuint(et_value);
            //std::cout << "et_apuint : " << et_apuint << "\n";
            //ap_uint<eta_bit_length_> eta_apuint(eta_value);
            //ap_uint<phi_bit_length_> phi_apuint(phi_value);
            uint64_t combined_value =
                ((num_constituents_value & maskN(num_constituents_bit_length_)) << (psi_R_bit_length_ + et_bit_length_ + eta_bit_length_ + phi_bit_length_)) |
                ((psi_R_value & maskN(psi_R_bit_length_)) << (et_bit_length_ + eta_bit_length_ + phi_bit_length_)) |
                ((et_value   & maskN(et_bit_length_  )) << (eta_bit_length_ + phi_bit_length_)) |
                ((eta_value  & maskN(eta_bit_length_ )) <<  phi_bit_length_) |
                ( phi_value  & maskN(phi_bit_length_ ));
            //ap_uint<total_bits_> combined_value = (ap_uint<total_bits_>(diam_apuint) << (et_bit_length_ + eta_bit_length_ + phi_bit_length_ ) | ap_uint<et_bit_length_ + eta_bit_length_ + phi_bit_length_ >(et_apuint) << (eta_bit_length_ + phi_bit_length_)) | (ap_uint<et_bit_length_ + eta_bit_length_ + phi_bit_length_ >(eta_apuint) << phi_bit_length_) | phi_apuint;
            //std::cout << "total_bits_: " << std::dec << total_bits_ << "\n";
            //std::cout << "combined_value : " << std::hex << combined_value << "\n";
            // Convert to hexadecimal (for the last field)
            std::stringstream hex_stream;
            hex_stream << std::hex << std::nouppercase << std::setfill('0') << std::setw(total_bits_output_ / 4) << combined_value;
            std::string hexValue = hex_stream.str();

            //std::cout << "hex_stream: " << std::hex << std::setfill('0') << std::setw(total_bits_output_ / 4) << hex_stream.str() << "\n";

            //std::cout << "et_bitset.to_string() : " << et_bitset.to_string() << "\n";
            // Output in the required format
            outFile << "0x" << std::setw(2) << std::setfill('0') << std::hex << iOutput
            << " " << mergedio_bitset.to_string() << "|" << psi_R_bitset.to_string() << "|" << et_bitset.to_string() << "|" << eta_bitset.to_string() << "|" << phi_bitset.to_string()
            << " 0x" << hexValue << std::endl; 

            //totalOutputJetMergedIO[iOutput] += numio_value; 
            //if (iOutput == 0) allOutputJetMergedIOLeading.push_back(numio_value);
            //if (iOutput == 1) allOutputJetMergedIOSubLeading.push_back(numio_value);
        }
    }   

    //std::cout << "average merged IO for leading seed: " << float(totalOutputJetMergedIO[0] / maxEvent_) << " and for sub-leading seed: " << float(totalOutputJetMergedIO[1] / maxEvent_) << "\n";
    //unsigned int maxValueLeading = *std::max_element(allOutputJetMergedIOLeading.begin(), allOutputJetMergedIOLeading.end());
    //unsigned int maxValueSubLeading = *std::max_element(allOutputJetMergedIOSubLeading.begin(), allOutputJetMergedIOSubLeading.end());
    //std::cout << "max number of merged input objects (leading): " << maxValueLeading << " and for sub-leading: " << maxValueSubLeading << " for n input objects: " << maxObjectsConsidered_ << "\n";
    
    //std::cout << "Test bench completed." << std::endl;
    return 0;
}
