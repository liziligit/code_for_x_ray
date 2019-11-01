//一个个像素判断，像素比本底高30的为有效像素
//边界有超过3个像素的，此帧不要
//有效像素ADC值相加
//算所有像素的重心
//最大像素的值，及坐标点
//找到最大X,Y形成矩形圈住图像
//判断矩形对角线长度

//此程序针对正离子
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include <unistd.h>
#include <stdio.h>
#include "iostream"
#include "Riostream.h"
#include "TGraph.h"
#include "TSpectrum.h"
#include "TMath.h"


using namespace RooFit;
using namespace std;

TCanvas *c1 = new TCanvas("c1","Canvas",800,800);

void GetFileListSubDir(string dirname, const char* suffix, const char* prefix, vector<string>& fileList);

void Topmetall2()
{
    //
    // readout the background file
    //
    //TGraph2D *gr2d = new TGraph2D();
    const int nFrame = 758;
    const int nXpixel = 72;
    const int nYpixel = 72;
    const int nLength = (nXpixel*nYpixel)*nFrame;
    const int nHeader = 1024;
    int headerdat[nHeader];
    int sumbgdat[nXpixel*nYpixel] = {0};
    int avgbgdat[nXpixel*nYpixel] ={0};
    int avgbg;
    int sigdata[nXpixel][nYpixel] = {0};		
    unsigned short  _data = 0;
    int mx;
    int my;
    double Ylength;
    double Xlength;
    //ofstream fout(TString::Format("Parameter.txt"));
    ofstream outfileFrame("./outframe362.dat", ios::binary);
    ifstream infileBg("./pede.pd1", ios::binary);
    if (!infileBg){
       cout<<"Can not open file"<<endl;
       return;
    }
    if(infileBg.good()){

        for(int j=0; j<(nXpixel*nYpixel); j++) {
            for(int i=0; i<nFrame; i++) {
                infileBg.seekg((-(nXpixel*nYpixel*(nFrame-i)-j))*sizeof(_data),ios::end);
                infileBg.read((char*)(&_data), sizeof(_data));
                sumbgdat[j] += _data;
            }
            avgbgdat[j] = sumbgdat[j]/nFrame;  
        }
    }
    //
    // readout the pd1 data: dat = pd1 - avg(bg)
    //
    
     TFile *newf = new TFile("test327.root","recreate");
     TTree *t_data = new TTree("t_data","data filtered result");
        Int_t nEvent;          t_data->Branch("nEvent",&nEvent,"nEvent/I");
        Int_t ndatapack;      t_data->Branch("ndatapack",&ndatapack,"ndatapack/I");
        Int_t Nframe;      t_data->Branch("Nframe",&Nframe,"Nframe/I");
        Int_t eventnum;      t_data->Branch("eventnum",&eventnum,"eventnum/I");
        Int_t peakADC;      t_data->Branch("peakADC",&peakADC,"peakADC/I");
        Int_t Xpeakpixel;        t_data->Branch("Xpeakpixel",&Xpeakpixel,"Xpeakpixel/I");
        Int_t Ypeakpixel;        t_data->Branch("Ypeakpixel",&Ypeakpixel,"Ypeakpixel/I");
        Int_t hitpixnum;      t_data->Branch("hitpixnum",&hitpixnum,"hitpixnum/I");
        Double_t XYlength;      t_data->Branch("XYlength",&XYlength,"XYlength/D");
        Double_t XYratio;      t_data->Branch("XYratio",&XYratio,"XYratio/D");
        Int_t sumADC;       t_data->Branch("sumADC",&sumADC,"sumADC/I");
        Double_t Xcorepixel;        t_data->Branch("Xcorepixel",&Xcorepixel,"Xcorepixel/D");
        Double_t Ycorepixel;        t_data->Branch("Ycorepixel",&Ycorepixel,"Ycorepixel/D");
        Double_t PCgap;      t_data->Branch("PCgap",&PCgap,"PCgap/D");  
  
    vector<string> rfileList;
    GetFileListSubDir("./",".pd1", "xbeam_",rfileList);
    nEvent=0;
    ndatapack=0;

    for(size_t ii=0;ii<rfileList.size();ii++)
    {
        cout<<rfileList[ii]<<endl;
        TString ifilename0 = rfileList[ii];
        ifstream infileSig(ifilename0, ios::binary);
        TString ofilename = rfileList[ii];
        ofilename = ofilename.Replace(ofilename.Length()-8,11,"dataout.root");
        cout<<"==>Converting ... " << ifilename0<<" to "<<ofilename<<endl;
        if (!infileSig){
            cout<<"Can not open file "<<ifilename0<<endl;
            return 0;
        }
        cout<<"==>Reading "<<endl;
        ndatapack++;
        eventnum=0;
        int priorsumADC=0;
        if(infileSig.good()){

            for(int i=0; i<nFrame; i++) {
            	hitpixnum=0;
            	sumADC=0;
            	peakADC=0;
            	double Xxcorepixel=0;
            	double Yycorepixel=0;
            	int edgenum=0;
            	int Xmax=0;
            	int Xmin=71;
            	int Ymax=0;
            	int Ymin=71;
              int X = 0, Y = 0;
                for(int j=0; j<(nXpixel*nYpixel); j++) {
                    X=j/72;
                    Y=j%72;
                    infileSig.seekg((-(nXpixel*nYpixel*(nFrame-i)-j))*sizeof(_data),ios::end);
                    infileSig.read((char*)(&_data), sizeof(_data));
                    sigdata[X][Y] = -1*(_data - avgbgdat[j]);
                    
                    if (sigdata[X][Y] > peakADC ) {
	  	                peakADC = sigdata[X][Y];
	  	                 Xpeakpixel=X;
                       Ypeakpixel=Y;
	  	              }
	  	              
	  	              if (sigdata[X][Y] > 20){
	  	              	hitpixnum++;
	  	              	if (X==0 || X==71 || Y==0 ||  Y==71){
	  	              		edgenum++;
	  	              	}
	  	                sumADC = 	sumADC + sigdata[X][Y];
	  	                Xxcorepixel = Xxcorepixel + X*sigdata[X][Y];
	  	                Yycorepixel = Yycorepixel + Y*sigdata[X][Y];
	  	                if (X > Xmax){
	  	                	Xmax = X;
	  	                }
	  	                if (X < Xmin){
	  	                	Xmin = X;
	  	                }
	  	                if (Y > Ymax){
	  	                	Ymax = Y;
	  	                }
	  	                if (Y < Ymin){
	  	                	Ymin = Y;
	  	                }
	  	              }
                }
                               
                Xlength =  Xmax-Xmin;
                Ylength =  Ymax-Ymin; 
                XYlength = TMath::Sqrt((Xlength*Xlength) + (Ylength*Ylength));
                if ( sumADC >35 && priorsumADC < sumADC &&hitpixnum > 8  && edgenum < 3 ){//ADC和，前一帧，像素个数，边界个数
                  if (Xlength > Ylength){
            	      XYratio=Xlength/Ylength;
                  }
                  else {
            	      XYratio=Ylength/Xlength;
                  }
                
                  Xcorepixel = Xxcorepixel/sumADC;
                  Ycorepixel = Yycorepixel/sumADC;
                  PCgap = TMath::Sqrt((Xpeakpixel-Xcorepixel)*(Xpeakpixel-Xcorepixel) + (Ypeakpixel-Ycorepixel)*(Ypeakpixel-Ycorepixel));
                  Nframe = i;	
                  nEvent++; 	
                  eventnum++;
                  
                  //TH2F *H2 = new TH2F("H2"," ",72,0,71,72,0,71);
                  //H2->SetStats(0);
                  for(int m=0;m<(72*72);m++){
                	  mx=m/72;
                	  my=m%72;
                    outfileFrame.write((char*)&sigdata[mx][my],sizeof(sigdata[mx][my]));
                   // H2->Fill(mx,my,sigdata[mx][my]);
                  }
                 // H2->Draw("Colz");
                 // c1->Update();
                 // char buf[100];
                 // sprintf(&buf[0],"%d_frame%d.jpg",ndatapack,i);
                 // c1->SaveAs(buf);
                  
                  t_data->Fill();
                  
                 // fout<<ndatapack<<"-frame"<<i<<"---  nEvent:"<<nEvent<<" peakADC:"<<peakADC<<" hitpixnum:"<<hitpixnum<<" PCgap:"<<PCgap<<" XYlength:"<<XYlength<<" XYratio:"<<XYratio<<" sumADC:"<<sumADC<<endl;
                  i = i + 3;
                }
                priorsumADC = sumADC;
              }
             
        }
        
        infileSig.close();
    }
    newf->cd();	
              t_data->Write();
 //return 0;
 }
