#pragma once
#include<stdexcept>

using std::logic_error;

template<typename Key>
class Fibonachi_Heap
{
public:
	class Pointer;

	Pointer insert(Key T);
	Fibonachi_Heap();
	Key extractMin();
	bool isEmpty() const;
	Key getMin() const;
	void change(Pointer *Ptr, Key T);
	void erase(Pointer *Ptr);
	void merge(Fibonachi_Heap &otherHeap);
	Fibonachi_Heap(const Fibonachi_Heap&) = delete;
	Fibonachi_Heap& operator=(const Fibonachi_Heap&) = delete;
	~Fibonachi_Heap();
private:
	struct Node;
	struct BTree;

	void recalcmin();
	void cascadingCut(BTree* a);
	void cut(BTree* a);
	void update(Node* a);
	void merge(Node* a);
	void eraseNode(Node *a);
	void addNode(Node *a);
	void consolidate();
	void updatemin(Node* other);
	size_t calcmaxdeg();

	static void del(Node* &a);
	static void push(Node** data, Node* a);
	static BTree * mergeBTree(BTree* first, BTree* second);

	size_t size = 0;
	Node* min = nullptr;
	Node* leftSon = nullptr;
	Node* rightSon = nullptr;
};

template<typename Key>
class Fibonachi_Heap<Key>::Pointer
{
	friend Fibonachi_Heap;
public:
	Pointer(Pointer&& right) noexcept
	{
		son = right.son;
		son->owner = this;
		right.son = nullptr;
	}
	Pointer(const Pointer&) = delete;
	Pointer& operator=(const Pointer&) = delete;
	inline Key get_value() const
	{
		if (!haveSon())
			throw logic_error("i haven't got son!!!");
		return son->data;
	}
	bool haveSon() const
	{
		return son != nullptr;
	}
	~Pointer()
	{
		if (son != nullptr)
			son->owner = nullptr;
	}
private:
	explicit Pointer(BTree *son_ = nullptr) :son(son_)
	{
		if (son != nullptr)
			son_->owner = this;
	}
	inline BTree* get_son() const
	{
		if (!haveSon())
			throw logic_error("i haven't got son!!!");
		return son;
	}
	void change_son(BTree* son_)
	{
		son = son_;
		if (son != nullptr)
			son->owner = this;
	}
	BTree* son = nullptr;
};

template<typename Key>
struct Fibonachi_Heap<Key>::BTree
{
	size_t st = 0;
	bool mark = false;
	BTree* pr = nullptr;
	Node* leftSon = nullptr;
	Node* rightSon = nullptr;
	Node* prNode = nullptr;
	Key data;
	Pointer *owner;
	BTree(Key T, Pointer* p) : data(T), owner(p)
	{
		p->change_son(this);
		leftSon = new Node();
		rightSon = leftSon;
	}
	BTree(const BTree&) = delete;
	BTree& operator=(const BTree&) = delete;
	void eraseNode(Node* a)
	{
		if (a == rightSon)
			throw logic_error("it's strange Btree erase Node");
		if (a == leftSon)
		{
			leftSon = leftSon->right;
		}
		st--;
		a->eraseNode();
	}
	~BTree()
	{
		if (owner != nullptr)
			owner->change_son(nullptr);
		while (leftSon->right != nullptr)
		{
			del(leftSon);
		}
		delete leftSon;
	}
};

