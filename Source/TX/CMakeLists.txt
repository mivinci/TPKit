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

  logging/Logger.h
  net/Addr.h
  net/DNS.h
  net/TCP.h
  net/UDP.h
  net/UNIX.h
  time/Clock.h
  time/Time.h
  thread/Condvar.h
  thread/Mutex.h
  runtime/Task.h
  thread/Thread.h
  runtime/Async.h
  runtime/Driver.h
  runtime/Pool.h
  runtime/Runtime.h
  runtime/Scheduler.h
  runtime/SingleThreadScheduler.h
  runtime/MultiThreadScheduler.h
)

set(TX_SOURCES
  logging/Logger.cc
  runtime/Pool.cc
)

set(TX_TEST_SOURCES
  logging/LoggerTest.cc
  net/AddrTest.cc
  thread/ThreadTest.cc
  runtime/AsyncTest.cc
  runtime/PoolTest.cc
)

add_library(TX ${TX_HEADERS} ${TX_SOURCES})
target_link_libraries(TX fmt::fmt)
target_compile_definitions(TX PRIVATE
  TPKit_DIR="${TPKit_DIR}"
)

add_executable(TX_Test ${TX_TEST_SOURCES})
target_link_libraries(TX_Test TX GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(TX_Test)
