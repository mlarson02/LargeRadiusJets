#include <string>
#include <sstream>
#include <iomanip>  // for std::setprecision
#include <fstream>
#include <iostream>
#include <cmath>
#include "UsedConstantsLUTs/constants.h"

inline double undigitize_phi(const std::bitset<phi_bit_length_>& phi_bits) {
    return phi_min_ + phi_bits.to_ulong() * (6.4 / 64.0);
}

inline double undigitize_eta(const std::bitset<eta_bit_length_>& eta_bits) {
    return eta_min_ + eta_bits.to_ulong() * (10.0 / 256.0);
}

inline double undigitize_et(const std::bitset<et_bit_length_>& et_bits) {
    return et_bits.to_ulong() * et_granularity_;
}

inline double undigitize_diam(const std::bitset<diam_bit_length_>& diam_bits) {
    return diam_bits.to_ulong() * 0.03125;
}

int wrapSym(int phi){
    int phiWrapped = phi;
    if(phi > PI_D) phiWrapped = phi - TWO_PI_D;
    if(phi < - PI_D) phiWrapped = phi + TWO_PI_D;
    return phiWrapped;
}

// Function to scale and digitize a value, returning the result as a binary string
template <int bit_length>
inline std::bitset<bit_length > digitize(double value, double min_val, double max_val) {
    // Ensure the value is within range
    if (value < min_val || value > max_val) {
        std::cerr << "Value is out of range" << "\n";
    }

    // Calculate scale factor
    double scale = (std::pow(2, bit_length)) / (max_val - min_val);
    
    // Calculate the digitized value and round it
    int digitized_value = static_cast<int>(std::round((value - min_val) * scale));
    
    // Return the digitized value as a binary string (with leading zeros to fit the bit length)
    std::bitset<bit_length > bin(digitized_value);

    return bin; // Extract only the relevant bits
}

double calcDeltaR2(double eta1, double phi1, double eta2, double phi2) {
    //std::cout << "eta1: " << eta1 << " eta2: " << eta2 << " phi1: " << phi1 << " phi2: " << phi2 << "\n";
    double dEta = eta1 - eta2;
    double dPhi = std::abs(phi1 - phi2);
    
    // Ensure Δφ is within [-π, π] range
    if (dPhi > M_PI) {
        dPhi = 2 * M_PI - dPhi;
    }

    return dEta * dEta + dPhi * dPhi;
}

