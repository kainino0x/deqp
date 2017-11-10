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
 * \brief Generic main().
 *//*--------------------------------------------------------------------*/

#include "tcuDefs.hpp"
#include "tcuCommandLine.hpp"
#include "tcuPlatform.hpp"
#include "tcuApp.hpp"
#include "tcuResource.hpp"
#include "tcuTestLog.hpp"
#include "deUniquePtr.hpp"

#include <cstdio>

#ifdef __EMSCRIPTEN__
#	include <emscripten.h>

void requestAnimationFrameCallback(void* arg) {
	tcu::App* app = reinterpret_cast<tcu::App*>(arg);
	try
	{
		if (!app->iterate())
		{
			// Iteration is done. Return success.
			emscripten_force_exit(0);
		}
	}
	catch (const std::exception& e)
	{
		tcu::die("%s", e.what());
	}
}
#endif

// Implement this in your platform port.
tcu::Platform* createPlatform (void);

int main (int argc, char** argv)
{
#if (DE_OS != DE_OS_WIN32)
	// Set stdout to line-buffered mode (will be fully buffered by default if stdout is pipe).
	setvbuf(stdout, DE_NULL, _IOLBF, 4*1024);
#endif

	try
	{
		tcu::CommandLine				cmdLine		(argc, argv);
		tcu::DirArchive					archive		(".");
		tcu::TestLog					log			(cmdLine.getLogFileName(), cmdLine.getLogFlags());
		de::UniquePtr<tcu::Platform>	platform	(createPlatform());
		de::UniquePtr<tcu::App>			app			(new tcu::App(*platform, archive, log, cmdLine));

#ifdef __EMSCRIPTEN__
		// Set the main loop (requestAnimationFrame) callback.
		emscripten_set_main_loop_arg(requestAnimationFrameCallback, app.get(), 0, false);
		// Exit this synchronous main function to let the callbacks run.
		emscripten_exit_with_live_runtime();
#else
		// Main loop.
		for (;;)
		{
			if (!app->iterate())
				break;
		}
#endif
	}
	catch (const std::exception& e)
	{
		tcu::die("%s", e.what());
	}

	return 0;
}
