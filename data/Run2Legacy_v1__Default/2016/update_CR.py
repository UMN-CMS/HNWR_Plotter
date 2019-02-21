import os

which = "CR"
#which = "SR"

files = [
which+'_rebins.txt',
which+'_xaxis.txt',
which+'_yaxis.txt',
]

for f in files:
  lines = open('tmp_'+f).readlines()

  tmpf = open('tmp_tmp_'+f,'w')

  for line in lines:

    words = line.split()
    region = words[0]
    var = words[1]

    if "rebins" in f:

      if "Pt" in var:
        tmpf.write(line.replace('-999','10'))
      elif "Eta" in var:
        tmpf.write(line.replace('-999','5'))
      elif "ZCand_Mass" in var:
        if "mlllt150" in region:
          tmpf.write(line.replace('-999','1'))
        else:
          tmpf.write(line.replace('-999','50'))

      elif "MET" in var or "HT" in var or "MT" in var:
        tmpf.write(line.replace('-999','10'))

      elif "HNFatJet_SDMass" in var:
        tmpf.write(line.replace('-999','10'))

      elif "HNFatJet_Mass" in var:
        tmpf.write(line.replace('-999','10'))

      elif "NCand_Mass" in var:
        tmpf.write(line.replace('-999','5'))

      elif "WRCand_Mass" in var:
        tmpf.write(line.replace('-999','20'))

      elif "NCand_Pt" in var:
        tmpf.write(line.replace('-999','5'))

      elif "WRCand_Pt" in var:
        tmpf.write(line.replace('-999','5'))

      elif "ZCand_Pt" in var:
        tmpf.write(line.replace('-999','20'))

      else:
        tmpf.write(line)

    if "xaxis" in f:

      if "_0_Pt" in var:
        tmpf.write(line.replace('-999\t-999','0\t1000'))
      elif "_1_Pt" in var:
        tmpf.write(line.replace('-999\t-999','0\t1000'))
      elif "ZCand_Mass" in var:
        if "CR1" in region:
           tmpf.write(line.replace('-999\t-999','0\t150'))
        elif "CR2" in region:
          tmpf.write(line.replace('-999\t-999','70\t110'))
        elif "mlllt150" in region:
          tmpf.write(line.replace('-999\t-999','0\t150'))
        else:
          tmpf.write(line.replace('-999\t-999','0\t1000'))

      elif "MET" in var: 
        tmpf.write(line.replace('-999\t-999','0\t300'))
      elif "HNFatJet_SDMass" in var:
        tmpf.write(line.replace('-999\t-999','0\t1000'))
      elif "HNFatJet_Mass" in var:
        tmpf.write(line.replace('-999\t-999','0\t1000'))
      elif "HT" in var:
        tmpf.write(line.replace('-999\t-999','0\t800'))
      elif "MT" in var:
        tmpf.write(line.replace('-999\t-999','0\t500'))

      elif "NCand_Mass" in var:
        tmpf.write(line.replace('-999\t-999','0\t2500'))

      elif "WRCand_Mass" in var:
        tmpf.write(line.replace('-999\t-999','0\t5500'))

      elif ("nPileUp" in var) or ("nPV" in var):
        tmpf.write(line.replace('-999\t-999','0\t100'))

      elif "ZCand_Pt" in var:
        tmpf.write(line.replace('-999\t-999','0\t1000'))

      else:
        tmpf.write(line)

  os.system('mv tmp_tmp_'+f+' '+f)
  os.system('rm tmp_'+f)


