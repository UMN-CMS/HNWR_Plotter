import os,ROOT,math
import CMS_lumi, tdrstyle, mylib, canvas_margin
from array import array

Syst = 'Central'
Prefix = ''
if Syst!='Central':
  Prefix = 'Syst_'+Syst+'_'

def RebinWRMass2(hist, region, DataYear):

  lastbin = hist.GetXaxis().GetNbins()
  vec_bins = [800, 1400, 1600, 2000, 8000]
  if "Boosted" in region:
    vec_bins = [800, 1800, 8000]

  #if ('LowWR' in region) or ('DYCR' in region):
  if ('LowWR' in region):
    tmp_vec_bins = [0, 200, 300, 400, 500, 600, 700, 800]
    for b in vec_bins:
      tmp_vec_bins.append(b)
    vec_bins = tmp_vec_bins

  n_bin = len(vec_bins)-1
  hist = hist.Rebin(n_bin, hist.GetName(), array("d", vec_bins) )
  return hist

def Rebin(hist, region, var, nRebin, Year):
  if var=='WRCand_Mass':
    #return RebinWRMass2(hist, region, Year)
    return mylib.RebinWRMass(hist, region, Year)
  else:
    if nRebin>0:
      hist.Rebin(nRebin)
      return hist
    else:
      return hist

ROOT.TH1.AddDirectory(False)

tdrstyle.setTDRStyle()

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

Years = [
"2016",
"2017",
"2018",
]

Regions = [
'Resolved',
'Boosted'
]

