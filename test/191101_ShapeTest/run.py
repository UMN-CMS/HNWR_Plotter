import os,ROOT,sys,tdrstyle,math
from array import array


ROOT.TH1.AddDirectory(ROOT.kFALSE)
tdrstyle.setTDRStyle()

BinSize = int(sys.argv[1])
region = sys.argv[2]

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']

#dataset='Run2Legacy_v3__BoostedOnlyTest'

ENV_PLOT_PATH = os.environ['PLOT_PATH']

base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/2016/"

line_SigXsec = open(WORKING_DIR+'/data/Run2Legacy_v3__Default/xsec_190705_GenXsecAN_eeANDmm.txt').readlines()

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

outfile = ROOT.TFile("out_"+dataset+"_"+region+".root","RECREATE")

h_Bkgd = ROOT.TH1D('h_Bkgd','',800,0.,8000.)

NRebin = int(BinSize/10)
MyNewBins = [0, 800, 1000, 1200, 1400, 1600, 2000, 2400, 2800, 3200, 8000]
if "Boosted" in region:
  MyNewBins = [0, 800, 1000, 1200, 1400, 1600, 1800, 8000]

for sample in allsamples:
  f = ROOT.TFile(base_filepath+'HNWRAnalyzer_SkimTree_LRSMHighPt_'+sample+'.root')
  h = f.Get(region+'/WRCand_Mass_'+region)
  if not h:
    continue
  h_Bkgd.Add(h)

if NRebin>0:
  h_Bkgd.Rebin(NRebin)
  #h_Bkgd.Scale(1./BinSize)
else:
  h_Bkgd = h_Bkgd.Rebin(len(MyNewBins)-1, h_Bkgd.GetName(), array("d",MyNewBins))
  #for ix in range(0,len(MyNewBins)-1):
    #if h_Bkgd.GetBinContent(ix+1)!=0:
      #h_Bkgd.SetBinContent( ix+1, h_Bkgd.GetBinContent(ix+1)/(MyNewBins[ix+1]-MyNewBins[ix]) )


c_1 = ROOT.TCanvas('c_1','',600,600)
c_1.cd()
c_1.SetLogy(True)
h_Bkgd.SetLineColor(ROOT.kBlack)
h_Bkgd.GetYaxis().SetTitle("Events per bin")
h_Bkgd.GetYaxis().SetRangeUser(1E-4,1E3)
h_Bkgd.Draw("histe1")

#### expo

h_Bkgd.Fit("expo","P","",800,8000)
fit_Bkgd_expo = h_Bkgd.GetFunction("expo").Clone()
fit_Bkgd_expo.SetLineColor(ROOT.kRed)
fit_Bkgd_expo.Draw("same")

#### Aexpo(-Bx)/x^C
func_1 = ROOT.TF1('func_1','[0]*exp(-1.*[1]*x)/pow(x,[2])',800,8000)
func_1.SetParameter(0,math.exp(fit_Bkgd_expo.GetParameter(0)))
func_1.SetParameter(1,-1.*fit_Bkgd_expo.GetParameter(1))
func_1.SetParameter(2,1)
h_Bkgd.Fit('func_1',"P","",800,8000)
fit_Bkgd_func_1 = h_Bkgd.GetFunction('func_1').Clone()
fit_Bkgd_func_1.SetLineColor(ROOT.kBlue)
fit_Bkgd_func_1.Draw("same")


outfile.cd()
h_Bkgd.Write()

SigMasses = [
[4000,400],
]

for SigMass in SigMasses:
  MWR = SigMass[0]
  MN = SigMass[1]
  sample = 'WRtoNLtoLLJJ_WR%d_N%d'%(MWR,MN)
  f = ROOT.TFile(base_filepath+'/Signal/HNWRAnalyzer_'+sample+'.root')
  h = f.Get(region+'/WRCand_Mass_'+region)
  if h:


    if NRebin>0:
      h.Rebin(NRebin)
      #h.Scale(1./BinSize)
    else:
      h = h.Rebin(len(MyNewBins)-1, h.GetName(), array("d",MyNewBins))
      #for ix in range(0,len(MyNewBins)-1):
        #if h.GetBinContent(ix+1)!=0:
          #h.SetBinContent( ix+1, h.GetBinContent(ix+1)/( MyNewBins[ix+1]-MyNewBins[ix] ) )

    FoundXsec = False
    #### xsec
    for line in line_SigXsec:
      words = line.split()
      x_MWR = int(words[0])
      x_MN = int(words[1])
      x_xsec = float(words[2])
      if (MWR==x_MWR) and (MN==x_MN):
        FoundXsec = True
        h.Scale(x_xsec)
        break

    if not FoundXsec:
      print "Xsec not found for : (%d,%d)" % (MWR, MN)

    h.SetLineColor(ROOT.kGreen)
    h.Draw("histsame")
    outfile.cd()
    h.Write()

#### Retrieve fit expo funciton and get histogram
#### exp(A+Bx)

if NRebin>0:

  h_bkgd_fit_to_binned = ROOT.TH1D('h_bkgd_fit_to_binned','',800/NRebin,0.,8000.)

  ExpoConst = fit_Bkgd_expo.GetParameter(0)
  ExpoSlope = fit_Bkgd_expo.GetParameter(1)

  print '@@@@ ExpoConst = '+str(ExpoConst)
  print '@@@@ ExpoSlope = '+str(ExpoSlope)
  print '@@@@ %.10f'%( math.exp( ExpoConst + ExpoSlope*4000. ) )

  for ix in range(0, h_bkgd_fit_to_binned.GetXaxis().GetNbins()):

    x_l = h_bkgd_fit_to_binned.GetXaxis().GetBinLowEdge(ix+1)
    x_r = h_bkgd_fit_to_binned.GetXaxis().GetBinUpEdge(ix+1)
    print '[%d,%d]'%(x_l,x_r)
    if x_r>800:

      integral_expo = (math.exp(ExpoConst)/ExpoSlope)*( math.exp(ExpoSlope*x_r)-math.exp(ExpoSlope*x_l) )/BinSize
      h_bkgd_fit_to_binned.SetBinContent(ix+1,integral_expo)
      h_bkgd_fit_to_binned.SetBinError(ix+1,math.sqrt(integral_expo))

  h_bkgd_fit_to_binned.SetLineColor(ROOT.kGray)
  h_bkgd_fit_to_binned.SetLineWidth(3)
  #h_bkgd_fit_to_binned.Draw("histe1same")
  h_bkgd_fit_to_binned.Write()


c_1.SaveAs(dataset+"_"+region+"_Bkgd_BinSize"+str(int(BinSize))+"GeV.pdf")
c_1.Close()
outfile.Close()



