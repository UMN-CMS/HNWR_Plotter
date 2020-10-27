import os

lines = open('grep_output.txt').readlines()

for line in lines:
  # 2020_08_02_224646__635094__HNWRAnalyzer__Year2017__SkimTree_LRSMHighPt__RunSyst__TAMSA1/SingleElectron_periodB/job_7.log:[HighMassDataEvent][IsResolved_SR_EE] 297485:217:372476529	4204.11
  words = line.split()
  Year = words[0].split(':')[0].split('/')[0].split('__')[3].replace('Year','')
  sample = words[0].split(':')[0].split('/')[1]

  PD = sample.split('_')[0]
  period = sample.split('_')[1]

  EventType = words[0].split(':')[1].split('][')[1].replace(']','')
  eventinfo = words[1]

  eventinfo_filename = 'EventList_'+Year+'_'+PD+'_'+period+'_'+EventType+'.txt'
  out = open('output/'+eventinfo_filename,'a+')
  out.write(eventinfo+'\n')

  #print Year+'\t'+PD+'\t'+period+'\t'+EventType+'\t'+eventinfo

  out.close()


os.system('ls -1 output/*.txt &> tmp.txt')
lines = open('tmp.txt').readlines()
os.system('rm tmp.txt')
for line in lines:
  # EventList_2016_SingleElectron_periodE_IsBoosted_SR_EE.txt

  # /SingleElectron/Run2016C-17Jul2018-v1/MINIAOD

  words = line.replace('\n','').replace('.txt','').split('_')
  Year = words[1]
  PD = words[2]
  period = words[3].replace('period','')
  DASNameLines = open('samples/'+Year+'_DATA.txt').readlines()
  DASName = ''
  for DASNameLine in DASNameLines:
    if (PD in DASNameLine) and ('Run'+Year+period in DASNameLine):
      DASName = DASNameLine.strip('\n')
      break

  cmd = 'edmPickEvents.py "%s"'%(DASName)+' --output %s '%(line.replace('\n','').replace('.txt','').replace('output/',''))+line.strip('\n')
  print cmd
