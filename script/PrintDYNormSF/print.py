import os

lines = open('output.txt').readlines()
arrays = []

for line in lines:
  words = line.split()
  arrays.append( str(words[2]) )
  arrays.append( str(words[3]) )
print len(arrays)

print '''  if(DataYear==2016){{
    if(int_channel==0){{
      if(int_region==0){{
        DYNorm = {0};
        DYNorm_err = {1};
      }}
      else if(int_region==1){{
        DYNorm = {2};
        DYNorm_err = {3};
      }}
      else{{
        cout << "Wrong DY Norm" << endl;
        exit(EXIT_FAILURE);
      }}
    }}
    else if(int_channel==1){{
      if(int_region==0){{
        DYNorm = {4};
        DYNorm_err = {5};
      }}
      else if(int_region==1){{
        DYNorm = {6};
        DYNorm_err = {7};
      }}
      else{{
        cout << "Wrong DY Norm" << endl;
        exit(EXIT_FAILURE);
      }}
    }}
    else{{
      cout << "Wrong DY Norm" << endl;
      exit(EXIT_FAILURE);
    }}
  }}
  else if(DataYear==2017){{
    if(int_channel==0){{
      if(int_region==0){{
        DYNorm = {8};
        DYNorm_err = {9};
      }}
      else if(int_region==1){{
        DYNorm = {10};
        DYNorm_err = {11};
      }}
      else{{
        cout << "Wrong DY Norm" << endl;
        exit(EXIT_FAILURE);
      }}
    }}
    else if(int_channel==1){{
      if(int_region==0){{
        DYNorm = {12};
        DYNorm_err = {13};
      }}
      else if(int_region==1){{
        DYNorm = {14};
        DYNorm_err = {15};
      }}
      else{{
        cout << "Wrong DY Norm" << endl;
        exit(EXIT_FAILURE);
      }}
    }}
    else{{
      cout << "Wrong DY Norm" << endl;
      exit(EXIT_FAILURE);
    }}
  }}
  else if(DataYear==2018){{
    if(int_channel==0){{
      if(int_region==0){{
        DYNorm = {16};
        DYNorm_err = {17};
      }}
      else if(int_region==1){{
        DYNorm = {18};
        DYNorm_err = {19};
      }}
      else{{
        cout << "Wrong DY Norm" << endl;
        exit(EXIT_FAILURE);
      }}
    }}
    else if(int_channel==1){{
      if(int_region==0){{
        DYNorm = {20};
        DYNorm_err = {21};
      }}
      else if(int_region==1){{
        DYNorm = {22};
        DYNorm_err = {23};
      }}
      else{{
        cout << "Wrong DY Norm" << endl;
        exit(EXIT_FAILURE);
      }}
    }}
    else{{
      cout << "Wrong DY Norm" << endl;
      exit(EXIT_FAILURE);
    }}
  }}'''.format(arrays[0], arrays[1], arrays[2], arrays[3], arrays[4], arrays[5], arrays[6], arrays[7], arrays[8], arrays[9], arrays[10], arrays[11], arrays[12], arrays[13], arrays[14], arrays[15], arrays[16], arrays[17], arrays[18], arrays[19], arrays[20], arrays[21], arrays[22], arrays[23])

