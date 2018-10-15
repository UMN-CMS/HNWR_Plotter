import os

#which = "CR"
which = "SR"

files = [
which+'_rebins.txt',
which+'_xaxis.txt',
which+'_yaxis.txt',
]

for f in files:
  lines = open('tmp_'+f).readlines()

  tmpf = open('tmp_tmp_'+f,'w')

  for line in lines:

    if "rebins" in f:

      if "Pt" in line:
        tmpf.write(line.replace('-999','10'))
      elif "Eta" in line:
        tmpf.write(line.replace('-999','5'))
      elif "ZCand_Mass" in line:
        if "OnZ" in line:
          tmpf.write(line.replace('-999','1'))
        else:
          tmpf.write(line.replace('-999','50'))

      elif "MET" in line or "HT" in line or "MT" in line:
        tmpf.write(line.replace('-999','10'))

      elif "HNFatJet_Mass" in line:
        tmpf.write(line.replace('-999','10'))

      elif "N_Mass" in line:
        tmpf.write(line.replace('-999','5'))

      elif "WRCand_Mass" in line:
        tmpf.write(line.replace('-999','20'))

      else:
        tmpf.write(line)

    if "xaxis" in f:

      if "_0_Pt" in line:
        tmpf.write(line.replace('-999\t-999','0\t1000'))
      elif "_1_Pt" in line:
        tmpf.write(line.replace('-999\t-999','0\t1000'))
      elif "ZCand_Mass" in line:
        if "CR1" in line:
           tmpf.write(line.replace('-999\t-999','0\t150'))
        elif "CR2" in line:
          tmpf.write(line.replace('-999\t-999','70\t110'))
        elif "OnZ" in line:
          tmpf.write(line.replace('-999\t-999','70\t110'))
        else:
          tmpf.write(line.replace('-999\t-999','0\t1000'))

      elif "MET" in line: 
        tmpf.write(line.replace('-999\t-999','0\t300'))
      elif "HNFatJet_Mass" in line:
        tmpf.write(line.replace('-999\t-999','0\t500'))
      elif "HT" in line:
        tmpf.write(line.replace('-999\t-999','0\t800'))
      elif "MT" in line:
        tmpf.write(line.replace('-999\t-999','0\t500'))

      elif "N_Mass" in line:
        tmpf.write(line.replace('-999\t-999','0\t2500'))

      elif "WRCand_Mass" in line:
        tmpf.write(line.replace('-999\t-999','0\t5500'))

      else:
        tmpf.write(line)

  os.system('mv tmp_tmp_'+f+' '+f)
  os.system('rm tmp_'+f)


