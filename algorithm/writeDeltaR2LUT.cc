#include <iostream>
#include <fstream>
#include <vector>
#include "constants.h"
//#include helperFunctions.h
int main () {
    std::ofstream outfile("../data/LUTs/deltaR2Cut.dat");
    if (!outfile) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }
    //const unsigned int LUT_SIZE = (1 << (eta_bit_length_ + phi_bit_length_)); // Change as needed (UNUSED FOR NOW)
    //std::vector<unsigned int> lut(LUT_SIZE);
    for (unsigned int etaIt = 0; etaIt < (1 << eta_bit_length_); ++etaIt){
        for (unsigned int phiIt = 0; phiIt < (1 << phi_bit_length_); ++phiIt){
            float etaSquared = etaIt * eta_granularity_ * etaIt * eta_granularity_; 
            float phiSquared = phiIt * phi_granularity_ * phiIt * phi_granularity_;
            float deltaR2 = etaSquared + phiSquared;
            unsigned int lutValue = (deltaR2 < r2Cut_) ? 1 : 0;
            std::cout << "etaIt : " << etaIt << " and eta_granularity: " <<  eta_granularity_ << " and etaSquared: " << etaSquared << std::endl;
            std::cout << "phiIt : " << phiIt << " and phi_granularity: " <<  phi_granularity_ << " and phiSquared: " << phiSquared << std::endl;
            std::cout << "deltaR2: " << deltaR2 << " and lutValue: " << lutValue << "\n";
            outfile << lutValue << "\n";
        }
    }
    outfile.close();
    std::cout << "LUT written to lut.dat" << std::endl;
    return 0;
}
