void MakeTemplate(){

  vector<double> vec_bins_pt_b = {50, 55, 60, 65, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 300, 500, 2000};
  vector<double> vec_bins_pt_e = {50, 55, 60, 65, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 300, 2000};

  const int nbin_b = vec_bins_pt_b.size()-1;
  const int nbin_e = vec_bins_pt_e.size()-1;

  double bins_pt_b[nbin_b+1], bins_pt_e[nbin_e+1];
  cout << "@@@@ Barrel" << endl;
  for(int i=0; i<nbin_b+1; i++){
    bins_pt_b[i] = vec_bins_pt_b.at(i);
    cout << i << "\t" << bins_pt_b[i] << endl;
  }
  cout << "@@@@ EndCap" << endl;
  for(int i=0; i<nbin_e+1; i++){
    bins_pt_e[i] = vec_bins_pt_e.at(i);
    cout << i << "\t" << bins_pt_e[i] << endl;
  }

  TFile *file = new TFile("HistTemplate.root", "RECREATE");
  TH1F *hist_b = new TH1F("hist_b", "", nbin_b, bins_pt_b);
  TH1F *hist_e = new TH1F("hist_e", "", nbin_e, bins_pt_e);

  file->cd();
  hist_b->Write();
  hist_e->Write();
  file->Close();


}
