#ifndef HETEROGENEOUS_HETEROLIST_HPP
#define HETEROGENEOUS_HETEROLIST_HPP

/*
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*
* Copyright (c) 2015 Hirotatsu Armstrong
*/

/*!
* \file heterolist.hpp
*
* This templated class replicates the features of an std::list
* that can handle heterogeneous data types.
*/

#include <stdexcept>
#include <typeinfo>
#include <typeindex>
#include <list>
#include <string>

namespace heterogeneous
{
    /*!
    * \cond Skip Doxygen documentation of this forward declaration.
    */
    template<typename... Types> class heterolist;
    /*!
    * \endcond
    */

    template<typename T, typename... Types>
    class heterolist<T, Types...>
    {
        // Friends
        template<typename... Args> friend class heterolist;
		
    public:
        // Typedefs
        typedef T value_type;

        template<typename U>
        using container_type = std::list<U>;

        // Operators
        /*!
        * \brief Assigns contents of rhs into object.
        */
        heterolist<value_type, Types...>& operator=(const heterolist<value_type, Types...>& rhs)
        {
            setEQUALTO(rhs);
            return *this;
        }

    private:
        void setEQUALTO(const heterolist<value_type, Types...>& x)
        {
            //copy contents of x to object
            if (container_ != nullptr) delete container_;
            container_ = new container_type<value_type>(*static_cast< container_type<value_type>* >(x.container_) );
            next().setEQUALTO(x.next());
        }

    public:
        // Relational Operators & Methods
        // =========================================================================
        // all operators need to be implemented using the containers native
        // relational operators.  Defining relational operators based on one another
        // does not produce expected results.
        // std::list uses lexographical comparison
        // User must be aware of this as it may not produce expected result
        // =========================================================================

