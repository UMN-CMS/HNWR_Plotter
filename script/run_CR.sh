#!/bin/bash
for Year in 2016 2017 2018 -1
#for Year in -1
#for Year in 2016 2017 2018
do
  for c in 0 1
  #for c in 0
  do
    python python/Draw_CR.py -y ${Year} -c ${c}

    python python/Draw_CR.py -y ${Year} --ApplyZPtRwg -c ${c}
    python python/Draw_CR.py -y ${Year} --ApplyZPtRwg --ScaleMC -c ${c}
    python python/Draw_CR.py -y ${Year} --ApplyZPtRwg --ScaleMC --ApplyDYReshape -c ${c}

    ##python python/Draw_CR.py -y ${Year} --ScaleMC -c ${c}
    ##python python/Draw_CR.py -y ${Year} --ScaleMC --ApplyDYReshape -c ${c}

  done
done
