#include <boost\any.hpp>
#include <boost\variant.hpp>
#include <vector>
#include <string>
#include <iostream>

#include "adaptor.hpp"

int main()
{
    typedef boost::any data_type;
    //typedef boost::variant<float, double, int, std::string> data_type;
	
    typedef std::vector<data_type> container_type;

    try
    {
		// create container of type-erased types
        container_type container;
        container.push_back(3.1415f);
        container.push_back(3.141516);
        container.push_back(std::string("a"));
        container.push_back(1986);
        container.push_back(std::string("b"));
        container.push_back(std::string("c"));
        container.push_back(2004);
        container.push_back(69.69);
        container.push_back(std::string("d"));

		
		// create heterogeneous adaptor for container
        heterogeneous::adaptor<container_type> p(container);

        // reverse iterate over std::strings
        for (auto itr = p.rbegin<std::string>(); itr != p.rend<std::string>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        // swap string elements 0 ("a") and 2 ("c")
        p.swap<std::string>(0, 2);
        // iterate over std::strings
        for (auto itr = p.begin<std::string>(); itr != p.end<std::string>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;
		
        // undo swap
        p.swap<std::string>(0, 2);
		
        // iterate over std::strings
        for ( auto itr = p.begin<std::string>(); itr != p.end<std::string>(); ++itr )
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;
		
        // iterate over ints
        for ( auto itr = p.begin<int>(); itr != p.end<int>(); ++itr )
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        // iterate over floats
        for ( auto itr = p.begin<float>(); itr != p.end<float>(); ++itr )
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        // iterate over doubles...but check if empty first
        if ( !p.empty<double>() )
        {
            for ( auto itr = p.begin<double>(); itr != p.end<double>(); ++itr )
            {
                std::cout << *itr << std::endl;
            }
            std::cout << std::endl;
        }

		
        // swap 1st int with 4th std::string
        p.swap<int, std::string>(0, 3);
		// swapping values of different types can produce unexpected results
		// for example, swapping the 0th int and the 4th std::string also
        // results in the 1st int becoming the 2nd and the 2nd int the 1st
        // and the 4th std::string becoming the second, the second becoming the 3rd,
        // and the 3rd becoming the fourth
		
        // iterate over ints
        for (auto itr = p.begin<int>(); itr != p.end<int>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;
        // iterate over std::strings
        for (auto itr = p.begin<std::string>(); itr != p.end<std::string>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

		
		// get first int
        std::cout << p.first<int>() << std::endl;
		// get last std::string
        std::cout << p.last<std::string>() << std::endl;
        std::cout << std::endl;

        // get the first data_type holding a std::string : p.at<std::string>(0)
		// use get<std::string> to convert data_type to std::string and print
		// *** container_type& get<typename T>(data_type& ) MUST BE DEFINED ***
        std::cout << boost::get<std::string>( p.at<std::string>(0) ) << std::endl;
		
        // adaptor has a built in get<typename T>() method that can be used
		// *** container_type& get<typename T>(data_type& ) MUST BE DEFINED ***
        std::cout << p.get<std::string>(0) << std::endl;
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << '\n';
    }

    return 0;
}