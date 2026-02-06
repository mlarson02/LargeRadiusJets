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

inline double undigitize_diam(const std::bitset<psi_R_bit_length_>& diam_bits) {
    return diam_bits.to_ulong() * 0.00390625; // FIXME shouldn't be hard-coded
}

int wrapSym(int phi){
    int phiWrapped = phi;
    if(phi > PI_D) phiWrapped = phi - TWO_PI_D;
    if(phi < - PI_D) phiWrapped = phi + TWO_PI_D;
    return phiWrapped;
}
/*
// Function to scale and digitize a value, returning the result as a binary string
template <int bit_length>
inline std::bitset<bit_length > digitize(double value, double min_val, double max_val, std::string type) {
    // Ensure the value is within range
    if(value < min_val) value = min_val;
    if(value > max_val) value = max_val;
    if (value < min_val || value > max_val) {
        std::cout << "value that is out of range; " << value << "\n";
        std::cout << "for min val : " << min_val << " and max val: " << max_val << "\n";
        std::cout << "for type: " << type << "\n";
        std::cout << "Value is out of range for bitlength: " << bit_length << "\n";
    }

    // Calculate scale factor
    double scale = (std::pow(2, bit_length)) / (max_val - min_val);
    std::cout << "scale: " << scale << "\n";
    
    // Calculate the digitized value and round it
    int digitized_value = static_cast<int>(std::round((value - min_val) * scale));
    
    // Return the digitized value as a binary string (with leading zeros to fit the bit length)
    std::bitset<bit_length > bin(digitized_value);

    return bin; // Extract only the relevant bits
}*/

unsigned int digitize(double value, int bit_length, double min_val, double max_val) {
    // Check if value is in range
    if (value < min_val) {
        value = min_val;
        std::cout << "Warning: Value " << value
          << " is out of range (" << min_val
          << ", " << max_val << ")\n";
    }
    if (value > max_val){
        value = max_val;
        std::cout << "Warning: Value " << value
          << " is out of range (" << min_val
          << ", " << max_val << ")\n";
    }

    double scale = (std::pow(2, bit_length) - 1) / (max_val - min_val);
    return static_cast<unsigned int>(std::round((value - min_val) * scale));
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
    /*if (in[2] < min_val) {
        min_val = in[2];
        min_idx = 2; // FIXME this should be set dynamically based on how big nSeedsDeltaR_ is...
    }*/
    /*if (in[3] < min_val) {
        min_val = in[3];
        min_idx = 3;
    }*/

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
                if(rCut == 0.001){
                    std::cout << "idx: " << idx << " and last_one_index: " << last_one_index << "\n";
                }
            }
            ++idx;
        }
    }
    
    unsigned int lut_max_size = last_one_index + 1;
    if(rCut == 0.001){
        std::cout << "lut_max_size: " << lut_max_size << "\n";
    }
    
    return lut_max_size;
}

// Returns input NTuple file name given parameters
std::string makeInputFileName(bool signalBool, std::string signalString,
                              std::string inputRootFilePath = "/home/larsonma/LargeRadiusJets/data/inputNTuples/") {
    std::ostringstream ss;
    
    if (signalBool) {
        if(signalString == "VBF_hh_bbbb") ss << inputRootFilePath << "mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_DAOD_NTUPLE_GEP.root";
        else if (signalString == "ggF_hh_bbbb") ss << inputRootFilePath << "mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_DAOD_NTUPLE_GEP.root";
        else if (signalString == "ZvvHbb") ss << inputRootFilePath << "mc21_14TeV_ZvvH125_bb_e8557_s4422_r16130_DAOD_NTUPLE_GEP.root";
        else if (signalString == "ttbar_had") ss << inputRootFilePath << "mc21_14TeV_ttbar_hdamp258p75_allhad_e8557_s4422_r16130_DAOD_NTUPLE_GEP.root";
        else if (signalString == "Zprime_ttbar") ss << inputRootFilePath << "mc21_14TeV_flatpT_Zprime_tthad_e8557_s4422_r16130_DAOD_NTUPLE_GEP.root";       
    } else {
        ss << inputRootFilePath << "mc21_14TeV_jj_JZ_e8557_s4422_r16130_DAOD_NTUPLE_GEP.root";
    }
    return ss.str();
}

