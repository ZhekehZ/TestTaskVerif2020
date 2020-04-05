cmake_minimum_required(VERSION 3.15)
project(Spinlock2020)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

add_executable(test tests.cpp)
target_link_libraries(test PRIVATE pthread)

add_executable(genmc_test genmc.c)
target_link_libraries(genmc_test PRIVATE pthread)
