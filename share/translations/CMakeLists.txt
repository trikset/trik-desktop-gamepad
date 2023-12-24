set(TS_FILES 
    trikDesktopGamepad_de.ts
    trikDesktopGamepad_en.ts
    trikDesktopGamepad_ru.ts
    trikDesktopGamepad_fr.ts)

if(Qt6_FOUND)
    qt_add_translations(${PROJECT_NAME} TS_FILES ${TS_FILES})
else()
    qt5_add_translation(QM_FILES ${TS_FILES})
    include(TranslationUtils)
    add_translations_resource(QRC_FILE ${QM_FILES})
    add_custom_target(create_qm ALL DEPENDS ${QM_FILES})
    add_custom_target(create_qrc ALL DEPENDS ${QRC_FILE})
    add_dependencies(create_qrc create_qm)
    add_dependencies(${PROJECT_NAME} create_qrc)
    target_sources(${PROJECT_NAME} PRIVATE ${QRC_FILE})
endif()

install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/translations)
