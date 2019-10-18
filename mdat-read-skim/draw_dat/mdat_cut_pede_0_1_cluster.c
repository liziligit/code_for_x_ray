// #include <stdio.h>
// #include "TGraph.h"
// #include <string.h> // for strrchr()
// #include <vector>
// #include <cstdlib> // exit()
#include "loader_file.h"
// using namespace std;

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
    if (a.get(x - 1, y) == 1)
        cluster(x - 1, y, vx, a);
    if (a.get(x, y + 1) == 1)
        cluster(x, y + 1, vx, a);
    if (a.get(x, y - 1) == 1)
        cluster(x, y - 1, vx, a);

    if (a.get(x + 1, y - 1) == 1)
        cluster(x + 1, y - 1, vx, a);
    if (a.get(x + 1, y + 1) == 1)
        cluster(x + 1, y + 1, vx, a);
    if (a.get(x - 1, y - 1) == 1)
        cluster(x - 1, y - 1, vx, a);
    if (a.get(x - 1, y + 1) == 1)
        cluster(x - 1, y + 1, vx, a);

    vx.push_back(x);
    vx.push_back(y);

    return 0;
};

void mdat_cut_pede_0_1_cluster()
{
    TCanvas *c1 = new TCanvas("c1", "Canvas", 0, 0, 1200, 600);
    c1->Divide(2,1);
    TH2F *H2;
    TH2F *H3;
    const int min2d = 0;
    const int max2d = 1;
    const int NX = 72;
    const int NY = 72;
    char str[30];
    int sumsig;

    binArray a;
    a.nRow = NX;
    a.nCol = NY;
    a.d = new int[a.nRow * a.nCol];
    const int mini_cluster_size_boundary = 1;//边界像素个数满足一定数量才处理
    const int mini_cluster_size_area = 10;//中间像素个数满足一定数量才处理
    for (int i = 0; i < a.nRow; i++)
    {
        for (int j = 0; j < a.nCol; j++)
        {
            a.getP(i, j)[0] = 0;
        }
    }

        vector<int> vx;
        vector<vector<int> > vc; //多维向量
        vc.clear();

        vector<vector <int> > ivec;
        ivec.resize(a.nRow,vector<int>(a.nCol, 0));

        vector<vector <int> > ivec2;
        ivec2.resize(a.nRow,vector<int>(a.nCol, 0));
    
    char inPdedFile[] = "../data/pede.txt";
    char inDataFile[] = "../data/out5.mdat";
    ifstream infilePede(inPdedFile);
    ifstream infileSig(inDataFile, ios::binary);

    //////////////////////////////////////////////////read Pede
    int iChipT = 0, iPixelT = 0, iCounter = 0;
    float pedestalT = 0., noiseT = 0.;
    float meanPed[5184];
    float rmsPed[5184];

    while (!infilePede.eof() && iCounter < 5184)
    {
        infilePede >> iChipT >> iPixelT >> pedestalT >> noiseT;
        meanPed[iCounter] = pedestalT;
        // meanPed[iCounter] = 0;//pedestalT for .mdat, zero for .dat
        rmsPed[iCounter] = noiseT;
        iCounter++;
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
    // for (int i = 0; i < iFrames; i++)
    for (int i = 0; i < 76; i++)
    {
        H2 = new TH2F(Form("H2_%d", i), "Projection", 72, 0, 72, 72, 0, 72);
        H3 = new TH2F(Form("H3_%d", i), "Projection", 72, 0, 72, 72, 0, 72);
        sumsig = 0;
        unsigned short _data_short[NX][NY];
        infileSig.read((char *)(&_data_short), sizeof(_data_short));
        for (int ii = 0; ii < NX; ii++)
        {
            for (int jj = 0; jj < NY; jj++)
            {
                a.getP(ii, jj)[0] = _data_short[ii][jj] - (meanPed[ii * 72 + jj] + 5 * rmsPed[ii * 72 + jj]);
                
                if (a.getP(ii, jj)[0] > 0)
                {
                    a.getP(ii, jj)[0] = 1;
                }
                else if (a.getP(ii, jj)[0] <= 0)
                {
                    a.getP(ii, jj)[0] = 0;
                }
                array3D[i][ii][jj] = a.getP(ii, jj)[0];
                ivec[ii][jj] = a.getP(ii, jj)[0];
                ivec2[ii][jj] = a.getP(ii, jj)[0];
                H2->Fill(ii, jj, ivec[ii][jj]);
                sumsig = sumsig + a.getP(ii, jj)[0];
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
        sprintf(str, "frame %d", i);
        H2->SetTitle(str);
        H2->GetZaxis()->SetRangeUser(min2d, max2d);
        H2->SetFillColor(1);
        H2->SetStats(0);
        H2->Draw("box");

//*******************del boundary and mini isolate cluster*******************************
    // cout << "cluster No is: " << vc.size() << endl;
    for (int i = 0; i < vc.size(); i++) //vc中的vector元素的个数
    {
        if ( vc[i].size() / 2 >= mini_cluster_size_boundary ) //边界有至少一个像素，则删除
        {
            cout << endl;
            // cout << "Pixels of " << i << "th valid cluster: " << vc[i].size() / 2 << endl;
            for (int j = 0; j < vc[i].size(); j++) //vc中第i个vector元素的长度
            {    
                
                if (j % 2 == 0)
                {
                    // cout << "(" << vc[i][j] << "," << vc[i][j + 1] << ")";
                    if(vc[i].size() / 2 <= mini_cluster_size_area){//像素个数少于mini_cluster_size_area，则删除
                        ivec2[vc[i][j]][vc[i][j + 1]] = 0;
                        // cout<< "x ";
                    }
                    
                }
                if(vc[i][j]==0 || vc[i][j]==a.nCol-1)//ok
                {
                    // cout << "xxx ";
                    for (int j = 0; j < vc[i].size(); j++)
                    {
                        if (j % 2 == 0)
                        {
                            ivec2[vc[i][j]][vc[i][j + 1]] = 0;
                            // cout << "(" << vc[i][j] << "," << vc[i][j + 1] << ")";
                        }
                    }
                    // break;//不显示后续的坐标
                }
            }
        }

        // if(vc[i].size() / 2 <= mini_cluster_size_area) //像素个数少于mini_cluster_size_area，则删除
        // {
        //     for (int j = 0; j < vc[i].size(); j++){
        //         if (j % 2 == 0){
        //             cout << "(" << vc[i][j] << "," << vc[i][j + 1] << ")"<< "x ";
        //             ivec2[vc[i][j]][vc[i][j + 1]] = 0;
        //         }
        //     }
        // }
    }
    // cout << endl;
//*******************ddel boundary and mini isolate cluster*******************************

    // cout << "cluster No is: " << vc.size() << endl;
    // for (int i = 0; i < vc.size(); i++) //vc中的vector元素的个数
    // {
    //     if (vc[i].size() / 2 >= mini_cluster_size_boundary) //像素个数满足一定数量
    //     {
    //         cout << endl;
    //         cout << "Pixels of " << i << "th valid cluster: " << vc[i].size() / 2 << endl;
    //         for (int j = 0; j < vc[i].size(); j++) //vc中第i个vector元素的长度
    //         {
    //             if (j % 2 == 0)
    //             {
    //                 cout << "(" << vc[i][j] << "," << vc[i][j + 1] << ")";
    //             }
    //         }
    //     }
    // }
    // cout << endl;

    for (int i = 0; i < a.nRow; i++)
    {
        for (int j = 0; j < a.nCol; j++)
        {
            H3->Fill(i, j, ivec2[i][j]);
        }
    }


    
    c1->cd(2);
    sprintf(str, "frame %d", i);
    H3->SetTitle(str);
    H3->GetZaxis()->SetRangeUser(min2d, max2d);
    H3->SetFillColor(1);
    H3->SetStats(0);
    H3->Draw("box");  

        // cout << i << ":" << sumsig << endl;
        // sprintf(str, "frame %d", i);
        // H2->SetTitle(str);
        // H2->GetZaxis()->SetRangeUser(min2d, max2d);
        // H2->SetFillColor(1);
        // H2->Draw("box");
        // H2->SetStats(0);
        // c1->Modified();
        // c1->Update();

        if (i == 75)
        {
            char buf[100];
            sprintf(&buf[0], "./mdat_frame%d_0_1.png", i);
            c1->SaveAs(buf);
        }

        if (gSystem->ProcessEvents()) //不能去除，否则没有动画
            break;
    }

    infileSig.close();
    infilePede.close();
}
