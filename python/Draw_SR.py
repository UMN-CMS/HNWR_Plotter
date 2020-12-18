import os,ROOT
from Plotter import SampleGroup, LRSMSignalInfo, Variable, Region, Systematic
from Plotter import Plotter
from IsCorrelated import IsCorrelated
import mylib
import argparse

ROOT.gROOT.SetBatch(ROOT.kTRUE)

## Arguments

parser = argparse.ArgumentParser(description='SR plot commands')
parser.add_argument('-c', dest='Category', type=int, default=0)
parser.add_argument('-y', dest='Year', type=int)
parser.add_argument('--debug',action='store_true')
parser.add_argument('--blind',action='store_true')
parser.add_argument('--ScaleMC', action='store_true')
parser.add_argument('--ApplyZPtRwg', action='store_true')
parser.add_argument('--ApplyDYReshape', action='store_true')
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
LRSMSignalInfoToDraw.useOfficial = True
LRSMSignalInfoToDraw.Color = ROOT.kBlack
LRSMSignalInfoToDraw.Style = 5
LRSMSignalInfoToDraw.xsec = 1. * mylib.GetSignalXsec(WORKING_DIR+'/data/'+dataset+'/xsec_190705_GenXsecAN_eeANDmm.txt', mWR, mN)
LRSMSignalInfoToDraw.kfactor = mylib.GetKFactor(mWR,mN)


m = Plotter()

m.DoDebug = args.debug

#### In/Out
m.DataYear = args.Year
str_Year = str(args.Year)
m.InputDirectory = WORKING_DIR+'/rootfiles/'+dataset+"/Regions/"
if args.Year<0:
  str_Year = 'YearCombined'
m.DataDirectory = str_Year
m.Filename_prefix = "HNWRAnalyzer"
m.Filename_suffix = ""
m.Filename_skim = "_SkimTree_LRSMHighPt"
m.OutputDirectory = ENV_PLOT_PATH+"/"+dataset+"/SR/"+str_Year+"/"
if args.ApplyZPtRwg:
  m.OutputDirectory = ENV_PLOT_PATH+"/"+dataset+"/SR/"+str_Year+"/ApplyZPtRwg/"
if args.ApplyDYReshape:
  m.OutputDirectory = ENV_PLOT_PATH+"/"+dataset+"/SR/"+str_Year+"/ApplyZPtRwg_ApplyDYReshape/"

#### Category
m.ScaleMC = args.ScaleMC

#### Systematic
tmp_Systematics = [
  "Lumi",
  "JetRes",
  "JetEn",
  "MuonRecoSF",
  "MuonEn",
  "MuonIDSF",
  "MuonISOSF",
  "MuonTriggerSF",
  "ElectronRecoSF",
  "ElectronRes",
  "ElectronEn",
  "ElectronIDSF",
  "ElectronTriggerSF",
  "LSFSF",
  "PU",
  "ZPtRw",
  "Prefire",
  "DYNorm",
  "DYReshapeSyst",
  "NonPromptNorm",
  "OthersNorm",
]
#tmp_Systematics = ["Lumi"]

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
if args.ApplyZPtRwg:
  SampleGroup_DY_2016.Samples=['DYJets_MG_HT_Reweighted']
  SampleGroup_DY_2017.Samples=['DYJets_MG_HT_Reweighted']
  SampleGroup_DY_2018.Samples=['DYJets_MG_HT_Reweighted']
if args.ApplyDYReshape:
  SampleGroup_DY_2016.Samples=['DYJets_MG_HT_Reweighted_Reshaped']
  SampleGroup_DY_2017.Samples=['DYJets_MG_HT_Reweighted_Reshaped']
  SampleGroup_DY_2018.Samples=['DYJets_MG_HT_Reweighted_Reshaped']

#### Define Samples
if args.Year>0:
  exec('m.SampleGroups = [SampleGroup_Others_%s, SampleGroup_NonPrompt_%s, SampleGroup_DY_%s, SampleGroup_TT_TW_%s]'%(args.Year,args.Year,args.Year,args.Year))
else:
  m.SampleGroups = [
    SampleGroup_Others_2016, SampleGroup_Others_2017, SampleGroup_Others_2018,
    SampleGroup_NonPrompt_2016, SampleGroup_NonPrompt_2017, SampleGroup_NonPrompt_2018,
    SampleGroup_DY_2016, SampleGroup_DY_2017, SampleGroup_DY_2018,
    SampleGroup_TT_TW_2016, SampleGroup_TT_TW_2017, SampleGroup_TT_TW_2018,
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
m.PrintRegions()

#### Define Variables
m.VariablesToDraw = [
  Variable('ZCand_Pt', 'p_{T}^{ll} (GeV)', 'GeV'),
  Variable('ZCand_Mass', 'm_{ll} (GeV)', 'GeV'),
  Variable('WRCand_Mass', 'm_{W_{R}} (GeV)', 'GeV'),
  Variable('Lepton_0_Pt', 'p_{T} of the leading lepton', 'GeV'),
  Variable('Lepton_0_Eta', '#eta of the leading lepton', ''),
  Variable('Lepton_1_Pt', 'p_{T} of the subleading lepton', 'GeV'),
  Variable('Lepton_1_Eta', '#eta of the subleading lepton', ''),
  Variable('HNFatJet_Eta', '#eta of the AK8 jet', ''),
  Variable('HNFatJet_Pt', 'p_{T} of the AK8 jet (GeV)', 'GeV'),
]
m.PrintVariables()

#### Draw
m.Draw()
