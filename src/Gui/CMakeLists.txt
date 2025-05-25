target_sources(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow.cpp ${GUI_INC_DIR}/MainWindow.hpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/ChartWindow.cpp ${GUI_INC_DIR}/ChartWindow.hpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/MonitorController.cpp ${GUI_INC_DIR}/MonitorController.hpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/PingWorker.cpp ${GUI_INC_DIR}/PingWorker.hpp)

target_sources(${TEST_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PingWorker.cpp ${GUI_INC_DIR}/PingWorker.hpp)


