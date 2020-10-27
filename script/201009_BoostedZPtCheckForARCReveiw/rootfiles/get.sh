#!/bin/bash

for Year in 2016 2017 2018
do
  mkdir -p ${Year}
  mv /data6/Users/jskim/SKFlatOutput/Run2Legacy_v4/HNWRAnalyzer/${Year}/201009_BoostedZPtCheckForARCReveiw__ApplyDYPtReweight__/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT_Reweighted.root ${Year}/
done
