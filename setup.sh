#!/bin/bash

if [[ $HOSTNAME == "jskim-OptiPlex-9020" ]]; then

  #export PLOT_PATH=/home/jskim/cernbox/www/HNWR_13TeV/
  export PLOT_PATH=/home/jskim/Dropbox/HNWR_13TeV/plots/
  export WWW_PLOT_PATH=/var/www/html/HNWR_13TeV/
  alias pl='rsync -auv --delete-excluded '$PLOT_PATH/$CATANVERSION' '$WWW_PLOT_PATH
  alias plforce='rsync -auv -I --delete-excluded output/'$CATANVERSION' '$WWW_PLOT_PATH

elif [[ $HOSTNAME == *"lxplus"* ]]; then

	export CMS_PATH=/cvmfs/cms.cern.ch
	source $CMS_PATH/cmsset_default.sh
	export SCRAM_ARCH=slc6_amd64_gcc630
	cd /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_9_4_4/src/
	eval `scramv1 runtime -sh`
	cd -
	source /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_9_4_4/external/slc6_amd64_gcc630/bin/thisroot.sh

  export PLOT_PATH=$PLOTTER_WORKING_DIR/output/
  export WWW_PLOT_PATH=/eos/user/j/jskim/www/HNWR_13TeV/
  alias pl='rsync -auv --delete-excluded output/'$CATANVERSION' '$WWW_PLOT_PATH
  alias plforce='rsync -auv -I --delete-excluded output/'$CATANVERSION' '$WWW_PLOT_PATH


fi

export PLOTTER_WORKING_DIR=`pwd`
export FILE_PATH=$PLOTTER_WORKING_DIR/rootfiles/

export PYTHONPATH=$PYTHONPATH:$PLOTTER_WORKING_DIR/include/

export SCRIPT_DIR=$PLOTTER_WORKING_DIR/script/
export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$PLOTTER_WORKING_DIR/include/:$PLOTTER_WORKING_DIR/src/

export CATVERSION="Run2Legacy_v3"
export CATANVERSION="Run2Legacy_v3__Default"

alias lqout='cd '$PLOT_PATH
alias webout='cd '$WWW_PLOT_PATH
alias makehtml='make_html_master --To=HNWR_13TeV'
alias makehtml2='make_html_master --To=HNWR_13TeV --From=*/'

mkdir -p $FILE_PATH/$CATANVERSION
