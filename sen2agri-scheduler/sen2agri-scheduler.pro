include(../common.pri)

QT -= gui
QT += core dbus

DESTDIR = bin

CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += ../Optional

TEMPLATE = app

TARGET = sen2agri-scheduler

SOURCES += main.cpp \
    taskloader.cpp \
    schedulerapp.cpp \
    taskplanner.cpp \
    ochestratorproxy.cpp \
    resourcereader.cpp \
    runestimator.cpp

#adaptor.files = ../dbus-interfaces/org.esa.sen2agri.orchestrator.xml
#adaptor.header_flags = -i ../sen2agri-common/model.hpp

#DBUS_ADAPTORS += adaptor

orchestrator_interface.files = ../dbus-interfaces/org.esa.sen2agri.orchestrator.xml
orchestrator_interface.header_flags = -i ../sen2agri-common/model.hpp

DBUS_INTERFACES += orchestrator_interface

#DISTFILES += \
#    ../dbus-interfaces/org.esa.sen2agri.orchestrator.xml \
#    dist/org.esa.sen2agri.orchestrator.conf \
#    dist/org.esa.sen2agri.orchestrator.service \
#    dist/sen2agri-orchestrator.service

LIBS += -L$$OUT_PWD/../sen2agri-common/ -lsen2agri-common

INCLUDEPATH += $$PWD/../sen2agri-common
DEPENDPATH += $$PWD/../sen2agri-common

PRE_TARGETDEPS += $$OUT_PWD/../sen2agri-common/libsen2agri-common.a

target.path = /usr/bin

#interface.path = /usr/share/dbus-1/interfaces
#interface.files = ../dbus-interfaces/org.esa.sen2agri.orchestrator.xml

#dbus-policy.path = /etc/dbus-1/system.d
#dbus-policy.files = dist/org.esa.sen2agri.orchestrator.conf

#dbus-service.path = /usr/share/dbus-1/system-services
#dbus-service.files = dist/org.esa.sen2agri.orchestrator.service

#systemd-service.path = /usr/lib/systemd/system
#systemd-service.files = dist/sen2agri-orchestrator.service

#INSTALLS += target interface dbus-policy dbus-service systemd-service

HEADERS += \
    pch.hpp \
    scheduledtask.hpp \
    taskloader.hpp \
    schedulerapp.hpp \
    taskplanner.hpp \
    resourcereader.hpp \
    ochestratorproxy.hpp \
    runestimator.hpp