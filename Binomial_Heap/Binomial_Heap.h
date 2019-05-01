#pragma once
#include<stdexcept>

using std::logic_error;

template<typename Key>
class Binomial_Heap
{
public:
	class Pointer;

	Pointer insert(Key T);
	Binomial_Heap();
	Key extractMin();
	bool isEmpty() const;
	Key getMin() const;
	void erase(Pointer *pointer);
	void change(Pointer *pointer, Key T);
	void merge(Binomial_Heap &otherHeap);
	Binomial_Heap(const Binomial_Heap&) = delete;
	Binomial_Heap& operator=(const Binomial_Heap&) = delete;
	~Binomial_Heap();
private:
	struct Node;
	struct Binomial_Tree;

	explicit Binomial_Heap(Node* a);
	Binomial_Tree * mergeBTree(Binomial_Tree* first, Binomial_Tree* second);
	void swapBTree(Binomial_Tree* first, Binomial_Tree *second);
	void recalculateMin();
	void merge(Binomial_Heap *otherHeap);
	Node* merge(Node* first, Node* second);
	void erase(Node *a);

	static void goNext(Node* &a);
	static void deleteNode(Node* a);
	static void deleteAndStep(Node* &a);

	Node* min = nullptr;
	Node* leftSon = nullptr;
};

template<typename Key>
class Binomial_Heap<Key>::Pointer
{
	friend Binomial_Heap;
public:
	Pointer(Pointer&& right) noexcept
	{
		change_son(right.son);
		right.son = nullptr;
	}
	Pointer(const Pointer&) = delete;
	Pointer& operator=(const Pointer&) = delete;
	inline Key get_value() const
	{
		if (!haveSon())
			throw logic_error("i haven't got son!!!");
		return son->value;
	}
	bool haveSon() const
	{
		return son != nullptr;
	}
	~Pointer()
	{
		if (haveSon())
			son->owner = nullptr;
	}
private:
	explicit Pointer(Binomial_Tree *son_ = nullptr)
	{
		change_son(son_);
	}
	inline Binomial_Tree* get_son() const
	{
		if (!haveSon())
			throw logic_error("i haven't got son!!!");
		return son;
	}
	void change_son(Binomial_Tree* son_)
	{
		son = son_;
		if (son != nullptr)
			son->owner = this;
	}
	Binomial_Tree* son = nullptr;
};

template<typename Key>
struct Binomial_Heap<Key>::Binomial_Tree
{
	size_t degree = 0;
	Binomial_Tree* parent = nullptr;
	Node* leftSon = nullptr;
	Node* rightSon = nullptr;
	Node* parentNode = nullptr;
	Key value;
	Pointer *owner;
	Binomial_Tree(Key T, Pointer* pointer) : value(T), owner(pointer)
	{
		pointer->change_son(this);
		leftSon = new Node();
		rightSon = leftSon;
	}
	Binomial_Tree(const Binomial_Tree&) = delete;
	Binomial_Tree& operator=(const Binomial_Tree&) = delete;
	~Binomial_Tree()
	{
		if (owner != nullptr)
			owner->change_son(nullptr);
		while (leftSon->right != nullptr)
		{
			deleteAndStep(leftSon);
		}
		delete leftSon;
	}
};

template<typename Key>
struct Binomial_Heap<Key>::Node
{
	Node* left = nullptr;
	Node* right = nullptr;
	Node() = default;
	Node(const Node&) = delete;
	Node& operator=(const Node&) = delete;
	explicit Node(Node* left) : left(left)
	{
	}
	explicit Node(Binomial_Tree* Tree)
	{
		changeTree(Tree);
		right = new Node(this);
	}
	explicit Node(Node* left, Node* right, Binomial_Tree* Tree) : left(left), right(right)
	{
		changeTree(Tree);
	}
	void insertLeft(Binomial_Tree* a)
	{
		Node *t = new Node(left, this, a);
		if (left != nullptr)
		{
			left->right = t;
		}
		left = t;
	}
	Binomial_Tree* getTree()
	{
		return Tree;
	}
	void changeTree(Binomial_Tree* a)
	{
		Tree = a;
		if (Tree != nullptr)
			Tree->parentNode = this;
	}
	void erase()
	{
		if (right == nullptr)
			throw logic_error("i not have right");
		right->left = left;
		if (left != nullptr)
			left->right = right;
	}

	~Node()
	{
		if (Tree != nullptr)
			delete Tree;
	}
private:
	Binomial_Tree* Tree = nullptr;
};

template<typename Key>
typename Binomial_Heap<Key>::Pointer Binomial_Heap<Key>::insert(Key T)
{
	typename Binomial_Heap<Key>::Pointer p;
	merge(new Binomial_Heap(new Node(new Binomial_Tree(T, &p))));
	return p;
}

template<typename Key>
Binomial_Heap<Key>::Binomial_Heap()
{
	leftSon = new Node();
	min = leftSon;
}

template<typename Key>
Key Binomial_Heap<Key>::extractMin()
{
	if (isEmpty())
		throw logic_error("i haven't got elements");
	Key rez = getMin();

	Node* tmp = min;
	erase(min);

	merge(new Binomial_Heap(min->getTree()->leftSon));

	tmp->getTree()->leftSon = new Node();
	delete tmp;

	return rez;
}

