cmake_minimum_required(VERSION 2.8)

file(GLOB RappelzRDBTool_src *.cpp *.h)
set(RappelzRDBTool_target RappelzRDBToolCl)

include_directories(${PROJECT_SOURCE_DIR}/Base)
link_directories(${LIBRARY_OUTPUT_PATH})

add_definitions(-DBUILDING_CLIENT -DAPPNAME=\"${RappelzRDBTool_target}\")
add_executable(${RappelzRDBTool_target} ${RappelzRDBTool_src})
target_link_libraries(${RappelzRDBTool_target} RpzRdbBase)
install(TARGETS ${RappelzRDBTool_target} DESTINATION ./)
