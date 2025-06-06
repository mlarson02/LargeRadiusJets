#include "jet_tagger_top.h"
void jet_tagger_top(etEtaPhi seedValues[nTotalSeeds_], etEtaPhi inputObjectValues[maxObjectsConsidered_], ap_uint<et_bit_length_ > outputJetEtValues[nSeeds_], ap_uint<eta_bit_length_ > outputJetEtaValues[nSeeds_], ap_uint<phi_bit_length_ > outputJetPhiValues[nSeeds_]){
    #pragma HLS INTERFACE m_axi port=seedValues offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axi port=inputObjectValues offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axi port=outputJetEtValues offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axi port=outputJetEtaValues offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axi port=outputJetPhiValues offset=slave bundle=gmem

    #pragma HLS INTERFACE s_axilite port=seedValues bundle=control
    #pragma HLS INTERFACE s_axilite port=inputObjectValues bundle=control
    #pragma HLS INTERFACE s_axilite port=outputJetEtValues bundle=control
    #pragma HLS INTERFACE s_axilite port=outputJetEtaValues bundle=control
    #pragma HLS INTERFACE s_axilite port=outputJetPhiValues bundle=control

    #pragma HLS INTERFACE s_axilite port=return bundle=control

    process_event(seedValues, inputObjectValues, outputJetEtValues, outputJetEtaValues, outputJetPhiValues);
}