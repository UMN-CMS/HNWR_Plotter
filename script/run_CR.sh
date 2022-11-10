#!/bin/bash

## Year : 2016/2017/2018/-1 (-1=YearCombined)
for Year in -1
do
  ## c : 0=DYCR, 1=FlavorCR
  for c in 0
  do

    ## Nominal DY MC
    python python/Draw_CR.py -y ${Year} -c ${c}
    ## DY+ZPtReweight
    python python/Draw_CR.py -y ${Year} --ApplyZPtRwg -c ${c}
    ## DY+ZPtReweight+JetPtReshape
    python python/Draw_CR.py -y ${Year} --ApplyZPtRwg --ApplyDYReshape -c ${c}

  done
done
