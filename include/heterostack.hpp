#ifndef HETEROGENEOUS_HETEROSTACK_HPP
#define HETEROGENEOUS_HETEROSTACK_HPP

/*
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*
* Copyright (c) 2015 Hirotatsu Armstrong
*/

/*!
* \file heterostack.hpp
*
* This templated class replicates the features of an std::stack
* that can handle heterogeneous data types.
*/

#include <stdexcept>
#include <typeinfo>
#include <typeindex>
#include <stack>
#include <string>

namespace heterogeneous
{
    /*!
    * \cond Skip Doxygen documentation of this forward declaration.
    */
    template<typename... Types> class heterostack;
    /*!
    * \endcond
    */

    template<typename T, typename... Types>
    class heterostack<T, Types...>
    {
        // Friends
        template<typename... Args> friend class heterostack;
		
    public:
        // Typedefs
        typedef T value_type;

        template<typename U>
        using container_type = std::stack<U>;

        // Relational Operators & Methods
        // =========================================================================
        // all operators need to be implemented using the containers native
        // relational operators.  Defining relational operators based on one another
        // does not produce expected results.
        // std::stack uses lexographical comparison
        // User must be aware of this as it may not produce expected result
        // =========================================================================

        /*!
        * \brief Returns true if == operator evaluates to true for each element in object.
        */
        bool operator==(const heterostack<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) == *rhs.container<value_type, 0>())) return false;
            return next().operator==(rhs.next());
        }

        /*!
        * \brief Returns true if == operator evaluates to false for any element in object.
        */
        bool operator!=(const heterostack<value_type, Types...>& rhs)
        {
            return !operator==(rhs);
        }

        /*!
        * \brief Returns true if < operator evaluates to true for each element in object.
        */
        bool operator<(const heterostack<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) < *rhs.container<value_type, 0>())) return false;
            return next().operator<(rhs.next());
        }

        /*!
        * \brief Returns true if > operator evaluates to true for each element in object.
        */
        bool operator>(const heterostack<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) > *rhs.container<value_type, 0>())) return false;
            return next().operator>(rhs.next());
        }

        /*!
        * \brief Returns true if <= operator evaluates to true for each element in object.
        */
        bool operator<=(const heterostack<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) <= *rhs.container<value_type, 0>())) return false;
            return next().operator<=(rhs.next());
        }

        /*!
        * \brief Returns true if >= operator evaluates to true for each element in object.
        */
        bool operator>= (const heterostack<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) >= *rhs.container<value_type, 0>())) return false;
            return next().operator>=(rhs.next());
        }

        /*!
        * \brief Same as operator==() but strictly enforces container element size matching.
        */
        bool eq(const heterostack<value_type, Types...>& rhs)
        {
            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;

            // Although this code is duplicated in operator==, we cannot call operator== here as a replacement.
            // The above test for matching number of elements must be checked recursively
            if (!(*static_cast< container_type<value_type>* >(container_) == *rhs.container<value_type, 0>())) return false;
            return next().eq(rhs.next());
        }

        /*!
        * \brief Same as operator!=() but strictly enforces container element size matching.
        */
        bool ne(const heterostack<value_type, Types...>& rhs)
        {
            return !eq(rhs);
        }

        /*!
        * \brief Same as operator<() but strictly enforces container element size matching.
        */
        bool lt(const heterostack<value_type, Types...>& rhs)
        {
            // if zero elements, cannot be less than
            if (static_cast<container_type<value_type>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;
            if (!(*static_cast< container_type<value_type>* >(container_) < *rhs.container<value_type, 0>())) return false;
            return next().lt(rhs.next());
        }

        /*!
        * \brief Same as operator>() but strictly enforces container element size matching.
        */
        bool gt(const heterostack<value_type, Types...>& rhs)
        {
            // if zero elements, cannot be greater than
            if (static_cast<container_type<value_type>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;

            if (!(*static_cast< container_type<value_type>* >(container_) > *rhs.container<value_type, 0>())) return false;
            return next().gt(rhs.next());
        }

        /*!
        * \brief Same as operator<=() but strictly enforces container element size matching.
        */
        bool lte(const heterostack<value_type, Types...>& rhs)
        {
            // if zero elements, cannot be less than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;

            if (!(*static_cast< container_type<value_type>* >(container_) <= *rhs.container<value_type, 0>())) return false;
            return next().lte(rhs.next());
        }

        /*!
        * \brief Same as operator>=() but strictly enforces container element size matching.
        */
        bool gte(const heterostack<value_type, Types...>& rhs)
        {
            // if zero elements, cannot be greater than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;

            if (!(*static_cast< container_type<value_type>* >(container_) >= *rhs.container<value_type, 0>())) return false;
            return next().gte(rhs.next());
        }

    public:
        // Constructors & Destructors
        heterostack() : container_(new container_type<value_type>), next_(nullptr), counter_(nullptr)
        {
            //First, call private constructor for all elements downstream (init list)
            //Private constructor does not allocate memory for counter_

            //Then, FOR FOR FIRST ELEMENT ONLY, allocate memory for counter_
            counter_ = new size_t;
            *counter_ = 0;

            //next().setcounter() only needs to be called from first element.
            //It will recursively set the counter_ pointer for all elements downstream.
            next().setcounter(counter_);
        };

        ~heterostack() {}; //counter_ is deallocated in heterostack<>

    private:
        heterostack(size_t* pntr) : container_(new container_type<value_type>), next_(pntr), counter_(pntr)
        { /*this constructor does not allocate memory for counter_*/ };

    public:
        // Capacity
        /*!
        * \brief Returns the total number of elements in object.
        */
        size_t size()
        {
            size_t result = 1;
            next().size(result);

            return result;
        }

    private:
        void size(size_t& val)
        {
            next().size(++val);
        }

    public:
        /*!
        * \brief Returns number of elements for the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        size_t size() const
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->size();
                }
                ++(*counter_);
            }

            return next().template size<U, N>();
        }

        /*!
        * \brief Resizes whether the Nth container of type U is empty.
        */
        template <typename U, size_t N = 0>
        bool empty()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->empty();
                }
                ++(*counter_);
            }

            return next().template empty<U, N>();
        }

        /*!
        * \brief Returns whether object stores type U.
        */
        template <typename U>
        bool contains()
        {
            if (typeid(U) == typeid(value_type)) return true;
            return next().template contains<U>();
        }

        /*!
        * \brief Returns the number of containers with type U.
        */
        template <typename U>
        size_t multiplicity()
        {
            size_t result = 0;
            next().template multiplicity<U>(result);

            if (typeid(U) == typeid(value_type))
                ++result;

            return result;
        }

    private:
        template <typename U>
        void multiplicity(size_t& val)
        {
            next().template multiplicity<U>(val);

            if (typeid(U) == typeid(value_type))
                ++val;
        }

     public:
        /*!
        * \brief Returns reference to the top element of the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        U& top()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->top();
                }
                ++(*counter_);
            }

            return next().template top<U, N>();
        }

        /*!
        * \brief Returns const reference to the top element of the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        const U& top() const
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->top();
                }
                ++(*counter_);
            }

            return next().template top<U, N>();
        }

        /*!
        * \brief Returns reference to the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        container_type<U>* container()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_);
                }
                ++(*counter_);
            }

            return next().template container<U, N>();
        }

        /*!
        * \brief Returns const reference to the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        const container_type<U>* container() const
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< const container_type<U>* >(container_);
                }
                ++(*counter_);
            }

            return next().template container<U, N>();
        }

        /*!
        * \brief Returns std::type_index of items within the Nth container in object.
        */
        template <size_t N = 0>
        std::type_index type()
        {
            if (*counter_ == N)
            {
                *counter_ = 0;
                return std::type_index(typeid(value_type));
            }

            ++(*counter_);

            return next().template type<N>();
        }

        // Modifiers
        /*!
        * \brief Adds val at the top element of the Nth container of type U.
        */
        template<typename U, size_t N = 0, typename V>
        void push(const V& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->push(val);
                    return;
                }
                ++(*counter_);
            }

            return next().template push<U, N>(val);
        }

        /*!
        * \brief Adds val as the top element of the Nth container of type U.
        */
        template<typename U, size_t N = 0, typename V>
        void push(V&& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->push(val);
                    return;
                }
                ++(*counter_);
            }

            return next().template push<U, N>(val);
        }

        /*!
        * \brief Removes the top element of the Nth container of type U.
        */
        template<typename U, size_t N = 0>
        void pop()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->pop();
                    return;
                }
                ++(*counter_);
            }

            return next().template pop<U, N>();
        }

        /*!
        * \brief Swaps the contents of the Nth container of type U with that of x.
        */
        template<typename U, size_t N = 0>
        void swap(container_type<U>& x)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->swap(x);
                    return;
                }
                ++(*counter_);
            }

            return next().template swap<U, N>(x);
        }

        /*!
        * \brief Swaps contents of object with x.
        */
        void swap(heterostack<value_type, Types...>& x)
        {
            void* temp = container_;
            container_ = x.container_;
            x.container_ = temp;

            next().swap(x.next());
        }

        /*!
        * \brief The Nth container of type U is extended by inserting a new element at the top of container.
        */
        template<typename U, size_t N = 0, typename... Args>
        void emplace(Args&&... args)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast<container_type<U>*>(container_)->emplace(args...);
                    return;
                }
                ++(*counter_);
            }

            next().template emplace<U, N>(args...);
        }

    private:
        void* container_; //pointer to container_type<value_type>
        heterostack<Types...> next_;
        size_t* counter_;

        // Helper Methods
        heterostack<Types...>& next()
        {
            return next_;
        }

        const heterostack<Types...>& next() const
        {
            return next_;
        }

        void setcounter(size_t*& pntr)
        {
            counter_ = pntr;
            next().setcounter(pntr);
        }
    };

    /*!
    * \cond Skip Doxygen documentation of this specialization.
    */
	template<typename T>
    class heterostack<T>
    {
        // Friends
        template<typename... Args> friend class heterostack;

    public:
        // Typedefs
        typedef T value_type;

        template<typename U>
        using container_type = std::stack<U>;

        // Relational Operators & Methods
        // =========================================================================
        // all operators need to be implemented using the containers native
        // relational operators.  Defining relational operators based on one another
        // does not produce expected results.
        // std::stack uses lexographical comparison
        // User must be aware of this as it may not produce expected result
        // =========================================================================

        /*!
        * \brief Returns true if == operator evaluates to true for each element in object.
        */
        bool operator==(const heterostack<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) == *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Returns true if == operator evaluates to false for any element in object.
        */
        bool operator!=(const heterostack<value_type>& rhs)
        {
            return !operator==(rhs);
        }

        /*!
        * \brief Returns true if < operator evaluates to true for each element in object.
        */
        bool operator<(const heterostack<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) < *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Returns true if > operator evaluates to true for each element in object.
        */
        bool operator>(const heterostack<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) > *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Returns true if <= operator evaluates to true for each element in object.
        */
        bool operator<=(const heterostack<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) <= *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Returns true if >= operator evaluates to true for each element in object.
        */
        bool operator>= (const heterostack<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) >= *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Same as operator==() but strictly enforces container element size matching.
        */
        bool eq(const heterostack<value_type>& rhs)
        {
            // number of elements must match
            if ( static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size() ) return false;
            return *static_cast< container_type<value_type>* >(container_) == *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Same as operator!=() but strictly enforces container element size matching.
        */
        bool ne(const heterostack<value_type>& rhs)
        {
            return !eq(rhs);
        }

        /*!
        * \brief Same as operator<() but strictly enforces container element size matching.
        */
        bool lt(const heterostack<value_type>& rhs)
        {
            // if zero elements, cannot be less than
            if (static_cast<container_type<value_type>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;
            return *static_cast< container_type<value_type>* >(container_) < *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Same as operator>() but strictly enforces container element size matching.
        */
        bool gt(const heterostack<value_type>& rhs)
        {
            // if zero elements, cannot be greater than
            if (static_cast<container_type<value_type>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;
            return *static_cast< container_type<value_type>* >(container_) > *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Same as operator<=() but strictly enforces container element size matching.
        */
        bool lte(const heterostack<value_type>& rhs)
        {
            // if zero elements, cannot be less than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;
            return *static_cast< container_type<value_type>* >(container_) <= *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Same as operator>=() but strictly enforces container element size matching.
        */
        bool gte(const heterostack<value_type>& rhs)
        {
            // if zero elements, cannot be greater than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;
            return *static_cast< container_type<value_type>* >(container_) >= *rhs.container<value_type, 0>();
        }

        // Constructors & Destructors
        heterostack() : container_(new container_type<value_type>), counter_(nullptr)
        {
            counter_ = new size_t;
            *counter_ = 0;
        };

        ~heterostack()
		{
			if( counter_ != nullptr ) delete counter_;
		};

    private:
        heterostack(size_t* pntr) : container_(new container_type<value_type>), counter_(pntr)
        { /*this constructor does not allocate memory for counter_*/ };

    public:
        // Capacity
        /*!
        * \brief Returns the total number of elements in object.
        */
        size_t size() { return 1; }

    private:
        void size(size_t& val) { ++val; }

    public:
        /*!
        * \brief Returns number of elements for the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        size_t size() const
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->size();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Resizes whether the Nth container of type U is empty.
        */
        template <typename U, size_t N = 0>
        bool empty()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->empty();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns whether object stores type U.
        */
        template <typename U>
        bool contains()
        {
            return ( typeid(U) == typeid(value_type) );
        }

        /*!
        * \brief Returns the number of containers with type U.
        */
        template <typename U>
        size_t multiplicity()
        {
            return (typeid(U) == typeid(value_type)) ? 1 : 0;
        }

    private:
        template <typename U>
        void multiplicity(size_t& val)
        {
            if (typeid(U) == typeid(value_type)) ++val;
        }

     public:
        // Element Access
        /*!
        * \brief Returns reference to the top element of the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        U& top()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->top();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns const reference to the top element of the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        const U& top() const
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->top();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns reference to the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        container_type<U>* container()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns const reference to the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        const container_type<U>* container() const
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< const container_type<U>* >(container_);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns std::type_index of items within the Nth container in object.
        */
        template <size_t N = 0>
        std::type_index type()
        {
            if (*counter_ == N)
            {
                *counter_ = 0;
                return std::type_index(typeid(value_type));
            }
            *counter_ = 0;

            throw std::out_of_range(std::string("Element N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        // Modifiers
        /*!
        * \brief Adds val as the top element of the Nth container of type U.
        */
        template<typename U, size_t N = 0, typename V>
        void push(const V& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->push(val);
                    return;
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Adds val as the top element of the Nth container of type U.
        */
        template<typename U, size_t N = 0, typename V>
        void push(V&& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->push(val);
                    return;
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Removes the top element of the Nth container of type U.
        */
        template<typename U, size_t N = 0>
        void pop()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->pop();
                    return;
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Swaps the contents of the Nth container of type U with that of x.
        */
        template<typename U, size_t N = 0>
        void swap(container_type<U>& x)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->swap(x);
                    return;
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Swaps contents of object with x.
        */
        void swap(heterostack<T>& x)
        {
            void* temp = container_;
            container_ = x.container_;
            x.container_ = temp;
        }

        /*!
        * \brief The Nth container of type U is extended by inserting a new element at the top of the container.
        */
        template<typename U, size_t N = 0, typename... Args>
        void emplace(Args&&... args)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast<container_type<U>*>(container_)->emplace(args...);
                    return;
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

    private:
        void* container_; //pointer to container_type<value_type>
        size_t* counter_;

        // Helper Methods
        void setcounter(size_t*& pntr)
        {
            counter_ = pntr;
        }
    };
    /*!
    * \endcond
    */


    template<typename... Args_lhs, typename... Args_rhs>
    bool operator==(const heterostack<Args_lhs...>& lhs, const heterostack<Args_rhs...>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator!=(const heterostack<Args_lhs...>& lhs, const heterostack<Args_rhs...>& rhs)
    {
        return lhs.operator!=(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator<(const heterostack<Args_lhs...>& lhs, const heterostack<Args_rhs...>& rhs)
    {
        return lhs.operator<(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator<=(const heterostack<Args_lhs...>& lhs, const heterostack<Args_rhs...>& rhs)
    {
        return lhs.operator<=(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator>(const heterostack<Args_lhs...>& lhs, const heterostack<Args_rhs...>& rhs)
    {
        return lhs.operator>(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator>=(const heterostack<Args_lhs...>& lhs, const heterostack<Args_rhs...>& rhs)
    {
        return lhs.operator>=(rhs);
    }
}

#endif // HETEROGENEOUS_HETEROSTACK_HPP