QT -= core gui
#QT -= gui

TEMPLATE = lib

SOURCES += \
    Sources/main.cpp \
    Sources/hook.cpp \
#    Sources/mm_api.cpp \
#    Sources/mm_win32.cpp

HEADERS += \
    Sources/hook.h \
#    Sources/mm_api.h \
#    Sources/mm_win32.h

win32:LIBS += -lKernel32 -lUser32 -lole32 \
             -luuid -loleaut32 -loleacc \
             -lDwmapi -lPsapi
