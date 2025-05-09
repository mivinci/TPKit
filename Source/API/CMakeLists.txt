set(API_HEADERS
  Errno.h
  Export.h
  Transport.h
)

set(API_SOURCES
  Transport.cc
)

add_library(API ${API_HEADERS} ${API_SOURCES})
target_link_libraries(API PUBLIC Transport)
