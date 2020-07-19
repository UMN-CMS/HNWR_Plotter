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

RegionSets = [
[
  ["HNWR_EMu_Resolved_SR", "e#mu", ROOT.kBlack],
  ["HNWR_SingleMuon_Resolved_SR", "#mu#mu", ROOT.kBlue],
  ["HNWR_SingleElectron_Resolved_SR", "ee", ROOT.kRed],
],
[
  ["HNWR_SingleElectron_EMu_Boosted_CR", "e#mu", ROOT.kBlack],
  ["HNWR_SingleMuon_Boosted_SR", "#mu#mu", ROOT.kBlue],
],
[
  ["HNWR_SingleMuon_EMu_Boosted_CR", "e#mu", ROOT.kBlack],
  ["HNWR_SingleElectron_Boosted_SR", "ee", ROOT.kRed],
],
]

for Year in Years:

  basedir = FILE_PATH+'/'+dataset+'/Regions/'+str(Year)+'/'
  outDir = PLOT_PATH+'/'+dataset+'/TTBarShapes/'+str(Year)+'/'
  os.system('mkdir -p '+outDir)

  f_TT = ROOT.TFile(basedir+'/HNWRAnalyzer_SkimTree_LRSMHighPt_TTLX_powheg.root')

  for RegionSet in RegionSets:

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

    latex_Lumi.SetTextSize(0.035)
    latex_Lumi.SetTextFont(42)
    latex_Lumi.DrawLatex(0.73, 0.96, mylib.TotalLumi(float(Year))+" fb^{-1} (13 TeV)")

    c1_up.cd()
    hist_dummy_up = ROOT.TH1D('hist_dummy_up', '', 800, 0., 8000.)
    hist_dummy_up = mylib.RebinWRMass(hist_dummy_up, RegionSet[0][0], int(Year))
    hist_dummy_up.GetXaxis().SetRangeUser(800, 8000)
    hist_dummy_up.GetYaxis().SetRangeUser(1E-4, 1)
    hist_dummy_up.GetYaxis().SetTitle("A.U.")
    hist_dummy_up.Draw("hist")

    c1_down.cd()
    hist_dummy_down = ROOT.TH1D('hist_dummy_down', '', 800, 0., 8000.)
    hist_dummy_down = mylib.RebinWRMass(hist_dummy_down, RegionSet[0][0], int(Year))
    hist_dummy_down.GetXaxis().SetRangeUser(800, 8000)
    xtitle = 'm(lljj) (GeV)'
    if 'Boosted' in RegionSet[0][0]:
      xtitle = 'm(lJ) (GeV)'
    hist_dummy_down.GetXaxis().SetTitle(xtitle)
    hist_dummy_down.GetYaxis().SetRangeUser(0.5, 3.0)
    hist_dummy_down.SetNdivisions(504,"Y")
    hist_dummy_down.Draw("hist")

    g1_x = [-9000, 9000]
    g1_y = [1, 1]
    g1 = ROOT.TGraph(2, array("d", g1_x ), array("d", g1_y ))
    g1.Draw("same")

    hist_dummy_up, hist_dummy_down = canvas_margin.hist_axis(hist_dummy_up, hist_dummy_down)

    h_Ref = 0
    y_max, y_min = -999, 9999999999


    #### test shape uncertainty from em/ee/mm shapes
    shapeUnctName = ''
    if RegionSet[0][0]=='HNWR_EMu_Resolved_SR':
      shapeUnctName = 'ResolvedShapeUnct'
    elif RegionSet[0][0]=='HNWR_SingleElectron_EMu_Boosted_CR':
      shapeUnctName = 'BoostedMuJetShapeUnct'
    elif RegionSet[0][0]=='HNWR_SingleMuon_EMu_Boosted_CR':
      shapeUnctName = 'BoostedEJetShapeUnct'
    f_out = ROOT.TFile(outDir+'shapes_'+shapeUnctName+'.root','RECREATE')

    h_out_Up = hist_dummy_up.Clone()
    h_out_Up.SetName(shapeUnctName+'Up')
    h_out_Up.GetYaxis().SetRangeUser(0,999999)
    h_out_Down = hist_dummy_up.Clone()
    h_out_Down.SetName(shapeUnctName+'Down')
    h_out_Down.GetYaxis().SetRangeUser(0,999999)
    for z in range(0, h_out_Up.GetXaxis().GetNbins()):
      h_out_Up.SetBinContent(z+1, 1)
      h_out_Down.SetBinContent(z+1, 1)

    for i_Region in range(0,len(RegionSet)):

      Region = RegionSet[i_Region][0]
      RegionAlias = RegionSet[i_Region][1]
      Color = RegionSet[i_Region][2]

      h = f_TT.Get(Region+'/WRCand_Mass_'+Region)
      h = mylib.RebinWRMass(h, Region, int(Year))
      h.Scale(1./h.Integral())
      h.SetLineWidth(3)
      h.SetLineColor(Color)

      y_max = max( y_max, h.GetMaximum() )
      y_min = min( y_min, h.GetMinimum() )

      lg.AddEntry(h, RegionAlias, "l")

      c1_up.cd()
      h.Draw("histe1same")

      c1_down.cd()
      h_Ratio = ROOT.TH1D('h_Ratio', '', 800, 0., 8000.)
      h_Ratio = mylib.RebinWRMass(h_Ratio, RegionSet[0][0], int(Year))
      if h_Ref==0:
        h_Ref = h.Clone("h_Ref")
      else:
        for z in range(0, h_Ratio.GetXaxis().GetNbins()):
          r = h.GetBinContent(z+1)/h_Ref.GetBinContent(z+1)

          y_Num = h.GetBinContent(z+1)
          e_Num = h.GetBinError(z+1)/y_Num
          y_Den = h_Ref.GetBinContent(z+1)
          e_Den = h_Ref.GetBinError(z+1)/y_Den

          r = y_Num/y_Den
          r_Err = math.sqrt( e_Num*e_Num + e_Den*e_Den ) * r

          h_Ratio.SetBinContent(z+1, r)
          h_Ratio.SetBinError(z+1, r_Err)

          h_out_Up.SetBinContent(z+1, max(h_out_Up.GetBinContent(z+1), r))
          h_out_Down.SetBinContent(z+1, min(h_out_Down.GetBinContent(z+1), r))

        h_Ratio.SetMarkerSize(0)
        h_Ratio.SetLineWidth(3)
        h_Ratio.SetLineColor(Color)
        h_Ratio.Draw("histe1same")

    c1.cd()
    lg.Draw()

    c1_up.cd()
    hist_dummy_up.GetYaxis().SetRangeUser(0.1*y_min, 10*y_max)
    hist_dummy_up.Draw("axissame")

    c1_down.cd()
    hist_dummy_down.Draw("axissame")

    #### test
    h_out_Up.SetLineColor(ROOT.kGreen)
    h_out_Up.SetLineStyle(3)
    h_out_Up.SetLineWidth(3)
    h_out_Up.Draw("histsame")
    h_out_Down.SetLineColor(ROOT.kGreen)
    h_out_Down.SetLineStyle(3)
    h_out_Down.SetLineWidth(3)
    h_out_Down.Draw("histsame")

    f_out.cd()
    h_out_Up.Write()
    h_out_Down.Write()
    f_out.Close()

    c1.SaveAs(outDir+"/"+RegionSet[0][0]+".pdf")
    c1.Close()

