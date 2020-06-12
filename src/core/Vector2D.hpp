/*******************************************************************************
 * 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef VECTOR2D_HPP_
#define VECTOR2D_HPP_

//#include <assert.h>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <core/core.h>

#include "Exception.hpp"

namespace impact_tr4 {
        /**
       	 * A 2D vector @f$ \mathbf{v}\in R^2 @f$.
         *
         *  The Vector2D class is a subclass of boost::numerics::ublas::bounded_vector. Vector3D supports all ublas vector operations including:
         *  - dot/inner product: s = inner_prod( v1, v2 )
         *  - subtraction: v3 = v2 - v1
         *  - addition: v3 = v1 + v2
         *  - scaling: v2 = v1 * s
         *  - norm: s = norm_1(v1), s = norm_2(v1), s = norm_inf(v1)
         *  - sum: s = sum(v1)
         *
         *  Usage example:
         *  @code
         *  using namespace rw::math;
         *
         *  Vector2D<> v1(1.0, 2.0);
         *  Vector2D<> v2(6.0, 7.0);
         *  double d = inner_prod( v1, v2 );
         *  Vector2D<> v4 = v2 - v1;
         *  @endcode
         */
        template<class T = float_t>
            class Vector2D : public boost::numeric::ublas::bounded_vector<T, 2>{
            	// Typedef the base bector class:
                typedef boost::numeric::ublas::bounded_vector<T, 2> Base_vector;

                public:

		/**
		 * @brief Creates a 2D vector initialized with 0's
		 */
		Vector2D(): Base_vector(2) {
		    (*this)[0] = 0;
		    (*this)[1] = 0;
		}

                /**
                 * @brief Creates a 2D vector
                 * @param x [in] @f$ x @f$
                 * @param y [in] @f$ y @f$
                 * @param z [in] @f$ z @f$
                 */
                Vector2D(T x, T y) : Base_vector(2){
                    (*this)[0] = x;
                    (*this)[1] = y;
                }

                /**
                 * @brief Creates a 2D vector from vector_expression
                 * @param r [in] an ublas vector_expression
                 */
                template <class R> Vector2D (const boost::numeric::ublas::vector_expression<R>& r) : Base_vector(r)
                {}

                /**
                 * @brief Assigns vector expression to 2D vector object
                 * @param r [in] an ublas vector_expression
                 */
                template <class R> void operator=(const boost::numeric::ublas::vector_expression<R>& r)
                {
                    Base_vector::operator=(r);
                }

	    /**
	     * @brief Sets the values of the 2D vector
	     * @param x [in] @f$ x @f$
	     * @param y [in] @f$ y @f$
	     */
        void set(T x, T y) {
            (*this)[0] = x;
            (*this)[1] = y;
        }

//        const T& operator[](size_t i) const { return (*this)(i); }
//        T& operator[](size_t i) { return (*this)(i); }

		/**
		 * @brief Returns reference to vector element
		 * @param i [in] index in the vector \f$i\in \{0,1\} \f$
		 * @return const reference to element
		 */
		const T& operator()(size_t i) const {
			Assert(!ARG_CHECK || isValidIndex(i), IndexOutOfBoundsException("IndexOutOfBoundsException in const operator(size_t i) of Vector2D.hpp!"));
		    return (*this)[i];
		}

		/**
		 * @brief Returns reference to vector element
		 * @param i [in] index in the vector \f$i\in \{0,1\} \f$
		 * @return reference to element
		 */
		T& operator()(size_t i) {
			Assert(!ARG_CHECK || isValidIndex(i), IndexOutOfBoundsException("IndexOutOfBoundsException in operator(size_t i) of Vector2D.hpp!"));
		    return (*this)[i];
		}

		bool operator==(const Vector2D& rhs) const {
			return ((*this)(0)==rhs(0) && (*this)(1) == rhs(1));
		}

		bool isValidIndex(size_t index) const {
			return (index>=0 && index<=1);
		}

		/**
		 * @brief Returns the normalized vector \f$\mathbf{n}=\frac{\mathbf{v}}{\|\mathbf{v}\|} \f$.
		 * In case \f$ \|mathbf{v}\| = 0\f$ the zero vector is returned.
		 * @param v [in] \f$ \mathbf{v} \f$ which should be normalized
		 * return the normalized vector \f$ \mathbf{n} \f$
		 */
		friend Vector2D<T> normalize(const Vector2D<T>& v) {
		    T length = norm_2(v);
		    if (length != 0)
			return Vector2D<T>(v(0)/length, v(1)/length);
		    else
			return Vector2D<T>(0,0);
		}

		/** Some static constant vectors. */
		static const Vector2D<T> ZEROS, ONES, MINUS_ONES, E1, E2;
        }; // end class Vector2D

	    /** Typedef for 2D vector with floating point values */
	    typedef Vector2D<float_t> Vector2f;
	    /** Typedef for 2D vector with integer values */
	    typedef Vector2D<int> Vector2i;
		/** Typedef for 2D vector with unsigned char values */
		typedef Vector2D<uchar> Vector2uc;

		template <class T>
		/*static*/ const Vector2D<T> Vector2D<T>::ZEROS = Vector2D<T>(0,0);

		template <class T>
		/*static*/ const Vector2D<T> Vector2D<T>::ONES = Vector2D<T>(1,1);

		template <class T>
		/*static*/ const Vector2D<T> Vector2D<T>::MINUS_ONES = Vector2D<T>(-1,-1);

		template <class T>
		/*static*/ const Vector2D<T> Vector2D<T>::E1 = Vector2D<T>(1,0);

		template <class T>
		/*static*/ const Vector2D<T> Vector2D<T>::E2 = Vector2D<T>(0,1);

} // end of namespace impact_tr4

#endif /*VECTOR2D_HPP_*/
