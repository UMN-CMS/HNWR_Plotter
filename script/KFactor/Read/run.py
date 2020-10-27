import os,ROOT

PLOTTER_WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
CATANVERSION = os.environ['CATANVERSION']

basedir = '/data6/Users/jskim/SKFlatOutput/Run2Legacy_v4/HNWRAnalyzer/'

Years = [
"2016",
"2017",
"2018",
]

for Year in Years:

  os.system('mkdir -p /data6/Users/jskim/SKFlatAnalyzer/data/Run2Legacy_v4/'+Year+'/HNWRKFactor/')
  out = open('/data6/Users/jskim/SKFlatAnalyzer/data/Run2Legacy_v4/'+Year+'/HNWRKFactor/AveragedKFactor.txt','w')

  #Check mass first
  os.system('ls -1 '+basedir+'/'+Year+'/Signal__CalculateAverageKFactor__/*.root &> tmp.txt')
  lines = open('tmp.txt').readlines()
  os.system('rm tmp.txt')
  for line in lines:
    words = line.split('/')
    short_filename = words[-1].strip('\n')
    mass = short_filename.replace('HNWRAnalyzer_WRtoNLtoLLJJ_','').replace('.root','')

    f = ROOT.TFile(line.strip('\n'))

    SignalFlavour = f.Get('SignalFlavour')
    ElectronChannel_KFactor = f.Get('ElectronChannel_KFactor')
    MuonChannel_KFactor = f.Get('MuonChannel_KFactor')

    frac_ee = SignalFlavour.GetBinContent(2) / SignalFlavour.Integral();
    frac_mm = SignalFlavour.GetBinContent(3) / SignalFlavour.Integral();

    avg_kfactor_ee = ElectronChannel_KFactor.GetBinContent(1) / frac_ee
    avs_kfactor_mm = MuonChannel_KFactor.GetBinContent(1) / frac_mm

    f.Close()

    out.write(mass+'\t'+str(avg_kfactor_ee)+'\t'+str(avs_kfactor_mm)+'\n')

  out.Close()

  os.system('cp /data6/Users/jskim/SKFlatAnalyzer/data/Run2Legacy_v4/'+Year+'/HNWRKFactor/AveragedKFactor.txt '+PLOTTER_WORKING_DIR+'/data/'+CATANVERSION+'/'+Year+'/')
