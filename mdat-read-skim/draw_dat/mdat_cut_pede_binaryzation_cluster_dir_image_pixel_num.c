//去除本底，将大于一定像素个数的径迹输出到文件

#include <TH2F.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TStyle.h>
// #include <TColor.h>
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
#include <iomanip>//格式化输出
#include "loader_file.h"

// using namespace RooFit;
using namespace std;


void mdat_cut_pede_binaryzation_cluster_dir_image_pixel_num(int iStart_num, int iAccout)
// void mdat_cut_pede_binaryzation_cluster_dir_image()
{
    TCanvas *c1 = new TCanvas("c1", "Canvas", 0, 0, 600, 600);
    TH2F *H2;
    int min2d;
    int max2d;//设置自动颜色范围，请看下面内容
    const int NX = 72;
    const int NY = 72;
    char str[30];
    int sumsig;
    int hit_pixel_num = 0;
    int hit_pixel = 0;

    // char inPdedFile[] = "../data/Ne10DME-80kPa-DV350GV630IV300-55Fe5-80/pede.txt";
    // char inDataFile_dir[] = "../data/Ne10DME-80kPa-DV350GV630IV300-55Fe5-80/";//180 pixels

    char inPdedFile[] = "../data/Ne20DME-80kPa-DV350GV760IV300-55Fe5-10/pede.txt";
    char inDataFile_dir[] = "../data/Ne20DME-80kPa-DV350GV760IV300-55Fe5-10/";//90 pixels

    // char inPdedFile[] = "../data/Ne20DME-80kPa-DV350GV770IV300-X-ray-generator5-10/pede.txt";
    // char inDataFile_dir[] = "../data/Ne20DME-80kPa-DV350GV770IV300-X-ray-generator5-10/";//100 pixels

    // char inPdedFile[] = "../data/Ne10DME-80kPa-DV350GV630IV300-X-ray-generator/pede.txt";//int型，需改3处
    // char inDataFile_dir[] = "../data/Ne10DME-80kPa-DV350GV630IV300-X-ray-generator/";//150 pixels

    char inDataFile[200];
    sprintf(inDataFile, "%s%d-%d.mdat", inDataFile_dir, iStart_num + 1, iStart_num + iAccout);

    ifstream infilePede(inPdedFile);
    ifstream infileSig(inDataFile, ios::binary);

    //////////////////////////////////////////////////read Pede
    int iChipT = 0, iPixelT = 0, iCounter = 0;
    float pedestalT = 0., noiseT = 0.;
    float meanPed[5184];
    while (!infilePede.eof() && iCounter < 5184)
    {
        infilePede >> iChipT >> iPixelT >> pedestalT >> noiseT;
        // meanPed[iCounter] = pedestalT;
        meanPed[iCounter] = 0;//pedestalT for .mdat, zero for .dat-------->3/3
        // rmsPed[iChipT*nPixelsOnChip+iPixelT] = noiseT;
        iCounter++;
    }

    //////////////////////////////////////////////////How many Frame counts
    unsigned short _data0_short[NX][NY]; //size of 1 frame for .mdat, .pd1
    // int _data0_short[NX][NY]; //size of 1 frame for .mdat, .pd1-------->2/3
    int fz = file_loder::file_size(inDataFile);
    // cout << "the size of file is: " << fz << endl;
    int iFrames = 0;
    iFrames = fz / sizeof(_data0_short);
    // cout << "iFrame num is: " << iFrames << endl; //808
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

    // array3D_pixel_num.resize(DEPTH); //1
    // for (int i = 0; i < DEPTH; ++i)
    // {                              //1
    //     array3D_pixel_num[i].resize(HEIGHT); //2

    //     for (int j = 0; j < HEIGHT; ++j)  //2
    //         array3D_pixel_num[i][j].resize(LONGTH); //3
    // }
    //////////////////////////////////////////////////for 3D array

    for (int i = 0; i < iFrames; i++)
    // for (int i = 0; i <= 2317; i++)
    {
        H2 = new TH2F(Form("H2_%d", i), "Projection", 72, 0, 72, 72, 0, 72);
        sumsig = 0;
        unsigned short _data_short[NX][NY];//for .mdat
        // int _data_short[NX][NY];//for .dat-------->1/3
        infileSig.read((char *)(&_data_short), sizeof(_data_short));
        for (int ii = 0; ii < NX; ii++)
        {
            for (int jj = 0; jj < NY; jj++)
            {
                array3D[i][ii][jj] = _data_short[ii][jj] - meanPed[ii * 72 + jj];
                hit_pixel = array3D[i][ii][jj] > 0 ? 1 : 0;
                hit_pixel_num = hit_pixel_num + hit_pixel;
                // sumsig = sumsig + array3D[i][ii][jj];
                H2->Fill(ii, jj, array3D[i][ii][jj]);
                hit_pixel = 0;
            }
        }
//找出像素值的范围，以设置SetRangeUser
        max2d = array3D[i][0][0];
        min2d = array3D[i][0][0];

        for (int ii = 0; ii < NX; ii++)
        {
            for (int jj = 0; jj < NY; jj++)
            {
                max2d = (array3D[i][ii][jj] > max2d) ? array3D[i][ii][jj] : max2d;
                min2d = (array3D[i][ii][jj] < max2d) ? array3D[i][ii][jj] : min2d;
                // a.getP(ii, jj)[0] = (a.getP(ii, jj)[0] > 0) ? 1 : 0;
            }
        }

        sprintf(str, "frame %d", i);
        H2->SetTitle(str);
        H2->GetZaxis()->SetRangeUser(min2d - (min2d+max2d) * 0.2, max2d - (min2d+max2d) * 0.1);
        gStyle->SetPalette(104);//kTemperatureMap,-10,80
        H2->Draw("Colz");
        H2->SetStats(0);
        c1->Modified();
        c1->Update();

        // if(hit_pixel_num >= 180){
        if(hit_pixel_num >= 90){
        // if(hit_pixel_num >= 100){
        // if(hit_pixel_num >= 150){
            cout << min2d << ":" << max2d << endl;
            char buf[200];
            sprintf(&buf[0],"%spixel_num/%d-%d-%d.png" , inDataFile_dir, iStart_num + 1, iStart_num + iAccout, i);
            c1->SaveAs(buf);
        }
        max2d = 0;
        min2d = 0;
        hit_pixel_num = 0;

        // if (gSystem->ProcessEvents()) //不能去除，否则没有动画
        //     break;
    }

    infileSig.close();
    infilePede.close();
    // output.close();
    // output2.close();
}
