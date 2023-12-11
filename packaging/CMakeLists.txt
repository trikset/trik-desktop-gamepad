set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

# Generate file name in deb format:
# <PackageName>_<VersionNumber>-<DebianRevisionNumber>_<DebianArchitecture>.deb
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY 
  "A remote control for controlling TRIK programmable educational robots")
set(CPACK_PACKAGE_VENDOR "TRIK")

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/${PROJECT_NAME}")

set(CPACK_PACKAGE_CONTACT $ENV{DEBMAIL})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER $ENV{DEBFULLNAME})

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

# Values of variables prefixed with CPACK_ will be escaped before being written to the configuration files,
# so that the cpack program receives them exactly as they were specified
set(CPACK_VERBATIM_VARIABLES YES)

# Generate defined format of packages in case running cpack without arguments
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR DEB)
else()
    set(CPACK_GENERATOR TGZ)
endif()

include(CPack)
