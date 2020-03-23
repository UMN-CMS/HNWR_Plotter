import os

filenames=['SR_rebins.txt', 'SR_xaxis.txt'] 
#filenames=['CR_rebins.txt', 'CR_xaxis.txt']
#filenames=['CR2_rebins.txt', 'CR2_xaxis.txt']

var_last = "LSFFatJet_Size"
NewVars = ["Lepton_Size", "OFLepton_Size"]

for filename in filenames:

  tempfile = open('test_'+filename,'w')

  lines = open(filename).readlines()

  savethem=[]

  for line in lines:

    tempfile.write(line)
    words = line.split()
    if words[1] == var_last:

      for NewVar in NewVars:
        newline = line.replace(var_last, NewVar)

        if "rebins" in filename:
          newline = words[0]+'\t'+NewVar+'\t1\n'
        if "xaxis" in filename:
          newline = words[0]+'\t'+NewVar+'\t0\t5\n'

        tempfile.write(newline)

  tempfile.close()
  os.system('mv test_'+filename+' '+filename)
