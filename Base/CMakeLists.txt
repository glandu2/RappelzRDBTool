cmake_minimum_required(VERSION 2.8.3)
set(TARGET_NAME RpzRdbBase)

add_definitions(-DBUILDING_BASE)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

file(GLOB RappelzRDBTool_basedll_src *.cpp *.h)

add_definitions(-DLIBDIR=\".\" -DICONV_CONST=const -DHAVE_WORKING_O_NOFOLLOW=1)
set(RappelzRDBTool_basedll_src "${RappelzRDBTool_basedll_src};${CMAKE_CURRENT_LIST_DIR}/libiconv/include/iconv.h;${CMAKE_CURRENT_LIST_DIR}/libiconv/src/iconv.c;${CMAKE_CURRENT_LIST_DIR}/libiconv/src/localcharset.c")
include_directories(${CMAKE_CURRENT_LIST_DIR}/libiconv/include)

#SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--kill-at -Wl,--output-def,${LIBRARY_OUTPUT_PATH}/${TARGET_NAME}.def" )
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--kill-at -Wl,--output-def,${LIBRARY_OUTPUT_PATH}/${TARGET_NAME}.def" )


add_library(${TARGET_NAME} SHARED ${RappelzRDBTool_basedll_src})
if(WIN32)
        target_link_libraries(${TARGET_NAME} odbc32)
else(WIN32)
        target_link_libraries(${TARGET_NAME} odbc)
endif(WIN32)
install(TARGETS ${TARGET_NAME} DESTINATION ./)
