import os,ROOT

baseDir = '/data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v4__Default/Regions/'

Years = [
'2016',
'2017',
'2018',
]

Channels = [
'Electron',
'Muon'
]

Regions = [
'Resolved',
'Boosted',
]

for Year in Years:

  f_Nominal = ROOT.TFile(baseDir+'/'+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT.root')
  f_Rew = ROOT.TFile(baseDir+'/'+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT_Reweighted.root')

  for Region in Regions:

    data_values = []
    values = []

    for Channel in Channels:

      dirName = 'HNWR_Single'+Channel+'_'+Region+'_SR'

      h_Nominal = f_Nominal.Get(dirName+'/NEvent_'+dirName)
      h_Rew = f_Rew.Get(dirName+'/NEvent_'+dirName)

      y_Nominal = h_Nominal.GetBinContent(1)
      y_Rew = h_Rew.GetBinContent(1)

      values.append( y_Rew/y_Nominal )

      f_Data = ROOT.TFile(baseDir+'/'+Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_data_Single'+Channel+'.root')
      h_Data = f_Data.Get(dirName+'/NEvent_'+dirName)
      y_Data = h_Data.GetBinContent(1)
      data_values.append( y_Data )

    nickname = Year+'\t'+Region
    if Region == 'Boosted':
      nickname = '\t'+Region
    #print '%s\t%1.3f\t%1.3f'%(nickname,values[0],values[1])
    print '%s\t%1.3f\t%1.3f'%(nickname,data_values[0],data_values[1])
