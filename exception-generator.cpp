// --

#include <iostream>
#include <string>

#include "aux-raw.h"
#include "exception-generator.h"

namespace {

template <typename T>
void
OutputValue(
	const std::string& a_function,
	const std::string& a_label,
	const T& a_value)
{
	std::cout << a_function << ": " << a_label << "=" << std::flush; // caution!
	std::cout << a_value << std::endl;
}


#define OUTPUT_VALUE(value) \
	OutputValue(__FUNCTION__, #value, value)

#pragma warning(disable:4723) // Disable potential divide by 0 warning

template <typename T>
T
ZeroDiv()
{
	T zero = 0;
	return 666/zero;
}

void
IntZeroDiv()
{ OUTPUT_VALUE(ZeroDiv<int>()); }

void
FloatZeroDiv()
{ OUTPUT_VALUE(ZeroDiv<float>()); }

void
DoubleZeroDiv()
{ OUTPUT_VALUE(ZeroDiv<double>()); }

void
InvalidFloatToIntCast()
{ OUTPUT_VALUE(static_cast<int>(ZeroDiv<float>())); }

#pragma warning(default:4723)

void
NullPtrAccess()
{
	int* p = 0;
	*p = 666;
	OUTPUT_VALUE(*p);
}

void
DeletedPtrAccess()
{
	int* p = new int(666);
	delete p;
	*p = 666;
	OUTPUT_VALUE(*p);
}

void
OutOfBoundsStdVectorIndexing()
{
	std::vector<int> v;
	v[0] = 666;
	OUTPUT_VALUE(v[0]);
}

void
OutOfBoundsOfOldCArrayIndexing()
{
	int v[10] = {0};
	v[42] = 666;
	OUTPUT_VALUE(v[42]);
}

void
OutOfBoundsDynamicOldCArrayIndexing()
{
	int* const v = new int[10];
	v[10] = 666;
	OUTPUT_VALUE(v[10]);
	delete [] v;
}

void
InvalidIteratorAccess()
{
	std::vector<int> v;
	OUTPUT_VALUE(*(v.end()));
}

void
CppThrow666()
{	throw 666; }

#define DANGEROUS_FUNCTION(f) dump::DangerousFunction(#f, f)

template <typename T, size_t N>
size_t ArraySize( const T(&)[N] ) {return N;}

const std::vector<dump::DangerousFunction>&
GetTheVector()
{
	static const dump::DangerousFunction functions[] = {
		DANGEROUS_FUNCTION(IntZeroDiv),
		DANGEROUS_FUNCTION(FloatZeroDiv),
		DANGEROUS_FUNCTION(DoubleZeroDiv),
		DANGEROUS_FUNCTION(InvalidFloatToIntCast),
		DANGEROUS_FUNCTION(NullPtrAccess),
		DANGEROUS_FUNCTION(DeletedPtrAccess),
		DANGEROUS_FUNCTION(OutOfBoundsStdVectorIndexing),
		DANGEROUS_FUNCTION(OutOfBoundsOfOldCArrayIndexing),
		DANGEROUS_FUNCTION(OutOfBoundsDynamicOldCArrayIndexing),
		DANGEROUS_FUNCTION(InvalidIteratorAccess),
		DANGEROUS_FUNCTION(CppThrow666)
	};
	
	static
	const std::vector<dump::DangerousFunction> theVector(
		&functions[0], &functions[ArraySize(functions)]);

	return theVector;
}
} // namespace

dump::DangerousFunctions::DangerousFunctions()
: m_functions(GetTheVector())
{}

// -- eof
