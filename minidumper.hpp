#ifndef HELLO_DUMP__MINIDUMPER_HPP
#define HELLO_DUMP__MINIDUMPER_HPP

#include <Windows.h>

namespace dump {

bool MakeMinidump(EXCEPTION_POINTERS*);

} // namespace dump

#endif // HELLO_DUMP__MINIDUMPER_HPP
