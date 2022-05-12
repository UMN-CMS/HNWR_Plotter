#!/bin/bash
#for Year in 2016 2017 2018 -1
for Year in -1
do
  #for c in 0 1
  for c in 0
  do
    python python/Draw_NoLSFCut.py -y ${Year} --ScaleMC --ApplyZPtRwg -c ${c}
    #python python/Draw_NoLSFCutPostFit.py -y ${Year} -c ${c}

    #python python/Draw_NoLSFNoSDMassCut.py -y ${Year} --ScaleMC --ApplyZPtRwg -c ${c}
  done
done
