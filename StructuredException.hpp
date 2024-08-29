#ifndef HELLO_DUMP__STRUCTUREDEXCEPTION_HPP
#define HELLO_DUMP__STRUCTUREDEXCEPTION_HPP

#include <Windows.h> // _EXCEPTION_POINTERS

#include <exception>

namespace dump {

class StructuredException : public std::exception {
public:
  StructuredException(unsigned int nSeCode, _EXCEPTION_POINTERS*);
  StructuredException(StructuredException&);

  unsigned int GetSeCode() const;
  _EXCEPTION_POINTERS* GetSePointers() const;
  void* GetExceptionAddress() const;

  void Delete();
  int ReportError(unsigned int a_type, unsigned int a_IDHelp) const;

  bool GetErrorMessage(std::string& a_description) const;

private:
  unsigned int m_nSeCode;
  _EXCEPTION_POINTERS* m_pExcPointers;
};

} // namespace dump

#endif // HELLO_DUMP__STRUCTUREDEXCEPTION_HPP
