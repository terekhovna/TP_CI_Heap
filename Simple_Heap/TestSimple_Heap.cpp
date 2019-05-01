#define BOOST_TEST_MODULE testsimple_heap
#include <boost/test/included/unit_test.hpp>
#include "Simple_Heap.h"

long long A = 39572931, B = 39581212, C = 9534028, M = 42424243;
int X = 17, Y = 19;

int rnd(long long a, long long b = 0)
{
	return int((A * a + B * b + C) % M);
}

template<typename T = int>
T rnd()
{
	int Z = rnd(X, Y);
	X = Y;
	Y = Z;
	return Y;
}

template<>
double rnd()
{
	int Z = rnd(X, Y);
	X = Y;
	Y = Z;
	return Y / 100.;
}

BOOST_AUTO_TEST_SUITE(TestSimpleHeap);

BOOST_AUTO_TEST_CASE(littleRandom1) //n = 9
{
	Simple_Heap<int> h;
	h.insert(-2);
	h.insert(-12);
	h.insert(18);
	h.insert(18);
	h.insert(8);
	h.insert(-2);
	BOOST_CHECK_EQUAL(h.getMin(), -12);
	BOOST_CHECK_EQUAL(h.extractMin(), -12);
	h.insert(-12);
	h.insert(-2);
	h.insert(-2);
	BOOST_CHECK_EQUAL(h.getMin(), -12);
	BOOST_CHECK_EQUAL(h.extractMin(), -12);
	BOOST_CHECK_EQUAL(h.getMin(), -2);
	BOOST_CHECK_EQUAL(h.extractMin(), -2);
	BOOST_CHECK_EQUAL(h.getMin(), -2);
	BOOST_CHECK_EQUAL(h.extractMin(), -2);
	BOOST_CHECK_EQUAL(h.getMin(), -2);
	BOOST_CHECK_EQUAL(h.extractMin(), -2);
	BOOST_CHECK_EQUAL(h.getMin(), -2);
	BOOST_CHECK_EQUAL(h.extractMin(), -2);
	BOOST_CHECK_EQUAL(h.getMin(), 8);
	BOOST_CHECK_EQUAL(h.extractMin(), 8);
	BOOST_CHECK_EQUAL(h.getMin(), 18);
	BOOST_CHECK_EQUAL(h.extractMin(), 18);
	BOOST_CHECK_EQUAL(h.getMin(), 18);
	BOOST_CHECK_EQUAL(h.extractMin(), 18);
	BOOST_CHECK_EQUAL(h.isEmpty(), true);
}

BOOST_AUTO_TEST_CASE(littleRandom2) //n = 5
{
	Simple_Heap<int> h;
	h.insert(28);
	h.insert(18);
	BOOST_CHECK_EQUAL(h.getMin(), 18);
	BOOST_CHECK_EQUAL(h.extractMin(), 18);
	BOOST_CHECK_EQUAL(h.getMin(), 28);
	BOOST_CHECK_EQUAL(h.extractMin(), 28);
	BOOST_CHECK_THROW(h.extractMin(), logic_error);
	h.insert(-12);
	h.insert(8);
	BOOST_CHECK_EQUAL(h.getMin(), -12);
	BOOST_CHECK_EQUAL(h.extractMin(), -12);
	BOOST_CHECK_EQUAL(h.getMin(), 8);
	BOOST_CHECK_EQUAL(h.extractMin(), 8);
	h.insert(-12);
	BOOST_CHECK_EQUAL(h.getMin(), -12);
	BOOST_CHECK_EQUAL(h.extractMin(), -12);
	BOOST_CHECK_EQUAL(h.isEmpty(), true);
}

BOOST_AUTO_TEST_CASE(littleRandom3) //n = 8
{
	Simple_Heap<int> h;
	h.insert(-2);
	h.insert(-12);
	h.insert(18);
	BOOST_CHECK_EQUAL(h.getMin(), -12);
	BOOST_CHECK_EQUAL(h.extractMin(), -12);
	BOOST_CHECK_EQUAL(h.getMin(), -2);
	BOOST_CHECK_EQUAL(h.extractMin(), -2);
	BOOST_CHECK_EQUAL(h.getMin(), 18);
	BOOST_CHECK_EQUAL(h.extractMin(), 18);
	BOOST_CHECK_THROW(h.extractMin(), logic_error);
	h.insert(8);
	h.insert(8);
	BOOST_CHECK_EQUAL(h.getMin(), 8);
	BOOST_CHECK_EQUAL(h.extractMin(), 8);
	h.insert(-2);
	h.insert(-12);
	h.insert(8);
	BOOST_CHECK_EQUAL(h.getMin(), -12);
	BOOST_CHECK_EQUAL(h.extractMin(), -12);
	BOOST_CHECK_EQUAL(h.getMin(), -2);
	BOOST_CHECK_EQUAL(h.extractMin(), -2);
	BOOST_CHECK_EQUAL(h.getMin(), 8);
	BOOST_CHECK_EQUAL(h.extractMin(), 8);
	BOOST_CHECK_EQUAL(h.getMin(), 8);
	BOOST_CHECK_EQUAL(h.extractMin(), 8);
	BOOST_CHECK_EQUAL(h.isEmpty(), true);
}

