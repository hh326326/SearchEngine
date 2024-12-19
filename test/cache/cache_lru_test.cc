
#include "cache/lru_cache.h"
#include <string>
#include <iostream>

using namespace std;
using namespace hh;

int main()
{
    LRUCache lc(2);

    //lc.readFromFile("cache.dat");
    string s1("yes");
    json j1 = nlohmann::json("yes1 yes2 yes3"); 

    string s2("here");
    json j2 = nlohmann::json("here1 here2 here3"); 

    string s3("you");
    json j3 = nlohmann::json("you1 you2"); 

    lc.Put(s1, j1);
    lc.Put(s2, j2);
    lc.Put(s3, j3);
    cout << "before search: " << endl;
    cout << "lc._resultList: " << endl;
    lc.show();
    cout << endl;

    string s("here");
    json j;
    if(lc.Get(s, j))
    {
        cout << s << " found!" << endl; 
        cout << s << " " << j << endl;
    }

    cout << "lc._resultList: " << endl;
    lc.show();
    cout << endl;

    cout << "lc._pendingUpdateList: " << endl;
    for(auto it : lc.GetPendingUpdateList())
    {
        cout << it << endl;
    }

    lc.Update(lc);
    cout << "after update: " << endl;
    lc.show();
    cout << endl;
    
    lc.WriteToFile("word_cache.dat");

}
