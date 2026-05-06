#include "jet_tag_adv.h"
#define WRITE_LUT 0

// Main function
void jet_tag_adv(input seedValues[nTotalSeeds_], input inputObjectValues[maxObjectsConsidered_], output (&outputJetValues)[nSeedsOutput_]){ // FIXME potentially use templated / overloaded func to deal with whether write out files while running synth or c-sim
    // Pragma for partitioning (allowing simultaneous access to) LUT array
    #pragma HLS ARRAY_PARTITION variable=seedValues complete
    #pragma HLS ARRAY_PARTITION variable=inputObjectValues complete
    #pragma HLS ARRAY_PARTITION variable=outputJetValues complete
    // PRAGMAS FOR WRITING DATA TO FPGA BRAMS (TESTING IMPLEMENTATION ONLY)
    // AXI4-Master interfaces for input arrays
    //#pragma HLS INTERFACE m_axi port=seedValues        bundle=gmem0 offset=slave depth=nSeedsInput_
    //#pragma HLS INTERFACE m_axi port=inputObjectValues bundle=gmem1 offset=slave depth=maxObjectsConsidered_
    // AXI4-Master interfaces for output arrays
    //#pragma HLS INTERFACE m_axi port=outputJetValues   bundle=gmem2 offset=slave depth=nSeedsOutput_
    // AXI4-Lite interface only for control signals (function arguments, etc.)
    //#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
    /*std::cout << "--------------------" << "\n";
    std::cout << "constants: "<< "\n";
    std::cout << "deltaR2_granularity_: " << deltaR2_granularity_ << "\n";
    std::cout << "digitized_delta_R2Cut_: " << digitized_delta_R2Cut_ << "\n";
    std::cout << "r2 cut: " << r2Cut_ << "\n";*/
    for (unsigned int i = 0; i < nSeedsOutput_; ++i)
        #pragma HLS unroll
        outputJetValues[i] = 0;

    input seedValuesForSubjets[nTotalSeeds_];
    #pragma HLS ARRAY_PARTITION variable=seedValuesForSubjets complete
    for (unsigned int i = 0; i < nTotalSeeds_; ++i){
        #pragma HLS unroll
        seedValuesForSubjets[i] = seedValues[i];
    } 
    
    // Perform overlap removal (OR) ensuring that leading, subleading seeds don't overlap within deltaR < 2 * jet radius
    ap_int<eta_bit_length_ + 1> deltaEta_LeadSublSeeds = seedValues[0].range(eta_high_, eta_low_) - seedValues[1].range(eta_high_, eta_low_);
    ap_int<phi_bit_length_ + 1> deltaPhi_LeadSublSeeds = seedValues[0].range(phi_high_, phi_low_) - seedValues[1].range(phi_high_, phi_low_);
    ap_uint<eta_bit_length_ > uDeltaEta_LeadSublSeeds = deltaEta_LeadSublSeeds[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEta_LeadSublSeeds ) : static_cast<ap_uint<eta_bit_length_>>( deltaEta_LeadSublSeeds );
    ap_uint<phi_bit_length_ > uDeltaPhi_LeadSublSeeds = deltaPhi_LeadSublSeeds[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhi_LeadSublSeeds ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhi_LeadSublSeeds );
    if (uDeltaPhi_LeadSublSeeds >= pi_digitized_in_phi_) uDeltaPhi_LeadSublSeeds = 2 * pi_digitized_in_phi_ - uDeltaPhi_LeadSublSeeds;
    ap_uint<2*eta_bit_length_>     etaSq_LS = uDeltaEta_LeadSublSeeds * uDeltaEta_LeadSublSeeds;
    #pragma HLS bind_op variable=etaSq_LS op=mul impl=dsp
    ap_uint<2*(phi_bit_length_-1)> phiSq_LS = ap_uint<phi_bit_length_-1>(uDeltaPhi_LeadSublSeeds) * ap_uint<phi_bit_length_-1>(uDeltaPhi_LeadSublSeeds);
    #pragma HLS bind_op variable=phiSq_LS op=mul impl=dsp
    ap_uint<2*eta_bit_length_+1> rawSum_LS = ap_uint<2*eta_bit_length_+1>(etaSq_LS) + phiSq_LS;
    ap_uint<deltaR2_bits_> deltaR2LeadingSubleading = rawSum_LS > ap_uint<deltaR2_bits_>((1<<deltaR2_bits_)-1)
                                                      ? ap_uint<deltaR2_bits_>((1<<deltaR2_bits_)-1)
                                                      : ap_uint<deltaR2_bits_>(rawSum_LS);

    //std::cout << "seed 1 OR eta: " << seedValues[0].range(eta_high_, eta_low_) << " , phi: " << seedValues[0].range(phi_high_, phi_low_) << "\n";
    //std::cout << "seed 2 OR eta: " << seedValues[1].range(eta_high_, eta_low_) << " , phi: " << seedValues[1].range(phi_high_, phi_low_) << "\n";
    //std::cout << "deltaR2leadingsubleading: " << deltaR2LeadingSubleading << "\n";
    //std::cout << "deltaR^2 cut: " << 2 * 2 * digitized_delta_R2Cut_ << "\n";
    if(deltaR2LeadingSubleading <= 2 * 2 * digitized_delta_R2Cut_){ // (2 * R_Cut) ^ 2

        for(unsigned int iSeedOR = nSeedsOutput_; iSeedOR < nTotalSeeds_; iSeedOR++){
            #pragma HLS unroll
            //std::cout << "iSeed OR : "<< iSeedOR << "\n";
            if(seedValues[iSeedOR].range(et_high_, et_low_) == 0 && seedValues[iSeedOR].range(eta_high_, eta_low_) == 0 && seedValues[iSeedOR].range(phi_high_, phi_low_) == 0) continue; // don't consider if et, eta, phi all = 0 (no jet)
            ap_int<eta_bit_length_ + 1> deltaEtaNthLeading = seedValues[0].range(eta_high_, eta_low_) - seedValues[iSeedOR].range(eta_high_, eta_low_);
            ap_int<phi_bit_length_ + 1> deltaPhiNthLeading = seedValues[0].range(phi_high_, phi_low_) - seedValues[iSeedOR].range(phi_high_, phi_low_);
            ap_uint<eta_bit_length_ > uDeltaEtaNthLeading = deltaEtaNthLeading[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEtaNthLeading ) : static_cast<ap_uint<eta_bit_length_>>( deltaEtaNthLeading );
            ap_uint<phi_bit_length_ > uDeltaPhiNthLeading = deltaPhiNthLeading[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhiNthLeading ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhiNthLeading );
            if (uDeltaPhiNthLeading >= pi_digitized_in_phi_) uDeltaPhiNthLeading = 2 * pi_digitized_in_phi_ - uDeltaPhiNthLeading;
            ap_uint<2*eta_bit_length_>     etaSq_NL = uDeltaEtaNthLeading * uDeltaEtaNthLeading;
            #pragma HLS bind_op variable=etaSq_NL op=mul impl=dsp
            ap_uint<2*(phi_bit_length_-1)> phiSq_NL = ap_uint<phi_bit_length_-1>(uDeltaPhiNthLeading) * ap_uint<phi_bit_length_-1>(uDeltaPhiNthLeading);
            #pragma HLS bind_op variable=phiSq_NL op=mul impl=dsp
            ap_uint<2*eta_bit_length_+1> rawSum_NL = ap_uint<2*eta_bit_length_+1>(etaSq_NL) + phiSq_NL;
            ap_uint<deltaR2_bits_> deltaR2LeadingSubleadingNthLeading = rawSum_NL > ap_uint<deltaR2_bits_>((1<<deltaR2_bits_)-1)
                                                                         ? ap_uint<deltaR2_bits_>((1<<deltaR2_bits_)-1)
                                                                         : ap_uint<deltaR2_bits_>(rawSum_NL);
            //std::cout << "deltaR2 Nth leading: " << deltaR2LeadingSubleadingNthLeading << "\n";
            if(deltaR2LeadingSubleadingNthLeading > 2 * 2 * digitized_delta_R2Cut_){
                //std::cout << "triggering OR for iSeed OR : "<< iSeedOR << "\n";
                ap_uint<et_bit_length_ > swappedEt = seedValues[iSeedOR].range(et_high_, et_low_);
                ap_uint<eta_bit_length_ > swappedEta = seedValues[iSeedOR].range(eta_high_, eta_low_);
                ap_uint<phi_bit_length_ > swappedPhi = seedValues[iSeedOR].range(phi_high_, phi_low_);

                ap_uint<et_bit_length_ > originalSubleadingEt = seedValues[1].range(et_high_, et_low_);
                ap_uint<eta_bit_length_ > originalSubleadingEta = seedValues[1].range(eta_high_, eta_low_);
                ap_uint<phi_bit_length_ > originalSubleadingPhi = seedValues[1].range(phi_high_, phi_low_);

                // Swap the entire (Et, eta, phi) triplet for original subleading, new subleading seed
                seedValues[1].range(et_high_, et_low_) = swappedEt; // Et
                seedValues[1].range(eta_high_, eta_low_) = swappedEta; // eta
                seedValues[1].range(phi_high_, phi_low_) = swappedPhi; // phi

                seedValues[iSeedOR].range(et_high_, et_low_) = originalSubleadingEt; // Et
                seedValues[iSeedOR].range(eta_high_, eta_low_) = originalSubleadingEta; // eta
                seedValues[iSeedOR].range(phi_high_, phi_low_) = originalSubleadingPhi; // phi

                break; // break out of loop as we've found something to swap, thus preventing overlapping large-R jets
            } // If deltaR^2 between original leading, other proto-seed farther than 2 times jet radius 
        } // Loop through potential additional seeds to find 
    } // If deltaR^2 between original leading, subleading closer than 2 times jet radius
    
    
     // FIXME make this entire process more dynamic to account for nSeedsOutput_ != 2 (progressively do this for highest Et seeds rather than for 1st 2 seeds immediately)
    // NEW PRE-PROCESSING OF SEEDS - SELECT IN BETWEEN LEADING, SUBLEADING JFEX SRJ, CLOSEST OF 3rd - 6th highest ENERGY JFEX SRJS as NEW SEEDS
    ap_uint<4> protoSeedCounter[nSeedsOutput_] = {0, 0};
        
    bool indicesofProtoSeeds[nSeedsOutput_][nSeedsDeltaR_] = {false};
    ap_int<4> indices[2] = {-1, -1};
    #pragma HLS bind_storage variable=indicesofProtoSeeds type=RAM_1P impl=lutram
    //std::cout << "Seed 1 Eta, Phi: " << seedValues[0].range(eta_high_, eta_low_) << " , " << seedValues[0].range(phi_high_, phi_low_) << "\n";
    //std::cout << "Seed 2 Eta, Phi: " << seedValues[1].range(eta_high_, eta_low_) << " , " << seedValues[1].range(phi_high_, phi_low_) << "\n";
    //std::cout << " ----------------- SEED POS OPT -----------------" << "\n";
    for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; iSeed++){ // loop through and calculate deltaR between leading, subleading & 3rd - 6th highest Et JFEX SRJ
        #pragma HLS unroll
        
        //std::cout << "iSeed: " << iSeed << "\n";
        for (unsigned int iPreSeed = 0; iPreSeed < nSeedsDeltaR_; iPreSeed++){
            #pragma HLS unroll
            //std::cout << "iPreSeed: " << iPreSeed << "\n";
            //std::cout << "seed eta: " << seedValues[iSeed].range(eta_high_, eta_low_) << " , phi: " << seedValues[iSeed].range(phi_high_, phi_low_) << " , et: " << seedValues[iSeed].range(et_high_, et_low_) << "\n";
            //std::cout << "pre seed eta: " << seedValues[iPreSeed + nSeedsOutput_].range(eta_high_, eta_low_) << " , phi: " << seedValues[iPreSeed + nSeedsOutput_].range(phi_high_, phi_low_) << " , et: " << seedValues[iPreSeed + nSeedsOutput_].range(et_high_, et_low_) << "\n";
            if(seedValues[iPreSeed + nSeedsOutput_].range(et_high_, et_low_) == 0) continue;
            ap_int<eta_bit_length_ + 1> deltaEta = seedValues[iSeed].range(eta_high_, eta_low_) - seedValues[iPreSeed + nSeedsOutput_].range(eta_high_, eta_low_);
            ap_int<phi_bit_length_ + 1> deltaPhi = seedValues[iSeed].range(phi_high_, phi_low_) - seedValues[iPreSeed + nSeedsOutput_].range(phi_high_, phi_low_);
            // Use unsigned type for absolute values, and ensure both operands are of the same type
            ap_uint<eta_bit_length_> uDeltaEta = deltaEta[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEta ) : static_cast<ap_uint<eta_bit_length_>>( deltaEta );
            ap_uint<phi_bit_length_> uDeltaPhi = deltaPhi[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhi ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhi );
            if (uDeltaPhi >= pi_digitized_in_phi_) uDeltaPhi = 2 * pi_digitized_in_phi_ - uDeltaPhi;
            ap_uint<phi_bit_length_ - 1> corrDeltaPhi = uDeltaPhi; // using corr delta phi saves 1 bit, unsure if necessary?
            ap_uint<2*eta_bit_length_>     etaSq   = uDeltaEta   * uDeltaEta;
            #pragma HLS bind_op variable=etaSq   op=mul impl=dsp
            ap_uint<2*(phi_bit_length_-1)> phiSq   = corrDeltaPhi * corrDeltaPhi;
            #pragma HLS bind_op variable=phiSq   op=mul impl=dsp
            ap_uint<2*eta_bit_length_+1>   rawSum  = ap_uint<2*eta_bit_length_+1>(etaSq) + phiSq;
            ap_uint<deltaR2_bits_>         deltaR2 = rawSum > ap_uint<deltaR2_bits_>((1<<deltaR2_bits_)-1)
                                                      ? ap_uint<deltaR2_bits_>((1<<deltaR2_bits_)-1)
                                                      : ap_uint<deltaR2_bits_>(rawSum);
            //std::cout << "deltaR2: " << deltaR2 << " , digitized_d_search_squared_: " << digitized_d_search_squared_ << "\n";
            if (deltaR2 <= digitized_d_search_squared_){
                protoSeedCounter[iSeed]++;
                indicesofProtoSeeds[iSeed][iPreSeed] = true;
                indices[iSeed] = iPreSeed;
                //std::cout << "setting indices to: " << iPreSeed << " , -> indices[iSeed]: " << indices[iSeed] << "\n";
            }
        }
        //std::cout << "proto seed coutner[iseed]: " <<  protoSeedCounter[iSeed] << "\n";
    }
    
    // Account for the case when multiple seeds are within deltaR customalizable value - use highest Et seed to compute halfway point
    // For seed 0
    if (protoSeedCounter[0] > 1) {
        //std::cout << "mnultiple proto seeds [seed 0]:" << "\n";
        ap_uint<et_bit_length_> maxEt = 0;
        for (unsigned int iPreSeed = 0; iPreSeed < nSeedsDeltaR_; iPreSeed++) {
            #pragma HLS unroll
            if (indicesofProtoSeeds[0][iPreSeed]) {
                ap_uint<et_bit_length_> et = seedValues[iPreSeed + nSeedsOutput_].range(et_high_, et_low_);
                if (et > maxEt) {
                    maxEt = et;
                    indices[0] = iPreSeed;
                }
            }
        }
    }

    // For seed 1
    if (protoSeedCounter[1] > 1) {
        //std::cout << "mnultiple proto seeds [seed 1]:" << "\n";
        ap_uint<et_bit_length_> maxEt = 0;
        for (unsigned int iPreSeed = 0; iPreSeed < nSeedsDeltaR_; iPreSeed++) {
            #pragma HLS unroll
            if (indicesofProtoSeeds[1][iPreSeed]) {
                ap_uint<et_bit_length_> et = seedValues[iPreSeed + nSeedsOutput_].range(et_high_, et_low_);
                if (et > maxEt) {
                    maxEt = et;
                    indices[1] = iPreSeed;
                }
            }
        }
    }

    // FIXME next do bitonic sorting of deltaR2ValuesSeed, get index back to original seedValues array corresponding to closest to each seed (figure out what to do if they are same)
    // FIXME replace with a templated version of this s.t. you don't need to rewrite code for different number sorted
    // Printouts for debugging seed position recalculation
    /*if(deltaR2ValuesSeed[0][index_of_closest_seed1] <= digitized_d_search_squared_){
       std::cout << "index of closest seed 1: " << index_of_closest_seed1 << " and delta R of closest seed 1: " << deltaR2ValuesSeed[0][index_of_closest_seed1] * deltaR_step_ << " and energy of closest seed 1: " << seedValues[indices[0] + nSeedsOutput_].range(et_high_, et_low_) << "\n";
    }
    else{
       std::cout << "first seed unchanged" << "\n";
    }
    if(deltaR2ValuesSeed[1][index_of_closest_seed1] <= digitized_d_search_squared_){
       std::cout << "index of closest seed 2: " << index_of_closest_seed2 << " and delta R of closest seed 2: " << deltaR2ValuesSeed[1][index_of_closest_seed2] * deltaR_step_ << " and energy of closest seed 2: " << seedValues[indices[0] + nSeedsOutput_].range(et_high_, et_low_) << "\n";
    }
    else{
       std::cout << "second seed unchanged" << "\n";
    }*/
    
    // Account for the case when both original seeds would be shifted toward the same pre seeds, 
    // this would cause unnecessary overlap - this should be better thought out, 
    // but for now just prevent lower E_T seed from being shifted at all
    //std::cout << "indices[0]: " << indices[0] << "\n";
    //std::cout << "indices[1]: " << indices[1] << "\n";
    bool skipSecondSeed = false;
    if(indices[0] == indices[1] && indices[0] != -1){ // FIXME implement in HLS
        skipSecondSeed = true;
    }
    // next update leading, subleading seed positions (and energy?) to be in between closest other seed 
    for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; iSeed++){
        #pragma HLS unroll
        //std::cout << "iSeed: " << iSeed << " , skipSecondSeed: " << skipSecondSeed << "\n";
        if(skipSecondSeed == true && iSeed == 1) continue;
        if(indices[iSeed] == -1) continue;
        
        //std::cout << "-------------- calcing mid point -----------------" << "\n"; 
        //std::cout << "iSeed: " << iSeed << "\n";
        //fflush(stdout);

        const ap_int<phi_bit_length_ + 2> PI_D     =  ap_int<phi_bit_length_ + 2>(pi_digitized_in_phi_);
        const ap_int<phi_bit_length_ + 2> TWO_PI_D =  ap_int<phi_bit_length_ + 2>((1 << phi_bit_length_) - 1);

        //std::cout << "pi_digitized_in_phi_: " << pi_digitized_in_phi_ << "\n";
        //std::cout << "PI_D: " << PI_D  << " , TWO_PI_D: " << TWO_PI_D << "\n";

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


        // --- Compute midpoint in signed domain ---
        ap_int<eta_bit_length_ + 1> eta_mid = (eta1 + eta2) >> 1; // Integer division by 2
        //ap_int<phi_bit_length_ + 1> phi_mid = (phi1 + phi2) >> 1;
        //ap_int<eta_bit_length_ + 3> eta_mid_weighted = (et2 * eta2 + et1 * eta1) / etSum;
        //ap_int<phi_bit_length_ + 3> phi_mid_weighted = (et2 * phi2 + et1 * phi1) / etSum;

       //std::cout << "eta 1 : " << eta1 << " eta 2 : " << eta2 << "\n";
       //std::cout << "phi1 : " << phi1 << " phi 2  : " << phi2 << "\n";
        
        // --- Optional: re-wrap phi midpoint to [-π, π) range if needed ---

        //if (phi_mid >= pi_digitized_in_phi_) 
        //    phi_mid -= (1 << phi_bit_length_);
        //if (phi_mid < -(pi_digitized_in_phi_ + 1)) 
        //    phi_mid += (1 << phi_bit_length_);
        
        //if (phi_mid_weighted >= pi_digitized_in_phi_)
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

    // must also account for when deltaR2ValuesSeed not set (no deltaR < 2.5 --> leave seed position as is)
    
    for (unsigned int iSeed = 0; iSeed < nSeedsOutput_; ++iSeed){ 
        #pragma HLS unroll
        ap_uint<et_bit_length_ > outputJetEt = 0;
        ap_uint<num_subjets_length_ > numSubjets = 0;
        
        //std::cout << "----------" << "\n";
        //std::cout << "iSeed: " << iSeed << "\n";
        //std::cout << "ORIGINAL seed eta: " << seedValuesForSubjets[iSeed].range(eta_high_, eta_low_) << " , phi: " << seedValuesForSubjets[iSeed].range(phi_high_, phi_low_) << " , et: " << seedValuesForSubjets[iSeed].range(et_high_, et_low_) << "\n";
        //std::cout << "seed eta: " << seedValues[iSeed].range(eta_high_, eta_low_) << " , phi: " << seedValues[iSeed].range(phi_high_, phi_low_) << " , et: " << seedValues[iSeed].range(et_high_, et_low_) << "\n";

        // NOTE: since seedValues != original seedValues after overlap removal & seed optimization, 
        // only do subjet finding in separate loop comparing the seedValues eta, phi & originalSeedValues eta, phi
        /*if(seedValues[iSeed].range(et_high_, et_low_) > subjet_et_threshold_){ // > 25 GeV nominally
            if(numSubjets < ((1 << num_subjets_length_) - 1)){ // Clamp number of subjets to maximum value
                numSubjets++;
            }
            else{
                numSubjets = ((1 << num_subjets_length_) - 1);
            }
        }*/
        if(seedValues[iSeed].range(et_high_, et_low_) != 0){
            for (unsigned int iInput = 0; iInput < maxObjectsConsidered_; ++iInput){ // loop through input objects to consider merging
                #pragma HLS unroll
                //std::cout << "iInput: " << iInput << "\n";
                //std::cout << "input eta: " << inputObjectValues[iInput].range(eta_high_, eta_low_) << " , phi: " << inputObjectValues[iInput].range(phi_high_, phi_low_) << " , et: " << inputObjectValues[iInput].range(et_high_, et_low_) << "\n";
                // Calculate signed differences (deltaEta and deltaPhi)
                ap_int<eta_bit_length_ + 1> deltaEta = seedValues[iSeed].range(eta_high_, eta_low_) - inputObjectValues[iInput].range(eta_high_, eta_low_);
                ap_int<phi_bit_length_ + 1> deltaPhi = seedValues[iSeed].range(phi_high_, phi_low_) - inputObjectValues[iInput].range(phi_high_, phi_low_);
                
                // Use unsigned type for absolute values, and ensure both operands are of the same type
                ap_uint<eta_bit_length_> uDeltaEta = deltaEta[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEta ) : static_cast<ap_uint<eta_bit_length_>>( deltaEta );
                ap_uint<phi_bit_length_> uDeltaPhi = deltaPhi[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhi ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhi );

                if (uDeltaPhi >= pi_digitized_in_phi_) uDeltaPhi = 2 * pi_digitized_in_phi_ - uDeltaPhi;
                ap_uint<phi_bit_length_ - 1> corrDeltaPhi = uDeltaPhi; // using corr delta phi saves 1 bit, unsure if necessary?

                ap_uint<2*eta_bit_length_>     etaSq_IO = uDeltaEta   * uDeltaEta;
                #pragma HLS bind_op variable=etaSq_IO op=mul impl=dsp
                ap_uint<2*(phi_bit_length_-1)> phiSq_IO = corrDeltaPhi * corrDeltaPhi;
                #pragma HLS bind_op variable=phiSq_IO op=mul impl=dsp
                ap_uint<2*eta_bit_length_+1>   rawSum_IO = ap_uint<2*eta_bit_length_+1>(etaSq_IO) + phiSq_IO;
                ap_uint<deltaR2_bits_>         deltaR2   = rawSum_IO > ap_uint<deltaR2_bits_>((1<<deltaR2_bits_)-1)
                                                            ? ap_uint<deltaR2_bits_>((1<<deltaR2_bits_)-1)
                                                            : ap_uint<deltaR2_bits_>(rawSum_IO);
                //std::cout << " corrDeltaPhi : " << corrDeltaPhi << "\n";
                //std::cout << "deltaR^2: " << uDeltaEta * uDeltaEta * eta_granularity_ * eta_granularity_ + corrDeltaPhi * corrDeltaPhi * phi_granularity_ * phi_granularity_ << "\n";
                //int phiLength = 1 << (phi_bit_length_);
                //std::cout << "phiLength: " << phiLength << " and uDeltaEta * philength: " << uDeltaEta * (1 << phi_bit_length_) << "\n";
                //ap_uint<eta_bit_length_ + phi_bit_length_ + 2 > lut_index = (uDeltaEta << (phi_bit_length_ - 1)) | corrDeltaPhi; // Calculate LUT index corresponding to whether input object passes R^2 cut
                //std::cout << "lut_index: " << std::dec << lut_index << "\n";
                //std::cout << "deltaR2 : " << deltaR2 << " , digitized deltaR2 cut: " << digitized_delta_R2Cut_ << "\n";
                if (deltaR2 <= digitized_delta_R2Cut_){ // only consider if lut index is smaller than max size (past max size, all values are False) // FIXME will throw an error for deltaR!=1.0
                    if(outputJetEt + inputObjectValues[iInput][0] >= ((1 << et_bit_length_) - 1)){
                        outputJetEt = ((1 << et_bit_length_) - 1); // if would exceed max Et, set equal to max Et 
                    } 
                    else{
                        outputJetEt += inputObjectValues[iInput].range(et_high_, et_low_); // add input object Et to seed Et for resultant output jet Et
                    }
                    //std::cout << "outputJetEt after sum: " << outputJetEt << "\n";

                }
            }
            
            for(unsigned int iSubjet = 0; iSubjet < nTotalSeeds_; iSubjet++){
                #pragma HLS unroll 
                //std::cout << "iSubjet: " << iSubjet << "\n";
                //std::cout << "seedValuesForSubjets[iSubjet].range(et_high_, et_low_): " << seedValuesForSubjets[iSubjet].range(et_high_, et_low_) << " , subjet_et_threshold_: " << subjet_et_threshold_ << "\n";
                if(seedValuesForSubjets[iSubjet].range(et_high_, et_low_) > subjet_et_threshold_){ // > 25 GeV nominally
                    //std::cout << " passes et threshold" << "\n";
                    // Calculate signed differences (deltaEta and deltaPhi)
                    ap_int<eta_bit_length_ + 1> deltaEta = seedValues[iSeed].range(eta_high_, eta_low_) - seedValuesForSubjets[iSubjet].range(eta_high_, eta_low_);
                    ap_int<phi_bit_length_ + 1> deltaPhi = seedValues[iSeed].range(phi_high_, phi_low_) - seedValuesForSubjets[iSubjet].range(phi_high_, phi_low_);
                    
                    // Use unsigned type for absolute values, and ensure both operands are of the same type
                    ap_uint<eta_bit_length_> uDeltaEta = deltaEta[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEta ) : static_cast<ap_uint<eta_bit_length_>>( deltaEta );
                    ap_uint<phi_bit_length_> uDeltaPhi = deltaPhi[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhi ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhi );

                    if (uDeltaPhi >= pi_digitized_in_phi_) uDeltaPhi = 2 * pi_digitized_in_phi_ - uDeltaPhi;
                    ap_uint<phi_bit_length_ - 1> corrDeltaPhi = uDeltaPhi; 

                    ap_uint<2*eta_bit_length_>     etaSq_SJ = uDeltaEta   * uDeltaEta;
                    #pragma HLS bind_op variable=etaSq_SJ op=mul impl=dsp
                    ap_uint<2*(phi_bit_length_-1)> phiSq_SJ = corrDeltaPhi * corrDeltaPhi;
                    #pragma HLS bind_op variable=phiSq_SJ op=mul impl=dsp
                    ap_uint<2*eta_bit_length_+1>   rawSum_SJ = ap_uint<2*eta_bit_length_+1>(etaSq_SJ) + phiSq_SJ;
                    ap_uint<deltaR2_bits_>         deltaR2   = rawSum_SJ > ap_uint<deltaR2_bits_>((1<<deltaR2_bits_)-1)
                                                                ? ap_uint<deltaR2_bits_>((1<<deltaR2_bits_)-1)
                                                                : ap_uint<deltaR2_bits_>(rawSum_SJ);
                    //ap_uint<eta_bit_length_ + phi_bit_length_ + 2 > lut_index = (uDeltaEta << (phi_bit_length_ - 1)) | corrDeltaPhi; // Calculate LUT index corresponding to whether input object passes R^2 cut
                    //if (!(lut_index >= max_R2lut_size_) && lut_[lut_index]){ // only consider if lut index is smaller than max size (past max size, all values are False) // FIXME will throw an error for deltaR!=1.0
                    //std::cout << "deltaR2: " << deltaR2 << " , delta_R2Cut_: " << digitized_delta_R2Cut_ << "\n";
                    if (deltaR2 <= digitized_delta_R2Cut_){
                        //std::cout << "FOUND SUBJET" << "\n";
                        numSubjets = (numSubjets < ap_uint<num_subjets_length_>((1 << num_subjets_length_) - 1))
                                    ? ap_uint<num_subjets_length_>(numSubjets + 1)
                                    : ap_uint<num_subjets_length_>((1 << num_subjets_length_) - 1);
                    }
                }
            }
        } 
        
        //std::cout << "numSubjets: " << numSubjets << "\n";
        outputJetValues[iSeed].range(padded_zeroes_high_, padded_zeroes_low_) = 0; 
        outputJetValues[iSeed].range(num_subjets_high_, num_subjets_low_) = numSubjets;
        outputJetValues[iSeed].range(et_high_, et_low_) = outputJetEt;
        outputJetValues[iSeed].range(eta_high_, eta_low_) = seedValues[iSeed].range(eta_high_, eta_low_);
        outputJetValues[iSeed].range(phi_high_, phi_low_) = seedValues[iSeed].range(phi_high_, phi_low_);
        
        //std::cout << "number of merged input objects for seed: "  << std::dec << iSeed << " is : " << std::dec << numMergedIO << "\n";
    }
}