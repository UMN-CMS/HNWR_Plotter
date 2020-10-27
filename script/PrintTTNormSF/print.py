import os

temp_lines = open('output.txt').readlines()
arrays = []

#### sort
lines = []
for i in range(0,len(temp_lines)/2):
  l_Boosted = temp_lines[2*i]
  l_Resolved = temp_lines[2*i+1]
  lines.append(l_Resolved)
  lines.append(l_Boosted)


for line in lines:
  print line.strip('\n')
  words = line.split()
  arrays.append( str(words[1]) )
  arrays.append( str(words[3]) )

print '''  if(DataYear==2016){{
    if(int_channel==0){{
      if(int_region==0){{
        TTNorm = {0};
        TTNorm_err = {1};
      }}
      else if(int_region==1){{
        TTNorm = {2};
        TTNorm_err = {3};
      }}
      else{{
        cout << "Wrong TT Norm" << endl;
        exit(EXIT_FAILURE);
      }}
    }}
    else if(int_channel==1){{
      if(int_region==0){{
        TTNorm = {4};
        TTNorm_err = {5};
      }}
      else if(int_region==1){{
        TTNorm = {6};
        TTNorm_err = {7};
      }}
      else{{
        cout << "Wrong TT Norm" << endl;
        exit(EXIT_FAILURE);
      }}
    }}
    else{{
      cout << "Wrong TT Norm" << endl;
      exit(EXIT_FAILURE);
    }}
  }}
  else if(DataYear==2017){{
    if(int_channel==0){{
      if(int_region==0){{
        TTNorm = {8};
        TTNorm_err = {9};
      }}
      else if(int_region==1){{
        TTNorm = {10};
        TTNorm_err = {11};
      }}
      else{{
        cout << "Wrong TT Norm" << endl;
        exit(EXIT_FAILURE);
      }}
    }}
    else if(int_channel==1){{
      if(int_region==0){{
        TTNorm = {12};
        TTNorm_err = {13};
      }}
      else if(int_region==1){{
        TTNorm = {14};
        TTNorm_err = {15};
      }}
      else{{
        cout << "Wrong TT Norm" << endl;
        exit(EXIT_FAILURE);
      }}
    }}
    else{{
      cout << "Wrong TT Norm" << endl;
      exit(EXIT_FAILURE);
    }}
  }}
  else if(DataYear==2018){{
    if(int_channel==0){{
      if(int_region==0){{
        TTNorm = {16};
        TTNorm_err = {17};
      }}
      else if(int_region==1){{
        TTNorm = {18};
        TTNorm_err = {19};
      }}
      else{{
        cout << "Wrong TT Norm" << endl;
        exit(EXIT_FAILURE);
      }}
    }}
    else if(int_channel==1){{
      if(int_region==0){{
        TTNorm = {20};
        TTNorm_err = {21};
      }}
      else if(int_region==1){{
        TTNorm = {22};
        TTNorm_err = {23};
      }}
      else{{
        cout << "Wrong TT Norm" << endl;
        exit(EXIT_FAILURE);
      }}
    }}
    else{{
      cout << "Wrong TT Norm" << endl;
      exit(EXIT_FAILURE);
    }}
  }}'''.format(arrays[0], arrays[1], arrays[2], arrays[3], arrays[4], arrays[5], arrays[6], arrays[7], arrays[8], arrays[9], arrays[10], arrays[11], arrays[12], arrays[13], arrays[14], arrays[15], arrays[16], arrays[17], arrays[18], arrays[19], arrays[20], arrays[21], arrays[22], arrays[23])

