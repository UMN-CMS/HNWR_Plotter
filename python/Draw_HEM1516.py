import os,ROOT,math
import CMS_lumi, tdrstyle
from array import array

def GetMaximumHist(h, ErrorScale=1.):

  maxval = -9999.
  for i in range(0,h.GetXaxis().GetNbins()):

    y = h.GetBinContent(i+1)
    err = h.GetBinError(i+1)

    if (y+ErrorScale*err > maxval):
      maxval = y+ErrorScale*err

  return maxval

ROOT.TH1.AddDirectory(False)

tdrstyle.setTDRStyle()

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

Year = '2018'
basedir = FILE_PATH+'/'+dataset+'/HEM1516/'+Year+'/'
outDir = PLOT_PATH+'/'+dataset+'/HEM1516/'+Year+'/'
os.system('mkdir -p '+outDir)

PDs = [
'EGamma',
'SingleMuon',
]
Periods = [
'AB',
'CD',
'ABCD',
]

VariableSets = [
['WRCand_Mass', 100, 800., 8000., 'm(lj) GeV)'],
#['FatJet_0_Energy', 200, 200., 2000., 'E(J) (GeV)'],
#['FatJet_0_Pt', 200, 200., 2000., 'p_{T}(J) (GeV)'],
]

etaphis = [
'Eta1_Phi1',
'Eta1_Phi-1',
'Eta-1_Phi1',
'Eta-1_Phi-1',
]
colors = [
ROOT.kBlack,
ROOT.kBlue,
ROOT.kGreen,
ROOT.kRed,
]
aliases = [
'#eta>0 & #phi>0',
'#eta>0 & #phi<0',
'#eta<0 & #phi>0',
'#eta<0 & #phi<0 (HEM 15/16)',
]

for VariableSet in VariableSets:

  Variable = VariableSet[0]
  NRebin = VariableSet[1]
  XMin = VariableSet[2]
  XMax = VariableSet[3]

  for PD in PDs:

    TriggerLepton = 'Electron'
    if PD=='SingleMuon':
      TriggerLepton = 'Muon'

    Region = 'HNWR_Single'+TriggerLepton+'_EMu_Boosted_CR'

    for Period in Periods:

      f = ROOT.TFile(basedir+'/HNWRAnalyzer_SkimTree_LRSMHighPt_EGamma_'+Period+'.root')

      tdir = f.Get(Region)
      if not tdir:
        print '@@@@ Directory not found'
        print '@@@@ filename = '+f.GetName()
        print '@@@@ dirname = '+Region

      c1 = ROOT.TCanvas('c1','',600,600)
      c1.cd()

      hist_dummy = ROOT.TH1D('hist_dummy', '', 8000, 0., 8000.)
      hist_dummy.Draw('hist')
      hist_dummy.GetYaxis().SetTitle('Events')
      hist_dummy.GetXaxis().SetTitle('m(lJ) (GeV)')

      hist_dummy.GetXaxis().SetRangeUser(XMin,XMax)
      #hist_dummy.GetYaxis().SetRangeUser(0.9,1E2)

      lg = ROOT.TLegend(0.6, 0.65, 0.95, 0.9)
      lg.SetBorderSize(0)
      lg.SetFillStyle(0)
      lg.SetEntrySeparation(0.25)
      lg.SetMargin(0.15)

      y_max = -1
      dict_hists = dict()
      for i_etaphi in range(0,len(etaphis)):

        etaphi = etaphis[i_etaphi]
        color = colors[i_etaphi]
        alias = aliases[i_etaphi]

        h = tdir.Get(Variable+'_'+etaphi+'_'+Region)

        if Variable=='WRCand_Mass':
          vec_bins = [800, 1000, 1200, 1500, 1800, 8000]
          n_bin = len(vec_bins)-1
          h = h.Rebin(n_bin, h.GetName(), array("d", vec_bins) )
        else:
          h.Rebin(NRebin)
        h.SetLineWidth(2)
        h.SetLineColor(color)
        lg.AddEntry(h, alias, 'le')

        y_max = max(y_max, GetMaximumHist(h))

        dict_hists[etaphi] = h
        h.Draw('pe1same')

      hist_dummy.GetYaxis().SetRangeUser(0., 1.2*y_max)

      #c1.SetLogy()

      lg.Draw()

      c1.SaveAs(outDir+'/'+Region+'_'+Variable+'_'+Period+'.pdf')
      c1.Close()

      f.Close()
