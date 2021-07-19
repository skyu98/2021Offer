#include <mutex>
#include <thread>
#include <iostream>

using namespace std;
mutex m1;
mutex m2;

void threadFuncA() {
    {
        std::lock_guard<mutex> lockA(m1);
        cout << "threadA gets m1!" << endl;
        this_thread::sleep_for(chrono::seconds(2));

        lock_guard<mutex> lockB(m2);
        cout << "threadA gets m2!" << endl;
    }
    
    cout << "threadA releases m1 & m2!" << endl;
}

void threadFuncB() {
    {
        this_thread::sleep_for(chrono::seconds(1));
        lock_guard<mutex> lockB(m2);
        cout << "threadB gets m2!" << endl;

        lock_guard<mutex> lockA(m1);
        cout << "threadB gets m1!" << endl;
    }
    
    cout << "threadB releases m1 & m2!" << endl;
}

int main() {
    thread A(threadFuncA);
    thread B(threadFuncB);

    A.join();
    B.join();
    return 0;
}