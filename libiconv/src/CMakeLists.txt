cmake_minimum_required(VERSION 2.8.12)

file(GLOB_RECURSE SOURCE_FILES *.c *.h ../include/*.h)

add_library(${TARGET_NAME} ${SOURCE_FILES})
target_include_directories(${TARGET_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include")

if(BUILD_SHARED_LIBS)
	target_compile_definitions(${TARGET_NAME} PRIVATE BUILDING_LIBICONV INTERFACE USING_ICONV_SHARED)
endif()

target_compile_definitions(${TARGET_NAME} PRIVATE
	LIBDIR=\".\"
	ICONV_CONST=const
	HAVE_WORKING_O_NOFOLLOW=1
)

install(TARGETS ${TARGET_NAME} DESTINATION ./)
