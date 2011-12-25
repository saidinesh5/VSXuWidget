project(VSXuWidget)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(VSXuWidget_SRCS VSXuWidget.cpp main.cpp)
qt4_automoc(${VSXuWidget_SRCS})
add_executable(VSXuWidget ${VSXuWidget_SRCS})
target_link_libraries(VSXuWidget ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
