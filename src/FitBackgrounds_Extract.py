#!/usr/bin/env python

import os,ROOT,math
import CMS_lumi, tdrstyle
import argparse

parser = argparse.ArgumentParser(description='SKFlat Command')
parser.add_argument('-o', dest='Output')
args = parser.parse_args()

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
ENV_PLOT_PATH = os.environ['PLOT_PATH']

logbasedir = ENV_PLOT_PATH+dataset+'/FitBackgrounds/2016/'

out = open(args.Output,'w')

regions = [
'Resolved',
'Boosted',
]
channels = [
'Electron',
'Muon',
]
samples =[
'DY',
'tt',
]

for i in range(0,2):

  region = regions[i]

  for j in range(0,2):

    channel = channels[j]

    for k in range(0,2):

      sample = samples[k]

      Prefix = '%s\t%s\t%s'%(region,channel,sample)

      lines_log = open(logbasedir+'/log_%d_%d_%d.log' % (i,j,k) ).readlines()

      index_logstart = 0
      index_logend = 0
      for iline in range(0,len(lines_log)):
        if "@@@@ Fit results" in lines_log[iline]:
          index_logstart = iline
        if "@@@@ END Fit results" in lines_log[iline]:
          index_logend = iline

      CurrentFunction = ""
      CurrentPrefix = ""
      for iline in range(index_logstart+1, index_logend):
        line = lines_log[iline].strip('\n')
        if "@@@@   Func = " in line:
          CurrentFunction = line.replace('@@@@   Func = ','')
          CurrentPrefix = Prefix+'\t'+CurrentFunction
        else:
          ## RooRealVar::PolyExt_8Par_p0 = 5.20575 +/- 4.79425  L(1e-05 - 20)
          words = line.strip('\n').split()
          ParName = words[0].replace('RooRealVar::','')
          ParCentral = words[2]
          ParError = words[4]
          ParRangeLeft = words[5].replace('L(','')
          ParRangeRight = words[7].replace(')','')

          #if 'p0' in ParName:
          towrite =  CurrentPrefix+'\t%s\t%s\t%s\t%s\t%s\n'%(ParName,ParCentral,ParError,ParRangeLeft,ParRangeRight)
          out.write(towrite)

out.close()
