#define BOOST_TEST_MODULE testall_heap
#include <boost/test/included/unit_test.hpp>
#include <ctime>
#include "../Fibonachi_Heap/Fibonachi_Heap.h"
#include "../Binomial_Heap/Binomial_Heap.h"
#include "../Heap/Heap.h"

long long A = 39572931, B = 39581212, C = 9534028, M = 42424243;
int X = 17, Y = 19;
clock_t start, end;

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

template<size_t N>
size_t rnd()
{
	return rnd<size_t>() % N;
}

void resetrand(int x = 17, int y = 19)
{
	X = x;
	Y = y;
}

BOOST_AUTO_TEST_SUITE(RandomEachToEach)

template<typename Type, template<typename> typename Heap1, template<typename> typename Heap2>
void check(bool EmpGetmin = false, size_t chI = 60, size_t chEx = 20, size_t chC = 50, size_t chEr = 20, size_t iter = 100000)
{
	Heap1<Type> h;
	Heap2<Type> g;
	Vector<typename Heap1<Type>::Pointer*> p1;
	Vector<typename Heap2<Type>::Pointer*> p2;
	size_t sz = 0;
	for (size_t i = 0; i < iter; ++i)
	{
		if (EmpGetmin)
		{
			BOOST_CHECK_EQUAL(h.isEmpty(), g.isEmpty()); // isEmpty

			if (sz != 0) //getMin
				BOOST_CHECK_EQUAL(h.getMin(), g.getMin());
		}

		if (rnd<100>() < chI) // check insert
		{
			sz++;
			Type tmp = rnd<Type>();
			p1.push_back(new typename Heap1<Type>::Pointer(h.insert(tmp)));
			p2.push_back(new typename Heap2<Type>::Pointer(g.insert(tmp)));
		}

		if (sz != 0 && rnd<100>() < chEx) // check extract
		{
			BOOST_CHECK_EQUAL(h.extractMin(), g.extractMin());
			sz--;
		}

		if (sz != 0 && rnd<100>() < chC) // check change
		{
			size_t id = (size_t)(rnd()) % p1.size();
			while (!p1[id]->haveSon())
			{
				id = (size_t)(rnd()) % p1.size();
			}
			Type tmp = rnd<Type>();
			if (tmp > p1[id]->get_value())
			{
				tmp = p1[id]->get_value();
			}
			h.change(p1[id], tmp);
			g.change(p2[id], tmp);
		}

		if (sz != 0 && rnd<100>() < chEr) // check erase
		{
			size_t id = (size_t)(rnd()) % p1.size();
			if (sz != 0)
			{
				while (!p1[id]->haveSon())
				{
					id = (size_t)(rnd()) % p1.size();
				}
				h.erase(p1[id]);
				g.erase(p2[id]);
				sz--;
			}
		}
	}
	for (size_t i = 0; i < sz; ++i)
	{
		BOOST_CHECK_EQUAL(h.isEmpty(), g.isEmpty());
		BOOST_CHECK_EQUAL(h.extractMin(), g.extractMin());
	}
	BOOST_CHECK_EQUAL(h.isEmpty(), g.isEmpty());
	for (size_t i = 0; i < p1.size(); i++)
		delete p1[i];

	for (size_t i = 0; i < p2.size(); i++)
		delete p2[i];
}

template<template<typename> typename Heap1, template<typename> typename Heap2> 
void Test(size_t iter)
{
	check<int, Heap1, Heap2>(true);
	check<double, Heap1, Heap2>(true);
	check<int, Heap1, Heap2>(false, 60, 20, 50, 20, iter);
	check<int, Heap1, Heap2>(false, 80, 20, 50, 20, iter);
}

BOOST_AUTO_TEST_CASE(HeapAndBinomial_Heap)
{
	Test<Heap, Binomial_Heap>(300000);
}

BOOST_AUTO_TEST_CASE(HeapAndFibonachi_Heap)
{
	Test<Heap, Fibonachi_Heap>(500000);
}

BOOST_AUTO_TEST_CASE(BinomialHeapAndFibonachi_Heap)
{
	Test<Binomial_Heap, Fibonachi_Heap>(200000);
}

BOOST_AUTO_TEST_SUITE_END()