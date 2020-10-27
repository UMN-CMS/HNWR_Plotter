import os,ROOT,tdrstyle
from array import array

tdrstyle.setTDRStyle()

baseDir = '/data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v4__Default/Limit/Asymptotic/2016/'
asymDir = '2020_02_21_101605__Year2016_AllCorrelated_NewKFactor'
fullDir =  '2020_03_04_114228__Year2016_Full'

outDir = '/data6/Users/jskim/HNWR_Plotter/output/Run2Legacy_v4__Default/200309_AsymVSFull/'
os.system('mkdir -p '+outDir)

mWRs = [
2000,
3000,
4000,
5000,
]

channels = [
"EE",
"MuMu",
]
regions = [
'Resolved',
'Boosted',
]

for mWR in mWRs:

  mNs = [100]
  for i in range(0, int(mWR/200)-1 ):
    this_mN = 200*(i+1)
    mNs.append( this_mN )
  mNs.append(mWR-100)
  print mNs

  tlatex_mass = ROOT.TLatex()
  tlatex_mass.SetNDC()
  tlatex_mass.SetTextSize(0.035);

  for channel in channels:
    for region in regions:

      lines_asym = open(baseDir+asymDir+'.txt').readlines()
      lines_full = open(baseDir+fullDir+'.txt').readlines()

      x_asym = []
      y_asym = []
      x_asym_1up = []
      y_asym_1up = []
      x_asym_1dn = []
      y_asym_1dn = []

      x_full = []
      y_full = []
      x_full_1up = []
      y_full_1up = []
      x_full_1dn = []
      y_full_1dn = []

      for mN in mNs:

        #### asym
        for line in lines_asym:
          words = line.split('\t')
          isMatched = (words[0]==channel) and (words[1]==region) and (words[2]==str(mWR)) and (words[3]==str(mN))
          if isMatched:
            if words[4]!="":
              x_asym.append( mN )
              y_asym.append( float(words[4]) )
            if words[5]!="":
              x_asym_1up.append( mN )
              y_asym_1up.append( float(words[5]) )
            if words[6]!="":
              x_asym_1dn.append( mN )
              y_asym_1dn.append( float(words[6]) )

            break

        #### full
        for line in lines_full:
          words = line.split('\t')
          isMatched = (words[0]==channel) and (words[1]==region) and (words[2]==str(mWR)) and (words[3]==str(mN))
          if isMatched:
            if words[4]!="":
              x_full.append( mN )
              y_full.append( float(words[4]) )
            if words[5]!="":
              x_full_1up.append( mN )
              y_full_1up.append( float(words[5]) )
            if words[6]!="":
              x_full_1dn.append( mN )
              y_full_1dn.append( float(words[6]) )

            break


      gr_asym = ROOT.TGraph(len(x_asym), array("d", x_asym), array("d", y_asym))
      gr_asym.SetLineWidth(2)
      gr_asym.SetLineColor(ROOT.kBlack)

      gr_asym_1up = ROOT.TGraph(len(x_asym_1up), array("d", x_asym_1up), array("d", y_asym_1up))
      gr_asym_1up.SetLineStyle(3)
      gr_asym_1up.SetLineWidth(2)
      gr_asym_1up.SetLineColor(ROOT.kBlack)

      gr_asym_1dn = ROOT.TGraph(len(x_asym_1dn), array("d", x_asym_1dn), array("d", y_asym_1dn))
      gr_asym_1dn.SetLineStyle(3)
      gr_asym_1dn.SetLineWidth(2)
      gr_asym_1dn.SetLineColor(ROOT.kBlack)

      gr_full = ROOT.TGraph(len(x_full), array("d", x_full), array("d", y_full))
      gr_full.SetLineWidth(2)
      gr_full.SetLineColor(ROOT.kRed)

      gr_full_1up = ROOT.TGraph(len(x_full_1up), array("d", x_full_1up), array("d", y_full_1up))
      gr_full_1up.SetLineStyle(3)
      gr_full_1up.SetLineWidth(2)
      gr_full_1up.SetLineColor(ROOT.kRed)

      gr_full_1dn = ROOT.TGraph(len(x_full_1dn), array("d", x_full_1dn), array("d", y_full_1dn))
      gr_full_1dn.SetLineStyle(3)
      gr_full_1dn.SetLineWidth(2)
      gr_full_1dn.SetLineColor(ROOT.kRed)

      lg = ROOT.TLegend(0.6, 0.65, 0.9, 0.9)
      lg.AddEntry(gr_asym, 'Asymptotic, Expected', 'l')
      lg.AddEntry(gr_asym_1up, 'Asymptotic, 65% expected', 'l')
      lg.AddEntry(gr_full, 'HybridNew, Expected', 'l')
      lg.AddEntry(gr_full_1up, 'HybridNew, 65% expected', 'l')

      c1 = 0
      c1 = ROOT.TCanvas('c1','',600,600)
      c1.cd()

      hist_dummy = 0
      hist_dummy = ROOT.TH1D("hist_dummy", "", 5000, 0., 5000.)
      hist_dummy.Draw("hist")

      hist_dummy.GetYaxis().SetTitle("Cross section (fb)")
      hist_dummy.GetYaxis().SetRangeUser(1, 3000)
      hist_dummy.GetYaxis().SetLabelSize(0.050)

      hist_dummy.GetXaxis().SetTitle("m_{N} (GeV)")
      hist_dummy.GetXaxis().SetRangeUser(0, float(mWR)+100)
      hist_dummy.GetXaxis().SetLabelSize(0.037)

      c1.SetLogy()

      gr_asym.Draw("lsame")
      gr_asym_1up.Draw("lsame")
      gr_asym_1dn.Draw("lsame")

      gr_full.Draw("lsame")
      gr_full_1up.Draw("lsame")
      gr_full_1dn.Draw("lsame")

      lg.Draw()
      tlatex_mass.DrawLatex(0.24, 0.87, channel+' '+region)
      tlatex_mass.DrawLatex(0.24, 0.82, 'm_{W_{R}} = '+str(mWR)+' GeV')

      c1.SaveAs(outDir+channel+'_'+region+'_'+str(mWR)+'.pdf')
      c1.Close()
