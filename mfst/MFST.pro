QT -= gui

TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += ../PNN

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Sources/compat.cc \
    Sources/flags.cc \
    Sources/fst-types.cc \
    Sources/fst.cc \
    Sources/mapped-file.cc \
    Sources/mman.cc \
    Sources/properties.cc \
    Sources/symbol-table-ops.cc \
    Sources/symbol-table.cc \
    Sources/util.cc \
    Sources/weight.cc

HEADERS += \
    Sources/mman.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
