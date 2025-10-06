
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <array>
#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include <cmath>
#include <TMath.h>
#include "TTree.h"
#include "TBranch.h"
#include "TSystem.h"
#include "TDirectory.h"
#include "TROOT.h"
#include <cstdio>
#include <filesystem>
#include "emulationHelperFunctions.h"

// Function for processing provided number of loops with JetTagger algorithm,
// then writing output large radius jets to a new TTree & text files
void eventLoop(std::string inputNTuplePath, std::string outputNTuplePath,std::string outputTextFilePath){

    // Relevant data for algorithm read from inputNTuple

    // CaloTopoTowers
    std::vector<double>* caloTopoTowerEtValues = nullptr;
    std::vector<double>* caloTopoTowerEtaValues = nullptr;
    std::vector<double>* caloTopoTowerPhiValues = nullptr;

    // Topo422 Clusters
    std::vector<double>* topo422EtValues = nullptr;
    std::vector<double>* topo422EtaValues = nullptr;
    std::vector<double>* topo422PhiValues = nullptr;

    // GEPBasicClusters
    std::vector<double>* gepBasicClustersEtValues = nullptr;
    std::vector<double>* gepBasicClustersEtaValues = nullptr;
    std::vector<double>* gepBasicClustersPhiValues = nullptr;

    // gFEX small radius jets
    std::vector<unsigned int>* gFexSRJEtIndexValues = nullptr;
    std::vector<double>* gFexSRJEtValues = nullptr;
    std::vector<double>* gFexSRJEtaValues = nullptr;
    std::vector<double>* gFexSRJPhiValues = nullptr;

    // jFEX small radius jets
    std::vector<unsigned int>* jFexSRJEtIndexValues = nullptr;
    std::vector<double>* jFexSRJEtValues = nullptr;
    std::vector<double>* jFexSRJEtaValues = nullptr;
    std::vector<double>* jFexSRJPhiValues = nullptr;

    // Output JetTagger large radius jets
    std::vector<double>* jetTaggerLRJDiamValues = nullptr;
    std::vector<double>* jetTaggerLRJEtValues = nullptr;
    std::vector<double>* jetTaggerLRJEtaValues = nullptr;
    std::vector<double>* jetTaggerLRJPhiValues = nullptr;

    // Output JetTagger leading large radius jets
    std::vector<double>* jetTaggerLeadingLRJDiamValues = nullptr;
    std::vector<double>* jetTaggerLeadingLRJEtValues = nullptr;
    std::vector<double>* jetTaggerLeadingLRJEtaValues = nullptr;
    std::vector<double>* jetTaggerLeadingLRJPhiValues = nullptr;

    // Output JetTagger subleading large radius jets
    std::vector<double>* jetTaggerSubleadingLRJDiamValues = nullptr;
    std::vector<double>* jetTaggerSubleadingLRJEtValues = nullptr;
    std::vector<double>* jetTaggerSubleadingLRJEtaValues = nullptr;
    std::vector<double>* jetTaggerSubleadingLRJPhiValues = nullptr;

    // Open input ROOT file
    TFile* inputFile = TFile::Open(inputNTuplePath.c_str(), "READ");
    if (!inputFile || inputFile->IsZombie()) {
        std::cerr << "Error: Could not open file " << inputNTuplePath << std::endl;
        return;
    }

    // Open output ROOT file - assume it has already been copied from the input ROOT file, use to write to a new tree
    TFile* outputFile = TFile::Open(outputNTuplePath.c_str(), "UPDATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error: Could not open file " << outputNTuplePath << std::endl;
        return;
    }

    // TTrees and branches required as input to algorithm
    TTree* caloTopoTowerTree = (TTree*)inputFile->Get("caloTopoTowerTree");
    TTree* topo422Tree = (TTree*)inputFile->Get("topo422Tree");
    TTree* gepBasicClustersTree = (TTree*)inputFile->Get("gepBasicClustersTree");
    TTree* gFexSRJTree = (TTree*)inputFile->Get("gFexSRJTree");
    TTree* jFexSRJTree = (TTree*)inputFile->Get("jFexSRJTree"); // Note that jFEX, gFEX trees are pre-sorted by Et in LRJNTupler.cc

    outputFile->cd();
    // Create new output TTrees to be written to the output file
    TTree* jetTaggerLRJs = new TTree("jetTaggerLRJsTree", "Tree storing event-wise Substructure variable, Et, Eta, Phi");
    jetTaggerLRJs->Branch("Diam", &jetTaggerLRJDiamValues);
    jetTaggerLRJs->Branch("Et", &jetTaggerLRJEtValues);
    jetTaggerLRJs->Branch("Eta", &jetTaggerLRJEtaValues);
    jetTaggerLRJs->Branch("Phi", &jetTaggerLRJPhiValues);

    TTree* jetTaggerLeadingLRJs = new TTree("jetTaggerLeadingLRJsTree", "Tree storing event-wise Substructure variable, Et, Eta, Phi");
    jetTaggerLeadingLRJs->Branch("Diam", &jetTaggerLeadingLRJDiamValues);
    jetTaggerLeadingLRJs->Branch("Et", &jetTaggerLeadingLRJEtValues);
    jetTaggerLeadingLRJs->Branch("Eta", &jetTaggerLeadingLRJEtaValues);
    jetTaggerLeadingLRJs->Branch("Phi", &jetTaggerLeadingLRJPhiValues);

    TTree* jetTaggerSubleadingLRJs = new TTree("jetTaggerSubleadingLRJsTree", "Tree storing event-wise Substructure variable, Et, Eta, Phi");
    jetTaggerSubleadingLRJs->Branch("Diam", &jetTaggerSubleadingLRJDiamValues);
    jetTaggerSubleadingLRJs->Branch("Et", &jetTaggerSubleadingLRJEtValues);
    jetTaggerSubleadingLRJs->Branch("Eta", &jetTaggerSubleadingLRJEtaValues);
    jetTaggerSubleadingLRJs->Branch("Phi", &jetTaggerSubleadingLRJPhiValues);


    // === caloTopoTowerTree ===
    caloTopoTowerTree->SetBranchAddress("Et", &caloTopoTowerEtValues);
    caloTopoTowerTree->SetBranchAddress("Eta", &caloTopoTowerEtaValues);
    caloTopoTowerTree->SetBranchAddress("Phi", &caloTopoTowerPhiValues);

    // === topo422Tree ===
    topo422Tree->SetBranchAddress("Et", &topo422EtValues);
    topo422Tree->SetBranchAddress("Eta", &topo422EtaValues);
    topo422Tree->SetBranchAddress("Phi", &topo422PhiValues);

    // === gepBasicClustersTree ===
    gepBasicClustersTree->SetBranchAddress("Et", &gepBasicClustersEtValues);
    gepBasicClustersTree->SetBranchAddress("Eta", &gepBasicClustersEtaValues);
    gepBasicClustersTree->SetBranchAddress("Phi", &gepBasicClustersPhiValues);

    // === gFexSRJTree ===
    gFexSRJTree->SetBranchAddress("EtIndex", &gFexSRJEtIndexValues);
    gFexSRJTree->SetBranchAddress("Et", &gFexSRJEtValues);
    gFexSRJTree->SetBranchAddress("Eta", &gFexSRJEtaValues);
    gFexSRJTree->SetBranchAddress("Phi", &gFexSRJPhiValues);

    // === jFexSRJTree ===
    jFexSRJTree->SetBranchAddress("EtIndex", &jFexSRJEtIndexValues);
    jFexSRJTree->SetBranchAddress("Et", &jFexSRJEtValues);
    jFexSRJTree->SetBranchAddress("Eta", &jFexSRJEtaValues);
    jFexSRJTree->SetBranchAddress("Phi", &jFexSRJPhiValues);

    std::ofstream f_output(outputTextFilePath);

    if (!f_output.is_open()) {
        std::cerr << "Error: One or more output files could not be opened for writing!" << std::endl;
        // handle error or return
    }

    // Event loop 
    unsigned int eventsToProcess = gepBasicClustersTree->GetEntries();
    for (unsigned int iEvt = 0; iEvt < eventsToProcess; iEvt++) {
        // Write event header to output text files
        f_output << "Event : " << std::dec << iEvt << "\n";

        std::bitset<diam_bit_length_> diam_bitset_array[nSeedsOutput_];
        std::bitset<et_bit_length_  > et_bitset_array[nSeedsOutput_];
        std::bitset<eta_bit_length_ > eta_bitset_array[nSeedsOutput_];
        std::bitset<phi_bit_length_ > phi_bitset_array[nSeedsOutput_];

        // Clear vectors for branches
        jetTaggerLRJDiamValues->clear();
        jetTaggerLRJEtValues->clear();
        jetTaggerLRJEtaValues->clear();
        jetTaggerLRJPhiValues->clear();

        jetTaggerLeadingLRJDiamValues->clear();
        jetTaggerLeadingLRJEtValues->clear();
        jetTaggerLeadingLRJEtaValues->clear();
        jetTaggerLeadingLRJPhiValues->clear();

        jetTaggerSubleadingLRJDiamValues->clear();
        jetTaggerSubleadingLRJEtValues->clear();
        jetTaggerSubleadingLRJEtaValues->clear();
        jetTaggerSubleadingLRJPhiValues->clear();


        gepBasicClustersTree->GetEntry(iEvt); // get input object data
        jFexSRJTree->GetEntry(iEvt); // get seed data

        unsigned int seedValues[nSeedsInput_][3] = {0}; // 3 = number of data types (Et, Eta, Phi)
        unsigned int inputObjectValues[maxObjectsConsidered_][3] ={0};
        unsigned int outputJetValues[nSeedsOutput_][4] = {0}; // 4 = number of data types (Substructure, Eta, Phi)
        //std::cout << "rMergeConsiderCutDigitized_ : " << rMergeConsiderCutDigitized_ << "\n";
        // Digitize and format data in same way expected for running in HLS
        for(unsigned int iSeed = 0; iSeed < nSeedsInput_; iSeed++){
            if(jFexSRJEtValues->size() < nSeedsInput_){
                if(iSeed >= jFexSRJEtValues->size()){
                    seedValues[iSeed][0] = 0;
                    seedValues[iSeed][1] = 0;
                    seedValues[iSeed][2] = 0;
                    continue;
                }
            }
            seedValues[iSeed][0] = digitize(jFexSRJEtValues->at(iSeed), et_bit_length_,
                              static_cast<double>(et_min_), static_cast<double>(et_max_));
            seedValues[iSeed][1] = digitize(jFexSRJEtaValues->at(iSeed), eta_bit_length_, eta_min_, eta_max_);
            seedValues[iSeed][2] = digitize(jFexSRJPhiValues->at(iSeed), phi_bit_length_, phi_min_, phi_max_);
            if (iSeed < 2){
                //std::cout << "seed et: " << std::dec << seedValues[iSeed][0] << " eta: " << seedValues[iSeed][1] << " phi: " << seedValues[iSeed][2] << "\n";
                //std::cout << "UNDIGITIZED Et: " << jFexSRJEtValues->at(iSeed) << " eta: " << jFexSRJEtaValues->at(iSeed) << " and phi: " << jFexSRJPhiValues->at(iSeed) << "\n";
            }
        }
        for(unsigned int iIO = 0; iIO < maxObjectsConsidered_; iIO++){
            inputObjectValues[iIO][0] = digitize(gepBasicClustersEtValues->at(iIO), et_bit_length_,
                              static_cast<double>(et_min_), static_cast<double>(et_max_));
            inputObjectValues[iIO][1] = digitize(gepBasicClustersEtaValues->at(iIO), eta_bit_length_, eta_min_, eta_max_);
            inputObjectValues[iIO][2] = digitize(gepBasicClustersPhiValues->at(iIO), phi_bit_length_, phi_min_, phi_max_);
            //std::cout << "iInput: " << iIO << "\n";
            //std::cout << "input et: " << std::dec << inputObjectValues[iIO][0] << " eta: " << inputObjectValues[iIO][1] << " phi: " << inputObjectValues[iIO][2] << "\n";
            //std::cout << "UNDIGITIZED Et: " << gepBasicClustersEtValues->at(iIO) << " eta: " << gepBasicClustersEtaValues->at(iIO) << " and phi: " << gepBasicClustersPhiValues->at(iIO) << "\n";
        }

        // Pasted in HLS algorithm

    
        // FIXME make this entire process more dynamic to account for nSeedsOutput_ != 2 (progressively do this for highest Et seeds rather than for 1st 2 seeds immediately)
        // NEW PRE-PROCESSING OF SEEDS - SELECT IN BETWEEN LEADING, SUBLEADING JFEX SRJ, CLOSEST OF 3rd - 6th highest ENERGY JFEX SRJS as NEW SEEDS
        unsigned int deltaRValuesSeed[nSeedsOutput_][nSeedsDeltaR_] = {0}; 
        unsigned int deltaRValuesGreaterThan5Counter[nSeedsOutput_] = {0};

        bool indicesOfDeltaRValuesGreaterThanrMergeCut[nSeedsOutput_][nSeedsDeltaR_] = {false};
        for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; iSeed++){ // loop through and calculate deltaR between leading, subleading & 3rd - 6th highest Et JFEX SRJ
            for (unsigned int iPreSeed = 0; iPreSeed < nSeedsDeltaR_; iPreSeed++){
                deltaRValuesSeed[iSeed][iPreSeed] = (1 << deltaRBits_) - 1; // Set to maximum value in case closer seed not found --> don't consider 0 values in sorting
                int deltaEta = seedValues[iSeed][1] - seedValues[iPreSeed + nSeedsOutput_][1];
                int deltaPhi = seedValues[iSeed][2] - seedValues[iPreSeed + nSeedsOutput_][2];
                // Use unsigned type for absolute values, and ensure both operands are of the same type
                unsigned int uDeltaEta = std::abs(deltaEta); 
                unsigned int uDeltaPhi = std::abs(deltaPhi);
                if (uDeltaPhi >= pi_digitized_in_phi_) uDeltaPhi = 2 * pi_digitized_in_phi_ - uDeltaPhi;
                unsigned int corrDeltaPhi = uDeltaPhi; // wrapped around positive deltaPhi
                unsigned int lutR_index = uDeltaEta * (1 << (phi_bit_length_ - 1) ) + corrDeltaPhi; // Compute lut index
                //std::cout << "lutR_index: " << lutR_index << "\n";
                if (!(lutR_index >= max_Rlut_size_)){
                    deltaRValuesSeed[iSeed][iPreSeed] = lutR_[lutR_index];
                    //std::cout << "lutR_[lutR_index]: " <<  lutR_[lutR_index] << "\n";
                    if (lutR_[lutR_index] <= rMergeConsiderCutDigitized_){
                        //std::cout << "lutR_[lutR_index] in cut: " << lutR_[lutR_index] << "\n";
                        deltaRValuesGreaterThan5Counter[iSeed]++;
                        indicesOfDeltaRValuesGreaterThanrMergeCut[iSeed][iPreSeed] = true;
                    }
                }
            }
        }

        unsigned int indices[nSeedsOutput_] = {0};
        // Account for the case when multiple seeds are within deltaR customalizable value - use highest Et seed to compute halfway point
        // For seed 0
        //std::cout << "deltaRValuesGreaterThan5Counter[0] : " << deltaRValuesGreaterThan5Counter[0] << "\n";
        //std::cout << "deltaRValuesGreaterThan5Counter[1] : " << deltaRValuesGreaterThan5Counter[1] << "\n";
        if (deltaRValuesGreaterThan5Counter[0] > 1) {
            unsigned int maxEt = 0;
            for (unsigned int iPreSeed = 0; iPreSeed < nSeedsDeltaR_; iPreSeed++) {
                if (indicesOfDeltaRValuesGreaterThanrMergeCut[0][iPreSeed]) {
                    unsigned int et = seedValues[iPreSeed + nSeedsOutput_][0];
                    if (et > maxEt) {
                        maxEt = et;
                        indices[0] = iPreSeed;
                    }
                }
            }
        }
        else{
            indices[0] = index_of_min(deltaRValuesSeed[0]);
        }

        // For seed 1
        if (deltaRValuesGreaterThan5Counter[1] > 1) {
            unsigned int maxEt = 0;
            for (unsigned int iPreSeed = 0; iPreSeed < nSeedsDeltaR_; iPreSeed++) {
                if (indicesOfDeltaRValuesGreaterThanrMergeCut[1][iPreSeed]) {
                    unsigned int et = seedValues[iPreSeed + nSeedsOutput_][0];
                    if (et > maxEt) {
                        maxEt = et;
                        indices[1] = iPreSeed;
                    }
                }
            }
        }
        else{
            indices[1] = index_of_min(deltaRValuesSeed[1]);
        }

        // FIXME replace with a templated version of this s.t. you don't need to rewrite code for different number sorted
        

        // next update leading, subleading seed positions (and energy?) to be in between closest other seed 
        for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; iSeed++){
            if (deltaRValuesSeed[iSeed][indices[iSeed]] > rMergeConsiderCutDigitized_) continue; // If no other proto-seeds found within rMergeCut

            /*auto wrapSym = [&](ap_int<phi_bit_length_ + 2> x) -> ap_int<phi_bit_length_ + 2> {
                if (x >  PI_D)   x -= TWO_PI_D;
                if (x < -PI_D)  x += TWO_PI_D;
                return x;
            };*/

            // Get Et values for the two seeds (assumed already extracted)
            //ap_uint<et_bit_length_> et1 = seedValues[iSeed].range(et_high_, et_low_);
            //ap_uint<et_bit_length_> et2 = seedValues[indices[iSeed] + nSeedsOutput_].range(et_high_, et_low_);

            // Promote to wider type to avoid overflow
            //ap_uint<et_bit_length_ + 1> etSum = et1 + et2;
            //if (etSum == 0) etSum = 1; // avoid divide-by-zero

            // Convert eta to signed integers centered at 0
            int eta1 = seedValues[iSeed][1] - (1 << (eta_bit_length_ - 1));
            int eta2 = seedValues[indices[iSeed] + nSeedsOutput_][1] - (1 << (eta_bit_length_ - 1));

            // Convert phi to signed integers centered at 0

            // Signed, extended-width versions to avoid overflow on sums/diffs

            int phi1s = seedValues[iSeed][2] - (1 << (phi_bit_length_ - 1));
            int phi2s = seedValues[indices[iSeed] + nSeedsOutput_][2] - (1 << (phi_bit_length_ - 1));

            // --- Shortest-arc midpoint (unweighted) ---
            int dphi = phi2s - phi1s;

            dphi = wrapSym(dphi);                          // now in [-PI, PI)

            int phi_mid = phi1s + (dphi >> 1);  // arithmetic shift (divide by 2)

            phi_mid = wrapSym(phi_mid);

            // --- Compute midpoint in signed domain ---
            int eta_mid = (eta1 + eta2) >> 1; // Integer division by 2
            //ap_int<phi_bit_length_ + 1> phi_mid = (phi1 + phi2) >> 1;
            //ap_int<eta_bit_length_ + 3> eta_mid_weighted = (et2 * eta2 + et1 * eta1) / etSum;
            //ap_int<phi_bit_length_ + 3> phi_mid_weighted = (et2 * phi2 + et1 * phi1) / etSum;
            
            // --- Optional: re-wrap phi midpoint to [-π, π) range if needed ---

            //if (phi_mid > pi_digitized_in_phi_) 
            //    phi_mid -= (1 << phi_bit_length_);
            //if (phi_mid < -(pi_digitized_in_phi_ + 1)) 
            //    phi_mid += (1 << phi_bit_length_);
            
            //if (phi_mid_weighted > pi_digitized_in_phi_)
            //    phi_mid_weighted -= (1 << phi_bit_length_);
            //if (phi_mid_weighted < -(pi_digitized_in_phi_ + 1))
            //    phi_mid_weighted += (1 << phi_bit_length_);

        //std::cout << "eta mid : " << eta_mid << " phi_mid: " << phi_mid << "\n";

            // --- Convert midpoints back to digitized unsigned format ---
            //ap_uint<eta_bit_length_> eta_mid_digitized = eta_mid_weighted + (1 << (eta_bit_length_ - 1));
            //ap_uint<phi_bit_length_> phi_mid_digitized = phi_mid_weighted + (1 << (phi_bit_length_ - 1));
            unsigned int eta_mid_digitized = eta_mid + (1 << (eta_bit_length_ - 1));
            unsigned int phi_mid_digitized = phi_mid + (1 << (phi_bit_length_ - 1));

            seedValues[iSeed][1] = eta_mid_digitized; // FIXME replace 1, 2 with etaIndexInput, phiIndexInput
            seedValues[iSeed][2] = phi_mid_digitized;
        }
        
        for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; ++iSeed){ // FIXME no longer considering highest Et seed first (need to implement some sorting)
            unsigned int outputJetEt = 0;
            unsigned int numMergedIO = 0; 
            unsigned int highestEtMergedIOEta[2];
            unsigned int highestEtMergedIOPhi[2];

            for (unsigned int iInput = 0; iInput < maxObjectsConsidered_; ++iInput){ // loop through input objects to consider merging

                // Calculate signed differences (deltaEta and deltaPhi)
                //std::cout << "-----------------------------" << "\n";
                //std::cout << "iSeed: " << iSeed << "\n";
                //std::cout << "seed eta: " << seedValues[iSeed][1] << " and phi: " << seedValues[iSeed][2] << "\n";
                int deltaEta = seedValues[iSeed][1] - inputObjectValues[iInput][1];
                int deltaPhi = seedValues[iSeed][2] - inputObjectValues[iInput][2];
                
                // Use unsigned type for absolute values, and ensure both operands are of the same type
                unsigned int uDeltaEta = std::abs(deltaEta);
                unsigned int uDeltaPhi = std::abs(deltaPhi);

                if (uDeltaPhi >= pi_digitized_in_phi_) uDeltaPhi = 2 * pi_digitized_in_phi_ - uDeltaPhi;
                unsigned int corrDeltaPhi = uDeltaPhi; // using corr delta phi saves 1 bit, unsure if necessary?

                unsigned int lut_index = uDeltaEta * (1 << (phi_bit_length_ - 1) ) + corrDeltaPhi; // Calculate LUT index corresponding to whether input object passes R^2 cut

                if (!(lut_index >= max_R2lut_size_) && lut_[lut_index]){ // only consider if lut index is smaller than max size (past max size, all values are False)
                    outputJetEt += inputObjectValues[iInput][0]; // add input object Et to seed Et for resultant output jet Et
                    if(numMergedIO <= 1){
                        highestEtMergedIOEta[numMergedIO] = inputObjectValues[iInput][1];
                        highestEtMergedIOPhi[numMergedIO] = inputObjectValues[iInput][2];
                    }
                    numMergedIO++; 
                }
            }

            // Compute delta R between the two highest Et topoclusters merged to a large R jet FIXME need to account for case when 0 or 1 input objects are merged! if statement should be fine for now?
            unsigned int jet_diam = 0;
            if(numMergedIO > 1){
                int deltaEta_mergedIO = highestEtMergedIOEta[0] - highestEtMergedIOEta[1];
                int deltaPhi_mergedIO = highestEtMergedIOPhi[0] - highestEtMergedIOPhi[1];

                // Use unsigned type for absolute values, and ensure both operands are of the same type
                unsigned int uDeltaEta_mergedIO = std::abs(deltaEta_mergedIO);
                unsigned int uDeltaPhi_mergedIO = std::abs(deltaPhi_mergedIO);

                if (uDeltaPhi_mergedIO >= pi_digitized_in_phi_) uDeltaPhi_mergedIO = 2 * pi_digitized_in_phi_ - uDeltaPhi_mergedIO;
                unsigned int corrDeltaPhi_mergedIO = uDeltaPhi_mergedIO; // using corr delta phi saves 1 bit, unsure if necessary?
                unsigned int lut_index_mergedIO = uDeltaEta_mergedIO * (1 << (phi_bit_length_ - 1) ) + corrDeltaPhi_mergedIO;
                if(lut_index_mergedIO < max_R_5b_lut_size_) jet_diam = lutR_5b_[lut_index_mergedIO];
            }

            // Values you already computed:
            uint32_t diam_value = static_cast<uint32_t>(jet_diam)        & maskN(diam_bit_length_);
            //std::cout << "dim_value: " << diam_value << "\n";
            uint32_t et_value   = static_cast<uint32_t>(outputJetEt)      & maskN(et_bit_length_);
            // NOTE: this mirrors your original snippet exactly (eta_bitset took seedValues[iSeed][1] and phi_bitset took [2]).
            uint32_t eta_value  = static_cast<uint32_t>(seedValues[iSeed][1]) & maskN(eta_bit_length_);
            uint32_t phi_value  = static_cast<uint32_t>(seedValues[iSeed][2]) & maskN(phi_bit_length_);

            //std::cout << "jet_diam: " << jet_diam << " and output jet Et: " << outputJetEt << "\n";
        //std::cout << "output jet eta, phi: " << seedValues[iSeed][1] << " , " << seedValues[iSeed][2] << "\n";

            // Set output values (unchanged semantic layout)
            outputJetValues[iSeed][0] = diam_value; // substructure
            outputJetValues[iSeed][1] = et_value;   // Et
            outputJetValues[iSeed][2] = seedValues[iSeed][1]; // phi
            outputJetValues[iSeed][3] = seedValues[iSeed][2]; // eta

            // Bitset strings (unchanged)
            std::bitset<diam_bit_length_> diam_bitset(diam_value);
            std::bitset<et_bit_length_  > et_bitset(et_value);
            std::bitset<eta_bit_length_ > eta_bitset(eta_value);
            std::bitset<phi_bit_length_ > phi_bitset(phi_value);

            

            diam_bitset_array[iSeed] = diam_bitset;
            et_bitset_array[iSeed] = et_bitset;
            eta_bitset_array[iSeed] = eta_bitset;
            phi_bitset_array[iSeed] = phi_bitset;

            // Pack into one word: [diam | et | eta | phi] (same field order as your ap_uint expression)
            uint32_t combined_value =
                ((diam_value & maskN(diam_bit_length_)) << (et_bit_length_ + eta_bit_length_ + phi_bit_length_)) |
                ((et_value   & maskN(et_bit_length_  )) << (eta_bit_length_ + phi_bit_length_)) |
                ((eta_value  & maskN(eta_bit_length_ )) <<  phi_bit_length_) |
                ( phi_value  & maskN(phi_bit_length_ ));

                std::cout << "combined_value : " << std::hex << combined_value << "\n";
            // Hex string with width = total_bits_/4 (=8 for 32 bits)
            std::stringstream hex_stream;
            hex_stream << std::hex << std::nouppercase << std::setfill('0') << std::setw(total_bits_ / 4) << combined_value;
            std::string hexValue = hex_stream.str();

            // Final print (identical shape)
            f_output << "0x" << std::hex << std::setw(2) << std::setfill('0') << iSeed
                    << " "  << diam_bitset.to_string() << "|" << et_bitset.to_string()
                    << "|"  << eta_bitset.to_string()  << "|" << phi_bitset.to_string()
                    << " 0x" << hexValue << std::endl;

            // Write data to all LRJ branch
            jetTaggerLRJDiamValues->push_back(undigitize_diam(diam_bitset));
            jetTaggerLRJEtValues->push_back(undigitize_et(et_bitset));
            jetTaggerLRJEtaValues->push_back(undigitize_eta(eta_bitset));
            jetTaggerLRJPhiValues->push_back(undigitize_phi(phi_bitset));
        } // Loop through seeds 
        // Write data to leading, subleading jet branches
        if(outputJetValues[0][1] >= outputJetValues[1][1]){ // to ensure leading, subleading sorted correctly
            jetTaggerLeadingLRJDiamValues->push_back(undigitize_diam(diam_bitset_array[0]));
            jetTaggerLeadingLRJEtValues->push_back(undigitize_et(et_bitset_array[0]));
            jetTaggerLeadingLRJEtaValues->push_back(undigitize_eta(eta_bitset_array[0]));
            jetTaggerLeadingLRJPhiValues->push_back(undigitize_phi(phi_bitset_array[0]));

            jetTaggerSubleadingLRJDiamValues->push_back(undigitize_diam(diam_bitset_array[1]));
            jetTaggerSubleadingLRJEtValues->push_back(undigitize_et(et_bitset_array[1]));
            jetTaggerSubleadingLRJEtaValues->push_back(undigitize_eta(eta_bitset_array[1]));
            jetTaggerSubleadingLRJPhiValues->push_back(undigitize_phi(phi_bitset_array[1]));
        }
        else{
            jetTaggerLeadingLRJDiamValues->push_back(undigitize_diam(diam_bitset_array[1]));
            jetTaggerLeadingLRJEtValues->push_back(undigitize_et(et_bitset_array[1]));
            jetTaggerLeadingLRJEtaValues->push_back(undigitize_eta(eta_bitset_array[1]));
            jetTaggerLeadingLRJPhiValues->push_back(undigitize_phi(phi_bitset_array[1]));

            jetTaggerSubleadingLRJDiamValues->push_back(undigitize_diam(diam_bitset_array[0]));
            jetTaggerSubleadingLRJEtValues->push_back(undigitize_et(et_bitset_array[0]));
            jetTaggerSubleadingLRJEtaValues->push_back(undigitize_eta(eta_bitset_array[0]));
            jetTaggerSubleadingLRJPhiValues->push_back(undigitize_phi(phi_bitset_array[0]));
        }
        

        // Fill branch to output ntuple with 
        jetTaggerLRJs->Fill();
        jetTaggerLeadingLRJs->Fill();
        jetTaggerSubleadingLRJs->Fill();



    } // Loop through events
    // Close output files
    outputFile->cd();
    jetTaggerLRJs->Write("", TObject::kOverwrite);
    jetTaggerLeadingLRJs->Write("", TObject::kOverwrite);
    jetTaggerSubleadingLRJs->Write("", TObject::kOverwrite);
    outputFile->Close();
    inputFile->Close();

} // Event loop function


