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

void
RunException(const dump::DangerousFunction& func)
{
	std::cout << "Going to execute dangerous function " << func.GetName()
		<< std::endl;
	func();
}

DWORD WINAPI
ExceptionThread(LPVOID lpParameter)
{
	const dump::DangerousFunction* const pFunc =
		static_cast<dump::DangerousFunction*>(lpParameter);

	RunException(*pFunc);

	return 0;
}

void
RunExceptionOnThread(const dump::DangerousFunction& func) 
{
	std::cout << "Wait for thread..." << std::endl;

	const HANDLE hThread =
		CreateThread(
			NULL, 0, ExceptionThread,
			static_cast<LPVOID>(const_cast<dump::DangerousFunction*>(&func)),
			0, NULL);

	// Do not access same objects than the thread (func, cout,...)
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
}

void
RunException(bool runOnThread, const dump::DangerousFunction& func)
{
	if (runOnThread)
		RunExceptionOnThread(func);
	else
		RunException(func);
}

const dump::DangerousFunction&
AskForDangerousFunction(const dump::DangerousFunctions& exceptions)
{
	std::cout << "Choose a exception:" << std::endl;
	
	for (size_t i=0; i < exceptions.m_functions.size(); ++i)
		std::cout << "\t[" << i << "] -> " <<
			exceptions.m_functions[i].GetName() << std::endl;

	size_t i;
	do { // Loop to account for silly users
		std::string line;
		std::getline(std::cin, line);
		std::stringstream(line) >> i;
	} while (i >= exceptions.m_functions.size());

	return exceptions.m_functions[i];
}

bool
AskForRunOnThread()
{
	std::cout << "Run on worker thread?: [y/n]" << std::endl;

	char thread;
	std::cin >> thread;

	return thread == 'y';
}

void InvalidArguments()
{
	std::cout << "Invalid arguments" << std::endl;
}

class ArgsReader
{
public:
	enum State {eAsk, eAllCrashes, eSpecificCrash, eInvalidArgs};

	ArgsReader(int argc, char* argv[])
	:	m_state(eAsk),
		m_dangerousFunctionID(0),
		m_runOnThread(false)
	{
		for (int i=1; i<argc; ++i)
		{
			const std::string allFlag = "/all";
			const std::string crashNumFlag = "/c";
			const std::string threadFlag = "/t";

			if (std::string(argv[i]) == allFlag)
				m_state = eAllCrashes;
			else if (std::string(argv[i],crashNumFlag.size()) == crashNumFlag)
			{
				std::stringstream(std::string(argv[i]+crashNumFlag.size()))
					>> m_dangerousFunctionID;
				m_state = eSpecificCrash;
			}
			else if (std::string(argv[i]) == threadFlag)
				m_runOnThread = true;
		}
	}

	State m_state;
	size_t m_dangerousFunctionID;
	bool m_runOnThread;
};

} // namespace

int
main(int argc, char* argv[])
{
	const ArgsReader args(argc, argv);
	const dump::DangerousFunctions functions;
	switch(args.m_state)
	{
	case ArgsReader::eAsk:
		RunException(AskForRunOnThread(),
			AskForDangerousFunction(functions));

		std::cout << "Success" << std::endl;
		break;

	case ArgsReader::eAllCrashes:
		typedef dump::DangerousFunctions::FunctionIT IT;
		for (IT it = functions.m_functions.begin();
			it != functions.m_functions.end(); ++it)
			RunException(args.m_runOnThread, *it);

		std::cout << "Success" << std::endl;
		break;

	case ArgsReader::eSpecificCrash:
		if (args.m_dangerousFunctionID < functions.m_functions.size())
		{
			RunException(args.m_runOnThread,
				functions.m_functions[args.m_dangerousFunctionID]);
			std::cout << "Success" << std::endl;
		}
		else
			InvalidArguments();
		break;


	default:
		InvalidArguments();
		break;
	}

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
