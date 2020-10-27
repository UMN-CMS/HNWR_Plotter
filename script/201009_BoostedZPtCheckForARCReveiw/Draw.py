import os,ROOT
import mylib
import canvas_margin
import tdrstyle
import CMS_lumi, tdrstyle
import math
from array import array

tdrstyle.setTDRStyle()

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

varName = 'GenZ_Mass'

Years = [
"2016",
"2017",
"2018",
]

Channels = [
"Electron",
"Muon",
]

Regions = [
"Resolved",
"Boosted",
]

nRebin = 50
PtBins = [
0, 2, 4, 6, 8, 10, 12, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 225, 250, 350, 500, 1000, 2000, 80000
]
if varName=='GenZ_Mass':
  PtBins = [
50, 60, 70, 80, 85, 86, 87, 88, 88.5, 89, 89.2, 89.4, 89.6, 89.8, 90, 90.2, 90.4, 90.6, 90.8, 91, 91.2, 91.4, 91.6, 91.8, 92, 92.2, 92.4, 92.6, 92.8, 93, 93.2, 93.4, 93.6, 93.8, 94, 95, 100, 110, 120, 180, 200, 300, 400, 500 , 1000, 2000
]

NPtBin = len(PtBins)-1

for Year in Years:

  outDir = PLOT_PATH+'/'+dataset+'/201009_ZPtCheckForARCReveiw/'+Year+'/'
  os.system('mkdir -p '+outDir)

  for Channel in Channels:

    f = ROOT.TFile('rootfiles/'+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT_Reweighted.root')

    c1 = ROOT.TCanvas('c1', '', 600, 600)

    h_dummy = ROOT.TH1D('h_dummy', '', 8000, 0., 8000)
    h_dummy = h_dummy.Rebin(NPtBin, h_dummy.GetName(), array("d", PtBins) )

    h_dummy.GetYaxis().SetLabelSize(0.04)
    h_dummy.GetYaxis().SetTitleSize(0.070)
    h_dummy.GetYaxis().SetTitleOffset(1.10)

    h_dummy.GetXaxis().SetLabelSize(0.04)

    h_dummy.GetYaxis().SetTitle('Normalized')
    h_dummy.GetXaxis().SetTitle(varName+' (GeV)')

    h_dummy.Draw('hist')

    h_Resolved = f.Get('HNWR_Single'+Channel+'_Resolved_SR/'+varName+'_HNWR_Single'+Channel+'_Resolved_SR')
    h_Resolved = h_Resolved.Rebin(NPtBin, h_Resolved.GetName(), array("d", PtBins) )
    h_Resolved.Scale(1./h_Resolved.Integral())
    h_Resolved.SetLineColor(ROOT.kBlack)

    h_Boosted  = f.Get('HNWR_Single'+Channel+'_Boosted_SR/'+varName+'_HNWR_Single'+Channel+'_Boosted_SR')
    h_Boosted = h_Boosted.Rebin(NPtBin, h_Boosted.GetName(), array("d", PtBins) )
    h_Boosted.Scale(1./h_Boosted.Integral())
    h_Boosted.SetLineColor(ROOT.kRed)

    h_Resolved.Draw('histsame')
    h_Boosted.Draw('histsame')

    yMax = max( h_Resolved.GetMaximum(), h_Boosted.GetMaximum() )
    h_dummy.GetYaxis().SetRangeUser(0., 1.2*yMax)

    h_dummy.GetXaxis().SetRangeUser(0., 2000.)

    chLegend = 'ee' if (Channel=='Electron') else '#mu#mu'
    lg = ROOT.TLegend(0.55, 0.60, 0.9, 0.9)
    lg.AddEntry(h_Resolved, 'Resolved (%s)'%(chLegend), 'l')
    lg.AddEntry(h_Boosted, 'Boosted (%s)'%(chLegend), 'l')
    lg.Draw()

    latex_CMSPriliminary = ROOT.TLatex()
    latex_Lumi = ROOT.TLatex()

    latex_CMSPriliminary.SetNDC()
    latex_Lumi.SetNDC()
    latex_CMSPriliminary.SetTextSize(0.035)
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}")

    latex_Lumi.SetTextSize(0.035)
    latex_Lumi.SetTextFont(42)
    latex_Lumi.DrawLatex(0.73, 0.96, mylib.TotalLumi(float(Year))+" fb^{-1} (13 TeV)")

    c1.SaveAs(outDir+'/'+Year+'_'+Channel+'_'+varName+'.pdf')
    c1.Close()

  # rootfiles/2016/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT_Reweighted.root
