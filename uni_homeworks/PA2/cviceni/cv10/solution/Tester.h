/**
 * Tester for BI-PA2 tutorials
 * @version 0.0.2
*/

#define TESTER_SHARE_RESULT

#include <vector>
#include <functional>
#include <sstream>
#include <iostream>
#include <list>
#include <string>
#include <unistd.h>

#ifdef TESTER_SHARE_RESULT
#include "HTTPRequest.hpp"
#endif

#define expect(arg) Tester::expect_(arg, #arg, __FILE__, __LINE__)

bool operator==(const std::list<std::string> left, const std::list<std::string> right)
{
	if (left.size() != right.size())
		return false;
	auto itLeft = left.begin();
	auto itRight = right.begin();
	while (itLeft != left.end())
	{
		if (*itLeft != *itRight || itRight == right.end())
			return false;
		itLeft++;
		itRight++;
	}
	return true;
}

std::ostream& operator<<(std::ostream& os, const std::list<std::string> list)
{
	os << "{ ";
	for (const auto& item : list)
		os << item << ", ";
	os << "}";
	return os;
}

class Tester
{
private:
#ifdef TESTER_SHARE_RESULT
	std::string userName; // = "Jeff"
#endif

	std::vector<std::function<void()>> tests;

	// TODO add error message
	void finalize(size_t i, bool success)
	{
		std::cout << (success ? "SUCCESS" : "FAILURE") << std::endl;
		std::cout << "Reached test " << i << std::endl;
#ifdef TESTER_SHARE_RESULT
		http::Request request{"http://api.tester.classroom.danielbreiner.eu"};
		std::stringstream body;
		body << "\
        {\"workspace\": \"breindan\",\
         \"name\": \"" << userName << "\",\
         \"testIndex\": " << i << ",\
         \"success\": " << success << "}";
		const auto response = request.send(
			"POST",
			body.str(),
			{ {"Content-Type", "application/json"} });
		std::cout << "Shared tester result. (" << response.status.reason << ") " << std::string{response.body.begin(), response.body.end()} << std::endl;
#endif
	}

public:
	Tester()
	{
		std::cout << " -- Tester START --" << std::endl;
#ifdef TESTER_SHARE_RESULT
		if (userName.empty())
		{
			char hostname[255];
			gethostname(hostname, 255);
			userName = hostname;
		}
#endif
	}
	~Tester()
	{
		std::cout << " -- Tester END --" << std::endl;
	}

	void addTest(std::function<void()> test)
	{
		tests.push_back(test);
	}

	void run()
	{
		size_t i;
		for (i = 0; i < tests.size(); i++)
		{
			std::cout << "Running test " << i + 1 << std::endl;
			try
			{
				tests[i]();
			}
			catch (...)
			{
				finalize(i, false);
				throw;
			}
			std::cout << "OK" << std::endl;
		}
		finalize(i, true);
	}

	template<typename T>
	static void expect_(T condition, const char* code, const char* file, int line)
	{
		if (condition)
			return;

		std::stringstream sstream;
		sstream << "Condition failed in (" << code << ") at " << file << ":" << line;
		throw std::runtime_error(sstream.str());
	}

	template<typename T>
	static void expectEqual(T left, T right)
	{
		if (left == right)
			return;

		std::stringstream sstream;
		sstream << "Expected " << left << " == " << right;
		throw std::runtime_error(sstream.str());
	}

	template<typename T>
	static void expectNotEqual(T left, T right)
	{
		if (left != right)
			return;

		std::stringstream sstream;
		sstream << "Expected " << left << " != " << right;
		throw std::runtime_error(sstream.str());
	}
};