BOOST_AUTO_TEST_CASE(Test_erase)
{
	int *r = new int[10];
	bool *used = new bool[10];
	for (int iter = 0; iter < 100; ++iter)
	{
		Simple_Heap<int> h;
		for (int i = 0; i < 10; ++i)
		{
			r[i] = rnd();
			used[i] = false;
			h.insert(r[i]);
		}
		int ud = (rnd<size_t>()) % 10;
		used[ud] = true;
		h.erase(r[ud]);
		for (int i = 0; i < 9; ++i)
		{
			int tc = h.extractMin();
			bool f = false;
			for (int j = 0; j < 10; ++j)
			{
				if (r[j] == tc && !used[j])
				{
					used[j] = true;
					f = true;
					break;
				}
			}
			BOOST_CHECK_EQUAL(f, true);
		}
		BOOST_CHECK_THROW(h.erase(rnd()), logic_error);
	}
	delete[] r;
	delete[] used;
}

BOOST_AUTO_TEST_CASE(Test_change)
{
	int *r = new int[10];
	bool *used = new bool[10];
	for (int iter = 0; iter < 100; ++iter)
	{
		Simple_Heap<int> h;
		for (int i = 0; i < 10; ++i)
		{
			r[i] = rnd();
			used[i] = false;
			h.insert(r[i]);
		}
		int ud = (rnd<size_t>()) % 10;
		int tmp = r[ud];
		r[ud] = rnd();
		h.change(tmp, r[ud]);
		for (int i = 0; i < 10; ++i)
		{
			int tc = h.extractMin();
			bool f = false;
			for (int j = 0; j < 10; ++j)
			{
				if (r[j] == tc && !used[j])
				{
					used[j] = true;
					f = true;
					break;
				}
			}
			BOOST_CHECK_EQUAL(f, true);
		}
		BOOST_CHECK_THROW(h.change(rnd(), rnd()), logic_error);
	}
	delete[] r;
	delete[] used;
}

BOOST_AUTO_TEST_CASE(Test_constructor)
{
	int *r = new int[100];
	bool *used = new bool[100];
	for (int iter = 0; iter < 30; ++iter)
	{
		for (int i = 0; i < 100; ++i)
		{
			r[i] = rnd();
			used[i] = false;
		}
		int fn = (rnd<size_t>()) % 50 + 50;
		Simple_Heap<int> h(r, r + fn);
		for (int i = 0; i < fn; ++i)
		{
			int tc = h.extractMin();
			bool f = false;
			for (int j = 0; j < fn; ++j)
			{
				if (r[j] == tc && !used[j])
				{
					used[j] = true;
					f = true;
					break;
				}
			}
			BOOST_CHECK_EQUAL(f, true);
		}
		BOOST_CHECK_EQUAL(h.isEmpty(), true);
	}
	delete[] r;
	delete[] used;
}

BOOST_AUTO_TEST_CASE(Test_template) //n = 10
{
	Simple_Heap<double> h;
	BOOST_CHECK_THROW(h.extractMin(), logic_error);
	h.insert(4.81);
	BOOST_CHECK_EQUAL(h.extractMin(), 4.81);
	h.insert(-1.45);
	h.insert(0.43);
	BOOST_CHECK_EQUAL(h.extractMin(), -1.45);
	h.insert(-0.97);
	h.insert(-1.43);
	h.insert(7.01);
	h.insert(6.95);
	BOOST_CHECK_EQUAL(h.extractMin(), -1.43);
	BOOST_CHECK_EQUAL(h.extractMin(), -0.97);
	h.insert(6.86);
	BOOST_CHECK_EQUAL(h.extractMin(), 0.43);
	h.insert(2.86);
	h.insert(-2.36);
	BOOST_CHECK_EQUAL(h.extractMin(), -2.36);
	BOOST_CHECK_EQUAL(h.extractMin(), 2.86);
	BOOST_CHECK_EQUAL(h.extractMin(), 6.86);
	BOOST_CHECK_EQUAL(h.extractMin(), 6.95);
	BOOST_CHECK_EQUAL(h.extractMin(), 7.01);
	BOOST_CHECK_EQUAL(h.isEmpty(), true);
}

BOOST_AUTO_TEST_SUITE_END();