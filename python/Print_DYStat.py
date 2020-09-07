import os,ROOT,math
import CMS_lumi, tdrstyle, mylib, canvas_margin
from array import array

def Rebin(hist, region, var, nRebin, Year):
  if var=='WRCand_Mass':
    #return RebinWRMass2(hist, region, Year)
    return mylib.RebinWRMass(hist, region, Year)
  else:
    if nRebin>0:
      hist.Rebin(nRebin)
      return hist
    else:
      return hist

ROOT.TH1.AddDirectory(False)

tdrstyle.setTDRStyle()

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

Years = [
"2016",
"2017",
"2018",
]

Regions = [
'HNWR_SingleElectron_Resolved_DYCR',
'HNWR_SingleMuon_Resolved_DYCR',
'HNWR_SingleElectron_Boosted_DYCR',
'HNWR_SingleMuon_Boosted_DYCR',

'HNWR_SingleElectron_Resolved_SR',
'HNWR_SingleMuon_Resolved_SR',
'HNWR_SingleElectron_Boosted_SR',
'HNWR_SingleMuon_Boosted_SR',

]

VariableSets = [
['WRCand_Mass', -1, 800., 8000., 'm(W_{WR}) (GeV)'],
]

for Year in Years:

  basedir = FILE_PATH+'/'+dataset+'/Regions/'+Year+'/'

  f_NLO = ROOT.TFile(basedir+'/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_Pt.root')
  f_LO = ROOT.TFile(basedir+'/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT.root')
  f_LORwg = ROOT.TFile(basedir+'/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT_Reweighted.root')

  for Region in Regions:

    for VariableSet in VariableSets:

      Variable = VariableSet[0]
      nRebin = VariableSet[1]
      xMin = VariableSet[2]
      xMax = VariableSet[3]
      xtitle = VariableSet[4]

      ## Get hist
      h_NLO = f_NLO.Get(Region+'/'+Variable+'_'+Region)
      h_LO = f_LO.Get(Region+'/'+Variable+'_'+Region)
      h_LORwg = f_LORwg.Get(Region+'/'+Variable+'_'+Region)

      ## Make overflow
      h_NLO.GetXaxis().SetRangeUser(xMin,xMax)
      h_NLO = mylib.MakeOverflowBin(h_NLO)

      h_LO.GetXaxis().SetRangeUser(xMin,xMax)
      h_LO = mylib.MakeOverflowBin(h_LO)

      h_LORwg.GetXaxis().SetRangeUser(xMin,xMax)
      h_LORwg = mylib.MakeOverflowBin(h_LORwg)

      ## Rebin
      h_NLO = Rebin(h_NLO, Region, Variable, nRebin, Year)
      h_LO = Rebin(h_LO, Region, Variable, nRebin, Year)
      h_LORwg = Rebin(h_LORwg, Region, Variable, nRebin, Year)

      this_bin = h_NLO.FindBin(5000)

      Nerr_NLO = h_NLO.GetBinContent(this_bin) / h_NLO.GetBinError(this_bin)
      Nerr_LO = h_LO.GetBinContent(this_bin) / h_LO.GetBinError(this_bin)

      Nerr_NLO = Nerr_NLO*Nerr_NLO
      Nerr_LO = Nerr_LO*Nerr_LO

      print Year+'\t'+Region+'\t'+'%1.2f\t%1.2f'%(Nerr_LO,Nerr_NLO)

