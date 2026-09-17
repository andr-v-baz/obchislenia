#include <iostream>
#include <thread>
#include <list>
using namespace std;

list<int> l;

void AddToList(int n)
{
    for (int i = 0; i < 10; i++)
    {
        l.push_back(n + i);
        cout << "Added: " << n + i << endl;
    }
}

void ListContains(int n)
{
    for (int i = 0; i < 10; i++)
    {
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
}

int main()
{
    int n = 5;

    thread t1(AddToList, n);
    thread t2(ListContains, n);

    t1.join();
    t2.join();

    return 0;
}
