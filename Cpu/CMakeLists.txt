cmake_minimum_required(VERSION 3.0.0)
#project(shart_cpu VERSION 0.1.0)

#create a cpu library so we can reuse da code
add_library(
    CpuLib SHARED
    #Cpu2.h
    Cpu.cpp
    #CpuThreading.h
    CpuThreading.cpp
    ppu.cpp # ppu isnt used rn but yea
)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED) # THREAD FIX
# target_link_libraries(CpuLib PRIVATE Threads::Threads)

#ok this is the actual rart hack thing
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
target_include_directories(CpuLib PUBLIC ./)




#lets make an executable too
add_executable(CpuTest main.cpp)
#link the CpuLib so it compiles lol
target_link_libraries(CpuTest CpuLib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})