/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "WorkerTest.hpp"

#include <fwThread/Timer.hpp>
#include <fwThread/Worker.hpp>

#include <fwCore/spyLog.hpp>

#include <fwTest/Exception.hpp>

#include <exception>
#include <iostream>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::fwThread::ut::WorkerTest );

namespace fwThread
{
namespace ut
{

static ::fwTest::Exception e(""); // force link with fwTest

//------------------------------------------------------------------------------

void WorkerTest::setUp()
{
    // Set up context before running a test.
}
//------------------------------------------------------------------------------

void WorkerTest::tearDown()
{
    // Clean up after the test run.
}

//-----------------------------------------------------------------------------

struct TestHandler
{
    TestHandler() :
        m_step(0),
        m_threadCheckOk(true)
    {
        m_constructorThreadId = ::fwThread::getCurrentThreadId();
    }

    //------------------------------------------------------------------------------

    void nextStep()
    {
        std::this_thread::sleep_for( std::chrono::milliseconds(50));
        this->nextStepNoSleep();
    }

    //------------------------------------------------------------------------------

    void nextStepNoSleep()
    {

        m_threadCheckOk &= (m_constructorThreadId != ::fwThread::getCurrentThreadId());
        m_threadCheckOk &= (m_workerThreadId == ::fwThread::getCurrentThreadId());
        ++m_step;
    }

    //------------------------------------------------------------------------------

    void setWorkerId(::fwThread::ThreadIdType id)
    {
        m_workerThreadId = id;
    }

