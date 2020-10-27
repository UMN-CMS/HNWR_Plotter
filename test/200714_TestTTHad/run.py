import os,ROOT

Year = "2016"

samples = [
'TTLL_powheg',
'TTLJ_powheg',
'TTJJ_powheg',
]

regions = [
'Resolved_SR',
'Boosted_SR',
]
channels = [
'Electron',
'Muon',
]

for sample in samples:

  f = ROOT.TFile(Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+sample+'.root')
  out = sample
  for region in regions:
    for channel in channels:
      dirName = 'HNWR_Single'+channel+'_'+region
      h = f.Get(dirName+'/NEvent_'+dirName)
      y = 0.
      if h:
        y = h.GetBinContent(1)
      out += '\t'+str(y)
  f.Close()
  print out
