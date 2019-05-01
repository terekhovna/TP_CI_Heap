#define BOOST_TEST_MODULE testheap
#include <boost/test/included/unit_test.hpp>
#include <ctime>
#include "Heap.h"
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
			a.change(&ch, tc);
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

//Here I test Heap::insert, Heap::getmin
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

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(TestTime)

BOOST_AUTO_TEST_CASE(constructor)
{
	for (int it = 0; it < 100; it++)
	{
		size_t raz = rnd<size_t>() % 50 + 50;;
		int* r = new int[raz];
		for (size_t i = 0; i < raz; i++)
		{
			r[i] = rnd();
		}
		Heap<int> h(r, r + raz);
		Simple_Heap<int> g(r, r + raz);
		for (size_t i = 0; i < raz; i++)
		{
			BOOST_CHECK_EQUAL(h.extractMin(), g.extractMin());
		}
		BOOST_CHECK_EQUAL(h.isEmpty(), true);
		delete[] r;
	}

	for (size_t it = 0; it < 5; ++it)
	{
		size_t n = rnd<size_t>() % 500000 + 500000;
		int* r = new int[n];
		for (size_t i = 0; i < n; ++i)
		{
			r[i] = rnd();
		}
		start = clock();
		Heap<int> h(r, r + n);
		end = clock();
		BOOST_TEST_MESSAGE("for n = " << n << " time is " << (end - start) / (double)CLOCKS_PER_SEC * 1000 << "ms");
		delete[] r;
	}
	size_t n = 1000000;
	int* r = new int[n];
	for (size_t i = 0; i < n; ++i)
	{
		r[i] = n - i;
	}
	start = clock();
	Heap<int> h(r, r + n);
	end = clock();
	BOOST_TEST_MESSAGE("for the worst case, when n = " << n << " time is " << (end - start) / (double)CLOCKS_PER_SEC * 1000 << "ms");
	delete[] r;
}

template<typename Type = int>
void checkTime(size_t numOfOper, size_t chI = 60, size_t chEx = 20, size_t chC = 50, size_t chEr = 20)
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
				ope[2]++;
				opers++;
				h.change(p[id], rnd<Type>());
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
		checkTime(rnd<2000000>() + 2000000);

	for (size_t er = 40; er <= 80; er += 20)
		checkTime(rnd<5000000>() + 5000000, 100, er, 0, 0);
}

BOOST_AUTO_TEST_CASE(optimaze)
{
	for (int it = 0; it < 100; it++)
	{
		size_t raz = rnd<size_t>() % 50 + 50;
		size_t ud = rnd<size_t>() % raz;
		int* r = new int[raz];
		for (size_t i = 0; i < raz; i++)
		{
			r[i] = rnd();
		}
		Heap<int> h;
		h.optimaze(raz, ud);
		Simple_Heap<int> g;
		size_t kol = 0, re = raz, u = ud, in = 0;
		for (size_t i = 0; i < raz + ud; i++)
		{
			if (re == 0 || (rnd<100>() <= 30 && kol != 0 && u > 0))
			{
				BOOST_CHECK_EQUAL(h.extractMin(), g.extractMin());
				kol--;
				u--;
			}
			else
			{
				h.insert(r[in]);
				g.insert(r[in]);
				in++;
				kol++;
				re--;
			}
		}
		delete[] r;
	}

	for (size_t it = 0; it < 6; ++it)
	{
		size_t n = rnd<size_t>() % 1000000 + 1000000;
		size_t ud = rnd<size_t>() % (n / (1 << (2 * it)) + 1);
		if (it == 5)
		{
			ud = 0;
		}
		int* r = new int[n];
		for (size_t i = 0; i < n; i++)
		{
			r[i] = rnd();
		}
		Heap<int> h;
		size_t kol = 0, re = n, u = ud, in = 0;
		start = clock();
		h.optimaze(n, ud);
		int x = X, y = Y;
		for (size_t i = 0; i < n + ud; i++)
		{
			if (re == 0 || (rnd<100>() <= 30 && kol != 0 && u > 0))
			{
				h.extractMin();
				kol--;
				u--;
			}
			else
			{
				h.insert(r[in]);
				in++;
				kol++;
				re--;
			}
		}
		end = clock();
		BOOST_TEST_MESSAGE("\nfor inserts = " << n << " erases = " << ud << "\n\t time is " << (end - start) / (double)CLOCKS_PER_SEC * 1000 << "ms");
		{
			resetrand(x, y);
			Heap<int> h;
			size_t kol = 0, re = n, u = ud, in = 0;
			start = clock();
			for (size_t i = 0; i < n + ud; i++)
			{
				if (re == 0 || (rnd<100>() <= 30 && kol != 0 && u > 0))
				{
					h.extractMin();
					kol--;
					u--;
				}
				else
				{
					h.insert(r[in]);
					in++;
					kol++;
					re--;
				}
			}
			end = clock();
			BOOST_TEST_MESSAGE("Heap without optimaze for this test\n\t time is " << (end - start) / (double)CLOCKS_PER_SEC * 1000 << "ms\n");
		}
		delete[] r;
	}
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()