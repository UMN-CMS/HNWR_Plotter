import os,ROOT,math
import CMS_lumi, tdrstyle
from IsCorrelated import IsCorrelated

def IsCorrelatedToString(syst):

  if IsCorrelated(syst):
    return "Correlated"
  else:
    return "Uncorrelated"

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']

Years = [
'2016',
'2017',
'2018',
]
channels = [
'SingleElectron',
'SingleMuon',
]
regions = [
'Resolved',
'Boosted',
]

ResolvedMasses = [
'WR5000_N2000',
'WR5000_N3000',
'WR5000_N4000',
]
BoostedMasses = [
'WR5000_N100',
'WR5000_N200',
'WR5000_N400',
]

print '@@@@ Refrence mass points are'
print 'Resolved : ',
print ResolvedMasses
print 'Boosted : ',
print BoostedMasses
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'

ToRuns = [

  [
    'AllMCBkgd',
    'All bkgd./Signal',
    [
      ["JetRes","Jet energy resolution"],
      ["JetEn","Jet energy scale"],
      ["MuonRecoSF","Muon reconstruction"],
      ["MuonEn","Muon momentum scale"],
      ["MuonIDSF","Muon identification"],
      ["MuonISOSF","Muon isolation"],
      ["MuonTriggerSF","Muon trigger"],
      ["ElectronRecoSF","Electron reconstruction"],
      ["ElectronRes","Electron energy resolution"],
      ["ElectronEn","Electron energy scale"],
      ["ElectronIDSF","Electron identification"],
      ["ElectronTriggerSF","Electron trigger"],
      ["LSFSF","LSF scale factor"],
      ["PU","Pileup modeling"],
    ],
  ],

  [ 
    'DYJets_MG_HT_Reweighted',
    '\\DYJ',
    [ 
      ["ZPtRw","\\PZ \\pt"],
    ],
  ],

]

print 'Integrated luminosity & All bkgd./Signal & '+IsCorrelatedToString('Lumi')+' & 2.3--2.5 (2.3--2.5) & 2.3--2.5 (2.3--2.5) & 2.3--2.5 (2.3--2.5) & 2.3--2.5 (2.3--2.5) \\\\'

