//去除本底，ADC值的和输出到文件，以便画出能谱图

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

// TCanvas *c1 = new TCanvas("c1", "Canvas", 0, 0, 600, 600);
void mdat_cut_pede_binaryzation_cluster_dir_energy(int iStart_num, int iAccout)
{
    // TH2F *H2;
    const int min2d = -10;
    const int max2d = 10;
    const int NX = 72;
    const int NY = 72;
    char str[30];
    int sumsig;
    // int ldata;//read for .dat
    //  unsigned short ldata;//read for .mdat
    // char inPdedFile[] = "../data/pede.txt";
    // char inPdedFile[] = "/Volumes/Elements/THGEM+Topmetal_data/Ne10DME-80kPa-DV350GV630IV300-55Fe/pede.txt";
    // char inDataFile[] = "/Volumes/Elements/THGEM+Topmetal_data/Ne10DME-80kPa-DV350GV630IV300-55Fe/out2.mdat";
    // char inDataFile[] = "/Volumes/Elements/THGEM+Topmetal_data/Ne10DME-80kPa-DV350GV630IV300-55Fe/out1.mdat";
    char inPdedFile[] = "../data/Ne10DME-80kPa-DV350GV630IV300-55Fe5-20/pede.txt";
    char inDataFile_dir[] = "../data/Ne10DME-80kPa-DV350GV630IV300-55Fe5-20/";

    // char inPdedFile[] = "../data/Ne20DME-80kPa-DV350GV760IV300-55Fe5-30/pede.txt";
    // char inDataFile_dir[] = "../data/Ne20DME-80kPa-DV350GV760IV300-55Fe5-30/";

    // char inPdedFile[] = "../data/Ne20DME-80kPa-DV350GV770IV300-X-ray-generator/pede.txt";
    // char inDataFile_dir[] = "../data/Ne20DME-80kPa-DV350GV770IV300-X-ray-generator/";

    // char inPdedFile[] = "../data/Ne10DME-80kPa-DV350GV630IV300-X-ray-generator/pede.txt";
    // char inDataFile_dir[] = "../data/Ne10DME-80kPa-DV350GV630IV300-X-ray-generator/";

    char inDataFile[200];
    sprintf(inDataFile, "%s%d-%d.mdat", inDataFile_dir, iStart_num + 1, iStart_num + iAccout);
    // char inDataFile[] = "../data/out5.mdat";
    // char inDataFile[] = "../data/out7.mdat";
    // char inDataFile[] = "../data/extract55Fe01_0_3000.mdat";
    // char output_txt[] = "./output_txt.dat"; //输出72*72矩阵
    char output_energy[] = "../data/Ne10DME-80kPa-DV350GV630IV300-55Fe5-20/00_output_energy_1_10000.dat"; //输出每帧ADC的和
    // char output_energy[] = "../data/Ne20DME-80kPa-DV350GV760IV300-55Fe5-30/00_output_energy_1_9000.dat"; //输出每帧ADC的和
    // char output_energy[] = "../data/Ne20DME-80kPa-DV350GV770IV300-X-ray-generator/00_output_energy_1_5665.dat"; //输出每帧ADC的和
    // char output_energy[] = "../data/Ne10DME-80kPa-DV350GV630IV300-X-ray-generator/00_output_energy_1_5299.dat"; //输出每帧ADC的和

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
        // cout << pedestalT << endl;
    }

    //////////////////////////////////////////////////How many Frame counts
    unsigned short _data0_short[NX][NY]; //size of 1 frame for .mdat, .pd1
    // int _data0_short[NX][NY]; //size of 1 frame for .mdat, .pd1-------->2/3
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
    // ofstream output;
    // output.open(output_txt);//清空模式

    ofstream output2;
    output2.open(output_energy, ios::out | ios::app);//追加模式

    for (int i = 0; i < iFrames; i++)
    // for (int i = 0; i < 2; i++)
    {
        // H2 = new TH2F(Form("H2_%d", i), "Projection", 72, 0, 72, 72, 0, 72);
        sumsig = 0;
        unsigned short _data_short[NX][NY];//for .mdat
        // int _data_short[NX][NY];//for .dat-------->1/3
        infileSig.read((char *)(&_data_short), sizeof(_data_short));
        for (int ii = 0; ii < NX; ii++)
        {
            for (int jj = 0; jj < NY; jj++)
            {
                array3D[i][ii][jj] = _data_short[ii][jj] - meanPed[ii * 72 + jj];
                sumsig = sumsig + array3D[i][ii][jj];
                // H2->Fill(ii, jj, array3D[i][ii][jj]);
            }
        }

        output2 << fixed << setprecision(0) <<setiosflags(ios::left)<< setw(5) << i << " " << sumsig << endl;
        // cout << i << ":" << sumsig << endl;
        // sprintf(str, "frame %d", i);
        // H2->SetTitle(str);
        // H2->GetZaxis()->SetRangeUser(min2d, max2d);
        // H2->Draw("Colz");
        // H2->SetStats(0);
        // c1->Modified();
        // c1->Update();

        // if(i == 75){
            // char buf[100];
            // sprintf(&buf[0],"./mdat_frame%d_%d.png",i,sumsig);
            // c1->SaveAs(buf);
        // }

        // sleep(1);//second
        // usleep(1000000); // will sleep for 1s

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

        // if (gSystem->ProcessEvents()) //不能去除，否则没有动画
        //     break;
    }

    infileSig.close();
    infilePede.close();
    // output.close();
    output2.close();
}
