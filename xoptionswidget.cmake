include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/xoptions.cmake)

set(XOPTIONSWIDGET_SOURCES
    ${XOPTIONS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xoptionswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xoptionswidget.ui
)
