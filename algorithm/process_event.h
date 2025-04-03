#ifndef _PROCESS_EVENT_H_
#define _PROCESS_EVENT_H_
#include "/home/larsonma/LargeRadiusJets/algorithm/helperFunctions.h"
#include <iostream>
#include <array>
#include <vector>
#include <bitset>


// Main function
inline void process_event(std::array<std::array<double, 3>, nTotalSeeds_ >& seedValues, std::array<std::array<double, 3>, maxObjectsConsidered_ >& inputObjectValues, std::array<std::bitset<et_bit_length_ >, nSeeds_ >& outputJetEtValues, std::array<std::bitset<eta_bit_length_ >, nSeeds_ >& outputJetEtaValues, std::array<std::bitset<phi_bit_length_ >, nSeeds_ >& outputJetPhiValues); // FIXME potentially use templated / overloaded func to deal with whether write out files while running synth or c-sim
    
#endif