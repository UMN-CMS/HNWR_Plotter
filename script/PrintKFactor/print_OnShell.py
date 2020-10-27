import os

lines = open('result.txt').readlines()

print 'double GetKFactor(int mWR, int mN){'

counter = 0
for line in lines:

  if 'WR' in line:
    continue

  words = line.split()
  # m(WR) 100 m(WR)/2 m(WR)-100
  # 200 1.13 1.13 1.13

  ifline = 'if'
  if counter>0:
    ifline = 'else if'

  print '''  {0}(mWR=={1}) return {2};'''.format(ifline, words[0], words[3])

  counter += 1

print '''  else{
    cout << "[GetKFactor] Wrong mWR and mN : " << mWR << "\\t" << mN << endl;
    return 1.;
  }
}'''
