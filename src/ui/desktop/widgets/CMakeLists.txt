set(WIDGETS_HEADERS
    NfBrowserView.h
    NfCentralWidget.h
    NfLeftPanel.h
    NfMainMenu.h
    NfMainWindow.h
    NfPanel.h
    NfPathBrowser.h
    NfPreviewModeView.h
    NfSettingsDialog.h
    NfStyledWidget.h
    NfThumbnailsModeView.h
    NfTopBar.h
)

set(WIDGETS_SOURCES
    NfBrowserView.cpp
    NfCentralWidget.cpp
    NfLeftPanel.cpp
    NfMainMenu.cpp
    NfMainWindow.cpp
    NfPanel.cpp
    NfPathBrowser.cpp
    NfPreviewModeView.cpp
    NfSettingsDialog.cpp
    NfStyledWidget.cpp
    NfThumbnailsModeView.cpp
    NfTopBar.cpp
)

add_library(NfDesktopWidgets STATIC
    ${WIDGETS_SOURCES}
    ${WIDGETS_HEADERS}
)

add_library(${PROJECT_NAME}::DesktopWidgets ALIAS NfDesktopWidgets)

target_include_directories(NfDesktopWidgets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Dependencies
target_link_libraries(NfDesktopWidgets PUBLIC
        Qt6::Widgets
        ${PROJECT_NAME}::CommonUiLibrary
)
