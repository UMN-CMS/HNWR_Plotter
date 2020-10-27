import os,ROOT,math
import CMS_lumi, tdrstyle

PLOT_PATH = os.environ['PLOT_PATH']
outbasedir = PLOT_PATH+'/EGamma/'
os.system('mkdir -p '+outbasedir)

Years = [
"2016",
"2017",
#"2017_WithEle115",
#"2018_OldHEEP",
"2018",
]

For2018LOtoNLOSyst = 2.

tdrstyle.setTDRStyle()

bins_eta = [-2.5, -1.566, -1.4442, 0.0, 1.4442, 1.566, 2.5]

f_template = ROOT.TFile('HistTemplate.root')
h_template_b = f_template.Get('hist_b')
h_template_e = f_template.Get('hist_e')

for Year in Years:

  bins_pt_b = [50, 55, 60, 65, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 300, 500, 2000]
  bins_pt_e = [50, 55, 60, 65, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 300, 2000]
  if Year=="2017": ## no Ele115
    bins_pt_e = [50, 55, 60, 65, 70, 80, 90, 100, 150, 200, 250, 300, 2000]

  lumi = 35.9
  if "2017" in Year:
    lumi = 41.5
  elif "2018" in Year:
    lumi = 59.7

  os.system('mkdir -p '+outbasedir+str(Year))
  f_out = ROOT.TFile(outbasedir+str(Year)+'/egammaEffi.txt_EGM2D.root','RECREATE')

  f_b = ROOT.TFile('Barrel/'+str(Year)+'/egammaEffi.txt_EGM2D.root')
  f_e = ROOT.TFile('EndCap/'+str(Year)+'/egammaEffi.txt_EGM2D.root')

  W = 800
  H = 800
  yUp = 0.45

  c = ROOT.TCanvas('c1','',50,50,H,W)
  c.SetTopMargin(0.055)
  c.SetBottomMargin(0.10)
  c.SetLeftMargin(0.12)

  p1 = ROOT.TPad( 'c1_up', '', 0, yUp, 1,   1, 0,0,0)
  p2 = ROOT.TPad( 'c1_do', '', 0,   0, 1, yUp, 0,0,0)
  p1.SetBottomMargin(0.0075)
  p1.SetTopMargin(   c.GetTopMargin()*1/(1-yUp))
  p2.SetTopMargin(   0.0075)
  p2.SetBottomMargin( c.GetBottomMargin()*1/yUp)
  p1.SetLeftMargin( c.GetLeftMargin() )
  p2.SetLeftMargin( c.GetLeftMargin() )
  firstGraph = True
  leg = ROOT.TLegend(0.5,0.80,0.95 ,0.92)
  leg.SetFillColor(0)
  leg.SetBorderSize(0)

  p1.SetLogx()
  p2.SetLogx()
  xMin = 50
  xMax = 2000

  #### eff
  gr_DATAEff_b = 0
  gr_DATAEff_e = 0
  gr_MCEff_b = 0
  gr_MCEff_e = 0
  gr_sf_b = 0
  gr_sf_e = 0

  for which in range(0,3):

    histname = 'EGamma_SF2D'
    Type = 'SF'
    if which==1:
      histname = 'EGamma_EffData2D'
      Type = 'DataEff'
    elif which==2:
      histname = 'EGamma_EffMC2D'
      Type = 'MCEff'

    h_b = f_b.Get(histname)
    h_e = f_e.Get(histname)

    #### Barrel

    # 2D

    h_b_out = h_b.Clone()
    h_b_out.SetName(Type+'_TH2F_Barrel')
    h_b_out.SetTitle('')
    for ix in range(0,len(bins_eta)-1):
      if not (ix+1==3 or ix+1==4):
        for iy in range(0,len(bins_pt_b)-1):
          h_b_out.SetBinContent(ix+1,iy+1,0)
      else:
        if "2018" in Year:
          for iy in range(0,len(bins_pt_b)-1):
            this_err = h_b_out.GetBinError(ix+1,iy+1)
            h_b_out.SetBinError(ix+1,iy+1,For2018LOtoNLOSyst*this_err)

    # 1D

    h_b_1D = h_template_b.Clone()
    h_b_1D.SetName(Type+'_TH1F_Barrel')
    gr_b_1D = ROOT.TGraphErrors(len(bins_pt_b)-1)
    gr_b_1D.SetName(Type+'_TGraphErrors_Barrel')
    for iy in range(0,len(bins_pt_b)-1):

      eff_minus = h_b.GetBinContent(3, iy+1)
      efferr_minus = h_b.GetBinError(3, iy+1)

      eff_plus = h_b.GetBinContent(4, iy+1)
      efferr_plus = h_b.GetBinError(4, iy+1)

      if which==2:
        efferr_minus=1
        efferr_plus=1

      errData2 = 1.0 / (1.0/(efferr_minus*efferr_minus)+1.0/(efferr_plus*efferr_plus))
      wData_minus   = 1.0 / (efferr_minus * efferr_minus) * errData2
      wData_plus    = 1.0 / (efferr_plus * efferr_plus) * errData2

      newEffData      = wData_minus * eff_minus + wData_plus * eff_plus;
      newErrEffData   = math.sqrt(errData2)
      if which==2:
        newErrEffData = 0.
      #### No LO-to-NLO syst available
      if Year==2018:
        newErrEffData = For2018LOtoNLOSyst*newErrEffData

      h_b_1D.SetBinContent(iy+1, newEffData)
      h_b_1D.SetBinError(iy+1, newErrEffData)

      gr_b_1D.SetPoint     (iy, (bins_pt_b[iy+1]+bins_pt_b[iy])/2., newEffData)
      gr_b_1D.SetPointError(iy, (bins_pt_b[iy+1]-bins_pt_b[iy])/2., newErrEffData)

    #### EndCap

    # 2D

    h_e_out = h_e.Clone()
    h_e_out.SetName(Type+'_TH2F_EndCap')
    h_e_out.SetTitle('')
    for ix in range(0,len(bins_eta)-1):
      if not (ix+1==1 or ix+1==6):
        for iy in range(0,len(bins_pt_e)-1):
          h_e_out.SetBinContent(ix+1,iy+1,0)
      else:
        if Year==2018:
          for iy in range(0,len(bins_pt_e)-1):
            this_err = h_e_out.GetBinError(ix+1,iy+1)
            h_e_out.SetBinError(ix+1,iy+1,For2018LOtoNLOSyst*this_err)

    # 1D

    h_e_1D = h_template_e.Clone()
    h_e_1D.SetName(Type+'_TH1F_EndCap')
    gr_e_1D = ROOT.TGraphErrors(len(bins_pt_e)-1)
    gr_e_1D.SetName(Type+'_TGraphErrors_EndCap')
    for iy in range(0,len(bins_pt_e)-1):

      eff_minus = h_e.GetBinContent(1, iy+1)
      efferr_minus = h_e.GetBinError(1, iy+1)

      eff_plus = h_e.GetBinContent(6, iy+1)
      efferr_plus = h_e.GetBinError(6, iy+1)

      if which==2:
        efferr_minus=1
        efferr_plus=1

      errData2 = 1.0 / (1.0/(efferr_minus*efferr_minus)+1.0/(efferr_plus*efferr_plus))
      wData_minus   = 1.0 / (efferr_minus * efferr_minus) * errData2
      wData_plus    = 1.0 / (efferr_plus * efferr_plus) * errData2

      newEffData      = wData_minus * eff_minus + wData_plus * eff_plus;
      newErrEffData   = math.sqrt(errData2)
      if which==2:
        newErrEffData = 0.
      #### No LO-to-NLO syst available
      if "2018" in Year:
        newErrEffData = For2018LOtoNLOSyst*newErrEffData

      h_e_1D.SetBinContent(iy+1, newEffData)
      h_e_1D.SetBinError(iy+1, newErrEffData)

      gr_e_1D.SetPoint     (iy, (bins_pt_e[iy+1]+bins_pt_e[iy])/2., newEffData)
      gr_e_1D.SetPointError(iy, (bins_pt_e[iy+1]-bins_pt_e[iy])/2., newErrEffData)

    #### SF
    if which==0:

      gr_sf_b = gr_b_1D.Clone()
      gr_sf_e = gr_e_1D.Clone()

    #### Eff
    if which==1:

      gr_DATAEff_b = gr_b_1D.Clone()
      gr_DATAEff_e = gr_e_1D.Clone()

    elif which==2:

      gr_MCEff_b = gr_b_1D.Clone()
      gr_MCEff_e = gr_e_1D.Clone()

    f_out.cd()

    h_b_out.Write()
    #h_b_1D.Write()
    #gr_b_1D.Write()

    h_e_out.Write()
    #h_e_1D.Write()
    #gr_e_1D.Write()

  #### Eff
  p1.cd()
  gr_DATAEff_b.SetLineWidth(2)
  gr_DATAEff_b.SetMarkerColor(ROOT.kBlack)
  gr_DATAEff_b.SetLineColor(ROOT.kBlack)
  gr_DATAEff_b.GetHistogram().GetYaxis().SetTitle("Data efficiency" )
  gr_DATAEff_b.GetHistogram().GetYaxis().SetTitleOffset(1)

  gr_MCEff_b.SetLineWidth(2)
  gr_MCEff_b.SetLineStyle(3)
  gr_MCEff_b.SetMarkerColor(ROOT.kBlack)
  gr_MCEff_b.SetMarkerStyle(4)
  gr_MCEff_b.SetLineColor(ROOT.kBlack)

  gr_DATAEff_e.SetLineWidth(2)
  gr_DATAEff_e.SetMarkerColor(ROOT.kRed)
  gr_DATAEff_e.SetLineColor(ROOT.kRed)

  gr_MCEff_e.SetLineWidth(2)
  gr_MCEff_e.SetLineStyle(3)
  gr_MCEff_e.SetMarkerColor(ROOT.kRed)
  gr_MCEff_e.SetMarkerStyle(4)
  gr_MCEff_e.SetLineColor(ROOT.kRed)

  gr_DATAEff_b.Draw("AP")
  gr_MCEff_b.Draw("PSAME")
  gr_DATAEff_e.Draw("PSAME")
  gr_MCEff_e.Draw("PSAME")

  gr_DATAEff_b.GetHistogram().GetXaxis().SetLimits(xMin,xMax)
  gr_DATAEff_b.GetYaxis().SetRangeUser(0.5, 1.5)

  leg.AddEntry( gr_DATAEff_b, '%1.3f #leq | #eta | #leq  %1.3f (Data)' % (0.000,1.444), "PL")
  leg.AddEntry( gr_MCEff_b, '%1.3f #leq | #eta | #leq  %1.3f (MC)' % (0.000,1.444), "PL")
  leg.AddEntry( gr_DATAEff_e, '%1.3f #leq | #eta | #leq  %1.3f (Data)' % (1.566,2.500), "PL")
  leg.AddEntry( gr_MCEff_e, '%1.3f #leq | #eta | #leq  %1.3f (MC)' % (1.566,2.500), "PL")

  #### Sf
  p2.cd()
  gr_sf_b.SetLineWidth(2)
  gr_sf_b.SetMarkerColor(ROOT.kBlack)
  gr_sf_b.SetLineColor(ROOT.kBlack)
  gr_sf_b.GetHistogram().GetXaxis().SetTitle("p_{T}  [GeV]")
  gr_sf_b.GetHistogram().GetXaxis().SetTitleOffset(1)
  gr_sf_b.GetHistogram().GetYaxis().SetTitle("Data / MC " )
  gr_sf_b.GetHistogram().GetYaxis().SetTitleOffset(1)
  gr_sf_b.GetHistogram().GetXaxis().SetMoreLogLabels()
  gr_sf_b.GetHistogram().GetXaxis().SetNoExponent()

  gr_sf_e.SetLineWidth(2)
  gr_sf_e.SetMarkerColor(ROOT.kRed)
  gr_sf_e.SetLineColor(ROOT.kRed)

  gr_sf_b.Draw("AP")
  gr_sf_e.Draw("PSAME")

  gr_sf_b.GetHistogram().GetXaxis().SetLimits(xMin,xMax)
  gr_sf_b.GetYaxis().SetRangeUser(0.85,1.12)

  c.cd()
  p1.Draw()
  p2.Draw()

  leg.Draw()
  CMS_lumi.lumi_13TeV = "%+3.1f fb^{-1}" % lumi
  CMS_lumi.CMS_lumi(c, 4, 10)

  c.SaveAs(outbasedir+'/'+str(Year)+'/'+'summary.pdf')
  c.Close()

  f_out.Close()


#        errData2 = 1.0 / (1.0/(self.errEffData*self.errEffData)+1.0/(eff.errEffData*eff.errEffData))
#
#        wData1   = 1.0 / (self.errEffData * self.errEffData) * errData2
#        wData2   = 1.0 / (eff .errEffData * eff .errEffData) * errData2
#        newEffData      = wData1 * self.effData + wData2 * eff.effData;
#        newErrEffData   = math.sqrt(errData2)
