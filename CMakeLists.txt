cmake_minimum_required(VERSION 3.10)
project(PANIC)

set(TARGET PANIC)
set(TEST_TARGET PANIC_Tests)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GUI_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/Gui)

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent Charts)
qt_standard_project_setup()

add_executable(${TARGET})

# starget_compile_options(${TARGET} PRIVATE -fsanitize=address )
target_link_options(${TARGET} PRIVATE -lpthread)

enable_testing()

find_package(GTest REQUIRED)
add_executable(${TEST_TARGET})
target_link_libraries(${TEST_TARGET} PUBLIC GTest::GTest GTest::Main)
target_link_libraries(${TARGET} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Concurrent
    Qt6::Charts
)

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${TEST_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Важно добавлять add_subdirectory в конец CMakeLists.txt
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)

set_target_properties(${TARGET}
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

install(TARGETS ${TARGET}
        RUNTIME DESTINATION bin)

