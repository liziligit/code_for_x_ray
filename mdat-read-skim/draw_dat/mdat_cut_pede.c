#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include <unistd.h>
#include <stdio.h>
#include "iostream"
#include "Riostream.h"
#include "TGraph.h"
#include "TSpectrum.h"
// #include "TApplication.h"
// #include <iostream>//for file size
// #include <fstream>//for file size
#include <vector>

using namespace RooFit;
using namespace std;

TCanvas *c1 = new TCanvas("c1", "Canvas", 0, 0, 600, 600);
void mdat_cut_pede()
{
    TH2F *H2;
    const int min2d = -10;
    const int max2d = 10;
    const int NX = 72;
    const int NY = 72;
    char str[30];
    int sumsig;
    // int ldata;//read for .dat
    //  unsigned short ldata;//read for .mdat

    ifstream infilePede("../data/pede.txt"); //for counts

    ifstream infileSig0("../data/out9.mdat", ios::binary); //for counts
    ifstream infileSig("../data/out9.mdat", ios::binary);
    //    ifstream infileSig0("../2outframe.dat", ios::binary);//for counts
    //    ifstream infileSig("../2outframe.dat", ios::binary);
    //////////////////////////////////////////////////read Pede
    if (!infilePede.is_open())
    {
        cout << "No pedestal table found!" << endl;
        return;
    }
    int iChipT = 0, iPixelT = 0, iCounter = 0;
    float pedestalT = 0., noiseT = 0.;
    float meanPed[5184];
    while (!infilePede.eof() && iCounter < 5184)
    {
        infilePede >> iChipT >> iPixelT >> pedestalT >> noiseT;
        meanPed[iCounter] = pedestalT;
        // meanPed[iCounter] = 0;//pedestalT for .mdat, zero for .dat-------->3/3
        // rmsPed[iChipT*nPixelsOnChip+iPixelT] = noiseT;
        iCounter++;
        // cout << pedestalT << endl;
    }
    //////////////////////////////////////////////////read Pede
    if (!infileSig0)
    {
        cout << "Can not open data file" << endl;
        return;
    }
    cout << "==>Reading " << endl;

    if (!infileSig)
    {
        cout << "Can not open data file" << endl;
        return;
    }
    cout << "==>Reading " << endl;

    //////////////////////////////////////////////////How many Frame counts
    int iFrames = 0;
    while (infileSig0.good())
    {
        unsigned short _data0[NX][NY]; //unsigned short for .mdat, int for .dat-------->1/3
        // int _data0[NX][NY];//unsigned short for .mdat, int for .dat-------->1/3

        infileSig0.read((char *)(&_data0), sizeof(_data0));
        // cout<<ndata<<endl;
        iFrames++;
    }
    infileSig0.close();
    iFrames = iFrames - 1;
    cout << "iFrame num is: " << iFrames << endl; //808
    //////////////////////////////////////////////////How many Frame counts

    //////////////////////////////////////////////////How many size and Frame counts
    // ifstream in("../pede.mdat");
    // in.seekg(0, ios::end); //设置文件指针到文件流的尾部
    // streampos ps = in.tellg(); //读取文件指针的位置
    // cout << "File size is: " << ps << endl;
    // int iFrames = ps / (NX*NY*2);
    // cout<<"Frame num is: "<<iFrames<<endl;
    // in.close(); //关闭文件流
    //////////////////////////////////////////////////How many size and Frame counts

    // TH2F *H2[iFrames + 1];
    // for (int i = 0; i < iFrames + 1; i++)
    // TH2F *H2[iFrames];
    // for (int i = 0; i < iFrames; i++)
    // {
    //     H2[i] = new TH2F(Form("H2_%d", i), Form("H2_%d", i), 72, 0, 72, 72, 0, 72);
    // }
    

    //////////////////////////////////////////////////for 3D array
    vector<vector<vector<float>>> array3D;
    // int DEPTH = iFrames + 1;
    int DEPTH = iFrames;
    int HEIGHT = NY;
    int LONGTH = NX;
    // 初始化
    array3D.resize(DEPTH); //1
    for (int i = 0; i < DEPTH; ++i)
    {                              //1
        array3D[i].resize(HEIGHT); //2

        for (int j = 0; j < HEIGHT; ++j)  //2
            array3D[i][j].resize(LONGTH); //3
    }
    //////////////////////////////////////////////////for 3D array

    int ndata = 0;
    while(infileSig.good() && ndata < 2)
    // while (infileSig.good())
    {
        sumsig = 0;
        // int pixels = 0;
        unsigned short _data[NX][NY]; //unsigned short for .mdat, int for .dat-------->2/3
                                      // int _data[NX][NY];//unsigned short for .mdat, int for .dat-------->2/3
        H2 = new TH2F(Form("h_%d", ndata), "", 72, 0, 72, 72, 0, 72);
        infileSig.read((char *)(&_data), sizeof(_data));
        for (int ii = 0; ii < NX; ii++)
        {
            for (int jj = 0; jj < NY; jj++)
            {
                sumsig = sumsig + _data[ii][jj];
                array3D[ndata][ii][jj] = _data[ii][jj] - meanPed[ii * 72 + jj];
                H2->Fill(ii, jj, array3D[ndata][ii][jj]);
            }
        }

        // cout<<ndata<<endl;     //0~808
        cout<< ndata << ":" << sumsig<<endl;
        sprintf(str, "frame %d", ndata);
        H2->SetTitle(str);
        H2->GetZaxis()->SetRangeUser(min2d, max2d);
        H2->Draw("Colz");
        H2->SetStats(0);
        c1->Modified();
        c1->Update();
        // delete gROOT->FindObject("H2");
        ndata++;
        // cout<<ndata<<endl;      //1~809
        // char buf[100];
        //  sprintf(&buf[0],"./mdat_frame%d_matrix_to_txt.jpg",ndata);
        //  c1->SaveAs(buf);

        if (gSystem->ProcessEvents()) //不能去除，否则没有动画
            break;
    }

    // for(int i=0;i<iFrames;i++){
    //         delete H2[i];
    // 	}

    infileSig.close();
    infilePede.close();
    // cout << array3D[0][71][71] << endl;
    // cout << array3D[iFrames - 2][71][71] << endl;
    // cout << array3D[iFrames - 1][71][71] << endl;
    // cout << array3D[iFrames][71][71] << endl;
    // cout << array3D[iFrames+1][71][71] << endl;//will show error
}
