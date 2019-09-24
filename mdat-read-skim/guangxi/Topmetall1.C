 #include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include <unistd.h>
#include <stdio.h>
#include "iostream"
#include "Riostream.h"
#include "TGraph.h"
#include "TSpectrum.h"


using namespace RooFit;
using namespace std;

TCanvas *c1 = new TCanvas("c1","Canvas",800,800);


void Topmetall1()
{
   // TGraph2D *gr2d = new TGraph2D();
    //
    // readout the background file
    //
    
    const int nXpixel = 72;
    const int nYpixel = 72;
    int ndata = 0;
    int sumsig,mx,my;
    int ldata;


    //ofstream fout(TString::Format("SumData1.txt"));
    ifstream infileSig("./2outframe.dat", ios::binary);
    if (!infileSig){
       cout<<"Can not open file"<<endl;
       return;
    }
        cout<<"==>Reading "<<endl;
        
        while(infileSig.good()){
           TH2F *H2 = new TH2F("H2"," ",72,0,71,72,0,71);
           H2->SetStats(0);	 
            ndata=ndata+1;
            sumsig = 0;
         for ( int mn=0; mn<(nXpixel*nYpixel); mn++){
           infileSig.read((char *)(&ldata), sizeof(ldata));
            sumsig = sumsig +  ldata;
            mx=mn/72;
            my=mn%72;
            H2->Fill(mx,my,ldata);
          }
          cout<<sumsig<<endl;
         // fout<<sumsig<<endl;
          H2->Draw("Colz");
         
                 c1->Update();
                char buf[100];
                 sprintf(&buf[0],"./2outframe_dat/frame%d.jpg",ndata);
                 c1->SaveAs(buf);
       }
}
