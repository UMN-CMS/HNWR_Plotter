for year in 2016 2017 2018
do
  for i in 0 1 2
  do

    ## No DYPtReweight, No MCSF
    root -l -b -q "src/Draw_CR.C($year, $i, false, false)"

    ## DYPtReweight, No MCSF
    root -l -b -q "src/Draw_CR.C($year, $i, true, false)"

    ## DYPtReweight, MCSF
    root -l -b -q "src/Draw_CR.C($year, $i, true, true)"

  done
done

