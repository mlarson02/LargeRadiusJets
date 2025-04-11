
#ifndef CONSTANTS_H  // Check if the macro is defined
#define CONSTANTS_H  // Define the macro
// Constants used by SW & FW implementation 
// Customizable parameters / global variables
constexpr bool sortSeeds_ = true;
constexpr bool iterative_ = false;
constexpr unsigned int nTotalSeeds_ = 12;
constexpr unsigned int nSeeds_ = 2;
constexpr unsigned int maxObjectsConsidered_ = 1024; // -1 for no limit 
constexpr unsigned int inputEnergyCut_ = 1; // in GeV (only consider merging to seed if > this)
constexpr float et_granularity_ = 0.25;
constexpr float r2Cut_ = 1.0;

constexpr unsigned int et_bit_length_ = 13; 
constexpr unsigned int eta_bit_length_ = 11; 
constexpr unsigned int phi_bit_length_ = 8; 

constexpr float phi_min_ = -3.2;
constexpr float phi_max_ = 3.2;

constexpr float eta_min_ = -5.0;
constexpr float eta_max_ = 5.0;

constexpr int et_min_ = 0.0;
constexpr int et_max_ = 2048.0;

constexpr float phi_range_ = phi_max_ - phi_min_;
constexpr float phi_granularity_ = phi_range_ / (1 << phi_bit_length_);

constexpr float eta_range_ = eta_max_ - eta_min_; 
constexpr float eta_granularity_ = eta_range_ / (1 << eta_bit_length_);  

constexpr bool useMax_ = false;
#endif