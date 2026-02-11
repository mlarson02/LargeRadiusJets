#include "jet_tag.h"
#define WRITE_LUT 0

// Main function
void jet_tag(input inputObjectValues[maxObjectsConsidered_], output (&outputJetValues)[nSeedsOutput_]){ // FIXME potentially use templated / overloaded func to deal with whether write out files while running synth or c-sim
    // Pragma for partitioning (allowing simultaneous access to) LUT array
    #pragma HLS ARRAY_PARTITION variable=lut_ cyclic factor=4 dim=1
    #pragma HLS ARRAY_PARTITION variable=lutR_8b_ cyclic factor=4 dim=1
    //#pragma HLS ARRAY_PARTITION variable=inputObjectValues cyclic factor=4 dim=1 
    // PRAGMAS FOR WRITING DATA TO FPGA BRAMS (TESTING IMPLEMENTATION ONLY)
    // AXI4-Master interfaces for input arrays
    //#pragma HLS INTERFACE m_axi port=seedValues        bundle=gmem0 offset=slave depth=nSeedsInput_
    //#pragma HLS INTERFACE m_axi port=inputObjectValues bundle=gmem1 offset=slave depth=maxObjectsConsidered_
    // AXI4-Master interfaces for output arrays
    //#pragma HLS INTERFACE m_axi port=outputJetValues   bundle=gmem2 offset=slave depth=nSeedsOutput_
    // AXI4-Lite interface only for control signals (function arguments, etc.)
    //#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
    
    for (unsigned int i = 0; i < nSeedsOutput_; ++i)
        #pragma HLS unroll
        outputJetValues[i] = 0;
    
    for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; ++iSeed){ // Loop through seeds (which access up to Nth element of input object values now)
        #pragma HLS unroll
        ap_uint<eta_bit_length_ > seedEta = inputObjectValues[iSeed].range(eta_high_, eta_low_);
        ap_uint<phi_bit_length_ > seedPhi = inputObjectValues[iSeed].range(phi_high_, phi_low_);
        ap_uint<et_bit_length_ > outputJetEt = inputObjectValues[iSeed].range(et_high_, et_low_);
        //ap_uint<psi_R_bit_length_ + et_bit_length_ > psi_R_temp = 0; 
        //ap_uint<num_constituents_bit_length_ > numMergedIO = 0; 

        for (unsigned int iInput = nSeedsOutput_; iInput < maxObjectsConsidered_; ++iInput){ // loop through input objects to consider merging
            #pragma HLS unroll
           
            // Calculate signed differences (deltaEta and deltaPhi)
            ap_int<eta_bit_length_ + 1> deltaEta = inputObjectValues[iSeed].range(eta_high_, eta_low_) - inputObjectValues[iInput].range(eta_high_, eta_low_);
            ap_int<phi_bit_length_ + 1> deltaPhi = inputObjectValues[iSeed].range(phi_high_, phi_low_) - inputObjectValues[iInput].range(phi_high_, phi_low_);
            
            // Use unsigned type for absolute values, and ensure both operands are of the same type
            ap_uint<eta_bit_length_> uDeltaEta = deltaEta[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEta ) : static_cast<ap_uint<eta_bit_length_>>( deltaEta );
            ap_uint<phi_bit_length_> uDeltaPhi = deltaPhi[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhi ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhi );

            if (uDeltaPhi >= pi_digitized_in_phi_) uDeltaPhi = 2 * pi_digitized_in_phi_ - uDeltaPhi;
            ap_uint<phi_bit_length_ - 1> corrDeltaPhi = uDeltaPhi; // using corr delta phi saves 1 bit, unsure if necessary?

            ap_uint<eta_bit_length_ + phi_bit_length_ + 2 > lut_index = uDeltaEta * (1 << (phi_bit_length_ - 1) ) + corrDeltaPhi; // Calculate LUT index corresponding to whether input object passes R^2 cut

            if (!(lut_index >= max_R2lut_size_) && lut_[lut_index]){ // only consider if lut index is smaller than max size (past max size, all values are False) // FIXME will throw an error for deltaR!=1.0
                if(outputJetEt + inputObjectValues[iInput].range(et_high_, et_low_) >= ((1 << (et_bit_length_)) - 1)){
                    outputJetEt = ((1 << (et_bit_length_)) - 1); // if would exceed max Et, set equal to max Et and break out of input object loop
                //    break; // br
                } 
                else{
                    //#pragma HLS bind_op variable=outputJetEt op=add impl=dsp latency=-1
                    outputJetEt += inputObjectValues[iInput].range(et_high_, et_low_); // add input object Et to seed Et for resultant output jet Et
                }
                
                //#pragma HLS bind_op variable=psi_R_temp op=add impl=dsp latency=-1
                //std::cout << "lutR_8b_[lut_index]: " << lutR_8b_[lut_index] << "\n";
                //std::cout << "inputObjectValues et: " << inputObjectValues[iInput].range(et_high_, et_low_) << "\n";
                //psi_R_temp += inputObjectValues[iInput].range(et_high_, et_low_) * lutR_8b_[lut_index];
                //std::cout << "psi_R_temp: " << psi_R_temp << "\n";
               //#pragma HLS bind_op variable=numMergedIO op=add impl=dsp latency=-1
                //numMergedIO++; 
            }
        }

        // Normalize by 1/Jet Et
        //ap_uint<psi_R_bit_length_ > psi_R_final = psi_R_temp / outputJetEt;
        //std::cout << "outputJetEt: " << outputJetEt << "\n";
        //std::cout << "psi_R_final: " << psi_R_final << "\n";
        //if (outputJetEt != 0){
           // psi_R_final  
        //}

        outputJetValues[iSeed].range(padded_zeroes_high_, psi_R_low_) = 0; 
        //outputJetValues[iSeed].range(num_constituents_high_, num_constituents_low_) = numMergedIO;
        //outputJetValues[iSeed].range(psi_R_high_, psi_R_low_) = psi_R_final; 
        outputJetValues[iSeed].range(et_high_, et_low_) = outputJetEt;
        outputJetValues[iSeed].range(eta_high_, eta_low_) = seedEta;
        outputJetValues[iSeed].range(phi_high_, phi_low_) = seedPhi;
    }
}