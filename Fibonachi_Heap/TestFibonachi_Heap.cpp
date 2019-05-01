#define BOOST_TEST_MODULE testbinomial_heap
#include <boost/test/included/unit_test.hpp>
#include <ctime>
#include "Fibonachi_Heap.h"
#include "../Simple_Heap/Simple_Heap.h"

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

template<typename T>
using Heap = Fibonachi_Heap<T>;

BOOST_AUTO_TEST_SUITE(TestHeap)

BOOST_AUTO_TEST_SUITE(TestHeapPointer)

//Here I test Pointer::haveSon
BOOST_AUTO_TEST_CASE(Test_haveSon)
{
	for (size_t iter = 0; iter < 100; ++iter)
	{
		Heap<int> a;
		size_t n = (rnd<size_t>()) % 100;
		for (size_t i = 0; i < n; ++i)
		{
			a.insert(rnd());
		}
		int tc = rnd() % 100;
		Heap<int>::Pointer ch = a.insert(tc);
		size_t sz = 0;
		n = (rnd<size_t>()) % 100;
		bool f = true;
		for (size_t i = 0; i < n; i++)
		{
			BOOST_CHECK_EQUAL(ch.haveSon(), f);
			if (rnd() % 3)
			{
				if (sz)
				{
					if (a.extractMin() == tc)
					{
						f = false;
					}
					--sz;
				}
			}
			else
			{
				sz++;
				int tmp = rnd() % 100;
				while (tmp == tc)
					tmp = rnd() % 100;
				a.insert(tmp);
			}
		}
		BOOST_CHECK_EQUAL(ch.haveSon(), f);
		while (sz != 0)
		{
			BOOST_CHECK_EQUAL(ch.haveSon(), f);
			if (a.extractMin() == tc)
			{
				f = false;
			}
			sz--;
		}
		BOOST_CHECK_EQUAL(ch.haveSon(), f);
	}
}

