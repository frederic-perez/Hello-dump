// --

#include <iostream>

#include <Windows.h> // Note: Must be the first one to avoid compiler errors!
#include <Dbghelp.h>

#include "aux-raw.h"
#include "minidumper.h"

// Code below based on
// http://stackoverflow.com/questions/5028781/
// c-how-to-write-a-sample-code-that-will-crash-and-produce-dump-file

bool
dump::MakeMinidump(EXCEPTION_POINTERS* e)
{
	auto handleDbgHelp = LoadLibraryA("dbghelp");
	if (handleDbgHelp == nullptr) {
		std::cerr << __FUNCTION__ << ": Error: handleDbgHelp == nullptr\n";
		return false;
	}

	auto addressMiniDumpWriteDump =
		(decltype(&MiniDumpWriteDump))
		GetProcAddress(handleDbgHelp, "MiniDumpWriteDump");
	if (addressMiniDumpWriteDump == nullptr) {
		std::cerr << __FUNCTION__
			<< ": Error: addressMiniDumpWriteDump == nullptr\n";
		return false;
	}

	char name[MAX_PATH];
	{
		auto nameEnd =
			name + GetModuleFileNameA(GetModuleHandleA(0), name, MAX_PATH);
		SYSTEMTIME t;
		GetSystemTime(&t);
		wsprintfA(nameEnd - strlen(".exe"),
			"_%4d%02d%02d_%02d%02d%02d.dmp",
			t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	}

	auto handleFile =
		CreateFileA(
			name,
			GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
			0);
	if (handleFile == INVALID_HANDLE_VALUE) {
		std::cerr << __FUNCTION__
			<< ": Error: handleFile == INVALID_HANDLE_VALUE\n";
		return false;
	}

	MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
	exceptionInfo.ThreadId = GetCurrentThreadId();
	exceptionInfo.ExceptionPointers = e;
	exceptionInfo.ClientPointers = FALSE;

	auto dumped =
		addressMiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			handleFile,
			MINIDUMP_TYPE(MiniDumpWithIndirectlyReferencedMemory|MiniDumpScanMemory),
			e ? &exceptionInfo : nullptr,
			nullptr,
			nullptr);

	CloseHandle(handleFile);

	const bool succeeded = dumped == TRUE;

	if (succeeded)
		std::cout << __func__ << ": succeeded" << std::endl;
	else
		std::cerr << __func__ << ": Failed\n";

	return succeeded;
}

// -- eof
