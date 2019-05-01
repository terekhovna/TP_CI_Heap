#pragma once
#include "../Vector/Vector.h"

using std::logic_error;
using std::out_of_range;

template<class Key>
class Simple_Heap
{
public:
	void insert(Key key);
	Key extractMin();
	bool isEmpty() const;
	Key getMin() const;
	void erase(Key Ptr);
	void change(Key Ptr, Key key);

	Simple_Heap(Vector<Key> value);
	Simple_Heap();
	template<class Iterator>
	Simple_Heap(Iterator begin, Iterator end);
private:
	size_t find(Key key) const;
	Vector<Key> data;
};


template<class Key>
Simple_Heap<Key>::Simple_Heap() : data(10000)
{

}

template<class Key>
Simple_Heap<Key>::Simple_Heap(Vector<Key> value) : data(10000)
{
	for (int i = 0; i < value.size(); ++i)
		data.push_back(value[i]);
}

template<class Key>
template<class Iterator>
Simple_Heap<Key>::Simple_Heap(Iterator begin, Iterator end) : data(10000)
{
	for (; begin != end; ++begin)
	{
		data.push_back(*begin);
	}
}

template<class Key>
void Simple_Heap<Key>::insert(Key key)
{
	if (data.size() == data.getcapacity())
		throw out_of_range("So much!!!");
	data.push_back(key);
}

template<class Key>
bool Simple_Heap<Key>::isEmpty() const
{
	return data.isEmpty();
}

template<class Key>
Key Simple_Heap<Key>::getMin() const
{
	if (isEmpty())
		throw logic_error("Simple_Heap is Empty!!!");
	size_t min = 0;
	for (size_t i = 1; i < data.size(); ++i)
		if (data[min] > data[i])
			min = i;
	return data[min];
}

template<class Key>
Key Simple_Heap<Key>::extractMin()
{
	if (isEmpty())
		throw logic_error("Simple_Heap is Empty!!!");
	size_t min = 0;
	for (size_t i = 1; i < data.size(); ++i)
		if (data[min] > data[i])
			min = i;
	Key answer = data[min];
	Key tmp = data[min];
	data[min] = data[data.size() - 1];
	data[data.size() - 1] = tmp;
	data.pop_back();
	return answer;
}

template<class Key>
void Simple_Heap<Key>::erase(Key key)
{
	size_t Ptr = find(key);
	if (Ptr >= data.size())
		throw logic_error("Didn't found!!!");
	Key tmp = data[Ptr];
	data[Ptr] = data[data.size() - 1];
	data[data.size() - 1] = Ptr;
	data.pop_back();
}

template<class Key>
void Simple_Heap<Key>::change(Key key, Key value)
{
	size_t Ptr = find(key);
	if (Ptr >= data.size())
		throw logic_error("Didn't found!!!");
	data[Ptr] = value;
}

template<class Key>
size_t Simple_Heap<Key>::find(Key key) const
{
	for (size_t i = 0; i < data.size(); ++i)
		if (data[i] == key)
			return i;
	return data.size();
}