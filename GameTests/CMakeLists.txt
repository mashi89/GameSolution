cmake_minimum_required(VERSION 3.14)
project(GameTests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

set(GAME_SRC_DIR "${CMAKE_SOURCE_DIR}/../GameProject")

set(GAME_SOURCES
    ${GAME_SRC_DIR}/Logger.cpp
    ${GAME_SRC_DIR}/Sprite.cpp
    ${GAME_SRC_DIR}/GameLoop.cpp
    ${GAME_SRC_DIR}/GameManager.cpp
    ${GAME_SRC_DIR}/AudioManager.cpp
    ${GAME_SRC_DIR}/NetworkManager.cpp
)

set(TEST_SOURCES
    tests/test_MathUtils.cpp
    tests/test_RenderColor.cpp
    tests/test_Sprite.cpp
    tests/test_Logger.cpp
    tests/test_GameManager.cpp
    tests/test_AudioManager.cpp
    tests/test_NetworkManager.cpp
    tests/test_GameLoop.cpp
)

add_executable(GameTests ${GAME_SOURCES} ${TEST_SOURCES})
target_include_directories(GameTests PRIVATE ${GAME_SRC_DIR})
target_link_libraries(GameTests GTest::GTest GTest::Main)

include(GoogleTest)
gtest_discover_tests(GameTests)
