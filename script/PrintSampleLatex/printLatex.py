import os
import mylib

lines_xsec = open('result.txt').readlines()
lines_nevent = open('data.txt').readlines()

NMAXLine = 35

table_index = 1
counter = 0
temp_lines = ''

table_prefix = '''\\begin{table}[htp]
\\topcaption{MINIAOD datasets, LO cross-sections, and NLO-to-LO $k$-factor for signal samples.}
\label{tab:officialsignalsamples}
\centering

  \\begin{tabular}{ |c|c|c| }
    \hline
    Sample Name & $\sigma (pb)$, LO & $k^{\mathrm{NLO}/\mathrm{LO}}$ \\\\
    \hline'''

for line in lines_xsec:

  if counter==0:
    print table_prefix.replace('officialsignalsamples','SignalCrossSections_'+str(table_index))

  words = line.split()
  mWR = words[0]
  mN = words[1]
  xsec = words[2]
  nevent = ""
  PD = ""

  for l in lines_nevent:

    # WRtoNLtoLLJJ_WR1000_N100  WRtoNLtoLLJJ_WR1000_N100  1.  77500 77248

    temp_PD = l.split()[0]
    temp_nevent = l.split()[3]

    temp_mWR = temp_PD.split('_')[1].replace('WR','')
    temp_mN = temp_PD.split('_')[2].replace('N','')

    if mWR==temp_mWR and mN==temp_mN:
      PD = temp_PD
      nevent = temp_nevent
      break

  power = xsec[-4:].replace('E','')
  digit = xsec[:-4]
  xsec = digit+' \\times 10^{'+power+'}'

  kfactor = mylib.GetKFactor(int(mWR), int(mN))

  #out = PD.replace('_','\_')+' & $'+xsec+'$ & $'+nevent+'$ \\\\\n'
  out = PD.replace('_','\_')+' & $'+xsec+'$ & $'+'%1.2f'%kfactor+'$ \\\\\n'

  counter += 1
  temp_lines += out

  if counter==NMAXLine:
    print temp_lines
    print '''    \hline
  \end{tabular}

\end{table}'''
    counter = 0
    table_index += 1
    temp_lines = ''

print temp_lines
print '''    \hline
  \end{tabular}

\end{table}'''
