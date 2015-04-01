// --

#include <iostream>
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

int
main(int /*argc*/, char* /*argv*/[])
{
	std::cout << "Hello, dump!" << std::endl;

#ifdef EHA
	_set_se_translator(SETranslator);
#endif
	SetUnhandledExceptionFilter(UnhandledHandler);

	//using dump::StructuredException;

	try {
		dump::GenerateException();
	} catch (dump::StructuredException* e) {
		std::string description;
		e->GetErrorMessage(description);
		e->Delete();
		std::cerr << __func__
			<< ": Caught a dump::StructuredException in main\n" << description
			<< '\n';
	} catch (const std::exception& e) {
		std::cerr << __func__
			<< ": Caught a std::exception in main\n" << e.what() << '\n';
	} catch (...) { // catch block will only be executed under /EHa
		std::cerr << __func__
			<< ": Caught a ... exception in main\n";
	}

	std::cout << "Bye, dump!" << std::endl;
	return EXIT_SUCCESS;
}

// -- eof
