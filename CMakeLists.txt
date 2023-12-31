cmake_minimum_required(VERSION 3.10)
enable_testing()
enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)	# Currently ranges feature requires c++latest instead of c++20.
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(POSIX	$<PLATFORM_ID:Linux>)
set(WIN32	$<PLATFORM_ID:Windows>)
set(ANDK	$<BOOL:${CMAKE_ANDROID_API}>)

project(xcp)

if(CMAKE_CXX_COMPILER_ID EQUAL MSVC)
	message("MSVC cl")
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /Zc:__cplusplus")
	if(CMAKE_CXX_FLAGS MATCHES "std:c[+][+]20")
		string(REGEX REPLACE "std:c[+][+]+20" "std:c++latest" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++latest")
	endif()
elseif(CMAKE_CXX_COMPILER_ID EQUAL GNU)
	message("GNU g++")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
	message("LLVM clang++")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -stdlib=libc++")
endif()

find_package(GTest)
find_package(Threads)
add_executable(xcp
	main.cpp
	utility.hpp
	lexer.hpp
)
target_include_directories(xcp	PRIVATE .)
if(CMAKE_CXX_COMPILER_ID EQUAL MSVC)
	target_compile_definitions(xcp	PUBLIC	_CRT_SECURE_NO_WARNINGS)
endif()
if(POSIX)
	target_compile_definitions(xcp	PUBLIC	xxx_posix)
elseif(WIN32)
	target_compile_definitions(xcp	PUBLIC	xxx_win32)
elseif(ANDK)
	target_compile_definitions(xcp	PUBLIC	xxx_andk)
endif()
target_link_libraries(xcp	${CMAKE_THREAD_LIBS_INIT})

if(GTest_FOUND)
	add_executable(xcp_ut
		ut_lexer.cpp
		utility.hpp
		lexer.hpp
	)
	target_link_libraries(xcp_ut	GTest::gtest_main GTest::gtest)
	add_test(xcp_ut
		COMMAND ./xcp_ut
	)
endif()
