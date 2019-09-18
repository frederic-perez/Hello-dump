#pragma once

#include <Windows.h>

namespace dump {

bool MakeMinidump(EXCEPTION_POINTERS*);

} // namespace dump