#### 2) Syst

Systs = [
    "JetResUp", "JetResDown",
    "JetEnUp", "JetEnDown",

    "MuonRecoSFUp", "MuonRecoSFDown",
    "MuonEnUp", "MuonEnDown",
    "MuonIDSFUp", "MuonIDSFDown",
    "MuonISOSFUp", "MuonISOSFDown",
    "MuonTriggerSFUp", "MuonTriggerSFDown",

    "ElectronRecoSFUp", "ElectronRecoSFDown",
    "ElectronResUp", "ElectronResDown",
    "ElectronEnUp", "ElectronEnDown",
    "ElectronIDSFUp", "ElectronIDSFDown",
    "ElectronTriggerSFUp", "ElectronTriggerSFDown",

    "PUUp", "PUDown",
]
Colors = [
ROOT.kRed,
ROOT.kOrange,

ROOT.kYellow+1,
ROOT.kGreen,
ROOT.kBlue,
ROOT.kViolet,
ROOT.kGray,

ROOT.kYellow+1,
ROOT.kGreen,
ROOT.kBlue,
ROOT.kViolet,
ROOT.kGray,

ROOT.kCyan,
]

Regions = [
  "Resolved",
  "Boosted",
]
Channels = [
  "EE",
  "MuMu",
]

for Year in Years:

  basedir = FILE_PATH+'/'+dataset+'/Regions/'+str(Year)+'/'
  outDir = PLOT_PATH+'/'+dataset+'/TTBarShapes/'+str(Year)+'/'
  os.system('mkdir -p '+outDir)

  f_TT = ROOT.TFile(basedir+'/HNWRAnalyzer_SkimTree_LRSMHighPt_TTLX_powheg.root')

  for Channel in Channels:
    PD = "Electron"
    if Channel=="MuMu":
      PD = "Muon"
    for Region in Regions:

  
      dirName = "HNWR_Single"+PD+"_"+Region+"_SR"

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

      latex_Lumi.SetTextSize(0.035)
      latex_Lumi.SetTextFont(42)
      latex_Lumi.DrawLatex(0.73, 0.96, mylib.TotalLumi(float(Year))+" fb^{-1} (13 TeV)")

      c1_up.cd()
      hist_dummy_up = ROOT.TH1D('hist_dummy_up', '', 800, 0., 8000.)
      hist_dummy_up = mylib.RebinWRMass(hist_dummy_up, dirName, int(Year))
      hist_dummy_up.GetXaxis().SetRangeUser(800, 8000)
      hist_dummy_up.GetYaxis().SetTitle("Events / bin")
      hist_dummy_up.Draw("hist")

      c1_down.cd()
      hist_dummy_down = ROOT.TH1D('hist_dummy_down', '', 800, 0., 8000.)
      hist_dummy_down = mylib.RebinWRMass(hist_dummy_down, dirName, int(Year))
      hist_dummy_down.GetXaxis().SetRangeUser(800, 8000)
      xtitle = 'm(lljj) (GeV)'
      if Region=='Boosted':
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

      c1_up.cd()
      h_Central = f_TT.Get(dirName+'/WRCand_Mass_'+dirName)
      h_Central = mylib.RebinWRMass(h_Central, dirName, int(Year))
      h_Central.SetLineWidth(2)
      h_Central.SetLineColor(ROOT.kBlack)
      h_Central.Draw("histsame")
      y_max = -999
      y_min = 9999999999
      y_max = h_Central.GetMaximum()
      y_min = h_Central.GetMinimum()

      for i_Syst in range(0,len(Systs)):

        Syst = Systs[i_Syst]
        Color = Colors[int(i_Syst/2)]

        if Channel=="EE" and 'Muon' in Syst:
          continue
        if Channel=="MuMu" and 'Electron' in Syst:
          continue
        if Region=="Resolved" and 'LSF' in Syst:
          continue

        systDirName = 'Syst_'+Syst+'_'+dirName

        h = f_TT.Get(systDirName+'/WRCand_Mass_'+systDirName)
        h = mylib.RebinWRMass(h, dirName, int(Year))
        h.SetLineWidth(1)
        h.SetLineColor(Color)

        if 'Up' in Syst:
          lg.AddEntry(h, Syst.replace('Up',''), 'l')

        y_max = max( y_max, h.GetMaximum() )
        y_min = min( y_min, h.GetMinimum() )

        c1_up.cd()
        h.Draw("histsame")

        c1_down.cd()
        h_Ratio = ROOT.TH1D('h_Ratio', '', 800, 0., 8000.)
        h_Ratio = mylib.RebinWRMass(h_Ratio, dirName, int(Year))
        for z in range(0, h_Ratio.GetXaxis().GetNbins()):
          r = h.GetBinContent(z+1)/h_Central.GetBinContent(z+1)

          y_Num = h.GetBinContent(z+1)
          e_Num = h.GetBinError(z+1)/y_Num
          y_Den = h_Central.GetBinContent(z+1)
          e_Den = h_Central.GetBinError(z+1)/y_Den

          r = y_Num/y_Den
          r_Err = math.sqrt( e_Num*e_Num + e_Den*e_Den ) * r

          h_Ratio.SetBinContent(z+1, r)
          h_Ratio.SetBinError(z+1, r_Err)
        h_Ratio.SetMarkerSize(0)
        h_Ratio.SetLineWidth(1)
        h_Ratio.SetLineColor(Color)
        h_Ratio.Draw("histsame")

      c1.cd()
      lg.Draw()

      c1_up.cd()
      h_Central.Draw("histsame")
      hist_dummy_up.GetYaxis().SetRangeUser(0.1*y_min, 10*y_max)
      hist_dummy_up.Draw("axissame")
      c1_down.cd()
      hist_dummy_down.Draw("axissame")

      c1.SaveAs(outDir+"/Syst_"+dirName+".pdf")
      c1.Close()

