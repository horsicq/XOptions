INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/xoptionswidget.ui

HEADERS += \
    $$PWD/xoptionswidget.h

SOURCES += \
    $$PWD/xoptionswidget.cpp

!contains(XCONFIG, xoptions) {
    XCONFIG += xoptions
    include($$PWD/xoptions.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/xoptionswidget.cmake
