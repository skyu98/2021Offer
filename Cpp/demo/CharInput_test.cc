#include <iostream>
#include <string.h>
#include <set>
using namespace std;
char* removeInS2(char* s1, const char* s2)
{
    if(!s2) return s1;
    set<char> inS2;
    int len = strlen(s2);
    const char* cur = s2;
    for(int i = 0;i < len;++i)
    {
        inS2.insert(*cur);
        ++cur;
    }

    len = strlen(s1);
    char* s1_cur = s1;
    while(*s1_cur)
    {
        if(inS2.find(*s1_cur) != inS2.end())
        {
            auto tmp = s1_cur;
            while(*tmp)
            {
                *tmp = *(tmp + 1);
                ++tmp;
            }
            --s1_cur;
        }
        ++s1_cur;
    }
    return s1;
}

int main()
{
    char* s1 = new char[100];
    char* s2 = new char[100];
    cin >> s1 >> s2;
    cout << "s1 is: " << s1 << endl;
    cout << "s2 is: " << s2 << endl;

    removeInS2(s1, s2);

    cout << "s1 is: " << s1 << endl;
    
    delete[] s1;
    delete[] s2;
    return 0;
}
