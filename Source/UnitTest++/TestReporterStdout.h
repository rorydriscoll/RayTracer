#ifndef UNITTEST_TESTREPORTERSTDOUT_H
#define UNITTEST_TESTREPORTERSTDOUT_H

#include "TestReporter.h"

namespace UnitTest {

class TestReporterStdout : public TestReporter
{
	virtual void ReportTestStart(TestDetails const& test) override;
    virtual void ReportFailure(TestDetails const& test, char const* failure) override;
    virtual void ReportTestFinish(TestDetails const& test, float secondsElapsed) override;
    virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed) override;
};

}

#endif 
