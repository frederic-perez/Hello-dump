// --

#pragma once

#include <vector>

namespace dump {

class ExceptionFunc {
public:
	typedef void (*FuncDef)();

	ExceptionFunc(const char* sName, FuncDef func)
		: m_sName(sName), m_func(func) {}

	const std::string& GetName() const { return m_sName; }
	void operator()() const {return m_func();}

private:
	std::string m_sName;
	FuncDef m_func;
};

class Exceptions {
public:
	typedef std::vector<ExceptionFunc> ExcCont;
	typedef ExcCont::const_iterator ExcIT;

	Exceptions();
	const ExcCont m_functions;

private:
	Exceptions& operator=(const Exceptions&); // Disable
};

} // namespace dump

// -- eof
