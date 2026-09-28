INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# Add xxfclib_external to XCONFIG when the parent already links xxfclib.
INCLUDEPATH += $$PWD/../xxfclib/include
!contains(XCONFIG, xxfclib_external):!contains(XCONFIG, xxterminal) {
    XCONFIG += xxterminal
    include($$PWD/../xxfclib/xxterminal.pri)
}
!contains(XCONFIG, xxfclib_external):!contains(XCONFIG, xxsettings) {
    XCONFIG += xxsettings
    include($$PWD/../xxfclib/xxsettings.pri)
}

HEADERS += \
    $$PWD/codecs/codec_cp437.h \
    $$PWD/xoptions.h \
    $$PWD/xthreadobject.h \
    $$PWD/xcolorstring.h \
    $$PWD/xconsoloutput.h

SOURCES += \
    $$PWD/codecs/codec_cp437.cpp \
    $$PWD/xoptions.cpp \
    $$PWD/xoptions_settings.cpp \
    $$PWD/xthreadobject.cpp \
    $$PWD/xcolorstring.cpp \
    $$PWD/xconsoloutput.cpp

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/xoptions.cmake
