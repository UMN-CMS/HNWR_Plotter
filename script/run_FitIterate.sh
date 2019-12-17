#!/bin/bash


for iter in {1..50}
do

  #### update 
  PrevIter=$(expr ${iter} - 1)

  cd $PLOTTER_WORKING_DIR/data/Run2Legacy_v3__Default/2016/FitParameters/
  echo Result_${PrevIter}.txt
  python UpdateFromResult.py -i Result_${PrevIter}.txt
  cd $PLOTTER_WORKING_DIR

  #### run

  for i in 0 1
  do
    for j in 0 1
    do
      for k in 0 1
      do
        root -l -b -q "src/FitBackgrounds.C($i,$j,$k,true,$iter)" &> output/Run2Legacy_v3__Default/FitBackgrounds/2016/log_${i}_${j}_${k}.log
      done
    done
  done

  cd $PLOTTER_WORKING_DIR
  python src/FitBackgrounds_Extract.py -o data/Run2Legacy_v3__Default/2016/FitParameters/Result_${iter}.txt

done
