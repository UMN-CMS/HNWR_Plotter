import os

Years = [
"2016",
"2017",
"2018",
]

fnames = [
"0_0",
"0_1",
"1_0",
"1_1",
]

for Year in Years:

  liness = []
  for fname in fnames:
    lines = open('log_'+Year+'_'+fname+'.log').readlines()
    liness.append(lines)

  outfile = open('Summary_'+Year+'.txt','w')

  for i in range(2,len(liness[0])):
    tmp_words = liness[0][i].split()
    out = tmp_words[0]+'\t'+tmp_words[1]
    for i_f in range(0,len(liness)):
      out += '\t'+liness[i_f][i].split()[2]
    outfile.write(out+'\n')
  outfile.close()
