#include <string>
#include <iostream>

#include <deque>
#include <list>
#include <queue>
#include <stack>
#include <vector>

#include "heterodeque.hpp"
#include "heterolist.hpp"
#include "heteroqueue.hpp"
#include "heterostack.hpp"
#include "heterovector.hpp"

int main()
{
    try
    {
        //currently, only default constructor implemented
        heterogeneous::heterodeque<int, double, std::string, int, int, double, std::string> hd;
        heterogeneous::heterolist<int, double, std::string, int, int, double, std::string> hl;
        heterogeneous::heteroqueue<int, double, std::string, int, int, double, std::string> hq;
        heterogeneous::heterostack<int, double, std::string, int, int, double, std::string> hs;
        heterogeneous::heterovector<int, double, std::string, int, int, double, std::string> hv;

        //direct insertion of doubles and std::strings into heterodeque
        //hv.reserve<double>(3);
        hv.push_back<double>(4.3);
        hv.push_back<std::string,0>("one");
        hv.push_back<std::string,0>(std::string("two"));
        hv.push_back<double, 0>(3.411);
        hv.push_back<std::string,0>(std::string("three"));
        std::string four = "four";
        hv.push_back<std::string, 0>(four);
        hv.push_back<double>(99.999);
        //hv.push_back(1.2f); //exception: type float does not exist in hv

        //create an std::deque of ints
        std::vector<int> i;
        i.push_back(9);
        i.push_back(3);
        i.push_back(1);

        //assignment of std::deque<int> to heterodeque
        hv.set<int,1>(i); //sets second element of ints to i
        hv.resize<int>(4, 35); //add 4th value of 35, default to first int element
        hv.insert<int>(hv.begin<int,0>(), std::initializer_list < int > {0, 0}); //inserts two 0s at beginning of first int element
        hv.emplace_back<int,1>(4);  //add value of 4 to back of second element of ints

        //============================
        //data access methods below...
        //============================
        std::cout << hv.front<std::string>() << std::endl; //first std::string
        std::cout << hv.back<double>() << std::endl; //last double
        std::cout << hv.at<double>(0) << std::endl; //first double
        //std::cout << *hv.data<double>() << std::endl; //first double
        std::cout << std::endl;

        //iterate over ints
        //element 0
        for (auto itr = hv.begin<int,0>(); itr != hv.end<int,0>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;
        //element 1
        for (auto itr = hv.begin<int,1>(); itr != hv.end<int,1>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        //reverse iterate over strings
        hv.container<std::string,0>()->push_back("implicitly convert me to a std::string!"); //implicit conversion
        for (auto itr = hv.rbegin<std::string>(); itr != hv.rend<std::string>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        //reverse iterate (const) over doubles
        hv.container<double,0>()->push_back(4335);
        hv.container<double,1>()->push_back(497.0f);
        //element 0
        for (auto itr = hv.crbegin<double,0>(); itr != hv.crend<double,0>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;
        //element 1
        for (auto itr = hv.crbegin<double, 1>(); itr != hv.crend<double, 1>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;
        //erase last double and iterate (const) 
        hv.erase<double>(--hv.end<double>());
        for (auto itr = hv.cbegin<double>(); itr != hv.cend<double>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        //get info about various types in heterodeque
        std::cout << hv.size<int,0>() << std::endl;
        std::cout << hv.size<int,1>() << std::endl;
        std::cout << hv.size<int,2>() << std::endl;
        //std::cout << hv.size<int,3>() << std::endl;  //exception, fouth element of type int doesn't exist
        std::cout << hv.size<double>() << std::endl;
        std::cout << hv.size<std::string>() << std::endl;
        std::cout << hv.size() << " total number of type elements in heterodeque" << std::endl;
        std::cout << hv.type<2>().name() << " type of third element in heterodeque" << std::endl;
        std::cout << std::endl;

        //return type multiplicity
        std::cout << hv.multiplicity<int>() << " integer multiplicity" << std::endl;
        std::cout << hv.multiplicity<double>() << " double multiplicity" << std::endl;
        std::cout << hv.multiplicity<float>() << " float multiplicity" << std::endl;
        std::cout << hv.multiplicity<std::string>() << " double multiplicity" << std::endl;
        std::cout << std::endl;

        // check whether heterodeque contains type
        std::cout << hv.contains<double>() << std::endl;
        std::cout << hv.contains<float>() << std::endl;
        std::cout << hv.contains<std::string>() << std::endl;
    }
    catch (std::exception err)
    {
        std::cerr << err.what() << std::endl;
    }

    return 0;
}