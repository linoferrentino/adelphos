cmake_minimum_required(VERSION 3.16)

project (adelphos LANGUAGES C)

set (Adelphos_VERSION_MAJOR 0)
set (Adelphos_VERSION_MINOR 0)
set (Adelphos_VERSION_MAINT 1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wfatal-errors")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic" )


include_directories("${PROJECT_BINARY_DIR}/include")

# The various directories which are used 
add_subdirectory(common)
add_subdirectory(lib)
add_subdirectory(cli)
add_subdirectory(test)

