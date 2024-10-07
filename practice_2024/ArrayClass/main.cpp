#include <iostream>
#include "Arr.h"
using namespace std;
/*
* c++���� ����ü�� Ŭ������ ū ���̴� ����.
*
* ������ ����ü�� �⺻ ���� �����ڰ� public�̰�,
* Ŭ������ private�̴�.
* ��� �ÿ��� ���������̴�.
*
* ���������� ����ü�� ������ ������ ������ �� �ַ� ����ϰ�,
* Ŭ������ �������� ������ ��ü ������ ���α׷����� �پ��� ����� Ȱ���ϰ��� �� �� ���
*/
struct MyStruct {
    //public:
    int i;

    MyStruct() : i(10) {}
    MyStruct(int i) : i(i) {} //����� ���� ������ ȣ��
};

class MyClass {
    //private:
    int i;

public:
    //MyClass() { i = 100; }
    MyClass() : i(100) {}
    MyClass(const MyClass& other) : i(other.i) {} //���� ������
    void SetInt(int i) { this->i = i; }
    void PrintInt() { printf("%d\n", i); };
    MyClass& operator = (const MyClass& other) { //���� ������
        i = other.i;
        return *this;
    }
    /* �⺻������, �⺻�Ҹ���, ���Կ�����, ��������ڴ� ��þ��ص� �ڵ����� ������
       ������ ����ڰ� ����ϸ� �����Ϸ��� ���� ������
       ���� �����ڸ� ����ص� �⺻ ������ �����ȵ�. (�������̱� ����)
    */
};
int main()
{
    /*
    MyStruct s(50);
    MyClass c; // ������ �ڵ� ȣ��� MyClass() { c_i = 100; }
    c.SetInt(20);
    c.PrintInt();
    */

    ArrClass<int> ac;
    ac.push_back(10);
    ac.push_back(20);
    ac.push_back(30);
    ac.push_back(40);
    ac.push_back(50);

    ArrClass<int>::iterator myiter = ac.begin();
    myiter = ac.erase(myiter); //�ٽ� ���� �����ָ� vaild == false �� ����

    for (; myiter != ac.end(); ++myiter) {
        cout << *myiter << endl;
    }
    --myiter;
    for (; myiter != ac.begin(); --myiter) {
        cout << *myiter << endl;
    }
    cout << *(myiter--) << endl;


    MyClass t1;
    MyClass t2(t1); //���� ����
    MyClass t3 = t1; //�����Ϸ��� ���� �������� �ڵ� ��ȯ ����
    return 0;
}
