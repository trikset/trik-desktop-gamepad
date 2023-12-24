QT += core gui network widgets multimedia multimediawidgets
CONFIG += c++14
DEFINES += QT_NO_ACCESSIBILITY

SOURCES += \
	$$PWD/src/main.cpp \
	$$PWD/src/gamepadForm.cpp \
	$$PWD/src/connectForm.cpp \
	$$PWD/src/connectionManager.cpp \
	$$PWD/src/standardStrategy.cpp \
	$$PWD/src/accelerateStrategy.cpp \
	$$PWD/src/strategy.cpp

TRANSLATIONS += \
	$$PWD/share/translations/trikDesktopGamepad_ru.ts \
	$$PWD/share/translations/trikDesktopGamepad_en.ts \
	$$PWD/share/translations/trikDesktopGamepad_fr.ts \
	$$PWD/share/translations/trikDesktopGamepad_de.ts

HEADERS += \
	$$PWD/src/gamepadForm.h \
	$$PWD/src/connectForm.h \
	$$PWD/src/connectionManager.h \
	$$PWD/src/standardStrategy.h \
	$$PWD/src/accelerateStrategy.h \
	$$PWD/src/strategy.h

FORMS += \
	$$PWD/src/gamepadForm.ui \
	$$PWD/src/connectForm.ui

RESOURCES += \
	$$PWD/share/images.qrc \
	$$PWD/share/fonts.qrc

# Single Application implementation
include(dependencies/singleapplication.pro)
