#include <string>
#include <iostream>

#include <deque>
#include <list>
#include <queue>
#include <stack>
#include <vector>

#include "heterogeneous.hpp"


template<typename T>
void func(T C)
{
	for (auto itr = C.begin(); itr != C.end(); ++itr)
		std::cout << *itr << std::endl;
	std::cout << std::endl;
}


int main()
{
    try
    {
		heterogeneous::vector<int, double, std::string, double> hv;

		hv.get<int>().push_back(2);
		hv.get<double>().push_back(3.1415); // implicit element 0
		hv.get<double, 0>().push_back(9.1); // explicitly element 0
		hv.get<std::string>().push_back(std::string("hello "));
		hv.get<std::string>().push_back("world!");

		std::cout << hv.get<int>().size() << std::endl; // 1
		std::cout << hv.get<double>().size() << std::endl; // 2
		std::cout << hv.get<double, 1>().size() << std::endl; // 0
		std::cout << hv.get<std::string>().size() << std::endl; // 2
		std::cout << hv.size() << std::endl; // 4, number of elements of heterovector (int, double, std::string, double)
		std::cout << std::endl;

		// print all integers (implicit element 0)
		for (auto itr = hv.get<int>().begin(); itr != hv.get<int>().end(); ++itr)
			std::cout << *itr << std::endl;
		std::cout << std::endl;

		// print all doubles
		heterogeneous::for_each<double>(hv, [&](auto C)
		{
			for (auto itr = C.begin(); itr != C.end(); ++itr)
			{
				std::cout << *itr << std::endl;
			}
			std::cout << std::endl;
		});

		// print all items
		heterogeneous::for_all(hv, [&](auto C)
		{
			for (auto itr = C.begin(); itr != C.end(); ++itr)
			{
				std::cout << *itr << std::endl;
			}
			std::cout << std::endl;
		});
		
    }
    catch (std::exception err)
    {
        std::cerr << err.what() << std::endl;
    }

    return 0;
}