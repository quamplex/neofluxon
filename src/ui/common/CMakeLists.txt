set(COMMON_HEADERS
    context/NfContext.h
    NfQtPixmap.h
    NfUiFolderModeState.h
    NfUiLibraryModeState.h
    NfUiShootsModeState.h
    NfUiState.h
    NfPhotoProvider.h
)

set(COMMON_SOURCES
    NfQtPixmap.cpp
    NfUiFolderModeState.cpp
    NfUiLibraryModeState.cpp
    NfUiShootsModeState.cpp
    NfUiState.cpp
    NfPhotoProvider.cpp
)

add_library(NfCommonUi STATIC
    ${COMMON_SOURCES}
    ${COMMON_HEADERS}
)

add_library(${PROJECT_NAME}::CommonUiLibrary ALIAS NfCommonUi)

target_include_directories(NfCommonUi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(NfCommonUi
        PUBLIC
        Qt6::Core
        Qt6::Gui
        ${PROJECT_NAME}::CoreLibrary
)
