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

TCanvas *c1 = new TCanvas("c1","Canvas",0,0,800,800);


void draw_dat()
{
   // TGraph2D *gr2d = new TGraph2D();
    //
    // readout the background file
    //
    
    const int nXpixel = 72;
    const int nYpixel = 72;
    int ndata = 0;
    int sumsig,mx,my;
    int ldata;//read for .dat 
   //  unsigned short ldata;//read for .mdat 


    //ofstream fout(TString::Format("SumData1.txt"));
    ifstream infileSig("../2outframe.dat", ios::binary);
   // ifstream infileSig("../out1.mdat", ios::binary);
    if (!infileSig){
       cout<<"Can not open file"<<endl;
       return;
    }
        cout<<"==>Reading "<<endl;
        
        while(infileSig.good() && ndata < 3){
           TH2F *H2 = new TH2F("H2"," ",72,0,72,72,0,72);
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
          cout<<ndata<<endl;
          cout<<sumsig<<endl;
        
          // TAxis *axis = H2->GetXaxis();
          // axis->SetLabelColor(1);
          H2->Draw("Colz");
          // gPad->Update();
          // TPaletteAxis *palette = (TPaletteAxis*)H2->GetListOfFunctions()->FindObject("palette");
          // palette->GetAxis()->SetLabelSize(1.);//error
          // palette->GetAxis()->SetTitleSize(0.001);//error
          // palette->GetAxis()->SetLabelColor(2);//error
          // palette->SetY2NDC(0.7);
          
         
                 c1->Update();
                char buf[100];
                 sprintf(&buf[0],"./dat_frame%d.jpg",ndata);
                 c1->SaveAs(buf);
       }
}
