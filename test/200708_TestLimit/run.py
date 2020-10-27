import os,ROOT
import mylib
import canvas_margin
import tdrstyle
import CMS_lumi, tdrstyle
import math
from array import array

tdrstyle.setTDRStyle()
ROOT.TH1.AddDirectory(False)

lines = open('output_sorted.txt').readlines()

NBkgds = [
0.01, 0.1, 1, 8, 9, 10, 11, 12, 100, 1000,
]

ESigs = [
1.10,
1.20,
1.30,
1.40,
1.50,
]

for ESig in ESigs:

  c1 = ROOT.TCanvas('c1','',800,800)
  c1.SetLogy()

  c1_Ratio = ROOT.TCanvas('c1_Ratio','',800,800)

  histdict = dict()

  hist_dummy = ROOT.TH1D('hist_dummy', '', 20, 0., 2.0)
  hist_dummy.GetXaxis().SetTitle('Background uncertainty')
  hist_dummy.GetYaxis().SetTitle('CL_{S} upper limit')
  hist_dummy.GetYaxis().SetRangeUser(1E-1, 1E4)

  c1.cd()
  hist_dummy.Draw('axis')
  c1_Ratio.cd()
  hist_dummy.Draw('axis')

  lg = ROOT.TLegend(0.6, 0.6, 0.85, 0.8)
  lg.SetBorderSize(0)
  lg.SetFillStyle(0)

  for i_NBkgd in range(0,len(NBkgds)):
    NBkgd = NBkgds[i_NBkgd]
    Color = i_NBkgd+1
    if Color==5:
      Color = ROOT.kYellow+1
    EBkgds = []
    Limits = []
    LimitRatios = []
    for line in lines:
      words = line.strip('\n').split()
      if float(words[0])==NBkgd and float(words[1])==ESig:
        EBkgds.append( float(words[2])-1.0 )
        Limits.append( float(words[3]) )
        LimitRatios.append( float(words[3])/float(NBkgd) )

    #print '@@@@@@@@@@'
    #print EBkgds
    #print Limits

    gr = ROOT.TGraph(len(EBkgds), array('d', EBkgds), array('d', Limits))
    gr.SetLineColor(Color)
    gr.SetMarkerColor(Color)
    histdict[str(NBkgd)] = gr

    c1.cd()
    gr.Draw('lpsame')

    gr_Ratio = ROOT.TGraph(len(EBkgds), array('d', EBkgds), array('d', LimitRatios))
    gr_Ratio.SetLineColor(Color)
    gr_Ratio.SetMarkerColor(Color)
    histdict[str(NBkgd)+'_Ratio'] = gr_Ratio

    c1_Ratio.cd()
    gr_Ratio.Draw('lpsame')

    lg.AddEntry(gr, 'Bkgd. = '+str(NBkgd), 'lp')

  c1.cd()
  lg.Draw()
  c1.SaveAs('plot_ESigs'+str(ESig)+'.pdf')
  c1.Close()

  c1_Ratio.cd()
  hist_dummy.GetXaxis().SetTitle('Background uncertainty')
  hist_dummy.GetYaxis().SetTitle('CL_{S} upper limit / Bkgd.')
  hist_dummy.GetYaxis().SetRangeUser(0., 2.0)
  lg.Draw()
  c1_Ratio.SaveAs('ratioplot_ESigs'+str(ESig)+'.pdf')
  c1_Ratio.Close()
