INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/xoptionswidget.h

SOURCES += \
    $$PWD/xoptionswidget.cpp

FORMS += \
    $$PWD/xoptionswidget.ui

!contains(XCONFIG, xoptions) {
    XCONFIG += xoptions
    include($$PWD/xoptions.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/xoptionswidget.cmake
