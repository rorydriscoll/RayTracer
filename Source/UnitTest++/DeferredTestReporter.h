#ifndef UNITTEST_DEFERREDTESTREPORTER_H
#define UNITTEST_DEFERREDTESTREPORTER_H

#include "TestReporter.h"
#include "DeferredTestResult.h"

#include <vector>

namespace UnitTest
{

class DeferredTestReporter : public TestReporter
{
public:
    virtual void ReportTestStart(TestDetails const& details) override;
    virtual void ReportFailure(TestDetails const& details, char const* failure) override;
    virtual void ReportTestFinish(TestDetails const& details, float secondsElapsed) override;

    typedef std::vector< DeferredTestResult > DeferredTestResultList;
    DeferredTestResultList& GetResults();

private:
    DeferredTestResultList m_results;
};

}

#endif
