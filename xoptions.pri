INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/codecs/codec_cp437.h \
    $$PWD/xoptions.h \
    $$PWD/xthreadobject.h \
    $$PWD/xprocesswatch.h

SOURCES += \
    $$PWD/codecs/codec_cp437.cpp \
    $$PWD/xoptions.cpp \
    $$PWD/xthreadobject.cpp \
    $$PWD/xprocesswatch.cpp

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/xoptions.cmake
