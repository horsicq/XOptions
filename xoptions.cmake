include_directories(${CMAKE_CURRENT_LIST_DIR})

set(XOPTIONS_SOURCES
    ${XOPTIONS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xoptions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xoptions.h
    ${CMAKE_CURRENT_LIST_DIR}/codecs/codec_cp437.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codecs/codec_cp437.h
    ${CMAKE_CURRENT_LIST_DIR}/xthreadobject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xthreadobject.h
    ${CMAKE_CURRENT_LIST_DIR}/xprocesswatch.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xprocesswatch.h
    ${CMAKE_CURRENT_LIST_DIR}/xcolorstring.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xcolorstring.h
)
