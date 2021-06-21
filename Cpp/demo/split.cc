#include <iostream>
#include <sstream> // for isstream
#include <string>
#include <vector>

using namespace std;

void splitStrToVec(const string& str, const string& split, vector<string>& vec) {
    size_t beg = 0, end = str.find(split);
    size_t len = split.size();
    while (end != string::npos) {
        vec.emplace_back(str.substr(beg, end - beg));
        beg = end + len;
        end = str.find(split, beg);
    }  
    if(beg != str.size()) {
        vec.emplace_back(str.substr(beg));
    }
}

void splitStrToVec(const string& str, char split, vector<string>& vec) {
    stringstream ss(str);
    string tmp;
    while(getline(ss, tmp, split)) {
        vec.emplace_back(std::move(tmp));
    }
}

int main() {
    string line;
    vector<string> vec;
    
    getline(cin, line);
    // splitStrToVec(line, ",", vec);
    splitStrToVec(line, ',', vec);

    for(auto& v : vec) {
        cout << v << endl;
    }
}
