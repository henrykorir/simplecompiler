#include <buckethash.h>
#include <string>
#include <functionalX.h>
#include <unittest.h>
#include <iostream>
#include <stringX.h>
#include <debug.h>

struct string2int : public std::unary_function<std::string, int>
{
	int operator()(const std::string& s) const
	{
		int x = 1000;
		foreach (char c, s.begin(), s.end())
		{
			x  += c * 457981;
		}
		return x;
	}
};

class hashset_test : public sc::unittest
{
public:
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
	}

	/* overwrite */ virtual void run_test()
	{
		kog::buckethash<std::string, int, string2int> hashdata(32);

		kog::assert::are_equal(hashdata.begin(), hashdata.end());

		hashdata["129"] = 45;
		hashdata["fda"] = 145;
		hashdata["fdafd"] = 445;
		hashdata["af"] = 6745;
		hashdata["fdare"] = 485;

		typedef std::pair<std::string, int> stringint_pair;
		foreach (const stringint_pair& p, hashdata.begin(), hashdata.end())
		{
			std::cout<<stringX::format("[%s] = %d", p.first.c_str(), p.second)<<std::endl;
		}

		std::cout<<"reverse_iterator test"<<std::endl;
		foreach (const stringint_pair& p, hashdata.rbegin(), hashdata.rend())
		{
			std::cout<<stringX::format("[%s] = %d", p.first.c_str(), p.second)<<std::endl;
		}

		hashdata.insert("fdsa", 43);
		kog::assert::are_equal(6, (int)hashdata.size());

		hashdata["af"] = -1;

		std::cout<<"insert test "<<std::endl;
		foreach (const stringint_pair& p, hashdata.begin(), hashdata.end())
		{
			std::cout<<stringX::format("[%s] = %d", p.first.c_str(), p.second)<<std::endl;
		}
	}
};

NEW_UNITTEST(hashset_test);