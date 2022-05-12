import os,ROOT
from array import array

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

PreORPosts = [
"Pre",
"Post",
]

for PreORPost in PreORPosts:

  Mass_Boosted = "WR5000_N100"

  basedir = FILE_PATH+'/'+dataset+'/'+PreORPost+'Fit/'
  outdirBase = FILE_PATH+'/'+dataset+'/'+PreORPost+'Fit_ChannelMerged/'

  Years = [
  "2016",
  "2017",
  "2018",
  "YearCombined",
  ]

  Channels = [
  "EE",
  "MuMu",
  ]

  Regions = [
  "Resolved_SR",
  "Boosted_SR",
  ]

  Samples = [
  'TT_TW',
  'DYJets_MG_HT_ReweightedQCDErrorEWCorr_Reshaped',
  'NonPrompt',
  'Others',
  'total_background',
  ]

  ## MC first
  for Sample in Samples:

    for Year in Years:

      fpath = basedir+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+Sample+'.root'
      f = ROOT.TFile(fpath)

      os.system('mkdir -p '+outdirBase+Year)
      f_out = ROOT.TFile(outdirBase+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+Sample+'.root','RECREATE')

      for Region in Regions:

        dirName_EE = 'HNWR_SingleElectron_'+Region
        hist_EE = f.Get(dirName_EE+'/WRCand_Mass_'+dirName_EE)

        dirName_MM = 'HNWR_SingleMuon_'+Region
        hist_MM = f.Get(dirName_MM+'/WRCand_Mass_'+dirName_MM)

        hist_LL = hist_EE.Clone('WRCand_Mass_HNWR_SingleLepton_'+Region)
        hist_LL.Add(hist_MM)

        f_out.cd()
        f_out.mkdir('HNWR_SingleLepton_'+Region)
        f_out.cd('HNWR_SingleLepton_'+Region)
        hist_LL.Write()
        f_out.cd()

      f_out.Close()

  ## Data
  for Year in Years:

    fpath = basedir+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+Sample+'.root'
    f_EE = ROOT.TFile(basedir+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_data_SingleElectron.root')
    f_MM = ROOT.TFile(basedir+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_data_SingleMuon.root')

    f_out = ROOT.TFile(outdirBase+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_data_SingleLepton.root','RECREATE')

    for Region in Regions:

      dirName_EE = 'HNWR_SingleElectron_'+Region
      hist_EE = f_EE.Get(dirName_EE+'/WRCand_Mass_'+dirName_EE)

      dirName_MM = 'HNWR_SingleMuon_'+Region
      hist_MM = f_MM.Get(dirName_MM+'/WRCand_Mass_'+dirName_MM)

      hist_LL = hist_EE.Clone('WRCand_Mass_HNWR_SingleLepton_'+Region)
      hist_LL.Add(hist_MM)

      f_out.cd()
      f_out.mkdir('HNWR_SingleLepton_'+Region)
      f_out.cd('HNWR_SingleLepton_'+Region)
      hist_LL.Write()
      f_out.cd()

    f_out.Close()
