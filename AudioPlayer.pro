QT += core gui multimedia multimediawidgets webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about.cpp \
    general.cpp \
    main.cpp \
    mainwindow.cpp \
    musicbrowser.cpp \
    newplaylist.cpp \
    options.cpp \
    radio.cpp \
    tagsedit.cpp

HEADERS += \
    about.h \
    general.h \
    mainwindow.h \
    musicbrowser.h \
    newplaylist.h \
    options.h \
    radio.h \
    tagsedit.h

FORMS += \
    about.ui \
    general.ui \
    mainwindow.ui \
    musicbrowser.ui \
    newplaylist.ui \
    options.ui \
    radio.ui \
    tagsedit.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    C:/Users/olegg/OneDrive/Изображения/Aua(logo night).png

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/release/ -ltag
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/debug/ -ltag
#else:unix: LIBS += -L$$PWD/../../../../usr/lib/ -ltag

#INCLUDEPATH += $$PWD/../../../../usr/include/taglib
#DEPENDPATH += $$PWD/../../../../usr/include/taglib


RESOURCES += \
    Resources.qrc

#INCLUDEPATH += C:/Program Files (x86)/taglib/bin
#LIBS += -L"C:\\Program Files (x86)\\taglib\\bin"
#LIBS += -llibtag
#INCLUDEPATH += 'C:/Program Files (x86)/taglib/include'
#DEPENDPATH += 'C:/Program Files (x86)/taglib/include'

INCLUDEPATH += C:/Program Files (x86)/taglib/lib/x64-v140
LIBS += -L"C:\\Program Files (x86)\\taglib\\lib\\x64-v140"
INCLUDEPATH += C:/Program Files (x86)/taglib/bin/x64-v140
LIBS += -L"C:\\Program Files (x86)\\taglib\\bin\\x64-v140"
LIBS += -ltaglib
INCLUDEPATH += 'C:/Program Files (x86)/taglib/include'
DEPENDPATH += 'C:/Program Files (x86)/taglib/include'

#unix|win32: LIBS += -L$$PWD/../../libs/build-taglib-master-Desktop_Qt_5_15_2_MSVC2019_64bit-Release/taglib/ -ltag

#INCLUDEPATH += 'C:/Program Files (x86)/taglib/include'
#DEPENDPATH += 'C:/Program Files (x86)/taglib/include'

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../libs/build-taglib-master-Desktop_Qt_5_15_2_MSVC2019_64bit-Release/taglib/tag.lib
#else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../libs/build-taglib-master-Desktop_Qt_5_15_2_MSVC2019_64bit-Release/taglib/libtag.a
