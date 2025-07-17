#include "process_event.h"
#define WRITE_LUT 0

// Main function
void process_event(input seedValues[nTotalSeeds_], input inputObjectValues[maxObjectsConsidered_], input outputJetValues[nSeedsOutput_]){ // FIXME potentially use templated / overloaded func to deal with whether write out files while running synth or c-sim
    // Pragma for partitioning (allowing simultaneous access to) LUT array
    //#pragma HLS ARRAY_PARTITION variable=lut_ cyclic factor=4 dim=1
    #pragma HLS ARRAY_PARTITION variable=inputObjectValues cyclic factor=4 dim=1 
    /*
    
    // PRAGMAS FOR WRITING DATA TO FPGA BRAMS (TESTING IMPLEMENTATION ONLY)
    // AXI4-Master interfaces for input arrays
    #pragma HLS INTERFACE m_axi port=seedValues        bundle=gmem0 offset=slave depth=nTotalSeeds_
    #pragma HLS INTERFACE m_axi port=inputObjectValues bundle=gmem1 offset=slave depth=maxObjectsConsidered_
    // AXI4-Master interfaces for output arrays
    #pragma HLS INTERFACE m_axi port=outputJetValues   bundle=gmem2 offset=slave depth=nSeeds_
    // AXI4-Lite interface only for control signals (function arguments, etc.)
    #pragma HLS INTERFACE s_axilite port=return bundle=CTRL
    */

    // NEW PRE-PROCESSING OF SEEDS - SELECT IN BETWEEN LEADING, SUBLEADING JFEX SRJ, CLOSEST OF 3rd - 6th highest ENERGY JFEX SRJS as NEW SEEDS
    ap_uint<8> deltaRValuesSeed[nSeedsDeltaR_][nSeedsOutput_]; // FIXME replace 8 with constant from constants.h, easy
    for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; iSeed++){ // loop through and calculate deltaR between leading, subleading & 3rd - 6th highest Et JFEX SRJ
        for (unsigned int iPreSeed = nSeedsOutput_; iPreSeed < nSeedsInput_; iPreSeed++){
            ap_int<eta_bit_length_ + 1> deltaEta = seedValues[iSeed].range(eta_high_, eta_low_) - seedValues[iPreSeed + nSeedsOutput_].range(eta_high_, eta_low_);
            ap_int<phi_bit_length_ + 1> deltaPhi = seedValues[iSeed].range(phi_high_, phi_low_) - seedValues[iPreSeed + nSeedsOutput_].range(phi_high_, phi_low_);
            
            // Use unsigned type for absolute values, and ensure both operands are of the same type
            ap_uint<eta_bit_length_> uDeltaEta = deltaEta[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEta ) : static_cast<ap_uint<eta_bit_length_>>( deltaEta );
            ap_uint<phi_bit_length_> uDeltaPhi = deltaPhi[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhi ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhi );
            ap_uint<phi_bit_length_ - 1> corrDeltaPhi; // for fixing delta phi calculation
            if (uDeltaPhi >= pi_digitized_in_phi_) uDeltaPhi = 2 * pi_digitized_in_phi_ - uDeltaPhi;
            corrDeltaPhi = uDeltaPhi; // using corr delta phi saves 1 bit, unsure if necessary?
            ap_uint<eta_bit_length_ + phi_bit_length_ + 2 > lutR_index = uDeltaEta * (1 << (phi_bit_length_ - 1) ) + corrDeltaPhi;
            if (!(lutR_index >= max_Rlut_size_) && lut_[lutR_index]){
                deltaRValuesSeed[iPreSeed][iSeed];
            }
        }
    }

    // FIXME next do bitonic sorting of deltaRValuesSeed, get index back to original seedValues array corresponding to closest to each seed (figure out what to do if they are same)



    // next update leading, subleading seed positions (and energy?) to be in between closest other seed 

    // must also account for when deltaRValuesSeed not set (no deltaR < 2.5 --> leave seed position as is)
    


    ap_uint<et_bit_length_ > outputJetEt;
    ap_uint<io_bit_length_ > numMergedIO; 
    for (unsigned int iSeed = 0; iSeed < nSeeds_; ++iSeed){ // FIXME no longer considering highest Et seed first (need to implement some sorting)
        #pragma HLS unroll
        //std::cout << "-------------- NEW SEED -------------- " << "\n";
        //std::cout << "iSeed: " << iSeed << "\n";
        numMergedIO = 0;
        outputJetEt = 0;//seedValues[iSeed].range(et_high_, et_low_); // reset outputjet values for each seed, to values of seed
        outputJetValues[iSeed] = 0;
        for (unsigned int iInput = 0; iInput < maxObjectsConsidered_; ++iInput){ // loop through input objects to consider merging
            //#pragma HLS loop_tripcount min=512 max=1024
            #pragma HLS unroll
            //#pragma HLS unroll skip_exit_check factor=UNROLLFACTOR // pragma to unroll input object loop by pre-defined unroll factor
            //#pragma HLS pipeline II=PIPELINEII // pragma to pipeline innermost loop by pre-defined iteration interval, needs optimization! 
            #if useInputEnergyCut_
            if (inputObjectValues[iInput].range(et_high_, et_low_) <= inputEnergyCut_) continue; // skip past input objects below some minimum energy cut, if enabled 
            #endif
            /*
            std::cout << "iInput: " << iInput << "\n";
            std::cout << "inputObjectValues[iInput]: " << std::hex << inputObjectValues[iInput] << "\n";
            std::cout << "input et: " << std::dec << inputObjectValues[iInput].range(et_high_, et_low_) << " eta: " << inputObjectValues[iInput].range(eta_high_, eta_low_) << " phi: " << inputObjectValues[iInput].range(phi_high_, phi_low_) << "\n";
            std::cout << "seedValues[iSeed]: " << std::hex << seedValues[iSeed] << "\n";
            std::cout << "seed et: " << std::dec << seedValues[iSeed].range(et_high_, et_low_) << " eta: " << seedValues[iSeed].range(eta_high_, eta_low_) << " phi: " << seedValues[iSeed].range(phi_high_, phi_low_) << "\n";
            */
            // Calculate signed differences (deltaEta and deltaPhi)
            ap_int<eta_bit_length_ + 1> deltaEta = seedValues[iSeed].range(eta_high_, eta_low_) - inputObjectValues[iInput].range(eta_high_, eta_low_);
            ap_int<phi_bit_length_ + 1> deltaPhi = seedValues[iSeed].range(phi_high_, phi_low_) - inputObjectValues[iInput].range(phi_high_, phi_low_);
            
            // Use unsigned type for absolute values, and ensure both operands are of the same type
            ap_uint<eta_bit_length_> uDeltaEta = deltaEta[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEta ) : static_cast<ap_uint<eta_bit_length_>>( deltaEta );
            ap_uint<phi_bit_length_> uDeltaPhi = deltaPhi[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhi ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhi );
            
            //std::cout << "deltaPhi: " << deltaPhi << " uDeltaPhi: " << uDeltaPhi << "\n";
            //std::cout << "uDeltaEta: " << uDeltaEta << " uDeltaPhi: " << uDeltaPhi << "\n";
            ap_uint<phi_bit_length_ - 1> corrDeltaPhi; // for fixing delta phi calculation
            if (uDeltaPhi >= pi_digitized_in_phi_) uDeltaPhi = 2 * pi_digitized_in_phi_ - uDeltaPhi;
            corrDeltaPhi = uDeltaPhi; // using corr delta phi saves 1 bit, unsure if necessary?
            //std::cout << " corrDeltaPhi : " << corrDeltaPhi << "\n";
            //std::cout << "deltaR^2: " << uDeltaEta * uDeltaEta * eta_granularity_ * eta_granularity_ + corrDeltaPhi * corrDeltaPhi * phi_granularity_ * phi_granularity_ << "\n";
            //int phiLength = 1 << (phi_bit_length_);
            //std::cout << "phiLength: " << phiLength << " and uDeltaEta * philength: " << uDeltaEta * (1 << phi_bit_length_) << "\n";
            ap_uint<eta_bit_length_ + phi_bit_length_ + 2 > lut_index = uDeltaEta * (1 << (phi_bit_length_ - 1) ) + corrDeltaPhi; // Calculate LUT index corresponding to whether input object passes R^2 cut
            //std::cout << "lut_index: " << lut_index << "\n";
            //volatile unsigned int lut_index = 
            //bool passesCut = !(lut_index >= max_lut_size_) && lut_[lut_index];
            //volatile bool readoutPassesCut = passesCut;
            if (!(lut_index >= max_R2lut_size_) && lut_[lut_index]){ // only consider if lut index is smaller than max size (past max size, all values are False)
                outputJetEt += inputObjectValues[iInput].range(et_high_, et_low_); // add input object Et to seed Et for resultant output jet Et
                numMergedIO++; 
                //std::cout << "merged!" << "\n";
            }
        }
        /*
        outputJetValues[iSeed].range(et_bit_length_ - 1, 0) = outputJetEt; // set output Et 
        outputJetValues[iSeed].range(et_bit_length_ + eta_bit_length_ - 1, et_bit_length_) = seedValues[iSeed].range(et_bit_length_ + eta_bit_length_ - 1, et_bit_length_); // set output eta to seed eta
        outputJetValues[iSeed].range(total_bits_ - 1, total_bits_ - phi_bit_length_) = seedValues[iSeed].range(total_bits_ - 1, total_bits_ - phi_bit_length_); // set output phi to seed phi
        */
        // FOR IMPLEMENTATION ONLY
        outputJetValues[iSeed].range(io_high_, io_low_) = numMergedIO / 2; 
        outputJetValues[iSeed].range(et_high_, et_low_) = outputJetEt;
        outputJetValues[iSeed].range(eta_high_, eta_low_) = seedValues[iSeed].range(eta_high_, eta_low_);
        outputJetValues[iSeed].range(phi_high_, phi_low_) = seedValues[iSeed].range(phi_high_, phi_low_);

        //std::cout << "number of merged input objects for seed: "  << std::dec << iSeed << " is : " << std::dec << numMergedIO << "\n";
    }

}