target_sources(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/host.cpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/IP.cpp)

target_sources(${TEST_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/host.cpp
                                      ${CMAKE_CURRENT_SOURCE_DIR}/IP.cpp)