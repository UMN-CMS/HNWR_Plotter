import os,ROOT
import tdrstyle
from PUProbData import *

def TotalLumi(Year):

  if Year==2016:
    return "35.92"
  elif Year==2017:
    return "41.53"
  elif Year==2018:
    return "59.74"
  else:
    print "WTF"
    return "35.9";

Year = "2016"

pu1 = prob_DATA_2016
pu1_Up = prob_DATA_2016_Up
pu1_Down = prob_DATA_2016_Down

pu2_Full = prob_MC_2016
pu2_Fast = prob_MC_Fast_2016

#### Change to yours
outdir = '/data6/Users/jskim/HNWR_Plotter/output/Run2Legacy_v4__Default/PUDistribution/'

outname = "Fast_"+str(Year)

NBin = len(pu1)
## check number
print '@@@@ # of values of pu DATA = '+str(len(pu1))
print '@@@@ # of values of pu MC (Full) = '+str(len(pu2_Full))
print '@@@@ # of values of pu MC (Fast) = '+str(len(pu2_Fast))

## normalizaion
Sum1 = 0
Sum1_Up = 0
Sum1_Down = 0
Sum2_Full = 0
Sum2_Fast = 0
for i in range(0,NBin):
  Sum1 += pu1[i]
  Sum1_Up += pu1_Up[i]
  Sum1_Down += pu1_Down[i]
  Sum2_Full += pu2_Full[i]
  if i<len(pu2_Fast):
    Sum2_Fast += pu2_Fast[i]

for i in range(0,NBin):
  pu1[i] /= Sum1
  pu1_Up[i] /= Sum1_Up
  pu1_Down[i] /= Sum1_Down
  pu2_Full[i] /= Sum2_Full
  if i<len(pu2_Fast):
    pu2_Fast[i] /= Sum2_Fast

tdrstyle.setTDRStyle()

hist_1 = ROOT.TH1D('PUProb_DATA', '', NBin, 0., float(NBin))
hist_1_Up = ROOT.TH1D('PUProb_DATA_Up', '', NBin, 0., float(NBin))
hist_1_Down = ROOT.TH1D('PUProb_DATA_Down', '', NBin, 0., float(NBin))
hist_2_Full = ROOT.TH1D('PUProb_MC_Full', '', NBin, 0., float(NBin))
hist_2_Fast = ROOT.TH1D('PUProb_MC_Fast', '', NBin, 0., float(NBin))


for i in range(0,NBin):
  hist_1.SetBinContent(i+1,pu1[i])
  hist_1_Up.SetBinContent(i+1,pu1_Up[i])
  hist_1_Down.SetBinContent(i+1,pu1_Down[i])
  hist_2_Full.SetBinContent(i+1,pu2_Full[i])
  if i<len(pu2_Fast):
    hist_2_Fast.SetBinContent(i+1,pu2_Fast[i])

hist_1.SetLineColor(1)
hist_1.SetLineWidth(3)
hist_1_Up.SetLineColor(1)
hist_1_Up.SetLineStyle(3)
hist_1_Up.SetLineWidth(3)
hist_1_Down.SetLineColor(1)
hist_1_Down.SetLineStyle(5)
hist_1_Down.SetLineWidth(3)

hist_2_Full.SetLineColor(ROOT.kBlue)
hist_2_Full.SetLineWidth(3)

hist_2_Fast.SetLineColor(ROOT.kRed)
hist_2_Fast.SetLineWidth(3)

y_max = max( hist_2_Full, max( hist_1.GetMaximum(), max( hist_1_Up.GetMaximum(), hist_1_Down.GetMaximum() ) ) )

c1 = ROOT.TCanvas('c1','',800,800)
c1.cd()
hist_1.Draw('histsame')
hist_1_Up.Draw('histsame')
hist_1_Down.Draw('histsame')
hist_2_Full.Draw('histsame')
hist_2_Fast.Draw('histsame')

hist_1.GetXaxis().SetTitle("Number of pileup")
hist_1.GetXaxis().SetRangeUser(0.,50.)
hist_1.GetYaxis().SetTitle("Prob.")
hist_1.GetYaxis().SetRangeUser(0., 1.1*y_max)

lg = ROOT.TLegend(0.65, 0.75, 0.9, 0.9)
lg.AddEntry(hist_1, 'DATA', 'l')
lg.AddEntry(hist_1_Up, 'DATA (+1 s.d.)', 'l')
lg.AddEntry(hist_1_Down, 'DATA (-1 s.d.)', 'l')
lg.AddEntry(hist_2_Full, 'MC (FullSim)', 'l')
lg.AddEntry(hist_2_Fast, 'MC (FastSim)', 'l')
lg.Draw()

latex_Lumi = ROOT.TLatex()
latex_Lumi.SetNDC()
latex_Lumi.SetTextSize(0.035)
latex_Lumi.SetTextFont(42)
latex_Lumi.DrawLatex(0.72, 0.96, TotalLumi(Year)+" fb^{-1} (13 TeV)")

c1.SaveAs(outdir+'/DATAvsMC_'+outname+'.pdf')
c1.SaveAs(outdir+'/DATAvsMC_'+outname+'.png')
c1.Close()

