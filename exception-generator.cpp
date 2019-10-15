#include <iostream>
#include <string>

#include "aux-raw.h"
#include "exception-generator.h"

namespace {

template <typename T>
void
OutputValue(const std::string& a_function, const std::string& a_label, const T& a_value)
{
  std::cout << a_function << ": " << a_label << "=" << std::flush; // caution!
  std::cout << a_value << std::endl;
}

#define OUTPUT_VALUE(value) OutputValue(__FUNCTION__, #value, value)

#pragma warning(disable : 4723) // Disable potential divide by 0 warning

constexpr int Ten = 10;
constexpr int Forty_two = 42;
constexpr int Six_six_six = 666;

template <typename T>
T
ZeroDiv()
{
  T zero = 0;
  return Six_six_six / zero;
}

void
IntZeroDiv()
{
  OUTPUT_VALUE(ZeroDiv<int>());
}

void
FloatZeroDiv()
{
  OUTPUT_VALUE(ZeroDiv<float>());
}

void
DoubleZeroDiv()
{
  OUTPUT_VALUE(ZeroDiv<double>());
}

void
InvalidFloatToIntCast()
{
  OUTPUT_VALUE(static_cast<int>(ZeroDiv<float>()));
}

#pragma warning(default : 4723)

void
NullPtrAccess()
{
  int* p = nullptr;
  *p = Six_six_six;
  OUTPUT_VALUE(*p);
}

void
DeletedPtrAccess()
{
  int* p = new int(Six_six_six);
  delete p;
  *p = Six_six_six;
  OUTPUT_VALUE(*p);
}

void
DeleteDeletedPtr()
{
  int* p = new int(Six_six_six);
  *p = Forty_two; // Silly operation on *p to avoid Cppcheck warning
  delete p;
  delete p;
}

void
OutOfBoundsStdVectorIndexing()
{
  std::vector<int> v;
  v[0] = Six_six_six;
  OUTPUT_VALUE(v[0]);
}

void
OutOfBoundsOfOldCArrayIndexing()
{
  int v[Ten] = {0};
  v[Forty_two] = Six_six_six;
  OUTPUT_VALUE(v[Forty_two]);
}

void
OutOfBoundsDynamicOldCArrayIndexing()
{
  int* const v = new int[Ten];
  v[Ten] = Six_six_six;
  OUTPUT_VALUE(v[Ten]);
  delete[] v;
}

void
InvalidIteratorAccess()
{
  std::vector<int> v;
  OUTPUT_VALUE(*(v.end()));
}

void
CppThrow666()
{
  throw Six_six_six;
}

void
CppThrowStdException()
{
  throw std::exception();
}

void
CaughtCppThrowStdException()
{
  try {
    throw std::exception();
  } catch (const std::exception&) {
    std::cerr << "Exception (std::exception) caught\n";
  }
}

size_t
FactorialRecursive(const size_t a_n)
{
  return a_n <= 1 ? 1 : a_n * FactorialRecursive(a_n - 1);
}

void
ComputeFactorialOfABigNumber()
{
  const size_t result = FactorialRecursive(22);
  OUTPUT_VALUE(result);
}

#define DANGEROUS_FUNCTION(f) dump::DangerousFunction(#f, f, true)
#define SUICIDE_FUNCTION(f) dump::DangerousFunction(#f, f, false)

const std::vector<dump::DangerousFunction>&
GetTheVector()
{
  static const dump::DangerousFunction functions[] = {DANGEROUS_FUNCTION(IntZeroDiv),
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
                                                      DANGEROUS_FUNCTION(CaughtCppThrowStdException),
                                                      DANGEROUS_FUNCTION(ComputeFactorialOfABigNumber)};

  static const std::vector<dump::DangerousFunction> theVector(&functions[0], &functions[aux::ArraySize(functions)]);

  return theVector;
}

} // namespace

dump::DangerousFunctions::DangerousFunctions() : d_functions(GetTheVector())
{}
