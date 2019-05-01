#pragma once
#include "../Vector/Vector.h"
#include<stdexcept>

using std::logic_error;

template<typename Key>
class Heap
{
public:
	class Pointer;

	Heap() = default;
	Pointer insert(Key key);
	Key extractMin();
	bool isEmpty() const;
	Key getMin() const;
	void erase(Pointer *Ptr);
	void change(Pointer *Ptr, Key key);
	void optimaze(size_t insertCount, size_t extractCount);

	template<typename Iterator>
	Heap(Iterator begin, Iterator end);
private:
	struct Vertex;

	size_t K = 2;
	Vector<Vertex> data;

	void shiftUp(size_t num);
	void shiftDown(size_t num);

	template<typename Type>
	static void swap(Type& first, Type& second);

	static void swapVertex(Vertex& first, Vertex& second);
};

template<typename Key>
class Heap<Key>::Pointer
{
	friend Heap<Key>;
public:
	Pointer(const Pointer&) = delete;
	Pointer(Pointer&& right) noexcept
	{
		son = right.son;
		son->owner = this;
	}
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
private:
	explicit Pointer(Vertex *son_) :son(son_)
	{
		son_->owner = this;
	}
	inline size_t get_num() const
	{
		if (!haveSon())
			throw logic_error("i haven't got son!!!");
		return son->num;
	}
	void change_son(Vertex * son_)
	{
		son = son_;
	}
	Vertex* son = nullptr;
};

template<typename Key>
struct Heap<Key>::Vertex
{
	Key value;
	Pointer *owner = nullptr;
	size_t num = -1;
	explicit Vertex(Key value_ = 0, size_t num_ = -1) : value(value_), num(num_)
	{
	}
	void erase()
	{
		if (owner != nullptr)
		{
			owner->change_son(nullptr);
			owner = nullptr;
		}
	}
	Vertex(const Vertex& right)
	{
		*this = right;
	}
	Vertex& operator=(const Vertex& right)
	{
		if (this == &right)
			return *this;
		if (right.owner != nullptr)
			right.owner->change_son(this);
		owner = right.owner;
		num = right.num;
		value = right.value;
		return *this;
	}
};

template<typename Key>
typename Heap<Key>::Pointer Heap<Key>::insert(Key key)
{
	data.push_back(Vertex(key, data.size()));
	Pointer P(&data[data.size() - 1]);
	shiftUp(data.size() - 1);
	return P;
}

template<typename Key>
Key Heap<Key>::getMin() const
{
	if (isEmpty())
		throw logic_error("Heap is Empty!!!");
	return data[0].value;
}

template<typename Key>
Key Heap<Key>::extractMin()
{
	if (isEmpty())
		throw logic_error("Heap is Empty!!!");
	Key rez = getMin();
	swapVertex(data[0], data[data.size() - 1]);
	data[data.size() - 1].erase();
	data.pop_back();
	shiftDown(0);
	return rez;
}

template<typename Key>
bool Heap<Key>::isEmpty() const
{
	return data.isEmpty();
}

template<typename Key>
template<typename Iterator>
Heap<Key>::Heap(Iterator begin, Iterator end)
{
	for (; begin != end; ++begin)
	{
		data.push_back(Vertex(*begin, data.size()));
	}
	for (int i = data.size() - 1; i >= 0; i--)
	{
		shiftDown(i);
	}
}

template<typename Key>
void Heap<Key>::optimaze(size_t insertCount, size_t extractCount)
{
	if (!isEmpty())
		throw logic_error("Heap is not Empty!!!");
	if (extractCount == 0)
		extractCount = 1;
	if ((insertCount / extractCount) > 2)
		K = insertCount / extractCount;
	else
		K = 2;
}

template<typename Key>
void Heap<Key>::shiftUp(size_t num)
{
	while (num != 0 && data[num].value < data[(num - 1) / K].value)
	{
		swapVertex(data[num], data[(num - 1) / K]);
		num = (num - 1) / K;
	}
}

template<typename Key>
void Heap<Key>::shiftDown(size_t num)
{
	while (true)
	{
		size_t min = num;
		for (size_t son = K * num + 1; (son <= K * num + K) && (son < data.size()); ++son)
		{
			if (data[son].value < data[min].value)
				min = son;
		}
		if (min == num)
			break;
		swapVertex(data[num], data[min]);
		num = min;
	}
}

template<typename Key>
void Heap<Key>::erase(Heap::Pointer *Ptr)
{
	size_t ver = Ptr->get_num();
	while (ver != 0)
	{
		swapVertex(data[ver], data[(ver - 1) / K]);
		ver = (ver - 1) / K;
	}
	extractMin();
}

template<typename Key>
void Heap<Key>::change(Heap::Pointer *Ptr, Key key)
{
	data[Ptr->get_num()].value = key;
	shiftUp(Ptr->get_num());
	shiftDown(Ptr->get_num());
}


template<typename Key>
template<typename Type>
void Heap<Key>::swap(Type &first, Type &second)
{
	Type tmp = first;
	first = second;
	second = tmp;
}

template<typename Key>
void Heap<Key>::swapVertex(Heap::Vertex &first, Heap::Vertex &second)
{
	swap(first, second);
	swap(first.num, second.num);
}
