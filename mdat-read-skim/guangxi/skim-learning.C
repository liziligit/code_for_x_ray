#include "MyEventClass.h"

const char *filetypes[] = {
    "mdata file", "*.mdat",
    0, 0};

TString SelectFile()
{
    static TString dir(".");
    TGFileInfo fi;
    fi.fFileTypes = filetypes;
    fi.fIniDir = StrDup(dir);
    printf("fIniDir = %s\n", fi.fIniDir);
    new TGFileDialog(gClient->GetRoot(), NULL, kFDOpen, &fi);
    if (fi.fFilename == NULL)
    {
        printf("No file selected!\n");
        return TString("NONE");
    }
    printf("Open directory: %s\n", fi.fIniDir);
    return fi.fIniDir;
}

//______________________________________________________________________________
// read data into MyEventClass
//
const int NX = 72;
const int NY = 72;
const int nsig = 3;
TH2F *hPed;
vector<TH1F *> fPed;//fPed这个容器里装的是指向TH1F型的指针
vector<MyEventClass *> fEventList;

void ReadData(TString pp)
{
    int tid;
    for(int i=0; i<fEventList.size(); i++) delete fEventList[i];
    fEventList.clear();

    ifstream ifSignal(pp, ios::binary);
    while (ifSignal.good())
    {
        unsigned short _data[NX][NY];//0~65535,unsigned short是16位,占2个字节
        ifSignal.read((char *)(&_data), sizeof(_data));//&_data是unsigned short型变量的地址，
        //将unsigned short型转换成char型指针,sizeofu(_data)为5184*2=10368字节

        MyEventClass *fEvent = new MyEventClass(tid, 0, NX - 1, 0, NY - 1);
        for (int ii = 0; ii < NX; ii++)
            for (int jj = 0; jj < NY; jj++)
                fEvent->Fill(ii, jj, _data[ii][jj]);

        fEvent->GenerateHist(hPed, kFALSE);
        fEventList.push_back(fEvent);//cout<<fEventList[i]<<endl;
        tid++;
    }
    ifSignal.close();
}

void SaveData(ofstream *ofSignal)
{
    unsigned short data[NX][NY];

    for(int i=0; i<NX; i++)
        for(int j=0; j<NY; j++)
            data[i][j] = 0xFFFF;
    
    ofSignal->write((char *)(&data), sizeof(data));

    for(int i=0; i<NX; i++)
        for(int j=0; j<NY; j++)
            data[i][j] = hPed->GetBinContent(i+1, j+1);
    
    ofSignal->write((char *)(&data), sizeof(data));

    for(int i=0; i<fEventList.size(); i++)
    {
        if(fEventList[i]->GetDataQuality() == QF_Ped) continue;//满足条件时不执行循环体剩余部分,const int QF_Ped = 0;
        if(fEventList[i]->GetDataQuality() == QF_AfterPulse) continue;//const QF_AfterPulse = 3;

        cout<<" Event: "<<i<<" is stored."<<endl;
        for(int ii=0; ii<NX; ii++)
            for(int jj=0; jj<NY; jj++)
                data[ii][jj] = fEventList[i]->GetData(ii, jj);
        
        ofSignal->write((char *)(&data), sizeof(data));
    }
}

//______________________________________________________________________________
//
void AnalysisPed(TString pp)
{
    if (hPed != NULL)
    {
        delete hPed;
        for(int i=0; i<fPed.size(); i++) delete fPed[i];
        fPed.clear();
    }

    hPed = new TH2F("hPed", "Pedestal", NX, 1, NX + 1, NY, 1, NY + 1);

    //----------
    for (int i = 0; i < NX; i++)
    {
        for (int j = 0; j < NY; j++)
        {
            int xmin = -1;
            int xmax = -1;
            for (int k = 0; k < fEventList.size(); k++)
            {
                if(fEventList[k]->GetDataQuality() != QF_Ped) continue;
                double dtmp = fEventList[k]->GetData(i, j);
                xmin = (xmin == -1) ? dtmp : xmin;
                xmax = (xmax == -1) ? dtmp : xmax;
                xmin = (xmin < dtmp) ? xmin : dtmp;
                xmax = (xmax > dtmp) ? xmax : dtmp;
            }

            TH1F *f = (TH1F *)gDirectory->Get(Form("h%d", i * NY + j));
            if(f!=NULL) delete f;
            
            f = new TH1F(Form("h%d", i * NY + j), Form("histogram for %d,%d", i, j), xmax - xmin + 10, xmin - 5, xmax + 5);
            for (int k = 0; k < fEventList.size(); k++)
                f->Fill(fEventList[k]->GetData(i, j));

            fPed.push_back(f);
        }
    }

    for (int i = 0; i < NX; i++)
        for (int j = 0; j < NY; j++)
            hPed->SetBinContent(i + 1, j + 1, fPed[i * NY + j]->GetMean() + nsig * 1.1);//const int nsig = 3;

    cout << "----> ped analysis for " << pp << " is finished" << endl;
}

