#include "process_event.h"
#define WRITE_LUT 0

// Main function
void process_event(input seedValues[nTotalSeeds_], input inputObjectValues[maxObjectsConsidered_], input outputJetValues[nSeeds_]){ // FIXME potentially use templated / overloaded func to deal with whether write out files while running synth or c-sim
    // Pragma for partitioning (allowing simultaneous access to) LUT array
    #pragma HLS ARRAY_PARTITION variable=lut_ cyclic factor=8 dim=1

    // PRAGMAS FOR WRITING DATA TO FPGA BRAMS (TESTING IMPLEMENTATION ONLY)
    // AXI4-Master interfaces for input arrays
    #pragma HLS INTERFACE m_axi port=seedValues        bundle=gmem0 offset=slave depth=nTotalSeeds_
    #pragma HLS INTERFACE m_axi port=inputObjectValues bundle=gmem1 offset=slave depth=maxObjectsConsidered_

    // AXI4-Master interfaces for output arrays
    #pragma HLS INTERFACE m_axi port=outputJetValues   bundle=gmem2 offset=slave depth=nSeeds_

    // AXI4-Lite interface only for control signals (function arguments, etc.)
    #pragma HLS INTERFACE s_axilite port=return bundle=CTRL

    #pragma HLS array_partition variable=inputObjectValues type=cyclic factor=4

    //int nMergedInput = 0;
    for (unsigned int iSeed = 0; iSeed < nSeeds_; ++iSeed){ // FIXME no longer considering highest Et seed first (need to implement some sorting)
        #pragma HLS unroll

        ap_uint<et_bit_length_ > outputJetEt = seedValues[iSeed].range(et_bit_length_-1, 0); // reset outputjet values for each seed, to values of seed
        //ap_uint<eta_bit_length_ > outputJetEta = seedValues[iSeed].eta; 
        //ap_uint<phi_bit_length_ > outputJetPhi = seedValues[iSeed].phi; 
        for (unsigned int iInput = 0; iInput < maxObjectsConsidered_; ++iInput){ // loop through input objects to consider merging
            //#pragma HLS loop_tripcount min=512 max=1024
            #pragma HLS unroll // pragma to unroll input object loop completely
            // let auto-pipelining do the work
            #if useInputEnergyCut_
            if (inputObjectValues[iInput].range(et_bit_length_-1, 0) >= inputEnergyCut_) continue; // skip past input objects below some minimum energy cut, if enabled 
            #endif
            //if (inputObjectValues[iInput].et == 0) break; // FIXME assumes that inputobjects are already sorted by Et (which seems to be the case from the loop through the topo422 aod collection)
            // Calculate signed differences (deltaEta and deltaPhi)
            ap_int<eta_bit_length_ + 1> deltaEta = seedValues[iSeed].range(et_bit_length_ + eta_bit_length_ - 1, et_bit_length_) - inputObjectValues[iInput].range(et_bit_length_ + eta_bit_length_ - 1, et_bit_length_);
            ap_int<phi_bit_length_ + 1> deltaPhi = seedValues[iSeed].range(total_bits - 1, total_bits - phi_bit_length_) - inputObjectValues[iInput].range(total_bits - 1, total_bits - phi_bit_length_);

            // Use unsigned type for absolute values, and ensure both operands are of the same type
            ap_uint<eta_bit_length_> uDeltaEta = deltaEta[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEta ) : static_cast<ap_uint<eta_bit_length_>>( deltaEta );
            ap_uint<phi_bit_length_> uDeltaPhi = deltaPhi[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhi ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhi );

            //ap_uint<eta_bit_length_ > deltaEta = (seedValues[iSeed].eta >= inputObjectValues[iInput].eta) ? seedValues[iSeed].eta - inputObjectValues[iInput].eta : inputObjectValues[iInput].eta - seedValues[iSeed].eta;
            //ap_uint<phi_bit_length_ > deltaPhi = (seedValues[iSeed].phi >= inputObjectValues[iInput].phi) ? seedValues[iSeed].phi - inputObjectValues[iInput].phi : inputObjectValues[iInput].phi - seedValues[iSeed].phi;
            unsigned int lut_index = uDeltaEta * (1 << phi_bit_length_) + uDeltaPhi; // Calculate LUT index corresponding to whether input object passes R^2 cut
            if (!(lut_index >= max_lut_size_) && lut_[lut_index]){ // only consider if lut index is smaller than max size (past max size, all values are False)
                outputJetEt += inputObjectValues[iInput].range(et_bit_length_-1, 0); // add input object Et to seed Et for resultant output jet Et
            }
        }

        outputJetValues[iSeed].range(et_bit_length_ - 1, 0) = outputJetEt; // set output Et 
        outputJetValues[iSeed].range(et_bit_length_ + eta_bit_length_ - 1, et_bit_length_) = seedValues[iSeed].range(et_bit_length_ + eta_bit_length_ - 1, et_bit_length_); // set output eta to seed eta
        outputJetValues[iSeed].range(total_bits - 1, total_bits - phi_bit_length_) = seedValues[iSeed].range(total_bits - 1, total_bits - phi_bit_length_); // set output phi to seed phi

    }
}