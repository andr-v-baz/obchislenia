#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable cv;
int i = 0;

void Thread1() {
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [] { return i == 1; });
    cout << "Message from Thread 1" << endl;
}

void Thread2() {
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [] { return i == 1; });
    cout << "Message from Thread 2" << endl;
}

void Thread3() {
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [] { return i == 1; });
    cout << "Message from Thread 3" << endl;
}

void Notify() {
    this_thread::sleep_for(chrono::milliseconds(100));

    {
        lock_guard<mutex> lock(mtx);
        i = 1;
    }

    cout << "notify_one()" << endl;
    cv.notify_one();
}

int main() {
    thread t1(Thread1);
    thread t2(Thread2);
    thread t3(Thread3);
    thread notifier(Notify);

    notifier.join();

    this_thread::sleep_for(chrono::milliseconds(100));
    cv.notify_all();

    t1.join();
    t2.join();
    t3.join();

    return 0;
}