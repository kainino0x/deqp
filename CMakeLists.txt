# dEQP cmake file

cmake_minimum_required(VERSION 2.6)

# dEQP Target.
set(DEQP_TARGET "default" CACHE STRING "dEQP Target (default, android...)")

project(dEQP-Core-${DEQP_TARGET})

if (EMSCRIPTEN)
	set(NOT_EMSCRIPTEN OFF)
else ()
	set(NOT_EMSCRIPTEN ON)
endif ()

option(DEQP_ENABLE_GLES2	"Build OpenGL ES 2.0 tests" ON)
option(DEQP_ENABLE_GLES3	"Build OpenGL ES 3.0 tests" ON)
option(DEQP_ENABLE_GLES31	"Build OpenGL ES 3.1 tests" ON)
option(DEQP_ENABLE_VULKAN	"Build Vulkan tests" ${NOT_EMSCRIPTEN})
option(DEQP_TEST_EGL		"Build EGL tests" ${NOT_EMSCRIPTEN})
# TODO(kainino@chromium.org): Remove EGL dependency from Emscripten build.
# Then, this can be changed to ${NOT_EMSCRIPTEN}.
option(DEQP_USE_EGL			"Build with EGL support" ON)

include(framework/delibs/cmake/Defs.cmake NO_POLICY_SCOPE)
include(framework/delibs/cmake/CFlags.cmake)

add_definitions(-DDE_ASSERT_FAILURE_CALLBACK)

# dEQP-specific configuration. Target file should override these.
set(DEQP_TARGET_NAME		"UNKNOWN")		# Target name


set(DEQP_SUPPORT_GLES2		${DEQP_ENABLE_GLES2})
set(DEQP_GLES2_LIBRARIES	)				# GLESv2 libraries. If empty, run-time linking is used
set(DEQP_SUPPORT_GLES3		${DEQP_ENABLE_GLES3})
set(DEQP_GLES3_LIBRARIES	)				# GLESv3 libraries. If empty, run-time linking is used
set(DEQP_SUPPORT_GLES31		${DEQP_ENABLE_GLES31})
set(DEQP_SUPPORT_VULKAN		${DEQP_ENABLE_VULKAN})
set(DEQP_EGL_LIBRARIES		)				# EGL libraries

# Legacy APIs that don't use run-time loading
set(DEQP_SUPPORT_GLES1		OFF)			# Is GLESv1 supported
set(DEQP_GLES1_LIBRARIES	)				# GLESv1 libraries
set(DEQP_SUPPORT_VG			OFF)			# Is OpenVG supported
set(DEQP_VG_LIBRARIES		)				# VG libraries

set(DEQP_SUPPORT_GLX		OFF)			# Is GLX supported \todo [2016-10-12 pyry] X11 specific - move to framework/platform/

set(DEQP_PLATFORM_LIBRARIES	)				# Other platform libraries

set(DEQP_PLATFORM_COPY_LIBRARIES	)		# Libraries / binaries that need to be copied to binary directory

# Delibs include directories
include_directories(
	framework/delibs/debase
	framework/delibs/decpp
	framework/delibs/depool
	framework/delibs/dethread
	framework/delibs/deutil
	framework/delibs/destream
	)

# Include target-specific definitions
include(targets/${DEQP_TARGET}/${DEQP_TARGET}.cmake)

# zlib
find_path(ZLIB_INCLUDE_PATH	zlib.h)
find_library(ZLIB_LIBRARY	z)

if (NOT ZLIB_INCLUDE_PATH OR NOT ZLIB_LIBRARY)
	message(STATUS "System version of zlib not found, using external/zlib")
	add_subdirectory(external/zlib)
	# \note ZLIB_LIBRARY and ZLIB_INCLUDE_PATH are promoted from external/zlib/CMakeLists.txt
endif ()

include_directories(${ZLIB_INCLUDE_PATH})

# libpng
#
# The FindPNG module defines PNG_INCLUDE_DIRS and PNG_LIBRARIES. But dEQP's
# CMake files expect the non-standard PNG_INCLUDE_PATH and PNG_LIBRARY. Set the
# non-standard variables here to retain compatibility with dEQP's existing
# CMake files.
include(FindPNG)
set(PNG_INCLUDE_PATH ${PNG_INCLUDE_DIRS})
set(PNG_LIBRARY ${PNG_LIBRARIES})

