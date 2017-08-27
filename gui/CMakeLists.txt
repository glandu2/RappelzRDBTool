cmake_minimum_required(VERSION 3.1)
set(TARGET_NAME RappelzRDBToolQt)

find_package(Qt4 4.8 REQUIRED QtCore QtGui)

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

QT4_CREATE_TRANSLATION(TRANSLATIONS ${SOURCES} ${HEADERS} RappelzRDBToolQt_sample.ts)

set(CMAKE_AUTOMOC ON)
add_executable(${TARGET_NAME} WIN32 ${SOURCES} ${HEADERS} ${FORMS} RappelzRDBToolQt_sample.ts)
target_link_libraries(${TARGET_NAME} Qt4::QtCore Qt4::QtGui RpzRdbBase)
target_compile_options(${TARGET_NAME} PRIVATE -Zc:wchar_t-)

