# HCL: Heterogeneous Container Library

## Tested On : (requires C++14 generic lambda)
* vc14 RC (Visual Studio 2015 RC)


## Documentation

* **doc/html/index.html**
    * Doxygen generated html documentation for HCL
* **doc/latex/refman.pdf**
    * Doxygen generated Latex pdf documentation for HCL


## Use

		heterogeneous::vector<int, double, std::string, double> hv;

		hv.get<int>().push_back(2);
		hv.get<double>().push_back(3.1415); // implicit element 0
		hv.get<double, 0>().push_back(9.1); // explicit element 0
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

		// for_each vector of double, print all elements
		heterogeneous::for_each<double>(hv, [&](auto C)
		{
			for (auto itr = C.begin(); itr != C.end(); ++itr)
			{
				std::cout << *itr << std::endl;
			}
			std::cout << std::endl;
		});

		// for_all vectors, print all elements
		heterogeneous::for_all(hv, [&](auto C)
		{
			for (auto itr = C.begin(); itr != C.end(); ++itr)
			{
				std::cout << *itr << std::endl;
			}
			std::cout << std::endl;
		});
		
		// check if all containers contain elements all greater than 0.0
		std::cout <<
		heterogeneous::all_of<double>(hv, [](auto C)->bool
		{
			for (auto itr = C.begin(); itr != C.end(); ++itr)
			{
				if( !(*itr > 0.0) ) return false;
			}

			return true;
		})
		<< std::endl;

## Examples
* examples directory contains source code with several use examples.