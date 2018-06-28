#include "TestList.h"
#include "Test.h"

#include <cassert>

namespace UnitTest {

TestList::TestList() 
    : m_head(nullptr)
    , m_tail(nullptr)
{
}

void TestList::Add(Test* test)
{
    if (m_tail == nullptr)
    {
        assert(m_head == nullptr);
        m_head = test;
        m_tail = test;
    }
    else
    {
        m_tail->next = test;
        m_tail = test;
    }
}

Test* TestList::GetHead() const
{
    return m_head;
}

ListAdder::ListAdder(TestList& list, Test* test)
{
    list.Add(test);
}

}
