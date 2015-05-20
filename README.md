# HCL: Heterogeneous Container Library

## Tested On
* g++ 4.9.1 (MinGW-w64)
* MS vc12 (Visual Studio 2013)
* MS vc14 RC (Visual Studio 2015 RC)
* LLVM clang 3.6.0
 

## Documentation

* doc/html/index.html
    * Doxygen generated html documentation for HCL
* doc/latex/refman.pdf
    * Doxygen generated Latex pdf documentation for HCL


## Header Files
* **adaptor.hpp**:
   Provides a common interface for retrieving data from a sequence
   container of type-erased data types (boost::any, boost::variant).
   Type-erased type MUST implement a get<typename T>() function natively,
   or user must define it.
   
* **heterovector.hpp**:
   Provides complete std::vector functionality with the ability to natively
   hold elements of different types.  Each type is stored internally in its
   own std::vector, so all operations maintain their inherent benefit over other
   sequence container types.
   
   Typical usage: Add template parameter to normal std::vector methods.
   
		heterogeneous:heterovector<int,double,std::string, double> hv;

		hv.push_back<int>(2);
		hv.push_back<double>(3.1415); //defaults to first double element
		hv.push_back<double,0>(9); //implicit conversion to double
		hv.push_back<std::string>( std::string("hello") );
		hv.push_back<std::string>("world!"); //implicit conversion to std::string
		
		hv.size<int>; // 1
		hv.size<double>; // 2 (defaults to first double element)
		hv.size<double,1>; // 0
		hv.size<std::string> // 2
		
		// print all integers
		for(auto itr = ht.begin<int>(); itr != ht.end<int>(); ++ht
		std::cout << *itr << std::endl;
		
		// print all doubles (first element)
		for(auto itr = ht.begin<double,0>(); itr != ht.end<double,0>(); ++ht
		std::cout << *itr << std::endl;
		
		// print all std::strings
		for(auto itr = ht.begin<std::string,0>(); itr != ht.end<std::string,0>(); ++ht
		std::cout << *itr << std::endl;
		
		// print all doubles (second element)
		for(auto itr = ht.begin<double,1>(); itr != ht.end<double,1>(); ++ht
		std::cout << *itr << std::endl;
	  
* **heterodeque.hpp**:
   Provides complete std::deque functionality with the ability to natively
   hold elements of different types.  Each type is stored internally in its
   own std::deque, so all operations maintain their inherent benefit over other
   sequence container types.


## Examples
* examples directory contains source code with several use examples.
