#ifndef HETEROGENEOUS_ADAPTOR_HPP
#define HETEROGENEOUS_ADAPTOR_HPP

/*
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*
* Copyright (c) 2015 Hirotatsu Armstrong
*/

/*!
* \file adaptor.hpp
*
* This templated class defines an interface for retrieving
* and modifying the contents of a container of any type-erased
* data type (boost::any, boost::variant, etc)
*/

#include <assert.h>
#include <stdexcept>
#include <typeinfo>
#include <typeindex>
#include <string>

#include <boost\any.hpp>
namespace boost
{
    /*!
    * \brief get<container_type> overload for boost::any.
    */
    template<typename container_type>
    inline container_type& get(boost::any& a)
    {
        return *boost::any_cast<container_type>(&a);
    }
}

namespace heterogeneous
{
    // Forward declare iterator classes
    template<typename container_t, typename native_t> class type_iterator;
    template<typename container_t, typename native_t> class type_reverse_iterator;

    template <typename container_type>
    class adaptor
    {
        /*
        * typename container_type : container type (std::vector<erased_type>, std::list<erased_type>, etc)
        * typename erased_type : type-erased data type (boost::any, boost::variant<...>, etc)
        * typename native_typeA : native data type (int, float, std::string, etc) requested from erased_type
        * typename native_typeB : native data type (int, float, std::string, etc) requested from erased_type
        */

        template<typename container_t, typename native_t> friend class type_iterator;
        template<typename container_t, typename native_t> friend class type_reverse_iterator;

    public:

        // typedefs
        typedef typename container_type::value_type erased_type;

        template<typename native_typeA>
        using iterator =  type_iterator< container_type, native_typeA > ;

        template<typename native_typeA>
        using riterator = type_reverse_iterator< container_type, native_typeA >;

        template<typename native_typeA>
        iterator<native_typeA> begin()
        {
            return iterator<native_typeA>(this, 0);
        }

        template<typename native_typeA>
        riterator<native_typeA> rbegin()
        {
            return riterator<native_typeA>(this, 0);
        }

        template<typename native_typeA>
        iterator<native_typeA> end()
        {
            return iterator<native_typeA>(this, container_->size());
        }

        template<typename native_typeA>
        riterator< native_typeA> rend()
        {
            return riterator<native_typeA>(this, container_->size());
        }

        // constructors and destructors
        adaptor(container_type* object)
         : container_(object)
        {};

        adaptor(container_type& object)
         : container_(&object)
        {};

        ~adaptor() {};

        // methods
        // Capacity
        /*!
        * \brief Returns number of elements of type native_typeA in object.
        */
        template<typename native_typeA>
        size_t size()
        {
            size_t n = 0;
            for (auto ptr = begin<native_typeA>(); ptr != end<native_typeA>(); ++ptr)
            {
                ++n;
            }

            return n;
        }

        /*!
        * \brief Returns whether object contains any elements of type native_typeA or not.
        */
        template <typename native_typeA>
        bool empty()
        {
            //more efficient than calling size<native_typeA>()
            //as we dont have to actually iterate through and
            //count the number of objects
            if ( begin<native_typeA>() == end<native_typeA>() ) return true;
            return false;
        }

        // Element Access
        /*!
        * \brief Returns reference to first element of type native_typeA.
        *
        * If no elements of type native_typeA exist, throws std::out_of_range exception.
        */
        template<typename native_typeA>
        native_typeA& first()
        {
            auto itr = begin<native_typeA>();
            if (itr == end<native_typeA>())
                throw std::out_of_range("std::out_of_range: Attempted to index an out of range element of type " + std::string(typeid(native_typeA).name()));
            return *itr;
        }

        /*!
        * \brief Returns reference to last element of type native_typeA.
        *
        * If no elements of type native_typeA exist, throws std::out_of_range exception.
        */
        template<typename native_typeA>
        native_typeA& last()
        {
            auto ritr = rbegin<native_typeA>();
            if (ritr == rend<native_typeA>())
                throw std::out_of_range("std::out_of_range: Attempted to index an out of range element of type " + std::string(typeid(native_typeA).name()));
            return *ritr;
        }

        /*!
        * \brief Returns reference to the type-erased object of the ith element of native type native_typeA.
        *
        * If the ith element of type native_typeA does not exsist, throws std::out_of_range exception.
        *
        * @param i Index of the element of type native_typeA of which to return a reference to.
        */
        template<typename native_typeA>
        erased_type& at(const size_t& i)
        {
            int n = -1;
            for (auto itr = container_->begin(); itr != container_->end(); ++itr)
            {
                if (itr->type().name() == typeid(native_typeA).name())
                {
                    ++n;
                    if (n == static_cast<int>(i)) return *itr;
                }
            }

            throw std::out_of_range("std::out_of_range: Attempted to index an out of range element of type " + std::string(typeid(native_typeA).name()));
        }

        /*!
        * \brief Returns reference to the i-th type-erased object.
        *
        * If the ith element does not exsist, throws std::out_of_range exception.
        *
        * @param i Index of the element of which to return a reference to.
        */
        erased_type& at(const size_t& i)
        {
            int n = 0;
            for (auto itr = container_->begin(); itr != container_->end(); ++itr, ++n)
            {
                if (n == static_cast<int>(i)) return *itr;
            }

            throw std::out_of_range("std::out_of_range: Attempted to index an out of range container element.");
        }

        /*!
        * \brief Returns reference to ith element of type native_typeA.
        *
        * If the ith element of type native_typeA does not exsist, throws std::out_of_range exception.
        *
        * @param i Index of the element of type native_typeA of which to return a reference to.
        */
        template<typename native_typeA>
        native_typeA& get(const size_t& i)
        {
            auto itr = begin<native_typeA>() + i;
            if (itr == end<native_typeA>())
                throw std::out_of_range("std::out_of_range: Attempted to index an out of range element of type " + std::string(typeid(native_typeA).name()));
            return *itr;
        }

