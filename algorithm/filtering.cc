#include "process_event.h"

void top (ap_uint<13> inputArray[maxObjectsConsidered_], ap_uint<13> filterArray[maxObjectsConsidered_], ap_uint<4096> bitmask1, ap_uint<4096> bitmask2) { // ap_uint doesn't seem to support higher than 8192 bits? so we're splitting the possible Et values (8192) into two words
    #pragma HLS ARRAY_PARTITION variable=inputArray complete
    #pragma HLS ARRAY_PARTITION variable=filterArray complete
    
    // these words are supposed to be arrays, but it's faster to use ap_uint
    bitmask1 = 0;
    bitmask2 = 0;

    // checks filter array and makes the bitmask index to 1 (e.g. an Et value of 1000 in the filterArray will make the 1000th bit HIGH for bitmask1, 4096 will make the 1st bit HIGH for bitmask 2, etc)
    loop1: for (unsigned int iInput = 0; iInput < maxObjectsConsidered_; iInput++) { // unrolling for more than 2 seems to bottleneck
        int temp = filterArray[iInput];
        if (temp < 4096) {
            bitmask1 |= (ap_uint<4096>(1) << temp);
        } else {
            bitmask2 |= (ap_uint<4096>(1) << (temp - 4096));
        }
    }

    // for each element of inputArray, if its value (when turned into the filterArray index) is HIGH, then make it zero
    loop2: for (unsigned int iInput = 0; iInput < maxObjectsConsidered_; iInput++) {
        int temp = inputArray[iInput];
        if (temp < 4096) {
            if (bitmask1.test(temp)) {
                inputArray[iInput] = 0;
            }
        } else {
            if (bitmask2.test(temp - 4096)) {
                inputArray[iInput] = 0;
            }
        }
    }
}