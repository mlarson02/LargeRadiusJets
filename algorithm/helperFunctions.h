#ifndef HELPER_FUNCTIONS_H  // Check if the macro is defined
#define HELPER_FUNCTIONS_H  // Define the macro
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <array>
#include <ap_int.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>  // For sprintf()




// Customizable parameters / global variables
//const std::string memPrintsPath_ = "/eos/user/m/mlarson/LargeRadiusJets/MemPrints/"; // FIXME define pre-compile statement if running on lxplus or millerlabml01
const std::string memPrintsPath_ = "/home/larsonma/LargeRadiusJets/data/MemPrints/";
const unsigned int maxEvent_ = 1000;
//const std::string fileName_ = "mc21_14TeV_hh_bbbb_vbf_novhh"; 
constexpr bool sortSeeds_ = true;
constexpr bool iterative_ = false;
constexpr unsigned int nTotalSeeds_ = 12;
constexpr unsigned int nSeeds_ = 2;
constexpr unsigned int maxObjectsConsidered_ = 1024; // -1 for no limit 
constexpr unsigned int inputEnergyCut_ = 1; // in GeV (only consider merging to seed if > this)
constexpr double et_granularity_ = 0.25;
constexpr double r2Cut_ = 1.0;
constexpr bool signalBool_ = false;

const std::string fileName_ = signalBool_ ? "mc21_14TeV_hh_bbbb_vbf_novhh" : "mc21_14TeV_jj_JZ3";


constexpr unsigned int et_bit_length_ = 13; 
constexpr unsigned int eta_bit_length_ = 11; 
constexpr unsigned int phi_bit_length_ = 8; 

constexpr double phi_min_ = -3.2;
constexpr double phi_max_ = 3.2;

constexpr double eta_min_ = -5.0;
constexpr double eta_max_ = 5.0;

constexpr int et_min_ = 0.0;
constexpr int et_max_ = 2048.0;

constexpr bool useMax_ = false;

inline void format_output(char* buffer, int buffer_size, int iSeed, int et_bin, int eta_bin, int phi_bin, int hexValue) {
    snprintf(buffer, buffer_size, 
             "0x%02X %d|%d|%d 0x%X", 
             iSeed, et_bin, eta_bin, phi_bin, hexValue);
}


// Helper functions
inline double undigitize_phi(const std::bitset<8>& phi_bits) {
    return phi_min_ + phi_bits.to_ulong() * (6.4 / 256.0);
}

inline double undigitize_eta(const std::bitset<11>& eta_bits) {
    return eta_min_ + eta_bits.to_ulong() * (10.0 / 2048.0);
}

inline double undigitize_et(const std::bitset<13>& et_bits) {
    return et_bits.to_ulong() * et_granularity_;
}

// Function to scale and digitize a value, returning the result as a binary string
template <int bit_length>
inline std::bitset<bit_length > digitize(double value, double min_val, double max_val) {
    // Ensure the value is within range
    if (value < min_val || value > max_val) {
        std::cerr << "Value is out of range" << "\n";
    }
    //std::cout << "value : " << value << "\n";

    // Calculate scale factor
    double scale = (std::pow(2, bit_length)) / (max_val - min_val);

    //std::cout << "scale: " << scale << "\n";
    
    // Calculate the digitized value and round it
    int digitized_value = static_cast<int>(std::round((value - min_val) * scale));

    //std::cout << "digitized value: " << digitized_value << "\n";
    
    // Return the digitized value as a binary string (with leading zeros to fit the bit length)
    std::bitset<bit_length > bin(digitized_value);
    //std::pair<std::bitset<bit_length >, unsigned int> string_dig_pair(bin, digitized_value);
    return bin; // Extract only the relevant bits
}

inline double calcDeltaR2(double eta1, double phi1, double eta2, double phi2) {
    //std::cout << "eta1: " << eta1 << " eta2: " << eta2 << " phi1: " << phi1 << " phi2: " << phi2 << "\n";
    double dEta = eta1 - eta2;
    double dPhi = std::abs(phi1 - phi2);
    
    // Ensure Δφ is within [-π, π] range
    if (dPhi > M_PI) {
        dPhi = 2 * M_PI - dPhi;
    }
    //std::cout << "dr2: " << dEta * dEta + dPhi * dPhi << "\n";
    return dEta * dEta + dPhi * dPhi;
}

inline void redirect_output_to_file() {
    // Open the file in write mode
    freopen("output.txt", "w", stdout);  // This will redirect stdout to output.txt
}

inline bool energyMax(double et, double et_cutoff){
    return et <= et_cutoff;
}