        // Modifiers
        /*!
        * \brief Swaps the positions of two elements of differing types.
        *
        * Finds the i-th element of type native_typeA and the j-th element of type native_typeB.
        * If both exist, the positions are swapped, and function returns true.
        * If i >= size<native_typeA>() or j >= size<native_typeB>(), no swap is performed
        * and function returns false.
        *
        * @param i Index of the element of type native_typeA to swap with j.
        * @param j Index of the element of type native_typeB to swap with i.
        */
        template<typename native_typeA, typename native_typeB>
        bool swap(const size_t& i, const size_t& j)
        {
            if (i >= size<native_typeA>() || j >= size<native_typeB>()) return false;

            //get references to locations
            erased_type& Aref = at<native_typeA>(i);
            erased_type& Bref = at<native_typeB>(j);

            //get values at locations
            native_typeA tempA = get<native_typeA>(i);
            native_typeB tempB = get<native_typeB>(j);

            //swap values
            Aref = tempB;
            Bref = tempA;

            return true;
        }

        /*!
        * \brief Swaps the positions of two elements of same types.
        *
        * Finds the i-th and j-th elements of type native_typeA.
        * If both exist, the positions are swapped, and function returns true.
        * If i,j >= size<native_typeA>(), no swap is performed
        * and function returns false.
        *
        * @param i Index of the element of type native_typeA to swap with j.
        * @param j Index of the element of type native_typeA to swap with i.
        */
        template<typename native_typeA>
        bool swap(const size_t& i, const size_t& j)
        {
            return swap<native_typeA, native_typeA>(i, j);
        }

    private:
        container_type* container_;
    };

    template <typename container_t, typename native_t>
    class type_iterator
    {
    private:
        adaptor<container_t>* pointer_;
        size_t index_;
    public:
        type_iterator(adaptor<container_t>* ptr, const size_t& offset)
         : pointer_(ptr), index_(offset)
        {
            assert(offset <= pointer_->container_->size());

            //find first element of type container_t after offset
            for (auto itr = pointer_->container_->begin() + index_; itr != pointer_->container_->end(); ++itr, ++index_)
            {
                if (itr->type().name() == typeid(native_t).name()) break;
            }
        }

        type_iterator& operator=(const type_iterator& rhs)
        {
            pointer_ = rhs.pointer_;
            index_ = rhs.index_;
            return *this;
        }

        bool operator==(const type_iterator& rhs)
        {
            if (pointer_ != rhs.pointer_) return false;
            if (index_ != rhs.index_) return false;
            return true;
        }

        bool operator!=(const type_iterator& rhs) { return !(*this == rhs); }

        native_t& operator*()
        {
            return boost::get<native_t>( *(pointer_->container_->begin() + index_) );
        }

        type_iterator& operator ++()
        {
            ++index_;
            for (auto itr = pointer_->container_->begin() + index_; itr != pointer_->container_->end(); ++itr, ++index_)
            {
                if (itr->type().name() == typeid(native_t).name()) break;
            }

            return *this;
        }

        type_iterator& operator +(size_t rhs)
        {
            if (rhs == 0) return *this;

            size_t n = 0;
            ++index_;
            for (auto itr = pointer_->container_->begin() + index_; itr != pointer_->container_->end(); ++itr, ++index_)
            {
                if (itr->type().name() == typeid(native_t).name())
                {
                    ++n;
                    if (n == rhs) break;
                }
            }

            return *this;
        }
    }; // type_iterator

    template <typename container_t, typename native_t>
    class type_reverse_iterator
    {
    private:
        adaptor<container_t>* pointer_;
        size_t index_;
    public:
        type_reverse_iterator(adaptor<container_t>* ptr, const size_t& offset)
         : pointer_(ptr), index_(offset)
        {
            assert(offset <= pointer_->container_->size());

            //find first element of type container_t after offset
            for (auto itr = pointer_->container_->rbegin() + index_; itr != pointer_->container_->rend(); ++itr, ++index_)
            {
                if (itr->type().name() == typeid(native_t).name()) break;
            }
        }

        type_reverse_iterator& operator=(const type_reverse_iterator& rhs)
        {
            pointer_ = rhs.pointer_;
            index_ = rhs.index_;
            return *this;
        }

        bool operator==(const type_reverse_iterator& rhs)
        {
            if (pointer_ != rhs.pointer_) return false;
            if (index_ != rhs.index_) return false;
            return true;
        }

        bool operator!=(const type_reverse_iterator& rhs) { return !(*this == rhs); }

        native_t& operator*()
        {
            return boost::get<native_t>( *(pointer_->container_->rbegin() + index_) );
        }

        type_reverse_iterator& operator ++()
        {
            ++index_;
            for (auto itr = pointer_->container_->rbegin() + index_; itr != pointer_->container_->rend(); ++itr, ++index_)
            {
                if (itr->type().name() == typeid(native_t).name()) break;
            }

            return *this;
        }

        type_reverse_iterator& operator +(size_t rhs)
        {
            if (rhs == 0) return *this;

            size_t n = 0;
            ++index_;
            for (auto itr = pointer_->container_->rbegin() + index_; itr != pointer_->container_->rend(); ++itr, ++index_)
            {
                if (itr->type().name() == typeid(native_t).name())
                {
                    ++n;
                    if (n == rhs) break;
                }
            }

            return *this;
        }
    }; // type_reverse_iterator
}

#endif // HETEROGENEOUS_ADAPTOR_HPP