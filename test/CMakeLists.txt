target_sources(${TEST_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/MACTest.cpp
                                      ${CMAKE_CURRENT_SOURCE_DIR}/HostTest.cpp
                                      ${CMAKE_CURRENT_SOURCE_DIR}/InitTest.cpp
                                      ${CMAKE_CURRENT_SOURCE_DIR}/IPTest.cpp)

add_test(NAME HostTest COMMAND ${TEST_TARGET})
add_test(NAME InitTest COMMAND ${TEST_TARGET})
add_test(NAME IPTest   COMMAND ${TEST_TARGET})
add_test(NAME MACTest  COMMAND ${TEST_TARGET})