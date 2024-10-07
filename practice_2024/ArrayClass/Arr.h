#pragma once
#include <assert.h>
template <typename T>
class ArrClass
{
private:
	T* data;
	int count;
	int maxCount;

public:
	ArrClass() : data(nullptr), count(0), maxCount(2)
	{
		data = new T[maxCount];
	}
	~ArrClass() { delete[] data; } // 동적 배열 해제

	T* getData() { return data; }
	int size() { return count; }
	int capacity() { return maxCount; }
	T& operator[] (int idx) { return data[idx]; }

	void resize(int resizeCount);
	void push_back(const T& newData);
	void clear() { count = 0; }

	class iterator; //전방선언
	iterator begin();
	iterator end();
	iterator erase(iterator& iter);

	class iterator { // 이너 클래스, 클래스의 private, 탬플릿 사용 가능 //반대는 x
		ArrClass* arr; //iter가 가리킬 데이터 관리하는 가변배열 주소
		T* iterData; // 데이터들의 시작 주소, 주소 변경 시 감지 가능 (예외 처리할 때 필요)
		int iterIdx; //가리키는 데이터 인덱스. -1일 땐 end() 상태
		bool vaild;

	public:
		iterator() : arr(nullptr), iterData(nullptr), iterIdx(-1), vaild(false) {}
		iterator(ArrClass* pArr, T* pData, int idx) :
			arr(pArr), iterData(pData), iterIdx(idx), vaild(false) {
			if (nullptr != pArr && 0 <= idx) vaild = true;
		}
		~iterator() {}

		T& operator *() {
			if (arr->data != iterData || iterIdx < 0 || !vaild)
				assert(nullptr); //주소가 달라진 경우 or 데이터가 없는 경우 예외
			return iterData[iterIdx];
		}

		iterator& operator ++() { //전위연산자
			if (arr->data != iterData || iterIdx < 0)
				assert(nullptr); //주소가 달라진 경우or데이터가 없는 경우 먼저 예외처리

			if (arr->size() - 1 == iterIdx) iterIdx = -1; //iter가 마지막요소를 가리키고 있음
			else ++iterIdx;

			return *this; //연쇄적으로 사용가능 EX)  ++(++i);
		}
		iterator operator ++(int) { //후위연산자 / 반환타입: 별개의 iter(복사본) 반환
			iterator copy_iter = *this; //객체 복사 생성 (메모리 비용 발생)
			++(*this); // 반환 후 증가시켜줌
			return copy_iter;
		}

		iterator& operator --() {
			if (arr->data != iterData || arr->size() == 0)
				assert(nullptr); // 주소가 달라진 경우 or 데이터가 없는 경우 예외 처리

			if (iterIdx == -1)
				iterIdx = arr->size() - 1; // end 상태일 때, 마지막 요소로 설정
			else
				--iterIdx;

			return *this;
		}

		iterator operator --(int) {
			iterator copy_iter = *this;
			--(*this);
			return copy_iter;
		}

		bool operator == (const iterator& other) const { //객체와 인덱스가 같아야함
			return iterData == other.iterData && iterIdx == other.iterIdx;
		}

		bool operator != (const iterator& other) const {
			return !(*this == other); // 이미 구현한 == 를 사용
		}

		friend class ArrClass; // iter의 private를 ArrC에서도 사용 가능
	};
};

template <typename T>
void ArrClass<T>::resize(int resizeCount) {
	if (maxCount >= resizeCount) return;
	T* newData = new T[resizeCount]; // 데이터 복사
	for (int i = 0; i < count; i++) {
		newData[i] = data[i];
	}
	delete[] data; // 기존 데이터 해제
	data = newData; // 새로 작성한 데이터 주소 연결
	maxCount = resizeCount;
}

template <typename T>
void ArrClass<T>::push_back(const T& pushData) {
	if (maxCount <= count) resize(maxCount * 2);
	data[count++] = pushData;
}


template <typename T>
typename ArrClass<T>::iterator ArrClass<T>::begin() { // 반환 타입이 이너 클래스 일 때 
	if (0 == count) return iterator(this, data, -1); //2번 째 인자에 객체(가변배열) data 대입
	return iterator(this, data, 0); //임시 객체로 시작가리키는 iter 반환 
}

template <typename T>
typename ArrClass<T>::iterator ArrClass<T>::end() {
	return iterator(this, data, -1); //데이터 없음 begin() == end()
}

template <typename T>
typename ArrClass<T>::iterator ArrClass<T>::erase(iterator& iter) {
	//iter가 다른 객체 가리킴 || end iter일 경우 
	if (iter.arr != this || end() == iter || count <= iter.iterIdx)
		assert(nullptr);

	//iter가 가리키는 데이터 배열 내에서 제거
	int loopCount = count - (iter.iterIdx + 1);

	for (int i = 0; i < loopCount; i++) {
		data[i + iter.iterIdx] = data[i + iter.iterIdx + 1]; //데이터 한칸 앞으로 땡김
	}
	iter.vaild = false;
	count--;

	return iterator(this, data, iter.iterIdx);
} //삭제된 데이터 다음 데이터 가리킴(위치는 같음)