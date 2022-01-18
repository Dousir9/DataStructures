//
//  main.cpp
//  DataStructures
//
//  Created by Dousir9 on 2022/1/18.
//

#include <iostream>

#include "skip_list.h"

using namespace std;

int main(int argc, const char * argv[]) {
    SkipList<int, int> sp(32);
    sp.set(6, 10);
    cout << sp.get(6) << '\n';
    sp.erase(6);
    
    return 0;
}
