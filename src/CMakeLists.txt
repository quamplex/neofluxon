add_library(${PROJECT_NAME}::GlobalHeaders INTERFACE)

target_include_directories(${PROJECT_NAME}::GlobalHeaders INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(core)
add_subdirectory(ui)

