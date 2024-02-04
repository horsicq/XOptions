INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/codecs/codec_cp437.h \
    $$PWD/xoptions.h

SOURCES += \
    $$PWD/codecs/codec_cp437.cpp \
    $$PWD/xoptions.cpp

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/xoptions.cmake
