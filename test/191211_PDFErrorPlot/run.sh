#!/bin/bash

cd /data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v3__Default/Regions/2016/Signal/
rm *.root
rsync -auv -e 'ssh -p 1240' jskim@147.47.242.42:/data4/Users/jskim/SKFlatOutput/Run2Legacy_v3/HNWRAnalyzer/2016/RunSyst__Signal__RunXsecSyst__/*.root ./
cd /data6/Users/jskim/HNWR_Plotter/test/191211_PDFErrorPlot/
root -l -b -q "Draw_PDFError.C(2016,\"NNPDF31_nnlo_hessian_pdfas\",false)"


for PDF in NNPDF31_nnlo_as_0118_mc PDF4LHC15_nnlo_mc NNPDF30_nnlo_as_0118_mc NNPDF23_lo_as_0130_qed
do
  cd /data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v3__Default/Regions/2016/Signal/
  rm *.root
  rsync -auv -e 'ssh -p 1240' jskim@147.47.242.42:/data4/Users/jskim/SKFlatOutput/Run2Legacy_v3/HNWRAnalyzer/2016/RunSyst__Signal__RunXsecSyst__RunNewPDF__${PDF}__/*.root ./
  cd /data6/Users/jskim/HNWR_Plotter/test/191211_PDFErrorPlot/
  root -l -b -q "Draw_PDFError.C(2016,\"${PDF}\",true)"
done

for PDF in NNPDF30_nnlo_as_0118_hessian NNPDF31_nnlo_as_0118_mc_hessian_pdfas PDF4LHC15_nnlo_100
do
  cd /data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v3__Default/Regions/2016/Signal/
  rm *.root
  rsync -auv -e 'ssh -p 1240' jskim@147.47.242.42:/data4/Users/jskim/SKFlatOutput/Run2Legacy_v3/HNWRAnalyzer/2016/RunSyst__Signal__RunXsecSyst__RunNewPDF__${PDF}__/*.root ./
  cd /data6/Users/jskim/HNWR_Plotter/test/191211_PDFErrorPlot/
  root -l -b -q "Draw_PDFError.C(2016,\"${PDF}\",false)"
done
