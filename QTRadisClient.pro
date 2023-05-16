TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += core

SOURCES += \
        alloc.c \
        async.c \
        client-qt.cpp \
        dict.c \
        hiredis.c \
        main.cpp \
        net.c \
        read.c \
        sds.c \
        sockcompat.c

HEADERS += \
    ThreadPool.h \
    alloc.h \
    async.h \
    async_private.h \
    client-qt.h \
    dict.h \
    fmacros.h \
    hiredis.h \
    net.h \
    qtAdapter.h \
    read.h \
    sds.h \
    sdsalloc.h \
    sockcompat.h \
    win32.h
