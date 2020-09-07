import os,ROOT,math,mylib
import CMS_lumi, tdrstyle
from IsCorrelated import IsCorrelated

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']

Years = [
'2016',
'2017',
'2018',
]

dirNames = [
#'HNWR_SingleElectron_Resolved_SR',
#'HNWR_SingleElectron_Boosted_SR',
#'HNWR_SingleMuon_Resolved_SR',
#'HNWR_SingleMuon_Boosted_SR',
#'HNWR_EMu_Resolved_SR',
'HNWR_SingleElectron_EMu_Boosted_CR',
#'HNWR_SingleMuon_EMu_Boosted_CR',
]

Samples = [
'WJets_MG_HT',
#'Others',
]

for Year in Years:

  for dirName in dirNames:

    for Sample in Samples:

      f = ROOT.TFile(WORKING_DIR+'/rootfiles/'+dataset+'/Regions/'+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+Sample+'.root')

      if not f:
        continue

      h = f.Get(dirName+'/WRCand_Mass_'+dirName)

      if not h:
        continue

      h = mylib.RebinWRMass(h, dirName, 2016)

      binIndex = h.FindBin(5000)

      y = h.Integral()
      y = h.GetBinContent(binIndex)
      e = h.GetBinError(binIndex)

      #if y<=0.:
      #  continue

      Neff = (y/e)*(y/e)

      f.Close()
      print '%s\t%s\t%s\t%f\t%f\t%f'%(Year,dirName,Sample,y,e,Neff)
