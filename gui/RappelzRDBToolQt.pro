# -------------------------------------------------
# Project created by QtCreator 2012-07-12T12:36:12
# -------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RappelzRDBToolQt
TEMPLATE = app
DEFINES += BUILDING_CLIENT _CRT_SECURE_NO_WARNINGS
#QT_NO_DEBUG_OUTPUT

#CONFIG += console
CONFIG -= app_bundle
CONFIG += debug_and_release debug_and_release_target

#QMAKE_CXXFLAGS += -march=pentium3 -mtune=generic -mfpmath=sse -msse -mmmx
#QMAKE_CXXFLAGS += -arch:SSE

DESTDIR = ../bin

win32 {
LIBS += ../bin/RpzRdbBase.lib Shell32.lib comdlg32.lib odbc32.lib
}
unix {
LIBS += ../bin/RpzRdbBase.so -Wl,-R,\'\$\$ORIGIN/\' -lodbc
}

FORMS += \
    Maingui.ui \
    SqlConfigDialog.ui \
    DatabaseView.ui \
    DatabaseDescManageDialog.ui \
    HashConverterDialog.ui \
    SqlOpenSaveDialog.ui \
    LogWindow.ui

SOURCES += \
    main.cpp \
    Maingui.cpp \
    DatabaseTableModel.cpp \
    SqlConfigDialog.cpp \
    DatabaseView.cpp \
    PersistentData.cpp \
    DatabaseDescriptionListModel.cpp \
    OpenSaveSource.cpp \
    TabBarEventFilter.cpp \
    DatabaseDescManageDialog.cpp \
    NameToHash.c \
    Settings.cpp \
    HashConverterDialog.cpp \
    HashToName.c \
    FileDialog.cpp \
    FileToRenameListModel.cpp \
    SqlOpenSaveDialog.cpp \
    LogWindow.cpp

HEADERS += \
    Maingui.h \
    ../Base/ExportDLL.h \
    ../Base/DataType.h \
    ../Base/IRowManipulator.h \
    ../Base/IObject.h \
    ../Base/IDatabaseDescription.h \
    ../Base/IDatabase.h \
    ../Base/Guid.h \
    DatabaseTableModel.h \
    SqlConfigDialog.h \
    DatabaseView.h \
    PersistentData.h \
    DatabaseDescriptionListModel.h \
    OpenSaveSource.h \
    TabBarEventFilter.h \
    DatabaseDescManageDialog.h \
    NameToHash.h \
    Settings.h \
    HashConverterDialog.h \
    HashToName.h \
    FileDialog.h \
    FileToRenameListModel.h \
    SqlOpenSaveDialog.h \
    LogWindow.h

TRANSLATIONS = RappelzRDBToolQt_sample.ts
