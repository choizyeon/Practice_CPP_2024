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
	~ArrClass() { delete[] data; } // ���� �迭 ����

	T* getData() { return data; }
	int size() { return count; }
	int capacity() { return maxCount; }
	T& operator[] (int idx) { return data[idx]; }

	void resize(int resizeCount);
	void push_back(const T& newData);
	void clear() { count = 0; }

	class iterator; //���漱��
	iterator begin();
	iterator end();
	iterator erase(iterator& iter);

	class iterator { // �̳� Ŭ����, Ŭ������ private, ���ø� ��� ���� //�ݴ�� x
		ArrClass* arr; //iter�� ����ų ������ �����ϴ� �����迭 �ּ�
		T* iterData; // �����͵��� ���� �ּ�, �ּ� ���� �� ���� ���� (���� ó���� �� �ʿ�)
		int iterIdx; //����Ű�� ������ �ε���. -1�� �� end() ����
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
				assert(nullptr); //�ּҰ� �޶��� ��� or �����Ͱ� ���� ��� ����
			return iterData[iterIdx];
		}

		iterator& operator ++() { //����������
			if (arr->data != iterData || iterIdx < 0)
				assert(nullptr); //�ּҰ� �޶��� ���or�����Ͱ� ���� ��� ���� ����ó��

			if (arr->size() - 1 == iterIdx) iterIdx = -1; //iter�� ��������Ҹ� ����Ű�� ����
			else ++iterIdx;

			return *this; //���������� ��밡�� EX)  ++(++i);
		}
		iterator operator ++(int) { //���������� / ��ȯŸ��: ������ iter(���纻) ��ȯ
			iterator copy_iter = *this; //��ü ���� ���� (�޸� ��� �߻�)
			++(*this); // ��ȯ �� ����������
			return copy_iter;
		}

		iterator& operator --() {
			if (arr->data != iterData || arr->size() == 0)
				assert(nullptr); // �ּҰ� �޶��� ��� or �����Ͱ� ���� ��� ���� ó��

			if (iterIdx == -1)
				iterIdx = arr->size() - 1; // end ������ ��, ������ ��ҷ� ����
			else
				--iterIdx;

			return *this;
		}

		iterator operator --(int) {
			iterator copy_iter = *this;
			--(*this);
			return copy_iter;
		}

		bool operator == (const iterator& other) const { //��ü�� �ε����� ���ƾ���
			return iterData == other.iterData && iterIdx == other.iterIdx;
		}

		bool operator != (const iterator& other) const {
			return !(*this == other); // �̹� ������ == �� ���
		}

		friend class ArrClass; // iter�� private�� ArrC������ ��� ����
	};
};

template <typename T>
void ArrClass<T>::resize(int resizeCount) {
	if (maxCount >= resizeCount) return;
	T* newData = new T[resizeCount]; // ������ ����
	for (int i = 0; i < count; i++) {
		newData[i] = data[i];
	}
	delete[] data; // ���� ������ ����
	data = newData; // ���� �ۼ��� ������ �ּ� ����
	maxCount = resizeCount;
}

template <typename T>
void ArrClass<T>::push_back(const T& pushData) {
	if (maxCount <= count) resize(maxCount * 2);
	data[count++] = pushData;
}


template <typename T>
typename ArrClass<T>::iterator ArrClass<T>::begin() { // ��ȯ Ÿ���� �̳� Ŭ���� �� �� 
	if (0 == count) return iterator(this, data, -1); //2�� ° ���ڿ� ��ü(�����迭) data ����
	return iterator(this, data, 0); //�ӽ� ��ü�� ���۰���Ű�� iter ��ȯ 
}

template <typename T>
typename ArrClass<T>::iterator ArrClass<T>::end() {
	return iterator(this, data, -1); //������ ���� begin() == end()
}

template <typename T>
typename ArrClass<T>::iterator ArrClass<T>::erase(iterator& iter) {
	//iter�� �ٸ� ��ü ����Ŵ || end iter�� ��� 
	if (iter.arr != this || end() == iter || count <= iter.iterIdx)
		assert(nullptr);

	//iter�� ����Ű�� ������ �迭 ������ ����
	int loopCount = count - (iter.iterIdx + 1);

	for (int i = 0; i < loopCount; i++) {
		data[i + iter.iterIdx] = data[i + iter.iterIdx + 1]; //������ ��ĭ ������ ����
	}
	iter.vaild = false;
	count--;

	return iterator(this, data, iter.iterIdx);
} //������ ������ ���� ������ ����Ŵ(��ġ�� ����)