#ifndef HETEROGENEOUS_HETEROVECTOR_HPP
#define HETEROGENEOUS_HETEROVECTOR_HPP

/*
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*
* Copyright (c) 2015 Hirotatsu Armstrong
*/

/*!
* \file heterovector.hpp
*
* This templated class replicates the features of an std::vector
* that can handle heterogeneous data types.
*/

#include <stdexcept>
#include <typeinfo>
#include <typeindex>
#include <vector>
#include <string>

namespace heterogeneous
{
    /*!
    * \cond Skip Doxygen documentation of this forward declaration.
    */
    template<typename... Types> class heterovector;
    /*!
    * \endcond
    */

    template<typename T, typename... Types>
    class heterovector<T, Types...>
    {
        // Friends
        template<typename... Args> friend class heterovector;

    public:
        // Typedefs
        typedef T value_type;

        template<typename U>
        using container_type = std::vector<U>;

        // Operators
        heterovector<T, Types...>& operator=(const heterovector<T, Types...>& x)
        {
            setEQUALTO(x);
            return *this;
        }

    private:
        void setEQUALTO(const heterovector<T, Types...>& x)
        {
            //copy contents of x into object
            if (container_ != nullptr) delete container_;
            container_ = new container_type<T>(*static_cast< container_type<T>* >(x.container_) );
            next().setEQUALTO(x.next());
        }

    public:
        // Relational Operators & Methods
        // =========================================================================
        // all operators need to be implemented using the containers native
        // relational operators.  Defining relational operators based on one another
        // does not produce expected results.
        // std::vector uses lexographical comparison
        // User must be aware of this as it may not produce expected result
        // =========================================================================

		/*!
        * \brief Returns true if == operator evaluates to true for each element in object.
        */
        bool operator==(const heterovector<T, Types...>& rhs)
        {
            if (!(*static_cast< container_type<T>* >(container_) == *rhs.container<T, 0>())) return false;
            return next().operator==(rhs.next());
        }

		/*!
        * \brief Returns true if == operator evaluates to false for any element in object.
        */
        bool operator!=(const heterovector<T, Types...>& rhs)
        {
            return !operator==(rhs);
        }
		
		/*!
        * \brief Returns true if < operator evaluates to true for each element in object.
        */
        bool operator<(const heterovector<T, Types...>& rhs)
        {
            if (!(*static_cast< container_type<T>* >(container_) < *rhs.container<T, 0>())) return false;
            return next().operator<(rhs.next());
        }

		/*!
        * \brief Returns true if > operator evaluates to true for each element in object.
        */
        bool operator>(const heterovector<T, Types...>& rhs)
        {
            if (!(*static_cast< container_type<T>* >(container_) > *rhs.container<T, 0>())) return false;
            return next().operator>(rhs.next());
        }

		/*!
        * \brief Returns true if <= operator evaluates to true for each element in object.
        */
        bool operator<=(const heterovector<T, Types...>& rhs)
        {
            if (!(*static_cast< container_type<T>* >(container_) <= *rhs.container<T, 0>())) return false;
            return next().operator<=(rhs.next());
        }

		/*!
        * \brief Returns true if >= operator evaluates to true for each element in object.
        */
        bool operator>= (const heterovector<T, Types...>& rhs)
        {
            if (!(*static_cast< container_type<T>* >(container_) >= *rhs.container<T, 0>())) return false;
            return next().operator>=(rhs.next());
        }

		/*!
        * \brief Same as operator==() but strictly enforces container element size matching.
        */
        bool eq(const heterovector<T, Types...>& rhs)
        {
            // number of elements must match
            if (static_cast<container_type<T>*>(container_)->size() != rhs.container<T, 0>()->size()) return false;

            // Although this code is duplicated in operator==, we cannot call operator== here as a replacement.
            // The above test for matching number of elements must be checked recursively
            if (!(*static_cast< container_type<T>* >(container_) == *rhs.container<T, 0>())) return false;
            return next().eq(rhs.next());
        }

