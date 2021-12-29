#pragma once

#include <functional>
#include <vector>

namespace dump {

class DangerousFunction {
public:
  using FuncDef = std::function<void()>;

  DangerousFunction(const char* sName, FuncDef func, bool catchable = true)
  : d_name(sName), d_func(std::move(func)), d_catchable(catchable)
  {}

  const std::string& GetName() const { return d_name; }
  bool IsCatchable() const { return d_catchable; }
  void operator()() const { return d_func(); }

private:
  const std::string d_name;
  FuncDef d_func;
  const bool d_catchable;
};

class DangerousFunctions { // ie. a container of DangerousFunction objects
public:
  using ContainerOfDangerousFunctions = std::vector<DangerousFunction>;

  DangerousFunctions();

  DangerousFunctions& operator=(const DangerousFunctions&) = delete;

  const ContainerOfDangerousFunctions d_functions;
};

} // namespace dump
