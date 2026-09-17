#include <iostream>
#include <thread>
#include <list>
#include <mutex>
#include <chrono>
using namespace std;

list<int> l;
mutex m;

void AddToList(int n)
{
    lock_guard<mutex> lock(m);

    l.push_back(n);
    cout << "Added: " << n << endl;
}

void ListContains(int n)
{
    lock_guard<mutex> lock(m);

    bool found = false;

    for (int x : l)
    {
        if (x == n)
            found = true;
    }

    if (found)
        cout << n << " is in list" << endl;
    else
        cout << n << " is not in list" << endl;
}

int main()
{
    int n = 5;

    for (int i = 0; i < 10; i++)
    {
        thread t1(AddToList, n + i);
        thread t2(ListContains, n);

        t1.detach();
        t2.detach();
    }

    this_thread::sleep_for(chrono::milliseconds(500));

    return 0;
}