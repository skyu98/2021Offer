#include <iostream>

using namespace std;

int main() {
    /*  char    ASCII(0 ~ 127)
    *   0 ~ 9 :  48 ~ 57
    *   A ~ Z :  65 ~ 90 
    *   a ~ z :  98 ~ 122
    */
    // char numChar = '7';
    // char upperChar = 'H';
    // char lowerChar = 'g';
    // char outOfASCII = 129; // 无法显示
    // char* str = "gg";
    // printf("Chars are : %c, %c, %c, %c\n", numChar, upperChar, lowerChar, outOfASCII);
    // int numInt = static_cast<int>(numChar);
    // int upperInt = static_cast<int>(upperChar);
    // int lowerInt = static_cast<int>(lowerChar);
    // int outInt = static_cast<int>(outOfASCII);  // 结果为-127
    // int strInt = static_cast<int>(*str);
    // printf("Ints are : %d, %d, %d, %d, %d\n", numInt, upperInt, lowerInt, outInt, strInt);
    union obj {
        union obj* next;
        char data[1];
    };
    obj* myBlock = (obj*)malloc(128);
    char* ptr = myBlock->data;
    printf("%p\n", (void*)myBlock);
    printf("%p, %p\n", ptr, myBlock->data);
}