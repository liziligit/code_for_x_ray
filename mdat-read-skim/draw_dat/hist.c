#include <iostream>
#include "TGraph.h"
using namespace std;

TH1F *H2;
int k = 0;

int hist()
{
    TCanvas *c1 = new TCanvas("c1","Canvas",0,0,500,500);
    H2 = new TH1F("H2","ADC",12, 0, 12);//一个个的增加
    for(int i = 0; i < 6; i++)
    {
        H2->SetBinContent(i,i);// H2->SetBinContent(iBin,sumsig);
    }
    for(int i = 6; i < 12; i++)
    {
        H2->SetBinContent(i,i);// H2->SetBinContent(iBin,sumsig);
    }
    H2->Draw();

return 0;
}