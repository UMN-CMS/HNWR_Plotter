#!/bin/bash

mWR=5000
mN=100

#for PDF in NNPDF23_lo_as_0130_qed NNPDF31_nnlo_as_0118_mc NNPDF31_nnlo_as_0118_mc_hessian_pdfas NNPDF31_nnlo_hessian_pdfas PDF4LHC15_nnlo_100 PDF4LHC15_nnlo_mc
for PDF in  NNPDF31_nnlo_as_0118_mc PDF4LHC15_nnlo_mc
do
  root -l -b -q "run.C($mWR,$mN,\"$PDF\")" &> log_${mWR}_${mN}_${PDF}.log &
done
