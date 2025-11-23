INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

win32:LIBS += -luser32 -lshell32 -lole32

HEADERS += \
    $$PWD/codecs/codec_cp437.h \
    $$PWD/xoptions.h \
    $$PWD/xthreadobject.h \
    $$PWD/xprocesswatch.h

contains(QT, widgets) {
    INCLUDEPATH += $$PWD/../gui_source
}

SOURCES += \
    $$PWD/codecs/codec_cp437.cpp \
    $$PWD/xoptions.cpp \
    $$PWD/xthreadobject.cpp \
    $$PWD/xprocesswatch.cpp

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/xoptions.cmake
