#include "canvas_margin.h"
#include "mylib.h"

void Get_DYPtReweight(int xxx=0){

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
  
  int nrebin = 50;

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/OnZ/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/DYPtReweight/"+Year+"/";


  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }


  //==== MCFR Samples

  vector<TString> bkgds;

  if(Year=="2016"){

    bkgds = {
      "WJets_MG",
      "WZ_pythia", "ZZ_pythia", "WW_pythia",
      "TTLL_powheg", "TTLJ_powheg",
    };

  }
  if(Year=="2017"){

    bkgds = {
      "TTLL_powheg", "TTLJ_powheg",
      "WJets_MG",
      "ttW", "ttZ", "TTG",
      "WZ_pythia", "ZZ_pythia", "WW_pythia",
      "WWW", "WWZ", "WZZ", "ZZZ",
    };

  }

  TString samplename_DY10to50 = "DYJets10to50";
  TString samplename_DY50 = "DYJets";
  if(Year=="2017"){
    samplename_DY10to50 = "DYJets10to50_MG";
    samplename_DY50 = "DYJets";
  }

  TFile *file_DY10to50 = new TFile(base_filepath+"/HNWROnZ_"+samplename_DY10to50+".root");
  TFile *file_DY50 = new TFile(base_filepath+"/HNWROnZ_"+samplename_DY50+".root");

  TCanvas *c_reweight = new TCanvas("c_reweight", "", 600, 600);
  canvas_margin(c_reweight);
  TLegend *lg_ratio = new TLegend(0.2, 0.8, 0.5, 0.9);

  TFile *outfile = new TFile(base_plotpath+"/DYPtReweight.root","RECREATE");

  for(int it_fl=0; it_fl<2; it_fl++){

    TString leptonFlavour = "Electron";
    if(it_fl==1){
      leptonFlavour = "Muon";
    }

    TString dirname = "HNWR_Single"+leptonFlavour+"_OnZ";
    TString histname = "ZCand_Pt_"+dirname;

    TFile *file_DATA = new TFile(base_filepath+"/HNWROnZ_data_Single"+leptonFlavour+".root");
    TH1D *hist_DATA = (TH1D *)file_DATA->Get(dirname+"/"+histname);


    for(unsigned int it_bkgd=0; it_bkgd<bkgds.size(); it_bkgd++){
      TFile *file_bkgd = new TFile(base_filepath+"/HNWROnZ_"+bkgds.at(it_bkgd)+".root");
      TH1D *hist_bkgd = (TH1D *)file_bkgd->Get(dirname+"/"+histname);
      if(!hist_bkgd) continue;
      hist_DATA->Add(hist_bkgd, -1.);
    }

    TH1D *hist_DY10to50 = (TH1D *)file_DY10to50->Get(dirname+"/"+histname);
    TH1D *hist_DY50 = (TH1D *)file_DY50->Get(dirname+"/"+histname);

    if(hist_DY10to50) hist_DY50->Add(hist_DY10to50);

    hist_DATA->Rebin(nrebin);
    hist_DY50->Rebin(nrebin);

    hist_DATA->Scale(1./hist_DATA->Integral());
    hist_DY50->Scale(1./hist_DY50->Integral());

    TCanvas *c_shape = new TCanvas("c_shape", "", 600, 600);
    canvas_margin(c_shape);
    c_shape->cd();

    hist_DATA->SetLineColor(kBlack);
    hist_DY50->SetLineColor(kRed);
    hist_DATA->SetLineWidth(3);
    hist_DY50->SetLineWidth(3);

    hist_axis(hist_DATA);

    hist_DATA->Draw("histsame");
    hist_DY50->Draw("histsame");

    TLegend *lg_shape = new TLegend(0.6, 0.8, 0.9, 0.9);
    lg_shape->AddEntry(hist_DATA, "Data");
    lg_shape->AddEntry(hist_DY50, "MC");
    lg_shape->Draw();

    hist_DATA->GetXaxis()->SetRangeUser(0., 1000.);
    hist_DATA->GetYaxis()->SetTitle("Shape");
    hist_DATA->GetXaxis()->SetTitle("p_{T} of dilepton (GeV)");
    c_shape->SaveAs(base_plotpath+"/Shape_"+leptonFlavour+".pdf");
    c_shape->SaveAs(base_plotpath+"/Shape_"+leptonFlavour+".png");
    c_shape->Close();

    //==== ratio

    c_reweight->cd();
    TH1D *hist_ratio = (TH1D *)hist_DATA->Clone();
    for(int z=1; z<=hist_DATA->GetXaxis()->GetNbins(); z++){
      double y_DATA = hist_DATA->GetBinContent(z);
      double y_DY50 = hist_DY50->GetBinContent(z);

      double rel_e_DATA = hist_DATA->GetBinError(z)/y_DATA;
      double rel_e_DY50 = hist_DY50->GetBinError(z)/y_DY50;

      double rel_e_combined = sqrt( rel_e_DATA*rel_e_DATA + rel_e_DY50*rel_e_DY50 );

      double ratio = 1., err_ratio = 0.;
      if(y_DATA>0 && y_DY50>0){
        ratio = y_DATA/y_DY50;
        err_ratio = rel_e_combined*ratio;
      }
      hist_ratio->SetBinContent(z,ratio);
      hist_ratio->SetBinError(z, err_ratio);
    }


    if(it_fl==0){
      hist_axis(hist_ratio);
      hist_ratio->SetLineColor(kRed);
      hist_ratio->GetYaxis()->SetTitle("Reweight");
      hist_ratio->GetYaxis()->SetRangeUser(0.7, 1.3);
      hist_ratio->GetXaxis()->SetRangeUser(0., 1000.);
    }
    else{
      hist_ratio->SetLineColor(kBlue);
    }
    hist_ratio->SetLineWidth(3);
    hist_ratio->Draw("histe1same");

    if(it_fl==0){
      lg_ratio->AddEntry(hist_ratio, "ee", "l");
    }
    if(it_fl==1) lg_ratio->AddEntry(hist_ratio, "#mu#mu", "l");

    outfile->cd();
    hist_ratio->SetName(leptonFlavour);
    hist_ratio->Write();

  }
  outfile->Close();

  c_reweight->cd();
  lg_ratio->Draw();

  TLatex latex_CMSPriliminary, latex_Lumi;
  latex_CMSPriliminary.SetNDC();
  latex_Lumi.SetNDC();
  latex_CMSPriliminary.SetTextSize(0.035);
  latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");

  latex_Lumi.SetTextSize(0.035);
  latex_Lumi.SetTextFont(42);
  latex_Lumi.DrawLatex(0.72, 0.96, TotalLumi);

  c_reweight->SaveAs(base_plotpath+"/Reweight.pdf");
  c_reweight->SaveAs(base_plotpath+"/Reweight.png");
  c_reweight->Close();

}
