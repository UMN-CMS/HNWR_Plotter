#include "Plotter.h"

Plotter::Plotter(){
  
  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  setTDRStyle();

  gStyle->SetOptStat(0);
  DoDebug = false;
  gErrorIgnoreLevel = kError;

  filename_skim = "";

  DataYear = 2016;

}

Plotter::~Plotter(){

  system("rm -rf "+path_rebins);
  system("rm -rf "+path_y_axis);
  system("rm -rf "+path_x_axis);
  
}

void Plotter::draw_hist(){

  for(i_cut = 0; i_cut < histname_suffix.size(); i_cut++){

    //==== This step, 
    //==== plotpath = ~~~/plots/v8-0-7.15/Regions/use_FR_method/fake_Dijet/
    if(DoDebug) cout << "[draw_hist] plotpath = " << plotpath << endl;
    thiscut_plotpath = plotpath+"/"+histname_suffix[i_cut];
    if(ApplyMCNormSF.at(i_cut)) thiscut_plotpath = plotpath+"/MCNormSFed"+histname_suffix[i_cut];

    cout
    << endl
    << "################### Writing in Directory " << histname_suffix[i_cut] << " ###################" << endl
    << endl;

    //==== Make rebin/y_axis/x_axis file for this cut here
    MakeRebins();
    MakeYAxis();
    MakeXAxis();

    TString DirName = histname_suffix[i_cut];
    //TString temp_suffix = histname_suffix[i_cut];
    //TString DirName = temp_suffix.Remove(0,1);

    for(i_var = 0; i_var < histname.size(); i_var++){

      if( find( CutVarSkips.begin(), CutVarSkips.end(), make_pair(histname_suffix[i_cut], histname[i_var]) ) != CutVarSkips.end() ){
        continue;
      }
      
      cout << "[Drawing " << histname[i_var] << "]" << endl;
      
      TH1D* MC_stacked_staterr = NULL;
      TH1D* MC_stacked_allerr = NULL;
      TH1D* MC_stacked_allerr_Up = NULL;
      TH1D* MC_stacked_allerr_Down = NULL;
      THStack* MC_stacked = new THStack("MC_stacked", "");
      TH1D* hist_data = NULL;
      vector<TH1D*> hist_signal;
      
      TLegend *lg;
      if(drawratio.at(i_cut)){
        //==== CR
        if(signal_LRSMinfo.size()==0) lg = new TLegend(0.60, 0.35, 0.96, 0.92);
        //==== SR
        else lg = new TLegend(0.51, 0.36, 0.94, 0.91);
      }
      else{
        //==== CR
        if(signal_LRSMinfo.size()==0) lg = new TLegend(0.60, 0.35, 0.95, 0.92);
        //==== SR
        else lg = new TLegend(0.55, 0.50, 0.93, 0.90);
      }

      clear_legend_info();
      
      signal_survive.clear();

      bool AnyEntry = false;
      map< TString, TH1D * > map_hist_y;

      for(i_file = 0; i_file < bkglist.size()+1+signal_LRSMinfo.size(); i_file++){ // +1 for data
      
        TString filepath, current_sample, signal_name_for_tex;
        
        //==== root file path name
        //==== bkg
        if( i_file < bkglist.size() ){
          TString tmp = bkglist[i_file];
          if(bkglist[i_file].Contains("fake") || bkglist[i_file].Contains("chargeflip") || bkglist[i_file].Contains("FromEMu") ) tmp += "_"+PrimaryDataset[i_cut];
          filepath = "./rootfiles/"+data_class+"/"+filename_prefix+filename_skim+"_"+tmp+filename_suffix;
          current_sample = bkglist[i_file];
        }
        //==== data for i_file = bkglist.size()
        else if( i_file == bkglist.size() ){
          filepath = "./rootfiles/"+data_class+"/"+filename_prefix+filename_skim+"_data_"+PrimaryDataset[i_cut]+filename_suffix;
          current_sample = "data";
        }
        //==== signal starting from i_file = bkglist.size()+1
        else{

          int signal_index = i_file-bkglist.size()-1;

          if(!signal_draw[signal_index]) continue;

          //==== Read from data PD
          TString WhichChannel = "MuMu";
          if(histname_suffix[i_cut].Contains("SingleElectron")) WhichChannel = "EE";

          //==== If data PD != this signal lep channl, continue
          if(signal_LRSMinfo.at(signal_index).lep_channel != WhichChannel) continue;
          signal_LRSMinfo.at(signal_index).SetNames();
          if(DoDebug) cout << "signal_index = " << signal_index << " => mass = " << signal_LRSMinfo[signal_index].GetFileName() << endl;

          TString WhichChannel_for_tex = WhichChannel;

          //==== CAT signal alias
          TString string_signal_mass = signal_LRSMinfo.at(signal_index).GetFileName();
          signal_name_for_tex = signal_LRSMinfo.at(signal_index).GetTEXName();

          filepath = "./rootfiles/"+data_class+"/Signal/"+filename_prefix+"_"+string_signal_mass+filename_suffix;
          //cout << filepath << endl;
          current_sample = signal_name_for_tex;
        }

        if(DoDebug){
          cout
          << "filepath = " << filepath << endl
          << "hisname = " << histname[i_var]+"_"+histname_suffix[i_cut] << endl;
        }
        
        //==== get root file
        if(gSystem->AccessPathName(filepath)){
          if(DoDebug){
            cout << "No file : " << filepath << endl;
          }
          continue;
        }
        TFile* file = new TFile(filepath);
        if( !file ){
          if(DoDebug){
            cout << "No file : " << filepath << endl;
          }
          continue;
        }

        TDirectory *dir = (TDirectory *)file->Get(DirName);
        if(!dir){
          if(DoDebug){
            cout << "No Directory : " << file->GetName() << "\t" << DirName << endl;
            file->ls();
          }
          file->Close();
          delete file;
          continue;
        }
        file->cd(DirName);

        //==== full histogram name
        TString fullhistname = histname[i_var]+"_"+histname_suffix[i_cut];
        
        //==== get histogram
        TH1D* hist_temp = (TH1D*)dir->Get(fullhistname);
        if(!hist_temp || hist_temp->GetEntries() == 0){
          if(DoDebug){
            cout << "No histogram : " << current_sample << endl;
          }
          file->Close();
          delete file;
          continue;
        }
        AnyEntry = true;

        //==== set histogram name, including sample name
        hist_temp->SetName(fullhistname+"_"+current_sample);

        //==== rebin
        hist_temp = Rebin(hist_temp);
        //hist_temp->Rebin( n_rebin() );
      
        //==== set X-axis range
        SetXaxisRange(hist_temp);
        
        //==== make overflows bins
        TH1D *hist_final = MakeOverflowBin(hist_temp);

        //==== Stat Error Propations for Fake
        //==== TODO Later..

        //==== Remove Negative bins
        TAxis *xaxis = hist_final->GetXaxis();
        for(int ccc=1; ccc<=xaxis->GetNbins(); ccc++){
          if(DoDebug) cout << current_sample << "\t["<<xaxis->GetBinLowEdge(ccc) <<", "<<xaxis->GetBinUpEdge(ccc) << "] : " << hist_final->GetBinContent(ccc) << endl;
          if(hist_final->GetBinContent(ccc)<0){
            hist_final->SetBinContent(ccc, 0.);
            hist_final->SetBinError(ccc, 0.);
          }
        }

        TString current_MCsector = "";
        //==== Set Attributes here
        //==== bkg
        if( i_file < bkglist.size() ){
          //==== get which MC sector
          current_MCsector = find_MCsector();
          int n_bins = hist_final->GetXaxis()->GetNbins();
          if(!MC_stacked_allerr){

            const Double_t *xcopy=hist_final->GetXaxis()->GetXbins()->GetArray();
            MC_stacked_allerr = new TH1D("MC_stacked_allerr", "", n_bins, xcopy);
            MC_stacked_allerr_Up = new TH1D("MC_stacked_allerr_Up", "", n_bins, xcopy);
            MC_stacked_allerr_Down = new TH1D("MC_stacked_allerr_Down", "", n_bins, xcopy);
            MC_stacked_staterr = new TH1D("MC_stacked_staterr", "", n_bins, xcopy);

          }
          hist_final->SetFillColor(map_sample_string_to_legendinfo[current_MCsector].second);
          hist_final->SetLineColor(map_sample_string_to_legendinfo[current_MCsector].second);

          //==== MC Norm Scaling
          if(ApplyMCNormSF.at(i_cut)){
            //hist_final->Scale(analysisInputs.MCNormSF[current_sample]);

            //==== FIXME for DY
            if(current_sample.Contains("DYJets")){
              double DYNorm = GetDYNormSF(DataYear, histname_suffix[i_cut]);
              hist_final->Scale(DYNorm);
            }
            else{
              hist_final->Scale(analysisInputs.MCNormSF[current_sample]);
            }

          }

          //==== Add star error histogram now, just before adding systematic
          MC_stacked_staterr->Add(hist_final);

          //==== Now Add systematic to hist_final (which has stat err only)
          //==== Start with BinContent = Central, Error = 0 histogram
          //==== Square sum each bin
          TH1D *hist_SumSystUpMax = (TH1D *)hist_final->Clone();
          TH1D *hist_SumSystDownMax = (TH1D *)hist_final->Clone();

          if(current_sample.Contains("EMuMethod")){
            //==== TODO Add EMu syst

            double EMuSyst = 0.20;
            if( histname_suffix[i_cut].Contains("Boosted") ) EMuSyst = 0.30;

            for(int i=0; i<=hist_final->GetXaxis()->GetNbins()+1; i++){
              double y = hist_final->GetBinContent(i);
              double err_Stat = hist_final->GetBinError(i);
              double err_EMu = y * EMuSyst;
              double new_err = sqrt( err_Stat*err_Stat + err_EMu*err_EMu );

              hist_SumSystUpMax->SetBinError( i, new_err );
              hist_SumSystDownMax->SetBinError( i, new_err );
            }

          }
          //==== MC
          else{

            //==== norm
            //==== 1) Lumi
            double Norm_RelSyst_Lumi = LumiError();
            //==== 2) Xsec //TODO add more samples
            double Norm_RelSyst_Xsec = 0;
            if(ApplyMCNormSF.at(i_cut)){
              if(current_sample.Contains("DYJets")){

                double Scaled_value = GetDYNormSF(DataYear, histname_suffix[i_cut]);
                double SFErr = GetDYNormSF(DataYear, histname_suffix[i_cut], true);

                Norm_RelSyst_Xsec = SFErr/Scaled_value;

              }
              else{
              }
            }

            //==== sum
            double Sum_Norm_RelSyst = sqrt( Norm_RelSyst_Lumi*Norm_RelSyst_Lumi + Norm_RelSyst_Xsec*Norm_RelSyst_Xsec );

            //==== Add Norm Syst to hist_final
            for(int i=0; i<=hist_final->GetXaxis()->GetNbins()+1; i++){
              double y = hist_final->GetBinContent(i);
              double err_Stat = hist_final->GetBinError(i);
              double err_Norm = y * Sum_Norm_RelSyst;
              double new_err = sqrt( err_Stat*err_Stat + err_Norm*err_Norm );

              hist_SumSystUpMax->SetBinError( i, new_err );
              hist_SumSystDownMax->SetBinError( i, new_err );
            }

            //==== shape
            for(unsigned it_Syst=0; it_Syst<Systs.size(); it_Syst++){

              TString Syst = Systs.at(it_Syst);

              //==== For this systematic source,

              //==== Get Up histogram
              TDirectory *dir_Up = (TDirectory *)file->Get("Syst_"+Syst+"Up_"+DirName);
              TH1D *hist_Up = NULL;
              if(dir_Up){
                hist_Up = (TH1D *)dir_Up->Get( histname[i_var]+"_Syst_"+Syst+"Up_"+DirName );
                if(hist_Up){
                  hist_Up = Rebin(hist_Up);
                  //hist_Up->Rebin( n_rebin() );
                }
                else{
                  hist_Up = (TH1D *)hist_final->Clone();
                  EmptyHistogram(hist_Up);
                }
              }
              else{
                hist_Up = (TH1D *)hist_final->Clone();
                EmptyHistogram(hist_Up);
              }

              //==== Get Down histogram
              TDirectory *dir_Down = (TDirectory *)file->Get("Syst_"+Syst+"Down_"+DirName);
              TH1D *hist_Down = NULL;
              if(dir_Down){
                hist_Down = (TH1D *)dir_Down->Get( histname[i_var]+"_Syst_"+Syst+"Down_"+DirName );
                if(hist_Down){
                  hist_Down = Rebin(hist_Down);
                  //hist_Down->Rebin( n_rebin() );
                }
                else{
                  hist_Down = (TH1D *)hist_final->Clone();
                  EmptyHistogram(hist_Down);
                }
              }
              else{
                hist_Down = (TH1D *)hist_final->Clone();
                EmptyHistogram(hist_Down);
              }

              //==== set X-axis range
              SetXaxisRange(hist_Up);
              SetXaxisRange(hist_Down);

              //==== make overflows bins
              TH1D *hist_Up_final = MakeOverflowBin(hist_Up);
              TH1D *hist_Down_final = MakeOverflowBin(hist_Down);

              //==== Remove Negative bins
              TAxis *xaxis = hist_final->GetXaxis();
              for(int ccc=1; ccc<=xaxis->GetNbins(); ccc++){
                if(hist_Up_final->GetBinContent(ccc)<0){
                  hist_Up_final->SetBinContent(ccc, 0.);
                  hist_Up_final->SetBinError(ccc, 0.);
                }
                if(hist_Down_final->GetBinContent(ccc)<0){
                  hist_Down_final->SetBinContent(ccc, 0.);
                  hist_Down_final->SetBinError(ccc, 0.);
                }
              }

              //==== Apply the same MC Norm Scaling
              if(ApplyMCNormSF.at(i_cut)){
                //hist_final->Scale(analysisInputs.MCNormSF[current_sample]);

                //==== FIXME for DY
                if(current_sample.Contains("DYJets")){
                  double DYNorm = GetDYNormSF(DataYear, histname_suffix[i_cut]);
                  hist_Up_final->Scale(DYNorm);
                  hist_Down_final->Scale(DYNorm);
                }
                else{
                  hist_Up_final->Scale(analysisInputs.MCNormSF[current_sample]);
                  hist_Down_final->Scale(analysisInputs.MCNormSF[current_sample]);
                }

              }

              //==== Convert this to MaxUp and MaxDown
              //==== hist_SystMax.at(0) : Content = Central, Errors = Maximum up-side variation of this syst source
              //==== hist_SystMax.at(1) : Content = Central, Errors = Maximum down-side variation of this syst source
              vector<TH1D *> hist_SystMax = ConvertSystematic(hist_final, hist_Up_final, hist_Down_final);

              AddSystematic(hist_SumSystUpMax, hist_SystMax.at(0));
              AddSystematic(hist_SumSystDownMax, hist_SystMax.at(1));

            } // END Loop syst

          }

          MC_stacked_allerr_Up->Add(hist_SumSystUpMax);
          MC_stacked_allerr_Down->Add(hist_SumSystDownMax);
          MC_stacked_allerr->Add(hist_final);

          MC_stacked->Add(hist_final);
          //MC_stacked_allerr->Add(hist_final);

        }
        //==== data for i_file = bkglist.size()
        else if( i_file == bkglist.size() ){
          hist_final->SetMarkerStyle(20);
          hist_final->SetMarkerSize(1.2);
          TString temp_hist_name(hist_final->GetName());
          hist_data = (TH1D*)hist_final->Clone();
        }
        //==== signal starting from i_file = bkglist.size()+1
        else if( i_file > bkglist.size() ){
          int signal_index = i_file-bkglist.size()-1;
          if(DoDebug) cout << "signal index = " << signal_index << ", mass = " << signal_LRSMinfo[signal_index].GetFileName() << endl;
          hist_final->SetLineColor(signal_color[signal_index]);
          hist_final->SetLineWidth(3);
          hist_final->SetLineStyle(signal_style[signal_index]);
          TString temp_hist_name(hist_final->GetName());
          //hist_final->SetName(temp_hist_name+"_signal_"+TString::Itoa(signal_LRSMinfo[signal_index], 10));
          //hist_final->SetName(temp_hist_name+"_signal_"+TString::Itoa(signal_LRSMinfo[signal_index], 10));

          hist_final->Scale( signa_xsec[signal_index] );

          hist_signal.push_back( (TH1D*)hist_final->Clone() );
          signal_survive.push_back( signal_LRSMinfo.at(signal_index) );
        }
        else{
          cout << "[Warning] attirubte setting, i_file > total sample size? This should not happen!" << endl;
        }

        //==== fill legend info
        fill_legend(lg, hist_final);

        if(histname[i_var]=="NEvent"){
          cout << "current_sample = " << current_sample << endl;
          TString alias = "";
          if(current_sample.Contains("data")) alias = "data";
          else if(current_sample.Contains("Signal")) alias = signal_name_for_tex;
          else{
            alias = map_sample_string_to_legendinfo[current_MCsector].first;
          }
          cout << "==> alias = " << alias << endl;

          if( map_hist_y.find(alias) == map_hist_y.end() ){
            map_hist_y[alias] = new TH1D(alias, "", 1, 0., 1);
          }

          map_hist_y[alias]->Add(hist_final);
        }
        
        file->Close();
        delete file;
        
        if(DoDebug) cout << "end of this sample" << endl;
        
      } // END loop over samples

      if(histname[i_var]=="NEvent"){
        MakeTexFile(map_hist_y);
      }

      if(!AnyEntry) continue;
      if(DoDebug) cout << "[Draw Canvas]" << endl;

      //==== with MC_stacked_allerr_Up and MC_stacked_allerr_Down, create gr_MC_allerr
      TGraphAsymmErrors *gr_MC_allerr = GetAsymmError(MC_stacked_allerr_Up, MC_stacked_allerr_Down);

/*
      //==== FIXME debug syst
      int debug_bin = MC_stacked_allerr_Up->FindBin(90.);
      cout << "MC_stacked_allerr_Down : " << MC_stacked_allerr_Down->GetBinContent(debug_bin) << "\t" << MC_stacked_allerr_Down->GetBinError(debug_bin) << endl;
      cout << "MC_stacked_allerr_Up : " << MC_stacked_allerr_Up->GetBinContent(debug_bin) << "\t" << MC_stacked_allerr_Up->GetBinError(debug_bin) << endl; 
      cout << "MC_stacked_allerr : " << MC_stacked_allerr->GetBinContent(debug_bin) << "\t" << MC_stacked_allerr->GetBinError(debug_bin) << endl;
      cout << "@@@@ gr_MC_allerr @@@@" << endl;
      gr_MC_allerr->Print();
*/

      if(!drawdata.at(i_cut) && hist_data){
        TString tmpname = hist_data->GetName();
        hist_data = (TH1D*)MC_stacked_allerr->Clone();
        SetErrorZero(hist_data);
        hist_data->SetName(tmpname);
        hist_data->SetMarkerStyle(20);
        hist_data->SetMarkerSize(1.2);
        hist_data->SetMarkerColor(kBlack);
        hist_data->SetLineColor(kBlack);
      }
      draw_canvas(MC_stacked, MC_stacked_staterr, gr_MC_allerr, hist_data, hist_signal, lg, drawdata.at(i_cut));

      //==== legend is already deleted in draw_canvas()
      //delete lg; 
      
    } // END loop over variables

    //system("rm "+thiscut_plotpath+"/hists.root");


  } // END loop over cuts
  
  
}

