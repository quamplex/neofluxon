# --- Sources Definition ---
set(MODEL_HEADERS
    NfBrowserModel.h
    NfFolderModel.h
)

set(MODEL_SOURCES
    NfBrowserModel.cpp
    NfFolderModel.cpp
)

add_library(NfDesktopModels STATIC
    ${MODEL_SOURCES}
    ${MODEL_HEADERS}
)

add_library(${PROJECT_NAME}::DesktopModels ALIAS NfDesktopModels)

target_include_directories(NfDesktopModels PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(NfDesktopModels
        PUBLIC
        Qt6::Core
        Qt6::Gui
        ${PROJECT_NAME}::CommonUiLibrary
)
