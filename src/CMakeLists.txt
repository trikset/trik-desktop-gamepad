set(SOURCES 
    accelerateStrategy.cpp
    connectForm.cpp
    connectionManager.cpp
    gamepadForm.cpp
    main.cpp
    standardStrategy.cpp
    strategy.cpp
    ../share/fonts.qrc 
    ../share/images.qrc)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    ${QT_LIBRARIES} 
    SingleApplication
)

if(Qt6_FOUND)
    target_compile_definitions(${PROJECT_NAME} PRIVATE TRIK_USE_QT6)
endif()

set_target_properties(${PROJECT_NAME} 
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BIN})
