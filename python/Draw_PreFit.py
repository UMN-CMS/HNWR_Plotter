import os,ROOT
from Plotter import SampleGroup, LRSMSignalInfo, Variable, Region, Systematic
from Plotter import Plotter
from IsCorrelated import IsCorrelated
import mylib
import argparse

ROOT.gROOT.SetBatch(ROOT.kTRUE)

## Arguments

parser = argparse.ArgumentParser(description='PreFit plot commands')
parser.add_argument('-c', dest='Category', type=int, default=0)
parser.add_argument('-y', dest='Year', type=int)
parser.add_argument('--debug',action='store_true')
parser.add_argument('--blind',action='store_true')
parser.add_argument('--ScaleMC', action='store_true')
args = parser.parse_args()

## Blind mode
UnblindData = not args.blind

## Enviroment
WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
ENV_PLOT_PATH = os.environ['PLOT_PATH']

## Signal mass
mWR, mN = 5000, 3000
if args.Category==1:
  mWR, mN = 5000, 200
LRSMSignalInfoToDraw = LRSMSignalInfo(mWR = mWR, mN = mN)
LRSMSignalInfoToDraw.Color = ROOT.kBlack
LRSMSignalInfoToDraw.useOfficial = True
LRSMSignalInfoToDraw.Style = 5
LRSMSignalInfoToDraw.xsec = mylib.GetSignalXsec(WORKING_DIR+'/data/'+dataset+'/xsec_190705_GenXsecAN_eeANDmm.txt', mWR, mN)
LRSMSignalInfoToDraw.kfactor = mylib.GetKFactor(mWR,mN)


m = Plotter()

m.DoDebug = args.debug

#### In/Out
m.DataYear = args.Year
str_Year = str(args.Year)
m.InputDirectory = WORKING_DIR+'/rootfiles/'+dataset+"/PreFit/"
if args.Year<0:
  str_Year = 'YearCombined'
m.DataDirectory = str_Year
m.Filename_prefix = "HNWRAnalyzer"
m.Filename_suffix = ""
m.Filename_skim = "_SkimTree_LRSMHighPt"
m.OutputDirectory = ENV_PLOT_PATH+"/"+dataset+"/PreFit/"+str_Year+"/"

#### Category
m.ScaleMC = False

#### Systematic
tmp_Systematics = []
m.ErrorFromShape = True
m.AddErrorLinear = False
if args.Year<0:
  m.AddErrorLinear = True

m.Systematics = [ Systematic(Name="Central", Direction=0, Year=-1) ]
for s in tmp_Systematics:
  isCorr = IsCorrelated(s)
  if isCorr:
    m.Systematics.append( Systematic(Name=s, Direction=+1, Year=-1) )
    m.Systematics.append( Systematic(Name=s, Direction=-1, Year=-1) )
  else:
    if args.Year>0:
      m.Systematics.append( Systematic(Name=s, Direction=+1, Year=args.Year) )
      m.Systematics.append( Systematic(Name=s, Direction=-1, Year=args.Year) )
    else:
      for Y in [2016,2017,2018]:
        m.Systematics.append( Systematic(Name=s, Direction=+1, Year=Y) )
        m.Systematics.append( Systematic(Name=s, Direction=-1, Year=Y) )
m.PrintSystematics()

#### Binning infos
m.SetBinningFilepath(
  WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/SR_rebins.txt',
  WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/SR_xaxis.txt',
  WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/SR_yaxis.txt',
)

#### Predef samples
from PredefinedSamples import *
SampleGroup_DY_2016.Samples=['DYJets_MG_HT_Reweighted_Reshaped']
SampleGroup_DY_2017.Samples=['DYJets_MG_HT_Reweighted_Reshaped']
SampleGroup_DY_2018.Samples=['DYJets_MG_HT_Reweighted_Reshaped']

#### Define Samples
if args.Year>0:
  exec('m.SampleGroups = [SampleGroup_Others_%s, SampleGroup_NonPrompt_%s, SampleGroup_DY_%s, SampleGroup_TT_TW_%s, SampleGroup_total_background_%s]'%(args.Year,args.Year,args.Year,args.Year,args.Year))
else:
  m.SampleGroups = [
    SampleGroup_Others_2016, SampleGroup_Others_2017, SampleGroup_Others_2018,
    SampleGroup_NonPrompt_2016, SampleGroup_NonPrompt_2017, SampleGroup_NonPrompt_2018,
    SampleGroup_DY_2016, SampleGroup_DY_2017, SampleGroup_DY_2018,
    SampleGroup_TT_TW_2016, SampleGroup_TT_TW_2017, SampleGroup_TT_TW_2018,
    ## dummy total background
    SampleGroup_total_background_YearCombined,
  ]
