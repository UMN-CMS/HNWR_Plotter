import os

lines = open('result.txt').readlines()

print 'double GetKFactor(int mWR, int mN){'

counter = 0
for line in lines:

  if 'WR' in line:
    continue

  words = line.split()
  #print words

  ifline = 'if'
  if counter>0:
    ifline = 'else if'

  print '''  {3}(mWR=={0}){{
    if(mN < mWR/2.) return {1};
    else return {2};
  }}'''.format(words[0],words[1],words[3],ifline)

  counter += 1

print '''  else{
    cout << "[GetKFactor] Wrong mWR and mN : " << mWR << "\\t" << mN << endl;
    return 1.;
  }
}'''
