#include "Plotter.cc"
#include <fstream>

void Draw_SR(int XXX=0, bool MakeShape=false){

  bool ScaleMC = false;

  //==============
  //==== get env
  //==============
  
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  //====================
  //==== decalre class
  //====================
  
  Plotter m;
  m.DoDebug = false;
  m.MakeShape = MakeShape;
  
  //=====================
  //==== set data class
  //=====================
  
  m.data_class = dataset+"/Regions/";
  
  //================================
  //==== set prefixes and suffixes
  //================================
  
  m.filename_prefix = "HNWRAnalyzer";
  m.filename_suffix = ".root";
  
  //=========================
  //==== set sample mapping
  //=========================
  
  m.map_sample_string_to_list["DY"] = {"DYJets_10to50_MG", "DYJets"};
  m.map_sample_string_to_list["WJets_MG"] = {"WJets_MG"};
  m.map_sample_string_to_list["ZToLL_M_50_120"] = {"ZToLL_M_50_120"};
  m.map_sample_string_to_list["ZToLL_M_120_200"] = {"ZToLL_M_120_200"};
  m.map_sample_string_to_list["ZToLL_M_200_400"] = {"ZToLL_M_200_400"};
  m.map_sample_string_to_list["ZToLL_M_400_800"] = {"ZToLL_M_400_800"};
  m.map_sample_string_to_list["ZToLL_M_800_1400"] = {"ZToLL_M_800_1400"};
  m.map_sample_string_to_list["ZToLL_M_1400_2300"] = {"ZToLL_M_1400_2300"};
  m.map_sample_string_to_list["ZToLL_M_2300_3500"] = {"ZToLL_M_2300_3500"};
  m.map_sample_string_to_list["ZToLL_M_3500_4500"] = {"ZToLL_M_3500_4500"};
  m.map_sample_string_to_list["ZToLL_M_4500_6000"] = {"ZToLL_M_4500_6000"};
  m.map_sample_string_to_list["ZToLL_M_6000_Inf"] = {"ZToLL_M_6000_Inf"};
  m.map_sample_string_to_list["WJets_MG"] = {"WJets_MG"};
  m.map_sample_string_to_list["VV_incl"] = {"WZ_pythia", "ZZ_pythia", "WW_pythia"};
  m.map_sample_string_to_list["VV_excl"] = {"ZZTo2L2Q", "ZZTo4L_powheg", "WZTo2L2Q", "WZTo3LNu", "WWTo2L2Nu_powheg"};
  m.map_sample_string_to_list["WW_excl"] = {"WWTo2L2Nu_powheg"};
  m.map_sample_string_to_list["ttbar"] = {"TTLL_powheg", "TTLJ_powheg", "TTJJ_powheg"};
  m.map_sample_string_to_list["chargeflip"] = {"chargeflip"};
  m.map_sample_string_to_list["fake"] = {"fake"};
  m.map_sample_string_to_list["FromEMu"] = {"FromEMu"};

  m.map_sample_string_to_legendinfo["DY"] = make_pair("DY", kYellow);
  m.map_sample_string_to_legendinfo["WJets_MG"] = make_pair("W", kCyan);
  m.map_sample_string_to_legendinfo["ZToLL_M_50_120"] = make_pair("m(Z) : 50-120", kRed-2);
  m.map_sample_string_to_legendinfo["ZToLL_M_120_200"] = make_pair("m(Z) : 120-200", kRed-10);
  m.map_sample_string_to_legendinfo["ZToLL_M_200_400"] = make_pair("m(Z) : 200-400", kOrange);
  m.map_sample_string_to_legendinfo["ZToLL_M_400_800"] = make_pair("m(Z) : 400-800", kYellow);
  m.map_sample_string_to_legendinfo["ZToLL_M_800_1400"] = make_pair("m(Z) : 800-1400", kGreen+3);
  m.map_sample_string_to_legendinfo["ZToLL_M_1400_2300"] = make_pair("m(Z) : 1400-2300", kBlue+3);
  m.map_sample_string_to_legendinfo["ZToLL_M_2300_3500"] = make_pair("m(Z) : 2300-3500", kViolet);
  m.map_sample_string_to_legendinfo["ZToLL_M_3500_4500"] = make_pair("m(Z) : 3500-4500", kGray);
  m.map_sample_string_to_legendinfo["ZToLL_M_4500_6000"] = make_pair("m(Z) : 4500-6000", kGray+2);
  m.map_sample_string_to_legendinfo["ZToLL_M_6000_Inf"] = make_pair("m(Z) : 6000-Inf", kBlack);
  m.map_sample_string_to_legendinfo["WJets_MG"] = make_pair("W", 870);
  m.map_sample_string_to_legendinfo["VV_incl"] = make_pair("diboson", kSpring-1);
  m.map_sample_string_to_legendinfo["VV_excl"] = make_pair("diboson", kSpring-1);
  m.map_sample_string_to_legendinfo["WW_excl"] = make_pair("diboson", kSpring-1);
  m.map_sample_string_to_legendinfo["ttbar"] = make_pair("ttbar", kRed);
  m.map_sample_string_to_legendinfo["chargeflip"] = make_pair("Mismeas. sign bkgd.", kYellow);
  m.map_sample_string_to_legendinfo["fake"] = make_pair("Misid. lepton background", 870);
  m.map_sample_string_to_legendinfo["FromEMu"] = make_pair("Flavour Symm.", kRed);
  
  //===============================
  //==== set and make sample list
  //===============================

  //==== _Di<Lepton>_<JetSel>_<ifOffZ>_<charge>

  //==== One Lepton
  if(XXX==0){
    m.samples_to_use = {"VV_incl", "DY", "ttbar", "WJets_MG"};

    m.histname_suffix = {
      "HNWR_SingleMuon_IsoMu27_OneLepton_AwayFatJet",
      "HNWR_SingleMuon_IsoMu27_OneLepton_AwayDiJet",
      "HNWR_SingleElectron_OneLepton_AwayFatJet",
      "HNWR_SingleElectron_OneLepton_AwayDiJet",

/*
      "HNWR_SingleMuon_IsoMu27_TwoLepton_TwoJet_OS",
      "HNWR_SingleElectron_TwoLepton_TwoJet_OS",
      "HNWR_SingleMuon_IsoMu27_TwoLepton_TwoJet_SS",
      "HNWR_SingleElectron_TwoLepton_TwoJet_SS",
*/

    };

  }

  //==== Two Lepton OS
  if(XXX==1){
    m.samples_to_use = {"WJets_MG", "VV_incl", "ttbar", "DY"};

    m.histname_suffix = {
      "HNWR_SingleMuon_IsoMu27_TwoLepton_TwoJet_OS",
      "HNWR_SingleElectron_TwoLepton_TwoJet_OS",
    };

  }

  //==== Two Lepton SS
  if(XXX==2){
    m.samples_to_use = {"VV_incl", "fake", "chargeflip"};

    m.histname_suffix = {

      "HNWR_SingleMuon_IsoMu27_TwoLepton_TwoJet_SS",
      "HNWR_SingleElectron_TwoLepton_TwoJet_SS",

    };

  }

  //============================
  //==== set variables to draw
  //============================
  
  m.histname = {
    "NEvent",
    "Lepton_0_Pt", "Lepton_0_Eta", "Lepton_0_TrkRelIso",
    "Lepton_1_Pt", "Lepton_1_Eta", "Lepton_1_TrkRelIso",
    "Jet_0_Pt", "Jet_0_Eta",
    "Jet_1_Pt", "Jet_1_Eta",
    "HNFatJet_Pt", "HNFatJet_Eta", "HNFatJet_Mass",
    "ZCand_Mass", "dR_ll",
    "MET", "HT",
    "MT",
    "Jet_Size", "NBJets",
    "N_Mass", "WRCand_Mass",
  };

  m.x_title = {
    "# of events",
    "Leading lepton p_{T} (GeV)", "Leading lepton #eta", "Leading lepton TrkRelIso",
    "Subleading lepton p_{T} (GeV)", "Subleading lepton #eta", "Subleading lepton TrkRelIso",
    "Leading jet p_{T} (GeV)", "Leading jet #eta",
    "Subleading jet p_{T} (GeV)", "Subleading jet #eta",
    "Away AK8 jet p_{T} (GeV)", "Away AK8 jet #eta", "Away AK8 jet Mass",
    "m(ll) (GeV)", "#DeltaR(ll)",
    "#slash{E}_{T}^{miss} (GeV)", "H_{T} (GeV)",
    "m_{T} (GeV)",
    "# of jets", "# of b-tagged jets",
    "m_{N} (GeV)", "m_{WR} (GeV)",
  };

  m.units = {
    "int",
    "GeV", "", "",
    "GeV", "", "",
    "GeV", "",
    "GeV", "",
    "GeV", "", "GeV",
    "GeV", "GeV",
    "GeV", "GeV",
    "GeV",
    "int", "int",
    "GeV", "GeV",
  };

/*
  m.histname = {
    "Lepton_0_Pt", "Lepton_1_Pt",
    "N_Mass", "ZP_Mass",
  };
  m.x_title = {
    "Leading lepton p_{T} (GeV)", "Subleading lepton p_{T} (GeV)",
    "m_{N} (GeV)", "m_{Z'} (GeV)",
  };
  m.units = {
    "GeV", "GeV",
    "GeV", "GeV",
  };
*/
/*
  m.histname = {
    "ZCand_Mass"
  };
  m.x_title = {
    "m(ll) (GeV)"
  };
  m.units = {
    "GeV",
  };


  if(MakeShape){
    m.histname = {
      "ZP_Mass"
    };
    m.x_title = {
      "m_{Z'} (GeV)"
    };
    m.units = {
      "GeV",
    };
  }
*/
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){

    //==== PD
    if(m.histname_suffix.at(i).Contains("DiMuon")){
      m.PrimaryDataset.push_back("DoubleMuon");
      m.LeptonChannels.push_back(21);
    }
    else if(m.histname_suffix.at(i).Contains("SingleElectron")){
      m.PrimaryDataset.push_back("SingleElectron");
      m.LeptonChannels.push_back(22);
    }
    else if(m.histname_suffix.at(i).Contains("SingleMuon")){
      m.PrimaryDataset.push_back("SingleMuon");
      m.LeptonChannels.push_back(21); //FIXME
    }
    else if(m.histname_suffix.at(i).Contains("DiPhoton")){
      m.PrimaryDataset.push_back("DoubleEG");
      m.LeptonChannels.push_back(22); //FIXME
    }
    else{
      cout << "ERROR : PD not correct" << endl;
      return;
    }

    //==== RegionType : + when SS, - when OS
    bool IsSS = m.histname_suffix.at(i).Contains("_SS");
    int int_IsSS = +1;
    if(!IsSS) int_IsSS = -1;

    if(m.histname_suffix.at(i).Contains("SR")){
      m.RegionType.push_back(int_IsSS);
    }
    else if(m.histname_suffix.at(i).Contains("CR1")){
      m.RegionType.push_back(int_IsSS*10);
    }
    else if(m.histname_suffix.at(i).Contains("CR2")){
      m.RegionType.push_back(int_IsSS*20);
    }
    else if(m.histname_suffix.at(i).Contains("CR3")){
      m.RegionType.push_back(int_IsSS*30);
    }
    else{
      m.RegionType.push_back(0);
    }

    //==== Log plot boolean
    if(XXX==0) m.UseLogy.push_back(1);
    if(XXX==1) m.UseLogy.push_back(1);
    if(XXX==2) m.UseLogy.push_back(1);

    if(ScaleMC) m.ApplyMCNormSF.push_back(true);
    else m.ApplyMCNormSF.push_back(false);

    m.drawdata.push_back(false);
    //m.drawdata.push_back(true);

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

  m.analysisInputs.SetMCSF(WORKING_DIR+"/data/"+dataset+"/MCSF.txt", m.bkglist);

  //======================
  //==== Get Systematics
  //======================

  m.analysisInputs.SetCalculatedSysts(WORKING_DIR+"/data/"+dataset+"/Syst.txt");

  //=============================
  //==== set signal mass points
  //=============================

  LRSMSignalInfo lrsminfo;
  lrsminfo.GetMassMapsPlot();
  vector<Color_t> colors_WR = {kGreen, kViolet, kGray, kOrange};

  int it_sig=-1;
  for(map< int, vector<int> >::iterator it=lrsminfo.maps_WR_to_N.begin(); it!=lrsminfo.maps_WR_to_N.end(); it++){

    it_sig++;

    int m_WR = it->first;
    vector<int> this_m_Ns = it->second;

    for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

      int m_N = this_m_Ns.at(it_N);

      LRSMSignalInfo lrsminfo;
      lrsminfo.prod_channel="SchWR";
      lrsminfo.generator="aMCNLO";
      lrsminfo.lep_channel = "EE";
      lrsminfo.mass_WR = m_WR;
      lrsminfo.mass_N = m_N;;
      lrsminfo.SetNames();

      m.signal_LRSMinfo.push_back(lrsminfo);
      m.signal_color.push_back(colors_WR.at(it_sig));
      m.signal_style.push_back(it_N+1);
      m.signal_draw.push_back(true);


      //==== copy MuMu from EE
      lrsminfo.lep_channel = "MuMu";
      lrsminfo.SetNames();
      m.signal_LRSMinfo.push_back(lrsminfo);
      m.signal_color.push_back(colors_WR.at(it_sig));
      m.signal_style.push_back(it_N+1);
      m.signal_draw.push_back(true);

    }

  }

  //=====================================
  //==== set signal mass for each class
  //=====================================

  for(unsigned int i=0; i<m.signal_LRSMinfo.size(); i++){
    LRSMSignalInfo this_lrsm = m.signal_LRSMinfo.at(i);

    m.map_class_to_LRSMSignalInfo[Plotter::All].push_back( this_lrsm );

  }
  m.AllSignalClasses = {Plotter::All};

  //=============
  //==== rebins
  //=============
  
  //==== script to generate rebins
  ofstream skeleton_rebins("./data/tmp_SR_rebins.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_rebins
      //<< "m.rebins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<endl;
    }
  }
  skeleton_rebins.close();

  m.SetRebins(WORKING_DIR+"/data/"+dataset+"/SR_rebins.txt");

  //=============
  //==== y_maxs
  //=============
  
  //==== script to generate rebins
  ofstream skeleton_y_maxs("./data/tmp_SR_yaxis.txt", ios::trunc);
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

  m.SetYAxis(WORKING_DIR+"/data/"+dataset+"/SR_yaxis.txt"); 

  //=============
  //==== x_mins
  //=============

  //==== script to generate rebins
  ofstream skeleton_x_mins("./data/tmp_SR_xaxis.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_x_mins
      //<< "  m.x_mins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<"\t"<<-999<<endl;
    }
  }
  skeleton_x_mins.close();

  m.SetXAxis(WORKING_DIR+"/data/"+dataset+"/SR_xaxis.txt");

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

  m.plotpath = ENV_PLOT_PATH+"/"+dataset+"/SR/";
  m.make_plot_directory();
  m.outputdir_for_shape = ENV_PLOT_PATH+"/"+dataset+"/FilesForShapes/SR/";
  
  //==========================
  //==== finally, draw plots
  //==========================
  
  m.draw_hist();
  
}






