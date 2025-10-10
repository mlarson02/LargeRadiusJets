// Constants used by JetTagger Emulation

static inline uint32_t maskN(unsigned n) { return (n >= 32) ? 0xFFFFFFFFu : ((1u << n) - 1u); }
constexpr unsigned int nSeedsInput_ = 6;
constexpr unsigned int nSeedsOutput_ = 2;
constexpr unsigned int maxObjectsConsidered_ = 128;
constexpr double et_granularity_ = 0.25;
constexpr double r2Cut_ = 1.0000;
constexpr double rMergeCut_ = 2.0000;
constexpr unsigned int et_bit_length_ = 13;
constexpr unsigned int eta_bit_length_ = 8;
constexpr unsigned int phi_bit_length_ = 6;
constexpr unsigned int diam_bit_length_ = 5;
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
constexpr unsigned int et_max_ = 2048;
constexpr unsigned int max_R2lut_size_ = 803;
constexpr unsigned int max_Rlut_size_ = 1634;
constexpr double deltaR_max_ = 10.48187;
constexpr unsigned int nSeeds_ = 2;
constexpr unsigned int max_R_5b_lut_size_ = 803;
constexpr double phi_range_ = phi_max_ - phi_min_;

constexpr unsigned int lut_size_ = (1u << (eta_bit_length_ + phi_bit_length_));
constexpr unsigned int total_bits_ = diam_bit_length_ + et_bit_length_ + eta_bit_length_ + phi_bit_length_;

constexpr unsigned int phi_low_  = 0;
constexpr unsigned int phi_high_ = phi_low_ + phi_bit_length_ - 1;

constexpr unsigned int eta_low_  = phi_high_ + 1;
constexpr unsigned int eta_high_ = eta_low_ + eta_bit_length_ - 1;

constexpr unsigned int et_low_   = eta_high_ + 1;
constexpr unsigned int et_high_  = et_low_ + et_bit_length_ - 1;

constexpr unsigned int diam_low_  = et_high_ + 1;
constexpr unsigned int diam_high_ = diam_low_ + diam_bit_length_ - 1;

constexpr unsigned int nSeedsDeltaR_ = nSeedsInput_ - nSeedsOutput_;

static const bool lut_[max_R2lut_size_] =
#include "deltaR2LUT.h"
;

static const unsigned int lutR_[max_Rlut_size_] = 
#include "deltaRLUT.h"
;

static const unsigned int lutR_5b_[max_R_5b_lut_size_] = 
#include "deltaRLUT_5b.h"
;

constexpr unsigned int deltaR_levels_ = (1u << deltaRBits_); // 256
constexpr float deltaR_step_ = static_cast<float>(deltaR_max_) / (deltaR_levels_ - 1); // ~0.041
constexpr unsigned int rMergeConsiderCutDigitized_ = static_cast<unsigned int>(
    (rMergeCut_) / deltaR_step_ + 0.5f);

constexpr unsigned int diam_levels_ = (1u << diam_bit_length_); // 32
constexpr float diam_step_ = static_cast<float>(r2Cut_) / (diam_levels_ - 1); // step in R^2 per code
