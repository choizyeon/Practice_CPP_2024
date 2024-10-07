#include <iostream>
#include "ArrClass.h"
using namespace std;
/*
* c++에서 구조체와 클래스의 큰 차이는 없다.
*
* 하지만 구조체는 기본 접근 제한자가 public이고,
* 클래스는 private이다.
* 상속 시에도 마찬가지이다.
*
* 관례적으로 구조체는 간단한 데이터 구조일 시 주로 사용하고,
* 클래스는 복합적인 구조와 객체 지향적 프로그래밍의 다양한 기능을 활용하고자 할 때 사용
*/
struct MyStruct {
//public:
    int i;

    MyStruct() : i(10) {}
    MyStruct(int i) : i(i) {} //사용자 정의 생성자 호출
};

class MyClass {
//private:
    int i;

public:
    //MyClass() { i = 100; }
    MyClass() : i(100) {}
    MyClass(const MyClass& other) : i(other.i) {} //복사 생성자
    void SetInt(int i) { this->i = i; } 
    void PrintInt() { printf("%d\n", i); };
    MyClass& operator = (const MyClass& other) { //대입 연산자
        i = other.i; 
        return *this; 
    } 
    /* 기본생성자, 기본소멸자, 대입연산자, 복사생성자는 명시안해도 자동으로 생성됨 
       하지만 사용자가 명시하면 컴파일러가 생성 안해줌
       복사 생성자를 명시해도 기본 생성자 생성안됨. (생성자이기 때문)
    */
};
int main()
{
    /*
    MyStruct s(50);
    MyClass c; // 생성자 자동 호출됨 MyClass() { c_i = 100; }
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
    myiter = ac.erase(myiter); //다시 대입 안해주면 vaild == false 인 상태
   
    for (; myiter != ac.end(); ++myiter) {
        cout << *myiter << endl;
    }
    --myiter;
    for (; myiter != ac.begin(); --myiter) {
        cout << *myiter << endl;
    }
    cout << *(myiter--) << endl;
   

    MyClass t1;
    MyClass t2(t1); //복사 생성
    MyClass t3 = t1; //컴파일러가 복사 생성으로 자동 변환 해줌
    return 0;
}
