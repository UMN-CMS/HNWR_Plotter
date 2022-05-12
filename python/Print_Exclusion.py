import os,ROOT,math,mylib
import CMS_lumi, tdrstyle
import math
from array import array
import argparse

ROOT.gErrorIgnoreLevel = ROOT.kFatal
tdrstyle.setTDRStyle()
ROOT.TH1.AddDirectory(False)

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
ENV_PLOT_PATH = os.environ['PLOT_PATH']

outbaseDir = ENV_PLOT_PATH+"/"+dataset+"/201207_LimitContourReader"

dirName = 'NEW_2021_06_20_060204__YearCombined_YearCombinedShapeForBadNuisancesFixedShape'
#dirName = 'NEW_2021_06_06_021919__YearCombined_BinMaxFlatError'
dirName_words = dirName.split('__')
Year = dirName_words[1].split('_')[0].replace('Year','')
if Year=='Combined':
  Year = 'YearCombined'
f_out = ROOT.TFile(ENV_PLOT_PATH+"/"+dataset+"/Limit/Asymptotic/"+Year+"/"+dirName+"/output.root")

channels = [
'EE',
'MuMu',
]

mNs = [
-500,
200,
]

limitTypes = [
'Exp',
'Obs',
]

header = 'mN\tEE (Exp)\tEE (Obs)\tMM (Exp)\tMM (Obs)'
print header

for mN in mNs:

  values = []

  for channel in channels:

    for limitType in limitTypes:

      gr_Limit = f_out.Get(channel+'_Combined_1D_'+limitType+'_N'+str(mN))
      gr_XSEC = f_out.Get(channel+'_Combined_1D_XSEC_N'+str(mN))

      nx = gr_Limit.GetN()
      this_xs = []
      for ix in range(0,nx):
        this_x = ROOT.Double()
        this_y = ROOT.Double()
        gr_Limit.GetPoint(ix,this_x,this_y)
        this_xs.append(this_x)
      x_min = this_xs[0]
      x_max = this_xs[nx-1]
      dx = 1
      NX = (x_max-x_min)/dx

      previous_ratio = -1
      for i in range(0,int(NX)):
        x = x_min + i*dx

        this_Limit = gr_Limit.Eval(x)
        this_XSEC = gr_XSEC.Eval(x)
        this_ratio = this_XSEC/this_Limit

        if this_ratio==0.0:
          this_ratio = -1
        if previous_ratio>1.0 and this_ratio<1.0:
          values.append(x)
          break
        previous_ratio = this_ratio

  out = str(mN)
  for v in values:
    out += '\t'+'%1.1f'%(v/1000.)

  print out
