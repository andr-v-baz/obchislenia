#include <iostream>
#include <thread>
#include <future>
#include <deque>
#include <vector>
#include <mutex>
#include <condition_variable>

using namespace std;

deque<packaged_task<long long()>> tasks;
deque<int> numbers;

mutex mtx;
condition_variable cv;

bool finished = false;

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

void Worker() {
    while (true) {
        packaged_task<long long()> task;
        int n;

        {
            unique_lock<mutex> lock(mtx);

            cv.wait(lock, [] {
                return !tasks.empty() || finished;
            });

            if (tasks.empty() && finished)
                break;

            task = move(tasks.front());
            tasks.pop_front();

            n = numbers.front();
            numbers.pop_front();
        }

        cout << "Calculating prime #" << n << endl;
        task();
    }
}

int main() {
    thread worker(Worker);

    vector<future<long long>> results;
    vector<int> requestedNumbers;

    while (true) {
        int n;

        cout << "Enter n (0 - stop): ";
        cin >> n;

        if (n == 0)
            break;

        packaged_task<long long()> task([n] {
            return findPrime(n);
        });

        results.push_back(task.get_future());
        requestedNumbers.push_back(n);

        {
            lock_guard<mutex> lock(mtx);
            tasks.push_back(move(task));
            numbers.push_back(n);
        }

        cv.notify_one();
    }

    {
        lock_guard<mutex> lock(mtx);
        finished = true;
    }

    cv.notify_one();

    for (size_t i = 0; i < results.size(); i++) {
        cout << requestedNumbers[i]
             << "-th prime = "
             << results[i].get()
             << endl;
    }

    worker.join();

    return 0;
}