// Returns output NTuple file name given parameters
std::string makeOutputFileName(double rMergeCut,
                               unsigned int NIOs,
                               unsigned int nSeeds,
                               double RSquaredCut,
                               bool signalBool,
                               std::string signalString,
                               std::string inputObjectType,
                               std::string seedObjectType,
                               bool useSKObjects,
                               std::string outputRootFilePath = "/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression_FixedSeedPosRecalcBug/") {
    std::string usePUSuppress;
    if(useSKObjects){
        usePUSuppress = "SK";
    }
    else{
        usePUSuppress = "NoSK";
    }
    std::ostringstream ss;
    if (signalBool) {
        if(signalString == "VBF_hh_bbbb") ss << outputRootFilePath << "mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_";
        else if (signalString == "ggF_hh_bbbb") ss << outputRootFilePath << "mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_";
        else if (signalString == "ZvvHbb") ss << outputRootFilePath << "mc21_14TeV_ZvvH125_bb_e8557_s4422_r16130_";
        else if (signalString == "ttbar_had") ss << outputRootFilePath << "mc21_14TeV_ttbar_hdamp258p75_allhad_e8557_s4422_r16130_";
        else if (signalString == "Zprime_ttbar") ss << outputRootFilePath << "mc21_14TeV_flatpT_Zprime_tthad_e8557_s4422_r16130_";   
    } else {
        ss << outputRootFilePath << "mc21_14TeV_jj_JZ_e8557_s4422_r16130_";
    }
    
    ss << "rMerge_" << std::setprecision(3) << rMergeCut << "_"
       << "IOs_" << NIOs << "_"
       << "Seeds_" << nSeeds << "_"
       << "R2_" << std::setprecision(3) << RSquaredCut << "_IO_" << inputObjectType << "_Seed_" << seedObjectType << "_" << usePUSuppress << ".root";

    return ss.str();
}

