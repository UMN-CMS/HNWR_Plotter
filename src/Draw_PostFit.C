#include "Plotter.cc"
#include <fstream>

void Draw_PostFit(int Year=2016, int WhichRegion=0){

  bool DrawFittedNuisance = true;

  //==============
  //==== get env
  //==============
  
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  //====================
  //==== decalre class
  //====================
  
  Plotter m;
  m.DoDebug = false;
  m.DataYear = Year;
  m.DrawPostFit = true;

  //=====================
  //==== set data class
  //=====================

  TString str_Year = TString::Itoa(Year,10);
  m.data_class = dataset+"/PostFit/"+str_Year+"/";
  if(Year<0){
    str_Year = "YearCombined";
    m.data_class = dataset+"/PostFit/";
  }

  //================================
  //==== set prefixes and suffixes
  //================================
  
  m.filename_prefix = "HNWRAnalyzer";
  m.filename_suffix = ".root";
  m.filename_skim = "_SkimTree_LRSMHighPt";
  
  //=========================
  //==== set sample mapping
  //=========================

  m.map_sample_string_to_list["ZJets_MG_HT_Reweighted"]     = {"DYJets_MG_HT_Reweighted"};
  m.map_sample_string_to_list["WJets_MG_HT"] = {"WJets_MG_HT"};
  m.map_sample_string_to_list["Multiboson"] = {"VV","VVV"};
  m.map_sample_string_to_list["SingleTop"] = {"SingleTop"};
  m.map_sample_string_to_list["ttX"] = {"ttX"};
  m.map_sample_string_to_list["ttbar"] = {"TTLX_powheg"};

  m.map_sample_string_to_legendinfo["ZJets_MG_HT_Reweighted"] = make_pair("Z+Jets", kYellow);
  m.map_sample_string_to_legendinfo["WJets_MG_HT"] = make_pair("W+Jets", 870);
  m.map_sample_string_to_legendinfo["Multiboson"] = make_pair("Multiboson", kSpring-1);
  m.map_sample_string_to_legendinfo["SingleTop"] = make_pair("Single top", kRed+2);
  m.map_sample_string_to_legendinfo["ttX"] = make_pair("ttV", kOrange+2);
  m.map_sample_string_to_legendinfo["ttbar"] = make_pair("t#bar{t}", kRed);

  if(Year<0){

    m.map_sample_string_to_list["ZJets_MG_HT_Reweighted"]  = {
      "2016 DYJets10to50_MG_Reweighted", "2016 DYJets_MG_HT_Reweighted",
      "2017 DYJets10to50_MG_Reweighted", "2017 DYJets_MG_HT_Reweighted",
      "2018 DYJets10to50_MG_Reweighted", "2018 DYJets_MG_HT_Reweighted",
    };
    m.map_sample_string_to_list["WJets_MG_HT"] = {
      "2016 WJets_MG_HT",
      "2017 WJets_MG_HT",
      "2018 WJets_MG_HT",
    };
    m.map_sample_string_to_list["Multiboson"] = {
      "2016 VV",
      "2017 VV",
      "2018 VV",
      "2016 VVV",
      "2017 VVV",
      "2018 VVV",
    };
    m.map_sample_string_to_list["SingleTop"] = {
      "2016 SingleTop",
      "2017 SingleTop",
      "2018 SingleTop",
    };
    m.map_sample_string_to_list["ttX"] = {
      "2016 ttX",
      "2017 ttX",
      "2018 ttX",
    };
    m.map_sample_string_to_list["ttbar"] = {
      "2016 TTLX_powheg",
      "2017 TTLX_powheg",
      "2018 TTLX_powheg",
    };

    m.map_sample_string_to_legendinfo["ZJets_MG_HT_Reweighted"] = make_pair("Z+Jets", kYellow);
    m.map_sample_string_to_legendinfo["WJets_MG_HT"] = make_pair("W+Jets", 870);
    m.map_sample_string_to_legendinfo["Multiboson"] = make_pair("Multiboson", kSpring-1);
    m.map_sample_string_to_legendinfo["SingleTop"] = make_pair("Single top", kRed+2);
    m.map_sample_string_to_legendinfo["ttX"] = make_pair("ttV", kOrange+2);
    m.map_sample_string_to_legendinfo["ttbar"] = make_pair("t#bar{t}", kRed);

  }

  //===============================
  //==== set and make sample list
  //===============================

  //==== _Di<Lepton>_<JetSel>_<ifOffZ>_<charge>

  //==== EMu
  if(WhichRegion==0){

    m.samples_to_use = {"Multiboson", "ttX", "SingleTop", "WJets_MG_HT", "ZJets_MG_HT_Reweighted", "ttbar"};

    m.histname_suffix = {

      //==== Resolved EMu
      "HNWR_EMu_Resolved_SR",

      //==== Boosted EMu
      "HNWR_SingleElectron_EMu_Boosted_CR",
      "HNWR_SingleMuon_EMu_Boosted_CR",
    };

  }

  //==== SR
  else if(WhichRegion==1){
    m.samples_to_use = {"WJets_MG_HT", "ttX", "Multiboson", "SingleTop", "ZJets_MG_HT_Reweighted", "ttbar"};

    m.histname_suffix = {

      "HNWR_SingleElectron_Resolved_SR",
      "HNWR_SingleElectron_Boosted_SR",

      "HNWR_SingleMuon_Resolved_SR",
      "HNWR_SingleMuon_Boosted_SR",

    };

  }

  //============================
  //==== set variables to draw
  //============================
  
  m.histname = {
    "NEvent", "nPileUp", "nPV", "N_VTX",
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
    "LSFFatJet_Size", "Lepton_Size", "OFLepton_Size",
  };

  m.x_title = {
    "# of events", "# of PU", "# of PV", "# of vtx",
    "Leading lepton p_{T} (GeV)", "Leading lepton #eta", "Leading lepton TrkRelIso",
    "Subleading lepton p_{T} (GeV)", "Subleading lepton #eta", "Subleading lepton TrkRelIso",
    "#DeltaR(l_{1},l_{2})",
    "Leading jet p_{T} (GeV)", "Leading jet #eta",
    "Subleading jet p_{T} (GeV)", "Subleading jet #eta",
    "Away AK8 jet p_{T} (GeV)", "Away AK8 jet #eta", "Away AK8 jet Mass", "Away AK8 jet SFMass",
    "p_{T} of dilepton (GeV)", "m(ll) (GeV)", "#DeltaR(ll)",
    "#slash{E}_{T}^{miss} (GeV)", "H_{T} (GeV)",
    "m_{T} (GeV)",
    "# of jets", "# of b-tagged jets",
    "m_{N} (GeV)", "m_{W_{R}} (GeV)",
    "p_{T} of N (GeV)", "p_{T} of W (GeV)",
    "# of merged jets", "# of leptons", "# of OF leptons",
  };

  m.units = {
    "int", "int", "int", "int",
    "GeV", "", "",
    "GeV", "", "",
    "",
    "GeV", "",
    "GeV", "",
    "GeV", "", "GeV", "GeV",
    "GeV", "GeV", "GeV",
    "GeV", "GeV",
    "GeV",
    "int", "int",
    "GeV", "GeV",
    "GeV", "GeV",
    "", "int", "int",
  };

  if(Year<0){
    m.histname = {
      "WRCand_Mass"
    };
    m.x_title = {
      "m_{WR} (GeV)",
    };
    m.units = {
      "GeV",
    };
  }

/*
  m.histname = {
    "ZCand_Mass",
  };
  m.x_title = {
    "m(ll) (GeV)",
  };
  m.units = {
    "GeV",
  };
*/

  m.histname = {
    "WRCand_Mass"
  };
  m.x_title = {
    "m_{WR} (GeV)",
  };
  m.units = {
    "GeV",
  };

/*
  m.histname = {
    "ZCand_Pt"
  };
  m.x_title = {
    "p_{T} of dilepton (GeV)",
  };
  m.units = {
    "GeV",
  };
*/

  for(unsigned int i=0; i<m.histname_suffix.size(); i++){

    TString this_region = m.histname_suffix.at(i);
    //==== PD
    if(this_region.Contains("SingleElectron")){
      m.PrimaryDataset.push_back("SingleElectron");
    }
    else if(this_region.Contains("SingleMuon")){
      m.PrimaryDataset.push_back("SingleMuon");
    }
    else if(this_region.Contains("EMu")){
      m.PrimaryDataset.push_back("SingleMuon");
    }
    else{
      cout << "ERROR : PD not correct" << endl;
      return;
    }

    if(this_region=="HNWR_SingleElectron_Resolved_SR"){
      m.LeptonChannels.push_back(21);
      m.RegionType.push_back(10);
    }
    else if(this_region=="HNWR_SingleElectron_Boosted_SR"){
      m.LeptonChannels.push_back(21);
      m.RegionType.push_back(20);
    }
    else if(this_region=="HNWR_SingleMuon_Resolved_SR"){
      m.LeptonChannels.push_back(22);
      m.RegionType.push_back(10);
    }
    else if(this_region=="HNWR_SingleMuon_Boosted_SR"){
      m.LeptonChannels.push_back(22);
      m.RegionType.push_back(20);
    }
    else if(this_region=="HNWR_EMu_Resolved_SR"){
      m.LeptonChannels.push_back(23);
      m.RegionType.push_back(12);
    }
    else if(this_region=="HNWR_SingleElectron_EMu_Boosted_CR"){
      m.LeptonChannels.push_back(23);
      m.RegionType.push_back(23);
    }
    else if(this_region=="HNWR_SingleMuon_EMu_Boosted_CR"){
      m.LeptonChannels.push_back(23);
      m.RegionType.push_back(22);
    }


    m.UseLogy.push_back(1);

    m.ApplyMCNormSF.push_back(false);

    if(WhichRegion==0){
      m.drawdata.push_back(true);
    }
    else{
      m.drawdata.push_back(false);
    }

    m.drawratio.push_back(true);

  }

/*
  //==== FIXME test
  m.histname = {"m_lljj_lljjWclosest", "m_llj"};
  m.x_title = {"m(l^{#pm}l^{#pm}W_{jet}) (GeV)", "m(l^{#pm}l^{#pm}W_{jet}) (GeV)"};
  m.units = {"GeV", "GeV"};
*/

  cout << "m.histname.size() = " <<  m.histname.size() << endl;
  cout << "m.x_title.size() = " << m.x_title.size() << endl;
  cout << "m.units.size() = " << m.units.size() << endl;
  if(m.histname.size()==m.x_title.size() && m.x_title.size()==m.units.size()){
    cout << "--> OKAY" << endl;
  }
  else{
    cout << "--> WRONG" << endl;
    return;
  }
  cout << "Histname\tXTitle\tUnit" << endl;
  for(unsigned int i=0; i<m.histname.size(); i++){
    cout << m.histname.at(i) << "\t" << m.x_title.at(i) << "\t" << m.units.at(i) << endl;
  }

  //====================
  //==== make bkg list
  //====================

  m.make_bkglist();

  //=====================
  //==== Fill MCNorm SF
  //=====================

  m.analysisInputs.SetMCSF(WORKING_DIR+"/data/"+dataset+"/"+str_Year+"/MCSF.txt", m.bkglist);

  //======================
  //==== Get Systematics
  //======================

  m.analysisInputs.SetCalculatedSysts(WORKING_DIR+"/data/"+dataset+"/"+str_Year+"/Syst.txt");

  //=============================
  //==== set signal mass points
  //=============================

  if(WhichRegion==1){
    TString xsectxtfilepath = WORKING_DIR+"/data/"+dataset+"/xsec_190705_GenXsecAN_eeANDmm.txt";

    LRSMSignalInfo lrsminfo;
    lrsminfo.GetMassMapsPlot();
    //vector<Color_t> colors_WR = {kGreen, kViolet, kGray, kOrange};
    vector<Color_t> colors_WR = {kBlack, kGray, kSpring};

    int it_sig=-1;
    for(map< double, vector<double> >::iterator it=lrsminfo.maps_WR_to_N.begin(); it!=lrsminfo.maps_WR_to_N.end(); it++){

      it_sig++;

      double m_WR = it->first;
      vector<double> this_m_Ns = it->second;

      for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

        double m_N = this_m_Ns.at(it_N);

        string line_xsec;
        ifstream in_xsec(xsectxtfilepath);
        double this_xsec = -1.;
        while(getline(in_xsec,line_xsec)){
          std::istringstream is( line_xsec );

          double mwr, mn, xsec;

          is >> mwr;
          is >> mn;
          is >> xsec;

          if(m_WR==mwr && m_N==mn){

            //==== applying k-factor
            double kfactor = GetKFactor(mwr, mn);
            cout << mwr << "\t" << mn << "\t" << xsec << "\t" << kfactor  << endl;
            this_xsec = xsec * kfactor;
            break;
          }

        }
        //cout << m_WR << "\t" << m_N << "\t" << this_xsec << endl;

        LRSMSignalInfo lrsminfo;
        lrsminfo.prod_channel="SchWR";
        lrsminfo.generator="MGFastSim";
        lrsminfo.lep_channel = "EE";
        lrsminfo.mass_WR = m_WR;
        lrsminfo.mass_N = m_N;
        lrsminfo.SetNames();

        m.signal_LRSMinfo.push_back(lrsminfo);
        m.signal_color.push_back(colors_WR.at(it_sig));
        m.signal_style.push_back(it_N+2);
        m.signa_xsec.push_back(this_xsec);
        m.signal_draw.push_back(true);


        //==== copy MuMu from EE
        lrsminfo.lep_channel = "MuMu";
        lrsminfo.SetNames();
        m.signal_LRSMinfo.push_back(lrsminfo);
        m.signal_color.push_back(colors_WR.at(it_sig));
        m.signal_style.push_back(it_N+2);
        m.signa_xsec.push_back(this_xsec);
        m.signal_draw.push_back(true);

      }

    }

    //=====================================
    //==== set signal mass for each class
    //=====================================

    for(unsigned int i=0; i<m.signal_LRSMinfo.size(); i++){
      LRSMSignalInfo this_lrsm = m.signal_LRSMinfo.at(i);

      double m_WR = this_lrsm.mass_WR;
      double m_N = this_lrsm.mass_N;

      if(m_N/m_WR < 0.3){
        m.map_class_to_LRSMSignalInfo[Plotter::Boosted].push_back( this_lrsm );
      }
      else{
        m.map_class_to_LRSMSignalInfo[Plotter::Resolved].push_back( this_lrsm );
      }

      //m.map_class_to_LRSMSignalInfo[Plotter::All].push_back( this_lrsm );

    }
    m.AllSignalClasses = {Plotter::Resolved, Plotter::Boosted};

  }

  //=============
  //==== rebins
  //=============

  TString CRorSR = "CR";
  if(WhichRegion==1) CRorSR = "SR";

  //==== script to generate rebins
  ofstream skeleton_rebins("./data/tmp_"+CRorSR+"_rebins.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_rebins
      //<< "m.rebins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<endl;
    }
  }
  skeleton_rebins.close();

  m.SetRebins(WORKING_DIR+"/data/"+dataset+"/"+str_Year+"/"+CRorSR+"_rebins.txt");

  //=============
  //==== y_maxs
  //=============
  
  //==== script to generate rebins
  ofstream skeleton_y_maxs("./data/tmp_"+CRorSR+"_yaxis.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_y_maxs
      //<< "  m.y_maxs[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<endl;
    }
  }
  skeleton_y_maxs.close();


  //==== default max
  m.default_y_max = 20.;
  m.default_y_min = 0.;

  m.SetYAxis(WORKING_DIR+"/data/"+dataset+"/"+str_Year+"/"+CRorSR+"_yaxis.txt");

  //=============
  //==== x_mins
  //=============

  //==== script to generate rebins
  ofstream skeleton_x_mins("./data/tmp_"+CRorSR+"_xaxis.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_x_mins
      //<< "  m.x_mins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<"\t"<<-999<<endl;
    }
  }
  skeleton_x_mins.close();

  m.SetXAxis(WORKING_DIR+"/data/"+dataset+"/"+str_Year+"/"+CRorSR+"_xaxis.txt");

  //===============
  //==== k-factor
  //===============
  
  m.k_factor = 1;
  
  //=================================
  //==== mixing at generation level
  //=================================
  
  m.log_of_generation_mixing = 1.;
  
  //===============================
  //==== prepare plot directories
  //===============================

  m.plotpath = ENV_PLOT_PATH+"/"+dataset+"/PostFit/"+str_Year+"/";

  m.make_plot_directory();
  
  //==========================
  //==== finally, draw plots
  //==========================
  
  m.draw_hist();
  
}






