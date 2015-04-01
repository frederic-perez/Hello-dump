# -- CMake build rules

cmake_minimum_required(VERSION 2.8)

project(Hello-dump)

# Boost stuff
if(WIN32)
	set(Boost_USE_STATIC_LIBS ON)
else()
	set(Boost_USE_STATIC_LIBS OFF)
endif()
set(Boost_USE_MULTITHREADED ON)
# find_package(Boost) # <-- Use this simple line for using only headers
find_package(Boost COMPONENTS system filesystem)
if (Boost_FOUND)
	message(STATUS "Boost found:")
	include_directories(${Boost_INCLUDE_DIRS})
	message(STATUS "  Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
	message(STATUS "  Boost_LIBRARIES: ${Boost_LIBRARIES}
	Note: Boost_LIBRARIES can be empty if no COMPONENTS are used.") # Used below
else ()
	message(FATAL_ERROR "Boost not found - Hint(?): ENV{BOOST_ROOT} is $ENV{BOOST_ROOT}")
endif ()

set(SRC_LIST
	aux-raw.h
	exception-generator.cpp exception-generator.h
	main.cpp
	minidumper.cpp minidumper.h
	StructuredException.cpp StructuredException.h
)

add_executable(${PROJECT_NAME} ${SRC_LIST})

message(STATUS "CMAKE_C_PLATFORM_ID is ${CMAKE_C_PLATFORM_ID}")
message(STATUS "CMAKE_SYSTEM_NAME is ${CMAKE_SYSTEM_NAME}")

if("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
	if(MSVC)
		message(STATUS "Entering Windows/MSVC branch.")
		string(CONCAT CMAKE_CXX_FLAGS
			"${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS "
			"/W4 /EHsc /wd4351") # Add /W4 /WX /EHsc
		# '- http://msdn.microsoft.com/en-US/library/thxezb7y.aspx:
		#		  /wd n  Disables the compiler warning that is specified in n.
		#			For example, /wd4326 disables compiler warning C4351.
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /FORCE:MULTIPLE")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
		message(STATUS "CMAKE_EXE_LINKER_FLAGS are: " ${CMAKE_EXE_LINKER_FLAGS})
	else()
		message(ERROR "Uncontemplated case.")
	endif()
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	if(CMAKE_COMPILER_IS_GNUCXX)
		message(STATUS "Entering Linux/CMAKE_COMPILER_IS_GNUCXX branch.")
		message(ERROR "Uncontemplated case.")
	else()
		message(STATUS "Entering Linux/else branch. We assume clang++.")
		message(ERROR "Uncontemplated case.")
	endif()
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
	message(ERROR "Uncontemplated case.")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
else()
	message(FATAL_ERROR "Unknown system")
endif()

message(STATUS "CMAKE_CXX_FLAGS are:" ${CMAKE_CXX_FLAGS})

# Link the executable to the required libraries

target_link_libraries(${PROJECT_NAME}
	${Boost_LIBRARIES}
#	${Boost_LIBRARIES} ${OpenIGTLink_LIBRARIES}
)

# -- eof
