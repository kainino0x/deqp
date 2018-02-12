#ifndef _TCUEMSCRIPTENPLATFORMWEB_HPP
#define _TCUEMSCRIPTENPLATFORMWEB_HPP
/*-------------------------------------------------------------------------
 * drawElements Quality Program Tester Core
 * ----------------------------------------
 *
 * Copyright 2018 The Android Open Source Project
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
 * \brief Emscripten platform implemented on html5.h without EGL.
 *//*--------------------------------------------------------------------*/

#include "tcuDefs.hpp"
#include "tcuPlatform.hpp"
#include "gluPlatform.hpp"

#ifdef DEQP_SUPPORT_EGL
#	error "Emscripten EGL platform should not build if DEQP_SUPPORT_EGL is defined."
#endif

namespace tcu
{
namespace emscripten_web
{

class Platform : public tcu::Platform, private glu::Platform
{
public:
	Platform();
	virtual ~Platform();

	virtual const glu::Platform& getGLPlatform() const { return static_cast<const glu::Platform&>(*this); }
};

} // emscripten_web
} // tcu

#endif // _TCUEMSCRIPTENPLATFORMWEB_HPP
