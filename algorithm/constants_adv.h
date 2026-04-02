#ifndef CONSTANTS_ADV_H
#define CONSTANTS_ADV_H
// Constants used by SW & FW implementation

#define UNROLLFACTOR 16
#define PIPELINEII 3

constexpr unsigned int nTotalSeeds_ = 10;
constexpr unsigned int nSeedsInput_ = 6;
constexpr unsigned int nSeedsOutput_ = 2;
constexpr unsigned int maxObjectsConsidered_ = 128;
constexpr double et_granularity_ = 0.125;
constexpr unsigned int subjet_et_threshold_ = 201;
constexpr double r2Cut_ = 1.21;
constexpr double rCut_ = 1.1;
constexpr double rMergeCut_ = 2.0;
constexpr unsigned int et_bit_length_ = 13;
constexpr unsigned int eta_bit_length_ = 7;
constexpr unsigned int phi_bit_length_ = 6;
constexpr unsigned int eta_range_ = 98;
constexpr unsigned int num_subjets_length_ = 2;
constexpr unsigned int deltaRBits_ = 8;
constexpr double phi_min_ = -3.2;
constexpr double phi_max_ = 3.2;
constexpr unsigned int pi_digitized_in_phi_ = 31;
constexpr double eta_min_ = -4.9;
constexpr double eta_max_ = 4.9;
constexpr double eta_granularity_ = 0.1;
constexpr double phi_granularity_ = 0.1;
constexpr unsigned int et_min_ = 0;
constexpr unsigned int et_max_ = 1024;
constexpr double phi_range_ = 6.4;


constexpr unsigned int padded_zeroes_length_ = 64 - et_bit_length_ - eta_bit_length_ - phi_bit_length_ - num_subjets_length_ - num_subjets_length_;
constexpr unsigned int padded_zeroes_length_32b_ = 32 - et_bit_length_ - eta_bit_length_ - phi_bit_length_;
constexpr unsigned int total_bits_input_ = padded_zeroes_length_32b_ + et_bit_length_ + eta_bit_length_ + phi_bit_length_;
constexpr unsigned int total_bits_output_ = padded_zeroes_length_ + num_subjets_length_ + num_subjets_length_ + et_bit_length_ + eta_bit_length_ + phi_bit_length_;
typedef ap_uint<total_bits_input_> input; // need 32b input, 64b output!
typedef ap_uint<total_bits_output_> output;

constexpr unsigned int phi_low_  = 0;
constexpr unsigned int phi_high_ = phi_low_ + phi_bit_length_ - 1;

constexpr unsigned int eta_low_  = phi_high_ + 1;
constexpr unsigned int eta_high_ = eta_low_ + eta_bit_length_ - 1;

constexpr unsigned int et_low_   = eta_high_ + 1;
constexpr unsigned int et_high_  = et_low_ + et_bit_length_ - 1;

constexpr unsigned int num_subjets_low_  = et_high_ + 1;
constexpr unsigned int num_subjets_high_ = num_subjets_low_ + num_subjets_length_ - 1;

constexpr unsigned int padded_zeroes_low_  = num_subjets_high_ + 1;
constexpr unsigned int padded_zeroes_high_ = padded_zeroes_low_ + padded_zeroes_length_ - 1;

constexpr unsigned int nSeedsDeltaR_ = nSeedsInput_ - nSeedsOutput_;

constexpr double deltaR2_granularity_ = eta_granularity_ * eta_granularity_; // FIXME THIS SHOULD BE EQUIVALENT TO SQUARING PHI_GRANULARITY_ - maybe add an exception if they are not the same

constexpr unsigned int digitized_delta_R2Cut_ = static_cast<unsigned int>(r2Cut_/deltaR2_granularity_);

constexpr unsigned int digitized_d_search_squared_ = static_cast<unsigned int>(((rMergeCut_) * (rMergeCut_))/deltaR2_granularity_);

        
#endif // CONSTANTS_ADV_H
