import os,ROOT

massWR = 3000
masses = []
masses.append( '%s,%s'%(massWR,100) )
for i in range(0, massWR/200):
  this_mN = 200*(i+1)
  if this_mN>=massWR:
    continue
  masses.append( '%s,%s'%(massWR,this_mN) )
masses.append('%s,%s'%(massWR,massWR-100) )

print masses

regions = [
["HNWR_EMu_Resolved_SR", 911.5],
["HNWR_SingleElectron_EMu_Boosted_CR", 161.346],
["HNWR_SingleMuon_EMu_Boosted_CR", 210.785],
]

#region = "HNWR_EMu_Resolved_SR"
#CRYield = 911.5

#region = "HNWR_SingleElectron_EMu_Boosted_CR"
#CRYield = 161.346

#region = "HNWR_SingleMuon_EMu_Boosted_CR"
#CRYield = 210.785

Year = "2016"

xsec_lines = open('../../data/Run2Legacy_v4__Default/xsec_190705_GenXsecAN_eeANDmm.txt').readlines()

FILE_PATH = os.environ['FILE_PATH']
CATANVERSION = os.environ['CATANVERSION']

baseDir = FILE_PATH+'/'+CATANVERSION+'/Regions/'+Year+'/Signal/'

for mass in masses:
  words = mass.split(',')
  mWR = words[0]
  mN = words[1]

  ## Get xsec
  xsec = -1
  for line in xsec_lines:
    tmp_words = line.split()
    tmp_WR = tmp_words[0]
    tmp_N = tmp_words[1]
    if mWR==tmp_WR and mN==tmp_N:
      xsec = float(tmp_words[2])
      break

  if xsec<0:
    print mWR+'\t'+mN+' : no xsec'

  out = '(%s,%s)' % (mWR,mN)

  f = ROOT.TFile(baseDir+"HNWRAnalyzer_WRtoNLtoLLJJ_WR"+mWR+"_N"+mN+".root")

  for region in regions:
    dirName = region[0]
    h = f.Get(dirName+"/NEvent_"+dirName)
    this_frac = 0
    if h:
      this_frac = 100. * h.GetBinContent(1) * xsec / region[1]
    out += ' & %1.3f'%( abs(this_frac) ) 

  print out+' \\\\'
