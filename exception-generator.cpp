// --

#include <iostream>

#include "aux-raw.h"
#include "exception-generator.h"

namespace {

template <typename T>
void ZeroDiv()
{
	T zero = 0;
	T zeroDiv = 666/zero;
	zeroDiv += 1;
}

void IntZeroDiv() {
	ZeroDiv<int>();
}

void FloatZeroDiv() {
	ZeroDiv<float>();
}

void NullPtrDereference() {
	int* p = 0;
	*p = 666;
}

void DeletedPtrDereference() {
	int* p = new int(666);
	delete p;
	*p = 666;
}

void OutOfBoundsStdVectorIndexing() {
   std::vector<int> v;
   v[0] = 666;
}

void OutOfBoundsArrayIndexing() {
	int v[10] = {0};
	v[10] = 666;
}

void OutOfBoundsDynamicArrayIndexing() {
	int* const v = new int[10];
	v[10] = 666;
	delete [] v;
}

void CppThrowInt() {
   throw 666;
}

#define EXCEPTION_FUNC(f) dump::ExceptionFunc(#f, f)

template <typename T, size_t N>
size_t ArraySize( const T(&)[N] ) {return N;}

std::vector<dump::ExceptionFunc> GetFunctionSet() {
	static const dump::ExceptionFunc functions[] = {
		EXCEPTION_FUNC(IntZeroDiv),
		EXCEPTION_FUNC(FloatZeroDiv),
		EXCEPTION_FUNC(NullPtrDereference),
		EXCEPTION_FUNC(DeletedPtrDereference),
		EXCEPTION_FUNC(OutOfBoundsStdVectorIndexing),
		EXCEPTION_FUNC(OutOfBoundsArrayIndexing),
		EXCEPTION_FUNC(OutOfBoundsDynamicArrayIndexing),
		EXCEPTION_FUNC(CppThrowInt)
	};

	return std::vector<dump::ExceptionFunc>(&functions[0],
		&functions[ArraySize(functions)]);
}
} // namespace

dump::Exceptions::Exceptions()
: m_functions(GetFunctionSet())
{
}

// -- eof
