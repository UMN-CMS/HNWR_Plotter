#include "canvas_margin.h"
#include "mylib.h"

void Get_DYNormalization(int xxx=0){

  TString filename_prefix = "HNWROnZ_";
  //==== FIXME temp 2018
  if(xxx!=2) filename_prefix += "SkimTree_LRSMHighPt_";

  //gErrorIgnoreLevel = kFatal;

  setTDRStyle();

  TString Year = "2016";
  double LumiError = 0.025;
  if(xxx==1){
    Year = "2017";
    LumiError = 0.023;
  }
  else if(xxx==2){
    Year = "2018";
    LumiError = 0.025;
  }

  vector<TString> Systs = {
    "JetRes",
    "JetEn",
    "MuonEn",
    "MuonIDSF",
    "ElectronRes",
    "ElectronEn",
    "ElectronIDSF",
  };
  //==== XXX No syst here
  Systs.clear();

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/OnZ/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/DYNormalization/"+Year+"/";

/*
  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
*/

  //==== MCs

  vector<TString> bkgds;
  TString samplename_DY10to50 = "DYJets10to50";
  TString samplename_DY50 = "DYJets";
  if(Year=="2016"){

    bkgds = {
      "WJets_MG",
      "WZ_pythia", "ZZ_pythia", "WW_pythia",
      "TTLL_powheg", "TTLJ_powheg",
    };

    samplename_DY10to50 = "DYJets10to50";
    samplename_DY50 = "DYJets";

  }
  else if(Year=="2017"){

    bkgds = {
      "TTLL_powheg", "TTLJ_powheg",
      "WJets_MG",
      "ttW", "ttZ",
      "WZ_pythia", "ZZ_pythia", "WW_pythia",
      "WWW", "WWZ", "WZZ", "ZZZ",
    };

    samplename_DY10to50 = "DYJets10to50_MG";
    samplename_DY50 = "DYJets";

  }
  else if(Year=="2018"){

    bkgds = {
      "TTLL_powheg", "TTLJ_powheg",
      "WZ_pythia", "ZZ_pythia", "WW_pythia",
    };

    samplename_DY10to50 = "DYJets10to50_MG";
    samplename_DY50 = "DYJets_MG";

  }

  TFile *file_DY10to50 = new TFile(base_filepath+"/"+filename_prefix+samplename_DY10to50+".root");
  TFile *file_DY50 = new TFile(base_filepath+"/"+filename_prefix+samplename_DY50+".root");

  for(int it_fl=0; it_fl<2; it_fl++){

    TString leptonFlavour = "Electron";
    if(it_fl==1){
      leptonFlavour = "Muon";
    }

    TString dirname = "HNWR_Single"+leptonFlavour+"_OnZ";
    TString var = "NEvent";

    TFile *file_DATA = new TFile(base_filepath+"/"+filename_prefix+"data_Single"+leptonFlavour+".root");
    TH1D *hist_DATA = (TH1D *)file_DATA->Get(dirname+"/"+var+"_"+dirname);

    for(unsigned int it_bkgd=0; it_bkgd<bkgds.size(); it_bkgd++){
      TFile *file_bkgd = new TFile(base_filepath+"/"+filename_prefix+bkgds.at(it_bkgd)+".root");
      TH1D *hist_bkgd = (TH1D *)file_bkgd->Get(dirname+"/"+var+"_"+dirname);
      if(!hist_bkgd) continue;

      //==== FIXME TEMP 2018
      if(xxx==2){
        hist_bkgd->Scale(Get2018DataSurvFrac(leptonFlavour));
      }

      hist_DATA->Add(hist_bkgd, -1.);
    }

    TH1D *hist_DY10to50 = (TH1D *)file_DY10to50->Get(dirname+"/"+var+"_"+dirname);
    TH1D *hist_DY50 = (TH1D *)file_DY50->Get(dirname+"/"+var+"_"+dirname);
    if(hist_DY10to50) hist_DY50->Add(hist_DY10to50);
    //==== FIXME TEMP 2018
    if(xxx==2){
      hist_DY50->Scale(Get2018DataSurvFrac(leptonFlavour));
    }

    //==== Central

    double y_DATA = hist_DATA->GetBinContent(1);
    double y_MC = hist_DY50->GetBinContent(1);

    //==== Stat error

    double RelStatError_DATA = hist_DATA->GetBinError(1) / hist_DATA->GetBinContent(1);
    double RelStatError_MC = hist_DY50->GetBinError(1) / hist_DY50->GetBinContent(1);

    //==== Syst error

    double RelSystError_MC = 0.;

    //==== norm
    //==== 1) Xsec
    double xsec_central = 2075.14;
    double err_int = 0.33 / xsec_central;
    double err_pdf = 10.80 / xsec_central;
    double err_scale = 0.02;
    //==== sum
    double NormSyst_Xsec = sqrt( err_int*err_int + err_pdf*err_pdf + err_scale*err_scale);
    //==== Add
    RelSystError_MC = sqrt( RelSystError_MC*RelSystError_MC + NormSyst_Xsec*NormSyst_Xsec );

    //==== shape
    for(unsigned it_Syst=0; it_Syst<Systs.size(); it_Syst++){

      TString Syst = Systs.at(it_Syst);

      TDirectory *dir_Up = (TDirectory *)file_DY50->Get("Syst_"+Syst+"Up_"+dirname);
      TH1D *hist_Up = NULL;
      if(dir_Up){
        hist_Up = (TH1D *)dir_Up->Get( var+"_Syst_"+Syst+"Up_"+dirname );
      }
      else{
        hist_Up = (TH1D *)hist_DY50->Clone();
      }

      TDirectory *dir_Down = (TDirectory *)file_DY50->Get("Syst_"+Syst+"Down_"+dirname);
      TH1D *hist_Down = NULL;
      if(dir_Down){
        hist_Down = (TH1D *)dir_Down->Get( var+"_Syst_"+Syst+"Down_"+dirname );
      }
      else{
        hist_Down = (TH1D *)hist_DY50->Clone();
      }

      //==== FIXME TEMP 2018      
      if(xxx==2){
        hist_Up->Scale(Get2018DataSurvFrac(leptonFlavour));
        hist_Down->Scale(Get2018DataSurvFrac(leptonFlavour));
      }

      double y_Up = hist_Up->Integral();
      double y_Down = hist_Down->Integral();

      double reldiff_Up = fabs(y_Up - y_MC)/y_MC;
      double reldiff_Down = fabs(y_Down - y_MC)/y_MC;

      double rel_Syst = sqrt( (reldiff_Up*reldiff_Up + reldiff_Down*reldiff_Down) / 2. );

      //cout << "Syst source = " << Syst << " : " << rel_Syst << endl;

      RelSystError_MC = sqrt( RelSystError_MC*RelSystError_MC + rel_Syst*rel_Syst );
    }

    double SF = y_DATA/y_MC;
    double SF_StarErr = sqrt(RelStatError_DATA*RelStatError_DATA+RelStatError_MC*RelStatError_MC) * SF;
    double SF_Syst = RelSystError_MC * SF;

    cout << "["<<leptonFlavour<<"]"<< endl;

    //==== For latex
    //printf("%1.3f \\pm %1.3f\\stat \\pm %1.3f\\thy\n", SF, SF_StarErr, SF_Syst);

    //==== for values
    cout << SF << "\t" << sqrt(  SF_StarErr*SF_StarErr + SF_Syst*SF_Syst ) << endl;


  }

}
