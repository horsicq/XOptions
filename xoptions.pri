INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/codecs/codec_cp437.h \
    $$PWD/xoptions.h \
    $$PWD/codecs/codec_cp437.h

SOURCES += \
    $$PWD/codecs/codec_cp437.cpp \
    $$PWD/xoptions.cpp \
    $$PWD/codecs/codec_cp437.cpp

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/xoptions.cmake
