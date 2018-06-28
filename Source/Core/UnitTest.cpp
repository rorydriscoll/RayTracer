#include <Core/UnitTest.h>
#include <Core/Assert.h>
#include <Core/Log.h>
#include <UnitTest++/TestRunner.h>

int UnitTest::RunTests()
{
	Log::Initialize(Severity::Fatal);

	Assert::ThrowExceptions();

	const int failures = RunAllTests();

	Log::Shutdown();

	return failures;
}
