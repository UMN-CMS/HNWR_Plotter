for i in 0 1 2
do
  #root -l -b -q "src/Draw_ZPtReweight.C($i)"

  root -l -b -q "src/Draw_PrivateZPtReweight.C($i)"

done
