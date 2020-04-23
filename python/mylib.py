import os,ROOT
import math
from array import array

def TotalLumi(DataYear):

  if DataYear==2016:
    return "35.92"
  if DataYear==2017:
    return "41.53"
  if DataYear==2018:
    return "59.74"
  if DataYear<0:
    return "137"
  else:
    print "[mylib.py, TotalLumi()] Wrong DataYear : %d"%DataYear
    return "35.9"

def LumiError(DataYear):

  if DataYear==2016:
    return 0.025
  elif DataYear==2017:
    return 0.023
  elif DataYear==2018:
    return 0.025
  else:
    print "[mylib.py, LumiError()] Wrong DataYear : %d"%DataYear
    return 0.

def MakeOverflowBin(hist):

  #### GetXaxis().SetRangeUser() should be done first

  #### 0     1                                          n_bin_origin
  ####     |===|===|===|===|===|===|===|===|===|===|===|===|===|
  ####           bin_first        bin_last
  ####             |===|===|===|===|===|
  #### |   |                                                   |   |
  #### under                                                   over
  #### flow                                                    flow
  #### |<=========>|                   |<=========================>|
  ####  all underflow                           all overflow

  #### Original NBins
  n_bin_origin = hist.GetXaxis().GetNbins()
  #### Changed NBins
  bin_first = hist.GetXaxis().GetFirst()
  bin_last = hist.GetXaxis().GetLast()
  n_bin_inrange = bin_last-bin_first+1

  x_first_lowedge = hist.GetXaxis().GetBinLowEdge(bin_first)
  x_last_upedge = hist.GetXaxis().GetBinUpEdge(bin_last)

  Allunderflows = hist.Integral(0, bin_first-1)
  Allunderflows_error = hist.GetBinError(0)
  Allunderflows_error = Allunderflows_error*Allunderflows_error
  for i in range(1, bin_first-1 +1):
    Allunderflows_error += (hist.GetBinError(i))*(hist.GetBinError(i))
  Allunderflows_error = math.sqrt(Allunderflows_error)

  Alloverflows = hist.Integral(bin_last+1, n_bin_origin+1)
  Alloverflows_error = hist.GetBinError(n_bin_origin+1)
  Alloverflows_error = Alloverflows_error*Alloverflows_error
  for i in range(bin_last+1, n_bin_origin +1):
    Alloverflows_error += (hist.GetBinError(i))*(hist.GetBinError(i))
  Alloverflows_error = math.sqrt(Alloverflows_error)

  #### Make X-bin array
  temp_xbins = [] ## size = n_bin_inrange+1
  counter=0
  for i in range(bin_first,bin_last +1):
    temp_xbins.append( hist.GetXaxis().GetBinLowEdge(i) )
    counter += 1
  temp_xbins.append( hist.GetXaxis().GetBinUpEdge(bin_last) )

  hist_out = ROOT.TH1D(hist.GetName()+'tmp', hist.GetTitle(), n_bin_inrange, array("d",temp_xbins))
  for i in range(1,n_bin_inrange+1):
    this_content = hist.GetBinContent(bin_first-1+i)
    this_error = hist.GetBinError(bin_first-1+i)
    #print '[%d,%d] : %f'%(hist_out.GetXaxis().GetBinLowEdge(i),hist_out.GetXaxis().GetBinUpEdge(i),this_content)

    #### underflows
    if i==1:
      this_content += Allunderflows
      this_error = math.sqrt( this_error*this_error + Allunderflows_error*Allunderflows_error )

    #### overflows
    if i==n_bin_inrange:
      this_content += Alloverflows
      this_error = math.sqrt( this_error*this_error + Alloverflows_error*Alloverflows_error )

    hist_out.SetBinContent(i, this_content)
    hist_out.SetBinError(i, this_error)

  hist_out.SetName(hist.GetName())
  return hist_out

def RebinWRMass(hist, region, DataYear, IsShape=False):

  lastbin = hist.GetXaxis().GetNbins()
  vec_bins = [0, 800, 1000, 1200, 1400, 1600, 2000, 2400, 2800, 3200, 8000]
  if "Boosted" in region:
    if DataYear==2016 and ( ("HNWR_SingleMuon_EMu_Boosted_CR" in region) or ("HNWR_SingleElectron_Boosted_SR" in region) or ("elFatJet" in region) ):
      vec_bins = [0, 800, 1000, 1200, 1500, 1800, 8000]
    else:
      vec_bins = [0, 800, 1000, 1200, 1500, 1700, 8000]

  if IsShape:
    vec_bins.remove(0)

  n_bin = len(vec_bins)-1
  hist = hist.Rebin(n_bin, hist.GetName(), array("d", vec_bins) )
  return hist

