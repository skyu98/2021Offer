#include <iostream>
#include <string>

using namespace std;

string intToHexStr(int num) {
    int div = num / (0xf + 1);
    int mod = num % (0xf + 1);

    string HexStr = "";
    if(div < 10) {
        HexStr.append(to_string(div));
    }
    else {
        HexStr.push_back('a' + div - 10);
    }

    if(mod < 10) {
        HexStr.append(to_string(mod));
    }
    else {
        HexStr.push_back('a' + mod - 10);
    }

    return HexStr;
}

int main() {
    string dd = "";
    getline(cin, dd);
    
    const char* cur = dd.c_str();

    string hex = "0x";
    while(*cur) {
        int curNum = 0;
        while(*cur && *cur != '.') {
            curNum = curNum * 10 + (*cur - '0');
            ++cur;
        }
        hex.append(intToHexStr(curNum));

        if(*cur) {
            ++cur;
        }
        else {
            break;
        }
    }
    
    cout << hex << endl;
    return 0;
}