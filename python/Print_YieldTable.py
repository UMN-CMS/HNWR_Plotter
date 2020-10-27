import os,ROOT,math,mylib
import CMS_lumi, tdrstyle
from IsCorrelated import IsCorrelated

ROOT.TH1.AddDirectory(False)

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']

ResolvedIntegralStart = 2400
BoostedIntegralStart = 1200

Years = [
'2016',
'2017',
'2018',
]
channels = [
'SingleElectron',
'SingleMuon',
]
regions = [
'Resolved',
'Boosted',
]

ResolvedMasses = [
'WR5000_N3000',
]
BoostedMasses = [
'WR5000_N100',
]

Bkgds = [
'DYJets_MG_HT_Reweighted_Reshaped',
'TTLX_powheg',
'Others',
'total_background',
]

SumYields = [
# [ee resol, ee boosted, mm resol, mm boosted]
[0.,0.,0.,0.], ## DY
[0.,0.,0.,0.], ## TT
[0.,0.,0.,0.], ## Others
[0.,0.,0.,0.], ## total
]

#### Each year

for i_Year in range(0,len(Years)):

  Year = Years[i_Year]

  for i_channel in range(0,len(channels)):

    channel = channels[i_channel]

    channel_latex = '$\Pe\Pe$' if (channel=='SingleElectron') else '$\mu\mu$'

    for i_region in range(0,len(regions)):

      region = regions[i_region]

      dirName = 'HNWR_'+channel+'_'+region+'_SR'

      h_Data = 0

      if (i_channel==0) and (i_region==0):
        out = '\multirow{4}{*}{'+Year+'}      & \multirow{2}{*}{'+channel_latex+'}'
      elif (i_region==0):
        out = '                           & \multirow{2}{*}{'+channel_latex+'}'
      else:
        out = '                           &                          '
      out += ' & '+region

      f_Data = ROOT.TFile(WORKING_DIR+'/rootfiles/'+dataset+'/PostFit/'+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_data_'+channel+'.root')
      h_Data = f_Data.Get(dirName+'/WRCand_Mass_'+dirName)
      h_Data = mylib.RebinWRMass(h_Data, dirName, 2016)

      for i_Bkgd in range(0,len(Bkgds)):
        Bkgd = Bkgds[i_Bkgd]
        f_Bkgd = ROOT.TFile(WORKING_DIR+'/rootfiles/'+dataset+'/PostFit/'+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+Bkgd+'.root')
        h_Bkgd = f_Bkgd.Get(dirName+'/WRCand_Mass_'+dirName)

        integral_start = ResolvedIntegralStart if ('Resolved' in region) else BoostedIntegralStart
        integral_start = h_Bkgd.FindBin(integral_start)
        this_Bkgd_yield = 0
        this_Bkgd_err = ROOT.Double()
        this_Bkgd_yield = h_Bkgd.IntegralAndError(integral_start,9999,this_Bkgd_err)

        out += ' & $%1.2f \pm %1.2f$'%(this_Bkgd_yield, this_Bkgd_err)
        SumYields[i_Bkgd][2*i_channel+i_region] += this_Bkgd_yield

      alpha = 1. - 0.6827

      integral_start = ResolvedIntegralStart if ('Resolved' in region) else BoostedIntegralStart
      integral_start = h_Data.FindBin(integral_start)
      N = h_Data.Integral(integral_start,9999)
      L = 0.                                          if (N==0.) else (ROOT.Math.gamma_quantile(alpha/2.,N,1.))
      U = ( ROOT.Math.gamma_quantile_c(alpha,N+1,1) ) if (N==0.) else (ROOT.Math.gamma_quantile_c(alpha/2.,N+1.,1.))

      #out += ' & $%d^{+%1.1f}_{-%1.1f}$'%(N,N-L,U-N)+' \\\\'
      out += ' & $%d$'%(N)+' \\\\'

      print out
  print '\\hline'

#### Year Combined

for i_channel in range(0,len(channels)):

  channel = channels[i_channel]

  channel_latex = '$\Pe\Pe$' if (channel=='SingleElectron') else '$\mu\mu$'

  for i_region in range(0,len(regions)):

    region = regions[i_region]

    dirName = 'HNWR_'+channel+'_'+region+'_SR'

    h_Data = 0

    if (i_channel==0) and (i_region==0):
      out = '\multirow{4}{*}{Combinded} & \multirow{2}{*}{'+channel_latex+'}'
    elif (i_region==0):
      out = '                           & \multirow{2}{*}{'+channel_latex+'}'
    else:
      out = '                           &                          '
    out += ' & '+region

    f_Data = ROOT.TFile(WORKING_DIR+'/rootfiles/'+dataset+'/PostFit/YearCombined/HNWRAnalyzer_SkimTree_LRSMHighPt_data_'+channel+'.root')
    h_Data = f_Data.Get(dirName+'/WRCand_Mass_'+dirName)
    h_Data = mylib.RebinWRMass(h_Data, dirName, 2016)

    str_EEorMuMu = 'EE' if ('Electron' in channel) else 'MuMu'
    f_TotalBkgd = ROOT.TFile(WORKING_DIR+'/rootfiles/'+dataset+'/PostFit/YearCombined/HNWRAnalyzer_SkimTree_LRSMHighPt_total_background.root')
    h_TotalBkgd = f_TotalBkgd.Get(dirName+'/WRCand_Mass_'+dirName)
    h_TotalBkgd = mylib.RebinWRMass(h_TotalBkgd, dirName, 2016)

    LastBin_Data = h_Data.FindBin(5000)

    for i_Bkgd in range(0,len(Bkgds)):
      Bkgd = Bkgds[i_Bkgd]
      f_Bkgd = ROOT.TFile(WORKING_DIR+'/rootfiles/'+dataset+'/PostFit/YearCombined/HNWRAnalyzer_SkimTree_LRSMHighPt_'+Bkgd+'.root')
      h_Bkgd = f_Bkgd.Get(dirName+'/WRCand_Mass_'+dirName)
      LastBin_Bkgd = h_Bkgd.FindBin(5000)

      integral_start = ResolvedIntegralStart if ('Resolved' in region) else BoostedIntegralStart
      integral_start = h_Bkgd.FindBin(integral_start)
      this_Bkgd_yield = 0
      this_Bkgd_err = ROOT.Double()
      this_Bkgd_yield = h_Bkgd.IntegralAndError(integral_start,9999,this_Bkgd_err)

      this_rel_err = this_Bkgd_err/this_Bkgd_yield

      this_Yield_From_Sum = SumYields[i_Bkgd][2*i_channel+i_region]
      this_err_From_Sum = this_rel_err*this_Yield_From_Sum

      out += ' & $%1.2f \pm %1.2f$'%(this_Yield_From_Sum,this_err_From_Sum)

    alpha = 1. - 0.6827

    integral_start = ResolvedIntegralStart if ('Resolved' in region) else BoostedIntegralStart
    integral_start = h_Data.FindBin(integral_start)

    N = h_Data.Integral(integral_start,9999)
    L = 0.                                          if (N==0.) else (ROOT.Math.gamma_quantile(alpha/2.,N,1.))
    U = ( ROOT.Math.gamma_quantile_c(alpha,N+1,1) ) if (N==0.) else (ROOT.Math.gamma_quantile_c(alpha/2.,N+1.,1.))

    #out += ' & $%d^{+%1.1f}_{-%1.1f}$'%(N,U-N,N-L)+' \\\\'
    out += ' & $%d$'%(N)+' \\\\'

    print out

print '----------------------------------------------'
for a in SumYields:
  print a
