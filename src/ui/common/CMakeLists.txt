set(COMMON_HEADERS
    NfPhotoProvider.h
    NfQtPixmap.h
    NfUiState.h
)

set(COMMON_SOURCES
    NfPhotoProvider.cpp
    NfQtPixmap.cpp
    NfUiState.cpp
)

add_library(NfCommonUi STATIC
    ${COMMON_SOURCES}
    ${COMMON_HEADERS}
)

add_library(${PROJECT_NAME}::CommonUiLibrary ALIAS NfCommon)

target_include_directories(NfCommonUi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(NfCommonUi
        PUBLIC
        Qt6::Core
        Qt6::Gui
)
