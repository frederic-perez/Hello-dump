#include <csignal>
#include <iostream>
#include <sstream>
#include <string>

#include "StructuredException.hpp"

#include "aux-raw.hpp"
#include "exception-generator.hpp"
#include "minidumper.hpp"

// Code below based on
// http://stackoverflow.com/questions/5028781/
// c-how-to-write-a-sample-code-that-will-crash-and-produce-dump-file

LONG CALLBACK
UnhandledHandler(EXCEPTION_POINTERS* e)
{
  std::cerr << __func__ << ": Exception caught (" << std::hex << e->ExceptionRecord->ExceptionCode << ")\n";
  exit(EXIT_SUCCESS);
  // return EXCEPTION_EXECUTE_HANDLER;

#undef TODO_MAKE_MINI_DUMP
#ifdef TODO_MAKE_MINI_DUMP
  const bool succeeded = dump::MakeMinidump(e);
  if (succeeded)
    std::cout << __func__ << ": dump::MakeMinidump succeeded" << std::endl;
  else
    std::cerr << __func__ << ": Error: dump::MakeMinidump failed\n";
  return EXCEPTION_CONTINUE_SEARCH;
#endif
}

void
SignalHandler(int signal)
{
  std::cout << __func__ << " signal captured (" << signal << ")." << std::endl;
  exit(EXIT_SUCCESS);
}

void
UnhandledExceptionHandler()
{
  std::cout << __func__ << " terminate." << std::endl;
  exit(EXIT_SUCCESS);
}

void
InvalidParameterHandler(
  const wchar_t* /*expression*/,
  const wchar_t* /*function*/,
  const wchar_t* /*file*/,
  unsigned int /*line*/,
  uintptr_t /*pReserved*/)
{
  std::cerr << __func__ << ": CRT caught.\n";
  exit(EXIT_SUCCESS);
}

namespace {

void
Execute(const dump::DangerousFunction& a_function)
{
  std::cout << "Going to execute dangerous function " << a_function.GetName() << std::endl;
  a_function();
}

DWORD WINAPI
DangerousFunctionThread(LPVOID lpParameter)
{
  const dump::DangerousFunction* const dangerousFunction = static_cast<dump::DangerousFunction*>(lpParameter);

  Execute(*dangerousFunction);

  return 0;
}

void
ExecuteWithinThread(const dump::DangerousFunction& a_function)
{
  std::cout << "Going to execute dangerous function " << a_function.GetName() << " within a thread" << std::endl;

  const HANDLE hThread = CreateThread(
    nullptr,
    0,
    DangerousFunctionThread,
    static_cast<LPVOID>(const_cast<dump::DangerousFunction*>(&a_function)),
    0,
    nullptr);

  if (hThread == nullptr) {
    std::cerr << __func__ << ": CreateThread failed -- returning\n";
    return;
  }
  // Do not access same objects than the thread (a_function, cout,...)
  WaitForSingleObject(hThread, INFINITE);

  CloseHandle(hThread);
}

void
Execute(bool a_runInThread, const dump::DangerousFunction& a_function)
{
  if (a_runInThread) {
    ExecuteWithinThread(a_function);
  } else {
    Execute(a_function);
  }
}

const dump::DangerousFunction&
AskForDangerousFunction(const dump::DangerousFunctions& exceptions)
{
  std::cout << "Select a dangerous function:" << std::endl;

  for (size_t i = 0; i < exceptions.d_functions.size(); ++i) {
    std::cout << "\t[" << i << "] -> " << exceptions.d_functions[i].GetName() << std::endl;
  }

  size_t i;
  do { // Loop to account for silly users
    std::string line;
    std::getline(std::cin, line);
    std::istringstream(line) >> i;
  } while (i >= exceptions.d_functions.size());

  return exceptions.d_functions[i];
}

bool
AskForRunInThread()
{
  std::cout << "Run on worker thread [y/n]? " << std::flush;

  char thread;
  std::cin >> thread;

  return thread == 'y';
}

void
InvalidArguments()
{
  std::cerr << "Invalid arguments\n";
}

class ArgsReader {
public:
  enum State { eAsk, eAllFunctions, eCatchableFunctions, eSpecificFunction, eInvalidArgs };

  ArgsReader(int argc, char* argv[]) : m_state(eAsk), m_dangerousFunctionID(0), m_runOnThread(false)
  {
    const std::string allFlag = "/all";
    const std::string allCatchableFlag = "/catchable";
    const std::string crashNumFlag = "/c";
    const std::string threadFlag = "/t";

    for (int i = 1; i < argc; ++i) {
      if (argv[i] == allFlag) {
        m_state = eAllFunctions;
      } else if (argv[i] == allCatchableFlag) {
        m_state = eCatchableFunctions;
      } else if (std::string(argv[i], crashNumFlag.size()) == crashNumFlag) {
        std::istringstream(std::string(argv[i] + crashNumFlag.size())) >> m_dangerousFunctionID;
        m_state = eSpecificFunction;
      } else if (argv[i] == threadFlag) {
        m_runOnThread = true;
      }
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
  // set_terminate(UnhandledExceptionHandler);
  SetUnhandledExceptionFilter(UnhandledHandler);
  signal(SIGSEGV, SignalHandler);
  // AddVectoredExceptionHandler(1, UnhandledHandler);
  _set_invalid_parameter_handler(InvalidParameterHandler);

  const ArgsReader args(argc, argv);
  const dump::DangerousFunctions functions;
  switch (args.m_state) {
  case ArgsReader::eAsk: {
    const bool runInThread = AskForRunInThread();
    for (;;) {
      Execute(runInThread, AskForDangerousFunction(functions));
      std::cout << "Succeeded" << std::endl;
    }
  } break;

  case ArgsReader::eAllFunctions: {
    for (const auto& function : functions.d_functions) {
      Execute(args.m_runOnThread, function);
    }
    std::cout << "Succeeded" << std::endl;
  } break;

  case ArgsReader::eCatchableFunctions: {
    for (const auto& function : functions.d_functions) {
      if (function.IsCatchable()) {
        Execute(args.m_runOnThread, function);
      }
    }
    std::cout << "Succeeded" << std::endl;
  } break;

  case ArgsReader::eSpecificFunction:
    if (args.m_dangerousFunctionID < functions.d_functions.size()) {
      Execute(args.m_runOnThread, functions.d_functions[args.m_dangerousFunctionID]);
      std::cout << "Succeeded" << std::endl;
    } else {
      InvalidArguments();
    }
    break;

  default: InvalidArguments(); break;
  }
  return EXIT_SUCCESS;
}
