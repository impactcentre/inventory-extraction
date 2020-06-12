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
#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <iostream>
#include <exception>

namespace impact_tr4
{
	// Define constant variable ARG_CHECK, for usage with Assert method
	#ifdef NDEBUG
	const bool ARG_CHECK = false; // set ARG_CHECK to false if NDEBUG is set! 
	#else
	const bool ARG_CHECK = true; // set ARG_CHECK to true is NDEBUG not set!
	#endif

	/** User defined assertion method throwing an exception */
	template <class X, class A> inline void Assert(A assertion)
	{
		if (!assertion) throw X();
	}
 
	/** User defined assertion method throwing a given exception object. */
	template <class A, class E> inline void Assert(A assertion, E except)
	{
		if (!assertion) throw except;
	}

	/*! Generic exception class. All other exception classes should derive from this class. */
	class Exception;
	/** A class for file not found related exceptions. */
	class FileNotFoundException;	
	/** A class for image related exceptions. */
	class ImageException;
	/** A class for all index-out-of-bounds related exceptions. */
	class IndexOutOfBoundsException;
	/** A class for all exception related to the wrong specification of dimensions. */
	class WrongDimensionException;
	/** A class for all xml related exceptions. */
	class XMLException;
	/** A class for exceptions, where some kind of data was not found or not computed yet. */
	class NoDataException;
	/* A class for all exceptions, where a specific type is unsupported. */
	class UnsupportedTypeException;
	/* A class for all exceptions, where a null pointer occurs accidentally. */
	class NullPointerException;
	  
	class Exception : public std::exception
	{
		std::string _error_type;
		std::string _error_msg;
		std::string _msg;

	public:
		/** Constructs the exception object using msg as error message. */
		Exception(std::string msg = std::string("An exception has ocurred!"))
		{
			_error_type = "Exception";
			_error_msg = msg;
			setMessage();
		}

		virtual ~Exception() throw() { ; }
		/** Set the description for the type of error. */
		void setErrorType(std::string typeStr) { _error_type = typeStr; setMessage(); }
		/** Sets the error message for this exception. */
		void setErrorMsg(std::string messageStr) { _error_msg = messageStr; setMessage(); }
		void setMessage() { _msg = _error_type; _msg += ": "; _msg += _error_msg; }
		/** Returns error message consisting of error type and message. */
		virtual const char* what() const throw()
		{ return _msg.c_str(); }
	};
	
	class FileNotFoundException : public Exception
	{
	public:
		//! Constructs the exception object using msg as error message.
		FileNotFoundException(std::string msg = std::string("An exception has ocurred!"))
		: Exception(msg) { this->setErrorType("FileNotFoundException"); }
	};

	class ImageException : public Exception
	{
	public:
		//! Constructs the exception object using msg as error message.
		ImageException(std::string msg = std::string("An exception has ocurred!"))
		: Exception(msg) { this->setErrorType("ImageException"); }
	};

	class IndexOutOfBoundsException : public Exception
	{
	public:
		//! Constructs the exception object using msg as error message.
		IndexOutOfBoundsException(std::string msg = std::string("An exception has ocurred!"))
		: Exception(msg) { this->setErrorType("IndexOutOfBoundsException"); }
	};
	
	class WrongDimensionException : public Exception
	{
	public:
		//! Constructs the exception object using msg as error message.
		WrongDimensionException(std::string msg = std::string("An exception has ocurred!"))
		: Exception(msg) { this->setErrorType("WrongDimensionException"); }
	};

	class XMLException : public Exception
	{
	public:
		//! Constructs the exception object using msg as error message.
		XMLException(std::string msg = std::string("An exception has ocurred!"))
		: Exception(msg) { this->setErrorType("XMLException"); }
	};

	class NoDataException : public Exception
	{
	public:
		//! Constructs the exception object using msg as error message.
		NoDataException(std::string msg = std::string("An exception has ocurred!"))
		: Exception(msg) { this->setErrorType("NoDataException"); }
	};
	
	class UnsupportedTypeException : public Exception
	{
	public:
		//! Constructs the exception object using msg as error message.
		UnsupportedTypeException(std::string msg = std::string("An exception has ocurred!"))
		: Exception(msg) { this->setErrorType("UnsupportedTypeException"); }
	};
	
	class NullPointerException : public Exception
	{
	public:
		//! Constructs the exception object using msg as error message.
		NullPointerException(std::string msg = std::string("A null-pointer-exception has ocurred!"))
		: Exception(msg) { this->setErrorType("NullPointerException"); }
	};

	
}

#endif /*EXCEPTION_HPP_*/
