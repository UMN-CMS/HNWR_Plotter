import os,ROOT
import tdrstyle
from PUProbData import *

Year = 2018

exec('pu1 = prob_DATA_'+str(Year))
exec('pu2 = prob_MC_'+str(Year))

#### Change to yours
outdir = '/eos/user/j/jskim/www/Public/HNWR_13TeV/Run2LegacyPileUpInfo/'
outname = str(Year)

## check number
if len(pu1) != len(pu2):
  print '# of values of pu1 = '+str(len(pu1))
  print '# of values of pu2 = '+str(len(pu2))
  exit()

NBin = len(pu1)

## normalizaion
Sum1 = 0
Sum2 = 0
for i in range(0,NBin):
  Sum1 += pu1[i]
  Sum2 += pu2[i]
SFs = []
for i in range(0,NBin):
  pu1[i] /= Sum1
  pu2[i] /= Sum2
  SFs.append( pu1[i]/pu2[i] )

tdrstyle.setTDRStyle()

hist_1 = ROOT.TH1D('PUProb_DATA', '', NBin, 0., float(NBin))
hist_2 = ROOT.TH1D('PUProb_MC', '', NBin, 0., float(NBin))
hist_SF = ROOT.TH1D('PUReweight_'+outname, '', NBin, 0., float(NBin))

for i in range(0,NBin):
  hist_1.SetBinContent(i+1,pu1[i])
  hist_2.SetBinContent(i+1,pu2[i])
  hist_SF.SetBinContent(i+1,SFs[i])

hist_1.SetLineColor(1)
hist_2.SetLineColor(2)

c1 = ROOT.TCanvas('c1','',800,800)
c1.cd()
hist_1.Draw('histsame')
hist_2.Draw('histsame')

hist_1.GetXaxis().SetTitle("NVtx")
hist_1.GetYaxis().SetTitle("Prob.")

lg = ROOT.TLegend(0.65, 0.75, 0.9, 0.9)
lg.AddEntry(hist_1, 'DATA', 'l')
lg.AddEntry(hist_2, 'MC', 'l')
lg.Draw()

c1.SaveAs(outdir+'/DATAvsMC_'+outname+'.pdf')
c1.SaveAs(outdir+'/DATAvsMC_'+outname+'.png')
c1.Close()

c2 = ROOT.TCanvas('c1','',800,800)
c2.cd()
hist_SF.Draw('histsame')
hist_SF.GetXaxis().SetTitle("NVtx")
hist_SF.GetYaxis().SetTitle("PU Reweight")
hist_SF.GetYaxis().SetRangeUser(0.,4.0)
c2.SaveAs(outdir+'/PUReweight_'+outname+'.pdf')
c2.SaveAs(outdir+'/PUReweight_'+outname+'.png')

outroot = ROOT.TFile(outdir+'/PUReweight_'+outname+'.root','RECREATE')
outroot.cd()
hist_1.Write()
hist_2.Write()
hist_SF.Write()
outroot.Close()

c2.Close()