#### Signal
m.SignalsToDraw = [LRSMSignalInfoToDraw]

#### Print
m.PrintSamples()

#### Define reiongs
if args.Category==0:
  m.RegionsToDraw = [
    Region('HNWR_SingleElectron_Resolved_SR', 'SingleElectron', UnblindData=UnblindData, Logy=1, TLatexAlias='#splitline{ee}{Resolved SR}'),
    Region('HNWR_SingleMuon_Resolved_SR', 'SingleMuon', UnblindData=UnblindData, Logy=1, TLatexAlias='#splitline{#mu#mu}{Resolved SR}'),
  ]
elif args.Category==1:
  m.RegionsToDraw = [
    Region('HNWR_SingleElectron_Boosted_SR', 'SingleElectron', UnblindData=UnblindData, Logy=1, TLatexAlias='#splitline{ee}{Boosted SR}'),
    Region('HNWR_SingleMuon_Boosted_SR', 'SingleMuon', UnblindData=UnblindData, Logy=1, TLatexAlias='#splitline{#mu#mu}{Boosted SR}'),
  ]
elif args.Category==2:
  m.RegionsToDraw = [
    ## Resolved EMu
    Region('HNWR_EMu_Resolved_SR', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{e#mu}{Resolved flavor CR}'),
    ## Boosted EMu
    Region('HNWR_SingleElectron_EMu_Boosted_CR', 'SingleElectron', UnblindData=True, Logy=1, TLatexAlias='#splitline{e+#mu-Jet}{Boosted flavor CR}'),
    Region('HNWR_SingleMuon_EMu_Boosted_CR', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{#mu+e-Jet}{Boosted flavor CR}'),
  ]
  m.SignalsToDraw = []
  #### Binning infos
  m.SetBinningFilepath(
    WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/CR_rebins.txt',
    WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/CR_xaxis.txt',
    WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/CR_yaxis.txt',
  )
elif args.Category==3:
  #### Define Samples
  if args.Year>0:
    exec('m.SampleGroups = [SampleGroup_Others_%s, SampleGroup_NonPrompt_%s, SampleGroup_TT_TW_%s, SampleGroup_DY_%s, SampleGroup_total_background_%s]'%(args.Year,args.Year,args.Year,args.Year,args.Year))
  else:
    m.SampleGroups = [
      SampleGroup_Others_2016, SampleGroup_Others_2017, SampleGroup_Others_2018,
      SampleGroup_NonPrompt_2016, SampleGroup_NonPrompt_2017, SampleGroup_NonPrompt_2018,
      SampleGroup_TT_TW_2016, SampleGroup_TT_TW_2017, SampleGroup_TT_TW_2018,
      SampleGroup_DY_2016, SampleGroup_DY_2017, SampleGroup_DY_2018,
      ## dummy total background
      SampleGroup_total_background_YearCombined,
    ]
  #### Define reiongs
  m.RegionsToDraw = [
    Region('HNWR_SingleElectron_Resolved_DYCR', 'SingleElectron', UnblindData=True, Logy=1, TLatexAlias='#splitline{ee}{Resolved DY CR}'),
    Region('HNWR_SingleMuon_Resolved_DYCR', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{#mu#mu}{Resolved DY CR}'),
    Region('HNWR_SingleElectron_Boosted_DYCR', 'SingleElectron', UnblindData=True, Logy=1, TLatexAlias='#splitline{ee}{Boosted DY CR}'),
    Region('HNWR_SingleMuon_Boosted_DYCR', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{#mu#mu}{Boosted DY CR}'),
  ]
  m.SignalsToDraw = []
  #### Binning infos
  m.SetBinningFilepath(
    WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/CR_rebins.txt',
    WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/CR_xaxis.txt',
    WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/CR_yaxis.txt',
  )


m.PrintRegions()

#### Define Variables
m.VariablesToDraw = [
  Variable('WRCand_Mass', 'm_{W_{R}} (GeV)', 'GeV'),
]
m.PrintVariables()

#### Extra lines
m.ExtraLines='''tl = ROOT.TLatex()
tl.SetNDC()
tl.SetTextSize(0.037)
tl.DrawLatex(0.2, 0.815, 'Prefit')
'''

#### Draw
m.Draw()
