for Year in 2016 2017 2018 -1
#for Year in -1
do
  for c in 0 1
  #for c in 1
  do
    #python python/Draw_SR.py -y ${Year} --ScaleMC --ApplyZPtRwg --ApplyDYReshape -c ${c} --blind
    #python python/Draw_SR.py -y ${Year} --ScaleMC --ApplyZPtRwg --ApplyDYReshape -c ${c}


    python python/Draw_SR.py -y ${Year} -c ${c}
    python python/Draw_SR.py -y ${Year} --ApplyZPtRwg -c ${c}
    python python/Draw_SR.py -y ${Year} --ApplyZPtRwg --ScaleMC -c ${c}
    python python/Draw_SR.py -y ${Year} --ApplyZPtRwg --ScaleMC --ApplyDYReshape -c ${c}


  done
done
