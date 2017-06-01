#------------------------------------------------- 
# 
# Project created by QtCreator 2017-05-22T00:35:05 
# 
#------------------------------------------------- 
 
QT       += core gui 
 
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets 
 
TARGET = mappinggrid 
TEMPLATE = app 
 
# The following define makes your compiler emit warnings if you use 
# any feature of Qt which as been marked as deprecated (the exact warnings 
# depend on your compiler). Please consult the documentation of the 
# deprecated API in order to know how to port your code away from it. 
DEFINES += QT_DEPRECATED_WARNINGS 
 
# You can also make your code fail to compile if you use deprecated APIs. 
# In order to do so, uncomment the following line. 
# You can also select to disable deprecated APIs only up to a certain version of Qt. 
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0 
 
 
SOURCES += main.cpp \ 
    scenegrid.cpp \ 
    pid.cpp \ 
    occupancygrid.cpp \ 
    keyboard.cpp \ 
    controller.cpp \ 
    bot.cpp \ 
    util.cpp \
    bayesian.cpp \
    histogramic.cpp \
    potentialfield.cpp
 
HEADERS  += \ 
    scenegrid.h \ 
    pid.h \ 
    occupancygrid.h \ 
    keyboard.h \ 
    controller.h \ 
    bot.h \ 
    util.h \
    bayesian.h \
    histogramic.h \
    potentialfield.h
 
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/Aria/lib/release/ -lAria 
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/Aria/lib/debug/ -lAria 
else:unix: LIBS += -L$$PWD/../../../../../../usr/local/Aria/lib/ -lAria 
 
INCLUDEPATH += $$PWD/../../../../../../usr/local/Aria/include 
DEPENDPATH += $$PWD/../../../../../../usr/local/Aria/include 
 
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/Aria/lib/release/ -lArNetworking 
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/Aria/lib/debug/ -lArNetworking 
else:unix: LIBS += -L$$PWD/../../../../../../usr/local/Aria/lib/ -lArNetworking 
 
INCLUDEPATH += $$PWD/../../../../../../usr/local/Aria/ArNetworking/include 
DEPENDPATH += $$PWD/../../../../../../usr/local/Aria/ArNetworking/include 
