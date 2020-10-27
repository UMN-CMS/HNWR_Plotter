import os,ROOT,tdrstyle
from array import array

tdrstyle.setTDRStyle()
ROOT.TH1.AddDirectory(False)

histSuffix = 'Data'

Years = [
"2016",
"2017",
"2018",
]
Colors = [
ROOT.kBlack,
ROOT.kRed,
ROOT.kBlue,
]

#### Trigger SF
fileNames = [
'HighPtMuonTriggerSF_Run2016.root',
'HighPtMuonTriggerSF_Run2017.root',
'HighPtMuonTriggerSF_Run2018.root',
]
histNames = [
'Eff_'+histSuffix,
'Eff_'+histSuffix,
'Eff_'+histSuffix,
]
#histNames = [
#'Eff_MC',
#'Eff_MC',
#'Eff_MC',
#]

c1 = ROOT.TCanvas('c1','',600,600)

h_1Ds = []
gr_1Ds = []

lg = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)

for i in range(0,3):

  Year = Years[i]
  fileName = fileNames[i]
  histName = histNames[i]
  Color = Colors[i]

  #### x : pt, y : eta
  f_2D = ROOT.TFile(Year+'/'+fileName)
  h_2D = f_2D.Get(histName)

  xaxis = h_2D.GetXaxis()

  xbins = [xaxis.GetBinLowEdge(1)]
  for j in range(0,xaxis.GetNbins()):
    xbins.append( xaxis.GetBinUpEdge(j+1) )

  h_1D = ROOT.TH1D(Year,'',xaxis.GetNbins(),array('d',xbins))
  for j in range(0,xaxis.GetNbins()):
    this_eff = h_2D.GetBinContent(j+1,1)
    h_1D.SetBinContent(j+1, this_eff )
    h_1D.SetBinError(j+1, 0 )

  h_1D.SetLineColor(Color)
  h_1D.SetMarkerColor(Color)
  h_1D.SetLineWidth(2)

  h_1D.GetXaxis().SetRangeUser(20,1000)
  h_1D.GetXaxis().SetLabelSize(0.035)
  h_1D.GetXaxis().SetTitle("p_{T} (GeV)")

  h_1D.GetYaxis().SetTitle(histSuffix+" trigger efficiency")
  h_1D.GetYaxis().SetRangeUser(0.8,1.05)
  h_1D.GetYaxis().SetLabelSize(0.035)
  h_1Ds.append( h_1D )

  gr_1D = ROOT.TGraphAsymmErrors(h_1D)
  gr_1Ds.append( gr_1D )

  c1.cd()

  lg.AddEntry(h_1D, Year, 'l')

  h_1D.Draw("axissame")
  gr_1D.Draw("lpsame")

lg.Draw()
c1.SaveAs("all_"+histSuffix+".pdf")
 
