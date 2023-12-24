set(QT_COMPONENTS 
    Core 
    Gui 
    Network
    Widgets 
    Multimedia 
    MultimediaWidgets 
    LinguistTools)

#find_package(Qt6 COMPONENTS ${QT_COMPONENTS})
#if (NOT Qt6_FOUND)
#    find_package(Qt5 5.15 REQUIRED ${QT_COMPONENTS})
#endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS})

message("FOUND QT (default): ${QT_DEFAULT_MAJOR_VERSION}")
message("FOUND QT: ${QT_VERSION_MAJOR}")

set(QT_LIBRARIES 
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Gui 
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets)
