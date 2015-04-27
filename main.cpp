// --

#include <iostream>
#include <sstream>
#include <string>

#include "StructuredException.h"

#include "aux-raw.h"
#include "exception-generator.h"
#include "minidumper.h"

// Code below based on
// http://stackoverflow.com/questions/5028781/
// c-how-to-write-a-sample-code-that-will-crash-and-produce-dump-file

void
SETranslator(UINT a_codeSE, _EXCEPTION_POINTERS* a_excPointers)
{	throw new dump::StructuredException(a_codeSE, a_excPointers); }

LONG CALLBACK
UnhandledHandler(EXCEPTION_POINTERS* e)
{
	const bool succeeded = dump::MakeMinidump(e);
	if (succeeded)
		std::cout << __func__ << ": dump::MakeMinidump succeeded" << std::endl;
	else
		std::cerr << __func__ << ": Error: dump::MakeMinidump failed\n";
	return EXCEPTION_CONTINUE_SEARCH;
}

namespace {
void RunException(const dump::ExceptionFunc& func) {
	std::cout << "Run " << func.GetName() << std::endl;
	func();
}

DWORD WINAPI ExceptionThread(LPVOID lpParameter) {
	const dump::ExceptionFunc* const pFunc =
		static_cast<dump::ExceptionFunc*>(lpParameter);

	RunException(*pFunc);

	return 0;
}

void RunExceptionOnThread(const dump::ExceptionFunc& func) {

	std::cout << "Wait for thread..." << std::endl;

	const HANDLE hThread = CreateThread(NULL, 0, ExceptionThread,
		static_cast<LPVOID>(const_cast<dump::ExceptionFunc*>(&func)),
		0, NULL);
	// Do not access same objects than the thread (func, cout,...)
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
}

const dump::ExceptionFunc&
AskForException(const dump::Exceptions& exceptions ) {
	std::cout << "Choose a exception:" << std::endl;
	
	for( size_t n = 0; n < exceptions.m_functions.size(); ++n)
	{
		std::cout << "\t[" << n << "] -> " <<
			exceptions.m_functions[n].GetName() << std::endl;
	}

	size_t n;
	do {
		std::string line;
		std::getline(std::cin, line);
		std::stringstream(line) >> n;
	} while (n >= exceptions.m_functions.size());

	return exceptions.m_functions[n];
}

bool AskForRunOnThread() {
	std::cout << "Run on worker thread?: [y/n]" << std::endl;

	char thread;
	std::cin >> thread;

	return thread == 'y';
}

} // namespace

int
main(int /*argc*/, char* /*argv*/[])
{
	dump::Exceptions exceptions;
	
	const dump::ExceptionFunc& func = AskForException(exceptions);

	if (AskForRunOnThread())
		RunExceptionOnThread(func);
	else
		RunException(func);

	std::cout << "Success" << std::endl;
//#ifdef EHA
//	_set_se_translator(SETranslator);
//#endif
//	SetUnhandledExceptionFilter(UnhandledHandler);
//
//	//using dump::StructuredException;
//
//	try {
//		dump::GenerateException(/*dump::eBadAlloc*/);
//	} catch (dump::StructuredException* e) {
//		std::string description;
//		e->GetErrorMessage(description);
//		e->Delete();
//		std::cerr << __func__
//			<< ": Caught a dump::StructuredException in main\n" << description
//			<< '\n';
//	} catch (const std::exception& e) {
//		std::cerr << __func__
//			<< ": Caught a std::exception in main\n" << e.what() << '\n';
//	} catch (...) { // catch block will only be executed under /EHa
//		std::cerr << __func__
//			<< ": Caught a ... exception in main\n";
//	}
//
//	std::cout << "Bye, dump!" << std::endl;
	return EXIT_SUCCESS;
}

// -- eof
