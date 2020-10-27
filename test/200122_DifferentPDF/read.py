import os

os.system('ls -1 *.log &> tmp.txt')
logfiles = open('tmp.txt').readlines()
os.system('rm tmp.txt')
for logfile in logfiles:
  logfile = logfile.strip('\n')
  lines = open(logfile).readlines()

  WRMass = logfile.split('_')[1]
  NMass = logfile.split('_')[2]
  PDF = logfile.replace('.log','').replace('log_'+WRMass+'_'+NMass+'_','')

  #print 'WR = %s, N = %s' % (WRMass,NMass)

#[SignalSystematics] bin 6 : 0.327407, error = 0.971675

  LastBinPDFErrorLine = ""
  for i_l in range(0,len(lines)):
    j_l = len(lines)-i_l-1
    line = lines[j_l]

    if "[SignalSystematics] bin 6" in line and not ( ("-->") in line ):
      LastBinPDFErrorLine = line
      break

  words = LastBinPDFErrorLine.split()
  # ['[SignalSystematics]', 'bin', '6', ':', '0.327407,', 'error', '=', '0.971675']
  binValue = float(words[4].replace(',',''))
  binError = float(words[7])
  print '%s\t%f\t%f\t%f' % (PDF,binValue, binError, 100.*binError/binValue)


  #log_5000_100_NNPDF23_lo_as_0130_qed.log
