file(GLOB CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

file(GLOB CORE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(NfCore STATIC
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)

add_library(${PROJECT_NAME}::CoreLibrary ALIAS NfCore)

target_include_directories(NfCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
