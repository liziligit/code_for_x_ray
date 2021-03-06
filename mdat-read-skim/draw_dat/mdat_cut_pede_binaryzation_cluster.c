//find cluster in matrix for tometal-ii-
//image binaryzation and extract cluster
//author Lizili 20191020

//problem
//5-176右边界去除
//5-75,127中间斑点正常
//7-137右边界去除
//7-277下边界去除
//7-57中间第二大斑点删除v
//7-50中间斑点显示

// #include <vector>
// #include <unistd.h>//sleep(s)
#include <stdio.h> //创建文件夹
#include "loader_file.h"
#include "cluster.h"
using namespace std;

int mdat_cut_pede_binaryzation_cluster()
{
    TCanvas *c1 = new TCanvas("c1", "Canvas", 0, 0, 1200, 600);
    c1->Divide(2, 1);
    TH2F *H2;
    TH2F *H3;
    const int min2d = -10;
    const int max2d = 10;
    const int NX = 72;
    const int NY = 72;
    char str2[30];
    char str3[30];
    int sumsig_ADC;
    double cog_x = 0;
    double cog_y = 0;
    double up_x = 0;
    double up_y = 0;
    double down_x = 0;
    double boundary_min = 10;//重心坐标下限
    double boundary_max = 60;//重心坐标上限

    binArray a;
    a.nRow = NX;
    a.nCol = NY;
    a.d = new int[a.nRow * a.nCol];
    const int mini_cluster_size_area = 20; //像素个数小于10个，就删除
    for (int i = 0; i < a.nRow; i++)
    {
        for (int j = 0; j < a.nCol; j++)
        {
            a.getP(i, j)[0] = 0;
        }
    }

    vector<int> vx;
    vx.clear();
    vector<vector<int> > vc; //二维向量
    vc.clear();

    vector<int> ivec_sumsigADC; //存放每帧中束团的ADC值的和，没有束团则ADC值的和为0
    ivec_sumsigADC.clear();
    vector<int> ivec_sumsigADC_filter; //存放有束团的帧的ADC值的和，帧数较少
    ivec_sumsigADC_filter.clear();
    vector<int> isBoundary; //非边界束团则标记为0，是边界束团则标记为1
    isBoundary.clear();

    char inPdedFile[] = "../data/pede.txt";
    // char inDataFile[] = "../data/out5.mdat";
    char inDataFile[] = "../data/out3.mdat";
    // char inDataFile[] = "../data/out7.mdat";
    // char inDataFile[] = "../data/out8.mdat";
    // char inDataFile[] = "../data/out9.mdat";
    // char inDataFile[] = "../data/out10.mdat";
    // char inDataFile[] = "../data/out11.mdat";
    // char inDataFile[] = "../data/out12.mdat";
    // char output_txt[] = "./output_txt.dat"; //for debug
    char output_mdat[] = "../data/out_extract.mdat"; //for debug
    ifstream infilePede(inPdedFile);
    ifstream infileSig(inDataFile, ios::binary);
    ofstream output_m(output_mdat, ios::out | ios::binary | ios::app);

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
    // cout << "the size of file is: " << fz << endl;
    int iFrames = 0;
    iFrames = fz / sizeof(_data0_short);
    cout << "iFrame num is: " << iFrames << endl; //808
    //////////////////////////////////////////////////How many Frame counts

    //*******************for 3D array*******************************
    int DEPTH = iFrames;
    int HEIGHT = NY;
    int LONGTH = NX;
    // 初始化
    vector<vector<vector<float> > > array3D(DEPTH, vector<vector<float>>(HEIGHT, vector<float>(LONGTH, 0)));
    // 初始化，用来存放过滤后的帧
    vector<vector<vector<unsigned short> > > ivec(DEPTH, vector<vector<unsigned short>>(HEIGHT, vector<unsigned short>(LONGTH, 0)));
    //*******************for 3D array*******************************

    // ofstream output;
    // output.open(output_txt);//覆盖模式

    // for (int i = 0; i < iFrames; i++)
    for (int i = 0; i <= 116; i++)
    {
        H2 = new TH2F(Form("H2_%d", i), "Projection", 72, 0, 72, 72, 0, 72);
        H3 = new TH2F(Form("H3_%d", i), "Projection", 72, 0, 72, 72, 0, 72);
        sumsig_ADC = 0;
        unsigned short _data_short[NX][NY];
        infileSig.read((char *)(&_data_short), sizeof(_data_short));
        for (int ii = 0; ii < NX; ii++)
        {
            for (int jj = 0; jj < NY; jj++)
            {
                array3D[i][ii][jj] = _data_short[ii][jj] - (meanPed[ii * 72 + jj] + 5 * rmsPed[ii * 72 + jj]);
                a.getP(ii, jj)[0] = array3D[i][ii][jj];

                a.getP(ii, jj)[0] = (a.getP(ii, jj)[0] > 0) ? 1 : 0;

                ivec[i][ii][jj] = a.getP(ii, jj)[0];

                array3D[i][ii][jj] = array3D[i][ii][jj] + 5 * rmsPed[ii * 72 + jj];

                H2->Fill(ii, jj, array3D[i][ii][jj]);
            }
        }

        for (int ii = 0; ii < a.nRow; ii++)
        {
            for (int jj = 0; jj < a.nCol; jj++)
            {
                if (a.get(ii, jj) == 0)
                    continue;
                vx.clear();
                cluster(ii, jj, vx, a);
                vc.push_back(vx);
            }
        }

        //*******************del boundary and mini isolate cluster*******************************
        vector<int> sum_ivec(vc.size(), 0);    //束团ADC的值
        int index_max = 0;                     //index_max用于接收最大值下标
        for (int ii = 0; ii < vc.size(); ii++) //vc中的vector元素的个数，即一帧中束团的个数
        {
            //*******************只保留特定像素数量的束团*******************************
            if (vc[ii].size() / 2 <= mini_cluster_size_area) //像素个数少于一定量，则删除
            {
                for (int jj = 0; jj < vc[ii].size(); jj++) //vc中第i个vector元素的长度
                {
                    if (jj % 2 == 0)
                    {
                        ivec[i][vc[ii][jj]][vc[ii][jj + 1]] = 0;
                    }
                }
            }
            //*******************只保留特定像素数量的束团*******************************
            //*******************去除边界束团*****************************************
            for (int jj = 0; jj < vc[ii].size(); jj++)
            {
                if (vc[ii][jj] == 0 || vc[ii][jj] == a.nCol - 1) //边界的删除
                {
                    for (int jj = 0; jj < vc[ii].size(); jj++)
                    {
                        if (jj % 2 == 0)
                        {
                            ivec[i][vc[ii][jj]][vc[ii][jj + 1]] = 0;
                        }
                    }
                    break; //终止外层循环
                }
            }
            //*******************去除边界束团*****************************************
            sum_ivec[ii] = 0;
            for (int jj = 0; jj < vc[ii].size(); jj++)
            {
                if (jj % 2 == 0)
                {
                    //对每个束团中像素值求和，因为值为0，1，所以是束团像素的个数
                    sum_ivec[ii] = sum_ivec[ii] + ivec[i][vc[ii][jj]][vc[ii][jj + 1]];
                }
            }
        } //end for (int ii = 0; ii < vc.size(); ii++) //vc中的vector元素的个数，即束团的个数

        //*******************找到最大束团的下标位置*******************************
        for (int i_sum_ivec = 0; i_sum_ivec < sum_ivec.size(); ++i_sum_ivec) //找最大束团值及坐标
        {
            if (sum_ivec[i_sum_ivec] > sum_ivec[index_max])
            {
                index_max = i_sum_ivec; //用下标存储最大值下标，不仅可以找到最大值，也可以找到它的位置
            }
        }
        //*******************找到最大束团的下标位置*******************************
        // cout << "pixels counts=" << sum_ivec[index_max] << ",index_max=" << index_max << endl;//束团中像素的个数，vc.size()为0时不能输出

        //*******************剩余束团中，只保留最大的*******************************
        for (int ii = 0; ii < vc.size(); ii++)
        {
            for (int jj = 0; jj < vc[ii].size(); jj++)
            {
                if (ii == index_max)
                {
                    continue;
                }
                else
                {
                    if (jj % 2 == 0)
                    {
                        ivec[i][vc[ii][jj]][vc[ii][jj + 1]] = 0;
                    }
                }
            }
        }
        //*******************剩余束团中，只保留最大的*******************************

        //*******************del boundary and mini isolate cluster*******************************

        for (int ii = 0; ii < a.nRow; ii++)
        {
            for (int jj = 0; jj < a.nCol; jj++)
            {
                if (ivec[i][ii][jj] == 1)
                {
                    ivec[i][ii][jj] = array3D[i][ii][jj];
                }
                H3->Fill(ii, jj, ivec[i][ii][jj]);
                sumsig_ADC = sumsig_ADC + ivec[i][ii][jj];
            }
        }

        ivec_sumsigADC.push_back(i);
        ivec_sumsigADC.push_back(sumsig_ADC);
        // cout << ivec_sumsigADC[2 * i] << " : " << ivec_sumsigADC[2 * i + 1] << endl; //显示每帧的ADC值和

        c1->cd(1);
        sprintf(str2, "%s frame %d", inDataFile, i);
        H2->SetTitle(str2);
        H2->GetZaxis()->SetRangeUser(min2d, max2d);
        // H2->SetFillColor(1);
        H2->SetStats(0);
        H2->Draw("Colz");

        c1->cd(2);
        sprintf(str3, "%s frame %d", inDataFile, i);
        H3->SetTitle(str3);
        H3->GetZaxis()->SetRangeUser(min2d, max2d);
        H3->SetStats(0);
        H3->Draw("Colz");
        c1->Update();

        // createFolder("./6");
        // if (335 <= i && i <= 348)
        // {
        // char buf[100];
        // sprintf(&buf[0], "./00/mdat_frame%d_%d.png", i, ivec_sumsigADC[2*i+1]);
        // c1->SaveAs(buf);
        // // }

        // sleep(1);//second
        // usleep(1000000); // will sleep for 1s
        sum_ivec.clear();
        vc.clear();

        // if(i >= 619)
        // {
        //     sleep(5);//second
        // }

        if (gSystem->ProcessEvents()) //不能去除，否则没有动画
            break;
    } //end for (int i = 0; i < iFrames; i++)

    //*******************extract good cluster*******************************
    for (int i = 1; i < ivec_sumsigADC.size() / 2 - 1; i++) //第一帧的束团可能为上一个文件的图像，不能取
    {
        if (ivec_sumsigADC[2 * i + 1] != 0)
        {
            //提取条件，前1帧须为0，且前后帧不能接触边界，后一帧较小
            if (ivec_sumsigADC[2 * i + 1 - 2] == 0 &&                      //前1帧
                ivec_sumsigADC[2 * i + 1 + 2] < ivec_sumsigADC[2 * i + 1]) //后1帧小些
            {
                ivec_sumsigADC_filter.push_back(ivec_sumsigADC[2 * i]);
                ivec_sumsigADC_filter.push_back(ivec_sumsigADC[2 * i + 1]);
            }
        }
        else
        {
            continue;
        }
    }
//*******************求最终束团的重心*******************************
    for(int i = 0; i < ivec_sumsigADC_filter.size() / 2; i++)
    {
        up_x = 0;
        up_y = 0;
        down_x = 0;
        for (int ii = 0; ii < NX; ii++)
        {
            for(int jj = 0; jj < NY; jj++)
            {
                up_x = up_x + ivec[ivec_sumsigADC_filter[2 * i]][ii][jj] * ii;
                down_x = down_x + ivec[ivec_sumsigADC_filter[2 * i]][ii][jj];
                up_y = up_y + ivec[ivec_sumsigADC_filter[2 * i]][ii][jj] * jj;
            }
        }
        cog_x = up_x / down_x;
        cog_y = up_y / down_x;

        cout << "(" << cog_x << "," << cog_y << ")"<< endl;

        if (boundary_max <= cog_x || cog_x <= boundary_min || boundary_max <= cog_y || cog_y <= boundary_min)
        {
            ivec_sumsigADC_filter[2 * i + 1] = 0;
        }
        else
        {
            continue;
        }
    }
//*******************求最终束团的重心*******************************

    int iCount_exract = 0;
    for (int i = 0; i < ivec_sumsigADC_filter.size() / 2; i++)
    {
        if (ivec_sumsigADC_filter[2 * i + 1] != 0)
        {
            cout << ivec_sumsigADC_filter[2 * i] << ":" << ivec_sumsigADC_filter[2 * i + 1] << endl;

            for (int ii = 0; ii < NX; ii++)
            {
                for (int jj = 0; jj < NY; jj++)
                {
                    //只输出有束团的帧到文件
                    // output_m.write((char *)(&ivec[ivec_sumsigADC_filter[2 * i]][ii][jj]), sizeof(ivec[ivec_sumsigADC_filter[2 * i]][ii][jj])); //extract to file
                }
            }
            iCount_exract++;
        }
        else
        {
            continue;
        }
    }

    // cout << "extract "<< ivec_sumsigADC_filter.size() / 2 << " frame"<< endl;
    cout << "extract "<< iCount_exract << " frame"<< endl;

    //*******************extract good cluster*******************************

    ivec_sumsigADC.clear();
    ivec_sumsigADC_filter.clear();
    array3D.clear();
    ivec.clear();

    output_m.close();
    infileSig.close();
    infilePede.close();

    return 0;
}
