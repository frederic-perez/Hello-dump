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
DeleteDeletedPtr()
{
	int* p = new int(666);
	*p = 42; // Silly operation on *p to avoid Cppcheck warning
	delete p;
	delete p;
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
{ throw 666; }

void
CppThrowStdException()
{ throw std::exception(); }

void
CaughtCppThrowStdException() {
	try {
		throw std::exception();
	} catch (const std::exception&) {
		std::cerr << "Exception (std::exception) caught" << std::endl;
	}
}


#define DANGEROUS_FUNCTION(f) dump::DangerousFunction(#f, f, true)
#define SUICIDE_FUNCTION(f) dump::DangerousFunction(#f, f, false)

const std::vector<dump::DangerousFunction>&
GetTheVector()
{
	static const dump::DangerousFunction functions[] = {
		DANGEROUS_FUNCTION(IntZeroDiv),
		DANGEROUS_FUNCTION(FloatZeroDiv),
		DANGEROUS_FUNCTION(DoubleZeroDiv),
		DANGEROUS_FUNCTION(InvalidFloatToIntCast),
		DANGEROUS_FUNCTION(NullPtrAccess),
		SUICIDE_FUNCTION(DeletedPtrAccess),
		SUICIDE_FUNCTION(DeleteDeletedPtr),
		DANGEROUS_FUNCTION(OutOfBoundsStdVectorIndexing),
		SUICIDE_FUNCTION(OutOfBoundsOfOldCArrayIndexing),
		SUICIDE_FUNCTION(OutOfBoundsDynamicOldCArrayIndexing),
		DANGEROUS_FUNCTION(InvalidIteratorAccess),
		DANGEROUS_FUNCTION(CppThrow666),
		DANGEROUS_FUNCTION(CppThrowStdException),
		DANGEROUS_FUNCTION(CaughtCppThrowStdException)
	};
	
	static
	const std::vector<dump::DangerousFunction> theVector(
		&functions[0], &functions[aux::ArraySize(functions)]);

	return theVector;
}

} // namespace

dump::DangerousFunctions::DangerousFunctions()
: d_functions(GetTheVector())
{}

// -- eof