void Plotter::make_bkglist(){
  for(unsigned int i=0; i<samples_to_use.size(); i++){
    MCsector_first_index.push_back( bkglist.size() );
    if(DoDebug) cout << "[make_bkglist] " << "MCsector_first_index.push_back(" <<  bkglist.size() << ")" << endl;
    bkglist.insert(bkglist.end(),
                   map_sample_string_to_list[samples_to_use.at(i)].begin(),
                   map_sample_string_to_list[samples_to_use.at(i)].end()
                   );
  }
  cout << "We will use :" << endl;
  for(unsigned int i=0; i<bkglist.size(); i++) cout << " " << bkglist[i] << endl;
}

void Plotter::SetRebins(TString filepath){

  cout << "[Plotter::SetRebins] Get rebins from " << filepath << endl;

  map< TString, map<TString, int> > ALL_rebins;

  string elline;
  ifstream in(filepath);
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString cut;
    TString histname;
    double value;
    is >> cut;
    is >> histname;
    is >> value;
    (ALL_rebins[cut])[histname] = value;
  }

  path_rebins = filepath+"tmp/";
  gSystem->mkdir(path_rebins, kTRUE);
  for(map< TString, map<TString, int> >::iterator thiscut=ALL_rebins.begin(); thiscut!=ALL_rebins.end(); thiscut++){
    ofstream skeleton_rebins(path_rebins+thiscut->first, ios::trunc);
    for(map<TString, int>::iterator thishistname=(thiscut->second).begin(); thishistname!=(thiscut->second).end(); thishistname++){
      skeleton_rebins<<(thishistname->first)<<"\t"<<(thishistname->second)<<endl;
    }
    skeleton_rebins.close();
  }

}


