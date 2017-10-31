cmake_minimum_required(VERSION 3.1)
set(TARGET_NAME RpzRdbBase)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

file(GLOB RappelzRDBTool_basedll_src
	src/*.cpp
	src/*.h
	include/*.h
)

add_library(${TARGET_NAME} ${RappelzRDBTool_basedll_src})
target_include_directories(${TARGET_NAME} PUBLIC include)
target_compile_definitions(${TARGET_NAME} PRIVATE BUILDING_BASE)
target_link_libraries(${TARGET_NAME} lua iconv)
if(WIN32)
        target_link_libraries(${TARGET_NAME} odbc32)
else()
	    target_link_libraries(${TARGET_NAME} odbc dl)
endif()

install(DIRECTORY include/ DESTINATION ./ COMPONENT "dev")
install(TARGETS ${TARGET_NAME} ARCHIVE DESTINATION ./ COMPONENT "dev"
	LIBRARY DESTINATION ./
	RUNTIME DESTINATION ./)
