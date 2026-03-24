INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/codecs/codec_cp437.h \
    $$PWD/xoptions.h \
    $$PWD/xthreadobject.h \
    $$PWD/xcolorstring.h

SOURCES += \
    $$PWD/codecs/codec_cp437.cpp \
    $$PWD/xoptions.cpp \
    $$PWD/xthreadobject.cpp \
    $$PWD/xcolorstring.cpp

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/xoptions.cmake
