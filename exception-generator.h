// --

#pragma once

#include <vector>

namespace dump {

class DangerousFunction {
public:
	typedef void (*FuncDef)();

	DangerousFunction(const char* sName, FuncDef func)
	: m_sName(sName), m_func(func)
	{}

	const std::string& GetName() const { return m_sName; }
	void operator()() const {	return m_func(); }

private:
	std::string m_sName;
	FuncDef m_func;
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
