#!/bin/bash

for mWR in 2000 3000 4000 5000 6000
do

  root -l -b -q "run.C(${mWR},100)"
  root -l -b -q "run_SigPDF.C(${mWR},100)"

  root -l -b -q "run.C(${mWR},400)"
  root -l -b -q "run_SigPDF.C(${mWR},400)"

done

