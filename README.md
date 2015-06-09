# HCL: Heterogeneous Container Library

## Tested On : requires C++14 generic Lambda
* vc14 RC (Visual Studio 2015 RC)
* clang 3.6.0


## Documentation

* **doc/html/index.html**
    * Doxygen generated html documentation for HCL
* **doc/latex/refman.pdf**
    * Doxygen generated Latex pdf documentation for HCL


## Use

        heterogeneous::vector<int, double, std::string, double> hv;

        hv.get<int>.push_back(2);
        hv.get<double>.push_back(3.1415); // implicit element 0
        hv.get<double,0>.push_back(9); // explicitly element 0
        hv.get<std::string>.push_back(std::string("hello "));
        hv.get<std::string>.push_back("world!");

        hv.get<int>.size(); // 1
        hv.get<double>.size(); // 2
        hv.get<double,1>.size(); // 0
        hv.get<std::string>.size(); // 2
        hv.size(); // 4, number of elements of heterovector (int, double, std::string, double)

        // print all integers (implicit element 0)
        for (auto itr = hv.get<int>.begin() itr != hv.get<int>.end(); ++itr)
            std::cout << *itr << std::endl;

		// print all doubles
		heterogeneous::for_each<double>( hv, [&](auto C)
		{
		  for (auto itr = C.begin(); itr != C.end(); ++itr)
		  {
              std::cout << *itr << std::endl;
		  }
          std::cout << std::endl;
		});
		
		// print all items
		heterogeneous::for_all( hv, [&](auto C)
		{
		  for (auto itr = C.begin(); itr != C.end(); ++itr)
		  {
              std::cout << *itr << std::endl;
		  }
          std::cout << std::endl;
		});

## Examples
* examples directory contains source code with several use examples.