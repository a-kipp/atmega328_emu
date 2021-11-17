cmake_minimum_required(VERSION 3.20)

project(atmega328_emu)

add_subdirectory(src)
add_subdirectory(test)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(atmega328_emu PRIVATE Threads::Threads)