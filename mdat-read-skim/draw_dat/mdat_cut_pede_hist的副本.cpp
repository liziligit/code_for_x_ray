// #include "TROOT.h"
// #include "TFile.h"
// #include "TTree.h"
//#include <unistd.h>
#include <stdio.h>
// #include "iostream"
// #include "Riostream.h"
#include "TGraph.h"
// #include "TSpectrum.h"
// #include "TApplication.h"
// #include <iostream>//for file size
// #include <fstream>//for file size
#include <string.h>// for strrchr()
#include <vector>
#include <cstdlib>// exit()
#include "loader_file.h"

// using namespace RooFit;
using namespace std;

TH2F *H2;
TH1D *projh2X;
TH1D *projh2Y;
TPad *right_pad, *top_pad, *center_pad;

// TCanvas *c1 = new TCanvas("c1","Canvas",0,0,500,500);
void mdat_cut_pede_hist()
{   
    auto c1 = new TCanvas("c1", "c1",500,500);
    gStyle->SetOptStat(0);
    const int min2d = -10;
    const int max2d = 10;
    const int NX = 72;
    const int NY = 72;
    char str[30];
    int sumsig;
    unsigned short _data0[NX][NY];//size of 1 frame
    int mode = 0;
    // int ldata;//read for .dat 
    // unsigned short ldata;//read for .mdat 
    char inPdedFile[] = "../pede.txt";
    char inDataFile[] = "../out5.mdat";
    // char inDataFile[] = "../2outframe.dat";

    // char inPdedFile[] = "/Users/lizili/Desktop/CSNS/20190917_TM1_1_hpdaq_onechannel_just_code/data/runData/cnsn_20190917ok/xpede_2.txt";
    // char inDataFile[] = "/Users/lizili/Desktop/CSNS/20190917_TM1_1_hpdaq_onechannel_just_code/data/runData/cnsn_20190917ok/xbeam_30.pd1";

    // char inPdedFile[] = "/Users/lizili/Desktop/TM1_1_hpdaq_onechannel_mac/data/runData/500hv29_01/xpede_2901.txt";
    // char inDataFile[] = "/Users/lizili/Desktop/TM1_1_hpdaq_onechannel_mac/data/runData/500hv29_01/xbeam_1.pd1";

    ifstream infilePede(inPdedFile);//for counts 
    // ifstream infileSig0(inDataFile, ios::binary);//for counts
    ifstream infileSig(inDataFile, ios::binary);
//////////////////////////////////////////////////identified the suffix of input file and iframes
char dot = '.';
char *suffix;
suffix = strrchr(inDataFile, dot);
// cout << "the suffix is " << "|" << suffix << "|" << endl;

if (strcmp(suffix,".mdat") == 0)//如果相等
{
 cout << "suffix is mdat" << endl;
}
else if(strcmp(suffix,".pd1") == 0)
{
cout << "suffix is pd1" << endl;
mode = 1;
}
else
{
cout << "error file type!" << endl;
exit(1);//退出进程
}

int fz=file_loder::file_size(inDataFile);
cout << "the size of file is: "<< fz << endl;
int iFrames = fz / sizeof(_data0);
cout<<"iFrame num is: "<<iFrames<<endl;//808
//////////////////////////////////////////////////identified the suffix of input file and iframes

//////////////////////////////////////////////////read Pede
   if(!infilePede.is_open()) {
        cout << "No pedestal table found!" << endl;
        return ;
    }
    int iChipT = 0, iPixelT = 0, iCounter=0;
    float pedestalT = 0., noiseT = 0.;
    float meanPed[5184];
    while(!infilePede.eof() && iCounter<5184) {
        infilePede >> iChipT >> iPixelT >> pedestalT >> noiseT;
        meanPed[iCounter] = pedestalT;
        // meanPed[iCounter] = 0;//pedestalT for .mdat, zero for .dat-------->3/3
        // rmsPed[iChipT*nPixelsOnChip+iPixelT] = noiseT;
        iCounter++;
    }
//////////////////////////////////////////////////read Pede


//////////////////////////////////////////////////for 3D array
    vector<vector<vector<float> > > array3D;
    int DEPTH = iFrames+1;
    int HEIGHT = NY;
    int LONGTH = NX;
	// 初始化
	array3D.resize(DEPTH);//1
	for (int i = 0; i < DEPTH; ++i) {//1
		array3D[i].resize(HEIGHT);//2
 
		for (int j = 0; j < HEIGHT; ++j)//2
			array3D[i][j].resize(LONGTH);//3
	}
//////////////////////////////////////////////////for 3D array

        int ndata = 0;
        if(mode == 1)
        {
        char header[1024];
        infileSig.read(header, 1024);
        }
        
        center_pad = new TPad("center_pad", "center_pad",0.0,0.0,0.6,0.6);
        center_pad->Draw();
        right_pad = new TPad("right_pad", "right_pad",0.55,0.0,1.0,0.6);
        right_pad->Draw();
        top_pad = new TPad("top_pad", "top_pad",0.0,0.55,0.6,1.0);
        top_pad->Draw(); 

        while(infileSig.good() && ndata < iFrames+1){//&& ndata < iFrames+1
        // while(infileSig.good() && ndata < 20){//&& ndata < iFrames+1
        H2 = new TH2F(Form("H2_%d", ndata),"Projection",72,0,72,72,0,72);

        // sumsig = 0;
        unsigned short _data[NX][NY];//unsigned short for .mdat, int for .dat-------->2/3
        // int _data[NX][NY];//unsigned short for .mdat, int for .dat-------->2/3

         infileSig.read((char *)(&_data), sizeof(_data));
         for (int ii = 0; ii < NX; ii++){
               for (int jj = 0; jj < NY; jj++){
                //    sumsig = sumsig + _data[ii][jj];
                    array3D[ndata][ii][jj] = _data[ii][jj] - meanPed[ii*72+jj];
                    // H2[ndata]->Fill(ii, jj, array3D[ndata][ii][jj]);
                    H2->Fill(ii, jj, array3D[ndata][ii][jj]);
                   }
            }

        projh2X = H2->ProjectionX();
        projh2Y = H2->ProjectionY();

        // cout<<ndata<<endl;     //0~808
        // sprintf(str, "frame %d",ndata);
        // H2->SetTitle(str);
        H2->GetZaxis()->SetRangeUser(min2d,max2d);

        center_pad->cd();
        gStyle->SetPalette(1);
        H2->Draw("COLZ");

        // H2->SetStats(0);

        top_pad->cd();
        projh2X->SetFillColor(kBlue-2);
        projh2X->SetTitle("ProjectionX");
        projh2X->GetYaxis()->SetRangeUser(0,1000);
        projh2X->Draw("bar");

        right_pad->cd();
        projh2Y->SetFillColor(kBlue-2);
        sprintf(str, "frame %d",ndata);
        projh2Y->SetTitle(str);
        projh2Y->GetYaxis()->SetRangeUser(0,1000);
        projh2Y->Draw("hbar"); 
        ndata++;
        // ndata+=1;
        // cout<<ndata<<endl;      //1~809
            // char buf[100];
            //  sprintf(&buf[0],"./mdat_frame%d_matrix_to_txt.jpg",ndata);
            //  c1->SaveAs(buf);

        c1->cd();
        // TLatex *t = new TLatex();
        // t->SetTextFont(42);
        // t->SetTextSize(0.05);
        // t->DrawLatex(0.6,0.88, str);
        // t->DrawLatex(0.6,0.88, "The histogram and");
        // t->DrawLatex(0.6,0.85,"its two projections");

        auto ex = new TExec("zoom","ZoomExec()");
        H2->GetListOfFunctions()->Add(ex);
        c1->Update(); 

        // sleep(1);//second

        if (gSystem->ProcessEvents())//不能去除，否则没有动画
            break;
       }

// delete H2;//手动清空堆Heap中内存
// H2 = NULL;//使堆Heap的指针指向空

infileSig.close();
infilePede.close();
}

void ZoomExec()
{
   int xfirst = H2->GetXaxis()->GetFirst();
   int xlast = H2->GetXaxis()->GetLast();
   double xmin = H2->GetXaxis()->GetBinLowEdge(xfirst);
   double xmax = H2->GetXaxis()->GetBinUpEdge(xlast);
   projh2X->GetXaxis()->SetRangeUser(xmin, xmax);
   top_pad->Modified();

   int yfirst = H2->GetYaxis()->GetFirst();
   int ylast = H2->GetYaxis()->GetLast();
   double ymin = H2->GetYaxis()->GetBinLowEdge(yfirst);
   double ymax = H2->GetYaxis()->GetBinUpEdge(ylast);
   projh2Y->GetXaxis()->SetRangeUser(ymin, ymax);
   right_pad->Modified();
}