void Plotter::SetYAxis(TString filepath){

  cout << "[Plotter::SetYAxis] Get Yaxis from " << filepath << endl;

  map< TString, map<TString, double> > ALL_y_maxs;

  string elline;
  ifstream in(filepath);
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString cut;
    TString histname;
    double value;
    is >> cut;
    is >> histname;
    is >> value;
    (ALL_y_maxs[cut])[histname] = value;
  }

  path_y_axis = filepath+"tmp/";
  gSystem->mkdir(path_y_axis, kTRUE);
  for(map< TString, map<TString, double> >::iterator thiscut=ALL_y_maxs.begin(); thiscut!=ALL_y_maxs.end(); thiscut++){
    ofstream skeleton_y_maxs(path_y_axis+thiscut->first, ios::trunc);
    for(map<TString, double>::iterator thishistname=(thiscut->second).begin(); thishistname!=(thiscut->second).end(); thishistname++){
      skeleton_y_maxs<<(thishistname->first)<<"\t"<<(thishistname->second)<<endl;
    }
    skeleton_y_maxs.close();
  }
  
}

void Plotter::SetXAxis(TString filepath){

  cout << "[Plotter::SetXAxis] Get Xaxis from " << filepath << endl;

  map< TString, map<TString, vector<double>> > ALL_x_axis;

  string elline;
  ifstream in(filepath);
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString cut;
    TString histname;
    double value_min, value_max;
    is >> cut;
    is >> histname;
    is >> value_min;
    is >> value_max;
    (ALL_x_axis[cut])[histname].push_back(value_min);
    (ALL_x_axis[cut])[histname].push_back(value_max);
  }

  path_x_axis = filepath+"tmp/";

  gSystem->mkdir(path_x_axis, kTRUE);
  for(map< TString, map<TString, vector<double>> >::iterator thiscut=ALL_x_axis.begin(); thiscut!=ALL_x_axis.end(); thiscut++){
    ofstream skeleton_x_axis(path_x_axis+thiscut->first, ios::trunc);
    for(map<TString, vector<double>>::iterator thishistname=(thiscut->second).begin(); thishistname!=(thiscut->second).end(); thishistname++){
      skeleton_x_axis<<(thishistname->first)<<"\t"<<(thishistname->second).at(0)<<"\t"<<(thishistname->second).at(1)<<endl;
    }
    skeleton_x_axis.close();
  }

}

void Plotter::MakeRebins(){

  temp_rebins.clear();

  string elline;
  ifstream in(path_rebins+histname_suffix[i_cut]);
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString histname;
    double value;
    is >> histname;
    is >> value;
    //==== if -999, don't set
    if(value!=-999){
      temp_rebins[histname] = value;
    }
  }

}

void Plotter::MakeYAxis(){

  temp_y_maxs.clear();

  string elline;
  ifstream in(path_y_axis+histname_suffix[i_cut]);
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString histname;
    double value;
    is >> histname;
    is >> value;
    //==== if -999, don't set
    if(value!=-999){
      temp_y_maxs[histname] = value;
    }
  }

}

void Plotter::MakeXAxis(){

  temp_x_mins.clear();
  temp_x_maxs.clear();

  string elline;
  ifstream in(path_x_axis+histname_suffix[i_cut]);
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString histname;
    double value_min, value_max;
    is >> histname;
    is >> value_min;
    is >> value_max;
    //==== if -999, don't set
    if(value_min!=-999){
      temp_x_mins[histname] = value_min;
    }
    if(value_max!=-999){
      temp_x_maxs[histname] = value_max;
    }
  }

}

TString Plotter::find_MCsector(){
  for(unsigned int i=0; i<MCsector_first_index.size()-1; i++){
    if(MCsector_first_index.at(i) <= i_file && i_file < MCsector_first_index.at(i+1)){
      if(DoDebug) cout << "[find_MCsector] returned MCsector is " << samples_to_use.at(i) << endl;
      return samples_to_use.at(i);
    }
  }
  if(DoDebug) cout << "[find_MCsector] returned MCsector is " << samples_to_use.back() << endl;
  return samples_to_use.back();
}

void Plotter::clear_legend_info(){
  hist_for_legend_bkg.clear();
  hist_for_legend_signal.clear();
  MCsector_survive.clear();
  for(int i=0; i<samples_to_use.size(); i++){
    MCsector_survive[samples_to_use.at(i)] = false;
  }

}

