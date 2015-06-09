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
        //currently, only default constructor implemented
        heterogeneous::vector<int, double, std::string, int, int, double, std::string> hv;

        hv.get<double>().push_back(4.3);
        hv.get<std::string, 0>().push_back("one");
        hv.get<std::string, 0>().push_back(std::string("two"));
        hv.get<double, 0>().push_back(3.411);
        hv.get<std::string, 0>().push_back(std::string("three"));
        std::string four = "four";
        hv.get<std::string, 0>().push_back(four);
        hv.get<double>().push_back(99.999);
        //hv.push_back(1.2f); //exception: type float does not exist in hv


        //create an std::vector of ints
        std::vector<int> i;
        i.push_back(9);
        i.push_back(3);
        i.push_back(1);


        //assignment of std::vector<int> to heterovector
        hv.get<int, 1>() = i;
        hv.get<int>().resize(4, 35); //add 4th value of 35, default to first int element


        //============================
        //data access methods below...
        //============================
        std::cout << hv.get<std::string>().front() << std::endl; //first std::string
        std::cout << hv.get<double>().back() << std::endl; //last double
        std::cout << hv.get<double>().at(0) << std::endl; //first double
        std::cout << std::endl;


        //iterate over ints
        //element 0
        for (auto itr = hv.get<int, 0>().begin(); itr != hv.get<int, 0>().end(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;
        //element 1
        for (auto itr = hv.get<int, 1>().begin(); itr != hv.get<int, 1>().end(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        //reverse iterate over strings
        hv.get<std::string,0>().push_back("implicitly convert me to a std::string!"); //implicit conversion
        for (auto itr = hv.get<std::string>().rbegin(); itr != hv.get<std::string>().rend(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        //reverse iterate (const) over doubles
        hv.get<double,0>().push_back(4335);
        hv.get<double,1>().push_back(497.3f);

        //element 0
        for (auto itr = hv.get<double, 0>().crbegin(); itr != hv.get<double, 0>().crend(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        //element 1
        for (auto itr = hv.get<double, 1>().crbegin(); itr != hv.get<double, 1>().crend(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        //erase last double and iterate (const) 
        hv.get<double>().erase(--hv.get<double>().end());
        for (auto itr = hv.get<double>().cbegin(); itr != hv.get<double>().cend(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;


        //get info about various types in heterodeque
        std::cout << hv.get<int, 0>().size() << std::endl;
        std::cout << hv.get<int, 1>().size() << std::endl;
        std::cout << hv.get<int, 2>().size() << std::endl;
        std::cout << hv.get<double>().size() << std::endl;
        std::cout << hv.get<std::string>().size() << std::endl;
        std::cout << hv.size() << " total number of type elements in heterodeque" << std::endl;
        std::cout << hv.type<2>().name() << " type of third element in heterodeque" << std::endl;
        std::cout << std::endl;


        //return type multiplicity
        std::cout << hv.multiplicity<int>() << " integer multiplicity" << std::endl;
        std::cout << hv.multiplicity<double>() << " double multiplicity" << std::endl;
        std::cout << hv.multiplicity<float>() << " float multiplicity" << std::endl;
        std::cout << hv.multiplicity<std::string>() << " double multiplicity" << std::endl;
        std::cout << std::endl;

        // print each double
		heterogeneous::for_each<double>( hv, [&](auto C)
		{
		for (auto itr = C.begin(); itr != C.end(); ++itr)
            std::cout << *itr << std::endl;
        std::cout << std::endl;
		});
		
		// print all values
		heterogeneous::for_all( hv, [&](auto C)
		{
		for (auto itr = C.begin(); itr != C.end(); ++itr)
            std::cout << *itr << std::endl;
        std::cout << std::endl;
		});
		
    }
    catch (std::exception err)
    {
        std::cerr << err.what() << std::endl;
    }

    return 0;
}