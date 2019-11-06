//root Hitmap1.C
#include <unistd.h>
#include <vector>
#include "TThread.h"
#include <TFile.h>
#include <TNtuple.h>
#include <TH2.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TRandom3.h>
#include <TBenchmark.h>
#include <TInterpreter.h>
#include <TApplication.h>
#include <iostream>
#include <stdlib.h>

#include <string>
#include <fstream>
#include <sstream>//字符串流stringstream

using namespace std;
int x_ray_energy_spectrum2()
{
    // int col02, col04, col05;
    // char col01[100], col03[100];
    int col01, col02;

    // gStyle->SetOptStat(1111);
    gStyle->SetOptFit();//1011

    TCanvas *c1 = new TCanvas("c1", "stacked hists", 500, 500);

    TH1F *chip01 = new TH1F("chip01", "This is the energy spectrum", 120, 0, 12000);
    // TH1F *chip01 = new TH1F("chip01", "This is the energy spectrum", 80, 0, 8000);
    // TH1F *chip01 = new TH1F("chip01", "This is the energy spectrum", 100, 0, 10000);

    chip01->SetLineColor(kRed);

    char inputfile01[100];

    sprintf(inputfile01, "../data/Ne10DME-80kPa-DV350GV630IV300-55Fe5-70/00_output_energy_1_10000.dat");
    // sprintf(inputfile01, "../data/Ne20DME-80kPa-DV350GV760IV300-55Fe/00_output_energy_1_9000.dat");
    // sprintf(inputfile01, "../data/Ne20DME-80kPa-DV350GV770IV300-X-ray-generator/00_output_energy_1_5665.dat");

    ifstream fin(inputfile01, std::ios::in);

    //   }

    char line[1024]={0};//用于定义读取一行的文本的变量
    int x = 0;
    int y = 0;//定义了 x y 2个变量，用于存放读取1行数据后，分别存放每行的2个数据

    //用一个循环读取每行数据，读取行的函数是getline()函数，然后利用stringstream将每行文本自动按照空格分列，并分别存放到对应的二个整形变量中
    while(fin.getline(line, sizeof(line)))
    {
        stringstream word(line);
        word >> x;
        word >> y;
        // fscanf(fp01, "%s %d %s %d %d", col01, &col02, col03, &col04, &col05);
        // fscanf(fp01, "%d %d", &col01, &col02);
        // fscanf (fp02,"%d %s %s %d %s %d %s %d %s %lf %s %lf %s %s", &ccol01, ccol02, ccol03, &ccol04, ccol05, &ccol06, ccol07, &ccol08, ccol09, &ccol10, ccol11, &ccol12, ccol13, ccol14);
        // chip01->Fill(col05);
        // chip01->Fill(col02);
        chip01->Fill(y);

        // if ((Lines % 50) == 0)
        // {
        //     c1->Modified();
        //     c1->Update();
        // }
    }

    chip01->Draw();
    // chip01->Fit("gaus","Q");//gaus fit，Q is Quiet mode, no show paremeters in shell
    chip01->Fit("gaus");//fit chip01
    // chip01->Fit("gaus","R","",2700,12000);//R is Use the range speciﬁed in the function range
    // chip01->Fit("gaus","R","",680,4000);//R is Use the range speciﬁed in the function range
    // chip01->Fit("gaus","R","",100,6000);//R is Use the range speciﬁed in the function range

    TLatex l1;
    l1.SetTextFont(42);
    l1.SetTextAlign(21);
    l1.SetTextSize(.04);
    l1.DrawLatexNDC(.5, .03, "ADC value");

    TLatex l2;
    l2.SetTextFont(42);
    l2.SetTextAlign(21);
    l2.SetTextSize(.04);
    l2.SetTextAngle(90);
    l2.DrawLatexNDC(.03, .5, "Count");

    TLatex l3;
    l3.SetTextFont(42);
    l3.SetTextAlign(21);
    l3.SetTextSize(.04);
    l3.SetTextAngle(0);
    l3.SetTextColor(kBlue);
    l3.DrawLatexNDC(.63, .64, "Energy count");

    c1->SaveAs(TString::Format("./Ne10DME-80kPa-DV350GV630IV300-55Fe5-70.png"));
    // c1->SaveAs(TString::Format("./Ne20DME-80kPa-DV350GV760IV300-55Fe.png"));
    // c1->SaveAs(TString::Format("./Ne20DME-80kPa-DV350GV770IV300-X-ray-generator.png"));

    fin.clear();//就是刷新缓存
    fin.close();//并关闭文件
    return 0;
}
