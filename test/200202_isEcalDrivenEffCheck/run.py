import os,ROOT,tdrstyle
from array import array

tdrstyle.setTDRStyle()

PLOTTER_WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']

fileDir = '/data6/Users/jskim/SKFlatOutput/Run2Legacy_v4/ExampleRun/2016/IsEcalDrivenTest__/'

masses = open('mass.txt').readlines()
NMass = len(masses)
outDir = './output/'
os.system('mkdir -p '+outDir)

x = []
x_label = []
y = []
y2 = []

for it_mass in range(0,len(masses)):

  mass = masses[it_mass].strip('\n')
  fname = 'ExampleRun_WRtoNLtoLLJJ_'+mass+'.root'
  f = ROOT.TFile(fileDir+fname)
  h = f.Get('PassFullHEEP')
  this_eff = h.GetMean()

  x.append( it_mass )
  x_label.append( mass )
  y.append( this_eff )
  y2.append( this_eff*this_eff )

c1 = ROOT.TCanvas('c1','',1100,800)
c1.cd()
c1.SetLeftMargin( 0.16 );

gr_eff = ROOT.TGraph(NMass, array("d", x), array("d", y))
gr_eff.Draw("alp")
gr_eff.GetXaxis().SetRangeUser(0,NMass+1)
gr_eff.GetXaxis().SetTitle("Mass bin")
gr_eff.GetYaxis().SetRangeUser(0.98, 1.0)
gr_eff.GetYaxis().SetTitle("Eff.(isEcalDriven==true)")

c1.SaveAs(outDir+"/eff.pdf")
c1.Close()

c2 = ROOT.TCanvas('c2','',1100,800)
c2.cd()
c2.SetLeftMargin( 0.16 );

gr_eff2 = ROOT.TGraph(NMass, array("d", x), array("d", y2))
gr_eff2.Draw("alp")
gr_eff2.GetXaxis().SetRangeUser(0,NMass+1)
gr_eff2.GetXaxis().SetTitle("Mass bin")
gr_eff2.GetYaxis().SetRangeUser(0.98, 1.0)
gr_eff2.GetYaxis().SetTitle("Eff.(isEcalDriven==true)^{2}")

c2.SaveAs(outDir+"/eff2.pdf")
c2.Close()
