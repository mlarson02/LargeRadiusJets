#include "jet_tag_adv.h"
#define WRITE_LUT 0

// Main function
void jet_tag_adv(input seedValues[nSeedsInput_], input inputObjectValues[maxObjectsConsidered_], output (&outputJetValues)[nSeedsOutput_]){ // FIXME potentially use templated / overloaded func to deal with whether write out files while running synth or c-sim
    // Pragma for partitioning (allowing simultaneous access to) LUT array
    #pragma HLS ARRAY_PARTITION variable=lut_ cyclic factor=4 dim=1
    #pragma HLS ARRAY_PARTITION variable=lutR_8b_ cyclic factor=4 dim=1
    #pragma HLS ARRAY_PARTITION variable=inputObjectValues cyclic factor=4 dim=1 
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
    ap_uint<deltaRBits_> deltaRValuesSeed[nSeedsOutput_][nSeedsDeltaR_] = {0}; 
    ap_uint<3> deltaRValuesGreaterThan5Counter[nSeedsOutput_] = {0};
    //std::cout << "deltaRValuesGreaterThan5Counter 0th: " << deltaRValuesGreaterThan5Counter[0] << "\n";
    //std::cout << "deltaRValuesGreaterThan5Counter 1st: " << deltaRValuesGreaterThan5Counter[1] << "\n";
    bool indicesOfDeltaRValuesGreaterThanrMergeCut[nSeedsOutput_][nSeedsDeltaR_] = {false};
    //std::cout << "Seed 1 Eta, Phi: " << seedValues[0].range(eta_high_, eta_low_) << " , " << seedValues[0].range(phi_high_, phi_low_) << "\n";
    //std::cout << "Seed 2 Eta, Phi: " << seedValues[1].range(eta_high_, eta_low_) << " , " << seedValues[1].range(phi_high_, phi_low_) << "\n";

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
            if (uDeltaPhi >= pi_digitized_in_phi_) uDeltaPhi = 2 * pi_digitized_in_phi_ - uDeltaPhi;
            ap_uint<phi_bit_length_ - 1> corrDeltaPhi = uDeltaPhi; // using corr delta phi saves 1 bit, unsure if necessary?
            ap_uint<eta_bit_length_ + phi_bit_length_ + 2 > lutR_index = uDeltaEta * (1 << (phi_bit_length_ - 1) ) + corrDeltaPhi;
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
    
    ap_uint<2> index_of_closest_seed1 = 0;
    ap_uint<2> index_of_closest_seed2 = 0;
    // Account for the case when multiple seeds are within deltaR customalizable value - use highest Et seed to compute halfway point
    // For seed 0
    if (deltaRValuesGreaterThan5Counter[0] > 1) {
        ap_uint<et_bit_length_> maxEt = 0;
        for (unsigned int iPreSeed = 0; iPreSeed < nSeedsDeltaR_; iPreSeed++) {
            #pragma HLS unroll
            if (indicesOfDeltaRValuesGreaterThanrMergeCut[0][iPreSeed]) {
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
        for (unsigned int iPreSeed = 0; iPreSeed < nSeedsDeltaR_; iPreSeed++) {
            #pragma HLS unroll
            if (indicesOfDeltaRValuesGreaterThanrMergeCut[1][iPreSeed]) {
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
    // Printouts for debugging seed position recalculation
    /*if(deltaRValuesSeed[0][index_of_closest_seed1] <= rMergeConsiderCutDigitized_){
       std::cout << "index of closest seed 1: " << index_of_closest_seed1 << " and delta R of closest seed 1: " << deltaRValuesSeed[0][index_of_closest_seed1] * deltaR_step_ << " and energy of closest seed 1: " << seedValues[indices[0] + nSeedsOutput_].range(et_high_, et_low_) << "\n";
    }
    else{
       std::cout << "first seed unchanged" << "\n";
    }
    if(deltaRValuesSeed[1][index_of_closest_seed1] <= rMergeConsiderCutDigitized_){
       std::cout << "index of closest seed 2: " << index_of_closest_seed2 << " and delta R of closest seed 2: " << deltaRValuesSeed[1][index_of_closest_seed2] * deltaR_step_ << " and energy of closest seed 2: " << seedValues[indices[0] + nSeedsOutput_].range(et_high_, et_low_) << "\n";
    }
    else{
       std::cout << "second seed unchanged" << "\n";
    }*/
    

    // next update leading, subleading seed positions (and energy?) to be in between closest other seed 
    for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; iSeed++){
        #pragma HLS unroll
        if (deltaRValuesSeed[iSeed][indices[iSeed]] > rMergeConsiderCutDigitized_) continue; // FIXME replace 64 (deltaR = 2.5 digitized)
        
        //std::cout << "-------------- calcing mid point -----------------" << "\n"; 
        //std::cout << "iSeed: " << iSeed << "\n";
        //fflush(stdout);

        const ap_int<phi_bit_length_ + 2> PI_D     =  ap_int<phi_bit_length_ + 2>(pi_digitized_in_phi_);
        const ap_int<phi_bit_length_ + 2> TWO_PI_D =  ap_int<phi_bit_length_ + 2>((1 << phi_bit_length_) - 1);

        auto wrapSym = [&](ap_int<phi_bit_length_ + 2> x) -> ap_int<phi_bit_length_ + 2> {
            #pragma HLS INLINE
            if (x >  PI_D)   x -= TWO_PI_D;
            if (x < -PI_D)  x += TWO_PI_D;
            return x;
        };

        // Get Et values for the two seeds (assumed already extracted)
        //ap_uint<et_bit_length_> et1 = seedValues[iSeed].range(et_high_, et_low_);
        //ap_uint<et_bit_length_> et2 = seedValues[indices[iSeed] + nSeedsOutput_].range(et_high_, et_low_);

        // Promote to wider type to avoid overflow
        //ap_uint<et_bit_length_ + 1> etSum = et1 + et2;
        //if (etSum == 0) etSum = 1; // avoid divide-by-zero

        // Convert eta to signed integers centered at 0
        ap_int<eta_bit_length_ + 1> eta1 = seedValues[iSeed].range(eta_high_, eta_low_) - (1 << (eta_bit_length_ - 1));
        ap_int<eta_bit_length_ + 1> eta2 = seedValues[indices[iSeed] + nSeedsOutput_].range(eta_high_, eta_low_) - (1 << (eta_bit_length_ - 1));
        //std::cout << "eta 1 : " << eta1 << " and eta2 : " << eta2 << "\n";
        // Convert phi to signed integers centered at 0
        //ap_int<phi_bit_length_ + 1> phi1 = seedValues[iSeed].range(phi_high_, phi_low_) - (1 << (phi_bit_length_ - 1));
        //ap_int<phi_bit_length_ + 1> phi2 = seedValues[indices[iSeed] + nSeedsOutput_].range(phi_high_, phi_low_) - (1 << (phi_bit_length_ - 1));

        // Signed, extended-width versions to avoid overflow on sums/diffs
        //std::cout << "phi1: " << seedValues[iSeed].range(phi_high_, phi_low_) << " and phi2: " << seedValues[indices[iSeed] + nSeedsOutput_].range(phi_high_, phi_low_) << "\n";
        ap_int<phi_bit_length_ + 2> phi1s = seedValues[iSeed].range(phi_high_, phi_low_) - (1 << (phi_bit_length_ - 1));
        ap_int<phi_bit_length_ + 2> phi2s = seedValues[indices[iSeed] + nSeedsOutput_].range(phi_high_, phi_low_) - (1 << (phi_bit_length_ - 1));
        //std::cout << "phi1s: " << phi1s << " and phi2s: " << phi2s << "\n";
        // --- Shortest-arc midpoint (unweighted) ---
        ap_int<phi_bit_length_ + 2> dphi = phi2s - phi1s;
        //std::cout << "dphi before wrap :" << dphi << "\n";
        dphi = wrapSym(dphi);                          // now in [-PI, PI)
        //std::cout << "dphi after wrap :" << dphi << "\n";
        ap_int<phi_bit_length_ + 2> phi_mid = phi1s + (dphi >> 1);  // arithmetic shift (divide by 2)
        //std::cout << "phi_mid before wrap :" << phi_mid << "\n";
        phi_mid = wrapSym(phi_mid);
        //std::cout << "phi_mid after wrap :" << phi_mid << "\n";

        //std::cout << "dphi: " << dphi << "\n";
        //std::cout << "phi_mid: " << phi_mid << "\n";

        // --- Compute midpoint in signed domain ---
        ap_int<eta_bit_length_ + 1> eta_mid = (eta1 + eta2) >> 1; // Integer division by 2
        //ap_int<phi_bit_length_ + 1> phi_mid = (phi1 + phi2) >> 1;
        //ap_int<eta_bit_length_ + 3> eta_mid_weighted = (et2 * eta2 + et1 * eta1) / etSum;
        //ap_int<phi_bit_length_ + 3> phi_mid_weighted = (et2 * phi2 + et1 * phi1) / etSum;

       //std::cout << "eta 1 : " << eta1 << " eta 2 : " << eta2 << "\n";
       //std::cout << "phi1 : " << phi1 << " phi 2  : " << phi2 << "\n";
        
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
        ap_uint<eta_bit_length_> eta_mid_digitized = eta_mid + (1 << (eta_bit_length_ - 1));
        ap_uint<phi_bit_length_> phi_mid_digitized = phi_mid + (1 << (phi_bit_length_ - 1));

        //std::cout << "phi_mid_digitized: " << phi_mid_digitized << "\n";
        
        //std::cout << "eta_mid_digitized: " << eta_mid_digitized << " phi_mid_digitized: " << phi_mid_digitized << "\n";
        seedValues[iSeed].range(eta_high_, eta_low_) = eta_mid_digitized;
        seedValues[iSeed].range(phi_high_, phi_low_) = phi_mid_digitized;
    }

    // must also account for when deltaRValuesSeed not set (no deltaR < 2.5 --> leave seed position as is)
    unsigned int numSubjets[nSeedsOutput_] = {0};
    for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; ++iSeed){ 
        #pragma HLS unroll
        ap_uint<et_bit_length_ > outputJetEt = 0;

        for (unsigned int iInput = 0; iInput < maxObjectsConsidered_; ++iInput){ // loop through input objects to consider merging
            #pragma HLS unroll

            // Calculate signed differences (deltaEta and deltaPhi)
            ap_int<eta_bit_length_ + 1> deltaEta = seedValues[iSeed].range(eta_high_, eta_low_) - inputObjectValues[iInput].range(eta_high_, eta_low_);
            ap_int<phi_bit_length_ + 1> deltaPhi = seedValues[iSeed].range(phi_high_, phi_low_) - inputObjectValues[iInput].range(phi_high_, phi_low_);
            
            // Use unsigned type for absolute values, and ensure both operands are of the same type
            ap_uint<eta_bit_length_> uDeltaEta = deltaEta[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEta ) : static_cast<ap_uint<eta_bit_length_>>( deltaEta );
            ap_uint<phi_bit_length_> uDeltaPhi = deltaPhi[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhi ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhi );

            if (uDeltaPhi >= pi_digitized_in_phi_) uDeltaPhi = 2 * pi_digitized_in_phi_ - uDeltaPhi;
            ap_uint<phi_bit_length_ - 1> corrDeltaPhi = uDeltaPhi; // using corr delta phi saves 1 bit, unsure if necessary?
            //std::cout << " corrDeltaPhi : " << corrDeltaPhi << "\n";
            //std::cout << "deltaR^2: " << uDeltaEta * uDeltaEta * eta_granularity_ * eta_granularity_ + corrDeltaPhi * corrDeltaPhi * phi_granularity_ * phi_granularity_ << "\n";
            //int phiLength = 1 << (phi_bit_length_);
            //std::cout << "phiLength: " << phiLength << " and uDeltaEta * philength: " << uDeltaEta * (1 << phi_bit_length_) << "\n";
            ap_uint<eta_bit_length_ + phi_bit_length_ + 2 > lut_index = (uDeltaEta << (phi_bit_length_ - 1)) | corrDeltaPhi; // Calculate LUT index corresponding to whether input object passes R^2 cut
            //std::cout << "lut_index: " << std::dec << lut_index << "\n";

            if (!(lut_index >= max_R2lut_size_) && lut_[lut_index]){ // only consider if lut index is smaller than max size (past max size, all values are False) // FIXME will throw an error for deltaR!=1.0
                if(outputJetEt + inputObjectValues[iInput][0] >= (1 << (et_bit_length_) - 1)){
                    outputJetEt = (1 << (et_bit_length_) - 1); // if would exceed max Et, set equal to max Et and break out of input object loop
                //    break; // br
                } 
                else{
                    outputJetEt += inputObjectValues[iInput].range(et_high_, et_low_); // add input object Et to seed Et for resultant output jet Et
                }

            }
        }
        for(unsigned int iSubjet = 0; iSubjet < nSeedsInput_; iSubjet++){
            #pragma HLS unroll

            if(seedValues[iSubjet].range(et_high_, et_low_) <= subjet_et_threshold_) continue;
            // Calculate signed differences (deltaEta and deltaPhi)
            ap_int<eta_bit_length_ + 1> deltaEta = seedValues[iSeed].range(eta_high_, eta_low_) - seedValues[iSubjet].range(eta_high_, eta_low_);
            ap_int<phi_bit_length_ + 1> deltaPhi = seedValues[iSeed].range(phi_high_, phi_low_) - seedValues[iSubjet].range(phi_high_, phi_low_);
            
            // Use unsigned type for absolute values, and ensure both operands are of the same type
            ap_uint<eta_bit_length_> uDeltaEta = deltaEta[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEta ) : static_cast<ap_uint<eta_bit_length_>>( deltaEta );
            ap_uint<phi_bit_length_> uDeltaPhi = deltaPhi[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhi ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhi );

            if (uDeltaPhi >= pi_digitized_in_phi_) uDeltaPhi = 2 * pi_digitized_in_phi_ - uDeltaPhi;
            ap_uint<phi_bit_length_ - 1> corrDeltaPhi = uDeltaPhi; 

            ap_uint<eta_bit_length_ + phi_bit_length_ + 2 > lut_index = (uDeltaEta << (phi_bit_length_ - 1)) | corrDeltaPhi; // Calculate LUT index corresponding to whether input object passes R^2 cut
            if (!(lut_index >= max_R2lut_size_) && lut_[lut_index]){ // only consider if lut index is smaller than max size (past max size, all values are False) // FIXME will throw an error for deltaR!=1.0
                numSubjets[iSeed]++;
            }
        }
        outputJetValues[iSeed].range(padded_zeroes_high_, padded_zeroes_low_) = 0; 
        outputJetValues[iSeed].range(num_subjets_LRJ2_high_, num_subjets_LRJ2_low_) = numSubjets[1];
        outputJetValues[iSeed].range(num_subjets_LRJ1_high_, num_subjets_LRJ1_low_) = numSubjets[0]; 
        outputJetValues[iSeed].range(et_high_, et_low_) = outputJetEt;
        outputJetValues[iSeed].range(eta_high_, eta_low_) = seedValues[iSeed].range(eta_high_, eta_low_);
        outputJetValues[iSeed].range(phi_high_, phi_low_) = seedValues[iSeed].range(phi_high_, phi_low_);
        
        //std::cout << "number of merged input objects for seed: "  << std::dec << iSeed << " is : " << std::dec << numMergedIO << "\n";
    }
}