std::string makeOutputTextFileName(double rMergeCut,
                               unsigned int NIOs,
                               unsigned int nSeeds,
                               double RSquaredCut,
                               bool signalBool,
                               std::string signalString,
                               std::string inputObjectType,
                               std::string seedObjectType,
                               bool useSKObjects,
                               std::string outputTextFilePath = "/home/larsonma/LargeRadiusJets/data/MemPrintsEmulation/") {
    std::string usePUSuppress;
    if(useSKObjects){
        usePUSuppress = "SK";
    }
    else{
        usePUSuppress = "NoSK";
    }                            
    std::ostringstream ss;
    if (signalBool) {
        if(signalString == "VBF_hh_bbbb") ss << outputTextFilePath << "mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_";
        else if (signalString == "ggF_hh_bbbb") ss << outputTextFilePath << "mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_";
        else if (signalString == "ZvvHbb") ss << outputTextFilePath << "mc21_14TeV_ZvvH125_bb_e8557_s4422_r16130_";
        else if (signalString == "ttbar_had") ss << outputTextFilePath << "mc21_14TeV_ttbar_hdamp258p75_allhad_e8557_s4422_r16130_";
        else if (signalString == "Zprime_ttbar") ss << outputTextFilePath << "mc21_14TeV_flatpT_Zprime_tthad_e8557_s4422_r16130_"; 
    } else {
        ss << outputTextFilePath << "mc21_14TeV_jj_JZ_e8557_s4422_r16130_";
    }
    ss << "rMerge_" << std::setprecision(3) << rMergeCut << "_"
       << "IOs_" << NIOs << "_"
       << "Seeds_" << nSeeds << "_"
       << "R2_" << std::setprecision(3) << RSquaredCut << "_IO_" << inputObjectType << "_Seed_" << seedObjectType << "_" << usePUSuppress << ".dat";

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
                            unsigned int nSeedsInput,
                            unsigned int max_R2lut_size,
                            unsigned int max_Rlut_size,
                            unsigned int max_R_8b_lut_size)
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
    out << "static inline uint32_t maskN(unsigned n) { return (n >= 32) ? 0xFFFFFFFFu : ((1u << n) - 1u); }\n";
    out << "constexpr unsigned int nSeedsInput_ = " << nSeedsInput << ";\n";//= 6;\n";
    out << "constexpr unsigned int nSeedsOutput_ = " << nSeedsOutput << ";\n";
    out << "constexpr unsigned int maxObjectsConsidered_ = " << maxObjectsConsidered << ";\n";
    out << "constexpr double et_granularity_ = 0.125;\n";
    out << "constexpr double min_et_seed_pos_recalc_ = 10.0;\n";
    out << "constexpr double r2Cut_ = " << r2Cut << ";\n";
    out << "constexpr double rMergeCut_ = " << rMergeCut << ";\n";
    out << "constexpr unsigned int et_bit_length_ = 13;\n";
    out << "constexpr unsigned int eta_bit_length_ = 8;\n";
    out << "constexpr unsigned int phi_bit_length_ = 6;\n";
    out << "constexpr unsigned int psi_R_bit_length_ = 8;\n";
    out << "constexpr unsigned int num_constituents_bit_length_ = 9;\n";
    out << "constexpr unsigned int padded_zeroes_length_ = 64 - et_bit_length_ - eta_bit_length_ - phi_bit_length_ - psi_R_bit_length_ - num_constituents_bit_length_;\n";
    out << "constexpr unsigned int total_bits_output_ = padded_zeroes_length_ + num_constituents_bit_length_ + psi_R_bit_length_ + et_bit_length_ + eta_bit_length_ + phi_bit_length_;\n";
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
    out << "constexpr unsigned int et_max_ = 1024;\n";
    out << "constexpr unsigned int max_R2lut_size_ = " << max_R2lut_size << ";\n";
    out << "constexpr unsigned int max_Rlut_size_ = " << max_Rlut_size << ";\n";
    out << "constexpr double deltaR_max_ = 10.48187;\n";
    out << "constexpr unsigned int nSeeds_ = " << nSeedsOutput << ";\n";
    out << "constexpr unsigned int max_R_8b_lut_size_ = " << max_R_8b_lut_size << ";\n";
    out << "constexpr double phi_range_ = phi_max_ - phi_min_;\n\n";

    out << "constexpr unsigned int lut_size_ = (1u << (eta_bit_length_ + phi_bit_length_));\n";
    out << "constexpr unsigned int total_bits_ = padded_zeroes_length_ + psi_R_bit_length_ + et_bit_length_ + eta_bit_length_ + phi_bit_length_;\n\n";

    out << "constexpr unsigned int phi_low_  = 0;\n";
    out << "constexpr unsigned int phi_high_ = phi_low_ + phi_bit_length_ - 1;\n";

    out << "constexpr unsigned int eta_low_  = phi_high_ + 1;\n";
    out << "constexpr unsigned int eta_high_ = eta_low_ + eta_bit_length_ - 1;\n";

    out << "constexpr unsigned int et_low_   = eta_high_ + 1;\n";
    out << "constexpr unsigned int et_high_  = et_low_ + et_bit_length_ - 1;\n";

    out << "constexpr unsigned int psi_R_low_  = et_high_ + 1;\n";
    out << "constexpr unsigned int psi_R_high_ = psi_R_low_ + psi_R_bit_length_ - 1;\n";

    out << "constexpr unsigned int num_constituents_low_  = psi_R_high_ + 1;\n";
    out << "constexpr unsigned int num_constituents_high_ = num_constituents_low_ + num_constituents_bit_length_ - 1;\n";

    out << "constexpr unsigned int padded_zeroes_low_  = num_constituents_high_ + 1;\n";
    out << "constexpr unsigned int padded_zeroes_high_ = padded_zeroes_low_ + padded_zeroes_length_ - 1;\n";

    out << "constexpr unsigned int nSeedsDeltaR_ = 2;//nSeedsInput_ - nSeedsOutput_;\n"; // FIXME unhard code this to account for when using cone jets! 

    out << "static const bool lut_[max_R2lut_size_] =\n";
    out << "#include \"deltaR2LUT.h\"\n";
    out << ";\n\n";

    out << "static const unsigned int lutR_[max_Rlut_size_] = \n";
    out << "#include \"deltaRLUT.h\"\n";
    out << ";\n\n";

    out << "static const unsigned int lutR_8b_[max_R_8b_lut_size_] = \n";
    out << "#include \"deltaRLUT_8b.h\"\n";
    out << ";\n\n";
 
    out << "constexpr unsigned int deltaR_levels_ = (1u << deltaRBits_); // 256\n";
    out << "constexpr float deltaR_step_ = static_cast<float>(deltaR_max_) / (deltaR_levels_ - 1); // ~0.041\n";
    out << "constexpr unsigned int rMergeConsiderCutDigitized_ = static_cast<unsigned int>(\n";
    out << "    (rMergeCut_) / deltaR_step_ + 0.5f);\n\n"; // round to nearest

    out << "constexpr unsigned int psi_R_levels_ = (1u << psi_R_bit_length_); // 256\n";
    out << "constexpr float psi_R_step_ = static_cast<float>(1) / (psi_R_levels_ - 1); // step in R^2 per code\n";

    // ---- file ends ----
    std::cout << "writing constants file to: " << header_path << "\n";
    out.close();
}