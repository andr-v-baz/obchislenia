#include <iostream>
#include <thread>
#include <future>
#include <cmath>
#include <chrono>

using namespace std;

bool isPrime(long long n) {
    if (n < 2) return false;

    for (long long i = 2; i * i <= n; i++)
        if (n % i == 0)
            return false;

    return true;
}

long long findPrime(int n) {
    int count = 0;
    long long number = 1;

    while (count < n) {
        number++;

        if (isPrime(number))
            count++;
    }

    return number;
}

void Thread1(
    int n,
    promise<long long> p1,
    promise<bool> signal,
    promise<long long> p2
) {
    long long first = findPrime(n);
    p1.set_value(first);

    signal.set_value(true);

    long long second = findPrime(n * 10);
    p2.set_value(second);
}

void Thread2(int n, future<bool> signal) {
    bool start = signal.get();

    if (start) {
        this_thread::sleep_for(chrono::seconds(2));

        cout << "sqrt(" << n << ") = "
             << sqrt(n) << endl;
    }
}

int main() {
    int n;

    cout << "Enter n: ";
    cin >> n;

    promise<long long> p1;
    promise<long long> p2;
    promise<bool> signal;

    future<long long> f1 = p1.get_future();
    future<long long> f2 = p2.get_future();
    future<bool> signalFuture = signal.get_future();

    thread t1(
        Thread1,
        n,
        move(p1),
        move(signal),
        move(p2)
    );

    thread t2(
        Thread2,
        n,
        move(signalFuture)
    );

    cout << n << "-th prime = "
         << f1.get() << endl;

    cout << n * 10 << "-th prime = "
         << f2.get() << endl;

    t1.join();
    t2.join();

    return 0;
}