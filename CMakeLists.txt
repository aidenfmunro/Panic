cmake_minimum_required(VERSION 3.10)
project(PANIC)

set(TARGET PANIC)
set(TEST_TARGET PANIC_Tests)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${TARGET})

# target_compile_options(${TARGET} PRIVATE -fsanitize=address -g)
target_link_options(${TARGET} PRIVATE -lpthread)

enable_testing()

find_package(GTest REQUIRED)
add_executable(${TEST_TARGET})
target_link_libraries(${TEST_TARGET} PUBLIC GTest::GTest GTest::Main)

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${TEST_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Важно добавлять add_subdirectory в конец CMakeLists.txt
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)

set_target_properties(${TARGET}
                      PROPERTIES 
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)