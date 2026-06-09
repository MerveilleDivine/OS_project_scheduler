cmake_minimum_required(VERSION 3.10)

project(cpu_scheduler_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(scheduler
    src/main.cpp
    src/Parser.cpp
    src/Scheduler.cpp
    src/Reporter.cpp
)

target_include_directories(scheduler PRIVATE include)

enable_testing()
add_test(
    NAME scheduler_cli_regression_tests
    COMMAND ${CMAKE_COMMAND} -E env SCHEDULER_BINARY=$<TARGET_FILE:scheduler> bash ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_cli_tests.sh
)
