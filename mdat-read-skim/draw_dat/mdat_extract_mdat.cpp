//从mdat中提取指定帧到新mdat
#include <stdio.h>
// #include "TGraph.h"
#include <string.h> // for strrchr()
#include <vector>
#include <cstdlib> // exit()å
#include "loader_file.h"
using namespace std;

int main(void)
{
    const int NX = 72;
    const int NY = 72;
    char inDataFile[] = "../data/out5.mdat";
    char output_txt[] = "../data/out55-127.mdat"; //for debug
    ifstream infileSig(inDataFile, ios::binary);

    ofstream output(output_txt, ios::out | ios::binary);

    // for (int i = 0; i < iFrames; i++)
    for (int i = 0; i < 128; i++)
    {
        unsigned short _data_short[NX][NY];
        infileSig.read((char *)(&_data_short), sizeof(_data_short));
        if(i == 127)
        {
            output.write((char *)(&_data_short), sizeof(_data_short));
        }
        
    }

    infileSig.close();
    output.close();

    return 0;
}
