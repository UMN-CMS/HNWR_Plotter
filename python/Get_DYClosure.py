import os,ROOT,math
import CMS_lumi, tdrstyle, mylib, canvas_margin
from array import array

ROOT.TH1.AddDirectory(False)

tdrstyle.setTDRStyle()

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

outdirBase = PLOT_PATH+'/'+dataset+'/DYClosure/'

Years = [
"2016",
"2017",
"2018",
]

Regions = [
'Resolved',
'Boosted',
]

Channels = [
'Electron',
'Muon',
]

CRName = 'DYCR2'

DYName = 'DYJets_MG_HT_Reweighted_ReshapedDYCR1'
Bkgds = [
'TT_TW',
'NonPrompt',
'Others',
]


for Year in Years:

  basedir = FILE_PATH+'/'+dataset+'/Regions/'+Year+'/'

  out = ROOT.TFile(outdirBase+'/'+Year+'/DYClorueUncertainty.root','RECREATE')

  f_DYReshapeRwg = ROOT.TFile(PLOT_PATH+'/'+dataset+'/DYReshapeRatio/'+Year+'/shapes.root')

  for Region in Regions:

    h_data_Sum = 0
    h_DY_Sum = 0
    h_Bkgd_Sum = 0

    h_DYReshapeRwg = f_DYReshapeRwg.Get(Region+'_ratio_AllCh')

    for i_Channel in range(0,len(Channels)):
      Channel = Channels[i_Channel]
      dirName = 'HNWR_Single'+Channel+'_'+Region+'_'+CRName

      ## data
      f_data = ROOT.TFile(basedir+'/HNWRAnalyzer_SkimTree_LRSMHighPt_data_Single'+Channel+'.root')
      h_data = f_data.Get(dirName+'/WRCand_Mass_'+dirName)

      ## DY
      f_DY = ROOT.TFile(basedir+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+DYName+'.root')
      h_DY = f_DY.Get(dirName+'/WRCand_Mass_'+dirName)
      MCSF, MCSFerr = mylib.GetDYNormSF(int(Year), dirName)
      h_DY.Scale(MCSF)

      ## Bkgd
      h_Bkgd = 0
      for i_Bkgd in range(0,len(Bkgds)):
        Bkgd = Bkgds[i_Bkgd]
        f_Bkgd_tmp = ROOT.TFile(basedir+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+Bkgd+'.root')
        h_Bkgd_tmp = f_Bkgd_tmp.Get(dirName+'/WRCand_Mass_'+dirName)
        if i_Bkgd==0:
          h_Bkgd = h_Bkgd_tmp.Clone()
        else:
          h_Bkgd.Add(h_Bkgd_tmp)

      ## Add
      if i_Channel==0:
        h_data_Sum = h_data.Clone()
        h_DY_Sum = h_DY.Clone()
        h_Bkgd_Sum = h_Bkgd.Clone()
      else:
        h_data_Sum.Add(h_data)
        h_DY_Sum.Add(h_DY)
        h_Bkgd_Sum.Add(h_Bkgd)

    h_data_Sum = mylib.RebinWRMass(h_data_Sum, Region, Year)
    h_DY_Sum = mylib.RebinWRMass(h_DY_Sum, Region, Year)
    h_Bkgd_Sum = mylib.RebinWRMass(h_Bkgd_Sum, Region, Year)

    h_TotalBkgd = h_Bkgd_Sum.Clone('h_TotalBkgd')
    h_TotalBkgd.Add(h_DY_Sum)

    ## uncertainty
    h_Unct = h_DY_Sum.Clone(Region)
    h_NewDYReshapeRwg = h_DYReshapeRwg.Clone()
    for ix in range(0,h_DY_Sum.GetXaxis().GetNbins()):
      iBin = ix+1
      x_l = h_DY_Sum.GetXaxis().GetBinLowEdge(iBin)
      x_r = h_DY_Sum.GetXaxis().GetBinUpEdge(iBin)
      y_Data = h_data_Sum.GetBinContent(iBin)
      y_DY_Sum = h_DY_Sum.GetBinContent(iBin)
      y_Bkgd_Sum = h_Bkgd_Sum.GetBinContent(iBin)

      diff = abs(y_Data-y_DY_Sum-y_Bkgd_Sum)
      relerr = diff/y_DY_Sum
      h_Unct.SetBinContent(iBin,relerr)
      h_Unct.SetBinError(iBin,0)
      print '[%d,%d] : Data = %d, DY = %1.2f, Bkgd = %1.2f -> Relative error on DY = %1.2f'%(x_l, x_r, y_Data, y_DY_Sum, y_Bkgd_Sum, relerr)
      ## Rewrite DYReshape rootfile
      ## h_DYReshapeRwg : starts from 0
      this_DYrwg = h_NewDYReshapeRwg.GetBinContent(iBin+1) ## iBin starts from 1
      this_DYrwgErr = h_NewDYReshapeRwg.GetBinError(iBin+1)
      this_NewErr = relerr * this_DYrwg
      newErr = math.sqrt( this_DYrwgErr*this_DYrwgErr + this_NewErr*this_NewErr )
      h_NewDYReshapeRwg.SetBinError(iBin+1,newErr)
      #print '[%d,%d] : iBin+1 = %d, newErr = %1.2f'%(x_l, x_r, iBin+1, newErr)

    out.cd()
    h_Unct.Write()
    h_NewDYReshapeRwg.Write()

    ## Draw

    c1 = ROOT.TCanvas('c1', '', 800, 800)

    h_dummy_up = ROOT.TH1D('hist_dummy_up', '', 800, 0., 8000.)
    h_dummy_up.Draw('hist')
    h_dummy_up.GetYaxis().SetRangeUser(0.,1.5)

    ## y=1 graph
    g1_x = [-9000, 9000]
    g1_y = [1, 1]
    g1 = ROOT.TGraph(2, array("d", g1_x ), array("d", g1_y ))
    g1.Draw("same")

    ## up

    h_DYReshapeRwg.SetLineColor(ROOT.kBlack)
    h_NewDYReshapeRwg.SetLineColor(ROOT.kRed)

    h_NewDYReshapeRwg.Draw("histsamee1")
    h_DYReshapeRwg.Draw("histsamee1")

    c1.SaveAs(outdirBase+'/'+Year+'/'+Region+'.pdf')

  out.Close()

