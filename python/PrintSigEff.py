import os,ROOT,math
import CMS_lumi, tdrstyle

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']

Year = 2016
TotalLumi = 35918.219

basedir = FILE_PATH+'/'+dataset+'/Regions/'+str(Year)+'/Signal/'

channels = [
'SingleElectron',
'SingleMuon',
]
regions = [
'Resolved',
'Boosted',
]

xseclines = open(WORKING_DIR+'/data/'+dataset+'/xsec_190705_GenXsecAN_eeANDmm.txt').readlines()

for xsecline in xseclines:

  words = xsecline.split()
  mWR = words[0]
  mN = words[1]

  #### filename
  fname = 'HNWRAnalyzer_WRtoNLtoLLJJ_'+'WR%s_N%s'%(mWR,mN)+'.root'

  #### Get TFile
  f = ROOT.TFile(basedir+fname)

  for channel in channels:
    for region in regions:

      #### SignalFlavour
      h_SignalFlavour = f.Get('SignalFlavour')
      LepFlavFrac = h_SignalFlavour.GetBinContent(2)/h_SignalFlavour.GetEntries()
      if channel=='SingleMuon':
        LepFlavFrac = h_SignalFlavour.GetBinContent(3)/h_SignalFlavour.GetEntries()

      #### dirName
      dirName = 'HNWR_'+channel+'_'+region+'_SR'

      #### Num
      h_Num = f.Get(dirName+'/NEvent_'+dirName)
      y_Num = 0
      if h_Num:
        y_Num = h_Num.GetBinContent(1)
      this_eff = y_Num/(TotalLumi*LepFlavFrac)
      print '%s\t%s\t%s\t%s\t%1.3f'%(region,channel.replace('Single',''),mWR,mN,this_eff)

  f.Close()
