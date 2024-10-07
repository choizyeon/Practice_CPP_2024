#pragma once
#include <assert.h>
enum class NODE_TYPE { //������
	PARENT, //0
	LCHILD, //1
	RCHILD, //2
	END,    //3
};

template <typename T1, typename T2>
struct tPair {
	T1 first;
	T2 second;
};

template <typename T1, typename T2>
tPair<T1, T2> make_bstPair(const T1& _first, const T2& _second) {
	return tPair<T1, T2> {_first, _second};
}

template <typename T1, typename T2>
struct tBSTNode {
	tPair<T1, T2> pair; //data
	tBSTNode* arrNode[(int)NODE_TYPE::END];

	tBSTNode() : pair(), arrNode{} {}
	tBSTNode(const tPair<T1, T2>&_pair, tBSTNode* _pParent, tBSTNode* _pLChild, tBSTNode* _pRChild)
		: pair(_pair), arrNode{ _pParent,_pLChild, _pRChild } {}

	bool IsRoot() {
		if (arrNode[(int)NODE_TYPE::PARENT] == nullptr)
			return true;
		return false;
	}

	bool IsLeftChild() {
		if (arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::LCHILD] == this)
			return true;
		return false;
	}

	bool IsRightChild() {
		if (arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::RCHILD] == this)
			return true;
		return false;
	}

	bool IsLeaf() { //�ܸ� ��� ����
		if (nullptr == arrNode[(int)NODE_TYPE::LCHILD] && nullptr == arrNode[(int)NODE_TYPE::RCHILD])
			return true;
		return false;
	}

	bool IsFull() { //�ڽ� �ΰ�
		if (arrNode[(int)NODE_TYPE::LCHILD] && arrNode[(int)NODE_TYPE::RCHILD])
			return true;
		return false;
	}
};

template <typename T1, typename T2>
class CBST {
	tBSTNode<T1, T2>* m_pRoot; //��Ʈ ��� �ּ�
	int m_iCount; //������ ����

public:
	CBST() : m_pRoot(nullptr), m_iCount(0) {}

	bool insert(const tPair<T1, T2>& _pair);
	tBSTNode<T1, T2>* GetInOrderSuccessor(tBSTNode<T1, T2>* _pNode);
	tBSTNode<T1, T2>* GetInOrderPredecessor(tBSTNode<T1, T2>* _pNode);

	class iterator;
	iterator begin();
	iterator end();
	iterator find(const T1& _find);
	iterator erase(const iterator& _iter);

private:
	tBSTNode<T1, T2>* DeleteNode(tBSTNode<T1, T2>* _pTargetNode);
	
	class iterator {
		CBST<T1, T2>* m_pBST;
		tBSTNode<T1, T2>* m_pNode; //����Ű�� �ִ� ���, null�� ��� end()

	public:
		iterator() : m_pBST(nullptr), m_pNode(nullptr) {}
		iterator(CBST<T1, T2>* _pBST, tBSTNode<T1, T2>* _pNode) 
			: m_pBST(_pBST), m_pNode(_pNode) {}
		
		bool operator ==(const iterator& _other) {
			if (m_pBST == _other.m_pBST && m_pNode == _other.m_pNode) return true;
			return false;
		}

		bool operator !=(const iterator& _other) {
			return !(*this == _other);
		}

		const tPair<T1, T2>& operator *() { //������ ��ȯ
			assert(m_pNode); //end() (nullptr)�� �� ���
			return m_pNode->pair;
		}

		const tPair<T1, T2>* operator ->() { //�ּҰ� ��ȯ
			assert(m_pNode); //end() (nullptr)�� �� ���
			return &m_pNode->pair; //pair �ּ� ��ȯ
		}
		
		iterator& operator ++() {
			m_pNode = m_pBST->GetInOrderSuccessor(m_pNode);
			return *this;
		}

		friend class CBST<T1, T2>;
	};

};

template<typename T1, typename T2>
inline bool CBST<T1, T2>::insert(const tPair<T1, T2>& _pair){
	tBSTNode<T1, T2>* newNode = new tBSTNode<T1, T2>(_pair, nullptr, nullptr, nullptr);

	//ù �� ° �����͸� ������ �����Ͱ� ��Ʈ �����Ͱ� ��
	if (nullptr == m_pRoot) {
		m_pRoot = newNode;
	}
	else { //ũ�� �� �� ����
		tBSTNode<T1, T2>* pNode = m_pRoot;
		NODE_TYPE node_type = NODE_TYPE::END;

		while (true) {
			if (pNode->pair.first < newNode->pair.first)
				node_type = NODE_TYPE::RCHILD;
			else if (pNode->pair.first > newNode->pair.first) 
				node_type = NODE_TYPE::LCHILD;
			else 
				return false;

			if (nullptr == pNode->arrNode[(int)node_type]) {
				pNode->arrNode[(int)node_type] = newNode;
				newNode->arrNode[(int)NODE_TYPE::PARENT] = pNode;
				break;
			}
			else {
				pNode = pNode->arrNode[(int)node_type];
			}
		}
	}
	

	m_iCount++;
	return true;
}

