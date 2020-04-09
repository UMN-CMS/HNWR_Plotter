#include "mylib.h"
#include "canvas_margin.h"
#include "FunctionGenerator.C"

void SetParameterFromText(FitHistogram& m, TString txtFileName, TString FunctionName);
void FunctionToHist(TF1 *fitFunc, TH1D *hist, double FuncBinSize);
void StatError(TH1D *hist_Central, TH1D *hist_PreFit);

void MergeBinError(TH1D *hist, TH1D *hist_add);

//==== TODO add str_Year here
void FitBackgrounds(int Year=2016, int i_region=0, int i_channel=0, int i_sample=0, bool UpdateRange=false, int NIter=0){

  setTDRStyle();
  SumW2Error(kTRUE);
  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString str_Year = TString::Itoa(Year,10);
  TString region = "Resolved_SR";
  TString regionalias = "Resolved";
  TString channel = "Electron";
  if(i_region==0){
    region = "Resolved_SR";
    regionalias = "Resolved";
  }
  else if(i_region==1){
    region = "Boosted_SR";
    regionalias = "Boosted";
  }

  if(i_channel==0) channel = "Electron";
  else if(i_channel==1) channel = "Muon";

  TString sample = "DYJets_MG_HT_Reweighted";
  TString samplealias = "DY";
  if(i_sample==1){
    sample = "EMuMethod_TTLX_powheg";
    samplealias = "tt";
  }


  TString dirname = "HNWR_Single"+channel+"_"+region;
  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+str_Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/FitBackgrounds/"+str_Year+"/Iter_"+TString::Itoa(NIter,10)+"/";
  gSystem->mkdir(base_plotpath, kTRUE);

  //==== output
  TFile *outfile = new TFile(base_plotpath+"/shapes_"+region+"_"+channel+"_"+sample+".root","RECREATE");

  int NRebin = 40;
  double FitRange_l = 800;
  double FitRange_r = 4000;

  TFile *file = new TFile(base_filepath+"HNWRAnalyzer_SkimTree_LRSMHighPt_"+sample+".root");
  TH1D *hist = (TH1D *)file->Get(dirname+"/WRCand_Mass_"+dirname);
  double integral = hist->Integral();
  hist->Rebin(NRebin);

  //==== TEST remove negative
  for(int i=1; i<=hist->GetXaxis()->GetNbins(); i++){
    double this_y = hist->GetBinContent(i);
    double this_e = hist->GetBinError(i);
    if(this_y<0){
      //hist->SetBinContent(i,0);
      //hist->SetBinError(i,0);
    }
    if(this_y-this_e<0){
      //hist->SetBinContent(i,0);
      //hist->SetBinError(i,0);
    }
  }

  TCanvas *c = new TCanvas("c","",600,600);
  canvas_margin(c);
  c->SetLogy();
  c->cd();

  //==== axis
  TH1D *hist_dummy = new TH1D("hist_dummy", "", 8000, 0., 8000.);
  hist_axis(hist_dummy);
  hist_dummy->GetXaxis()->SetRangeUser(800.,8000.);
  if(i_region==0) hist_dummy->GetXaxis()->SetTitle("m(lljj) (GeV)");
  else hist_dummy->GetXaxis()->SetTitle("m(lJ) (GeV)");
  hist_dummy->GetYaxis()->SetRangeUser(1E-12,500.);
  hist_dummy->GetYaxis()->SetTitle("Events / "+TString::Itoa(10*NRebin,10)+" GeV");
  hist_dummy->Draw("axis");

  vector<TString> FitFunctions;
  TString FitCentral = "PolyExpo_2Par";
  TH1D *hist_Central = new TH1D("Central", "", int(8000./400.), 0., 8000.);
  TH1D *hist_Central_Stat;;

  FunctionGenerator fg;
  fg.integralValue = hist->Integral();
  cout << "@@@@ Integral = " << fg.integralValue << endl;

  vector<TF1 *> fitFuncs;
  vector<TFitResultPtr> fitResults;
  TH1D *confBand = new TH1D("confBand", "", 8000/(10*NRebin), 0., 8000.);;

  //==== write fit result in txt file
  ofstream fitresultTXT;
  fitresultTXT.open(base_plotpath+"/FitResult_"+region+"_"+channel+"_"+samplealias+".txt");

  for(int i_fit=0; i_fit<16; i_fit++){

    FitHistogram m;
    m.doDebug = false;
    m.SetHist(hist);
    m.SetFitVar(800., 8000.);
    m.ClearParameters();
    m.InitFitVar();

    m.fitRangeMin = FitRange_l;
    m.fitRangeMax = FitRange_r;

    Color_t color;
    Style_t style;
    if(i_fit==0){

      fg.Run(m, "Dijet", 4);

      color = kBlack;
      style = 1;

    }
    else if(i_fit==1){

      fg.Run(m, "Dijet", 5);

      color = kBlack;
      style = 2;

    }
    else if(i_fit==2){

      fg.Run(m, "Dijet", 6);

      color = kBlack;
      style = 3;

    }
    else if(i_fit==3){

      fg.Run(m, "Dijet", 7);

      color = kBlack;
      style = 4;

    }
    else if(i_fit==4){

      fg.Run(m, "ATLAS", 4);

      color = kBlue;
      style = 2;

    }
    else if(i_fit==5){

      fg.Run(m, "ATLAS", 5);

      color = kBlue;
      style = 3;

    }
    else if(i_fit==6){

      fg.Run(m, "ATLAS", 6);

      color = kBlue;
      style = 4;

    }
    else if(i_fit==7){

      fg.Run(m, "ATLAS", 7);

      color = kBlue;
      style = 5;

    }
    else if(i_fit==8){

      fg.Run(m, "ModifiedExpo", 4);

      color = kGreen;
      style = 1;

    }
    else if(i_fit==9){

      fg.Run(m, "PolyExt", 5);

      color = kRed;
      style = 2;

    }
    else if(i_fit==10){

      fg.Run(m, "PolyExt", 6);

      color = kRed;
      style = 3;

    }
    else if(i_fit==11){

      fg.Run(m, "PolyExt", 7);

      color = kRed;
      style = 4;

    }
    else if(i_fit==12){

      fg.Run(m, "PolyExt", 8);

      color = kRed;
      style = 5;

    }
    else if(i_fit==13){

      fg.Run(m, "PolyExpo", 2);

      color = kViolet;
      style = 1;

    }
    else if(i_fit==14){

      fg.Run(m, "PolyExpo", 3);

      color = kViolet;
      style = 2;

    }
    else if(i_fit==15){

      fg.Run(m, "PolyExpo", 4);

      color = kViolet;
      style = 3;

    }


    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << "@@@@ " << m.Name << endl;
    cout << "@@@@ Fit function name = " << m.functionalForm << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << endl;


    if(UpdateRange){
      TString txtFileName = ENV_PLOT_PATH+"/"+dataset+"/FitBackgrounds/"+str_Year+"/Iter_"+TString::Itoa(NIter,10)+"/Input_"+region+"_"+channel+"_"+samplealias+".txt";
      SetParameterFromText(m, txtFileName, m.Name);
    }
    FitFunctions.push_back( m.Name );

    m.Fit();

    //==== print
    for(int it_par=0; it_par<m.parRangeMins.size(); it_par++){
      fitresultTXT << region << "\t" << channel << "\t" << samplealias << "\t" << m.Name << "\t" << it_par << "\t" << m.fitFunc->GetParameter(it_par) << "\t" << m.fitFunc->GetParError(it_par) << "\t" << m.parInit.at(it_par) << "\t" << m.parRangeMins.at(it_par) << "\t" << m.parRangeMaxs.at(it_par) << endl;
      cout << region << "\t" << channel << "\t" << samplealias << "\t" << m.Name << "\t" << it_par << "\t" << m.fitFunc->GetParameter(it_par) << "\t" << m.fitFunc->GetParError(it_par) << "\t" << m.parInit.at(it_par) << "\t" << m.parRangeMins.at(it_par) << "\t" << m.parRangeMaxs.at(it_par) << endl;
    }

    m.fitFunc->SetLineColor(color);
    m.fitFunc->SetLineStyle(style);
    m.fitFunc->SetLineWidth(3);
    fitFuncs.push_back( m.fitFunc );
    fitResults.push_back( m.fitResultPtr );

    if(m.Name==FitCentral){
      FunctionToHist(m.fitFunc, hist_Central, NRebin*10);

      hist_Central_Stat = (TH1D *)hist_Central->Clone();
      hist_Central_Stat->SetName("TESTSTAT");

      StatError(hist_Central_Stat, hist);

      //==== merge
      MergeBinError(hist_Central, hist_Central_Stat);
    }

  }
  fitresultTXT.close();

  TGraphAsymmErrors *gr_data = new TGraphAsymmErrors(hist);
  gr_data->SetLineWidth(2);
  gr_data->SetLineColor(kBlack);
  gr_data->SetMarkerStyle(20);
  gr_data->SetMarkerColor(kBlack);
  gr_data->Draw("psame");

  //confBand->SetFillColor(kRed);
  //confBand->Draw("E3same");

  //TLegend *lg = new TLegend(0.6, 0.65, 0.9, 0.9);
  TLegend *lg = new TLegend(0.2, 0.30, 0.5, 0.55);
  for(unsigned int i=0; i<fitFuncs.size(); i++){

    fitFuncs.at(i)->Draw("same");

    double chi2 = fitFuncs.at(i)->GetChisquare();
    double ndf = fitFuncs.at(i)->GetNDF();

    double Likelihood = fitResults.at(i)->MinFcnValue();

    TString alias = fitFuncs.at(i)->GetName();

    TString str_chi2 = TString::Format("%1.3f", chi2/ndf);
    alias += " (#chi2/NDF = "+str_chi2+")";

/*
    TString str_chi2 = TString::Format("%1.3f", Likelihood);
    alias += " (ML = "+str_chi2+")";
*/
    lg->AddEntry(fitFuncs.at(i), alias, "l");

    //fitResults.at(i)->GetCovarianceMatrix().Print();

  }
  hist_Central->SetLineColor(kGray);
  hist_Central->SetLineWidth(2);
  hist_Central->SetMarkerColor(0);
  hist_Central->SetMarkerSize(0);
  hist_Central->SetFillStyle(3013);
  hist_Central->SetFillColor(kBlack);
  hist_Central->Draw("e2same");
/*
  hist_Central_Stat->SetLineColor(kGray);
  hist_Central_Stat->SetLineWidth(1);
  hist_Central_Stat->SetMarkerColor(0);
  hist_Central_Stat->SetMarkerSize(0);
  hist_Central_Stat->Draw("histe1same");
*/

  lg->AddEntry(hist_Central, "Final fitted function", "l");

  outfile->cd();
  hist->Write();
  hist_Central->Write();
  outfile->Close();

  lg->Draw();

  //==== TLatex
  TLatex latex_CMSPriliminary, latex_Lumi, channelname;
  latex_CMSPriliminary.SetNDC();
  latex_Lumi.SetNDC();
  channelname.SetNDC();
  latex_CMSPriliminary.SetTextSize(0.035);
  latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");

  TString TotalLumi = "35.92 fb^{-1} (13 TeV)";
  if(str_Year=="2017"){
    TotalLumi = "41.53 fb^{-1} (13 TeV)";
  }
  if(str_Year=="2018"){
    TotalLumi = "59.74 fb^{-1} (13 TeV)";
  }
  latex_Lumi.SetTextSize(0.035);
  latex_Lumi.SetTextFont(42);
  latex_Lumi.DrawLatex(0.72, 0.96, TotalLumi);

  channelname.SetNDC();
  channelname.SetTextSize(0.037);
  channelname.DrawLatex(0.2, 0.23, channel+" "+regionalias);
  channelname.DrawLatex(0.2, 0.18, samplealias);

  c->SaveAs(base_plotpath+"/"+region+"_"+channel+"_"+samplealias+".pdf");
  c->Close();


}

