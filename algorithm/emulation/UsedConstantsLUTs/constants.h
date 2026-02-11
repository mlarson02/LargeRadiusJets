// Constants used by JetTagger Emulation

static inline uint32_t maskN(unsigned n) { return (n >= 32) ? 0xFFFFFFFFu : ((1u << n) - 1u); }
constexpr unsigned int nSeedsInput_ = 6;
constexpr unsigned int nSeedsOutput_ = 2;
constexpr unsigned int maxObjectsConsidered_ = 128;
constexpr double et_granularity_ = 0.125;
constexpr double min_et_seed_pos_recalc_ = 10.0;
constexpr double r2Cut_ = 1.2100;
constexpr double rMergeCut_ = 2.0000;
constexpr unsigned int et_bit_length_ = 13;
constexpr unsigned int eta_bit_length_ = 8;
constexpr unsigned int phi_bit_length_ = 6;
constexpr unsigned int psi_R_bit_length_ = 8;
constexpr unsigned int num_constituents_bit_length_ = 9;
constexpr unsigned int padded_zeroes_length_ = 64 - et_bit_length_ - eta_bit_length_ - phi_bit_length_ - psi_R_bit_length_ - num_constituents_bit_length_;
constexpr unsigned int total_bits_output_ = padded_zeroes_length_ + num_constituents_bit_length_ + psi_R_bit_length_ + et_bit_length_ + eta_bit_length_ + phi_bit_length_;
constexpr unsigned int deltaRBits_ = 8;
constexpr double phi_min_ = -3.2;
constexpr double phi_max_ = 3.2;
constexpr unsigned int pi_digitized_in_phi_ = 31;
const int PI_D     =  int(pi_digitized_in_phi_);
const int TWO_PI_D =  int(1 << phi_bit_length_);
constexpr double eta_min_ = -5.0;
constexpr double eta_max_ = 5.0;
constexpr double eta_granularity_ = 0.0390625;
constexpr double phi_granularity_ = 0.1;
constexpr unsigned int et_min_ = 0;
constexpr unsigned int et_max_ = 1024;
constexpr unsigned int max_R2lut_size_ = 964;
constexpr unsigned int max_Rlut_size_ = 1634;
constexpr double deltaR_max_ = 10.48187;
constexpr unsigned int nSeeds_ = 2;
constexpr unsigned int max_R_8b_lut_size_ = 898;
constexpr double phi_range_ = phi_max_ - phi_min_;

constexpr unsigned int lut_size_ = (1u << (eta_bit_length_ + phi_bit_length_));
constexpr unsigned int total_bits_ = padded_zeroes_length_ + psi_R_bit_length_ + et_bit_length_ + eta_bit_length_ + phi_bit_length_;

constexpr unsigned int phi_low_  = 0;
constexpr unsigned int phi_high_ = phi_low_ + phi_bit_length_ - 1;
constexpr unsigned int eta_low_  = phi_high_ + 1;
constexpr unsigned int eta_high_ = eta_low_ + eta_bit_length_ - 1;
constexpr unsigned int et_low_   = eta_high_ + 1;
constexpr unsigned int et_high_  = et_low_ + et_bit_length_ - 1;
constexpr unsigned int psi_R_low_  = et_high_ + 1;
constexpr unsigned int psi_R_high_ = psi_R_low_ + psi_R_bit_length_ - 1;
constexpr unsigned int num_constituents_low_  = psi_R_high_ + 1;
constexpr unsigned int num_constituents_high_ = num_constituents_low_ + num_constituents_bit_length_ - 1;
constexpr unsigned int padded_zeroes_low_  = num_constituents_high_ + 1;
constexpr unsigned int padded_zeroes_high_ = padded_zeroes_low_ + padded_zeroes_length_ - 1;
constexpr unsigned int nSeedsDeltaR_ = 4;//nSeedsInput_ - nSeedsOutput_;
static const bool lut_[max_R2lut_size_] =
#include "deltaR2LUT.h"
;

static const unsigned int lutR_[max_Rlut_size_] = 
#include "deltaRLUT.h"
;

static const unsigned int lutR_8b_[max_R_8b_lut_size_] = 
#include "deltaRLUT_8b.h"
;

constexpr unsigned int deltaR_levels_ = (1u << deltaRBits_); // 256
constexpr float deltaR_step_ = static_cast<float>(deltaR_max_) / (deltaR_levels_ - 1); // ~0.041
constexpr unsigned int rMergeConsiderCutDigitized_ = static_cast<unsigned int>(
    (rMergeCut_) / deltaR_step_ + 0.5f);

constexpr unsigned int psi_R_levels_ = (1u << psi_R_bit_length_); // 256
constexpr float psi_R_step_ = static_cast<float>(1) / (psi_R_levels_ - 1); // step in R^2 per code
