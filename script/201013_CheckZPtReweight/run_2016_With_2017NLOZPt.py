import os,ROOT

baseDir = '/data6/Users/jskim/SKFlatOutput/Run2Legacy_v4/HNWRAnalyzer/2016/'
f_Nominal = ROOT.TFile(baseDir+'/RunSyst__ApplyDYPtReweight__/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT_Reweighted.root')
f_NewPDF = ROOT.TFile(baseDir+'/TEST201015_2017NLOZPt__RunSyst__ApplyDYPtReweight__/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT_Reweighted.root')

Channels = [
'Electron',
'Muon'
]

Regions = [
'Resolved',
'Boosted',
]

for Region in Regions:

  for Channel in Channels:

    dirName = 'HNWR_Single'+Channel+'_'+Region+'_DYCR'

    h_Nominal = f_Nominal.Get(dirName+'/NEvent_'+dirName)
    h_NewPDF = f_NewPDF.Get(dirName+'/NEvent_'+dirName)

    y_Nominal = h_Nominal.GetBinContent(1)
    y_NewPDF = h_NewPDF.GetBinContent(1)

    print '%s\t%s\t%1.2f\t%1.2f'%(Region,Channel,y_Nominal,y_NewPDF)
