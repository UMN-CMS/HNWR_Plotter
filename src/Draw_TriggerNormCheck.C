#include "canvas_margin.h"
#include "mylib.h"

void Draw_TriggerNormCheck(){

  int Year = 2017;

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/TriggerNormCheck/"+TString::Itoa(Year,10)+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/TriggerNormCheck/"+TString::Itoa(Year,10)+"/";

  vector<TString> PDs = {
    "SingleMuon",
    "DoubleMuon",
    "SingleElectron",
    //"SinglePhoton",
  };

  vector< vector<TString> > triggerlists = {
    {"HLT_Mu20_v", "HLT_Mu27_v", "HLT_Mu50_v"},
    {"HLT_Mu8_v", "HLT_Mu17_v"},
    {"HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v", "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v", "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"},
    //{"HLT_Photon25_v", "HLT_Photon33_v", "HLT_Photon50_v", "HLT_Photon75_v", "HLT_Photon90_v", "HLT_Photon120_v", "HLT_Photon150_v", "HLT_Photon175_v", "HLT_Photon200_v"},
  };

  vector<TString> samples = {
    "DYJets", "WJets_MG",
  };
  vector<TString> aliases = {
    "DY", "W",
  };
  vector<Color_t> colors = {
    kYellow, kGreen,
  };

  vector<TString> vars = {
    "Z_CR_Z_Mass", "Z_CR_Lepton_0_Pt", "Z_CR_Lepton_1_Pt",
  };
  vector<TString> xtitles = {
    "m(ll) (GeV)", "Leading lepton p_{T} (GeV)", "Subleading lepton p_{T} (GeV)",
  };

  double x_1[2], y_1[2];
  x_1[0] = 5000;  y_1[0] = 1;
  x_1[1] = -5000;  y_1[1] = 1;
  TGraph *g1 = new TGraph(2, x_1, y_1);

  for(unsigned int it_PD=0; it_PD<PDs.size(); it_PD++){

    TString PD = PDs.at(it_PD);
    vector<TString> triggerlist = triggerlists.at(it_PD);

    TFile *file_DATA = new TFile(base_filepath+"/TriggerNormCheck_data_"+PD+".root");

    TString LepFlav = "Electron";
    if(PD=="SingleMuon" || PD=="DoubleMuon") LepFlav = "Muon";

    TString this_plotdir = base_plotpath+"/"+PD+"/";
    gSystem->mkdir(this_plotdir, kTRUE);

    for(unsigned int it_trig=0; it_trig<triggerlist.size(); it_trig++){

      TString trig = triggerlist.at(it_trig);

      for(unsigned int it_var=0; it_var<vars.size(); it_var++){

        TString var = vars.at(it_var);
        TString xtitle = xtitles.at(it_var);


        TCanvas *c_norm = new TCanvas("c_norm", "", 600, 600);

        TPad *c_norm_up = new TPad("c_norm_up", "", 0, 0.25, 1, 1);
        TPad *c_norm_down = new TPad("c_norm_down", "", 0, 0, 1, 0.25);
        canvas_margin(c_norm, c_norm_up, c_norm_down);

        c_norm_up->Draw();
        c_norm_down->Draw();
        c_norm_up->cd();

        TLegend *lg_Norm = new TLegend(0.7, 0.65, 0.95, 0.90);
        lg_Norm->SetBorderSize(0);
        lg_Norm->SetFillStyle(0);

        int x_nbin = 40;
        double x_min = 70.;
        double x_max = 110.;

        if(var.Contains("_Pt")){
          x_nbin = 20;
          x_min = 0.;
          x_max = 200.;
        }

        TH1D *dummy = new TH1D("dummy", "", x_nbin, x_min, x_max);
        dummy->Draw("hist");
        dummy->GetYaxis()->SetTitle("Events");

        TString this_histname = LepFlav+"_"+PD+"/"+PD+"_TriggerNorm_"+trig+"_"+var;

        TH1D *hist_DATA = (TH1D *)file_DATA->Get(this_histname);
        lg_Norm->AddEntry(hist_DATA, "Data", "pe");

        for(unsigned int it_sample=0; it_sample<samples.size(); it_sample++){
          int ri = samples.size()-1-it_sample;
          TH1D *histlg = new TH1D("dummy_"+samples.at(ri),"",1, 0., 1.);
          histlg->SetFillColor(colors.at(ri));
          histlg->SetLineColor(colors.at(ri));
          lg_Norm->AddEntry(histlg, aliases.at(ri) ,"f");
        }

        THStack *stack_bkgd = new THStack("stack_bkgd", "");
        for(unsigned int it_sample=0; it_sample<samples.size(); it_sample++){
          TString sample = samples.at(it_sample);
          Color_t color = colors.at(it_sample);
          TFile *file = new TFile(base_filepath+"/TriggerNormCheck_"+sample+".root");
          TH1D *hist = (TH1D *)file->Get(this_histname);
          if(hist){
            hist->SetFillColor(color);
            hist->SetLineColor(color);
            stack_bkgd->Add(hist);
          }
        }
        stack_bkgd->Draw("histsame");
        TH1D *hist_bkgd = (TH1D *)stack_bkgd->GetStack()->Last();

        double y_max = max( GetMaximum(hist_bkgd), GetMaximum(hist_DATA) );
        dummy->GetYaxis()->SetRangeUser(1.,5.*y_max);
        c_norm_up->SetLogy();

        hist_DATA->SetMarkerStyle(20);
        hist_DATA->SetMarkerSize(1.6);
        hist_DATA->SetMarkerColor(kBlack);
        hist_DATA->SetLineColor(kBlack);
        hist_DATA->Draw("pesame");
        lg_Norm->Draw();

        c_norm_down->cd();

        TH1D *hist_ratio = (TH1D *)hist_DATA->Clone();
        hist_ratio->Divide(hist_bkgd);
        hist_ratio->SetLineWidth(2.0);
        hist_ratio->SetMarkerSize(0.);
        hist_ratio->SetLineColor(kBlack);
        hist_ratio->Draw("p");
        hist_ratio->GetYaxis()->SetRangeUser(0,2.0);
        hist_ratio->SetNdivisions(504,"Y");
        hist_ratio->GetYaxis()->SetRangeUser(0,1.9);
        hist_ratio->GetXaxis()->SetTitle(xtitle);

        hist_axis(dummy, hist_ratio);

        g1->Draw("same");

        if(var=="Z_CR_Z_Mass"){
          cout << PD << "\t" << trig << "\t" << hist_DATA->Integral() / hist_bkgd->Integral() << endl;
        }

        c_norm->SaveAs(this_plotdir+trig+"_"+var+".pdf");
        c_norm->Close();

      }
      

    }

  }

}
