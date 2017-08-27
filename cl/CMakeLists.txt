cmake_minimum_required(VERSION 2.8)

file(GLOB RappelzRDBTool_src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
foreach(srcfile ${RappelzRDBTool_src})
    string(REPLACE ".cpp" "" exefile ${srcfile})
    add_executable(${exefile} ${srcfile})
    target_link_libraries(${exefile} RpzRdbBase)
    install(TARGETS ${exefile} DESTINATION ./)
endforeach()