def GetMaximum(a, ErrorScale=1.):

  NX = a.GetN()

  maxval = -9999.
  for i in range(0,NX):

    x = ROOT.Double(0.)
    y = ROOT.Double(0.)

    a.GetPoint(i, x, y)
    yerr_low  = a.GetErrorYlow(i)
    yerr_high = a.GetErrorYhigh(i)

    if (y+ErrorScale*yerr_high > maxval):
      maxval = y+ErrorScale*yerr_high

  return maxval

def GetAsymmError(MC_stacked_allerr_Up, MC_stacked_allerr_Down):

  NBin = MC_stacked_allerr_Up.GetXaxis().GetNbins()
  x = []
  x_lerr = []
  x_rerr = []
  y = []
  y_lerr = []
  y_rerr = []

  for i in range(0,NBin):

    x.append( MC_stacked_allerr_Up.GetXaxis().GetBinCenter(i+1) )
    x_lerr.append( x[i] - MC_stacked_allerr_Up.GetXaxis().GetBinLowEdge(i+1) )
    x_rerr.append( MC_stacked_allerr_Up.GetXaxis().GetBinUpEdge(i+1) - x[i] )

    y.append( MC_stacked_allerr_Up.GetBinContent(i+1) )
    y_lerr.append( MC_stacked_allerr_Down.GetBinError(i+1) )
    y_rerr.append( MC_stacked_allerr_Up.GetBinError(i+1) )

  out = ROOT.TGraphAsymmErrors(NBin, array("d", x), array("d", y),  array("d", x_lerr), array("d", x_rerr), array("d", y_lerr), array("d", y_rerr))
  return out

def GetDYNormSF(DataYear, channel):

  DYNorm = 1.
  DYNorm_err = 0.

  int_channel = -1 ## 0 : ee, 1 : mm
  if "Electron" in channel:
    int_channel = 0
  elif "Muon" in channel:
    int_channel = 1
  elif "_EMu_" in channel:
    return 1., 0.
  else:
    print 'Wrong channel name '+channel
    exit()

  int_region = -1 ## 0 : Resolved, 1 : Boosted
  if "Resolved" in channel:
    int_region = 0;
  elif "Boosted" in channel:
    int_region = 1;
  else:
    print 'Wrong region name '+channel
    exit()

  #print '%s -> int_channel = %d, int_region = %d'%(channel,int_channel,int_region)

  if DataYear==2016:
    if int_channel==0:
      if int_region==0:
        DYNorm = 0.948615
        DYNorm_err = 0.0203336
      elif int_region==1:
        DYNorm = 0.869791
        DYNorm_err = 0.0247752
      else:
        print "Wrong DY Norm"
        exit()
    elif int_channel==1:
      if int_region==0:
        DYNorm = 0.964614
        DYNorm_err = 0.020369
      elif int_region==1:
        DYNorm = 0.838429
        DYNorm_err = 0.0247198
      else:
        print "Wrong DY Norm"
        exit()
    else:
      print "Wrong DY Norm"
      exit()
  elif DataYear==2017:
    if int_channel==0:
      if int_region==0:
        DYNorm = 1.03096
        DYNorm_err = 0.0218684
      elif int_region==1:
        DYNorm = 0.97541
        DYNorm_err = 0.0290485
      else:
        print "Wrong DY Norm"
        exit()
    elif int_channel==1:
      if int_region==0:
        DYNorm = 1.07024
        DYNorm_err = 0.0225204
      elif int_region==1:
        DYNorm = 0.96038
        DYNorm_err = 0.028569
      else:
        print "Wrong DY Norm"
        exit()
    else:
      print "Wrong DY Norm"
      exit()
  elif DataYear==2018:
    if int_channel==0:
      if int_region==0:
        DYNorm = 0.974832
        DYNorm_err = 0.0205765
      elif int_region==1:
        DYNorm = 0.861063
        DYNorm_err = 0.0235285
      else:
        print "Wrong DY Norm"
        exit()
    elif int_channel==1:
      if int_region==0:
        DYNorm = 1.00537
        DYNorm_err = 0.0210814
      elif int_region==1:
        DYNorm = 0.844995
        DYNorm_err = 0.0233501
      else:
        print "Wrong DY Norm"
        exit()
    else:
      print "Wrong DY Norm"
      exit()

  return DYNorm, DYNorm_err
