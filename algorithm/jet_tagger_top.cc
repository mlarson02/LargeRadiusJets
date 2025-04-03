#include "jet_tagger_top.h"
void jet_tagger_top(std::array<std::array<double, 3>, nTotalSeeds_ >& seedValues, std::array<std::array<double, 3>, maxObjectsConsidered_ >& inputObjectValues, std::array<std::bitset<et_bit_length_ >, nSeeds_ >& outputJetEtValues, std::array<std::bitset<eta_bit_length_ >, nSeeds_ >& outputJetEtaValues, std::array<std::bitset<phi_bit_length_ >, nSeeds_ >& outputJetPhiValues){
    process_event(seedValues, inputObjectValues, outputJetEtValues, outputJetEtaValues, outputJetPhiValues);
}