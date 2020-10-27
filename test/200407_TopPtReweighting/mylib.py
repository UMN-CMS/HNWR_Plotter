import os,ROOT
from array import array

def TotalLumi(DataYear):

  if DataYear==2016:
    return "35.92"
  if DataYear==2017:
    return "41.53"
  if DataYear==2018:
    return "59.74"
  if DataYear<0:
    return "137"
  else:
    print "[mylib.py, TotalLumi()] Wrong DataYear : %d"%DataYear
    return "35.9"

def RebinWRMass(hist, region, DataYear):

  lastbin = hist.GetXaxis().GetNbins()
  vec_bins = [0., 800, 1000, 1200, 1400, 1600, 2000, 2400, 2800, 3200, 8000]
  if "Boosted" in region:
    if DataYear==2016 and ( ("HNWR_SingleMuon_EMu_Boosted_CR" in region) or ("HNWR_SingleElectron_Boosted_SR" in region) or ("elFatJet" in region) ):
      vec_bins = [0., 800, 1000, 1200, 1500, 1800, 8000]
    else:
      vec_bins = [0., 800, 1000, 1200, 1500, 1700, 8000]

  n_bin = len(vec_bins)-1
  hist = hist.Rebin(n_bin, hist.GetName(), array("d", vec_bins) )
  return hist
