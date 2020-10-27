for Year in 2016 2017 2018 -1
#for Year in -1
do
  for c in 0 1 2 3
  #for c in 3
  do
    #python python/Draw_PostFit.py -y ${Year} -c ${c} --blind
    #python python/Draw_PreFit.py -y ${Year} -c ${c} --blind

    ###python python/Draw_CROnlyPostFit.py -y ${Year} -c ${c} --blind
    ###python python/Draw_CROnlyPreFit.py -y ${Year} -c ${c} --blind

    python python/Draw_PostFit.py -y ${Year} -c ${c}
    python python/Draw_PreFit.py -y ${Year} -c ${c}
  done
done
