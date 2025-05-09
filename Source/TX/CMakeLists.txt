enable_testing()

set(TX_HEADERS
  Assert.h
  Bits.h
  ByteArray.h
  Endian.h
  Exception.h
  Function.h
  Memory.h
  Option.h
  Own.h
  Path.h
  Platform.h
  Result.h
  String.h

  fs/File.h
  io/Error.h
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
  runtime/BlockingPool.h
  runtime/Driver.h
  runtime/EventSource.h
  runtime/EventLoop.h
  runtime/Runtime.h
  runtime/Scheduler.h
  runtime/SingleThreadScheduler.h
  runtime/MultiThreadScheduler.h
  runtime/Task.h
)

set(TX_SOURCES
  log/Log.cc
  net/Addr.cc
  runtime/BlockingPool.cc
)

set(TX_TEST_SOURCES
  fs/FileTest.cc
  log/LogTest.cc
  net/AddrTest.cc
  thread/ThreadTest.cc
  runtime/BlockingPoolTest.cc
  runtime/SingleThreadSchedulerTest.cc
  runtime/RuntimeTest.cc
)

add_library(TX ${TX_HEADERS} ${TX_SOURCES})
target_link_libraries(TX PUBLIC fmt::fmt)

add_executable(TX_Test ${TX_TEST_SOURCES})
target_link_libraries(TX_Test TX GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(TX_Test)