if (NOT PNG_INCLUDE_PATH OR NOT PNG_LIBRARY)
	message(STATUS "System version of libpng not found, using external/libpng")
	add_subdirectory(external/libpng)
	# \note PNG_LIBRARY and PNG_INCLUDE_PATH are promoted from external/libpng/CMakeLists.txt
endif ()

# glslang
if (DEQP_SUPPORT_VULKAN)
	add_subdirectory(external/glslang)
endif ()

# spirv-tools
if (DEQP_SUPPORT_VULKAN)
	add_subdirectory(external/spirv-tools)
endif ()

include_directories(${PNG_INCLUDE_PATH})

message(STATUS "DEQP_TARGET_NAME        = ${DEQP_TARGET_NAME}")
message(STATUS "DEQP_SUPPORT_GLES1      = ${DEQP_SUPPORT_GLES1}")
message(STATUS "DEQP_SUPPORT_GLES2      = ${DEQP_SUPPORT_GLES2}")
message(STATUS "DEQP_SUPPORT_GLES3      = ${DEQP_SUPPORT_GLES3}")
message(STATUS "DEQP_SUPPORT_GLES31     = ${DEQP_SUPPORT_GLES31}")
message(STATUS "DEQP_SUPPORT_VULKAN     = ${DEQP_SUPPORT_VULKAN}")
message(STATUS "DEQP_GLES1_LIBRARIES    = ${DEQP_GLES1_LIBRARIES}")
message(STATUS "DEQP_GLES2_LIBRARIES    = ${DEQP_GLES2_LIBRARIES}")
message(STATUS "DEQP_GLES3_LIBRARIES    = ${DEQP_GLES3_LIBRARIES}")
message(STATUS "DEQP_SUPPORT_VG         = ${DEQP_SUPPORT_VG}")
message(STATUS "DEQP_VG_LIBRARIES       = ${DEQP_VG_LIBRARIES}")
message(STATUS "DEQP_USE_EGL            = ${DEQP_USE_EGL}")
message(STATUS "DEQP_TEST_EGL           = ${DEQP_TEST_EGL}")
message(STATUS "DEQP_EGL_LIBRARIES      = ${DEQP_EGL_LIBRARIES}")
message(STATUS "DEQP_PLATFORM_LIBRARIES = ${DEQP_PLATFORM_LIBRARIES}")

# Defines
add_definitions(-DDEQP_TARGET_NAME="${DEQP_TARGET_NAME}")

if (DEQP_SUPPORT_GLES1)
	add_definitions(-DDEQP_SUPPORT_GLES1=1)
endif ()

if (DEQP_SUPPORT_VG)
	add_definitions(-DDEQP_SUPPORT_VG=1)
endif ()

# Entry points loaded on run-time?
if (NOT EMSCRIPTEN)
	if (NOT DEFINED DEQP_GLES2_LIBRARIES)
		add_definitions(-DDEQP_GLES2_RUNTIME_LOAD=1)
	endif ()

	if (NOT DEFINED DEQP_GLES3_LIBRARIES)
		add_definitions(-DDEQP_GLES3_RUNTIME_LOAD=1)
	endif ()

	if (NOT DEFINED DEQP_EGL_LIBRARIES)
		add_definitions(-DDEQP_EGL_RUNTIME_LOAD=1)
	endif ()

	# OpenGL entry points are always loaded on run-time
	add_definitions(-DDEQP_OPENGL_RUNTIME_LOAD=1)
endif ()

# Legacy APIs that don't support run-time loading
if (DEQP_SUPPORT_GLES1 AND NOT DEFINED DEQP_GLES1_LIBRARIES)
	message(FATAL_ERROR "Run-time loading of GLES1 is not supported (DEQP_GLES1_LIBRARIES is not set)")
endif ()

if (DEQP_SUPPORT_GLES2)
	add_definitions(-DDEQP_SUPPORT_GLES2=1)
endif ()

if (DEQP_SUPPORT_GLES3)
	add_definitions(-DDEQP_SUPPORT_GLES3=1)
endif ()

if (DEQP_SUPPORT_GLES31)
	add_definitions(-DDEQP_SUPPORT_GLES31=1)
endif ()

if (DEQP_SUPPORT_VULKAN)
	add_definitions(-DDEQP_SUPPORT_VULKAN=1)
endif ()

if (DEQP_SUPPORT_VG AND NOT DEFINED DEQP_VG_LIBRARIES)
	message(FATAL_ERROR "Run-time loading of VG is not supported (DEQP_VG_LIBRARIES is not set)")
endif ()

if (DEQP_USE_EGL)
	add_definitions(-DDEQP_USE_EGL=1)
