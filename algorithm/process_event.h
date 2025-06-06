#ifndef _PROCESS_EVENT_H_
#define _PROCESS_EVENT_H_
#include "/home/larsonma/LargeRadiusJets/algorithm/helperFunctions.h"
#include "/home/larsonma/LargeRadiusJets/algorithm/constants.h"
#include <iostream>
#include <array>
#include <bitset>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <cmath>
//#include "helperFunctions.h"

// Main function
void process_event(input seedValues[nTotalSeeds_], input inputObjectValues[maxObjectsConsidered_], input outputJetValues[nSeeds_]); // FIXME potentially use templated / overloaded func to deal with whether write out files while running synth or c-sim
    
#endif