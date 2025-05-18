target_sources(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Net)