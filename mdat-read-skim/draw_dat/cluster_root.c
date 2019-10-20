//find cluster in matrix for tometal-ii-
//image binaryzation
//author Lizili 20191017

#include <iostream>
#include <vector>
// #include "TGraph.h"
using namespace std;

class binArray
{
public:
    int nCol;
    int nRow;
    int *d;

    int get(int x, int y)
    {
        if (x < 0)
            return 0;
        if (x >= nCol)
            return 0;
        if (y < 0)
            return 0;
        if (y >= nRow)
            return 0;

        return d[nRow * x + y];
    }
    int *getP(int x, int y)
    {
        return d + nRow * x + y;
    }
};

int cluster(int x, int y, vector<int> &vx, binArray &a)
{
    a.getP(x, y)[0] = 0; //当前像素值设为0

    if (a.get(x + 1, y) == 1)
        cluster(x + 1, y, vx, a);
    if (a.get(x, y + 1) == 1)
        cluster(x, y + 1, vx, a);
    if (a.get(x - 1, y) == 1)
        cluster(x - 1, y, vx, a);
    if (a.get(x, y - 1) == 1)
        cluster(x, y - 1, vx, a);

    if (a.get(x + 1, y - 1) == 1)
        cluster(x + 1, y - 1, vx, a);
    if (a.get(x + 1, y + 1) == 1)
        cluster(x + 1, y + 1, vx, a);
    if (a.get(x - 1, y + 1) == 1)
        cluster(x - 1, y + 1, vx, a);
    if (a.get(x - 1, y - 1) == 1)
        cluster(x - 1, y - 1, vx, a);

    vx.push_back(x);
    vx.push_back(y);

    return 0;
};


