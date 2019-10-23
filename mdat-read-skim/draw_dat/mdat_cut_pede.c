// #include "TROOT.h"
// #include "TFile.h"
// #include "TTree.h"
// #include <unistd.h>
#include <stdio.h>
// #include "iostream"
// #include "Riostream.h"
#include "TGraph.h"
// #include "TSpectrum.h"
// #include "TApplication.h"
// #include <iostream>//for file size
// #include <fstream>//for file size
#include <string.h> // for strrchr()
#include <vector>
#include <cstdlib> // exit()
#include "loader_file.h"

// using namespace RooFit;
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
    char inPdedFile[] = "../data/pede.txt";
    // char inDataFile[] = "../data/out5.mdat";
    char inDataFile[] = "../data/out7.mdat";
    char output_txt[] = "./output_txt.dat"; //for debug
    ifstream infilePede(inPdedFile);
    ifstream infileSig(inDataFile, ios::binary);

    //////////////////////////////////////////////////read Pede
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

    //////////////////////////////////////////////////How many Frame counts
    unsigned short _data0_short[NX][NY]; //size of 1 frame for .mdat, .pd1
    int fz = file_loder::file_size(inDataFile);
    cout << "the size of file is: " << fz << endl;
    int iFrames = 0;
    iFrames = fz / sizeof(_data0_short);
    cout << "iFrame num is: " << iFrames << endl; //808
    //////////////////////////////////////////////////How many Frame counts

    vector<vector<vector<float>>> array3D;
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
    ofstream output;
    output.open(output_txt);//清空模式

    // for (int i = 0; i < iFrames; i++)
    for (int i = 0; i < 76; i++)
    {
        H2 = new TH2F(Form("H2_%d", i), "Projection", 72, 0, 72, 72, 0, 72);
        sumsig = 0;
        unsigned short _data_short[NX][NY];
        infileSig.read((char *)(&_data_short), sizeof(_data_short));
        for (int ii = 0; ii < NX; ii++)
        {
            for (int jj = 0; jj < NY; jj++)
            {
                array3D[i][ii][jj] = _data_short[ii][jj] - meanPed[ii * 72 + jj];
                sumsig = sumsig + array3D[i][ii][jj];
                H2->Fill(ii, jj, array3D[i][ii][jj]);
            }
        }

        cout << i << ":" << sumsig << endl;
        sprintf(str, "frame %d", i);
        H2->SetTitle(str);
        H2->GetZaxis()->SetRangeUser(min2d, max2d);
        H2->Draw("Colz");
        H2->SetStats(0);
        c1->Modified();
        c1->Update();

        // if(i == 75){
        //     char buf[100];
        //     sprintf(&buf[0],"./mdat_frame%d.png",i);
        //     c1->SaveAs(buf);
        // }

        // if(i == 75){
        //     for (int ii = 0; ii < NX; ii++)
        //     {
        //         for (int jj = 0; jj < NY; jj++)
        //         {
        //             output << fixed << setprecision(0) <<setiosflags(ios::left)<< setw(3) << array3D[i][ii][jj] << ",";
        //         }
        //         output << "xxxxxx "<< endl;
        //     }
        // }

        if (gSystem->ProcessEvents()) //不能去除，否则没有动画
            break;
    }

    infileSig.close();
    infilePede.close();
    output.close();
}
