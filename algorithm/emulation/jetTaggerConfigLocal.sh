#!/usr/bin/env bash
# loops over all requested parameter values (does nothing else)

rMergeCuts=(0.001 1.5 2.0 2.5 3.5)
rSquaredCuts=(0.81 1.0 1.21 1.44)
nIOs=(64 128 256 512)
nSeeds=(2)
signals=(true false)
jzSlices=({0..9})   # 0 through 9


for rMerge in "${rMergeCuts[@]}"; do
    for r2 in "${rSquaredCuts[@]}"; do
        for ios in "${nIOs[@]}"; do
            for seeds in "${nSeeds[@]}"; do
            
            echo "signal=$signal jz=$jz rMerge=$rMerge r2=$r2 nIOs=$ios nSeeds=$seeds"

            

                for signal in "${signals[@]}"; do
                    for jz in "${jzSlices[@]}"; do
            :
            

            


            # TODO: add your commands here, e.g.
            # echo "signal=$signal jz=$jz rMerge=$rMerge r2=$r2 nIOs=$ios nSeeds=$seeds"
          done
        done
      done
    done
  done
done
