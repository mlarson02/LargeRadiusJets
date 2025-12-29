
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
void eventLoop(std::string inputNTuplePath, std::string outputNTuplePath,std::string outputTextFilePath, std::string inputObjectType, std::string seedObjectType){

    //std::cout << "test 1 " << "\n";
    const bool enableEtWeighting = true;
    // Relevant data for algorithm read from inputNTuple

    // CaloTopoTowers
    std::vector<double>* caloTopoTowerEtValues = nullptr;
    std::vector<double>* caloTopoTowerEtaValues = nullptr;
    std::vector<double>* caloTopoTowerPhiValues = nullptr;

    // Topo422 Clusters
    std::vector<double>* topo422EtValues = nullptr;
    std::vector<double>* topo422EtaValues = nullptr;
    std::vector<double>* topo422PhiValues = nullptr;

    // GEPCellsTowers
    std::vector<double>* gepCellsTowersEtValues = nullptr;
    std::vector<double>* gepCellsTowersEtaValues = nullptr;
    std::vector<double>* gepCellsTowersPhiValues = nullptr;

    // GEPBasicClusters
    std::vector<double>* gepBasicClustersEtValues = nullptr;
    std::vector<double>* gepBasicClustersEtaValues = nullptr;
    std::vector<double>* gepBasicClustersPhiValues = nullptr;

    // GEPBasicTopoTowers
    std::vector<double>* gepBasicTopoTowersEtValues = nullptr;
    std::vector<double>* gepBasicTopoTowersEtaValues = nullptr;
    std::vector<double>* gepBasicTopoTowersPhiValues = nullptr;

    // Cone jets from TrigGepPerf
    std::vector<double>* gepWTAConeCellsTowersJetspTValues = nullptr;
    std::vector<double>* gepWTAConeCellsTowersJetsEtaValues = nullptr;
    std::vector<double>* gepWTAConeCellsTowersJetsPhiValues = nullptr;
    std::vector<double>* gepWTAConeCellsTowersJetsMassValues = nullptr;
    std::vector<unsigned int >* gepWTAConeCellsTowersJetsNConstituentsValues = nullptr;

    /*std::vector<double>* gepConeGEPBasicClustersJetspTValues = nullptr;
    std::vector<double>* gepConeGEPBasicClustersJetsEtaValues = nullptr;
    std::vector<double>* gepConeGEPBasicClustersJetsPhiValues = nullptr;
    std::vector<double>* gepConeGEPBasicClustersJetsMassValues = nullptr;
    std::vector<unsigned int >* gepConeGEPBasicClustersJetsNConstituentsValues = nullptr;*/

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
    std::vector<double>* jetTaggerLRJPsi_RValues = nullptr;
    std::vector<unsigned int >* jetTaggerLRJMergedIndicesValues = nullptr;
    std::vector<double>* jetTaggerLRJEtValues = nullptr;
    std::vector<double>* jetTaggerLRJEtaValues = nullptr;
    std::vector<double>* jetTaggerLRJPhiValues = nullptr;

    // Output JetTagger leading large radius jets
    std::vector<double>* jetTaggerLeadingLRJPsi_RValues = nullptr;
    std::vector<unsigned int >* jetTaggerLeadingLRJMergedIndicesValues = nullptr;
    std::vector<double>* jetTaggerLeadingLRJEtValues = nullptr;
    std::vector<double>* jetTaggerLeadingLRJEtaValues = nullptr;
    std::vector<double>* jetTaggerLeadingLRJPhiValues = nullptr;

    // Output JetTagger subleading large radius jets
    std::vector<double>* jetTaggerSubleadingLRJPsi_RValues = nullptr;
    std::vector<unsigned int >* jetTaggerSubleadingLRJMergedIndicesValues = nullptr;
    std::vector<double>* jetTaggerSubleadingLRJEtValues = nullptr;
    std::vector<double>* jetTaggerSubleadingLRJEtaValues = nullptr;
    std::vector<double>* jetTaggerSubleadingLRJPhiValues = nullptr;

    //std::cout << "test 2 " << "\n";

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

    //std::cout << "test 3 " << "\n";

    // TTrees and branches required as input to algorithm
    TTree* caloTopoTowerTree = (TTree*)inputFile->Get("caloTopoTowerTree");
    TTree* topo422Tree = (TTree*)inputFile->Get("topo422Tree");
    TTree* gepBasicClustersTree = (TTree*)inputFile->Get("gepBasicClustersTree");
    TTree* gepCellsTowersTree = (TTree*)inputFile->Get("gepCellsTowersTree");
    TTree* gepBasicTopoTowersTree = (TTree*)inputFile->Get("gepBasicTopoTowersTree");
    TTree* gepWTAConeCellsTowersJetsTree = (TTree*)inputFile->Get("gepWTAConeCellsTowersJetsTree");
    //TTree* gepConeBasicClustersJetsTree =(TTree*)inputFile->Get("gepConeBasicClustersJetsTree"); 
    TTree* gFexSRJTree = (TTree*)inputFile->Get("gFexSRJTree");
    TTree* jFexSRJTree = (TTree*)inputFile->Get("jFexSRJTree"); // Note that jFEX, gFEX trees are pre-sorted by Et in LRJNTupler.cc

    outputFile->cd();
    // Create new output TTrees to be written to the output file
    TTree* jetTaggerLRJs = new TTree("jetTaggerLRJsTree", "Tree storing event-wise Substructure variable, Et, Eta, Phi");
    jetTaggerLRJs->Branch("Psi_R", &jetTaggerLRJPsi_RValues);
    jetTaggerLRJs->Branch("MergedIndices", &jetTaggerLRJMergedIndicesValues);
    jetTaggerLRJs->Branch("Et", &jetTaggerLRJEtValues);
    jetTaggerLRJs->Branch("Eta", &jetTaggerLRJEtaValues);
    jetTaggerLRJs->Branch("Phi", &jetTaggerLRJPhiValues);

    TTree* jetTaggerLeadingLRJs = new TTree("jetTaggerLeadingLRJsTree", "Tree storing event-wise Substructure variable, Et, Eta, Phi");
    jetTaggerLeadingLRJs->Branch("Psi_R", &jetTaggerLeadingLRJPsi_RValues);
    jetTaggerLeadingLRJs->Branch("MergedIndices", &jetTaggerLeadingLRJMergedIndicesValues);
    jetTaggerLeadingLRJs->Branch("Et", &jetTaggerLeadingLRJEtValues);
    jetTaggerLeadingLRJs->Branch("Eta", &jetTaggerLeadingLRJEtaValues);
    jetTaggerLeadingLRJs->Branch("Phi", &jetTaggerLeadingLRJPhiValues);

    TTree* jetTaggerSubleadingLRJs = new TTree("jetTaggerSubleadingLRJsTree", "Tree storing event-wise Substructure variable, Et, Eta, Phi");
    jetTaggerSubleadingLRJs->Branch("Psi_R", &jetTaggerSubleadingLRJPsi_RValues);
    jetTaggerSubleadingLRJs->Branch("MergedIndices", &jetTaggerSubleadingLRJMergedIndicesValues);
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

    // === gepCellsTowersTree ===
    gepCellsTowersTree->SetBranchAddress("Et", &gepCellsTowersEtValues);
    gepCellsTowersTree->SetBranchAddress("Eta", &gepCellsTowersEtaValues);
    gepCellsTowersTree->SetBranchAddress("Phi", &gepCellsTowersPhiValues);

    // === gepBasicTopoTowersTree ===
    gepBasicTopoTowersTree->SetBranchAddress("Et", &gepBasicTopoTowersEtValues);
    gepBasicTopoTowersTree->SetBranchAddress("Eta", &gepBasicTopoTowersEtaValues);
    gepBasicTopoTowersTree->SetBranchAddress("Phi", &gepBasicTopoTowersPhiValues);

    // gep WTA cone cells towers jets
    gepWTAConeCellsTowersJetsTree->SetBranchAddress("pT", &gepWTAConeCellsTowersJetspTValues);
    gepWTAConeCellsTowersJetsTree->SetBranchAddress("Eta", &gepWTAConeCellsTowersJetsEtaValues);
    gepWTAConeCellsTowersJetsTree->SetBranchAddress("Phi", &gepWTAConeCellsTowersJetsPhiValues);
    gepWTAConeCellsTowersJetsTree->SetBranchAddress("Mass", &gepWTAConeCellsTowersJetsMassValues);
    gepWTAConeCellsTowersJetsTree->SetBranchAddress("NConstituents", &gepWTAConeCellsTowersJetsNConstituentsValues);
    
    // gep cone cells towers jets
    /*gepWTAConeCellsTowersJetsTree->SetBranchAddress("pT", &gepConeCellsTowersJetspTValues);
    gepConeCellsTowersJetsTree->SetBranchAddress("Eta", &gepConeCellsTowersJetsEtaValues);
    gepConeCellsTowersJetsTree->SetBranchAddress("Phi", &gepConeCellsTowersJetsPhiValues);
    gepConeCellsTowersJetsTree->SetBranchAddress("Mass", &gepConeCellsTowersJetsMassValues);
    gepConeCellsTowersJetsTree->SetBranchAddress("NConstituents", &gepConeCellsTowersJetsNConstituentsValues);*/

    // GEP CONE JETS NOT MEANT TO USE CLUSTERS, NOR non-WTA
    // gep cone basic clusters jets
    /*gepConeBasicClustersJetsTree->SetBranchAddress("pT", &gepConeGEPBasicClustersJetspTValues);
    gepConeBasicClustersJetsTree->SetBranchAddress("Eta", &gepConeGEPBasicClustersJetsEtaValues);
    gepConeBasicClustersJetsTree->SetBranchAddress("Phi", &gepConeGEPBasicClustersJetsPhiValues);
    gepConeBasicClustersJetsTree->SetBranchAddress("Mass", &gepConeGEPBasicClustersJetsMassValues);
    gepConeBasicClustersJetsTree->SetBranchAddress("NConstituents", &gepConeGEPBasicClustersJetsNConstituentsValues);*/

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
    unsigned int overlapRemovalCounter = 0;
    unsigned int eventsToProcess = gepBasicClustersTree->GetEntries();
    for (unsigned int iEvt = 0; iEvt < eventsToProcess; iEvt++) {
        if (iEvt % 100 == 0) std::cout << "iEvt: " << iEvt << "\n";
        // Write event header to output text files
        f_output << "Event : " << std::dec << iEvt << "\n";

        //std::cout << "procesing event: " << std::dec << iEvt << "\n";

        std::bitset<num_constituents_bit_length_> numMergedIO_bitset_array[nSeedsOutput_];
        std::bitset<psi_R_bit_length_> psi_R_bitset_array[nSeedsOutput_];
        std::bitset<et_bit_length_  > et_bitset_array[nSeedsOutput_];
        std::bitset<eta_bit_length_ > eta_bitset_array[nSeedsOutput_];
        std::bitset<phi_bit_length_ > phi_bitset_array[nSeedsOutput_];

        // Clear vectors for branches
        jetTaggerLRJPsi_RValues->clear();
        jetTaggerLRJMergedIndicesValues->clear();
        jetTaggerLRJEtValues->clear();
        jetTaggerLRJEtaValues->clear();
        jetTaggerLRJPhiValues->clear();

        jetTaggerLeadingLRJPsi_RValues->clear();
        jetTaggerLeadingLRJMergedIndicesValues->clear();
        jetTaggerLeadingLRJEtValues->clear();
        jetTaggerLeadingLRJEtaValues->clear();
        jetTaggerLeadingLRJPhiValues->clear();

        jetTaggerSubleadingLRJPsi_RValues->clear();
        jetTaggerSubleadingLRJMergedIndicesValues->clear();
        jetTaggerSubleadingLRJEtValues->clear();
        jetTaggerSubleadingLRJEtaValues->clear();
        jetTaggerSubleadingLRJPhiValues->clear();

        if(seedObjectType == "jFEXSRJ"){
            jFexSRJTree->GetEntry(iEvt); // get seed data
        }
        else if(seedObjectType == "gepWTAConeCellsTowersJets"){
            gepWTAConeCellsTowersJetsTree->GetEntry(iEvt); // get seed object data
        }
        //else if(seedObjectType == "gepConeBasicClustersJets"){
        //    gepConeBasicClustersJetsTree->GetEntry(iEvt); // get input object data
        //}

        if(inputObjectType == "gepBasicClusters"){
            gepBasicClustersTree->GetEntry(iEvt); // get input object data
        }
        else if(inputObjectType == "gepCellsTowers"){
            gepCellsTowersTree->GetEntry(iEvt); // get input object data
        }
        else if(inputObjectType == "gepTopoTowers"){
            gepBasicTopoTowersTree->GetEntry(iEvt); // get input object data
        }
        else if(inputObjectType == "gepWTAConeCellsTowersJets"){
            gepWTAConeCellsTowersJetsTree->GetEntry(iEvt); // get input object data
        }
        //else if(inputObjectType == "gepConeBasicClustersJets"){
        //    gepConeBasicClustersJetsTree->GetEntry(iEvt); // get input object data
        //}
        else {
            std::cerr << "INPUT OBJECT UNKNOWN!" << "\n";
        }

        unsigned int seedValues[nSeedsInput_][3] = {0}; // 3 = number of data types (Et, Eta, Phi)
        unsigned int inputObjectValues[maxObjectsConsidered_][3] ={0};
        unsigned int outputJetValues[nSeedsOutput_][5] = {0}; // 5 = number of data types (constituents ,Substructure, Et, Eta, Phi)
        //std::cout << "rMergeConsiderCutDigitized_ : " << rMergeConsiderCutDigitized_ << "\n";
        // Digitize and format data in same way expected for running in HLS
        for(unsigned int iSeed = 0; iSeed < nSeedsInput_; iSeed++){
            //std::cout << "iSeed :" << iSeed << "\n";
            //std::cout << "jFexSRJEtValues->size(): " << jFexSRJEtValues->size() << "\n";
            if(seedObjectType == "jFEXSRJ"){
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
            }
            else if(seedObjectType == "gepWTAConeCellsTowersJets"){
                //std::cout << " seedobject type is correct" << "\n";
                //std::cout << "gepConeGEPBasicClustersJetspTValues->size() :" << gepWTAConeCellsTowersJetspTValues->size() << "\n";
                if(gepWTAConeCellsTowersJetspTValues->size() < nSeedsInput_){
                    if(iSeed >= gepWTAConeCellsTowersJetspTValues->size()){
                        seedValues[iSeed][0] = 0;
                        seedValues[iSeed][1] = 0;
                        seedValues[iSeed][2] = 0;
                        continue;
                    }
                }
                seedValues[iSeed][0] = digitize(gepWTAConeCellsTowersJetspTValues->at(iSeed), et_bit_length_,
                                static_cast<double>(et_min_), static_cast<double>(et_max_));
                //std::cout << "seedValues[iSeed][0] after digi: " << seedValues[iSeed][0] << "\n";
                seedValues[iSeed][1] = digitize(gepWTAConeCellsTowersJetsEtaValues->at(iSeed), eta_bit_length_, eta_min_, eta_max_);
                seedValues[iSeed][2] = digitize(gepWTAConeCellsTowersJetsPhiValues->at(iSeed), phi_bit_length_, phi_min_, phi_max_);
                //std::cout << "---------------------" << "\n";
                //std::cout << "iSeed: " << iSeed << "\n";
                //std::cout << "seed Et : " << seedValues[iSeed][0] << "\n";
                //std::cout << "seed eta: " << seedValues[iSeed][1] << " and phi: " << seedValues[iSeed][2] << "\n";
            }

            else if(seedObjectType == "gepWTAConeCellsTowersJets"){
                if(gepWTAConeCellsTowersJetspTValues->size() < nSeedsInput_){
                    if(iSeed >= gepWTAConeCellsTowersJetspTValues->size()){
                        seedValues[iSeed][0] = 0;
                        seedValues[iSeed][1] = 0;
                        seedValues[iSeed][2] = 0;
                        continue;
                    }
                }
                seedValues[iSeed][0] = digitize(gepWTAConeCellsTowersJetspTValues->at(iSeed), et_bit_length_,
                                static_cast<double>(et_min_), static_cast<double>(et_max_));
                seedValues[iSeed][1] = digitize(gepWTAConeCellsTowersJetsEtaValues->at(iSeed), eta_bit_length_, eta_min_, eta_max_);
                seedValues[iSeed][2] = digitize(gepWTAConeCellsTowersJetsPhiValues->at(iSeed), phi_bit_length_, phi_min_, phi_max_);
            }
            
            
            //if (iSeed < 2){
                //std::cout << "seed et: " << std::dec << seedValues[iSeed][0] << " eta: " << seedValues[iSeed][1] << " phi: " << seedValues[iSeed][2] << "\n";
                //std::cout << "UNDIGITIZED Et: " << jFexSRJEtValues->at(iSeed) << " eta: " << jFexSRJEtaValues->at(iSeed) << " and phi: " << jFexSRJPhiValues->at(iSeed) << "\n";
            //}
        }
        if(iEvt == 103){
            std::cout << "------------------------- iEvt = 103 ----------------------" << "\n"; 
            std::cout << "leading et, eta, phi before: " << undigitize_et(seedValues[0][0]) << " , " << undigitize_eta(seedValues[0][1]) << " , " << undigitize_phi(seedValues[0][2]) << "\n";
            std::cout << "subleading et, eta, phi before: " << undigitize_et(seedValues[1][0]) << " , " << undigitize_eta(seedValues[1][1]) << " , " << undigitize_phi(seedValues[1][2]) << "\n";
        }
        //std::cout << "subleading et, eta, phi before: " << undigitize_et(seedValues[1][0]) << " , " << undigitize_eta(seedValues[1][1]) << " , " << undigitize_phi(seedValues[1][2]) << "\n";
        // Perform overlap removal (OR) ensuring that leading, subleading seeds don't overlap within deltaR < 2.0 /// FIXME /// do this with a LUT
        double deltaRLeadingSubleading = sqrt(calcDeltaR2(undigitize_eta(seedValues[0][1]), undigitize_phi(seedValues[0][2]), undigitize_eta(seedValues[1][1]), undigitize_phi(seedValues[1][2])));
        if(deltaRLeadingSubleading <= 2.0 * sqrt(r2Cut_)){
            //std::cout << "triggering OR removal" << "\n";
            for(unsigned int iSeedOR = nSeedsOutput_; iSeedOR < nSeedsInput_; iSeedOR++){
                double deltaRLeadingNthLeading = sqrt(calcDeltaR2(undigitize_eta(seedValues[0][1]), undigitize_phi(seedValues[0][2]), undigitize_eta(seedValues[iSeedOR][1]), undigitize_phi(seedValues[iSeedOR][2])));
                if(deltaRLeadingNthLeading >=  2.0 * sqrt(r2Cut_)){
                    // Swap the entire (Et, eta, phi) triplet for original subleading, new subleading seed
                    std::swap(seedValues[1][0], seedValues[iSeedOR][0]); // Et
                    std::swap(seedValues[1][1], seedValues[iSeedOR][1]); // eta
                    std::swap(seedValues[1][2], seedValues[iSeedOR][2]); // phi
                    overlapRemovalCounter++;
                    break; // break out of loop as we've found something to swap, preventing overlapping large-R jets
                }
            }
        }
        //std::cout << "subleading et, eta, phi after: " << undigitize_et(seedValues[1][0]) << " , " << undigitize_eta(seedValues[1][1]) << " , " << undigitize_phi(seedValues[1][2]) << "\n";
        
        unsigned int objectsProcessed = maxObjectsConsidered_;
        if(inputObjectType == "gepBasicClusters"){
            if (maxObjectsConsidered_ > gepBasicClustersEtValues->size()) objectsProcessed = gepBasicClustersEtValues->size();
        }
        else if(inputObjectType == "gepCellsTowers"){
            if (maxObjectsConsidered_ > gepCellsTowersEtValues->size()) objectsProcessed = gepCellsTowersEtValues->size();
        }
        else if(inputObjectType == "gepTopoTowers"){
            if (maxObjectsConsidered_ > gepBasicTopoTowersEtValues->size()) objectsProcessed = gepBasicTopoTowersEtValues->size();
        }
        else if(inputObjectType == "gepWTAConeCellsTowersJets"){
            if (maxObjectsConsidered_ + nSeedsInput_ >= gepWTAConeCellsTowersJetspTValues->size()){
                if(gepWTAConeCellsTowersJetspTValues->size() >= nSeedsInput_ ) objectsProcessed = gepWTAConeCellsTowersJetspTValues->size() - nSeedsInput_;
                else objectsProcessed = 0;
            } 
        }
        /*else if(inputObjectType == "gepConeBasicClustersJets"){
            if (maxObjectsConsidered_ + nSeedsInput_ >= gepConeGEPBasicClustersJetspTValues->size()){
                if(gepConeGEPBasicClustersJetspTValues->size() >= nSeedsInput_ ) objectsProcessed = gepConeGEPBasicClustersJetspTValues->size() - nSeedsInput_;
                else objectsProcessed = 0;
            }
        }*/
        
        for(unsigned int iIO = 0; iIO < objectsProcessed; iIO++){
            if(inputObjectType == "gepBasicClusters"){
                inputObjectValues[iIO][0] = digitize(gepBasicClustersEtValues->at(iIO), et_bit_length_,
                              static_cast<double>(et_min_), static_cast<double>(et_max_));
                inputObjectValues[iIO][1] = digitize(gepBasicClustersEtaValues->at(iIO), eta_bit_length_, eta_min_, eta_max_);
                inputObjectValues[iIO][2] = digitize(gepBasicClustersPhiValues->at(iIO), phi_bit_length_, phi_min_, phi_max_);
            }
            else if(inputObjectType == "gepCellsTowers"){

                //if(gepCellsTowersEtValues->at(iIO) > 10.0){
                //    //std::cout << "gepCellsTowersEtValues->at(iIO): " << gepCellsTowersEtValues->at(iIO) << "\n";
                //    //std::cout << "io index: " << iIO << "\n";
                //}
                
                inputObjectValues[iIO][0] = digitize(gepCellsTowersEtValues->at(iIO), et_bit_length_,
                              static_cast<double>(et_min_), static_cast<double>(et_max_));
                //std::cout << "digitized et: " << inputObjectValues[iIO][0] << "\n";
                inputObjectValues[iIO][1] = digitize(gepCellsTowersEtaValues->at(iIO), eta_bit_length_, eta_min_, eta_max_);
                inputObjectValues[iIO][2] = digitize(gepCellsTowersPhiValues->at(iIO), phi_bit_length_, phi_min_, phi_max_);
            }
            else if(inputObjectType == "gepTopoTowers"){
                inputObjectValues[iIO][0] = digitize(gepBasicTopoTowersEtValues->at(iIO), et_bit_length_,
                              static_cast<double>(et_min_), static_cast<double>(et_max_));
                inputObjectValues[iIO][1] = digitize(gepBasicTopoTowersEtaValues->at(iIO), eta_bit_length_, eta_min_, eta_max_);
                inputObjectValues[iIO][2] = digitize(gepBasicTopoTowersPhiValues->at(iIO), phi_bit_length_, phi_min_, phi_max_);
            }
            else if(inputObjectType == "gepWTAConeCellsTowersJets"){
                //std::cout << "iIO: " << iIO << "\n";
                inputObjectValues[iIO][0] = digitize(gepWTAConeCellsTowersJetspTValues->at(iIO + nSeedsInput_), et_bit_length_,
                              static_cast<double>(et_min_), static_cast<double>(et_max_));
                //std::cout << "input et: " << inputObjectValues[iIO][0] << " , " << "input eta: " << inputObjectValues[iIO][1] << "\n";
                inputObjectValues[iIO][1] = digitize(gepWTAConeCellsTowersJetsEtaValues->at(iIO + nSeedsInput_), eta_bit_length_, eta_min_, eta_max_);
                inputObjectValues[iIO][2] = digitize(gepWTAConeCellsTowersJetsPhiValues->at(iIO + nSeedsInput_), phi_bit_length_, phi_min_, phi_max_);
            }
            /*else if(inputObjectType == "gepConeBasicClustersJets"){
                inputObjectValues[iIO][0] = digitize(gepConeGEPBasicClustersJetspTValues->at(iIO + nSeedsInput_), et_bit_length_,
                              static_cast<double>(et_min_), static_cast<double>(et_max_));
                inputObjectValues[iIO][1] = digitize(gepConeGEPBasicClustersJetsEtaValues->at(iIO + nSeedsInput_), eta_bit_length_, eta_min_, eta_max_);
                inputObjectValues[iIO][2] = digitize(gepConeGEPBasicClustersJetsPhiValues->at(iIO + nSeedsInput_), phi_bit_length_, phi_min_, phi_max_);
            }*/
            
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
            unsigned int et1 = seedValues[iSeed][0];
            unsigned int et2 = seedValues[indices[iSeed] + nSeedsOutput_][0];
            unsigned int etSum = et1 + et2; 
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

            int phi_mid;
            

            // --- Compute midpoint in signed domain ---
            int eta_mid;
            if(!enableEtWeighting || etSum == 0){ // Must account for case when there aren't any cone jets (or seeds in general) --> etSum == 0
                eta_mid = (eta1 + eta2) >> 1; // Integer division by 2
                phi_mid = phi1s + (dphi >> 1);  // arithmetic shift (divide by 2)

                phi_mid = wrapSym(phi_mid);
            } 
            else{
                //std::cout << "etSum: " << etSum << "\n";
                eta_mid = (et2 * eta2 + et1 * eta1) / etSum;
                phi_mid = phi1s + ( et2 * dphi ) / etSum;
            }
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

        std::vector<unsigned int > jet1MergedIndices;
        std::vector<unsigned int > jet2MergedIndices;
        
        for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; ++iSeed){ // FIXME no longer considering highest Et seed first (need to implement some sorting)
            unsigned int outputJetEt = 0;
            //std::cout << "seedValues[iSeed][0]: " << seedValues[iSeed][0] << "\n";
            if(inputObjectType == "gepWTAConeCellsTowersJets" || inputObjectType == "gepConeBasicClustersJets") outputJetEt = seedValues[iSeed][0]; // If clustering using jets, output jet E_T should start with seed E_T
            unsigned int numMergedIO = 0; 
            unsigned int jet_psi_R = 0;
            unsigned int highestEtMergedIOEta[2];
            unsigned int highestEtMergedIOPhi[2];

            //std::cout << "outputjetet: " << outputJetEt << "\n";

            for (unsigned int iInput = 0; iInput < objectsProcessed; ++iInput){ // loop through input objects to consider merging

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
                    if(outputJetEt + inputObjectValues[iInput][0] >= int(et_max_/et_granularity_ - 1)) outputJetEt = int(et_max_/et_granularity_ - 1);
                    else outputJetEt += inputObjectValues[iInput][0]; // add input object Et to seed Et for resultant output jet Et
                    jetTaggerLRJMergedIndicesValues->push_back(iInput);
                    if(iSeed == 0) jet1MergedIndices.push_back(iInput);
                    if(iSeed == 1) jet2MergedIndices.push_back(iInput);
                    if(numMergedIO <= 1){
                        highestEtMergedIOEta[numMergedIO] = inputObjectValues[iInput][1];
                        highestEtMergedIOPhi[numMergedIO] = inputObjectValues[iInput][2];
                    }
                    // Compute psi_R using 8b 0,1 deltaR LUT
                    unsigned int lut_index_mergedIO = uDeltaEta * (1 << (phi_bit_length_ - 1) ) + corrDeltaPhi;
                    if(iEvt == 3674){
                        std::cout << "iEvt 3674 -------------------------" << "\n";
                        std::cout << "lut_index_mergedIO: " << lut_index_mergedIO << "\n";
                        std::cout << "deltaR: " << lutR_8b_[lut_index_mergedIO] << "\n";
                        std::cout << "input object et: " << inputObjectValues[iInput][0] << "\n";
                        std::cout << "psi_R contribution: " << inputObjectValues[iInput][0] * lutR_8b_[lut_index_mergedIO] << "\n";
                    }
                    
                    jet_psi_R += inputObjectValues[iInput][0] * lutR_8b_[lut_index_mergedIO];
                    numMergedIO++; 
                    if(iEvt == 3674){
                        std::cout << "psi_R_temp : " << jet_psi_R << "\n";
                        std::cout << "nummergedIO temp: " << numMergedIO << "\n";
                    }
                }
            }
            if(iEvt == 3674){
                std::cout << "jetpsi_R before: " << jet_psi_R << "\n";
            }
            
            if(outputJetEt > 0) jet_psi_R = jet_psi_R / outputJetEt;
            else jet_psi_R = 0;
            if(iEvt == 3674){
                std::cout << "jetpsi_R after: " << jet_psi_R << "\n";
                std::cout << "nummergedIO final: " << numMergedIO << "\n";
            }

            
            

            // Compute delta R between the two highest Et topoclusters merged to a large R jet FIXME need to account for case when 0 or 1 input objects are merged! if statement should be fine for now?
            /*unsigned int jet_diam = 0;
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
            }*/

            // Values you already computed:
            
            uint64_t numMergedIO_value = static_cast<uint32_t>(numMergedIO)        & maskN(num_constituents_bit_length_);
            uint64_t psi_R_value = static_cast<uint32_t>(jet_psi_R)        & maskN(psi_R_bit_length_);
            //std::cout << "dim_value: " << diam_value << "\n";
            uint64_t et_value   = static_cast<uint32_t>(outputJetEt)      & maskN(et_bit_length_);
            // NOTE: this mirrors your original snippet exactly (eta_bitset took seedValues[iSeed][1] and phi_bitset took [2]).
            uint64_t eta_value  = static_cast<uint32_t>(seedValues[iSeed][1]) & maskN(eta_bit_length_);
            uint64_t phi_value  = static_cast<uint32_t>(seedValues[iSeed][2]) & maskN(phi_bit_length_);

            //std::cout << "jet_diam: " << jet_diam << " and output jet Et: " << outputJetEt << "\n";
        //std::cout << "output jet eta, phi: " << seedValues[iSeed][1] << " , " << seedValues[iSeed][2] << "\n";

            // Set output values (unchanged semantic layout)
            outputJetValues[iSeed][0] = numMergedIO_value; // constituents
            outputJetValues[iSeed][1] = psi_R_value; // substructure
            outputJetValues[iSeed][2] = et_value;   // Et
            outputJetValues[iSeed][3] = seedValues[iSeed][1]; // phi
            outputJetValues[iSeed][4] = seedValues[iSeed][2]; // eta

            // Bitset strings (unchanged)
            std::bitset<num_constituents_bit_length_> numMergedIO_bitset(numMergedIO_value);
            std::bitset<psi_R_bit_length_> psi_R_bitset(psi_R_value);
            std::bitset<et_bit_length_  > et_bitset(et_value);
            std::bitset<eta_bit_length_ > eta_bitset(eta_value);
            std::bitset<phi_bit_length_ > phi_bitset(phi_value);

            
            numMergedIO_bitset_array[iSeed] = numMergedIO_bitset;
            psi_R_bitset_array[iSeed] = psi_R_bitset;
            et_bitset_array[iSeed] = et_bitset;
            eta_bitset_array[iSeed] = eta_bitset;
            phi_bitset_array[iSeed] = phi_bitset;

            // Pack into one word: [diam | et | eta | phi] (same field order as your ap_uint expression)
            uint64_t combined_value =
                ((numMergedIO_value & maskN(num_constituents_bit_length_)) << (psi_R_bit_length_ + et_bit_length_ + eta_bit_length_ + phi_bit_length_)) |
                ((psi_R_value & maskN(psi_R_bit_length_)) << (et_bit_length_ + eta_bit_length_ + phi_bit_length_)) |
                ((et_value   & maskN(et_bit_length_  )) << (eta_bit_length_ + phi_bit_length_)) |
                ((eta_value  & maskN(eta_bit_length_ )) <<  phi_bit_length_) |
                ( phi_value  & maskN(phi_bit_length_ ));

            // Hex string with width = total_bits_/4 (=8 for 32 bits)
            std::stringstream hex_stream;
            hex_stream << std::hex << std::nouppercase << std::setfill('0') << std::setw(total_bits_output_ / 4) << combined_value;
            std::string hexValue = hex_stream.str();

            // Final print (identical shape)
            f_output << "0x" << std::hex << std::setw(2) << std::setfill('0') << iSeed
                    << " "  << numMergedIO_bitset.to_string() << "|" << psi_R_bitset.to_string() << "|" << et_bitset.to_string()
                    << "|"  << eta_bitset.to_string()  << "|" << phi_bitset.to_string()
                    << " 0x" << hexValue << std::endl;

            // Write data to all LRJ branch
            jetTaggerLRJPsi_RValues->push_back(undigitize_diam(psi_R_bitset));
            //std::cout << "undigitized psi_R: " << undigitize_diam(psi_R_bitset) << "\n";
            jetTaggerLRJEtValues->push_back(undigitize_et(et_bitset));
            jetTaggerLRJEtaValues->push_back(undigitize_eta(eta_bitset));
            jetTaggerLRJPhiValues->push_back(undigitize_phi(phi_bitset));
        } // Loop through seeds 
        // Write data to leading, subleading jet branches
        if(outputJetValues[0][2] >= outputJetValues[1][2]){ // to ensure leading, subleading sorted correctly
            jetTaggerLeadingLRJPsi_RValues->push_back(undigitize_diam(psi_R_bitset_array[0]));
            for(unsigned int i = 0; i < jet1MergedIndices.size(); i++){ // necessary to ensure merged indices match to leading, subleading correctly
                jetTaggerLeadingLRJMergedIndicesValues->push_back(jet1MergedIndices[i]);
            }
            for(unsigned int j = 0; j < jet2MergedIndices.size(); j++){
                jetTaggerSubleadingLRJMergedIndicesValues->push_back(jet2MergedIndices[j]);
            }
            jetTaggerLeadingLRJEtValues->push_back(undigitize_et(et_bitset_array[0]));
            jetTaggerLeadingLRJEtaValues->push_back(undigitize_eta(eta_bitset_array[0]));
            jetTaggerLeadingLRJPhiValues->push_back(undigitize_phi(phi_bitset_array[0]));

            jetTaggerSubleadingLRJPsi_RValues->push_back(undigitize_diam(psi_R_bitset_array[1]));
            jetTaggerSubleadingLRJEtValues->push_back(undigitize_et(et_bitset_array[1]));
            jetTaggerSubleadingLRJEtaValues->push_back(undigitize_eta(eta_bitset_array[1]));
            jetTaggerSubleadingLRJPhiValues->push_back(undigitize_phi(phi_bitset_array[1]));
        }
        else{
            jetTaggerLeadingLRJPsi_RValues->push_back(undigitize_diam(psi_R_bitset_array[1]));
            jetTaggerLeadingLRJEtValues->push_back(undigitize_et(et_bitset_array[1]));
            jetTaggerLeadingLRJEtaValues->push_back(undigitize_eta(eta_bitset_array[1]));
            jetTaggerLeadingLRJPhiValues->push_back(undigitize_phi(phi_bitset_array[1]));
            for(unsigned int i = 0; i < jet1MergedIndices.size(); i++){ // necessary to ensure merged indices match to leading, subleading correctly
                jetTaggerSubleadingLRJMergedIndicesValues->push_back(jet1MergedIndices[i]);
            }
            for(unsigned int j = 0; j < jet2MergedIndices.size(); j++){
                jetTaggerLeadingLRJMergedIndicesValues->push_back(jet2MergedIndices[j]);
            }

            jetTaggerSubleadingLRJPsi_RValues->push_back(undigitize_diam(psi_R_bitset_array[0]));
            jetTaggerSubleadingLRJEtValues->push_back(undigitize_et(et_bitset_array[0]));
            jetTaggerSubleadingLRJEtaValues->push_back(undigitize_eta(eta_bitset_array[0]));
            jetTaggerSubleadingLRJPhiValues->push_back(undigitize_phi(phi_bitset_array[0]));
        }
        // Fill branch to output ntuple with 
        jetTaggerLRJs->Fill();
        jetTaggerLeadingLRJs->Fill();
        jetTaggerSubleadingLRJs->Fill();
    } // Loop through events
    std::cout << "number of overlaps removed: " << overlapRemovalCounter << " for " << eventsToProcess << " events." << "\n";
    // Close output files
    outputFile->cd();
    std::cout << "writing output file" << "\n";
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
                      bool condorBool, // whether running using condor batch job submission (requires change in filepaths)
                      std::string inputObjectType = "gepCellsTowers", // Possibilities: "gepCellsTowers", "gepWTAConeCellsTowersJets", "gepTopoTowers", "gepBasicClusters"
                      std::string seedObjectType = "gepWTAConeCellsTowersJets"){ // Possibilities: "gepWTAConeCellsTowersJets" or "jFEXSRJ"  // FIXME allow this to be changed in executable
    // Construct input and output ntuple, LUT paths based on configuration type
    auto infile = makeInputFileName(signalBool); // FIXME update this when running with condor.
    auto outntuplefile = makeOutputFileName(rMergeCut, numberIOs, nSeeds, RSquaredCut, signalBool, inputObjectType, seedObjectType);
    auto outtextfile = makeOutputTextFileName(rMergeCut, numberIOs, nSeeds, RSquaredCut, signalBool, inputObjectType, seedObjectType);
    std::cout << "infile: " << infile << "\n";
    std::cout << "outntuplefile: " << outntuplefile << "\n"; 
    std::cout << "outtextfile: " << outtextfile << "\n";       
    try { // FIXME update this when running with condor.
        //std::cout << "why is this not working" << "\n";
        std::filesystem::copy_file(infile, outntuplefile, 
                      std::filesystem::copy_options::overwrite_existing);
        std::cout << "File copied successfully\n";
    } catch (std::filesystem::filesystem_error& e) {
        std::cerr << "Copy failed: " << e.what() << '\n';
    }            
    //gSystem->RedirectOutput("debuglog.log", "w");
    eventLoop(infile, outntuplefile, outtextfile, inputObjectType, seedObjectType);
    //gSystem->Exit(0);
} 
