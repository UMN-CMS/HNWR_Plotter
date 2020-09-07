import os,ROOT,math
import CMS_lumi, tdrstyle
from array import array

ROOT.TH1.AddDirectory(False)

tdrstyle.setTDRStyle()

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

lines_inclusive = open(WORKING_DIR+'/script/PrintKFactor/result.txt').readlines()
lines_calculated = open(WORKING_DIR+'/data/Run2Legacy_v4__Default/2016/AveragedKFactor.txt').readlines()

mWRs = []
k_l_incls = []
k_r_incls = []
k_l_cals = []
k_r_cals = []

NsToCalc = [
200,
400,
1000,
2000,
3000,
]
Colors = [
ROOT.kBlue,
ROOT.kGreen,
ROOT.kViolet,
ROOT.kYellow+1,
ROOT.kCyan,
]
WRArraysToCalc = []
kArraysToCalc = []
for i_NsToCalc in range(0,len(NsToCalc)):
  WRArraysToCalc.append( [] )
  kArraysToCalc.append( [] )

for line_incl in lines_inclusive:
  if 'WR' in line_incl:
    continue

  words_incl = line_incl.split()
  mWR_incl = int(words_incl[0])
  k_l_incl = float(words_incl[1])
  k_r_incl = float(words_incl[3])

  mass_l = 'WR'+words_incl[0]+'_N100'
  mass_r = 'WR'+words_incl[0]+'_N'+str(mWR_incl-100)

  k_l_cal = 1.
  k_r_cal = 1.

  for line_cal in lines_calculated:
    words_cal = line_cal.split()
    if words_cal[0]==mass_l:
      k_l_cal = float(words_cal[1])
      break
  for line_cal in lines_calculated:
    words_cal = line_cal.split()
    if words_cal[0]==mass_r:
      k_r_cal = float(words_cal[1])
      break

  for i_NToCalc in range(0,len(NsToCalc)):
    NToCalc = NsToCalc[i_NToCalc]
    this_mass = 'WR'+words_incl[0]+'_N'+str(NToCalc)
    for line_cal in lines_calculated:
      words_cal = line_cal.split()
      if words_cal[0]==this_mass:
        WRArraysToCalc[i_NToCalc].append( float(mWR_incl) )
        kArraysToCalc[i_NToCalc].append( float(words_cal[1]) )
        

  mWRs.append( float(mWR_incl) )
  k_l_incls.append( k_l_incl )
  k_r_incls.append( k_r_incl )
  k_l_cals.append( k_l_cal )
  k_r_cals.append( k_r_cal )

nWR = len(mWRs)
gr_l_incl = ROOT.TGraph( nWR, array('d', mWRs), array('d', k_l_incls) )
gr_r_incl = ROOT.TGraph( nWR, array('d', mWRs), array('d', k_r_incls) )
gr_l_cal = ROOT.TGraph( nWR, array('d', mWRs), array('d', k_l_cals) )
gr_r_cal = ROOT.TGraph( nWR, array('d', mWRs), array('d', k_r_cals) )

c1 = ROOT.TCanvas('c1','',800,800)
c1.cd()

hist_dummy = ROOT.TH1D('hist_dummy', '', 8000, 0., 8000.)

hist_dummy.GetYaxis().SetRangeUser(0.8, 1.8)
hist_dummy.GetYaxis().SetTitle('Averaged k-factor')
hist_dummy.GetXaxis().SetTitle('m_{WR} (GeV)')
hist_dummy.GetYaxis().SetLabelSize(0.04)
hist_dummy.GetYaxis().SetTitleSize(0.054)
hist_dummy.GetYaxis().SetTitleOffset(1.30)
hist_dummy.GetXaxis().SetLabelSize(0.03)
hist_dummy.GetXaxis().SetTitleSize(0.05)

hist_dummy.Draw('hist')

gr_l_incl.SetLineColor(ROOT.kBlack)
gr_l_cal.SetLineColor(ROOT.kBlack)
gr_l_cal.SetLineStyle(3)

gr_r_incl.SetLineColor(ROOT.kRed)
gr_r_cal.SetLineColor(ROOT.kRed)
gr_r_cal.SetLineStyle(3)

gr_l_incl.Draw('lsame')
gr_r_incl.Draw('lsame')
gr_l_cal.Draw('lsame')
gr_r_cal.Draw('lsame')

lg = ROOT.TLegend(0.25, 0.20, 0.8, 0.40)
lg.SetBorderSize(0)
lg.SetFillStyle(0)
lg.AddEntry(gr_l_incl, 'Inclusive, m_{N} = 100 GeV', 'l')
lg.AddEntry(gr_r_incl, 'Inclusive, m_{N} = m_{WR} - 100 GeV', 'l')
lg.AddEntry(gr_l_cal, 'Integrated, m_{N} = 100 GeV', 'l')
lg.AddEntry(gr_r_cal, 'Integrated, m_{N} = m_{WR} - 100 GeV', 'l')
lg.Draw()

lg2 = ROOT.TLegend(0.25, 0.70, 0.8, 0.9)
lg2.SetBorderSize(0)
lg2.SetFillStyle(0)
gr_dict = dict()
for i_NToCalc in range(0,len(NsToCalc)):
  #print '[N = %d GeV]'%(NsToCalc[i_NToCalc])
  #print kArraysToCalc[i_NToCalc]
  gr = ROOT.TGraph( len(WRArraysToCalc[i_NToCalc]), array('d', WRArraysToCalc[i_NToCalc]), array('d', kArraysToCalc[i_NToCalc]) )
  gr.SetLineStyle(5)
  gr.SetLineColor( Colors[i_NToCalc] )
  gr_dict[NsToCalc[i_NToCalc]] = gr
  gr.Draw('lsame')

  lg2.AddEntry(gr, 'Integrated, m_{N} = %d GeV'%(NsToCalc[i_NToCalc]), 'l')
lg2.Draw()

c1.SaveAs(PLOT_PATH+'/'+dataset+'/KFactor/kfactor.pdf')
c1.Close()


#### k-factor vs Q2 plot

c2 = ROOT.TCanvas('c2','',800,800)
c2.cd()

hist_dummy.GetYaxis().SetTitle('k-factor')
hist_dummy.GetXaxis().SetTitle('m_{lN} (GeV)')

hist_dummy.Draw('hist')

f_sh = ROOT.TFile(WORKING_DIR+'/script/KFactor/FromSH/kfactor.root')
h_sh = f_sh.Get('kfactor_all')

h_sh.Draw('histsame')

c2.SaveAs(PLOT_PATH+'/'+dataset+'/KFactor/kfactor_vs_mlnN.pdf')
c2.Close()
