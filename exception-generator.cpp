#include <iostream>
#include <string>

#include "aux-raw.hpp"
#include "exception-generator.hpp"

namespace {

template <typename T>
void
OutputValue(const std::string& a_function, const std::string& a_label, const T& a_value)
{
  std::cout << a_function << ": " << a_label << "=" << std::flush; // caution!
  std::cout << a_value << std::endl;
}

#define OUTPUT_VALUE(value) OutputValue(__FUNCTION__, #value, value)

#pragma warning(disable : 4723) // Disable warning "potential divide by 0"
#pragma warning(disable : 4789) // Disable warning "destination of memory copy is too small"

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
  auto p = new int(Six_six_six);
  delete p;
  *p = Six_six_six;
  OUTPUT_VALUE(*p);
}

void
DeleteDeletedPtr()
{
  auto const p = new int(Six_six_six);
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
  auto const v = new int[Ten];
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

[[ noreturn ]]
void
CppThrow666()
{
  throw Six_six_six;
}

[[ noreturn ]]
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
FactorialRecursive(const size_t n)
{
  return n <= 1 ? 1 : n * FactorialRecursive(n - 1);
}

void
ComputeFactorialOfABigNumber()
{
  const size_t result = FactorialRecursive(22);
  OUTPUT_VALUE(result);
}

#define DANGEROUS_FUNCTION(f) dump::DangerousFunction(#f, f, true)
#define SUICIDAL_FUNCTION(f) dump::DangerousFunction(#f, f, false)

const std::vector<dump::DangerousFunction> functions{
  DANGEROUS_FUNCTION(IntZeroDiv),
  DANGEROUS_FUNCTION(FloatZeroDiv),
  DANGEROUS_FUNCTION(DoubleZeroDiv),
  DANGEROUS_FUNCTION(InvalidFloatToIntCast),
  DANGEROUS_FUNCTION(NullPtrAccess),
  SUICIDAL_FUNCTION(DeletedPtrAccess),
  SUICIDAL_FUNCTION(DeleteDeletedPtr),
  DANGEROUS_FUNCTION(OutOfBoundsStdVectorIndexing),
  SUICIDAL_FUNCTION(OutOfBoundsOfOldCArrayIndexing),
  SUICIDAL_FUNCTION(OutOfBoundsDynamicOldCArrayIndexing),
  DANGEROUS_FUNCTION(InvalidIteratorAccess),
  DANGEROUS_FUNCTION(CppThrow666),
  DANGEROUS_FUNCTION(CppThrowStdException),
  DANGEROUS_FUNCTION(CaughtCppThrowStdException),
  DANGEROUS_FUNCTION(ComputeFactorialOfABigNumber)
};

} // namespace

dump::DangerousFunctions::DangerousFunctions() : d_functions(functions)
{}
