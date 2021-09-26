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

# Requesting more warnings
QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic -Wold-style-cast -Wconversion
QMAKE_CXXFLAGS += -Winit-self -Wunreachable-code
QMAKE_CXXFLAGS += -Werror -Wno-conversion
QMAKE_CXXFLAGS += -Wno-error=deprecated-declarations

# Suppressing warnings in Qt system headers files
QMAKE_CXXFLAGS += -isystem "$$[QT_INSTALL_HEADERS]"
# QMAKE_CXXFLAGS += -isystem "$(QTDIR)/include/QtMultimediaWidgets"

TARGET = gamepad
TEMPLATE = app

#CONFIG -= debug_and_release_target build_all debug_and_release
#CONFIG += object_parallel_to_source
#OBJECTS_DIR=.build/obj
#MOC_DIR=.build/moc
#UI_DIR=.build/uic
#RCC_DIR=.build/rcc

macx {
	CONFIG -= app_bundle
	QMAKE_LFLAGS += -rpath . -rpath @executable_path/../Lib -rpath @executable_path/../Frameworks -rpath @executable_path/../../../
}

include(trikDesktopGamepad.pri)
