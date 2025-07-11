// Still figuring out the proper space-time tradeoff.

#include "process_event.h"
#define WRITE_LUT 0

/*
=================================================================================
=================================================================================
ODD-EVEN SORTING
=================================================================================
=================================================================================
*/

void odd_even_sort(ap_uint<et_bit_length_> data[nSeeds_]) {
    #pragma HLS PIPELINE II=1

    for (int i = 0; i < nSeeds_; i++) {
        #pragma HLS UNROLL
        for (int j = (i % 2); j < nSeeds_ - 1; j += 2) {
            #pragma HLS UNROLL
            if (data[j] > data[j + 1]) {
                int tmp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tmp;
            }
        }
    }

    
}

/*
=================================================================================
=================================================================================
ARRAY SUM (ATTEMPT TO LOCALIZE ARRAY TO AVOID ACCUMULATION DEPENDENCY)
=================================================================================
=================================================================================
*/

void array_sum(ap_uint<et_bit_length_> partialSums[maxObjectsConsidered_], ap_uint<et_bit_length_> sum) {
    ap_uint<et_bit_length_> temp = 0;
    for (unsigned int i = 0; i < maxObjectsConsidered_; i++) {
        #pragma HLS UNROLL
        temp += partialSums[i];
    }

    sum += temp;
}

/*
=================================================================================
=================================================================================
ET CALCULATION (TAKES INTO ACCOUNT IF FILTERING IS ENABLED)
=================================================================================
=================================================================================
*/

void et_calculation(bool filtering, input filterArray[maxObjectsConsidered_], input inputArray[maxObjectsConsidered_], unsigned int numSeed, input seedValues[nTotalSeeds_], input outputArray[nTotalSeeds_]) {
    if (filtering) { // Filter, turns overlapped values to zero; need to confirm this is what we want
        bool bitmask[1 << et_bit_length_];
        for (unsigned int i = 0; i  < 1 << et_bit_length_; i++) {
            #pragma HLS PIPILINE II=1
            bitmask[i] = false;
        }

        for (unsigned int iInput = 0; iInput < maxObjectsConsidered_; iInput++) {
            #pragma HLS UNROLL
            bitmask[filterArray[iInput]] = true;
        }

        for (unsigned int iInput = 0; iInput < maxObjectsConsidered_; iInput++) {
            #pragma HLS UNROLL
            if (bitmask[inputArray[iInput]]) {
                inputArray[iInput] = 0;
            }
        }
    }

    // Actual Et calculation

    auto seed = seedValues[numSeed];
    unsigned int numMergedIO = 0;
    ap_uint<et_bit_length_> partialSums[maxObjectsConsidered_];
    outputArray[numSeed] = 0;

    for (unsigned int iInput = 0; iInput < maxObjectsConsidered_; iInput++) {
        #pragma HLS UNROLL
        auto inputVal = inputArray[iInput];

        #if useInputEnergyCut_
            if (inputVal.range(et_high_, et_low_) >= inputEnergyCut_) continue; 
        #endif

        ap_int<eta_bit_length_ + 1> deltaEta = seed.range(et_bit_length_ + eta_bit_length_ - 1, et_bit_length_) - inputArray[iInput].range(et_bit_length_ + eta_bit_length_ - 1, et_bit_length_);
        ap_int<phi_bit_length_ + 1> deltaPhi = seed.range(total_bits - 1, total_bits - phi_bit_length_) - inputArray[iInput].range(total_bits - 1, total_bits - phi_bit_length_);

        ap_uint<eta_bit_length_> uDeltaEta = deltaEta[eta_bit_length_] ? static_cast<ap_uint<eta_bit_length_>>( -deltaEta ) : static_cast<ap_uint<eta_bit_length_>>( deltaEta );
        ap_uint<phi_bit_length_> uDeltaPhi = deltaPhi[phi_bit_length_] ? static_cast<ap_uint<phi_bit_length_>>( -deltaPhi ) : static_cast<ap_uint<phi_bit_length_>>( deltaPhi );

        unsigned int lut_index = uDeltaEta * (1 << phi_bit_length_) + uDeltaPhi; 
        if (!(lut_index >= max_lut_size_) && lut_[lut_index]){ 
            partialSums[iInput] = inputVal.range(et_max_, et_min_); 
            numMergedIO++; 
        }
    }

    ap_uint<et_bit_length_> outputJetEt = seed.range(et_max_, et_min_);
    array_sum(partialSums, outputJetEt);

    // FOR IMPLEMENTATION ONLY
    // outputArray[numSeed].range(io_max_, io_min_) = numMergedIO / 2; 
    outputArray[numSeed].range(et_max_, et_min_) = outputJetEt;
    outputArray[numSeed].range(eta_max_, eta_min_) = seed.range(eta_max_, eta_min_);
    outputArray[numSeed].range(phi_max_, phi_min_) = seed.range(phi_max_, phi_min_);
}

/*
=================================================================================
=================================================================================
TOP FUNCTION
=================================================================================
=================================================================================
*/

void top(input seedValues[nTotalSeeds_], input inputObjectValues[nTotalSeeds_][maxObjectsConsidered_], input outputJetValues[nSeeds_]) {
    // Pragmas
    #pragma HLS ARRAY_PARTITION variable=lut_ cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=inputObjectValues complete dim=1 
    #pragma HLS ARRAY_PARTITION variable=inputObjectValues cyclic factor=4 dim=2 
    #pragma HLS ARRAY_PARTITION variable=outputJetValues complete
    #pragma HLS ARRAY_PARTITION variable=seedValues complete 

    // First step: Calculating all of the Et's pre-filter
    for (unsigned int iSeed = 0; iSeed < nSeeds_; iSeed++) {
        #pragma HLS UNROLL
        ap_uint<et_bit_length_ + eta_bit_length_ + phi_bit_length_> dummy[maxObjectsConsidered_] = {0};
        et_calculation(false, dummy, inputObjectValues[iSeed], iSeed, seedValues, outputJetValues);
    }

    // Second step: Sorting
    ap_uint<et_bit_length_> tempArray[nSeeds_];
    for (int i = 0; i < nSeeds_; i++) {
        #pragma HLS UNROLL
        tempArray[i] = outputJetValues[i];
    }
    odd_even_sort(tempArray);
    int filterVal = tempArray[0];
    int filterIndex = 0;

    for (unsigned int iSeed = 0; iSeed < nSeeds_; iSeed++) { // Might cause problems when unrolled
        #pragma HLS UNROLL
        if (filterVal == outputJetValues[iSeed]) {
            filterIndex = iSeed;
        }
    }

    // Third step: Calculating all of the Et's post-filter
    for (unsigned int iSeed = 0; iSeed < nSeeds_; iSeed++) {
        #pragma HLS UNROLL
        if (iSeed != filterIndex) {
            et_calculation(true, inputObjectValues[filterIndex], inputObjectValues[iSeed], iSeed, seedValues, outputJetValues);
        }
    }
}
