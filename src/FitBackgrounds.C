using namespace RooFit;
#include "mylib.h"
#include "canvas_margin.h"
#include "PDFGenerator.C"

//==== TODO add year here
void FitBackgrounds(int i_region=0, int i_channel=0, int i_sample=0){

  setTDRStyle();
  SumW2Error(kTRUE);
  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString Year = "2016";
  TString region = "Resolved_SR";
  TString channel = "Electron";
  if(i_region==0) region = "Resolved_SR";
  else if(i_region==1) region = "Boosted_SR";
  if(i_channel==0) channel = "Electron";
  else if(i_channel==1) channel = "Muon";

  TString sample = "DYJets_MG_HT_Reweighted";
  TString samplealias = "DY";
  if(i_sample==1){
    sample = "EMuMethod_TTLX_powheg";
    samplealias = "tt";
  }


  TString dirname = "HNWR_Single"+channel+"_"+region;
  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/FitBackgrounds/"+Year+"/";
  gSystem->mkdir(base_plotpath, kTRUE);

  //==== output
  TFile *outfile = new TFile(base_plotpath+"/shapes_"+region+"_"+channel+"_"+sample+".root","RECREATE");

  int NRebin = 20;
  double FitRange_l = 800;
  double FitRange_r = 8000;

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
  c->cd();

  //==== axis
  TH1D *hist_dummy = new TH1D("hist_dummy", "", 8000, 0., 8000.);
  hist_axis(hist_dummy);
  hist_dummy->GetXaxis()->SetRangeUser(800.,8000.);
  if(i_region==0) hist_dummy->GetXaxis()->SetTitle("m(lljj) (GeV)");
  else hist_dummy->GetXaxis()->SetTitle("m(lJ) (GeV)");
  hist_dummy->GetYaxis()->SetRangeUser(1E-6,500.);
  hist_dummy->GetYaxis()->SetTitle("Events / "+TString::Itoa(10*NRebin,10)+" GeV");
  hist_dummy->Draw("axis");

  TLegend *lg = new TLegend(0.6, 0.65, 0.9, 0.9);

  RooPlot* xframe;
  vector<TString> FitFunctions;
  vector<RooFitResult *> FitResults;
  TString FitCentral = "Dijet_4Par";

  PDFGenerator pg;

  for(int i_fit=0; i_fit<9; i_fit++){

    FitHistogram m;
    m.doDebug = false;
    m.SetHist(hist);
    m.SetFitVar("mwr", 800., 8000.);
    m.ClearPArameters();
    m.InitFitVar();

    m.fitRangeMin = 800.;
    m.fitRangeMax = 8000.;

    Color_t color;
    Style_t style;
    if(i_fit==0){

      pg.Run(m, "Dijet", 4, "mwr");

      color = kBlack;
      style = 1;

    }
    else if(i_fit==1){

      pg.Run(m, "Dijet", 5, "mwr");

      color = kBlack;
      style = i_fit+1;

    }
    else if(i_fit==2){

      pg.Run(m, "Dijet", 6, "mwr");

      color = kBlack;
      style = i_fit+1;

    }
    else if(i_fit==3){

      pg.Run(m, "PolyExt", 5, "mwr");

      color = kRed;
      style = i_fit+1;

    }
    else if(i_fit==4){

      pg.Run(m, "PolyExt", 6, "mwr");

      color = kRed;
      style = i_fit+1;

    }
    else if(i_fit==5){

      pg.Run(m, "PolyExt", 7, "mwr");

      color = kRed;
      style = i_fit+1;

    }
    else if(i_fit==6){

      pg.Run(m, "PolyExt", 8, "mwr");

      color = kRed;
      style = i_fit+1;

    }
    else if(i_fit==7){

      pg.Run(m, "ATLAS", 4, "mwr");

      color = kBlue;
      style = i_fit+1;

    }
    else if(i_fit==8){

      pg.Run(m, "ATLAS", 5, "mwr");

      color = kBlue;
      style = i_fit+1;

    }
    else if(i_fit==9){

      pg.Run(m, "ATLAS", 6, "mwr");

      color = kBlue;
      style = i_fit+1;

    }
    else if(i_fit==10){

      pg.Run(m, "ATLAS", 7, "mwr");

      color = kBlue;
      style = i_fit+1;

    }


    FitFunctions.push_back( m.Name );

    m.Fit();

    FitResults.push_back( m.getFitResult() );

    RooRealVar* rrv = m.getFitVar();
    RooDataHist* rdh = m.getDataHist();

    if(i_fit==0){
      xframe = rrv->frame();
      rdh->plotOn(xframe, MarkerColor(0), LineColor(0), Name("FirstDataHist") );
    }

    if(m.Name==FitCentral){
      //m.getPdf()->plotOn( xframe, FillColor(kBlack+1), Range(800,8000), VisualizeError(*(m.getFitResult())), Name(m.Name+"_Linearfiterrorband") );
      m.getPdf()->plotOn( xframe, FillColor(0), LineColor(0), Range(800,8000), VisualizeError(*(m.getFitResult()),1,kFALSE ), DrawOption("L"), Name(m.Name+"_Samplingfiterrorband") );
    }

    m.getPdf()->plotOn(xframe,LineColor(color), LineStyle(style), LineWidth(2), Range(800,8000), Name(m.Name+"_Central") );
    //m.getPdf()->plotOn(xframe,LineColor(0), Range(800,8000), Name(m.Name+"_Central") );

    lg->AddEntry(xframe->findObject(m.Name+"_Central") , m.Name, "l");

    //rdh->plotOn(xframe, Name(m.Name+"_DataOverlay") );

  }

  xframe->Draw("same");

  c->SetLogy();

  cout << "@@@@ Printing items" << endl;
  for(int i=0; i<xframe->numItems(); i++){
    cout << xframe->getObject(i)->GetName() << "\t" << xframe->getObject(i)->GetUniqueID() << "\t" << endl;
  }

  //==== Fit Summary
  cout << "@@@@ Fit results @@@@" << endl;
  for(int i=0; i<FitFunctions.size(); i++){
    TString fitFunc = FitFunctions.at(i);
    cout << "@@@@   Func = " << fitFunc << endl;
    RooFitResult* FitResult = FitResults.at(i);
    const RooArgList& arglist = FitResult->floatParsFinal();
    const int NFitPar = arglist.getSize();
    for(int j=0; j<NFitPar; j++){
      RooAbsArg *raa = arglist.at(j);
      raa->Print();
    }
  }


  //==== Get data
  TGraph *gr_data = (TGraph *)xframe->findObject("FirstDataHist");
  gr_data->SetMarkerColor(kBlack);
  gr_data->SetLineColor(kBlack);

  //==== Retrieve fit paramter error band as TGraph
  TGraph *gr_central = (TGraph *)xframe->findObject(FitCentral+"_Central");
  RooCurve* fiterrorband = xframe->getCurve(FitCentral+"_Samplingfiterrorband");
  TGraph *gr_fitup = new TGraph(gr_central->GetN());
  TGraph *gr_fitdn = new TGraph(gr_central->GetN());
  for(int i =0; i<fiterrorband->GetN(); i++){
    if(i<gr_central->GetN())
      gr_fitup->SetPoint(i, fiterrorband->GetX()[i], fiterrorband->GetY()[i]);
    else
      gr_fitdn->SetPoint(i, fiterrorband->GetX()[i], fiterrorband->GetY()[i]);
  }
  //==== Now integrate them to make histograms
  TH1D *hist_central = new TH1D("hist_central", "", (8000-800)/(10*NRebin), 800., 8000.);
  TH1D *hist_fitup = new TH1D("hist_fitup", "", (8000-800)/(10*NRebin), 800., 8000.);
  TH1D *hist_fitdn = new TH1D("hist_fitdn", "", (8000-800)/(10*NRebin), 800., 8000.);
  for(int i=1; i<=hist_central->GetXaxis()->GetNbins(); i++){
    double x_l = hist_central->GetXaxis()->GetBinLowEdge(i);
    double x_r = hist_central->GetXaxis()->GetBinUpEdge(i);

    double this_bincontent_central = IntegrateGraph(gr_central, x_l, x_r, 1000)/(10*NRebin);
    double this_bincontent_fitup   = IntegrateGraph(gr_fitup, x_l, x_r, 1000)/(10*NRebin);
    double this_bincontent_fitdn   = IntegrateGraph(gr_fitdn, x_l, x_r, 1000)/(10*NRebin);

    hist_central->SetBinContent(i, this_bincontent_central);
    hist_central->SetBinError(i, sqrt(this_bincontent_central));
    hist_fitup->SetBinContent(i, this_bincontent_fitup);
    hist_fitdn->SetBinContent(i, this_bincontent_fitdn);

  }

  hist_central->SetLineColor(kBlack);
  hist_fitup->SetLineColor(kGray);
  hist_fitdn->SetLineColor(kGray);

  //==== fit variations as systematics
  //==== Create a graph together to plot error bands
  //==== GetN should be 4+2*GetNbins
  const int NSystGraph = 4+2*hist_central->GetXaxis()->GetNbins();
  TGraph *systerrorband = new TGraph(NSystGraph);
  //==== And of couse, syst up/down histogram by integration
  TH1D *hist_systup = (TH1D *)hist_central->Clone(); hist_systup->SetName("hist_systup");
  TH1D *hist_systdn = (TH1D *)hist_central->Clone(); hist_systdn->SetName("hist_systdn");
  //==== save graphs and create the band later
  vector<TGraph *> FitVarsGraphs;
  //==== get edge values (i.e., at 800 and 8000);
  double systup_800(gr_central->GetY()[0]), systdn_800(gr_central->GetY()[0]);
  double systup_8000(gr_central->GetY()[gr_central->GetN()-1]), systdn_8000(gr_central->GetY()[gr_central->GetN()-1]);
  for(int i=0; i<FitFunctions.size(); i++){

    TString fitFunc = FitFunctions.at(i);
    if(fitFunc==FitCentral) continue;

    cout << "@@@@ Fit variation with : " << fitFunc << endl;
    TGraph *this_gr = (TGraph *)xframe->findObject(fitFunc+"_Central");
    FitVarsGraphs.push_back( this_gr );

    //==== edge values
    systup_800  = max( systup_800 ,  this_gr->GetY()[0] );
    systdn_800  = min( systdn_800 ,  this_gr->GetY()[0] );
    systup_8000 = max( systup_8000 , this_gr->GetY()[this_gr->GetN()-1] );
    systdn_8000 = min( systdn_8000 , this_gr->GetY()[this_gr->GetN()-1] );

  }
  systerrorband->SetPoint(0, 800., systup_800);
  systerrorband->SetPoint(NSystGraph-1, 800., systdn_800);
  systerrorband->SetPoint(hist_central->GetXaxis()->GetNbins()+1, 8000., systup_8000);
  systerrorband->SetPoint(hist_central->GetXaxis()->GetNbins()+2, 8000., systdn_8000);
  //=== Okay, now for each bin, loop over graphs and set values
  for(int j=1; j<=hist_central->GetXaxis()->GetNbins(); j++){
    double x_l = hist_central->GetXaxis()->GetBinLowEdge(j);
    double x_r = hist_central->GetXaxis()->GetBinUpEdge(j);
    double x_c = hist_central->GetXaxis()->GetBinCenter(j);

    double this_central = hist_central->GetBinContent(j);
    double this_max(this_central), this_min(this_central);
    for(int ig=0; ig<FitVarsGraphs.size(); ig++){
      double this_fitvar = IntegrateGraph(FitVarsGraphs.at(ig), x_l, x_r, 1000)/(10*NRebin);
      this_max = max( this_max, this_fitvar );
      this_min = min( this_min, this_fitvar );
    }

    hist_systup->SetBinContent(j, this_max);
    hist_systdn->SetBinContent(j, this_min);

    systerrorband->SetPoint(j, x_c, this_max);
    systerrorband->SetPoint(NSystGraph-1-j, x_c, this_min);

  }

  hist_systup->SetLineColor(kRed);
  hist_systdn->SetLineColor(kRed);

  systerrorband->SetLineColor(0);
  systerrorband->SetFillColorAlpha(kRed, 0.4);
  systerrorband->Draw("fsame");
  fiterrorband->SetFillColorAlpha(kBlue, 0.4);
  fiterrorband->Draw("fsame");

  for(int ig=0; ig<FitVarsGraphs.size(); ig++){
    FitVarsGraphs.at(ig)->Draw("lsame");
  }

  gr_central->SetLineColor(kBlack);
  gr_central->Draw("lsame");
  gr_data->Draw("psame");

  //==== To save are the histograms
  outfile->cd();
  hist_central->Write();
  hist_fitup->Write();
  hist_fitdn->Write();
  hist_systup->Write();
  hist_systdn->Write();
  outfile->Close();

  //hist_systup->Draw("histsame");
  //hist_systdn->Draw("histsame");

  lg->AddEntry(fiterrorband, "Fit parameter error", "f");
  lg->AddEntry(systerrorband, "Fit variation", "f");
  lg->Draw();

  c->SaveAs(base_plotpath+"/"+region+"_"+channel+"_"+samplealias+".pdf");
  c->SaveAs(base_plotpath+"/"+region+"_"+channel+"_"+samplealias+".png");
  c->Close();

  //====================================================
  //==== Fisher test
  //====================================================

  TFile *testfile = new TFile("testfile.root", "RECREATE");
  testfile->cd();
  //==== 1) hist starts from 0 GeV. Change it here
  TH1D *hist_Data_800GeV = new TH1D("hist_Data_800GeV", "", (8000-800)/(10*NRebin), 800., 8000.);
  int dummy_counter = 1;
  for(int j=1; j<=hist->GetXaxis()->GetNbins(); j++){
    double x_l = hist->GetXaxis()->GetBinLowEdge(j);
    double x_r = hist->GetXaxis()->GetBinUpEdge(j);
    double x_c = hist->GetXaxis()->GetBinCenter(j);
    if(x_l>=800){
      hist_Data_800GeV->SetBinContent( dummy_counter, hist->GetBinContent(j) );
      dummy_counter++;
    }
  }
  hist->Write();
  hist_Data_800GeV->Write();
  testfile->Close();

  vector<TGraph *> AllFitGraphs;
  vector<double> RSSs;
  for(int i=0; i<FitFunctions.size(); i++){
    TString fitFunc = FitFunctions.at(i);
    TGraph *this_gr = (TGraph *)xframe->findObject(fitFunc+"_Central");
    AllFitGraphs.push_back( this_gr );
    RSSs.push_back( 0. );
  }

  int N_DATA(0);
  for(int j=1; j<=hist_Data_800GeV->GetXaxis()->GetNbins(); j++){

    double x_l = hist_Data_800GeV->GetXaxis()->GetBinLowEdge(j);
    double x_r = hist_Data_800GeV->GetXaxis()->GetBinUpEdge(j);

    double this_datapoint = hist_Data_800GeV->GetBinContent(j);
    if(this_datapoint==0){
      cout << "[F-Test] skipping ["<<x_l<<","<<x_r<<"] : zero bin content" << endl; 
    }
    N_DATA++;

    for(int ig=0; ig<AllFitGraphs.size(); ig++){
      double this_fitvar = IntegrateGraph(AllFitGraphs.at(ig), x_l, x_r, 1000)/(10*NRebin);
      RSSs.at(ig) += (this_datapoint - this_fitvar) *  (this_datapoint - this_fitvar);
    }

  }

  for(int ig=0; ig<AllFitGraphs.size(); ig++){
    TString grname = AllFitGraphs.at(ig)->GetName();
    cout << grname << " : " << RSSs.at(ig) << endl;
  }

  vector< pair<TString, TString> > FTestPairNames;
  vector< pair<double, double> > FTestPairNPars;

  FTestPairNames.push_back( make_pair( "Dijet_4Par_Central", "Dijet_5Par_Central" ) );
  FTestPairNPars.push_back(  make_pair( 4, 5 ) );

  FTestPairNames.push_back( make_pair( "Dijet_5Par_Central", "Dijet_6Par_Central" ) );
  FTestPairNPars.push_back(  make_pair( 5, 6 ) );

  FTestPairNames.push_back( make_pair( "PolyExt_5Par_Central", "PolyExt_6Par_Central" ) );
  FTestPairNPars.push_back(  make_pair( 5, 6 ) );

  FTestPairNames.push_back( make_pair( "PolyExt_6Par_Central", "PolyExt_7Par_Central" ) );
  FTestPairNPars.push_back(  make_pair( 6, 7 ) );

  FTestPairNames.push_back( make_pair( "PolyExt_7Par_Central", "PolyExt_8Par_Central" ) );
  FTestPairNPars.push_back(  make_pair( 7, 8 ) );

  FTestPairNames.push_back( make_pair( "ATLAS_4Par_Central", "ATLAS_5Par_Central" ) );
  FTestPairNPars.push_back(  make_pair( 4, 5 ) );

  FTestPairNames.push_back( make_pair( "ATLAS_5Par_Central", "ATLAS_6Par_Central" ) );
  FTestPairNPars.push_back(  make_pair( 5, 6 ) );

  FTestPairNames.push_back( make_pair( "ATLAS_6Par_Central", "ATLAS_7Par_Central" ) );
  FTestPairNPars.push_back(  make_pair( 6, 7 ) );

  for(int i_pair=0; i_pair<FTestPairNames.size(); i_pair++){

    TString Func_1 = FTestPairNames.at(i_pair).first;
    TString Func_2 = FTestPairNames.at(i_pair).second;
    int NPar_1 = FTestPairNPars.at(i_pair).first;
    int NPar_2 = FTestPairNPars.at(i_pair).second;


    cout << "[F-Test] testing Func_1 = " << Func_1 << ", Func_2 = " << Func_2 << endl;

    double RSS_1(-1), RSS_2(-1);

    for(int ig=0; ig<AllFitGraphs.size(); ig++){
      TString grname = AllFitGraphs.at(ig)->GetName();
      if(grname==Func_1) RSS_1 = RSSs.at(ig);
      if(grname==Func_2) RSS_2 = RSSs.at(ig);
    }

    double F21 = ( (RSS_1-RSS_2)/(NPar_2-NPar_1) ) / ( (RSS_2)/(N_DATA-NPar_2) );
    double CL21 = 1. - ROOT::Math::fdistribution_cdf(F21, NPar_2-NPar_1, N_DATA-NPar_2 );

    cout << "[F-Test]   N_DATA = " << N_DATA << endl;
    cout << "[F-Test]   RSS_1 = " << RSS_1 << endl;
    cout << "[F-Test]   RSS_2 = " << RSS_2 << endl;
    cout << "[F-Test]   NPar_1 = " << NPar_1 << endl;
    cout << "[F-Test]   NPar_2 = " << NPar_2 << endl;
    cout << "[F-Test]   --> F21 = " << F21 << endl;
    cout << "[F-Test]   --> CL21 = " << CL21 << endl;

  }


}


