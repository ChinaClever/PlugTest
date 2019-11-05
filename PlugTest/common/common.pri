
INCLUDEPATH += $$PWD

include(snmp/snmp.pri)
include(cfgcom/cfgcom.pri)
include(msgcom/msgcom.pri)
include(tabcom/tabcom.pri)
include(serialport/serialport.pri)
include(datapacket/datapacket.pri)
include(backcolour/backcolour.pri)

HEADERS += \	
    $$PWD/common.h
	
SOURCES += \	
    $$PWD/common.cpp

FORMS += \




