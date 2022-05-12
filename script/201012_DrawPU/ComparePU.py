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
    print "WTF : "+str(Year)
    return "35.9";

Years = [
2016,
2017,
2018,
]
IsFast = False

for Year in Years:

  exec('pu1 = prob_DATA_'+str(Year))
  exec('pu1_Up = prob_DATA_'+str(Year)+'_Up')
  exec('pu1_Down = prob_DATA_'+str(Year)+'_Down')
  if IsFast:
    exec('pu2 = prob_MC_Fast_'+str(Year))
  else:
    exec('pu2 = prob_MC_'+str(Year))

  #### Change to yours
  #outdir = '/home/jskim/cernbox/www/Public/HNWR_13TeV/Run2LegacyPileUpInfo/' ## my laptop
  #outdir = '/eos/user/j/jskim/www/Public/HNWR_13TeV/Run2LegacyPileUpInfo/' ## lxplus
  outdir = '/data6/Users/jskim/HNWR_Plotter/output/Run2Legacy_v4__Default/PUDistribution/'

  if IsFast:
    outname = "Fast_"+str(Year)
  else:
    outname = str(Year)

  NBin = len(pu1)
  ## check number
  if len(pu1) != len(pu2):
    print '@@@@ # of values of pu1 = '+str(len(pu1))
    print '@@@@ # of values of pu2 = '+str(len(pu2))
    NBin = min( len(pu1), len(pu2) )
    print '@@@@ NBin = min(pu1,pu2) = '+str(NBin)
    #exit()

  ## normalizaion
  Sum1 = 0
  Sum1_Up = 0
  Sum1_Down = 0
  Sum2 = 0
  for i in range(0,NBin):
    Sum1 += pu1[i]
    Sum1_Up += pu1_Up[i]
    Sum1_Down += pu1_Down[i]
    Sum2 += pu2[i]
  SFs = []
  SFs_Up = []
  SFs_Down = []
  for i in range(0,NBin):
    pu1[i] /= Sum1
    pu1_Up[i] /= Sum1_Up
    pu1_Down[i] /= Sum1_Down
    pu2[i] /= Sum2

    if pu2[i]==0:
      SFs.append( 0. )
      SFs_Up.append( 0. )
      SFs_Down.append( 0. )
    else:
      SFs.append( pu1[i]/pu2[i] )
      SFs_Up.append( pu1_Up[i]/pu2[i] )
      SFs_Down.append( pu1_Down[i]/pu2[i] )

  tdrstyle.setTDRStyle()

  hist_1 = ROOT.TH1D('PUProb_DATA', '', NBin, 0., float(NBin))
  hist_1_Up = ROOT.TH1D('PUProb_DATA_Up', '', NBin, 0., float(NBin))
  hist_1_Down = ROOT.TH1D('PUProb_DATA_Down', '', NBin, 0., float(NBin))
  hist_2 = ROOT.TH1D('PUProb_MC', '', NBin, 0., float(NBin))
  hist_SF = ROOT.TH1D('PUReweight_'+str(Year), '', NBin, 0., float(NBin))
  hist_SF_Up = ROOT.TH1D('PUReweight_'+str(Year)+'_Up', '', NBin, 0., float(NBin))
  hist_SF_Down = ROOT.TH1D('PUReweight_'+str(Year)+'_Down', '', NBin, 0., float(NBin))

  for i in range(0,NBin):
    hist_1.SetBinContent(i+1,pu1[i])
    hist_1_Up.SetBinContent(i+1,pu1_Up[i])
    hist_1_Down.SetBinContent(i+1,pu1_Down[i])
    hist_2.SetBinContent(i+1,pu2[i])
    hist_SF.SetBinContent(i+1,SFs[i])
    hist_SF_Up.SetBinContent(i+1,SFs_Up[i])
    hist_SF_Down.SetBinContent(i+1,SFs_Down[i])

  hist_1.SetLineColor(1)
  hist_1.SetLineWidth(3)
  hist_1_Up.SetLineColor(1)
  hist_1_Up.SetLineStyle(3)
  hist_1_Up.SetLineWidth(3)
  hist_1_Down.SetLineColor(1)
  hist_1_Down.SetLineStyle(5)
  hist_1_Down.SetLineWidth(3)
  hist_2.SetLineColor(2)
  hist_2.SetLineWidth(3)

  y_max = max( hist_2, max( hist_1.GetMaximum(), max( hist_1_Up.GetMaximum(), hist_1_Down.GetMaximum() ) ) )

  c1 = ROOT.TCanvas('c1','',800,800)
  c1.cd()
  hist_1.Draw('histsame')
  hist_1_Up.Draw('histsame')
  hist_1_Down.Draw('histsame')
  hist_2.Draw('histsame')

  hist_1.GetXaxis().SetTitle("Number of pileup")
  hist_1.GetYaxis().SetTitle("Prob.")
  hist_1.GetYaxis().SetRangeUser(0., 1.1*y_max)

  lg = ROOT.TLegend(0.65, 0.75, 0.9, 0.9)
  lg.AddEntry(hist_1, 'DATA', 'l')
  lg.AddEntry(hist_1_Up, 'DATA (+1 s.d.)', 'l')
  lg.AddEntry(hist_1_Down, 'DATA (-1 s.d.)', 'l')
  lg.AddEntry(hist_2, 'MC', 'l')
  lg.Draw()

  latex_Lumi = ROOT.TLatex()
  latex_Lumi.SetNDC()
  latex_Lumi.SetTextSize(0.035)
  latex_Lumi.SetTextFont(42)
  latex_Lumi.DrawLatex(0.72, 0.96, 'Run'+str(Year)+" (13 TeV)")

  c1.SaveAs(outdir+'/DATAvsMC_'+outname+'.pdf')
  c1.SaveAs(outdir+'/DATAvsMC_'+outname+'.png')
  c1.Close()

  c2 = ROOT.TCanvas('c1','',800,800)
  c2.cd()
  hist_SF.Draw('histsame')
  hist_SF.GetXaxis().SetTitle("Number of pileup")
  hist_SF.GetYaxis().SetTitle("PU Reweight")
  hist_SF.GetYaxis().SetRangeUser(0.,4.0)
  hist_SF.SetLineWidth(3)

  hist_SF_Up.SetLineStyle(3)
  hist_SF_Up.SetLineWidth(3)
  hist_SF_Up.Draw('histsame')

  hist_SF_Down.SetLineStyle(5)
  hist_SF_Down.SetLineWidth(3)
  hist_SF_Down.Draw('histsame')

  latex_Lumi.DrawLatex(0.72, 0.96, TotalLumi(Year)+" fb^{-1} (13 TeV)")
  c2.SaveAs(outdir+'/PUReweight_'+outname+'.pdf')
  c2.SaveAs(outdir+'/PUReweight_'+outname+'.png')

  outroot = ROOT.TFile(outdir+'/PUReweight_'+outname+'.root','RECREATE')
  outroot.cd()
  hist_1.Write()
  hist_1_Up.Write()
  hist_1_Down.Write()
  hist_2.Write()
  hist_SF.Write()
  hist_SF_Up.Write()
  hist_SF_Down.Write()

  print '#### Data mean = '+str(hist_1.GetMean())
  print '#### MC mean = '+str(hist_2.GetMean())

  #### If Fast sim, check normalizaion
  if IsFast:

    hist_SFed_MC = hist_2.Clone()
    hist_SFed_MC_Up = hist_2.Clone()
    hist_SFed_MC_Down = hist_2.Clone()

    for i in range(0,NBin):

      hist_SFed_MC.SetBinContent(i+1, hist_2.GetBinContent(i+1) * hist_SF.GetBinContent(i+1) )
      hist_SFed_MC_Up.SetBinContent(i+1, hist_2.GetBinContent(i+1) * hist_SF_Up.GetBinContent(i+1) )
      hist_SFed_MC_Down.SetBinContent(i+1, hist_2.GetBinContent(i+1) * hist_SF_Down.GetBinContent(i+1) )

    integral = hist_SFed_MC.Integral()
    integral_Up = hist_SFed_MC_Up.Integral()
    integral_Down = hist_SFed_MC_Down.Integral()

    print '@@@@ Integrals : %1.3f\t%1.3f\t%1.3f' % (integral, integral_Up, integral_Down)

    hist_SF_Normed = hist_SF.Clone()
    hist_SF_Normed_Up = hist_SF_Up.Clone()
    hist_SF_Normed_Down = hist_SF_Down.Clone()

    hist_SF_Normed.Scale(1./integral)
    hist_SF_Normed_Up.Scale(1./integral_Up)
    hist_SF_Normed_Down.Scale(1./integral_Down)

    hist_SF_Normed.SetName('PUReweight_'+str(Year)+'_Normed')
    hist_SF_Normed_Up.SetName('PUReweight_'+str(Year)+'_Normed_Up')
    hist_SF_Normed_Down.SetName('PUReweight_'+str(Year)+'_Normed_Down')

    ### Check

    hist_NormedSFed_MC = hist_2.Clone()
    hist_NormedSFed_MC_Up = hist_2.Clone()
    hist_NormedSFed_MC_Down = hist_2.Clone()

    for i in range(0,NBin):

      hist_NormedSFed_MC.SetBinContent(i+1, hist_2.GetBinContent(i+1) * hist_SF_Normed.GetBinContent(i+1) )
      hist_NormedSFed_MC_Up.SetBinContent(i+1, hist_2.GetBinContent(i+1) * hist_SF_Normed_Up.GetBinContent(i+1) )
      hist_NormedSFed_MC_Down.SetBinContent(i+1, hist_2.GetBinContent(i+1) * hist_SF_Normed_Down.GetBinContent(i+1) )

    normed_integral = hist_NormedSFed_MC.Integral()
    normed_integral_Up = hist_NormedSFed_MC_Up.Integral()
    normed_integral_Down = hist_NormedSFed_MC_Down.Integral()
    print '@@@@ Normed integrals : %1.3f\t%1.3f\t%1.3f' % (normed_integral, normed_integral_Up, normed_integral_Down)
    hist_NormedSFed_MC.SetName('NormedSFedMC')
    hist_NormedSFed_MC_Up.SetName('NormedSFedMC_Up')
    hist_NormedSFed_MC_Down.SetName('NormedSFedMC_Down')
    hist_NormedSFed_MC.Write()
    hist_NormedSFed_MC_Up.Write()
    hist_NormedSFed_MC_Down.Write()


    hist_SF_Normed.Write()
    hist_SF_Normed_Up.Write()
    hist_SF_Normed_Down.Write()


  outroot.Close()

  c2.Close()
