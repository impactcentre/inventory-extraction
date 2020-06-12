/**
 * @file Vector3D.hpp
 */
#ifndef VECTOR3D_HPP_
#define VECTOR3D_HPP_

//#include <assert.h>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <core/core.h>

#include "Exception.hpp"

namespace impact_tr4 {    /**
         * A 3D vector @f$ \mathbf{v}\in R^3 @f$.
         *
         *  The Vector3D class is a subclass of boost::numerics::ublas::bounded_vector. Vector3D supports all ublas vector operations including:
         *  - dot/inner product: s = inner_prod( v1, v2 )
         *  - subtraction: v3 = v2 - v1
         *  - addition: v3 = v1 + v2
         *  - scaling: v2 = v1 * s
         *  - norm: s = norm_1(v1), s = norm_2(v1), s = norm_inf(v1)
         *  - sum: s = sum(v1)
         *
         *  In addition, Vector3D supports the cross product operator:
         *  v3 = cross( v1, v2)
         *
         *  Usage example:
         *  @code
         *  using namespace rw::math;
         *
         *  Vector3D<> v1(1.0, 2.0, 3.0);
         *  Vector3D<> v2(6.0, 7.0, 8.0);
         *  Vector3D<> v3 = cross( v1, v2 );
         *  double d = inner_prod( v1, v2 );
         *  Vector3D<> v4 = v2 - v1;
         *  @endcode
         */
        template<class T = float_t>
            class Vector3D : public boost::numeric::ublas::bounded_vector<T, 3>{
            	// Typedef the base bector class:
                typedef boost::numeric::ublas::bounded_vector<T, 3> Base_vector;

                public:

		/**
		 * @brief Creates a 3D vector initialized with 0's
		 */
		Vector3D(): Base_vector(3) {
		    (*this)[0] = 0;
		    (*this)[1] = 0;
		    (*this)[2] = 0;
		}

                /**
                 * @brief Creates a 3D vector
                 * @param x [in] @f$ x @f$
                 * @param y [in] @f$ y @f$
                 * @param z [in] @f$ z @f$
                 */
                Vector3D(T x, T y, T z) : Base_vector(3){
                    (*this)[0] = x;
                    (*this)[1] = y;
                    (*this)[2] = z;
                }

                /**
                 * @brief Creates a 3D vector from vector_expression
                 * @param r [in] an ublas vector_expression
                 */
                template <class R> Vector3D (const boost::numeric::ublas::vector_expression<R>& r) : Base_vector(r)
                {}

                /**
                 * @brief Assigns vector expression to 3D vector object
                 * @param r [in] an ublas vector_expression
                 */
                template <class R> void operator=(const boost::numeric::ublas::vector_expression<R>& r)
                {
                    Base_vector::operator=(r);
                }
	    /**
	     * @brief Sets the values of the 3D vector
	     * @param x [in] @f$ x @f$
	     * @param y [in] @f$ y @f$
	     * @param z [in] @f$ z @f$
	     */
        void set(T x, T y, T z) {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
        }

		/**
		 * @brief Returns reference to vector element
		 * @param i [in] index in the vector \f$i\in \{0,1,2\} \f$
		 * @return const reference to element
		 */
		const T& operator()(size_t i) const {
			assert(isValidIndex(i));
		    return (*this)[i];
		}

		/**
		 * @brief Returns reference to vector element
		 * @param i [in] index in the vector \f$i\in \{0,1,2\} \f$
		 * @return reference to element
		 */
		T& operator()(size_t i) {
//			assert(isValidIndex(i));
			Assert(!ARG_CHECK || isValidIndex(i), IndexOutOfBoundsException("IndexOutOfBoundsException in operator() of Vector3D.hpp!"));
		    return (*this)[i];
		}

		bool operator==(const Vector3D& rhs) const {
			return ((*this)(0)==rhs(0) && (*this)(1) == rhs(1) && (*this)(2) == rhs(2));
		}

		bool isValidIndex(size_t index) const {
			return (index>=0 && index<=2);
		}



                /**
                 * @brief Calculates the 3D vector cross product @f$ \mathbf{v1} \times \mathbf{v2} @f$
                 * @param v1 [in] @f$ \mathbf{v1} @f$
                 * @param v2 [in] @f$ \mathbf{v2} @f$
                 *
                 * @return the 3D vector cross product @f$ \mathbf{v1} \times \mathbf{v2} @f$
                 *
                 * The 3D vector cross product is defined as:
                 *
                 * @f$
                 * \mathbf{v1} \times \mathbf{v2} = \left[\begin{array}{c}
                 *  v1_y * v2_z - v1_z * v2_y \\
                 *  v1_z * v2_x - v1_x * v2_z \\
                 *  v1_x * v2_y - v1_y * v2_x
                 * \end{array}\right]
                 * @f$
                 */
                friend Vector3D<T> cross(const Vector3D<T>& v1, const Vector3D<T>& v2){
                        return Vector3D<T>(
                                v1[1] * v2[2] - v1[2] * v2[1],
                                v1[2] * v2[0] - v1[0] * v2[2],
                                v1[0] * v2[1] - v1[1] * v2[0]
                                );
		}

		/**
		 * @brief Returns the normalized vector \f$\mathbf{n}=\frac{\mathbf{v}}{\|\mathbf{v}\|} \f$.
		 * In case \f$ \|mathbf{v}\| = 0\f$ the zero vector is returned.
		 * @param v [in] \f$ \mathbf{v} \f$ which should be normalized
		 * return the normalized vector \f$ \mathbf{n} \f$
		 */
		friend Vector3D<T> normalize(const Vector3D<T>& v) {
		    T length = norm_2(v);
		    if (length != 0)
			return Vector3D<T>(v(0)/length, v(1)/length, v(2)/length);
		    else
			return Vector3D<T>(0,0,0);
		}



            };

       /** Typedef for 3D vector with floating point values */
       typedef Vector3D<float_t> Vector3f;
	   /** Typedef for 3D vector with integer values */
	   typedef Vector3D<int> Vector3i;
	   /** Typedef for 3D vector with unsigned char values */
	   typedef Vector3D<uchar> Vector3uc;

       /**@}*/

} // end of namespace impact_tr4

#endif /*VECTOR3D_HPP_*/
