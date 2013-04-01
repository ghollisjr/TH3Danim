#include "TH3Danim.hh"
#include <TRandom2.h>
void test(int drawtime, double freq=3)
{
  TH3Danim h("h","Histogram",100,0,10,100,-5,5,100,-5,15);
  TRandom2 r;
  for(int i = 0; i < 10000000; ++i) {
    double x,y,z;
    x = r.Gaus(5,1);
    y = r.Gaus(0,1);
    z = x+y+r.Gaus(0,3);
    h.Fill(x,y,z);
  }
  h.SetDrawFrequency(freq);
  h.SetShownAxes("yx");
  h.AnimDraw(drawtime);
}
