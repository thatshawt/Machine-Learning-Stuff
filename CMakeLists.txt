cmake_minimum_required(VERSION 3.0.0)
project(Machine_Learning_Things VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(Cpu)
add_subdirectory(Common)
add_subdirectory("Evolutionary Assembly")
add_subdirectory(Simple-Web-Server)

# include_directories(
#         ${PROJECT_SOURCE_DIR}/**
#         /Common
#         /Cpu
#         /Evolutionary Assembly
# )

# link_libraries(Machine_Learning_Things shartcommons shartolution shartcpu)
# add_executable(Machine_Learning_Things Cpu/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
