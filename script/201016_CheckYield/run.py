import os,ROOT
import mylib
import canvas_margin
import tdrstyle
import CMS_lumi, tdrstyle
import math
from array import array

Years = [
"2016",
"2017",
"2018",
]

Channels = [
"Electron",
"Muon",
]

Regions = [
"Resolved_DYCR",
"Boosted_DYCR",
]

Bkgds = [
"DYJets_MG_HT_Reweighted",
"TTLX_powheg",
"Others",
]

baseDir = "/data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v4__Default/Regions/"

for Year in Years:

  for Region in Regions:

    for Channel in Channels:

      dirName = "HNWR_Single"+Channel+"_"+Region

      f_data = ROOT.TFile(baseDir+"/"+Year+"/HNWRAnalyzer_SkimTree_LRSMHighPt_data_Single"+Channel+".root")
      h_data = f_data.Get(dirName+"/NEvent_"+dirName)
      y_data = h_data.GetBinContent(1)

      y_TotBkgd = 0

      out = Year+'\t'+dirName+'\t'+'%1.2f'%(y_data)

      for Bkgd in Bkgds:
        f_Bkgd = ROOT.TFile(baseDir+"/"+Year+"/HNWRAnalyzer_SkimTree_LRSMHighPt_"+Bkgd+".root")
        h_Bkgd = f_Bkgd.Get(dirName+"/NEvent_"+dirName)
        y_Bkgd = h_Bkgd.GetBinContent(1)
        y_TotBkgd += y_Bkgd

        out += '\t%1.2f'%(y_Bkgd)

      print out
