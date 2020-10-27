import os,ROOT,tdrstyle

region = 'HNWR_SingleMuon_Resolved_SR'

ROOT.TH1.AddDirectory(ROOT.kFALSE)
tdrstyle.setTDRStyle()

masses = [
#'WR4000_N100',
#'WR4000_N200',
#'WR4000_N400',
#'WR4000_N600',
'WR4000_N1000',
'WR4000_N2000',
'WR4000_N3000',
]
colors = [
ROOT.kBlack,
ROOT.kBlue,
ROOT.kGreen,
ROOT.kMagenta,
ROOT.kGray,
]

VarInfos = [
['dRj1j2', 60,0.,6.,1,-1.,'#DeltaR(j_{1},j_{2})'],
['Jet_0_Pt',50, 0., 1000.,20,1E-4,'Leading jet p_{T} (GeV)'],
['Jet_1_Pt',50, 0., 1000.,20,1E-4,'Subleading jet p_{T} (GeV)'],
['SumPtl2j1j2',20,0., 2000.,10,1E-4,'Sum p_{T} (l_{2}+j_{1}+j_{2}) (GeV)'],
['FatJet_LSF_Size',3,0.,3.,1,1E-2,'Number of fatjet'],
['FatJet_0_LSF',30, 0.7, 1.,1,1E-4,'Leading fatjet LSF'],
['MaxdRN', 60,0.,6.,1,-1.,'#DeltaR(N,i)'],
]

for it_VarInfo in range(0,len(VarInfos)):

  VarInfo = VarInfos[it_VarInfo]

  Var = VarInfo[0]
  NBin = int(VarInfo[1])
  BinXMin = VarInfo[2]
  BinXMax = VarInfo[3]
  NRebin = VarInfo[4]
  YMin = VarInfo[5]
  XTitle = VarInfo[6]

  c_1 = ROOT.TCanvas('c_1','',600,600)

  h_axis = ROOT.TH1D('h_axis','',NBin,BinXMin,BinXMax)
  h_axis.Draw("axis")
  h_axis.GetXaxis().SetTitle(XTitle)
  h_axis.GetYaxis().SetTitle('A.U.')

  f_TTLL = ROOT.TFile('rootfiles/HNWRAnalyzer_SkimTree_LRSMHighPt_TTLL_powheg.root')
  h_TTLL = f_TTLL.Get(region+'/'+Var+'_'+region)
  h_TTLL.Rebin(NRebin)
  h_TTLL.SetLineColor(ROOT.kRed)
  h_TTLL.SetLineWidth(3)
  yield_TTLL = h_TTLL.Integral()
  h_TTLL.Scale(1./h_TTLL.Integral())
  h_TTLL.SetFillColor(ROOT.kRed)
  h_TTLL.Draw('histsame')

  #f_TTLJ = ROOT.TFile('rootfiles/HNWRAnalyzer_SkimTree_LRSMHighPt_TTLJ_powheg.root')
  #h_TTLJ = f_TTLJ.Get(region+'/'+Var+'_'+region)
  #h_TTLJ.Rebin(NRebin)
  #h_TTLJ.SetLineColor(ROOT.kRed)
  #h_TTLJ.SetLineStyle(3)
  #h_TTLJ.SetLineWidth(3)
  #yield_TTLJ = h_TTLJ.Integral()
  #h_TTLJ.Scale(1./h_TTLJ.Integral())
  #h_TTLJ.Draw('histsame')

  y_max = -1

  lg = ROOT.TLegend(0.6,0.6, 0.93,0.93)
  lg.SetFillStyle(0)
  lg.SetBorderSize(0)
  lg.AddEntry(h_TTLL, 'ttbar leptonic (%1.2f)' % (yield_TTLL), 'l')
  #lg.AddEntry(h_TTLJ, 'ttbar semileptonic (%1.2f)' % (yield_TTLJ), 'l')

  for it_mass in range(0,len(masses)):
    mass = masses[it_mass]
    f = ROOT.TFile('rootfiles/Signal/HNWRAnalyzer_WRtoNLtoLLJJ_'+mass+'.root')
    h = f.Get(region+'/'+Var+'_'+region)
    h.Rebin(NRebin)
    h.SetLineColor(colors[it_mass])
    h.SetLineWidth(3)

    h_SignalFlavour = f.Get('SignalFlavour')
    LepFrac = 0.5
    if 'SingleElectron' in region:
      LepFrac = h_SignalFlavour.GetBinContent(1)/h_SignalFlavour.GetEntries()
    Eff = h.Integral() / (35918.219 * 1. * LepFrac)
    lg.AddEntry(h,mass+' (%1.1f%%)' % (Eff*100.),'l')

    h.Scale(1./h.Integral())
    h.Draw('histsame')
    y_max = max(y_max, h.GetMaximum())

  h_axis.GetYaxis().SetRangeUser(0., 1.2*y_max)
  if YMin>0:
    c_1.SetLogy()
    h_axis.GetYaxis().SetRangeUser(YMin,10.*y_max)
  h_axis.Draw("axissame")
  lg.Draw()
  c_1.SaveAs('/data6/Users/jskim/HNWR_Plotter/output/Run2Legacy_v3__Default/dRj1j2Test/'+Var+'.pdf')
  c_1.Close()



