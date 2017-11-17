/*-------------------------------------------------------------------------
 * drawElements Quality Program Execution Server
 * ---------------------------------------------
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
 * \brief dEQP as a JS library.
 *//*--------------------------------------------------------------------*/

#include <emscripten/bind.h>
#include "tcuPlatform.hpp"
#include "tcuCommandLine.hpp"
#include "tcuTestLog.hpp"
#include "tcuTestContext.hpp"
#include "tcuTestPackage.hpp"
#include "tcuTestSessionExecutor.hpp"
#include "tcuResource.hpp"
#include "tcuApp.hpp"
//
// Implement this in your platform port.
tcu::Platform* createPlatform (void);

using namespace emscripten;

namespace {
	class dEQPJS {
	public:
		explicit dEQPJS(const std::string& args)
			: m_archive(".")
			, m_platform(createPlatform())
			, m_cmdLine("dummy_arg_0 " + args)
			, m_log("/dev/stdout") {
			m_app = std::unique_ptr<tcu::App>(new tcu::App(*m_platform, m_archive, m_log, m_cmdLine));
		}

		bool iterate() {
			return m_app->iterate();
		}

	private:
		tcu::DirArchive m_archive;
		std::unique_ptr<tcu::Platform> m_platform;
		tcu::CommandLine m_cmdLine;
		tcu::TestLog m_log;
		std::unique_ptr<tcu::App> m_app;
	};
}

EMSCRIPTEN_BINDINGS(deqp) {
	class_<dEQPJS>("dEQPJS")
		.constructor<const std::string&>()
		.function("iterate", &dEQPJS::iterate)
		;
}