//______________________________________________________________________________
// skim a data file
void Skim(TString pp, ofstream *ofSignal)
{
    cout << "----> reading " << pp << endl;

    // 1. read data
    ReadData(pp);

    // 2. analysis ped
    AnalysisPed(pp);

    // 3. analysis data with new ped
    for(int i=0; i<fEventList.size(); i++)
        fEventList[i]->GenerateHist(hPed, kFALSE);

    // 4. analysis cluster
    int istart = -1;
    int istop = -1;
    vector<vector<int>> clusterList(2);//二维向量
    for (int ii = 0; ii < fEventList.size(); ii++)
    {
        if (fEventList[ii]->GetClusterSize() < 20)
        {
            if (istart == -1)
                continue;
            istart = -1;
            istop = ii - 1;
            clusterList[1].push_back(ii - 1);
            continue;
        }

        if (istart == -1)
        {
            istart = ii;
            clusterList[0].push_back(ii);
            continue;
        }
    }

    if (istart != -1)
        clusterList[1].push_back(fEventList.size() - 1);

    // 5. set data flag
    cout << "cluster: " << clusterList[0].size() << endl;
    for (int ii = 0; ii < clusterList[0].size(); ii++)
    {
        cout << ii << ": " << clusterList[0].at(ii) << " ~ " << clusterList[1].at(ii) << endl;

        int dataFlag = QF_Hit;//const int QF_Hit = 1;
        // muon like:
        int ibeg = clusterList[0].at(ii);
        if (fEventList[ibeg]->GetPulseHeight() < 600)
            dataFlag = QF_MuLike;//const int QF_MuLike = 2;

        fEventList[ibeg]->SetDataQuality(dataFlag);
        cout << "\t" << fEventList[ibeg]->GetDataQuality() << ", " << fEventList[ibeg]->GetClusterSize() << ", " << fEventList[ibeg]->GetPulseHeight() << endl;

        for (int jj = clusterList[0].at(ii) + 1; jj <= clusterList[1].at(ii); jj++)
        {
            fEventList[jj]->SetDataQuality(QF_AfterPulse);//const int QF_AfterPulse = 3;
            cout << "\t" << fEventList[jj]->GetDataQuality() << ", " << fEventList[jj]->GetClusterSize() << ", " << fEventList[jj]->GetPulseHeight() << endl;
        }
    }

    // 5. analysis ped again with the data flag
    AnalysisPed(pp);

    //6. store data
    SaveData(ofSignal);
}

//______________________________________________________________________________
//
void skim()
{
    hPed = NULL;

    // 2. read directory
    TString path = SelectFile();
    FILE *fp = gSystem->OpenPipe("ls " + path + "/*.mdat", "r");
    if (!fp)
    {
        cout << "----> NO mdat data exists in " << path << "!" << endl;
        return;
    }

    vector<TString> mdatList;
    char line[1000];
    while (fgets(line, sizeof(line), fp))
    {
        TString s(line);
        if (s.Index(".mdat") == -1)
            continue;
        mdatList.push_back(s.ReplaceAll("\n", ""));
    }
    cout << "----> " << mdatList.size() << " mdat files exist." << endl;
    
    // 3. output file
    TString outFile = path+".data";
    ofstream *OfSignal = new ofstream(outFile.Data());

    // 4. skim of all data files
    for (int i = 0; i < (int)mdatList.size(); i++)
    {
        Skim(mdatList[i], OfSignal);
    }
}
