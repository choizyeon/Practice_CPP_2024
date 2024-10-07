#include <iostream>
#include "CBST.h"
using namespace std;
int main()
{
    CBST<int, int> bstint;
    tPair<int, int> pair;
    bstint.insert(make_bstPair(100, 0));           //            100
    bstint.insert(make_bstPair(150, 0));           //       50          150
    bstint.insert(make_bstPair(50, 0));            //   25      75  125     175
    bstint.insert(make_bstPair(25, 0));
    bstint.insert(make_bstPair(75, 0));
    bstint.insert(make_bstPair(125, 0));
    bstint.insert(make_bstPair(175, 0));

    CBST<int, int>::iterator iter = bstint.begin();
    iter = bstint.find(150);
    iter = bstint.erase(iter);

    /*
    (*iter).first;
    iter->first;
    
    (*iter).second;
    iter->second; // iter->pair->second; (->pair 생략해서 operator)
    */
    for (iter = bstint.begin(); iter != bstint.end(); ++iter) {
        wcout << iter->first << "  " << iter->second << endl;
    }
    
    return 0;
}