int cluster_root()
{
    TCanvas *c1 = new TCanvas("c1", "c1", 0, 0, 1200, 600);
    c1->Divide(2,1);
    TH2F *H2;
    // H2 = new TH2F("H2", "Projection", 15, 0, 15, 15, 0, 15);
    TH2F *H3;
    // H3 = new TH2F("H3", "Projection", 15, 0, 15, 15, 0, 15);

    H2 = new TH2F("H2", "Projection", 72, 0, 72, 72, 0, 72);
    H3 = new TH2F("H3", "Projection", 72, 0, 72, 72, 0, 72);

    const int min2d = 0;
    const int max2d = 1;
    const int mini_cluster_size_area = 2;//像素个数小于等于2个，就删除
    binArray a;
    // a.nRow = 15;
    // a.nCol = 15;
    a.nRow = 72;
    a.nCol = 72;
    a.d = new int[a.nRow * a.nCol];
    for (int i = 0; i < a.nRow; i++)
    {
        for (int j = 0; j < a.nCol; j++)
        {
            a.getP(i, j)[0] = 0;
        }
    }

//     a.getP(3, 3)[0] = 1;
//     a.getP(3, 4)[0] = 1;
//     a.getP(3, 5)[0] = 1;
//     a.getP(4, 4)[0] = 1;
//     a.getP(4, 3)[0] = 1;
//     a.getP(4, 5)[0] = 1;

//     a.getP(6, 1)[0] = 1;
//     a.getP(6, 2)[0] = 1;
//     a.getP(7, 1)[0] = 1;
//     a.getP(7, 2)[0] = 1;

//     a.getP(3, 7)[0] = 1;
//     a.getP(4, 8)[0] = 1;
//     a.getP(4, 9)[0] = 1;
//     a.getP(3, 9)[0] = 1;
//     a.getP(3, 10)[0] = 1;

//     a.getP(10, 10)[0] = 1;
//     a.getP(11, 11)[0] = 1;
//     a.getP(12, 12)[0] = 1;
//     a.getP(13, 13)[0] = 1;
// ///////////////////////////////////////////
//     a.getP(7, 8)[0] = 1;
//     a.getP(7, 9)[0] = 1;
//     a.getP(8, 8)[0] = 1;
//     a.getP(8, 9)[0] = 1;

//     a.getP(8, 4)[0] = 1;
//     a.getP(8, 5)[0] = 1;
//     a.getP(9, 4)[0] = 1;
//     a.getP(9, 5)[0] = 1;

//     a.getP(2, 0)[0] = 1;
//     a.getP(2, 1)[0] = 1;
//     a.getP(3, 0)[0] = 1;
//     a.getP(3, 1)[0] = 1;

//     a.getP(0, 6)[0] = 1;
//     a.getP(0, 7)[0] = 1;
//     a.getP(1, 6)[0] = 1;
//     a.getP(1, 7)[0] = 1;

//     a.getP(9, 1)[0] = 1;
//     a.getP(9, 2)[0] = 1;
//     a.getP(10, 1)[0] = 1;
//     a.getP(10, 2)[0] = 1;

//     a.getP(0, 9)[0] = 1;
//     a.getP(0, 10)[0] = 1;
//     a.getP(1, 9)[0] = 1;
//     a.getP(1, 10)[0] = 1;

//     a.getP(12, 4)[0] = 1;
//     a.getP(12, 3)[0] = 1;
//     a.getP(13, 2)[0] = 1;
//     a.getP(14, 1)[0] = 1;

//     a.getP(3, 14)[0] = 1;
//     a.getP(4, 14)[0] = 1;
//     a.getP(5, 14)[0] = 1;
//     a.getP(6, 14)[0] = 1;
//     a.getP(7, 14)[0] = 1;
//     a.getP(8, 14)[0] = 1;
//     a.getP(9, 14)[0] = 1;

//     a.getP(1, 12)[0] = 1;
//     a.getP(2, 12)[0] = 1;
//     a.getP(3, 12)[0] = 1;

//     a.getP(11, 7)[0] = 1;
//     a.getP(12, 7)[0] = 1;

//     a.getP(14, 5)[0] = 1;
//     a.getP(14, 6)[0] = 1;

//     a.getP(14, 9)[0] = 1;

//     a.getP(6, 12)[0] = 1;

        a.getP(6, 2)[0] = 1;//
        a.getP(7, 2)[0] = 1;
        a.getP(8, 2)[0] = 1;//
        a.getP(4, 3)[0] = 1;
        a.getP(5, 3)[0] = 1;
        a.getP(6, 3)[0] = 1;
        a.getP(7, 3)[0] = 1;
        a.getP(8, 3)[0] = 1;
        a.getP(9, 3)[0] = 1;
        a.getP(10, 3)[0] = 1;
        a.getP(3, 4)[0] = 1;//
        a.getP(4, 4)[0] = 1;
        a.getP(5, 4)[0] = 1;
        a.getP(6, 4)[0] = 1;
        a.getP(7, 4)[0] = 1;
        a.getP(8, 4)[0] = 1;
        a.getP(9, 4)[0] = 1;
        a.getP(10, 4)[0] = 1;
        a.getP(2, 5)[0] = 1;//
        a.getP(3, 5)[0] = 1;
        a.getP(4, 5)[0] = 1;
        a.getP(5, 5)[0] = 1;
        a.getP(6, 5)[0] = 1;
        a.getP(7, 5)[0] = 1;
        a.getP(8, 5)[0] = 1;
        a.getP(9, 5)[0] = 1;
        a.getP(10, 5)[0] = 1;
        a.getP(11, 5)[0] = 1;
        a.getP(2, 6)[0] = 1;//
        a.getP(3, 6)[0] = 1;
        a.getP(4, 6)[0] = 1;
        a.getP(5, 6)[0] = 1;
        a.getP(6, 6)[0] = 1;
        a.getP(7, 6)[0] = 1;
        a.getP(8, 6)[0] = 1;
        a.getP(9, 6)[0] = 1;
        a.getP(10, 6)[0] = 1;
        a.getP(11, 6)[0] = 1;
        a.getP(12, 6)[0] = 1;
        a.getP(2, 7)[0] = 1;//
        a.getP(3, 7)[0] = 1;
        a.getP(4, 7)[0] = 1;
        a.getP(5, 7)[0] = 1;
        a.getP(6, 7)[0] = 1;
        a.getP(7, 7)[0] = 1;
        a.getP(8, 7)[0] = 1;
        a.getP(9, 7)[0] = 1;
        a.getP(10, 7)[0] = 1;
        a.getP(11, 7)[0] = 1;
        a.getP(12, 7)[0] = 1;
        a.getP(2, 8)[0] = 1;//
        a.getP(3, 8)[0] = 1;
        a.getP(4, 8)[0] = 1;
        a.getP(5, 8)[0] = 1;
        a.getP(6, 8)[0] = 1;
        a.getP(7, 8)[0] = 1;
        a.getP(8, 8)[0] = 1;
        a.getP(9, 8)[0] = 1;
        a.getP(10, 8)[0] = 1;
        a.getP(11, 8)[0] = 1;
        a.getP(12, 8)[0] = 1;
        a.getP(2, 9)[0] = 1;//
        a.getP(3, 9)[0] = 1;
        a.getP(4, 9)[0] = 1;
        a.getP(5, 9)[0] = 1;
        a.getP(6, 9)[0] = 1;
        a.getP(7, 9)[0] = 1;
        a.getP(8, 9)[0] = 1;
        a.getP(9, 9)[0] = 1;
        a.getP(10, 9)[0] = 1;
        a.getP(11, 9)[0] = 1;
        a.getP(12, 9)[0] = 1;
        a.getP(2, 10)[0] = 1;//
        a.getP(3, 10)[0] = 1;
        a.getP(4, 10)[0] = 1;
        a.getP(5, 10)[0] = 1;
        a.getP(6, 10)[0] = 1;
        a.getP(7, 10)[0] = 1;
        a.getP(8, 10)[0] = 1;
        a.getP(9, 10)[0] = 1;
        a.getP(10, 10)[0] = 1;
        a.getP(11, 10)[0] = 1;
        a.getP(3, 11)[0] = 1;//
        a.getP(4, 11)[0] = 1;
        a.getP(5, 11)[0] = 1;
        a.getP(6, 11)[0] = 1;
        a.getP(7, 11)[0] = 1;
        a.getP(8, 11)[0] = 1;
        a.getP(9, 11)[0] = 1;
        a.getP(10, 11)[0] = 1;
        a.getP(4, 12)[0] = 1;//
        a.getP(5, 12)[0] = 1;
        a.getP(6, 12)[0] = 1;
        a.getP(7, 12)[0] = 1;
        a.getP(8, 12)[0] = 1;
        a.getP(9, 12)[0] = 1;
        a.getP(10, 12)[0] = 1;
        a.getP(5, 13)[0] = 1;//
        a.getP(6, 13)[0] = 1;
        a.getP(7, 13)[0] = 1;

        a.getP(1, 13)[0] = 1;
        a.getP(13, 12)[0] = 1;
        a.getP(13, 1)[0] = 1;
        a.getP(2, 1)[0] = 1;
        a.getP(6, 0)[0] = 1;

    vector<int> vx;
    vector<vector<int> > vc; //多维向量
    vc.clear();

    vector<vector <int> > ivec;
    ivec.resize(a.nRow,vector<int>(a.nCol, 0));

    vector<vector <int> > ivec2;
    ivec2.resize(a.nRow,vector<int>(a.nCol, 0));

    for(int i = 0; i < a.nRow; i++){
        for(int j = 0; j < a.nCol; j++){
            ivec[i][j] = a.getP(i, j)[0];
            ivec2[i][j] = a.getP(i, j)[0];
            H2->Fill(i, j, ivec[i][j]);
        }
    }

    for (int i = 0; i < a.nRow; i++)
    {
        for (int j = 0; j < a.nCol; j++)
        {
            if (a.get(i, j) == 0)
                continue;
            vx.clear();
            cluster(i, j, vx, a);
            vc.push_back(vx);
        }
    }

    c1->cd(1);
    H2->GetZaxis()->SetRangeUser(min2d, max2d);
    H2->SetFillColor(1);
    H2->SetStats(0);
    H2->Draw("box");

//*******************del boundary and mini isolate cluster*******************************
    cout << "cluster No is: " << vc.size() << endl;
    for (int i = 0; i < vc.size(); i++) //vc中的vector元素的个数
    {
            cout << endl;
            cout << "Pixels of " << i << "th valid cluster: " << vc[i].size() / 2 << endl;
            for (int j = 0; j < vc[i].size(); j++) //vc中第i个vector元素的长度
            {    
                
                if (j % 2 == 0)
                {
                    cout << "(" << vc[i][j] << "," << vc[i][j + 1] << ")";
                    if(vc[i].size() / 2 <= mini_cluster_size_area){//像素个数少于mini_cluster_size_area，则删除
                        ivec2[vc[i][j]][vc[i][j + 1]] = 0;
                        cout<< "x ";
                    }
                    
                }
                if(vc[i][j]==0 || vc[i][j]==a.nCol-1)//边界的删除
                {
                    cout << "xxx ";
                    for (int j = 0; j < vc[i].size(); j++)
                    {
                        if (j % 2 == 0)
                        {
                            // ivec2[vc[i][j]][vc[i][j + 1]] = 0;
                        }
                    }
                    // break;//不显示后续的坐标
                }
            }
    }
    cout << endl;
//*******************ddel boundary and mini isolate cluster*******************************

    for (int i = 0; i < a.nRow; i++)
    {
        for (int j = 0; j < a.nCol; j++)
        {
            H3->Fill(i, j, ivec2[i][j]);
        }
    }

    c1->cd(2);
    H3->GetZaxis()->SetRangeUser(min2d, max2d);
    H3->SetFillColor(1);
    H3->SetStats(0);
    H3->Draw("box");

    char buf[100];
    sprintf(&buf[0], "./cluster_root_compare.png");
    c1->SaveAs(buf);

    return 0;
}