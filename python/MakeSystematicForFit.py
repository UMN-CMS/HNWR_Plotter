#!/usr/bin/env python

import os,ROOT,math
import CMS_lumi, tdrstyle
from array import array
import argparse

def UseWideBinError(hist, binnings):
  NOldBin = hist.GetXaxis().GetNbins()
  NNewBin = len(binnings)-1
  NMergedBinSize = NOldBin - NNewBin + 1
  WideBinHist = hist.Rebin(NNewBin, hist.GetName()+"_WideBin", array("d", binnings))

  Splitted_BinContent = WideBinHist.GetBinContent( NNewBin )
  Splitted_BinError = WideBinHist.GetBinError( NNewBin )

  for ix in range(0,hist.GetXaxis().GetNbins()):
    x_l = hist.GetXaxis().GetBinLowEdge(ix+1)
    x_r = hist.GetXaxis().GetBinUpEdge(ix+1)
    #print '[[UseWideBinError] %d,%d] : %f'%(x_l,x_r,hist.GetBinContent(ix+1))

  #print "[UseWideBinError] %f +- %f"%(Splitted_BinContent, Splitted_BinError)

  hist_out = hist.Clone()
  for ix in range(0,hist.GetXaxis().GetNbins()):
    if ix+1 >= NNewBin:
      hist_out.SetBinContent( ix+1, Splitted_BinContent/NMergedBinSize )
      hist_out.SetBinError( ix+1, Splitted_BinError/math.sqrt(NMergedBinSize) )
  return hist_out


parser = argparse.ArgumentParser(description='SKFlat Command')
parser.add_argument('-y', dest='Year')
parser.add_argument('-i', dest='Iter')
args = parser.parse_args()

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
ENV_PLOT_PATH = os.environ['PLOT_PATH']

logbasedir = ENV_PLOT_PATH+dataset+'/FitBackgrounds/2016/'

regions = [
'Resolved',
'Boosted',
]
channels = [
'Electron',
'Muon',
]
samples =[
'DYJets_MG_HT_Reweighted',
'EMuMethod_TTLX_powheg',
]

Systs = [
    "JetRes",
    "JetEn",
    "MuonReco",
    "MuonEn",
    "MuonIDSF",
    "MuonTriggerSF",
    "ElectronRes",
    "ElectronEn",
    "ElectronIDSF",
    "ElectronTriggerSF",
    "LSFSF",
    "PU",
    "ZPtRw",
]

for i_sample in range(0,len(samples)):

  sample = samples[i_sample]

  file_out = ROOT.TFile(FILE_PATH+'/'+dataset+'/Regions/'+args.Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_FromFit_'+sample+'.root','RECREATE')

  f_ori = ROOT.TFile(FILE_PATH+'/'+dataset+'/Regions/'+args.Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+sample+'.root')

  for i_region in range(0,len(regions)):

    region = regions[i_region]

    for i_channel in range(0,len(channels)):

      channel = channels[i_channel]

      fitoutput = ROOT.TFile(ENV_PLOT_PATH+'/'+dataset+'/FitBackgrounds/'+args.Year+'/Iter_'+args.Iter+'/shapes_'+region+'_SR_'+channel+'_'+sample+'.root')
      hist_fitted = fitoutput.Get("Central")

      print '%s\t%s\t%s' % (sample, region, channel)

      #### Now let's get the scale factor
      dirName = 'HNWR_Single'+channel+'_'+region+'_SR'
      hist_prefit = f_ori.Get(dirName+'/WRCand_Mass_'+dirName)
      hist_prefit.Rebin(40)

      file_out.cd()
      file_out.mkdir(dirName)
      file_out.cd(dirName)
      hist_fitted.SetName( hist_prefit.GetName() )
      hist_fitted.Write()
      file_out.cd()

      #### Find non zero bin
      LastNonZeroBinIX=0
      WideBins = []
      NBins = hist_prefit.GetXaxis().GetNbins()
      for ix in range(0,NBins):
        x_l = hist_prefit.GetXaxis().GetBinLowEdge(ix+1)
        x_r = hist_prefit.GetXaxis().GetBinUpEdge(ix+1)
        print '[%d,%d] : %f'%(x_l,x_r,hist_prefit.GetBinContent(ix+1))
        if x_r<=800:
          WideBins.append(x_l)
          continue
        if hist_prefit.GetBinContent(ix+1) > 0:
          WideBins.append(x_l)
        else:
          #print '[%d,%d] : %f'%(x_l,x_r,hist_prefit.GetBinContent(ix+1))
          LastNonZeroBinIX=ix
          WideBins.append( hist_prefit.GetXaxis().GetBinUpEdge(NBins) )
          break
      print "LastNonZeroBinIX = "+str(LastNonZeroBinIX)
      print WideBins
      print "--> Merging"

      hist_prefit_splitted = UseWideBinError(hist_prefit, WideBins)
      for ix in range(0,NBins):
        x_l = hist_prefit_splitted.GetXaxis().GetBinLowEdge(ix+1)
        x_r = hist_prefit_splitted.GetXaxis().GetBinUpEdge(ix+1)
        print '[%d,%d] : %f'%(x_l,x_r,hist_prefit_splitted.GetBinContent(ix+1))

      for Syst in Systs:

        dirUp = 'Syst_'+Syst+'Up_'+dirName
        hist_Up = f_ori.Get(dirUp+'/WRCand_Mass_'+dirUp)
        if not hist_Up:
          continue
        hist_Up.Rebin(40)
        hist_Up_splitted = UseWideBinError(hist_Up, WideBins)
        hist_Up_splitted.Divide(hist_prefit_splitted)

        if Syst =="JetRes":
          print "Checking JetRes Syst Up.."
          for ix in range(0,NBins):
            x_l = hist_Up_splitted.GetXaxis().GetBinLowEdge(ix+1)
            x_r = hist_Up_splitted.GetXaxis().GetBinUpEdge(ix+1)
            print '[%d,%d] : %f'%(x_l,x_r,hist_Up_splitted.GetBinContent(ix+1))

        dirDown = 'Syst_'+Syst+'Down_'+dirName
        hist_Down = f_ori.Get(dirDown+'/WRCand_Mass_'+dirDown)
        hist_Down.Rebin(40)
        hist_Down_splitted = UseWideBinError(hist_Down, WideBins)
        hist_Down_splitted.Divide(hist_prefit_splitted)

        for ix in range(0,hist_Up_splitted.GetXaxis().GetNbins()):

          hist_Up.SetBinContent(ix+1, hist_Up_splitted.GetBinContent(ix+1) * hist_fitted.GetBinContent(ix+1))
          hist_Down.SetBinContent(ix+1, hist_Down_splitted.GetBinContent(ix+1) * hist_fitted.GetBinContent(ix+1))

        file_out.cd()

        file_out.mkdir(dirUp)
        file_out.cd(dirUp)
        hist_Up.Write()
        file_out.cd()

        file_out.mkdir(dirDown)
        file_out.cd(dirDown)
        hist_Down.Write()
        file_out.cd()

      #### Finalize

      fitoutput.Close()

  file_out.Close()
