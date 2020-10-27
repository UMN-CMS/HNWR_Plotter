#!/bin/bash
for PDF in NNPDF23_lo_as_0130_qed NNPDF31_nnlo_as_0118_mc NNPDF31_nnlo_as_0118_mc_hessian_pdfas NNPDF31_nnlo_hessian_pdfas PDF4LHC15_nnlo_100 PDF4LHC15_nnlo_mc
do
  root -l -b -q "run_SigPDF.C(5000, 100, \"${PDF}\")"
  root -l -b -q "run_SigPDF.C(5000, 200, \"${PDF}\")"
  root -l -b -q "run_SigPDF.C(5000, 400, \"${PDF}\")"

  root -l -b -q "run_SigPDF.C(5000, 3000, \"${PDF}\")"
  root -l -b -q "run_SigPDF.C(5000, 4000, \"${PDF}\")"
done
