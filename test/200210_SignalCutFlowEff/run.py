import os,ROOT,tdrstyle
from array import array

tdrstyle.setTDRStyle()
ROOT.TH1.AddDirectory(False);

PLOTTER_WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']

masses = open('mass.txt').readlines()

#CutName = 'CutFlow/METFilter_HNWR'
#CutName = 'CutFlow/NTightLeptonIsTwo_SingleMuon_HNWR'
#CutName = 'CutFlow/NTightLeptonIsTwo_SingleMuon_PassTrigger_HNWR'
#CutName = 'CutFlow/NTightLeptonIsTwo_SingleMuon_TwoAK4Jets_HNWR'
CutName = 'CutFlow/NTightLeptonIsTwo_SingleMuon_mllGT200_HNWR'
#CutName = 'CutFlow/NTightLeptonIsTwo_SingleMuon_mWRGT800_mll400_HNWR'


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

outDir = './output/'
os.system('mkdir -p '+outDir)

dict_WR_to_Ns = dict()

for mass in masses:

  # WR1400_N100

  words = mass.split('_')
  WR = int(words[0].replace('WR',''))
  N = int(words[1].replace('N',''))

  if WR in dict_WR_to_Ns:
    dict_WR_to_Ns[WR].append(N)
  else:
    dict_WR_to_Ns[WR] = [N]

dict_WR_to_Ns = sorted(dict_WR_to_Ns.items())

for mass in dict_WR_to_Ns:

  WR = mass[0]
  Ns = sorted(mass[1])
  print '@@@@ WR = '+str(WR)
  print Ns

  c1 = ROOT.TCanvas('c1','',600,600)
  c1.cd()
  hist_dummy = ROOT.TH1D("hist_dummy","",8000,0.,8000.)
  hist_dummy.Draw("hist")

  gr_Effs = []

  for it_Year in range(0,len(Years)):
  #for it_Year in range(0,1):

    Year = Years[it_Year]
    Color = Colors[it_Year]

    x = []
    y = []

    #BaseDir = '/data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v4__Default/Regions/'+Year+'/Signal/'
    BaseDir = '/data6/Users/jskim/SKFlatOutput/Run2Legacy_v4/HNWRAnalyzer/'+Year+'/TESTNoTriggerEff__Signal__/'

    for N in Ns:
      f = ROOT.TFile(BaseDir+'/HNWRAnalyzer_WRtoNLtoLLJJ_WR%s_N%s.root'%(str(WR),str(N)))
      h_NoCut = f.Get('CutFlow/NoCut_HNWR')
      y_NoCut = h_NoCut.GetBinContent(1)

      h_Cut = f.Get(CutName)
      Eff = 0.
      if h_Cut:
        y_Cut = h_Cut.GetBinContent(1)
        Eff = y_Cut/y_NoCut*2.
      x.append(N)
      y.append(Eff)

    gr_Eff = ROOT.TGraph(len(Ns), array("d", x ), array("d", y ))
    gr_Effs.append( gr_Eff )

    gr_Eff.SetLineColor(Color)
    gr_Eff.SetMarkerColor(Color)
    gr_Eff.SetLineWidth(3)
    gr_Eff.Draw("lpsame")
    hist_dummy.GetXaxis().SetRangeUser(0., x[-1])

  #for gr_Eff in gr_Effs:
  #  gr_Eff.Print()

  c1.SaveAs(outDir+'/'+str(WR)+'_'+CutName.replace('/','__')+'.pdf')
  c1.Close()
