/*-------------------------------------------------------------------------
 * drawElements Quality Program Tester Core
 * ----------------------------------------
 *
 * Copyright 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *//*!
 * \file
 * \brief Emscripten platform.
 *//*--------------------------------------------------------------------*/

#include "tcuEmscriptenPlatform.hpp"
#include "egluNativeDisplay.hpp"
#include "egluNativeWindow.hpp"
#include "egluGLContextFactory.hpp"
#include "eglwLibrary.hpp"
#include "eglwEnums.hpp"
#include "deMemory.h"

#include "emscripten.h"

#ifdef DEQP_EGL

tcu::Platform* createPlatform (void)
{
	return new tcu::emscripten::Platform();
}

namespace tcu
{
namespace emscripten
{

using namespace eglw;

enum
{
	DEFAULT_WINDOW_WIDTH	= 400,
	DEFAULT_WINDOW_HEIGHT	= 300
};

static const eglu::NativeDisplay::Capability	DISPLAY_CAPABILITIES	= eglu::NativeDisplay::CAPABILITY_GET_DISPLAY_LEGACY;
static const eglu::NativeWindow::Capability		WINDOW_CAPABILITIES		= eglu::NativeWindow::CAPABILITY_CREATE_SURFACE_LEGACY;

class Display : public eglu::NativeDisplay
{
public:
								Display				(void) : eglu::NativeDisplay(DISPLAY_CAPABILITIES) , m_library(DE_NULL) {}
								~Display			(void) {}

	EGLNativeDisplayType		getLegacyNative		(void) { return EGL_DEFAULT_DISPLAY; }
	const Library&				getLibrary			(void) const { return m_library; }


private:
	DefaultLibrary	m_library;
};

class DisplayFactory : public eglu::NativeDisplayFactory
{
public:
								DisplayFactory		(void);
								~DisplayFactory		(void) {}

	eglu::NativeDisplay*		createDisplay		(const EGLAttrib* attribList) const;
};

class Window : public eglu::NativeWindow
{
public:
								Window				(int width, int height);
								~Window				(void);

	EGLNativeWindowType			getLegacyNative		(void) { return &m_nativeWindow; }

	IVec2						getSize				(void) const;

private:
	int	m_nativeWindow;
	int	width;
	int	height;
};

class WindowFactory : public eglu::NativeWindowFactory
{
public:
								WindowFactory		(void) : eglu::NativeWindowFactory("emscripten", "Emscripten Canvas", WINDOW_CAPABILITIES) {}
								~WindowFactory		(void) {}

	eglu::NativeWindow*			createWindow		(eglu::NativeDisplay* display, const eglu::WindowParams& params) const;
};

// DisplayFactory

DisplayFactory::DisplayFactory (void)
	: eglu::NativeDisplayFactory("default", "EGL_DEFAULT_DISPLAY", DISPLAY_CAPABILITIES)
{
	m_nativeWindowRegistry.registerFactory(new WindowFactory());
}

eglu::NativeDisplay* DisplayFactory::createDisplay (const EGLAttrib*) const
{
	return new Display();
}

// WindowFactory

eglu::NativeWindow* WindowFactory::createWindow (eglu::NativeDisplay*, const eglu::WindowParams& params) const
{
	const int	width	= params.width	!= eglu::WindowParams::SIZE_DONT_CARE ? params.width	: DEFAULT_WINDOW_WIDTH;
	const int	height	= params.height	!= eglu::WindowParams::SIZE_DONT_CARE ? params.height	: DEFAULT_WINDOW_HEIGHT;

	return new Window(width, height);
}

// Window

Window::Window (int width, int height)
	: eglu::NativeWindow(WINDOW_CAPABILITIES)
	, width(width)
	, height(height)
{
	emscripten_set_canvas_size(width, height);
	m_nativeWindow = 0;
}

Window::~Window (void)
{
}

IVec2 Window::getSize (void) const
{
	return IVec2(width, height);
}

// Platform

Platform::Platform (void)
{
	m_nativeDisplayFactoryRegistry.registerFactory(new DisplayFactory());
	m_contextFactoryRegistry.registerFactory(new eglu::GLContextFactory(m_nativeDisplayFactoryRegistry));
}

Platform::~Platform (void)
{
}

} // emscripten
} // tcu

#endif
