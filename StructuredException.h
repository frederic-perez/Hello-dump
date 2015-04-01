// --

#pragma once

#include <exception>

namespace dump {

class StructuredException : public std::exception
{
public:
	StructuredException(unsigned int nSeCode, _EXCEPTION_POINTERS*);
	StructuredException(StructuredException&);

	unsigned int GetSeCode() const;
	_EXCEPTION_POINTERS* GetSePointers() const;
	void* GetExceptionAddress() const;

	void Delete();
	int ReportError(unsigned int nType, unsigned int nIDHelp) const;

	bool
	GetErrorMessage(
		std::string& a_description, unsigned int* pnHelpContext = nullptr) const;

private:
	unsigned int m_nSeCode;
	_EXCEPTION_POINTERS* m_pExcPointers;
};

} // namespace dump

// -- eof