#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
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

    const std::string lutR_output_path = "../data/LUTs/deltaR.h";
    std::ofstream outfileR(lutR_output_path);

    if (!outfileR) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    const std::string lutR_32b_output_path = "../data/LUTs/deltaR_32b.h";
    std::ofstream outfileR32b(lutR_32b_output_path);
    if (!outfileR32b) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    unsigned int idx = 0;
    outfile << "{\n    ";

    //std::cout << "r2Cut_ for LUT: " << r2Cut_ << "\n";

    for (unsigned int etaIt = 0; etaIt < (1 << eta_bit_length_); ++etaIt) {
        for (unsigned int phiIt = 0; phiIt < (1 << (phi_bit_length_ - 1)); ++phiIt) { // phi bit length - 1 as max deltaPhi = pi, not 2pi
            if (idx >= max_R2lut_size_) break; // STOP when you reach max_lut_size_!
                
            //std::cout << "idx: " << idx << "\n";
            //std::cout << "etaIt: " << etaIt << " eta_granularity_: " << eta_granularity_ << "\n";
            //std::cout << "phiIt: " << phiIt << " phi_granularity_: " << phi_granularity_ << "\n";

            float etaSquared = etaIt * eta_granularity_ * etaIt * eta_granularity_;
            float phiSquared = phiIt * phi_granularity_ * phiIt * phi_granularity_;
            float deltaR2 = etaSquared + phiSquared;
            //std::cout << "etaSquared: " << etaSquared << " phiSquared: " << phiSquared << " bool: " << (deltaR2 < r2Cut_) << "\n";
            outfile << ((deltaR2 <= r2Cut_) ? 1 : 0);

            idx++;
            if (idx < max_R2lut_size_)
                outfile << ", " << ((idx % 16 == 0) ? "\n    " : "");
        }
        if (idx >= max_R2lut_size_) break;
    }

    outfile << "\n};\n";
    outfile.close();

    unsigned int i = 0;
    outfileR << "{\n    ";

    //std::cout << "r2Cut_ for LUT: " << r2Cut_ << "\n";

    for (unsigned int etaIt = 0; etaIt < (1 << eta_bit_length_); ++etaIt) {
        for (unsigned int phiIt = 0; phiIt < (1 << (phi_bit_length_ - 1)); ++phiIt) { // phi bit length - 1 as max deltaPhi = pi, not 2pi
            if (i >= max_Rlut_size_) break;

            float deltaEta = etaIt * eta_granularity_;
            float deltaPhi = phiIt * phi_granularity_;
            float deltaR2 = deltaEta * deltaEta + deltaPhi * deltaPhi;
            float deltaR = std::sqrt(deltaR2);

            // Digitize deltaR to 8-bit value
            uint8_t digitizedDeltaR = static_cast<uint8_t>(deltaR / deltaR_step_ + 0.5f);
            if (digitizedDeltaR > 255) digitizedDeltaR = 255; // Clamp (optional safety)

            outfileR << static_cast<unsigned int>(digitizedDeltaR); // cast for readability

            i++;
            if (i < max_Rlut_size_)
                outfileR << ", " << ((i % 16 == 0) ? "\n    " : "");
        }
        if (i >= max_Rlut_size_) break;
    }

    outfileR << "\n};\n";
    outfileR.close();



    unsigned int iR = 0;
    outfileR32b << "{\n    ";

    //std::cout << "r2Cut_ for LUT: " << r2Cut_ << "\n";

    for (unsigned int etaIt = 0; etaIt < (1 << eta_bit_length_); ++etaIt) {
        for (unsigned int phiIt = 0; phiIt < (1 << (phi_bit_length_ - 1)); ++phiIt) { // phi bit length - 1 as max deltaPhi = pi, not 2pi
            if (iR >= max_R_32b_lut_size_) break;
            

            float deltaEta = etaIt * eta_granularity_;
            float deltaPhi = phiIt * phi_granularity_;
            float deltaR2 = deltaEta * deltaEta + deltaPhi * deltaPhi;
            float deltaR = std::sqrt(deltaR2);
            //std::cout << "deltaR: " << deltaR << "\n";
            // Digitize deltaR to 8-bit value
            uint8_t digitizedDeltaR = static_cast<uint8_t>(deltaR / diam_step_ + 0.5f);
            if (digitizedDeltaR > 31) digitizedDeltaR = 31; // Clamp (optional safety)
            
            outfileR32b << static_cast<unsigned int>(digitizedDeltaR); // cast for readability

            iR++;
            //std::cout << "digitizedDeltaR: " << std::dec << static_cast<unsigned int>(digitizedDeltaR) << "\n";
            if (iR < max_R_32b_lut_size_)
                outfileR32b << ", " << ((iR % 16 == 0) ? "\n    " : "");
        }
        if (iR >= max_R_32b_lut_size_) break;
    }

    outfileR32b << "\n};\n";
    outfileR32b.close();

    std::cout << "dR^2 LUT written to " << lut_output_path << " with size " << max_R2lut_size_ << std::endl;
    std::cout << "dR LUT written to " << lutR_output_path << " with size " << max_Rlut_size_ << std::endl;
    std::cout << "dR LUT written to " << lutR_32b_output_path << " with size " << max_R_32b_lut_size_ << std::endl;
    return 0;
}
#endif