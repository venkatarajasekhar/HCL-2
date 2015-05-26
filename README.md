# HCL: Heterogeneous Container Library

## Tested On
* GNU g++ 4.9.1 (x86_64-win32-seh-rev1, MinGW-w64)
* MS vc12 (Visual Studio 2013)
* MS vc14 RC (Visual Studio 2015 RC)
* LLVM clang 3.6.0


## Documentation

* **doc/html/index.html**
    * Doxygen generated html documentation for HCL
* **doc/latex/refman.pdf**
    * Doxygen generated Latex pdf documentation for HCL


## Header Files
* **adaptor.hpp**:
   Provides a common interface for retrieving data from a sequence
   container of type-erased data types (boost::any, boost::variant).
   Type-erased type MUST implement a *get\<typename T\>()* function natively,
   or user must define it.

* **heterodeque.hpp**:
   Provides complete std::deque functionality with the ability to natively
   hold elements of different types.  Each type is stored internally in its
   own std::deque, so all operations maintain their inherent benefit over other
   sequence container types.

* **heterolist.hpp**:
   Provides complete std::list functionality with the ability to natively
   hold elements of different types.  Each type is stored internally in its
   own std::list, so all operations maintain their inherent benefit over other
   sequence container types.

* **heteroqueue.hpp**:
   Provides complete std::queue functionality with the ability to natively
   hold elements of different types.  Each type is stored internally in its
   own std::queue, so all operations maintain their inherent benefit over other
   sequence container types.

* **heterostack.hpp**:
   Provides complete std::stack functionality with the ability to natively
   hold elements of different types.  Each type is stored internally in its
   own std::stack, so all operations maintain their inherent benefit over other
   sequence container types.

* **heterovector.hpp**:
   Provides complete std::vector functionality with the ability to natively
   hold elements of different types.  Each type is stored internally in its
   own std::vector, so all operations maintain their inherent benefit over other
   sequence container types.

Typical usage for all *heterocontainers*:
     Just add template parameter to normal *std::container* methods.

        heterogeneous::heterovector<int, double, std::string, double> hv;

        hv.push_back<int>(2);
        hv.push_back<double>(3.1415); // defaults to first double element
        hv.push_back<double, 0>(9); // specifically denote first double element
        hv.push_back<std::string>(std::string("hello "));
        hv.push_back<std::string>("world!"); // implicit conversion to std::string

        hv.size<int>(); // 1
        hv.size<double>(); // 2 (defaults to first double element)
        hv.size<double, 1>(); // 0
        hv.size<std::string>(); // 2
        hv.size(); // 4, number of elements of heterovector (int, double, std::string, double)

        // print all integers
        for (auto itr = hv.begin<int>(); itr != hv.end<int>(); ++itr)
            std::cout << *itr << std::endl;

        // print all doubles (first element)
        for (auto itr = hv.begin<double, 0>(); itr != hv.end<double, 0>(); ++itr)
            std::cout << *itr << std::endl;

        // print all std::strings
        for (auto itr = hv.begin<std::string, 0>(); itr != hv.end<std::string, 0>(); ++itr)
            std::cout << *itr << std::endl;

        // print all doubles (second element)
        for (auto itr = hv.begin<double, 1>(); itr != hv.end<double, 1>(); ++itr)
            std::cout << *itr << std::endl;


## Examples
* examples directory contains source code with several use examples.