endif ()

if (DEQP_TEST_EGL)
	add_definitions(-DDEQP_TEST_EGL=1)
endif ()

if (DE_COMPILER_IS_MSC)
	# Don't nag about std::copy for example
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS")
endif ()

# delibs projects
add_subdirectory(framework/delibs/debase)
add_subdirectory(framework/delibs/depool)
add_subdirectory(framework/delibs/dethread)
add_subdirectory(framework/delibs/destream)
add_subdirectory(framework/delibs/deutil)
add_subdirectory(framework/delibs/decpp)

# ExecServer
if (NOT EMSCRIPTEN)
	add_subdirectory(execserver)
endif ()

# Executor framework and tools
if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/executor)
	if (NOT EMSCRIPTEN)
		add_subdirectory(executor)
	endif ()
endif ()

# Test framework include directories
include_directories(
	framework/common
	framework/qphelper
	framework/opengl
	framework/opengl/wrapper
	framework/referencerenderer
	framework/opengl/simplereference
	framework/randomshaders
	framework/egl
	framework/egl/wrapper
	external/vulkancts/framework/vulkan
	)

if (DE_OS_IS_ANDROID OR DE_OS_IS_IOS)
	# On Android deqp modules are compiled as libraries and linked into final .so
	set(DEQP_MODULE_LIBRARIES )
	set(DEQP_MODULE_ENTRY_POINTS )
endif ()

if (DE_OS_IS_WIN32)
	include_directories(framework/platform/win32)
endif ()

# Macro for adding targets for copying binaries (usually target libraries) to the target destination dir
macro (target_copy_files target dep_name files)
	if (NOT "${files}" STREQUAL "")
		set(COPY_TARGETS )
		foreach (SRCNAME ${files})
			get_filename_component(BASENAME ${SRCNAME} NAME)
			set(DSTNAME "${CMAKE_CURRENT_BINARY_DIR}/${BASENAME}")
			add_custom_command(OUTPUT ${DSTNAME}
							   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SRCNAME} ${DSTNAME})
			set(COPY_TARGETS ${COPY_TARGETS} ${DSTNAME})
		endforeach ()

		add_custom_target(${dep_name} ALL DEPENDS ${COPY_TARGETS})
		add_dependencies(${target} ${dep_name})
	endif ()
endmacro (target_copy_files)

# Adds a new LINK_FLAGS directive to the given target, while keeping the previous linker flags.
macro (append_linker_flag MODULE_NAME FLAG)
	get_property(link_flags TARGET ${MODULE_NAME} PROPERTY LINK_FLAGS)
	set(link_flags "${link_flags} ${FLAG}")
	set_target_properties(${MODULE_NAME} PROPERTIES LINK_FLAGS "${link_flags}")
endmacro (append_linker_flag)

# Macro for adding dEQP module
macro (add_deqp_module MODULE_NAME SRCS LIBS ENTRY)
	if (DE_OS_IS_ANDROID OR DE_OS_IS_IOS OR EMSCRIPTEN)
		# Single-binary targets
		add_library(${MODULE_NAME} STATIC ${SRCS})
		target_link_libraries(${MODULE_NAME} ${LIBS})

		set(DEQP_MODULE_LIBRARIES		${DEQP_MODULE_LIBRARIES} ${MODULE_NAME})
		set(DEQP_MODULE_ENTRY_POINTS	${DEQP_MODULE_ENTRY_POINTS} "${CMAKE_CURRENT_SOURCE_DIR}/${ENTRY}")

		# Forward to parent scope
		set(DEQP_MODULE_LIBRARIES		${DEQP_MODULE_LIBRARIES} PARENT_SCOPE)
		set(DEQP_MODULE_ENTRY_POINTS	${DEQP_MODULE_ENTRY_POINTS} PARENT_SCOPE)

	else ()
		# Separate binary per target
		add_executable(${MODULE_NAME} ${CMAKE_SOURCE_DIR}/framework/platform/tcuMain.cpp ${ENTRY} ${SRCS})
		target_link_libraries(${MODULE_NAME} tcutil-platform ${LIBS})
		target_copy_files(${MODULE_NAME} platform-libs-${MODULE_NAME} "${DEQP_PLATFORM_COPY_LIBRARIES}")
	endif ()

	# Data file target
	add_custom_target(${MODULE_NAME}-data)
	add_dependencies(${MODULE_NAME} ${MODULE_NAME}-data)
endmacro (add_deqp_module)

