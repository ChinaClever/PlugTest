#include "plugthread.h"

PlugThread::PlugThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    mSerial = nullptr;
    mSnmp = new SnmpThread(this);
    mItem = ConfigBase::bulid()->item;
}

PlugThread::~PlugThread()
{
    isRun = false;
    wait();
}

void PlugThread::startThread()
{
    if(!isRun) {
        mSnmp->startRead(mItem->ip);
        start();
    }
}

void PlugThread::quitThread()
{
    isRun = false;
}

void PlugThread::openFun(int i)
{
    sDataPacket *packet = DataPackets::bulid()->get(i);
    packet->en = 1;
    packet->all++;
    packet->action = 1;
    mSnmp->makeRequest(mItem->oids[i]);
    packet->value = mSnmp->getValue(650);
    if(packet->value > 0) {
        packet->ok++;
    } else {
        packet->err++;
    }
}

void PlugThread::closeFun(int i)
{
    sDataPacket *packet = DataPackets::bulid()->get(i);
    packet->en = 1;
    packet->all++;
    packet->action = 1;
    mSnmp->makeRequest(mItem->oids[i]);
    packet->value = mSnmp->getValue(650);
    if(packet->value == 0) {
        packet->ok++;
    } else {
        packet->err++;
    }
}

int PlugThread::rtuOpenCmd()
{
    QByteArray array = cm_StringToHex(mItem->openCmd);
    int ret = mSerial->write(array);
    if(ret <= 0) {
        qDebug() << "PlugThread rtuOpenCmd err";
    }

    return ret;
}

int PlugThread::rtuCloseCmd()
{
    QByteArray array = cm_StringToHex(mItem->closeCmd);
    int ret = mSerial->write(array);
    if(ret <= 0) {
        qDebug() << "PlugThread rtuCloseCmd err";
    }
     return ret;
}


void PlugThread::run()
{
    isRun = true;
    while(isRun) {
        rtuOpenCmd();
        sleep(mItem->delay/2);
        for(int i=0; i<SNMP_SIZE; ++i) {
            if(mItem->snmpEn[i]) openFun(i);
        }

        rtuCloseCmd();
        sleep(mItem->delay/2);
        for(int i=0; i<SNMP_SIZE; ++i) {
            if(mItem->snmpEn[i]) closeFun(i);
        }

    }
}
