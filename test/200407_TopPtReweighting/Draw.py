import os,ROOT
import mylib,canvas_margin
import tdrstyle

tdrstyle.setTDRStyle()

Sample = "TTLL_powheg"

Years = [
"2016",
"2017",
"2018",
]

Regions = [
['HNWR_EMu_Resolved_SR','Resolved e#mu CR'],
['HNWR_SingleElectron_EMu_Boosted_CR','Boosted e#mu CR w/ #mu-Jet'],
['HNWR_SingleMuon_EMu_Boosted_CR','Boosted e#mu CR w/ e-Jet'],
]

outDirBase = "./output/"

for Year in Years:
  outDir = outDirBase+Year+"/"
  f_Without = ROOT.TFile("Without/"+Year+"/HNWRAnalyzer_SkimTree_LRSMHighPt_"+Sample+".root")
  f_With = ROOT.TFile("With/"+Year+"/HNWRAnalyzer_SkimTree_LRSMHighPt_"+Sample+".root")
  for iRegion in Regions:

    Region = iRegion[0]
    RegionLatex = iRegion[1]

    h_Without = f_Without.Get(Region+'/WRCand_Mass_'+Region)
    h_Without = mylib.RebinWRMass(h_Without, Region, int(Year))
    h_Without.GetXaxis().SetRangeUser(800,8000)
    h_Without.SetLineWidth(3)
    h_Without.SetLineColor(ROOT.kBlack)

    h_With = f_With.Get(Region+'/WRCand_Mass_'+Region)
    h_With = mylib.RebinWRMass(h_With, Region, int(Year))
    h_With.GetXaxis().SetRangeUser(800,8000)
    h_With.SetLineWidth(3)
    h_With.SetLineColor(ROOT.kRed)

    c1 = ROOT.TCanvas('c1','',800,800)
    c1_up = ROOT.TPad("c1_up", "", 0, 0.25, 1, 1)
    c1_down = ROOT.TPad("c1_down", "", 0, 0, 1, 0.25)
    c1, c1_up, c1_down = canvas_margin.canvas_margin(c1, c1_up, c1_down)
    c1.Draw()
    c1_up.Draw()
    c1_down.Draw()

    c1_up.cd()
    c1_up.SetLogy(True)
    h_Without.Draw("histe1same")
    h_Without.GetYaxis().SetTitle("Events / bin")
    h_With.Draw("histe1same")

    c1.cd()
    lg = ROOT.TLegend(0.55, 0.7, 0.90, 0.90)
    lg.SetBorderSize(0)
    lg.SetFillStyle(0)
    lg.AddEntry(h_Without, 't#bar{t}, without reweighting', 'l')
    lg.AddEntry(h_With, 't#bar{t}, with top p_{T} reweighting', 'l')
    lg.Draw()

    str_channel = "#splitline{e#mu}{"+RegionLatex+"}"
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

    c1_down.cd()
    h_Ratio = h_With.Clone()
    h_Ratio.Divide(h_Without)
    h_Without, h_Ratio = canvas_margin.hist_axis(h_Without,h_Ratio)

    h_Ratio.SetLineWidth(3)
    h_Ratio.SetLineColor(ROOT.kBlack)
    h_Ratio.GetYaxis().SetRangeUser(0.8,1.1)
    xtitle = 'm(lljj) (GeV)'
    if 'Boosted' in Region:
      xtitle = 'm(lJ) (GeV)'
    h_Ratio.GetXaxis().SetTitle(xtitle)
    h_Ratio.GetYaxis().SetTitle("With/Without")
    h_Ratio.SetNdivisions(504,"Y")

    h_Ratio.Draw("histsame")

    c1.SaveAs(outDir+'/'+Region+".pdf")
    c1.Close()
  f_Without.Close()
  f_With.Close()