///////////////////////////get frame data
/*/pd1数据头的内容
 int dataSize(){return ((int   *)header+1*4)[0];};
 int adcBit(){return ((int   *)header+2*4)[0];};
 int adcCha(){return ((int   *)header+3*4)[0];};
 int samPerPix(){return ((int   *)header+4*4)[0];};
 float dt(){return ((float   *)header+5*4)[0];};
 int nPix(){return ((int   *)header+6*4)[0];};
 int nFrame(){return ((int   *)header+7*4)[0];};
 int year(){return ((int   *)header+8*4)[0];};
 int month(){return ((int   *)header+9*4)[0];};
 int day(){return ((int   *)header+10*4)[0];};
 int hour(){return ((int   *)header+11*4)[0];};
 int min(){return ((int   *)header+12*4)[0];};
 int sec(){return ((int   *)header+13*4)[0];};
 
 float *chaDat;//存储一个通道数据的缓冲区
 float *pixDat; //存储一个像素数据的缓冲区
 float *frameDat; //存储一个像素数据的缓冲区
 
 int nPixDat;   //一个像素上的采样数
 int nChaDat;   //一个通道内的采样数
 int nFrameDat;   //sample in frameDat
 
int getFrame(int i,int cha){//i 0~nFrame()-1 cha 0~8
    if(i>=nFrame()) return -1;
    if(i<0) return -1;
    if(cha>=adcCha()) return -1;
    if(cha<0) return -1;
    
    int frameLen=dataSize()/adcCha()/nFrame()/(sizeof(unsigned short));
    //cout<<"data size is "<<dataSize()<<" adc channel "<<adcCha()<<" chaLen "<<chaLen<<endl;
    // int chaLen1=nFrame()*nPix()*samPerPix();
    // cout<<"--------------------"<<endl;
    // cout<<"frameLen="<<frameLen<<endl;
    // cout<<chaLen1<<endl;
    
    if(nFrameDat==0){
        frameDat=new float[frameLen];
        nFrameDat=frameLen;
    }else if(nFrameDat<frameLen){
        delete[]frameDat;
        frameDat=new float[frameLen];
        nFrameDat=frameLen;
    }
    unsigned short *ps = (unsigned short *)p;
    for(int j=0;j<frameLen;j++)
    {
        frameDat[j]=(float)(ps[nPix()*adcCha()*i+adcCha()*j+cha]);
    }
    
    
    return 0;
}
/*/



