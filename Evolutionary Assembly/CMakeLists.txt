cmake_minimum_required(VERSION 3.0.0)
#project(shart_evolution VERSION 0.1.0)

add_library(
    EvolutionLib SHARED
    #Genetic.h
    Genetic.cpp
)
#CpuLib already includes CommonsLib so idk if we need to do that again
target_link_libraries(EvolutionLib CpuLib)
target_include_directories(EvolutionLib PUBLIC ./)

#lets make that test thing
add_executable(EvolutionTest main.cpp)
#link the stuff so it compiles
target_link_libraries(EvolutionTest EvolutionLib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})