for Year in Years:

  basedir = FILE_PATH+'/'+dataset+'/Regions/'+Year+'/'
  outDir = PLOT_PATH+'/'+dataset+'/CompareDY/'+Year+'/'
  os.system('mkdir -p '+outDir)

  f = ROOT.TFile(PLOT_PATH+'/'+dataset+'/CompareDY/'+Year+'/shapes_DY.root')
  f_DATAoverLO = ROOT.TFile(PLOT_PATH+'/'+dataset+'/DYReshapeRatio/'+Year+'/shapes.root')

  for Region in Regions:

    h_Ratio_DY_Electron = f.Get('Ratio_HNWR_SingleElectron_'+Region+'_DYCR')
    h_Ratio_SR_Electron = f.Get('Ratio_HNWR_SingleElectron_'+Region+'_SR')
    h_Ratio_DY_Muon = f.Get('Ratio_HNWR_SingleMuon_'+Region+'_DYCR')
    h_Ratio_SR_Muon = f.Get('Ratio_HNWR_SingleMuon_'+Region+'_SR')

    #### ee+mm
    ## NLO
    h_NLO_DY_Electron = f.Get('NLO_HNWR_SingleElectron_'+Region+'_DYCR')
    h_NLO_SR_Electron = f.Get('NLO_HNWR_SingleElectron_'+Region+'_SR')
    h_NLO_DY_Muon = f.Get('NLO_HNWR_SingleMuon_'+Region+'_DYCR')
    h_NLO_SR_Muon = f.Get('NLO_HNWR_SingleMuon_'+Region+'_SR')
    ## LO
    h_LO_DY_Electron = f.Get('LO_HNWR_SingleElectron_'+Region+'_DYCR')
    h_LO_SR_Electron = f.Get('LO_HNWR_SingleElectron_'+Region+'_SR')
    h_LO_DY_Muon = f.Get('LO_HNWR_SingleMuon_'+Region+'_DYCR')
    h_LO_SR_Muon = f.Get('LO_HNWR_SingleMuon_'+Region+'_SR')
    ## Add mm to ee, NLO
    h_NLO_DY_Electron.Add( h_NLO_DY_Muon )
    h_NLO_SR_Electron.Add( h_NLO_SR_Muon )
    ## Add mm to ee, LO
    h_LO_DY_Electron.Add( h_LO_DY_Muon )
    h_LO_SR_Electron.Add( h_LO_SR_Muon )
    ## Make eemm NLO/LO ratio
    h_Ratio_DY_All = h_NLO_DY_Electron.Clone('h_Ratio_DY_All')
    h_Ratio_SR_All = h_NLO_SR_Electron.Clone('h_Ratio_SR_All')
    h_Ratio_DY_All.Divide( h_LO_DY_Electron )
    h_Ratio_SR_All.Divide( h_LO_SR_Electron )

    c1 = ROOT.TCanvas('c1', '', 800, 800)

    c1_up = ROOT.TPad("c1_up", "", 0, 0.25, 1, 1)
    c1_down = ROOT.TPad("c1_down", "", 0, 0, 1, 0.25)
    c1, c1_up, c1_down = canvas_margin.canvas_margin(c1, c1_up, c1_down)
    c1.Draw()
    c1_up.Draw()
    c1_down.Draw()

    c1_up.cd()

    h_Ratio_DY_Electron.SetLineColor(ROOT.kBlue)
    h_Ratio_SR_Electron.SetLineColor(ROOT.kBlue)
    h_Ratio_DY_Muon.SetLineColor(ROOT.kRed)
    h_Ratio_SR_Muon.SetLineColor(ROOT.kRed)
    h_Ratio_DY_All.SetLineColor(ROOT.kBlack)
    h_Ratio_SR_All.SetLineColor(ROOT.kBlack)

    h_Ratio_DY_Electron.SetLineStyle(1)
    h_Ratio_SR_Electron.SetLineStyle(3)
    h_Ratio_DY_Muon.SetLineStyle(1)
    h_Ratio_SR_Muon.SetLineStyle(3)
    h_Ratio_DY_All.SetLineStyle(1)
    h_Ratio_SR_All.SetLineStyle(3)

    ## Legend
    lg = ROOT.TLegend(0.5, 0.70, 0.92, 0.91)
    lg.SetBorderSize(0)
    lg.SetFillStyle(0)
    #lg.AddEntry(h_Ratio_DY_Electron, 'Ratio_{DYCR}, ee', 'l')
    #lg.AddEntry(h_Ratio_SR_Electron, 'Ratio_{SR}, ee', 'l')
    #lg.AddEntry(h_Ratio_DY_Muon, 'Ratio_{DYCR}, #mu#mu', 'l')
    #lg.AddEntry(h_Ratio_SR_Muon, 'Ratio_{SR}, #mu#mu', 'l')
    lg.AddEntry(h_Ratio_DY_All, 'Ratio_{DYCR}, ee+#mu#mu', 'l')
    lg.AddEntry(h_Ratio_SR_All, 'Ratio_{SR}, ee+#mu#mu', 'l')

    ## Copy NLO axis
    NLOAxis = h_Ratio_DY_Electron.GetXaxis()
    nBin = NLOAxis.GetNbins()
    xBins = [NLOAxis.GetBinLowEdge(1)]
    for ix in range(0,nBin):
      xBins.append( NLOAxis.GetBinUpEdge(ix+1) )
    xBins = array("d",xBins)

    h_dummy_up = ROOT.TH1D('h_dumy_up', '', nBin, xBins)
    h_dummy_up.GetYaxis().SetRangeUser(0, 2.0)
    h_dummy_up.GetYaxis().SetTitle('NLO/LO ratio')

    h_dummy_down = ROOT.TH1D('h_dumy_down', '', nBin, xBins)
    h_dummy_down.GetYaxis().SetRangeUser(0.,2.0)
    h_dummy_down.SetNdivisions(504,"Y")
    h_dummy_down.GetXaxis().SetTitle('m(WR_{RECO}) (GeV)')
    h_dummy_down.GetYaxis().SetRangeUser(0,2.0)
    h_dummy_down.GetYaxis().SetTitle("#frac{Ratio_{SR}}{Ratio_{DYCR}}")
    h_dummy_down.SetFillColor(0)
    h_dummy_down.SetMarkerSize(0)
    h_dummy_down.SetMarkerStyle(0)
    h_dummy_down.SetLineColor(ROOT.kWhite)

    h_dummy_up, h_dummy_down = canvas_margin.hist_axis(h_dummy_up, h_dummy_down)

    ## draw up
    c1_up.cd()
    h_dummy_up.Draw('axis')

    #h_Ratio_DY_Electron.Draw('histe1same')
    #h_Ratio_SR_Electron.Draw('histe1same')
    #h_Ratio_DY_Muon.Draw('histe1same')
    #h_Ratio_SR_Muon.Draw('histe1same')
    h_Ratio_DY_All.Draw('histe1same')
    h_Ratio_SR_All.Draw('histe1same')

    h_dummy_up.Draw("axissame")

    lg.Draw()

    ## draw down
    c1_down.cd()

    h_dummy_down.Draw('axis')

    h_Ratio_SRoverDY_Electron = h_Ratio_SR_Electron.Clone('h_Ratio_SRoverDY_Electron')
    h_Ratio_SRoverDY_Electron.Divide(h_Ratio_DY_Electron)

    h_Ratio_SRoverDY_Muon = h_Ratio_SR_Muon.Clone('h_Ratio_SRoverDY_Muon')
    h_Ratio_SRoverDY_Muon.Divide(h_Ratio_DY_Muon)

    h_Ratio_SRoverDY_All = h_Ratio_SR_All.Clone('h_Ratio_SRoverDY_All')
    h_Ratio_SRoverDY_All.Divide(h_Ratio_DY_All)

    h_Ratio_SRoverDY_Electron.SetLineStyle(1)
    h_Ratio_SRoverDY_Muon.SetLineStyle(1)
    h_Ratio_SRoverDY_All.SetLineStyle(1)

    #h_Ratio_SRoverDY_Electron.Draw('histe1same')
    #h_Ratio_SRoverDY_Muon.Draw('histe1same')
    h_Ratio_SRoverDY_All.Draw('histe1same')
 
    g1_x = [-9000, 9000]
    g1_y = [1, 1]
    g1 = ROOT.TGraph(2, array("d", g1_x ), array("d", g1_y ))
    g1.Draw("same")

    ## TLatex
    c1.cd()

    latex_CMSPriliminary = ROOT.TLatex()
    latex_Lumi = ROOT.TLatex()

    latex_CMSPriliminary.SetNDC()
    latex_Lumi.SetNDC()
    latex_CMSPriliminary.SetTextSize(0.035)
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}")

    latex_Lumi.SetTextSize(0.035)
    latex_Lumi.SetTextFont(42)
    latex_Lumi.DrawLatex(0.73, 0.96, mylib.TotalLumi(float(Year))+" fb^{-1} (13 TeV)")

    ## Save

    c1.SaveAs(outDir+'/RatioComparison_%s.pdf'%(Region))
    c1.Close()


    ##########################################
    #### Data/LO and NLO/LO at the DY CR

    c2 = ROOT.TCanvas('c2', '', 800, 800)

    c2_up = ROOT.TPad("c2_up", "", 0, 0.25, 1, 1)
    c2_down = ROOT.TPad("c2_down", "", 0, 0, 1, 0.25)
    c2, c2_up, c2_down = canvas_margin.canvas_margin(c2, c2_up, c2_down)
    c2.Draw()
    c2_up.Draw()
    c2_down.Draw()

    c2_up.cd()

    #### NLO/LO at the CR
    #### Data/LO at the CR from shapes.root
    h_DATAoverLO = f_DATAoverLO.Get(Region+'_ratio_AllCh')
    h_DATAoverLO = Rebin(h_DATAoverLO, Region, 'WRCand_Mass', -1, Year)
    h_DATAoverLO.SetLineColor(ROOT.kBlue)
    h_DATAoverLO.SetLineWidth(2)

    ## Legend
    lg2 = ROOT.TLegend(0.5, 0.70, 0.92, 0.91)
    lg2.SetBorderSize(0)
    lg2.SetFillStyle(0)
    lg2.AddEntry(h_Ratio_DY_All, 'NLO/LO', 'l')
    lg2.AddEntry(h_DATAoverLO, 'Data/LO', 'l')

    h_dummy_up.GetYaxis().SetRangeUser(0, 2.0)
    h_dummy_up.GetYaxis().SetTitle('')

    h_dummy_down.GetYaxis().SetRangeUser(0.,2.0)
    h_dummy_down.SetNdivisions(504,"Y")
    h_dummy_down.GetXaxis().SetTitle('m(WR_{RECO}) (GeV)')
    h_dummy_down.GetYaxis().SetRangeUser(0,2.0)
    h_dummy_down.GetYaxis().SetTitle("#frac{NLO/LO}{Data/LO}")
    h_dummy_down.SetFillColor(0)
    h_dummy_down.SetMarkerSize(0)
    h_dummy_down.SetMarkerStyle(0)
    h_dummy_down.SetLineColor(ROOT.kWhite)

    ## draw up
    c2_up.cd()
    h_dummy_up.Draw('axis')

    h_Ratio_DY_All.Draw('histe1same')
    h_DATAoverLO.Draw('histe1same')

    h_dummy_up.Draw("axissame")

    lg2.Draw()

    ## draw down
    c2_down.cd()

    h_dummy_down.Draw('axis')

    #### Make ratio
    h_zzzz = h_Ratio_DY_All.Clone('h_zzzz')
    h_zzzz.Divide(h_DATAoverLO)
    h_zzzz.Draw('histe1same')

    g1.Draw("same")

    ## TLatex
    c2.cd()

    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}")
    latex_Lumi.DrawLatex(0.73, 0.96, mylib.TotalLumi(float(Year))+" fb^{-1} (13 TeV)")

    ## Save

    c2.SaveAs(outDir+'/NLOoverLO_vs_DataoverLO_%s.pdf'%(Region))
    c2.Close()

