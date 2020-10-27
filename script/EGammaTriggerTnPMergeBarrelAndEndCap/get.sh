for dir in 190510_EGammaMeeting_Trigger 190720_RecommendedBinForEndCap
do

  for year in 2016 2017 2018
  do

    outdir=$dir/$year/
    mkdir -p $outdir
    cp /Users/jskim/cernbox/www/HNWR_13TeV/EGammaTnP/results/$dir/$year/PassTrigger/egammaEffi.txt_EGM2D.root $outdir/

  done

done

mv 190510_EGammaMeeting_Trigger Barrel
mv 190720_RecommendedBinForEndCap EndCap
