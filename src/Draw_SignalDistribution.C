#include "canvas_margin.h"
#include "mylib.h"
#include "LRSMSignalInfo.h"

void Draw_SignalDistribution(int xxx=0){

  TString bkgdsample_prompt = "DYJets_MG_HT_Reweighted";
  TString bkgdsample_fake = "TTLX_powheg";

  TString Year = "2016";

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString region = "Resolved_SR";
  if(xxx==0){}
  else if(xxx==1){
    region = "Boosted_SR";
  }
  else{
    return;
  }

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/SignalDistribution/"+Year+"/"+region+"/";

  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  //==== WR mass

  LRSMSignalInfo lrsminfo;
  lrsminfo.GetMassMapsSigDist();

  vector<TString> channels = {
    "MuMu",
    "EE",
  };

  vector<Color_t> colors = {
    kRed, kRed+2, kOrange, kOrange+2, kGreen, kGreen+2, kBlue, kBlue+2, kViolet, kViolet+2, kBlack, kCyan, kGray,
  };

  TFile *file_bkg_prompt = new TFile(base_filepath+"/HNWRAnalyzer_SkimTree_LRSMHighPt_"+bkgdsample_prompt+".root");
  TFile *file_bkg_fake = new TFile(base_filepath+"/HNWRAnalyzer_SkimTree_LRSMHighPt_"+bkgdsample_fake+".root");

  TString this_plotpath = base_plotpath+"/";
  gSystem->mkdir(this_plotpath, kTRUE);

  for(unsigned int it_channel=0; it_channel<channels.size(); it_channel++){

    TString channel = channels.at(it_channel);

    TString Suffix = "SingleElectron";
    if(channel=="MuMu") Suffix = "SingleMuon";

    vector<TString> vars, xtitles;

    vars = {
      "Lepton_0_Pt", "Lepton_0_Eta", "Lepton_0_RelIso", "Lepton_0_TrkRelIso",
      "Lepton_1_Pt", "Lepton_1_Eta", "Lepton_1_RelIso", "Lepton_1_TrkRelIso",
      "HNFatJet_SDMass", "HNFatJet_Mass",
      "ZCand_Mass",
      "WRCand_Mass", "WRCand_Pt",
      "NCand_Mass", "NCand_Pt",
      "MET", 
      "NBJets",
    };

    xtitles = {
      "Leading lepton p_{T} (GeV)", "Leading lepton #eta", "Leading lepton PFRelIso", "Leading lepton TrkRelIso",
      "Subleading lepton p_{T} (GeV)", "Subleading lepton #eta", "Subleading lepton PFRelIso", "Subleading lepton TrkRelIso",
      "Away AK8 jet SD Mass", "Away AK8 jet Mass",
      "m_{ll} (GeV)",
      "m_{WR} (GeV)", "p_{T} of WR (GeV)",
      "m_{N} (GeV)", "p_{T} of N (GeV)",
      "MET (GeV)",
      "Number of b-tagged jet",
    };

    for(unsigned int it_var=0; it_var<vars.size(); it_var++){

      TString var = vars.at(it_var);
      TString histname = "HNWR_"+Suffix+"_"+region+"/"+var+"_HNWR_"+Suffix+"_"+region;

      double old_width = 600;
      double new_width = 750;
      double height = 600;

      bool MakeWideCanvas = false;

      TCanvas *c1 = new TCanvas("c1", "", new_width, height);
      canvas_margin(c1);

      double right_margin = (0.05*old_width+new_width-old_width)/new_width;
      double left_margin = 0.16*(old_width/new_width);
      if(MakeWideCanvas){
        c1->SetLeftMargin( 0.30 );
        right_margin = 0.25;
        left_margin = 0.09;
      }

      c1->SetRightMargin( right_margin );
      c1->SetLeftMargin( left_margin );

      //cout << right_margin << endl;

      c1->Draw();
      c1->cd();

      int n_rebin = 1;
      double x_max = 1000.;
      double x_min = 0.;
      if(var.Contains("Pt")){
        x_min = 10.;
        x_max = 3000.;
        n_rebin = 50;
      }
      if(var.Contains("WRCand_Pt")){
        x_min = 0.;
        x_max = 3000.;
        n_rebin = 50;
      }
      if(var.Contains("NCand_Pt")){
        x_min = 0.;
        x_max = 3000.;
        n_rebin = 10;
      }
      if(var.Contains("WRCand_Mass")){
        x_max = 8000.;
        n_rebin = 10;
      }
      if(var.Contains("ZCand_Mass")){
        x_max = 500.;
        n_rebin = 10;
      }
      if(var.Contains("NCand_Mass")){
        x_max = 1000.;
        n_rebin = 2;
      }
      if(var.Contains("HNFatJet_Mass")){
        x_max = 1000.;
        n_rebin = 20;
      }
      if(var.Contains("HNFatJet_SDMass")){
        x_max = 1000.;
        n_rebin = 20;
      }
      if(var.Contains("MET")){
        x_max = 1000.;
        n_rebin = 10;
      }
      if(var.Contains("Size")){
        x_max = 5.;
        n_rebin = 1;
        if(var.Contains("FatJet")){
          x_max = 10.;
          n_rebin = 1;
        }
      }
      if(var.Contains("Eta")){
        x_max = 3.;
        n_rebin = 3;
      }
      if(var.Contains("Iso")){
        x_max = 1.;
        n_rebin = 1;
      }
      if(var.Contains("valid")){
        x_max = 30.;
        n_rebin = 1;
        if(var.Contains("validHits")){
          x_max = 80.;
          n_rebin = 1;
        }
      }
      if(var.Contains("GlobalChi2")){
        x_max = 30.;
        n_rebin = 1;
      }
      if(var.Contains("Eta")){
        x_min = -3.;
      }
      if(var.Contains("MVA")){
        x_min = -1.;
        x_max = 1.;
        n_rebin = 5;
      }
      if(var.Contains("MissingHits")){
        x_max = 10.;
        n_rebin = 1;
      }
      if(var.Contains("dPhi")){
        x_min = 0.;
        x_max = 4.;
        n_rebin = 1;
      }
      if(var=="NBJets"){
        x_min = 0.;
        x_max = 10.;
        n_rebin = 1;
      }

      bool PassOrFail = false;
      if(var.Contains("GsfCtfScPixChargeConsistency") || var.Contains("PassesConvVeto") ){
        PassOrFail = true;
        x_max = 2.;
        n_rebin = 1;
      }

      bool CheckIsoPass = false;
      if(var.Contains("IsolationPass")){
        CheckIsoPass = true;
        x_max = 10.;
        n_rebin = 1;
      }

      int n_bin = (x_max-x_min)*100.; // 0.01
      if(CheckIsoPass) n_bin = 10;
      if(PassOrFail) n_bin = 2;

      //cout << var << " : x_min = " << x_min << ", x_max = " << x_max << ", rebin = " << n_rebin << endl;

      TH1D *hist_dummy = new TH1D("hist_dummy", "", n_bin, x_min, x_max);
      hist_dummy->Draw("hist");
      hist_axis(hist_dummy);

      hist_dummy->GetYaxis()->SetLabelSize(0.04);
      hist_dummy->GetYaxis()->SetTitleSize(0.06);
      hist_dummy->GetYaxis()->SetTitleOffset(0.92);
      if(MakeWideCanvas){
        hist_dummy->GetYaxis()->SetTitleOffset(0.70);
      }

      hist_dummy->GetYaxis()->SetRangeUser(0., 1.1);


      bool XAxisLog = false;
      if(var.Contains("Pt")){
        hist_dummy->GetXaxis()->SetRangeUser(10., x_max);
        c1->SetLogx();
      }
/*
      bool YAxisLog = false;
      if(var.Contains("Iso")&&var.Contains("Rel")){
        YAxisLog = true;
        hist_dummy->GetYaxis()->SetRangeUser(0.001, 2.);
        hist_dummy->GetXaxis()->SetRangeUser(0.001, 2.);
        c1->SetLogy();
      }
*/
      if(var.Contains("DeltaR")){
        hist_dummy->GetXaxis()->SetRangeUser(0., 5.0);
        n_rebin = 1;
      }

      hist_dummy->Rebin(n_rebin);
      hist_dummy->GetYaxis()->SetTitle("Events");
      hist_dummy->GetXaxis()->SetTitle(xtitles.at(it_var));


      if(CheckIsoPass){

        hist_dummy->GetXaxis()->SetRangeUser(0, 8);
        if(channel=="ElEl") hist_dummy->GetXaxis()->SetRangeUser(0,9);

        hist_dummy->GetYaxis()->SetTitle("Isolation Efficiency");
        hist_dummy->GetXaxis()->SetBinLabel(1, "None");
        hist_dummy->GetXaxis()->SetBinLabel(2, "RelIso<0.15");
        hist_dummy->GetXaxis()->SetBinLabel(3, "MiniRelIso<0.15");
        hist_dummy->GetXaxis()->SetBinLabel(4, "MiniRelIso<0.40");
        hist_dummy->GetXaxis()->SetBinLabel(5, "MultiIso Loose");
        hist_dummy->GetXaxis()->SetBinLabel(6, "MiniRelIso<0.16");
        if(channel=="ElEl") hist_dummy->GetXaxis()->SetBinLabel(6, "MiniRelIso<0.12");
        hist_dummy->GetXaxis()->SetBinLabel(7, "MultiIso Tight");
        hist_dummy->GetXaxis()->SetBinLabel(8, "SUSY");
        if(channel=="ElEl") hist_dummy->GetXaxis()->SetBinLabel(9, "HeepID");


      }
      if(PassOrFail){
        hist_dummy->GetXaxis()->SetBinLabel(1, "Fail");
        hist_dummy->GetXaxis()->SetBinLabel(2, "Pass");
      }

      TLegend *lg = new TLegend(0.77, 0.15, 0.98, 0.95);
      lg->SetBorderSize(0);
      lg->SetFillStyle(0);

      double this_ymax = -999;

      //==== Background
/*
      TH1D *hist_bkg_prompt = (TH1D *)file_bkg_prompt->Get(histname);
      if(hist_bkg_prompt){
        hist_bkg_prompt->Rebin(n_rebin);
        if(CheckIsoPass) hist_bkg_prompt->Scale(1./hist_bkg_prompt->GetBinContent(1));
        else if(PassOrFail) hist_bkg_prompt->Scale(1./hist_bkg_prompt->GetEntries());
        else hist_bkg_prompt->Scale(1./hist_bkg_prompt->GetEntries());
        hist_bkg_prompt->SetFillColorAlpha(kCyan,0.50);
        hist_bkg_prompt->SetLineWidth(0);
        hist_bkg_prompt->SetLineColor(0);
        hist_bkg_prompt->Draw("histsame");
        lg->AddEntry(hist_bkg_prompt, bkgdsample_prompt+" (Prompt)", "f");
        this_ymax = max(this_ymax,GetMaximum(hist_bkg_prompt));
      }
*/
      TH1D *hist_bkg_fake = (TH1D *)file_bkg_fake->Get(histname);
      if(hist_bkg_fake){
        hist_bkg_fake->Rebin(n_rebin);
        if(CheckIsoPass) hist_bkg_fake->Scale(1./hist_bkg_fake->GetBinContent(1));
        else if(PassOrFail) hist_bkg_fake->Scale(1./hist_bkg_fake->GetEntries());
        else hist_bkg_fake->Scale(1./hist_bkg_fake->Integral());
        hist_bkg_fake->SetFillColorAlpha(kGray,0.50);
        hist_bkg_fake->SetLineWidth(0);
        hist_bkg_fake->SetLineColor(0);
        hist_bkg_fake->Draw("histsame");
        lg->AddEntry(hist_bkg_fake, bkgdsample_fake+" (Fake)", "f");
        this_ymax = max(this_ymax,GetMaximum(hist_bkg_fake));
      }


      int it_WR = -1;
      for(map< double, vector<double> >::iterator it=lrsminfo.maps_WR_to_N.begin(); it!=lrsminfo.maps_WR_to_N.end(); it++){

        it_WR++;
        if(it_WR>=colors.size()) break;

        double m_WR = it->first;
        vector<double> this_m_Ns = it->second;

        for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

          double m_N = this_m_Ns.at(it_N);

          //cout << "  mN = " << hnmass << endl;
          LRSMSignalInfo this_lrsminfo;
          this_lrsminfo.mass_WR = m_WR;
          this_lrsminfo.mass_N = m_N;
          this_lrsminfo.generator = "MGFastSim";
          this_lrsminfo.prod_channel = "SchWR";
          this_lrsminfo.lep_channel = channel;
          this_lrsminfo.SetNames();

          TString filename = this_lrsminfo.GetFileName();
          TFile *file = new TFile(base_filepath+"/Signal/HNWRAnalyzer_"+filename+".root");
          TH1D *hist = (TH1D *)file->Get(histname);

          if(!hist){
            file->Close();
            continue;
          }

          hist->Rebin(n_rebin);
          hist->SetLineColor(colors.at(it_WR));
          hist->SetLineWidth(2);
          hist->SetLineStyle(it_N+1);
          double this_scale = hist->Integral();
          if(CheckIsoPass) this_scale = hist->GetBinContent(1);
          else if(PassOrFail) this_scale = hist->Integral();
          hist->Scale(1./this_scale);
          
          hist->Draw("histsame");
          //cout << "  mN = " << hnmass << endl;

          this_ymax = max(this_ymax,GetMaximum(hist));

          lg->AddEntry(hist, this_lrsminfo.GetLegendAlias(), "l");

          file->Close();

        }

      }

      TLatex latex_CMSPriliminary, latex_Lumi;
      latex_CMSPriliminary.SetNDC();
      latex_Lumi.SetNDC();
      latex_CMSPriliminary.SetTextSize(0.035);
      //latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
      latex_Lumi.SetTextSize(0.035);
      //latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");

      lg->Draw();

      hist_dummy->Draw("axissame");

      double y_max_scale = 1.2;
      //if(YAxisLog) y_max_scale = 2.;
      hist_dummy->GetYaxis()->SetRangeUser(0.001, y_max_scale*this_ymax); //FIXME use getymin?

      c1->SaveAs(this_plotpath+"/HN"+channel+"_"+var+".pdf");
      c1->SaveAs(this_plotpath+"/HN"+channel+"_"+var+".png");


      c1->Close();

    } // END Loop variable

  } // END Loop channel

}




