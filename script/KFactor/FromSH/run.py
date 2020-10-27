import os,ROOT,math
import CMS_lumi, tdrstyle
from array import array

ROOT.TH1.AddDirectory(False)

tdrstyle.setTDRStyle()

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

f_ori = ROOT.TFile(WORKING_DIR+'/data/'+dataset+'/KFactor.root')
h_ori = f_ori.Get('kfactor_all')

f_out = ROOT.TFile('kfactor.root','RECREATE')
f_out.cd()

nRebin = 200
h_ori.Rebin(nRebin)
h_ori.Scale(1./float(nRebin))
h_ori.Write()
f_out.Close()


