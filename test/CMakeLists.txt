target_sources(${TEST_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/InitTest.cpp)

add_test(NAME InitTest COMMAND ${TEST_TARGET})