template<typename Key>
struct Fibonachi_Heap<Key>::Node
{
	Node* left = nullptr;
	Node* right = nullptr;
	Node() = default;
	Node(const Node&) = delete;
	Node& operator=(const Node&) = delete;
	explicit Node(Node* left) : left(left)
	{
	}
	explicit Node(BTree* Tree)
	{
		changeTree(Tree);
		right = new Node(this);
	}
	explicit Node(Node* left, Node* right, BTree* Tree) : left(left), right(right)
	{
		changeTree(Tree);
	}
	void insertLeft(Node* a)
	{
		a->left = this->left;
		a->right = this;
		if (this->left != nullptr)
		{
			this->left->right = a;
		}
		this->left = a;
	}
	BTree* getTree()
	{
		return Tree;
	}
	void changeTree(BTree* a)
	{
		Tree = a;
		if (Tree != nullptr)
			Tree->prNode = this;
	}
	void eraseNode()
	{
		if (this->right == nullptr)
			throw logic_error("i not have right");
		this->right->left = this->left;
		if (this->left != nullptr)
			this->left->right = this->right;
		this->left = nullptr;
		this->right = nullptr;
	}
	~Node()
	{
		if (Tree != nullptr)
			delete Tree;
	}
private:
	BTree* Tree = nullptr;
};

template<typename Key>
typename Fibonachi_Heap<Key>::Pointer Fibonachi_Heap<Key>::insert(Key T)
{
	typename Fibonachi_Heap<Key>::Pointer p;
	addNode(new Node(nullptr, nullptr, new BTree(T, &p)));
	size++;
	return p;
}

template<typename Key>
void Fibonachi_Heap<Key>::addNode(Fibonachi_Heap::Node* a)
{
	rightSon->insertLeft(a);
	if (leftSon == rightSon)
		leftSon = a;
	updatemin(a);
}

template<typename Key>
void Fibonachi_Heap<Key>::cascadingCut(Fibonachi_Heap::BTree* a)
{
	if (a->pr == nullptr)
		return;
	if (!a->mark)
		a->mark = true;
	else
		cut(a);
}

template<typename Key>
void Fibonachi_Heap<Key>::cut(Fibonachi_Heap::BTree* a)
{
	if (a->pr == nullptr)
		return;

	a->mark = false;
	a->pr->eraseNode(a->prNode);
	BTree* tmp = a->pr;
	addNode(a->prNode);
	a->pr = nullptr;
	cascadingCut(tmp);
}

template<typename Key>
void Fibonachi_Heap<Key>::update(Fibonachi_Heap::Node* a)
{
	while (a->getTree() != nullptr)
	{
		a->getTree()->pr = nullptr;
		a->getTree()->mark = false;
		a = a->right;
	}
}

template<typename Key>
Fibonachi_Heap<Key>::Fibonachi_Heap()
{
	leftSon = new Node();
	min = leftSon;
	rightSon = leftSon;
}

template<typename Key>
Key Fibonachi_Heap<Key>::extractMin()
{
	if (isEmpty())
		throw logic_error("i haven't got elements");

	Key rez = getMin();

	Node* tmp = min;

	eraseNode(tmp);
	update(tmp->getTree()->leftSon);
	merge(tmp->getTree()->leftSon);
	rightSon = tmp->getTree()->rightSon;

	size--;
	consolidate();

	tmp->getTree()->leftSon = new Node();
	delete tmp;

	return rez;
}

template<typename Key>
Key Fibonachi_Heap<Key>::getMin() const
{
	if (isEmpty())
		throw logic_error("i haven't got elements");
	return min->getTree()->data;
}

template<typename Key>
bool Fibonachi_Heap<Key>::isEmpty() const
{
	return size == 0;
}

template<typename Key>
void Fibonachi_Heap<Key>::erase(Fibonachi_Heap::Pointer *Ptr)
{
	if (!(Ptr->haveSon()))
		throw logic_error("i'am empty");
	BTree* tc = Ptr->get_son();
	cut(tc);
	min = tc->prNode;
	extractMin();
}

template<typename Key>
void Fibonachi_Heap<Key>::change(Fibonachi_Heap::Pointer *Ptr, Key T)
{
	if (!(Ptr->haveSon()))
		throw logic_error("i'am empty");

	BTree* tc = Ptr->get_son();

	if (tc->data < T)
		throw  logic_error("you are increase!!!");

	tc->data = T;

	if (tc->pr == nullptr)
		updatemin(tc->prNode);

	if (tc->pr != nullptr && tc->pr->data > tc->data)
		cut(tc);
}

