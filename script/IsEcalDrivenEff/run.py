import os,ROOT

basedir = '/data4/Users/jskim/SKFlatOutput/Run2Legacy_v4/ExampleRun/2017/IsEcalDrivenTest__/'

masses = open('mass.txt').readlines()

for mass in masses:

  mass = mass.strip('\n')

  # WRtoNLtoLLJJ_WR7000_N4600

  words = mass.split('_')
  mWR = words[1].replace('WR','')
  mN = words[2].replace('N','')

  f = ROOT.TFile(basedir+'/ExampleRun_'+mass+'.root')
  h = f.Get('PassFullHEEP')
  print '%s\t%s\t%f' % (mWR,mN,h.GetMean())