    int m_step;
    bool m_threadCheckOk;
    ::fwThread::ThreadIdType m_constructorThreadId;
    ::fwThread::ThreadIdType m_workerThreadId;
};

//-----------------------------------------------------------------------------

void WorkerTest::basicTest()
{
    {
        ::fwThread::Worker::sptr worker = ::fwThread::Worker::New();

        TestHandler handler;
        handler.setWorkerId(worker->getThreadId());
        worker->post( std::bind( &TestHandler::nextStep, &handler) );
        worker->post( std::bind( &TestHandler::nextStep, &handler) );
        worker->post( std::bind( &TestHandler::nextStep, &handler) );

        worker->stop();
        CPPUNIT_ASSERT_EQUAL(3, handler.m_step);
        CPPUNIT_ASSERT_EQUAL(true, handler.m_threadCheckOk);
    }

    {
        ::fwThread::Worker::sptr worker = ::fwThread::Worker::New();

        TestHandler handler;
        handler.setWorkerId(worker->getThreadId());
        worker->post( std::bind( &TestHandler::nextStepNoSleep, &handler) );
        worker->post( std::bind( &TestHandler::nextStepNoSleep, &handler) );
        worker->post( std::bind( &TestHandler::nextStepNoSleep, &handler) );

        worker->stop();
        CPPUNIT_ASSERT_EQUAL(3, handler.m_step);
        CPPUNIT_ASSERT_EQUAL(true, handler.m_threadCheckOk);
    }
}

//-----------------------------------------------------------------------------

void WorkerTest::timerTest()
{
    //Basic tests
    {
        ::fwThread::Worker::sptr worker = ::fwThread::Worker::New();

        TestHandler handler;
        handler.setWorkerId(worker->getThreadId());

        ::fwThread::Timer::sptr timer = worker->createTimer();

        timer->setFunction( std::bind( &TestHandler::nextStep, &handler) );
        timer->setDuration(std::chrono::milliseconds(100));

        timer->start();
        {
            CPPUNIT_ASSERT(timer->isRunning());
            CPPUNIT_ASSERT(handler.m_threadCheckOk);
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
        timer->stop();

        CPPUNIT_ASSERT_EQUAL(2, handler.m_step);
        CPPUNIT_ASSERT(!timer->isRunning());
        CPPUNIT_ASSERT(handler.m_threadCheckOk);

        worker->stop();
    }

    //Test the timer when we used a function which has it time duration longer than the timer's call duration.
    //The timer mustn't call the function until the last call has finished.
    /*
     * It mustn't do that (values in milliseconds) :
     *  10     50
     * |--|----------|
     *    |10    50
     *    |--|----------|
     *       |10    50
     *       |--|----------|
     *
     * But that instead :
     *  10     50     0    50      0     50
     * |--|----------|-|----------|-|----------|
     */
    {
        ::fwThread::Worker::sptr worker = ::fwThread::Worker::New();

        TestHandler handler;
        handler.setWorkerId(worker->getThreadId());

        ::fwThread::Timer::sptr timer = worker->createTimer();

        timer->setFunction( std::bind( &TestHandler::nextStep, &handler) );
        timer->setDuration(std::chrono::milliseconds(10));

        timer->start();
        {
            CPPUNIT_ASSERT(timer->isRunning());
            CPPUNIT_ASSERT(handler.m_threadCheckOk);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        timer->stop();

        CPPUNIT_ASSERT_EQUAL(2, handler.m_step);
        CPPUNIT_ASSERT(!timer->isRunning());
        CPPUNIT_ASSERT(handler.m_threadCheckOk);

        worker->stop();
    }

    // Test for "stop()" blocking until call has finished it's execution
    {
        ::fwThread::Worker::sptr worker = ::fwThread::Worker::New();

        TestHandler handler;
        handler.setWorkerId(worker->getThreadId());

        ::fwThread::Timer::sptr timer = worker->createTimer();

        timer->setFunction( std::bind( &TestHandler::nextStep, &handler) );
        timer->setDuration(std::chrono::milliseconds(10));

        timer->start();
        {
            CPPUNIT_ASSERT(timer->isRunning());
            CPPUNIT_ASSERT(handler.m_threadCheckOk);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        timer->stop();

        CPPUNIT_ASSERT(handler.m_threadCheckOk);
        CPPUNIT_ASSERT(!timer->isRunning());

        int step = handler.m_step;

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        CPPUNIT_ASSERT(handler.m_threadCheckOk);
        CPPUNIT_ASSERT(!timer->isRunning());
        CPPUNIT_ASSERT_EQUAL(step, handler.m_step);

        worker->stop();
    }

    /// Test for multiple start, stop
    {
        ::fwThread::Worker::sptr worker = ::fwThread::Worker::New();

        TestHandler handler;
        handler.setWorkerId(worker->getThreadId());

        ::fwThread::Timer::sptr timer = worker->createTimer();

        timer->setFunction( std::bind( &TestHandler::nextStep, &handler) );
        timer->setDuration(std::chrono::milliseconds(10));

        CPPUNIT_ASSERT(handler.m_threadCheckOk);
        CPPUNIT_ASSERT(!timer->isRunning());
        timer->start();
        CPPUNIT_ASSERT(handler.m_threadCheckOk);
        CPPUNIT_ASSERT(timer->isRunning());
        timer->start();
        CPPUNIT_ASSERT(handler.m_threadCheckOk);
        CPPUNIT_ASSERT(timer->isRunning());
        timer->start();
        CPPUNIT_ASSERT(handler.m_threadCheckOk);
        CPPUNIT_ASSERT(timer->isRunning());
        timer->start();
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        timer->stop();
        CPPUNIT_ASSERT(handler.m_threadCheckOk);
        CPPUNIT_ASSERT(!timer->isRunning());
        timer->stop();
        CPPUNIT_ASSERT(handler.m_threadCheckOk);
        CPPUNIT_ASSERT(!timer->isRunning());
        timer->stop();
        CPPUNIT_ASSERT(handler.m_threadCheckOk);
        CPPUNIT_ASSERT(!timer->isRunning());
        timer->stop();
        CPPUNIT_ASSERT(handler.m_threadCheckOk);
        CPPUNIT_ASSERT(!timer->isRunning());

        CPPUNIT_ASSERT_EQUAL(2, handler.m_step);

        worker->stop();
    }

    /// Test for instant stop
    {
        ::fwThread::Worker::sptr worker = ::fwThread::Worker::New();

        TestHandler handler;
        handler.setWorkerId(worker->getThreadId());

        ::fwThread::Timer::sptr timer = worker->createTimer();

        timer->setFunction( std::bind( &TestHandler::nextStep, &handler) );
        timer->setDuration(std::chrono::milliseconds(10));

        CPPUNIT_ASSERT(handler.m_threadCheckOk);
        CPPUNIT_ASSERT(!timer->isRunning());
        timer->start();
        timer->stop();
        CPPUNIT_ASSERT(handler.m_threadCheckOk);
        CPPUNIT_ASSERT(!timer->isRunning());

        CPPUNIT_ASSERT_EQUAL(0, handler.m_step);

        worker->stop();
    }

    /// Test when delete timer during execution
    {
        ::fwThread::Worker::sptr worker = ::fwThread::Worker::New();

        TestHandler handler;
        handler.setWorkerId(worker->getThreadId());

        {
            ::fwThread::Timer::sptr timer = worker->createTimer();

            timer->setFunction( std::bind( &TestHandler::nextStep, &handler) );
            timer->setDuration(std::chrono::milliseconds(10));

            CPPUNIT_ASSERT(handler.m_threadCheckOk);
            CPPUNIT_ASSERT(!timer->isRunning());
            timer->start();
            CPPUNIT_ASSERT(handler.m_threadCheckOk);
            CPPUNIT_ASSERT(timer->isRunning());
            std::this_thread::sleep_for(std::chrono::milliseconds(70));
        }

        CPPUNIT_ASSERT(handler.m_threadCheckOk);
        CPPUNIT_ASSERT_EQUAL(1, handler.m_step);

        worker->stop();
    }
}

//-----------------------------------------------------------------------------

} //namespace ut
} //namespace fwThread
