#include<stdio.h>//sprintf()
#include <vector>
#include <iostream>
#include<fstream>//ifsream读,ofstream写,fstream读写
#include <sys/types.h>//opendir()
#include <dirent.h>//opendir()
#include "loader_file.h"//file_size()
#include "TGraph.h"
using namespace std;

TH1F *H2;

int extractId(string & nameId, string head,string tail)
{
//nameId like beam_1.pd1, get lenghth of all parts
int total=nameId.length();
int tailLength=tail.length();//for .mdat
int headLength=head.length();//for out
if(nameId.substr(0,headLength)!=head) return -1;

int dataLength=total-tailLength-headLength;
string num=nameId.substr(headLength,dataLength);//获得从headlength开始，长度为dataLength的字符串
return atoi(num.c_str());
}

int find_track()
{
    char pedefn[] = "../data/pede.txt";
    char beamfn[] = "../data/";
    /////get beamId in to list
    vector<string> name_id;

    DIR * dir;
    struct dirent * ptr;
    dir = opendir(beamfn); //打开一个目录

    cout << pedefn << endl;
    cout << beamfn << "out*.mdat"<<endl;

    while((ptr = readdir(dir)) != NULL) //循环读取目录数据
        {
            name_id.push_back(ptr->d_name);
        }
    closedir(dir);//关闭目录指针

    //extract id number
    vector<int> idList;

    for(std::vector<int>::size_type j=0;j<name_id.size();j++)
    {
        //cout << name_id[j] << endl;
        int eId=extractId(name_id[j],"out",".mdat");

        if(eId>=0)
        {
        idList.push_back(eId);
        }
    }

    //sort number into list point to name_id 
    //sort is not necessary if id is already  in order
    sort(idList.begin(), idList.end());
    for(std::vector<int>::size_type j=0;j<idList.size();j++)
    {
        cout << idList[j] << ' ';
    }
    cout << "!!!" <<endl; 

    cout << "The num of out*.mdat in idList[] is: " << idList.size() << " files" << " -> " << "[" << idList.front() << "..." << idList.back() << "]" << endl;

    int iStart_num = 0;
    int iAccout = 3;

    cout << "Begin to analysis IdList[]: " << iStart_num << " to " << iAccout << endl;

    char inDataFile[] = "../data/out5.mdat";
    ifstream infilePede(pedefn);
    ifstream infileSig(inDataFile, ios::binary);


    //*******************count the number of iFrames******
    int NX = 72, NY = 72;
    unsigned short _data0_short[NX][NY];//size of 1 frame for .mdat

    int fz=file_loder::file_size(inDataFile);
    cout << "the size of file is: "<< fz << endl;
    int iFrames = 0;
    iFrames = fz / sizeof(_data0_short);
    cout<<"iFrame num is: "<<iFrames<<endl;//758
    //*******************count the number of iFrames******

    //*******************read Pede file************************
    int iChipT = 0, iPixelT = 0, iCounter=0;
    float pedestalT = 0., noiseT = 0.;
    float meanPed[5184];
   
    while(!infilePede.eof() && iCounter<5184) {
        infilePede >> iChipT >> iPixelT >> pedestalT >> noiseT;
        meanPed[iCounter] = pedestalT;       
        iCounter++;
        }
    infilePede.close();  
    //*******************read Pede file************************

    //*******************for 3D array*******************************
    vector<vector<vector<float> > > array3D;
    int DEPTH = iFrames+1;
    int HEIGHT = NY;
    int LONGTH = NX;
	// 初始化
	array3D.resize(DEPTH);//1
	for (int i = 0; i < DEPTH; ++i) {//1
		array3D[i].resize(HEIGHT);//2
 
		for (int j = 0; j < HEIGHT; ++j)//2
			array3D[i][j].resize(LONGTH);//3
	}
    //*******************for 3D array*******************************


//*******************draw the hist***************************************
auto c1 = new TCanvas("c1","2-d options",10,10,800,600);
H2 = new TH1F("H2","ADC",iFrames,0,iFrames);
int sumsig;

for(int i = 0; i < iFrames+1; i++)//&& i < iFrames+1
        {
        sumsig = 0;
        int _data_int[NX][NY];//unsigned short for .mdat, int for .dat-------->2/3
        unsigned short _data_short[NX][NY];//unsigned short for .mdat, int for .dat-------->2/3

        infileSig.read((char *)(&_data_short), sizeof(_data_short));

        for (int ii = 0; ii < NX; ii++){
            for (int jj = 0; jj < NY; jj++){               
                array3D[i][ii][jj] = _data_short[ii][jj] - meanPed[ii*72+jj];
                sumsig = sumsig + array3D[i][ii][jj];               
                }
            }

        // H2->Fill(sumsig,i);
        H2->SetBinContent(i,sumsig);
        // cout<<sumsig<<endl;     //0~808
        // sprintf(str, "frame %d",ndata);
        // H2->SetTitle(str);
        // gStyle->SetPalette(1);
        H2->Draw();
        // ndata++;
            // char buf[100];
            //  sprintf(&buf[0],"./mdat_frame%d_matrix_to_txt.jpg",ndata);
            //  c1->SaveAs(buf);
// delete H2;//手动清空堆Heap中内存
// H2 = NULL;//使堆Heap的指针指向空
}
infileSig.close();
//*******************draw the hist***************************************
    return 0;
}