void Plotter::fill_legend(TLegend* lg, TH1D* hist){
  //==== here, hist_for_legned = {"A", "B", "D"}
  //==== now, push_back data and signal to make
  //==== hist_for_legned = {"A", "B", "D", "data", "HN40", "HN50", "HN60"}
  
  //==== bkg
  if( i_file < bkglist.size() ){
    TString current_MCsector = find_MCsector();
    if(DoDebug) cout << "[fill_legend] current_MCsector is " << current_MCsector << endl;
    if( !MCsector_survive[current_MCsector] ){
      if(DoDebug) cout << "[fill_legend] => is saved" << endl;
      hist_for_legend_bkg.push_back((TH1D*)hist->Clone());
      MCsector_survive[current_MCsector] = true;
    }
  }
  //==== data
  else if( i_file == (int)bkglist.size() ){
    hist_for_legend_data = (TH1D*)hist->Clone();
    if(DoDebug) cout << "Data added in hist_for_legend" << endl;
  }
  //==== signals
  else if( i_file > (int)bkglist.size() ){
    hist_for_legend_signal.push_back((TH1D*)hist->Clone());
    if(DoDebug) cout << "Signal added in hist_for_legend" << endl;
  }
  else{
    cout << "[Warning] fill_legend, i_file > total sample size? This should not happen!" << endl;
  }


}

void Plotter::draw_legend(TLegend* lg, bool DrawData){
  // Example :
  //                      0    1    2    3
  // samples_to_use   = {"A", "B", "C", "D"}
  // MCsector_survive = { T ,  T ,  F ,  T }
  //
  //                       0    1    2      3        4          5
  // hist_for_legend are {"A", "B", "D", "data", "signal1", "signal2"}
  // i_data = 6 - 2 - 1 = 3
  
  if(DoDebug) cout << "[draw_legend] printing MCsector_survive" << endl;
  for(auto it = MCsector_survive.begin(); it != MCsector_survive.end(); ++it){
    if(DoDebug) cout << "[draw_legend] " << it->first << " is " << it->second << endl;
  }
  for(int i=samples_to_use.size()-1, j=hist_for_legend_bkg.size()-1; i>=0; i--){
    if(MCsector_survive[samples_to_use.at(i)]){
      if(DoDebug) cout << "[draw_legend] " << samples_to_use.at(i) << " is added in legend" << endl;
      lg->AddEntry(hist_for_legend_bkg.at(j), map_sample_string_to_legendinfo[samples_to_use.at(i)].first, "f");
      j--;
    }
  }

  //==== Signal
  if(CurrentSC==NoClass){

    for(unsigned int i=0; i<signal_survive.size(); i++){
      LRSMSignalInfo this_lrsm = signal_survive.at(i);
      lg->AddEntry(hist_for_legend_signal.at(i), this_lrsm.GetLegendAlias(), "l");
    }

  }
  else{

    for(int i_sigcl=0; i_sigcl<AllSignalClasses.size(); i_sigcl++){

      signal_class this_cl = AllSignalClasses.at(i_sigcl);

      if(CurrentSC==this_cl){

        for(unsigned int i=0; i<signal_survive.size(); i++){
          LRSMSignalInfo this_lrsm = signal_survive.at(i);
          if( find(map_class_to_LRSMSignalInfo[this_cl].begin(), map_class_to_LRSMSignalInfo[this_cl].end(), this_lrsm) != map_class_to_LRSMSignalInfo[this_cl].end()){
            lg->AddEntry(hist_for_legend_signal.at(i), this_lrsm.GetLegendAlias(), "l");
          }
        }

      }

    }

  }

  lg->SetFillStyle(0);
  lg->SetBorderSize(0);
  lg->Draw();
}

