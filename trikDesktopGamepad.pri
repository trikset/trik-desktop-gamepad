QT += core gui network widgets multimedia multimediawidgets

CONFIG += c++11

TARGET = gamepad
TEMPLATE = app

SOURCES += \
	$$PWD/main.cpp \
	$$PWD/gamepadForm.cpp \
	$$PWD/connectForm.cpp \
	$$PWD/connectionManager.cpp \
	$$PWD/standardStrategy.cpp \
	$$PWD/accelerateStrategy.cpp \
	$$PWD/strategy.cpp

TRANSLATIONS += \
	$$PWD/languages/trikDesktopGamepad_ru.ts \
	$$PWD/languages/trikDesktopGamepad_en.ts \
	$$PWD/languages/trikDesktopGamepad_fr.ts \
	$$PWD/languages/trikDesktopGamepad_de.ts

HEADERS += \
	$$PWD/gamepadForm.h \
	$$PWD/connectForm.h \
	$$PWD/connectionManager.h \
	$$PWD/standardStrategy.h \
	$$PWD/accelerateStrategy.h \
	$$PWD/strategy.h

FORMS += \
	$$PWD/gamepadForm.ui \
	$$PWD/connectForm.ui

RESOURCES += \
	$$PWD/images.qrc \
	$$PWD/fonts.qrc

include(thirdparty/singleApplication.pro)
