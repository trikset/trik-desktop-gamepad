include(GNUInstallDirs)
install(
    DIRECTORY fonts images
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}
)

add_subdirectory(translations)
