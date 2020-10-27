using namespace RooFit;
#include "mylib.h"
#include "canvas_margin.h"

void test(int i_region=0, int i_channel=0){

  setTDRStyle();
  SumW2Error(kTRUE);
  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString region = "Resolved_SR";
  TString channel = "Electron";
  if(i_region==0) region = "Resolved_SR";
  else if(i_region==1) region = "Boosted_SR";

  if(i_channel==0) channel = "Electron";
  else if(i_channel==1) channel = "Muon";

  int NRebin = 20;
  TString Year = "2016";

  TString dirname = "HNWR_Single"+channel+"_"+region;

  //=== If not, use geenral
  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";

  //=============
  //==== DY
  //=============

  double FitRange_l_DY = 800;
  double FitRange_r_DY = 8000;

  TFile *file_DY = new TFile(base_filepath+"HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT_Reweighted.root");
  TH1D *hist_DY = (TH1D *)file_DY->Get(dirname+"/WRCand_Mass_"+dirname);
  double integral_DY = hist_DY->Integral();
  hist_DY->Rebin(NRebin);

  //==== data from histogram
  RooRealVar mwr_DY("mwr_DY", "mwr_DY", 800., 8000.);
  RooDataHist rooDataHist_DY("rooDataHist_DY", "rooDataHist_DY", mwr_DY, hist_DY);

  //==== TH1:Fit test
  hist_DY->Fit("expo","P","",1200,8000);
  cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
  cout << "A = " << hist_DY->GetFunction("expo")->GetParameter(0) << endl;
  cout << "B = " << hist_DY->GetFunction("expo")->GetParameter(1) << endl;
  cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;

  //==== exp(AX)
  RooRealVar expo_slope_DY("expo_slope_DY", "expo_slope_DY", -1e-02, -1e-04);
  RooExponential expo_DY("expo_DY", "expo_DY", mwr_DY, expo_slope_DY);

  //==== (1-x)^(A) * (1+x)^(Bx)
  RooRealVar func_1_A_DY("func_1_A_DY", "func_1_A_DY", -1e-2, 1e-2);
  RooRealVar func_1_B_DY("func_1_B_DY", "func_1_B_DY", -1e-2, 1e-2);
  RooGenericPdf func_1_DY("func_1_DY","pow(1-mwr_DY,func_1_A_DY)*pow(1+mwr_DY,func_1_B_DY*mwr_DY)",RooArgSet(mwr_DY,func_1_A_DY,func_1_B_DY));

  //==== dijet (AN19_073)
  RooRealVar func_dijet_p0_DY("func_dijet_p0_DY", "func_dijet_p0_DY", 1E-5, 20.);
  RooRealVar func_dijet_p1_DY("func_dijet_p1_DY", "func_dijet_p1_DY", 0, 20.);
  RooRealVar func_dijet_p2_DY("func_dijet_p2_DY", "func_dijet_p2_DY", 0, 20.);
  RooRealVar func_dijet_p3_DY("func_dijet_p3_DY", "func_dijet_p3_DY", 0, 20.);
  RooGenericPdf func_dijet_DY("func_dijet_DY", "func_dijet_p0_DY * pow(1-mwr_DY/13000.,func_dijet_p1_DY)/ pow(mwr_DY/13000., func_dijet_p2_DY+func_dijet_p3_DY*TMath::Log10(mwr_DY/13000.))", RooArgSet(mwr_DY,func_dijet_p0_DY,func_dijet_p1_DY,func_dijet_p2_DY,func_dijet_p3_DY) );

  //==== trijet analysis (AN18_269)
  RooRealVar func_trijet_p0_DY("func_trijet_p0_DY", "func_trijet_p0_DY", 1E-5, 100.);
  RooRealVar func_trijet_p1_DY("func_trijet_p1_DY", "func_trijet_p1_DY", 0. , 50.);
  RooRealVar func_trijet_p2_DY("func_trijet_p2_DY", "func_trijet_p2_DY", 13000./800., 100.);
  RooRealVar func_trijet_p3_DY("func_trijet_p3_DY", "func_trijet_p3_DY", 0, 50.);
  RooRealVar func_trijet_p4_DY("func_trijet_p4_DY", "func_trijet_p4_DY", 0, 50.);
  RooGenericPdf func_trijet_DY("func_trijet_DY", "func_trijet_p0_DY * (func_trijet_p2_DY * mwr_DY/13000. - 1) / pow(mwr_DY/13000., func_trijet_p1_DY + func_trijet_p3_DY * TMath::Log10(mwr_DY/13000.) + func_trijet_p4_DY * TMath::Log10(mwr_DY/13000.) * TMath::Log10(mwr_DY/13000.))", RooArgSet(mwr_DY,func_trijet_p0_DY,func_trijet_p1_DY,func_trijet_p2_DY,func_trijet_p3_DY,func_trijet_p4_DY) );

  //==== Draw
  RooPlot* xframe_DY = mwr_DY.frame();

  TCanvas *c_DY = new TCanvas("c_DY","",600,600);
  canvas_margin(c_DY);
  c_DY->cd();
  rooDataHist_DY.plotOn(xframe_DY);

  expo_DY.fitTo(rooDataHist_DY, Range(FitRange_l_DY,FitRange_r_DY));
  expo_DY.plotOn(xframe_DY, Range(800,8000));

  //func_1_DY.fitTo(rooDataHist_DY, Range(FitRange_l_DY,FitRange_r_DY));
  //func_1_DY.plotOn(xframe_DY,LineColor(kRed), Range(800,8000));

  func_dijet_DY.fitTo(rooDataHist_DY, Range(FitRange_l_DY,FitRange_r_DY));
  func_dijet_DY.plotOn(xframe_DY,LineColor(kGreen), Range(800,8000));

  //==== create histogram with this
  TH1D *hist_DY_fitted = new TH1D("hist_DY_fitted", "", 8000/200, 0, 8000);
  for(int i=1; i<=hist_DY_fitted->GetXaxis()->GetNbins(); i++){
    double x_l = hist_DY_fitted->GetXaxis()->GetBinLowEdge(i);
    double x_r = hist_DY_fitted->GetXaxis()->GetBinUpEdge(i);

    if(x_l<800) continue;

    cout << "Calculating integral of ("<<x_l<<", "<<x_r<<") .." << endl;
    mwr_DY.setRange("thisrange", x_l, x_r);
    RooAbsReal *temp_integral_func_dijet_DY = func_dijet_DY.createIntegral(mwr_DY, NormSet(mwr_DY), Range("thisrange"));

    double this_bincontent = integral_DY * temp_integral_func_dijet_DY->getVal();
    cout << "--> " << this_bincontent << endl;
    hist_DY_fitted->SetBinContent(i,this_bincontent);
    hist_DY_fitted->SetBinError(i,sqrt(this_bincontent));
  }
  hist_DY_fitted->SetLineColor(kGreen);
  hist_DY_fitted->SetLineStyle(3);
  hist_DY_fitted->SetLineWidth(3);
  hist_DY_fitted->SetMarkerColor(kGreen);
  hist_DY_fitted->Draw("histe1same");

  func_trijet_DY.fitTo(rooDataHist_DY, Range(FitRange_l_DY,FitRange_r_DY));
  func_trijet_DY.plotOn(xframe_DY,LineColor(kViolet), Range(800,8000));

  xframe_DY->Draw();
  xframe_DY->SetMinimum(1E-6);
  xframe_DY->SetMaximum(500);

  hist_DY_fitted->Draw("histsame");

  c_DY->SetLogy();
  c_DY->SaveAs(region+"_"+channel+"_DY.pdf");
  c_DY->SaveAs(region+"_"+channel+"_DY.png");
  c_DY->Close();

  //=========
  //==== tt
  //=========

  double FitRange_l_tt = 800;
  double FitRange_r_tt = 8000;

  TFile *file_tt = new TFile(base_filepath+"HNWRAnalyzer_SkimTree_LRSMHighPt_EMuMethod_TTLX_powheg.root");
  TH1D *hist_tt = (TH1D *)file_tt->Get(dirname+"/WRCand_Mass_"+dirname);
  double integral_tt = hist_tt->Integral();
  hist_tt->Rebin(NRebin);

  //==== data from histogram
  RooRealVar mwr_tt("mwr_tt", "mwr_tt", 800., 8000.);
  RooDataHist rooDataHist_tt("rooDataHist_tt", "rooDataHist_tt", mwr_tt, hist_tt);

  //==== exp(A+BX)
  RooRealVar expo_slope_tt("expo_slope_tt", "expo_slope_tt", -1e-02, -1e-04);
  RooExponential expo_tt("expo_tt", "expo_tt", mwr_tt, expo_slope_tt);

  //==== (1-x)^(A) * (1+x)^(Bx)
  RooRealVar func_1_A_tt("func_1_A_tt", "func_1_A_tt", -1e-2, 1e-2);
  RooRealVar func_1_B_tt("func_1_B_tt", "func_1_B_tt", -1e-2, 1e-2);
  RooGenericPdf func_1_tt("func_1_tt","pow(1-mwr_tt,func_1_A_tt)*pow(1+mwr_tt,func_1_B_tt*mwr_tt)",RooArgSet(mwr_tt,func_1_A_tt,func_1_B_tt));

  //==== dijet (AN19_073)
  RooRealVar func_dijet_p0_tt("func_dijet_p0_tt", "func_dijet_p0_tt", 1E-5, 20.);
  RooRealVar func_dijet_p1_tt("func_dijet_p1_tt", "func_dijet_p1_tt", 0, 20.);
  RooRealVar func_dijet_p2_tt("func_dijet_p2_tt", "func_dijet_p2_tt", 0, 20.);
  RooRealVar func_dijet_p3_tt("func_dijet_p3_tt", "func_dijet_p3_tt", 0, 20.);
  RooGenericPdf func_dijet_tt("func_dijet_tt", "func_dijet_p0_tt * pow(1-mwr_tt/13000.,func_dijet_p1_tt)/ pow(mwr_tt/13000., func_dijet_p2_tt+func_dijet_p3_tt*TMath::Log10(mwr_tt/13000.))", RooArgSet(mwr_tt,func_dijet_p0_tt,func_dijet_p1_tt,func_dijet_p2_tt,func_dijet_p3_tt) );

  //==== trijet analysis (AN18_269)
  RooRealVar func_trijet_p0_tt("func_trijet_p0_tt", "func_trijet_p0_tt", 1E-5, 100.);
  RooRealVar func_trijet_p1_tt("func_trijet_p1_tt", "func_trijet_p1_tt", 0. , 50.);
  RooRealVar func_trijet_p2_tt("func_trijet_p2_tt", "func_trijet_p2_tt", 13000./800., 100.);
  RooRealVar func_trijet_p3_tt("func_trijet_p3_tt", "func_trijet_p3_tt", 0, 50.);
  RooRealVar func_trijet_p4_tt("func_trijet_p4_tt", "func_trijet_p4_tt", 0, 50.);
  RooGenericPdf func_trijet_tt("func_trijet_tt", "func_trijet_p0_tt * (func_trijet_p2_tt * mwr_tt/13000. - 1) / pow(mwr_tt/13000., func_trijet_p1_tt + func_trijet_p3_tt * TMath::Log10(mwr_tt/13000.) + func_trijet_p4_tt * TMath::Log10(mwr_tt/13000.) * TMath::Log10(mwr_tt/13000.))", RooArgSet(mwr_tt,func_trijet_p0_tt,func_trijet_p1_tt,func_trijet_p2_tt,func_trijet_p3_tt,func_trijet_p4_tt) );

  //==== Draw
  RooPlot* xframe_tt = mwr_tt.frame();

  TCanvas *c_tt = new TCanvas("c_tt","",600,600);
  canvas_margin(c_tt);
  c_tt->cd();
  rooDataHist_tt.plotOn(xframe_tt);

  expo_tt.fitTo(rooDataHist_tt, Range(FitRange_l_tt,FitRange_r_tt));
  expo_tt.plotOn(xframe_tt, Range(800,8000));

  //func_1_tt.fitTo(rooDataHist_tt, Range(FitRange_l_tt,FitRange_r_tt));
  //func_1_tt.plotOn(xframe_tt,LineColor(kRed), Range(800,8000));

  func_dijet_tt.fitTo(rooDataHist_tt, Range(FitRange_l_tt,FitRange_r_tt));
  func_dijet_tt.plotOn(xframe_tt,LineColor(kGreen), Range(800,8000));

  //==== create histogram with this
  TH1D *hist_tt_fitted = new TH1D("hist_tt_fitted", "", 8000/200, 0, 8000);
  for(int i=1; i<=hist_tt_fitted->GetXaxis()->GetNbins(); i++){
    double x_l = hist_tt_fitted->GetXaxis()->GetBinLowEdge(i);
    double x_r = hist_tt_fitted->GetXaxis()->GetBinUpEdge(i);

    if(x_l<800) continue;

    cout << "Calculating integral of ("<<x_l<<", "<<x_r<<") .." << endl;
    mwr_tt.setRange("thisrange", x_l, x_r);
    RooAbsReal *temp_integral_func_dijet_tt = func_dijet_tt.createIntegral(mwr_tt, NormSet(mwr_tt), Range("thisrange"));

    double this_bincontent = integral_tt * temp_integral_func_dijet_tt->getVal();
    cout << "--> " << this_bincontent << "\t" << sqrt(this_bincontent) << endl;
    hist_tt_fitted->SetBinContent(i,this_bincontent);
    hist_tt_fitted->SetBinError(i,sqrt(this_bincontent));
  }
  hist_tt_fitted->SetLineColor(kGreen);
  hist_tt_fitted->SetLineStyle(3);
  hist_tt_fitted->SetLineWidth(3);
  hist_tt_fitted->Draw("histsame");

  func_trijet_tt.fitTo(rooDataHist_tt, Range(FitRange_l_tt,FitRange_r_tt));
  func_trijet_tt.plotOn(xframe_tt,LineColor(kViolet), Range(800,8000));

  xframe_tt->Draw();
  xframe_tt->SetMinimum(1E-6);
  xframe_tt->SetMaximum(500);

  hist_tt_fitted->Draw("histsame");

  c_tt->SetLogy();
  c_tt->SaveAs(region+"_"+channel+"_tt.pdf");
  c_tt->SaveAs(region+"_"+channel+"_tt.png");
  c_tt->Close();

  //==== summary
  cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
  cout << "#### DY" << endl;
  cout << "Fit range : (" << FitRange_l_DY << ", " << FitRange_r_DY << ")" << endl;
  cout << "1) exponential : exp(Ax)" << endl;
  expo_slope_DY.Print();
  cout << "2) (1-x)^(A) * (1+x)^(Bx)" << endl;
  func_1_A_DY.Print();
  func_1_B_DY.Print();
  cout << "3) dijet function" << endl;
  func_dijet_p0_DY.Print();
  func_dijet_p1_DY.Print();
  func_dijet_p2_DY.Print();
  func_dijet_p3_DY.Print();
  cout << "4) trijet function" << endl;
  func_trijet_p0_DY.Print();
  func_trijet_p1_DY.Print();
  func_trijet_p2_DY.Print();
  func_trijet_p3_DY.Print();
  func_trijet_p4_DY.Print();

  cout << "#### emu" << endl;
  cout << "Fit range : (" << FitRange_l_tt << ", " << FitRange_r_tt << ")" << endl;
  cout << "1) exponential : exp(Ax)" << endl;
  expo_slope_tt.Print();
  cout << "2) (1-x)^(A) * (1+x)^(Bx)" << endl;
  func_1_A_tt.Print();
  func_1_B_tt.Print();
  cout << "3) dijet function" << endl;
  func_dijet_p0_tt.Print();
  func_dijet_p1_tt.Print();
  func_dijet_p2_tt.Print();
  func_dijet_p3_tt.Print();
  cout << "4) trijet function" << endl;
  func_trijet_p0_tt.Print();
  func_trijet_p1_tt.Print();
  func_trijet_p2_tt.Print();
  func_trijet_p3_tt.Print();
  func_trijet_p4_tt.Print();

  //==== Make output
  TFile *file_fitted_hists = new TFile("rootfile_fitted_hists_"+region+"_"+channel+".root","RECREATE");
  hist_DY_fitted->Write();
  hist_tt_fitted->Write();
  file_fitted_hists->Close();

}
