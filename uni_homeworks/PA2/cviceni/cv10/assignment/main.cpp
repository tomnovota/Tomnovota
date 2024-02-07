#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

#include "Tester.h"
#include "pair.h"
#include "vector.h"

// #define TEST 10

int main()
{
	Tester tester;

#if TEST >= 1
	std::cout << " -- Flag TEST=" << TEST << " enabled --" << std::endl;
#else
	std::cout << " -- No test flag enabled (compile with -DTEST=<n>). --" << std::endl;
#endif

	// Pair<First, Second>
#if TEST >= 1
	tester.addTest([]() {
		Pair intPair(0, 1);
		expect(intPair.get_first() == 0);
		expect(intPair.get_second() == 1);

		Pair charPair('a', 'b');
		expect(charPair.get_first() == 'a');
		expect(charPair.get_second() == 'b');

		Pair<std::string, long> stringLongPair("test", __LONG_MAX__);
		expect(stringLongPair.get_first() == "test");
		expect(stringLongPair.get_second() == __LONG_MAX__);

		std::cout << "Pair test OK" << std::endl;
		});
#endif

	// Vector<T>::filter
#if TEST >= 2
	const int INT_DATA_COUNT = 10;
	Vector<int> int_vec(INT_DATA_COUNT);
	const int STR_DATA_COUNT = 5;
	Vector<std::string> str_vec(STR_DATA_COUNT);

	tester.addTest([&]() {
		// empty initial Vector
		{
			Vector<int> empty;
			Vector result = empty.filter([](const int& _) { return true; });
			expect(result == empty);
		}

		for (int i = 0; i < INT_DATA_COUNT; i++)
		{
			int_vec.push_back(i);
		}

		str_vec = {"Lorem", "ipsum", "dolor", "sit", "amet"};

		// predicate matched by multiple
		{
			Vector result = int_vec.filter([](const int& x) { return x > 5; });
			expect(result == (Vector<int>{6, 7, 8, 9}));
		}
		{
			Vector result = int_vec.filter([](const int& x) { return x % 3 == 1; });
			expect(result == (Vector<int>{1, 4, 7}));
		}

		// predicate matched by none
		{
			Vector result = int_vec.filter([](const int& x) { return x > 10; });
			expect(result == Vector<int>{});
		}

		// predicate matched by multiple, different data type
		{
			Vector result =
				str_vec.filter([](const std::string& x) { return x.length() < 5; });
			expect(result == (Vector<std::string>{"sit", "amet"}));
		}

		std::cout << "Vector<T>::filter test OK" << std::endl;
		});
#endif

	// Vector<T>::map
#if TEST >= 3
	tester.addTest([&]() {
		// int -> int
		{
			Vector<int> result = int_vec.map<int>([](const int& x) { return x + 1; });
			Vector<int> expected(INT_DATA_COUNT);
			for (int i = 0; i < INT_DATA_COUNT; i++)
			{
				expected.push_back(i + 1);
			}
			expect(result == expected);
		}

		// std::string -> size_t
		{
			Vector<size_t> result =
				str_vec.map<size_t>([](const std::string& x) { return x.length(); });
			expect(result == (Vector<size_t>{5, 5, 5, 3, 4}));
		}

		// int ->(filter)-> int -> std::string
		{
			Vector<std::string> result = int_vec.filter([](const int& x) { return x < 3; })
				.map<std::string>([](const int& x) {
				std::string str = "";
				for (int i = 0; i < x; i++)
				{
					str.append("x");
				}
				return str;
					});
			expect(result == (Vector<std::string>{"", "x", "xx"}));
		}

		std::cout << "Vector<T>::map test OK" << std::endl;
		});
#endif

	// operator<<(os, Vector<char>)
#if TEST >= 4
	tester.addTest([&]() {
		std::ostringstream oss;
		{
			oss.str("");
			oss.clear();
			oss << int_vec;
			expect(oss.str() == "[ 0 1 2 3 4 5 6 7 8 9 ]");
		}
		{
			Vector<char> char_vec{'H', 'e', 'l', 'l', 'o'};

			oss.clear();
			oss.str("");
			oss << char_vec;
			expect(oss.str() == "Hello");

			char_vec.push_back(',');
			char_vec.push_back(' ');
			char_vec.push_back('W');
			char_vec.push_back('o');
			char_vec.push_back('r');
			char_vec.push_back('l');
			char_vec.push_back('d');
			char_vec.push_back('!');

			oss.clear();
			oss.str("");
			oss << char_vec;
			expect(oss.str() == "Hello, World!");
		}
		std::cout << "operator<<(os, Vector<char>) test OK" << std::endl;
		});
#endif

	// std::transform
#if TEST >= 5
	tester.addTest([&]() {
		std::vector<char> word { 'H', 'e', 'l', 'l', 'o' };
		expect(word == std::vector<char>({ 'H', 'e', 'l', 'l', 'o' }));
		// TODO: Use std::transform to convert `word` to uppercase
		// std::transform();
		expect(word == std::vector<char>({ 'H', 'E', 'L', 'L', 'O' }));
		});
#endif

	tester.run();
}
