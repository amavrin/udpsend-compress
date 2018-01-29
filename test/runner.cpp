/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_SockTest_init = false;
#include "sock_send_test.h"

static SockTest suite_SockTest;

static CxxTest::List Tests_SockTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_SockTest( "sock_send_test.h", 9, "SockTest", suite_SockTest, Tests_SockTest );

static class TestDescription_suite_SockTest_testCanSendUDP : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SockTest_testCanSendUDP() : CxxTest::RealTestDescription( Tests_SockTest, suiteDescription_SockTest, 11, "testCanSendUDP" ) {}
 void runTest() { suite_SockTest.testCanSendUDP(); }
} testDescription_suite_SockTest_testCanSendUDP;

static class TestDescription_suite_SockTest_testCanCompressData : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SockTest_testCanCompressData() : CxxTest::RealTestDescription( Tests_SockTest, suiteDescription_SockTest, 25, "testCanCompressData" ) {}
 void runTest() { suite_SockTest.testCanCompressData(); }
} testDescription_suite_SockTest_testCanCompressData;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
