#ifndef HETEROGENEOUS_HETEROARRAY_HPP
#define HETEROGENEOUS_HETEROARRAY_HPP

/*
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*
* Copyright (c) 2015 Hirotatsu Armstrong
*/

/*!
* \file heteroarray.hpp
*
* This templated class replicates the features of an std::array
* that can handle heterogeneous data types.
*/

#include <stdexcept>
#include <typeinfo>
#include <typeindex>
#include <array>
#include <string>

namespace heterogeneous
{
    template<std::size_t n>
    using length = std::integral_constant<std::size_t, n>;

    /*!
    * \cond Skip Doxygen documentation of this forward declaration.
    */
    template<typename... Types> class heteroarray;
    /*!
    * \endcond
    */

    template<typename T, size_t M, typename... Types>
    class heteroarray<T, length<M>, Types...>
    {
        // Friends
        template<typename... Args> friend class heteroarray;
		
    public:
        // Typedefs
        typedef T value_type;

        template<typename U, size_t N>
        using container_type = std::array<U, N>;

        // Relational Operators & Methods
        // =========================================================================
        // all operators need to be implemented using the containers native
        // relational operators.  Defining relational operators based on one another
        // does not produce expected results.
        // std::array uses lexographical comparison
        // User must be aware of this as it may not produce expected result
        // =========================================================================

