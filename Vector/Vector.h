#pragma once
#include<stdexcept>

using std::logic_error;
using std::out_of_range;

template<class Key>
class Vector
{
public:
	void push_back(Key key);
	void pop_back();
	size_t size() const;
	bool isEmpty() const;
	size_t getcapacity() const;
	void resize(size_t new_size);

	Vector() = default;
	Vector(const Vector&) = delete;
	Vector(const Vector&&) = delete;
	Vector& operator=(const Vector&) = delete;
	explicit Vector(size_t size_);
	Key& operator [](size_t num);
	const Key& operator [](size_t num) const;
	~Vector();
private:
	Key* data = nullptr;
	size_t last = 0;
	size_t capacity = 0;
};

template<class Key>
void Vector<Key>::push_back(Key key)
{
	if (capacity <= last)
		resize((last + 1) * 2);
	data[last] = key;
	++last;
}

template<class Key>
void Vector<Key>::pop_back()
{
	if (size() == 0)
		throw logic_error("Vector is Empty!");
	--last;
}

template<class Key>
size_t Vector<Key>::size() const
{
	return last;
}

template<class Key>
size_t Vector<Key>::getcapacity() const
{
	return capacity;
}

template<class Key>
bool Vector<Key>::isEmpty() const
{
	return size() == 0;
}

template<class Key>
void Vector<Key>::resize(size_t new_size)
{
	Key* new_data;
	if (new_size == 0)
		new_data = nullptr;
	else
		new_data = new Key[new_size];
	capacity = new_size;
	if (last > capacity)
		last = capacity;
	for (size_t i = 0; i < last; ++i)
	{
		new_data[i] = data[i];
	}
	if(data != nullptr)
		delete[] data;
	data = new_data;
}

template<class Key>
Vector<Key>::Vector(size_t size_)
{
	resize(size_);
}

template<class Key>
Key& Vector<Key>::operator [](size_t num)
{
	if (0 > num || num >= size())
		throw out_of_range("Out of Range!!!");
	return data[num];
}

template<class Key>
const Key& Vector<Key>::operator [](size_t num) const
{
	if (0 > num || num >= size())
		throw out_of_range("Out of Range!!!");
	return data[num];
};

template<class Key>
Vector<Key>::~Vector()
{
	if (data != nullptr)
		delete[] data;
}