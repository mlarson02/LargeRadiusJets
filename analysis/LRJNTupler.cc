// To execute: e.g., root ; .L nTupler.C ; nTupler(true, true, true) 
#include <algorithm>
#include <numeric>   // std::iota

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <filesystem> // C++17
#include <algorithm>
#include <stdexcept>  // for std::runtime_error
//#include "xAODCutFlow/CutBookkeeperContainer.h"
//#include "xAODCutFlow/CutBookkeeper.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TH2F.h"
#include "analysisHelperFunctions.h"

// Used for digitized data writing
struct OutputFiles {
    std::string topo422;
    std::string caloTopoTowers;
    std::string gepBasicClusters;
    std::string gepCellTowers;
    std::string gepBasicTopoTowers;
    std::string gFex;
    std::string jFex;
};

// Settings
const bool afBool = true;
const bool vbfBool = true;
const unsigned int nJZSlices = 10;

// Constants required for reweighting

// In barns^-1 - 7.5*10^34 cm^-2 s^-1 * 1 s (HL-LHC 200 PU inst. lumi * 1 second) - use 1 second to make rates plots easy
const double reweightLuminosity = 7.5e10;

// Filter efficiencies from AMI [in b]
const double filterEffienciesByJZSlice[nJZSlices] = {0.9716436,    // JZ0
                                                     0.03777559,   // JZ1
                                                     0.01136654,   // JZ2
                                                     0.01367042,   // JZ3
                                                     0.01628158,   // JZ4
                                                     0.01905588,   // JZ5
                                                     0.01352844,   // JZ6
                                                     0.01764909,   // JZ7
                                                     0.01887484,   // JZ8
                                                     0.02827565};  // JZ9

// Cross sections from AMI [in b]
const double crossSectionsByJZSlice[nJZSlices] = {0.07893,      // JZ0
                                                  0.09679,      // JZ1
                                                  0.0026805,    // JZ2
                                                  0.000029984,  // JZ3
                                                  2.972e-7,     // JZ4
                                                  5.5384e-09,   // JZ5
                                                  3.2616e-10,   // JZ6
                                                  2.1734e-11,   // JZ7
                                                  9.2995e-13,   // JZ8
                                                  3.4519e-14};  // JZ9

// From CutBookkeeper printed from python script
const double sumOfEventWeightsByJZSlice[nJZSlices] = {10000.0,                // JZ0
                                                      467.7515499125784,      // JZ1
                                                      3.9288560244923474,     // JZ2
                                                      0.08040490627634789,    // JZ3
                                                      0.0025011268072319126,  // JZ4
                                                      0.00019515390387392362, // JZ5
                                                      3.325120293798389e-05,  // JZ6
                                                      9.518299562145949e-06,  // JZ7 
                                                      2.8526936132339253e-06, // JZ8
                                                      3.274964361399163e-09}; // JZ9 

// Helper to construct memprint / test vector filenames
inline OutputFiles makeMemPrintFilenames(bool signalBool, bool vbfBool, int jzSlice) {
    static const std::string base = "/data/larsonma/LargeRadiusJets/MemPrints/";

    std::string tag;
    if (signalBool) {
        tag = vbfBool ? "mc21_14TeV_hh_bbbb_vbf_novhh"
                      : "mc21_14TeV_HHbbbb_HLLHC";
    } else {
        // generic: works for all slices 0..9 etc
        tag = "mc21_14TeV_jj_JZ" + std::to_string(jzSlice);
    }

    OutputFiles out;
    out.topo422         = base + "CaloTopo_422/"    + tag + "_topo422.dat";
    out.caloTopoTowers  = base + "CaloTopoTowers/"  + tag + "_calotopotowers.dat";
    out.gepBasicClusters= base + "GEPBasicClusters/"+ tag + "_gepbasicclusters.dat";
    out.gepCellTowers= base + "GEPCellsTowers/"+ tag + "_gepcellstowers.dat";
    out.gepBasicTopoTowers= base + "GEPBasicTopoTowers/"+ tag + "_gepbasictopotowers.dat";
    out.gFex            = base + "gFex/"            + tag + "_gfex_smallrj.dat";
    out.jFex            = base + "jFex/"            + tag + "_jfex_smallrj.dat";
    return out;
}

// Helper to get input DAOD + GEP ntuple file names
std::string makeFileDir(bool vbfBool, bool signalBool, int jzSlice) {
    static const std::string kRoot = "/data/larsonma/LargeRadiusJets/datasets";

    if (signalBool) {
        return vbfBool
            ? kRoot + "/DAOD_TrigGepPerf/Signal_HHbbbb_VBF"
            : kRoot + "/Signal_HHbbbb_DAODAOD/DAOD/ggF/"
              "mc21_14TeV.603277.PhPy8EG_PDF4LHC21_HHbbbb_HLLHC_chhh1p0.deriv.DAOD_JETM42.e8564_s4422_r16130_p6658";
    }

    // background path
    if (jzSlice < 0 || jzSlice > 9) {
        throw std::out_of_range("jzSlice must be in [0, 9]");
    }
    return kRoot + "/DAOD_TrigGepPerf/Background_jj_JZ" + std::to_string(jzSlice);
}

// Helper function to get output ntuple file name
TString makeOutputFileName(bool vbfBool, bool signalBool, int jzSlice) {
    static const TString outDir = "outputRootFiles/";

    if (vbfBool) {
        if (signalBool) {
            return outDir + "mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_DAOD_NTUPLE_GEP.root";
            
        } else {
            if (jzSlice < 0 || jzSlice > 9)
                throw std::out_of_range("jzSlice must be between 0 and 9");
            return outDir + "mc21_14TeV_jj_JZ" + TString(std::to_string(jzSlice))
                + "_e8557_s4422_r16130_DAOD_NTUPLE_GEP.root";
        }
    } else {
        if (signalBool) {
            return outDir + "mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_DAOD_NTUPLE_GEP.root";
        } else {
            if (jzSlice < 0 || jzSlice > 9)
                throw std::out_of_range("jzSlice must be between 0 and 9");
            return outDir + "mc21_14TeV_jj_JZ" + TString(std::to_string(jzSlice))
                + "_e8557_s4422_r16130_DAOD_NTUPLE_GEP.root";
        }
    }
}


unsigned int digitize(double value, int bit_length, double min_val, double max_val) {
    // Check if value is in range
    if (value < min_val) {
        value = min_val;
        std::cout << "Warning: Value " << value
          << " is out of range (" << min_val
          << ", " << max_val << ")\n";
    }
    if (value > max_val){
        value = max_val;
        std::cout << "Warning: Value " << value
          << " is out of range (" << min_val
          << ", " << max_val << ")\n";
    }

    double scale = (std::pow(2, bit_length) - 1) / (max_val - min_val);
    return static_cast<unsigned int>(std::round((value - min_val) * scale));
}

// Define phi wrapping
float delta_phi(float phi1, float phi2) {
    float dphi = phi1 - phi2;
    while (dphi > M_PI) dphi -= 2*M_PI;
    while (dphi < -M_PI) dphi += 2*M_PI;
    return dphi;
}

// Define delta R
float delta_R(float eta1, float phi1, float eta2, float phi2) {
    float deta = eta1 - eta2;
    float dphi = delta_phi(phi1, phi2);
    return sqrt(deta*deta + dphi*dphi);
}

// Recursively find first non-Higgs daughters
void find_non_higgs_daughters(const xAOD::TruthParticle* particle,
                              std::vector<const xAOD::TruthParticle*>& result) {
    if (!particle) return;

    // If the particle is NOT a Higgs, store it directly
    if (particle->pdgId() != 25) {
        result.push_back(particle);
        return;
    }

    // If it is a Higgs, recurse through its children
    for (unsigned int i = 0; i < particle->nChildren(); ++i) {
        find_non_higgs_daughters(particle->child(i), result);
    }
}


