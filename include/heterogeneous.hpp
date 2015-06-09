#ifndef HETEROGENEOUS
#define HETEROGENEOUS

/*
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*
* Copyright (c) 2015 Hirotatsu Armstrong
*/

/*!
* \file heterogeneous.hpp
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


#include <vector>
namespace heterogeneous
{
    /*!
    * \cond Skip Doxygen documentation of this forward declaration.
    */
    template<typename... Types> class vector;
    /*!
    * \endcond
    */

    template<typename T, typename... Types>
    class vector<T, Types...>
    {
        // Friends
        template<typename... Args> friend class vector;

    public:
        // Typedefs
        typedef T value_type;

        template<typename U>
        using container_type = std::vector<U>;

    private:
        void* container_;
        vector<Types...> next_;
        size_t* counter_;

        // Helper Functions
        vector<Types...>& next()
        {
            return next_;
        }

        const vector<Types...>& next() const
        {
            return next_;
        }

        void setcounter(size_t*& pntr)
        {
            counter_ = pntr;
            next().setcounter(pntr);
        }

    public:
        // Constructors & Destructors
        vector() : container_(new container_type<value_type>), next_(nullptr), counter_(nullptr)
        {
            //counter_ is deallocated in vector<T> specializetion destructor
            counter_ = new size_t;
            *counter_ = 0;

            next().setcounter(counter_);
        };

        ~vector()
		{
			if (container_ != nullptr) delete static_cast< container_type<value_type>* >(container_);
		};

    private:
        vector(size_t* pntr) : container_(new container_type<value_type>), next_(pntr), counter_(pntr)
        { /*this constructor does not allocate memory for counter_*/ };

    public:
        // Operators
        /*!
        * \brief Assigns contents of rhs to vector.
        */
        vector<value_type, Types...>& operator=(const vector<value_type, Types...>& rhs)
        {
            setEQUALTO(rhs);
            return *this;
        }

    private:
        void setEQUALTO(const vector<value_type, Types...>& x)
        {
            if (container_ != nullptr) delete static_cast< container_type<value_type>* >(container_);
            container_ = new container_type<value_type>(*static_cast< container_type<value_type>* >(x.container_));
            next().setEQUALTO(x.next());
        }

    public:
        // Relational Operators & Methods
        /*!
        * \brief Returns true if == operator evaluates to true for each element in object.
        */
        bool operator==(const vector<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) == rhs.get<value_type, 0>())) return false;
            return next().operator==(rhs.next());
        }

        /*!
        * \brief Returns true if == operator evaluates to false for any element in object.
        */
        bool operator!=(const vector<value_type, Types...>& rhs)
        {
            return !operator==(rhs);
        }

        /*!
        * \brief Returns true if < operator evaluates to true for each element in object.
        */
        bool operator<(const vector<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) < rhs.get<value_type, 0>())) return false;
            return next().operator<(rhs.next());
        }

        /*!
        * \brief Returns true if > operator evaluates to true for each element in object.
        */
        bool operator>(const vector<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) > rhs.get<value_type, 0>())) return false;
            return next().operator>(rhs.next());
        }

        /*!
        * \brief Returns true if <= operator evaluates to true for each element in object.
        */
        bool operator<=(const vector<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) <= rhs.get<value_type, 0>())) return false;
            return next().operator<=(rhs.next());
        }

        /*!
        * \brief Returns true if >= operator evaluates to true for each element in object.
        */
        bool operator>= (const vector<value_type, Types...>& rhs)
        {
            if (!(*static_cast< container_type<value_type>* >(container_) >= rhs.get<value_type, 0>())) return false;
            return next().operator>=(rhs.next());
        }

        /*!
        * \brief Same as operator==() but strictly enforces container element size matching.
        */
        bool eq(const vector<value_type, Types...>& rhs)
        {
            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.get<value_type, 0>().size()) return false;

            // Although this code is duplicated in operator==, we cannot call operator== here as a replacement.
            // The above test for matching number of elements must be checked recursively
            if (!(*static_cast< container_type<value_type>* >(container_) == rhs.get<value_type, 0>())) return false;
            return next().eq(rhs.next());
        }

        /*!
        * \brief Same as operator!=() but strictly enforces container element size matching.
        */
        bool ne(const vector<value_type, Types...>& rhs)
        {
            return !eq(rhs);
        }

        /*!
        * \brief Same as operator<() but strictly enforces container element size matching.
        */
        bool lt(const vector<value_type, Types...>& rhs)
        {
            // if zero elements, cannot be less than
            if (static_cast<container_type<value_type>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.get<value_type, 0>().size()) return false;
            if (!(*static_cast< container_type<value_type>* >(container_) < rhs.get<value_type, 0>())) return false;
            return next().lt(rhs.next());
        }

        /*!
        * \brief Same as operator>() but strictly enforces container element size matching.
        */
        bool gt(const vector<value_type, Types...>& rhs)
        {
            // if zero elements, cannot be greater than
            if (static_cast<container_type<value_type>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.get<value_type, 0>().size()) return false;

            if (!(*static_cast< container_type<value_type>* >(container_) > rhs.get<value_type, 0>())) return false;
            return next().gt(rhs.next());
        }

        /*!
        * \brief Same as operator<=() but strictly enforces container element size matching.
        */
        bool lte(const vector<value_type, Types...>& rhs)
        {
            // if zero elements, cannot be less than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.get<value_type, 0>().size()) return false;

            if (!(*static_cast< container_type<value_type>* >(container_) <= rhs.get<value_type, 0>())) return false;
            return next().lte(rhs.next());
        }

        /*!
        * \brief Same as operator>=() but strictly enforces container element size matching.
        */
        bool gte(const vector<value_type, Types...>& rhs)
        {
            // if zero elements, cannot be greater than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.get<value_type, 0>().size()) return false;

            if (!(*static_cast< container_type<value_type>* >(container_) >= rhs.get<value_type, 0>())) return false;
            return next().gte(rhs.next());
        }

        // Methods
        /*!
        * \brief Returns the total number of elements in vector.
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
        * \brief Returns reference to the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        container_type<U>& get()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return *static_cast< container_type<U>* >(container_);
                }
                ++(*counter_);
            }

            return next().template get<U, N>();
        }

        /*!
        * \brief Returns const reference to the Nth container of type U.
        */
        template <typename U, size_t N = 0>
        const container_type<U>& get() const
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return &static_cast< const container_type<U>* >(container_);
                }
                ++(*counter_);
            }

            return next().template get<U, N>();
        }
		
		// Algorithms
		template<typename F>
		bool all_of(F fn)
		{
			for (auto itr = container_->begin(); itr != container_->end(); ++itr)
			{
				if (!fn(*itr)) return false;
			}

			return next().template all_of(fn);
		}

		template<typename U, class F>
		bool all_of(F fn)
		{
			if (typeid(U) != typeid(value_type))
				return next().template all_of<U>(fn);

			for (auto itr = container_->begin(); itr != container_->end(); ++itr)
			{
				if (!fn(*itr)) return false;
			}

			return next().template all_of<U>(fn);
		}

		template<typename F>
		bool any_of(F fn)
		{
			for (auto itr = container_->begin(); itr != container_->end(); ++itr)
			{
				if (fn(*itr)) return true;
			}

			return next().template any_of(fn);
		}

		template<typename U, class F>
		bool any_of(F fn)
		{
			if (typeid(U) != typeid(value_type))
				return next().template any_of<U>(fn);

			for (auto itr = container_->begin(); itr != container_->end(); ++itr)
			{
				if (fn(*itr)) return true;
			}

			return next().template any_of<U>(fn);
		}

		template<typename F>
		bool none_of(F fn)
		{
			for (auto itr = container_->begin(); itr != container_->end(); ++itr)
			{
				if (fn(*itr)) return false;
			}

			return next().template none_of(fn);
		}

		template<typename U, typename F>
		bool none_of(F fn)
		{
			if (typeid(U) != typeid(value_type))
				return next().template none_of<U>(fn);

			for (auto itr = container_->begin(); itr != container_->end(); ++itr)
			{
				if (fn(*itr)) return false;
			}

			return next().template none_of<U>(fn);
		}

		template<typename Function>
		Function for_each(Function fn)
        {
            fn( *static_cast< container_type<value_type>* >(container_) );
            return next().template for_each(fn);
        }

		template<typename U, typename Function>
		Function for_each(Function fn)
		{
			if(typeid(U) == typeid(value_type))
			  fn(*static_cast< container_type<value_type>* >(container_));

			return next().template for_each<U>(fn);
		}

		/*!
		* \brief Swaps contents of object with x.
		*/
		void swap(vector<value_type, Types...>& x)
		{
			void* temp = container_;
			container_ = x.container_;
			x.container_ = temp;

			next().swap(x.next());
		}
    };

    /*!
    * \cond Skip Doxygen documentation of this specialization.
    */
    template<typename T>
    class vector<T>
    {
        // Friends
        template<typename... Args> friend class vector;

    public:
        // Typedefs
        typedef T value_type;

        template<typename U>
        using container_type = std::vector<U>;

    private:
        void* container_;
        size_t* counter_;

        void setcounter(size_t*& pntr)
        {
            counter_ = pntr;
        }

    public:
        vector() : container_(new container_type<value_type>), counter_(nullptr)
        {
            counter_ = new size_t;
            *counter_ = 0;
        };

        ~vector()
        {
			if (container_ != nullptr) delete static_cast< container_type<value_type>* >(container_);
            if (counter_ != nullptr) delete counter_;
        };

    private:
        vector(size_t* pntr) : container_(new container_type<value_type>), counter_(pntr)
        { /*this constructor does not allocate memory for counter_*/ };

	public:
        // Operators
        vector<value_type>& operator=(const vector<value_type>& x)
        {
            setEQUALTO(x);
            return *this;
        }

    private:
        void setEQUALTO(const vector<value_type>& x)
        {
            if (container_ != nullptr) delete static_cast< container_type<value_type>* >(container_);
            container_ = new container_type<value_type>(*static_cast< container_type<value_type>* >(x.container_));
        }

    public:
        // Relational Operators & Methods
        bool operator==(const vector<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) == rhs.get<value_type, 0>();
        }

        bool operator!=(const vector<value_type>& rhs)
        {
            return !operator==(rhs);
        }

        bool operator<(const vector<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) < rhs.get<value_type, 0>();
        }

        bool operator>(const vector<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) > rhs.get<value_type, 0>();
        }

        bool operator<=(const vector<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) <= rhs.get<value_type, 0>();
        }

        bool operator>= (const vector<value_type>& rhs)
        {
            return *static_cast< container_type<value_type>* >(container_) >= rhs.get<value_type, 0>();
        }

        bool eq(const vector<value_type>& rhs)
        {
            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.get<value_type, 0>().size()) return false;
            return *static_cast< container_type<value_type>* >(container_) == rhs.get<value_type, 0>();
        }

        bool ne(const vector<value_type>& rhs)
        {
            return !eq(rhs);
        }

        bool lt(const vector<value_type>& rhs)
        {
            // if zero elements, cannot be less than
            if (static_cast<container_type<value_type>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.get<value_type, 0>().size()) return false;
            return *static_cast< container_type<value_type>* >(container_) < rhs.get<value_type, 0>();
        }

        bool gt(const vector<value_type>& rhs)
        {
            // if zero elements, cannot be greater than
            if (static_cast<container_type<value_type>*>(container_)->size() == 0) return false;

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.get<value_type, 0>().size()) return false;
            return *static_cast< container_type<value_type>* >(container_) > rhs.get<value_type, 0>();
        }

        bool lte(const vector<value_type>& rhs)
        {
            // if zero elements, cannot be less than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.get<value_type, 0>().size()) return false;
            return *static_cast< container_type<value_type>* >(container_) <= rhs.get<value_type, 0>();
        }

        bool gte(const vector<value_type>& rhs)
        {
            // if zero elements, cannot be greater than, but can be equal!

            // number of elements must match
            if (static_cast<container_type<value_type>*>(container_)->size() != rhs.get<value_type, 0>().size()) return false;
            return *static_cast< container_type<value_type>* >(container_) >= rhs.get<value_type, 0>();
        }

        size_t size() { return 1; }

    private:
        void size(size_t& val) { ++val; }

    public:
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
        template <typename U, size_t N = 0>
        container_type<U>& get()
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return *static_cast< container_type<U>* >(container_);
                }
            }
            *counter_ = 0;

            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

        template <typename U, size_t N = 0>
        const container_type<U>& get() const
        {
            if (typeid(U) == typeid(value_type))
            {
                if (*counter_ == N)
                {
                    *counter_ = 0;
                    return *static_cast< const container_type<U>* >(container_);
                }
            }
            *counter_ = 0;

            throw std::invalid_argument(std::string("Type ") + std::string(typeid(U).name()) + std::string(" with index N=") + std::to_string(N) + std::string(" does not exist in object."));
        }

		// Algorithms
		template<typename F>
		bool all_of(F fn)
		{
			for (auto itr = container_->begin(); itr != container_->end(); ++itr)
			{
				if (!fn(*itr)) return false;
			}

			return true;
		}

		template<typename U, class F>
		bool all_of(F fn)
		{
			if (typeid(U) != typeid(value_type)) return true;

			for (auto itr = container_->begin(); itr != container_->end(); ++itr)
			{
				if (!fn(*itr)) return false;
			}

			return true;
		}

		template<typename F>
		bool any_of(F fn)
		{
			for (auto itr = container_->begin(); itr != container_->end(); ++itr)
			{
				if (fn(*itr)) return true;
			}

			return false;
		}

		template<typename U, class F>
		bool any_of(F fn)
		{
			if (typeid(U) != typeid(value_type)) return false;

			for (auto itr = container_->begin(); itr != container_->end(); ++itr)
			{
				if (fn(*itr)) return true;
			}

			return false;
		}

		template<typename F>
		bool none_of(F fn)
		{
			for (auto itr = container_->begin(); itr != container_->end(); ++itr)
			{
				if (fn(*itr)) return false;
			}

			return true;
		}

		template<typename U, typename F>
		bool none_of(F fn)
		{
			if (typeid(U) != typeid(value_type)) return true;

			for (auto itr = container_->begin(); itr != container_->end(); ++itr)
			{
				if (fn(*itr)) return false;
			}

			return true;
		}

		template<typename Function>
        Function for_each(Function fn )
        {
            fn( *static_cast< container_type<value_type>* >(container_) );
			return fn;
        }

		template<typename U, typename Function>
		Function for_each(Function fn)
		{
			if( typeid(U) == typeid(value_type) )
			  fn(*static_cast< container_type<value_type>* >(container_));

			return fn;
		}

		void swap(vector<T>& x)
		{
			void* temp = container_;
			container_ = x.container_;
			x.container_ = temp;
		}
    };
    /*!
    * \endcond
    */

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator==(const vector<Args_lhs...>& lhs, const vector<Args_rhs...>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator!=(const vector<Args_lhs...>& lhs, const vector<Args_rhs...>& rhs)
    {
        return lhs.operator!=(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator<(const vector<Args_lhs...>& lhs, const vector<Args_rhs...>& rhs)
    {
        return lhs.operator<(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator<=(const vector<Args_lhs...>& lhs, const vector<Args_rhs...>& rhs)
    {
        return lhs.operator<=(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator>(const vector<Args_lhs...>& lhs, const vector<Args_rhs...>& rhs)
    {
        return lhs.operator>(rhs);
    }

    template<typename... Args_lhs, typename... Args_rhs>
    bool operator>=(const vector<Args_lhs...>& lhs, const vector<Args_rhs...>& rhs)
    {
        return lhs.operator>=(rhs);
    }




	template<typename T, typename... Types, class F>
	bool all_of(vector<T, Types...>& hv, F fn)
	{
		return hv.template all_of(fn);
	}

	template<typename U, typename T, typename... Types, class F>
	bool all_of(vector<T, Types...>& hv, F fn)
	{
		hv.template all_of<U>(fn);
	}

	template<typename T, typename... Types, class F>
	bool any_of(vector<T, Types...>& hv, F fn)
	{
		return hv.template any_of(fn);
	}

	template<typename U, typename T, typename... Types, class F>
	bool any_of(vector<T, Types...>& hv, F fn)
	{
		hv.template any_of<U>(fn);
	}

	template<typename T, typename... Types, class F>
	bool none_of(vector<T, Types...>& hv, F fn)
	{
		return hv.template none_of(fn);
	}

	template<typename U, typename T, typename... Types, class F>
	bool none_of(vector<T, Types...>& hv, F fn)
	{
		hv.template none_of<U>(fn);
	}



	template<typename T, typename... Types, class F>
	auto for_each(vector<T, Types...>& hv, F fn)
	{
		return hv.template for_each(fn);
	}

	template<typename U, typename T, typename... Types, class Function>
	Function for_each(vector<T, Types...>& hv, Function fn)
	{
		return hv.template for_each<U>(fn);
	}










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
        using iterator = type_iterator< container_type, native_typeA >;

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
            if (begin<native_typeA>() == end<native_typeA>()) return true;
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
            return boost::get<native_t>(*(pointer_->container_->begin() + index_));
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
            return boost::get<native_t>(*(pointer_->container_->rbegin() + index_));
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

#endif // HETEROGENEOUS