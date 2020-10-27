#!/bin/bash
for nbin in 10 100 200 -1 400
do
  for region in HNWR_SingleElectron_Resolved_SR HNWR_SingleElectron_Boosted_SR HNWR_SingleMuon_Resolved_SR HNWR_SingleMuon_Boosted_SR
  do
    python run.py $nbin $region -b
  done
done
