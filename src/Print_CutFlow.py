import os,ROOT,math
import CMS_lumi, tdrstyle

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
ENV_PLOT_PATH = os.environ['PLOT_PATH']

Years = [
'2016',
#'2017',
]

Bkgds = [
'TTLX_powheg',
]

Suffix = 'SingleMuon'

CutSets = [
  [
'NoCut', 
'METFilter',
'NTightLeptonIsTwo_'+Suffix,
'NTightLeptonIsTwo_'+Suffix+'_PassTrigger',
'NTightLeptonIsTwo_'+Suffix+'_TwoAK4Jets',
'NTightLeptonIsTwo_'+Suffix+'_dRSeparation',
'NTightLeptonIsTwo_'+Suffix+'_mllGT200',
'NTightLeptonIsTwo_'+Suffix+'_mWRGT800',
'NTightLeptonIsTwo_'+Suffix+'_mWRGT800_mll500',
  ],


  [
'NoCut',
'METFilter',
'NotResolved',
'NotResolved_'+Suffix,
'NotResolved_'+Suffix+'_PassTrigger',
'NotResolved_'+Suffix+'_NoLowmll',
'NotResolved_'+Suffix+'_HasMergedJet',
'NotResolved_'+Suffix+'_NoExtraTight',
'NotResolved_'+Suffix+'_HasSFLooseLepton',
'NotResolved_'+Suffix+'_NoHasOFLooseLepton',
'NotResolved_'+Suffix+'_mllGT200',
'NotResolved_'+Suffix+'_mWRGT800',
  ],

]
SignalsSets = [
  [
[3000, 1400], [4000, 2000], [5000, 3000],
  ],

  [
[3000,200],
  ],

]


for Year in Years:

  base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year
  #base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/CutFlow/"+Year+"/";
  #os.system('mkdir -p '+base_plotpath)

  for i_CutSet in range(0,len(CutSets)):

    CutSet = CutSets[i_CutSet]
    Signals = SignalsSets[i_CutSet]

    #### header
    header = 'Selection\t'
    for Bkgd in Bkgds:
      header += Bkgd+'\t'
    for Signal in Signals:
      mWR = Signal[0]
      mN = Signal[1]
      header += '(%d,%d)\t' % (mWR,mN)
    header = header[:-1]
    print header

    #### Loop over cut
    for Cut in CutSet:
      row = Cut+'\t'

      #### Bkgd
      for Bkgd in Bkgds:
        f = ROOT.TFile(base_filepath+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+Bkgd+'.root')
        h = f.Get('CutFlow/'+Cut+'_HNWR')
        y = 0.
        if h:
          y = h.GetEntries()
        row += '%1.2f' % (y)+'\t'

      #### Signal
      for Signal in Signals:
        mWR = Signal[0]
        mN = Signal[1]
        f = ROOT.TFile(base_filepath+'/Signal/HNWRAnalyzer_WRtoNLtoLLJJ_WR'+str(mWR)+'_N'+str(mN)+'.root')
        h = f.Get('CutFlow/'+Cut+'_HNWR')
        y = 0.
        if h:
          y = h.GetEntries()
        row += '%1.2f' % (y)+'\t'

      row = row[:-1]
      print row
