cmake_minimum_required(VERSION 3.0.0)
#project(shart_cpu VERSION 0.1.0)

#create a cpu library so we can reuse da code
add_library(
    CpuLib SHARED
    #Cpu2.h
    src/Cpu.cpp
    #CpuThreading.h
    src/CpuThreading.cpp
    src/ppu.cpp # ppu isnt used rn but yea
)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED) # THREAD FIX
# target_link_libraries(CpuLib PRIVATE Threads::Threads)

#so i ripped this from some github issue
if(THREADS_HAVE_PTHREAD_ARG)
    set_property(TARGET CpuLib PROPERTY COMPILE_OPTIONS "-pthread")
    set_property(TARGET CpuLib PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif()

# if(CMAKE_THREAD_LIBS_INIT)
#     target_link_libraries(CpuLib "${CMAKE_THREAD_LIBS_INIT}")
# endif()

#link the CommonsLib cus that has the helper.h that we need
target_link_libraries(CpuLib
    PUBLIC CommonsLib
    "${CMAKE_THREAD_LIBS_INIT}"
    PRIVATE Threads::Threads #THREAD FIX
)
target_include_directories(CpuLib
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#lets make an executable too
add_executable(CpuTest src/main.cpp)
#link the CpuLib so it compiles lol
target_link_libraries(CpuTest CpuLib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})