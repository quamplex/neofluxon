file(GLOB CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

file(GLOB CORE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(${PROJECT_NAME}::CoreLibrary STATIC
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)

target_include_directories(${PROJECT_NAME}::CoreLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_NAME}::GlobalHeaders
)
