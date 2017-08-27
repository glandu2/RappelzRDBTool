cmake_minimum_required(VERSION 2.8)
project(RappelzRDB)

#if(COMMAND cmake_policy)
#	cmake_policy(SET CMP0003 NEW)
#endif(COMMAND cmake_policy)

add_definitions(-DNDEBUG)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/bin)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    SET(CMAKE_EXE_LINKER_FLAGS "/MANIFEST:NO /INCREMENTAL:NO")
    SET(CMAKE_MODULE_LINKER_FLAGS "/MANIFEST:NO /INCREMENTAL:NO")
    SET(CMAKE_SHARED_LINKER_FLAGS "/MANIFEST:NO /INCREMENTAL:NO")
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Base Base)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Databases Databases)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/cl cl)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gui gui)
