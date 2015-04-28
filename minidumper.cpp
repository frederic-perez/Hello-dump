// --

#include <iostream>

#include <Windows.h> // Note: Must be the first one to avoid compiler errors!

#include "aux-raw.h"
#include "minidumper.h"

// Code available for example at
// http://stackoverflow.com/questions/5028781/
// c-how-to-write-a-sample-code-that-will-crash-and-produce-dump-file

bool
dump::MakeMinidump(EXCEPTION_POINTERS* /*e*/)
{
	return false; // TODO
}

// -- eof