template<typename Key>
void Fibonachi_Heap<Key>::merge(Fibonachi_Heap &otherHeap)
{
	merge(otherHeap.leftSon);
	updatemin(otherHeap.min);
	rightSon = otherHeap.rightSon;
	size += otherHeap.size;

	otherHeap.size = 0;
	otherHeap.leftSon = new Node();
	otherHeap.min = otherHeap.leftSon;
	otherHeap.rightSon = otherHeap.leftSon;
}

template<typename Key>
Fibonachi_Heap<Key>::~Fibonachi_Heap()
{
	while (leftSon->right != nullptr)
	{
		del(leftSon);
	}
	delete leftSon;
}

template<typename Key>
typename Fibonachi_Heap<Key>::BTree * Fibonachi_Heap<Key>::mergeBTree(Fibonachi_Heap::BTree* first, Fibonachi_Heap::BTree* second)
{
	if (second->data < first->data)
		return mergeBTree(second, first);
	Node* tmp = new Node(nullptr, nullptr, second);
	first->rightSon->insertLeft(tmp);
	if (first->leftSon == first->rightSon)
		first->leftSon = tmp;
	first->st++;
	second->pr = first;
	return first;
}

template<typename Key>
void Fibonachi_Heap<Key>::recalcmin()
{
	min = leftSon;
	Node* tc = leftSon;
	while (tc->getTree() != nullptr)
	{
		updatemin(tc);
		tc = tc->right;
	}
}

template<typename Key>
void Fibonachi_Heap<Key>::consolidate()
{
	size_t r = calcmaxdeg();
	Node** st = new Node*[r];
	for (size_t i = 0; i < r; ++i)
		st[i] = nullptr;

	Node* tc = leftSon;
	Node* tmp;
	while (tc->right != nullptr)
	{
		tc = tc->right;

		tmp = tc->left;
		tmp->eraseNode();
		push(st, tmp);
	}
	min = rightSon;
	leftSon = rightSon;
	for (size_t i = 0; i < r; ++i)
	{
		if (st[i] == nullptr)
			continue;
		addNode(st[i]);
	}
	delete[] st;
}

template<typename Key>
void Fibonachi_Heap<Key>::merge(Fibonachi_Heap::Node* a)
{
	Node* tmp = rightSon;
	rightSon = a;
	if (tmp->left != nullptr)
		tmp->left->right = a;
	else
		leftSon = a;
	a->left = tmp->left;
	delete tmp;
}

template<typename Key>
void Fibonachi_Heap<Key>::updatemin(Fibonachi_Heap::Node* other)
{
	if (other->getTree() == nullptr)
		return;
	if (min->getTree() == nullptr || min->getTree()->data > other->getTree()->data)
		min = other;
}

template<typename Key>
void Fibonachi_Heap<Key>::del(Fibonachi_Heap::Node* &a)
{
	if (a->right == nullptr)
		throw logic_error("i have no right");
	a = a->right;
	delete a->left;
}

template<typename Key>
void Fibonachi_Heap<Key>::push(Fibonachi_Heap::Node** data, Fibonachi_Heap::Node* a)
{
	if (data[a->getTree()->st] == nullptr)
		data[a->getTree()->st] = a;
	else
	{
		Node* tmp = data[a->getTree()->st];
		data[a->getTree()->st] = nullptr;

		BTree* c = mergeBTree(tmp->getTree(), a->getTree());
		a->changeTree(c);

		tmp->changeTree(nullptr);
		delete tmp;

		push(data, a);
	}
}

template<typename Key>
size_t Fibonachi_Heap<Key>::calcmaxdeg()
{
	size_t x = 0, y = 1, z, n = 1;
	x = 0;
	y = 1;
	while (y < size)
	{
		z = x + y;
		x = y;
		y = z;
		n++;
	}
	return n + 10;
}

template<typename Key>
void Fibonachi_Heap<Key>::eraseNode(Fibonachi_Heap::Node *a)
{
	if (a == leftSon)
	{
		leftSon = leftSon->right;
	}
	a->eraseNode();
}