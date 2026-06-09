cmake_minimum_required(VERSION 3.10)

project(cpu_scheduler_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(scheduler scheduler.cpp)

enable_testing()
add_test(
    NAME scheduler_cli_regression_tests
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_cli_tests.sh
)
