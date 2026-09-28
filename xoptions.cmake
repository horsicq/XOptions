include_directories(${CMAKE_CURRENT_LIST_DIR})

# Set XOPTIONS_USE_EXTERNAL_XXFCLIB when the parent already links xxfclib.
include_directories(${CMAKE_CURRENT_LIST_DIR}/../xxfclib/include)
set(_XOPTIONS_C_SOURCES)
if(NOT XOPTIONS_USE_EXTERNAL_XXFCLIB)
    enable_language(C)
    include(${CMAKE_CURRENT_LIST_DIR}/../xxfclib/xxterminal.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/../xxfclib/xxsettings.cmake)
    set(_XOPTIONS_C_SOURCES ${XXTERMINAL_SOURCES} ${XXSETTINGS_SOURCES})
    list(REMOVE_DUPLICATES _XOPTIONS_C_SOURCES)
    link_libraries(${XXSETTINGS_LIBRARIES})
    add_definitions(-DXXFC_STATIC)
endif()

set(XOPTIONS_SOURCES
    ${XOPTIONS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xoptions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xoptions.h
    ${CMAKE_CURRENT_LIST_DIR}/xoptions_settings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codecs/codec_cp437.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codecs/codec_cp437.h
    ${CMAKE_CURRENT_LIST_DIR}/xthreadobject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xthreadobject.h
    ${CMAKE_CURRENT_LIST_DIR}/xcolorstring.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xcolorstring.h
    ${CMAKE_CURRENT_LIST_DIR}/xconsoloutput.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xconsoloutput.h
    ${_XOPTIONS_C_SOURCES}
)
unset(_XOPTIONS_C_SOURCES)
