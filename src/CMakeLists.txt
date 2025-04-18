# set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src.cpp)

target_sources(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_sources(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
${SOURCES})

target_sources(${TEST_TARGET} PRIVATE ${SOURCES})