for ToRun in ToRuns:

  BkgdSample = ToRun[0]
  BkgdSampleLatex = ToRun[1]
  Systs = ToRun[2]

  for Syst in Systs:

    SystAlias = Syst[0]
    SystLatex = Syst[1]
    out = SystLatex+' & '+BkgdSampleLatex+' & '+IsCorrelatedToString(SystAlias)

    for channel in channels:

      EEorMuMu = "EE"
      if "Muon" in channel:
        EEorMuMu = "MuMu"

      for i_sample in range(0,2):

        Samples = [BkgdSample]

        #### Exception A
        # 1) ZPtRw only for bkgd
        if SystAlias=="ZPtRw" and i_sample==1:
          out += '& \\NA'
          continue

        #### 1--2
        #### first element : region 0
        #### second element : region 1
        systRanges = []
        for region in regions:

          if i_sample==1:
            if region=='Resolved':
              Samples = ResolvedMasses
            else:
              Samples = BoostedMasses

          #### Loop over years to get range
          syst_Min = 999999999
          syst_Max = -999999999
          for Sample in Samples:

            #### filename
            fname = 'HNWRAnalyzer_SkimTree_LRSMHighPt_'+Sample+'.root'
            if i_sample==1:
              fname = 'HNWRAnalyzer_WRtoNLtoLLJJ_'+Sample+'.root'

            for Year in Years:
              basedir = FILE_PATH+'/'+dataset+'/Regions/'+Year+'/'
              if i_sample==1:
                basedir += 'Signal_'+EEorMuMu+'/'

              #### dirName
              dirName = 'HNWR_'+channel+'_'+region+'_SR'

              #### Get TFile
              f = ROOT.TFile(basedir+fname)

              #### Get Nominal
              h_Nom = f.Get(dirName+'/NEvent_'+dirName)
              y_Nom = h_Nom.GetBinContent(1)

              #### Get Up
              h_Up = f.Get('Syst_'+SystAlias+'Up_'+dirName+'/NEvent_'+'Syst_'+SystAlias+'Up_'+dirName)
              y_Up = h_Up.GetBinContent(1)
              diff_Up = abs(y_Up-y_Nom)
              #### Get Down
              h_Down = f.Get('Syst_'+SystAlias+'Down_'+dirName+'/NEvent_'+'Syst_'+SystAlias+'Down_'+dirName)
              y_Down = h_Down.GetBinContent(1)
              diff_Down = abs(y_Down-y_Nom)

              #### Calculate syst
              this_syst = max( 100.*diff_Up/y_Nom, 100.*diff_Down/y_Nom )

              #### Replace syst_Min and syst_Max
              syst_Min = min( syst_Min, this_syst )
              syst_Max = max( syst_Max, this_syst )

              #### Close TFile
              f.Close()

          #### Year loop is done for this region
          #### Append str_syst

          str_syst_Min = '%1.1f'%syst_Min
          str_syst_Max = '%1.1f'%syst_Max

          if str_syst_Min=='0.0':
            str_syst_Min = '0'
          if str_syst_Max=='0.0':
            str_syst_Max = '0'

          if str_syst_Min==str_syst_Max:
            systRanges.append( str_syst_Min )
          else:
            systRanges.append( str_syst_Min+'--'+str_syst_Max )

        if 'Electron' in SystAlias and channel=='SingleMuon':
          out += ' & \\NA'
          continue
        if 'Muon' in SystAlias and channel=='SingleElectron':
          out += ' & \\NA'
          continue

        str_resolved_syst = systRanges[0]
        if str_resolved_syst=="0":
          str_resolved_syst="$<0.1$"

        str_boosted_syst = systRanges[1]
        if str_boosted_syst=="0":
          str_boosted_syst="$<0.1$"

        #### Exception B
        # 1) LSFSF only for boosted
        if SystAlias=="LSFSF":
          out += ' & %s~(%s)' % ('\\NA', str_boosted_syst)
          continue

        out += ' & %s (%s)' % (str_resolved_syst, str_boosted_syst)

    print out+' \\\\'

#print '''Flavor sideband & \\ttbar & 20 (30) & \NA & 20 (30) & \NA \\\\'''

print '''DY normalizaion & \DYJ & Correlated & 30 (30) & \NA & 30 (30) & \NA \\\\'''

#### PDF uncertainty for signal

PDFErrorSet_out = 'PDF error & Signal & Correlated'
AlphaS_out = '\\alpS & Signal & Correlated'
Scale_out = 'renormalization/factorization scales & Signal & Correlated'

