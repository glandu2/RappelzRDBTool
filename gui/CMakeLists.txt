cmake_minimum_required(VERSION 3.1)
set(TARGET_NAME RappelzRDBToolQt)

find_package(Qt5 QUIET COMPONENTS Core Gui Widgets LinguistTools)
if(NOT Qt5_FOUND)
	find_package(Qt4 4.8 REQUIRED QtCore QtGui)
endif()

if(Qt5_FOUND)
	message(STATUS "Using Qt5")
else()
	message(STATUS "Using Qt4")
endif()

set(FORMS
    Maingui.ui
    SqlConfigDialog.ui
    DatabaseView.ui
    DatabaseDescManageDialog.ui
    HashConverterDialog.ui
    SqlOpenSaveDialog.ui
    LogWindow.ui
)

set(SOURCES
    main.cpp
    Maingui.cpp
    DatabaseTableModel.cpp
    SqlConfigDialog.cpp
    DatabaseView.cpp
    PersistentData.cpp
    DatabaseDescriptionListModel.cpp
    OpenSaveSource.cpp
    TabBarEventFilter.cpp
    DatabaseDescManageDialog.cpp
    NameToHash.c
    Settings.cpp
    HashConverterDialog.cpp
    HashToName.c
    FileDialog.cpp
    FileToRenameListModel.cpp
    SqlOpenSaveDialog.cpp
    LogWindow.cpp
)

set(HEADERS
    Maingui.h
    DatabaseTableModel.h
    SqlConfigDialog.h
    DatabaseView.h
    PersistentData.h
    DatabaseDescriptionListModel.h
    OpenSaveSource.h
    TabBarEventFilter.h
    DatabaseDescManageDialog.h
    NameToHash.h
    Settings.h
    HashConverterDialog.h
    HashToName.h
    FileDialog.h
    FileToRenameListModel.h
    SqlOpenSaveDialog.h
    LogWindow.h
)

if(NOT Qt5_FOUND)
	QT4_CREATE_TRANSLATION(TRANSLATIONS ${SOURCES} ${HEADERS} RappelzRDBToolQt_sample.ts)
else()
	QT5_CREATE_TRANSLATION(TRANSLATIONS ${SOURCES} ${HEADERS} RappelzRDBToolQt_sample.ts)
endif()

configure_file(version.h.in version.h @ONLY)


if(Qt5_FOUND AND WIN32 AND TARGET Qt5::qmake AND NOT TARGET Qt5::windeployqt)
	get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)

	execute_process(
		COMMAND "${_qt5_qmake_location}" -query QT_INSTALL_PREFIX
		RESULT_VARIABLE return_code
		OUTPUT_VARIABLE qt5_install_prefix
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	set(QT5_WINDEPLOYQT "${qt5_install_prefix}/bin/windeployqt.exe")
endif()


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${TARGET_NAME} WIN32 ${SOURCES} ${HEADERS} ${FORMS} ${TRANSLATIONS} version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
target_link_libraries(${TARGET_NAME} RpzRdbBase)

# Used for both Qt4 with cmake < 3.8 and version.h
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

if(Qt5_FOUND)
	target_link_libraries(${TARGET_NAME} Qt5::Core Qt5::Gui Qt5::Widgets)
else()
	target_link_libraries(${TARGET_NAME} Qt4::QtCore Qt4::QtGui)
	target_compile_options(${TARGET_NAME} PRIVATE -Zc:wchar_t-)
endif()

install(TARGETS ${TARGET_NAME} DESTINATION ./)
install(FILES RappelzRDBToolQt_sample.ts DESTINATION ./)

if(EXISTS ${QT5_WINDEPLOYQT})
	# execute windeployqt in a tmp directory after build
	add_custom_command(TARGET ${TARGET_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"
		COMMAND set PATH=%PATH%$<SEMICOLON>${qt5_install_prefix}/bin
		COMMAND "${QT5_WINDEPLOYQT}"
			--dir "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"
			--no-quick-import
			--no-compiler-runtime
			--no-opengl-sw
			--no-virtualkeyboard
			--no-webkit2
			--no-angle
			"$<TARGET_FILE:${TARGET_NAME}>"
	)

	# copy deployment directory during installation
	install(
		DIRECTORY
		"${CMAKE_CURRENT_BINARY_DIR}/windeployqt/"
		DESTINATION ./
		COMPONENT bin-deps
	)
endif()

