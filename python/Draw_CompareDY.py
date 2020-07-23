import os,ROOT,math
import CMS_lumi, tdrstyle, mylib, canvas_margin
from array import array

Syst = 'Central'
Prefix = ''
if Syst!='Central':
  Prefix = 'Syst_'+Syst+'_'

def Rebin(hist, region, var, nRebin, Year):
  if var=='WRCand_Mass':
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
'HNWR_SingleElectron_Resolved_DYCR',
'HNWR_SingleMuon_Resolved_DYCR',
'HNWR_SingleElectron_Boosted_DYCR',
'HNWR_SingleMuon_Boosted_DYCR',
]

VariableSets = [
['ZCand_Pt', 100, 0., 1000., 'p_{T}(ll) (GeV)'],
['WRCand_Mass', -1, 800., 8000., 'm(W_{WR}) (GeV)'],
['Jet_0_Pt', 20, 0., 1000., 'p_{T} of the leading jet (GeV)'],
]

for Year in Years:

  basedir = FILE_PATH+'/'+dataset+'/Regions/'+Year+'/'
  outDir = PLOT_PATH+'/'+dataset+'/CompareDY/'+Year+'/'
  os.system('mkdir -p '+outDir)

  f_out = ROOT.TFile(outDir+'/shapes_DY.root','RECREATE')

  f_NLO = ROOT.TFile(basedir+'/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets.root')
  f_LO = ROOT.TFile(basedir+'/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT.root')
  f_LORwg = ROOT.TFile(basedir+'/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT_Reweighted.root')

  for Region in Regions:

    Region = Prefix+Region

    for VariableSet in VariableSets:

      Variable = VariableSet[0]
      nRebin = VariableSet[1]
      xMin = VariableSet[2]
      xMax = VariableSet[3]
      xtitle = VariableSet[4]

      c1 = ROOT.TCanvas('c1', '', 800, 800)

      c1_up = ROOT.TPad("c1_up", "", 0, 0.25, 1, 1)
      c1_down = ROOT.TPad("c1_down", "", 0, 0, 1, 0.25)
      c1, c1_up, c1_down = canvas_margin.canvas_margin(c1, c1_up, c1_down)
      c1.Draw()
      c1_up.Draw()
      c1_down.Draw()

      c1_up.cd()
      c1_up.SetLogy(True)

      ## Get hist
      h_NLO = f_NLO.Get(Region+'/'+Variable+'_'+Region)
      h_LO = f_LO.Get(Region+'/'+Variable+'_'+Region)
      h_LORwg = f_LORwg.Get(Region+'/'+Variable+'_'+Region)

      ## Make overflow
      h_NLO.GetXaxis().SetRangeUser(xMin,xMax)
      h_NLO = mylib.MakeOverflowBin(h_NLO)

      ## Rebin
      h_NLO = Rebin(h_NLO, Region, Variable, nRebin, Year)
      h_LO = Rebin(h_LO, Region, Variable, nRebin, Year)
      h_LORwg = Rebin(h_LORwg, Region, Variable, nRebin, Year)

      ## Scale
      h_NLO.Scale(1./h_NLO.Integral())
      h_LO.Scale(1./h_LO.Integral())
      h_LORwg.Scale(1./h_LORwg.Integral())

      h_NLO.SetLineColor(ROOT.kBlack)
      h_LO.SetLineColor(ROOT.kBlue)
      h_LORwg.SetLineColor(ROOT.kRed)

      h_NLO.SetLineWidth(2)
      h_LO.SetLineWidth(2)
      h_LORwg.SetLineWidth(2)

      ## Legend
      lg = ROOT.TLegend(0.5, 0.65, 0.94, 0.91)
      lg.SetBorderSize(0)
      lg.SetFillStyle(0)
      lg.AddEntry(h_NLO, 'NLO', 'le')
      lg.AddEntry(h_LO, 'LO, w/o Z-p_{T} reweighting', 'le')
      lg.AddEntry(h_LORwg, 'LO, w Z-p_{T} reweighting', 'le')

      ## Copy NLO axis
      NLOAxis = h_NLO.GetXaxis()
      nBin = NLOAxis.GetNbins()
      xBins = [NLOAxis.GetBinLowEdge(1)]
      for ix in range(0,nBin):
        xBins.append( NLOAxis.GetBinUpEdge(ix+1) )
      xBins = array("d",xBins)

      h_dummy_up = ROOT.TH1D('h_dumy_up', '', nBin, xBins)
      h_dummy_up.GetXaxis().SetRangeUser(xMin, xMax)
      h_dummy_up.GetYaxis().SetRangeUser(1E-4, 2)
      binsize = h_dummy_up.GetXaxis().GetBinUpEdge(1)-h_dummy_up.GetXaxis().GetBinLowEdge(1)
      str_binsize = '%d'%(binsize)
      if binsize!=int(binsize):
        str_binsize = '%1.2f'%(binsize)
      h_dummy_up.GetYaxis().SetTitle('Fraction / '+str_binsize+' Gev')

      h_dummy_down = ROOT.TH1D('h_dumy_down', '', nBin, xBins)
      h_dummy_down.GetYaxis().SetRangeUser(0.,2.0)
      h_dummy_down.SetNdivisions(504,"Y")
      h_dummy_down.GetXaxis().SetRangeUser(xMin, xMax)
      h_dummy_down.GetXaxis().SetTitle(xtitle)
      h_dummy_down.GetYaxis().SetRangeUser(0,1.2)
      h_dummy_down.GetYaxis().SetTitle("#frac{NLO}{LO}")
      h_dummy_down.SetFillColor(0)
      h_dummy_down.SetMarkerSize(0)
      h_dummy_down.SetMarkerStyle(0)
      h_dummy_down.SetLineColor(ROOT.kWhite)

      h_dummy_up, h_dummy_down = canvas_margin.hist_axis(h_dummy_up, h_dummy_down)

      ## draw up
      c1_up.cd()
      h_dummy_up.Draw('axis')
      h_NLO.Draw('histe1same')
      h_LO.Draw('histe1same')
      h_LORwg.Draw('histe1same')
      h_dummy_up.Draw("axissame")

      lg.Draw()

      ## draw down
      c1_down.cd()
      h_ratio_LO = h_LO.Clone('h_ratio_LO')
      h_ratio_LORwg = h_LORwg.Clone('h_ratio_LORwg')

      for ix in range(0, h_NLO.GetXaxis().GetNbins()):
        y_NLO = h_NLO.GetBinContent(ix+1)
        y_LO = h_LO.GetBinContent(ix+1)
        y_LORwg = h_LORwg.GetBinContent(ix+1)

        e_NLO = h_NLO.GetBinError(ix+1)
        e_LO = h_LO.GetBinError(ix+1)
        e_LORwg = h_LORwg.GetBinError(ix+1)

        rele_NLO = h_NLO.GetBinError(ix+1) / y_NLO if y_NLO>0 else 0
        rele_LO = h_LO.GetBinError(ix+1) / y_LO if y_LO>0 else 0
        rele_LORwg = h_LORwg.GetBinError(ix+1) / y_LORwg if y_LORwg>0 else 0

        ratio_LO = y_NLO / y_LO if y_LO>0 else 0
        ratio_LORwg = y_NLO / y_LORwg if y_LORwg>0 else 0
        rele_ratio_LO = math.sqrt( rele_NLO*rele_NLO + rele_LO*rele_LO )
        rele_ratio_LORwg = math.sqrt( rele_NLO*rele_NLO + rele_LORwg*rele_LORwg )

        h_ratio_LO.SetBinContent(ix+1, ratio_LO)
        h_ratio_LO.SetBinError(ix+1, rele_ratio_LO * ratio_LO)
        h_ratio_LORwg.SetBinContent(ix+1, ratio_LORwg)
        h_ratio_LORwg.SetBinError(ix+1, rele_ratio_LORwg * ratio_LORwg)

        if Variable=="WRCand_Mass":
          print 'rele_NLO = %f, rele_LORwg = %f -> ratio_LORwg = %f, rele_ratio_LORwg = %f, ratio_LORwg = %f, rele_ratio_LORwg * ratio_LORwg = %f'%(rele_NLO,rele_LORwg,ratio_LORwg,rele_ratio_LORwg,ratio_LORwg,rele_ratio_LORwg * ratio_LORwg)

      h_dummy_down.Draw("axis")
      #h_ratio_LO.Draw("histsame")
      h_ratio_LORwg.Draw("histe1same")

      g1_x = [-9000, 9000]
      g1_y = [1, 1]
      g1 = ROOT.TGraph(2, array("d", g1_x ), array("d", g1_y ))
      g1.Draw("same")

      if Variable=="WRCand_Mass":
        f_out.cd()
        h_ratio_LORwg.SetName(Region)
        h_ratio_LORwg.Write()

      ## Save

      c1.SaveAs(outDir+'/%s_%s.pdf'%(Region,Variable))
      c1.Close()
  f_out.Close()
