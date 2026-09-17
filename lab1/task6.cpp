#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <chrono>
using namespace std;

class someData
{
public:
    string name;
    string surname;
    string address;
    int age;
};

class exchangePerson
{
public:
    someData data;
    mutex m;

    static void JohnDoe(exchangePerson& p)
    {
        lock_guard<mutex> lock(p.m);

        p.data.name = "John";
        p.data.surname = "Doe";
        p.data.address = "Unknown";
        p.data.age = 120;
    }

    static void JacobSmith(exchangePerson& p)
    {
        lock_guard<mutex> lock(p.m);

        p.data.name = "Jacob";
        p.data.surname = "Smith";
        p.data.address = "Known";
        p.data.age = 1;
    }

    static void Swap(exchangePerson& p1, exchangePerson& p2)
    {
        if (&p1 == &p2)
            return;

        lock(p1.m, p2.m);

        lock_guard<mutex> lock1(p1.m, adopt_lock);
        lock_guard<mutex> lock2(p2.m, adopt_lock);

        cout << "Before swap:" << endl;
        cout << p1.data.name << " " << p1.data.surname << " "
             << p1.data.address << " " << p1.data.age << endl;

        cout << p2.data.name << " " << p2.data.surname << " "
             << p2.data.address << " " << p2.data.age << endl;

        swap(p1.data, p2.data);

        cout << "After swap:" << endl;
        cout << p1.data.name << " " << p1.data.surname << " "
             << p1.data.address << " " << p1.data.age << endl;

        cout << p2.data.name << " " << p2.data.surname << " "
             << p2.data.address << " " << p2.data.age << endl;
    }
};

int main()
{
    exchangePerson p1;
    exchangePerson p2;

    thread t1(exchangePerson::JohnDoe, ref(p1));
    thread t2(exchangePerson::JacobSmith, ref(p2));

    t1.detach();
    t2.detach();

    this_thread::sleep_for(chrono::milliseconds(100));

    thread t3(exchangePerson::Swap, ref(p1), ref(p2));
    t3.join();

    return 0;
}