void SetParameterFromText(FitHistogram& m, TString txtFileName, TString FunctionName){

  string elline;
  ifstream in(txtFileName);
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString region,channel,sample,functionName;
    int parIndex;
    double val,err,init,min,max;
    is >> region;
    is >> channel;
    is >> sample;
    is >> functionName;
    is >> parIndex;
    is >> val;
    is >> err;
    is >> init;
    is >> min;
    is >> max;

    if(FunctionName==functionName){
      m.SetParameter(parIndex, val, min, max);
      //m.SetParameter(parIndex, min, max);
    }
  }

}

void FunctionToHist(TF1 *fitFunc, TH1D *hist, double FuncBinSize){

  for(int ix=1; ix<=hist->GetXaxis()->GetNbins(); ix++){

    double x_l = hist->GetXaxis()->GetBinLowEdge(ix);
    double x_r = hist->GetXaxis()->GetBinUpEdge(ix);
    //cout << "["<< x_l << ","<< x_r << "]" << endl;

    if(x_r<=800) hist->SetBinContent(ix,0);
    else{
      double integral= fitFunc->Integral(x_l,x_r)/FuncBinSize;
      double integralerror = fitFunc->IntegralError(x_l,x_r)/FuncBinSize;
      //cout << "["<< x_l << ","<< x_r << "]" << " : " << intergral << endl;
      hist->SetBinContent(ix,integral);
      hist->SetBinError(ix, integralerror);
      cout << "["<< x_l << ","<< x_r << "]" << " : " << integral << "\t" << integralerror << endl;
    }

  }


}

