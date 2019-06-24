#include "canvas_margin.h"
#include "mylib.h"
#include "ObsPredComp.h"

void Draw_BinnedSample(int xxx=0){

  bool DrawCompPlot = true;

  setTDRStyle();

  TString Year = "2016";
  TString TotalLumi = "35.9 fb^{-1} (13 TeV)";
  if(xxx==1){
    Year = "2017";
    TotalLumi = "41.5 fb^{-1} (13 TeV)";
  }
  if(xxx==2){
    Year = "2018";
    TotalLumi = "60. fb^{-1} (13 TeV)";
  }

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/BinnedSample/"+Year+"/";

  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  vector< TString > SetNames, samples_Incl;
  vector< vector<TString> > lists_Binned;
  vector< vector<Color_t> > colors_Binned;

  if(xxx==0){
    SetNames = {
      "DYJets_MG_vs_HTBinned",
      "DYJets_vs_HTBinned",

      "DYJets_MG_vs_DYJets",
    };
    samples_Incl = {
      "DYJets_MG",
      "DYJets",

      "DYJets_MG",
    };
    lists_Binned = {
      {"DYJets_MG_HT-100to200", "DYJets_MG_HT-1200to2500", "DYJets_MG_HT-200to400", "DYJets_MG_HT-2500toInf", "DYJets_MG_HT-400to600", "DYJets_MG_HT-600to800", "DYJets_MG_HT-70to100", "DYJets_MG_HT-800to1200"},
      {"DYJets_MG_HT-100to200", "DYJets_MG_HT-1200to2500", "DYJets_MG_HT-200to400", "DYJets_MG_HT-2500toInf", "DYJets_MG_HT-400to600", "DYJets_MG_HT-600to800", "DYJets_MG_HT-70to100", "DYJets_MG_HT-800to1200"},

      {"DYJets"},
    };
    colors_Binned = {
      {kRed, kOrange, kYellow, kGreen, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue},
      {kRed, kOrange, kYellow, kGreen, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue},

      {kRed},
    };
  }
  else if(xxx==2){

    SetNames = {
      "DYJets_MG_vs_JetBinned",
      "DYJets_MG_vs_HTBinned",
    };
    samples_Incl = {
      "DYJets_MG",
      "DYJets_MG",
    };
    lists_Binned = {
      {"DY1Jets_MG", "DY2Jets_MG", "DY3Jets_MG", "DY4Jets_MG"},
      {"DYJets_MG_HT-100to200", "DYJets_MG_HT-1200to2500", "DYJets_MG_HT-200to400", "DYJets_MG_HT-2500toInf", "DYJets_MG_HT-400to600", "DYJets_MG_HT-600to800", "DYJets_MG_HT-70to100", "DYJets_MG_HT-800to1200"},
    };
    colors_Binned = {
      {kRed, kOrange, kYellow, kGreen, kBlue},
      {kRed, kOrange, kYellow, kGreen, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue},
    };

  }

  //==== Regions

  vector<TString> regions = {
    "HNWR_SingleElectron_Resolved_SR",
    "HNWR_SingleMuon_Resolved_SR",
    "HNWR_SingleElectron_Boosted_SR",
    "HNWR_SingleMuon_Boosted_SR",
    "HNWR_SingleElectron_Resolved_DYCR",
    "HNWR_SingleMuon_Resolved_DYCR",
    "HNWR_SingleElectron_Boosted_DYCR",
    "HNWR_SingleMuon_Boosted_DYCR",
  };
  vector<TString> regionaliases = {
    "ee Resolved SR",
    "#mu#mu Resolved SR",
    "ee Boosted SR",
    "#mu#mu Boosted SR",
    "ee Resolved DY CR",
    "#mu#mu Resolved DY CR",
    "ee Boosted DY CR",
    "#mu#mu Boosted DY CR",
  };

  //==== Variables

  vector<TString> vars = {
    "WRCand_Mass",
    "ZCand_Pt", "ZCand_Mass",
    "Jet_Size",
    "HT",
  };

  vector<TString> xtitles = {
    "m(W_{R}) (GeV)",
    "p_{T} of dilepton (GeV)", "m(ll) (GeV)", 
    "# of jets",
    "H_{T} (GeV)",
  };

  vector<int> rebins = {
    50,
    20, 20,
    1,
    50,
  };

  for(unsigned int it_Incl=0; it_Incl<samples_Incl.size(); it_Incl++){

    TString sample_Incl = samples_Incl.at(it_Incl);
    vector<TString> list_Binned = lists_Binned.at(it_Incl);
    vector<Color_t> color_Binned = colors_Binned.at(it_Incl);

    TFile *file_Incl = new TFile(base_filepath+"/HNWRAnalyzer_SkimTree_LRSMHighPt_"+sample_Incl+".root");
    vector<TFile *> files_Binned;
    for(unsigned int it_Binned=0; it_Binned<list_Binned.size(); it_Binned++){
      TFile *file_Binned = new TFile(base_filepath+"/HNWRAnalyzer_SkimTree_LRSMHighPt_"+list_Binned.at(it_Binned)+".root");;
      files_Binned.push_back( file_Binned );
    }

    for(unsigned it_region=0; it_region<regions.size(); it_region++){

      TString region = regions.at(it_region);
      TString regionalias = regionaliases.at(it_region);

      for(unsigned int it_var=0; it_var<vars.size(); it_var++){

        TString var = vars.at(it_var);

        int nrebin = rebins.at(it_var);

        TH1D *hist_Incl = (TH1D *)file_Incl->Get(region+"/"+var+"_"+region);
        if(!hist_Incl){
          //cout << region+"/"+var+"_"+region << endl;
          continue;
        }
        hist_Incl->Rebin(nrebin);

        TH1D *hist_Binned = NULL;
        THStack *stack_Binned = new THStack("stack_Binned", "");
        for(unsigned int it_Binned=0; it_Binned<list_Binned.size(); it_Binned++){
          TH1D *this_hist_Binned = (TH1D *)files_Binned.at(it_Binned)->Get(region+"/"+var+"_"+region);
          if(!this_hist_Binned){
            cout << "@@@@ [No Hist] " << list_Binned.at(it_Binned) << " : " << region << endl;
            continue;
          }
          this_hist_Binned->SetFillColor(color_Binned.at(it_Binned));
          this_hist_Binned->SetLineColor(color_Binned.at(it_Binned));
          this_hist_Binned->SetLineWidth(0);

          this_hist_Binned->Rebin(nrebin);
          stack_Binned->Add(this_hist_Binned);

          if(!hist_Binned) hist_Binned = (TH1D *)this_hist_Binned->Clone();
          else hist_Binned->Add(this_hist_Binned);
        }

        ObsPredComp *m_obj = new ObsPredComp();;
        m_obj->hist_Obs = hist_Incl;
        m_obj->hist_Pred = hist_Binned;
        //m_obj->stack_Pred = stack_Binned;
        m_obj->alias_Obs = "Inclusive";
        m_obj->alias_Pred = "Binned";
        m_obj->x_title = var;
        m_obj->TotalLumi = TotalLumi;
        TString this_outputdir = base_plotpath+SetNames.at(it_Incl)+"/";
        gSystem->mkdir(this_outputdir,kTRUE);

        if(var=="ZCand_Pt"){
          m_obj->SetXRange(0,500);
        }

        m_obj->Draw();

        m_obj->c_comp->cd();
        TString str_channel = regionalias;
        TLatex channelname;
        channelname.SetNDC();
        channelname.SetTextSize(0.037);
        channelname.DrawLatex(0.2, 0.88, str_channel);

        m_obj->Save(this_outputdir+region+"_"+var);
        
      }

    }

  }
  

}
