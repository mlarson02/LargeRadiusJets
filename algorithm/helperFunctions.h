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
const std::string eosPath_ = "/eos/user/m/mlarson/LargeRadiusJets/MemPrints/";
const unsigned int maxEvent_ = 1000;
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

const bool useMax_ = false;


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
    //std::cout << "value : " << value << "\n";

    // Calculate scale factor
    double scale = (std::pow(2, bit_length)) / (max_val - min_val);

    //std::cout << "scale: " << scale << "\n";
    
    // Calculate the digitized value and round it
    int digitized_value = static_cast<int>(std::round((value - min_val) * scale));

    //std::cout << "digitized value: " << digitized_value << "\n";
    
    // Return the digitized value as a binary string (with leading zeros to fit the bit length)
    std::bitset<bit_length> bin(digitized_value);
    std::pair<std::string, unsigned int> string_dig_pair(bin.to_string(), digitized_value);
    return string_dig_pair; // Extract only the relevant bits
}

double calcDeltaR2(double eta1, double phi1, double eta2, double phi2) {
    //std::cout << "eta1: " << eta1 << " eta2: " << eta2 << " phi1: " << phi1 << " phi2: " << phi2 << "\n";
    double dEta = eta1 - eta2;
    double dPhi = TMath::Abs(phi1 - phi2);
    
    // Ensure Δφ is within [-π, π] range
    if (dPhi > TMath::Pi()) {
        dPhi = 2 * TMath::Pi() - dPhi;
    }
    //std::cout << "dr2: " << dEta * dEta + dPhi * dPhi << "\n";
    return dEta * dEta + dPhi * dPhi;
}

void redirect_output_to_file() {
    // Open the file in write mode
    freopen("output.txt", "w", stdout);  // This will redirect stdout to output.txt
}

bool energyMax(double et, double et_cutoff){
    return et <= et_cutoff;
}

bool energyMin(double et, double et_cutoff){
    return et >= et_cutoff;
}

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