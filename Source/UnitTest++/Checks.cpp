#include "Checks.h"
#include <cstring>

namespace UnitTest {

namespace {

void CheckStringsEqual(TestResults& results, char const* expected, char const* actual, 
                       TestDetails const& details)
{
	using namespace std;

	if (!expected && actual)
	{
        UnitTest::MemoryOutStream stream;
        stream << "Expected null but was " << actual;

        results.OnTestFailure(details, stream.GetText());
		return;
	}

	if (expected && !actual)
	{
        UnitTest::MemoryOutStream stream;
        stream << "Expected '" << expected << "' but was null";

        results.OnTestFailure(details, stream.GetText());
		return;
	}

	if (!expected && !actual)
		return;

	if (strcmp(expected, actual))
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected '" << expected << "' but was '" << actual << "'";

        results.OnTestFailure(details, stream.GetText());
    }
}

}


void CheckEqual(TestResults& results, char const* expected, char const* actual,
                TestDetails const& details)
{
    CheckStringsEqual(results, expected, actual, details);
}

void CheckEqual(TestResults& results, char* expected, char* actual,
                TestDetails const& details)
{
    CheckStringsEqual(results, expected, actual, details);
}

void CheckEqual(TestResults& results, char* expected, char const* actual,
                TestDetails const& details)
{
    CheckStringsEqual(results, expected, actual, details);
}

void CheckEqual(TestResults& results, char const* expected, char* actual,
                TestDetails const& details)
{
    CheckStringsEqual(results, expected, actual, details);
}


}
