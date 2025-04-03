#include "process_event.h"

// Main function
void process_event(std::array<std::array<double, 3>, nTotalSeeds_ >& seedValues, std::array<std::array<double, 3>, maxObjectsConsidered_ >& inputObjectValues, std::array<std::bitset<et_bit_length_ >, nSeeds_ >& outputJetEtValues, std::array<std::bitset<eta_bit_length_ >, nSeeds_ >& outputJetEtaValues, std::array<std::bitset<phi_bit_length_ >, nSeeds_ >& outputJetPhiValues){ // FIXME potentially use templated / overloaded func to deal with whether write out files while running synth or c-sim
    // Open input file
    // perform sorting by Et here
    // Sort the vector of arrays for the specific event (iEvt)
    /*std::sort(seedValues.begin(), seedValues.end(), // FIXME possibly replace with insertion sort (note: this is O(n^2)) - consider instead looping through and only considering input objects with minimum energy
                [](const std::array<double, 3>& a, const std::array<double, 3>& b) {
                    return a[0] > b[0]; // Sort by highest Et (first element of the array)
                });*/
    //std::cout << "beginning process_event" << std::endl;
    for (unsigned int iSeed = 0; iSeed < nSeeds_; ++iSeed){ // FIXME no longer considering highest Et seed first
        //std::cout << "processing seed: " << iSeed << std::endl;
        //std::cout << "size of seed values: " << seedValues[iSeed].size() << "\n";
        double outputJetEt = seedValues[iSeed][0]; // reset outputjet values for each seed, to values of seed
        double outputJetEta = seedValues[iSeed][1]; 
        double outputJetPhi = seedValues[iSeed][2]; 

        //std::cout << "outputJetEt: " << outputJetEt << " and outputJetEta: " << outputJetEta << " and outputJetPhi: " << outputJetPhi << "\n";
        //std::cout << "seed Et, Eta, phi: " << seedValues[iSeed][0] << " , " << seedValues[iSeed][1] << " , " << seedValues[iSeed][2] << "\n";
        //std::cout << "inputObjectValues.size(): " << inputObjectValues.size() << "\n";
        for (unsigned int iInput = 0; iInput < inputObjectValues.size() && iInput < maxObjectsConsidered_; ++iInput){
            //std::cout << "processing iInput: " << iInput << std::endl;
            //std::cout << "topo Et, Eta, phi: " << inputObjectValues[iInput][0] << " , " << inputObjectValues[iInput][1] << " , " << inputObjectValues[iInput][2] << "\n";
            double dR2 = calcDeltaR2(seedValues[iSeed][1], seedValues[iSeed][2], inputObjectValues[iInput][1], inputObjectValues[iInput][2]);
            //std::cout << "deltaR2: " << dR2 << "\n";
            if (dR2 <= r2Cut_){
                //std::cout << "iInput: "<< iInput << "and inputObjectValues.size(): " <<  inputObjectValues.size() << "\n";
                outputJetEt += inputObjectValues[iInput][0];
                // Erase the element at iInput from inputObjectValues
                //std::cout << "iINput: " << iInput << " and inputObjectValues.begin() + iInput: " << inputObjectValues.begin() + iInput << " and inputObjectValues.size(): " <<inputObjectValues.size() << "\n";
                
                //inputObjectValues.erase(inputObjectValues.begin() + iInput); FIXME how to handle removing this object???? 

                // Adjust the loop index since the vector size has decreased
                //--iInput;
            }
        }
        //std::cout << "outputjetEt : " << outputJetEt << " original seed Et: " << seedValues[iSeed][0] << "\n";
        //std::cout << "outputJetEta: " << outputJetEta << " outputJetPhi: " << outputJetPhi << "\n";
        //std::cout << "outputJetEt: " << outputJetEt << " outputJetEta: " << outputJetEta << "outputJetPhi: " << outputJetPhi << "\n";
        // Convert to binary format (assuming fixed lengths)
        std::bitset<et_bit_length_ > et_bin = digitize<et_bit_length_>(outputJetEt, et_min_, et_max_); // Example: 13 bits for Et
        std::bitset<eta_bit_length_ > eta_bin = digitize<eta_bit_length_>(outputJetEta, eta_min_, eta_max_); // Example: 11 bits for eta
        std::bitset<phi_bit_length_ > phi_bin = digitize<phi_bit_length_>(outputJetPhi, phi_min_, phi_max_); // Example: 8 bits for phi
        outputJetEtValues[iSeed]= et_bin;
        outputJetEtaValues[iSeed] = eta_bin;
        outputJetPhiValues[iSeed] = phi_bin;

        //outputJetValues.push_back({et_bin, eta_bin, phi_bin});
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