void Plotter::draw_canvas(THStack *mc_stack, TH1D *mc_staterror, TGraphAsymmErrors *mc_allerror, TH1D *hist_data, vector<TH1D *> hist_signal, TLegend *legend, bool DrawData){

  if(!hist_data) return;

  //==== signal_class
  CurrentSC = NoClass;

  if(histname_suffix[i_cut].Contains("Boosted")) CurrentSC = Boosted;
  else CurrentSC = Resolved;

  //==== y=0 line
  double x_0[2], y_0[2];
  x_0[0] = -5000;  y_0[0] = 0;
  x_0[1] = 5000;  y_0[1] = 0;
  TGraph *g0 = new TGraph(2, x_0, y_0);
  //==== y=1 line
  double x_1[2], y_1[2];
  x_1[0] = 5000;  y_1[0] = 1;
  x_1[1] = -5000;  y_1[1] = 1;
  TGraph *g1 = new TGraph(2, x_1, y_1);
  
  //==== If we draw data, prepare top/bottom pads
  TCanvas* c1 = new TCanvas(histname[i_var], "", 800, 800);
  c1->Draw();
  c1->cd();

  TPad *c1_up;
  TPad *c1_down;
  c1_up = new TPad("c1", "", 0, 0.25, 1, 1);
  c1_down = new TPad("c1_down", "", 0, 0, 1, 0.25);

  if(drawratio.at(i_cut)){
    canvas_margin(c1, c1_up, c1_down);

    c1_up->Draw();
    c1_down->Draw();
    c1_up->cd();
  }
  else{
    canvas_margin(c1);
  }

/*
  if(histname[i_var]=="WRCand_Mass"){
    vector<double> new_binval = GetRebinZeroBackground(mc_stack, mc_staterror, mc_allerror, hist_data, hist_signal);
    if(new_binval.size()>0){

      const int new_nbins = new_binval.size()-1;
      double new_binvals[new_nbins+1];
      for(int i=0;i<new_nbins+1;i++) new_binvals[i] = new_binval.at(i);

      THStack *new_mc_stack = new THStack("new_mc_stack", "");
      TList *list_stack = mc_stack->GetHists();
      for(int i=0; i<list_stack->Capacity(); i++){
        TH1D *this_hist = (TH1D *)list_stack->At(i);
        this_hist = (TH1D *)this_hist->Rebin(new_nbins, "hnew1", new_binvals);
        new_mc_stack->Add(this_hist);
      }
      mc_stack = new_mc_stack;

      mc_staterror = (TH1D *)mc_staterror->Rebin(new_nbins, "hnew1", new_binvals);
      mc_allerror = (TH1D *)mc_allerror->Rebin(new_nbins, "hnew1", new_binvals);
      hist_data = (TH1D *)hist_data->Rebin(new_nbins, "hnew1", new_binvals);

      for(unsigned int i=0; i<hist_signal.size(); i++){
        hist_signal.at(i) = (TH1D *)hist_signal.at(i)->Rebin(new_nbins, "hnew1", new_binvals);
      }
    }
  }
*/

  //==== empty histogram for axis
  TH1D *hist_empty = (TH1D*)mc_stack->GetHists()->At(0)->Clone();
  hist_empty->SetName("DUMMY_FOR_AXIS");
  //=== get dX
  double dx = (hist_empty->GetXaxis()->GetXmax() - hist_empty->GetXaxis()->GetXmin())/hist_empty->GetXaxis()->GetNbins();
  TString YTitle = DoubleToString(dx);

  hist_empty->GetYaxis()->SetTitle(YTitle);
  hist_empty->SetLineWidth(0);
  hist_empty->SetLineColor(0);
  hist_empty->SetMarkerSize(0);
  hist_empty->SetMarkerColor(0);
  double Ymin = default_y_min;
  double YmaxScale = 1.2;
  if(UseLogy.at(i_cut)>0){
    Ymin = UseLogy.at(i_cut);
    YmaxScale = 10;
    hist_empty->SetMinimum(UseLogy.at(i_cut));

    if(drawratio.at(i_cut)) c1_up->SetLogy();
    else c1->SetLogy();
  }
  hist_empty->Draw("histsame");
  //==== hide X Label for top plot
  if(drawratio.at(i_cut)) hist_empty->GetXaxis()->SetLabelSize(0);
  
  //==== bkg
  if(!mc_stack->GetHists()){
    cout << "[No Background]" << endl;
    return;
  }
  mc_stack->Draw("histsame");

  //==== signal
  if(CurrentSC==NoClass){

    for(unsigned int i=0; i<signal_survive.size(); i++){
      LRSMSignalInfo this_lrsm = signal_survive.at(i);
      hist_signal[i]->Draw("histsame");
    }

  }
  else{

    for(int i_sigcl=0; i_sigcl<AllSignalClasses.size(); i_sigcl++){

      signal_class this_cl = AllSignalClasses.at(i_sigcl);

      if(CurrentSC==this_cl){

        for(unsigned int i=0; i<signal_survive.size(); i++){
          LRSMSignalInfo this_lrsm = signal_survive.at(i);
          if( find(map_class_to_LRSMSignalInfo[this_cl].begin(), map_class_to_LRSMSignalInfo[this_cl].end(), this_lrsm) != map_class_to_LRSMSignalInfo[this_cl].end()){
            hist_signal[i]->Draw("histsame");
          }
        }

      }

    }

  }

  //==== background err
  mc_allerror->SetMarkerColor(0);
  mc_allerror->SetMarkerSize(0);
  mc_allerror->SetFillStyle(3013);
  mc_allerror->SetFillColor(kBlack);
  mc_allerror->SetLineColor(0);
  mc_allerror->Draw("sameE2");

  //==== Draw Data at last
  vector<float> err_up_tmp;
  vector<float> err_down_tmp;
  const double alpha = 1 - 0.6827;
  TGraphAsymmErrors *gr_data = new TGraphAsymmErrors(hist_data);
  for(int i=0; i<gr_data->GetN(); ++i){
    int N = gr_data->GetY()[i];
    double L =  (N==0) ? 0  : (ROOT::Math::gamma_quantile(alpha/2,N,1.));
    double U =  (N==0) ? ( ROOT::Math::gamma_quantile_c(alpha,N+1,1) ) : ( ROOT::Math::gamma_quantile_c(alpha/2,N+1,1) );
    if( N!=0 ){
      gr_data->SetPointEYlow(i, N-L );
      gr_data->SetPointEXlow(i, 0);
      gr_data->SetPointEYhigh(i, U-N );
      gr_data->SetPointEXhigh(i, 0);
      err_down_tmp.push_back(N-L);
      err_up_tmp.push_back(U-N);
     }
    else{
      double zerodata_err_low = 0.1;
      double zerodata_err_high = 1.8;

      double xlow = gr_data->GetX()[i]-gr_data->GetEXlow()[i];
      double xhigh = gr_data->GetX()[i]+gr_data->GetEXhigh()[i];
      if(ZeroDataCheckCut(xlow,xhigh)){
        zerodata_err_low = 0.;
        zerodata_err_high = 0.;
      }

      gr_data->SetPointEYlow(i, zerodata_err_low);
      gr_data->SetPointEXlow(i, 0.);
      gr_data->SetPointEYhigh(i, zerodata_err_high);
      gr_data->SetPointEXhigh(i, 0.);
      err_down_tmp.push_back(zerodata_err_low);
      err_up_tmp.push_back(zerodata_err_high);
    }
  }
  gr_data->SetLineWidth(2.0);
  gr_data->SetMarkerSize(0.);
  gr_data->SetMarkerColor(kBlack);
  gr_data->SetLineColor(kBlack);
  hist_data->Draw("phistsame");
  gr_data->Draw("p0same");

  //==== ymax
  double AutoYmax = max( GetMaximum(gr_data), GetMaximum(mc_allerror) );
  hist_empty->GetYaxis()->SetRangeUser( Ymin, YmaxScale*AutoYmax );

  //==== legend
  legend->AddEntry(mc_allerror, "Stat.+syst. uncert.", "f");
  if(DrawData){
    legend->AddEntry(hist_data, "Data", "pe");
  }
  else{
    legend->AddEntry(hist_data, "Total background", "pe");
  }
  if(drawratio.at(i_cut)) c1_up->cd();
  draw_legend(legend, DrawData);
  hist_empty->Draw("axissame");

  //==== Ratio

  TString this_xtitle = x_title[i_var];
  if(histname[i_var]=="WRCand_Mass"){
    if(histname_suffix[i_cut].Contains("Resolved")){
      this_xtitle = "m_{lljj} (GeV)";
    }
    else if(histname_suffix[i_cut].Contains("Boosted")){
      this_xtitle = "m_{lJ} (GeV)";
    }
  }

  if(!drawratio.at(i_cut)){
    hist_axis(hist_empty);
    hist_empty->GetXaxis()->SetTitle(this_xtitle);
  }
  else{
    //==== MC-DATA
    c1_down->cd();
    TString name_suffix = hist_data->GetName();
    TH1D *ratio_point = (TH1D *)hist_data->Clone();
    ratio_point->SetName(name_suffix+"_central");

    TH1D *hist_empty_bottom = (TH1D *)ratio_point->Clone();
    hist_empty_bottom->GetYaxis()->SetRangeUser(0.,2.0);
    hist_empty_bottom->SetNdivisions(504,"Y");
    //hist_empty_bottom->GetYaxis()->SetRangeUser(0,YmaxScale*GetMaximum(ratio_point,0.));
    //hist_empty_bottom->GetYaxis()->SetRangeUser(0,1.9);
    hist_empty_bottom->GetXaxis()->SetTitle(this_xtitle);
    hist_empty_bottom->GetYaxis()->SetTitle("#frac{Obs.}{Pred.}");
    hist_empty_bottom->SetFillColor(0);
    hist_empty_bottom->SetMarkerSize(0);
    hist_empty_bottom->SetMarkerStyle(0);
    hist_empty_bottom->SetLineColor(kWhite);
    hist_empty_bottom->Draw("axis");
    hist_axis(hist_empty, hist_empty_bottom);

    TH1D *tmp_ratio_point = (TH1D *)hist_data->Clone();
    //==== just to get the ratio central value
    tmp_ratio_point->Divide(mc_staterror);
    TGraphAsymmErrors *gr_ratio_point = new TGraphAsymmErrors(tmp_ratio_point);
    gr_ratio_point->SetName("gr_"+name_suffix+"_central");
    gr_ratio_point->SetLineWidth(2.0);
    gr_ratio_point->SetMarkerSize(0.);
    gr_ratio_point->SetLineColor(kBlack);

    //==== again, stat is histogram
    TH1D *ratio_staterr = (TH1D *)hist_data->Clone();
    ratio_staterr->SetName(name_suffix+"_staterr");
    //==== syst is graph
    TGraphAsymmErrors *ratio_allerr = (TGraphAsymmErrors *)mc_allerror->Clone();
    ratio_allerr->SetName(name_suffix+"_allerr");

    for(int i=1; i<=ratio_point->GetXaxis()->GetNbins(); i++){
      //==== FIXME for zero? how?
      if(mc_staterror->GetBinContent(i)!=0){

        //==== ratio point
        //==== BinContent = Data/Bkgd
        //==== BinError = DataError/Bkgd
        ratio_point->SetBinContent( i, ratio_point->GetBinContent(i) / mc_staterror->GetBinContent(i) );
        ratio_point->SetBinError ( i, ratio_point->GetBinError(i) / mc_staterror->GetBinContent(i) );

        if(err_down_tmp.at(i-1)  !=0.) {
          gr_ratio_point->SetPointEYlow(i-1, err_down_tmp.at(i-1) / mc_staterror->GetBinContent(i) );
          gr_ratio_point->SetPointEXlow(i-1, 0);
          gr_ratio_point->SetPointEYhigh(i-1, err_up_tmp.at(i-1) / mc_staterror->GetBinContent(i));
          gr_ratio_point->SetPointEXhigh(i-1, 0);
        }
        else{
          gr_ratio_point->SetPointEYlow(i-1, 0);
          gr_ratio_point->SetPointEXlow(i-1, 0);
          gr_ratio_point->SetPointEYhigh(i-1, 1.8 / mc_staterror->GetBinContent(i));
          gr_ratio_point->SetPointEXhigh(i-1, 0);
        }
        //==== ratio staterr
        //==== BinContent = 1
        //==== BinError = Bkgd(Stat)Error/Bkgd
        ratio_staterr->SetBinContent( i, 1. );
        ratio_staterr->SetBinError( i, mc_staterror->GetBinError(i)/ mc_staterror->GetBinContent(i) );
        //==== ratio allerr
        //==== BinContent = 1
        //==== BinError = Bkgd(Stat+Syst)Error/Bkgd

        ratio_allerr->SetPoint(i-1,mc_staterror->GetXaxis()->GetBinCenter(i), 1.); 
        ratio_allerr->SetPointEYhigh( i-1, ratio_allerr->GetErrorYhigh(i-1) / mc_staterror->GetBinContent(i) );
        ratio_allerr->SetPointEYlow( i-1,  ratio_allerr->GetErrorYlow(i-1) / mc_staterror->GetBinContent(i) );
      }
      else if(mc_staterror->GetBinContent(i)==0 && ratio_point->GetBinContent(i)==0){
        ratio_point->SetBinContent( i, 0 );
        ratio_point->SetBinError ( i, 0 );
        gr_ratio_point->SetPoint(i-1, 0, 0);
        gr_ratio_point->SetPointEYlow(i-1, 0);
        gr_ratio_point->SetPointEXlow(i-1, 0);
        gr_ratio_point->SetPointEYhigh(i-1, 0);
        gr_ratio_point->SetPointEXhigh(i-1, 0);
        ratio_staterr->SetBinContent( i, 1. );
        ratio_staterr->SetBinError( i, 0);

        ratio_allerr->SetPoint(i-1,mc_staterror->GetXaxis()->GetBinCenter(i), 1.);
        ratio_allerr->SetPointEYhigh( i-1, 0. );
        ratio_allerr->SetPointEYlow( i-1, 0. );

      }
      //==== If bkgd <= 0
      else{
        double this_max_ratio = 5.0;
        double this_data = ratio_point->GetBinContent(i);
        double this_data_err = ratio_point->GetBinError(i);

        ratio_point->SetBinContent( i, this_max_ratio );
        ratio_point->SetBinError ( i, this_data_err*this_max_ratio/this_data );

        double tmp_x, tmp_y;
        gr_ratio_point->GetPoint(i-1, tmp_x, tmp_y);
        gr_ratio_point->SetPoint(i-1, tmp_x, this_max_ratio);
        gr_ratio_point->SetPointEYlow(i-1, err_down_tmp.at(i-1)*this_max_ratio/this_data);
        gr_ratio_point->SetPointEXlow(i-1, 0);
        gr_ratio_point->SetPointEYhigh(i-1, err_up_tmp.at(i-1)*this_max_ratio/this_data);
        gr_ratio_point->SetPointEXhigh(i-1, 0);
        ratio_staterr->SetBinContent( i, 1. );
        ratio_staterr->SetBinError( i, 0);

        ratio_allerr->SetPoint(i-1,mc_staterror->GetXaxis()->GetBinCenter(i), 1.);
        ratio_allerr->SetPointEYhigh( i-1, 0. );
        ratio_allerr->SetPointEYlow( i-1, 0. );

      }
    }
    double this_ratio_min = min(0.8,1.1*GetMinimum(ratio_point,0.));
    double this_ratio_max = max(1.2,1.1*GetMaximum(ratio_point,0.));
    hist_empty_bottom->GetYaxis()->SetRangeUser(0.5,1.5);
    //hist_empty_bottom->GetYaxis()->SetRangeUser(this_ratio_min,this_ratio_max);

    ratio_allerr->SetFillColor(kGray);
    ratio_allerr->SetFillStyle(1001);
    ratio_allerr->SetMarkerSize(0);
    ratio_allerr->SetMarkerStyle(0);
    ratio_allerr->SetLineColor(kWhite);
    ratio_allerr->Draw("E2same");

    ratio_staterr->SetFillColor(kOrange+2);
    ratio_staterr->SetMarkerSize(0);
    ratio_staterr->SetMarkerStyle(0);
    ratio_staterr->SetLineColor(kWhite);
    ratio_staterr->Draw("E2same");

    ratio_point->Draw("p9histsame");
    gr_ratio_point->Draw("p0same");

    TLegend *lg_ratio = new TLegend(0.7, 0.8, 0.9, 0.9);
    //lg_ratio->SetFillStyle(0);
    //lg_ratio->SetBorderSize(0);
    lg_ratio->SetNColumns(2);
    lg_ratio->AddEntry(ratio_staterr, "Stat.", "f");
    lg_ratio->AddEntry(ratio_allerr, "Stat.+syst.", "f");
    //lg_ratio->AddEntry(ratio_point, "Obs./Pred.", "p");
    lg_ratio->Draw();

    hist_empty_bottom->Draw("axissame");

    //==== y=1 line
    g1->Draw("same");
  }

  if(DoDebug) cout << "[draw_canvas] " << "plots are all drawn" << endl;

  //==== write lumi on the top
  c1->cd();

  if(LeptonChannels.at(i_cut)!=20){
    TLatex latex_CMSPriliminary, latex_Lumi;
    latex_CMSPriliminary.SetNDC();
    latex_Lumi.SetNDC();
    latex_CMSPriliminary.SetTextSize(0.035);
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");

    latex_Lumi.SetTextSize(0.035);
    latex_Lumi.SetTextFont(42);
    latex_Lumi.DrawLatex(0.72, 0.96, TotalLumi()+" fb^{-1} (13 TeV)");

    TString str_channel = GetStringChannelRegion(LeptonChannels.at(i_cut), RegionType.at(i_cut));
    TLatex channelname;
    channelname.SetNDC();
    channelname.SetTextSize(0.037);
    channelname.DrawLatex(0.2, 0.88, str_channel);
  }
  else{
    //==== This is for Paper
    TLatex latex_CMSPriliminary, latex_Lumi;
    latex_CMSPriliminary.SetNDC();
    latex_Lumi.SetNDC();
    latex_CMSPriliminary.SetTextSize(0.050);

    latex_CMSPriliminary.DrawLatex(0.20, 0.89, "#font[62]{CMS}");

    latex_Lumi.SetTextSize(0.035);
    latex_Lumi.SetTextFont(42);
    latex_Lumi.DrawLatex(0.72, 0.96, TotalLumi()+" fb^{-1} (13 TeV)");

    TLatex latex_eemmem;
    latex_eemmem.SetNDC();
    latex_eemmem.SetTextSize(0.037);
    latex_eemmem.DrawLatex(0.2, 0.85, "ee+#mu#mu+e#mu");

    TString str_channel = GetStringChannelRegion(LeptonChannels.at(i_cut), RegionType.at(i_cut));
    TLatex channelname;
    channelname.SetNDC();
    channelname.SetTextSize(0.037);
    channelname.DrawLatex(0.65, 0.45, str_channel);

  }

  if(DoDebug) cout << "[draw_canvas] TLatex all done" << endl;
  mkdir(thiscut_plotpath);
  if(DoDebug) cout << "[draw_canvas] output directory created : " << thiscut_plotpath << endl;
  c1->SaveAs(thiscut_plotpath+"/"+histname[i_var]+"_"+histname_suffix[i_cut]+".pdf");
  c1->SaveAs(thiscut_plotpath+"/"+histname[i_var]+"_"+histname_suffix[i_cut]+".png");

  delete legend;
  delete c1;
}

