cmake_minimum_required(VERSION 3.0.0)
#project(shart_evolution VERSION 0.1.0)

add_library(
    EvolutionLib SHARED
    #Genetic.h
    src/Genetic.cpp
)

#CpuLib already includes CommonsLib so idk if we need to do that again
target_link_libraries(EvolutionLib CpuLib)
target_include_directories(EvolutionLib
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#lets make that test thing
include_directories(${CXXTEST_INCLUDE_DIRS})

add_executable(EvolutionTest src/main.cpp)
#link the stuff so it compiles
target_link_libraries(EvolutionTest EvolutionLib)

find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()

    CXXTEST_ADD_TEST(unittest_evolve foo_test.cc
                      ${CMAKE_CURRENT_SOURCE_DIR}/tests/test1.h)
    target_link_libraries(unittest_evolve EvolutionLib) # yea we need this
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})