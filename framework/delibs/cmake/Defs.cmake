#-------------------------------------------------------------------------
# drawElements CMake utilities
# ----------------------------
#
# Copyright 2014 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#-------------------------------------------------------------------------

# \note Always include this file in main project file, with NO_POLICY_SCOPE
#       AFTER project(name) statement.
#
# project(deproject)
# include(delibs/cmake/Defs.cmake NO_POLICY_SCOPE)

cmake_policy(VERSION 2.6)

# \todo [pyry] More intelligent detection, perhaps use some script?

# cmake files can use DE_DEFS variable to check that this file has been included
set(DE_DEFS 1)

macro (DE_MAKE_ENV_BOOL BASE VALUE)
	if (${BASE} STREQUAL ${BASE}_${VALUE})
		set(${BASE}_IS_${VALUE} 1)
	else ()
		set(${BASE}_IS_${VALUE} 0)
	endif ()
endmacro ()

# Add build type RelWithAsserts
set(CMAKE_CXX_FLAGS_RELWITHASSERTS ${CMAKE_CXX_FLAGS_RELEASE})
set(CMAKE_C_FLAGS_RELWITHASSERTS ${CMAKE_C_FLAGS_RELEASE})
set(CMAKE_EXE_LINKER_FLAGS_RELWITHASSERTS ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHASSERTS ${CMAKE_SHARED_LINKER_FLAGS_RELEASE})

# Os detection
if (NOT DEFINED DE_OS)
	if (WIN32)
		set(DE_OS "DE_OS_WIN32")
	elseif (APPLE)
		set(DE_OS "DE_OS_OSX")
	elseif (UNIX)
		set(DE_OS "DE_OS_UNIX")
	else ()
		set(DE_OS "DE_OS_VANILLA")
	endif ()
endif ()

# DE_OS_IS_{PLATFORM} definitions
DE_MAKE_ENV_BOOL("DE_OS" "VANILLA")
DE_MAKE_ENV_BOOL("DE_OS" "WIN32")
DE_MAKE_ENV_BOOL("DE_OS" "UNIX")
DE_MAKE_ENV_BOOL("DE_OS" "WINCE")
DE_MAKE_ENV_BOOL("DE_OS" "OSX")
DE_MAKE_ENV_BOOL("DE_OS" "ANDROID")
DE_MAKE_ENV_BOOL("DE_OS" "IOS")

# Prevent mixed compile with GCC and Clang
if (NOT ("${CMAKE_C_COMPILER_ID}" MATCHES "GNU") EQUAL ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU"))
	message(FATAL_ERROR "CMake C and CXX compilers do not match. Both or neither must be GNU.")
elseif (NOT ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang") EQUAL ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
	message(FATAL_ERROR "CMake C and CXX compilers do not match. Both or neither must be Clang.")
endif ()

if (EMSCRIPTEN)
	set(CMAKE_C_COMPILER_ID "Emscripten")
	set(CMAKE_CXX_COMPILER_ID "Emscripten")

	set(c_cxx_flags "-s WASM=1 -s USE_WEBGL2=1 -s FULL_ES3=1")
		#-s WASM=1
		#-s USE_WEBGL2=1
		#-s FULL_ES3=1
		set(c_cxx_flags_debug "-s ASSERTIONS=1 -s DISABLE_EXCEPTION_CATCHING=0 -s DEMANGLE_SUPPORT=1 -g3")
		#-s ASSERTIONS=1
		#-s DEMANGLE_SUPPORT=1
		#-s DISABLE_EXCEPTION_CATCHING=0
		#-g3
		#-g4  # source maps

	set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${c_cxx_flags}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${c_cxx_flags}")
	set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}   ${c_cxx_flags_debug}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${c_cxx_flags_debug}")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO   "${CMAKE_C_FLAGS_RELWITHDEBINFO}   ${c_cxx_flags_debug}")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${c_cxx_flags_debug}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --emrun")

	add_definitions(-D_XOPEN_SOURCE=600)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

