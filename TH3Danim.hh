#ifndef TH3DANIM_HH
#define TH3DANIM_HH

#include <TH3D.h>
#include <TPad.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <time.h>

class TH3Danim : public TH3D {
  ClassDef(TH3Danim,1); //needed for interfacing with root

  TH1** animhists; //array of animated axis histograms

  Int_t nbins; //number of bins

  std::string shownaxes; //The axes plotted at each time step (e.g.,
		    //"xy","yx","xz", ...)

  Int_t period; //period between drawing 2-d histograms in microseconds

  std::string GetRemainingAxis(const std::string& s)
  {
    std::string remainingaxis;
    shownaxes = s;
    if(shownaxes == "xy" || shownaxes == "yx") remainingaxis = "z";
    else if(shownaxes == "xz" || shownaxes == "zx") remainingaxis = "y";
    else remainingaxis = "x";
    return remainingaxis;
  }

public:
  TH3Danim(const char* name, const char* title, Int_t nbinsx, Double_t xlo,
	   Double_t xhi, Int_t nbinsy, Double_t ylo, Double_t yhi,
	   Int_t nbinsz, Double_t zlo, Double_t zhi)
    : animhists(0), TH3D(name,title,nbinsx,xlo,xhi,nbinsy,ylo,yhi,nbinsz,zlo,zhi)
  {}

  TH3Danim(const TH3D& h) : animhists(0), TH3D(h) {}
  ~TH3Danim()
  {
    if(animhists != 0) delete[] animhists;
  }

  void SetDrawFrequency(Double_t f)
  {
    period = 1000000/f;
  }

  //This function selects the shown, as well as populating the
  //array of 2-d histograms
  void SetShownAxes(const std::string& s)
  {
    shownaxes = s;
    std::string remainingaxis = GetRemainingAxis(shownaxes);
    //Reset all axis ranges:
    GetXaxis()->SetRange(2,1);
    GetYaxis()->SetRange(2,1);
    GetZaxis()->SetRange(2,1);

    if(animhists)
      delete[] animhists;

    TAxis* animaxis;

    //handle axis:
    if(remainingaxis == "x")
      animaxis = GetXaxis();
    else if(remainingaxis == "y")
      animaxis = GetYaxis();
    else
      animaxis = GetZaxis();

    nbins = animaxis->GetNbins();
    animhists = new TH1*[nbins];
    for(int i = 1; i <= nbins; ++i) {
      animaxis->SetRange(i,i);
      std::stringstream ss;
      ss << shownaxes << i;
      animhists[i-1] = Project3D(ss.str().c_str());
    }
  }

  void AnimDraw(Int_t drawtime, const char* options="colz")
  {
    timespec starttime,oldtime,newtime;
    clock_gettime(CLOCK_REALTIME,&starttime);
    oldtime=newtime=starttime;
    Int_t bin=0;
    Int_t delta = 1;
    while(newtime.tv_sec - starttime.tv_sec <= drawtime) {
      clock_gettime(CLOCK_REALTIME,&newtime);
      long oldutime = oldtime.tv_sec*1000000+oldtime.tv_nsec/1000;
      long newutime = newtime.tv_sec*1000000+newtime.tv_nsec/1000;
      //if(newutime-oldutime < period) usleep(period+oldutime-newutime);
      //if(newutime-oldutime < period) continue;
      while(newutime-oldutime < period) {
	const Int_t timeremaining = period+oldutime-newutime;
	usleep(timeremaining/10);
	clock_gettime(CLOCK_REALTIME,&newtime);
	newutime = newtime.tv_sec*1000000+newtime.tv_nsec/1000;
      }
      /*if(newutime-oldutime < period) {
	usleep(period/3);
	continue;
	}*/
      animhists[bin]->Draw(options);
      gPad->Update();
      oldtime=newtime;
      if(bin == 0 && delta == -1) delta = 1;
      else if(bin == nbins-1 && delta == 1) delta = -1;
      bin += delta;
    }
  }
};

#endif
