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

#include "tcuEmscriptenPlatformWeb.hpp"
#include "deMemory.h"
#include "glwEnums.hpp"
#include "glwFunctions.hpp"
#include "glwInitES20Direct.hpp"
#include "glwInitES30Direct.hpp"
#include "glwInitES31Direct.hpp"
#include "glwInitES32Direct.hpp"
#include "tcuRenderTarget.hpp"

#include <emscripten.h>
#include <emscripten/html5.h>

tcu::Platform* createPlatform()
{
	return new tcu::emscripten_web::Platform();
}

namespace tcu
{
namespace emscripten_web
{

class WebGLRenderContext : public glu::RenderContext
{
	public:
		WebGLRenderContext(const glu::RenderConfig& config)
			: m_type(config.type)
		{
			TCU_CHECK(isContextTypeES(m_type));
			TCU_CHECK(config.redBits	<= 8); // DONT_CARE (-1) is also okay.
			TCU_CHECK(config.greenBits	<= 8); // DONT_CARE (-1) is also okay.
			TCU_CHECK(config.blueBits	<= 8); // DONT_CARE (-1) is also okay.
			const int width			= getValueOrDefault(config, &glu::RenderConfig::width,			400);
			const int height		= getValueOrDefault(config, &glu::RenderConfig::height,			300);
			TCU_CHECK(width > 0);
			TCU_CHECK(height > 0);
			const bool hasAlpha		= getValueOrDefault(config, &glu::RenderConfig::alphaBits,		0) > 0;
			const bool hasDepth		= getValueOrDefault(config, &glu::RenderConfig::depthBits,		0) > 0;
			const bool hasStencil	= getValueOrDefault(config, &glu::RenderConfig::stencilBits,	0) > 0;
			const bool antialias	= getValueOrDefault(config, &glu::RenderConfig::numSamples,		0) > 0;

			emscripten_set_canvas_element_size("#canvas", width, height);

			EmscriptenWebGLContextAttributes attr;
			emscripten_webgl_init_context_attributes(&attr);
			attr.alpha = hasAlpha;
			attr.depth = hasDepth;
			attr.stencil = hasStencil;
			attr.antialias = antialias;
			attr.premultipliedAlpha = true;
			attr.preserveDrawingBuffer = false;
			attr.enableExtensionsByDefault = true;
			attr.majorVersion = 0;
			attr.minorVersion = 0;
			switch (m_type.getMajorVersion() * 10 + m_type.getMinorVersion()) {
				case 32:
					glw::initES32Direct(&m_functions);
					attr.majorVersion = 2; // WebGL 2.0
					break;
				case 31:
					glw::initES31Direct(&m_functions);
					attr.majorVersion = 2; // WebGL 2.0
					break;
				case 30:
					glw::initES30Direct(&m_functions);
					attr.majorVersion = 2; // WebGL 2.0
					break;
				case 20:
					glw::initES20Direct(&m_functions);
					attr.majorVersion = 1; // WebGL 1.0
					break;
				default:
					TCU_CHECK(false);
					return;
			}

			m_context = emscripten_webgl_create_context(DE_NULL, &attr);
			emscripten_webgl_make_context_current(m_context);
			{
				int redBits = 0;
				int greenBits = 0;
				int blueBits = 0;
				int alphaBits = 0;
				int depthBits = 0;
				int stencilBits = 0;
				int numSamples = 0;
				m_functions.getIntegerv(GL_RED_BITS, &redBits);
				m_functions.getIntegerv(GL_GREEN_BITS, &greenBits);
				m_functions.getIntegerv(GL_BLUE_BITS, &blueBits);
				m_functions.getIntegerv(GL_ALPHA_BITS, &alphaBits);
				m_functions.getIntegerv(GL_DEPTH_BITS, &depthBits);
				m_functions.getIntegerv(GL_STENCIL_BITS, &stencilBits);
				m_functions.getIntegerv(GL_SAMPLES, &numSamples);

				m_renderTarget = tcu::RenderTarget(
						width, height, tcu::PixelFormat(redBits, greenBits, blueBits, alphaBits),
						depthBits, stencilBits, numSamples);
			}
		}

		~WebGLRenderContext()
		{
			emscripten_webgl_destroy_context(m_context);
			m_context = 0;
		}

		glu::ContextType getType() const override { return m_type; }
		const glw::Functions& getFunctions() const override { return m_functions; }
		const tcu::RenderTarget& getRenderTarget() const override { return m_renderTarget; }
		void postIterate() override {}

	private:
		const glu::ContextType m_type;
		EMSCRIPTEN_WEBGL_CONTEXT_HANDLE m_context = 0;
		glw::Functions m_functions;
		RenderTarget m_renderTarget = {};
};

class WebGLContextFactory : public glu::ContextFactory
{
public:
	WebGLContextFactory()
		: glu::ContextFactory("webgl", "WebGL Context")
	{
	}

	glu::RenderContext*	createContext (const glu::RenderConfig& config, const tcu::CommandLine&, const glu::RenderContext*) const override
	{
		return new WebGLRenderContext(config);
	}
};

Platform::Platform()
{
	m_contextFactoryRegistry.registerFactory(new WebGLContextFactory());
}

Platform::~Platform()
{
}

} // emscripten_web
} // tcu
