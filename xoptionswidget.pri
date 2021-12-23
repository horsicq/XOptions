INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, xoptions) {
    XCONFIG += xoptions
    include($$PWD/xoptions.pri)
}
