#include <iostream>
#include <vector>
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
    a.getP(x, y)[0] = 0;//当前像素值设为0

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

int main(int argc, char **argv)
{

    binArray a;
    a.nRow = 10;
    a.nCol = 10;
    a.d = new int[a.nRow * a.nCol];
    for (int i = 0; i < a.nRow; i++)
    {
        for (int j = 0; j < a.nCol; j++)
        {
            a.getP(i, j)[0] = 0;
        }
    }

    a.getP(3, 3)[0] = 1;
    a.getP(3, 4)[0] = 1;
    a.getP(3, 5)[0] = 1;
    a.getP(4, 4)[0] = 1;
    a.getP(4, 3)[0] = 1;
    a.getP(4, 5)[0] = 1;

    a.getP(7, 8)[0] = 1;
    a.getP(7, 9)[0] = 1;
    a.getP(8, 8)[0] = 1;
    a.getP(8, 9)[0] = 1;


    vector<int> vx;
    vector<vector<int> > vc;//多维向量
    vc.clear();

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

    cout << "n cluster " << vc.size() << endl;
    for (int i = 0; i < vc.size(); i++)
    {
        cout << "n address " << vc[i].size() << endl;
        for (int j = 0; j < vc[i].size(); j++)
        {
            cout << "address " << vc[i][j] << endl;
        }
    }
}