// Function for processing a different algorithm configuration
// Use(compiling, running with ROOT): .L jetTaggerEmulation.cc, jetTaggerEmulation(0.001, 128, 2, 1.0, true, 3, false)
void jetTaggerEmulation(double rMergeCut, // Distance in r-phi plane to look for other jFEX SRJs (from leading, subleading jets)
                                        // to recalculate seed position by taking midpoint between the two jets
                                        // to disable recalculation of seed position, set to 0.001 
                      unsigned int numberIOs, // number of input objects (clusters or towers) considered for merging to seeds
                      unsigned int nSeeds, // Number of seeds and consequently number of output jets
                      double RSquaredCut, // radius squared of output jets
                      bool signalBool,  // whether processing a signal or background process // FIXME add additional signal types for later
                      bool condorBool ){ // whether running using condor batch job submission (requires change in filepaths)
    // Construct input and output ntuple, LUT paths based on configuration type
    auto infile = makeInputFileName(signalBool); // FIXME update this when running with condor.
    auto outntuplefile = makeOutputFileName(rMergeCut, numberIOs, nSeeds, RSquaredCut, signalBool);
    auto outtextfile = makeOutputTextFileName(rMergeCut, numberIOs, nSeeds, RSquaredCut, signalBool);
    std::cout << "infile: " << infile << "\n";
    std::cout << "outntuplefile: " << outntuplefile << "\n"; 
    std::cout << "outtextfile: " << outtextfile << "\n";       
    try { // FIXME update this when running with condor.
        std::filesystem::copy_file(infile, outntuplefile, 
                      std::filesystem::copy_options::overwrite_existing);
        std::cout << "File copied successfully\n";
    } catch (std::filesystem::filesystem_error& e) {
        std::cerr << "Copy failed: " << e.what() << '\n';
    }            
    gSystem->RedirectOutput("debuglog.log", "w");
    eventLoop(infile, outntuplefile, outtextfile);
    gSystem->Exit(0);
} 
