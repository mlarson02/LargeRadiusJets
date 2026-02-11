#ifndef HELPER_FUNCTIONS_ADV_H  // Check if the macro is defined
#define HELPER_FUNCTIONS_ADV_H  // Define the macro
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
#include "ap_int.h"
#include "constants_adv.h"

// INTEGER VERSION, to be used by FW
// FIXME return 



static inline ap_uint<2> index_of_min(ap_uint<deltaRBits_> (&in)[nSeedsDeltaR_]) {
//#pragma HLS pipeline II=1
//#pragma HLS inline on

    ap_uint<deltaRBits_> min_val = in[0];
    ap_uint<2> min_idx = 0;

    if (in[1] < min_val) {
        min_val = in[1];
        min_idx = 1;
    }
    if (in[2] < min_val) {
        min_val = in[2];
        min_idx = 2;
    }
    if (in[3] < min_val) {
        min_val = in[3];
        min_idx = 3;
    }

    return min_idx;
}

/*
inline ap_uint <eta_bit_length_ * 2 + 1 > calcDeltaR2(ap_uint<eta_bit_length_ > eta1, ap_uint<phi_bit_length_ >  phi1, ap_uint<eta_bit_length_ >  eta2, ap_uint<phi_bit_length_ >  phi2) {
    
    // FIXME need to load in LUT, likely in testbench? 
    
    //std::cout << "eta1: " << eta1 << " eta2: " << eta2 << " phi1: " << phi1 << " phi2: " << phi2 << "\n";
    ap_uint<eta_bit_length_ > dEta = ((eta1 - eta2) < 0) ? - (eta2 - eta1) : eta1 - eta2;
    ap_uint<phi_bit_length_ > dPhi = ((phi1 - phi2) < 0) ? - (phi2 - phi1) : phi1 - phi2;
    ap_uint<eta_bit_length_ + phi_bit_length_ > lutIndex = (dEta) * (dPhi); //FIXME add calculation of lutIndex to simplify deltaR^2 calculation (or just do this in top function)
    //std::cout << "dr2: " << dEta * dEta + dPhi * dPhi << "\n";
    return dEta * dEta + dPhi * dPhi;
    //return lut[lutIndex]
}
*/
#endif