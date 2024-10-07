
#include <iostream>
#include "DLL.h"
using namespace std;
int main()
{
    CList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    CList<int>::iterator iter = list.begin();

    iter++;
    iter = list.insert(iter, 100);
    iter++;
    iter = list.erase(iter);

    for (iter = list.begin(); iter != list.end(); ++iter) {
        cout << *iter << endl;
    }

    return 0;
}