int Plotter::n_rebin(){
  
  TString cut = histname_suffix[i_cut];
  TString var = histname[i_var];
  
  map< TString, int >::iterator it = temp_rebins.find( var );
  if( it != temp_rebins.end() ){
    int TORETURN = it->second;
    //temp_rebins.erase( it );
    if(DoDebug) cout << "cut = " << cut << ", var = " << var << " => rebins = " << TORETURN << endl;
    return TORETURN;
  }
  else return 1;
  
}

double Plotter::y_max(){
  
  TString cut = histname_suffix[i_cut];
  TString var = histname[i_var];

  map< TString, double >::iterator it = temp_y_maxs.find( var );
  if( it != temp_y_maxs.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => rebins = " << temp_rebins[cut] << endl;
    double TORETURN = it->second;
    //temp_y_maxs.erase( it );
    return TORETURN;
  }
  else return default_y_max;

}

void Plotter::SetXaxisRange(TH1D* hist){
  
  TString cut = histname_suffix[i_cut];
  TString var = histname[i_var];
  
  double this_x_min = hist->GetXaxis()->GetBinLowEdge(1);
  double this_x_max = hist->GetXaxis()->GetBinUpEdge( hist->GetXaxis()->GetNbins() );

  map< TString, double >::iterator it = temp_x_mins.find( var );
  if( it != temp_x_mins.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => x_min = " << temp_x_mins[var] << endl;
    this_x_min = it->second;
    //temp_x_mins.erase( it );
  }
  it = temp_x_maxs.find( var );
  if( it != temp_x_maxs.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => x_max = " << temp_x_maxs[var] << endl;
    this_x_max = it->second;
    //temp_x_maxs.erase( it );
  }
  
  hist->GetXaxis()->SetRangeUser(this_x_min, this_x_max);
}

void Plotter::SetXaxisRange(THStack* mc_stack){
  
  TString cut = histname_suffix[i_cut];
  TString var = histname[i_var];
  
  double this_x_min = mc_stack->GetXaxis()->GetBinLowEdge(1);
  double this_x_max = mc_stack->GetXaxis()->GetBinUpEdge( mc_stack->GetXaxis()->GetNbins() );
  
  map< TString, double >::iterator it = temp_x_mins.find( var );
  if( it != temp_x_mins.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => x_min = " << temp_x_mins[var] << endl;
    this_x_min = it->second;
    //temp_x_mins.erase( it );
  }
  it = temp_x_maxs.find( var );
  if( it != temp_x_maxs.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => x_max = " << temp_x_maxs[var] << endl;
    this_x_max = it->second;
    //temp_x_maxs.erase( it );
  }
 
  mc_stack->GetXaxis()->SetRangeUser(this_x_min, this_x_max);
}

