#include "canvas_margin.h"
#include "mylib.h"

void Get_EMuRatio(){

  //gErrorIgnoreLevel = kFatal;

  setTDRStyle();

  TString Year = "2016";

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/EMuRatio/"+Year+"/";

  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
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
      "TT_powheg",
    };

    asym_samples = {

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
    100,
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
  };

  //==== First, obtain the ee,mm / em ratio from MC

  for(unsigned int it_sym_sample=0; it_sym_sample<sym_samples.size(); it_sym_sample++){

    TString sym_sample = sym_samples.at(it_sym_sample);

    TFile *file = new TFile(base_filepath+"/HNWRAnalyzer_"+sym_sample+".root");

    vector< vector<double> > ratios_for_each_SR;

    for(int it_SR=0; it_SR<3; it_SR++){

      TString SR = "Resolved";
      if(it_SR==1) SR = "Boosted_elFatJet";
      if(it_SR==2) SR = "Boosted_muFatJet";

      for(unsigned int it_var=0; it_var<vars.size(); it_var++){

        TString var = vars.at(it_var);
        TString xtitle = xtitles.at(it_var);
        int rebin = rebins.at(it_var);

        TString region_ee = "HNWR_SingleElectron_Resolved_SR";
        TString region_mm = "HNWR_SingleMuon_Resolved_SR";
        TString region_em = "HNWR_EMu_Resolved_SR";

        if(it_SR==1){
          region_ee = "HNWR_SingleElectron_Boosted_SR"; // isolated el + elJet
          region_mm = "HNWR_SingleElectron_Boosted_SR"; // dummy
          region_em = "HNWR_SingleMuon_EMu_Boosted_CR"; // isolated mu + elJet
        }
        if(it_SR==2){
          region_ee = "HNWR_SingleMuon_Boosted_SR"; // dummy
          region_mm = "HNWR_SingleMuon_Boosted_SR"; // isolated mu + muJet
          region_em = "HNWR_SingleElectron_EMu_Boosted_CR"; // isolated el + elJet
        }

        TH1D *hist_ee = (TH1D *)file->Get(region_ee+"/"+var+"_"+region_ee);
        TH1D *hist_mm = (TH1D *)file->Get(region_mm+"/"+var+"_"+region_mm);
        TH1D *hist_em = (TH1D *)file->Get(region_em+"/"+var+"_"+region_em);

        if(rebin>0){
          hist_ee->Rebin(rebin);
          hist_mm->Rebin(rebin);
          hist_em->Rebin(rebin);
        }
        else{

          if(var=="WRCand_Mass"){

          }

        }

        hist_ee->Divide(hist_em);
        hist_mm->Divide(hist_em);

        TCanvas *c1 = new TCanvas("c1", "", 600, 600);
        canvas_margin(c1);
        c1->cd();

        hist_ee->SetLineColor(kRed);
        hist_mm->SetLineColor(kBlue);

        hist_ee->Draw("histe1");
        hist_mm->Draw("histsamee1");

        hist_ee->GetYaxis()->SetRangeUser(0., 1.0);
        if(it_SR==1) hist_ee->GetYaxis()->SetRangeUser(0., 2.0);
        if(it_SR==2) hist_ee->GetYaxis()->SetRangeUser(0., 4.0);
        hist_ee->GetXaxis()->SetTitle(xtitle);

        c1->SaveAs(base_plotpath+"/Ratios_"+SR+"_"+var+"_"+sym_sample+".pdf");
        c1->SaveAs(base_plotpath+"/Ratios_"+SR+"_"+var+"_"+sym_sample+".png");

        if(var=="NEvent"){
          cout << sym_sample << "\t" << SR << "\t" << hist_ee->GetBinContent(1) << "\t" << hist_mm->GetBinContent(1) << endl;

          vector<double> ratios = { hist_ee->GetBinContent(1), hist_mm->GetBinContent(1) };
          ratios_for_each_SR.push_back( ratios );

        }

        c1->Close();

      } // END variable loop


    }

    //==== Ratio obtained with this sym_sample
    //==== Now make shape

    TFile *outfile = new TFile(base_filepath+"/HNWRAnalyzer_EMuMethod_"+sym_sample+".root","RECREATE");

    for(int it_SR=0; it_SR<3; it_SR++){

      TString SR = "Resolved";
      if(it_SR==1) SR = "Boosted_elFatJet";
      if(it_SR==2) SR = "Boosted_muFatJet";

      vector<double> ratios = ratios_for_each_SR.at(it_SR);

      cout << "@@@@ Making " << SR << " plots.." << endl;

      TString filename_DataEMu = "HNWRAnalyzer_data_SingleMuon.root";
      TString region_ee = "HNWR_SingleElectron_Resolved_SR";
      TString region_mm = "HNWR_SingleMuon_Resolved_SR";
      TString region_em = "HNWR_EMu_Resolved_SR";

      if(it_SR==1){
        filename_DataEMu = "HNWRAnalyzer_data_SingleMuon.root";
        region_ee = "HNWR_SingleElectron_Boosted_SR"; // isolated el + elJet
        region_mm = "HNWR_SingleElectron_Boosted_SR"; // dummy
        region_em = "HNWR_SingleMuon_EMu_Boosted_CR"; // isolated mu + elJet
      }
      if(it_SR==2){
        filename_DataEMu = "HNWRAnalyzer_data_SingleElectron.root";
        region_ee = "HNWR_SingleMuon_Boosted_SR"; // dummy
        region_mm = "HNWR_SingleMuon_Boosted_SR"; // isolated mu + muJet
        region_em = "HNWR_SingleElectron_EMu_Boosted_CR"; // isolated el + elJet
      }

      TFile *file_EMu = new TFile(base_filepath+"/"+filename_DataEMu);
      if(it_SR==0){
        outfile->mkdir(region_ee);
        outfile->mkdir(region_mm);
      }

      for(unsigned it_var=0; it_var<vars_to_draw.size(); it_var++){

        TString var = vars_to_draw.at(it_var);

        TH1D *hist_EMu = (TH1D *)file_EMu->Get(region_em+"/"+var+"_"+region_em);
        if(!hist_EMu) continue;

        if(it_SR==0){
          TH1D *hist_EE = (TH1D *)hist_EMu->Clone();
          TH1D *hist_MM = (TH1D *)hist_EMu->Clone();

          hist_EE->Scale(ratios.at(0));
          hist_EE->SetName(var+"_"+region_ee);
          outfile->cd(region_ee);
          hist_EE->Write();
          outfile->cd();

          hist_MM->Scale(ratios.at(1));
          hist_MM->SetName(var+"_"+region_mm);
          outfile->cd(region_mm);
          hist_MM->Write();
          outfile->cd();

        }
        if(it_SR==1){
          TH1D *hist_EE = (TH1D *)hist_EMu->Clone();

          hist_EE->Scale(ratios.at(0));
          hist_EE->SetName(var+"_"+region_ee);
          outfile->cd(region_ee);
          hist_EE->Write();
          outfile->cd();
       
        }
        if(it_SR==2){
          TH1D *hist_MM = (TH1D *)hist_EMu->Clone();

          hist_MM->Scale(ratios.at(1));
          hist_MM->SetName(var+"_"+region_mm);
          outfile->cd(region_mm);
          hist_MM->Write();
          outfile->cd();
       
        }

      } // END vars_to_draw

    } // END Loop SR

    outfile->Close();

  } // END sys_sample loop




}
