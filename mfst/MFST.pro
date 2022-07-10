QT -= gui

TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += ../

CONFIG += c++11

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
