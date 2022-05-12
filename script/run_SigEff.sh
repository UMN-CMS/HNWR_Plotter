#!/bin/bash
#for Year in 2016 2017 2018
for Year in 2018
do
  #root -l -b -q "src/Draw_SignalEfficiency.C(${Year})"

  root -l -b -q "src/Draw_SignalEfficiency_Official.C(${Year})" &> tmp/log_Draw_SignalEfficiency_Official_${Year}.log &

done
