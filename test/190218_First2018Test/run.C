#include "canvas_margin.h"

void run(){

  double Lumi_perioaA = 14002.865;
  double Lumi_perioaB = 7097.149;
  double Lumi_perioaC = 6937.082;
  double Lumi_perioaD = 31929.065;

  double reweight_lumi = Lumi_perioaA / (Lumi_perioaA+Lumi_perioaB+Lumi_perioaC+Lumi_perioaD);

  cout << reweight_lumi << endl;

}
