#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>
#include <vector>
#include <algorithm>
#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include <cmath>
#include <TMath.h>

// Customizable parameters / global variables
const unsigned int maxEvent_ = 5;
//const std::string fileName_ = "mc21_14TeV_hh_bbbb_vbf_novhh"; 
const bool sortSeeds_ = true;
const bool iterative_ = false;
const unsigned int nSeeds_ = 2;
const unsigned int maxObjectsConsidered_ = -1; // -1 for no limit 
const unsigned int inputEnergyCut_ = 1; // in GeV (only consider merging to seed if > this)
const double et_granularity_ = 0.25;
const double r2Cut_ = 1.0;
const bool signalBool_ = false;

const std::string fileName_ = signalBool_ ? "mc21_14TeV_hh_bbbb_vbf_novhh" : "mc21_14TeV_jj_JZ3";


const unsigned int et_bit_length_ = 13; 
const unsigned int eta_bit_length_ = 11; 
const unsigned int phi_bit_length_ = 8; 

const double phi_min_ = -3.2;
const double phi_max_ = 3.2;

const double eta_min_ = -5.0;
const double eta_max_ = 5.0;

const int et_min_ = 0.0;
const int et_max_ = 2048.0;


// Helper functions
double undigitize_phi(const std::bitset<8>& phi_bits) {
    return phi_min_ + phi_bits.to_ulong() * (6.4 / 256.0);
}

double undigitize_eta(const std::bitset<11>& eta_bits) {
    return eta_min_ + eta_bits.to_ulong() * (10.0 / 2048.0);
}

double undigitize_et(const std::bitset<13>& et_bits) {
    return et_bits.to_ulong() * et_granularity_;
}

// Function to scale and digitize a value, returning the result as a binary string
template <int bit_length>
std::pair<std::string, unsigned int> digitize(double value, double min_val, double max_val) {
    // Ensure the value is within range
    if (value < min_val || value > max_val) {
        throw std::out_of_range("Value is out of range");
    }
    std::cout << "value : " << value << "\n";

    // Calculate scale factor
    double scale = (std::pow(2, bit_length)) / (max_val - min_val);

    std::cout << "scale: " << scale << "\n";
    
    // Calculate the digitized value and round it
    int digitized_value = static_cast<int>(std::round((value - min_val) * scale));

    std::cout << "digitized value: " << digitized_value << "\n";
    
    // Return the digitized value as a binary string (with leading zeros to fit the bit length)
    std::bitset<bit_length> bin(digitized_value);
    std::pair<std::string, unsigned int> string_dig_pair(bin.to_string(), digitized_value);
    return string_dig_pair; // Extract only the relevant bits
}

double calcDeltaR2(double eta1, double phi1, double eta2, double phi2) {
    std::cout << "eta1: " << eta1 << " eta2: " << eta2 << " phi1: " << phi1 << " phi2: " << phi2 << "\n";
    double dEta = eta1 - eta2;
    double dPhi = TMath::Abs(phi1 - phi2);
    
    // Ensure Δφ is within [-π, π] range
    if (dPhi > TMath::Pi()) {
        dPhi = 2 * TMath::Pi() - dPhi;
    }
    std::cout << "dr2: " << dEta * dEta + dPhi * dPhi << "\n";
    return dEta * dEta + dPhi * dPhi;
}

void redirect_output_to_file() {
    // Open the file in write mode
    freopen("output.txt", "w", stdout);  // This will redirect stdout to output.txt
}