import os

lines = open('output.txt').readlines()
for line in lines:
  # ./log_card_NBkgd1000_ESig1.10_EBkgd1.10.log:Expected 50.0%: r < 185.7500
  words = line.split()
  # ['./log_card_NBkgd100_ESig1.10_EBkgd1.30.log:Expected', '50.0%:', 'r', '<', '47.0000']
  logname = words[0].replace('./log_card_','').replace('.log:Expected','')
  values = logname.split('_')
  # ['NBkgd1000', 'ESig1.10', 'EBkgd1.10']
  NBkgd = values[0].replace('NBkgd','')
  ESig = values[1].replace('ESig','')
  EBkgd = values[2].replace('EBkgd','')
  limit = words[4]
  print '%s\t%s\t%s\t%s'%(NBkgd,ESig,EBkgd,limit)
