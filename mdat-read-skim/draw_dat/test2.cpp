#include <iostream>
#include <vector>

using namespace std;

int main()
{
    // vector<double> aFrames_one_mdat;
    // int n = 0;
    // cout << "hello" << endl;
    // for (int m = 0; m < 2; m++)
    // {
    //     // vector<double> aFrames_one_mdat;
    //     aFrames_one_mdat.push_back(1);
    //     cout << aFrames_one_mdat[m + n] << " ";
    //     aFrames_one_mdat.push_back(1 * m);
    //     cout << aFrames_one_mdat[m + n + 1] << " ";
    //     // cout << endl;
    //     n++;
    // }
    // cout << aFrames_one_mdat.size() << endl;


    // vector<double> aFrames_one_mdat;
    int n = 0;
    cout << "hello" << endl;
    for (int m = 0; m < 2; m++)
    {
        vector<double> aFrames_one_mdat;
        aFrames_one_mdat.push_back(1);
        cout << aFrames_one_mdat[0] << " ";
        aFrames_one_mdat.push_back(1 * m);
        cout << \
        //
        aFrames_one_mdat[1] << " ";
        // cout << endl;
        n++;
        cout << aFrames_one_mdat.size() << endl;
    }
    // cout << aFrames_one_mdat.size() << endl;
    return 0;
}
