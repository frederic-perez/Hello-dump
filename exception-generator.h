// --

#pragma once

#if _MSC_VER >= 1800 // That is, from Visual Studio 2013
#  include <functional>
#endif

#include <vector>

namespace dump {

class DangerousFunction {
public:
#if _MSC_VER >= 1800 // That is, from Visual Studio 2013
  using FuncDef = std::function<void()>;
#else
  typedef void (*FuncDef)();
#endif

  DangerousFunction(const char* sName, FuncDef func, bool catchable = true)
  : d_name(sName), d_func(std::move(func)), d_catchable(catchable)
  {}

  const std::string& GetName() const { return d_name; }
  bool IsCatchable() const { return d_catchable; }
  void operator()() const { return d_func(); }

private:
  std::string d_name;
  FuncDef d_func;
  bool d_catchable;
};

class DangerousFunctions { // ie. a container of DangerousFunction objects
public:
  using ContainerOfDangerousFunctions = std::vector<DangerousFunction>;

  DangerousFunctions();

  DangerousFunctions& operator=(const DangerousFunctions&) = delete;

  const ContainerOfDangerousFunctions d_functions;
};

} // namespace dump

// -- eof
