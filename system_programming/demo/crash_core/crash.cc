#include <stdlib.h>

void FuncB() {
    int* ptr = nullptr;
    *ptr = 0;
}

void FuncA() {
    FuncB();
}

int main() {
    FuncA();
    return 0;
}