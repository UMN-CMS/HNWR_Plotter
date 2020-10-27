import os,ROOT,sys,tdrstyle,math
from array import array


ROOT.TH1.AddDirectory(ROOT.kFALSE)
tdrstyle.setTDRStyle()

region = "Resolved_SR"
channel = "Muon"

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']

ENV_PLOT_PATH = os.environ['PLOT_PATH']
base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/2016/"

allsamples = [
'EMuMethod_TTLX_powheg',
'DYJets_MG_HT_Reweighted',
'WWW',
'WWZ',
'WZZ',
'ZZZ',
'WZ_pythia',
'ZZ_pythia',
'WW_pythia',
'ttWToLNu',
'ttWToQQ',
'ttZ',
'SingleTop_sch_Lep',
'SingleTop_tW_antitop_NoFullyHad',
'SingleTop_tW_top_NoFullyHad',
'SingleTop_tch_antitop_Incl',
'SingleTop_tch_top_Incl',
'WJets_MG_HT',
'DYJets10to50_MG_Reweighted',
]

outfile = ROOT.TFile("out_"+dataset+"_"+region+"_"+channel+".root","RECREATE")

h_Bkgd = ROOT.TH1D('h_Bkgd','',800,0.,8000.)
h_Bkgd_withoutFit = ROOT.TH1D('h_Bkgd','',800,0.,8000.)

BinSize = -1
NRebin = int(BinSize/10)
MyNewBins = [0, 800, 1000, 1200, 1400, 1600, 2000, 2400, 2800, 3200, 8000]
if "Boosted" in region:
  MyNewBins = [0, 800, 1000, 1200, 1400, 1600, 1800, 8000]

for sample in allsamples:

  dirname = 'HNWR_Single'+channel+"_"+region

  f = ROOT.TFile(base_filepath+'HNWRAnalyzer_SkimTree_LRSMHighPt_'+sample+'.root')
  h = f.Get(dirname+'/WRCand_Mass_'+dirname)

  if not h:
    continue
  h_Bkgd.Add(h)

  if ("EMu" not in sample) and ("DYJets_MG_HT_Reweighted" not in sample):
    h_Bkgd_withoutFit.Add(h)

if NRebin>0:
  h_Bkgd.Rebin(NRebin)
  h_Bkgd_withoutFit.Rebin(NRebin)
else:
  h_Bkgd = h_Bkgd.Rebin(len(MyNewBins)-1, h_Bkgd.GetName(), array("d",MyNewBins))
  h_Bkgd_withoutFit = h_Bkgd_withoutFit.Rebin(len(MyNewBins)-1, h_Bkgd_withoutFit.GetName(), array("d",MyNewBins))


file_fit = ROOT.TFile('rootfile_fitted_hists_'+region+'_'+channel+'.root')
fitnames = ['hist_DY_fitted', 'hist_tt_fitted']
for fitname in fitnames:
  h = file_fit.Get(fitname)
  h = h.Rebin(len(MyNewBins)-1, h.GetName(), array("d",MyNewBins))
  h_Bkgd_withoutFit.Add(h)




c_1 = ROOT.TCanvas('c_1','',600,600)
c_1.cd()
c_1.SetLogy(True)
h_Bkgd.SetLineColor(ROOT.kBlack)
h_Bkgd.GetYaxis().SetTitle("Events per bin")
h_Bkgd.GetYaxis().SetRangeUser(1E-2,1E3)
h_Bkgd.Draw("histe1")

h_Bkgd_withoutFit.SetLineColor(ROOT.kRed)
h_Bkgd_withoutFit.Draw("histe1same")


outfile.cd()
h_Bkgd.Write()


c_1.SaveAs(dataset+"_"+region+"_"+channel+"_Bkgd_BinSize"+str(int(BinSize))+"GeV.pdf")
c_1.Close()
outfile.Close()



