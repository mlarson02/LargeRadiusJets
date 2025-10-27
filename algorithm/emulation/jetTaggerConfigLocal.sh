#!/usr/bin/env bash
# loops over all requested parameter values (does nothing else)
set -euo pipefail
# ---- parameter grids ----
#rMergeCuts=(0.001 1.5 2.0 2.5 3.5)
#rSquaredCuts=(0.81 1.0 1.21 1.44)
#nIOs=(64 128 256 512)
#nSeeds=(2)
#signals=(true false)

rMergeCuts=(0.001)
rSquaredCuts=(1.0)
nIOs=(128 256 512)
nSeeds=(2)
signals=(true false)

condor=false

# ---- paths ----
inputLUTFilePath="/home/larsonma/LargeRadiusJets/algorithm/emulation/LUT_Constants_Generation/LUTs/"
constants_base="/home/larsonma/LargeRadiusJets/algorithm/emulation/LUT_Constants_Generation/constants/"
dest_dir="/home/larsonma/LargeRadiusJets/algorithm/emulation/UsedConstantsLUTs/"
src_cc="/home/larsonma/LargeRadiusJets/algorithm/emulation/jetTaggerEmulation.cc"

# helper function to generate correct LUT names for each algorithm configuration
make_input_LUT_file_name() {
  local rMerge="$1" r2="$2" type="$3"
  local base="${4:-/home/larsonma/LargeRadiusJets/algorithm/emulation/LUT_Constants_Generation/LUTs/}"
  # match C++: setprecision(4) for rMerge, setprecision(3) for R2 (no std::fixed)
  local rMergeFmt r2Fmt
  rMergeFmt=$(printf "%.4g" "$rMerge")
  r2Fmt=$(printf "%.3g" "$r2")
  printf "%sLUT_%s_rMerge_%s_R2_%s.h" "$base" "$type" "$rMergeFmt" "$r2Fmt"
}

# helper function to generate correct constants file names for each algorithm configuration
make_input_constants_file_name() {
  local rMerge="$1" nIOs="$2" nSeeds="$3" r2="$4"
  local base="${5:-/home/larsonma/LargeRadiusJets/algorithm/emulation/LUT_Constants_Generation/constants/}"
  # Match C++ setprecision behavior (no fixed): 4 sig figs for rMerge, 3 for R2
  local rMergeFmt r2Fmt
  rMergeFmt=$(printf "%.4g" "$rMerge")
  r2Fmt=$(printf "%.3g" "$r2")
  printf "%sconstants_rMerge_%s_R2_%s_IOs_%s_Seeds_%s.h" \
         "$base" "$rMergeFmt" "$r2Fmt" "$nIOs" "$nSeeds"
}


# ---- main loops ----
for rMerge in "${rMergeCuts[@]}"; do
  for r2 in "${rSquaredCuts[@]}"; do
    for ios in "${nIOs[@]}"; do
      for seeds in "${nSeeds[@]}"; do

        # Build filenames
        constants_file=$(make_input_constants_file_name "$rMerge" "$ios" "$seeds" "$r2" "$constants_base")

        lut_output_path=$(make_input_LUT_file_name "$rMerge" "$r2" "deltaR2Cut" "$inputLUTFilePath")
        lutR_output_path=$(make_input_LUT_file_name "$rMerge" "$r2" "deltaR" "$inputLUTFilePath")
        lutR_5b_output_path=$(make_input_LUT_file_name "$rMerge" "$r2" "deltaR_8b" "$inputLUTFilePath")

        echo "Config: rMerge=$rMerge r2=$r2 nIOs=$ios nSeeds=$seeds"
        echo "  constants: $constants_file"
        echo "  LUTs:"
        echo "    $lut_output_path"
        echo "    $lutR_output_path"
        echo "    $lutR_5b_output_path"

        echo "copying files"
        cp -f "$constants_file" "$dest_dir/constants.h"

        cp -f "$lut_output_path" "$dest_dir/deltaR2LUT.h"
        cp -f "$lutR_output_path" "$dest_dir/deltaRLUT.h"
        cp -f "$lutR_5b_output_path" "$dest_dir/deltaRLUT_8b.h"

        # Run ROOT for each (signal, jz)
        for signal in "${signals[@]}"; do
          echo "Running ROOT: signal=$signal rMerge=$rMerge r2=$r2 nIOs=$ios nSeeds=$seeds"
          root -l -b -q "${src_cc}+(${rMerge}, ${ios}, ${seeds}, ${r2}, ${signal}, ${condor})"
        done
      done
    done
  done
done

rm -rf jetTaggerEmulation_cc*