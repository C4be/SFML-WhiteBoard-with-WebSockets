cmake_minimum_required(VERSION 3.5.0)
project(WhiteBoard VERSION 0.1.0 LANGUAGES C CXX)

# Добавляем папку src
add_executable(WhiteBoard src/main.cpp)

add_executable(BoardConn src/conwin.cpp)

# Поиск библиотеки SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Линкуем SFML с проектом
target_link_libraries(WhiteBoard sfml-graphics sfml-window sfml-system)
target_link_libraries(BoardConn sfml-graphics sfml-window sfml-system)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
