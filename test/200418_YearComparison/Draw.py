import os,ROOT,math
import CMS_lumi, tdrstyle
from array import array
import mylib,canvas_margin
import copy

tdrstyle.setTDRStyle()
ROOT.TH1.AddDirectory(False)

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

Years = [
"2016",
"2017",
"2018",
]
Colors = [
ROOT.kBlack,
ROOT.kBlue,
ROOT.kRed,
]

Regions = [
"HNWR_EMu_Resolved_LowWRCR",

#"HNWR_SingleElectron_Resolved_SR",
#"HNWR_SingleElectron_Boosted_SR",
"HNWR_SingleElectron_Boosted_LowWRCR",

#"HNWR_SingleMuon_Resolved_SR",
#"HNWR_SingleMuon_Boosted_SR",
"HNWR_SingleMuon_Boosted_LowWRCR",
]

Samples = [
'AllMCBkgd',
'TTLX_powheg',
'data',
]

#varName = 'WRCand_Mass'
#nRebin = -1
#xMin = 800
#xMax = 8000
#xTitle = 'm(lljj) (GeV)'

varName = 'WRCand_Mass'
nRebin = 20
xMin = 0
xMax = 800
xTitle = 'm(lljj) (GeV)'

for Sample in Samples:

  for Region in Regions:

    c1 = ROOT.TCanvas('c1', '', 800, 800)

    c1_up = ROOT.TPad("c1_up", "", 0, 0.25, 1, 1)
    c1_down = ROOT.TPad("c1_down", "", 0, 0, 1, 0.25)
    c1, c1_up, c1_down = canvas_margin.canvas_margin(c1, c1_up, c1_down)
    c1.Draw()
    c1_up.Draw()
    c1_down.Draw()

    c1_up.cd()
    c1_up.SetLogy(True)

    lg = ROOT.TLegend(0.55, 0.7, 0.90, 0.90)
    lg.SetBorderSize(0)
    lg.SetFillStyle(0)

    c1.cd()

    str_channel = ""
    channelname = ROOT.TLatex()
    channelname.SetNDC()
    channelname.SetTextSize(0.037)
    channelname.DrawLatex(0.2, 0.88, str_channel)

    latex_CMSPriliminary = ROOT.TLatex()
    latex_Lumi = ROOT.TLatex()

    latex_CMSPriliminary.SetNDC()
    latex_Lumi.SetNDC()
    latex_CMSPriliminary.SetTextSize(0.035)
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS Simulation} #font[42]{#it{#scale[0.8]{Preliminary}}}")

    c1_up.cd()
    hist_dummy_up = ROOT.TH1D('hist_dummy_up', '', 8000, 0., 8000.)
    if nRebin<0:
      hist_dummy_up = mylib.RebinWRMass(hist_dummy_up, Region, 2018, True)
    else:
      hist_dummy_up.Rebin(nRebin)
    hist_dummy_up.GetXaxis().SetRangeUser(xMin, xMax)
    hist_dummy_up.GetYaxis().SetRangeUser(1E-2, 200)
    if 'LowWR' in Region:
      hist_dummy_up.GetYaxis().SetRangeUser(1E-2, 1000)
    hist_dummy_up.GetYaxis().SetTitle("A.U.")
    hist_dummy_up.Draw("hist")

    c1_down.cd()
    hist_dummy_down = ROOT.TH1D('hist_dummy_down', '', 8000, 0., 8000.)
    if nRebin<0:
      hist_dummy_down = mylib.RebinWRMass(hist_dummy_down, Region, 2018, True)
    else:
      hist_dummy_down.Rebin(nRebin)
    hist_dummy_down.GetXaxis().SetRangeUser(xMin, xMax)
    xtitle = xTitle
    if 'Boosted' in Region:
      xtitle = 'm(lJ) (GeV)'
    hist_dummy_down.GetXaxis().SetTitle(xtitle)
    hist_dummy_down.GetYaxis().SetRangeUser(0.5, 1.5)
    hist_dummy_down.SetNdivisions(504,"Y")
    hist_dummy_down.Draw("hist")

    g1_x = [-9000, 9000]
    g1_y = [1, 1]
    g1 = ROOT.TGraph(2, array("d", g1_x ), array("d", g1_y ))
    g1.Draw("same")

    hist_dummy_up, hist_dummy_down = canvas_margin.hist_axis(hist_dummy_up, hist_dummy_down)

    h_Ref = 0

    histDicts = dict()

    outDir = PLOT_PATH+'/'+dataset+'/200418_YearComparison/'
    os.system('mkdir -p '+outDir)

    for i_Year in range(0,len(Years)):

      Year = Years[i_Year]
      Color = Colors[i_Year]

      basedir = FILE_PATH+'/'+dataset+'/Regions/'+str(Year)+'/'

      t_Sample = Sample
      if Sample=="data":
        if Region=="HNWR_EMu_Resolved_LowWRCR":
          t_Sample = "data_SingleMuon"
        elif 'SingleMuon' in Region:
          t_Sample = "data_SingleMuon"
        else:
          t_Sample = "data_SingleElectron"
      f_TT = ROOT.TFile(basedir+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+t_Sample+'.root')
      h = f_TT.Get(Region+'/'+varName+'_'+Region)
      h.SetLineWidth(3)
      h.SetLineColor(Color)
      histDicts[Year+'_'+varName+'_'+Region] = h.Clone()

      if nRebin<0:
        h = mylib.RebinWRMass(h, Region, 2018,True)
      else:
        h.Rebin(nRebin)

      h.Scale(1./float(mylib.TotalLumi(int(Year))))
      histDicts[Year+'_'+varName+'_'+Region] = h.Clone()

      lg.AddEntry(h, Year, "l")

      c1_up.cd()
      h.Draw("histsame")

      c1_down.cd()

      if h_Ref==0:
        h_Ref = h.Clone("h_Ref")
      else:
        h_Ratio = histDicts[Year+'_'+varName+'_'+Region]
        h_Ratio.Divide(h_Ref)
        h_Ratio.SetMarkerSize(0)
        h_Ratio.SetLineWidth(3)
        h_Ratio.SetLineColor(Color)
        h_Ratio.Draw("histe1same")

      c1.cd()
      lg.Draw()

      c1_up.cd()
      hist_dummy_up.Draw("axissame")

      c1_down.cd()
      hist_dummy_down.Draw("axissame")

    c1.SaveAs(outDir+'/'+Region+'_'+Sample+'.pdf')
    c1.Close()

