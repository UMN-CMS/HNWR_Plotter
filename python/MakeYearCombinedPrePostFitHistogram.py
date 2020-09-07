import os,ROOT
from array import array

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

PreORPosts = [
"Pre",
"Post",
]

Channels = [
"EE",
"MuMu",
]

Samples = [
'TTLX_powheg',
'DYJets_MG_HT_Reweighted_Reshaped',
'Others',
]

for PreORPost in PreORPosts:

  Mass = "WR5000_N3000"

  basedir = FILE_PATH+'/'+dataset+'/ShapesFromWorkspace/'
  outdirBase = FILE_PATH+'/'+dataset+'/'+PreORPost+'Fit/YearCombined/'

  #### total background

  out_total_background = ROOT.TFile(outdirBase+'HNWRAnalyzer_SkimTree_LRSMHighPt_total_background.root','RECREATE')

  for Channel in Channels:

    Lepton = 'Electron' if (Channel=='EE') else 'Muon'
    OppLepton = 'Muon' if (Lepton=='Electron') else 'Electron'

    Regions = [
      ['HNWR_Single'+Lepton+'_Boosted_DYCR','hists_YearCombined_card_'+Channel+'_Boosted_DYCR.root'],
      ['HNWR_Single'+Lepton+'_Boosted_SR','hists_YearCombined_card_'+Channel+'_Boosted_SR_WR5000_N3000.root'],
      ['HNWR_Single'+OppLepton+'_EMu_Boosted_CR','hists_YearCombined_card_'+Channel+'_EMuShape_Single'+OppLepton+'_EMu_Boosted_CR.root'],
      ['HNWR_Single'+Lepton+'_Resolved_DYCR','hists_YearCombined_card_'+Channel+'_Resolved_DYCR.root'],
      ['HNWR_Single'+Lepton+'_Resolved_SR','hists_YearCombined_card_'+Channel+'_Resolved_SR_WR5000_N3000.root'],
    ]
    if Channel=='EE':
      Regions.append( ['HNWR_EMu_Resolved_SR','hists_YearCombined_card_'+Channel+'_EMuShape_EMu_Resolved_SR.root'] )

    for Region in Regions:
      RegionName = Region[0]
      HistRootFile = ROOT.TFile(basedir+Region[1])

      shapeDir = HistRootFile.Get(PreORPost.lower()+'fit')
      h_TotalBkg = shapeDir.Get('TotalBkg')

      vec_bins = [0, 800, 1000, 1200, 1400, 1600, 2000, 2400, 2800, 3200, 8000]
      if "Boosted" in RegionName:
        vec_bins = [0, 800, 1000, 1200, 1500, 1800, 8000]

      rebinned_h_TotalBkg = ROOT.TH1D('WRCand_Mass_'+RegionName,'', len(vec_bins)-1, array('d',vec_bins))
      for ix in range(0,len(vec_bins)-1):
        iBin = ix+1
        if iBin==1:
          rebinned_h_TotalBkg.SetBinContent(iBin, 0)
          rebinned_h_TotalBkg.SetBinError(iBin, 0)
        else:
          rebinned_h_TotalBkg.SetBinContent(iBin, h_TotalBkg.GetBinContent(ix))
          rebinned_h_TotalBkg.SetBinError(iBin, h_TotalBkg.GetBinError(ix))

      out_total_background.cd()
      out_total_background.mkdir(RegionName)
      out_total_background.cd(RegionName)
      rebinned_h_TotalBkg.Write()
      out_total_background.cd()
  out_total_background.Close()

  #### each process

  for Sample in Samples:

    out = ROOT.TFile(outdirBase+'HNWRAnalyzer_SkimTree_LRSMHighPt_'+Sample+'.root','RECREATE')

    for Channel in Channels:

      Lepton = 'Electron' if (Channel=='EE') else 'Muon'
      OppLepton = 'Muon' if (Lepton=='Electron') else 'Electron'

      Regions = [
        ['HNWR_Single'+Lepton+'_Boosted_SR','hists_'+Sample+'__YearCombined_card_'+Channel+'_Boosted_SR_WR5000_N3000.root'],
        ['HNWR_Single'+Lepton+'_Resolved_SR','hists_'+Sample+'__YearCombined_card_'+Channel+'_Resolved_SR_WR5000_N3000.root'],
      ]

      for Region in Regions:
        RegionName = Region[0]
        HistRootFile = ROOT.TFile(basedir+Region[1])

        shapeDir = HistRootFile.Get(PreORPost.lower()+'fit')
        h_TotalBkg = shapeDir.Get('TotalBkg')

        vec_bins = [0, 800, 1000, 1200, 1400, 1600, 2000, 2400, 2800, 3200, 8000]
        if "Boosted" in RegionName:
          vec_bins = [0, 800, 1000, 1200, 1500, 1800, 8000]

        rebinned_h_TotalBkg = ROOT.TH1D('WRCand_Mass_'+RegionName,'', len(vec_bins)-1, array('d',vec_bins))
        for ix in range(0,len(vec_bins)-1):
          iBin = ix+1
          if iBin==1:
            rebinned_h_TotalBkg.SetBinContent(iBin, 0)
            rebinned_h_TotalBkg.SetBinError(iBin, 0)
          else:
            rebinned_h_TotalBkg.SetBinContent(iBin, h_TotalBkg.GetBinContent(ix))
            rebinned_h_TotalBkg.SetBinError(iBin, h_TotalBkg.GetBinError(ix))

        out.cd()
        out.mkdir(RegionName)
        out.cd(RegionName)
        rebinned_h_TotalBkg.Write()
        out.cd()
    out.Close()
