#pragma once
#include <assert.h>

//노드
template <typename T>
struct Node {
	T data;
	Node* nextPtr; // <T> 본인 안에서 사용할 땐 안붙여줘도 됨
	Node<T>* prevPtr; // <T> 명시적으로 표기

	Node() : data(), nextPtr(nullptr), prevPtr(nullptr) {}
	Node(const T& input, Node<T>* next = nullptr, Node<T>* prev = nullptr)
		: data(input), nextPtr(next), prevPtr(prev) {}
	//데이터 push할 때 사용
	//Node(const T& input) : data(input) ,nextPtr(nullptr), prevPtr(nullptr) {}
};

//양방향 연결 리스트
template <typename T>
class CList {
private:
	Node<T>* head;
	Node<T>* tail;
	int count;

public:
	CList();
	~CList();
	
	void push_back(const T& input);
	void push_front(const T& input);
	
	class iterator;
	iterator begin();
	iterator end();
	iterator erase(iterator& iter);
	iterator insert(const iterator& iter, const T& d);

	class iterator {
		CList<T>* pList; // 리스트
		Node<T>* pNode; // 현재 가리키는 노드의 주소, null인 경우 end
		bool valid; //유효성
	public:
		iterator() : pList(nullptr), pNode(nullptr), valid(false) {}
		iterator(CList<T>* pL, Node<T>* pN)
			: pList(pL), pNode(pN), valid(false) {
			if (pL != nullptr && pN != nullptr) valid = true;
		}
		~iterator() {}


		T& operator*() { return pNode->data; }
		bool operator == (const iterator& other) const{
			if (pList == other.pList && pNode == other.pNode) return true;
			else return false;
		}
		bool operator != (const iterator& other) {
			return !(*this == other);
		}
		iterator& operator ++() {
			if (pNode == nullptr || !valid) assert(nullptr);
			pNode = pNode->nextPtr;
			return *this;
		}
		iterator& operator ++(int) {
			iterator copyiter(*this);
			++(*this);
			return copyiter;
		}
		iterator& operator --() {
			if (pNode == nullptr || !valid) assert(nullptr);
			pNode = pNode->prevPtr;
			return *this;
		}
		iterator& operator --(int) {
			iterator copyiter(*this);
			--(*this);
			return copyiter;
		}
		
		friend class CList<T>; //friend class CList;도 가능
	};
};

template <typename T>
CList<T>::CList() : head(nullptr), tail(nullptr), count(0) {}

template<typename T>
CList<T>::~CList() {
	Node<T>* deleteNode = head;
	while (deleteNode) { // 다지워져 nullptr이 되면 반복 종료
		Node<T>* nextNode = deleteNode->nextPtr;
		delete(deleteNode);
		deleteNode = nextNode;

	}
}

template<typename T>
void CList<T>::push_back(const T& input) {
	//새로 입력된 데이터를 저장할 노드 동적할당
	Node<T>* newNode = new Node<T>(input, nullptr, tail);

	if (nullptr == tail) //처음 생성된 데이터
		tail = head = newNode; //처음이자 마지막 데이터
	else { //기존에 데이터가 있었음, 마지막 노드와 양방향 연결
		tail->nextPtr = newNode;
		tail = newNode;
	}

	count++;
}

template<typename T>
void CList<T>::push_front(const T& input) {
	Node<T>* newNode = new Node<T>(input, head, nullptr);

	if (nullptr == tail) //처음 생성된 데이터
		tail = head = newNode; //처음이자 마지막 데이터
	else { //기존에 데이터가 있었음, 헤드 노드와 양방향 연결
		head->prevPtr = newNode;
		head = newNode;
	}

	count++;
}

template<typename T>
inline typename CList<T>::iterator CList<T>::begin()
{
	return iterator(this, head);
}

template<typename T>
inline typename CList<T>::iterator CList<T>::end()
{
	return iterator(this, nullptr);
}


template<typename T>
inline typename CList<T>::iterator CList<T>::erase(iterator& iter) { 
	if (end() == iter) assert(nullptr);
	Node<T>* deleteNode = iter.pNode;
	Node<T>* prevNode = deleteNode->prevPtr;
	Node<T>* nextNode = deleteNode->nextPtr;

	if (prevNode != nullptr) prevNode->nextPtr = nextNode;
	else head = nextNode;

	if (nextNode != nullptr) nextNode->prevPtr = prevNode;
	else tail = prevNode;

	delete deleteNode;
	--count;

	return iterator(this, nextNode);
}

//iter 앞쪽에 데이터 삽입
template<typename T>
inline typename CList<T>::iterator CList<T>::insert(const iterator& iter, const T& d)
{
	if (end() == iter) assert(nullptr);

	Node<T>* newNode = new Node<T>(d, nullptr, nullptr); //추가할 노드

	//iter가 헤드 노드인 경우 (생성한 노드를 헤드로)
	if (iter.pNode == head) {
		iter.pNode->prevPtr = newNode;
		newNode->nextPtr = iter.pNode;
		head = newNode;
	}
	else {
		iter.pNode->prevPtr->nextPtr = newNode; //iter의 이전 노드로 간 후 
		newNode->prevPtr = iter.pNode->prevPtr; //다음 노드 주소 파트를 새로 생성한 노드로 지정

		iter.pNode->prevPtr = newNode; //iter가 가리키고 있는 노드의 이전을 새로운 노드로 지정
		newNode->nextPtr = iter.pNode; //새로운 노드의 다음을 iter가 가리키고 있는 노드로 지정
	}
	++count;
	return iterator(this, newNode);
}
