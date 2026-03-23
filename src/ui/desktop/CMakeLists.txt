# src/ui/desktop/CMakeLists.txt

set(CMAKE_AUTORCC ON)

# Collect sources and headers
file(GLOB_RECURSE DESKTOP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/models/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/*.cpp
)

file(GLOB_RECURSE DESKTOP_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/*.h
)

# Create executable
add_executable(${PROJECT_NAME}
    ${DESKTOP_SOURCES}
    ${DESKTOP_HEADERS}
    resources/resources.qrc
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    ${PROJECT_NAME}::CoreLibrary
)

# Include directories for UI-only headers
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/models
)

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
)
