#ifndef _TCUWIN32EGLNATIVEDISPLAYFACTORY_HPP
#define _TCUWIN32EGLNATIVEDISPLAYFACTORY_HPP
/*-------------------------------------------------------------------------
 * drawElements Quality Program Tester Core
 * ----------------------------------------
 *
 * Copyright 2014 The Android Open Source Project
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
 * \brief Win32 EGL native display factory
 *//*--------------------------------------------------------------------*/

#include "tcuDefs.hpp"
#include "egluNativeDisplay.hpp"
#include "eglwDefs.hpp"
#include "tcuWin32API.h"

namespace tcu
{
namespace win32
{

class EGLNativeDisplayFactory : public eglu::NativeDisplayFactory
{
public:
									EGLNativeDisplayFactory		(HINSTANCE instance);
	virtual							~EGLNativeDisplayFactory	(void);

	virtual eglu::NativeDisplay*	createDisplay				(const eglw::EGLAttrib* attribList) const;

private:
	const HINSTANCE					m_instance;
};

} // win32
} // tcu

#endif // _TCUWIN32EGLNATIVEDISPLAYFACTORY_HPP