		/*!
        * \brief Same as operator!=() but strictly enforces container element size matching.
        */
        bool ne(const heterovector<T, Types...>& rhs)
        {
            return !eq(rhs);
        }
		
		/*!
        * \brief Same as operator<() but strictly enforces container element size matching.
        */
        bool lt(const heterovector<T, Types...>& rhs)
        {
            // if zero elements, cannot be less than
            if (static_cast<container_type<T>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<T>*>(container_)->size() != rhs.container<T, 0>()->size()) return false;
            if (!(*static_cast< container_type<T>* >(container_) < *rhs.container<T, 0>())) return false;
            return next().lt(rhs.next());
        }

		/*!
        * \brief Same as operator>() but strictly enforces container element size matching.
        */
        bool gt(const heterovector<T, Types...>& rhs)
        {
            // if zero elements, cannot be greater than
            if (static_cast<container_type<T>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<T>*>(container_)->size() != rhs.container<T, 0>()->size()) return false;

            if (!(*static_cast< container_type<T>* >(container_) > *rhs.container<T, 0>())) return false;
            return next().gt(rhs.next());
        }

		/*!
        * \brief Same as operator<=() but strictly enforces container element size matching.
        */
        bool lte(const heterovector<T, Types...>& rhs)
        {
            // if zero elements, cannot be less than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<T>*>(container_)->size() != rhs.container<T, 0>()->size()) return false;

            if (!(*static_cast< container_type<T>* >(container_) <= *rhs.container<T, 0>())) return false;
            return next().lte(rhs.next());
        }

		/*!
        * \brief Same as operator>=() but strictly enforces container element size matching.
        */
        bool gte(const heterovector<T, Types...>& rhs)
        {
            // if zero elements, cannot be greater than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<T>*>(container_)->size() != rhs.container<T, 0>()->size()) return false;

            if (!(*static_cast< container_type<T>* >(container_) >= *rhs.container<T, 0>())) return false;
            return next().gte(rhs.next());
        }       

    public:
        // Constructors & Destructors
        heterovector() : container_(new container_type<T>), next_(nullptr), counter_(nullptr)
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

        ~heterovector() {}; //counter_ is deallocated in heterovector<>

    private:
        heterovector(size_t* pntr) : container_(new container_type<T>), next_(pntr), counter_(pntr)
        { /*this constructor does not allocate memory for counter_*/ };

