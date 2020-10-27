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

Years = [
"2016",
]

Channels = [
"Electron",
"Muon",
]

for Year in Years:

  outDir = PLOT_PATH+'/'+dataset+'/201009_ZPtCheckForARCReveiw/'+Year+'/'
  os.system('mkdir -p '+outDir)

  lines_Before = open('2016_Before.txt').readlines()
  lines_After = open('2016_After.txt').readlines()

  for Channel in Channels:

    x_ls = []
    x_rs = []

    Bkgds_Before = []
    Bkgds_After = []

    ratios_Before = []
    ratios_After = []

    for il in range(0, len(lines_Before)):

      line_Before = lines_Before[il]
      line_After = lines_After[il]

      words_Before = line_Before.split()
      words_After = line_After.split()

      if Channel in words_Before[0]:
        x_ls.append( words_Before[1] )
        x_rs.append( words_Before[2] )

        Bkgds_Before.append( float(words_Before[4]) )
        Bkgds_After.append( float(words_After[4]) )

        ratios_Before.append( float(words_Before[5]) )
        ratios_After.append( float(words_After[5]) )

    print x_ls
    print x_rs
    print Bkgds_Before
    print Bkgds_After
    print ratios_Before
    print ratios_After

    c1 = ROOT.TCanvas('c1', '', 800, 800)
    c1_up = ROOT.TPad("c1_up", "", 0, 0.25, 1, 1)
    c1_down = ROOT.TPad("c1_down", "", 0, 0, 1, 0.25)
    c1, c1_up, c1_down = canvas_margin.canvas_margin(c1, c1_up, c1_down)
    c1.Draw()
    c1_up.Draw()
    c1_down.Draw()

    c1_up.cd()

    h_Before = ROOT.TH1D('h_Before', '', 20, 0., 1000.)
    h_After = h_Before.Clone('h_Before')
    h_Ratio = h_Before.Clone('h_Ratio')

    for ix in range(0, len(ratios_Before)):

      h_Before.SetBinContent(ix+1, ratios_Before[ix])
      h_After.SetBinContent(ix+1, ratios_After[ix])
      h_Ratio.SetBinContent(ix+1, ratios_After[ix] - ratios_Before[ix])
      #print ratios_After[ix] - ratios_Before[ix]

    h_Before.SetLineColor(ROOT.kBlack)
    h_Before.Draw("hist")
    h_Before.GetXaxis().SetTitle('p_{T}(ll) (GeV)')
    h_Before.GetYaxis().SetTitle('Data/Sim.')
    h_Before.GetYaxis().SetRangeUser(0, 2.0)

    h_Before.SetMarkerSize(0)
    h_After.SetMarkerSize(0)
    h_After.SetLineColor(ROOT.kRed)
    h_After.Draw("histsame")

    chLegend = 'ee' if (Channel=='Electron') else '#mu#mu'
    lg = ROOT.TLegend(0.60, 0.65, 0.9, 0.9)
    lg.AddEntry(h_Before, 'Before', 'l')
    lg.AddEntry(h_After, 'After', 'l')
    lg.Draw()

    ## Down

    c1_down.cd()

    h_Ratio.Draw('hist')
    h_Ratio.GetYaxis().SetRangeUser(-0.20, 0.20)
    h_Ratio.SetNdivisions(504,"Y")

    ## Overall

    c1.cd()

    h_Before, h_Ratio = canvas_margin.hist_axis(h_Before, h_Ratio)
    h_Ratio.GetYaxis().SetTitle('After - Before')
    h_Ratio.GetXaxis().SetTitle('p_{T}(ll) (GeV)')

    latex_CMSPriliminary = ROOT.TLatex()
    latex_Lumi = ROOT.TLatex()

    latex_CMSPriliminary.SetNDC()
    latex_Lumi.SetNDC()
    latex_CMSPriliminary.SetTextSize(0.035)
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}")

    latex_Lumi.SetTextSize(0.035)
    latex_Lumi.SetTextFont(42)
    latex_Lumi.DrawLatex(0.73, 0.96, mylib.TotalLumi(float(Year))+" fb^{-1} (13 TeV)")

    channelname = ROOT.TLatex()
    channelname.SetNDC()
    channelname.SetTextSize(0.037)
    channelname.DrawLatex(0.2, 0.88, Channel+' channel')

    c1.SaveAs(outDir+'/'+Year+'_'+Channel+'_BeforeVSAfter__DATAoverBkgd.pdf')
    c1.Close()

    #################################################################################################################

    ###############
    #### Sim only
    ###############

    c2 = ROOT.TCanvas('c2', '', 800, 800)
    c2_up = ROOT.TPad("c2_up", "", 0, 0.25, 1, 1)
    c2_down = ROOT.TPad("c2_down", "", 0, 0, 1, 0.25)
    c2, c2_up, c2_down = canvas_margin.canvas_margin(c2, c2_up, c2_down)
    c2.Draw()
    c2_up.Draw()
    c2_down.Draw()

    c2_up.cd()

    h_Before = ROOT.TH1D('h_Before', '', 20, 0., 1000.)
    h_After = h_Before.Clone('h_Before')

    for ix in range(0, len(Bkgds_Before)):

      h_Before.SetBinContent(ix+1, Bkgds_Before[ix])
      h_After.SetBinContent(ix+1, Bkgds_After[ix])

    h_Ratio = h_After.Clone('h_Ratio')
    h_Ratio.Divide(h_Before)

    h_Before.SetLineColor(ROOT.kBlack)
    h_Before.Draw("hist")
    h_Before.GetXaxis().SetTitle('p_{T}(ll) (GeV)')
    h_Before.GetYaxis().SetTitle('Simulation / bin')
    h_Before.GetYaxis().SetRangeUser(1, 2E3)

    h_Before.SetMarkerSize(0)
    h_After.SetMarkerSize(0)
    h_After.SetLineColor(ROOT.kRed)
    h_After.Draw("histsame")

    chLegend = 'ee' if (Channel=='Electron') else '#mu#mu'
    lg = ROOT.TLegend(0.60, 0.65, 0.9, 0.9)
    lg.AddEntry(h_Before, 'Before', 'l')
    lg.AddEntry(h_After, 'After', 'l')
    lg.Draw()

    c2_up.SetLogy()

    ## Down

    c2_down.cd()

    h_Ratio.Draw('hist')
    h_Ratio.GetYaxis().SetRangeUser(0.80, 1.20)

    ## Overall

    c2.cd()

    h_Before, h_Ratio = canvas_margin.hist_axis(h_Before, h_Ratio)
    h_Before.GetXaxis().SetTitle('Simulation')
    h_Ratio.GetYaxis().SetTitle('After/Before')
    h_Ratio.GetXaxis().SetTitle('p_{T}(ll) (GeV)')

    latex_CMSPriliminary = ROOT.TLatex()
    latex_Lumi = ROOT.TLatex()

    latex_CMSPriliminary.SetNDC()
    latex_Lumi.SetNDC()
    latex_CMSPriliminary.SetTextSize(0.035)
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}")

    latex_Lumi.SetTextSize(0.035)
    latex_Lumi.SetTextFont(42)
    latex_Lumi.DrawLatex(0.73, 0.96, mylib.TotalLumi(float(Year))+" fb^{-1} (13 TeV)")

    channelname.DrawLatex(0.2, 0.88, Channel+' channel')

    c2.SaveAs(outDir+'/'+Year+'_'+Channel+'_BeforeVSAfter__Bkgd.pdf')
    c2.Close()
