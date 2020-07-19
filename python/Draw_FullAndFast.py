import os,ROOT,math
import CMS_lumi, tdrstyle
import canvas_margin
import mylib
from array import array

ROOT.TH1.AddDirectory(False)

tdrstyle.setTDRStyle()

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

Years = [
"2016",
"2017",
"2018",
]

nEta=4

Leptons = [
"Electron",
"Muon",
]

ptBins = [50, 150, 300, 500, 2000]
nptBins = len(ptBins)-1

for Year in Years:

  print '@@@@ Year = '+Year

  basedir = FILE_PATH+'/'+dataset+'/CompareFullAndFast/'+str(Year)+'/'
  outDir = PLOT_PATH+'/'+dataset+'/CompareFullAndFast/'+str(Year)+'/'
  os.system('mkdir -p '+outDir)

  #### get mass list
  os.system('ls -1 '+basedir+'CompareFullAndFast_Official_FullSim_WRtoNLtoLLJJ_*.root &> tmp_CompareFullAndFast.txt')
  lines = open('tmp_CompareFullAndFast.txt')
  os.system('rm tmp_CompareFullAndFast.txt')
  for line in lines:
    mass = line.strip('\n').split('/')[-1].replace('CompareFullAndFast_Official_FullSim_WRtoNLtoLLJJ_','').replace('.root','')
    print '@@@@   running '+mass
    f_Full = ROOT.TFile(basedir+'/CompareFullAndFast_Official_FullSim_WRtoNLtoLLJJ_'+mass+'.root')
    f_Fast = ROOT.TFile(basedir+'/CompareFullAndFast_WRtoNLtoLLJJ_'+mass+'.root')

    for Lepton in Leptons:

      for iEta in range(0,nEta):

        c1 = ROOT.TCanvas('c1', '', 800, 800)

        c1_up = ROOT.TPad("c1_up", "", 0, 0.25, 1, 1)
        c1_down = ROOT.TPad("c1_down", "", 0, 0, 1, 0.25)
        c1, c1_up, c1_down = canvas_margin.canvas_margin(c1, c1_up, c1_down)
        c1.Draw()
        c1_up.Draw()
        c1_down.Draw()

        c1_up.cd()

        lg = ROOT.TLegend(0.55, 0.7, 0.90, 0.90)
        lg.SetBorderSize(0)
        lg.SetFillStyle(0)

        c1.cd()

        str_channel = ""
        channelname = ROOT.TLatex()
        channelname.SetNDC()
        channelname.SetTextSize(0.037)
        channelname.DrawLatex(0.2, 0.88, str_channel)

        latex_CMSPriliminary = ROOT.TLatex()
        latex_Lumi = ROOT.TLatex()

        latex_CMSPriliminary.SetNDC()
        latex_Lumi.SetNDC()
        latex_CMSPriliminary.SetTextSize(0.035)
        latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS Simulation} #font[42]{#it{#scale[0.8]{Preliminary}}}")

        latex_Lumi.SetTextSize(0.035)
        latex_Lumi.SetTextFont(42)
        latex_Lumi.DrawLatex(0.73, 0.96, mylib.TotalLumi(float(Year))+" fb^{-1} (13 TeV)")

        c1_up.cd()
        hist_dummy_up = ROOT.TH1D('hist_dummy_up', '', nptBins, array("d", ptBins))
        hist_dummy_up.GetXaxis().SetRangeUser(50, 2000)
        hist_dummy_up.GetYaxis().SetRangeUser(0.4, 1.2)
        hist_dummy_up.GetYaxis().SetTitle("ID efficiency")
        hist_dummy_up.Draw("hist")

        c1_down.cd()
        hist_dummy_down = ROOT.TH1D('hist_dummy_down', '', nptBins, array("d", ptBins))
        hist_dummy_down.GetXaxis().SetRangeUser(50, 2000)
        hist_dummy_down.GetXaxis().SetTitle('p_{T} (GeV)')
        hist_dummy_down.GetYaxis().SetRangeUser(0.8, 1.1)
        hist_dummy_down.GetYaxis().SetTitle('Full/Fast')
        hist_dummy_down.SetNdivisions(504,"Y")
        hist_dummy_down.Draw("hist")

        g1_x = [-9000, 9000]
        g1_y = [1, 1]
        g1 = ROOT.TGraph(2, array("d", g1_x ), array("d", g1_y ))
        g1.Draw("same")

        hist_dummy_up, hist_dummy_down = canvas_margin.hist_axis(hist_dummy_up, hist_dummy_down)

        h_Ref = 0
        y_max, y_min = -999, 9999999999

        #### Loop over eta
        histdict=dict()

        #### Full
        h_Full_Den = f_Full.Get(Lepton+"_Den_Eta"+str(iEta)+"_Pt")
        h_Full_Num = f_Full.Get(Lepton+"_PassID_Eta"+str(iEta)+"_Pt")
        if not h_Full_Den:
          continue
        h_Full_Den = h_Full_Den.Rebin(nptBins, h_Full_Den.GetName(), array("d", ptBins))
        h_Full_Num = h_Full_Num.Rebin(nptBins, h_Full_Num.GetName(), array("d", ptBins))

        TEff_Full = ROOT.TEfficiency(h_Full_Num,h_Full_Den)
        gr_Eff_Full = TEff_Full.CreateGraph()
        gr_Eff_Full.SetLineWidth(2)
        gr_Eff_Full.SetLineColor(ROOT.kBlack)

        for ix in range(0,h_Full_Den.GetXaxis().GetNbins()):
          x_l = h_Full_Den.GetXaxis().GetBinLowEdge(ix+1)
          x_r = h_Full_Den.GetXaxis().GetBinUpEdge(ix+1)
          y_Den = h_Full_Den.GetBinContent(ix+1)
          e_Den = h_Full_Den.GetBinError(ix+1)
          y_Num = h_Full_Num.GetBinContent(ix+1)
          e_Num = h_Full_Num.GetBinError(ix+1)
          #print '[%1.0f, %1.0f] : %1.3f %1.3f %1.3f %1.3f'%(x_l,x_r,y_Den,e_Den,y_Num,e_Num)
          #### Zero
          if y_Num<=0.:
            y_Num=1.
          #### Relative err
          re_Den = e_Den/y_Den
          re_Num = e_Num/y_Num
          #### New err
          newErr = math.sqrt(re_Den*re_Den+re_Num*re_Num)*(y_Num/y_Den)
          #### Set
          h_Full_Num.SetBinContent(ix+1, y_Num/y_Den)
          h_Full_Num.SetBinError(ix+1, newErr)

        h_Full_Num.SetLineWidth(2)
        h_Full_Num.SetLineColor(ROOT.kBlack)

        #### Fast

        h_Fast_Den = f_Fast.Get(Lepton+"_Den_Eta"+str(iEta)+"_Pt")
        h_Fast_Num = f_Fast.Get(Lepton+"_PassID_Eta"+str(iEta)+"_Pt")
        if not h_Fast_Den:
          continue
        h_Fast_Den = h_Fast_Den.Rebin(nptBins, h_Fast_Den.GetName(), array("d", ptBins))
        h_Fast_Num = h_Fast_Num.Rebin(nptBins, h_Fast_Num.GetName(), array("d", ptBins))

        TEff_Fast = ROOT.TEfficiency(h_Fast_Num,h_Fast_Den)
        gr_Eff_Fast = TEff_Fast.CreateGraph()
        gr_Eff_Fast.SetLineWidth(2)
        gr_Eff_Fast.SetLineColor(ROOT.kRed)

        for ix in range(0,h_Fast_Den.GetXaxis().GetNbins()):
          x_l = h_Fast_Den.GetXaxis().GetBinLowEdge(ix+1)
          x_r = h_Fast_Den.GetXaxis().GetBinUpEdge(ix+1)
          y_Den = h_Fast_Den.GetBinContent(ix+1)
          e_Den = h_Fast_Den.GetBinError(ix+1)
          y_Num = h_Fast_Num.GetBinContent(ix+1)
          e_Num = h_Fast_Num.GetBinError(ix+1)
          #print '[%1.0f, %1.0f] : %1.3f %1.3f %1.3f %1.3f'%(x_l,x_r,y_Den,e_Den,y_Num,e_Num)
          #### Zero
          if y_Num<=0.:
            y_Num=1.
          #### Relative err
          re_Den = e_Den/y_Den
          re_Num = e_Num/y_Num
          #### New err
          newErr = math.sqrt(re_Den*re_Den+re_Num*re_Num)*(y_Num/y_Den)
          #### Set
          h_Fast_Num.SetBinContent(ix+1, y_Num/y_Den)
          h_Fast_Num.SetBinError(ix+1, newErr)

        h_Fast_Num.SetLineWidth(2)
        h_Fast_Num.SetLineColor(ROOT.kRed)

        histdict["Full_"+Lepton+"_Den_Eta"+str(iEta)+"_Pt"] = h_Full_Num
        histdict["Fast_"+Lepton+"_Den_Eta"+str(iEta)+"_Pt"] = h_Fast_Num
        c1_up.cd()

        lg.AddEntry(h_Full_Num, mass+' (FullSim)', 'l')
        lg.AddEntry(h_Fast_Num, mass+' (FastSim)', 'l')
        lg.Draw()

        h_Full_Num.Draw("histsamee1")
        h_Full_Num.GetXaxis().SetRangeUser(50,1000)
        h_Fast_Num.Draw("histsamee1")
        #gr_Eff_Full.Draw("psame")
        #gr_Eff_Fast.Draw("psame")

        #### ratio
        h_SF = h_Full_Num.Clone()
        for ix in range(0,h_Full_Num.GetXaxis().GetNbins()):
          x_l = h_Full_Num.GetXaxis().GetBinLowEdge(ix+1)
          x_r = h_Full_Num.GetXaxis().GetBinUpEdge(ix+1)
          y_Den = h_Fast_Num.GetBinContent(ix+1)
          e_Den = h_Fast_Num.GetBinError(ix+1)
          y_Num = h_Full_Num.GetBinContent(ix+1)
          e_Num = h_Full_Num.GetBinError(ix+1)
          #print '[%1.0f, %1.0f] : %1.3f %1.3f %1.3f %1.3f'%(x_l,x_r,y_Den,e_Den,y_Num,e_Num)
          #### Zero
          if y_Num<=0.:
            y_Num=1.
          #### Relative err
          re_Den = e_Den/y_Den
          re_Num = e_Num/y_Num
          #### New err
          newErr = math.sqrt(re_Den*re_Den+re_Num*re_Num)*(y_Num/y_Den)
          #### Set
          h_SF.SetBinContent(ix+1, y_Num/y_Den)
          h_SF.SetBinError(ix+1, newErr)
        c1_down.cd()
        h_SF.SetLineColor(ROOT.kBlack)
        h_SF.Draw("histsamee1")

        c1.SaveAs(outDir+'/'+Lepton+'_'+mass+'_Eta'+str(iEta)+'.pdf')
        c1.Close()
      #### END Eta region loop

    #### END [Electron, Muon] loop

    f_Full.Close()

    f_Fast.Close()
