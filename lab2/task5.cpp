#include <iostream>
#include <future>
#include <cmath>
#include <chrono>

using namespace std;

bool isPrime(long long n) {
    if (n < 2)
        return false;

    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }

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

void test(launch mode, int n, string name) {
    cout << "\n--- " << name << " ---" << endl;

    future<long long> result = async(mode, findPrime, n);

    int choice;
    double value;

    cout << "Choose a function:" << endl;
    cout << "1 - square root" << endl;
    cout << "2 - sine" << endl;
    cout << "3 - natural logarithm" << endl;

    while (true) {
        cout << "Your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error! Enter a number from 1 to 3." << endl;
        }
        else if (choice < 1 || choice > 3) {
            cout << "Error! Choose 1, 2 or 3." << endl;
        }
        else {
            break;
        }
    }

    cout << "Enter a number: ";
    cin >> value;

    if (choice == 1)
        cout << "Result: " << sqrt(value) << endl;
    else if (choice == 2)
        cout << "Result: " << sin(value) << endl;
    else
        cout << "Result: " << log(value) << endl;

    auto start = chrono::steady_clock::now();

    long long prime = result.get();

    auto end = chrono::steady_clock::now();

    auto time = chrono::duration_cast<chrono::milliseconds>(
        end - start
    ).count();

    cout << n << "-th prime number: " << prime << endl;
    cout << "Waiting time: " << time << " ms" << endl;
}

int main() {
    int n;

    while (true) {
        cout << "Enter prime number index: ";
        cin >> n;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error! Enter an integer." << endl;
        }
        else if (n <= 0) {
            cout << "Error! Number must be greater than 0." << endl;
        }
        else {
            break;
        }
    }

    test(launch::deferred, n, "DEFERRED");
    test(launch::async, n, "ASYNC");

    return 0;
}