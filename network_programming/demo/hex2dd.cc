#include <iostream>
#include <string>

using namespace std;

int hexCharToInt(const char* ch) {
    if(*ch >= '0' && *ch <= '9') {
        return *ch - '0';
    }
    else if(*ch >= 'a' && *ch <= 'f') {
        return *ch - 'a' + 10;
    }
    else 
        return -1;
}

int main() {
    string hex = "";
    getline(cin, hex);
    
    const char* cur = hex.c_str();
    if(*cur == '0' && *(cur + 1) == 'x') {
        cur += 2;
    }

    string dd = "";
    while(*cur) {
        int curNum = 0;
        curNum += hexCharToInt(cur++) * (0xf + 1);
        if(!(*cur)) return -1;
        curNum += hexCharToInt(cur++);

        dd.append(to_string(curNum));
        dd.append(".");
    }

    dd.back() = '\0';
    
    cout << dd << endl;
    return 0;
}