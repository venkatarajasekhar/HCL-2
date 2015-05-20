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

        heterogeneous::adaptor<container_type> p(container);

        //add data accessor function pointers for each data type to be accessed
        /*
        p.add_accessor<std::string>(boost_any_accessor<std::string>);
        p.add_accessor<int>(boost_any_accessor<int>);
        p.add_accessor<float>(boost_any_accessor<float>);
        p.add_accessor<double>(boost_any_accessor<double>);

        p.add_accessor<std::string>(boost_variant_accessor<std::string>);
        p.add_accessor<int>(boost_variant_accessor<int>);
        p.add_accessor<float>(boost_variant_accessor<float>);
        p.add_accessor<double>(boost_variant_accessor<double>);
        */

        //reverse iterate over std::strings
        for (auto itr = p.rbegin<std::string>(); itr != p.rend<std::string>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        //swap string elements 1 ("b") and 2 ("c")
        p.swap<std::string>(1, 2);
        //iterate over std::strings
        for (auto itr = p.begin<std::string>(); itr != p.end<std::string>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;
        //undo swap
        p.swap<std::string>(1, 2);
        //iterate over std::strings
        for (auto itr = p.begin<std::string>(); itr != p.end<std::string>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        //iterate over ints
        for (auto itr = p.begin<int>(); itr != p.end<int>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        //iterate over floats
        for (auto itr = p.begin<float>(); itr != p.end<float>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        //iterate over doubles...check if empty first
        if (!p.empty<double>())
        {
            for (auto itr = p.begin<double>(); itr != p.end<double>(); ++itr)
            {
                std::cout << *itr << std::endl;
            }
            std::cout << std::endl;
        }

        //swap first int with fourth string
        p.swap<int, std::string>(0, 3);
        //iterate over ints
        for (auto itr = p.begin<int>(); itr != p.end<int>(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
        std::cout << std::endl;

        std::cout << p.first<int>() << std::endl;
        std::cout << p.last<std::string>() << std::endl;


        //get the first data_type holding a std::string, use get<std::string> to convert data_type to string and print
        std::cout << std::endl << boost::get<std::string>(p.at<std::string>(0)) << std::endl;
        //string at element 0 in container is the 0th string inserted
        std::cout << p.get<std::string>(0) << std::endl;

        //get the second data_type element in container, use get<double> to convert to double and print
        //in this case, one has to know beforehand that the second element stores a double
        std::cout << boost::get<double>(p.at(1)) << std::endl;
        //double at element 1 is the 0th double inserted
        std::cout << p.get<double>(0) << std::endl;

    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << '\n';
    }

    return 0;
}