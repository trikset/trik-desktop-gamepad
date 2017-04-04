# Copyright 2016 CyberTech Labs Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# This file was modified by Yurii Litvinov and Mikhail Wall to make it comply with the requirements of trikRuntime
# project. See git revision history for detailed changes.


QMAKE_CXXFLAGS += -Wall -Wextra -pedantic -Wold-style-cast -Weffc++ #-Wconversion
QMAKE_CXXFLAGS += -Winit-self -Wunreachable-code

#message ("$(QTDIR)")

# these flags didn't work properly
#QMAKE_CXXFLAGS += -isystem "$(QTDIR)/include/QtMultimedia/"
#QMAKE_CXXFLAGS += -isystem "$(QTDIR)/include/QtCore"
#QMAKE_CXXFLAGS += -isystem "$(QTDIR)/include/QtGui/"
#QMAKE_CXXFLAGS += -isystem "$(QTDIR)/include/QtWidgets/"
#QMAKE_CXXFLAGS += -isystem "$(QTDIR)/include/QtMultimediaWidgets/"
#QMAKE_CXXFLAGS += -isystem "$(QTDIR)/include/QtNetwork/"
#QMAKE_CXXFLAGS += -isystem "$(QTDIR)/include//QtXml/"


QT += core gui network widgets multimedia multimediawidgets

CONFIG += c++11

TARGET = gamepad
TEMPLATE = app  

SOURCES += main.cpp \
        gamepadForm.cpp \
        connectForm.cpp \
        connectionManager.cpp

TRANSLATIONS += languages/trikDesktopGamepad_ru.ts \
                languages/trikDesktopGamepad_en.ts \
                languages/trikDesktopGamepad_fr.ts \
                languages/trikDesktopGamepad_de.ts

target.path =
HEADERS += \
        gamepadForm.h \
        connectForm.h \
        connectionManager.h

FORMS += \
        gamepadForm.ui \
        connectForm.ui

RESOURCES += \
        images.qrc \
        fonts.qrc

