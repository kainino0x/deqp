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
 * \brief dEQP as a JavaScript library, via WebAssembly.
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

#include <list>

// Implement this in your platform port.
tcu::Platform* createPlatform (void);

using namespace emscripten;

struct CaseResult
{
	std::string path;
	qpTestResult result = QP_TEST_RESULT_INTERNAL_ERROR;
	std::string description = "description not initialized";
};

class TestLog : public tcu::TestLog
{
public:
	TestLog(const char* fileName, deUint32 flags)
		: tcu::TestLog(fileName, flags)
	{
	}

	void startCase(const char* testCasePath, qpTestCaseType testCaseType) override
	{
		tcu::TestLog::startCase(testCasePath, testCaseType);
		currentResult = CaseResult {};
		currentResult.path = testCasePath;
	}

	void endCase(qpTestResult result, const char* description) override
	{
		tcu::TestLog::endCase(result, description);
		currentResult.result = result;
		currentResult.description = description;
		results.push_back(currentResult);
	}

	void terminateCase(qpTestResult result) override
	{
		tcu::TestLog::terminateCase(result);
		currentResult.result = result;
		currentResult.description = "";
		results.push_back(currentResult);
	}

public:
	bool peekNextExists()
	{
		return !results.empty();
	}

	std::string peekNextPath()
	{
		if (results.empty()) {
			return "";
		}
		return results.front().path;
	}

	qpTestResult peekNextResult()
	{
		if (results.empty()) {
			return QP_TEST_RESULT_INTERNAL_ERROR;
		}
		return results.front().result;
	}

	std::string peekNextResultName()
	{
		return std::string(qpGetTestResultName(peekNextResult()));
	}

	std::string peekNextDescription()
	{
		if (results.empty()) {
			return "no results left in the queue";
		}
		return results.front().description;
	}

	void pop()
	{
		if (results.empty()) {
			return;
		}
		results.pop_front();
	}

private:
	std::list<CaseResult> results;
	CaseResult currentResult;
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

	TestLog* getTestLog()
	{
		return m_log.get();
	}

private:
	tcu::DirArchive m_archive;
	std::unique_ptr<tcu::Platform> m_platform;
	tcu::CommandLine m_cmdLine;
	std::unique_ptr<TestLog> m_log;
	std::unique_ptr<tcu::App> m_app;
};

EMSCRIPTEN_BINDINGS(deqp) {
	enum_<qpTestResult>("qpTestResult")
		.value("PASS", QP_TEST_RESULT_PASS)
		.value("FAIL", QP_TEST_RESULT_FAIL)
		;
	class_<dEQPJS>("dEQPJS")
		.constructor<const std::string&>()
		.function("iterate", &dEQPJS::iterate)
		.function("getTestLog", &dEQPJS::getTestLog, allow_raw_pointers())
		;
	class_<TestLog>("TestLog")
		.function("peekNextExists", &TestLog::peekNextExists)
		.function("peekNextPath", &TestLog::peekNextPath)
		.function("peekNextResult", &TestLog::peekNextResult)
		.function("peekNextResultName", &TestLog::peekNextResultName)
		.function("peekNextDescription", &TestLog::peekNextDescription)
		.function("pop", &TestLog::pop)
		;
}
