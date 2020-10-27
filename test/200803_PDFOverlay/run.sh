#!/bin/bash
for Year in 2016 2017 2018
do
  for channel in 0 1
  do
    for region in 0 1
    do
      root -l -b -q "run.C(${Year},${channel},${region}) " &> log_${Year}_${channel}_${region}.log &
    done
  done
done
