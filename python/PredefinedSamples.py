import os,ROOT
from Plotter import SampleGroup

## DY
SampleGroup_DY_2016 = SampleGroup(
  Name='DY',
  Type='Bkgd',
  Samples=['DYJets_MG_HT'],
  Year = 2016,
  Color=ROOT.kYellow,
  Style=1,
  TLatexAlias='Z+Jets',
  LatexAlias='ZJets'
)
SampleGroup_DY_2017 = SampleGroup(
  Name='DY',
  Type='Bkgd',
  Samples=['DYJets_MG_HT'],
  Year = 2017,
  Color=ROOT.kYellow,
  Style=1,
  TLatexAlias='Z+Jets',
  LatexAlias='ZJets'
)
SampleGroup_DY_2018 = SampleGroup(
  Name='DY',
  Type='Bkgd',
  Samples=['DYJets_MG_HT'],
  Year = 2018,
  Color=ROOT.kYellow,
  Style=1,
  TLatexAlias='Z+Jets',
  LatexAlias='ZJets'
)

SampleGroup_ttbar_2016 = SampleGroup(
  Name='ttbar',
  Type='Bkgd',
  Samples=['TTLX_powheg'],
  Year = 2016,
  Color=ROOT.kRed,
  Style=1,
  TLatexAlias='t#bar{t}',
  LatexAlias='ttbar'
)
SampleGroup_ttbar_2017 = SampleGroup(
  Name='ttbar',
  Type='Bkgd',
  Samples=['TTLX_powheg'],
  Year = 2017,
  Color=ROOT.kRed,
  Style=1,
  TLatexAlias='t#bar{t}',
  LatexAlias='ttbar'
)
SampleGroup_ttbar_2018 = SampleGroup(
  Name='ttbar',
  Type='Bkgd',
  Samples=['TTLX_powheg'],
  Year = 2018,
  Color=ROOT.kRed,
  Style=1,
  TLatexAlias='t#bar{t}',
  LatexAlias='ttbar'
)

SampleGroup_Others_2016 = SampleGroup(
  Name='Others',
  Type='Bkgd',
  Samples=['Others'],
  Year = 2016,
  Color=870,
  Style=1,
  TLatexAlias='Other backgrounds',
  LatexAlias='Others'
)
SampleGroup_Others_2017 = SampleGroup(
  Name='Others',
  Type='Bkgd',
  Samples=['Others'],
  Year = 2017,
  Color=870,
  Style=1,
  TLatexAlias='Other backgrounds',
  LatexAlias='Others'
)
SampleGroup_Others_2018 = SampleGroup(
  Name='Others',
  Type='Bkgd',
  Samples=['Others'],
  Year = 2018,
  Color=870,
  Style=1,
  TLatexAlias='Other backgrounds',
  LatexAlias='Others'
)
