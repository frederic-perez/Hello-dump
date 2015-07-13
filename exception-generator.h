// --

#pragma once

#include <vector>

namespace dump {

class DangerousFunction {
public:
	typedef void (*FuncDef)();

	DangerousFunction(const char* sName, FuncDef func, bool catchable = true)
	: m_sName(sName), m_func(func), m_catchable(catchable)
	{}

	const std::string& GetName() const { return m_sName; }
	bool IsCatchable() const { return m_catchable; }
	void operator()() const {	return m_func(); }

private:
	std::string m_sName;
	FuncDef m_func;
	bool m_catchable;
};

class DangerousFunctions { // ie. a container of DangerousFunction objects
public:
	typedef std::vector<DangerousFunction> ContainerOfDangerousFunctions;
	typedef ContainerOfDangerousFunctions::const_iterator FunctionIT;

	DangerousFunctions();
	const ContainerOfDangerousFunctions m_functions;

private:
	DangerousFunctions& operator=(const DangerousFunctions&); // Disable
};

} // namespace dump

// -- eof
