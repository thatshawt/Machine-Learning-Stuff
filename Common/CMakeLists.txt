cmake_minimum_required(VERSION 3.0.0)
# project(shart_commons
#     VERSION 0.1.0
# )

add_library(
    CommonsLib INTERFACE
)

target_include_directories(CommonsLib INTERFACE ./)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})