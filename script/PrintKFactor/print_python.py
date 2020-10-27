import os

lines = open('result.txt').readlines()

print 'def GetKFactor(mWR, mN):'

counter = 0
for line in lines:

  if 'WR' in line:
    continue

  words = line.split()
  #print words

  ifline = 'if'
  if counter>0:
    ifline = 'elif'

  print '''  {3} mWR=={0}:
    if mN < mWR/2.:
      return {1}
    else:
      return {2}
  '''.format(words[0],words[1],words[3],ifline)

  counter += 1
