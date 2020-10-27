import os,ROOT
import mylib
import canvas_margin
import tdrstyle
import CMS_lumi, tdrstyle
import math
from array import array

tdrstyle.setTDRStyle()

baseDir = '/data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v4__Default/Regions/'

Years = [
'2016',
'2017',
'2018',
]

Channels = [
'Electron',
'Muon'
]

Regions = [
'Resolved',
'Boosted',
]

varName = "ZCand_Pt"

for Year in Years:

  for Region in Regions:

    data_values = []
    values = []

    for Channel in Channels:

      f_Nominal = ROOT.TFile(baseDir+'/'+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT.root')
      f_Rew = ROOT.TFile(baseDir+'/'+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT_Reweighted.root')
      f_RewRes = ROOT.TFile(baseDir+'/'+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT_Reweighted_Reshaped.root')


      dirName = 'HNWR_Single'+Channel+'_'+Region+'_SR'

      c1 = ROOT.TCanvas('c1', '', 800, 800)
      c1_up = ROOT.TPad("c1_up", "", 0, 0.25, 1, 1)
      c1_down = ROOT.TPad("c1_down", "", 0, 0, 1, 0.25)
      c1, c1_up, c1_down = canvas_margin.canvas_margin(c1, c1_up, c1_down)
      c1.Draw()
      c1_up.Draw()
      c1_down.Draw()

      c1_up.cd()

      h_Nominal = f_Nominal.Get(dirName+'/'+varName+'_'+dirName)
      h_Rew = f_Rew.Get(dirName+'/'+varName+'_'+dirName)
      h_RewRes = f_RewRes.Get(dirName+'/'+varName+'_'+dirName)

      h_Nominal.Rebin(50)
      h_Rew.Rebin(50)
      h_RewRes.Rebin(50)