for channel in channels:

  #### 1--2
  #### first element : region 0
  #### second element : region 1
  PDFErrorSet_systRanges = []
  AlphaS_systRanges = []
  Scale_systRanges = []

  EEorMuMu = "EE"
  if "Muon" in channel:
    EEorMuMu = "MuMu"

  for region in regions:

    Samples = []

    if region=='Resolved':
      Samples = ResolvedMasses
    else:
      Samples = BoostedMasses

    #### Loop over years to get range
    PDFErrorSet_syst_Min = 999999999
    PDFErrorSet_syst_Max = -999999999
    AlphaS_syst_Min = 999999999
    AlphaS_syst_Max = -999999999
    Scale_syst_Min = 999999999
    Scale_syst_Max = -999999999
    for Sample in Samples:
      fname = 'HNWRAnalyzer_WRtoNLtoLLJJ_'+Sample+'.root'
      for Year in Years:
        basedir = FILE_PATH+'/'+dataset+'/Regions/'+Year+'/Signal_'+EEorMuMu+'/'

        #### dirName
        dirName = 'HNWR_'+channel+'_'+region+'_SR'

        #### Get TFile
        f = ROOT.TFile(basedir+fname)

        #### SignalFlavour
        h_SignalFlavour = f.Get('SignalFlavour')
        LepFlavFrac = h_SignalFlavour.GetBinContent(2)/h_SignalFlavour.GetEntries()
        if channel=='SingleMuon':
          LepFlavFrac = h_SignalFlavour.GetBinContent(3)/h_SignalFlavour.GetEntries()

        #### 1) PDF Error set
        PDFErrorSet_AccEff_Nominal = 0.
        PDFErrorSet_AccEff_SumDiff = 0.
        for i_PDF in range(0,101):
          #### Denominator
          h_Den = f.Get('XsecSyst_Den/PDFWeights_Error_'+str(i_PDF)+'_XsecSyst_Den')
          #### Numerator
          h_Num = f.Get('XsecSyst_Num_'+dirName+'/PDFWeights_Error_'+str(i_PDF)+'_XsecSyst_Num_'+dirName)
          #### AccEff
          AccEff = h_Num.Integral()/(h_Den.GetBinContent(1)*LepFlavFrac)
          if i_PDF==0:
            PDFErrorSet_AccEff_Nominal = AccEff
          else:
            PDFErrorSet_AccEff_SumDiff += (AccEff-PDFErrorSet_AccEff_Nominal) * (AccEff-PDFErrorSet_AccEff_Nominal)
        PDFErrorSet_AccEff_SumDiff = math.sqrt(PDFErrorSet_AccEff_SumDiff)
        #### Replace syst_Min and syst_Max
        PDFErrorSet_syst_Min = min( PDFErrorSet_syst_Min, PDFErrorSet_AccEff_SumDiff/PDFErrorSet_AccEff_Nominal*100. )
        PDFErrorSet_syst_Max = max( PDFErrorSet_syst_Max, PDFErrorSet_AccEff_SumDiff/PDFErrorSet_AccEff_Nominal*100. )

        #### 2) AlphaS
        AlphaS_AccEff_Up = 0
        AlphaS_AccEff_Down = 0
        for i_PDF in range(0,2):
          #### Denominator
          h_Den = f.Get('XsecSyst_Den/PDFWeights_AlphaS_'+str(i_PDF)+'_XsecSyst_Den')
          #### Numerator
          h_Num = f.Get('XsecSyst_Num_'+dirName+'/PDFWeights_AlphaS_'+str(i_PDF)+'_XsecSyst_Num_'+dirName)
          #### AccEff
          AccEff = h_Num.Integral()/(h_Den.GetBinContent(1)*LepFlavFrac)
          if i_PDF==0:
            AlphaS_AccEff_Up = AccEff
          else:
            AlphaS_AccEff_Down = AccEff
        AlphaS_AccEff_error = abs(AlphaS_AccEff_Up-AlphaS_AccEff_Down)/2./PDFErrorSet_AccEff_Nominal*100.
        #### Replace syst_Min and syst_Max
        AlphaS_syst_Min = min( AlphaS_syst_Min, AlphaS_AccEff_error )
        AlphaS_syst_Max = max( AlphaS_syst_Max, AlphaS_AccEff_error )

        #### 3) Scale
        ScaleIDs = [
        1001, # 1) R=1.0 F = 1.0
        1006, # 2) R=2.0 F = 1.0
        1011, # 3) R=0.5 F = 1.0
        1016, # 4) R=1.0 F = 2.0
        1021, # 5) R=2.0 F = 2.0
        #1026, # 6) R=0.5 F = 2.0
        1031, # 7) R=1.0 F = 0.5
        #1036, # 8) R=2.0 F = 0.5
        1041  # 9) R=0.5 F = 0.5
        ]
        for i_Scale in range(0,len(ScaleIDs)):
          ScaleIDs[i_Scale] = ScaleIDs[i_Scale]-1001

        Scale_AccEff_Maxdiff = -9999
        for i_PDF in range(0,len(ScaleIDs)):
          #### Denominator
          h_Den = f.Get('XsecSyst_Den/PDFWeights_Scale_'+str(ScaleIDs[i_PDF])+'_XsecSyst_Den')
          #### Numerator
          h_Num = f.Get('XsecSyst_Num_'+dirName+'/PDFWeights_Scale_'+str(ScaleIDs[i_PDF])+'_XsecSyst_Num_'+dirName)
          #### AccEff
          AccEff = h_Num.Integral()/(h_Den.GetBinContent(1)*LepFlavFrac)
          Scale_AccEff_Maxdiff = max( Scale_AccEff_Maxdiff, abs(AccEff-PDFErrorSet_AccEff_Nominal) )
        #### Replace syst_Min and syst_Max
        Scale_syst_Min = min( Scale_syst_Min, Scale_AccEff_Maxdiff/PDFErrorSet_AccEff_Nominal*100. )
        Scale_syst_Max = max( Scale_syst_Max, Scale_AccEff_Maxdiff/PDFErrorSet_AccEff_Nominal*100. )

    #### Year loop is done for this region
    #### Append str_syst

    PDFErrorSet_str_syst_Min = '%1.1f'%PDFErrorSet_syst_Min
    PDFErrorSet_str_syst_Max = '%1.1f'%PDFErrorSet_syst_Max
    if PDFErrorSet_str_syst_Min=='0.0':
      PDFErrorSet_str_syst_Min='0'
    if PDFErrorSet_str_syst_Max=='0.0':
      PDFErrorSet_str_syst_Max='0'
    if PDFErrorSet_str_syst_Min==PDFErrorSet_str_syst_Max:
      PDFErrorSet_systRanges.append( PDFErrorSet_str_syst_Min )
    else:
      PDFErrorSet_systRanges.append( PDFErrorSet_str_syst_Min+'--'+PDFErrorSet_str_syst_Max )

    AlphaS_str_syst_Min = '%1.1f'%AlphaS_syst_Min
    AlphaS_str_syst_Max = '%1.1f'%AlphaS_syst_Max
    if AlphaS_str_syst_Min=='0.0':
      AlphaS_str_syst_Min='0'
    if AlphaS_str_syst_Max=='0.0':
      AlphaS_str_syst_Max='0'
    if AlphaS_str_syst_Min==AlphaS_str_syst_Max:
      AlphaS_systRanges.append( AlphaS_str_syst_Min )
    else:
      AlphaS_systRanges.append( AlphaS_str_syst_Min+'--'+AlphaS_str_syst_Max )

    Scale_str_syst_Min = '%1.1f'%Scale_syst_Min
    Scale_str_syst_Max = '%1.1f'%Scale_syst_Max
    if Scale_str_syst_Min=='0.0':
      Scale_str_syst_Min='0'
    if Scale_str_syst_Max=='0.0':
      Scale_str_syst_Max='0'
    if Scale_str_syst_Min==Scale_str_syst_Max:
      Scale_systRanges.append( Scale_str_syst_Min )
    else:
      Scale_systRanges.append( Scale_str_syst_Min+'--'+Scale_str_syst_Max )


  #### region loop is done
  #### now write A-B (C-D)

  PDFErrorSet_out += ' & \\NA & %s (%s)' % (PDFErrorSet_systRanges[0], PDFErrorSet_systRanges[1])
  AlphaS_out += ' & \\NA & %s (%s)' % (AlphaS_systRanges[0], AlphaS_systRanges[1])
  Scale_out += ' & \\NA & %s (%s)' % (Scale_systRanges[0], Scale_systRanges[1])

print PDFErrorSet_out+' \\\\'
print AlphaS_out+' \\\\'
print Scale_out+' \\\\'