//Here I test Pointer::get_value using Pointer::haveSon
BOOST_AUTO_TEST_CASE(Test_getvalue)
{
	for (size_t iter = 0; iter < 100; ++iter)
	{
		Heap<int> a;
		size_t n = (rnd<size_t>()) % 100;
		for (size_t i = 0; i < n; ++i)
		{
			a.insert(rnd());
		}
		int tc = rnd();
		Heap<int>::Pointer ch = a.insert(tc);
		size_t sz = 0;
		n = (rnd<size_t>()) % 100;
		for (size_t i = 0; i < n; i++)
		{
			if (rnd() % 3)
			{
				if (sz)
				{
					a.extractMin();
					--sz;
				}
			}
			else
			{
				sz++;
				a.insert(rnd());
			}
		}
		if (ch.haveSon())
			BOOST_CHECK_EQUAL(ch.get_value(), tc);
	}

	for (size_t iter = 0; iter < 100; ++iter)
	{
		Heap<int> a;
		size_t n = (rnd<size_t>()) % 100;
		for (size_t i = 0; i < n; ++i)
		{
			a.insert(rnd());
		}
		int tc = rnd();
		Heap<int>::Pointer ch = a.insert(tc);
		size_t sz = 0;
		n = (rnd<size_t>()) % 100;
		for (size_t i = 0; i < n; i++)
		{
			if (rnd() % 3)
			{
				if (sz)
				{
					a.extractMin();
					--sz;
				}
			}
			else
			{
				sz++;
				a.insert(rnd());
			}
		}
		tc = rnd();
		if (ch.haveSon())
		{
			if (tc > ch.get_value())
			{
				BOOST_CHECK_THROW(a.change(&ch, tc), logic_error);
				tc = ch.get_value();
			}
			a.change(&ch, tc);
		}
		n = (rnd<size_t>()) % 100;
		for (size_t i = 0; i < n; i++)
		{
			if (rnd() % 3)
			{
				if (sz)
				{
					a.extractMin();
					--sz;
				}
			}
			else
			{
				sz++;
				a.insert(rnd());
			}
		}
		if (ch.haveSon())
			BOOST_CHECK_EQUAL(ch.get_value(), tc);
	}
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(Random_with_SimpleHeap)

//Here I test Heap::insert, Heap::getMin
//Heap::extractmin, Heap::isEmpty,
//Heap::change, Heap::erase
//and tempalate
//using Simple_Heap and Pointer::get_value
template<typename Type>
void check(bool EmpGetmin = false, size_t chI = 60, size_t chEx = 20, size_t chC = 50, size_t chEr = 20)
{
	Simple_Heap<Type> g;
	Heap<Type> h;
	Vector<typename Heap<Type>::Pointer*> p;
	size_t sz = 0;
	for (size_t i = 0; i < 50; ++i)
	{
		if (EmpGetmin)
		{
			BOOST_CHECK_EQUAL(h.isEmpty(), g.isEmpty()); // isEmpty

			if (sz != 0) //getMin
				BOOST_CHECK_EQUAL(h.getMin(), g.getMin());
			else
				BOOST_CHECK_THROW(h.getMin(), logic_error);
		}

		if (rnd<100>() < chI) // check insert
		{
			sz++;
			Type tmp = rnd<Type>();
			g.insert(tmp);
			p.push_back(new typename Heap<Type>::Pointer(h.insert(tmp)));
		}

		if (rnd<100>() < chEx) // check extract
		{
			if (sz == 0)
			{
				BOOST_CHECK_THROW(h.extractMin(), logic_error);
			}
			else
			{
				BOOST_CHECK_EQUAL(h.extractMin(), g.extractMin());
				sz--;
			}
		}

		if (!p.isEmpty() && rnd<100>() < chC) // check change
		{
			size_t id = (size_t)(rnd()) % p.size();
			if (sz != 0)
			{
				while (!p[id]->haveSon())
				{
					BOOST_CHECK_THROW(h.change(p[id], rnd()), logic_error);
					id = (size_t)(rnd()) % p.size();
				}
				Type tmp = rnd<Type>();
				if (tmp > p[id]->get_value())
				{
					BOOST_CHECK_THROW(h.change(p[id], tmp), logic_error);
					tmp = p[id]->get_value();
				}
				g.change(p[id]->get_value(), tmp);
				h.change(p[id], tmp);
				BOOST_CHECK_EQUAL(p[id]->get_value(), tmp);
			}
			else
				BOOST_CHECK_THROW(h.change(p[id], rnd()), logic_error);
		}

		if (!p.isEmpty() && rnd<100>() < chEr) // check erase
		{
			size_t id = (size_t)(rnd()) % p.size();
			if (sz != 0)
			{
				while (!p[id]->haveSon())
				{
					BOOST_CHECK_THROW(h.erase(p[id]), logic_error);
					id = (size_t)(rnd()) % p.size();
				}
				g.erase(p[id]->get_value());
				h.erase(p[id]);
				sz--;
			}
			BOOST_CHECK_THROW(h.erase(p[id]), logic_error);
			BOOST_CHECK_EQUAL(p[id]->haveSon(), false);
		}
	}
	for (size_t i = 0; i < sz; ++i)
	{
		BOOST_CHECK_EQUAL(h.isEmpty(), g.isEmpty());
		BOOST_CHECK_EQUAL(h.extractMin(), g.extractMin());
	}
	BOOST_CHECK_THROW(h.extractMin(), logic_error);
	BOOST_CHECK_EQUAL(h.isEmpty(), g.isEmpty());
	for (size_t i = 0; i < p.size(); i++)
		delete p[i];
}

BOOST_AUTO_TEST_CASE(FinalRandomTest)
{
	check<int>(true);
	check<double>(true);
	for (size_t chC = 0; chC <= 100; chC += 20)
	{
		for (size_t chI = 20; chI <= 100; chI += 20)
		{
			check<int>(false, chI, 20, chC, 20);
			check<double>(false, chI, 20, chC, 20);
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()

template<typename T>
void TestMerge(size_t chI = 60, size_t chEx = 20, size_t Ch = 50)
{
	for (size_t iter = 0; iter < 100; iter++)
	{
		Heap<T> h1, h2;
		Simple_Heap<T> g1, g2;
		size_t n = rnd<100>() + 100;
		size_t sz1 = 0, sz2 = 0;
		for (size_t i = 0; i < n; i++)
		{
			T tmp = rnd<T>();
			if (rnd<100>() < chI)
			{
				if (rnd<100>() < Ch)
				{
					sz1++;
					h1.insert(tmp);
					g1.insert(tmp);
				}
				else
				{
					sz2++;
					h2.insert(tmp);
					g2.insert(tmp);
				}
			}

			if (rnd<100>() < chEx)
			{
				if (rnd<100>() < Ch)
				{
					if (sz1)
					{
						sz1--;
						h1.extractMin();
						g1.extractMin();
					}
				}
				else
				{
					if (sz2)
					{
						sz2--;
						h2.extractMin();
						g2.extractMin();
					}
				}
			}
		}
		h1.merge(h2);
		while (!g2.isEmpty())
			g1.insert(g2.extractMin());
		BOOST_CHECK_EQUAL(h2.isEmpty(), true);
		for (size_t i = 0; i < sz1 + sz2; i++)
		{
			BOOST_CHECK_EQUAL(h1.extractMin(), g1.extractMin());
		}
		BOOST_CHECK_EQUAL(h1.isEmpty(), true);
	}
}

BOOST_AUTO_TEST_CASE(Test_Merge)
{
	for (size_t chI = 40; chI <= 80; chI += 20)
		for (size_t chEx = 20; chEx <= 60; chEx += 20)
			for (size_t Ch = 20; Ch <= 80; Ch += 30)
			{
				TestMerge<int>(chI, chEx, Ch);
				TestMerge<double>(chI, chEx, Ch);
			}
}

BOOST_AUTO_TEST_SUITE(TestTime)

void Merge(size_t chI = 60, size_t chEx = 20, size_t Ch = 50)
{
	Heap<int> h1, h2;
	size_t n = rnd<2000000>() + 2000000;
	size_t sz1 = 0, sz2 = 0;
	for (size_t i = 0; i < n; i++)
	{
		int tmp = rnd<int>();
		if (rnd<100>() < chI)
		{
			if (rnd<100>() < Ch)
			{
				sz1++;
				h1.insert(tmp);
			}
			else
			{
				sz2++;
				h2.insert(tmp);
			}
		}

		if (rnd<100>() < chEx)
		{
			if (rnd<100>() < Ch)
			{
				if (sz1)
				{
					sz1--;
					h1.extractMin();
				}
			}
			else
			{
				if (sz2)
				{
					sz2--;
					h2.extractMin();
				}
			}
		}
	}
	start = clock();
	h1.merge(h2);
	end = clock();
	BOOST_TEST_MESSAGE("\nElements in first Heap = " << sz1 << ", in second = " << sz2 << " time is " << (end - start) / (double)CLOCKS_PER_SEC * 1000 << " ms\n");
}

void HardMerge()
{
	const int Kol = 7000, Num = 100;
	Heap<int>* h = new Heap<int>[Kol];
	size_t n = rnd<Num>() + Num;
	size_t sz = Kol * n;
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < Kol; j++)
		{
			h[j].insert(rnd());
		}
	}
	start = clock();
	for (size_t i = 1; i < Kol; i++)
	{
		h[0].merge(h[i]);
	}
	while (!h[0].isEmpty())
	{
		h[0].extractMin();
	}
	end = clock();
	BOOST_TEST_MESSAGE("\nElements in each Heap of " << Kol << " = " << n << " time is " << (end - start) / (double)CLOCKS_PER_SEC * 1000 << " ms\n");
	delete[] h;
}
BOOST_AUTO_TEST_CASE(MergeTime)
{
	Merge(60, 20, 50);
	Merge(60, 20, 70);
	Merge(80, 60, 50);
	Merge(80, 60, 70);
	Merge(60, 40, 30);
	HardMerge();
}

template<typename Type = int>
void checkTime(size_t numOfOper, size_t chI = 60, size_t chEx = 20, size_t chC = 70, size_t chEr = 20)
{
	Heap<Type> h;
	Vector<typename Heap<Type>::Pointer*> p;
	size_t sz = 0;
	size_t ope[] = { 0, 0, 0, 0 };
	size_t opers = 0;
	start = clock();
	while (opers < numOfOper)
	{
		if ((rnd<size_t>()) % 100 < chI) // insert
		{
			ope[0]++;
			opers++;
			sz++;
			if (chC != 0 || chEr != 0)
				p.push_back(new typename Heap<Type>::Pointer(h.insert(rnd<Type>())));
			else
				h.insert(rnd<Type>());
		}

		if ((rnd<size_t>()) % 100 < chEx) // extract
		{
			if (sz != 0)
			{
				ope[1]++;
				opers++;
				h.extractMin();
				sz--;
			}
		}

		if (!p.isEmpty() && (rnd<size_t>()) % 100 < chC) // change
		{
			size_t id = rnd<size_t>() % p.size();
			if (p[id]->haveSon())
			{
				Type tmp = rnd<Type>();
				if (tmp < p[id]->get_value())
				{
					ope[2]++;
					opers++;
					h.change(p[id], tmp);
				}
			}
		}

		if (!p.isEmpty() && (rnd<size_t>()) % 100 < chEr) // erase
		{
			size_t id = rnd<size_t>() % p.size();
			if (p[id]->haveSon())
			{
				ope[3]++;
				opers++;
				h.erase(p[id]);
				sz--;
			}
		}
	}
	end = clock();
	BOOST_TEST_MESSAGE("\n" << "Sum of operations = " << opers
		<< "\n\tInserts : " << ope[0] << "\n\tExtracts: " << ope[1] << "\n\tChanges : " << ope[2] << "\n\tErases  : " << ope[3]
		<< "\ntime is " << (end - start) / (double)CLOCKS_PER_SEC * 1000 << " ms\n");

	for (size_t i = 0; i < p.size(); i++)
		delete p[i];
}

BOOST_AUTO_TEST_CASE(TimeOnRandom)
{
	for (size_t it = 0; it < 3; ++it)
		checkTime(rnd<1000000>() + 1000000);

	for (size_t er = 40; er <= 80; er += 20)
		checkTime(rnd<3000000>() + 3000000, 100, er, 0, 0);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
