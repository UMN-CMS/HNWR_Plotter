import os,ROOT
from Plotter import SampleGroup, Variable, Region
from Plotter import Plotter
import argparse

ROOT.gROOT.SetBatch(ROOT.kTRUE)

## Arguments

parser = argparse.ArgumentParser(description='CR plot commands')
parser.add_argument('-c', dest='Category', type=int, default=0)
parser.add_argument('-y', dest='Year', type=int)
parser.add_argument('--ScaleMC', action='store_true')
parser.add_argument('--ApplyZPtRwg', action='store_true')
args = parser.parse_args()

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
ENV_PLOT_PATH = os.environ['PLOT_PATH']

m = Plotter()

#### In/Out
m.DataYear = args.Year
str_Year = str(args.Year)
m.InputDirectory = WORKING_DIR+'/rootfiles/'+dataset+"/Regions/"+str(args.Year)+"/"
if args.Year<0:
  str_Year = 'YearCombined'
  m.InputDirectory = WORKING_DIR+'/'+dataset+"/Regions/"
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
  #"LSFSF",
  "PU",
  #"ZPtRw",
  "Prefire",
]

m.Systematics = ["Central"]
for s in tmp_Systematics:
  m.Systematics.append(s+"Up")
  m.Systematics.append(s+"Down")

#### Binning infos
m.SetBinningFilepath(
  WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/CR_rebins.txt',
  WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/CR_xaxis.txt',
  WORKING_DIR+'/data/'+dataset+'/'+str_Year+'/CR_yaxis.txt',
)

#### Predef samples
####  __init__(self, Name="", Type=Type, Samples=[], Color=0, Style=1, TLatexAlias="", LatexAlias=""):
SampleGroup_DY = SampleGroup(
  Name='DY',
  Type='Bkgd',
  Samples=['DYJets_MG_HT'],
  Color=ROOT.kYellow,
  Style=1,
  TLatexAlias='Z+Jets',
  LatexAlias='ZJets'
)
if args.ApplyZPtRwg:
  SampleGroup_DY.Samples=['DYJets_MG_HT_Reweighted']

SampleGroup_ttbar = SampleGroup(
  Name='ttbar',
  Type='Bkgd',
  Samples=['TTLX_powheg'],
  Color=ROOT.kRed,
  Style=1,
  TLatexAlias='t#bar{t}',
  LatexAlias='ttbar'
)

SampleGroup_Others = SampleGroup(
  Name='Others',
  Type='Bkgd',
  Samples=['Others'],
  Color=870,
  Style=1,
  TLatexAlias='Other backgrounds',
  LatexAlias='Others'
)

if args.Category==0:
  #### Define Samples
  m.SampleGroups = [
    SampleGroup_Others,
    SampleGroup_ttbar,
    SampleGroup_DY,
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
  m.SampleGroups = [
    SampleGroup_Others,
    SampleGroup_DY,
    SampleGroup_ttbar,
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
