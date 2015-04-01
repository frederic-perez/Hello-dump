// --

#include <iostream>
//#include <string>

#include <Windows.h> // Note: Must be the first one to avoid compiler errors!
#include <Dbghelp.h>

// Code below based on
// http://stackoverflow.com/questions/5028781/
// c-how-to-write-a-sample-code-that-will-crash-and-produce-dump-file

bool
make_minidump(EXCEPTION_POINTERS* e)
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
		std::cout << __FUNCTION__ << ": succeeded" << std::endl;
	else
		std::cerr << __FUNCTION__ << ": Failed\n";

	return succeeded;
}

LONG CALLBACK
unhandled_handler(EXCEPTION_POINTERS* e)
{
	const bool succeeded = make_minidump(e);
	if (succeeded)
		std::cout << __FUNCTION__ << ": make_minidump succeeded" << std::endl;
	else
		std::cerr << __FUNCTION__ << ": Error: make_minidump failed\n";
	return EXCEPTION_CONTINUE_SEARCH;
}

int
main(int /*argc*/, char* /*argv*/[])
{
	std::cout << "Hello, dump!" << std::endl;

	SetUnhandledExceptionFilter(unhandled_handler);

	std::cout << "Strange value: " << *(int*)0 << std::endl;

	std::cout << "Bye, dump!" << std::endl;
	return EXIT_SUCCESS;
}

// -- eof
