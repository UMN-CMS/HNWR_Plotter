import os
import json
import math

import argparse
parser = argparse.ArgumentParser(description='option')
parser.add_argument('-i', dest='input')
args = parser.parse_args()

fileName = args.input
#'impacts_2016_card_CRAdded_EE_Boosted_WR3000_N1600.json'

lines = open(fileName,'r')
output = (json.load(lines))['params']

Unsorted = []
for k in output:
  name = k['name']
  fittedValues = k['fit']
  left = fittedValues[0]
  central = fittedValues[1]
  right = fittedValues[2]
  err_left = central-left
  err_right = right-central
  err_avg = math.sqrt( (err_left*err_left+err_right*err_right)/2. )
  #print '%s\t%1.3f\t%1.3f\t%1.3f\t%1.3f'%(name,central,(central-left),(right-central),err_avg)
  Unsorted.append( [name, central, err_left, err_right, err_avg] )

Sorted = []

for k in Unsorted:
  name = k[0]
  central = k[1]
  err_left = k[2]
  err_right = k[3]
  err_avg = k[4]

  if name.startswith('R_'):
    continue

  this_index = 0
  for j in Sorted:
    j_err_avg = j[4]
    if j_err_avg <= err_avg:
      this_index += 1
  Sorted.insert(this_index, k)

for k in Sorted:
  name = k[0]
  central = k[1]
  err_left = k[2]
  err_right = k[3]
  err_avg = k[4]
  print '%s\t%1.3f\t%1.3f\t%1.3f\t%1.3f'%(name,central,err_left,err_right,err_avg)