inline bool energyMin(double et, double et_cutoff){
    return et >= et_cutoff;
}
/* FIXME use compilation flag 
void SetPlotStyle();
void mySmallText(Double_t x, Double_t y, Color_t color, char* text);

void SetPlotStyle() {
    // from ATLAS plot style macro

    // use plain black on white colors
    gStyle->SetFrameBorderMode(0);
    gStyle->SetFrameFillColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadColor(0);
    gStyle->SetStatColor(0);
    gStyle->SetHistLineColor(1);

    gStyle->SetPalette(1);

    // set the paper & margin sizes
    gStyle->SetPaperSize(20, 26);
    gStyle->SetPadTopMargin(0.05);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadBottomMargin(0.16);
    gStyle->SetPadLeftMargin(0.16);

    // set title offsets (for axis label)
    gStyle->SetTitleXOffset(1.4);
    gStyle->SetTitleYOffset(1.4);

    // use large fonts
    gStyle->SetTextFont(42);
    gStyle->SetTextSize(0.07);
    gStyle->SetLabelFont(42, "x");
    gStyle->SetTitleFont(42, "x");
    gStyle->SetLabelFont(42, "y");
    gStyle->SetTitleFont(42, "y");
    gStyle->SetLabelFont(42, "z");
    gStyle->SetTitleFont(42, "z");
    gStyle->SetLabelSize(0.05, "x");
    gStyle->SetTitleSize(0.05, "x");
    gStyle->SetLabelSize(0.05, "y");
    gStyle->SetTitleSize(0.05, "y");
    gStyle->SetLabelSize(0.05, "z");
    gStyle->SetTitleSize(0.05, "z");

    // use bold lines and markers
    gStyle->SetMarkerStyle(20);
    gStyle->SetMarkerSize(1.2);
    gStyle->SetHistLineWidth(2.);
    gStyle->SetLineStyleString(2, "[12 12]");

    // get rid of error bar caps
    gStyle->SetEndErrorSize(0.);

    // do not display any of the standard histogram decorations
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);

    // put tick marks on top and RHS of plots
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
}

void mySmallText(Double_t x, Double_t y, Color_t color, char* text) {
    Double_t tsize = 0.044;
    TLatex l;
    l.SetTextSize(tsize);
    l.SetNDC();
    l.SetTextColor(color);
    l.DrawLatex(x, y, text);
}
    */

// new version for all events
template <unsigned int arraySize >
inline void extract_values_from_file(const std::string& fileName, std::array<std::array<double, 3>, arraySize >& values, unsigned int eventToProcess) {

    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }

    // Event iteration used for indexing
    int iEvt = -1; 
    //unsigned int lineIt = -1;
    std::string line;
    std::array<std::array<double, 3>, arraySize> valuesForEvent; 
    int lastEvent = 0; 
    int objectIt = -1;
    while (std::getline(inFile, line)) {
        //lineIt++; 
        int eventNumber;

        
        // Skip event header lines
        if (line.find("Event") != std::string::npos) {
            std::stringstream ss0(line);
            std::string temp;
            
            ss0 >> temp >> temp >> eventNumber;  // "Event :" is skipped, eventNumber is read
            //std::cout << "Event found: " << eventNumber << "and eventToProcess: " << eventToProcess << std::endl;
            if (iEvt >= 0 && iEvt == eventToProcess){
                //std::cout << "iEvt: " << iEvt << "\n";
                //std::cout << "event being processed: " << iEvt << "\n";
                values = valuesForEvent;
            }
            iEvt++;
            std::array<double, 3> zeroArray = {0.0, 0.0, 0.0};
            std::fill(std::begin(valuesForEvent), std::end(valuesForEvent), zeroArray);
            continue;
        }
        if (iEvt > eventToProcess) break;
        if (eventNumber == eventToProcess){
            if (lastEvent != eventToProcess) objectIt = -1;
            lastEvent = eventToProcess;
            objectIt++; 
            
            std::stringstream ss(line);
            std::string index, bin, hex_word;
            ss >> index >> bin >> hex_word;

            //std::cout << "index: " << index << " binary : " << bin << " hex_word: " << hex_word << "\n";

            // Validate line format
            size_t first_pipe = bin.find('|');
            size_t second_pipe = bin.rfind('|');
            if (first_pipe == std::string::npos || second_pipe == std::string::npos || first_pipe == second_pipe) {
                std::cerr << "Error: Malformed line -> " << line << std::endl;
                continue;
            }

            // Extract binary substrings
            std::string et_bin = bin.substr(0, first_pipe);
            std::string eta_bin = bin.substr(first_pipe + 1, second_pipe - first_pipe - 1);
            std::string phi_bin = bin.substr(second_pipe + 1);

            //try {
            // Convert binary to bitsets
            std::bitset<et_bit_length_> et_bits(et_bin);
            std::bitset<eta_bit_length_> eta_bits(eta_bin);
            std::bitset<phi_bit_length_> phi_bits(phi_bin);

            // Convert bitsets to floating-point values
            //std::cout << "Et bits: " << et_bits
            //          << ", Phi bits: " << phi_bits
            //          << ", Eta bits: " << eta_bits << std::endl;
            double et = undigitize_et(et_bits);
            double eta = undigitize_eta(eta_bits); // FIXME use digitized values in calculations, don't convert to double
            double phi = undigitize_phi(phi_bits);
            //std::cout << "Et: " << et
            //          << ", Phi: " << phi
            //          << ", Eta: " << eta << std::endl;

            // Store in vector
            //std::cout << "et " << et << "\n";
            valuesForEvent[objectIt]  = {et, eta, phi};
            //} 
            /*catch (const std::exception& e) {
                std::cerr << "Error processing line: " << line << " -> " << e.what() << std::endl;
            }*/
        }
        
    }
    if (maxEvent_ == 1000){
        values = valuesForEvent;
    }

    inFile.close();
    return;
}
#endif