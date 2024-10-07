#pragma once
#include <assert.h>
enum class NODE_TYPE { //열거형
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

	bool IsLeaf() { //단말 노드 인지
		if (nullptr == arrNode[(int)NODE_TYPE::LCHILD] && nullptr == arrNode[(int)NODE_TYPE::RCHILD])
			return true;
		return false;
	}

	bool IsFull() { //자식 두개
		if (arrNode[(int)NODE_TYPE::LCHILD] && arrNode[(int)NODE_TYPE::RCHILD])
			return true;
		return false;
	}
};

template <typename T1, typename T2>
class CBST {
	tBSTNode<T1, T2>* m_pRoot; //루트 노드 주소
	int m_iCount; //데이터 개수

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
		tBSTNode<T1, T2>* m_pNode; //가리키고 있는 노드, null일 경우 end()

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

		const tPair<T1, T2>& operator *() { //참조로 반환
			assert(m_pNode); //end() (nullptr)일 때 경고
			return m_pNode->pair;
		}

		const tPair<T1, T2>* operator ->() { //주소값 반환
			assert(m_pNode); //end() (nullptr)일 때 경고
			return &m_pNode->pair; //pair 주소 반환
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

	//첫 번 째 데이터면 삽입한 데이터가 루트 데이터가 됨
	if (nullptr == m_pRoot) {
		m_pRoot = newNode;
	}
	else { //크기 비교 후 삽입
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

template<typename T1, typename T2> //중위 후속자
inline tBSTNode<T1, T2>* CBST<T1, T2>::GetInOrderSuccessor(tBSTNode<T1, T2>* _pNode)
{
	tBSTNode<T1, T2>* pSuccessor = nullptr;
	// 1)오른쪽 자식 보유 -> 오른쪽 자식에서 왼쪽 자식이 없을 때 까지 내려감
	if (nullptr != _pNode->arrNode[(int)NODE_TYPE::RCHILD]) {
		pSuccessor = _pNode->arrNode[(int)NODE_TYPE::RCHILD];
		
		while (pSuccessor->arrNode[(int)NODE_TYPE::LCHILD]) {
			pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::LCHILD];
		}
	}
	// 2)조상 노드 중 최초의 왼쪽 노드
	else {
		pSuccessor = _pNode;

		while (true) {
			if (pSuccessor->IsRoot()) //더이상 부모가 없음 => 마지막 노드였음
				return nullptr;
			if (pSuccessor->IsLeftChild()) { //본인노드가 왼쪽 자식인지 체크
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT]; 
				break;
			}
			else 
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT];
		}
	}
	return pSuccessor;
}

template<typename T1, typename T2> //중위 선행자
inline tBSTNode<T1, T2>* CBST<T1, T2>::GetInOrderPredecessor(tBSTNode<T1, T2>* _pNode)
{
	return nullptr;
}

template<typename T1, typename T2>
inline typename CBST<T1, T2>::iterator CBST<T1, T2>::begin() //중위 순회 기준
{
	tBSTNode<T1, T2>* pNode = m_pRoot;
	while (pNode->arrNode[(int)NODE_TYPE::LCHILD]) { //왼쪽 자식이 null일 때까지 계속 순회
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
{	//insert()랑 비슷한 방식으로 동작
	tBSTNode<T1, T2>* pNode = m_pRoot;
	NODE_TYPE node_type = NODE_TYPE::END;

	while (true) {
		if (pNode->pair.first < _find)
			node_type = NODE_TYPE::RCHILD;
		else if (pNode->pair.first > _find)
			node_type = NODE_TYPE::LCHILD;
		else { //찾음 pNode가 찾으려는 노드가 됨
			break;
		}

		if (nullptr == pNode->arrNode[(int)node_type]) { //찾지 못함
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
	assert(this == _iter.m_pBST); //내부를 가리키는게 맞는지 확인
	tBSTNode<T1, T2>* pSuccessor = DeleteNode(_iter.m_pNode);

	return iterator(this, pSuccessor);
}

template<typename T1, typename T2>
inline tBSTNode<T1, T2>* CBST<T1, T2>::DeleteNode(tBSTNode<T1, T2>* _pTargetNode)
{
	tBSTNode<T1, T2>* pSuccessor = GetInOrderSuccessor(_pTargetNode);

	//삭제하려는 노드의 자식 개수를 확인
	if (_pTargetNode->IsLeaf()) { //자식이 하나도 없음
		if (_pTargetNode == m_pRoot) { //루트 노드일 경우
			m_pRoot = nullptr;
		}
		else { //루트 노드 x
			//부모 노드로 접근 후, 삭제될 자식 노드를 가리키는 포인터를 nullptr로 만듦
			if (_pTargetNode->IsLeftChild())
				_pTargetNode->arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::LCHILD] = nullptr;
			else
				_pTargetNode->arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::RCHILD] = nullptr;
		}

		delete _pTargetNode;
		--m_iCount;
	}
	else if (_pTargetNode->IsFull()) { //자식이 2개인 경우
		//삭제 될 자리에 중위 후속자의 값 복사 (중위 선행자로 코딩해도 됨)
		_pTargetNode->pair = pSuccessor->pair;
		//중위 후속자 노드를 삭제
		DeleteNode(pSuccessor);
		pSuccessor = _pTargetNode;
	}
	else { //자식이 1개인 경우
		//왼쪽인지 오른쪽인지 저장
		NODE_TYPE eChildType = NODE_TYPE::LCHILD;
		if(_pTargetNode->arrNode[(int)NODE_TYPE::RCHILD])
			eChildType = NODE_TYPE::RCHILD;

		if (_pTargetNode == m_pRoot) { //루트 노드일 경우 자식을 루트로 만듦
			m_pRoot = _pTargetNode->arrNode[(int)eChildType];
			_pTargetNode->arrNode[(int)eChildType]->arrNode[(int)NODE_TYPE::PARENT] = nullptr;
		}
		else {//삭제될 노드의 부모와, 삭제될 노드의 자식을 연결
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
