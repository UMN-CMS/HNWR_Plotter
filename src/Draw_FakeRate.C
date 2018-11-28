#include "canvas_margin.h"
#include "mylib.h"

void Draw_FakeRate(){

  bool OnlyNorm = false;
  bool UsePtCone = false;

  setTDRStyle();

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString OutPutSubDir = "181117_RemoveMETMT";

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/CalcFakeRate/"+OutPutSubDir+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/CalcFakeRate/"+OutPutSubDir+"/";

  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  //==== MCFR Samples

  vector<TString> MCFR_samples = {
    "QCD",
    "WJets_MG",
    "TT_powheg",
  };
  vector<TString> MCFR_alias = {
    "QCD",
    "W+Jets",
    "t#bar{t}",
  };
  vector<Color_t> MCFR_sample_colors = {
    kCyan,
    kGreen,
    kRed,
  };

  //==== Prompt MC Samples

  vector<TString> Prompt_samples = {
    "VVV",
    "VV",
    "SingleTop",
    "TT_powheg",
    "DY",
    "WJets_MG", 
  };
  vector<TString> Prompt_alias = {
    "Triboson",
    "Diboson",
    "top",
    "t#bar{t}",
    "DY",
    "W",
  };
  vector<Color_t> Prompt_sample_colors = {
    kCyan,
    kBlue,
    kMagenta,
    kRed,
    kYellow,
    kGreen,
  };

  //==== Variables

  vector<TString> Norm_vars = {
    "W_CR_Lepton_0_Pt", "W_CR_MET", "W_CR_MT",
    "MTcut_W_CR_Lepton_0_Pt", "MTcut_W_CR_MET", "MTcut_W_CR_MT",
    "Z_CR_Lepton_0_Pt", "Z_CR_Lepton_1_Pt", "Z_CR_Z_Mass",
  };
  vector<TString> Norm_xtitles = {
    "Leading lepton p_{T} (GeV)", "#slash{E}_{T}^{miss} (GeV)", "M_{T}(W-tagged Lepton, #slash{E}_{T}^{miss}) (GeV)",
    "Leading lepton p_{T} (GeV)", "#slash{E}_{T}^{miss} (GeV)", "M_{T}(W-tagged Lepton, #slash{E}_{T}^{miss}) (GeV)",
    "Leading lepton p_{T} (GeV)", "Subleading lepton p_{T} (GeV)", "m(OS) (GeV)",
  };
  vector<int> Norm_rebins = {
    10, 10, 10,
    10, 10, 10,
    10, 10, 1,
  };
  vector<int> Norm_xmaxs = {
    500, 500, 500,
    500, 500, 500,
    500, 500, 110,
  };

  //==== FR variables

  vector<TString> vars = {
    "Pt", "PtCone",
    "Eta",
    "RelIso", "MiniRelIso",
    "dXY", "dXYSig", "dZ", "dZSig", "IP3D", "IP3DSig",
    "Chi2", "MVANoIso",
    "TrkRelIso",
    "dPhi", "JetPtOverLeptonPt", "MET", "MT",
  };
  vector<TString> xtitles = {
    "p_{T} (GeV)", "p_{T}^{cone} (GeV)",
    "#eta", 
    "RelIso", "MiniRelIso",
    "|dXY| (cm)", "|dXYSig|", "|dZ| (cm)", "|dZSig|", "IP3D (cm)", "IP3DSig",
    "#chi^{2}", "MVA (No iso)",
    "TrkRelIso",
    "#Delta#phi", "p_{T,jet}/p_{T,Lepton}", "#slash{E}_{T}^{miss} (GeV)", "M_{T}(W-tagged Lepton, #slash{E}_{T}^{miss}) (GeV)",
  };
  vector<int> rebins = {
    50, 50,
    5,
    1, 1,
    10, 1, 10, 1, 10, 1,
    1, 5,
    1,
    1, 1, 1, 1,
  };
  vector<double> x_maxs = {
    2000., 2000.,
    3.,
    1.0, 1.0,
    0.5, 10, 0.5, 10, 0.5, 10,
    50, 1,
    1.0,
    4, 2, 100, 50,
  };

  vector<TString> LeptonFlavours = {
    "Electron",
    "Muon",
  };

  vector<TString> AwayJetMinPts = {"20", "30", "40", "60", "100", "500"};

  double x_1[2], y_1[2];
  x_1[0] = 5000;  y_1[0] = 1;
  x_1[1] = -5000;  y_1[1] = 1;
  TGraph *g1 = new TGraph(2, x_1, y_1);

  TLatex latex_CMSPriliminary, latex_Lumi;
  latex_CMSPriliminary.SetNDC();
  latex_Lumi.SetNDC();
  latex_CMSPriliminary.SetTextSize(0.035);

  latex_Lumi.SetTextSize(0.035);
  latex_Lumi.SetTextFont(42);

  for(unsigned int it_leptonflv=0; it_leptonflv<LeptonFlavours.size(); it_leptonflv++){

    TString Lepton = LeptonFlavours.at(it_leptonflv);

    vector<TString> IDs = {
      "HNWR",
    };

    vector<TString> Triggers;
    if(Lepton=="Electron"){
      Triggers = {"HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v", "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v", "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"};
    }
    if(Lepton=="Muon"){
      Triggers = {"HLT_Mu8_v", "HLT_Mu17_v", "HLT_Mu20_v", "HLT_Mu27_v", "HLT_Mu50_v"};
    }

    //==== Data pt-binning
    //====                                           75 = ptcut
    vector<double>     tmp_ptbinnings = {10,15,20,25,35,40,50,60,100,200,500,1000};
    if(Lepton=="Muon") tmp_ptbinnings = {10,15,20,25,30,40,55,60,100,200,500,1000};

    const int n_ptarray = tmp_ptbinnings.size()-1;
    double ptarray[n_ptarray+1];
    for(int i=0;i<n_ptarray+1;i++) ptarray[i] = tmp_ptbinnings.at(i);

    for(unsigned int it_id=0; it_id<IDs.size(); it_id++){

      TString ID = IDs.at(it_id);

      TString this_dirname = base_plotpath+"/"+Lepton+"_"+ID+"/";
      gSystem->mkdir(this_dirname, kTRUE);

      TString DATAPD = "SingleElectron";
      if(Lepton=="Muon") DATAPD = "Muon";
      TFile *file_DATA = new TFile(base_filepath+"/CalcFakeRate_data_"+DATAPD+".root");

      //==== Trigger Norm Check plots

      for(unsigned int it_trigger=0; it_trigger<Triggers.size(); it_trigger++){

        TString trig = Triggers.at(it_trigger);

        for(unsigned int it_var=0; it_var<Norm_vars.size(); it_var++){

          TString var = Norm_vars.at(it_var);

          if(OnlyNorm){
            if(var!="Z_CR_Z_Mass") continue;
          }

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

          TH1D *hist_DATA = (TH1D *)file_DATA->Get(Lepton+"_"+ID+"/"+ID+"_TriggerNorm_"+trig+"_"+var);
          if(!hist_DATA){
            c_norm->Close();
            delete c_norm;

            continue;
          }
          hist_DATA->Rebin(Norm_rebins.at(it_var));

          lg_Norm->AddEntry(hist_DATA, "Data", "pe");
          for(unsigned int it_sample=0; it_sample<Prompt_samples.size(); it_sample++){
            int ri = Prompt_samples.size()-1-it_sample;
            TH1D *histlg = new TH1D("hist_dummy_"+Prompt_samples.at(ri),"",1, 0., 1.);
            histlg->SetFillColor(Prompt_sample_colors.at(ri));
            histlg->SetLineColor(Prompt_sample_colors.at(ri));
            lg_Norm->AddEntry(histlg, Prompt_alias.at(ri) ,"f");
          }

          TH1D *dummy = (TH1D *)hist_DATA->Clone();
          hist_axis(dummy);
          dummy->GetYaxis()->SetRangeUser(1., GetMaximum(dummy));
          dummy->GetYaxis()->SetTitle("Events");
          if(!( var.Contains("Z_Mass") )) dummy->GetXaxis()->SetRangeUser(0,Norm_xmaxs.at(it_var));
          dummy->SetLineColor(0);
          dummy->Draw("hist");

          //==== Stack MC
          THStack *stack_bkgd = new THStack("stack_bkgd", "");
          for(unsigned int it_sample=0; it_sample<Prompt_samples.size(); it_sample++){
            TFile *filetemp = new TFile(base_filepath+"/CalcFakeRate_"+Prompt_samples.at(it_sample)+".root");
            TH1D *histtemp = (TH1D *)filetemp->Get(Lepton+"_"+ID+"/"+ID+"_TriggerNorm_"+trig+"_"+var);

            if(histtemp){
              histtemp->Rebin(Norm_rebins.at(it_var));
              histtemp->SetFillColor(Prompt_sample_colors.at(it_sample));
              histtemp->SetLineColor(Prompt_sample_colors.at(it_sample));
              stack_bkgd->Add(histtemp);
            }

            filetemp->Close();
            delete filetemp;
          }

          stack_bkgd->Draw("histsame");
          TH1D *hist_bkgd = (TH1D *)stack_bkgd->GetStack()->Last();

          hist_DATA->SetMarkerStyle(20);
          hist_DATA->SetMarkerSize(1.6);
          hist_DATA->SetMarkerColor(kBlack);
          hist_DATA->SetLineColor(kBlack);
          hist_DATA->Draw("psame");

          double y_max = max( GetMaximum(hist_DATA), GetMaximum(hist_bkgd) );
          dummy->GetYaxis()->SetRangeUser(1., 5.*y_max);

          if(OnlyNorm && var=="Z_CR_Z_Mass"){
            double total_DATA = hist_DATA->Integral();
            double total_Exp = hist_bkgd->Integral();
            cout << trig << "\t" << total_DATA/total_Exp << endl;
            c_norm->Close();
            delete c_norm;
            continue;
          }

          if(var.Contains("Z_Mass")){
            cout << "[Trigger Norm] " << ID << "\t" << trig << "\t" << hist_DATA->Integral()/hist_bkgd->Integral() << endl;
          }

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
          hist_ratio->GetXaxis()->SetTitle(Norm_xtitles.at(it_var));
          if(!( var.Contains("Z_Mass") )) hist_ratio->GetXaxis()->SetRangeUser(0,Norm_xmaxs.at(it_var));

          hist_axis(dummy, hist_ratio);

          g1->Draw("same");

          c_norm_up->SetLogy();

          c_norm->cd();
          latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
          latex_Lumi.DrawLatex(0.88, 0.96, "(2017)");
          latex_Lumi.DrawLatex(0.50, 0.96, "#font[62]{"+trig+"}");
          lg_Norm->Draw();

          c_norm->SaveAs(this_dirname+"/TriggerNorm_"+trig+"_"+var+".pdf");
          c_norm->SaveAs(this_dirname+"/TriggerNorm_"+trig+"_"+var+".png");
          c_norm->Close();
        
        }


      }
      if(OnlyNorm) continue;

      //==== Run over single varialbes, and get Fake Rates

      for(unsigned int it_var=0; it_var<vars.size(); it_var++){

        TString var = vars.at(it_var);

        if(Lepton=="Electron"){
          if(var=="Chi2") continue;
          if(var=="TrkRelIso") continue;
        }
        if(Lepton=="Muon"){
          if(var=="MVANoIso") continue;
        }

        //===========================================
        //==== MCFR for current Lepton && ID && VAR
        //===========================================

        //==== For all samples in one plot

        TCanvas *c_MCFR_all = new TCanvas("c_MCFR_all", "", 600, 600);
        canvas_margin(c_MCFR_all);

        double x_max = x_maxs.at(it_var);
        double x_min = 0;
        if(var.Contains("Eta")) x_min = -3.;
        if(var.Contains("MVANoIso")) x_min = -1.;
        double dx = 0.001;
        if(var=="Pt" || var=="PtCone") dx = 1.;
        dx *= double(rebins.at(it_var));

        //cout << var << "\t" << int((x_max-x_min)/dx) << endl;

        TH1D *dummy_all = new TH1D("dummy_all", "", int((x_max-x_min)/dx), x_min, x_max);
        hist_axis(dummy_all);
        dummy_all->GetYaxis()->SetRangeUser(0,1.0);
        dummy_all->GetYaxis()->SetTitle("FR");
        dummy_all->GetXaxis()->SetTitle(xtitles.at(it_var));
        dummy_all->Draw("hist");

        TLegend *lg = new TLegend(0.7, 0.65, 0.95, 0.90);
        lg->SetBorderSize(0);
        lg->SetFillStyle(0);

        bool IsMCFRFilled = false;

        for(unsigned int it_sample=0; it_sample<MCFR_samples.size(); it_sample++){

          TString sample = MCFR_samples.at(it_sample);

          TFile *file_MC = new TFile(base_filepath+"/CalcFakeRate_"+sample+".root");

          TH1D *hist_Den = (TH1D *)file_MC->Get(Lepton+"_"+ID+"/"+Lepton+"_"+ID+"_MCFR_Den_"+var);
          TH1D *hist_Num = (TH1D *)file_MC->Get(Lepton+"_"+ID+"/"+Lepton+"_"+ID+"_MCFR_Num_"+var);

          if(!hist_Den) continue;

          IsMCFRFilled = true;

          if(var=="Pt" || var=="PtCone"){

            hist_Den = (TH1D *)hist_Den->Rebin(n_ptarray, "hnew1", ptarray);
            hist_Num = (TH1D *)hist_Num->Rebin(n_ptarray, "hnew1", ptarray);

          }
          else{
            hist_Den->Rebin(rebins.at(it_var));
            hist_Num->Rebin(rebins.at(it_var));
          }

          //==== Den/Num distributions

          TCanvas *c_Dist = new TCanvas("c_Dist", "", 600, 600);
          canvas_margin(c_Dist);
          c_Dist->cd();

          TH1D *dummy = new TH1D("dummy", "", int((x_max-x_min)/dx), x_min, x_max);
          hist_axis(dummy);
          dummy->GetYaxis()->SetRangeUser(0,1.0);
          dummy->GetYaxis()->SetTitle("FR");
          //c_Dist->SetLogx();
          dummy->GetXaxis()->SetTitle(xtitles.at(it_var));
          dummy->Draw("hist");

          hist_Den->SetLineColor(kBlue);
          hist_Num->SetLineColor(kRed);

          hist_Den->Draw("histsame");
          hist_Num->Draw("histsame");

          double y_max = max( GetMaximum(hist_Den), GetMaximum(hist_Num) )*1.2;
          dummy->GetYaxis()->SetRangeUser(0, y_max);

          c_Dist->SaveAs(this_dirname+"/Dist_"+sample+"_"+var+".pdf");
          c_Dist->SaveAs(this_dirname+"/Dist_"+sample+"_"+var+".png");

          c_Dist->Close();

          //==== Fake Rate

          c_MCFR_all->cd();
          TEfficiency *Eff_FR = new TEfficiency(*hist_Num, *hist_Den);
          TGraphAsymmErrors *gr_FR = Eff_FR->CreateGraph();
          gr_FR->SetMarkerSize(0);
          gr_FR->SetLineWidth(3);
          gr_FR->SetLineColor(MCFR_sample_colors.at(it_sample));
          gr_FR->Draw("lsame");
          lg->AddEntry(gr_FR, MCFR_alias.at(it_sample), "l");

        } // END Loop MCFR sample

        if(IsMCFRFilled){

          c_MCFR_all->cd();
          lg->Draw();

          c_MCFR_all->SaveAs(this_dirname+"/1D_MCFR_AllSamples_"+var+".pdf");
          c_MCFR_all->SaveAs(this_dirname+"/1D_MCFR_AllSamples_"+var+".png");
        }

        c_MCFR_all->Close();

        //=====================
        //==== Now using DATA
        //=====================

        for(unsigned int it_jpt=0; it_jpt<AwayJetMinPts.size(); it_jpt++){

          TString this_AwayJetPt = AwayJetMinPts.at(it_jpt);

          TCanvas *c_FR = new TCanvas("c_FR", "", 600, 600);
          canvas_margin(c_FR);

          TCanvas *c_DATA_Den = new TCanvas("c_DATA_Den", "", 600, 600);
          canvas_margin(c_DATA_Den);
          TH1D *hist_DATA_Den = (TH1D *)file_DATA->Get(Lepton+"_"+ID+"/"+Lepton+"_"+ID+"_DATA_AwayJetPt"+this_AwayJetPt+"_Den_"+var);

          if(var=="Pt" || var=="PtCone"){
            hist_DATA_Den = (TH1D *)hist_DATA_Den->Rebin(n_ptarray, "hnew1", ptarray);
          }
          else{
            hist_DATA_Den->Rebin(rebins.at(it_var));
          }
          TH1D *hist_DATA_Den_Subtracted = (TH1D *)hist_DATA_Den->Clone();
          THStack *stack_bkgd_Den = new THStack("stack_bkgd_Den", "");


          TCanvas *c_DATA_Num = new TCanvas("c_DATA_Num", "", 600, 600);
          canvas_margin(c_DATA_Num);
          TH1D *hist_DATA_Num = (TH1D *)file_DATA->Get(Lepton+"_"+ID+"/"+Lepton+"_"+ID+"_DATA_AwayJetPt"+this_AwayJetPt+"_Num_"+var);
          if(var=="Pt" || var=="PtCone"){
            hist_DATA_Num = (TH1D *)hist_DATA_Num->Rebin(n_ptarray, "hnew1", ptarray);
          }
          else{
            hist_DATA_Num->Rebin(rebins.at(it_var));
          }
          TH1D *hist_DATA_Num_Subtracted = (TH1D *)hist_DATA_Num->Clone();
          THStack *stack_bkgd_Num = new THStack("stack_bkgd_Num", "");

          TLegend *lg_DATA = new TLegend(0.7, 0.65, 0.95, 0.90);
          lg_DATA->SetBorderSize(0);
          lg_DATA->SetFillStyle(0);

          lg_DATA->AddEntry(hist_DATA_Den, "Data", "pe");
          for(unsigned int it_sample=0; it_sample<Prompt_samples.size(); it_sample++){
            int ri = Prompt_samples.size()-1-it_sample;
            TH1D *histlg = new TH1D("histlg"+Prompt_samples.at(ri),"", 1, 0., 1.);
            histlg->SetFillColor(Prompt_sample_colors.at(ri));
            histlg->SetLineColor(Prompt_sample_colors.at(ri));
            lg_DATA->AddEntry(histlg, Prompt_alias.at(ri) ,"f");
          }


          for(unsigned int it_sample=0; it_sample<Prompt_samples.size(); it_sample++){
            TFile *filetemp = new TFile(base_filepath+"/CalcFakeRate_"+Prompt_samples.at(it_sample)+".root");
            TH1D *histtemp_Den = (TH1D *)filetemp->Get(Lepton+"_"+ID+"/"+Lepton+"_"+ID+"_DATA_AwayJetPt"+this_AwayJetPt+"_Den_"+var);
            TH1D *histtemp_Num = (TH1D *)filetemp->Get(Lepton+"_"+ID+"/"+Lepton+"_"+ID+"_DATA_AwayJetPt"+this_AwayJetPt+"_Num_"+var);

            if(histtemp_Den){
              histtemp_Den->SetFillColor(Prompt_sample_colors.at(it_sample));
              histtemp_Den->SetLineColor(Prompt_sample_colors.at(it_sample));

              if(var=="Pt" || var=="PtCone"){
                histtemp_Den = (TH1D *)histtemp_Den->Rebin(n_ptarray, "hnew1", ptarray);
              }
              else{
                histtemp_Den->Rebin(rebins.at(it_var));
              }

              hist_DATA_Den_Subtracted->Add(histtemp_Den, -1.);
              stack_bkgd_Den->Add(histtemp_Den);
            }
            if(histtemp_Num){
              histtemp_Num->SetFillColor(Prompt_sample_colors.at(it_sample));
              histtemp_Num->SetLineColor(Prompt_sample_colors.at(it_sample));

              if(var=="Pt" || var=="PtCone"){
                histtemp_Num = (TH1D *)histtemp_Num->Rebin(n_ptarray, "hnew1", ptarray);
              }
              else{
                histtemp_Num->Rebin(rebins.at(it_var));
              }

              hist_DATA_Num_Subtracted->Add(histtemp_Num, -1.);
              stack_bkgd_Num->Add(histtemp_Num);
            }

            filetemp->Close();
            delete filetemp;
          }

          //==== Den disbrib

          c_DATA_Den->cd();
          TH1D *dummy_DATA = new TH1D("dummy_DATA", "", int((x_max-x_min)/dx), x_min, x_max);
          hist_axis(dummy_DATA);
          dummy_DATA->GetYaxis()->SetRangeUser(1., GetMaximum(dummy_DATA));
          dummy_DATA->GetYaxis()->SetTitle("Events");
          dummy_DATA->GetXaxis()->SetTitle(xtitles.at(it_var));
          if(var=="Pt" || var=="PtCone") dummy_DATA->GetXaxis()->SetRangeUser(0,2000);
          dummy_DATA->SetLineColor(0);
          dummy_DATA->Draw("hist");

          stack_bkgd_Den->Draw("histsame");

          hist_DATA_Den->SetMarkerStyle(20);
          hist_DATA_Den->SetMarkerSize(1.6);
          hist_DATA_Den->SetMarkerColor(kBlack);
          hist_DATA_Den->SetLineColor(kBlack);
          hist_DATA_Den->Draw("psame");

          TH1D *hist_bkgd_Den = (TH1D *)stack_bkgd_Den->GetStack()->Last();
          double y_max = max( GetMaximum(hist_DATA_Den), GetMaximum(hist_bkgd_Den) );
          dummy_DATA->GetYaxis()->SetRangeUser(1., 5.*y_max);

          lg_DATA->Draw();
          c_DATA_Den->SetLogy();
          c_DATA_Den->SaveAs(this_dirname+"/1D_DATA_AwayJetPt"+this_AwayJetPt+"_Dist_Den_"+var+".pdf");
          c_DATA_Den->SaveAs(this_dirname+"/1D_DATA_AwayJetPt"+this_AwayJetPt+"_Dist_Den_"+var+".png");
          c_DATA_Den->Close();

          //==== Num distrib

          c_DATA_Num->cd();
          dummy_DATA->Draw("hist");

          stack_bkgd_Num->Draw("histsame");

          hist_DATA_Num->SetMarkerStyle(20);
          hist_DATA_Num->SetMarkerSize(1.6);
          hist_DATA_Num->SetMarkerColor(kBlack);
          hist_DATA_Num->SetLineColor(kBlack);
          hist_DATA_Num->Draw("psame");

          TH1D *hist_bkgd_Num = (TH1D *)stack_bkgd_Num->GetStack()->Last();
          y_max = max( GetMaximum(hist_DATA_Num), GetMaximum(hist_bkgd_Num) );
          dummy_DATA->GetYaxis()->SetRangeUser(1., 5.*y_max);

          lg_DATA->Draw();
          c_DATA_Num->SetLogy();
          c_DATA_Num->SaveAs(this_dirname+"/1D_DATA_AwayJetPt"+this_AwayJetPt+"_Dist_Num_"+var+".pdf");
          c_DATA_Num->SaveAs(this_dirname+"/1D_DATA_AwayJetPt"+this_AwayJetPt+"_Dist_Num_"+var+".png");
          c_DATA_Num->Close();

          //==== FR

          c_FR->cd();
          dummy_DATA->GetYaxis()->SetRangeUser(0,1.);
          dummy_DATA->GetYaxis()->SetTitle("FR");
          dummy_DATA->Draw("hist");
          if(var=="Pt" || var=="PtCone"){
            dummy_DATA->GetXaxis()->SetRangeUser(0,200);
          }

          //TEfficiency *Eff_FR = new TEfficiency(*hist_DATA_Num_Subtracted, *hist_DATA_Den_Subtracted);
          //TGraphAsymmErrors *gr_FR = Eff_FR->CreateGraph();
          TH1D *hist_FR = DivideHistograms(hist_DATA_Den_Subtracted,hist_DATA_Num_Subtracted);
          TGraphAsymmErrors *gr_FR = new TGraphAsymmErrors(hist_FR);
          gr_FR->SetMarkerSize(0);
          gr_FR->SetLineWidth(3);
          gr_FR->Draw("lsame");

          c_FR->SaveAs(this_dirname+"/1D_DATA_AwayJetPt"+this_AwayJetPt+"_FR_"+var+".pdf");
          c_FR->SaveAs(this_dirname+"/1D_DATA_AwayJetPt"+this_AwayJetPt+"_FR_"+var+".png");
          c_FR->Close();


        } // END Loop AwayJet Pt

      } // END Loop variable


      //==== For 2D plots

      TString Which2DToUse = "Pt_vs_Eta";
      if(UsePtCone) Which2DToUse = "PtCone_vs_Eta";

      TFile *outfile_FR = new TFile(this_dirname+"/"+Lepton+"_"+ID+"_FakeRates.root","RECREATE");
      for(unsigned int it_jpt=0; it_jpt<AwayJetMinPts.size(); it_jpt++){

        TString this_AwayJetPt = AwayJetMinPts.at(it_jpt);

        TCanvas *c_FR_2D = new TCanvas("c_FR_2D", "", 1000, 600);
        canvas_margin(c_FR_2D);
        c_FR_2D->cd();

        TH2D *hist_DATA_Den = (TH2D *)file_DATA->Get(Lepton+"_"+ID+"/"+Lepton+"_"+ID+"_DATA_AwayJetPt"+this_AwayJetPt+"_Den_"+Which2DToUse);
        TH2D *hist_DATA_Num = (TH2D *)file_DATA->Get(Lepton+"_"+ID+"/"+Lepton+"_"+ID+"_DATA_AwayJetPt"+this_AwayJetPt+"_Num_"+Which2DToUse);

        for(unsigned int it_sample=0; it_sample<Prompt_samples.size(); it_sample++){
          TFile *filetemp = new TFile(base_filepath+"/CalcFakeRate_"+Prompt_samples.at(it_sample)+".root");
          TH2D *histtemp_Den = (TH2D *)filetemp->Get(Lepton+"_"+ID+"/"+Lepton+"_"+ID+"_DATA_AwayJetPt"+this_AwayJetPt+"_Den_"+Which2DToUse);
          TH2D *histtemp_Num = (TH2D *)filetemp->Get(Lepton+"_"+ID+"/"+Lepton+"_"+ID+"_DATA_AwayJetPt"+this_AwayJetPt+"_Num_"+Which2DToUse);

          if(histtemp_Den){
            hist_DATA_Den->Add(histtemp_Den, -1.);
          }
          if(histtemp_Num){
            hist_DATA_Num->Add(histtemp_Num, -1.);
          }

          filetemp->Close();
          delete filetemp;
        }

        for(int ix=1; ix<=hist_DATA_Num->GetXaxis()->GetNbins(); ix++){
          for(int iy=1; iy<=hist_DATA_Num->GetYaxis()->GetNbins(); iy++){
            double this_Den = hist_DATA_Den->GetBinContent(ix,iy);
            double this_Num = hist_DATA_Num->GetBinContent(ix,iy);
            double this_FR = 1.;
            if(this_Den==0){
              this_Den = 1.;
              this_Num = 0.;
            }
            if(this_Den<0||this_Num<0){
              this_Den = 1.;
              this_Num = 0.;
            }
            hist_DATA_Den->SetBinContent(ix,iy,this_Den);
            hist_DATA_Num->SetBinContent(ix,iy,this_Num);
          }
        }

        TH2D *hist_FR_2D = (TH2D *)hist_DATA_Num->Clone();
        //hist_FR_2D->SetName(Lepton+"_2DFR_"+ID+"_AwayJetPt"+this_AwayJetPt);
        hist_FR_2D->SetName("AwayJetPt"+this_AwayJetPt);
        hist_FR_2D->Divide(hist_DATA_Den);

        outfile_FR->cd();
        hist_FR_2D->Write();

        c_FR_2D->cd();
        hist_FR_2D->Draw("colztext");
        hist_FR_2D->GetXaxis()->SetRangeUser(40,200);

        c_FR_2D->SaveAs(this_dirname+"/2D_DATA_AwayJetPt"+this_AwayJetPt+"_FR_"+Which2DToUse+".pdf");
        c_FR_2D->SaveAs(this_dirname+"/2D_DATA_AwayJetPt"+this_AwayJetPt+"_FR_"+Which2DToUse+".png");
        c_FR_2D->Close();


      } // END Loop AwayJet Pt

      if(outfile_FR) outfile_FR->Close();

    } // END Loop ID


  }

}
