#pragma once
#include <assert.h>

//���
template <typename T>
struct Node {
	T data;
	Node* nextPtr; // <T> ���� �ȿ��� ����� �� �Ⱥٿ��൵ ��
	Node<T>* prevPtr; // <T> ��������� ǥ��

	Node() : data(), nextPtr(nullptr), prevPtr(nullptr) {}
	Node(const T& input, Node<T>* next = nullptr, Node<T>* prev = nullptr)
		: data(input), nextPtr(next), prevPtr(prev) {}
	//������ push�� �� ���
	//Node(const T& input) : data(input) ,nextPtr(nullptr), prevPtr(nullptr) {}
};

//����� ���� ����Ʈ
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
		CList<T>* pList; // ����Ʈ
		Node<T>* pNode; // ���� ����Ű�� ����� �ּ�, null�� ��� end
		bool valid; //��ȿ��
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
		
		friend class CList<T>; //friend class CList;�� ����
	};
};

template <typename T>
CList<T>::CList() : head(nullptr), tail(nullptr), count(0) {}

template<typename T>
CList<T>::~CList() {
	Node<T>* deleteNode = head;
	while (deleteNode) { // �������� nullptr�� �Ǹ� �ݺ� ����
		Node<T>* nextNode = deleteNode->nextPtr;
		delete(deleteNode);
		deleteNode = nextNode;

	}
}

template<typename T>
void CList<T>::push_back(const T& input) {
	//���� �Էµ� �����͸� ������ ��� �����Ҵ�
	Node<T>* newNode = new Node<T>(input, nullptr, tail);

	if (nullptr == tail) //ó�� ������ ������
		tail = head = newNode; //ó������ ������ ������
	else { //������ �����Ͱ� �־���, ������ ���� ����� ����
		tail->nextPtr = newNode;
		tail = newNode;
	}

	count++;
}

template<typename T>
void CList<T>::push_front(const T& input) {
	Node<T>* newNode = new Node<T>(input, head, nullptr);

	if (nullptr == tail) //ó�� ������ ������
		tail = head = newNode; //ó������ ������ ������
	else { //������ �����Ͱ� �־���, ��� ���� ����� ����
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

//iter ���ʿ� ������ ����
template<typename T>
inline typename CList<T>::iterator CList<T>::insert(const iterator& iter, const T& d)
{
	if (end() == iter) assert(nullptr);

	Node<T>* newNode = new Node<T>(d, nullptr, nullptr); //�߰��� ���

	//iter�� ��� ����� ��� (������ ��带 ����)
	if (iter.pNode == head) {
		iter.pNode->prevPtr = newNode;
		newNode->nextPtr = iter.pNode;
		head = newNode;
	}
	else {
		iter.pNode->prevPtr->nextPtr = newNode; //iter�� ���� ���� �� �� 
		newNode->prevPtr = iter.pNode->prevPtr; //���� ��� �ּ� ��Ʈ�� ���� ������ ���� ����

		iter.pNode->prevPtr = newNode; //iter�� ����Ű�� �ִ� ����� ������ ���ο� ���� ����
		newNode->nextPtr = iter.pNode; //���ο� ����� ������ iter�� ����Ű�� �ִ� ���� ����
	}
	++count;
	return iterator(this, newNode);
}
