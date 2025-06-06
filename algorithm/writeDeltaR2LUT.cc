#include <iostream>
#include <fstream>
#include <iomanip>
#ifndef WRITE_LUT  // Check if the macro is defined
#define WRITE_LUT 1
#include "constants.h"


int main() {
    const std::string lut_output_path = "../data/LUTs/deltaR2Cut.h";
    std::ofstream outfile(lut_output_path);
    if (!outfile) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    unsigned int idx = 0;
    outfile << "{\n    ";

    //std::cout << "r2Cut_: " << r2Cut_ << "\n";

    for (unsigned int etaIt = 0; etaIt < (1 << eta_bit_length_); ++etaIt) {
        for (unsigned int phiIt = 0; phiIt < (1 << phi_bit_length_); ++phiIt) {
            if (idx >= max_lut_size_)  // STOP when you reach max_lut_size_!
                break;
            //std::cout << "idx: " << idx << "\n";
            //std::cout << "etaIt: " << etaIt << " eta_granularity_: " << eta_granularity_ << "\n";
            //std::cout << "phiIt: " << phiIt << " phi_granularity_: " << phi_granularity_ << "\n";

            float etaSquared = etaIt * eta_granularity_ * etaIt * eta_granularity_;
            float phiSquared = phiIt * phi_granularity_ * phiIt * phi_granularity_;
            float deltaR2 = etaSquared + phiSquared;
            //std::cout << "etaSquared: " << etaSquared << " phiSquared: " << phiSquared << " bool: " << (deltaR2 < r2Cut_) << "\n";
            outfile << ((deltaR2 <= r2Cut_) ? 1 : 0);

            idx++;
            if (idx < max_lut_size_)
                outfile << ", " << ((idx % 16 == 0) ? "\n    " : "");
        }
        if (idx >= max_lut_size_)
            break;
    }

    outfile << "\n};\n";
    outfile.close();

    std::cout << "LUT written to " << lut_output_path << " with size " << max_lut_size_ << std::endl;
    return 0;
}
#endif