        /*!
        * \brief Returns true if == operator evaluates to true for each element in object.
        */
        bool operator==(const heterolist<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) == *rhs.container<value_type, 0>())) return false;
            return next().operator==(rhs.next());
        }

        /*!
        * \brief Returns true if == operator evaluates to false for any element in object.
        */
        bool operator!=(const heterolist<value_type, Types...>& rhs)
        {
            return !operator==(rhs);
        }

        /*!
        * \brief Returns true if < operator evaluates to true for each element in object.
        */
        bool operator<(const heterolist<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) < *rhs.container<value_type, 0>())) return false;
            return next().operator<(rhs.next());
        }

        /*!
        * \brief Returns true if > operator evaluates to true for each element in object.
        */
        bool operator>(const heterolist<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) > *rhs.container<value_type, 0>())) return false;
            return next().operator>(rhs.next());
        }

        /*!
        * \brief Returns true if <= operator evaluates to true for each element in object.
        */
        bool operator<=(const heterolist<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) <= *rhs.container<value_type, 0>())) return false;
            return next().operator<=(rhs.next());
        }

        /*!
        * \brief Returns true if >= operator evaluates to true for each element in object.
        */
        bool operator>= (const heterolist<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) >= *rhs.container<value_type, 0>())) return false;
            return next().operator>=(rhs.next());
        }

        /*!
        * \brief Same as operator==() but strictly enforces container element size matching.
        */
        bool eq(const heterolist<value_type, Types...>& rhs)
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
        bool ne(const heterolist<value_type, Types...>& rhs)
        {
            return !eq(rhs);
        }

        /*!
        * \brief Same as operator<() but strictly enforces container element size matching.
        */
        bool lt(const heterolist<value_type, Types...>& rhs)
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
        bool gt(const heterolist<value_type, Types...>& rhs)
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
        bool lte(const heterolist<value_type, Types...>& rhs)
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
        bool gte(const heterolist<value_type, Types...>& rhs)
        {
            // if zero elements, cannot be greater than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;

            if (!(*static_cast< container_type<value_type>* >(container_) >= *rhs.container<value_type, 0>())) return false;
            return next().gte(rhs.next());
        }

        // Constructors & Destructors
        heterolist() : container_(new container_type<value_type>), next_(nullptr), counter_(nullptr)
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

        ~heterolist() {}; //counter_ is deallocated in heterolist<>

    private:
        heterolist(size_t* pntr) : container_(new container_type<value_type>), next_(pntr), counter_(pntr)
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
        // Element Access
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
        * \brief Adds val as the first element of the Nth container of type U.
        */
        template<typename U, size_t N = 0, typename V>
        void push_front(const V& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->push_front(val);
                    return;
                }
                ++(*counter_);
            }

            return next().template push_front<U, N>(val);
        }

        /*!
        * \brief Adds val as the first element of the Nth container of type U.
        */
        template<typename U, size_t N = 0, typename V>
        void push_front(V&& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->push_front(val);
                    return;
                }
                ++(*counter_);
            }

            return next().template push_front<U, N>(val);
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
        * \brief Removes the last element of the Nth container of type U.
        */
        template<typename U, size_t N = 0>
        void pop_front()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->pop_front();
                    return;
                }
                ++(*counter_);
            }

            return next().template pop_front<U, N>();
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
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast<container_type<U>*>(container_)->insert(position, first, last);
                    return;
                }
                ++(*counter_);
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
        void swap(heterolist<value_type, Types...>& x)
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
        * \brief The Nth container of type U is extended by inserting a new element before its current first element.
        */
        template<typename U, size_t N = 0, typename... Args>
        void emplace_front(Args&&... args)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast<container_type<U>*>(container_)->emplace_front(args...);
                    return;
                }
                ++(*counter_);
            }

            next().template emplace_front<U, N>(args...);
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

            return next().template set<U, N>(il);
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

        // Operations
        /*!
        * \brief Transfers elements from x into the Nth container of type U, inserting them at position.
        */
        template<typename U, size_t N = 0>
        void splice(typename container_type<U>::const_iterator position, container_type<U>& x)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->splice(position,x);
                }
                ++(*counter_);
            }

            return next().template splice<U, N>(position, x);
        }

        /*!
        * \brief Transfers elements from x into the Nth container of type U, inserting them at position.
        */
        template<typename U, size_t N = 0>
        void splice(typename container_type<U>::const_iterator position, container_type<U>&& x)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->splice(position, x);
                }
                ++(*counter_);
            }

            return next().template splice<U, N>(position, x);
        }

        /*!
        * \brief Transfers elements from x into the Nth container of type U, inserting them at position.
        */
        template<typename U, size_t N = 0>
        void splice(typename container_type<U>::const_iterator position, container_type<U>& x, typename container_type<U>::const_iterator i)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->splice(position, x, i);
                }
                ++(*counter_);
            }

            return next().template splice<U, N>(position, x, i);
        }

        /*!
        * \brief Transfers elements from x into the Nth container of type U, inserting them at position.
        */
        template<typename U, size_t N = 0>
        void splice(typename container_type<U>::const_iterator position, container_type<U>&& x, typename container_type<U>::const_iterator i)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->splice(position, x, i);
                }
                ++(*counter_);
            }

            return next().template splice<U, N>(position, x, i);
        }

        /*!
        * \brief Transfers elements from x into the Nth container of type U, inserting them at position.
        */
        template<typename U, size_t N = 0>
        void splice(typename container_type<U>::const_iterator position, container_type<U>& x,
        typename container_type<U>::const_iterator first, typename container_type<U>::const_iterator last)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->splice(position, x, first, last);
                }
                ++(*counter_);
            }

            return next().template splice<U, N>(position, x, first, last);
        }

        /*!
        * \brief Transfers elements from x into the Nth container of type U, inserting them at position.
        */
        template<typename U, size_t N = 0>
        void splice(typename container_type<U>::const_iterator position, container_type<U>&& x,
        typename container_type<U>::const_iterator first, typename container_type<U>::const_iterator last)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->splice(position, x, first, last);
                }
                ++(*counter_);
            }

            return next().template splice<U, N>(position, x, first, last);
        }

        /*!
        * \brief Removes from the Nth container of type U all the elements that compare equal to val.
        */
        template<typename U, size_t N = 0>
        void remove(const U& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->remove(val);
                }
                ++(*counter_);
            }

            return next().template remove<U, N>(val);
        }

        /*!
        * \brief Removes from the Nth container of type U all the elements for which Predicate pred returns true.
        */
        template<typename U, size_t N = 0, typename Predicate>
        void remove_if(Predicate pred)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->remove_if(pred);
                }
                ++(*counter_);
            }

            return next().template remove_if<U, N>(pred);
        }

        /*!
        * \brief Removes all but the first element from every consecutive group of equal elements in the Nth container of type U.
        */
        template<typename U, size_t N = 0>
        void unique()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->unique();
                }
                ++(*counter_);
            }

            return next().template unique<U, N>();
        }

        /*!
        * \brief Takes as argument a specific comparison function that determine the "uniqueness" of an element.
        *
        *  Calls binary_pred(*i,*(i-1)) for all pairs of elements and removes i from Nth container of type U if predicate returns true; 
        */
        template<typename U, size_t N = 0, typename BinaryPredicate>
        void unique(BinaryPredicate binary_pred)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->unique(binary_pred);
                }
                ++(*counter_);
            }

            return next().template unique<U, N>(binary_pred);
        }

        /*!
        * \brief Merges x into the list by transferring all of its elements at their respective ordered positions into the container (both containers shall already be ordered).
        */
        template<typename U, size_t N = 0>
        void merge(container_type<U>& x)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->merge(x);
                }
                ++(*counter_);
            }

            return next().template merge<U, N>(x);
        }

        /*!
        * \brief Merges x into the list by transferring all of its elements at their respective ordered positions into the container (both containers shall already be ordered).
        */
        template<typename U, size_t N = 0>
        void merge(container_type<U>&& x)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->merge(x);
                }
                ++(*counter_);
            }

            return next().template merge<U, N>(x);
        }

        /*!
        * \brief Merges x into the list by transferring all of its elements at their respective ordered positions into the container (both containers shall already be ordered).
        */
        template<typename U, size_t N = 0, typename Compare>
        void merge(container_type<U>& x, Compare comp)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->merge(x,comp);
                }
                ++(*counter_);
            }

            return next().template merge<U, N>(x,comp);
        }

        /*!
        * \brief Merges x into the list by transferring all of its elements at their respective ordered positions into the container (both containers shall already be ordered).
        */
        template<typename U, size_t N = 0, typename Compare>
        void merge(container_type<U>&& x, Compare comp)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->merge(x,comp);
                }
                ++(*counter_);
            }

            return next().template merge<U, N>(x,comp);
        }

        /*!
        * \brief Sorts the elements in the list, altering their position within the container.
        */
        template<typename U, size_t N = 0>
        void sort()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->sort();
                }
                ++(*counter_);
            }

            return next().template sort<U, N>();
        }

        /*!
        * \brief Sorts the elements in the list, altering their position within the container.
        */
        template<typename U, size_t N = 0, typename Compare>
        void sort(Compare comp)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->sort(comp);
                }
                ++(*counter_);
            }

            return next().template sort<U, N>(comp);
        }

        /*!
        * \brief Reverses the order of the elements in the list container.
        */
        template<typename U, size_t N = 0>
        void reverse()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->reverse();
                }
                ++(*counter_);
            }

            return next().template reverse<U, N>();
        }

    private:
        void* container_; //pointer to container_type<value_type>
        heterolist<Types...> next_;
        size_t* counter_;

        // Helper Methods
        heterolist<Types...>& next()
        {
            return next_;
        }

        const heterolist<Types...>& next() const
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
    class heterolist<T>
    {
        // Friends
        template<typename... Args> friend class heterolist;

    public:
        // Typedefs
        typedef T value_type;

        template<typename U>
        using container_type = std::list<U>;

        // Operators
        heterolist<value_type>& operator=(const heterolist<value_type>& x)
        {
            setEQUALTO(x);
            return *this;
        }

    private:
        void setEQUALTO(const heterolist<value_type>& x)
        {
            //copy contents of x into object
            if (container_ != nullptr) delete container_;
            container_ = new container_type<value_type>(*static_cast< container_type<value_type>* >(x.container_));
        }

    public:
        // Relational Operators & Methods
        // =========================================================================
        // all operators need to be implemented using the containers native
        // relational operators.  Defining relational operators based on one another
        // does not produce expected results.
        // std::list uses lexographical comparison
        // User must be aware of this as it may not produce expected result
        // =========================================================================

        /*!
        * \brief Returns true if == operator evaluates to true for each element in object.
        */
        bool operator==(const heterolist<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) == *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Returns true if == operator evaluates to false for any element in object.
        */
        bool operator!=(const heterolist<value_type>& rhs)
        {
            return !operator==(rhs);
        }

        /*!
        * \brief Returns true if < operator evaluates to true for each element in object.
        */
        bool operator<(const heterolist<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) < *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Returns true if > operator evaluates to true for each element in object.
        */
        bool operator>(const heterolist<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) > *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Returns true if <= operator evaluates to true for each element in object.
        */
        bool operator<=(const heterolist<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) <= *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Returns true if >= operator evaluates to true for each element in object.
        */
        bool operator>= (const heterolist<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) >= *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Same as operator==() but strictly enforces container element size matching.
        */
        bool eq(const heterolist<value_type>& rhs)
        {
            // number of elements must match
            if ( static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size() ) return false;
            return *static_cast< container_type<value_type>* >(container_) == *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Same as operator!=() but strictly enforces container element size matching.
        */
        bool ne(const heterolist<value_type>& rhs)
        {
            return !eq(rhs);
        }

        /*!
        * \brief Same as operator<() but strictly enforces container element size matching.
        */
        bool lt(const heterolist<value_type>& rhs)
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
        bool gt(const heterolist<value_type>& rhs)
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
        bool lte(const heterolist<value_type>& rhs)
        {
            // if zero elements, cannot be less than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;
            return *static_cast< container_type<value_type>* >(container_) <= *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Same as operator>=() but strictly enforces container element size matching.
        */
        bool gte(const heterolist<value_type>& rhs)
        {
            // if zero elements, cannot be greater than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;
            return *static_cast< container_type<value_type>* >(container_) >= *rhs.container<value_type, 0>();
        }

        // Constructors & Destructors
        heterolist() : container_(new container_type<value_type>), counter_(nullptr)
        {
            counter_ = new size_t;
            *counter_ = 0;
        };

        ~heterolist()
		{
			if( counter_ != nullptr ) delete counter_;
		};

    private:
        heterolist(size_t* pntr) : container_(new container_type<value_type>), counter_(pntr)
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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

            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Adds val as the first element of the Nth container of type U.
        */
        template<typename U, size_t N = 0, typename V>
        void push_front(const V& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->push_front(val);
                    return;
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Adds val as the first element of the Nth container of type U.
        */
        template<typename U, size_t N = 0, typename V>
        void push_front(V&& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->push_front(val);
                    return;
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Removes the first element of the Nth container of type U.
        */
        template<typename U, size_t N = 0>
        void pop_front()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U>* >(container_)->pop_front();
                    return;
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Inserts copies of the values between [first,last) into the Nth container of type U, starting at the location specified by position.
        */
        template<typename U, size_t N = 0, typename InputIterator>
        void insert(typename container_type<U>::const_iterator position, InputIterator first, InputIterator last)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast<container_type<U>*>(container_)->insert(position, first, last);
                    return;
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
        void swap(heterolist<T>& x)
        {
            void* temp = container_;
            container_ = x.container_;
            x.container_ = temp;
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief The Nth container of type U is extended by inserting a new element before its current first element.
        */
        template<typename U, size_t N = 0, typename... Args>
        void emplace_front(Args&&... args)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast<container_type<U>*>(container_)->emplace_front(args...);
                    return;
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));;
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
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
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        // Operations
        /*!
        * \brief Transfers elements from x into the Nth container of type U, inserting them at position.
        */
        template<typename U, size_t N = 0>
        void splice(typename container_type<U>::const_iterator position, container_type<U>& x)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->splice(position, x);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Transfers elements from x into the Nth container of type U, inserting them at position.
        */
        template<typename U, size_t N = 0>
        void splice(typename container_type<U>::const_iterator position, container_type<U>&& x)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->splice(position, x);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Transfers elements from x into the Nth container of type U, inserting them at position.
        */
        template<typename U, size_t N = 0>
        void splice(typename container_type<U>::const_iterator position, container_type<U>& x, typename container_type<U>::const_iterator i)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->splice(position, x, i);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Transfers elements from x into the Nth container of type U, inserting them at position.
        */
        template<typename U, size_t N = 0>
        void splice(typename container_type<U>::const_iterator position, container_type<U>&& x, typename container_type<U>::const_iterator i)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->splice(position, x, i);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Transfers elements from x into the Nth container of type U, inserting them at position.
        */
        template<typename U, size_t N = 0>
        void splice(typename container_type<U>::const_iterator position, container_type<U>& x,
        typename container_type<U>::const_iterator first, typename container_type<U>::const_iterator last)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->splice(position, x, first, last);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Transfers elements from x into the Nth container of type U, inserting them at position.
        */
        template<typename U, size_t N = 0>
        void splice(typename container_type<U>::const_iterator position, container_type<U>&& x,
        typename container_type<U>::const_iterator first, typename container_type<U>::const_iterator last)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->splice(position, x, first, last);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Removes from the Nth container of type U all the elements that compare equal to val.
        */
        template<typename U, size_t N = 0>
        void remove(const U& val)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->remove(val);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Removes from the Nth container of type U all the elements for which Predicate pred returns true.
        */
        template<typename U, size_t N = 0, typename Predicate>
        void remove_if(Predicate pred)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->remove_if(pred);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Removes all but the first element from every consecutive group of equal elements in the Nth container of type U.
        */
        template<typename U, size_t N = 0>
        void unique()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->unique();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Takes as argument a specific comparison function that determine the "uniqueness" of an element.
        *
        *  Calls binary_pred(*i,*(i-1)) for all pairs of elements and removes i from Nth container of type U if predicate returns true;
        */
        template<typename U, size_t N = 0, typename BinaryPredicate>
        void unique(BinaryPredicate binary_pred)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->unique(binary_pred);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Merges x into the list by transferring all of its elements at their respective ordered positions into the container (both containers shall already be ordered).
        */
        template<typename U, size_t N = 0>
        void merge(container_type<U>& x)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->merge(x);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Merges x into the list by transferring all of its elements at their respective ordered positions into the container (both containers shall already be ordered).
        */
        template<typename U, size_t N = 0>
        void merge(container_type<U>&& x)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->merge(x);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Merges x into the list by transferring all of its elements at their respective ordered positions into the container (both containers shall already be ordered).
        */
        template<typename U, size_t N = 0, typename Compare>
        void merge(container_type<U>& x, Compare comp)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->merge(x, comp);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Merges x into the list by transferring all of its elements at their respective ordered positions into the container (both containers shall already be ordered).
        */
        template<typename U, size_t N = 0, typename Compare>
        void merge(container_type<U>&& x, Compare comp)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->merge(x, comp);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Sorts the elements in the list, altering their position within the container.
        */
        template<typename U, size_t N = 0>
        void sort()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->sort();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Sorts the elements in the list, altering their position within the container.
        */
        template<typename U, size_t N = 0, typename Compare>
        void sort(Compare comp)
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->sort(comp);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Reverses the order of the elements in the list container.
        */
        template<typename U, size_t N = 0>
        void reverse()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U>* >(container_)->reverse();
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
    bool operator==(const heterolist<Args_lhs...>& lhs, const heterolist<Args_rhs...>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator!=(const heterolist<Args_lhs...>& lhs, const heterolist<Args_rhs...>& rhs)
    {
        return lhs.operator!=(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator<(const heterolist<Args_lhs...>& lhs, const heterolist<Args_rhs...>& rhs)
    {
        return lhs.operator<(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator<=(const heterolist<Args_lhs...>& lhs, const heterolist<Args_rhs...>& rhs)
    {
        return lhs.operator<=(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator>(const heterolist<Args_lhs...>& lhs, const heterolist<Args_rhs...>& rhs)
    {
        return lhs.operator>(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator>=(const heterolist<Args_lhs...>& lhs, const heterolist<Args_rhs...>& rhs)
    {
        return lhs.operator>=(rhs);
    }
}

#endif // HETEROGENEOUS_HETEROLIST_HPP