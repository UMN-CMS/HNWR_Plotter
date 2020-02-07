#include "Plotter.cc"
#include <fstream>

void Draw_Validation(int XXX=0){

  bool ScaleMC = false;
  bool UseBinnedDY = false;

  int Year = 2016;

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

  //=====================
  //==== set data class
  //=====================
  
  m.data_class = dataset+"/SKFlatValidation/"+TString::Itoa(Year,10)+"/";
  
  //================================
  //==== set prefixes and suffixes
  //================================
  
  m.filename_prefix = "SKFlatValidation";
  m.filename_suffix = ".root";
  
  //=========================
  //==== set sample mapping
  //=========================

  if(Year==2016){
    m.map_sample_string_to_list["ZJets"] = {"DYJets10to50_MG", "DYJets_MG"};
    m.map_sample_string_to_list["WJets_MG"] = {"WJets_MG"};
    m.map_sample_string_to_list["VV_incl"] = {"WZ_pythia", "ZZ_pythia", "WW_pythia"};
    m.map_sample_string_to_list["ttbar"] = {"TTLL_powheg"};

    m.map_sample_string_to_legendinfo["ZJets"] = make_pair("Z+Jets", kYellow);
    m.map_sample_string_to_legendinfo["WJets_MG"] = make_pair("W+Jets", 870);
    m.map_sample_string_to_legendinfo["VV_incl"] = make_pair("diboson", kSpring-1);
    m.map_sample_string_to_legendinfo["ttbar"] = make_pair("ttbar", kRed);
  }
  else if(Year==2017){

    m.map_sample_string_to_list["ZJets"] = {"DYJets10to50_MG", "DYJets"};
    m.map_sample_string_to_list["ZToLL"] = {"DYJets10to50_MG", "ZToLL"};
    m.map_sample_string_to_list["ZJets_MG_HT"] = {"DYJets10to50", "DYJets_MG_HT-100To200", "DYJets_MG_HT-1200To2500", "DYJets_MG_HT-200To400", "DYJets_MG_HT-2500ToInf", "DYJets_MG_HT-400To600", "DYJets_MG_HT-600To800", "DYJets_MG_HT-70To100", "DYJets_MG_HT-800To1200"};
    m.map_sample_string_to_list["WJets_MG"] = {"WJets_MG"};
    m.map_sample_string_to_list["WJets_MG_HT"] = {"WJets_MG_HT-100To200", "WJets_MG_HT-1200To2500", "WJets_MG_HT-200To400", "WJets_MG_HT-2500ToInf", "WJets_MG_HT-400To600", "WJets_MG_HT-600To800", "WJets_MG_HT-70To100", "WJets_MG_HT-800To1200"};
    m.map_sample_string_to_list["VV_incl"] = {"WZ_pythia", "ZZ_pythia", "WW_pythia"};
    m.map_sample_string_to_list["ttbar"] = {"TTLL_powheg", "TTLJ_powheg", "TTJJ_powheg"};

    m.map_sample_string_to_legendinfo["ZJets"] = make_pair("Z+Jets", kYellow);
    m.map_sample_string_to_legendinfo["ZJets_MG_HT"] = make_pair("Z+Jets", kYellow);
    m.map_sample_string_to_legendinfo["ZToLL"] = make_pair("Z+Jets", kRed-2);
    m.map_sample_string_to_legendinfo["WJets_MG"] = make_pair("W", 870);
    m.map_sample_string_to_legendinfo["WJets_MG_HT"] = make_pair("W", 870);
    m.map_sample_string_to_legendinfo["VV_incl"] = make_pair("diboson", kSpring-1);
    m.map_sample_string_to_legendinfo["ttbar"] = make_pair("ttbar", kRed);

  }
  else if(Year==2018){

    m.map_sample_string_to_list["ZJets"] = {"DYJets_MG"};
    m.map_sample_string_to_list["ttbar"] = {"TTLL_powheg", "TTLJ_powheg", "TTJJ_powheg"};
    m.map_sample_string_to_list["VV_incl"] = {"WZ_pythia", "ZZ_pythia", "WW_pythia"};

    m.map_sample_string_to_legendinfo["ZJets"] = make_pair("Z+Jets", kYellow);
    m.map_sample_string_to_legendinfo["VV_incl"] = make_pair("diboson", kSpring-1);
    m.map_sample_string_to_legendinfo["ttbar"] = make_pair("ttbar", kRed);

  }


  //===============================
  //==== set and make sample list
  //===============================

  //==== _Di<Lepton>_<JetSel>_<ifOffZ>_<charge>

  if(XXX==0){
    if(UseBinnedDY)
      m.samples_to_use = {"WJets_MG_HT", "VV_incl", "ttbar", "ZJets_MG_HT"};
    else
      m.samples_to_use = {"WJets_MG", "VV_incl", "ttbar", "ZJets"};

    vector<TString> paramnames = {
      "POG",
      "POGHighPt",
    };

    vector<TString> regions = {

      //==== OS

      "SingleMuon_OS",
      "SingleMuon_OnZ_OS",
      "SingleMuon_HigherDiLeptonPtCut_OS",
      "SingleMuon_WithBJet_METgt30_OS",

      "SingleElectron_OS",
      "SingleElectron_OnZ_OS",
      "SingleElectron_HigherDiLeptonPtCut_OS",
      "SingleElectron_WithBJet_METgt30_OS",

      //==== SS

      "SingleMuon_SS",
      "SingleMuon_OnZ_SS",
      "SingleMuon_HigherDiLeptonPtCut_SS",
      "SingleMuon_WithBJet_METgt30_SS",
      "SingleElectron_SS",
      "SingleElectron_OnZ_SS",
      "SingleElectron_HigherDiLeptonPtCut_SS",
      "SingleElectron_WithBJet_METgt30_SS",


/*
      "SingleMuon_W_CR",
      "SingleElectron_W_CR",
*/
    };

    for(unsigned int i=0; i<paramnames.size(); i++){
      for(unsigned int j=0; j<regions.size(); j++){
        m.histname_suffix.push_back( paramnames.at(i)+"_"+regions.at(j) );
      }
    }

  }
  if(XXX==1){
    if(UseBinnedDY)
      m.samples_to_use = {"VV_incl", "ttbar", "ZJets_MG_HT", "WJets_MG_HT"};
    else
      m.samples_to_use = {"VV_incl", "ttbar", "ZJets", "WJets_MG"};

    vector<TString> paramnames = {
      "POG",
      "POGHighPt",
    };

    vector<TString> regions = {
      "SingleMuon_W_CR",
      "SingleElectron_W_CR",
    };

    for(unsigned int i=0; i<paramnames.size(); i++){
      for(unsigned int j=0; j<regions.size(); j++){
        m.histname_suffix.push_back( paramnames.at(i)+"_"+regions.at(j) );
      }
    }

  }

  //============================
  //==== set variables to draw
  //============================
  
  m.histname = {
    "NEvent", "nPileUp", "nPV", "nPUForReweight",
    "Lepton_0_Pt", "Lepton_0_Eta", "Lepton_0_RelIso", "Lepton_0_MiniRelIso", "Lepton_0_MVANoIso",
    "Lepton_1_Pt", "Lepton_1_Eta", "Lepton_1_RelIso", "Lepton_1_MiniRelIso", "Lepton_1_MVANoIso",
    "ZCand_Mass", "ZCand_Pt", "ZCand_Eta",
    "MET", "HT",
    "MT",
    "Jet_Size",
    "NBJets_NoSF", "NBJets_WithSF",
  };

  m.x_title = {
    "# of events", "# of PU", "# of PV", "# of PV",
    "Leading lepton p_{T} (GeV)", "Leading lepton #eta", "Leading lepton RelIso","Leading lepton MiniRelIso","Leading lepton MVA (NoIso)",
    "Subleading lepton p_{T} (GeV)", "Subleading lepton #eta", "Subleading lepton RelIso","Subleading lepton MiniRelIso","Subleading lepton MVA (NoIso)",
    "m(Z) (GeV)", "Z p_{T} (GeV)", "Z #eta",
    "#slash{E}_{T}^{miss} (GeV)", "H_{T} (GeV)",
    "m_{T} (GeV)",
    "# of jets",
    "# of b-tagged jets (No SF)", "# of b-tagged jets (With SF)",
  };

  m.units = {
    "int", "int", "int", "int",
    "GeV", "", "", "", "",
    "GeV", "", "", "", "",
    "GeV", "GeV", "",
    "GeV", "GeV",
    "GeV",
    "int",
    "int", "int",
  };

  for(unsigned int i=0; i<m.histname_suffix.size(); i++){

    //==== PD
    if(m.histname_suffix.at(i).Contains("SingleMuon")){
      m.PrimaryDataset.push_back("SingleMuon");
      m.LeptonChannels.push_back(22); //FIXME
    }
    else if(m.histname_suffix.at(i).Contains("SingleElectron")){
      m.PrimaryDataset.push_back("SingleElectron");
      m.LeptonChannels.push_back(22); //FIXME
    }
    else{
      cout << "ERROR : PD not correct" << endl;
      return;
    }

    m.RegionType.push_back(0);

    //==== Log plot boolean
    if(XXX==0) m.UseLogy.push_back(1);
    if(XXX==1) m.UseLogy.push_back(1);

    if(ScaleMC) m.ApplyMCNormSF.push_back(true);
    else m.ApplyMCNormSF.push_back(false);

    //if(m.histname_suffix.at(i).Contains("Preselection")) m.drawdata.push_back(true);
    //else m.drawdata.push_back(false);

    //m.drawdata.push_back(false);

    m.drawdata.push_back(true);

    //if(m.histname_suffix.at(i).Contains("DiLepton")) m.drawratio.push_back(false);
    //else m.drawratio.push_back(true);

    m.drawratio.push_back(true);

  }


/*
  //==== test
  m.histname = {"ZCand_Mass"};
  m.x_title = {"m(Z) (GeV)"};
  m.units = {"GeV"};
*/
/*
  m.histname = {
    "Lepton_0_Pt",
    "Lepton_1_Pt",
  };
  m.x_title = {
    "Leading lepton p_{T} (GeV)",
    "Subleading lepton p_{T} (GeV)",
  };
  m.units = {
    "GeV",
    "GeV",
  };
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

  m.analysisInputs.SetMCSF(WORKING_DIR+"/data/"+dataset+"/"+TString::Itoa(Year,10)+"/MCSF.txt", m.bkglist);

  //======================
  //==== Get Systematics
  //======================

  m.analysisInputs.SetCalculatedSysts(WORKING_DIR+"/data/"+dataset+"/"+TString::Itoa(Year,10)+"/Syst.txt");

  //=============================
  //==== set signal mass points
  //=============================

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
  ofstream skeleton_rebins("./data/tmp_Validation_rebins.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_rebins
      //<< "m.rebins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<endl;
    }
  }
  skeleton_rebins.close();

  m.SetRebins(WORKING_DIR+"/data/"+dataset+"/"+TString::Itoa(Year,10)+"/Validation_rebins.txt");

  //=============
  //==== y_maxs
  //=============
  
  //==== script to generate rebins
  ofstream skeleton_y_maxs("./data/tmp_Validation_yaxis.txt", ios::trunc);
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

  m.SetYAxis(WORKING_DIR+"/data/"+dataset+"/"+TString::Itoa(Year,10)+"/Validation_yaxis.txt"); 

  //=============
  //==== x_mins
  //=============


  //==== script to generate rebins
  ofstream skeleton_x_mins("./data/tmp_Validation_xaxis.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_x_mins
      //<< "  m.x_mins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<"\t"<<-999<<endl;
    }
  }
  skeleton_x_mins.close();

  m.SetXAxis(WORKING_DIR+"/data/"+dataset+"/"+TString::Itoa(Year,10)+"/Validation_xaxis.txt");

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

  if(UseBinnedDY) m.plotpath = ENV_PLOT_PATH+"/"+m.data_class+"/BinnedDY/";
  else            m.plotpath = ENV_PLOT_PATH+"/"+m.data_class+"/InclusiveDY/";
  m.make_plot_directory();
  
  //==========================
  //==== finally, draw plots
  //==========================
  
  m.draw_hist();
  
}