# Compiler detection
if (NOT DEFINED DE_COMPILER)
	# \note " x" postfix is to work around bug in CMake that causes
	#       "MSVC" to translate to something completely different
	if (("${CMAKE_C_COMPILER_ID} x" MATCHES "MSVC") OR MSVC)
		set(DE_COMPILER "DE_COMPILER_MSC")
	elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "GNU")
		set(DE_COMPILER "DE_COMPILER_GCC")
	elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang" OR
		"${CMAKE_C_COMPILER_ID}" MATCHES "Emscripten")
		set(DE_COMPILER "DE_COMPILER_CLANG")

	# Guess based on OS
	elseif (DE_OS_IS_WIN32)
		set(DE_COMPILER "DE_COMPILER_MSC")
	elseif (DE_OS_IS_UNIX OR DE_OS_IS_ANDROID)
		set(DE_COMPILER "DE_COMPILER_GCC")
	elseif (DE_OS_IS_OSX OR DE_OS_IS_IOS)
		set(DE_COMPILER "DE_COMPILER_CLANG")

	else ()
		set(DE_COMPILER "DE_COMPILER_VANILLA")
	endif ()
endif ()

# DE_COMPILER_IS_{COMPILER} definitions
DE_MAKE_ENV_BOOL("DE_COMPILER" "VANILLA")
DE_MAKE_ENV_BOOL("DE_COMPILER" "MSC")
DE_MAKE_ENV_BOOL("DE_COMPILER" "GCC")
DE_MAKE_ENV_BOOL("DE_COMPILER" "CLANG")

# Pointer size detection
if (NOT DEFINED DE_PTR_SIZE)
	if (DEFINED CMAKE_SIZEOF_VOID_P)
		set(DE_PTR_SIZE ${CMAKE_SIZEOF_VOID_P})
	else ()
		set(DE_PTR_SIZE 4)
	endif ()
endif ()

# CPU detection
if (NOT DEFINED DE_CPU)
	if (DE_PTR_SIZE EQUAL 8)
		set(DE_CPU "DE_CPU_X86_64")
	else ()
		set(DE_CPU "DE_CPU_X86")
	endif ()
endif ()

# DE_CPU_IS_{CPU} definitions
DE_MAKE_ENV_BOOL("DE_CPU" "VANILLA")
DE_MAKE_ENV_BOOL("DE_CPU" "X86")
DE_MAKE_ENV_BOOL("DE_CPU" "ARM")
DE_MAKE_ENV_BOOL("DE_CPU" "ARM_64")

# \note [petri] Re-wrote in this ugly manner, because CMake 2.6 seems to
#               barf about the parenthesis in the previous way. Ugh.
#if (NOT ((DE_PTR_SIZE EQUAL 4) OR (DE_PTR_SIZE EQUAL 8)))
if (DE_PTR_SIZE EQUAL 4)
elseif (DE_PTR_SIZE EQUAL 8)
else ()
	message(FATAL_ERROR "DE_PTR_SIZE (${DE_PTR_SIZE}) is invalid")
endif ()

# Debug definitions
if (NOT DEFINED DE_DEBUG)
	if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithAsserts")
		set(DE_DEBUG 1)
	else ()
		set(DE_DEBUG 0)
	endif ()
endif ()

# Android API version
if (DE_OS_IS_ANDROID AND NOT DEFINED DE_ANDROID_API)
	set(DE_ANDROID_API 5)
endif ()

message(STATUS "DE_OS          = ${DE_OS}")
message(STATUS "DE_COMPILER    = ${DE_COMPILER}")
message(STATUS "DE_CPU         = ${DE_CPU}")
message(STATUS "DE_PTR_SIZE    = ${DE_PTR_SIZE}")
message(STATUS "DE_DEBUG       = ${DE_DEBUG}")
if (DE_OS_IS_ANDROID)
	message(STATUS "DE_ANDROID_API = ${DE_ANDROID_API}")
endif ()

# Expose definitions
if (DE_DEBUG)
	add_definitions(-DDE_DEBUG)
endif ()

add_definitions("-DDE_OS=${DE_OS}")
add_definitions("-DDE_COMPILER=${DE_COMPILER}")
add_definitions("-DDE_CPU=${DE_CPU}")
add_definitions("-DDE_PTR_SIZE=${DE_PTR_SIZE}")

if (DE_OS_IS_ANDROID)
	add_definitions("-DDE_ANDROID_API=${DE_ANDROID_API}")
endif ()
