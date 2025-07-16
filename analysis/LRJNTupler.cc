// Save as: process_xaod.C
// To execute: root -l -b -q process_xaod.C

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <filesystem> // C++17
#include <algorithm>

#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TH2F.h"
#include "analysisHelperFunctions.h"


// Settings
const bool afBool = true;
const bool higgsPtCut = true;
const bool jFexPlotsBool = false;

const float higgs_min_pt = 100.0; // GeV
const float higgs_min_average_pt = 200.0; // GeV

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
void find_non_higgs_daughters(const xAOD::TruthParticle* particle, std::vector<std::vector<float>>& result) {
    if (!particle) return;

    // Check if the particle is not a Higgs
    if (particle->pdgId() != 25) { // Not Higgs
        float E = particle->e();
        float pt = particle->pt();
        float eta = particle->eta();
        float phi = particle->phi();
        float p = (std::abs(eta) < 100) ? pt * std::cosh(eta) : INFINITY;
        float Et = (p > 0) ? (E * (pt / p)) : 0.0;

        // Push the result as a vector of floats (instead of a tuple)
        result.push_back({Et / 1000.0f, eta, phi}); // Store in GeV
        return;
    }

    // If the particle is a Higgs, recursively find its non-Higgs daughters
    for (unsigned int i = 0; i < particle->nChildren(); ++i) {
        find_non_higgs_daughters(particle->child(i), result);
    }
}