void Plotter::SetXaxisRangeBoth(THStack* mc_stack, TH1D* hist){

  TString cut = histname_suffix[i_cut];
  TString var = histname[i_var];
  
  double this_x_min = mc_stack->GetXaxis()->GetBinLowEdge(1);
  double this_x_max = mc_stack->GetXaxis()->GetBinUpEdge( mc_stack->GetXaxis()->GetNbins() );
  
  map< TString, double >::iterator it = temp_x_mins.find( var );
  if( it != temp_x_mins.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => x_min = " << temp_x_mins[var] << endl;
    this_x_min = it->second;
    //temp_x_mins.erase( it );
  }
  it = temp_x_maxs.find( var );
  if( it != temp_x_maxs.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => x_max = " << temp_x_maxs[var] << endl;
    this_x_max = it->second;
    //temp_x_maxs.erase( it );
  }

  mc_stack->GetXaxis()->SetRangeUser(this_x_min, this_x_max);
  hist->GetXaxis()->SetRangeUser(this_x_min, this_x_max);
}

TH1D* Plotter::MakeOverflowBin(TH1D* hist){

  //==== 0    1                                    n_bin_origin
  //====      |---------------------------------------|
  //====             bin_first      bin_last
  //====                |-------------|  
  //==== |   |                                        |   |
  //==== under                                         over
  //==== flow                                          flow
  //==== |<------------>|             |<----------------->|
  //====  all underflow                   all overflow

  //==== Original NBins
  int n_bin_origin = hist->GetXaxis()->GetNbins();
  //==== Changed NBins
  int bin_first = hist->GetXaxis()->GetFirst();
  int bin_last = hist->GetXaxis()->GetLast();
  int n_bin_inrange = bin_last-bin_first+1;
  
  double x_first_lowedge = hist->GetXaxis()->GetBinLowEdge(bin_first);
  double x_last_upedge = hist->GetXaxis()->GetBinUpEdge(bin_last);

  double Allunderflows = hist->Integral(0, bin_first-1);
  double Allunderflows_error = hist->GetBinError(0);
  Allunderflows_error = Allunderflows_error*Allunderflows_error;
  for(unsigned int i=1; i<=bin_first-1; i++){
    Allunderflows_error += (hist->GetBinError(i))*(hist->GetBinError(i));
  }
  Allunderflows_error = sqrt(Allunderflows_error);

  double Alloverflows = hist->Integral(bin_last+1, n_bin_origin+1);
  double Alloverflows_error = hist->GetBinError(n_bin_origin+1);
  Alloverflows_error = Alloverflows_error*Alloverflows_error;
  for(unsigned int i=bin_last+1; i<=n_bin_origin; i++){
    Alloverflows_error += (hist->GetBinError(i))*(hist->GetBinError(i));
  }
  Alloverflows_error = sqrt(Alloverflows_error);

  //==== Make X-bin array
  Double_t temp_xbins[n_bin_inrange+1];
  int counter=0;
  for(int i=bin_first;i<=bin_last;i++){
    temp_xbins[counter] = hist->GetXaxis()->GetBinLowEdge(i);
    counter++;
  }
  temp_xbins[n_bin_inrange+1-1] = hist->GetXaxis()->GetBinUpEdge(bin_last);
  const Double_t *xcopy=temp_xbins;

  TH1D *hist_out = new TH1D(hist->GetName(), hist->GetTitle(), n_bin_inrange, xcopy);
  for(unsigned int i=1; i<=n_bin_inrange; i++){
    double this_content = hist->GetBinContent(bin_first-1+i);
    double this_error = hist->GetBinError(bin_first-1+i);
    //cout << "["<<hist_out->GetXaxis()->GetBinLowEdge(i)<<", "<<hist_out->GetXaxis()->GetBinUpEdge(i)<<"] : "<<this_content<<endl;

    //==== underflows
    if(i==1){
      this_content += Allunderflows;
      this_error = TMath::Sqrt( this_error*this_error + Allunderflows_error*Allunderflows_error );
    }

    //==== overflows
    if(i==n_bin_inrange){
      this_content += Alloverflows;
      this_error = TMath::Sqrt( this_error*this_error + Alloverflows_error*Alloverflows_error );
    }
    
    hist_out->SetBinContent(i, this_content);
    hist_out->SetBinError(i, this_error);
    
  }
  
  return hist_out;
  
}

TString Plotter::DoubleToString(double dx){

  //cout << "[Plotter::DoubleToString] var = " << histname[i_var] << endl;
  //cout << "[Plotter::DoubleToString] unit = " << units[i_var] << endl;
  //cout << "[Plotter::DoubleToString] dx = " << dx << endl;

  //==== onebin
  if(units[i_var]=="int"){
    return "Events";
  }
  else if(histname[i_var].Contains("WRCand_Mass")){ //FIXME need rebin?
    return "Events / bin";
  }
  else{

    int dx_int = int(dx);
    int dx_first_two_digits = int((dx-dx_int)*100);
    int dx_first_three_digits = int((dx-dx_int)*1000);

    //==== has integer
    if(dx_int!=0){
      //==== but no digits
      if(dx_first_two_digits==0){
        return "Events / "+TString::Itoa(dx_int,10)+" "+units[i_var];
      }
      //=== also has digits
      else{
        return "Events / "+TString::Itoa(dx_int,10)+"."+TString::Itoa(dx_first_two_digits,10)+" "+units[i_var];
      }
    }
    //=== no integer
    else{
      return "Events / 0."+TString::Itoa(dx_first_two_digits,10)+" "+units[i_var];
    }
  }

  return "Events";

}

