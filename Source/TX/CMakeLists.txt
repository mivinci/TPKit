enable_testing()

set(TX_HEADERS
  Assert.h
  Bits.h
  ByteArray.h
  Endian.h
  Exception.h
  Function.h
  Log.h
  Memory.h
  Option.h
  Own.h
  Path.h
  Platform.h
  Result.h
  String.h
  Thread.h

  fs/File.h
  io/Error.h
  net/Addr.h
  net/DNS.h
  net/TCP.h
  net/UDP.h
  net/UNIX.h
  sync/Condvar.h
  sync/Mutex.h
  time/Clock.h
  time/Time.h
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
  Log.cc
  net/Addr.cc
  runtime/BlockingPool.cc
)

set(TX_TEST_SOURCES
  LogTest.cc
  ThreadTest.cc
  
  fs/FileTest.cc
  net/AddrTest.cc
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