// Main function
void nTupler(bool signalBool, bool vbfBool = true, unsigned int jzSlice = 3) {
    //ServiceHandle<StoreGateSvc> inputMetaStore("StoreGateSvc/InputMetaDataStore","");
    // Setup file paths based on whether processing signal or background, and vbf production or ggF production
    std::string fileDir = makeFileDir(vbfBool, signalBool, jzSlice);
    std::cout << "fileDir: " << fileDir << "\n";
    //xAOD::Init().ignore();

    // pick filenames
    OutputFiles out = makeMemPrintFilenames(signalBool, vbfBool, jzSlice);

    // open streams
    std::ofstream f_topotower(out.caloTopoTowers);
    std::ofstream f_gepbasicclusters(out.gepBasicClusters);
    std::ofstream f_gepcellstowers(out.gepCellTowers);
    std::ofstream f_gepbasictopotowers(out.gepBasicTopoTowers);
    std::ofstream f_topo(out.topo422);
    std::ofstream f_gfex(out.gFex);
    std::ofstream f_jfex(out.jFex);

    if (!f_topotower.is_open() || !f_topo.is_open() || !f_gfex.is_open() || !f_jfex.is_open() || !f_gepbasicclusters.is_open() || !f_gepcellstowers.is_open() || !f_gepbasictopotowers.is_open()) {
        std::cerr << "Error: One or more output files could not be opened for writing!" << std::endl;
        // handle error or return
    }

    TString outputFileName = makeOutputFileName(vbfBool, signalBool, jzSlice);
    
    // Create ROOT output file
    TFile* outputFile = new TFile(outputFileName, "RECREATE");

    // Create TTrees - commented out trees that cannot be filled with DAOD information.
    //TTree* truthParticleTree = new TTree("truthParticleTree", "Tree storing event-wise information about truth particles");
    TTree* eventInfoTree = new TTree("eventInfoTree", "Tree storing event information (e.g., mcEventWeight) required for rate computation");
    TTree* truthbTree = new TTree("truthbTree", "Tree storing event-wise information about truth particles");
    TTree* truthHiggsTree = new TTree("truthHiggsTree", "Tree storing event-wise information about truth particles");
    //TTree* truthVBFQuark = new TTree("truthVBFQuark", "Tree storing event-wise information about truth particles");
    TTree* caloTopoTowerTree = new TTree("caloTopoTowerTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gepBasicClustersTree = new TTree("gepBasicClustersTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gepCellsTowersTree = new TTree("gepCellsTowersTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gepBasicTopoTowersTree = new TTree("gepBasicTopoTowersTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gepConeCellsTowersJetsTree = new TTree("gepConeCellsTowersJetsTree", "Tree storing event-wise Pt, Eta, Phi, Mass, NConstituents");
    TTree* gepLeadingConeCellsTowersJetsTree = new TTree("gepLeadingConeCellsTowersJetsTree", "Tree storing event-wise Pt, Eta, Phi, Mass, NConstituents");
    TTree* gepSubleadingConeCellsTowersJetsTree = new TTree("gepSubleadingConeCellsTowersJetsTree", "Tree storing event-wise Pt, Eta, Phi, Mass, NConstituents");
    TTree* gepWTAConeCellsTowersJetsTree = new TTree("gepWTAConeCellsTowersJetsTree", "Tree storing event-wise Pt, Eta, Phi, Mass, NConstituents");
    TTree* gepLeadingWTAConeCellsTowersJetsTree = new TTree("gepLeadingWTAConeCellsTowersJetsTree", "Tree storing event-wise Pt, Eta, Phi, Mass, NConstituents");
    TTree* gepSubleadingWTAConeCellsTowersJetsTree = new TTree("gepSubleadingWTAConeCellsTowersJetsTree", "Tree storing event-wise Pt, Eta, Phi, Mass, NConstituents");
    TTree* gepConeBasicClustersJetsTree = new TTree("gepConeBasicClustersJetsTree", "Tree storing event-wise Pt, Eta, Phi, Mass, NConstituents");
    TTree* gepLeadingConeBasicClustersJetsTree = new TTree("gepLeadingConeBasicClustersJetsTree", "Tree storing event-wise Pt, Eta, Phi, Mass, NConstituents");
    TTree* gepSubleadingConeBasicClustersJetsTree = new TTree("gepSubleadingConeBasicClustersJetsTree", "Tree storing event-wise Pt, Eta, Phi, Mass, NConstituents");
    TTree* gepWTAConeBasicClustersJetsTree = new TTree("gepWTAConeBasicClustersJetsTree", "Tree storing event-wise Pt, Eta, Phi, Mass, NConstituents");
    TTree* gepLeadingWTAConeBasicClustersJetsTree = new TTree("gepLeadingWTAConeBasicClustersJetsTree", "Tree storing event-wise Pt, Eta, Phi, Mass, NConstituents");
    TTree* gepSubleadingWTAConeBasicClustersJetsTree = new TTree("gepSubleadingWTAConeBasicClustersJetsTree", "Tree storing event-wise Pt, Eta, Phi, Mass, NConstituents");
    TTree* topo422Tree = new TTree("topo422Tree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gFexSRJTree = new TTree("gFexSRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gFexLeadingSRJTree = new TTree("gFexLeadingSRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gFexSubleadingSRJTree = new TTree("gFexSubleadingSRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gFexLRJTree = new TTree("gFexLRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gFexLeadingLRJTree = new TTree("gFexLeadingLRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gFexSubleadingLRJTree = new TTree("gFexSubleadingLRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* inTimeAntiKt4TruthJetsTree = new TTree("inTimeAntiKt4TruthJetsTree", "Tree storing event-wise Et, eta phi");
    TTree* leadingInTimeAntiKt4TruthJetsTree = new TTree("leadingInTimeAntiKt4TruthJetsTree", "Tree storing event-wise Et, eta phi");
    TTree* subleadingInTimeAntiKt4TruthJetsTree = new TTree("subleadingInTimeAntiKt4TruthJetsTree", "Tree storing event-wise Et, eta phi");
    TTree* jFexSRJTree = new TTree("jFexSRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* jFexLeadingSRJTree = new TTree("jFexLeadingSRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* jFexSubleadingSRJTree = new TTree("jFexSubleadingSRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* jFexLRJTree = new TTree("jFexLRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* jFexLeadingLRJTree = new TTree("jFexLeadingLRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* jFexSubleadingLRJTree = new TTree("jFexSubleadingLRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* hltAntiKt4EMTopoJetsTree = new TTree("hltAntiKt4EMTopoJetsTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* leadingHltAntiKt4EMTopoJetsTree = new TTree("leadingHltAntiKt4EMTopoJetsTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* subleadingHltAntiKt4EMTopoJetsTree = new TTree("subleadingHltAntiKt4EMTopoJetsTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* recoAntiKt10UFOCSSKJets = new TTree("recoAntiKt10UFOCSSKJets", "Tree storing event-wise Et, Eta, Phi");
    TTree* leadingRecoAntiKt10UFOCSSKJets = new TTree("leadingRecoAntiKt10UFOCSSKJets", "Tree storing event-wise Et, Eta, Phi");
    TTree* subleadingRecoAntiKt10UFOCSSKJets = new TTree("subleadingRecoAntiKt10UFOCSSKJets", "Tree storing event-wise Et, Eta, Phi");
    TTree* truthAntiKt4TruthDressedWZJets = new TTree("truthAntiKt4TruthDressedWZJets", "Tree storing event-wise Et, Eta, Phi");
    TTree* leadingTruthAntiKt4TruthDressedWZJets = new TTree("leadingTruthAntiKt4TruthDressedWZJets", "Tree storing event-wise Et, Eta, Phi");
    TTree* subleadingTruthAntiKt4TruthDressedWZJets = new TTree("subleadingTruthAntiKt4TruthDressedWZJets", "Tree storing event-wise Et, Eta, Phi");
    /*TTree* recoAntiKt10LCTopoJets = new TTree("recoAntiKt10LCTopoJets", "Tree storing event-wise Et, Eta, Phi");
    TTree* leadingRecoAntiKt10LCTopoJets = new TTree("leadingRecoAntiKt10LCTopoJets", "Tree storing event-wise Et, Eta, Phi");
    TTree* subleadingRecoAntiKt10LCTopoJets = new TTree("recoAntiKt10LCTopoJets", "Tree storing event-wise Et, Eta, Phi");*/
    //TTree* gFexRhoRoI = new TTree("gFexRhoRoI", "Tree storing event-wise Et, Eta, Phi");

    // Variables to store data to be written to TTrees

    // Event info vectors
    std::vector<double> mcEventWeights;
    double sumOfWeightsForSample;
    int sampleJZSlice;
    std::vector<double> eventWeights;

    // Truth Particle vectors
    //std::vector<int> truthParticlePDGId, truthParticleStatus; 
    //std::vector<double> truthParticleEtValues, truthParticleEnergyValues, truthParticlepTValues, truthParticlepxValues, truthParticlepyValues, truthParticlepzValues, truthParticleEtaValues, truthParticlePhiValues;
    std::vector<unsigned int> higgsIndexValues, indexOfHiggsValues;
    std::vector<double> truthbquarksEtValues, truthbquarksEnergyValues, truthbquarkspTValues, truthbquarkspxValues, truthbquarkspyValues, truthbquarkspzValues, truthbquarksEtaValues, truthbquarksPhiValues;
    std::vector<double> truthHiggsEtValues, truthHiggsEnergyValues, truthHiggspTValues, truthHiggspxValues, truthHiggspyValues, truthHiggspzValues, truthHiggsEtaValues, truthHiggsPhiValues, truthHiggsInvMassValues;
    //std::vector<double> truthVBFQuarkValues, truthVBFQuarkEnergyValues, truthVBFQuarkpTValues, truthVBFQuarkpxValues, truthVBFQuarkpyValues, truthVBFQuarkpzValues, truthVBFQuarkEtaValues, truthVBFQuarkPhiValues;

    // Tower / cluster vectors
    std::vector<double> caloTopoTowerEtValues, caloTopoTowerEtaValues, caloTopoTowerPhiValues;
    std::vector<double> gepBasicClustersEtValues, gepBasicClustersEtaValues, gepBasicClustersPhiValues;
    std::vector<double> gepCellsTowersEtValues, gepCellsTowersEtaValues, gepCellsTowersPhiValues;
    std::vector<double> gepBasicTopoTowersEtValues, gepBasicTopoTowersEtaValues, gepBasicTopoTowersPhiValues;
    std::vector<double> topo422EtValues, topo422EtaValues, topo422PhiValues;

    // Cone jets from TrigGepPerf
    std::vector<double> gepConeCellsTowersJetspTValues, gepConeCellsTowersJetsEtaValues, gepConeCellsTowersJetsPhiValues, gepConeCellsTowersJetsMassValues;
    std::vector<unsigned int > gepConeCellsTowersJetsNConstituentsValues;
    std::vector<double> gepWTAConeCellsTowersJetspTValues, gepWTAConeCellsTowersJetsEtaValues, gepWTAConeCellsTowersJetsPhiValues, gepWTAConeCellsTowersJetsMassValues;
    std::vector<unsigned int > gepWTAConeCellsTowersJetsNConstituentsValues;
    std::vector<double> gepConeGEPBasicClustersJetspTValues, gepConeGEPBasicClustersJetsEtaValues, gepConeGEPBasicClustersJetsPhiValues, gepConeGEPBasicClustersJetsMassValues;
    std::vector<unsigned int > gepConeGEPBasicClustersJetsNConstituentsValues;
    std::vector<double> gepWTAConeGEPBasicClustersJetspTValues, gepWTAConeGEPBasicClustersJetsEtaValues, gepWTAConeGEPBasicClustersJetsPhiValues, gepWTAConeGEPBasicClustersJetsMassValues;
    std::vector<unsigned int > gepWTAConeGEPBasicClustersJetsNConstituentsValues;

    // Leading one jets from TrigGepPerf
    std::vector<double> gepLeadingConeCellsTowersJetspTValues, gepLeadingConeCellsTowersJetsEtaValues, gepLeadingConeCellsTowersJetsPhiValues, gepLeadingConeCellsTowersJetsMassValues;
    std::vector<unsigned int > gepLeadingConeCellsTowersJetsNConstituentsValues;
    std::vector<double> gepLeadingWTAConeCellsTowersJetspTValues, gepLeadingWTAConeCellsTowersJetsEtaValues, gepLeadingWTAConeCellsTowersJetsPhiValues, gepLeadingWTAConeCellsTowersJetsMassValues;
    std::vector<unsigned int > gepLeadingWTAConeCellsTowersJetsNConstituentsValues;
    std::vector<double> gepLeadingConeGEPBasicClustersJetspTValues, gepLeadingConeGEPBasicClustersJetsEtaValues, gepLeadingConeGEPBasicClustersJetsPhiValues, gepLeadingConeGEPBasicClustersJetsMassValues;
    std::vector<unsigned int > gepLeadingConeGEPBasicClustersJetsNConstituentsValues;
    std::vector<double> gepLeadingWTAConeGEPBasicClustersJetspTValues, gepLeadingWTAConeGEPBasicClustersJetsEtaValues, gepLeadingWTAConeGEPBasicClustersJetsPhiValues, gepLeadingWTAConeGEPBasicClustersJetsMassValues;
    std::vector<unsigned int > gepLeadingWTAConeGEPBasicClustersJetsNConstituentsValues;

    // Subleading one jets from TrigGepPerf
    std::vector<double> gepSubleadingConeCellsTowersJetspTValues, gepSubleadingConeCellsTowersJetsEtaValues, gepSubleadingConeCellsTowersJetsPhiValues, gepSubleadingConeCellsTowersJetsMassValues;
    std::vector<unsigned int > gepSubleadingConeCellsTowersJetsNConstituentsValues;
    std::vector<double> gepSubleadingWTAConeCellsTowersJetspTValues, gepSubleadingWTAConeCellsTowersJetsEtaValues, gepSubleadingWTAConeCellsTowersJetsPhiValues, gepSubleadingWTAConeCellsTowersJetsMassValues;
    std::vector<unsigned int > gepSubleadingWTAConeCellsTowersJetsNConstituentsValues;
    std::vector<double> gepSubleadingConeGEPBasicClustersJetspTValues, gepSubleadingConeGEPBasicClustersJetsEtaValues, gepSubleadingConeGEPBasicClustersJetsPhiValues, gepSubleadingConeGEPBasicClustersJetsMassValues;
    std::vector<unsigned int > gepSubleadingConeGEPBasicClustersJetsNConstituentsValues;
    std::vector<double> gepSubleadingWTAConeGEPBasicClustersJetspTValues, gepSubleadingWTAConeGEPBasicClustersJetsEtaValues, gepSubleadingWTAConeGEPBasicClustersJetsPhiValues, gepSubleadingWTAConeGEPBasicClustersJetsMassValues;
    std::vector<unsigned int > gepSubleadingWTAConeGEPBasicClustersJetsNConstituentsValues;

    // L1Calo jets vectors
    std::vector<unsigned int> gFexSRJEtIndexValues; // stores index in sorted by Et list of jets
    std::vector<double> gFexSRJEtValues, gFexSRJEtaValues, gFexSRJPhiValues;
    std::vector<double> gFexSRJLeadingEtValues, gFexSRJLeadingEtaValues, gFexSRJLeadingPhiValues;
    std::vector<double> gFexSRJSubleadingEtValues, gFexSRJSubleadingEtaValues, gFexSRJSubleadingPhiValues;
    std::vector<unsigned int> gFexLRJEtIndexValues; // stores index in sorted by Et list of jets
    std::vector<double> gFexLRJEtValues, gFexLRJEtaValues, gFexLRJPhiValues;
    std::vector<double> gFexLRJLeadingEtValues, gFexLRJLeadingEtaValues, gFexLRJLeadingPhiValues;
    std::vector<double> gFexLRJSubleadingEtValues, gFexLRJSubleadingEtaValues, gFexLRJSubleadingPhiValues;
    std::vector<unsigned int> jFexSRJEtIndexValues;
    std::vector<double> jFexSRJEtValues, jFexSRJEtaValues, jFexSRJPhiValues;
    std::vector<double> jFexSRJLeadingEtValues, jFexSRJLeadingEtaValues, jFexSRJLeadingPhiValues;
    std::vector<double> jFexSRJSubleadingEtValues, jFexSRJSubleadingEtaValues, jFexSRJSubleadingPhiValues;
    std::vector<unsigned int> jFexLRJEtIndexValues;
    std::vector<double> jFexLRJEtValues, jFexLRJEtaValues, jFexLRJPhiValues;
    std::vector<double> jFexLRJLeadingEtValues, jFexLRJLeadingEtaValues, jFexLRJLeadingPhiValues;
    std::vector<double> jFexLRJSubleadingEtValues, jFexLRJSubleadingEtaValues, jFexLRJSubleadingPhiValues;
    //std::vector<double> gFexRhoRoIEtValues, gFexRhoRoIEtaValues, gFexRhoRoIPhiValues; // skip adding these for now.

    // HLT jets vectors
    std::vector<unsigned int> hltAntiKt4SRJEtIndexValues; // stores index in sorted by Et list of jets
    std::vector<double> hltAntiKt4SRJEtValues, hltAntiKt4SRJEtaValues, hltAntiKt4SRJPhiValues;
    std::vector<double> hltAntiKt4SRJLeadingEtValues, hltAntiKt4SRJLeadingEtaValues, hltAntiKt4SRJLeadingPhiValues;
    std::vector<double> hltAntiKt4SRJSubleadingEtValues, hltAntiKt4SRJSubleadingEtaValues, hltAntiKt4SRJSubleadingPhiValues;

    // Reco offline jets vectors
    std::vector<unsigned int> recoAntiKt10LRJEtIndexValues;
    std::vector<double> recoAntiKt10LRJEtValues, recoAntiKt10LRJEtaValues, recoAntiKt10LRJPhiValues, recoAntiKt10LRJMassValues;
    std::vector<double> recoAntiKt10LRJLeadingEtValues, recoAntiKt10LRJLeadingEtaValues, recoAntiKt10LRJLeadingPhiValues, recoAntiKt10LRJLeadingMassValues;
    std::vector<double> recoAntiKt10LRJSubleadingEtValues, recoAntiKt10LRJSubleadingEtaValues, recoAntiKt10LRJSubleadingPhiValues, recoAntiKt10LRJSubleadingMassValues;

    // Truth WZ Antikt4 jets vectors
    std::vector<unsigned int> truthAntiKt4WZSRJEtIndexValues;
    std::vector<double> truthAntiKt4WZSRJEtValues, truthAntiKt4WZSRJEtaValues, truthAntiKt4WZSRJPhiValues, truthAntiKt4WZSRJMassValues;
    std::vector<double> truthAntiKt4WZSRJLeadingEtValues, truthAntiKt4WZSRJLeadingEtaValues, truthAntiKt4WZSRJLeadingPhiValues, truthAntiKt4WZSRJLeadingMassValues;
    std::vector<double> truthAntiKt4WZSRJSubleadingEtValues, truthAntiKt4WZSRJSubleadingEtaValues, truthAntiKt4WZSRJSubleadingPhiValues, truthAntiKt4WZSRJSubleadingMassValues;

    // In time anti-kt 4 truth jets vectors
    std::vector<unsigned int> inTimeAntiKt4TruthSRJEtIndexValues;
    std::vector<double> inTimeAntiKt4TruthSRJEtValues, inTimeAntiKt4TruthSRJEtaValues, inTimeAntiKt4TruthSRJPhiValues;
    std::vector<double> inTimeAntiKt4TruthSRJLeadingEtValues, inTimeAntiKt4TruthSRJLeadingEtaValues, inTimeAntiKt4TruthSRJLeadingPhiValues;
    std::vector<double> inTimeAntiKt4TruthSRJSubleadingEtValues, inTimeAntiKt4TruthSRJSubleadingEtaValues, inTimeAntiKt4TruthSRJSubleadingPhiValues;

    // Create branches for each TTree FIXME don't have access to full truth record for DAOD samples
    /*
    truthParticleTree->Branch("Et", &truthParticleEtValues);
    truthParticleTree->Branch("Eta", &truthParticleEtaValues);
    truthParticleTree->Branch("Phi", &truthParticlePhiValues);
    truthParticleTree->Branch("pdgId", &truthParticlePDGId);
    truthParticleTree->Branch("pdgStatus", &truthParticleStatus);
    truthParticleTree->Branch("pT", &truthParticlepTValues);
    truthParticleTree->Branch("px", &truthParticlepxValues);
    truthParticleTree->Branch("py", &truthParticlepyValues);
    truthParticleTree->Branch("pz", &truthParticlepzValues);*/

    // event info tree
    eventInfoTree->Branch("mcEventWeight", &mcEventWeights);
    eventInfoTree->Branch("sumOfWeightsForSample", &sumOfWeightsForSample);
    eventInfoTree->Branch("eventWeights", &eventWeights);
    eventInfoTree->Branch("sampleJZSlice", &sampleJZSlice);

    // truthbTree
    truthbTree->Branch("higgsIndex", &higgsIndexValues);
    truthbTree->Branch("Et", &truthbquarksEtValues);
    truthbTree->Branch("Eta", &truthbquarksEtaValues);
    truthbTree->Branch("Phi", &truthbquarksPhiValues);
    truthbTree->Branch("pT", &truthbquarkspTValues);
    truthbTree->Branch("px", &truthbquarkspxValues);
    truthbTree->Branch("py", &truthbquarkspyValues);
    truthbTree->Branch("pz", &truthbquarkspzValues);
    truthbTree->Branch("Energy", &truthbquarksEnergyValues);

    // truthHiggsTree
    truthHiggsTree->Branch("indexOfHiggs", &indexOfHiggsValues);
    truthHiggsTree->Branch("invMass", &truthHiggsInvMassValues);
    truthHiggsTree->Branch("Et", &truthHiggsEtValues);
    truthHiggsTree->Branch("Eta", &truthHiggsEtaValues);
    truthHiggsTree->Branch("Phi", &truthHiggsPhiValues);
    truthHiggsTree->Branch("pT", &truthHiggspTValues);
    truthHiggsTree->Branch("px", &truthHiggspxValues);
    truthHiggsTree->Branch("py", &truthHiggspyValues);
    truthHiggsTree->Branch("pz", &truthHiggspzValues);
    truthHiggsTree->Branch("Energy", &truthHiggsEnergyValues);

    // truthVBFQuark FIXME don't have for DAOD samples - unless can find corresponding aod events for which full truth record is stored
    /*
    truthVBFQuark->Branch("Et", &truthVBFQuarkValues);
    truthVBFQuark->Branch("Eta", &truthVBFQuarkEtaValues);
    truthVBFQuark->Branch("Phi", &truthVBFQuarkPhiValues);
    truthVBFQuark->Branch("pT", &truthVBFQuarkpTValues);
    truthVBFQuark->Branch("px", &truthVBFQuarkpxValues);
    truthVBFQuark->Branch("py", &truthVBFQuarkpyValues);
    truthVBFQuark->Branch("pz", &truthVBFQuarkpzValues);
    truthVBFQuark->Branch("Energy", &truthVBFQuarkEnergyValues);*/

    // caloTopoTowerTree
    caloTopoTowerTree->Branch("Et", &caloTopoTowerEtValues);
    caloTopoTowerTree->Branch("Eta", &caloTopoTowerEtaValues);
    caloTopoTowerTree->Branch("Phi", &caloTopoTowerPhiValues);

    // gepBasicClustersTree
    gepBasicClustersTree->Branch("Et", &gepBasicClustersEtValues);
    gepBasicClustersTree->Branch("Eta", &gepBasicClustersEtaValues);
    gepBasicClustersTree->Branch("Phi", &gepBasicClustersPhiValues);

    // gepCellsTowersTree
    gepCellsTowersTree->Branch("Et", &gepCellsTowersEtValues);
    gepCellsTowersTree->Branch("Eta", &gepCellsTowersEtaValues);
    gepCellsTowersTree->Branch("Phi", &gepCellsTowersPhiValues);

    // gepBasicTopoTowersTree
    gepBasicTopoTowersTree->Branch("Et", &gepBasicTopoTowersEtValues);
    gepBasicTopoTowersTree->Branch("Eta", &gepBasicTopoTowersEtaValues);
    gepBasicTopoTowersTree->Branch("Phi", &gepBasicTopoTowersPhiValues);

    // gep cone cells towers jets
    gepConeCellsTowersJetsTree->Branch("pT", &gepConeCellsTowersJetspTValues);
    gepConeCellsTowersJetsTree->Branch("Eta", &gepConeCellsTowersJetsEtaValues);
    gepConeCellsTowersJetsTree->Branch("Phi", &gepConeCellsTowersJetsPhiValues);
    gepConeCellsTowersJetsTree->Branch("Mass", &gepConeCellsTowersJetsMassValues);
    gepConeCellsTowersJetsTree->Branch("NConstituents", &gepConeCellsTowersJetsNConstituentsValues);

    // gep cone cells towers jets
    gepLeadingConeCellsTowersJetsTree->Branch("pT", &gepLeadingConeCellsTowersJetspTValues);
    gepLeadingConeCellsTowersJetsTree->Branch("Eta", &gepLeadingConeCellsTowersJetsEtaValues);
    gepLeadingConeCellsTowersJetsTree->Branch("Phi", &gepLeadingConeCellsTowersJetsPhiValues);
    gepLeadingConeCellsTowersJetsTree->Branch("Mass", &gepLeadingConeCellsTowersJetsMassValues);
    gepLeadingConeCellsTowersJetsTree->Branch("NConstituents", &gepLeadingConeCellsTowersJetsNConstituentsValues);

    // gep cone cells towers jets
    gepSubleadingConeCellsTowersJetsTree->Branch("pT", &gepSubleadingConeCellsTowersJetspTValues);
    gepSubleadingConeCellsTowersJetsTree->Branch("Eta", &gepSubleadingConeCellsTowersJetsEtaValues);
    gepSubleadingConeCellsTowersJetsTree->Branch("Phi", &gepSubleadingConeCellsTowersJetsPhiValues);
    gepSubleadingConeCellsTowersJetsTree->Branch("Mass", &gepSubleadingConeCellsTowersJetsMassValues);
    gepSubleadingConeCellsTowersJetsTree->Branch("NConstituents", &gepSubleadingConeCellsTowersJetsNConstituentsValues);

    // gep wta cone cells towers jets
    gepWTAConeCellsTowersJetsTree->Branch("pT", &gepWTAConeCellsTowersJetspTValues);
    gepWTAConeCellsTowersJetsTree->Branch("Eta", &gepWTAConeCellsTowersJetsEtaValues);
    gepWTAConeCellsTowersJetsTree->Branch("Phi", &gepWTAConeCellsTowersJetsPhiValues);
    gepWTAConeCellsTowersJetsTree->Branch("Mass", &gepWTAConeCellsTowersJetsMassValues);
    gepWTAConeCellsTowersJetsTree->Branch("NConstituents", &gepWTAConeCellsTowersJetsNConstituentsValues);

    // gep wta cone cells towers jets
    gepLeadingWTAConeCellsTowersJetsTree->Branch("pT", &gepLeadingWTAConeCellsTowersJetspTValues);
    gepLeadingWTAConeCellsTowersJetsTree->Branch("Eta", &gepLeadingWTAConeCellsTowersJetsEtaValues);
    gepLeadingWTAConeCellsTowersJetsTree->Branch("Phi", &gepLeadingWTAConeCellsTowersJetsPhiValues);
    gepLeadingWTAConeCellsTowersJetsTree->Branch("Mass", &gepLeadingWTAConeCellsTowersJetsMassValues);
    gepLeadingWTAConeCellsTowersJetsTree->Branch("NConstituents", &gepLeadingWTAConeCellsTowersJetsNConstituentsValues);

    // gep wta cone cells towers jets
    gepSubleadingWTAConeCellsTowersJetsTree->Branch("pT", &gepSubleadingWTAConeCellsTowersJetspTValues);
    gepSubleadingWTAConeCellsTowersJetsTree->Branch("Eta", &gepSubleadingWTAConeCellsTowersJetsEtaValues);
    gepSubleadingWTAConeCellsTowersJetsTree->Branch("Phi", &gepSubleadingWTAConeCellsTowersJetsPhiValues);
    gepSubleadingWTAConeCellsTowersJetsTree->Branch("Mass", &gepSubleadingWTAConeCellsTowersJetsMassValues);
    gepSubleadingWTAConeCellsTowersJetsTree->Branch("NConstituents", &gepSubleadingWTAConeCellsTowersJetsNConstituentsValues);

    // gep cone basic clusters jets
    gepConeBasicClustersJetsTree->Branch("pT", &gepConeGEPBasicClustersJetspTValues);
    gepConeBasicClustersJetsTree->Branch("Eta", &gepConeGEPBasicClustersJetsEtaValues);
    gepConeBasicClustersJetsTree->Branch("Phi", &gepConeGEPBasicClustersJetsPhiValues);
    gepConeBasicClustersJetsTree->Branch("Mass", &gepConeGEPBasicClustersJetsMassValues);
    gepConeBasicClustersJetsTree->Branch("NConstituents", &gepConeGEPBasicClustersJetsNConstituentsValues);

    // gep cone basic clusters jets
    gepLeadingConeBasicClustersJetsTree->Branch("pT", &gepLeadingConeGEPBasicClustersJetspTValues);
    gepLeadingConeBasicClustersJetsTree->Branch("Eta", &gepLeadingConeGEPBasicClustersJetsEtaValues);
    gepLeadingConeBasicClustersJetsTree->Branch("Phi", &gepLeadingConeGEPBasicClustersJetsPhiValues);
    gepLeadingConeBasicClustersJetsTree->Branch("Mass", &gepLeadingConeGEPBasicClustersJetsMassValues);
    gepLeadingConeBasicClustersJetsTree->Branch("NConstituents", &gepLeadingConeGEPBasicClustersJetsNConstituentsValues);

    // gep cone basic clusters jets
    gepSubleadingConeBasicClustersJetsTree->Branch("pT", &gepSubleadingConeGEPBasicClustersJetspTValues);
    gepSubleadingConeBasicClustersJetsTree->Branch("Eta", &gepSubleadingConeGEPBasicClustersJetsEtaValues);
    gepSubleadingConeBasicClustersJetsTree->Branch("Phi", &gepSubleadingConeGEPBasicClustersJetsPhiValues);
    gepSubleadingConeBasicClustersJetsTree->Branch("Mass", &gepSubleadingConeGEPBasicClustersJetsMassValues);
    gepSubleadingConeBasicClustersJetsTree->Branch("NConstituents", &gepSubleadingConeGEPBasicClustersJetsNConstituentsValues);

    // gep wta cone basic clusters jets
    gepWTAConeBasicClustersJetsTree->Branch("pT", &gepWTAConeGEPBasicClustersJetspTValues);
    gepWTAConeBasicClustersJetsTree->Branch("Eta", &gepWTAConeGEPBasicClustersJetsEtaValues);
    gepWTAConeBasicClustersJetsTree->Branch("Phi", &gepWTAConeGEPBasicClustersJetsPhiValues);
    gepWTAConeBasicClustersJetsTree->Branch("Mass", &gepWTAConeGEPBasicClustersJetsMassValues);
    gepWTAConeBasicClustersJetsTree->Branch("NConstituents", &gepWTAConeGEPBasicClustersJetsNConstituentsValues);

    // gep wta cone basic clusters jets
    gepLeadingWTAConeBasicClustersJetsTree->Branch("pT", &gepLeadingWTAConeGEPBasicClustersJetspTValues);
    gepLeadingWTAConeBasicClustersJetsTree->Branch("Eta", &gepLeadingWTAConeGEPBasicClustersJetsEtaValues);
    gepLeadingWTAConeBasicClustersJetsTree->Branch("Phi", &gepLeadingWTAConeGEPBasicClustersJetsPhiValues);
    gepLeadingWTAConeBasicClustersJetsTree->Branch("Mass", &gepLeadingWTAConeGEPBasicClustersJetsMassValues);
    gepLeadingWTAConeBasicClustersJetsTree->Branch("NConstituents", &gepLeadingWTAConeGEPBasicClustersJetsNConstituentsValues);

    // gep wta cone basic clusters jets
    gepSubleadingWTAConeBasicClustersJetsTree->Branch("pT", &gepSubleadingWTAConeGEPBasicClustersJetspTValues);
    gepSubleadingWTAConeBasicClustersJetsTree->Branch("Eta", &gepSubleadingWTAConeGEPBasicClustersJetsEtaValues);
    gepSubleadingWTAConeBasicClustersJetsTree->Branch("Phi", &gepSubleadingWTAConeGEPBasicClustersJetsPhiValues);
    gepSubleadingWTAConeBasicClustersJetsTree->Branch("Mass", &gepSubleadingWTAConeGEPBasicClustersJetsMassValues);
    gepSubleadingWTAConeBasicClustersJetsTree->Branch("NConstituents", &gepSubleadingWTAConeGEPBasicClustersJetsNConstituentsValues);


    // topo422Tree
    topo422Tree->Branch("Et", &topo422EtValues);
    topo422Tree->Branch("Eta", &topo422EtaValues);
    topo422Tree->Branch("Phi", &topo422PhiValues);

    // gFexSRJTree
    gFexSRJTree->Branch("EtIndex", &gFexSRJEtIndexValues);
    gFexSRJTree->Branch("Et", &gFexSRJEtValues);
    gFexSRJTree->Branch("Eta", &gFexSRJEtaValues);
    gFexSRJTree->Branch("Phi", &gFexSRJPhiValues);

    // gFexLeadingSRJTree
    gFexLeadingSRJTree->Branch("Et", &gFexSRJLeadingEtValues);
    gFexLeadingSRJTree->Branch("Eta", &gFexSRJLeadingEtaValues);
    gFexLeadingSRJTree->Branch("Phi", &gFexSRJLeadingPhiValues);

    // gFexSubleadingSRJTree
    gFexSubleadingSRJTree->Branch("Et", &gFexSRJSubleadingEtValues);
    gFexSubleadingSRJTree->Branch("Eta", &gFexSRJSubleadingEtaValues);
    gFexSubleadingSRJTree->Branch("Phi", &gFexSRJSubleadingPhiValues);

    // gFexLRJTree
    gFexLRJTree->Branch("EtIndex", &gFexLRJEtIndexValues);
    gFexLRJTree->Branch("Et", &gFexLRJEtValues);
    gFexLRJTree->Branch("Eta", &gFexLRJEtaValues);
    gFexLRJTree->Branch("Phi", &gFexLRJPhiValues);

    // gFexLeadingLRJTree
    gFexLeadingLRJTree->Branch("Et", &gFexLRJLeadingEtValues);
    gFexLeadingLRJTree->Branch("Eta", &gFexLRJLeadingEtaValues);
    gFexLeadingLRJTree->Branch("Phi", &gFexLRJLeadingPhiValues);

    // gFexSubleadingLRJTree
    gFexSubleadingLRJTree->Branch("Et", &gFexLRJSubleadingEtValues);
    gFexSubleadingLRJTree->Branch("Eta", &gFexLRJSubleadingEtaValues);
    gFexSubleadingLRJTree->Branch("Phi", &gFexLRJSubleadingPhiValues);

    // inTimeAntiKt4TruthJetsTree
    inTimeAntiKt4TruthJetsTree->Branch("EtIndex", &inTimeAntiKt4TruthSRJEtIndexValues);
    inTimeAntiKt4TruthJetsTree->Branch("Et", &inTimeAntiKt4TruthSRJEtValues);
    inTimeAntiKt4TruthJetsTree->Branch("Eta", &inTimeAntiKt4TruthSRJEtaValues);
    inTimeAntiKt4TruthJetsTree->Branch("Phi", &inTimeAntiKt4TruthSRJPhiValues);

    // leadingInTimeAntiKt4TruthJetsTree
    leadingInTimeAntiKt4TruthJetsTree->Branch("Et", &inTimeAntiKt4TruthSRJLeadingEtValues);
    leadingInTimeAntiKt4TruthJetsTree->Branch("Eta", &inTimeAntiKt4TruthSRJLeadingEtaValues);
    leadingInTimeAntiKt4TruthJetsTree->Branch("Phi", &inTimeAntiKt4TruthSRJLeadingPhiValues);

    // subleadingInTimeAntiKt4TruthJetsTree
    subleadingInTimeAntiKt4TruthJetsTree->Branch("Et", &inTimeAntiKt4TruthSRJSubleadingEtValues);
    subleadingInTimeAntiKt4TruthJetsTree->Branch("Eta", &inTimeAntiKt4TruthSRJSubleadingEtaValues);
    subleadingInTimeAntiKt4TruthJetsTree->Branch("Phi", &inTimeAntiKt4TruthSRJSubleadingPhiValues);

    // jFexSRJTree
    jFexSRJTree->Branch("EtIndex", &jFexSRJEtIndexValues);
    jFexSRJTree->Branch("Et", &jFexSRJEtValues);
    jFexSRJTree->Branch("Eta", &jFexSRJEtaValues);
    jFexSRJTree->Branch("Phi", &jFexSRJPhiValues);

    // jFexLeadingSRJTree
    jFexLeadingSRJTree->Branch("Et", &jFexSRJLeadingEtValues);
    jFexLeadingSRJTree->Branch("Eta", &jFexSRJLeadingEtaValues);
    jFexLeadingSRJTree->Branch("Phi", &jFexSRJLeadingPhiValues);

    // jFexSubleadingSRJTree
    jFexSubleadingSRJTree->Branch("Et", &jFexSRJSubleadingEtValues);
    jFexSubleadingSRJTree->Branch("Eta", &jFexSRJSubleadingEtaValues);
    jFexSubleadingSRJTree->Branch("Phi", &jFexSRJSubleadingPhiValues);

    // jFexLRJTree
    jFexLRJTree->Branch("EtIndex", &jFexLRJEtIndexValues);
    jFexLRJTree->Branch("Et", &jFexLRJEtValues);
    jFexLRJTree->Branch("Eta", &jFexLRJEtaValues);
    jFexLRJTree->Branch("Phi", &jFexLRJPhiValues);

    // jFexLeadingLRJTree
    jFexLeadingLRJTree->Branch("Et", &jFexLRJLeadingEtValues);
    jFexLeadingLRJTree->Branch("Eta", &jFexLRJLeadingEtaValues);
    jFexLeadingLRJTree->Branch("Phi", &jFexLRJLeadingPhiValues);

    // jFexSubleadingLRJTree
    jFexSubleadingLRJTree->Branch("Et", &jFexLRJSubleadingEtValues);
    jFexSubleadingLRJTree->Branch("Eta", &jFexLRJSubleadingEtaValues);
    jFexSubleadingLRJTree->Branch("Phi", &jFexLRJSubleadingPhiValues);

    // hltAntiKt4EMTopoJetsTreehltAntiKt4SRJEtIndexValues
    hltAntiKt4EMTopoJetsTree->Branch("EtIndex", &recoAntiKt10LRJEtIndexValues);
    hltAntiKt4EMTopoJetsTree->Branch("Et", &hltAntiKt4SRJEtValues);
    hltAntiKt4EMTopoJetsTree->Branch("Eta", &hltAntiKt4SRJEtaValues);
    hltAntiKt4EMTopoJetsTree->Branch("Phi", &hltAntiKt4SRJPhiValues);

    // leadingHltAntiKt4EMTopoJetsTree
    leadingHltAntiKt4EMTopoJetsTree->Branch("Et", &hltAntiKt4SRJLeadingEtValues);
    leadingHltAntiKt4EMTopoJetsTree->Branch("Eta", &hltAntiKt4SRJLeadingEtaValues);
    leadingHltAntiKt4EMTopoJetsTree->Branch("Phi", &hltAntiKt4SRJLeadingPhiValues);

    // subleadingHltAntiKt4EMTopoJetsTree
    subleadingHltAntiKt4EMTopoJetsTree->Branch("Et", &hltAntiKt4SRJSubleadingEtValues);
    subleadingHltAntiKt4EMTopoJetsTree->Branch("Eta", &hltAntiKt4SRJSubleadingEtaValues);
    subleadingHltAntiKt4EMTopoJetsTree->Branch("Phi", &hltAntiKt4SRJSubleadingPhiValues);

    // recoAntiKt10UFOCSSKJets
    recoAntiKt10UFOCSSKJets->Branch("EtIndex", &recoAntiKt10LRJEtIndexValues);
    recoAntiKt10UFOCSSKJets->Branch("Et", &recoAntiKt10LRJEtValues);
    recoAntiKt10UFOCSSKJets->Branch("Eta", &recoAntiKt10LRJEtaValues);
    recoAntiKt10UFOCSSKJets->Branch("Phi", &recoAntiKt10LRJPhiValues);
    recoAntiKt10UFOCSSKJets->Branch("Mass", &recoAntiKt10LRJMassValues);

    // leadingRecoAntiKt10UFOCSSKJets
    leadingRecoAntiKt10UFOCSSKJets->Branch("Et", &recoAntiKt10LRJLeadingEtValues);
    leadingRecoAntiKt10UFOCSSKJets->Branch("Eta", &recoAntiKt10LRJLeadingEtaValues);
    leadingRecoAntiKt10UFOCSSKJets->Branch("Phi", &recoAntiKt10LRJLeadingPhiValues);
    leadingRecoAntiKt10UFOCSSKJets->Branch("Mass", &recoAntiKt10LRJLeadingMassValues);

    // subleadingRecoAntiKt10UFOCSSKJets
    subleadingRecoAntiKt10UFOCSSKJets->Branch("Et", &recoAntiKt10LRJSubleadingEtValues);
    subleadingRecoAntiKt10UFOCSSKJets->Branch("Eta", &recoAntiKt10LRJSubleadingEtaValues);
    subleadingRecoAntiKt10UFOCSSKJets->Branch("Phi", &recoAntiKt10LRJSubleadingPhiValues);
    subleadingRecoAntiKt10UFOCSSKJets->Branch("Mass", &recoAntiKt10LRJSubleadingMassValues);

    // truthAntiKt4TruthDressedWZJets
    truthAntiKt4TruthDressedWZJets->Branch("EtIndex", &truthAntiKt4WZSRJEtIndexValues);
    truthAntiKt4TruthDressedWZJets->Branch("Et", &truthAntiKt4WZSRJEtValues);
    truthAntiKt4TruthDressedWZJets->Branch("Eta", &truthAntiKt4WZSRJEtaValues);
    truthAntiKt4TruthDressedWZJets->Branch("Phi", &truthAntiKt4WZSRJPhiValues);
    truthAntiKt4TruthDressedWZJets->Branch("Mass", &truthAntiKt4WZSRJMassValues);

    // leadingTruthAntiKt4TruthDressedWZJets
    leadingTruthAntiKt4TruthDressedWZJets->Branch("Et", &truthAntiKt4WZSRJLeadingEtValues);
    leadingTruthAntiKt4TruthDressedWZJets->Branch("Eta", &truthAntiKt4WZSRJLeadingEtaValues);
    leadingTruthAntiKt4TruthDressedWZJets->Branch("Phi", &truthAntiKt4WZSRJLeadingPhiValues);
    leadingTruthAntiKt4TruthDressedWZJets->Branch("Mass", &truthAntiKt4WZSRJLeadingMassValues);

    // subleadingTruthAntiKt4TruthDressedWZJets
    subleadingTruthAntiKt4TruthDressedWZJets->Branch("Et", &truthAntiKt4WZSRJSubleadingEtValues);
    subleadingTruthAntiKt4TruthDressedWZJets->Branch("Eta", &truthAntiKt4WZSRJSubleadingEtaValues);
    subleadingTruthAntiKt4TruthDressedWZJets->Branch("Phi", &truthAntiKt4WZSRJSubleadingPhiValues);
    subleadingTruthAntiKt4TruthDressedWZJets->Branch("Mass", &truthAntiKt4WZSRJSubleadingMassValues);

    

    // recoAntiKt10LCTopoJets - unsure if want to use these at all?? 
    /*
    recoAntiKt10LCTopoJets->Branch("Et", &recoAntiKt10LRJEtValues);
    recoAntiKt10LCTopoJets->Branch("Eta", &recoAntiKt10LRJEtaValues);
    recoAntiKt10LCTopoJets->Branch("Phi", &recoAntiKt10LRJPhiValues);

    // leadingRecoAntiKt10LCTopoJets
    leadingRecoAntiKt10LCTopoJets->Branch("Et", &recoAntiKt10LRJLeadingEtValues);
    leadingRecoAntiKt10LCTopoJets->Branch("Eta", &recoAntiKt10LRJLeadingEtaValues);
    leadingRecoAntiKt10LCTopoJets->Branch("Phi", &recoAntiKt10LRJLeadingPhiValues);

    // subleadingRecoAntiKt10LCTopoJets
    subleadingRecoAntiKt10LCTopoJets->Branch("Et", &recoAntiKt10LRJSubleadingEtValues);
    subleadingRecoAntiKt10LCTopoJets->Branch("Eta", &recoAntiKt10LRJSubleadingEtaValues);
    subleadingRecoAntiKt10LCTopoJets->Branch("Phi", &recoAntiKt10LRJSubleadingPhiValues);*/

    // gFexRhoRoI
    /*gFexRhoRoI->Branch("Et", &gFexRhoRoIEtValues);
    gFexRhoRoI->Branch("Eta", &gFexRhoRoIEtaValues);
    gFexRhoRoI->Branch("Phi", &gFexRhoRoIPhiValues);*/
    int higgsPassEventCounter = 0;
    namespace fs = std::filesystem;
    // Collect input file names
    std::vector<std::string> fileNames;     // DAOD_JETM42*.root
    std::vector<std::string> gepFileNames;  // GEP*.root

    auto has_prefix = [](const std::string& s, const std::string& p) {
        return s.rfind(p, 0) == 0; // starts with
    };
    auto has_suffix = [](const std::string& s, const std::string& suf) {
        return s.size() >= suf.size() &&
            s.compare(s.size() - suf.size(), suf.size(), suf) == 0; // ends with
    };

    // Loop 1: DAOD_JETM42*.root (from derivation of an AOD)
    for (const auto& entry : fs::directory_iterator(fileDir)) {
        if (!entry.is_regular_file()) continue;
        const std::string fn = entry.path().filename().string();
        if (has_prefix(fn, "DAOD_JETM42") && has_suffix(fn, ".root")) {
            fileNames.push_back(entry.path().string());
        }
    }

    // Loop 2: GEP*.root - load TrigGepPerf output ROOT file
    for (const auto& entry : fs::directory_iterator(fileDir)) {
        if (!entry.is_regular_file()) continue;
        const std::string fn = entry.path().filename().string();
        if (has_prefix(fn, "GEP") && has_suffix(fn, ".root")) {
            gepFileNames.push_back(entry.path().string());
        }
    }

    if (fileNames.empty()) {
        cout << "No ROOT files found in directory." << endl;
        return;
    }

    cout << "Found " << fileNames.size() << " files." << endl;

    // Main processing loop
    int fileIt = 0;
    for (const auto& fileName : fileNames) {
        fileIt++; 
        //if (fileIt > 9) break; 
        cout << "Processing file: " << fileName << endl;
        std::cout << "Processing GEP file: " << gepFileNames[fileIt - 1] << "\n";

        TFile* f = TFile::Open(fileName.c_str());
        if (!f || f->IsZombie()) {
            cerr << "Could not open " << fileName << endl;
            continue;
        }

        // Open the DAOD file
        xAOD::TEvent event(xAOD::TEvent::kClassAccess);
        if (!event.readFrom(f).isSuccess()) {
            cerr << "Cannot read xAOD from file." << endl;
            continue;
        }

        // --- Open GEP ROOT (non-xAOD) ---
        TFile* gf = TFile::Open(gepFileNames[fileIt - 1].c_str());
        if (!gf || gf->IsZombie()) {
            std::cerr << "Could not open GEP file: " << gepFileNames[fileIt - 1] << "\n";
            if (gf) { gf->Close(); delete gf; }
            f->Close(); delete f;
            continue;
        }

        TTree* gt = nullptr;
        gf->GetObject("ntuple", gt);
        if (!gt) {
            std::cerr << "GEP file has no TTree named 'ntuple'\n";
            gf->Close(); delete gf;
            f->Close(); delete f;
            continue;
        }

        // Enable needed branches from TrigGepPerf 
        gt->SetBranchStatus("*", 0);
        gt->SetBranchStatus("GEPBasicClusters_et",  1);
        gt->SetBranchStatus("GEPBasicClusters_eta", 1);
        gt->SetBranchStatus("GEPBasicClusters_phi", 1);
        gt->SetBranchStatus("GEPCellsTower_et",  1);
        gt->SetBranchStatus("GEPCellsTower_eta", 1);
        gt->SetBranchStatus("GEPCellsTower_phi", 1);
        gt->SetBranchStatus("GEPBasicTopoTower_et",  1);
        gt->SetBranchStatus("GEPBasicTopoTower_eta", 1);
        gt->SetBranchStatus("GEPBasicTopoTower_phi", 1);

        gt->SetBranchStatus("ConeGEPCellsTowerJets_pt",  1);
        gt->SetBranchStatus("ConeGEPCellsTowerJets_eta", 1);
        gt->SetBranchStatus("ConeGEPCellsTowerJets_phi", 1);
        gt->SetBranchStatus("ConeGEPCellsTowerJets_m", 1);
        gt->SetBranchStatus("ConeGEPCellsTowerJets_nConstituents", 1);

        gt->SetBranchStatus("WTAConeGEPCellsTowerJets_pt",  1);
        gt->SetBranchStatus("WTAConeGEPCellsTowerJets_eta", 1);
        gt->SetBranchStatus("WTAConeGEPCellsTowerJets_phi", 1);
        gt->SetBranchStatus("WTAConeGEPCellsTowerJets_m", 1);
        gt->SetBranchStatus("WTAConeGEPCellsTowerJets_nConstituents", 1);

        gt->SetBranchStatus("ConeGEPBasicClustersJets_pt",  1);
        gt->SetBranchStatus("ConeGEPBasicClustersJets_eta", 1);
        gt->SetBranchStatus("ConeGEPBasicClustersJets_phi", 1);
        gt->SetBranchStatus("ConeGEPBasicClustersJets_m", 1);
        gt->SetBranchStatus("ConeGEPBasicClustersJets_nConstituents", 1);

        gt->SetBranchStatus("WTAConeGEPBasicClustersJets_pt",  1);
        gt->SetBranchStatus("WTAConeGEPBasicClustersJets_eta", 1);
        gt->SetBranchStatus("WTAConeGEPBasicClustersJets_phi", 1);
        gt->SetBranchStatus("WTAConeGEPBasicClustersJets_m", 1);
        gt->SetBranchStatus("WTAConeGEPBasicClustersJets_nConstituents", 1);

        std::vector<float>* gepBasicClustersEt  = nullptr;
        std::vector<float>* gepBasicClustersEta = nullptr;
        std::vector<float>* gepBasicClustersPhi = nullptr;

        std::vector<float>* gepCellsTowersEt  = nullptr;
        std::vector<float>* gepCellsTowersEta = nullptr;
        std::vector<float>* gepCellsTowersPhi = nullptr;

        std::vector<float>* gepBasicTopoTowersEt  = nullptr;
        std::vector<float>* gepBasicTopoTowersEta = nullptr;
        std::vector<float>* gepBasicTopoTowersPhi = nullptr;

        std::vector<float>* gepConeCellsTowersJetsPt  = nullptr;
        std::vector<float>* gepConeCellsTowersJetsEta = nullptr;
        std::vector<float>* gepConeCellsTowersJetsPhi = nullptr;
        std::vector<float>* gepConeCellsTowersJetsMass = nullptr;
        std::vector<float>* gepConeCellsTowersJetsNConstituents = nullptr;

        std::vector<float>* gepWTAConeCellsTowersJetsPt  = nullptr;
        std::vector<float>* gepWTAConeCellsTowersJetsEta = nullptr;
        std::vector<float>* gepWTAConeCellsTowersJetsPhi = nullptr;
        std::vector<float>* gepWTAConeCellsTowersJetsMass = nullptr;
        std::vector<float>* gepWTAConeCellsTowersJetsNConstituents = nullptr;

        std::vector<float>* gepConeBasicClustersJetsPt  = nullptr;
        std::vector<float>* gepConeBasicClustersJetsEta = nullptr;
        std::vector<float>* gepConeBasicClustersJetsPhi = nullptr;
        std::vector<float>* gepConeBasicClustersJetsMass = nullptr;
        std::vector<float>* gepConeBasicClustersJetsNConstituents = nullptr;

        std::vector<float>* gepWTAConeBasicClustersJetsPt  = nullptr;
        std::vector<float>* gepWTAConeBasicClustersJetsEta = nullptr;
        std::vector<float>* gepWTAConeBasicClustersJetsPhi = nullptr;
        std::vector<float>* gepWTAConeBasicClustersJetsMass = nullptr;
        std::vector<float>* gepWTAConeBasicClustersJetsNConstituents = nullptr;

        gt->SetBranchAddress("GEPBasicClusters_et",  &gepBasicClustersEt);
        gt->SetBranchAddress("GEPBasicClusters_eta", &gepBasicClustersEta);
        gt->SetBranchAddress("GEPBasicClusters_phi", &gepBasicClustersPhi);

        gt->SetBranchAddress("GEPCellsTower_et",  &gepCellsTowersEt);
        gt->SetBranchAddress("GEPCellsTower_eta", &gepCellsTowersEta);
        gt->SetBranchAddress("GEPCellsTower_phi", &gepCellsTowersPhi);

        gt->SetBranchAddress("GEPBasicTopoTower_et",  &gepBasicTopoTowersEt);
        gt->SetBranchAddress("GEPBasicTopoTower_eta", &gepBasicTopoTowersEta);
        gt->SetBranchAddress("GEPBasicTopoTower_phi", &gepBasicTopoTowersPhi); 

        gt->SetBranchAddress("ConeGEPCellsTowerJets_pt", &gepConeCellsTowersJetsPt);
        gt->SetBranchAddress("ConeGEPCellsTowerJets_eta", &gepConeCellsTowersJetsEta);
        gt->SetBranchAddress("ConeGEPCellsTowerJets_phi", &gepConeCellsTowersJetsPhi); 
        gt->SetBranchAddress("ConeGEPCellsTowerJets_m", &gepConeCellsTowersJetsMass); 
        gt->SetBranchAddress("ConeGEPCellsTowerJets_nConstituents", &gepConeCellsTowersJetsNConstituents); 

        gt->SetBranchAddress("WTAConeGEPCellsTowerJets_pt",  &gepWTAConeCellsTowersJetsPt);
        gt->SetBranchAddress("WTAConeGEPCellsTowerJets_eta", &gepWTAConeCellsTowersJetsEta);
        gt->SetBranchAddress("WTAConeGEPCellsTowerJets_phi", &gepWTAConeCellsTowersJetsPhi); 
        gt->SetBranchAddress("WTAConeGEPCellsTowerJets_m", &gepWTAConeCellsTowersJetsMass); 
        gt->SetBranchAddress("WTAConeGEPCellsTowerJets_nConstituents", &gepWTAConeCellsTowersJetsNConstituents); 

        gt->SetBranchAddress("ConeGEPBasicClustersJets_pt",  &gepConeBasicClustersJetsPt);
        gt->SetBranchAddress("ConeGEPBasicClustersJets_eta", &gepConeBasicClustersJetsEta);
        gt->SetBranchAddress("ConeGEPBasicClustersJets_phi", &gepConeBasicClustersJetsPhi); 
        gt->SetBranchAddress("ConeGEPBasicClustersJets_m", &gepConeBasicClustersJetsMass); 
        gt->SetBranchAddress("ConeGEPBasicClustersJets_nConstituents", &gepConeBasicClustersJetsNConstituents); 

        gt->SetBranchAddress("WTAConeGEPBasicClustersJets_pt",  &gepWTAConeBasicClustersJetsPt);
        gt->SetBranchAddress("WTAConeGEPBasicClustersJets_eta", &gepWTAConeBasicClustersJetsEta);
        gt->SetBranchAddress("WTAConeGEPBasicClustersJets_phi", &gepWTAConeBasicClustersJetsPhi); 
        gt->SetBranchAddress("WTAConeGEPBasicClustersJets_m", &gepWTAConeBasicClustersJetsMass); 
        gt->SetBranchAddress("WTAConeGEPBasicClustersJets_nConstituents", &gepWTAConeBasicClustersJetsNConstituents); 

        std::cout << "  Number of events: " << event.getEntries() << endl;

        unsigned int passedEventsCounter = 0;
        unsigned int skippedEventsEmptyTruth = 0;
        unsigned int skippedEventsHSTP = 0;
        for (Long64_t iEvt = 0; iEvt < event.getEntries(); ++iEvt) { // NOTE assume that # of events for GEP and DAOD files is the same, else will get a seg fault.
            //std::cout << "iEvt: " << iEvt << "\n";
            // Retrieve truth and in time pileup jets first to apply hard-scatter-softer-than-PU (HSTP) filter (described here: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/JetEtMissMCSamples#Dijet_normalization_procedure_HS)
            // Also require that truth jet collection is not empty
            event.getEntry(iEvt);               // DAOD event iEvt
            gt->GetEntry(iEvt);                 // GEP event iEvt

            const xAOD::JetContainer* AntiKt4TruthDressedWZJets = nullptr;
            if (!event.retrieve(AntiKt4TruthDressedWZJets, "AntiKt4TruthDressedWZJets").isSuccess()) {
                std::cout << "Failed to retrieve reco Antik4 Truth Dressed WZ jets" << endl;
                skippedEventsEmptyTruth++;
                continue;
            }

            if ( AntiKt4TruthDressedWZJets->size() == 0 ){
                skippedEventsEmptyTruth++;
                continue;
            } 

            

            const xAOD::JetContainer* InTimeAntiKt4TruthJets = nullptr;
            if (!event.retrieve(InTimeAntiKt4TruthJets, "InTimeAntiKt4TruthJets").isSuccess()) {
                std::cout << "Failed to retrieve Truth jets" << endl;
                continue;
            }

            // Filter out events where hard scatter is softer than PU
            if(AntiKt4TruthDressedWZJets->at(0)->pt() <= InTimeAntiKt4TruthJets->at(0)->pt()){
                skippedEventsHSTP++;
                continue; 
            } 

            passedEventsCounter++;
            
            

            if ((iEvt % 100) == 0) std::cout << "iEvt: " << iEvt << "\n";
            

            // -- retrieve collections from DAOD ---
            const xAOD::EventInfo_v1* EventInfo = nullptr;
            if (!event.retrieve(EventInfo, "EventInfo").isSuccess()) {
                cerr << "Cannot access EventInfo" << endl;
                continue;
            }
            
            const xAOD::TruthParticleContainer* TruthBosonsWithDecayParticles = nullptr;
            if (!event.retrieve(TruthBosonsWithDecayParticles, "TruthBosonsWithDecayParticles").isSuccess()) {
                cerr << "Cannot access TruthBosonsWithDecayParticles" << endl;
                continue;
            }

            const xAOD::JetContainer* HLT_AntiKt4EMTopoJets_subjesIS = nullptr;
            if (!event.retrieve(HLT_AntiKt4EMTopoJets_subjesIS, "HLT_AntiKt4EMTopoJets_subjesIS").isSuccess()) {
                cerr << "Failed to retrieve HLT jets" << endl;
                continue;
            }

            const xAOD::gFexJetRoIContainer* L1_gFexSRJetRoI = nullptr;
            if (!event.retrieve(L1_gFexSRJetRoI, "L1_gFexSRJetRoI").isSuccess()) {
                std::cerr << "Failed to retrieve gFex SR jets" << std::endl;
                continue;
            }

            const xAOD::gFexJetRoIContainer* L1_gFexLRJetRoI = nullptr;
            if (!event.retrieve(L1_gFexLRJetRoI, "L1_gFexLRJetRoI").isSuccess()) {
                cerr << "Failed to retrieve gFex LR jets" << endl;
                continue;
            }

            const DataVector<xAOD::jFexSRJetRoI_v1>* L1_jFexSRJetRoI = nullptr;
            if (!event.retrieve(L1_jFexSRJetRoI, "L1_jFexSRJetRoI").isSuccess()) {
                std::cerr << "Failed to retrieve jFex SR jets" << std::endl;
                continue;
            }

            const DataVector<xAOD::jFexLRJetRoI_v1>* L1_jFexLRJetRoI = nullptr;
            if (!event.retrieve(L1_jFexLRJetRoI, "L1_jFexLRJetRoI").isSuccess()) {
                std::cerr << "Failed to retrieve jFex SR jets" << std::endl;
                continue;
            }

            /*
            const xAOD::gFexJetRoIContainer* L1_gFexRhoRoI = nullptr;
            if (!event.retrieve(L1_gFexRhoRoI, "L1_gFexRhoRoI").isSuccess()) {
                std::cerr << "Failed to retrieve gFex energy density" << std::endl;
                continue;
            }*/

            const xAOD::JetContainer* AntiKt10UFOCSSKJets = nullptr;
            if (!event.retrieve(AntiKt10UFOCSSKJets, "AntiKt10UFOCSSKJets").isSuccess()) {
                cerr << "Failed to retrieve reco Antik10 UFOCSSK jets" << endl;
                continue;
            }

            


            // Retrieve the CaloTopoClusters422 container
            const DataVector<xAOD::CaloCluster_v1>* CaloTopoClusters422 = nullptr;
            if (!event.retrieve(CaloTopoClusters422, "CaloTopoClusters422").isSuccess()) {
                std::cerr << "Failed to retrieve CaloTopoClusters422" << std::endl;
                continue;
            }

            // Retrieve the CaloCalAllTopoTowers container
            const DataVector<xAOD::CaloCluster_v1>* CaloCalAllTopoTowers = nullptr;
            if (!event.retrieve(CaloCalAllTopoTowers, "CaloCalAllTopoTowers").isSuccess()) {
                std::cerr << "Failed to retrieve CaloCalAllTopoTowers" << std::endl;
                continue;
            }


            // FIXME add all this, fill trees and clear vectors:
            hltAntiKt4SRJEtaValues.clear();
            hltAntiKt4SRJEtValues.clear();
            hltAntiKt4SRJPhiValues.clear();
            hltAntiKt4SRJEtIndexValues.clear();
            hltAntiKt4SRJLeadingEtValues.clear();
            hltAntiKt4SRJLeadingEtaValues.clear();
            hltAntiKt4SRJLeadingPhiValues.clear();
            hltAntiKt4SRJSubleadingEtValues.clear();
            hltAntiKt4SRJSubleadingEtaValues.clear();
            hltAntiKt4SRJSubleadingPhiValues.clear();
            inTimeAntiKt4TruthSRJEtValues.clear();
            inTimeAntiKt4TruthSRJEtIndexValues.clear();
            inTimeAntiKt4TruthSRJEtaValues.clear();
            inTimeAntiKt4TruthSRJPhiValues.clear();
            inTimeAntiKt4TruthSRJLeadingEtValues.clear();
            inTimeAntiKt4TruthSRJLeadingEtaValues.clear();
            inTimeAntiKt4TruthSRJLeadingPhiValues.clear();
            inTimeAntiKt4TruthSRJSubleadingEtValues.clear();
            inTimeAntiKt4TruthSRJSubleadingEtaValues.clear();
            inTimeAntiKt4TruthSRJSubleadingPhiValues.clear();
            recoAntiKt10LRJEtIndexValues.clear();
            recoAntiKt10LRJEtValues.clear();
            recoAntiKt10LRJEtaValues.clear();
            recoAntiKt10LRJPhiValues.clear();
            recoAntiKt10LRJMassValues.clear();
            recoAntiKt10LRJLeadingEtValues.clear();
            recoAntiKt10LRJLeadingEtaValues.clear();
            recoAntiKt10LRJLeadingPhiValues.clear();
            recoAntiKt10LRJLeadingMassValues.clear();
            recoAntiKt10LRJSubleadingEtValues.clear();
            recoAntiKt10LRJSubleadingEtaValues.clear();
            recoAntiKt10LRJSubleadingPhiValues.clear();
            recoAntiKt10LRJSubleadingMassValues.clear();
            truthAntiKt4WZSRJEtIndexValues.clear();
            truthAntiKt4WZSRJEtValues.clear();
            truthAntiKt4WZSRJEtaValues.clear();
            truthAntiKt4WZSRJPhiValues.clear();
            truthAntiKt4WZSRJMassValues.clear();
            truthAntiKt4WZSRJLeadingEtValues.clear();
            truthAntiKt4WZSRJLeadingEtaValues.clear();
            truthAntiKt4WZSRJLeadingPhiValues.clear();
            truthAntiKt4WZSRJLeadingMassValues.clear();
            truthAntiKt4WZSRJSubleadingEtValues.clear();
            truthAntiKt4WZSRJSubleadingEtaValues.clear();
            truthAntiKt4WZSRJSubleadingPhiValues.clear();
            truthAntiKt4WZSRJSubleadingMassValues.clear();
            
            gFexLRJEtValues.clear();
            gFexLRJEtaValues.clear();
            gFexLRJPhiValues.clear();
            gFexLRJEtIndexValues.clear();
            gFexLRJLeadingEtValues.clear();
            gFexLRJLeadingEtaValues.clear();
            gFexLRJLeadingPhiValues.clear();
            gFexLRJSubleadingEtValues.clear();
            gFexLRJSubleadingEtaValues.clear();
            gFexLRJSubleadingPhiValues.clear();
            gFexSRJEtValues.clear();
            gFexSRJEtaValues.clear();
            gFexSRJPhiValues.clear();
            gFexSRJEtIndexValues.clear();
            gFexSRJLeadingEtValues.clear();
            gFexSRJLeadingEtaValues.clear();
            gFexSRJLeadingPhiValues.clear();
            gFexSRJSubleadingEtValues.clear();
            gFexSRJSubleadingEtaValues.clear();
            gFexSRJSubleadingPhiValues.clear();
            jFexSRJEtValues.clear();
            jFexSRJEtaValues.clear();
            jFexSRJPhiValues.clear();
            jFexSRJEtIndexValues.clear();
            jFexSRJLeadingEtValues.clear();
            jFexSRJLeadingEtaValues.clear();
            jFexSRJLeadingPhiValues.clear();
            jFexSRJSubleadingEtValues.clear();
            jFexSRJSubleadingEtaValues.clear();
            jFexSRJSubleadingPhiValues.clear();
            jFexLRJEtValues.clear();
            jFexLRJEtaValues.clear();
            jFexLRJPhiValues.clear();
            jFexLRJEtIndexValues.clear();
            jFexLRJLeadingEtValues.clear();
            jFexLRJLeadingEtaValues.clear();
            jFexLRJLeadingPhiValues.clear();
            jFexLRJSubleadingEtValues.clear();
            jFexLRJSubleadingEtaValues.clear();
            jFexLRJSubleadingPhiValues.clear();
            topo422EtValues.clear();
            topo422EtaValues.clear();
            topo422PhiValues.clear();
            caloTopoTowerEtValues.clear();
            caloTopoTowerEtaValues.clear();
            caloTopoTowerPhiValues.clear();
            gepBasicClustersEtValues.clear();
            gepBasicClustersEtaValues.clear();
            gepBasicClustersPhiValues.clear();
            gepCellsTowersEtValues.clear();
            gepCellsTowersEtaValues.clear();
            gepCellsTowersPhiValues.clear();
            gepBasicTopoTowersEtValues.clear();
            gepBasicTopoTowersEtaValues.clear();
            gepBasicTopoTowersPhiValues.clear();
            gepConeCellsTowersJetspTValues.clear();
            gepConeCellsTowersJetsEtaValues.clear();
            gepConeCellsTowersJetsPhiValues.clear();
            gepConeCellsTowersJetsMassValues.clear();
            gepConeCellsTowersJetsNConstituentsValues.clear();
            gepWTAConeCellsTowersJetspTValues.clear();
            gepWTAConeCellsTowersJetsEtaValues.clear();
            gepWTAConeCellsTowersJetsPhiValues.clear();
            gepWTAConeCellsTowersJetsMassValues.clear();
            gepWTAConeCellsTowersJetsNConstituentsValues.clear();
            gepConeGEPBasicClustersJetspTValues.clear();
            gepConeGEPBasicClustersJetsEtaValues.clear();
            gepConeGEPBasicClustersJetsPhiValues.clear();
            gepConeGEPBasicClustersJetsMassValues.clear();
            gepConeGEPBasicClustersJetsNConstituentsValues.clear();
            gepWTAConeGEPBasicClustersJetspTValues.clear();
            gepWTAConeGEPBasicClustersJetsEtaValues.clear();
            gepWTAConeGEPBasicClustersJetsPhiValues.clear();
            gepWTAConeGEPBasicClustersJetsMassValues.clear();
            gepWTAConeGEPBasicClustersJetsNConstituentsValues.clear();
            gepLeadingConeCellsTowersJetspTValues.clear();
            gepLeadingConeCellsTowersJetsEtaValues.clear();
            gepLeadingConeCellsTowersJetsPhiValues.clear();
            gepLeadingConeCellsTowersJetsMassValues.clear();
            gepLeadingConeCellsTowersJetsNConstituentsValues.clear();
            gepLeadingWTAConeCellsTowersJetspTValues.clear();
            gepLeadingWTAConeCellsTowersJetsEtaValues.clear();
            gepLeadingWTAConeCellsTowersJetsPhiValues.clear();
            gepLeadingWTAConeCellsTowersJetsMassValues.clear();
            gepLeadingWTAConeCellsTowersJetsNConstituentsValues.clear();
            gepLeadingConeGEPBasicClustersJetspTValues.clear();
            gepLeadingConeGEPBasicClustersJetsEtaValues.clear();
            gepLeadingConeGEPBasicClustersJetsPhiValues.clear();
            gepLeadingConeGEPBasicClustersJetsMassValues.clear();
            gepLeadingConeGEPBasicClustersJetsNConstituentsValues.clear();
            gepLeadingWTAConeGEPBasicClustersJetspTValues.clear();
            gepLeadingWTAConeGEPBasicClustersJetsEtaValues.clear();
            gepLeadingWTAConeGEPBasicClustersJetsPhiValues.clear();
            gepLeadingWTAConeGEPBasicClustersJetsMassValues.clear();
            gepLeadingWTAConeGEPBasicClustersJetsNConstituentsValues.clear();
            gepSubleadingConeCellsTowersJetspTValues.clear();
            gepSubleadingConeCellsTowersJetsEtaValues.clear();
            gepSubleadingConeCellsTowersJetsPhiValues.clear();
            gepSubleadingConeCellsTowersJetsMassValues.clear();
            gepSubleadingConeCellsTowersJetsNConstituentsValues.clear();
            gepSubleadingWTAConeCellsTowersJetspTValues.clear();
            gepSubleadingWTAConeCellsTowersJetsEtaValues.clear();
            gepSubleadingWTAConeCellsTowersJetsPhiValues.clear();
            gepSubleadingWTAConeCellsTowersJetsMassValues.clear();
            gepSubleadingWTAConeCellsTowersJetsNConstituentsValues.clear();
            gepSubleadingConeGEPBasicClustersJetspTValues.clear();
            gepSubleadingConeGEPBasicClustersJetsEtaValues.clear();
            gepSubleadingConeGEPBasicClustersJetsPhiValues.clear();
            gepSubleadingConeGEPBasicClustersJetsMassValues.clear();
            gepSubleadingConeGEPBasicClustersJetsNConstituentsValues.clear();
            gepSubleadingWTAConeGEPBasicClustersJetspTValues.clear();
            gepSubleadingWTAConeGEPBasicClustersJetsEtaValues.clear();
            gepSubleadingWTAConeGEPBasicClustersJetsPhiValues.clear();
            gepSubleadingWTAConeGEPBasicClustersJetsMassValues.clear();
            gepSubleadingWTAConeGEPBasicClustersJetsNConstituentsValues.clear();
            truthHiggsInvMassValues.clear();
            truthHiggsEtValues.clear();
            truthHiggsEtaValues.clear();
            truthHiggsPhiValues.clear();
            truthHiggspTValues.clear();
            truthHiggspxValues.clear();
            truthHiggspyValues.clear();
            truthHiggspzValues.clear();
            truthHiggsEnergyValues.clear();
            indexOfHiggsValues.clear();
            higgsIndexValues.clear();
            truthbquarksEtValues.clear();
            truthbquarksEtaValues.clear();
            truthbquarksPhiValues.clear();
            truthbquarkspTValues.clear();
            truthbquarkspxValues.clear();
            truthbquarkspyValues.clear();
            truthbquarkspzValues.clear();
            truthbquarksEnergyValues.clear();
            mcEventWeights.clear();
            eventWeights.clear();
            sumOfWeightsForSample = 0;
            sampleJZSlice = 0;
            
            // Get sum of weights for sample, individual Monte Carlo event weight, and compute event weight used later for reweighting histograms
            sumOfWeightsForSample = sumOfEventWeightsByJZSlice[jzSlice];
            
            if (signalBool) sampleJZSlice = -1; 
            float mcEventWeight = EventInfo->mcEventWeight();
            //std::cout << "iEvt: " << iEvt << " and event weight: " << eventWeight << "\n";
            mcEventWeights.push_back(mcEventWeight);

            // Compute weight for histograms 
            double eventWeight = mcEventWeight * crossSectionsByJZSlice[jzSlice] * filterEffienciesByJZSlice[jzSlice] * reweightLuminosity / (sumOfEventWeightsByJZSlice[jzSlice]);
            eventWeights.push_back(eventWeight);



            // Initialize per-event counters
            unsigned int higgs_counter = -1;
            bool higgsPtCutPassed[2] = {false, false}; 
            // --- Loop over TruthParticles (for Higgs and B's) ---
            std::vector<std::vector<float > > allb_list;
            for (const auto& el : *TruthBosonsWithDecayParticles) {
                if (el->pdgId() == 25 && el->status() == 22) {
                    higgs_counter++;

                    float pt = el->pt() / 1000.0;
                    float eta = el->eta();
                    float phi = el->phi();
                    float px = el->px() / 1000.0;
                    float py = el->py() / 1000.0;
                    float pz = el->pz() / 1000.0;
                    float energy = el->e() / 1000.0;
                    float et = energy / cosh(eta);

                    // Fill truthHiggsTree variables
                    truthHiggsEtValues.push_back(et);
                    truthHiggsEtaValues.push_back(eta);
                    truthHiggsPhiValues.push_back(phi);
                    truthHiggspTValues.push_back(pt);
                    truthHiggspxValues.push_back(px);
                    truthHiggspyValues.push_back(py);
                    truthHiggspzValues.push_back(pz);
                    truthHiggsEnergyValues.push_back(energy);
                    indexOfHiggsValues.push_back(higgs_counter);

                    if (higgs_counter == 0) { // fill b's for both higgs, and store index of which higgs they correspond to.  
                        if (pt > 0) higgsPtCutPassed[0] = true;
                        std::vector<const xAOD::TruthParticle*> b1_list;
                        find_non_higgs_daughters(el, b1_list);

                        if (b1_list.size() == 2) {
                            TLorentzVector b1, b2;
                            b1.SetPxPyPzE(b1_list[0]->px() / 1000.0, b1_list[0]->py() / 1000.0,
                                        b1_list[0]->pz() / 1000.0, b1_list[0]->e() / 1000.0);
                            b2.SetPxPyPzE(b1_list[1]->px() / 1000.0, b1_list[1]->py() / 1000.0,
                                        b1_list[1]->pz() / 1000.0, b1_list[1]->e() / 1000.0);
                            double mH = (b1 + b2).M();  // invariant mass in GeV
                            truthHiggsInvMassValues.push_back(mH);
                        } // compute invariant mass

                        for (const auto* b : b1_list) {
                            if (!b) continue;

                            float pt   = b->pt() / 1000.0;
                            float eta  = b->eta();
                            float phi  = b->phi();
                            float px   = b->px() / 1000.0;
                            float py   = b->py() / 1000.0;
                            float pz   = b->pz() / 1000.0;
                            float E    = b->e()  / 1000.0;
                            float Et   = E / std::cosh(eta);

                            higgsIndexValues.push_back(0);  // from 1st Higgs
                            truthbquarksEtValues.push_back(Et);
                            truthbquarksEtaValues.push_back(eta);
                            truthbquarksPhiValues.push_back(phi);
                            truthbquarkspTValues.push_back(pt);
                            truthbquarkspxValues.push_back(px);
                            truthbquarkspyValues.push_back(py);
                            truthbquarkspzValues.push_back(pz);
                            truthbquarksEnergyValues.push_back(E);
                        }
                    }

                    if (higgs_counter == 1) {
                        if (pt > 0) higgsPtCutPassed[1] = true;
                        std::vector<const xAOD::TruthParticle*> b2_list;
                        find_non_higgs_daughters(el, b2_list);

                        if (b2_list.size() == 2) {
                            TLorentzVector b1, b2;
                            b1.SetPxPyPzE(b2_list[0]->px() / 1000.0, b2_list[0]->py() / 1000.0,
                                        b2_list[0]->pz() / 1000.0, b2_list[0]->e() / 1000.0);
                            b2.SetPxPyPzE(b2_list[1]->px() / 1000.0, b2_list[1]->py() / 1000.0,
                                        b2_list[1]->pz() / 1000.0, b2_list[1]->e() / 1000.0);
                            double mH = (b1 + b2).M();  // invariant mass in GeV
                            truthHiggsInvMassValues.push_back(mH);
                        }

                        for (const auto* b : b2_list) {
                            if (!b) continue;

                            float pt   = b->pt() / 1000.0;
                            float eta  = b->eta();
                            float phi  = b->phi();
                            float px   = b->px() / 1000.0;
                            float py   = b->py() / 1000.0;
                            float pz   = b->pz() / 1000.0;
                            float E    = b->e()  / 1000.0;
                            float Et   = E / std::cosh(eta);

                            higgsIndexValues.push_back(1);  // from 2nd Higgs
                            truthbquarksEtValues.push_back(Et);
                            truthbquarksEtaValues.push_back(eta);
                            truthbquarksPhiValues.push_back(phi);
                            truthbquarkspTValues.push_back(pt);
                            truthbquarkspxValues.push_back(px);
                            truthbquarkspyValues.push_back(py);
                            truthbquarkspzValues.push_back(pz);
                            truthbquarksEnergyValues.push_back(E);
                        } // loop through b2 list
                    } // if 2nd higgs in event
                } // if higgs truth particle of interest
            } // loop through truth bosons with decay particles
            bool higgsPtCutsPassed = higgsPtCutPassed[0] || higgsPtCutPassed[1];
            if (higgsPtCutsPassed) higgsPassEventCounter++;

            unsigned int gepbasicclusters_it = 0;
            for (unsigned int i = 0; i < gepBasicClustersEt->size(); ++i) {
                float gepBasicClusterEt  = (*gepBasicClustersEt)[i] / 1000.0;
                float gepBasicClusterEta = (*gepBasicClustersEta)[i];
                float gepBasicClusterPhi = (*gepBasicClustersPhi)[i];

                gepBasicClustersEtValues.push_back(gepBasicClusterEt);
                gepBasicClustersEtaValues.push_back(gepBasicClusterEta);
                gepBasicClustersPhiValues.push_back(gepBasicClusterPhi);


                if (gepBasicClusterEt < 0) continue; // don't store to digitized memories


                // Digitize each variable
                int phi_bin = digitize(gepBasicClusterPhi, phi_bit_length_, phi_min_, phi_max_);
                int eta_bin = digitize(gepBasicClusterEta, eta_bit_length_, eta_min_, eta_max_);
                int et_bin  = digitize(gepBasicClusterEt, et_bit_length_,
                              static_cast<double>(et_min_), static_cast<double>(et_max_));
                                        
                // 2. Build binary string (for debug or text output)
                std::stringstream binary_ss;
                binary_ss << std::bitset<et_bit_length_>(et_bin) << "|"
                        << std::bitset<eta_bit_length_>(eta_bin) << "|"
                        << std::bitset<phi_bit_length_>(phi_bin);
                std::string binary_word = binary_ss.str();

                // 3. Pack into 27-bit word (stored as uint32_t)
                uint32_t packed_word = (et_bin  << (eta_bit_length_ + phi_bit_length_)) |
                                    (eta_bin << phi_bit_length_) |
                                    (phi_bin);
                                    
                
                if (higgsPtCutsPassed || !signalBool){
                    if (gepbasicclusters_it == 0) {
                        f_gepbasicclusters << "Event : " << std::dec << iEvt << "\n";
                    }
                    // 4. Write to output file
                    f_gepbasicclusters << "0x" << std::hex << std::setw(2) << std::setfill('0') << gepbasicclusters_it
                                << " " << binary_word
                                << " 0x" << std::setw(8) << std::setfill('0') << packed_word << "\n";
                    gepbasicclusters_it++;
                }
            }

            // ---------- gepCellsTowers ----------
            {
                // Build index order sorted by Et (descending)
                std::vector<unsigned int> order(gepCellsTowersEt->size());
                std::iota(order.begin(), order.end(), 0u);
                std::sort(order.begin(), order.end(),
                        [&](unsigned int a, unsigned int b){
                            return (*gepCellsTowersEt)[a] > (*gepCellsTowersEt)[b];
                        });

                unsigned int gepcellstowers_it = 0;
                for (unsigned int i : order) {
                    float gepCellsTowersEtValue  = (*gepCellsTowersEt)[i] / 1000.0f;
                    float gepCellsTowersEtaValue = (*gepCellsTowersEta)[i];
                    float gepCellsTowersPhiValue = (*gepCellsTowersPhi)[i];

                    // push in Et-sorted order
                    gepCellsTowersEtValues.push_back(gepCellsTowersEtValue);
                    gepCellsTowersEtaValues.push_back(gepCellsTowersEtaValue);
                    gepCellsTowersPhiValues.push_back(gepCellsTowersPhiValue);

                    if (gepCellsTowersEtValue < 0) continue; // keep your existing rule

                    // Digitize
                    int phi_bin = digitize(gepCellsTowersPhiValue, phi_bit_length_, phi_min_, phi_max_);
                    int eta_bin = digitize(gepCellsTowersEtaValue, eta_bit_length_, eta_min_, eta_max_);
                    int et_bin  = digitize(gepCellsTowersEtValue,  et_bit_length_,
                                        static_cast<double>(et_min_), static_cast<double>(et_max_));

                    // Build binary string
                    std::stringstream binary_ss;
                    binary_ss << std::bitset<et_bit_length_>(et_bin)  << "|"
                            << std::bitset<eta_bit_length_>(eta_bin) << "|"
                            << std::bitset<phi_bit_length_>(phi_bin);
                    std::string binary_word = binary_ss.str();

                    // Pack into 27-bit word
                    uint32_t packed_word = (et_bin  << (eta_bit_length_ + phi_bit_length_)) |
                                        (eta_bin <<  phi_bit_length_) |
                                        (phi_bin);

                    if (higgsPtCutsPassed || !signalBool){
                        if (gepcellstowers_it == 0) {
                            f_gepcellstowers << "Event : " << std::dec << iEvt << "\n";
                        }
                        f_gepcellstowers << "0x" << std::hex << std::setw(2) << std::setfill('0') << gepcellstowers_it
                                        << " "  << binary_word
                                        << " 0x" << std::setw(8) << std::setfill('0') << packed_word << "\n";
                        ++gepcellstowers_it;
                    }
                }
            }

            // ---------- gepBasicTopoTowers ----------
            {
                std::vector<unsigned int> order(gepBasicTopoTowersEt->size());
                std::iota(order.begin(), order.end(), 0u);
                std::sort(order.begin(), order.end(),
                        [&](unsigned int a, unsigned int b){
                            return (*gepBasicTopoTowersEt)[a] > (*gepBasicTopoTowersEt)[b];
                        });

                unsigned int gepbasictopotowers_it = 0;
                for (unsigned int i : order) {
                    float gepBasicTopoTowersEtValue  = (*gepBasicTopoTowersEt)[i] / 1000.0f;
                    float gepBasicTopoTowersEtaValue = (*gepBasicTopoTowersEta)[i];
                    float gepBasicTopoTowersPhiValue = (*gepBasicTopoTowersPhi)[i];

                    // push in Et-sorted order
                    gepBasicTopoTowersEtValues.push_back(gepBasicTopoTowersEtValue);
                    gepBasicTopoTowersEtaValues.push_back(gepBasicTopoTowersEtaValue);
                    gepBasicTopoTowersPhiValues.push_back(gepBasicTopoTowersPhiValue);

                    if (gepBasicTopoTowersEtValue < 0) continue;

                    int phi_bin = digitize(gepBasicTopoTowersPhiValue, phi_bit_length_, phi_min_, phi_max_);
                    int eta_bin = digitize(gepBasicTopoTowersEtaValue, eta_bit_length_, eta_min_, eta_max_);
                    int et_bin  = digitize(gepBasicTopoTowersEtValue,  et_bit_length_,
                                        static_cast<double>(et_min_), static_cast<double>(et_max_));

                    std::stringstream binary_ss;
                    binary_ss << std::bitset<et_bit_length_>(et_bin)  << "|"
                            << std::bitset<eta_bit_length_>(eta_bin) << "|"
                            << std::bitset<phi_bit_length_>(phi_bin);
                    std::string binary_word = binary_ss.str();

                    uint32_t packed_word = (et_bin  << (eta_bit_length_ + phi_bit_length_)) |
                                        (eta_bin <<  phi_bit_length_) |
                                        (phi_bin);

                    if (higgsPtCutsPassed || !signalBool){
                        if (gepbasictopotowers_it == 0) {
                            f_gepbasictopotowers << "Event : " << std::dec << iEvt << "\n";
                        }
                        f_gepbasictopotowers << "0x" << std::hex << std::setw(2) << std::setfill('0') << gepbasictopotowers_it
                                            << " "  << binary_word
                                            << " 0x" << std::setw(8) << std::setfill('0') << packed_word << "\n";
                        ++gepbasictopotowers_it;
                    }
                }
            }

            // Loop over clusters and fill Et, Eta, Phi
            unsigned int topotower_it = 0;
            for (const auto* cluster : *CaloCalAllTopoTowers) {
                if (!cluster) continue;

                double et = cluster->e() / cosh(cluster->eta()) / 1000.0; // Et in GeV
                caloTopoTowerEtValues.push_back(et);
                caloTopoTowerEtaValues.push_back(cluster->eta());
                caloTopoTowerPhiValues.push_back(cluster->phi());

                if (et < 0) continue; // don't store to digitized memories

                // Digitize each variable
                int phi_bin = digitize(cluster->phi(), phi_bit_length_, phi_min_, phi_max_);
                int eta_bin = digitize(cluster->eta(), eta_bit_length_, eta_min_, eta_max_);
                int et_bin  = digitize(et, et_bit_length_,
                              static_cast<double>(et_min_), static_cast<double>(et_max_));
                                        
                // 2. Build binary string (for debug or text output)
                std::stringstream binary_ss;
                binary_ss << std::bitset<et_bit_length_>(et_bin) << "|"
                        << std::bitset<eta_bit_length_>(eta_bin) << "|"
                        << std::bitset<phi_bit_length_>(phi_bin);
                std::string binary_word = binary_ss.str();

                // 3. Pack into 27-bit word (stored as uint32_t)
                uint32_t packed_word = (et_bin  << (eta_bit_length_ + phi_bit_length_)) |
                                    (eta_bin << phi_bit_length_) |
                                    (phi_bin);
                                    
                
                if (higgsPtCutsPassed || !signalBool){
                    if (topotower_it == 0) {
                        f_topotower << "Event : " << std::dec << iEvt << "\n";
                    }
                    // 4. Write to output file
                    f_topotower << "0x" << std::hex << std::setw(2) << std::setfill('0') << topotower_it
                                << " " << binary_word
                                << " 0x" << std::setw(8) << std::setfill('0') << packed_word << "\n";
                    topotower_it++;
                }
            }

            unsigned int topocluster422_it = 0;
            // Loop over the clusters and store Et, Eta, Phi
            for (const auto* cluster : *CaloTopoClusters422) {
                if (!cluster) continue;

                double et = cluster->e() / cosh(cluster->eta()) / 1000.0; // Et in GeV
                topo422EtValues.push_back(et);
                topo422EtaValues.push_back(cluster->eta());
                topo422PhiValues.push_back(cluster->phi());

                

                if (et < 0) continue; // FIXME for now don't store negative Et to digitized memories
                // Digitize each variable
                int phi_bin = digitize(cluster->phi(), phi_bit_length_, phi_min_, phi_max_);
                int eta_bin = digitize(cluster->eta(), eta_bit_length_, eta_min_, eta_max_);
                int et_bin  = digitize(et, et_bit_length_,
                              static_cast<double>(et_min_), static_cast<double>(et_max_));

                //std::cout << "eta: " << cluster->eta() << " and eta_bin : " << eta_bin << "\n";
                                        
                // 2. Build binary string (for debug or text output)
                std::stringstream binary_ss;
                binary_ss << std::bitset<et_bit_length_>(et_bin) << "|"
                        << std::bitset<eta_bit_length_>(eta_bin) << "|"
                        << std::bitset<phi_bit_length_>(phi_bin);
                std::string binary_word = binary_ss.str();

                // 3. Pack into 27-bit word (stored as uint32_t)
                uint32_t packed_word = (et_bin  << (eta_bit_length_ + phi_bit_length_)) |
                                    (eta_bin << phi_bit_length_) |
                                    (phi_bin);

                //std::cout << "binary: " << binary_word << "\n";

                // 4. Write to output file
                
                if(higgsPtCutsPassed || !signalBool){
                    if (topocluster422_it == 0) {
                        f_topo << "Event : " << std::dec << iEvt << "\n";
                    }
                    f_topo << "0x" << std::hex << std::setw(2) << std::setfill('0') << topocluster422_it
                            << " " << binary_word
                            << " 0x" << std::setw(8) << std::setfill('0') << packed_word << "\n";
                    topocluster422_it++; 
                }
            }

            // GEP cone jets from GEPCellsTowers
            {
                std::vector<unsigned int> indices(gepConeCellsTowersJetsPt->size());
                std::iota(indices.begin(), indices.end(), 0);

                std::sort(indices.begin(), indices.end(),
                        [&](unsigned int a, unsigned int b) {
                            return (*gepConeCellsTowersJetsPt)[a] > (*gepConeCellsTowersJetsPt)[b];
                        });

                for (unsigned int i = 0; i < indices.size(); i++) {
                    unsigned int idx = indices[i];
                    float coneCellsTowersJetspT  = (*gepConeCellsTowersJetsPt)[idx] / 1000.0;
                    float coneCellsTowersJetsEta = (*gepConeCellsTowersJetsEta)[idx];
                    float coneCellsTowersJetsPhi = (*gepConeCellsTowersJetsPhi)[idx];
                    float coneCellsTowersJetsMass = (*gepConeCellsTowersJetsMass)[idx] / 1000.0;
                    unsigned int coneCellsTowersJetsNConstituents = (*gepConeCellsTowersJetsNConstituents)[idx];
                    if(i == 0){
                        gepLeadingConeCellsTowersJetspTValues.push_back(coneCellsTowersJetspT);
                        gepLeadingConeCellsTowersJetsEtaValues.push_back(coneCellsTowersJetsEta);
                        gepLeadingConeCellsTowersJetsPhiValues.push_back(coneCellsTowersJetsPhi);
                        gepLeadingConeCellsTowersJetsMassValues.push_back(coneCellsTowersJetsMass);
                        gepLeadingConeCellsTowersJetsNConstituentsValues.push_back(coneCellsTowersJetsNConstituents);
                    }
                    if (i == 1){
                        gepSubleadingConeCellsTowersJetspTValues.push_back(coneCellsTowersJetspT);
                        gepSubleadingConeCellsTowersJetsEtaValues.push_back(coneCellsTowersJetsEta);
                        gepSubleadingConeCellsTowersJetsPhiValues.push_back(coneCellsTowersJetsPhi);
                        gepSubleadingConeCellsTowersJetsMassValues.push_back(coneCellsTowersJetsMass);
                        gepSubleadingConeCellsTowersJetsNConstituentsValues.push_back(coneCellsTowersJetsNConstituents);
                    }
                
                    gepConeCellsTowersJetspTValues.push_back(coneCellsTowersJetspT);
                    gepConeCellsTowersJetsEtaValues.push_back(coneCellsTowersJetsEta);
                    gepConeCellsTowersJetsPhiValues.push_back(coneCellsTowersJetsPhi);
                    gepConeCellsTowersJetsMassValues.push_back(coneCellsTowersJetsMass);
                    gepConeCellsTowersJetsNConstituentsValues.push_back(coneCellsTowersJetsNConstituents);
                }
            }


            {
                std::vector<unsigned int> indices(gepWTAConeCellsTowersJetsPt->size());
                std::iota(indices.begin(), indices.end(), 0);

                std::sort(indices.begin(), indices.end(),
                        [&](unsigned int a, unsigned int b) {
                            return (*gepWTAConeCellsTowersJetsPt)[a] > (*gepWTAConeCellsTowersJetsPt)[b];
                        });

                for (unsigned int i = 0; i < indices.size(); i++) {
                    unsigned int idx = indices[i];
                    float WTAConeCellsTowersJetspT  = (*gepWTAConeCellsTowersJetsPt)[idx] / 1000.0;
                    float WTAConeCellsTowersJetsEta = (*gepWTAConeCellsTowersJetsEta)[idx];
                    float WTAConeCellsTowersJetsPhi = (*gepWTAConeCellsTowersJetsPhi)[idx];
                    float WTAConeCellsTowersJetsMass = (*gepWTAConeCellsTowersJetsMass)[idx] / 1000.0;
                    unsigned int WTAConeCellsTowersJetsNConstituents = (*gepWTAConeCellsTowersJetsNConstituents)[idx];
                    if(i == 0){
                        gepLeadingWTAConeCellsTowersJetspTValues.push_back(WTAConeCellsTowersJetspT);
                        gepLeadingWTAConeCellsTowersJetsEtaValues.push_back(WTAConeCellsTowersJetsEta);
                        gepLeadingWTAConeCellsTowersJetsPhiValues.push_back(WTAConeCellsTowersJetsPhi);
                        gepLeadingWTAConeCellsTowersJetsMassValues.push_back(WTAConeCellsTowersJetsMass);
                        gepLeadingWTAConeCellsTowersJetsNConstituentsValues.push_back(WTAConeCellsTowersJetsNConstituents);
                    }
                    if (i == 1){
                        gepSubleadingWTAConeCellsTowersJetspTValues.push_back(WTAConeCellsTowersJetspT);
                        gepSubleadingWTAConeCellsTowersJetsEtaValues.push_back(WTAConeCellsTowersJetsEta);
                        gepSubleadingWTAConeCellsTowersJetsPhiValues.push_back(WTAConeCellsTowersJetsPhi);
                        gepSubleadingWTAConeCellsTowersJetsMassValues.push_back(WTAConeCellsTowersJetsMass);
                        gepSubleadingWTAConeCellsTowersJetsNConstituentsValues.push_back(WTAConeCellsTowersJetsNConstituents);
                    }

                    gepWTAConeCellsTowersJetspTValues.push_back(WTAConeCellsTowersJetspT);
                    gepWTAConeCellsTowersJetsEtaValues.push_back(WTAConeCellsTowersJetsEta);
                    gepWTAConeCellsTowersJetsPhiValues.push_back(WTAConeCellsTowersJetsPhi);
                    gepWTAConeCellsTowersJetsMassValues.push_back(WTAConeCellsTowersJetsMass);
                    gepWTAConeCellsTowersJetsNConstituentsValues.push_back(WTAConeCellsTowersJetsNConstituents);
                }
            }


            {
                std::vector<unsigned int> indices(gepConeBasicClustersJetsPt->size());
                std::iota(indices.begin(), indices.end(), 0);

                std::sort(indices.begin(), indices.end(),
                        [&](unsigned int a, unsigned int b) {
                            return (*gepConeBasicClustersJetsPt)[a] > (*gepConeBasicClustersJetsPt)[b];
                        });

                for (unsigned int i = 0; i < indices.size(); i++) {
                    unsigned int idx = indices[i];
                    float coneGEPBasicClustersJetspT  = (*gepConeBasicClustersJetsPt)[idx] / 1000.0;
                    float coneGEPBasicClustersJetsEta = (*gepConeBasicClustersJetsEta)[idx];
                    float coneGEPBasicClustersJetsPhi = (*gepConeBasicClustersJetsPhi)[idx];
                    float coneGEPBasicClustersJetsMass = (*gepConeBasicClustersJetsMass)[idx] / 1000.0;
                    unsigned int coneGEPBasicClustersJetsNConstituents = (*gepConeBasicClustersJetsNConstituents)[idx];
                    if(i == 0){
                        gepLeadingConeGEPBasicClustersJetspTValues.push_back(coneGEPBasicClustersJetspT);
                        gepLeadingConeGEPBasicClustersJetsEtaValues.push_back(coneGEPBasicClustersJetsEta);
                        gepLeadingConeGEPBasicClustersJetsPhiValues.push_back(coneGEPBasicClustersJetsPhi);
                        gepLeadingConeGEPBasicClustersJetsMassValues.push_back(coneGEPBasicClustersJetsMass);
                        gepLeadingConeGEPBasicClustersJetsNConstituentsValues.push_back(coneGEPBasicClustersJetsNConstituents);
                    }
                    if (i == 1){
                        gepSubleadingConeGEPBasicClustersJetspTValues.push_back(coneGEPBasicClustersJetspT);
                        gepSubleadingConeGEPBasicClustersJetsEtaValues.push_back(coneGEPBasicClustersJetsEta);
                        gepSubleadingConeGEPBasicClustersJetsPhiValues.push_back(coneGEPBasicClustersJetsPhi);
                        gepSubleadingConeGEPBasicClustersJetsMassValues.push_back(coneGEPBasicClustersJetsMass);
                        gepSubleadingConeGEPBasicClustersJetsNConstituentsValues.push_back(coneGEPBasicClustersJetsNConstituents);
                    }   
                    
                    gepConeGEPBasicClustersJetspTValues.push_back(coneGEPBasicClustersJetspT);
                    gepConeGEPBasicClustersJetsEtaValues.push_back(coneGEPBasicClustersJetsEta);
                    gepConeGEPBasicClustersJetsPhiValues.push_back(coneGEPBasicClustersJetsPhi);
                    gepConeGEPBasicClustersJetsMassValues.push_back(coneGEPBasicClustersJetsMass);
                    gepConeGEPBasicClustersJetsNConstituentsValues.push_back(coneGEPBasicClustersJetsNConstituents);
                }
            }


            {
                std::vector<unsigned int> indices(gepWTAConeBasicClustersJetsPt->size());
                std::iota(indices.begin(), indices.end(), 0);

                std::sort(indices.begin(), indices.end(),
                        [&](unsigned int a, unsigned int b) {
                            return (*gepWTAConeBasicClustersJetsPt)[a] > (*gepWTAConeBasicClustersJetsPt)[b];
                        });

                for (unsigned int i = 0; i < indices.size(); i++) {
                    unsigned int idx = indices[i];
                    float WTAConeGEPBasicClustersJetspT  = (*gepWTAConeBasicClustersJetsPt)[idx] / 1000.0;
                    float WTAConeGEPBasicClustersJetsEta = (*gepWTAConeBasicClustersJetsEta)[idx];
                    float WTAConeGEPBasicClustersJetsPhi = (*gepWTAConeBasicClustersJetsPhi)[idx];
                    float WTAConeGEPBasicClustersJetsMass = (*gepWTAConeBasicClustersJetsMass)[idx] / 1000.0;
                    unsigned int WTAConeGEPBasicClustersJetsNConstituents = (*gepWTAConeBasicClustersJetsNConstituents)[idx];
                    if(i == 0){
                        gepLeadingWTAConeGEPBasicClustersJetspTValues.push_back(WTAConeGEPBasicClustersJetspT);
                        gepLeadingWTAConeGEPBasicClustersJetsEtaValues.push_back(WTAConeGEPBasicClustersJetsEta);
                        gepLeadingWTAConeGEPBasicClustersJetsPhiValues.push_back(WTAConeGEPBasicClustersJetsPhi);
                        gepLeadingWTAConeGEPBasicClustersJetsMassValues.push_back(WTAConeGEPBasicClustersJetsMass);
                        gepLeadingWTAConeGEPBasicClustersJetsNConstituentsValues.push_back(WTAConeGEPBasicClustersJetsNConstituents);
                    }
                    if (i == 1){
                        gepSubleadingWTAConeGEPBasicClustersJetspTValues.push_back(WTAConeGEPBasicClustersJetspT);
                        gepSubleadingWTAConeGEPBasicClustersJetsEtaValues.push_back(WTAConeGEPBasicClustersJetsEta);
                        gepSubleadingWTAConeGEPBasicClustersJetsPhiValues.push_back(WTAConeGEPBasicClustersJetsPhi);
                        gepSubleadingWTAConeGEPBasicClustersJetsMassValues.push_back(WTAConeGEPBasicClustersJetsMass);
                        gepSubleadingWTAConeGEPBasicClustersJetsNConstituentsValues.push_back(WTAConeGEPBasicClustersJetsNConstituents);
                    }
                    
                    gepWTAConeGEPBasicClustersJetspTValues.push_back(WTAConeGEPBasicClustersJetspT);
                    gepWTAConeGEPBasicClustersJetsEtaValues.push_back(WTAConeGEPBasicClustersJetsEta);
                    gepWTAConeGEPBasicClustersJetsPhiValues.push_back(WTAConeGEPBasicClustersJetsPhi);
                    gepWTAConeGEPBasicClustersJetsMassValues.push_back(WTAConeGEPBasicClustersJetsMass);
                    gepWTAConeGEPBasicClustersJetsNConstituentsValues.push_back(WTAConeGEPBasicClustersJetsNConstituents);
                }
            }


            // Temporary vector for sorting by Et
            std::vector<std::pair<size_t, double>> jFexSRJetEtWithIndex;

            for (size_t i = 0; i < L1_jFexSRJetRoI->size(); ++i) {
                const auto& jet = (*L1_jFexSRJetRoI)[i];
                double et = jet->et() / 1000.0; // Already in GeV
                jFexSRJetEtWithIndex.emplace_back(i, et);
                //std::cout << "i : " << i << " and Et : " << et << "\n";
            }

            // Sort descending by Et
            std::sort(jFexSRJetEtWithIndex.begin(), jFexSRJetEtWithIndex.end(),
                    [](const std::pair<size_t, double>& a, const std::pair<size_t, double>& b) {
                        return a.second > b.second;
                    });

            // Fill vectors in sorted order
            unsigned int jfex_it = 0;
            for (const auto& [index, et] : jFexSRJetEtWithIndex) {
                //std::cout << "index: " << index << "\n";
                //std::cout << "et : " << et << "\n";
                const auto& jet = (*L1_jFexSRJetRoI)[index];

                jFexSRJEtIndexValues.push_back(static_cast<unsigned int>(index));
                jFexSRJEtValues.push_back(et);
                jFexSRJEtaValues.push_back(jet->eta());
                jFexSRJPhiValues.push_back(jet->phi());

                if (et < 0) continue; // FIXME

                // Digitize each variable
                int phi_bin = digitize(jet->phi(), phi_bit_length_, phi_min_, phi_max_);
                int eta_bin = digitize(jet->eta(), eta_bit_length_, eta_min_, eta_max_);
                int et_bin  = digitize(et, et_bit_length_,
                              static_cast<double>(et_min_), static_cast<double>(et_max_));
                                        
                // 2. Build binary string (for debug or text output)
                std::stringstream binary_ss;
                binary_ss << std::bitset<et_bit_length_>(et_bin) << "|"
                        << std::bitset<eta_bit_length_>(eta_bin) << "|"
                        << std::bitset<phi_bit_length_>(phi_bin);
                std::string binary_word = binary_ss.str();

                // 3. Pack into 27-bit word (stored as uint32_t)
                uint32_t packed_word = (et_bin  << (eta_bit_length_ + phi_bit_length_)) |
                                    (eta_bin << phi_bit_length_) |
                                    (phi_bin);

                // 4. Write to output file
                
                if(higgsPtCutsPassed || !signalBool){
                    if (jfex_it == 0) {
                        f_jfex << "Event : " << std::dec << iEvt << "\n";
                    }
                    f_jfex << "0x" << std::hex << std::setw(2) << std::setfill('0') << jfex_it
                            << " " << binary_word
                            << " 0x" << std::setw(8) << std::setfill('0') << packed_word << "\n";
                    jfex_it++;
                }
            }

            // Leading jet
            if (!jFexSRJetEtWithIndex.empty()) {
                const auto& leading = (*L1_jFexSRJetRoI)[jFexSRJetEtWithIndex[0].first];
                jFexSRJLeadingEtValues.push_back(jFexSRJEtValues[0]);
                jFexSRJLeadingEtaValues.push_back(leading->eta());
                jFexSRJLeadingPhiValues.push_back(leading->phi());
            }

            // Subleading jet
            if (jFexSRJetEtWithIndex.size() > 1) {
                const auto& subleading = (*L1_jFexSRJetRoI)[jFexSRJetEtWithIndex[1].first];
                jFexSRJSubleadingEtValues.push_back(jFexSRJEtValues[1]);
                jFexSRJSubleadingEtaValues.push_back(subleading->eta());
                jFexSRJSubleadingPhiValues.push_back(subleading->phi());
            }

            // Temporary vector for sorting by Et
            std::vector<std::pair<size_t, double>> jFexLRJetEtWithIndex;

            for (size_t i = 0; i < L1_jFexLRJetRoI->size(); ++i) {
                const auto& jet = (*L1_jFexLRJetRoI)[i];
                double et = jet->et() / 1000.0; // Already in GeV
                jFexLRJetEtWithIndex.emplace_back(i, et);
            }

            // Sort descending by Et
            std::sort(jFexLRJetEtWithIndex.begin(), jFexLRJetEtWithIndex.end(),
                    [](const std::pair<size_t, double>& a, const std::pair<size_t, double>& b) {
                        return a.second > b.second;
                    });

            // Fill vectors in sorted order
            for (const auto& [index, et] : jFexLRJetEtWithIndex) {
                const auto& jet = (*L1_jFexLRJetRoI)[index];

                jFexLRJEtIndexValues.push_back(static_cast<unsigned int>(index));
                jFexLRJEtValues.push_back(et);
                jFexLRJEtaValues.push_back(jet->eta());
                jFexLRJPhiValues.push_back(jet->phi());

            }

            // Leading jet
            if (!jFexLRJetEtWithIndex.empty()) {
                const auto& leading = (*L1_jFexLRJetRoI)[jFexLRJetEtWithIndex[0].first];
                jFexLRJLeadingEtValues.push_back(jFexLRJEtValues[0]);
                jFexLRJLeadingEtaValues.push_back(leading->eta());
                jFexLRJLeadingPhiValues.push_back(leading->phi());
            }

            // Subleading jet
            if (jFexLRJetEtWithIndex.size() > 1) {
                const auto& subleading = (*L1_jFexLRJetRoI)[jFexLRJetEtWithIndex[1].first];
                jFexLRJSubleadingEtValues.push_back(jFexLRJEtValues[1]);
                jFexLRJSubleadingEtaValues.push_back(subleading->eta());
                jFexLRJSubleadingPhiValues.push_back(subleading->phi());
            }

            // Temporary vector for sorting by Et
            std::vector<std::pair<size_t, double>> gFexSRJetEtWithIndex;

            for (size_t i = 0; i < L1_gFexSRJetRoI->size(); ++i) {
                const auto& jet = (*L1_gFexSRJetRoI)[i];
                double et = jet->et() / 1000.0; // Already in GeV
                gFexSRJetEtWithIndex.emplace_back(i, et);
            }

            // Sort descending by Et
            std::sort(gFexSRJetEtWithIndex.begin(), gFexSRJetEtWithIndex.end(),
                    [](const std::pair<size_t, double>& a, const std::pair<size_t, double>& b) {
                        return a.second > b.second;
                    });

            // Fill vectors in sorted order
            unsigned int gfex_it = 0;
            for (const auto& [index, et] : gFexSRJetEtWithIndex) {
                const auto& jet = (*L1_gFexSRJetRoI)[index];

                gFexSRJEtIndexValues.push_back(static_cast<unsigned int>(index));
                gFexSRJEtValues.push_back(et);
                gFexSRJEtaValues.push_back(jet->eta());
                gFexSRJPhiValues.push_back(jet->phi());

                if (et < 0) continue;

                // Digitize each variable
                int phi_bin = digitize(jet->phi(), phi_bit_length_, phi_min_, phi_max_);
                int eta_bin = digitize(jet->eta(), eta_bit_length_, eta_min_, eta_max_);
                int et_bin  = digitize(et, et_bit_length_,
                              static_cast<double>(et_min_), static_cast<double>(et_max_));
                                        
                // 2. Build binary string (for debug or text output)
                std::stringstream binary_ss;
                binary_ss << std::bitset<et_bit_length_>(et_bin) << "|"
                        << std::bitset<eta_bit_length_>(eta_bin) << "|"
                        << std::bitset<phi_bit_length_>(phi_bin);
                std::string binary_word = binary_ss.str();

                // 3. Pack into 27-bit word (stored as uint32_t)
                uint32_t packed_word = (et_bin  << (eta_bit_length_ + phi_bit_length_)) |
                                    (eta_bin << phi_bit_length_) |
                                    (phi_bin);

                // 4. Write to output file
                
                if(higgsPtCutsPassed || !signalBool){
                    if (gfex_it == 0) {
                        f_gfex << "Event : " << std::dec << iEvt << "\n";
                    }
                    f_gfex << "0x" << std::hex << std::setw(2) << std::setfill('0') << gfex_it
                            << " " << binary_word
                            << " 0x" << std::setw(8) << std::setfill('0') << packed_word << "\n";
                    gfex_it++;
                }   
            }

            // Leading jet
            if (!gFexSRJetEtWithIndex.empty()) {
                const auto& leading = (*L1_gFexSRJetRoI)[gFexSRJetEtWithIndex[0].first];
                gFexSRJLeadingEtValues.push_back(gFexSRJEtValues[0]);
                gFexSRJLeadingEtaValues.push_back(leading->eta());
                gFexSRJLeadingPhiValues.push_back(leading->phi());
            }

            // Subleading jet
            if (gFexSRJetEtWithIndex.size() > 1) {
                const auto& subleading = (*L1_gFexSRJetRoI)[gFexSRJetEtWithIndex[1].first];
                gFexSRJSubleadingEtValues.push_back(gFexSRJEtValues[1]);
                gFexSRJSubleadingEtaValues.push_back(subleading->eta());
                gFexSRJSubleadingPhiValues.push_back(subleading->phi());
            }


            // Temporary vector for sorting by Et
            std::vector<std::pair<size_t, double>> gFexLRJetEtWithIndex;

            for (size_t i = 0; i < L1_gFexLRJetRoI->size(); ++i) {
                const auto& jet = (*L1_gFexLRJetRoI)[i];
                double et = jet->et() / 1000.0; // already in GeV
                gFexLRJetEtWithIndex.emplace_back(i, et);
            }

            // Sort descending by Et
            std::sort(gFexLRJetEtWithIndex.begin(), gFexLRJetEtWithIndex.end(),
                    [](const std::pair<size_t, double>& a, const std::pair<size_t, double>& b) {
                        return a.second > b.second;
                    });

            // Fill vectors in sorted order
            for (const auto& [index, et] : gFexLRJetEtWithIndex) {
                const auto& jet = (*L1_gFexLRJetRoI)[index];

                gFexLRJEtIndexValues.push_back(static_cast<unsigned int>(index));
                gFexLRJEtValues.push_back(et);
                gFexLRJEtaValues.push_back(jet->eta());
                gFexLRJPhiValues.push_back(jet->phi());
            }

            // Leading jet
            if (!gFexLRJetEtWithIndex.empty()) {
                const auto& leading = (*L1_gFexLRJetRoI)[gFexLRJetEtWithIndex[0].first];
                gFexLRJLeadingEtValues.push_back(gFexLRJEtValues[0]);
                gFexLRJLeadingEtaValues.push_back(leading->eta());
                gFexLRJLeadingPhiValues.push_back(leading->phi());
            }

            // Subleading jet
            if (gFexLRJetEtWithIndex.size() > 1) {
                const auto& subleading = (*L1_gFexLRJetRoI)[gFexLRJetEtWithIndex[1].first];
                gFexLRJSubleadingEtValues.push_back(gFexLRJEtValues[1]);
                gFexLRJSubleadingEtaValues.push_back(subleading->eta());
                gFexLRJSubleadingPhiValues.push_back(subleading->phi());
            }


            std::vector<std::pair<size_t, double>> hltJetEtWithIndex;
            for (size_t i = 0; i < HLT_AntiKt4EMTopoJets_subjesIS->size(); ++i) {
                const auto& el = (*HLT_AntiKt4EMTopoJets_subjesIS)[i];
                double et = el->e() / (1000.0 * cosh(el->eta()));
                hltJetEtWithIndex.emplace_back(i, et);  // Store index and Et for sorting
            }
            // Sort by descending Et
            std::sort(hltJetEtWithIndex.begin(), hltJetEtWithIndex.end(),
                    [](const std::pair<size_t, double>& a, const std::pair<size_t, double>& b) {
                        return a.second > b.second;
                    });
            // Now push back into vectors in sorted order
            for (const auto& [index, et] : hltJetEtWithIndex) {
                const auto& el = (*HLT_AntiKt4EMTopoJets_subjesIS)[index];
                hltAntiKt4SRJEtIndexValues.push_back(static_cast<unsigned int>(index));
                hltAntiKt4SRJEtValues.push_back(et);
                hltAntiKt4SRJEtaValues.push_back(el->eta());
                hltAntiKt4SRJPhiValues.push_back(el->phi());
            }

            // Store leading and subleading jets if available
            if (!hltJetEtWithIndex.empty()) {
                const auto& leading = (*HLT_AntiKt4EMTopoJets_subjesIS)[hltJetEtWithIndex[0].first];
                hltAntiKt4SRJLeadingEtValues.push_back(hltAntiKt4SRJEtValues[0]);
                hltAntiKt4SRJLeadingEtaValues.push_back(leading->eta());
                hltAntiKt4SRJLeadingPhiValues.push_back(leading->phi());
            }

            if (hltJetEtWithIndex.size() > 1) {
                const auto& subleading = (*HLT_AntiKt4EMTopoJets_subjesIS)[hltJetEtWithIndex[1].first];
                hltAntiKt4SRJSubleadingEtValues.push_back(hltAntiKt4SRJEtValues[1]);
                hltAntiKt4SRJSubleadingEtaValues.push_back(subleading->eta());
                hltAntiKt4SRJSubleadingPhiValues.push_back(subleading->phi());
            }

            // --- Loop over L1_gFexLRJetRoI ---
            //for (const auto& el : *gFexLRJets) {
            //    gfex_larger_jet_pt_values.push_back(el->et() / 1000.0);
            //    float gfex_larger_jet_Et = el->et() / 1000.0;
            //}

            // --- Loop over InTimeAntiKt4TruthJets ---
            // Temporary vector to hold (index, Et) for sorting
            std::vector<std::pair<size_t, double>> truthJetEtWithIndex;

            for (size_t i = 0; i < InTimeAntiKt4TruthJets->size(); ++i) {
                const auto& jet = (*InTimeAntiKt4TruthJets)[i];
                double et = jet->e() / (1000.0 * cosh(jet->eta()));
                truthJetEtWithIndex.emplace_back(i, et);  // Store original index and Et
            }

            // Sort by descending Et
            std::sort(truthJetEtWithIndex.begin(), truthJetEtWithIndex.end(),
                    [](const std::pair<size_t, double>& a, const std::pair<size_t, double>& b) {
                        return a.second > b.second;
                    });

            // Fill vectors in sorted order
            for (const auto& [index, et] : truthJetEtWithIndex) {
                const auto& jet = (*InTimeAntiKt4TruthJets)[index];
                inTimeAntiKt4TruthSRJEtIndexValues.push_back(static_cast<unsigned int>(index));
                inTimeAntiKt4TruthSRJEtValues.push_back(et);
                inTimeAntiKt4TruthSRJEtaValues.push_back(jet->eta());
                inTimeAntiKt4TruthSRJPhiValues.push_back(jet->phi());
            }

            // Store leading jet info if available
            if (!truthJetEtWithIndex.empty()) {
                const auto& leading = (*InTimeAntiKt4TruthJets)[truthJetEtWithIndex[0].first];
                inTimeAntiKt4TruthSRJLeadingEtValues.push_back(inTimeAntiKt4TruthSRJEtValues[0]);
                inTimeAntiKt4TruthSRJLeadingEtaValues.push_back(leading->eta());
                inTimeAntiKt4TruthSRJLeadingPhiValues.push_back(leading->phi());
            }

            // Store subleading jet info if available
            if (truthJetEtWithIndex.size() > 1) {
                const auto& subleading = (*InTimeAntiKt4TruthJets)[truthJetEtWithIndex[1].first];
                inTimeAntiKt4TruthSRJSubleadingEtValues.push_back(inTimeAntiKt4TruthSRJEtValues[1]);
                inTimeAntiKt4TruthSRJSubleadingEtaValues.push_back(subleading->eta());
                inTimeAntiKt4TruthSRJSubleadingPhiValues.push_back(subleading->phi());
            }

            // Temporary vector to hold (index, Et) for sorting
            std::vector<std::pair<size_t, double>> recoJetEtWithIndex;

            for (size_t i = 0; i < AntiKt10UFOCSSKJets->size(); ++i) {
                const auto& jet = (*AntiKt10UFOCSSKJets)[i];
                double et = jet->e() / (1000.0 * cosh(jet->eta()));
                recoJetEtWithIndex.emplace_back(i, et);
            }

            // Sort by descending Et
            std::sort(recoJetEtWithIndex.begin(), recoJetEtWithIndex.end(),
                    [](const std::pair<size_t, double>& a, const std::pair<size_t, double>& b) {
                        return a.second > b.second;
                    });

            // Fill vectors in sorted order
            for (const auto& [index, et] : recoJetEtWithIndex) {
                const auto& jet = (*AntiKt10UFOCSSKJets)[index];
                recoAntiKt10LRJEtIndexValues.push_back(static_cast<unsigned int>(index));
                recoAntiKt10LRJEtValues.push_back(et);
                recoAntiKt10LRJEtaValues.push_back(jet->eta());
                recoAntiKt10LRJPhiValues.push_back(jet->phi());
                recoAntiKt10LRJMassValues.push_back(jet->m() / 1000.0);
            }

            // Leading jet
            if (!recoJetEtWithIndex.empty()) {
                const auto& leading = (*AntiKt10UFOCSSKJets)[recoJetEtWithIndex[0].first];
                recoAntiKt10LRJLeadingEtValues.push_back(recoAntiKt10LRJEtValues[0]);
                recoAntiKt10LRJLeadingEtaValues.push_back(leading->eta());
                recoAntiKt10LRJLeadingPhiValues.push_back(leading->phi());
                recoAntiKt10LRJLeadingMassValues.push_back(leading->m() / 1000.0);
            }

            // Subleading jet
            if (recoJetEtWithIndex.size() > 1) {
                const auto& subleading = (*AntiKt10UFOCSSKJets)[recoJetEtWithIndex[1].first];
                recoAntiKt10LRJSubleadingEtValues.push_back(recoAntiKt10LRJEtValues[1]);
                recoAntiKt10LRJSubleadingEtaValues.push_back(subleading->eta());
                recoAntiKt10LRJSubleadingPhiValues.push_back(subleading->phi());
                recoAntiKt10LRJSubleadingMassValues.push_back(subleading->m() / 1000.0);
            }


            // Temporary vector to hold (index, Et) for sorting
            std::vector<std::pair<size_t, double>> truthWZJetEtWithIndex;

            for (size_t i = 0; i < AntiKt4TruthDressedWZJets->size(); ++i) {
                const auto& jet = (*AntiKt4TruthDressedWZJets)[i];
                double et = jet->e() / (1000.0 * cosh(jet->eta()));
                truthWZJetEtWithIndex.emplace_back(i, et);
            }

            // Sort by descending Et
            std::sort(truthWZJetEtWithIndex.begin(), truthWZJetEtWithIndex.end(),
                    [](const std::pair<size_t, double>& a, const std::pair<size_t, double>& b) {
                        return a.second > b.second;
                    });

            // Fill vectors in sorted order
            for (const auto& [index, et] : truthWZJetEtWithIndex) {
                const auto& jet = (*AntiKt4TruthDressedWZJets)[index];
                truthAntiKt4WZSRJEtIndexValues.push_back(static_cast<unsigned int>(index));
                truthAntiKt4WZSRJEtValues.push_back(et);
                truthAntiKt4WZSRJEtaValues.push_back(jet->eta());
                truthAntiKt4WZSRJPhiValues.push_back(jet->phi());
                truthAntiKt4WZSRJMassValues.push_back(jet->m() / 1000.0);
            }

            // Leading jet
            if (!truthWZJetEtWithIndex.empty()) {
                const auto& leading = (*AntiKt4TruthDressedWZJets)[truthWZJetEtWithIndex[0].first];
                truthAntiKt4WZSRJLeadingEtValues.push_back(truthAntiKt4WZSRJEtValues[0]);
                truthAntiKt4WZSRJLeadingEtaValues.push_back(leading->eta());
                truthAntiKt4WZSRJLeadingPhiValues.push_back(leading->phi());
                truthAntiKt4WZSRJLeadingMassValues.push_back(leading->m() / 1000.0);

            }

            // Subleading jet
            if (truthWZJetEtWithIndex.size() > 1) {
                const auto& subleading = (*AntiKt4TruthDressedWZJets)[truthWZJetEtWithIndex[1].first];
                truthAntiKt4WZSRJSubleadingEtValues.push_back(truthAntiKt4WZSRJEtValues[1]);
                truthAntiKt4WZSRJSubleadingEtaValues.push_back(subleading->eta());
                truthAntiKt4WZSRJSubleadingPhiValues.push_back(subleading->phi());
                truthAntiKt4WZSRJSubleadingMassValues.push_back(subleading->m() / 1000.0);
            }
            if(higgsPtCutsPassed || !signalBool){
                sampleJZSlice = jzSlice;
                eventInfoTree->Fill();
                truthbTree->Fill();
                truthHiggsTree->Fill();
                // truthVBFQuark->Fill();  // commented out as in your declaration
                caloTopoTowerTree->Fill();
                gepBasicClustersTree->Fill();
                gepCellsTowersTree->Fill();
                gepBasicTopoTowersTree->Fill();
                gepConeCellsTowersJetsTree->Fill();
                gepWTAConeCellsTowersJetsTree->Fill();
                gepConeBasicClustersJetsTree->Fill();
                gepWTAConeBasicClustersJetsTree->Fill();
                gepLeadingConeCellsTowersJetsTree->Fill();
                gepLeadingWTAConeCellsTowersJetsTree->Fill();
                gepLeadingConeBasicClustersJetsTree->Fill();
                gepLeadingWTAConeBasicClustersJetsTree->Fill();
                gepSubleadingConeCellsTowersJetsTree->Fill();
                gepSubleadingWTAConeCellsTowersJetsTree->Fill();
                gepSubleadingConeBasicClustersJetsTree->Fill();
                gepSubleadingWTAConeBasicClustersJetsTree->Fill();
                topo422Tree->Fill();
                gFexSRJTree->Fill();
                gFexLeadingSRJTree->Fill();
                gFexSubleadingSRJTree->Fill();
                gFexLRJTree->Fill();
                gFexLeadingLRJTree->Fill();
                gFexSubleadingLRJTree->Fill();
                inTimeAntiKt4TruthJetsTree->Fill();
                leadingInTimeAntiKt4TruthJetsTree->Fill();
                subleadingInTimeAntiKt4TruthJetsTree->Fill();
                jFexSRJTree->Fill();
                jFexLeadingSRJTree->Fill();
                jFexSubleadingSRJTree->Fill();
                jFexLRJTree->Fill();
                jFexLeadingLRJTree->Fill();
                jFexSubleadingLRJTree->Fill();
                hltAntiKt4EMTopoJetsTree->Fill();
                leadingHltAntiKt4EMTopoJetsTree->Fill();
                subleadingHltAntiKt4EMTopoJetsTree->Fill();
                recoAntiKt10UFOCSSKJets->Fill();
                leadingRecoAntiKt10UFOCSSKJets->Fill();
                subleadingRecoAntiKt10UFOCSSKJets->Fill();
                truthAntiKt4TruthDressedWZJets->Fill();
                leadingTruthAntiKt4TruthDressedWZJets->Fill();
                subleadingTruthAntiKt4TruthDressedWZJets->Fill();
            }
        } // loop through events
        std::cout << "for jz: " << jzSlice << " these many events passed: " << passedEventsCounter << " out of: " << event.getEntries() << "\n";
        std::cout << " these many events skipped due to empty truth container: " << skippedEventsEmptyTruth << " these many events skipped due to pt hard < pt pileup: " << skippedEventsHSTP << "\n";
        std::cout << "closing files" << "\n";
        gf->Close();
        f->Close();
    } // loop through filenames
    outputFile->cd();
    std::cout << "writing output file" << "\n";
    eventInfoTree->Write("", TObject::kOverwrite);
    truthbTree->Write("", TObject::kOverwrite);
    truthHiggsTree->Write("", TObject::kOverwrite);
    // truthVBFQuark->Write();  // Optional, if used
    caloTopoTowerTree->Write("", TObject::kOverwrite);
    gepBasicClustersTree->Write("", TObject::kOverwrite);
    gepCellsTowersTree->Write("", TObject::kOverwrite);
    gepBasicTopoTowersTree->Write("", TObject::kOverwrite);
    gepConeCellsTowersJetsTree->Write("", TObject::kOverwrite);
    gepWTAConeCellsTowersJetsTree->Write("", TObject::kOverwrite);
    gepConeBasicClustersJetsTree->Write("", TObject::kOverwrite);
    gepWTAConeBasicClustersJetsTree->Write("", TObject::kOverwrite);
    gepLeadingConeCellsTowersJetsTree->Write("", TObject::kOverwrite);
    gepLeadingWTAConeCellsTowersJetsTree->Write("", TObject::kOverwrite);
    gepLeadingConeBasicClustersJetsTree->Write("", TObject::kOverwrite);
    gepLeadingWTAConeBasicClustersJetsTree->Write("", TObject::kOverwrite);
    gepSubleadingConeCellsTowersJetsTree->Write("", TObject::kOverwrite);
    gepSubleadingWTAConeCellsTowersJetsTree->Write("", TObject::kOverwrite);
    gepSubleadingConeBasicClustersJetsTree->Write("", TObject::kOverwrite);
    gepSubleadingWTAConeBasicClustersJetsTree->Write("", TObject::kOverwrite);
    topo422Tree->Write("", TObject::kOverwrite);
    gFexSRJTree->Write("", TObject::kOverwrite);
    gFexLeadingSRJTree->Write("", TObject::kOverwrite);
    gFexSubleadingSRJTree->Write("", TObject::kOverwrite);
    gFexLRJTree->Write("", TObject::kOverwrite);
    gFexLeadingLRJTree->Write("", TObject::kOverwrite);
    gFexSubleadingLRJTree->Write("", TObject::kOverwrite);
    inTimeAntiKt4TruthJetsTree->Write("", TObject::kOverwrite);
    leadingInTimeAntiKt4TruthJetsTree->Write("", TObject::kOverwrite);
    subleadingInTimeAntiKt4TruthJetsTree->Write("", TObject::kOverwrite);
    jFexSRJTree->Write("", TObject::kOverwrite);
    jFexLeadingSRJTree->Write("", TObject::kOverwrite);
    jFexSubleadingSRJTree->Write("", TObject::kOverwrite);
    jFexLRJTree->Write("", TObject::kOverwrite);
    jFexLeadingLRJTree->Write("", TObject::kOverwrite);
    jFexSubleadingLRJTree->Write("", TObject::kOverwrite);
    hltAntiKt4EMTopoJetsTree->Write("", TObject::kOverwrite);
    leadingHltAntiKt4EMTopoJetsTree->Write("", TObject::kOverwrite);
    subleadingHltAntiKt4EMTopoJetsTree->Write("", TObject::kOverwrite);
    recoAntiKt10UFOCSSKJets->Write("", TObject::kOverwrite);
    leadingRecoAntiKt10UFOCSSKJets->Write("", TObject::kOverwrite);
    subleadingRecoAntiKt10UFOCSSKJets->Write("", TObject::kOverwrite);
    truthAntiKt4TruthDressedWZJets->Write("", TObject::kOverwrite);
    leadingTruthAntiKt4TruthDressedWZJets->Write("", TObject::kOverwrite);
    subleadingTruthAntiKt4TruthDressedWZJets->Write("", TObject::kOverwrite);
    outputFile->Close();
    std::cout << "Processing complete." << endl;
    std::cout << "higgsPassEventCounter: " << higgsPassEventCounter << "\n";
} // ntupler function

// processes all jzSlices + signal
void callNTupler(){
    //gSystem->RedirectOutput("NTupler.log", "w");
    std::vector<unsigned int > jzSlices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    //std::vector<unsigned int > jzSlices = {3};
    std::vector<std::string > jzOutputFilenames;
    for(auto jzSlice : jzSlices){
        nTupler(false, true, jzSlice); // call for each background jzSlice
        TString out = makeOutputFileName(false, false, jzSlice);
        jzOutputFilenames.push_back(out.Data());
    }

    // FIXME allow hadd to work automatically 
    //std::ostringstream cmd;
    //cmd << "hadd -f -k outputRootFiles/background_merged_nojz7or9.root";
    //for (const auto& f : jzOutputFilenames) cmd << " '" << f << "'";
    //int rc = gSystem->Exec(cmd.str().c_str());
    //if (rc != 0) {
    //    Error("callNTupler", "hadd failed with code %d", rc);
    //}

    nTupler(true, true); // call for signal
    for(auto jzOutputFileName : jzOutputFilenames){ // for manual hadd'ing for now
        std::cout << jzOutputFileName << "\n";
    }
    gSystem->Exit(0);
}