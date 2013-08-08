cmake_minimum_required(VERSION 2.8.3)
project(RpzRdbDatabases)

include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

IF(MINGW)
	SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--kill-at" )
	SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--kill-at" )
ENDIF()

file(GLOB RappelzRDBTool_databasesdll_src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
add_definitions(-DBUILDING_DATABASE)
foreach(srcfile ${RappelzRDBTool_databasesdll_src})
	string(REPLACE ".cpp" "" dllfile ${srcfile})
	add_library(${dllfile} SHARED ${srcfile})
	target_link_libraries(${dllfile} ${LIBRARY_OUTPUT_PATH}/RpzRdbBase.lib)
	install(TARGETS ${dllfile} DESTINATION ./Databases/)
endforeach()
