#include "process_event.h"
#define WRITE_LUT 0

// Main function
void process_event(input seedValues[nSeedsInput_], input inputObjectValues[maxObjectsConsidered_], input (&outputJetValues)[nSeedsOutput_]){ // FIXME potentially use templated / overloaded func to deal with whether write out files while running synth or c-sim
    // Pragma for partitioning (allowing simultaneous access to) LUT array
    //#pragma HLS ARRAY_PARTITION variable=lut_ cyclic factor=4 dim=1
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
        outputJetValues[i] = 0;

    
    
    
    

    
    // FIXME make this entire process more dynamic to account for nSeedsOutput_ != 2 (progressively do this for highest Et seeds rather than for 1st 2 seeds immediately)
    // NEW PRE-PROCESSING OF SEEDS - SELECT IN BETWEEN LEADING, SUBLEADING JFEX SRJ, CLOSEST OF 3rd - 6th highest ENERGY JFEX SRJS as NEW SEEDS
    ap_uint<deltaRBits_> deltaRValuesSeed[nSeedsOutput_][nSeedsDeltaR_]; // FIXME replace 8 with constant from constants.h, easy
    ap_uint<3> deltaRValuesGreaterThan5Counter[nSeedsOutput_];
    bool indicesOfDeltaRValuesGreaterThan5[nSeedsOutput_][nSeedsDeltaR_];
    bool anotherSeedWithinDeltaR2p5[nSeedsOutput_];
    for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; iSeed++){ // loop through and calculate deltaR between leading, subleading & 3rd - 6th highest Et JFEX SRJ
        #pragma HLS unroll
        for (unsigned int iPreSeed = 0; iPreSeed < nSeedsDeltaR_; iPreSeed++){
            #pragma HLS unroll
            deltaRValuesSeed[iSeed][iPreSeed] = (1 << deltaRBits_) - 1; // Set to maximum value in case closer seed not found --> don't consider 0 values in sorting
            ap_int<eta_bit_length_ + 1> deltaEta = seedValues[iSeed].range(eta_high_, eta_low_) - seedValues[iPreSeed + nSeedsOutput_].range(eta_high_, eta_low_);
            ap_int<phi_bit_length_ + 1> deltaPhi = seedValues[iSeed].range(phi_high_, phi_low_) - seedValues[iPreSeed + nSeedsOutput_].range(phi_high_, phi_low_);
            // Use unsigned type for absolute values, and ensure both operands are of the same type
            ap_uint<eta_bit_length_> uDeltaEta = deltaEta[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEta ) : static_cast<ap_uint<eta_bit_length_>>( deltaEta );
            ap_uint<phi_bit_length_> uDeltaPhi = deltaPhi[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhi ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhi );
            ap_uint<phi_bit_length_ - 1> corrDeltaPhi; // for fixing delta phi calculation
            if (uDeltaPhi >= pi_digitized_in_phi_) uDeltaPhi = 2 * pi_digitized_in_phi_ - uDeltaPhi;
            corrDeltaPhi = uDeltaPhi; // using corr delta phi saves 1 bit, unsure if necessary?
            ap_uint<eta_bit_length_ + phi_bit_length_ + 2 > lutR_index = uDeltaEta * (1 << (phi_bit_length_ - 1) ) + corrDeltaPhi;
            if (!(lutR_index >= max_Rlut_size_)){
                deltaRValuesSeed[iSeed][iPreSeed] = lutR_[lutR_index];
                if (lutR_[lutR_index] <= rMergeConsiderCutDigitized_){
                    deltaRValuesGreaterThan5Counter[iSeed]++;
                    indicesOfDeltaRValuesGreaterThan5[iSeed][iPreSeed] = true;
                }
            }
        }
    }



    ap_uint<2> index_of_closest_seed1;
    ap_uint<2> index_of_closest_seed2;
    // Account for the case when multiple seeds are within deltaR ~ 5 - use highest Et seed to compute halfway point
    // For seed 0
    if (deltaRValuesGreaterThan5Counter[0] > 1) {
        ap_uint<et_bit_length_> maxEt = 0;
        index_of_closest_seed1 = 0;
        for (unsigned int iPreSeed = 0; iPreSeed < nSeedsDeltaR_; iPreSeed++) {
            #pragma HLS unroll
            if (indicesOfDeltaRValuesGreaterThan5[0][iPreSeed]) {
                ap_uint<et_bit_length_> et = seedValues[iPreSeed + nSeedsOutput_].range(et_high_, et_low_);
                if (et > maxEt) {
                    maxEt = et;
                    index_of_closest_seed1 = iPreSeed;
                }
            }
        }
    }
    else{
        index_of_closest_seed1 = index_of_min(deltaRValuesSeed[0]);
    }

    // For seed 1
    if (deltaRValuesGreaterThan5Counter[1] > 1) {
        ap_uint<et_bit_length_> maxEt = 0;
        index_of_closest_seed2 = 0;
        for (unsigned int iPreSeed = 0; iPreSeed < nSeedsDeltaR_; iPreSeed++) {
            #pragma HLS unroll
            if (indicesOfDeltaRValuesGreaterThan5[1][iPreSeed]) {
                ap_uint<et_bit_length_> et = seedValues[iPreSeed + nSeedsOutput_].range(et_high_, et_low_);
                if (et > maxEt) {
                    maxEt = et;
                    index_of_closest_seed2 = iPreSeed;
                }
            }
        }
    }
    else{
        index_of_closest_seed2 = index_of_min(deltaRValuesSeed[1]);
    }


    // FIXME next do bitonic sorting of deltaRValuesSeed, get index back to original seedValues array corresponding to closest to each seed (figure out what to do if they are same)
     // FIXME replace with a templated version of this s.t. you don't need to rewrite code for different number sorted
    ap_uint<2> indices[2];
    indices[0] = index_of_closest_seed1;
    indices[1] = index_of_closest_seed2;
    if(deltaRValuesSeed[0][index_of_closest_seed1] <= rMergeConsiderCutDigitized_){
       //std::cout << "index of closest seed 1: " << index_of_closest_seed1 << " and delta R of closest seed 1: " << deltaRValuesSeed[0][index_of_closest_seed1] * deltaR_step_ << " and energy of closest seed 1: " << seedValues[indices[0] + nSeedsOutput_].range(et_high_, et_low_) << "\n";
    }
    else{
       //std::cout << "first seed unchanged" << "\n";
    }
    if(deltaRValuesSeed[1][index_of_closest_seed1] <= rMergeConsiderCutDigitized_){
       //std::cout << "index of closest seed 2: " << index_of_closest_seed2 << " and delta R of closest seed 2: " << deltaRValuesSeed[1][index_of_closest_seed2] * deltaR_step_ << " and energy of closest seed 2: " << seedValues[indices[0] + nSeedsOutput_].range(et_high_, et_low_) << "\n";
    }
    else{
       //std::cout << "second seed unchanged" << "\n";
    }
    
    

    // next update leading, subleading seed positions (and energy?) to be in between closest other seed 
    for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; iSeed++){
        #pragma HLS unroll
        if (deltaRValuesSeed[iSeed][indices[iSeed]] > rMergeConsiderCutDigitized_) continue; // FIXME replace 64 (deltaR = 2.5 digitized)
        
       //std::cout << "-------------- calcing mid point -----------------" << "\n"; 
        //std::cout << "iSeed: " << iSeed << "\n";
        //fflush(stdout);

        // Get Et values for the two seeds (assumed already extracted)
        ap_uint<et_bit_length_> et1 = seedValues[iSeed].range(et_high_, et_low_);
        ap_uint<et_bit_length_> et2 = seedValues[indices[iSeed] + nSeedsOutput_].range(et_high_, et_low_);

        // Promote to wider type to avoid overflow
        ap_uint<et_bit_length_ + 1> etSum = et1 + et2;
        if (etSum == 0) etSum = 1; // avoid divide-by-zero

        // Convert eta to signed integers centered at 0
        ap_int<eta_bit_length_ + 1> eta1 = seedValues[iSeed].range(eta_high_, eta_low_) - (1 << (eta_bit_length_ - 1));
        ap_int<eta_bit_length_ + 1> eta2 = seedValues[indices[iSeed] + nSeedsOutput_].range(eta_high_, eta_low_) - (1 << (eta_bit_length_ - 1));

        // Convert phi to signed integers centered at 0
        ap_int<phi_bit_length_ + 1> phi1 = seedValues[iSeed].range(phi_high_, phi_low_) - (1 << (phi_bit_length_ - 1));
        ap_int<phi_bit_length_ + 1> phi2 = seedValues[indices[iSeed] + nSeedsOutput_].range(phi_high_, phi_low_) - (1 << (phi_bit_length_ - 1));

        // --- Compute midpoint in signed domain ---
        //ap_int<eta_bit_length_ + 1> eta_mid = (eta1 + eta2) >> 1; // Integer division by 2
        //ap_int<phi_bit_length_ + 1> phi_mid = (phi1 + phi2) >> 1;
        ap_int<eta_bit_length_ + 3> eta_mid_weighted = (et2 * eta2 + et1 * eta1) / etSum;
        ap_int<phi_bit_length_ + 3> phi_mid_weighted = (et2 * phi2 + et1 * phi1) / etSum;

       //std::cout << "eta 1 : " << eta1 << " eta 2 : " << eta2 << "\n";
       //std::cout << "phi1 : " << phi1 << " phi 2  : " << phi2 << "\n";
        
        // --- Optional: re-wrap phi midpoint to [-π, π) range if needed ---
        /*
        if (phi_mid > pi_digitized_in_phi_) 
            phi_mid -= (1 << phi_bit_length_);
        if (phi_mid < -(pi_digitized_in_phi_ + 1)) 
            phi_mid += (1 << phi_bit_length_);
        */
        if (phi_mid_weighted > pi_digitized_in_phi_)
            phi_mid_weighted -= (1 << phi_bit_length_);
        if (phi_mid_weighted < -(pi_digitized_in_phi_ + 1))
            phi_mid_weighted += (1 << phi_bit_length_);

       //std::cout << "eta mid : " << eta_mid << " phi_mid: " << phi_mid << "\n";

        // --- Convert midpoints back to digitized unsigned format ---
        ap_uint<eta_bit_length_> eta_mid_digitized = eta_mid_weighted + (1 << (eta_bit_length_ - 1));
        ap_uint<phi_bit_length_> phi_mid_digitized = phi_mid_weighted + (1 << (phi_bit_length_ - 1));
        //ap_uint<eta_bit_length_> eta_mid_digitized = eta_mid + (1 << (eta_bit_length_ - 1));
        //ap_uint<phi_bit_length_> phi_mid_digitized = phi_mid + (1 << (phi_bit_length_ - 1));
        //std::cout << "eta_mid_digitized: " << eta_mid_digitized << " phi_mid_digitized: " << phi_mid_digitized << "\n";
        seedValues[iSeed].range(eta_high_, eta_low_) = eta_mid_digitized;
        seedValues[iSeed].range(phi_high_, phi_low_) = phi_mid_digitized;
    }
    



    // must also account for when deltaRValuesSeed not set (no deltaR < 2.5 --> leave seed position as is)
    


    ap_uint<et_bit_length_ > outputJetEt;
    ap_uint<io_bit_length_ > numMergedIO; 
    for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; ++iSeed){ // FIXME no longer considering highest Et seed first (need to implement some sorting)
        #pragma HLS unroll
        //std::cout << "-------------- NEW SEED -------------- " << "\n";
        //std::cout << "iSeed: " << iSeed << "\n";
        //fflush(stdout);
        //std::cout << "outputjetvalues[0] "  << outputJetValues[0] << "\n";
        //fflush(stdout);
        numMergedIO = 0;
        outputJetEt = 0;//seedValues[iSeed].range(et_high_, et_low_); // reset outputjet values for each seed, to values of seed
        //std::cout << "seedValues[iSeed]: " << std::hex << seedValues[iSeed] << "\n";
        //std::cout << "seed et: " << std::dec << seedValues[iSeed].range(et_high_, et_low_) << " eta: " << seedValues[iSeed].range(eta_high_, eta_low_) << " phi: " << seedValues[iSeed].range(phi_high_, phi_low_) << "\n";
        //std::cout << "before" << "\n";
        //fflush(stdout);
        //outputJetValues[iSeed] = input(0);
        //std::cout << "after" << "\n";
        //fflush(stdout);
        for (unsigned int iInput = 0; iInput < maxObjectsConsidered_; ++iInput){ // loop through input objects to consider merging
            //#pragma HLS loop_tripcount min=512 max=1024
            #pragma HLS unroll
            //#pragma HLS unroll skip_exit_check factor=UNROLLFACTOR // pragma to unroll input object loop by pre-defined unroll factor
            //#pragma HLS pipeline II=PIPELINEII // pragma to pipeline innermost loop by pre-defined iteration interval, needs optimization! 
            #if useInputEnergyCut_
            if (inputObjectValues[iInput].range(et_high_, et_low_) <= inputEnergyCut_) continue; // skip past input objects below some minimum energy cut, if enabled 
            #endif
            
           // std::cout << "iInput: " << iInput << "\n";
           //std::cout << "inputObjectValues[iInput]: " << std::hex << inputObjectValues[iInput] << "\n";
           //std::cout << "input et: " << std::dec << inputObjectValues[iInput].range(et_high_, et_low_) << " eta: " << inputObjectValues[iInput].range(eta_high_, eta_low_) << " phi: " << inputObjectValues[iInput].range(phi_high_, phi_low_) << "\n";
           

            // Calculate signed differences (deltaEta and deltaPhi)
            ap_int<eta_bit_length_ + 1> deltaEta = seedValues[iSeed].range(eta_high_, eta_low_) - inputObjectValues[iInput].range(eta_high_, eta_low_);
            ap_int<phi_bit_length_ + 1> deltaPhi = seedValues[iSeed].range(phi_high_, phi_low_) - inputObjectValues[iInput].range(phi_high_, phi_low_);
            
            // Use unsigned type for absolute values, and ensure both operands are of the same type
            ap_uint<eta_bit_length_> uDeltaEta = deltaEta[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEta ) : static_cast<ap_uint<eta_bit_length_>>( deltaEta );
            ap_uint<phi_bit_length_> uDeltaPhi = deltaPhi[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhi ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhi );
            
            //std::cout << "deltaPhi: " << std::dec <<  deltaPhi << " uDeltaPhi: " << uDeltaPhi << "\n";
            //std::cout << "uDeltaEta: " << uDeltaEta << " uDeltaPhi: " << uDeltaPhi << "\n";
            ap_uint<phi_bit_length_ - 1> corrDeltaPhi; // for fixing delta phi calculation
            if (uDeltaPhi >= pi_digitized_in_phi_) uDeltaPhi = 2 * pi_digitized_in_phi_ - uDeltaPhi;
            corrDeltaPhi = uDeltaPhi; // using corr delta phi saves 1 bit, unsure if necessary?
            //std::cout << " corrDeltaPhi : " << corrDeltaPhi << "\n";
            //std::cout << "deltaR^2: " << uDeltaEta * uDeltaEta * eta_granularity_ * eta_granularity_ + corrDeltaPhi * corrDeltaPhi * phi_granularity_ * phi_granularity_ << "\n";
            //int phiLength = 1 << (phi_bit_length_);
            //std::cout << "phiLength: " << phiLength << " and uDeltaEta * philength: " << uDeltaEta * (1 << phi_bit_length_) << "\n";
            ap_uint<eta_bit_length_ + phi_bit_length_ + 2 > lut_index = uDeltaEta * (1 << (phi_bit_length_ - 1) ) + corrDeltaPhi; // Calculate LUT index corresponding to whether input object passes R^2 cut
            //std::cout << "lut_index: " << std::dec << lut_index << "\n";
            //volatile unsigned int lut_index = 
            //bool passesCut = !(lut_index >= max_lut_size_) && lut_[lut_index];
            //volatile bool readoutPassesCut = passesCut;
            if (!(lut_index >= max_R2lut_size_) && lut_[lut_index]){ // only consider if lut index is smaller than max size (past max size, all values are False)
                outputJetEt += inputObjectValues[iInput].range(et_high_, et_low_); // add input object Et to seed Et for resultant output jet Et
                numMergedIO++; 
                //std::cout << "merged!" << "\n";
                //std::cout << "new output jet Et: " << outputJetEt << "\n";
            }
        }
        /*
        outputJetValues[iSeed].range(et_bit_length_ - 1, 0) = outputJetEt; // set output Et 
        outputJetValues[iSeed].range(et_bit_length_ + eta_bit_length_ - 1, et_bit_length_) = seedValues[iSeed].range(et_bit_length_ + eta_bit_length_ - 1, et_bit_length_); // set output eta to seed eta
        outputJetValues[iSeed].range(total_bits_ - 1, total_bits_ - phi_bit_length_) = seedValues[iSeed].range(total_bits_ - 1, total_bits_ - phi_bit_length_); // set output phi to seed phi
        
       //std::cout << "before range access test" << "\n";
       //std::cout << "Raw bits of outputJetValues[1]: "
          << outputJetValues[1].to_string(2) << "\n";
        fflush(stdout);
       for (unsigned int i = 0; i < nSeedsOutput_; ++i) {
           //std::cout << "outputJetValues[" << i << "]: " << outputJetValues[i] << "\n";
            fflush(stdout);
        }
       //std::cout << "Range access test:\n";
       //std::cout << outputJetValues[0].range(et_high_, et_low_) << "\n";
       //std::cout << outputJetValues[1].range(et_high_, et_low_) << "\n";


      //std::cout << "test if it gets here" << "\n";
       fflush(stdout);*/
        // FOR IMPLEMENTATION ONLY
        //std::cout << "final numMergedIO: " << numMergedIO << "\n";
        //std::cout << "final outputJetEt: " << outputJetEt << "\n";
        outputJetValues[iSeed].range(io_high_, io_low_) = numMergedIO / 2; 
        outputJetValues[iSeed].range(et_high_, et_low_) = outputJetEt;
        outputJetValues[iSeed].range(eta_high_, eta_low_) = seedValues[iSeed].range(eta_high_, eta_low_);
        outputJetValues[iSeed].range(phi_high_, phi_low_) = seedValues[iSeed].range(phi_high_, phi_low_);
        //std::cout << "number of merged input objects for seed: "  << std::dec << iSeed << " is : " << std::dec << numMergedIO << "\n";
    }

}