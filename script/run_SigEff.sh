#!/bin/bash
for Year in 2016 2017 2018
#for Year in 2018
do
  root -l -b -q "src/Draw_SignalEfficiency.C(${Year})"
done