template<typename Key>
Key Binomial_Heap<Key>::getMin() const
{
	if (isEmpty())
		throw logic_error("i haven't got elements");
	return min->getTree()->value;
}

template<typename Key>
bool Binomial_Heap<Key>::isEmpty() const
{
	return min->getTree() == nullptr;
}

template<typename Key>
void Binomial_Heap<Key>::erase(Binomial_Heap::Pointer *pointer)
{
	if (!(pointer->haveSon()))
		throw logic_error("i'am empty");

	Binomial_Tree* tmp = pointer->get_son();
	while (tmp->parent != nullptr)
	{
		swapBTree(tmp->parent, tmp);
		tmp = tmp->parent;
	}
	min = tmp->parentNode;
	extractMin();
}

template<typename Key>
void Binomial_Heap<Key>::change(Binomial_Heap::Pointer *pointer, Key T)
{
	if (!(pointer->haveSon()))
		throw logic_error("i'am empty");

	Binomial_Tree* tmp = pointer->get_son();
	if (tmp->value < T)
		throw logic_error("you are increase!!!");
	tmp->value = T;
	while (tmp->parent != nullptr)
	{
		if (tmp->parent->value <= tmp->value)
			break;
		swapBTree(tmp->parent, tmp);
		tmp = tmp->parent;
	}
	if (tmp->parent == nullptr)
		recalculateMin();
}

template<typename Key>
void Binomial_Heap<Key>::merge(Binomial_Heap &otherHeap)
{
	leftSon = merge(leftSon, otherHeap.leftSon);
	while (leftSon->left != nullptr)
		leftSon = leftSon->left;

	otherHeap.leftSon = new Node();
	otherHeap.min = otherHeap.leftSon;

	recalculateMin();
}

template<typename Key>
Binomial_Heap<Key>::~Binomial_Heap()
{
	while (leftSon->right != nullptr)
	{
		deleteAndStep(leftSon);
	}
	delete leftSon;
}

template<typename Key>
typename Binomial_Heap<Key>::Binomial_Tree * Binomial_Heap<Key>::mergeBTree(Binomial_Heap::Binomial_Tree* first, Binomial_Heap::Binomial_Tree* second)
{
	if (second->value < first->value)
		return mergeBTree(second, first);

	first->rightSon->insertLeft(second);

	if (first->leftSon == first->rightSon)
		first->leftSon = first->leftSon->left;

	first->degree++;
	second->parent = first;
	return first;
}

template<typename Key>
void Binomial_Heap<Key>::swapBTree(Binomial_Heap::Binomial_Tree* first, Binomial_Heap::Binomial_Tree *second)
{
	Key tmp = first->value;
	first->value = second->value;
	second->value = tmp;

	Pointer* tmp2 = first->owner;
	if (second->owner != nullptr)
		second->owner->change_son(first);
	if (tmp2 != nullptr)
		tmp2->change_son(second);
}

template<typename Key>
void Binomial_Heap<Key>::recalculateMin()
{
	min = leftSon;
	Node* tc = leftSon;
	while (tc->getTree() != nullptr)
	{
		if (min->getTree() == nullptr || min->getTree()->value > tc->getTree()->value)
			min = tc;
		tc = tc->right;
	}
}

template<typename Key>
void Binomial_Heap<Key>::merge(Binomial_Heap<Key> *otherHeap)
{
	merge(*otherHeap);
	delete otherHeap;
}

template<typename Key>
Binomial_Heap<Key>::Binomial_Heap(Binomial_Heap::Node* a)
{
	leftSon = a;
	recalculateMin();
}

template<typename Key>
typename Binomial_Heap<Key>::Node* Binomial_Heap<Key>::merge(Binomial_Heap::Node* first, Binomial_Heap::Node* second)
{
	while (second->getTree() != nullptr)
	{
		second->getTree()->parent = nullptr;
		if (first->getTree() == nullptr || 
			first->getTree()->degree > second->getTree()->degree)
		{
			first->insertLeft(second->getTree());

			goNext(second);
		}
		else if (first->getTree()->degree < second->getTree()->degree)
		{
			first = first->right;
		}
		else
		{
			Node* tmp = new Node(mergeBTree(first->getTree(), second->getTree()));
			
			goNext(first);
			first = merge(first, tmp);

			goNext(second);
		}
	}
	delete second;
	return first;
}

template<typename Key>
void Binomial_Heap<Key>::erase(Binomial_Heap::Node *a)
{
	if (a == leftSon)
	{
		leftSon = leftSon->right;
	}
	a->erase();
}

template<typename Key>
void Binomial_Heap<Key>::deleteNode(Binomial_Heap::Node* a)
{
	a->erase();
	delete a;
}

template<typename Key>
void Binomial_Heap<Key>::deleteAndStep(Binomial_Heap::Node* &a)
{
	if (a->right == nullptr)
		throw logic_error("i have no right");
	a = a->right;

	deleteNode(a->left);
}

template<typename Key>
void Binomial_Heap<Key>::goNext(Binomial_Heap::Node* &a)
{
	a->changeTree(nullptr);
	deleteAndStep(a);
}