void Plotter::mkdir(TString path){
  
  if( !gSystem->mkdir(path, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << path << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
  
}

void Plotter::make_plot_directory(){

  //TString ENV_PLOT_PATH = getenv("PLOT_PATH");
  //plotpath = ENV_PLOT_PATH+"/"+data_class;

  for(unsigned int i=0; i<samples_to_use.size(); i++){
    //if(samples_to_use.at(i).Contains("fake")) plotpath = plotpath+"/use_FR_method/"+samples_to_use.at(i);
    if(samples_to_use.at(i).Contains("FromEMu")) plotpath = plotpath+"/use_EMu_method/"+samples_to_use.at(i);
  }

  cout
  << endl
  << "###################################################" << endl
  << "OUTPUT ===> " << plotpath << endl
  << "###################################################" << endl
  << endl;
  
  mkdir(plotpath);
  
}

void Plotter::MakeTexFile(map< TString, TH1D * > hs){

  TString texfilepath = thiscut_plotpath+"/tex/";
  mkdir(texfilepath);

  ofstream ofile_tex(texfilepath+"/Yields.tex",ios::trunc);
  ofile_tex.setf(ios::fixed,ios::floatfield);
  ofile_tex << "\\documentclass[10pt]{article}" << endl;
  ofile_tex << "\\usepackage{epsfig,subfigure,setspace,xtab,xcolor,array,colortbl}" << endl;
  ofile_tex << "\\begin{document}" << endl;
  ofile_tex << "\\input{"+texfilepath+"/Table.txt}" << endl;
  ofile_tex << "\\end{document}" << endl;

  ofstream ofile(texfilepath+"/Table.txt",ios::trunc);
  ofile.precision(3);
  ofile.setf(ios::fixed,ios::floatfield); 
  ofile << "\\begin{table}[!tbh]" << endl;
  ofile << "  \\caption{" << endl;
  ofile << "    Yields" << endl;
  ofile << "  }" << endl;
  ofile << "  \\begin{center}" << endl;
  ofile << "    \\begin{tabular}{c|c}" << endl;
  ofile << "\\hline" << endl;
  ofile << " & Yields \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;

  TH1D *hist_bkgd = new TH1D("hist_bkgd", "", 1., 0., 1.);
  bool HasSignal = false;
  for(map< TString, TH1D * >::iterator it = hs.begin(); it != hs.end(); it++){
    TString name = it->first;

    if(name == "X + #gamma") name = "$X + \\gamma$";
    if(name == "Z + #gamma") name = "Z $+ \\gamma$";
    if(name == "W + #gamma") name = "W $+ \\gamma$";
    if(name == "top + #gamma") name = "top $+ \\gamma$";
    if(name.Contains("data")) continue;
    if(name.Contains("Signal")){
      HasSignal = true;
      continue;
    }
    TH1D *h_bkgd = it->second;
    ofile << name << " & $"<<h_bkgd->GetBinContent(1)<<" \\pm "<<h_bkgd->GetBinError(1)<<"$ \\\\" << endl;
    hist_bkgd->Add(h_bkgd);
  }
  ofile << "\\hline" << endl;
  ofile << "Total & $" << hist_bkgd->GetBinContent(1) << " \\pm " << hist_bkgd->GetBinError(1) << "$ \\\\" << endl;
  ofile << "\\hline" << endl;

  TH1D *hist_data = hs["data"];
  if(hist_data){
   ofile << "Data & $" << hist_data->GetBinContent(1) << " \\pm " << hist_data->GetBinError(1) << "$ \\\\" << endl;
   ofile << "\\hline" << endl;
  }
  else{
   TH1D *hist_empty = new TH1D("hist_data", "", 1, 0., 1.);
   hist_data = (TH1D*)hist_empty->Clone();
   ofile << "Data & $" << 0 << " \\pm " << 0 << "$ \\\\" << endl;
   ofile << "\\hline" << endl;
  }

  double signif = (hist_data->GetBinContent(1) - hist_bkgd->GetBinContent(1)) / sqrt( (hist_bkgd->GetBinError(1))*(hist_bkgd->GetBinError(1)) + (hist_data->GetBinError(1))*(hist_data->GetBinError(1)) );

  ofile << "Significance & $" <<signif<<" \\sigma$ \\\\" << endl;

  if(HasSignal){
    ofile << "\\hline" << endl;
    for(map< TString, TH1D * >::iterator it = hs.begin(); it != hs.end(); it++){
    TString name = it->first;
    if(name.Contains("Signal")){
      TH1D *h_bkgd = it->second;

      //==== name = PairMuMuZ1000N500
      ofile << name+" & $" << h_bkgd->GetBinContent(1) << " \\pm " << h_bkgd->GetBinError(1) << "$ \\\\" << endl;
      }
    }
  }

  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "    \\end{tabular}" << endl;
  ofile << "  \\end{center}" << endl;
  ofile << "\\end{table}" << endl;

  system("latex "+texfilepath+"/Yields.tex");
  system("dvipdf Yields.dvi");
  system("rm *aux");
  system("rm *log");
  system("rm *dvi");
  system("mv Yields.pdf "+texfilepath);

}

TString Plotter::TotalLumi(){

  if(DataYear==2016) return "35.92";
  else if(DataYear==2017) return "41.53";
  else if(DataYear==2018) return "59.74";
  else{
    cout << "[Plotter::TotalLumi] Wrong DataYear" << DataYear << endl;
    exit(EXIT_FAILURE);
    return "35.9";
  }
  

}

double Plotter::LumiError(){

  if(DataYear==2016) return 0.025;
  else if(DataYear==2017) return 0.023;
  else if(DataYear==2018) return 0.025;
  else{
    cout << "[Plotter::LumiError] Wrong DataYear" << DataYear << endl;
    exit(EXIT_FAILURE);
    return 0.;
  }

}

TString Plotter::GetStringChannelRegion(int A, int B){

  if(B==0) return "";

  //==== channel type

  TString channel = "";
  //==== A%10 : # of leptons
  //==== Last digit of A = 1 : ee
  //====                   2 : mm
  //====                   3 : em

  if(A==10) channel = "l";
  else if(A==11) channel = "e";
  else if(A==12) channel = "#mu";
  //==== Lepton Combined
  else if(A==20) channel = "ee,#mu#mu,e#mu";
  //==== mumu
  else if(abs(A)==21) channel = "ee";
  //==== ee
  else if(abs(A)==22) channel = "#mu#mu";
  //==== emu
  else if(abs(A)==23) channel = "e#mu";
  //==== three lepton
  else if(abs(A)==30) channel = "3l";
  //==== four lepton
  else if(abs(A)==40) channel = "4l";
  else{
  }

  //==== SS(+) of OS(-)
  if(abs(A)>=20 && abs(A)<=29){
    //if(A>0) channel = channel+" (SS)";
    //else    channel = channel+" (OS)";
  }

  TString region = "";

  //==== B%10 = 1 : Resolved
  //==== Last digit of B = 0 : SR
  //====                 = 1 : DYCR
  //====                 = 2 : EMu
  //====                 = 3 : LowWR CR

  //==== B%10 = 2 : Boosted
  //==== Last digit of B = 0 : SR
  //===                  = 1 : DYCR
  //====                 = 2 : EMu with el-jet
  //====                 = 3 : EMu with mu-jet 
  //====                 = 4 : LowWR
  //====                 = 5 : EMu LowWR with el-jet
  //====                 = 6 : EMu LowWR with mu-jet

  if(abs(B)==10) region = "Resolved SR";
  else if(abs(B)==11) region = "Resolved DY CR";
  else if(abs(B)==12) region = "Resolved e#mu CR";
  else if(abs(B)==13) region = "Resolved Low m(lljj)";

  else if(abs(B)==20) region = "Boosted SR";
  else if(abs(B)==21) region = "Boosted DY CR";
  else if(abs(B)==22) region = "Boosted e#mu w/ e-Jet";
  else if(abs(B)==23) region = "Boosted e#mu w/ #mu-Jet";
  else if(abs(B)==24) region = "Boosted Low m(lJ)";
  else if(abs(B)==25) region = "Boosted e#mu Low m(lJ) w/ e-Jet";
  else if(abs(B)==26) region = "Boosted e#mu Low m(lJ) w/ #mu-Jet";


  else if(abs(B)==30) region = "OnZ";

  else{

  }


  if(A==20) return region;
  else return "#splitline{"+channel+"}{"+region+"}";


}

bool Plotter::ZeroDataCheckCut(double xlow, double xhigh){

  if(histname_suffix[i_cut].Contains("_Low_")){
    if(histname[i_var]=="m_lljj_lljjWclosest"){
      if(xlow>=300) return true;
    }
    if(histname[i_var]=="m_llj"){
      if(xlow>=300) return true;
    }
  }

  return false;

}

vector<double> Plotter::GetRebinZeroBackground(THStack *mc_stack, TH1D *mc_staterror, TH1D *mc_allerror, TH1D *hist_data, vector<TH1D *> &hist_signal){

  int original_nbins = mc_allerror->GetXaxis()->GetNbins();
  vector<double> original_binval;
  for(int i=1; i<=original_nbins; i++){
    original_binval.push_back(mc_allerror->GetXaxis()->GetBinLowEdge(i));
  }
  original_binval.push_back(mc_allerror->GetXaxis()->GetBinUpEdge(original_nbins));

  //==== [original_binval]
  //====   99    99    77    22     0     1     3
  //==== 0-----1-----2-----3-----4-----5-----6-----7
  //====   (1)   (2)   (3)   (4)   (5)   (6)   (7)
  //==== [new_binval]
  //==== 0-----1-----2-----3-----4-----------------5

  vector<double> new_binval;
  for(int i=0; i<original_binval.size(); i++){
    double thisval = original_binval.at(i);
    if( mc_allerror->GetXaxis()->GetBinUpEdge(i) <= 500 ){
      new_binval.push_back(thisval);
    }
    else{
      if(mc_allerror->GetBinContent(i+1)<=0.){
        new_binval.push_back( original_binval.at( original_binval.size()-1 ) );
        break;
      }
      else{
        new_binval.push_back(thisval);
      }
    }
  }
/*
  //==== DEBUG
  for(unsigned int j=1; j<original_binval.size()-1; j++){
    cout << "["<<mc_allerror->GetXaxis()->GetBinLowEdge(j) << ", " << mc_allerror->GetXaxis()->GetBinUpEdge(j) << "] " << mc_allerror->GetBinContent(j) << endl;
  }
  for(unsigned int i=0; i<new_binval.size(); i++){
    cout << new_binval.at(i) << ",";
  }
  cout << endl;
*/


  return new_binval;

}

TH1D *Plotter::Rebin(TH1D *hist){

  if(histname[i_var]=="WRCand_Mass" && !( histname_suffix[i_cut].Contains("LowWRCR") ) ){

    hist = RebinWRMass(hist, histname_suffix[i_cut]);

/*
    int lastbin = hist->GetXaxis()->GetNbins();

    vector<double> vec_bins = {0, 800, 1000, 1200, 1400, 1600, 2000, 2400, 2800, 3200, 4000, 8000};
    if(histname_suffix[i_cut].Contains("Boosted")){
       vec_bins = {0, 800, 1000, 1200, 1600, 2000, 8000};
    };

    const int n_bin = vec_bins.size()-1;
    double ptArray[n_bin+1];
    for(int zzz=0;zzz<vec_bins.size();zzz++){
      ptArray[zzz] = vec_bins.at(zzz);
    }
    hist = (TH1D *)hist->Rebin(n_bin, hist->GetName(), ptArray);
*/

  }
  else{
    hist->Rebin( n_rebin() );
  }

  return hist;

}
