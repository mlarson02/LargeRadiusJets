#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include "../emulationHelperFunctions.h"


int main() {
    gSystem->RedirectOutput("debuglog.log", "w");
    const double etaGranularity = 0.0390625;
    const double phiGranularity = 0.1;
    const unsigned int phiBitLength = 6;
    const unsigned int etaBitLength = 8;

    std::vector<double > rMergeCuts = {0.001, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 3.5};
    std::vector<double > rSquaredCuts = {1.0, 1.21, 1.44};
    std::vector<unsigned int > nIOs = {32, 64, 128, 256, 512};
    std::vector<unsigned int > nSeeds = {2};
    std::vector<unsigned int > nSeedsInput = {6};

    for(double rMergeCut : rMergeCuts){ // Loop through algorithm configurations affecting constants.h
        for(double rSquaredCut : rSquaredCuts){ // And generate different constants files
            unsigned int max_R2lut_size = calculate_lut_max_size(rSquaredCut, etaBitLength, phiBitLength, etaGranularity, phiGranularity); // fixme remove unnecessary _ as no longer global variable
            unsigned int max_Rlut_size = calculate_lut_max_size(rMergeCut, etaBitLength, phiBitLength, etaGranularity, phiGranularity);
            unsigned int max_R_8b_lut_size = calculate_lut_max_size(std::sqrt(rSquaredCut), etaBitLength, phiBitLength, etaGranularity, phiGranularity);
            for(unsigned int nIOOption : nIOs){
                for(unsigned int nSeedOption : nSeeds){
                    for (unsigned int nSeedInputOption : nSeedsInput){
                        std::cout << "---------------------" << "\n";
                        std::cout << "writing constants file with: " << "\n";
                        std::cout << "rMergeCut: " << rMergeCut << "\n";
                        std::cout << "rSquaredCut: " << rSquaredCut << "\n";
                        std::cout << "nIOs: " << nIOOption << "\n";
                        std::cout << "nSeeds: " << nSeedOption << "\n";
                        std::cout << "max_R2lut_size: " << max_R2lut_size << "\n";
                        std::cout << "max_Rlut_size: " << max_Rlut_size << "\n";
                        std::cout << "max_R_8b_lut_size: " << max_R_8b_lut_size << "\n";
                        std::cout << "---------------------" << "\n";
                        std::string constantsFilePath = makeInputConstantsFileName(rMergeCut, nIOOption, nSeedOption, rSquaredCut);
                        write_constants_header(constantsFilePath, rSquaredCut, rMergeCut, nIOOption, nSeedOption, nSeedInputOption,
                                            max_R2lut_size, max_Rlut_size, max_R_8b_lut_size);
                    } 
                }
            }
        }
    }

    for(double rMergeCut : rMergeCuts){ // Loop through algorithm configurations affecting LUTs
        for(double rSquaredCut : rSquaredCuts){ // And generate different LUT files

           // Compute max LUT sizes used to minimize LUT size
           unsigned int max_R2lut_size = calculate_lut_max_size(rSquaredCut, etaBitLength, phiBitLength, etaGranularity, phiGranularity); // fixme remove unnecessary _ as no longer global variable
           unsigned int max_Rlut_size = calculate_lut_max_size(rMergeCut, etaBitLength, phiBitLength, etaGranularity, phiGranularity);
           unsigned int max_R_8b_lut_size = calculate_lut_max_size(std::sqrt(rSquaredCut), etaBitLength, phiBitLength, etaGranularity, phiGranularity);

           std::cout << "---------------------" << "\n";
           std::cout << "writing LUT files with: " << "\n";
           std::cout << "max_R2lut_size: " << max_R2lut_size << "\n";
           std::cout << "max_Rlut_size: " << max_Rlut_size << "\n";
           std::cout << "max_R_8b_lut_size: " << max_R_8b_lut_size << "\n";
           std::cout << "---------------------" << "\n";

           // Get output LUT file names
           std::string lut_output_path = makeInputLUTFileName(rMergeCut, rSquaredCut, "deltaR2Cut"); // Boolean whether deltaEta, deltaPhi values pass deltaR^2 cut
           std::ofstream outfile(lut_output_path);
           std::string lutR_output_path = makeInputLUTFileName(rMergeCut, rSquaredCut, "deltaR"); // Higher precision deltaR calculation
           std::ofstream outfileR(lutR_output_path);
           std::string lutR_8b_output_path = makeInputLUTFileName(rMergeCut, rSquaredCut, "deltaR_8b"); // Lower precision deltaR calculation
           std::ofstream outfileR8b(lutR_8b_output_path);
            
    unsigned int idx = 0;
    outfile << "{\n    ";

    //std::cout << "r2Cut_ for LUT: " << r2Cut_ << "\n";

    for (unsigned int etaIt = 0; etaIt < (1 << etaBitLength); ++etaIt) {
        for (unsigned int phiIt = 0; phiIt < (1 << (phiBitLength - 1)); ++phiIt) { // phi bit length - 1 as max deltaPhi = pi, not 2pi
            if (idx >= max_R2lut_size) break; // STOP when you reach max_lut_size_!
                
            //std::cout << "idx: " << idx << "\n";
            //std::cout << "etaIt: " << etaIt << " etaGranularity: " << etaGranularity << "\n";
            //std::cout << "phiIt: " << phiIt << " phiGranularity: " << phiGranularity << "\n";

            float etaSquared = etaIt * etaGranularity * etaIt * etaGranularity;
            float phiSquared = phiIt * phiGranularity * phiIt * phiGranularity;
            float deltaR2 = etaSquared + phiSquared;
            //std::cout << "etaSquared: " << etaSquared << " phiSquared: " << phiSquared << " bool: " << (deltaR2 < r2Cut_) << "\n";
            outfile << ((deltaR2 <= r2Cut_) ? 1 : 0);

            idx++;
            if (idx < max_R2lut_size)
                outfile << ", " << ((idx % 16 == 0) ? "\n    " : "");
        }
        if (idx >= max_R2lut_size) break;
    }

    outfile << "\n};\n";
    outfile.close();

    unsigned int i = 0;
    outfileR << "{\n    ";

    //std::cout << "r2Cut_ for LUT: " << r2Cut_ << "\n";

    for (unsigned int etaIt = 0; etaIt < (1 << etaBitLength); ++etaIt) {
        for (unsigned int phiIt = 0; phiIt < (1 << (phiBitLength - 1)); ++phiIt) { // phi bit length - 1 as max deltaPhi = pi, not 2pi
            if (i >= max_Rlut_size) break;

            float deltaEta = etaIt * etaGranularity;
            float deltaPhi = phiIt * phiGranularity;
            float deltaR2 = deltaEta * deltaEta + deltaPhi * deltaPhi;
            float deltaR = std::sqrt(deltaR2);

            // Digitize deltaR to 8-bit value
            uint8_t digitizedDeltaR = static_cast<uint8_t>(deltaR / deltaR_step_ + 0.5f);
            if (digitizedDeltaR > 255) digitizedDeltaR = 255; // Clamp (optional safety)

            outfileR << static_cast<unsigned int>(digitizedDeltaR); // cast for readability

            i++;
            if (i < max_Rlut_size)
                outfileR << ", " << ((i % 16 == 0) ? "\n    " : "");
        }
        if (i >= max_Rlut_size) break;
    }

    outfileR << "\n};\n";
    outfileR.close();



    unsigned int iR = 0;
    outfileR8b << "{\n    ";

    //std::cout << "r2Cut_ for LUT: " << r2Cut_ << "\n";

    for (unsigned int etaIt = 0; etaIt < (1 << etaBitLength); ++etaIt) {
        for (unsigned int phiIt = 0; phiIt < (1 << (phiBitLength - 1)); ++phiIt) { // phi bit length - 1 as max deltaPhi = pi, not 2pi
            if (iR >= max_R_8b_lut_size) break;
            

            float deltaEta = etaIt * etaGranularity;
            float deltaPhi = phiIt * phiGranularity;
            float deltaR2 = deltaEta * deltaEta + deltaPhi * deltaPhi;
            float deltaR = std::sqrt(deltaR2);
            //std::cout << "deltaR: " << deltaR << "\n";
            // Digitize deltaR to 8-bit value
            uint8_t digitizedDeltaR = static_cast<uint8_t>(deltaR / psi_R_step_ + 0.5f);
            if (digitizedDeltaR > 255) digitizedDeltaR = 255; // Clamp (optional safety)
            
            outfileR8b << static_cast<unsigned int>(digitizedDeltaR); // cast for readability

            iR++;
            //std::cout << "digitizedDeltaR: " << std::dec << static_cast<unsigned int>(digitizedDeltaR) << "\n";
            if (iR < max_R_8b_lut_size)
                outfileR8b << ", " << ((iR % 255 == 0) ? "\n    " : "");
        }
        if (iR >= max_R_8b_lut_size) break;
    }

    outfileR8b << "\n};\n";
    outfileR8b.close();

    std::cout << "dR^2 LUT written to " << lut_output_path << " with size " << max_R2lut_size << std::endl;
    std::cout << "dR LUT written to " << lutR_output_path << " with size " << max_Rlut_size << std::endl;
    std::cout << "dR LUT (8b) written to " << lutR_8b_output_path << " with size " << max_R_8b_lut_size << std::endl;


        }
    }

    gSystem->Exit(0);
    return 0;
}