#!/bin/bash
for year in 2016 2017 2018
do
  for i in 0 1
  do
    for j in 0 1
    do
      for k in 0 1
      do
        root -l -b -q "src/FitBackgrounds.C($year,$i,$j,$k,true,1)"
        #root -l -b -q "src/FitBackgrounds.C($year, $i,$j,$k)"

      done
    done
  done
done
#cd $PLOTTER_WORKING_DIR
#python src/FitBackgrounds_Extract.py -o data/Run2Legacy_v3__Default/2016/FitParameters/Result_0.txt

#source script/run_FitIterate.sh
