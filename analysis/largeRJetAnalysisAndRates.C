#include "analysisHelperFunctions.h"


void largeRJetAnalysisAndRates(bool overlayThreeFiles = false){
    using clock = std::chrono::steady_clock;
    auto t0 = clock::now();

    //ROOT::EnableImplicitMT(8);   // use 8 threads - this didn't speed anything up!
    //const std::string signalLargeRJetDataFileName = "/eos/home-m/mlarson/LargeRadiusJets/MemPrints/largeRJets/mc21_14TeV_hh_bbbb_vbf_novhh_largeR.dat";
    //const std::string backgroundLargeRJetDataFileName = "/eos/home-m/mlarson/LargeRadiusJets/MemPrints/largeRJets/mc21_14TeV_jj_JZ3_largeR.dat";
    constexpr bool vbfBool = true;
    std::vector<std::string > signalRootFileNames = {
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.44_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_1.5_IOs_128_Seeds_2_R2_1.44_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_2_IOs_32_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_2_IOs_64_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP_EtWeighted/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP_EtWeighted/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP_EtWeighted/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_1.5_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_2.5_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",

                                                    // NEW ggF + PU Suppression Files
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_NoSK.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_NoSK.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_NoSK.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_rMerge_2.5_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_2.5_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_NoSK.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_NoSK.root",
                                                    "/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                    "/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root"
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_rMerge_1.75_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_1.75_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_hh_bbbb_vbf_novhh_e8557_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                    
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.44_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_NoSK.root",
                                                    //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_HHbbbb_HLLHC_e8564_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.44_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root"
                                                        };
    std::vector<std::string > backgroundRootFileNames = {
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.44_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_1.5_IOs_128_Seeds_2_R2_1.44_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2_IOs_32_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2_IOs_64_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_1.5_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2.5_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_FixedHSTP/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2.5_IOs_128_Seeds_2_R2_1.44_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_OR.root",

                                                        // NEW PU Suppression Files
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_NoSK.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_NoSK.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2.5_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2.5_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_NoSK.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_NoSK.root",
                                                        "/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                        "/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root"
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_0.001_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_1.75_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",
                                                        //"/data/larsonma/LargeRadiusJets/outFputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_1.75_IOs_128_Seeds_2_R2_1.21_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root",

                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.44_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_NoSK.root",
                                                        //"/data/larsonma/LargeRadiusJets/outputNTuplesDev_PUSuppression/mc21_14TeV_jj_JZ_e8557_s4422_r16130_rMerge_2_IOs_128_Seeds_2_R2_1.44_IO_gepCellsTowers_Seed_gepWTAConeCellsTowersJets_SK.root"
                                                        };
    TString overlayOutputFileDir = "overlayMultipleFiles/largeRJetHistograms_ggF_PUSuppressed/";            
    gSystem->mkdir(overlayOutputFileDir);                                    
    gErrorIgnoreLevel = kError;
    std::cout << "number of signal files: " << signalRootFileNames.size() << " number of background files: " << backgroundRootFileNames.size() << "\n";
    analyze_files(signalRootFileNames, backgroundRootFileNames, overlayOutputFileDir, overlayThreeFiles);

    std::cout << "[TIMER] total: "
              << std::chrono::duration<double>(clock::now() - t0).count()
              << " s\n";
    gSystem->Exit(0);
}