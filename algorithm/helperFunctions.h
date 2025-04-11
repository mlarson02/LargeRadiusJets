#ifndef HELPER_FUNCTIONS_H  // Check if the macro is defined
#define HELPER_FUNCTIONS_H  // Define the macro
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
#include "constants.h"

// INTEGER VERSION, to be used by FW
inline ap_uint <eta_bit_length_ * 2 + 1 > calcDeltaR2(ap_uint<eta_bit_length_ > eta1, ap_uint<phi_bit_length_ >  phi1, ap_uint<eta_bit_length_ >  eta2, ap_uint<phi_bit_length_ >  phi2) {
    
    //lutIndex = ; FIXME add calculation of lutIndex to simplify deltaR^2 calculation (or just do this in top function)
    
    //std::cout << "eta1: " << eta1 << " eta2: " << eta2 << " phi1: " << phi1 << " phi2: " << phi2 << "\n";
    ap_uint<eta_bit_length_ > dEta = ((eta1 - eta2) < 0) ? - (eta2 - eta1) : eta1 - eta2;
    ap_uint<phi_bit_length_ > dPhi = ((phi1 - phi2) < 0) ? - (phi2 - phi1) : phi1 - phi2;

    //std::cout << "dr2: " << dEta * dEta + dPhi * dPhi << "\n";
    return dEta * dEta + dPhi * dPhi;
}

#endif