void GetFileListSubDir(string dirname, const char* suffix, const char* prefix, vector<string>& fileList)
 {
    //if (!dirname || !(*dirname)) return; // just a precaution
    TString pwd(gSystem->pwd());
    TSystemDirectory dir(dirname.data(), dirname.data());
    TList *files = dir.GetListOfFiles();
    gSystem->cd(pwd.Data()); // bug fix for ROOT prior to 5.34
    if (files) {
        TSystemFile *file;
        TString fname;
        string fpath;
        TIter next(files);
        while ( (file = (TSystemFile*)next()) ) {
            fname = file->GetName();
            fpath = dirname + "/"+ fname.Data();    // linux, mac files
            //fpath = dirname + "\\"+ fname.Data();  //win files
            if(file->IsDirectory())
            {
                if(strcmp(fname.Data(),".") != 0  &&  strcmp(fname.Data(),"..") != 0){
                    //    std::cout <<fpath << std::endl;
                    GetFileListSubDir( fpath, suffix, prefix, fileList );
                }
            }
            if ( (!prefix || !(*prefix) || fname.BeginsWith(prefix)) &&
                (!suffix || !(*suffix) || fname.EndsWith(suffix)) ) {
                //fname = fpath + "/" + fname.Data();
                fileList.push_back(fpath.data());
                std::cout <<fpath << std::endl;
            }
            
        }
    }
    delete files;
}
