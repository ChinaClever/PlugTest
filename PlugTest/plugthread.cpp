#include "plugthread.h"

PlugThread::PlugThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    mSerial = nullptr;
    mSnmp = new SnmpThread(this);
    mItem = ConfigBase::bulid()->item;
    connect(this , SIGNAL(requestSig(const QString&)),mSnmp,SLOT(makeRequestSlot(const QString&)));
}

PlugThread::~PlugThread()
{
    isRun = false;
    wait();
}

void PlugThread::startThread()
{
    if(!isRun) {
        if(!mItem->ip.isEmpty()){
            mSnmp->startRead(mItem->ip);
        }
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
    packet->all++;
    packet->action = 1;

    for(int k=0; k<5; k++) {
        delay(1);
        int value = 0;
        mSnmp->devDataV3(mItem->oids[i] , value);
        packet->value = value;
        if(packet->value>0)  break;
    }

    if(packet->value>0) {
        packet->ok++;
    } else {
        packet->err++;
    }
}

void PlugThread::closeFun(int i)
{
    sDataPacket *packet = DataPackets::bulid()->get(i);
    packet->all++;
    packet->action = 0;

    for(int k=0; k<5; k++) {
        delay(1);
        emit requestSig(mItem->oids[i]);
        packet->value = mSnmp->getValue(1000);
        if(packet->value==0)  break;
    }

    if(packet->value==0) {
        packet->ok++;
    } else {
        packet->err++;
    }
}

int PlugThread::rtuOpenCmd(int index)
{
    QByteArray array = cm_StringToHex(mItem->openCmd[index]);
    int ret = mSerial->write(array);
    if(ret <= 0) {
        qDebug() << "PlugThread rtuOpenCmd err";
    } else {
        delay(3);
    }

    return ret;
}

int PlugThread::rtuCloseCmd(int index)
{
    QByteArray array = cm_StringToHex(mItem->closeCmd[index]);
    int ret = mSerial->write(array);
    if(ret <= 0) {
        qDebug() << "PlugThread rtuCloseCmd err";
    } else {
        delay(3);
    }

    return ret;
}

void PlugThread::delay(int ms)
{
    if(ms <=0 ) ms = 1;
    for(int i=0; i<ms; ++i) {
        if(isRun) sleep(1);
        else break;
    }
}

void PlugThread::init()
{
    DataPackets::bulid()->clears();
    for(int i=0; i<SNMP_SIZE; ++i) {
        sDataPacket *packet = DataPackets::bulid()->get(i);
        if(mItem->snmpEn[i]) {
            packet->en = 1;
        } else {
            packet->en = 0;
        }
    }

    for(int k=0;k<RTUCMD_SIZE;k++)rtuCloseCmd(k); sleep(2);
}

void PlugThread::run()
{
    init();
    isRun = true;

    while(isRun) {
        if(!isRun) break;
        for(int k=0 ; k<RTUCMD_SIZE ; ++k)
        {
            if(mItem->rtuCmdEn[k]){
                rtuOpenCmd(k);
                delay(mItem->delay);
                for(int i=0; i<SNMP_SIZE; ++i) {
                    if(mItem->snmpEn[i]) openFun(i);
                }
                //delay(mItem->delay);

                if(!isRun) break;
                rtuCloseCmd(k);
                delay(mItem->delay);
                for(int i=0; i<SNMP_SIZE; ++i) {
                    if(mItem->snmpEn[i]) closeFun(i);
                }
                //delay(mItem->delay);
            }
        }
    }
    for(int k=0 ; k<RTUCMD_SIZE ; ++k)
        rtuCloseCmd(k);

}
