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
parser.add_argument('--ApplyDYReshape', action='store_true')
args = parser.parse_args()

## Enviroment

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

m.OutputDirectory = ENV_PLOT_PATH+"/"+dataset+"/CR/"+str_Year+"/"
if args.ApplyZPtRwg:
  m.OutputDirectory = ENV_PLOT_PATH+"/"+dataset+"/CR/"+str_Year+"/ApplyZPtRwg/"
if args.ApplyDYReshape:
  m.OutputDirectory = ENV_PLOT_PATH+"/"+dataset+"/CR/"+str_Year+"/ApplyZPtRwg_ApplyDYReshape/"

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
  "DYReshapeEEMM",
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
if args.ApplyDYReshape:
  SampleGroup_DY_2016.Samples=['DYJets_MG_HT_Reweighted_Reshaped']
  SampleGroup_DY_2017.Samples=['DYJets_MG_HT_Reweighted_Reshaped']
  SampleGroup_DY_2018.Samples=['DYJets_MG_HT_Reweighted_Reshaped']

###############
#### DY CR ####
###############

if args.Category==0:
  #### Define Samples
  if args.Year>0:
    exec('m.SampleGroups = [SampleGroup_Others_%s, SampleGroup_NonPrompt_%s, SampleGroup_TT_TW_%s, SampleGroup_DY_%s]'%(args.Year,args.Year,args.Year,args.Year))
  else:
    m.SampleGroups = [
      SampleGroup_Others_2016, SampleGroup_Others_2017, SampleGroup_Others_2018,
      SampleGroup_NonPrompt_2016, SampleGroup_NonPrompt_2017, SampleGroup_NonPrompt_2018,
      SampleGroup_TT_TW_2016, SampleGroup_TT_TW_2017, SampleGroup_TT_TW_2018,
      SampleGroup_DY_2016, SampleGroup_DY_2017, SampleGroup_DY_2018,
    ]

  #### Signals
  #### Print
  m.PrintSamples()

  #### Define reiongs
  m.RegionsToDraw = [
    ## 60<mll<150
    Region('HNWR_SingleElectron_Resolved_DYCR', 'SingleElectron', UnblindData=True, Logy=1, TLatexAlias='#splitline{ee}{Resolved DY CR}'),
    Region('HNWR_SingleMuon_Resolved_DYCR', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{#mu#mu}{Resolved DY CR}'),
    Region('HNWR_SingleElectron_Boosted_DYCR', 'SingleElectron', UnblindData=True, Logy=1, TLatexAlias='#splitline{ee}{Boosted DY CR}'),
    Region('HNWR_SingleMuon_Boosted_DYCR', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{#mu#mu}{Boosted DY CR}'),
    ## 60<mll<100
    Region('HNWR_SingleElectron_Resolved_DYCR1', 'SingleElectron', UnblindData=True, Logy=1, TLatexAlias='#splitline{ee}{Resolved DY CR1}'),
    Region('HNWR_SingleMuon_Resolved_DYCR1', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{#mu#mu}{Resolved DY CR1}'),
    Region('HNWR_SingleElectron_Boosted_DYCR1', 'SingleElectron', UnblindData=True, Logy=1, TLatexAlias='#splitline{ee}{Boosted DY CR1}'),
    Region('HNWR_SingleMuon_Boosted_DYCR1', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{#mu#mu}{Boosted DY CR1}'),
    ## 100<mll<150
    Region('HNWR_SingleElectron_Resolved_DYCR2', 'SingleElectron', UnblindData=True, Logy=1, TLatexAlias='#splitline{ee}{Resolved DY CR2}'),
    Region('HNWR_SingleMuon_Resolved_DYCR2', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{#mu#mu}{Resolved DY CR2}'),
    Region('HNWR_SingleElectron_Boosted_DYCR2', 'SingleElectron', UnblindData=True, Logy=1, TLatexAlias='#splitline{ee}{Boosted DY CR2}'),
    Region('HNWR_SingleMuon_Boosted_DYCR2', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{#mu#mu}{Boosted DY CR2}'),
    ## 200<mll<400
    Region('HNWR_SingleElectron_Resolved_DYCR3', 'SingleElectron', UnblindData=True, Logy=1, TLatexAlias='#splitline{ee}{Resolved DY CR3}'),
    Region('HNWR_SingleMuon_Resolved_DYCR3', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{#mu#mu}{Resolved DY CR3}'),
    Region('HNWR_SingleElectron_Boosted_DYCR3', 'SingleElectron', UnblindData=True, Logy=1, TLatexAlias='#splitline{ee}{Boosted DY CR3}'),
    Region('HNWR_SingleMuon_Boosted_DYCR3', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{#mu#mu}{Boosted DY CR3}'),
  ]
  m.PrintRegions()

######################
#### emu sideband ####
######################

if args.Category==1:
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

  #### Signals
  #### Print
  m.PrintSamples()

  #### Define reiongs
  m.RegionsToDraw = [
    ## Resolved EMu
    Region('HNWR_EMu_Resolved_SR', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{e#mu}{Resolved flavor CR}'),
    ## Boosted EMu
    Region('HNWR_SingleElectron_EMu_Boosted_CR', 'SingleElectron', UnblindData=True, Logy=1, TLatexAlias='#splitline{e+#mu-Jet}{Boosted flavor CR}'),
    Region('HNWR_SingleMuon_EMu_Boosted_CR', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{#mu+e-Jet}{Boosted flavor CR}'),
    ## Resolved EMu, but DYCR. This is dominated by ttbar, so it is here..
    ## This region is not important
    Region('HNWR_EMu_Resolved_DYCR', 'SingleMuon', UnblindData=True, Logy=1, TLatexAlias='#splitline{e#mu}{Resolved DY CR}'),
    ## LowWRCR emu regions
    Region('HNWR_EMu_Resolved_LowWRCR', 'SingleMuon', UnblindData=True, Logy=-1, TLatexAlias='#splitline{e#mu}{Resolved low-mass CR}'),
    Region('HNWR_SingleElectron_EMu_Boosted_LowWRCR', 'SingleElectron', UnblindData=True, Logy=-1, TLatexAlias='#splitline{e+#mu-Jet}{Boosted low-mass CR}'),
    Region('HNWR_SingleMuon_EMu_Boosted_LowWRCR', 'SingleMuon', UnblindData=True, Logy=-1, TLatexAlias='#splitline{#mu+e-Jet}{Boosted low-mass CR}'),
    ## Resolved LowWRCR for validation
    Region('HNWR_SingleElectron_Resolved_LowWRCR', 'SingleElectron', UnblindData=True, Logy=-1, TLatexAlias='#splitline{ee}{Resolved low-mass CR}'),
    Region('HNWR_SingleMuon_Resolved_LowWRCR', 'SingleMuon', UnblindData=True, Logy=-1, TLatexAlias='#splitline{#mu#mu}{Resolved low-mass CR}'),
    ## Boosted LowWRCR for validation
    Region('HNWR_SingleElectron_Boosted_LowWRCR', 'SingleElectron', UnblindData=True, Logy=-1, TLatexAlias='#splitline{ee}{Boosted low-mass CR}'),
    Region('HNWR_SingleMuon_Boosted_LowWRCR', 'SingleMuon', UnblindData=True, Logy=-1, TLatexAlias='#splitline{#mu#mu}{Boosted low-mass CR}'),
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
  Variable('Jet_0_Pt', 'p_{T} of the leading jet', 'GeV'),
  Variable('Jet_1_Pt', 'p_{T} of the subleading jet', 'GeV'),
  Variable('DiJet_Pt', 'p_{T} of the dijet', 'GeV'),
  Variable('DiJet_Mass', 'm(jj)', 'GeV'),
  Variable('Jet_0_Eta', '#eta of the leading jet', 'GeV'),
  Variable('Jet_1_Eta', '#eta of the subleading jet', 'GeV'),
  Variable('dRj1j2', '#Delta R(j1,j2)', ''),
]
#m.VariablesToDraw = [
#  Variable('WRCand_Mass', 'm_{W_{R}} (GeV)', 'GeV'),
#  Variable('ZCand_Pt', 'p_{T}^{ll} (GeV)', 'GeV'),
#  Variable('NEvent', 'onebin', ''),
#]
m.PrintVariables()

#### Draw
m.Draw()
