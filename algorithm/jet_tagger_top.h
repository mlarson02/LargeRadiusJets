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


void jet_tagger_top(etEtaPhi seedValues[nTotalSeeds_], etEtaPhi inputObjectValues[maxObjectsConsidered_], ap_uint<et_bit_length_ > outputJetEtValues[nSeeds_], ap_uint<eta_bit_length_ > outputJetEtaValues[nSeeds_], ap_uint<phi_bit_length_ > outputJetPhiValues[nSeeds_]);

#endif