import os,ROOT,math
import CMS_lumi, tdrstyle
from array import array

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

for Year in Years:

  basedir = FILE_PATH+'/'+dataset+'/ZPtReweight/'+Year+'/'
  outDir = PLOT_PATH+'/'+dataset+'/ZPtReweight/'+Year+'/'
  os.system('mkdir -p '+outDir)

  f_LO = ROOT.TFile(basedir+'Rebinned_GetZPtReweight_DYJets_LO.root')
  f_NLO = ROOT.TFile(basedir+'Rebinned_GetZPtReweight_DYJets_NLO.root')

  h_LO = f_LO.Get('MassVSPt')
  h_NLO = f_NLO.Get('MassVSPt')

  h_LO.Scale(1./h_LO.Integral())
  h_NLO.Scale(1./h_NLO.Integral())

  print str(h_LO.Integral())+'\t'+str(h_NLO.Integral())

  h_Ratio = h_NLO.Clone('Ratio')
  h_Ratio.Divide(h_LO)

  for ix in range(0,h_LO.GetXaxis().GetNbins()):
    ixBin = ix+1
    for iy in range(0,h_LO.GetYaxis().GetNbins()):
      iyBin = iy+1
      y_LO = h_LO.GetBinContent(ixBin, iyBin)
      y_NLO = h_NLO.GetBinContent(ixBin, iyBin)
      y_Ratio = h_Ratio.GetBinContent(ixBin, iyBin)

      e_LO = h_LO.GetBinError(ixBin, iyBin)
      e_NLO = h_NLO.GetBinError(ixBin, iyBin)
      e_Ratio = h_Ratio.GetBinError(ixBin, iyBin)

      mass_l = h_LO.GetXaxis().GetBinLowEdge(ixBin)
      mass_r = h_LO.GetXaxis().GetBinUpEdge(ixBin)
      pt_l = h_LO.GetYaxis().GetBinLowEdge(iyBin)
      pt_r = h_LO.GetYaxis().GetBinUpEdge(iyBin)

      if y_NLO==0:
        continue
      if y_LO==0:
        continue

      print 'mass [%d,%d], pt [%d,%d]'%(mass_l,mass_r,pt_l,pt_r)

      rele_LO = e_LO/y_LO
      rele_NLO = e_NLO/y_NLO
      rele_Ratio = e_Ratio/y_Ratio

      #print 'mass [%d,%d], pt [%d,%d] ; NLO : %1.3f +- %1.3f, LO : %1.3f +- %1.3f, Ratio = %1.3f +- %1.3f'%(mass_l,mass_r,pt_l,pt_r,y_NLO,e_NLO,y_LO,e_LO,y_Ratio,e_Ratio)
      #print 'mass [%d,%d], pt [%d,%d] ; (NLO, LO, Ratio) = (%1.3f, %1.3f %1.3f)' % (mass_l,mass_r,pt_l,pt_r, rele_NLO,rele_LO,rele_Ratio)

  out = ROOT.TFile(outDir+'/Ratio.root','RECREATE')
  out.cd()

  h_Ratio.GetXaxis().SetTitle("mass of Z boson (GeV)")
  h_Ratio.GetYaxis().SetTitle("p_{T} of Z boson (GeV)")

  h_Ratio.Write()
  out.Close()
