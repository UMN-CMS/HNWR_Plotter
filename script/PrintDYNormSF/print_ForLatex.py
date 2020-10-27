import os

lines = open('output.txt').readlines()
arrays = []

for line in lines:
  words = line.split()

  v = '%1.3f'%(float(words[2]))
  e = '%1.3f'%(float(words[3]))

  arrays.append( v )
  arrays.append( e )
print len(arrays)

print '''\\multirow{{2}}{{*}}{{2016}} & Resolved & ${0} \\pm {1}$ & ${4} \\pm {5}$ \\\\
                      & Boosted  & ${2} \\pm {3}$ & ${6} \\pm {7}$ \\\\
\\multirow{{2}}{{*}}{{2017}} & Resolved & ${8} \\pm {9}$ & ${12} \\pm {13}$ \\\\
                      & Boosted  & ${10} \\pm {11}$ & ${14} \\pm {15}$ \\\\
\\multirow{{2}}{{*}}{{2018}} & Resolved & ${16} \\pm {17}$ & ${20} \\pm {21}$ \\\\
                      & Boosted  & ${18} \\pm {19}$ & ${22} \\pm {23}$ \\\\
'''.format(arrays[0], arrays[1], arrays[2], arrays[3], arrays[4], arrays[5], arrays[6], arrays[7], arrays[8], arrays[9], arrays[10], arrays[11], arrays[12], arrays[13], arrays[14], arrays[15], arrays[16], arrays[17], arrays[18], arrays[19], arrays[20], arrays[21], arrays[22], arrays[23])
