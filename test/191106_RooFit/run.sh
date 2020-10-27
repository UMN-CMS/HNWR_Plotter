#!/bin/bash
for i_region in 0 1
do
  for i_channel in 0 1
  do
    root -l -b -q "test.C($i_region, $i_channel)"
  done
done

