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
#include "qpTestLog.h"

// Implement this in your platform port.
tcu::Platform* createPlatform (void);

using namespace emscripten;

namespace {
	class TestLog : public tcu::TestLog
	{
	public:
		TestLog(const char* fileName, deUint32 flags)
			: tcu::TestLog(fileName, flags)
		{
		}

		void startCase(const char* testCasePath, qpTestCaseType testCaseType)
		{
			tcu::TestLog::startCase(testCasePath, testCaseType);
			lastTestCasePath = testCasePath;
		}

		void endCase(qpTestResult result, const char* description)
		{
			tcu::TestLog::endCase(result, description);
			lastTestResult = result;
			lastTestResultDesc = description;
		}

		std::string lastTestCasePath;
		qpTestResult lastTestResult = QP_TEST_RESULT_PASS;
		std::string lastTestResultDesc;
	};

	class dEQPJS
	{
	public:
		explicit dEQPJS(const std::string& args)
			: m_archive(".")
			, m_platform(createPlatform())
			, m_cmdLine("dummy_arg_0 " + args)
		{
			m_log = std::unique_ptr<TestLog>(new TestLog(m_cmdLine.getLogFileName(), m_cmdLine.getLogFlags()));
			m_app = std::unique_ptr<tcu::App>(new tcu::App(*m_platform, m_archive, *m_log, m_cmdLine));
		}

		bool iterate()
		{
			return m_app->iterate();
		}

		std::string getTestCasePath() {
			return m_log->lastTestCasePath;
		}
		qpTestResult getTestResult() {
			return m_log->lastTestResult;
		}
		std::string getTestResultName() {
			return std::string(qpGetTestResultName(m_log->lastTestResult));
		}
		std::string getTestResultDesc() {
			return m_log->lastTestResultDesc;
		}

	private:
		tcu::DirArchive m_archive;
		std::unique_ptr<tcu::Platform> m_platform;
		tcu::CommandLine m_cmdLine;
		std::unique_ptr<TestLog> m_log;
		std::unique_ptr<tcu::App> m_app;
	};
}

EMSCRIPTEN_BINDINGS(deqp) {
	enum_<qpTestResult>("qpTestResult")
		.value("PASS", QP_TEST_RESULT_PASS)
		.value("FAIL", QP_TEST_RESULT_FAIL)
		;
	class_<dEQPJS>("dEQPJS")
		.constructor<const std::string&>()
		.function("iterate", &dEQPJS::iterate)
		.function("getTestCasePath", &dEQPJS::getTestCasePath)
		.function("getTestResult", &dEQPJS::getTestResult)
		.function("getTestResultName", &dEQPJS::getTestResultName)
		.function("getTestResultDesc", &dEQPJS::getTestResultDesc)
		;
}