// Main function
void nTupler(bool signalBool, bool daodBool) {
    SetPlotStyle();
    // Setup file paths
    string fileDir;
    if (daodBool){
        fileDir = "/data/larsonma/LargeRadiusJets/datasets/Signal_HHbbbb_DAOD_JETM42/mc21_14TeV.537536.MGPy8EG_hh_bbbb_vbf_novhh_5fs_l1cvv0cv1.deriv.DAOD_JETM42.e8557_s4422_r16130_p6658";
    }
    else{
        if (signalBool) {
            fileDir = afBool ?
                "/data/larsonma/LargeRadiusJets/datasets/Signal_HHbbbb/mc21_14TeV.537540.MGPy8EG_hh_bbbb_vbf_novhh_5fs_l1cvv1cv1.recon.AOD.e8557_s4422_r16130" :
                "/home/larsonma/LargeRadiusJets/datasets/Signal_HHbbbb/mc21_14TeV.537540.MGPy8EG_hh_bbbb_vbf_novhh_5fs_l1cvv1cv1.recon.AOD.e8557_s4422_r16130";
        } else {
            fileDir = afBool ?
                "/data/larsonma/LargeRadiusJets/datasets/Background_jj_JZ3/mc21_14TeV.537540.MGPy8EG_hh_bbbb_vbf_novhh_5fs_l1cvv1cv1.recon.AOD.e8557_s4422_r16130" :
                "/home/larsonma/LargeRadiusJets/datasets/Background_jj_JZ3/mc21_14TeV.537540.MGPy8EG_hh_bbbb_vbf_novhh_5fs_l1cvv1cv1.recon.AOD.e8557_s4422_r16130";
        }
    }


    TString outputFileName; 
    if (signalBool) outputFileName = "outputRootFiles/mc21_14TeV_hh_bbbb_vbf_novhh_NTUPLE.root";
    else outputFileName = "outputRootFiles/mc21_14TeV_jj_JZ3_NTUPLE.root";

    // Create ROOT output file
    TFile* outputFile = new TFile(outputFileName, "RECREATE");

    // Create a TTree
    TTree* caloTopoTowerTree = new TTree("caloTopoTowerTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* truthParticleTree = new TTree("truthParticleTree", "Tree storing event-wise information about truth particles");
    TTree* truthbTree = new TTree("truthbTree", "Tree storing event-wise information about truth particles");
    TTree* truthHiggsTree = new TTree("truthHiggsTree", "Tree storing event-wise information about truth particles");
    TTree* truthVBFQuark = new TTree("truthVBFQuark", "Tree storing event-wise information about truth particles");
    TTree* topo422Tree = new TTree("topo422Tree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gFexSRJTree = new TTree("gFexSRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gFexLeadingSRJTree = new TTree("gFexLeadingSRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gFexSubleadingSRJTree = new TTree("gFexSubleadingSRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gFexLRJTree = new TTree("gFexLRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gFexLeadingLRJTree = new TTree("gFexLeadingLRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* gFexSubleadingLRJTree = new TTree("gFexSubleadingLRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* inTimeAntiKt4TruthJetsTree = new TTree("inTimeAntiKt4TruthJetsTree", "Tree storing event-wise Et, eta phi");
    TTree* jFexSRJTree = new TTree("jFexSRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* jFexLeadingSRJTree = new TTree("jFexLeadingSRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* jFexSubleadingSRJTree = new TTree("jFexLeadingSRJTree", "Tree storing event-wise Et, Eta, Phi");
    TTree* hltAntiKt4EMTopoJetsTree = new TTree("hltAntiKt4EMTopoJetsTree", "Tree storing event-wise Et, Eta, Phi");

    // Variables to store data
    int topoEventNumber, gFexEventNumber, jFexEventNumber;
    std::vector<double> caloTopoEtValues, caloTopoEtaValues, caloTopoPhiValues;
    std::vector<double> topoEtValues, topoEtaValues, topoPhiValues;
    std::vector<double> gFexSRJEtValues, gFexSRJEtaValues, gFexSRJPhiValues;
    std::vector<double> gFexSRJLeadingEtValues, gFexSRJLeadingEtaValues, gFexSRJLeadingPhiValues;
    std::vector<double> gFexSRJSubleadingEtValues, gFexSRJSubleadingEtaValues, gFexSRJSubleadingPhiValues;
    std::vector<double> jFexSRJEtValues, jFexSRJEtaValues, jFexSRJPhiValues;
    std::vector<double> jFexSRJLeadingEtValues, jFexSRJLeadingEtaValues, jFexSRJLeadingPhiValues;
    std::vector<double> jFexSRJSubleadingEtValues, jFexSRJSubleadingEtaValues, jFexSRJSubleadingPhiValues;

    // Create branches
    topo422Tree->Branch("eventNumber", &topoEventNumber, "eventNumber/I");
    topo422Tree->Branch("Et", &topoEtValues);
    topo422Tree->Branch("Eta", &topoEtaValues);
    topo422Tree->Branch("Phi", &topoPhiValues);

    gFexSRJTree->Branch("eventNumber", &gFexEventNumber, "eventNumber/I");
    gFexSRJTree->Branch("Et", &gFexEtValues);
    gFexSRJTree->Branch("Eta", &gFexEtaValues);
    gFexSRJTree->Branch("Phi", &gFexPhiValues);

    jFexSRJTree->Branch("eventNumber", &jFexEventNumber, "eventNumber/I");
    jFexSRJTree->Branch("Et", &jFexEtValues);
    jFexSRJTree->Branch("Eta", &jFexEtaValues);
    jFexSRJTree->Branch("Phi", &jFexPhiValues);
    
    // Collect input file names
    vector<string> fileNames;
    for (const auto& entry : filesystem::directory_iterator(fileDir)) {
        if (entry.path().extension() == ".1") { // Ends with .1
            fileNames.push_back(entry.path().string());
        }
    }

    if (fileNames.empty()) {
        cout << "No ROOT files found in directory." << endl;
        return;
    }

    cout << "Found " << fileNames.size() << " files." << endl;

    // Declare vectors if not already
    /*
    std::vector<float> hlt_jet_pt_values;
    std::vector<float> gfex_larger_jet_pt_values;
    std::vector<float> jet_pt_values;
    std::vector<float> jet_log_pt_values;
    std::vector<float> deltaR_values;
    std::vector<float> higgs_pt_values_for_dr;
    std::vector<float> higgs_pt_values;
    std::vector<float> higgs_pt_values_by_event;
    std::vector<float> higgs_pt_values_after_cut;
    std::vector<float> b_Et_values;
    std::vector<float> b_Et_values_after_higgs_cut;

    std::vector<float> highestEt_b_closest_gfex_deltar_list;
    std::vector<float> highestEt_b_closest_truthjet_deltar_list;
    std::vector<float> second_highestEt_b_closest_gfex_deltar_list;
    std::vector<float> second_highestEt_b_closest_truthjet_deltar_list;
    std::vector<float> third_highestEt_b_closest_gfex_deltar_list;
    std::vector<float> third_highestEt_b_closest_truthjet_deltar_list;
    std::vector<float> fourth_highestEt_b_closest_gfex_deltar_list;
    std::vector<float> fourth_highestEt_b_closest_truthjet_deltar_list;

    std::vector<float> b_gfex_deltar_list;

    std::vector<float> b11_list, b21_list, b12_list, b22_list;
    */

    // Main processing loop
    int fileIt = 0;
    for (const auto& fileName : fileNames) {
        fileIt++; 
        if (fileIt > 9) break; 
        cout << "Processing file: " << fileName << endl;

        TFile* f = TFile::Open(fileName.c_str());
        if (!f || f->IsZombie()) {
            cerr << "Could not open " << fileName << endl;
            continue;
        }

        xAOD::TEvent event(xAOD::TEvent::kClassAccess);
        if (!event.readFrom(f).isSuccess()) {
            cerr << "Cannot read xAOD from file." << endl;
            continue;
        }

        cout << "  Number of events: " << event.getEntries() << endl;

        

        for (Long64_t entry = 0; entry < event.getEntries(); ++entry) {
            event.getEntry(entry);

            // -- ADD collections you want here ---
            const xAOD::TruthParticleContainer* truthParticles = nullptr;
            if (!event.retrieve(truthParticles, "TruthParticles").isSuccess()) {
                cerr << "Cannot access TruthParticles" << endl;
                continue;
            }

            const xAOD::JetContainer* HLTJets = nullptr;
            if (!event.retrieve(HLTJets, "HLT_AntiKt4EMTopoJets_subjesIS").isSuccess()) {
                cerr << "Failed to retrieve HLT jets" << endl;
                continue;
            }

            const xAOD::JetContainer* RecoJets = nullptr;
            if (!event.retrieve(RecoJets, "AntiKt10UFOCSSKJets").isSuccess()) {
                cerr << "Failed to retrieve HLT jets" << endl;
                continue;
            }


            const xAOD::gFexJetRoIContainer* gFexSRJets = nullptr;
            if (!event.retrieve(gFexSRJets, "L1_gFexSRJetRoI").isSuccess()) {
                std::cerr << "Failed to retrieve gFex SR jets" << std::endl;
                continue;
            }

            const xAOD::gFexJetRoIContainer* gFexLRJets = nullptr;
            if (!event.retrieve(gFexLRJets, "L1_gFexLRJetRoI").isSuccess()) {
                cerr << "Failed to retrieve gFex LR jets" << endl;
                continue;
            }

            const xAOD::JetContainer* TruthJets = nullptr;
            if (!event.retrieve(TruthJets, "InTimeAntiKt4TruthJets").isSuccess()) {
                cerr << "Failed to retrieve Truth jets" << endl;
                continue;
            }

            // Initialize per-event counters
            int higgs_counter = 0;
            bool higgs_passes_cut = false;

            float higgs_1_pt = 0, higgs_2_pt = 0;
            float higgs_1_eta = 0, higgs_1_phi = 0;
            float higgs_2_eta = 0, higgs_2_phi = 0;

            // --- Loop over HLT_AntiKt4EMTopoJets_subjesIS ---
            for (const auto& el : *HLTJets) {
                hlt_jet_pt_values.push_back(el->pt() / 1000.0);
                float hlt_jet_total_p = el->pt() * cosh(el->eta());
                float hlt_jet_ptoverp = el->pt() / hlt_jet_total_p;
                float hlt_jet_Et = (el->e() * hlt_jet_ptoverp) / 1000.0;
            }

            // --- Loop over L1_gFexLRJetRoI ---
            //for (const auto& el : *gFexLRJets) {
            //    gfex_larger_jet_pt_values.push_back(el->et() / 1000.0);
            //    float gfex_larger_jet_Et = el->et() / 1000.0;
            //}

            // --- Loop over InTimeAntiKt4TruthJets ---
            for (const auto& el : *TruthJets) {
                jet_pt_values.push_back(el->pt() / 1000.0);
                jet_log_pt_values.push_back(log10(el->pt() / 1000.0));
                float truth_jet_total_p = el->pt() * cosh(el->eta());
                float truth_jet_ptoverp = el->pt() / truth_jet_total_p;
                float truth_jet_Et = (el->e() * truth_jet_ptoverp) / 1000.0;
            }

            // --- Loop over TruthParticles (for Higgs and B's) ---
            std::vector<std::vector<float > > allb_list;
            for (const auto& el : *truthParticles) {
                if (el->pdgId() == 25 && el->status() == 22) {
                    higgs_counter++;
                    float total_p = el->pt() * cosh(el->eta());
                    float ptoverp = el->pt() / total_p;

                    if (higgs_counter == 1) {
                        higgs_1_pt = el->pt()/1000.0;
                        higgs_1_eta = el->eta();
                        higgs_1_phi = el->phi();
                        vector<vector<float>> b1_list;
                        find_non_higgs_daughters(el, b1_list);
                        b11_list = b1_list[0];
                        b21_list = b1_list[1];
                        float dR_1 = delta_R(b11_list[1], b11_list[2], b21_list[1], b21_list[2]);
                        deltaR_values.push_back(dR_1);
                        higgs_pt_values_for_dr.push_back(el->pt()/1000.0);
                    }
                    if (higgs_counter == 2) {
                        higgs_2_pt = el->pt()/1000.0;
                        higgs_2_eta = el->eta();
                        higgs_2_phi = el->phi();
                        vector<vector<float>> b2_list;
                        find_non_higgs_daughters(el, b2_list);
                        b12_list = b2_list[0];
                        b22_list = b2_list[1];
                        float dR_2 = delta_R(b12_list[1], b12_list[2], b22_list[1], b22_list[2]);
                        deltaR_values.push_back(dR_2);
                        higgs_pt_values_for_dr.push_back(el->pt()/1000.0);
                    }

                    float higgs_average_pt = (higgs_1_pt + higgs_2_pt) / 2.0;
                    higgs_pt_values.push_back(el->pt()/1000.0);
                    higgs_pt_values_by_event.push_back(el->pt()/1000.0);
                    std::cout << "higgs pt value: " << el->pt()/1000.0 << "\n";
                }
            }

            // --- After TruthParticles loop, build full b-list ---
            allb_list.push_back(b11_list);
            allb_list.push_back(b21_list);
            allb_list.push_back(b12_list);
            allb_list.push_back(b22_list);

            /*
            for (const auto& p : b11_list) allb_list.push_back({p[0], p[1], p[2]});
            for (const auto& p : b21_list) allb_list.push_back({p[0], p[1], p[2]});
            for (const auto& p : b12_list) allb_list.push_back({p[0], p[1], p[2]});
            for (const auto& p : b22_list) allb_list.push_back({p[0], p[1], p[2]});
            */

            std::sort(allb_list.begin(), allb_list.end(),
                [](const std::vector<float>& a, const std::vector<float>& b) {
                    return a[0] > b[0]; // Sort by Et descending
                });



            // --- Delta R matching between b-jets and truth jets / gFEX jets ---
            int b_iterator = 0;
            for (const auto& b : allb_list) {
                float b_Et  = b[0];
                float b_eta = b[1];
                float b_phi = b[2];
                std::vector<float> deltaR_truth_jets, deltaR_gFex_smallR_jets;

                for (const auto& jet : *TruthJets) {
                    deltaR_truth_jets.push_back(delta_R(b_eta, b_phi, jet->eta(), jet->phi()));
                }
                /*
                for (int i = 0 ; i < gFexSRJets->size(); i++){
                    deltaR_gFex_smallR_jets.push_back(delta_R(b_eta, b_phi, gFexSRJets[i].eta(), gFexSRJets[i].phi()));
                    b_gfex_deltar_list.push_back(delta_R(b_eta, b_phi, gFexSRJets[i].eta(), gFexSRJets[i].phi()));
                }*/

                for (const auto& jet : *gFexLRJets) {
                    deltaR_gFex_smallR_jets.push_back(delta_R(b_eta, b_phi, jet->eta(), jet->phi()));
                    b_gfex_deltar_list.push_back(delta_R(b_eta, b_phi, jet->eta(), jet->phi()));
                }

                std::sort(deltaR_truth_jets.begin(), deltaR_truth_jets.end());
                std::sort(deltaR_gFex_smallR_jets.begin(), deltaR_gFex_smallR_jets.end());

                if (!deltaR_truth_jets.empty() && !deltaR_gFex_smallR_jets.empty()) {
                    if (b_iterator == 0) {
                        highestEt_b_closest_truthjet_deltar_list.push_back(deltaR_truth_jets[0]);
                        highestEt_b_closest_gfex_deltar_list.push_back(deltaR_gFex_smallR_jets[0]);
                    }
                    if (b_iterator == 1) {
                        second_highestEt_b_closest_truthjet_deltar_list.push_back(deltaR_truth_jets[0]);
                        second_highestEt_b_closest_gfex_deltar_list.push_back(deltaR_gFex_smallR_jets[0]);
                    }
                    if (b_iterator == 2) {
                        third_highestEt_b_closest_truthjet_deltar_list.push_back(deltaR_truth_jets[0]);
                        third_highestEt_b_closest_gfex_deltar_list.push_back(deltaR_gFex_smallR_jets[0]);
                    }
                    if (b_iterator == 3) {
                        fourth_highestEt_b_closest_truthjet_deltar_list.push_back(deltaR_truth_jets[0]);
                        fourth_highestEt_b_closest_gfex_deltar_list.push_back(deltaR_gFex_smallR_jets[0]);
                    }
                }
                b_iterator++;
            }

            // --- Higgs Pt Cut ---
            if (higgsPtCut && signalBool) {
                if (higgs_pt_values_by_event[0] > 100.0 || higgs_pt_values_by_event[1] > 100.0){
                    higgs_passes_cut = true;
                    higgs_pt_values_after_cut.insert(higgs_pt_values_after_cut.end(), higgs_pt_values_by_event.begin(), higgs_pt_values_by_event.end());
                    for (const auto& b : allb_list) {
                        b_Et_values_after_higgs_cut.push_back(b[0]);
                    }
                }
                /*
                if (std::any_of(higgs_pt_values_by_event.begin(), higgs_pt_values_by_event.end(),
                                [&](float pt) { return (pt > higgs_min_pt); })) {
                    higgs_pt_values_after_cut.insert(higgs_pt_values_after_cut.end(),
                                                    higgs_pt_values_by_event.begin(), higgs_pt_values_by_event.end());
                    higgs_passes_cut = true;
                    for (const auto& b : allb_list) {
                        b_Et_values_after_higgs_cut.push_back(b[0]);
                    }*/
                        
                }
            }
        f->Close();
    }
    cout << "Processing complete." << endl;
}
