#include "canvas_margin.h"
#include "mylib.h"
#include "ObsPredComp.h"

void Get_EMuRatio(int xxx=2016){

  TString filename_prefix = "HNWRAnalyzer_SkimTree_LRSMHighPt_";

  TString WhichRegion = "SR";
  TString WhichRegionHelper = "CR";
  if(xxx<0){
    WhichRegion = "LowWRCR";
    WhichRegionHelper = "LowWRCR";
  }


  gErrorIgnoreLevel = kFatal;

  bool DrawCompPlot = true;

  setTDRStyle();

  TString Year = "2016";
  TString TotalLumi = "35.92 fb^{-1} (13 TeV)";
  if(abs(xxx)==2017){
    Year = "2017";
    TotalLumi = "41.53 fb^{-1} (13 TeV)";
  }
  if(abs(xxx)==2018){
    Year = "2018";
    TotalLumi = "59.74 fb^{-1} (13 TeV)";
  }

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/EMuRatio/"+Year+"/";
  if(xxx<0) base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/EMuRatio_LowWRCR/"+Year+"/";

  if( !gSystem->mkdir(base_plotpath+"/Comparison/", kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  //==== MCFR Samples

  vector<TString> sym_samples, asym_samples;

  if(Year=="2016"){

    sym_samples = {
      "TTLL_powheg",
      "TTLJ_powheg",
      "TTLX_powheg",
    };

    asym_samples = {
"DYJets",
"DYJets10to50",
      "WJets_MG",
      "WZ_pythia", "ZZ_pythia", "WW_pythia",
      "SingleTop_sch_Lep", "SingleTop_tW_antitop_NoFullyHad", "SingleTop_tW_top_NoFullyHad", "SingleTop_tch_antitop_Incl", "SingleTop_tch_top_Incl",
      "WWW", "WWZ", "WZZ", "ZZZ",
      "ttWToLNu", "ttWToQQ", "ttZ",
    };

  }

  else if(Year=="2017"){

    sym_samples = {
      "TTLL_powheg",
      "TTLJ_powheg",
      "TTLX_powheg",
    };

    asym_samples = {
"DYJets",
"DYJets10to50_MG",
      "WJets_MG",
      "ttW", "ttZ",
      "WZ_pythia", "ZZ_pythia", "WW_pythia",
      "WWW", "WWZ", "WZZ", "ZZZ",
      "SingleTop_sch_Lep", "SingleTop_tW_antitop_NoFullyHad", "SingleTop_tW_top_NoFullyHad" ,"SingleTop_tch_antitop_Incl" ,"SingleTop_tch_top_Incl",
    };

  }

  else if(Year=="2018"){

    sym_samples = {
      "TTLL_powheg",
      "TTLJ_powheg",
      "TTLX_powheg",
    };

    asym_samples = {
"DYJets_MG",
"DYJets10to50_MG",
      "WZ_pythia", "ZZ_pythia", "WW_pythia",
      "WJets_MG",
      "WWW", "WWZ", "WZZ", "ZZZ",
      "SingleTop_sch_Lep", "SingleTop_tW_antitop_Incl", "SingleTop_tW_top_Incl", "SingleTop_tch_antitop_Incl", "SingleTop_tch_top_Incl",
      "ttW", "ttZ",
    };

  }

  //==== Variables

  vector<TString> vars = {
    "NEvent",
    "ZCand_Mass",
    "WRCand_Mass",
  };

  vector<TString> xtitles = {
    "# of events",
    "m(ll) (GeV)",
    "m(W_{R}) (GeV)",
  };

  vector<int> rebins = {
    1,
    100,
    -1,
  };

  vector<TString> vars_to_draw = {

    "NEvent", "nPileUp", "nPV",
    "Lepton_0_Pt", "Lepton_0_Eta", "Lepton_0_TrkRelIso",
    "Lepton_1_Pt", "Lepton_1_Eta", "Lepton_1_TrkRelIso",
    "dPhi_ll",
    "Jet_0_Pt", "Jet_0_Eta",
    "Jet_1_Pt", "Jet_1_Eta",
    "HNFatJet_Pt", "HNFatJet_Eta", "HNFatJet_Mass", "HNFatJet_SDMass",
    "ZCand_Pt", "ZCand_Mass", "dR_ll",
    "MET", "HT",
    "MT",
    "Jet_Size", "NBJets",
    "NCand_Mass", "WRCand_Mass",
    "NCand_Pt", "WRCand_Pt",
    "LSFFatJet_Size",

//"WRCand_Mass"
  };

  TFile *file_TTLL = new TFile(base_filepath+"/"+filename_prefix+"TTLL_powheg.root");
  TFile *file_TTLJ = new TFile(base_filepath+"/"+filename_prefix+"TTLJ_powheg.root");

  //==== First, obtain the ee,mm / em ratio from MC

  TFile *file_Ratios = new TFile(base_plotpath+"/Ratios.root", "RECREATE");

  for(unsigned int it_sym_sample=0; it_sym_sample<sym_samples.size(); it_sym_sample++){

    TString sym_sample = sym_samples.at(it_sym_sample);

    TFile *file = new TFile(base_filepath+"/"+filename_prefix+sym_sample+".root");

    vector< vector<double> > ratios_for_each_SR;

    for(int it_SR=0; it_SR<3; it_SR++){

      TString SR = "Resolved";
      if(it_SR==1) SR = "Boosted_elFatJet";
      if(it_SR==2) SR = "Boosted_muFatJet";

      for(unsigned int it_var=0; it_var<vars.size(); it_var++){

        TString var = vars.at(it_var);
        TString xtitle = xtitles.at(it_var);
        int rebin = rebins.at(it_var);

        TString region_EE = "HNWR_SingleElectron_Resolved_"+WhichRegion;
        TString region_MM = "HNWR_SingleMuon_Resolved_"+WhichRegion;
        TString region_EM = "HNWR_EMu_Resolved_"+WhichRegion;

        if(it_SR==1){
          region_EE = "HNWR_SingleElectron_Boosted_"+WhichRegion; // isolated el + elJet
          region_MM = "HNWR_SingleElectron_Boosted_"+WhichRegion; // dummy
          region_EM = "HNWR_SingleMuon_EMu_Boosted_"+WhichRegionHelper; // isolated mu + elJet
        }
        if(it_SR==2){
          region_EE = "HNWR_SingleMuon_Boosted_"+WhichRegion; // dummy
          region_MM = "HNWR_SingleMuon_Boosted_"+WhichRegion; // isolated mu + muJet
          region_EM = "HNWR_SingleElectron_EMu_Boosted_"+WhichRegionHelper; // isolated el + elJet
        }

        TH1D *hist_EE = (TH1D *)file->Get(region_EE+"/"+var+"_"+region_EE);
        TH1D *hist_MM = (TH1D *)file->Get(region_MM+"/"+var+"_"+region_MM);
        TH1D *hist_EM = (TH1D *)file->Get(region_EM+"/"+var+"_"+region_EM);

        if(rebin>0){
          hist_EE->Rebin(rebin);
          hist_MM->Rebin(rebin);
          hist_EM->Rebin(rebin);
        }
        else{

          if(var=="WRCand_Mass"){
            vector<double> vec_bins;

            if(xxx>0){
              if(it_SR==0) vec_bins = {0, 200, 400, 600, 800, 1000, 1500, 2000, 8000};
              else vec_bins = {0, 200, 400, 600, 800, 1000, 1500, 8000};
            }
            else{
              if(it_SR==0) vec_bins = {0, 100, 200, 300, 400, 500, 600, 700, 800, 1000, 1500, 2000, 8000};
              else vec_bins = {0, 100, 200, 300, 400, 500, 600, 700, 800, 1000, 1500, 8000};
            }


            const int n_bin = vec_bins.size()-1;
            double ptArray[n_bin+1];
            for(int zzz=0;zzz<vec_bins.size();zzz++) ptArray[zzz] = vec_bins.at(zzz);
            hist_EE = (TH1D *)hist_EE->Rebin(n_bin, hist_EE->GetName(), ptArray);
            hist_MM = (TH1D *)hist_MM->Rebin(n_bin, hist_MM->GetName(), ptArray);
            hist_EM = (TH1D *)hist_EM->Rebin(n_bin, hist_EM->GetName(), ptArray);

          }

        }

        hist_EE->Divide(hist_EM);
        hist_MM->Divide(hist_EM);

        TCanvas *c1 = new TCanvas("c1", "", 600, 600);
        canvas_margin(c1);
        c1->cd();

        hist_EE->SetLineColor(kRed);
        hist_MM->SetLineColor(kBlue);
        hist_EE->SetLineWidth(3);
        hist_MM->SetLineWidth(3);

        if(it_SR==0){
          hist_EE->Draw("histe1");
          hist_axis(hist_EE);
          hist_MM->Draw("histsamee1");

          hist_EE->GetYaxis()->SetRangeUser(0., 1.5);
          hist_EE->GetXaxis()->SetTitle(xtitle);
          hist_EE->GetYaxis()->SetTitle("Ratio");
          TLegend *lg = new TLegend(0.6, 0.8, 0.9, 0.9);
          lg->AddEntry(hist_EE, "ee/e#mu", "l");
          lg->AddEntry(hist_MM, "#mu#mu/e#mu", "l");
          lg->Draw();
        }
        else if(it_SR==1){
          hist_EE->Draw("histe1");
          hist_axis(hist_EE);

          hist_EE->GetYaxis()->SetRangeUser(0., 2.0);
          hist_EE->GetXaxis()->SetTitle(xtitle);
          hist_EE->GetYaxis()->SetTitle("Ratio");
          TLegend *lg = new TLegend(0.6, 0.8, 0.9, 0.9);
          lg->AddEntry(hist_EE, "ee/e#mu", "l");
          lg->Draw();
        }
        else if(it_SR==2){
          hist_MM->Draw("histe1");
          hist_axis(hist_MM);
        
          hist_MM->GetYaxis()->SetRangeUser(0., 2.0);
          hist_MM->GetXaxis()->SetTitle(xtitle);
          hist_MM->GetYaxis()->SetTitle("Ratio");
          TLegend *lg = new TLegend(0.6, 0.8, 0.9, 0.9);
          lg->AddEntry(hist_MM, "#mu#mu/e#mu", "l");
          lg->Draw();
        }
        if(xxx<0){
          hist_MM->GetXaxis()->SetRangeUser(0., 1000.);
        }

        c1->SaveAs(base_plotpath+"/Ratios_"+SR+"_"+var+"_"+sym_sample+".pdf");
        c1->SaveAs(base_plotpath+"/Ratios_"+SR+"_"+var+"_"+sym_sample+".png");

        file_Ratios->cd();
        hist_EE->SetName("EE_Ratios_"+SR+"_"+var+"_"+sym_sample);
        hist_MM->SetName("MM_Ratios_"+SR+"_"+var+"_"+sym_sample);
        hist_EE->Write();
        hist_MM->Write();

/*
        if(var=="NEvent"){
          cout << sym_sample << "\t" << SR << "\t" << hist_EE->GetBinContent(1) << "\t" << hist_MM->GetBinContent(1) << endl;

          vector<double> ratios = { hist_EE->GetBinContent(1), hist_MM->GetBinContent(1) };
          ratios_for_each_SR.push_back( ratios );

        }
*/

        if(var=="WRCand_Mass"){

          //==== EE
          hist_EE->Fit("pol0");
          double fitted_ratio_EE = hist_EE->GetFunction("pol0")->GetParameter(0);
          //==== MM
          hist_MM->Fit("pol0");
          double fitted_ratio_MM = hist_MM->GetFunction("pol0")->GetParameter(0);

          vector<double> ratios = { fitted_ratio_EE, fitted_ratio_MM };
          cout << sym_sample << "\t" << SR << "\t" << fitted_ratio_EE << "\t" << fitted_ratio_MM << endl;
          ratios_for_each_SR.push_back( ratios );

          //==== TODO For syst


        }

        c1->Close();

      } // END variable loop


    }

    //==== Ratio obtained with this sym_sample
    //==== Now make shape

    TString emurootfilepath = base_filepath+"/"+filename_prefix+"EMuMethod_"+sym_sample+".root";
    if(xxx<0) emurootfilepath = base_filepath+"/"+filename_prefix+"EMuMethod_LowWRCR_"+sym_sample+".root";
    TFile *outfile = new TFile(emurootfilepath,"RECREATE");

    for(int it_SR=0; it_SR<3; it_SR++){

      TString SR = "Resolved";
      if(it_SR==1) SR = "Boosted_elFatJet";
      if(it_SR==2) SR = "Boosted_muFatJet";

      vector<double> ratios = ratios_for_each_SR.at(it_SR);

      cout << "@@@@ Making " << SR << " plots.." << endl;

      TString filename_DataEMu = filename_prefix+"data_SingleMuon.root";
      TString region_EE = "HNWR_SingleElectron_Resolved_"+WhichRegion;
      TString region_MM = "HNWR_SingleMuon_Resolved_"+WhichRegion;
      TString region_EM = "HNWR_EMu_Resolved_"+WhichRegion;

      if(it_SR==1){
        filename_DataEMu = filename_prefix+"data_SingleMuon.root";
        region_EE = "HNWR_SingleElectron_Boosted_"+WhichRegion; // isolated el + elJet
        region_MM = "HNWR_SingleElectron_Boosted_"+WhichRegion; // dummy
        region_EM = "HNWR_SingleMuon_EMu_Boosted_"+WhichRegionHelper; // isolated mu + elJet
      }
      if(it_SR==2){
        filename_DataEMu = filename_prefix+"data_SingleElectron.root";
        region_EE = "HNWR_SingleMuon_Boosted_"+WhichRegion; // dummy
        region_MM = "HNWR_SingleMuon_Boosted_"+WhichRegion; // isolated mu + muJet
        region_EM = "HNWR_SingleElectron_EMu_Boosted_"+WhichRegionHelper; // isolated el + elJet
      }

      TFile *file_EMu = new TFile(base_filepath+"/"+filename_DataEMu);
      if(it_SR==0){
        outfile->mkdir(region_EE);
        outfile->mkdir(region_MM);
      }
      if(it_SR==1){
        outfile->mkdir(region_EE);
      }
      if(it_SR==2){
        outfile->mkdir(region_MM);
      }

      for(unsigned it_var=0; it_var<vars_to_draw.size(); it_var++){

        TString var = vars_to_draw.at(it_var);

        TH1D *hist_EMu = (TH1D *)file_EMu->Get(region_EM+"/"+var+"_"+region_EM);
        if(!hist_EMu) continue;

        int n_rebin = 1;
        double x_min = -9999;
        double x_max = 9999;

        if(var=="WRCand_Mass"){
          n_rebin = 100;
        }

        //==== Subtract non-TT
        for(unsigned int it_asym=0; it_asym<asym_samples.size(); it_asym++){
          TFile *file_asym = new TFile(base_filepath+"/"+filename_prefix+asym_samples.at(it_asym)+".root");
          TH1D *hist_asym = (TH1D *)file_asym->Get(region_EM+"/"+var+"_"+region_EM);
          if(!hist_asym){
            file_asym->Close();
            continue;
          }

          hist_EMu->Add(hist_asym,-1.);
          file_asym->Close();
        }

        if(it_SR==0 || it_SR==1){

          //==== Make comparison plot

          if(!DrawCompPlot) continue;

          //==== 1) EE

          TH1D *hist_Pred_EE = (TH1D *)hist_EMu->Clone();
          hist_Pred_EE->Scale(ratios.at(0));
          hist_Pred_EE->SetName(var+"_"+region_EE);
          outfile->cd();
          outfile->cd(region_EE);
          hist_Pred_EE->Write();
          outfile->cd();

          //==== observed (cloneed from TTLL)
          TH1D *hist_Obs_EE = (TH1D *)file_TTLL->Get(region_EE+"/"+var+"_"+region_EE);
          //==== trkiso etc.. sometimes there is no histogram
          if(hist_Obs_EE){

            //==== Add TTLJ
            TH1D *hist_TTLJ_EE = (TH1D *)file_TTLJ->Get(region_EE+"/"+var+"_"+region_EE);
            if(hist_TTLJ_EE) hist_Obs_EE->Add(hist_TTLJ_EE);

            //==== rebin
            hist_Pred_EE->Rebin(n_rebin);
            hist_Obs_EE->Rebin(n_rebin);

            //==== Draw

            ObsPredComp m_EE;
            m_EE.hist_Obs = hist_Obs_EE;
            m_EE.hist_Pred = hist_Pred_EE;
            m_EE.alias_Obs = "MC";
            m_EE.alias_Pred = "e#mu-sideband";
            m_EE.x_title = var;
            m_EE.Logy = true;
            m_EE.TotalLumi = TotalLumi;
            m_EE.outputpath = base_plotpath+"/Comparison/"+SR+"_"+var+"_"+region_EE+"_"+sym_sample;
            m_EE.Run();

          }
        }

        if(it_SR==0 || it_SR==2){
          //==== 2) MM

          TH1D *hist_Pred_MM = (TH1D *)hist_EMu->Clone();
          hist_Pred_MM->Scale(ratios.at(1));
          hist_Pred_MM->SetName(var+"_"+region_MM);
          outfile->cd(region_MM);
          hist_Pred_MM->Write();
          outfile->cd();

          //==== observed (cloned from TTLL)
          TH1D *hist_Obs_MM = (TH1D *)file_TTLL->Get(region_MM+"/"+var+"_"+region_MM);
          //==== trkiso etc.. sometimes there is no histogram
          if(hist_Obs_MM){

            //==== Add TTLJ
            TH1D *hist_TTLJ_MM = (TH1D *)file_TTLJ->Get(region_MM+"/"+var+"_"+region_MM);
            if(hist_TTLJ_MM) hist_Obs_MM->Add(hist_TTLJ_MM);

            //==== rebin
            hist_Pred_MM->Rebin(n_rebin);
            hist_Obs_MM->Rebin(n_rebin);

            //==== Draw

            ObsPredComp m_MM;
            m_MM.hist_Obs = hist_Obs_MM;
            m_MM.hist_Pred = hist_Pred_MM;
            m_MM.alias_Obs = "MC";
            m_MM.alias_Pred = "e#mu-sideband";
            m_MM.x_title = var;
            m_MM.Logy = true;
            m_MM.TotalLumi = TotalLumi;
            m_MM.outputpath = base_plotpath+"/Comparison/"+SR+"_"+var+"_"+region_MM+"_"+sym_sample;
            m_MM.Run();

          }

        }

      } // END vars_to_draw

    } // END Loop SR

    outfile->Close();

  } // END sys_sample loop

  file_Ratios->Close();




}















