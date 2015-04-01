// --

#include <iostream>

#include "aux-raw.h"
#include "exception-generator.h"

namespace {

void
fail()
{ // generates SE and attempts to catch it using catch(...)
	// Copied and slightly edited from
	// https://msdn.microsoft.com/en-us/library/swezty51.aspx
	try {
		int i = 20, j = 1;
		for (; i > 0;)
			if (i > 0)
				--i;
		j /= i;	// This will throw a SE (divide by zero).
		std::cout << __func__ << ": j = " << j << std::endl;
	} catch (...) { // catch block will only be executed under /EHa
		std::cerr << __func__ << ": Caught an exception in catch (...)\n";
	}
}

} // namespace

void
dump::GenerateException()
{
	std::cout << __func__ << ": Strange value: " << *(int*)0 << std::endl;
/*
#undef FCPX_TRY_FAIL_20150330
#ifdef FCPX_TRY_FAIL_20150330
	__try {
		fail();
	}
	// __except will only catch an exception here...
	__except (EXCEPTION_EXECUTE_HANDLER) {
		// ...if the exception was not caught by the catch(...) inside fail()
		std::cerr << __func__
			<< ": An exception was caught in __except\n";
	}
#endif // FCPX_TRY_FAIL_20150330

	enum { eBadAlloc, eDivByZero, eBadPointerAccess, eThrowMessage };

	switch (eThrowMessage) {
	case eBadAlloc:
		throw std::bad_alloc();
	case eDivByZero:
		// A3dGlobals::SetA3dThreadUnhandledExceptionHandler FAILS, even when
		// setting FloatingPointExceptions to true.
	{
		int i = 0, j = 1;
		std::cout << __func__ << ": 1/0 is about to be computed" << std::endl;
		//#ifndef FCPX_TRY_FAIL_20150330
		//			try {
		//#endif
		j /= i;	// This will throw a SE (divide by zero).
		std::cout << __func__ << ": oneDividedByZero = " << j << std::endl;
		//#ifndef FCPX_TRY_FAIL_20150330
		//			} catch (...) {
		//				std::cerr << __func__ << ": 1/0 exception caught" << std::endl;
		//			}
		//#endif
	}
	break;
	case eBadPointerAccess:
		// A3dGlobals::SetA3dThreadUnhandledExceptionHandler FAILS
	{
		int* pointer = new int(666);
		delete pointer;
		pointer = 0;
		std::cout << __func__
			<< ": I am about to dereference a null pointer" << std::endl;
		*pointer = 42;
	}
	break;
	case eThrowMessage:
		// A3dGlobals::SetA3dThreadUnhandledExceptionHandler succeeds
	{
		const char* message = "Bye, cruel world";
		std::cout << __func__
			<< ": I am about to throw \"" << message << "\"" << std::endl;
		throw message;
	}
	default:
		throw 666;
	}
	*/
}

// -- eof