# Macro for adding data dirs to module
macro (add_data_dir MODULE_NAME SRC_DIR DST_DIR)
	if (DE_OS_IS_WIN32 OR DE_OS_IS_UNIX OR DE_OS_IS_OSX)
		add_custom_command(TARGET ${MODULE_NAME}-data POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${DST_DIR})

		if (EMSCRIPTEN)
			append_linker_flag(${MODULE_NAME} "--preload-file ${CMAKE_CURRENT_BINARY_DIR}/${DST_DIR}@./${DST_DIR}")
		endif ()

	elseif (DE_OS_IS_ANDROID)
		add_custom_command(TARGET ${MODULE_NAME}-data POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR} ${CMAKE_BINARY_DIR}/assets/${DST_DIR})

	elseif (DE_OS_IS_IOS)
		add_custom_command(TARGET ${MODULE_NAME}-data POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR} ${CMAKE_BINARY_DIR}/\${CONFIGURATION}\${EFFECTIVE_PLATFORM_NAME}/deqp.app/${DST_DIR})
	endif ()
endmacro (add_data_dir)

# Macro for adding individual data files to module
macro (add_data_file MODULE_NAME SRC_FILE DST_FILE)
	if (DE_OS_IS_WIN32 OR DE_OS_IS_UNIX OR DE_OS_IS_OSX)
		add_custom_command(TARGET ${MODULE_NAME}-data POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${DST_FILE})

	elseif (DE_OS_IS_ANDROID)
		add_custom_command(TARGET ${MODULE_NAME}-data POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_FILE} ${CMAKE_BINARY_DIR}/assets/${DST_FILE})

	elseif (DE_OS_IS_IOS)
		add_custom_command(TARGET ${MODULE_NAME}-data POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_FILE} ${CMAKE_BINARY_DIR}/\${CONFIGURATION}\${EFFECTIVE_PLATFORM_NAME}/deqp.app/${DST_FILE})
	endif ()
endmacro (add_data_file)

add_subdirectory(framework)

if (DEQP_SUPPORT_VULKAN)
	add_subdirectory(external/vulkancts/framework/vulkan)
endif ()

if (DE_COMPILER_IS_MSC)
	add_compile_options(/bigobj) # Required by glsBuiltinPrecisionTests.cpp
endif ()

add_subdirectory(modules)

if (DEQP_SUPPORT_VULKAN)
	add_subdirectory(external/vulkancts/modules/vulkan)
endif ()

# Single-binary targets
if (DE_OS_IS_ANDROID)
	include_directories(executor)

	add_library(deqp SHARED framework/platform/android/tcuAndroidMain.cpp framework/platform/android/tcuAndroidJNI.cpp framework/platform/android/tcuAndroidPlatformCapabilityQueryJNI.cpp framework/platform/android/tcuTestLogParserJNI.cpp ${DEQP_MODULE_ENTRY_POINTS})
	target_link_libraries(deqp tcutil-platform xecore ${DEQP_MODULE_LIBRARIES})

elseif (DE_OS_IS_IOS)
	# Code sign identity
	set(DEQP_IOS_CODE_SIGN_IDENTITY "drawElements" CACHE STRING "Code sign identity for iOS build")

	set(MACOSX_BUNDLE_PRODUCT_NAME "\${PRODUCT_NAME}")
	set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.drawelements.\${PRODUCT_NAME:identifier}")

	include_directories(framework/platform/ios)
	set(TESTERCORE_SRC_FILES
		framework/platform/ios/tcuEAGLView.h
		framework/platform/ios/tcuEAGLView.m
		framework/platform/ios/tcuIOSAppDelegate.h
		framework/platform/ios/tcuIOSAppDelegate.m
		framework/platform/ios/tcuIOSViewController.h
		framework/platform/ios/tcuIOSViewController.m
		framework/platform/ios/tcuIOSMain.m
		)
	set_source_files_properties(${TESTERCORE_SRC_FILES} COMPILE_FLAGS "-std=c99")

	add_executable(deqp MACOSX_BUNDLE ${TESTERCORE_SRC_FILES} ${DEQP_MODULE_ENTRY_POINTS})
	target_link_libraries(deqp tcutil-platform xscore ${DEQP_MODULE_LIBRARIES})
	set_target_properties(deqp PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
	set_target_properties(deqp PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer: ${DEQP_IOS_CODE_SIGN_IDENTITY}")
endif ()

if (EMSCRIPTEN)
	add_subdirectory(emscripten)
endif ()
