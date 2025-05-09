enable_testing()

set(TRANSPORT_HEADERS
  Log.h
  Result.h
  task/Task.h
  task/TaskManager.h
  task/TaskListener.h
)

set(TRANSPORT_SOURCES
  task/Task.cc
  task/TaskManager.cc
)

set(TRANSPORT_TEST_SOURCES
  task/TaskTest.cc
  task/TaskManagerTest.cc
)

add_library(Transport ${TRANSPORT_HEADERS} ${TRANSPORT_SOURCES})
target_link_libraries(Transport PUBLIC TX)

add_executable(Kernel_Test ${TRANSPORT_TEST_SOURCES})
target_link_libraries(Kernel_Test Transport GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(Kernel_Test)