        /*!
        * \brief Returns true if == operator evaluates to true for each element in object.
        */
        bool operator==(const heteroarray<value_type, length<M>, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type, M>* >(container_) == *rhs.container<value_type, 0>())) return false;
            return next().operator==(rhs.next());
        }

        /*!
        * \brief Returns true if == operator evaluates to false for any element in object.
        */
        bool operator!=(const heteroarray<value_type, length<M>, Types...>& rhs)
        {
            return !operator==(rhs);
        }

        /*!
        * \brief Returns true if < operator evaluates to true for each element in object.
        */
        bool operator<(const heteroarray<value_type, length<M>, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type, M>* >(container_) < *rhs.container<value_type, 0>())) return false;
            return next().operator<(rhs.next());
        }

        /*!
        * \brief Returns true if > operator evaluates to true for each element in object.
        */
        bool operator>(const heteroarray<value_type, length<M>, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type, M>* >(container_) > *rhs.container<value_type, 0>())) return false;
            return next().operator>(rhs.next());
        }

        /*!
        * \brief Returns true if <= operator evaluates to true for each element in object.
        */
        bool operator<=(const heteroarray<value_type, length<M>, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type, M>* >(container_) <= *rhs.container<value_type, 0>())) return false;
            return next().operator<=(rhs.next());
        }

        /*!
        * \brief Returns true if >= operator evaluates to true for each element in object.
        */
        bool operator>= (const heteroarray<value_type, length<M>, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type, M>* >(container_) >= *rhs.container<value_type, 0>())) return false;
            return next().operator>=(rhs.next());
        }

        /*!
        * \brief Same as operator==() but strictly enforces container element size matching.
        */
        bool eq(const heteroarray<value_type, length<M>, Types...>& rhs)
        {
            // number of elements must match
            if (static_cast<container_type<value_type, M>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;

            // Although this code is duplicated in operator==, we cannot call operator== here as a replacement.
            // The above test for matching number of elements must be checked recursively
            if (!(*static_cast< container_type<value_type, M>* >(container_) == *rhs.container<value_type, 0>())) return false;
            return next().eq(rhs.next());
        }

        /*!
        * \brief Same as operator!=() but strictly enforces container element size matching.
        */
        bool ne(const heteroarray<value_type, length<M>, Types...>& rhs)
        {
            return !eq(rhs);
        }

        /*!
        * \brief Same as operator<() but strictly enforces container element size matching.
        */
        bool lt(const heteroarray<value_type, length<M>, Types...>& rhs)
        {
            // if zero elements, cannot be less than
            if (static_cast<container_type<value_type, M>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<value_type, M>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;
            if (!(*static_cast< container_type<value_type, M>* >(container_) < *rhs.container<value_type, 0>())) return false;
            return next().lt(rhs.next());
        }

        /*!
        * \brief Same as operator>() but strictly enforces container element size matching.
        */
        bool gt(const heteroarray<value_type, length<M>, Types...>& rhs)
        {
            // if zero elements, cannot be greater than
            if (static_cast<container_type<value_type, M>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<value_type, M>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;

            if (!(*static_cast< container_type<value_type, M>* >(container_) > *rhs.container<value_type, 0>())) return false;
            return next().gt(rhs.next());
        }

        /*!
        * \brief Same as operator<=() but strictly enforces container element size matching.
        */
        bool lte(const heteroarray<value_type, length<M>, Types...>& rhs)
        {
            // if zero elements, cannot be less than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type, M>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;

            if (!(*static_cast< container_type<value_type, M>* >(container_) <= *rhs.container<value_type, 0>())) return false;
            return next().lte(rhs.next());
        }

        /*!
        * \brief Same as operator>=() but strictly enforces container element size matching.
        */
        bool gte(const heteroarray<value_type, length<M>, Types...>& rhs)
        {
            // if zero elements, cannot be greater than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type, M>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;

            if (!(*static_cast< container_type<value_type, M>* >(container_) >= *rhs.container<value_type, 0>())) return false;
            return next().gte(rhs.next());
        }

        // Constructors & Destructors
        heteroarray() : container_(new container_type<value_type, M>), next_(nullptr), counter_(nullptr)
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

        ~heteroarray() {}; //counter_ is deallocated in heteroarray<>

    private:
        heteroarray(size_t* pntr) : container_(new container_type<value_type, M>), next_(pntr), counter_(pntr)
        { /*this constructor does not allocate memory for counter_*/ };

    public:
        // Iterators
        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::iterator begin()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->begin();
                }
                ++(*counter_);
            }

            // use of next().template begin<U, L, N>();
            // with the additional 'template',
            // rather than next().begin<U, L, N>();
            // is to make the clang compiler happy
            // it wants an explicit declaration that
            // begin depends on template parameters.
            return next().template begin<U, L, N>();
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::iterator end()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->end();
                }
                ++(*counter_);
            }

            return next().template end<U, L, N>();
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::reverse_iterator rbegin()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->rbegin();
                }
                ++(*counter_);
            }

            return next().template rbegin<U, L, N>();
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::reverse_iterator rend()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->rend();
                }
                ++(*counter_);
            }

            return next().template rend<U, L, N>();
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::const_iterator cbegin()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->cbegin();
                }
                ++(*counter_);
            }

            return next().template cbegin<U, L, N>();
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::const_iterator cend()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->cend();
                }
                ++(*counter_);
            }

            return next().template cend<U, L, N>();
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::const_reverse_iterator crbegin()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->crbegin();
                }
                ++(*counter_);
            }

            return next().template crbegin<U, L, N>();
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::const_reverse_iterator crend()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->crend();
                }
                ++(*counter_);
            }

            return next().template crend<U, L, N>();
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
        template <typename U, size_t L, size_t N = 0>
        size_t size() const
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->size();
                }
                ++(*counter_);
            }

            return next().template size<U, L, N>();
        }

        /*!
        * \brief Returns the maximum number of elements the Nth container of type U can hold.
        */
        template <typename U, size_t L, size_t N = 0>
        size_t max_size()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->max_size();
                }
                ++(*counter_);
            }

            return next().template max_size<U, L, N>();
        }

        /*!
        * \brief Resizes whether the Nth container of type U is empty.
        */
        template <typename U, size_t L, size_t N = 0>
        bool empty()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->empty();
                }
                ++(*counter_);
            }

            return next().template empty<U, L, N>();
        }

        /*!
        * \brief Returns whether object stores type U.
        */
        template <typename U, size_t L>
        bool contains()
        {
            if (typeid(U) == typeid(value_type) && L == M) return true;
            return next().template contains<U,L>();
        }

        /*!
        * \brief Returns the number of containers with type U.
        */
        template <typename U, size_t L>
        size_t multiplicity()
        {
            size_t result = 0;
            next().template multiplicity<U, L>(result);

            if (typeid(U) == typeid(value_type) && L == M)
                ++result;

            return result;
        }

    private:
        template <typename U, size_t L>
        void multiplicity(size_t& val)
        {
            next().template multiplicity<U, L>(val);

            if (typeid(U) == typeid(value_type) && L == M)
                ++val;
        }

     public:
        // Element Access
        /*!
        * \brief Returns reference to the nth element of the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        U& at(size_t n)
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->at(n);
                }
                ++(*counter_);
            }

            return next().template at<U, L, N>(n);
        }

        /*!
        * \brief Returns const reference to the nth element of the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        const U& at(size_t n) const
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->at(n);
                }
                ++(*counter_);
            }

            return next().template at<U, L, N>(n);
        }

        /*!
        * \brief Returns reference to the first element of the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        U& front()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->front();
                }
                ++(*counter_);
            }

            return next().template front<U, L, N>();
        }

        /*!
        * \brief Returns const reference to the first element of the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        const U& front() const
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->front();
                }
                ++(*counter_);
            }

            return next().template front<U, L, N>();
        }

        /*!
        * \brief Returns reference to the last element of the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        U& back()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->back();
                }
                ++(*counter_);
            }

            return next().template back<U, L, N>();
        }

        /*!
        * \brief Returns const reference to the last element of the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        const U& back() const
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->back();
                }
                ++(*counter_);
            }

            return next().template back<U, L, N>();
        }

        /*!
        * \brief Returns pointer to the data contained in the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        U* data()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->data();
                }
                ++(*counter_);
            }

            return next().template data<U, L, N>();
        }

        /*!
        * \brief Returns const pointer to the data contained in the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        const U* data() const
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->data();
                }
                ++(*counter_);
            }

            return next().template data<U, L, N>();
        }

        /*!
        * \brief Returns reference to the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        container_type<U,L>* container()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_);
                }
                ++(*counter_);
            }

            return next().template container<U, L, N>();
        }

        /*!
        * \brief Returns const reference to the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        const container_type<U,L>* container() const
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< const container_type<U,L>* >(container_);
                }
                ++(*counter_);
            }

            return next().template container<U, L, N>();
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
        * \brief Fills the contents of the Nth container of type U with that of val.
        */
        template<typename U, size_t L, size_t N = 0>
        void fill(const U& val)
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U, L>* >(container_)->fill(val);
                    return;
                }
                ++(*counter_);
            }

            return next().template fill<U, L, N>(x);
        }

        /*!
        * \brief Swaps the contents of the Nth container of type U with that of x.
        */
        template<typename U, size_t L, size_t N = 0>
        void swap(container_type<U,L>& x)
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U,L>* >(container_)->swap(x);
                    return;
                }
                ++(*counter_);
            }

            return next().template swap<U, L, N>(x);
        }

        /*!
        * \brief Swaps contents of object with x.
        */
        void swap(heteroarray<value_type, length<M>, Types...>& x)
        {
            void* temp = container_;
            container_ = x.container_;
            x.container_ = temp;

            next().swap(x.next());
        }

    private:
        void* container_; //pointer to container_type<value_type, M>
        heteroarray<Types...> next_;
        size_t* counter_;

        // Helper Methods
        heteroarray<Types...>& next()
        {
            return next_;
        }

        const heteroarray<Types...>& next() const
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
	template<typename T, size_t M>
    class heteroarray<T, length<M> >
    {
        // Friends
        template<typename... Args> friend class heteroarray;

    public:
        // Typedefs
        typedef T value_type;

        template<typename U, size_t N>
        using container_type = std::array<U, N>;

        // Operators
        heteroarray<value_type, length<M>>& operator=(const heteroarray<value_type, length<M>>& x)
        {
            setEQUALTO(x);
            return *this;
        }

    private:
        void setEQUALTO(const heteroarray<value_type, length<M>>& x)
        {
            //copy contents of x into object
            if (container_ != nullptr) delete container_;
            container_ = new container_type<value_type, M>(*static_cast< container_type<value_type, M>* >(x.container_));
        }

    public:
        // Relational Operators & Methods
        // =========================================================================
        // all operators need to be implemented using the containers native
        // relational operators.  Defining relational operators based on one another
        // does not produce expected results.
        // std::array uses lexographical comparison
        // User must be aware of this as it may not produce expected result
        // =========================================================================

        /*!
        * \brief Returns true if == operator evaluates to true for each element in object.
        */
        bool operator==(const heteroarray<value_type, length<M>>& rhs)
        {
            return *static_cast< container_type<value_type, M>* >(container_) == *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Returns true if == operator evaluates to false for any element in object.
        */
        bool operator!=(const heteroarray<value_type, length<M>>& rhs)
        {
            return !operator==(rhs);
        }

        /*!
        * \brief Returns true if < operator evaluates to true for each element in object.
        */
        bool operator<(const heteroarray<value_type, length<M>>& rhs)
        {
            return *static_cast< container_type<value_type, M>* >(container_) < *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Returns true if > operator evaluates to true for each element in object.
        */
        bool operator>(const heteroarray<value_type, length<M>>& rhs)
        {
            return *static_cast< container_type<value_type, M>* >(container_) > *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Returns true if <= operator evaluates to true for each element in object.
        */
        bool operator<=(const heteroarray<value_type, length<M>>& rhs)
        {
            return *static_cast< container_type<value_type, M>* >(container_) <= *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Returns true if >= operator evaluates to true for each element in object.
        */
        bool operator>= (const heteroarray<value_type, length<M>>& rhs)
        {
            return *static_cast< container_type<value_type, M>* >(container_) >= *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Same as operator==() but strictly enforces container element size matching.
        */
        bool eq(const heteroarray<value_type, length<M>>& rhs)
        {
            // number of elements must match
            if ( static_cast<container_type<value_type, M>*>(container_)->size() != rhs.container<value_type, 0>()->size() ) return false;
            return *static_cast< container_type<value_type, M>* >(container_) == *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Same as operator!=() but strictly enforces container element size matching.
        */
        bool ne(const heteroarray<value_type, length<M>>& rhs)
        {
            return !eq(rhs);
        }

        /*!
        * \brief Same as operator<() but strictly enforces container element size matching.
        */
        bool lt(const heteroarray<value_type, length<M>>& rhs)
        {
            // if zero elements, cannot be less than
            if (static_cast<container_type<value_type, M>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<value_type, M>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;
            return *static_cast< container_type<value_type, M>* >(container_) < *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Same as operator>() but strictly enforces container element size matching.
        */
        bool gt(const heteroarray<value_type, length<M>>& rhs)
        {
            // if zero elements, cannot be greater than
            if (static_cast<container_type<value_type, M>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<value_type, M>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;
            return *static_cast< container_type<value_type, M>* >(container_) > *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Same as operator<=() but strictly enforces container element size matching.
        */
        bool lte(const heteroarray<value_type, length<M>>& rhs)
        {
            // if zero elements, cannot be less than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type, M>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;
            return *static_cast< container_type<value_type, M>* >(container_) <= *rhs.container<value_type, 0>();
        }

        /*!
        * \brief Same as operator>=() but strictly enforces container element size matching.
        */
        bool gte(const heteroarray<value_type, length<M>>& rhs)
        {
            // if zero elements, cannot be greater than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type, M>*>(container_)->size() != rhs.container<value_type, 0>()->size()) return false;
            return *static_cast< container_type<value_type, M>* >(container_) >= *rhs.container<value_type, 0>();
        }

        // Constructors & Destructors
        heteroarray() : container_(new container_type<value_type, M>), counter_(nullptr)
        {
            counter_ = new size_t;
            *counter_ = 0;
        };

        ~heteroarray()
		{
			if( counter_ != nullptr ) delete counter_;
		};

    private:
        heteroarray(size_t* pntr) : container_(new container_type<value_type, M>), counter_(pntr)
        { /*this constructor does not allocate memory for counter_*/ };

    public:
        // Iterators
        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::iterator begin()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->begin();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::iterator end()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->end();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::reverse_iterator rbegin()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->rbegin();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::reverse_iterator rend()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->rend();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::const_iterator cbegin()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->cbegin();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::const_iterator cend()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->cend();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::const_reverse_iterator crbegin()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->crbegin();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        template <typename U, size_t L, size_t N = 0>
        typename container_type<U,L>::const_reverse_iterator crend()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->crend();
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
        template <typename U, size_t L, size_t N = 0>
        size_t size() const
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->size();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns the maximum number of elements the Nth container of type U can hold.
        */
        template <typename U, size_t L, size_t N = 0>
        size_t max_size()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->max_size();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Resizes whether the Nth container of type U is empty.
        */
        template <typename U, size_t L, size_t N = 0>
        bool empty()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->empty();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns whether object stores type U.
        */
        template <typename U, size_t L>
        bool contains()
        {
            return (typeid(U) == typeid(value_type) && L == M);
        }

        /*!
        * \brief Returns the number of containers with type U.
        */
        template <typename U, size_t L>
        size_t multiplicity()
        {
            return (typeid(U) == typeid(value_type) && L == M) ? 1 : 0;
        }

    private:
        template <typename U, size_t L>
        void multiplicity(size_t& val)
        {
            if (typeid(U) == typeid(value_type) && L == M) ++val;
        }

     public:
        // Element Access
        /*!
        * \brief Returns reference to the nth element of the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        U& at(size_t n)
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->at(n);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns const reference to the nth element of the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        const U& at(size_t n) const
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->at(n);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns reference to the first element of the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        U& front()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->front();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns const reference to the first element of the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        const U& front() const
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->front();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns reference to the last element of the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        U& back()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->back();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns const reference to the last element of the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        const U& back() const
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->back();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns pointer to the data contained in the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        U* data()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->data();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns const pointer to the data contained in the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        const U* data() const
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_)->data();
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns reference to the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        container_type<U,L>* container()
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< container_type<U,L>* >(container_);
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Returns const reference to the Nth container of type U.
        */
        template <typename U, size_t L, size_t N = 0>
        const container_type<U,L>* container() const
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return static_cast< const container_type<U,L>* >(container_);
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
        * \brief Fills the contents of the Nth container of type U with that of val.
        */
        template<typename U, size_t L, size_t N = 0>
        void fill(const U& val)
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U, L>* >(container_)->fill(val);
                    return;
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Swaps the contents of the Nth container of type U with that of x.
        */
        template<typename U, size_t L, size_t N = 0>
        void swap(container_type<U,L>& x)
        {
            if (typeid(U) == typeid(value_type) && L == M)
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    static_cast< container_type<U,L>* >(container_)->swap(x);
                    return;
                }
            }
            *counter_ = 0;
            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        /*!
        * \brief Swaps contents of object with x.
        */
        void swap(heteroarray<T, length<M> >& x)
        {
            void* temp = container_;
            container_ = x.container_;
            x.container_ = temp;
        }

    private:
        void* container_; //pointer to container_type<value_type, M>
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
    bool operator==(const heteroarray<Args_lhs...>& lhs, const heteroarray<Args_rhs...>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator!=(const heteroarray<Args_lhs...>& lhs, const heteroarray<Args_rhs...>& rhs)
    {
        return lhs.operator!=(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator<(const heteroarray<Args_lhs...>& lhs, const heteroarray<Args_rhs...>& rhs)
    {
        return lhs.operator<(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator<=(const heteroarray<Args_lhs...>& lhs, const heteroarray<Args_rhs...>& rhs)
    {
        return lhs.operator<=(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator>(const heteroarray<Args_lhs...>& lhs, const heteroarray<Args_rhs...>& rhs)
    {
        return lhs.operator>(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator>=(const heteroarray<Args_lhs...>& lhs, const heteroarray<Args_rhs...>& rhs)
    {
        return lhs.operator>=(rhs);
    }
}

#endif // HETEROGENEOUS_HETEROARRAY_HPP