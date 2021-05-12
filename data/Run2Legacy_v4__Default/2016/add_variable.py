import os

filenames=['SR_rebins.txt', 'SR_xaxis.txt'] 
#filenames=['CR_rebins.txt', 'CR_xaxis.txt']
#filenames=['CR2_rebins.txt', 'CR2_xaxis.txt']

var_last = "OFLepton_Size"

## name rebin xmin xmax
NewVarSets = [
["ToBeCorrected_Jet_Pt", 20, 0, 2000],
]

for filename in filenames:

  tempfile = open('test_'+filename,'w')

  lines = open(filename).readlines()

  savethem=[]

  for line in lines:

    tempfile.write(line)
    words = line.split()
    if words[1] == var_last:

      for NewVarSet in NewVarSets:

        NewVar = NewVarSet[0]
        Rebin = NewVarSet[1]
        XMin = NewVarSet[2]
        XMax = NewVarSet[3]

        newline = line.replace(var_last, NewVar)

        if "rebins" in filename:
          newline = words[0]+'\t'+NewVar+'\t'+str(Rebin)+'\n'
        if "xaxis" in filename:
          newline = words[0]+'\t'+NewVar+'\t'+str(XMin)+'\t'+str(XMax)+'\n'

        tempfile.write(newline)

  tempfile.close()
  os.system('mv test_'+filename+' '+filename)
