import os,ROOT
from Plotter import SampleGroup, Variable, Region, Systematic
from Plotter import Plotter
from IsCorrelated import IsCorrelated
import argparse

ROOT.gROOT.SetBatch(ROOT.kTRUE)

## Arguments

parser = argparse.ArgumentParser(description='CR plot commands')
parser.add_argument('-c', dest='Category', type=int, default=0)
parser.add_argument('-y', dest='Year', type=int)
parser.add_argument('--debug',action='store_true')
parser.add_argument('--ScaleMC', action='store_true')
parser.add_argument('--ApplyZPtRwg', action='store_true')
args = parser.parse_args()

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
ENV_PLOT_PATH = os.environ['PLOT_PATH']

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
m.OutputDirectory = ENV_PLOT_PATH+"/"+dataset+"/PYTEST_CR/"+str_Year+"/"
if args.ApplyZPtRwg:
  m.OutputDirectory = ENV_PLOT_PATH+"/"+dataset+"/PYTEST_CR/"+str_Year+"/ApplyZPtRwg/"

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
  WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/CR_rebins.txt',
  WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/CR_xaxis.txt',
  WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/CR_yaxis.txt',
)

#### Predef samples
from PredefinedSamples import *
if args.ApplyZPtRwg:
  SampleGroup_DY_2016.Samples=['DYJets_MG_HT_Reweighted']
  SampleGroup_DY_2017.Samples=['DYJets_MG_HT_Reweighted']
  SampleGroup_DY_2018.Samples=['DYJets_MG_HT_Reweighted']

if args.Category==0:
  #### Define Samples
  if args.Year>0:
    exec('m.SampleGroups = [SampleGroup_Others_%s, SampleGroup_ttbar_%s, SampleGroup_DY_%s]'%(args.Year,args.Year,args.Year))
  else:
    m.SampleGroups = [
      SampleGroup_Others_2016, SampleGroup_Others_2017, SampleGroup_Others_2018,
      SampleGroup_ttbar_2016, SampleGroup_ttbar_2017, SampleGroup_ttbar_2018,
      SampleGroup_DY_2016, SampleGroup_DY_2017, SampleGroup_DY_2018,
    ]
  #### Signals
  #### Print
  m.PrintSamples()

  #### Define reiongs
  m.RegionsToDraw = [
    Region('HNWR_SingleElectron_Resolved_DYCR', 'SingleElectron', DrawData=True, Logy=1, TLatexAlias='#splitline{ee}{Resolved DY sideband}'),
    Region('HNWR_SingleMuon_Resolved_DYCR', 'SingleMuon', DrawData=True, Logy=1, TLatexAlias='#splitline{#mu#mu}{Resolved DY sideband}'),
    Region('HNWR_SingleElectron_Boosted_DYCR', 'SingleElectron', DrawData=True, Logy=1, TLatexAlias='#splitline{ee}{Boosted DY sideband}'),
    Region('HNWR_SingleMuon_Boosted_DYCR', 'SingleMuon', DrawData=True, Logy=1, TLatexAlias='#splitline{#mu#mu}{Boosted DY sideband}'),
  ]
  m.PrintRegions()

if args.Category==1:
  #### Define Samples
  if args.Year>0:
    exec('m.SampleGroups = [SampleGroup_Others_%s, SampleGroup_DY_%s, SampleGroup_ttbar_%s]'%(args.Year,args.Year,args.Year))
  else:
    m.SampleGroups = [
      SampleGroup_Others_2016, SampleGroup_Others_2017, SampleGroup_Others_2018,
      SampleGroup_DY_2016, SampleGroup_DY_2017, SampleGroup_DY_2018,
      SampleGroup_ttbar_2016, SampleGroup_ttbar_2017, SampleGroup_ttbar_2018,
    ]
  #### Signals
  #### Print
  m.PrintSamples()

  #### Define reiongs
  m.RegionsToDraw = [
    ## Resolved EMu
    Region('HNWR_EMu_Resolved_SR', 'SingleMuon', DrawData=True, Logy=1, TLatexAlias='#splitline{e#mu}{Resolved flavor sideband}'),
    ## Boosted EMu
    Region('HNWR_SingleElectron_EMu_Boosted_CR', 'SingleElectron', DrawData=True, Logy=1, TLatexAlias='#splitline{e+#mu-Jet}{Boosted flavor sideband}'),
    Region('HNWR_SingleMuon_EMu_Boosted_CR', 'SingleMuon', DrawData=True, Logy=1, TLatexAlias='#splitline{#mu+#e-Jet}{Boosted flavor sideband}'),
    ## Resolved EMu, but DYCR. This is dominated by ttbar, so it is here..
    ## This region is not important
    Region('HNWR_EMu_Resolved_DYCR', 'SingleMuon', DrawData=True, Logy=1, TLatexAlias='Resolved e#mu DY sideband'),
    ## LowWRCR emu regions
    Region('HNWR_EMu_Resolved_LowWRCR', 'SingleMuon', DrawData=True, Logy=-1, TLatexAlias='#splitline{e#mu}{Resolved low-mass sideband}'),
    Region('HNWR_SingleElectron_EMu_Boosted_LowWRCR', 'SingleElectron', DrawData=True, Logy=-1, TLatexAlias='#splitline{e+#mu-Jet}{Boosted low-mass sideband}'),
    Region('HNWR_SingleMuon_EMu_Boosted_LowWRCR', 'SingleMuon', DrawData=True, Logy=-1, TLatexAlias='#splitline{#mu+e-Jet}{Boosted low-mass sideband}'),
  ]
  m.PrintRegions()

#### Define Variables
m.VariablesToDraw = [
  Variable('ZCand_Pt', 'p_{T}^{ll} (GeV)', 'GeV'),
  Variable('ZCand_Mass', 'm_{ll} (GeV)', 'GeV'),
  Variable('WRCand_Mass', 'm_{W_{R}} (GeV)', 'GeV'),
]
m.PrintVariables()

#### Draw
m.Draw()