    public:
        // Iterators
        template <typename U, size_t N = 0>
        typename container_type<U>::iterator begin()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->begin();
                }
                ++(*counter_);
            }

            // use of next().template begin<U, N>();
            // with the additional 'template',
            // rather than next().begin<U, N>();
            // is to make the clang compiler happy
            // it wants an explicit declaration that
            // begin depends on template parameters.
            return next().template begin<U, N>();
        }

        template <typename U, size_t N = 0>
        typename container_type<U>::iterator end()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->end();
                }
                ++(*counter_);
            }

            return next().template end<U, N>();
        }

        template <typename U, size_t N = 0>
        typename container_type<U>::reverse_iterator rbegin()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->rbegin();
                }
                ++(*counter_);
            }

            return next().template rbegin<U, N>();
        }

        template <typename U, size_t N = 0>
        typename container_type<U>::reverse_iterator rend()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->rend();
                }
                ++(*counter_);
            }

            return next().template rend<U, N>();
        }

        template <typename U, size_t N = 0>
        typename container_type<U>::const_iterator cbegin()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->cbegin();
                }
                ++(*counter_);
            }

            return next().template cbegin<U, N>();
        }

        template <typename U, size_t N = 0>
        typename container_type<U>::const_iterator cend()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->cend();
                }
                ++(*counter_);
            }

            return next().template cend<U, N>();
        }

        template <typename U, size_t N = 0>
        typename container_type<U>::const_reverse_iterator crbegin()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->crbegin();
                }
                ++(*counter_);
            }

            return next().template crbegin<U, N>();
        }

        template <typename U, size_t N = 0>
        typename container_type<U>::const_reverse_iterator crend()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->crend();
                }
                ++(*counter_);
            }

            return next().template crend<U, N>();
        }

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
        * \brief Returns the maximum number of elements the Nth container of type U can hold.
        */
        template <typename U, size_t N = 0>
        size_t max_size()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->max_size();
                }
                ++(*counter_);
            }

            return next().template max_size<U, N>();
        }

        /*!
        * \brief Resizes the Nth container of type U so that it contains at least n elements.
        */
        template <typename U, size_t N = 0>
        void resize(size_t n)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->resize(n);
                }
                ++(*counter_);
            }

            return next().template resize<U, N>(n);
        }

        /*!
        * \brief Resizes the Nth container of type U so that it contains at least n elements.
        */
        template <typename U, size_t N = 0>
        void resize(size_t n, const U& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->resize(n, val);
                }
                ++(*counter_);
            }

            return next().template resize<U, N>(n, val);
        }

        /*!
        * \brief Returns the number of elements the Nth container of type U can store before reallocation.
        */
        template <typename U, size_t N = 0>
        size_t capacity()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->capacity();
                }
                ++(*counter_);
            }

            return next().template capacity<U, N>();
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
        * \brief Requests the capacity of Nth container of type U is enough to contain at least n elements.
        */
        template <typename U, size_t N = 0>
        void reserve(size_t n)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->reserve(n);
                }
                ++(*counter_);
            }

            return next().template reserve<U, N>(n);
        }

        /*!
        * \brief Resizes the Nth container of type U reduce its capacity to fit its size.
        */
        template <typename U, size_t N = 0>
        void shrink_to_fit()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->shrink_to_fit();
                }
                ++(*counter_);
            }

            return next().template shrink_to_fit<U, N>();
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
        // Element Access
        /*!
        * \brief Returns reference to the nth element of the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        U& at(size_t n)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->at(n);
                }
                ++(*counter_);
            }

            return next().template at<U, N>(n);
        }

        /*!
        * \brief Returns const reference to the nth element of the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        const U& at(size_t n) const
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->at(n);
                }
                ++(*counter_);
            }

            return next().template at<U, N>(n);
        }

        /*!
        * \brief Returns reference to the first element of the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        U& front()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->front();
                }
                ++(*counter_);
            }

            return next().template front<U, N>();
        }

        /*!
        * \brief Returns const reference to the first element of the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        const U& front() const
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->front();
                }
                ++(*counter_);
            }

            return next().template front<U, N>();
        }

        /*!
        * \brief Returns reference to the last element of the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        U& back()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->back();
                }
                ++(*counter_);
            }

            return next().template back<U, N>();
        }

        /*!
        * \brief Returns const reference to the last element of the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        const U& back() const
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->back();
                }
                ++(*counter_);
            }

            return next().template back<U, N>();
        }

        /*!
        * \brief Returns pointer to the data contained in the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        U* data()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->data();
                }
                ++(*counter_);
            }

            return next().template data<U, N>();
        }

        /*!
        * \brief Returns const pointer to the data contained in the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        const U* data() const
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->data();
                }
                ++(*counter_);
            }

            return next().template data<U, N>();
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
        * \brief Assigns new contents to the Nth container of type U, replacing its current contents, and modifying its size accordingly.
        */
        template<typename U, size_t N = 0, typename InputIterator>
        void assign(InputIterator first, InputIterator last)
        {
            if (typeid(U) == typeid(value_type))
            {
                static_cast< container_type<U>* >(container_)->assign(first, last);
                return;
            }

            return next().template assign<U, N>(first, last);
        }

        /*!
        * \brief Assigns new contents to the Nth container of type U, replacing its current contents, and modifying its size accordingly.
        */
        template<typename U, size_t N = 0>
        void assign(size_t n, const U& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->assign(n, val);
                    return;
                }
                ++(*counter_);
            }

            return next().template assign<U, N>(n, val);
        }

        /*!
        * \brief Assigns new contents to the Nth container of type U, replacing its current contents, and modifying its size accordingly.
        */
        template<typename U, size_t N = 0>
        void assign(std::initializer_list<U> il)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->assign(il);
                    return;
                }
                ++(*counter_);
            }

            return next().template assign<U, N>(il);
        }

        /*!
        * \brief Adds val as the last element of the Nth container of type U.
        */
        template<typename U, size_t N = 0, typename V>
        void push_back(const V& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->push_back(val);
                    return;
                }
                ++(*counter_);
            }

            return next().template push_back<U, N>(val);
        }

        /*!
        * \brief Adds val as the last element of the Nth container of type U.
        */
        template<typename U, size_t N = 0, typename V>
        void push_back(V&& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->push_back(val);
                    return;
                }
                ++(*counter_);
            }

            return next().template push_back<U, N>(val);
        }

        /*!
        * \brief Removes the last element of the Nth container of type U.
        */
        template<typename U, size_t N = 0>
        void pop_back()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->pop_back();
                    return;
                }
                ++(*counter_);
            }

            return next().template pop_back<U, N>();
        }

        /*!
        * \brief Inserts val into the Nth container of type U at the location specified by position.
        */
        template<typename U, size_t N = 0>
        void insert(typename container_type<U>::const_iterator position, const U& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->insert(position, val);
                    return;
                }
                ++(*counter_);
            }

            return next().template insert<U, N>(position, val);
        }

        /*!
        * \brief Inserts val into the Nth container of type U at the location specified by position.
        */
        template<typename U, size_t N = 0>
        void insert(typename container_type<U>::const_iterator position, U&& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->insert(position, val);
                    return;
                }
                ++(*counter_);
            }

            return next().template insert<U, N>(position, val);
        }

        /*!
        * \brief Inserts n copies of val into the Nth container of type U, starting at the location specified by position.
        */
        template<typename U, size_t N = 0>
        void insert(typename container_type<U>::const_iterator position, size_t n, const U& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->insert(position, n, val);
                    return;
                }
                ++(*counter_);
            }

            return next().template insert<U, N>(position, n, val);
        }

        /*!
        * \brief Inserts copies of the values between [first,last) into the Nth container of type U, starting at the location specified by position.
        */
        template<typename U, size_t N = 0, typename InputIterator>
        void insert(typename container_type<U>::const_iterator position, InputIterator first, InputIterator last)
        {
            if (typeid(U) == typeid(value_type))
            {
                static_cast< container_type<U>* >(container_)->insert(position, first, last);
                return;
            }

            return next().template insert<U, N>(position, first, last);
        }


        /*!
        * \brief Inserts il into the Nth container of type U, starting at the location specified by position.
        */
        template<typename U, size_t N = 0>
        void insert(typename container_type<U>::const_iterator position, std::initializer_list<U> il)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->insert(position, il);
                    return;
                }
                ++(*counter_);
            }

            next().template insert<U, N>(position, il);
        }

        /*!
        * \brief Removes the element specified by position in the Nth container of type U.
        */
        template<typename U, size_t N = 0>
        void erase(typename container_type<U>::const_iterator position)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->erase(position);
                    return;
                }
                ++(*counter_);
            }

            next().template erase<U, N>(position);
        }

        /*!
        * \brief Removes the range of elements between locations specified by first and last in the Nth container of type U.
        */
        template<typename U, size_t N = 0>
        void erase(typename container_type<U>::const_iterator first, typename container_type<U>::const_iterator last)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->erase(first, last);
                    return;
                }
                ++(*counter_);
            }

            return next().template erase<U, N>(first, last);
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
        void swap(heterovector<T, Types...>& x)
        {
            void* temp = container_;
            container_ = x.container_;
            x.container_ = temp;

            next().swap(x.next());
        }

        /*!
        * \brief Erases the of contents the Nth container of type U.
        */
        template<typename U, size_t N = 0>
        void clear()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->clear();
                    return;
                }
                ++(*counter_);
            }

            return next().template clear<U, N>();
        }

        /*!
        * \brief The Nth container of type U is extended by inserting a new element at position.
        */
        template<typename U, size_t N = 0, typename... Args>
        void emplace(typename container_type<U>::const_iterator position, Args&&... args)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->emplace(position, args...);
                    return;
                }
                ++(*counter_);
            }

            next().template emplace<U, N>(position, args...);
        }

        /*!
        * \brief The Nth container of type U is extended by inserting a new element after its current last element.
        */
        template<typename U, size_t N = 0, typename... Args>
        void emplace_back(Args&&... args)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast<container_type<U>*>(container_)->emplace_back(args...);
                    return;
                }
                ++(*counter_);
            }

            next().template emplace_back<U, N>(args...);
        }

        /*!
        * \brief Sets the contents of the Nth container of type U to those in x.
        */
        template<typename U, size_t N = 0>
        container_type<U>& set(const container_type<U>& x)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return *static_cast< container_type<U>* >(container_) = x;
                }
                ++(*counter_); //increment index
            }

            return next().template set<U, N>(x);
        }

        /*!
        * \brief Sets the contents of the Nth container of type U to those in x.
        */
        template<typename U, size_t N = 0>
        container_type<U>& set(container_type<U>&& x)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N) // check to see if we are at the Nth container of type U
                {
                    *counter_ = 0; // if so, reset counter to zero to get ready for next request
                    return *static_cast< container_type<U>* >(container_) = x;
                }
                ++(*counter_); // if not, increment counter
            }

            return next().template set<U, N>(x);
        }

        /*!
        * \brief Sets the contents of the Nth container of type U to il.
        */
        template<typename U, size_t N = 0>
        container_type<U>& set(std::initializer_list<U> il)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return *static_cast< container_type<U>* >(container_) = il;
                }
                ++(*counter_);
            }

            return next().template setU, N>(il);
        }

        // Allocator
        /*!
        * \brief Returns a copy of the allocator object associated with the Nth container of type U.
        */
        template<typename U, size_t N = 0>
        typename container_type<U>::allocator_type get_allocator() const
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->get_allocator();
                }
                ++(*counter_);
            }

            return next().template get_allocator<U, N>();
        }

    private:
        void* container_; //pointer to container_type<value_type>
        heterovector<Types...> next_;
        size_t* counter_;

        // Helper Methods
        heterovector<Types...>& next()
        {
            return next_;
        }

        const heterovector<Types...>& next() const
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
    // Entire heterovector<> specialization marked private
    template<> class heterovector<>
    {
        // Friends
        template<typename... Args> friend class heterovector;

        template<typename T>
        using container_type = std::vector <T>;

        heterovector() : counter_(nullptr) {};

        ~heterovector()
        {
            if (counter_ != nullptr) delete counter_;
        }

        heterovector(size_t* pntr) : counter_(nullptr) {};

        // Operator Helpers
        void setEQUALTO(const heterovector<>& x) {}

        // Relational Methods
        bool operator==(const heterovector<>& rhs)
        { return true; }

        bool operator<(const heterovector<>& rhs)
        { return true; }
		
        bool operator>(const heterovector<>& rhs)
        { return true; }

        bool operator<=(const heterovector<>& rhs)
        { return true; }
		
        bool operator>=(const heterovector<>& rhs)
        { return  true; }

		bool eq(const heterovector<>& rhs)
        { return true; }
		
		bool lt(const heterovector<>& rhs)
        { return true; }
		
		bool gt(const heterovector<>& rhs)
        { return true; }
		
		bool lte(const heterovector<>& rhs)
        { return true; }
		
		bool gte(const heterovector<>& rhs)
        { return true; }
		
        // Iterators
        template<typename U, size_t N = 0>
        typename container_type<U>::iterator begin()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        typename container_type<U>::iterator end()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        typename container_type<U>::reverse_iterator rbegin()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        typename container_type<U>::reverse_iterator rend()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        typename container_type<U>::const_iterator cbegin()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        typename container_type<U>::const_iterator cend()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        typename container_type<U>::const_reverse_iterator crbegin()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        typename container_type<U>::const_reverse_iterator crend()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        // Capacity
        template <typename U, size_t N = 0>
        size_t size() const
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        size_t size() { return 0; }

        void size(size_t& val) {}

        template <typename U, size_t N = 0>
        size_t max_size()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        void resize(size_t n)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        void resize(size_t n, const U& val)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        size_t capacity()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        bool empty()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        void reserve(size_t n)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        void shrink_to_fit() const
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U>
        bool contains() { return false; }

        template <typename U>
        size_t multiplicity() { return 0; }

        template <typename U>
        void multiplicity(size_t& val) {}

        // Element Access
        template <typename U, size_t N = 0>
        U& at(size_t n)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        const U& at(size_t n) const
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        U& front()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        const U& front() const
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        U& back()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        const U& back() const
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        U* data()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        const U* data() const
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        container_type<U>* container()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <typename U, size_t N = 0>
        const container_type<U>* container() const
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template <size_t N = 0>
        std::type_index type()
        { throw std::invalid_argument(std::string("N=") + std::to_string(N) + std::string(" elements do not exist in object.")); }

        // Modifiers
        template<typename U, size_t N = 0, typename InputIterator>
        void assign(InputIterator first, InputIterator last)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        void assign(size_t n, const U& val)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        void assign(std::initializer_list<U> il)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        void push_back(const U& val)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        void push_back(U&& val)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        void pop_back()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        void insert(typename container_type<U>::const_iterator position, const U& val)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        void insert(typename container_type<U>::const_iterator position, size_t n, const U& val)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0, typename InputIterator>
        void insert(typename container_type<U>::const_iterator position, InputIterator first, InputIterator last)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        void insert(typename container_type<U>::const_iterator position, U&& val)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        void insert(typename container_type<U>::const_iterator position, std::initializer_list<U> il)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        void erase(typename container_type<U>::const_iterator position)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        void erase(typename container_type<U>::const_iterator first, typename container_type<U>::const_iterator last)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        void swap(container_type<U>& x)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        void swap(heterovector<>& x) {}

        template<typename U, size_t N = 0>
        void clear()
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0, typename... Args>
        void emplace(typename container_type<U>::const_iterator position, Args&&... val)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0, typename... Args>
        void emplace_back(Args&&... args)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        container_type<U>& set(const container_type<U>& x)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U, size_t N = 0>
        container_type<U>& set(const container_type<U>&& x)
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        template<typename U>
        container_type<U>& set(std::initializer_list<U> il)
        { throw std::invalid_argument(std::string("Type ") + std::string(" does not exist in object.")); }

        // Allocator
        template<typename U, size_t N = 0>
        typename container_type<U>::allocator_type get_allocator() const
        { throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object.")); }

        size_t* counter_;

        void setcounter(size_t*& pntr)
        {
            counter_ = pntr;
        }
    };
	/*!
    * \endcond
    */

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator==(const heterovector<Args_lhs...>& lhs, const heterovector<Args_rhs...>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator!=(const heterovector<Args_lhs...>& lhs, const heterovector<Args_rhs...>& rhs)
    {
        return lhs.operator!=(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator<(const heterovector<Args_lhs...>& lhs, const heterovector<Args_rhs...>& rhs)
    {
        return lhs.operator<(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator<=(const heterovector<Args_lhs...>& lhs, const heterovector<Args_rhs...>& rhs)
    {
        return lhs.operator<=(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator>(const heterovector<Args_lhs...>& lhs, const heterovector<Args_rhs...>& rhs)
    {
        return lhs.operator>(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator>=(const heterovector<Args_lhs...>& lhs, const heterovector<Args_rhs...>& rhs)
    {
        return lhs.operator>=(rhs);
    }
}

#endif // HETEROGENEOUS_HETEROVECTOR_HPP