import os

Which = 'CR'
Which = 'NoLSFCut'

txtfiles = [
'rebins',
'xaxis',
'yaxis',
]

regionFrom = 'HNWR_SingleElectron_Boosted_SR'
regionTo   = 'HNWR_SingleMuon_EMu_Boosted_CR'

for txtfile in txtfiles:

  lines = open(Which+'_'+txtfile+'.txt').readlines()
  print 'tmp_'+Which+'_'+txtfile+'.txt'
  newout = open('tmp_'+Which+'_'+txtfile+'.txt','w')

  toReplace = ''
  for i_line in range(0,len(lines)):

    line = lines[i_line]
    words = line.split()

    region = words[0]

    if regionFrom in words[0]:
      toReplace += lines[i_line]
      newout.write(lines[i_line])
    else:
      if toReplace == '':
        newout.write(lines[i_line])
      else:
        newout.write(toReplace.replace(regionFrom,regionTo))
        newout.write(lines[i_line])
        toReplace = ''
  newout.close()
  os.system('mv tmp_'+Which+'_'+txtfile+'.txt '+Which+'_'+txtfile+'.txt')


