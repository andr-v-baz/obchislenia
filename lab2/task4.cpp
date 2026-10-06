#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

using namespace std;

queue<int> numbers;
mutex mtx;
condition_variable cv;

bool isPrime(int n) {
    if (n < 2) return false;

    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return false;

    return true;
}

void DataPreparation() {
    int n;

    cout << "Enter numbers (0 - stop): ";

    while (cin >> n && n != 0) {
        lock_guard<mutex> lock(mtx);
        numbers.push(n);
    }

    cv.notify_one();
}

void DataProcessing() {
    unique_lock<mutex> lock(mtx);

    cv.wait(lock, [] {
        return !numbers.empty();
    });

    cout << "Prime numbers: ";

    while (!numbers.empty()) {
        int n = numbers.front();
        numbers.pop();

        if (isPrime(n))
            cout << n << " ";
    }

    cout << endl;
}

int main() {
    thread t1(DataPreparation);
    t1.detach();

    thread t2(DataProcessing);
    t2.join();

    return 0;
}