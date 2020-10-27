#!/bin/bash
#for PDF in NNPDF23_lo_as_0130_qed NNPDF31_nnlo_as_0118_mc NNPDF31_nnlo_as_0118_mc_hessian_pdfas NNPDF31_nnlo_hessian_pdfas PDF4LHC15_nnlo_100 PDF4LHC15_nnlo_mc
for PDF in NNPDF23_lo_as_0130_qed
do
  root -l -b -q "Draw_PDFError.C(2016,\"${PDF}\") "
done
