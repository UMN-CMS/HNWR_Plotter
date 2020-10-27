#!/bin/bash

root -l -b -q "run.C(1000,600)"
root -l -b -q "run.C(2000,1000)"
root -l -b -q "run.C(3000,1600)"
root -l -b -q "run.C(4000,2000)"
root -l -b -q "run.C(5000,2600)"
root -l -b -q "run.C(6000,3000)"



root -l -b -q "run_SigPDF.C(1000,600)"
root -l -b -q "run_SigPDF.C(2000,1000)"
root -l -b -q "run_SigPDF.C(3000,1600)"
root -l -b -q "run_SigPDF.C(4000,2000)"
root -l -b -q "run_SigPDF.C(5000,2600)"
root -l -b -q "run_SigPDF.C(6000,3000)"

