import os

os.system('ls -1 json/*.json &> tmp.txt')
lines = open('tmp.txt').readlines()
os.system('rm tmp.txt')

for line in lines:
  line = line.strip('\n')
  fonly = line.replace('json/','').replace('.json','')
  print 'python run.py -i '+line+' &> log_'+fonly+'.log'
