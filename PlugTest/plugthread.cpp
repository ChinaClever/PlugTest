#include "plugthread.h"

PlugThread::PlugThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    mSnmp = new SnmpThread(this);
    mItem = ConfigBase::bulid()->item;
    connect(this , SIGNAL(requestSig(const QString&)),mSnmp,SLOT(makeRequestSlot(const QString&)));
}

PlugThread::~PlugThread()
{
    isRun = false;
    wait();
}

void PlugThread::startThread(int mode)
{
    if(!isRun) {
        mMode = mode;
        if(0 == mode){
            if(!mItem->swIp.isEmpty() && !mItem->testIp.isEmpty()){
                mSnmp->startRead(mode);
            }
        }else{
            if(!mItem->testIp.isEmpty()){
                mSnmp->startRead(mode);
            }
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
    packet->open++;
    packet->action = 1;
    QString str ,str1;

    for(int k=0; k<5; k++) {
        int value = 1;
        mSnmp->setInfo(mItem->swIp , mItem->oids[i] , QString::number(value));//开关

        mSnmp->devDataV3(mItem->testIp , mItem->readVolOids[i] , str);//read
        mSnmp->devDataV3(mItem->swIp , mItem->oids[i] , str1);//read

        if(str.toInt() > 200 && str1.toInt() == 1)  break;
    }

    if(str.toInt() > 200 && str1.toInt() == 1) {
        packet->ok++;
    } else {
        mSnmp->setInfo(mItem->swIp ,mItem->writeCrMinVolOids[0] , "2400");//开关
        mSnmp->setInfo(mItem->swIp ,mItem->writeMinVolOids[0] , "2400");//开关
        packet->err++;
    }
}

void PlugThread::closeFun(int i)
{
    sDataPacket *packet = DataPackets::bulid()->get(i);
    packet->close++;
    packet->action = 0;
    QString str1;

    for(int k=0; k<5; k++) {
        int value = 0;
        mSnmp->setInfo(mItem->swIp ,mItem->oids[i] , QString::number(value));//开关
        mSnmp->devDataV3(mItem->swIp ,mItem->oids[i] , str1);//read
        if(str1.toInt() == 0)  break;
    }
    if(str1.toInt() == 0) {
        packet->ok++;
    } else {
        mSnmp->setInfo(mItem->swIp ,mItem->writeCrMinVolOids[0] , "2400");//开关
        mSnmp->setInfo(mItem->swIp ,mItem->writeMinVolOids[0] , "2400");//开关
        packet->err++;
    }
}


void PlugThread::open2Fun(int i)
{
    sDataPacket *packet = DataPackets::bulid()->get(i);
    packet->open++;
    packet->action = 1;
    QString str ,str1;

    for(int k=0; k<5; k++) {
        int value = 1;
        mSnmp->setInfo(mItem->testIp , mItem->oids[i] , QString::number(value));//开关

        mSnmp->devDataV3(mItem->testIp , mItem->readCurOids[i] , str);//read cur
        mSnmp->devDataV3(mItem->testIp , mItem->oids[i] , str1);//read swtich

        if(str.toInt() > 10 && str1.toInt() == 1)  break;
    }

    if(str.toInt() > 10 && str1.toInt() == 1) {
        packet->ok++;
    } else {
        packet->err++;
    }
}

void PlugThread::close2Fun(int i)
{
    sDataPacket *packet = DataPackets::bulid()->get(i);
    packet->close++;
    packet->action = 0;
    QString str ,str1;

    for(int k=0; k<5; k++) {
        int value = 0;
        mSnmp->setInfo(mItem->testIp , mItem->oids[i] , QString::number(value));//开关
        mSnmp->devDataV3(mItem->testIp , mItem->readCurOids[i] , str);//read cur
        mSnmp->devDataV3(mItem->testIp , mItem->oids[i] , str1);//read switch

        if(str.toInt() == 0 && str1.toInt() == 0)  break;
    }

    if(str.toInt() == 0 && str1.toInt() == 0) {
        packet->ok++;
    } else {
        packet->err++;
    }
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
}

void PlugThread::run()
{
    init();
    isRun = true;

    while(isRun) {
        if(!isRun) break;

        if(0 == mMode){
            delay(mItem->delayOpen);
            openFun(0);

            if(!isRun) break;
            delay(mItem->delayClose);
            closeFun(0);
        }else{
            int num = mItem->bitNum;
            delay(mItem->delayClose);
            for(int i = 0 ; i < num ; i++){
                close2Fun(i);
            }
            delay(mItem->delayOpen);
            for(int i = 0 ; i < num ; i++){
                open2Fun(i);
            }
        }
    }

}
