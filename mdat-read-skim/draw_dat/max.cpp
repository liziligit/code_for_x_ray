#include <stdio.h>
#include <iostream>
#include<vector>

using namespace std;

int main(void)
{
// int a[1];
// vector<int> a;
vector<int> a(0,0);
// int i;
int index_max;//index_max用于接收最大值下标


// a[0] = 0;
// a[1] = 1;
// a[2] = 0;
// a[3] = 5;
// a[4] = 13;
// a[5] = 3;
// a[6] = 1;
// a[7] = 9;
// a[8] = 0;
// a[9] = 0;

// a[0] = 0;
// a[1] = 0;
// a[2] = 0;
// a[3] = 0;
// a[4] = 5;
// a[5] = 5;
// a[6] = 0;
// a[7] = 0;
// a[8] = 0;
// a[9] = 0;
// a.push_back(1);
cout << "a.size():"<< a.size() << endl;
// cout << "a[0]:"<< a[0] << endl;

for(int i=0;i<a.size();++i)
{
    // a[i] = 2*i;
    cout<< a[i] << endl;
}

index_max=0;//初始化最大值下标

for(int i=0;i<a.size();++i)
{
    if(a[i]>a[index_max])
    {
        index_max=i;//用下标存储最大值下标，不仅可以找到最大值，也可以找到它的位置
    }
}

// cout << "max=" << a[index_max] << ",index_max=" << index_max << endl;

return 0;
}
