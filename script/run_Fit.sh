#!/bin/bash
for i in 0 1
do
  for j in 0 1
  do
    for k in 0 1
    do
      root -l -b -q "src/FitBackgrounds.C($i,$j,$k)" &> output/Run2Legacy_v3__Default/FitBackgrounds/2016/log_${i}_${j}_${k}.log
    done
  done
done

cd $PLOTTER_WORKING_DIR
python src/FitBackgrounds_Extract.py -o data/Run2Legacy_v3__Default/2016/FitParameters/Result_0.txt

source script/run_FitIterate.sh
