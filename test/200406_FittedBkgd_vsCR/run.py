import os,ROOT

Region = "Boosted"

MassBins = [0, 800, 1000, 1200, 1400, 1600, 2000, 2400, 2800, 3200, 8000]
if Region=="Boosted":
  MassBins = [0, 800, 1000, 1200, 1500, 1700, 8000]

#shapeDir = "shapes_prefit"
shapeDir = "shapes_fit_b"

f_SROnly = ROOT.TFile('MuMu_'+Region+'_SROnly.root')
f_CRandSR = ROOT.TFile('MuMu_'+Region+'_CRandSR.root')

h_SROnly = f_SROnly.Get(shapeDir+'/'+Region+'_MuMu/total_background')
h_CRandSR = f_CRandSR.Get(shapeDir+'/SR/total_background')

for i in range(0,h_SROnly.GetXaxis().GetNbins()):
  if i==0:
    continue

  ix = i+1
  x_l = MassBins[i]
  x_r = MassBins[i+1]

  Err_SROnly = h_SROnly.GetBinError(ix)/h_SROnly.GetBinContent(ix)
  Err_CRandSR = h_CRandSR.GetBinError(ix)/h_CRandSR.GetBinContent(ix)
  print '%d\t%d\t%1.2f\t%1.2f\t%1.2f'%(x_l,x_r,100.*Err_SROnly,100.*Err_CRandSR,100.*Err_CRandSR/Err_SROnly)
