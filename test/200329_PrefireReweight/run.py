import os,ROOT

masses = [
"5000,100",
"5000,200",
"5000,400",
"5000,600",
"5000,800",
"5000,1000",
"5000,1200",
"5000,1400",
"5000,1600",
"5000,1800",
"5000,2000",
"5000,2200",
"5000,2400",
"5000,2600",
"5000,2800",
"5000,3000",
"5000,3200",
"5000,3400",
"5000,3600",
"5000,3800",
"5000,4000",
"5000,4200",
"5000,4400",
"5000,4600",
"5000,4800",
"5000,4900",
]

regions = [
"Resolved",
"Boosted",
]

channels = [
"Electron",
"Muon",
]

Years = [
"2016",
"2017",
]

for mass in masses:
  words = mass.split(',')
  mWR = words[0]
  mN = words[1]

  out = '(%s,%s)'%(mWR,mN)

  for Year in Years:
    f = ROOT.TFile(Year+"/HNWRAnalyzer_WRtoNLtoLLJJ_WR"+mWR+"_N"+mN+".root")

    for region in regions:
      frac_ee = 0
      frac_mm = 0

      for channel in channels:
        dirName = "HNWR_Single"+channel+"_"+region+"_SR"
        h = f.Get(dirName+"/PrefireRwg_"+dirName)
        this_avg = h.GetBinContent(1)/h.GetEntries()
        if channel=="Electron":
          frac_ee = (1-this_avg)*100
        else:
          frac_mm = (1-this_avg)*100
      out += ' & %1.2f (%1.2f)'%(frac_ee,frac_mm)

  print out+' \\\\'
