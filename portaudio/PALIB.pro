CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += ../

DEFINES += PA_USE_WMME
LIBS += -lwinmm -lole32 -luuid

CONFIG += c++11

HEADERS += \
    Sources/common/pa_allocation.h \
    Sources/common/pa_converters.h \
    Sources/common/pa_cpuload.h \
    Sources/common/pa_debugprint.h \
    Sources/common/pa_dither.h \
    Sources/common/pa_endianness.h \
    Sources/common/pa_gitrevision.h \
    Sources/common/pa_hostapi.h \
    Sources/common/pa_memorybarrier.h \
    Sources/common/pa_process.h \
    Sources/common/pa_ringbuffer.h \
    Sources/common/pa_stream.h \
    Sources/common/pa_trace.h \
    Sources/common/pa_types.h \
    Sources/common/pa_util.h \
    Sources/os/win/pa_win_coinitialize.h

SOURCES += \
    Sources/common/pa_allocation.c \
    Sources/common/pa_converters.c \
    Sources/common/pa_cpuload.c \
    Sources/common/pa_debugprint.c \
    Sources/common/pa_dither.c \
    Sources/common/pa_front.c \
    Sources/common/pa_process.c \
    Sources/common/pa_ringbuffer.c \
    Sources/common/pa_stream.c \
    Sources/common/pa_trace.c \
    Sources/hostapi/wmme/pa_win_wmme.c \
    Sources/os/win/pa_win_coinitialize.c \
    Sources/os/win/pa_win_hostapis.c \
    Sources/os/win/pa_win_util.c \
    Sources/os/win/pa_win_waveformat.c
