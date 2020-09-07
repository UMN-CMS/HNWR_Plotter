import os,ROOT,math
from array import array
import mylib,canvas_margin
import tdrstyle

tdrstyle.setTDRStyle()
ROOT.gStyle.SetOptStat(0)

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
PLOT_PATH = os.environ['PLOT_PATH']

filename = 'xsec_190705_GenXsecAN_eeANDmm_Sorted.txt'
filepath = WORKING_DIR+'/data/'+dataset+'/'+filename
lines = open(filepath).readlines()

outdir = PLOT_PATH+'/'+dataset+'/SignalXSec/'
os.system('mkdir -p '+outdir)

# 1000  100 6.383e+00 7.414e-03

c1 = ROOT.TCanvas('c1','',600,600)

h_dummy = ROOT.TH1D('h_dummy', '', int(2E4/10), 0., 2.*1E4)

h_dummy.SetTitle("");
h_dummy.GetYaxis().SetLabelSize(0.04);
h_dummy.GetYaxis().SetTitleSize(0.054);
h_dummy.GetYaxis().SetTitleOffset(1.30);
h_dummy.GetXaxis().SetLabelSize(0.03);
h_dummy.GetXaxis().SetTitleSize(0.05);

h_dummy.Draw('histsame')
h_dummy.GetXaxis().SetRangeUser(80.,2E4)
h_dummy.GetYaxis().SetRangeUser(1E-5,3E5)

h_dummy.GetYaxis().SetTitle("#sigma(pp#rightarrowW_{R})#bf{#it{#Beta}}(W_{R}#rightarroweeq#bar{q'}) (fb)")
h_dummy.GetXaxis().SetTitle('m_{N} (GeV)')

mWRsToDraw = [800, 1000]
for i in range(1,1000):
  tmp_mWR = 1000 + 400 * i
  if tmp_mWR>=7000:
    break
  mWRsToDraw.append( tmp_mWR )

print '@@@@ mWR to draw'
print mWRsToDraw

####
lg = ROOT.TLegend(0.8, 0.4, 0.91, 0.91)
lg.SetBorderSize(0)
lg.SetFillStyle(0)

####
dict_hists = dict()
counter = 1
for mWR in mWRsToDraw:
  this_mNs = []
  this_xsecs = []
  print '@@@@ mWR = '+str(mWR)
  for line in lines:
    words = line.split()
    tmp_mWR = float( words[0] )
    tmp_mN = float( words[1] )
    tmp_xsec = float( words[2] ) * 1000. ## converted to pb
    tmp_xsec = tmp_xsec / 2. ## ee+mm -> ee
    if int(mWR)==int(tmp_mWR):
     ## k-factor
     kfactor = mylib.GetKFactor(int(mWR), int(tmp_mN))

     this_mNs.append( tmp_mN )
     this_xsecs.append( tmp_xsec*kfactor )

  gr_xsec = ROOT.TGraph(len(this_mNs), array("d", this_mNs), array("d", this_xsecs))
  gr_xsec.SetName('gr_xsec_mWR'+str(mWR))
  gr_xsec.SetMarkerStyle(20)
  gr_xsec.SetMarkerColor(counter)
  gr_xsec.SetLineColor(counter)

  dict_hists[gr_xsec.GetName()] = gr_xsec

  gr_xsec.Draw('lpsame')

  lg.AddEntry( gr_xsec, 'm_{W_{R}} = %d GeV'%(mWR), 'lp')

  counter += 1
  if counter==10:
    counter += 1

####

lg.Draw()

c1.SetLogx()
c1.SetLogy()
c1.SaveAs(outdir+'/XSec.pdf')

c1.Close()