//===== saving trijet functions..
/*
    else if(i_fit==1){

      m.Name = "TrijetFunction_Alt5Par";
      m.functionalForm = "func_Trijet_Alt5Par_p0 * (func_Trijet_Alt5Par_p2 * mwr/13000. - 1) / pow(mwr/13000., func_Trijet_Alt5Par_p1 + func_Trijet_Alt5Par_p3 * log(mwr/13000.) + func_Trijet_Alt5Par_p4 * log(mwr/13000.) * log(mwr/13000.))";
      m.fitRangeMin = 800.;
      m.fitRangeMax = 8000.;

      m.InitParameters(5);

      m.parNames.at(0) = "func_Trijet_Alt5Par_p0";
      m.parRangeMins.at(0) = 1E-5;
      m.parRangeMaxs.at(0) = 100.;

      m.parNames.at(1) = "func_Trijet_Alt5Par_p1";
      m.parRangeMins.at(1) = 0.;
      m.parRangeMaxs.at(1) = 50.;

      m.parNames.at(2) = "func_Trijet_Alt5Par_p2";
      m.parRangeMins.at(2) = 13000./800., 100.;
      m.parRangeMaxs.at(2) = 100.;

      m.parNames.at(3) = "func_Trijet_Alt5Par_p3";
      m.parRangeMins.at(3) = 0.;
      m.parRangeMaxs.at(3) = 50.;

      m.parNames.at(4) = "func_Trijet_Alt5Par_p4";
      m.parRangeMins.at(4) = 0.;
      m.parRangeMaxs.at(4) = 50.;

      color = kBlack;
      style = 2;

    }
    else if(i_fit==2){

      m.Name = "TrijetFunction_Alt4Par";
      m.functionalForm = "func_Trijet_Alt4Par_p0 * (func_Trijet_Alt4Par_p2 * mwr/13000. - 1) / pow(mwr/13000., func_Trijet_Alt4Par_p1 + func_Trijet_Alt4Par_p3 * log(mwr/13000.))";
      m.fitRangeMin = 800.;
      m.fitRangeMax = 8000.;

      m.InitParameters(4);

      m.parNames.at(0) = "func_Trijet_Alt4Par_p0";
      m.parRangeMins.at(0) = 1E-5;
      m.parRangeMaxs.at(0) = 100.;

      m.parNames.at(1) = "func_Trijet_Alt4Par_p1";
      m.parRangeMins.at(1) = 0.;
      m.parRangeMaxs.at(1) = 50.;

      m.parNames.at(2) = "func_Trijet_Alt4Par_p2";
      m.parRangeMins.at(2) = 13000./800., 100.;
      m.parRangeMaxs.at(2) = 100.;

      m.parNames.at(3) = "func_Trijet_Alt4Par_p3";
      m.parRangeMins.at(3) = 0.;
      m.parRangeMaxs.at(3) = 50.;

      color = kBlack;
      style = 3;

    }
    else if(i_fit==3){

      m.Name = "TrijetFunction_Alt3Par";
      m.functionalForm = "func_Trijet_Alt3Par_p0 * (func_Trijet_Alt3Par_p2 * mwr/13000. - 1) / pow(mwr/13000., func_Trijet_Alt3Par_p1)";
      m.fitRangeMin = 800.;
      m.fitRangeMax = 8000.;

      m.InitParameters(3);

      m.parNames.at(0) = "func_Trijet_Alt3Par_p0";
      m.parRangeMins.at(0) = 1E-5;
      m.parRangeMaxs.at(0) = 100.;

      m.parNames.at(1) = "func_Trijet_Alt3Par_p1";
      m.parRangeMins.at(1) = 0.;
      m.parRangeMaxs.at(1) = 50.;

      m.parNames.at(2) = "func_Trijet_Alt3Par_p2";
      m.parRangeMins.at(2) = 13000./800., 100.;
      m.parRangeMaxs.at(2) = 100.;

      color = kBlack;
      style = 4;

    }



*/













