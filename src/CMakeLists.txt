#add_library(GlobalHeaders INTERFACE)
#add_library(${PROJECT_NAME}::GlobalHeaders ALIAS GlobalHeaders)

#target_include_directories(${PROJECT_NAME}::GlobalHeaders INTERFACE
#    ${CMAKE_CURRENT_SOURCE_DIR}/include
#)

add_subdirectory(core)
add_subdirectory(ui)

