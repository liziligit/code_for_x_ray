#include<iostream>
#include<vector>
using namespace std;

int main(){

// vector<vector<int> > vc;
// vc.clear();
// vector<int> vx;
// vx.clear();

vector<vector <int> > vc;
vc.resize(2,vector<int>(4, 0));//初始化0
vc.clear();

vector<vector <int> > vc2;
vc2.resize(2,vector<int>(4, 0));//初始化0
vc2.clear();

vector<int> vx(4, 1);
vx.clear();

vx.push_back(1);
vx.push_back(3);
vx.push_back(5);

vc.push_back(vx);
vc2.push_back(vx);
vx.clear();

vx.push_back(2);
vx.push_back(4);
vx.push_back(6);

vc.push_back(vx);
vc2.push_back(vx);

vx.clear();
for (int i = 0; i < vc.size(); i++)
    {
        for (int j = 0; j < vc[i].size(); j++)
        {
            cout << vc[i][j] <<",";
        }
        cout<<endl;
    }

vc.clear();

vx.push_back(11);
vx.push_back(33);
vx.push_back(55);

vc.push_back(vx);
vx.clear();

vx.push_back(22);
vx.push_back(44);
vx.push_back(66);

vc.push_back(vx);

for (int i = 0; i < vc.size(); i++)
    {
        for (int j = 0; j < vc[i].size(); j++)
        {
            cout << vc[i][j] <<",";
        }
        cout<<endl;
    }

// vc.clear();
// vc[0].clear();
// vc[1].clear();

vc[0][0] = 111;//clear()后不能这样赋值
vc[0][1] = 111;
vc[0][2] = 111;

vc[1][0] = 222;
vc[1][1] = 222;
vc[1][2] = 222;

vc[0][0] = 1111;
vc[0][1] = 1111;
vc[0][2] = 1111;

vc[1][0] = 2222;
vc[1][1] = 2222;
vc[1][2] = 2222;


for (int i = 0; i < vc.size(); i++)
    {
        for (int j = 0; j < vc[i].size(); j++)
        {
            cout << vc[i][j] <<",";
        }
        cout<<endl;
    }

// vc.clear();
vc[0][0] = vc2[0][0];//clear()
vc[0][1] = vc2[0][1];
vc[0][2] = vc2[0][2];

vc[1][0] = vc2[1][0];
vc[1][1] = vc2[1][1];
vc[1][2] = vc2[1][2];

for (int i = 0; i < vc.size(); i++)
    {
        for (int j = 0; j < vc[i].size(); j++)
        {
            cout << vc[i][j] <<",";
        }
        cout<<endl;
    }

cout << "vc.size: " << vc.size() << endl;
cout << "vc[0].size: " << vc[0].size() << endl;
cout << "vc[1].size: " << vc[1].size() << endl;

return 0;
}