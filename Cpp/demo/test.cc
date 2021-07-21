#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    cout << argc << endl;
    for(int i = 0;i < argc;++i) {
        char* str = argv[i];
        cout << str << endl;
    }
    return 0;
}