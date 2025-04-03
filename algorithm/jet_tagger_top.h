#ifndef SIM_TAGGER_TOP_H
#define SIM_TAGGER_TOP_H
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
#include "process_event.h"
//#include "helperFunctions.h"


void jet_tagger_top(std::array<std::array<double, 3>, nTotalSeeds_ >& seedValues, std::array<std::array<double, 3>, maxObjectsConsidered_ >& inputObjectValues, std::array<std::bitset<et_bit_length_ >, nSeeds_ >& outputJetEtValues, std::array<std::bitset<eta_bit_length_ >, nSeeds_ >& outputJetEtaValues, std::array<std::bitset<phi_bit_length_ >, nSeeds_ >& outputJetPhiValues);

#endif