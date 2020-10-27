import os,ROOT,tdrstyle

tdrstyle.setTDRStyle()

PLOTTER_WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']


WrongJECDir = '/data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v4__Default/Regions/2016/Signal/'
NewJECDir = '/data6/Users/jskim/SKFlatOutput/Run2Legacy_v4/HNWRAnalyzer/2016/RunSyst__Signal__RunXsecSyst__NewJEC__/'

masses = open('mass.txt').readlines()

region = 'Boosted'
dirName = 'HNWR_SingleMuon_'+region+'_SR'
histName = 'HNFatJet_Pt'
nRebin = 50

outDir = './output/'+dirName+'/'
os.system('mkdir -p '+outDir)

for mass in masses:

  mass = mass.strip('\n')
  fname = 'HNWRAnalyzer_WRtoNLtoLLJJ_'+mass+'.root'
  f_Wrong = ROOT.TFile(WrongJECDir+fname)
  f_New = ROOT.TFile(NewJECDir+fname)

  h_Wrong = f_Wrong.Get(dirName+'/'+histName+'_'+dirName)
  h_New = f_New.Get(dirName+'/'+histName+'_'+dirName)

  if not h_Wrong or not h_New:
    continue

  h_Wrong.Rebin(nRebin)
  h_New.Rebin(nRebin)

  yMax = max(h_Wrong.GetMaximum(), h_New.GetMaximum())

  h_Wrong.SetLineColor(ROOT.kRed)
  h_New.SetLineColor(ROOT.kBlue)

  c1 = ROOT.TCanvas('c1','',600,600)
  c1.SetLogy(True)
  c1.cd()

  h_Wrong.Draw("histsame")
  h_New.Draw("histsame")

  h_Wrong.GetYaxis().SetRangeUser(1E-5, yMax*10)

  c1.SaveAs(outDir+'/'+histName+'_'+dirName+'_'+mass+'.pdf')
  c1.Close()
