import os,ROOT
from array import array

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

basedir = FILE_PATH+'/'+dataset+'/NoLSFCutFitDiagnostics/'
outdirBase = FILE_PATH+'/'+dataset+'/NoLSFCutPostFit/'

Years = [
"2016",
"2017",
"2018",
"YearCombined",
]

Channels = [
"Electron",
"Muon",
]

Bkgds = [
'DYJets_MG_HT_Reweighted',
'NonPrompt',
'Others',
'TT_TW',
'TotalBkg',
]

for Year in Years:

  os.system('mkdir -p '+outdirBase+'/'+Year+'/')

  for Bkgd in Bkgds:

    if Year=="YearCombined" and Bkgd!="TotalBkg":
      continue

    f_out_BkgdName = Bkgd
    if Bkgd=='TotalBkg':
      f_out_BkgdName = 'total_background'
    f_out = ROOT.TFile(outdirBase+'/'+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+f_out_BkgdName+'.root','RECREATE')

    for Channel in Channels:

      fname = 'hists_YearCombined_card_EE_Boosted_SR_WR5000_N100.root' if Channel=="Electron" else 'hists_YearCombined_card_MuMu_Boosted_SR_WR5000_N100.root'
      f = ROOT.TFile(basedir+'/'+fname)

      histName = "Run"+Year+"_postfit/"+Bkgd
      if Year=="YearCombined":
        histName = "postfit/"+Bkgd
      h = f.Get(histName)

      dirName = 'HNWR_Single'+Channel+'_Boosted_SR'
      h.SetName('HNFatJet_LSF_'+dirName)

      f_out.mkdir(dirName)
      f_out.cd(dirName)
      h.Write()
      f_out.cd()

      f.Close()

    f_out.Close()
