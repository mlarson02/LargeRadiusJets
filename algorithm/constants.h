#ifndef CONSTANTS_H
#define CONSTANTS_H
// Constants used by SW & FW implementation

#define UNROLLFACTOR 16
#define PIPELINEII 3

#define sortSeeds_ false
#define iterative_ false
constexpr unsigned int nTotalSeeds_ = 200;
constexpr unsigned int nSeedsInput_ = 6;
constexpr unsigned int nSeedsOutput_ = 2;
constexpr unsigned int maxObjectsConsidered_ = 128;
constexpr unsigned int inputEnergyCut_ = 1;
#define useInputEnergyCut_ false
constexpr double et_granularity_ = 0.125;
constexpr double r2Cut_ = 1.0;
constexpr double rCut_ = 1.0;
constexpr double rMergeCut_ = 0.001;
constexpr unsigned int et_bit_length_ = 13;
constexpr unsigned int eta_bit_length_ = 8;
constexpr unsigned int phi_bit_length_ = 6;
constexpr unsigned int psi_R_bit_length_ = 8;
constexpr unsigned int num_constituents_bit_length_ = 9;
constexpr unsigned int deltaRBits_ = 8;
constexpr double phi_min_ = -3.2;
constexpr double phi_max_ = 3.2;
constexpr unsigned int pi_digitized_in_phi_ = 31;
constexpr double eta_min_ = -5.0;
constexpr double eta_max_ = 5.0;
constexpr double eta_granularity_ = 0.0390625;
constexpr double phi_granularity_ = 0.1;
constexpr unsigned int et_min_ = 0;
constexpr unsigned int et_max_ = 1024;
#define useMax_ false
constexpr unsigned int max_R2lut_size_ = 803;
constexpr unsigned int max_Rlut_size_ = 1;
constexpr double deltaR_max_ = 10.48187;
constexpr unsigned int deltaR_bits_ = 8;
constexpr unsigned int max_R_8b_lut_size_ = 803;
constexpr double phi_range_ = 6.4;



const unsigned int lut_size_ = (1 << (eta_bit_length_ + phi_bit_length_));
#if !WRITE_LUT
constexpr unsigned int padded_zeroes_length_ = 64 - et_bit_length_ - eta_bit_length_ - phi_bit_length_ - psi_R_bit_length_;
constexpr unsigned int total_bits_ = padded_zeroes_length_ + num_constituents_bit_length_ + psi_R_bit_length_ + et_bit_length_ + eta_bit_length_ + phi_bit_length_;
typedef ap_uint<total_bits_> input;

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


constexpr unsigned int nSeedsDeltaR_ = nSeedsInput_ - nSeedsOutput_;

static const bool lut_[max_R2lut_size_] =
#include "../data/LUTs/deltaR2LUT.h"
;

static const ap_uint<deltaRBits_ > lutR_[max_Rlut_size_] = 
#include "../data/LUTs/deltaRLUT.h"
;

static const ap_uint<psi_R_bit_length_ > lutR_8b_[max_R_8b_lut_size_] = 
#include "../data/LUTs/deltaRLUT_8b.h"
;

#endif
constexpr unsigned int deltaR_levels_ = (1 << deltaR_bits_); // 256
constexpr float deltaR_step_ = deltaR_max_ / (deltaR_levels_ - 1); // ~0.041
constexpr unsigned int rMergeConsiderCutDigitized_ = (rMergeCut_) / deltaR_step_;

constexpr unsigned int diam_levels_ = (1 << psi_R_bit_length_); // 32
constexpr float diam_step_ = r2Cut_ / (diam_levels_ - 1); // ~0.041
        
#endif // CONSTANTS_H
