#define BOOST_TEST_MODULE testvector 
#include <boost/test/included/unit_test.hpp>
#include "Vector.h"

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

BOOST_AUTO_TEST_SUITE(TestVector)

BOOST_AUTO_TEST_CASE(Test_pushback1)
{
	Vector<int> a;
	for (size_t i = 0; i < 100; ++i)
		a.push_back(rnd(i));
	for (size_t i = 0; i < 100; ++i)
		BOOST_CHECK_EQUAL(rnd(i), a[i]);
}

BOOST_AUTO_TEST_CASE(Test_pushback2)
{
	Vector<int> a;
	int* rez = new int[100];
	for (size_t i = 0; i < 100; ++i)
	{
		rez[i] = rnd();
		a.push_back(rez[i]);
	}
	for (size_t i = 0; i < 100; ++i)
		BOOST_CHECK_EQUAL(rez[i], a[i]);
	delete[] rez;
}

BOOST_AUTO_TEST_CASE(Test_popback1)
{
	Vector<int> a;
	for (size_t i = 0; i < 100; ++i)
		a.push_back(rnd(i));
	for (size_t i = 0; i < 50; ++i)
		a.pop_back();
	for (size_t i = 0; i < 50; ++i)
		BOOST_CHECK_EQUAL(rnd(i), a[i]);
}

BOOST_AUTO_TEST_CASE(Test_popback2)
{
	Vector<int> a;
	int* rez = new int[100];
	for (size_t i = 0; i < 100; ++i)
	{
		rez[i] = rnd();
		a.push_back(rez[i]);
	}
	for (size_t i = 0; i < 100; ++i)
		BOOST_CHECK_EQUAL(rez[i], a[i]);
	delete[] rez;
}

BOOST_AUTO_TEST_CASE(Test_size_isEmpty_getcapasity)
{
	Vector<int> a;
	for (size_t i = 0; i < 100; ++i)
	{
		BOOST_CHECK_EQUAL(a.size(), i);
		a.push_back(rnd());
		BOOST_CHECK_EQUAL((i < a.getcapacity()), true);
	}
	BOOST_CHECK_EQUAL(a.size(), 100);
	for (size_t i = 0; i < 100; ++i)
	{
		BOOST_CHECK_EQUAL(a.size(), 100 - i);
		BOOST_CHECK_EQUAL(a.isEmpty(), false);
		a.pop_back();
	}
	BOOST_CHECK_EQUAL(a.size(), 0);
	BOOST_CHECK_EQUAL(a.isEmpty(), true);
}

BOOST_AUTO_TEST_CASE(Test_resize)
{
	Vector<int> a;
	for (size_t i = 0; i < 100; ++i)
	{
		a.resize(i);
		BOOST_CHECK_EQUAL(a.getcapacity(), i);
	}
	a.resize(0);
	BOOST_CHECK_EQUAL(a.size(), 0);
	BOOST_CHECK_EQUAL(a.isEmpty(), true);
	for (size_t i = 0; i < 100; ++i)
	{
		a.push_back(rnd());
	}
	a.resize(50);
	BOOST_CHECK_EQUAL((a.size() <= a.getcapacity()), true);
	for (size_t i = 49; i > 0; --i)
	{
		a.resize(i);
		BOOST_CHECK_EQUAL((a.size() <= a.getcapacity()), true);
	}
}

BOOST_AUTO_TEST_CASE(Test_throws)
{
	Vector<int> a;
	BOOST_CHECK_THROW(a.pop_back(), logic_error);
	for (size_t i = 0; i < 10; ++i)
		a.push_back(rnd(i));
	BOOST_CHECK_THROW(a[10], out_of_range);
	BOOST_CHECK_THROW(a[112], out_of_range);
}

BOOST_AUTO_TEST_CASE(Test_acess)
{
	Vector<int> a;
	for (int i = 0; i < 10; ++i)
	{
		int tmp = rnd();
		a.push_back(tmp);
		BOOST_CHECK_EQUAL(a[i], tmp);
	}
	a[3] = 144;
	BOOST_CHECK_EQUAL(a[3], 144);
	for (int i = 0; i < 10; ++i)
	{
		int tmp1 = rnd(), tmp = rnd();
		a[(tmp1 % 10 + 10) % 10] = tmp;
		BOOST_CHECK_EQUAL(a[(tmp1 % 10 + 10) % 10], tmp);
	}
}

BOOST_AUTO_TEST_CASE(Test_Final)
{
	Vector<double> a;
	double* r = new double[5];
	for (size_t i = 0; i < 5; ++i)
	{
		r[i] = rnd();
		a.push_back(r[i]);
	}
	for (size_t i = 0; i < 5; ++i)
		BOOST_CHECK_EQUAL(r[i], a[i]);
	a[0] = 12;
	BOOST_CHECK_EQUAL(12, a[0]);
	BOOST_CHECK_THROW(a[5], out_of_range);
	for (size_t i = 0; i < 5; ++i)
		a.pop_back();
	BOOST_CHECK_THROW(a.pop_back(), logic_error);
	for (size_t i = 0; i < 5; ++i)
		a.push_back(rnd());
	a.resize(2);
	BOOST_CHECK_THROW(a[2], out_of_range);
	a.resize(0);
	BOOST_CHECK_THROW(a.pop_back(), logic_error);
	delete[] r;
}

BOOST_AUTO_TEST_SUITE_END();