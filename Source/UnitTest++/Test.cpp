#include "Test.h"
#include "TestList.h"
#include "ExecuteTest.h"

#ifdef UNITTEST_POSIX
    #include "Posix/SignalTranslator.h"
#endif

namespace UnitTest {

TestList& Test::GetTestList()
{
    static TestList s_list;
    return s_list;
}

Test::Test(char const* testName, char const* suiteName, char const* filename, int lineNumber)
    : m_details(testName, suiteName, filename, lineNumber)
    , next(nullptr)
    , m_timeConstraintExempt(false)
{
}

Test::~Test()
{
}

void Test::Run()
{
	ExecuteTest(*this, m_details);
}

void Test::RunImpl() const
{
}

}
