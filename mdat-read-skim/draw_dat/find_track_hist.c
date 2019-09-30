#include <stdio.h> //sprintf()
#include <vector>
#include <iostream>
#include <fstream>       //ifsream读,ofstream写,fstream读写
#include <sys/types.h>   //opendir()
#include <dirent.h>      //opendir()
#include "loader_file.h" //file_size()
#include "TGraph.h"
#include <unistd.h> //sleep(3);second
using namespace std;

TH1F *H2;
int NX = 72, NY = 72;

int extractId(string &nameId, string head, string tail)
{
    //nameId like beam_1.pd1, get lenghth of all parts
    int total = nameId.length();
    int tailLength = tail.length(); //for .mdat
    int headLength = head.length(); //for out
    if (nameId.substr(0, headLength) != head)
        return -1;

    int dataLength = total - tailLength - headLength;
    string num = nameId.substr(headLength, dataLength); //获得从headlength开始，长度为dataLength的字符串
    return atoi(num.c_str());
}

int find_track_hist()
{
    char pedefn[] = "../data/pede.txt";
    char beamfn[] = "../data/";
    char Output[] = "./output.txt"; //for debug
    // char pedefn[] = "/Volumes/Elements/THGEM+Topmetal_data/Ne10DME-80kPa-DV350GV630IV300-X-ray-generator/pede.txt";
    // char beamfn[] = "/Volumes/Elements/THGEM+Topmetal_data/Ne10DME-80kPa-DV350GV630IV300-X-ray-generator/";
    /////get beamId in to list
    vector<string> name_id;

    DIR *dir;
    struct dirent *ptr;
    dir = opendir(beamfn); //打开一个目录

    cout << pedefn << endl;
    cout << beamfn << "out*.mdat" << endl;

    while ((ptr = readdir(dir)) != NULL) //循环读取目录数据
    {
        name_id.push_back(ptr->d_name);
    }
    closedir(dir); //关闭目录指针

    //extract id number
    vector<int> idList;

    for (std::vector<int>::size_type j = 0; j < name_id.size(); j++)
    {
        //cout << name_id[j] << endl;
        int eId = extractId(name_id[j], "out", ".mdat");

        if (eId >= 0)
        {
            idList.push_back(eId);
        }
    }

    //sort number into list point to name_id
    //sort is not necessary if id is already  in order
    sort(idList.begin(), idList.end());
    for (std::vector<int>::size_type j = 0; j < idList.size(); j++)
    {
        cout << idList[j] << ' ';
    }
    cout << "!!!" << endl;
    cout << "The num of out*.mdat in idList[] is: " << idList.size() << " files"
         << " -> "
         << "[" << idList.front() << "..." << idList.back() << "]" << endl;

    int iStart_num = 4;
    int iAccout = 2;

    cout << "Begin to analysis IdList[]: begin " << iStart_num << " totle " << iAccout << endl;
    ifstream infilePede(pedefn);

    //*******************read Pede file************************
    int iChipT = 0, iPixelT = 0, iCounter = 0;
    float pedestalT = 0., noiseT = 0.;
    float meanPed[5184];

    while (!infilePede.eof() && iCounter < 5184)
    {
        infilePede >> iChipT >> iPixelT >> pedestalT >> noiseT;
        meanPed[iCounter] = pedestalT;
        iCounter++;
    }
    infilePede.close();
    //*******************read Pede file************************

    //*******************draw the hist***************************************
    TCanvas *c1 = new TCanvas("c1", "Canvas", 0, 0, 1800, 400);
    H2 = new TH1F("H2", "ADC", 809 * iAccout, 0, 809 * iAccout); //一个个的增加
    int sumsig;
    char inDataFile[200];
    int iBin = 0;

    for (int fileId = iStart_num; fileId < iStart_num + iAccout; fileId++)
    {
        //extract totle ADC of each frame
        vector<double> aFrames_one_mdat;

        // char inDataFile[] = "../data/out5.mdat";
        sprintf(inDataFile, "../data/out%d.mdat", idList[fileId]);
        // sprintf(inDataFile,"/Volumes/Elements/THGEM+Topmetal_data/Ne10DME-80kPa-DV350GV630IV300-X-ray-generator/out%d.mdat", idList[fileId]);

        cout << inDataFile << endl;

        ifstream infileSig(inDataFile, ios::binary);

        //*******************count the number of iFrames******
        unsigned short _data0_short[NX][NY]; //size of 1 frame for .mdat

        int fz = file_loder::file_size(inDataFile);
        // cout << "the size of file is: "<< fz << endl;
        int iFrames = 0;
        iFrames = fz / sizeof(_data0_short);
        cout << "iFrame num is: " << iFrames << endl; //758
        //*******************count the number of iFrames******

        //*******************for 3D array*******************************
        vector<vector<vector<float>>> array3D;
        int DEPTH = iFrames + 1;
        int HEIGHT = NY;
        int LONGTH = NX;
        // 初始化
        array3D.resize(DEPTH); //1
        for (int i = 0; i < DEPTH; ++i)
        {                              //1
            array3D[i].resize(HEIGHT); //2
            for (int j = 0; j < HEIGHT; ++j)
            {                                 //2
                array3D[i][j].resize(LONGTH); //3
            }
        }
        //*******************for 3D array*******************************
        ofstream of;
        of.open(Output);
        // H2 = new TH1F("H2","ADC",809 * iAccout, 0, 809 * iAccout);//只有一团
        for (int iFrameBegin = 0; iFrameBegin < iFrames + 1; iFrameBegin++) //&& i < iFrames+1
        {
            sumsig = 0;
            int _data_int[NX][NY];              //unsigned short for .mdat, int for .dat
            unsigned short _data_short[NX][NY]; //unsigned short for .mdat, int for .dat

            infileSig.read((char *)(&_data_short), sizeof(_data_short));

            for (int ii = 0; ii < NX; ii++)
            {
                for (int jj = 0; jj < NY; jj++)
                {
                    array3D[iFrameBegin][ii][jj] = _data_short[ii][jj] - meanPed[ii * 72 + jj];
                    sumsig = sumsig + array3D[iFrameBegin][ii][jj];
                }
            }
            H2->SetBinContent(iBin, sumsig);
            H2->SetLineWidth(1); //最少为1个pixel
            aFrames_one_mdat.push_back(sumsig);
            // cout<< iFrameBegin << " " << sumsig << " : " << aFrames_one_mdat[iFrameBegin] << endl;
            of<< iFrameBegin << " " << sumsig << " " << aFrames_one_mdat[iFrameBegin] << endl;
            iBin++;
        }
        // H2->Draw();
        // c1->Update();
        // sleep(1);//second
        // if (gSystem->ProcessEvents())//不能去除，否则没有动画
        //     break;
        infileSig.close();
        of.close();
    }

    H2->Draw(); //只画最后一次
    // c1->SaveAs(TString::Format("./55-1/55-1-%d.pdf", idList[iStart_num]));
    //*******************draw the hist***************************************
    return 0;
}