unsigned int index_of_min(unsigned int (&in)[nSeedsDeltaR_]) { // FIXME can't use ap_uint anymore...

    unsigned int min_val = in[0];
    unsigned int min_idx = 0;

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

unsigned int calculate_lut_max_size(double rCut,
                                     unsigned int eta_bit_length,
                                     unsigned int phi_bit_length,
                                     double eta_granularity,
                                     double phi_granularity)
{
    unsigned int last_one_index = 0;
    unsigned int idx = 0;

    for (unsigned int etaIt = 0; etaIt < (1u << eta_bit_length); ++etaIt) {
        for (unsigned int phiIt = 0; phiIt < (1u << (phi_bit_length - 1)); ++phiIt) {
            double etaSquared = std::pow(etaIt * eta_granularity, 2);
            double phiSquared = std::pow(phiIt * phi_granularity, 2);

            double deltaR = std::sqrt(etaSquared + phiSquared);

            if (deltaR < rCut) {
                last_one_index = idx;
            }
            ++idx;
        }
    }

    unsigned int lut_max_size = last_one_index + 1;
    return lut_max_size;
}

// Returns input NTuple file name given parameters
std::string makeInputFileName(bool signalBool, unsigned int jzSlice,
                              std::string inputRootFilePath = "/home/larsonma/LargeRadiusJets/data/inputNTuples/") {
    std::ostringstream ss;
    if (signalBool) {
        ss << inputRootFilePath << "mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_DAOD_NTUPLE_GEP.root";
    } else {
        ss << inputRootFilePath << "mc21_14TeV_jj_JZ" << jzSlice << "_e8557_s4422_r16130_DAOD_NTUPLE_GEP.root";
    }
    return ss.str();
}

// Returns output NTuple file name given parameters
std::string makeOutputFileName(double rMergeCut,
                               unsigned int NIOs,
                               unsigned int nSeeds,
                               double RSquaredCut,
                               bool signalBool,
                               unsigned int jzSlice,
                               std::string outputRootFilePath = "/home/larsonma/LargeRadiusJets/data/outputNTuples/") {
    std::ostringstream ss;
    if (signalBool) {
        ss << outputRootFilePath << "mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_";
    } else {
        ss <<outputRootFilePath << "mc21_14TeV_jj_JZ" << jzSlice << "_e8557_s4422_r16130";
    }
    ss << "rMerge_" << std::setprecision(3) << rMergeCut << "_"
       << "IOs_" << NIOs << "_"
       << "Seeds_" << nSeeds << "_"
       << "R2_" << std::setprecision(2) << RSquaredCut << ".root";

    return ss.str();
}

std::string makeOutputTextFileName(double rMergeCut,
                               unsigned int NIOs,
                               unsigned int nSeeds,
                               double RSquaredCut,
                               bool signalBool,
                               unsigned int jzSlice,
                               std::string outputRootFilePath = "/home/larsonma/LargeRadiusJets/data/MemPrintsEmulation/") {
    std::ostringstream ss;
    if (signalBool) {
        ss << outputRootFilePath << "mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_";
    } else {
        ss <<outputRootFilePath << "mc21_14TeV_jj_JZ" << jzSlice << "_e8557_s4422_r16130";
    }
    ss << "rMerge_" << std::setprecision(3) << rMergeCut << "_"
       << "IOs_" << NIOs << "_"
       << "Seeds_" << nSeeds << "_"
       << "R2_" << std::setprecision(2) << RSquaredCut << ".dat";

    return ss.str();
}

// Returns LUT file name given parameters
std::string makeInputLUTFileName(double rMergeCut,
                                 double RSquaredCut,
                                 std::string LUTType,
                                 std::string inputLUTFilePath = "/home/larsonma/LargeRadiusJets/algorithm/emulation/LUT_Constants_Generation/LUTs/") {
    std::ostringstream ss;
    ss << inputLUTFilePath << "LUT_" << LUTType << "_rMerge_" << std::setprecision(4) << rMergeCut << "_"
       << "R2_" << std::setprecision(3) << RSquaredCut << ".h";

    return ss.str();
}

// Returns Constants.h file name given parameters
std::string makeInputConstantsFileName(double rMergeCut,
                                       unsigned int NIOs,
                                       unsigned int nSeeds,
                                       double RSquaredCut,
                                       std::string constantsFilePath = "/home/larsonma/LargeRadiusJets/algorithm/emulation/LUT_Constants_Generation/constants/") {
    std::ostringstream ss;
    ss << constantsFilePath << "constants_rMerge_" << std::setprecision(4) << rMergeCut << "_"
       << "R2_" << std::setprecision(3) << RSquaredCut << "_IOs_" << NIOs << "_"
       << "Seeds_" << nSeeds << ".h";

    return ss.str();
}


void write_constants_header(const std::string& header_path,
                            double r2Cut,
                            double rMergeCut,
                            unsigned int maxObjectsConsidered,
                            unsigned int nSeedsOutput,
                            unsigned int max_R2lut_size,
                            unsigned int max_Rlut_size,
                            unsigned int max_R_5b_lut_size)
{
    std::ofstream out(header_path);
    if (!out) {
        std::cerr << "Failed to open " << header_path << " for writing.\n";
        return;
    }

    // choose a stable formatting for floating-point values in the header
    out.setf(std::ios::fixed);
    out << std::setprecision(4);

    // ---- file begins ----
    out << "// Constants used by JetTagger Emulation\n\n";

    // Fixed (from your template)
    out << "constexpr unsigned int nSeedsInput_ = 6;\n";
    out << "constexpr unsigned int nSeedsOutput_ = " << nSeedsOutput << ";\n";
    out << "constexpr unsigned int maxObjectsConsidered_ = " << maxObjectsConsidered << ";\n";
    out << "constexpr double et_granularity_ = 0.25;\n";
    out << "constexpr double r2Cut_ = " << r2Cut << ";\n";
    out << "constexpr double rMergeCut_ = " << rMergeCut << ";\n";
    out << "constexpr unsigned int et_bit_length_ = 13;\n";
    out << "constexpr unsigned int eta_bit_length_ = 8;\n";
    out << "constexpr unsigned int phi_bit_length_ = 6;\n";
    out << "constexpr unsigned int diam_bit_length_ = 5;\n";
    out << "constexpr unsigned int deltaRBits_ = 8;\n";
    out << "constexpr double phi_min_ = -3.2;\n";
    out << "constexpr double phi_max_ = 3.2;\n";
    out << "constexpr unsigned int pi_digitized_in_phi_ = 31;\n";
    out << "const int PI_D     =  int(pi_digitized_in_phi_);\n";
    out << "const int TWO_PI_D =  int(1 << phi_bit_length_);\n"; // wrap period (64 for 6 bits)
    out << "constexpr double eta_min_ = -5.0;\n";
    out << "constexpr double eta_max_ = 5.0;\n";
    out << "constexpr double eta_granularity_ = 0.0390625;\n";
    out << "constexpr double phi_granularity_ = 0.1;\n";
    out << "constexpr unsigned int et_min_ = 0;\n";
    out << "constexpr unsigned int et_max_ = 2048;\n";
    out << "constexpr unsigned int max_R2lut_size_ = 803;\n";
    out << "constexpr unsigned int max_Rlut_size_ = 2853;\n";
    out << "constexpr double deltaR_max_ = 10.48187;\n";
    out << "constexpr unsigned int nSeeds_ = 2;\n";
    out << "constexpr unsigned int max_R_5b_lut_size_ = 803;\n";
    out << "constexpr double phi_range_ = phi_max_ - phi_min_;\n\n";

    out << "constexpr unsigned int lut_size_ = (1u << (eta_bit_length_ + phi_bit_length_));\n";
    out << "constexpr unsigned int total_bits_ = diam_bit_length_ + et_bit_length_ + eta_bit_length_ + phi_bit_length_;\n\n";

    out << "constexpr unsigned int phi_low_  = 0;\n";
    out << "constexpr unsigned int phi_high_ = phi_low_ + phi_bit_length_ - 1;\n\n";

    out << "constexpr unsigned int eta_low_  = phi_high_ + 1;\n";
    out << "constexpr unsigned int eta_high_ = eta_low_ + eta_bit_length_ - 1;\n\n";

    out << "constexpr unsigned int et_low_   = eta_high_ + 1;\n";
    out << "constexpr unsigned int et_high_  = et_low_ + et_bit_length_ - 1;\n\n";

    out << "constexpr unsigned int diam_low_  = et_high_ + 1;\n";
    out << "constexpr unsigned int diam_high_ = diam_low_ + diam_bit_length_ - 1;\n\n";

    out << "constexpr unsigned int nSeedsDeltaR_ = nSeedsInput_ - nSeedsOutput_;\n\n";

    out << "static const bool lut_[max_R2lut_size_] =\n";
    out << "#include \"deltaR2LUT.h\"\n";
    out << ";\n\n";

    out << "static const unsigned int lutR_[max_Rlut_size_] = \n";
    out << "#include \"deltaRLUT.h\"\n";
    out << ";\n\n";

    out << "static const unsigned int lutR_5b_[max_R_5b_lut_size_] = \n";
    out << "#include \"deltaRLUT_5b.h\"\n";
    out << ";\n\n";

    out << "constexpr unsigned int deltaR_levels_ = (1u << deltaRBits_); // 256\n";
    out << "constexpr float deltaR_step_ = static_cast<float>(deltaR_max_) / (deltaR_levels_ - 1); // ~0.041\n";
    out << "constexpr unsigned int rMergeConsiderCutDigitized_ = static_cast<unsigned int>(\n";
    out << "    (rMergeCut_) / deltaR_step_ + 0.5f);\n\n"; // round to nearest

    out << "constexpr unsigned int diam_levels_ = (1u << diam_bit_length_); // 32\n";
    out << "constexpr float diam_step_ = static_cast<float>(r2Cut_) / (diam_levels_ - 1); // step in R^2 per code\n";

    // ---- file ends ----
    std::cout << "writing constants file to: " << header_path << "\n";
    out.close();
}