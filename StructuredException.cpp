// -- Examples on how to use C++11 features, and some other stuff

#include <iostream>
#include <sstream>
#include <windows.h> // _EXCEPTION_POINTERS

#include "aux-raw.h"
#include "StructuredException.h"

dump::StructuredException::StructuredException(
	unsigned int nSeCode, _EXCEPTION_POINTERS* pExcPointers)
: m_nSeCode(nSeCode),
	m_pExcPointers(pExcPointers)
{}

dump::StructuredException::StructuredException(StructuredException& a_rhs)
: m_nSeCode(a_rhs.m_nSeCode),
	m_pExcPointers(a_rhs.m_pExcPointers)
{}

unsigned int 
dump::StructuredException::GetSeCode() const
{	return m_nSeCode; }

_EXCEPTION_POINTERS*
dump::StructuredException::GetSePointers() const
{	return m_pExcPointers; }

void*
dump::StructuredException::GetExceptionAddress() const
{	return m_pExcPointers->ExceptionRecord->ExceptionAddress; }

void 
dump::StructuredException::Delete()
{
	delete this;
}

int
dump::StructuredException::ReportError(
	unsigned int a_type, unsigned int a_IDHelp) const
{
	/* TODO
	CString strMessage;
	GetErrorMessage(strMessage);
	*/
	std::cerr << __func__ << ": " //<< strMessage 
		<< ", type=" << a_type
		<< ", IDHelp=" << a_IDHelp << '\n';
	return 0; // return rc;
}

namespace {

void
SetErrorDescription(
	std::string& a_description,
	unsigned int a_SECode,
	_EXCEPTION_POINTERS* a_excPointers)
{
	std::ostringstream oss;
	oss << "Exception " << a_SECode << " at address 0x" << a_excPointers;
	a_description.assign(oss.str());
}

} // namespace

#define CASE(nSeCode, a_description) \
	case EXCEPTION_##nSeCode: \
		SetErrorDescription(\
			a_description, \
			EXCEPTION_##nSeCode, \
			(_EXCEPTION_POINTERS*)\
				m_pExcPointers->ExceptionRecord->ExceptionAddress); \
		break;

bool
dump::StructuredException::GetErrorMessage(
	std::string& a_description, unsigned int* pnHelpContext) const
{
	bool succeeded = true;

	if (pnHelpContext != nullptr)
		*pnHelpContext = 0;

	switch (m_nSeCode) {
		CASE(ACCESS_VIOLATION, a_description);
		CASE(DATATYPE_MISALIGNMENT, a_description);
		CASE(BREAKPOINT, a_description);
		CASE(SINGLE_STEP, a_description);
		CASE(ARRAY_BOUNDS_EXCEEDED, a_description);
		CASE(FLT_DENORMAL_OPERAND, a_description);
		CASE(FLT_DIVIDE_BY_ZERO, a_description);
		CASE(FLT_INEXACT_RESULT, a_description);
		CASE(FLT_INVALID_OPERATION, a_description);
		CASE(FLT_OVERFLOW, a_description);
		CASE(FLT_STACK_CHECK, a_description);
		CASE(FLT_UNDERFLOW, a_description);
		CASE(INT_DIVIDE_BY_ZERO, a_description);
		CASE(INT_OVERFLOW, a_description);
		CASE(PRIV_INSTRUCTION, a_description);
		CASE(IN_PAGE_ERROR, a_description);
		CASE(ILLEGAL_INSTRUCTION, a_description);
		CASE(NONCONTINUABLE_EXCEPTION, a_description);
		CASE(STACK_OVERFLOW, a_description);
		CASE(INVALID_DISPOSITION, a_description);
		CASE(GUARD_PAGE, a_description);
		CASE(INVALID_HANDLE, a_description);
	default:
		a_description.assign("Unknown exception");
		succeeded = false;
		break;
	}

	return succeeded;
}

// -- eof