void StatError(TH1D *hist_Central, TH1D *hist_PreFit){


  //==== check binning first
  for(int i=1; i<=hist_Central->GetXaxis()->GetNbins(); i++){
    double x_l = hist_PreFit->GetXaxis()->GetBinLowEdge(i);
    double x_r = hist_PreFit->GetXaxis()->GetBinUpEdge(i);
    printf("[%d,%d] : %f\t%f\n",int(x_l),int(x_r),hist_Central->GetBinContent(i),hist_Central->GetBinError(i));
  }
/*
  cout << "[PreFit] hist_PreFit" << endl;
  for(int i=1; i<=hist_PreFit->GetXaxis()->GetNbins(); i++){
    double x_l = hist_PreFit->GetXaxis()->GetBinLowEdge(i);
    double x_r = hist_PreFit->GetXaxis()->GetBinUpEdge(i);
    printf("[%d,%d]\n",int(x_l),int(x_r));
  }
*/

  //==== Find first zero bin from hist_PreFit
  int FirstZeroBinIndex = 0;
  for(int i=1; i<=hist_PreFit->GetXaxis()->GetNbins(); i++){
    double x_l = hist_PreFit->GetXaxis()->GetBinLowEdge(i);
    double x_r = hist_PreFit->GetXaxis()->GetBinUpEdge(i);
    if(x_r<=800) continue;
    if(hist_PreFit->GetBinContent(i)<=0){
      FirstZeroBinIndex = i;
      break;
    }
  }
  cout << "First zero at " << hist_PreFit->GetXaxis()->GetBinLowEdge(FirstZeroBinIndex) << ", " << hist_PreFit->GetXaxis()->GetBinUpEdge(FirstZeroBinIndex) << endl;

  //==== before nozero bin, assign error
  double newLargeBin[FirstZeroBinIndex];
  newLargeBin[0] = 0;
  for(int i=1; i<=FirstZeroBinIndex-2; i++){
    double x_l = hist_PreFit->GetXaxis()->GetBinLowEdge(i);
    double x_r = hist_PreFit->GetXaxis()->GetBinUpEdge(i);
    printf("[%d,%d]\n",int(x_l),int(x_r));
    double this_stat_error = hist_PreFit->GetBinError(i);
    newLargeBin[i] = x_r;
    hist_Central->SetBinError(i, this_stat_error);
  }
  newLargeBin[FirstZeroBinIndex-1] = 8000.;
  TH1D *hist_PreFit_Merge = (TH1D *)hist_PreFit->Rebin(FirstZeroBinIndex-1, "MERGE", newLargeBin);

  //cout << "[Merge]" << endl;
  for(int i=1; i<=hist_PreFit_Merge->GetXaxis()->GetNbins(); i++){
    double x_l = hist_PreFit_Merge->GetXaxis()->GetBinLowEdge(i);
    double x_r = hist_PreFit_Merge->GetXaxis()->GetBinUpEdge(i);
    //printf("%d : [%d,%d] -> %f\n",i,int(x_l),int(x_r),hist_PreFit_Merge->GetBinContent(i));
  }

  double LargeBinContent = hist_PreFit_Merge->GetBinContent(FirstZeroBinIndex-1);
  double LargeBinError = hist_PreFit_Merge->GetBinError(FirstZeroBinIndex-1);
  //cout << "[StatError] Large bin : " << LargeBinContent << "\t" << LargeBinError << endl;

  double Lastbinsize = newLargeBin[FirstZeroBinIndex-1]-newLargeBin[FirstZeroBinIndex-2];
  double NewBinError = LargeBinError/sqrt( Lastbinsize/400. );

  //==== now other bins
  for(int i=FirstZeroBinIndex-2+1; i<=hist_Central->GetXaxis()->GetNbins(); i++){
    double x_l = hist_PreFit->GetXaxis()->GetBinLowEdge(i);
    double x_r = hist_PreFit->GetXaxis()->GetBinUpEdge(i);
    printf("[%d,%d]\n",int(x_l),int(x_r));
    hist_Central->SetBinError(i, NewBinError);
  }

  for(int i=1; i<=hist_Central->GetXaxis()->GetNbins(); i++){
    double x_l = hist_PreFit->GetXaxis()->GetBinLowEdge(i);
    double x_r = hist_PreFit->GetXaxis()->GetBinUpEdge(i);
    printf("[%d,%d] : %f\t%f\n",int(x_l),int(x_r),hist_Central->GetBinContent(i),hist_Central->GetBinError(i));
  }

}

void MergeBinError(TH1D *hist, TH1D *hist_add){

  for(int i=1; i<=hist->GetXaxis()->GetNbins(); i++){
    if(hist->GetXaxis()->GetBinUpEdge(i)<=800.) continue;
    double err = hist->GetBinError(i);
    double err_add = hist_add->GetBinError(i);
    double newerr = sqrt(err*err+err_add*err_add);
    hist->SetBinError(i, newerr);
  }

}


