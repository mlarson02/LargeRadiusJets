#ifndef CONSTANTS_H
#define CONSTANTS_H
// Constants used by SW & FW implementation

#define UNROLLFACTOR 16
#define PIPELINEII 3

#define sortSeeds_ false
#define iterative_ false
constexpr unsigned int nTotalSeeds_ = 12;
constexpr unsigned int nSeeds_ = 2;
constexpr unsigned int maxObjectsConsidered_ = 128;
constexpr unsigned int inputEnergyCut_ = 1;
#define useInputEnergyCut_ false
constexpr double et_granularity_ = 0.25;
constexpr double r2Cut_ = 0.64;
constexpr unsigned int et_bit_length_ = 13;
constexpr unsigned int eta_bit_length_ = 8;
constexpr unsigned int phi_bit_length_ = 6;
constexpr double phi_min_ = -3.2;
constexpr double phi_max_ = 3.2;
constexpr double eta_min_ = -5.0;
constexpr double eta_max_ = 5.0;
constexpr double eta_granularity_ = 0.0390625;
constexpr double phi_granularity_ = 0.1;
constexpr unsigned int et_min_ = 0;
constexpr unsigned int et_max_ = 2048;
#define useMax_ false
constexpr unsigned int max_lut_size_ = 1282;
constexpr double phi_range_ = 6.4;



const unsigned int lut_size_ = (1 << (eta_bit_length_ + phi_bit_length_));
#if !WRITE_LUT

typedef ap_uint<et_bit_length_ + eta_bit_length_ + phi_bit_length_> input;
constexpr unsigned int total_bits = et_bit_length_ + eta_bit_length_ + phi_bit_length_;

static const bool lut_[max_lut_size_] =
#include "../data/LUTs/deltaR2Cut.h"
;

/*struct etEtaPhi {
ap_uint<et_bit_length_> et;
ap_uint<eta_bit_length_> eta;
ap_uint<phi_bit_length_> phi;
};*/
#endif
        
#endif // CONSTANTS_H
