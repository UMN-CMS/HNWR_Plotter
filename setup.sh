#!/bin/bash

export CMS_PATH=/cvmfs/cms.cern.ch
source $CMS_PATH/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc630
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_9_4_4/src/
eval `scramv1 runtime -sh`
cd -
source /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_9_4_4/external/slc6_amd64_gcc630/bin/thisroot.sh

export PLOTTER_WORKING_DIR=`pwd`
export FILE_PATH=$PLOTTER_WORKING_DIR/rootfiles/
export PLOT_PATH=/eos/user/j/jskim/www/HNWR_13TeV/
export SCRIPT_DIR=$PLOTTER_WORKING_DIR/script/
export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$PLOTTER_WORKING_DIR/include/:$PLOTTER_WORKING_DIR/src/

export CATVERSION="v946p1_3"
export CATANVERSION="v946p1_3__Default"

alias lqout='cd '$PLOT_PATH

mkdir -p $FILE_PATH/$CATANVERSION
