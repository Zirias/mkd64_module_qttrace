#-------------------------------------------------
#
# Project created by QtCreator 2014-01-13T19:36:18
#
#-------------------------------------------------

TARGET = qttrace
TEMPLATE = lib

SOURCES += traceviewer.cpp \
    blocksdisplay.cpp

HEADERS += traceviewer.h \
    blocksdisplay.h

FORMS += \
    blocksdisplay.ui

OTHER_FILES += \
    gfx/blReserved.png \
    gfx/blFree.png \
    gfx/blAllocated.png

RESOURCES += \
    gfx.qrc
