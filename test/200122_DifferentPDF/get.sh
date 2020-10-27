#!/bin/bash
rsync -auv -e 'ssh -p 1240' jskim@147.47.242.42:/data6/Users/jskim/SKFlatOutput/Run2Legacy_v4/HNWRAnalyzer/2016/RunSyst__Signal__RunXsecSyst__RunNewPDF__*__ ./
