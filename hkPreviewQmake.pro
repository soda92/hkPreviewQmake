QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = HKDemo
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

DEFINES += QT_DEPRECATED_WARNINGS

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#LIBS +=-LD:/src/hkPreviewQmake/lib -lHCCore -lHCNetSDK -lPlayCtrl
LIBS +=-LD:/src/hkPreviewQmake/lib -lHCNetSDK -lPlayCtrl
#LIBS +=-LD:/src/hkPreviewQmake/lib/HCNetSDKCom -lHCAlarm -lHCGeneralCfgMgr -lHCPreview
