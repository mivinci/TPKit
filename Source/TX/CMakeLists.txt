enable_testing()

set(TX_HEADERS
  Assert.h
  Bits.h
  Exception.h
  Option.h
  Own.h
  Memory.h
  Platform.h
  Result.h

  log/Log.h
  net/Addr.h
  net/DNS.h
  net/TCP.h
  net/UDP.h
  net/UNIX.h
  sync/Condvar.h
  sync/Mutex.h
  time/Clock.h
  time/Time.h
  thread/Thread.h
  runtime/Task.h
  runtime/Driver.h
  runtime/Pool.h
  runtime/Runtime.h
  runtime/Scheduler.h
  runtime/SingleThreadScheduler.h
  runtime/MultiThreadScheduler.h
)

set(TX_SOURCES
  log/Log.cc
  runtime/Pool.cc
)

set(TX_TEST_SOURCES
  log/LogTest.cc
  net/AddrTest.cc
  thread/ThreadTest.cc
  runtime/PoolTest.cc
)

add_library(TX ${TX_HEADERS} ${TX_SOURCES})
target_link_libraries(TX PUBLIC fmt::fmt)

add_executable(TX_Test ${TX_TEST_SOURCES})
target_link_libraries(TX_Test TX GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(TX_Test)
