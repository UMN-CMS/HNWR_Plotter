import os,ROOT,math
import CMS_lumi, tdrstyle

ROOT.TH1.AddDirectory(False)

tdrstyle.setTDRStyle()

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

histName = 'FatJet_0_LSF'

Year = 2016

basedir = FILE_PATH+'/'+dataset+'/LSFDistribution/'+str(Year)+'/'

outDir = PLOT_PATH+'/'+dataset+'/LSFDistribution/'+str(Year)+'/'
os.system('mkdir -p '+outDir)

BkgdSamples = [
'QCD_MuEnrichedPt5',
#'SkimTree_LRSMHighPt_DYJets',
#'SkimTree_LRSMHighPt_TTLL_powheg',
]
BkgdColors = [
ROOT.kRed,
]

SigMasses = [
'WR1000_N100',
'WR1000_N200',
'WR3000_N100',
'WR3000_N200',
'WR5000_N100',
'WR5000_N200',
]
SigColors = [
ROOT.kBlack,
ROOT.kGray,
ROOT.kBlue,
ROOT.kGreen,
ROOT.kViolet,
ROOT.kOrange,
]

c1 = ROOT.TCanvas('c1','',600,600)
c1.cd()

hist_dummy = ROOT.TH1D('hist_dummy', '', 100, 0., 1.)
hist_dummy.Draw('hist')
hist_dummy.GetYaxis().SetRangeUser(0., 1.)
hist_dummy.GetYaxis().SetTitle('Arbitrary unit')
hist_dummy.GetXaxis().SetTitle('LSF_{3}')

lg = ROOT.TLegend(0.2, 0.65, 0.8, 0.9)
lg.SetBorderSize(0)
lg.SetFillStyle(0)
lg.SetEntrySeparation(0.25)
lg.SetMargin(0.15)

#### Bkgds
for i_Bkgd in range(0,len(BkgdSamples)):

  BkgdSample = BkgdSamples[i_Bkgd]
  BkgdColor = BkgdColors[i_Bkgd]

  #### filename
  fname = 'MyPlayGround_'+BkgdSample+'.root'

  #### Get TFile
  f = ROOT.TFile(basedir+fname)

  h = f.Get(histName)
  h.Rebin(5)
  h.Scale(1./h.Integral())

  eff = h.Integral( h.FindBin(0.7), 999 )
  print BkgdSample+'\t'+'%1.3f'%(eff*100.)

  h.SetLineWidth(3)
  h.SetLineColor(BkgdColor)
  h.SetLineStyle(3)

  lg.AddEntry(h, BkgdSample, 'l')

  h.Draw("histsame")

#### Signals
for i_Sig in range(0,len(SigMasses)):

  Mass = SigMasses[i_Sig]
  Color = SigColors[i_Sig]

  mWR = Mass.split('_')[0].replace('WR','')
  mN = Mass.split('_')[1].replace('N','')

  #### filename
  fname = 'MyPlayGround_WRtoNLtoLLJJ_'+Mass+'.root'

  #### Get TFile
  f = ROOT.TFile(basedir+fname)

  h = f.Get(histName)
  h.Rebin(5)
  h.Scale(1./h.Integral())

  h.SetLineWidth(3)
  h.SetLineColor(Color)

  lg.AddEntry(h, 'm_{W_{R}}=%s GeV, m_{N}=%s GeV'%(mWR,mN), 'l')

  eff = h.Integral( h.FindBin(0.7), 999 )
  print Mass+'\t'+'%1.3f'%(eff*100.)

  h.Draw("histsame")

lg.Draw()

c1.SaveAs(outDir+histName+'.pdf')
c1.Close()
