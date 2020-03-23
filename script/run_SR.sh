#for year in 2016 2017 2018 -1
for year in -1
do
  for i in 0
  do

    ## No DYPtReweight, No MCSF
    root -l -b -q "src/Draw_SR.C($year, $i, false, false)"

    ## DYPtReweight, No MCSF
    root -l -b -q "src/Draw_SR.C($year, $i, true, false)"

    ## DYPtReweight, MCSF
    root -l -b -q "src/Draw_SR.C($year, $i, true, true)"

  done
done