template<typename T1, typename T2> //���� �ļ���
inline tBSTNode<T1, T2>* CBST<T1, T2>::GetInOrderSuccessor(tBSTNode<T1, T2>* _pNode)
{
	tBSTNode<T1, T2>* pSuccessor = nullptr;
	// 1)������ �ڽ� ���� -> ������ �ڽĿ��� ���� �ڽ��� ���� �� ���� ������
	if (nullptr != _pNode->arrNode[(int)NODE_TYPE::RCHILD]) {
		pSuccessor = _pNode->arrNode[(int)NODE_TYPE::RCHILD];
		
		while (pSuccessor->arrNode[(int)NODE_TYPE::LCHILD]) {
			pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::LCHILD];
		}
	}
	// 2)���� ��� �� ������ ���� ���
	else {
		pSuccessor = _pNode;

		while (true) {
			if (pSuccessor->IsRoot()) //���̻� �θ� ���� => ������ ��忴��
				return nullptr;
			if (pSuccessor->IsLeftChild()) { //���γ�尡 ���� �ڽ����� üũ
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT]; 
				break;
			}
			else 
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT];
		}
	}
	return pSuccessor;
}

template<typename T1, typename T2> //���� ������
inline tBSTNode<T1, T2>* CBST<T1, T2>::GetInOrderPredecessor(tBSTNode<T1, T2>* _pNode)
{
	return nullptr;
}

template<typename T1, typename T2>
inline typename CBST<T1, T2>::iterator CBST<T1, T2>::begin() //���� ��ȸ ����
{
	tBSTNode<T1, T2>* pNode = m_pRoot;
	while (pNode->arrNode[(int)NODE_TYPE::LCHILD]) { //���� �ڽ��� null�� ������ ��� ��ȸ
		pNode = pNode->arrNode[(int)NODE_TYPE::LCHILD];
	}

	return iterator(this, pNode);
}

template<typename T1, typename T2>
inline typename CBST<T1, T2>::iterator CBST<T1, T2>::end()
{
	return iterator(this, nullptr);
}

template<typename T1, typename T2>
inline typename CBST<T1, T2>::iterator CBST<T1, T2>::find(const T1& _find)
{	//insert()�� ����� ������� ����
	tBSTNode<T1, T2>* pNode = m_pRoot;
	NODE_TYPE node_type = NODE_TYPE::END;

	while (true) {
		if (pNode->pair.first < _find)
			node_type = NODE_TYPE::RCHILD;
		else if (pNode->pair.first > _find)
			node_type = NODE_TYPE::LCHILD;
		else { //ã�� pNode�� ã������ ��尡 ��
			break;
		}

		if (nullptr == pNode->arrNode[(int)node_type]) { //ã�� ����
			pNode = nullptr; //end iter
			break;
		}
		else {
			pNode = pNode->arrNode[(int)node_type];
		}
	}

	return iterator(this, pNode);
}

template<typename T1, typename T2>
inline typename CBST<T1, T2>::iterator CBST<T1, T2>::erase(const iterator& _iter)
{
	assert(this == _iter.m_pBST); //���θ� ����Ű�°� �´��� Ȯ��
	tBSTNode<T1, T2>* pSuccessor = DeleteNode(_iter.m_pNode);

	return iterator(this, pSuccessor);
}

template<typename T1, typename T2>
inline tBSTNode<T1, T2>* CBST<T1, T2>::DeleteNode(tBSTNode<T1, T2>* _pTargetNode)
{
	tBSTNode<T1, T2>* pSuccessor = GetInOrderSuccessor(_pTargetNode);

	//�����Ϸ��� ����� �ڽ� ������ Ȯ��
	if (_pTargetNode->IsLeaf()) { //�ڽ��� �ϳ��� ����
		if (_pTargetNode == m_pRoot) { //��Ʈ ����� ���
			m_pRoot = nullptr;
		}
		else { //��Ʈ ��� x
			//�θ� ���� ���� ��, ������ �ڽ� ��带 ����Ű�� �����͸� nullptr�� ����
			if (_pTargetNode->IsLeftChild())
				_pTargetNode->arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::LCHILD] = nullptr;
			else
				_pTargetNode->arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::RCHILD] = nullptr;
		}

		delete _pTargetNode;
		--m_iCount;
	}
	else if (_pTargetNode->IsFull()) { //�ڽ��� 2���� ���
		//���� �� �ڸ��� ���� �ļ����� �� ���� (���� �����ڷ� �ڵ��ص� ��)
		_pTargetNode->pair = pSuccessor->pair;
		//���� �ļ��� ��带 ����
		DeleteNode(pSuccessor);
		pSuccessor = _pTargetNode;
	}
	else { //�ڽ��� 1���� ���
		//�������� ���������� ����
		NODE_TYPE eChildType = NODE_TYPE::LCHILD;
		if(_pTargetNode->arrNode[(int)NODE_TYPE::RCHILD])
			eChildType = NODE_TYPE::RCHILD;

		if (_pTargetNode == m_pRoot) { //��Ʈ ����� ��� �ڽ��� ��Ʈ�� ����
			m_pRoot = _pTargetNode->arrNode[(int)eChildType];
			_pTargetNode->arrNode[(int)eChildType]->arrNode[(int)NODE_TYPE::PARENT] = nullptr;
		}
		else {//������ ����� �θ��, ������ ����� �ڽ��� ����
			if (_pTargetNode->IsLeftChild()) {
				_pTargetNode->arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::LCHILD] = 
					_pTargetNode->arrNode[(int)eChildType];
			}
			else {
				_pTargetNode->arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::RCHILD] =
					_pTargetNode->arrNode[(int)eChildType];
			}

			_pTargetNode->arrNode[(int)eChildType]->arrNode[(int)NODE_TYPE::PARENT] =
				_pTargetNode->arrNode[(int)NODE_TYPE::PARENT];

		}

		delete _pTargetNode;
		--m_iCount;
	}

	
	return pSuccessor;
}
