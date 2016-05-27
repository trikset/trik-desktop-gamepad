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

QT += core gui network widgets
QT += core gui multimedia
QT += multimediawidgets
QT += concurrent widgets

CONFIG += c++11

TARGET = gamepad
TEMPLATE = app  

SOURCES += main.cpp \
	gamepadForm.cpp \
    connectForm.cpp

target.path =
HEADERS += \
	gamepadForm.h \
    connectForm.h

FORMS += \
	gamepadForm.ui \
    connectForm.ui

DISTFILES += \
    GamepadForm.ui.qml

LIBS += -L/usr/local/lib -lSDL2 -ldl -lpthread
INCLUDES += /usr/local/include

RESOURCES += \
    images.qrc
