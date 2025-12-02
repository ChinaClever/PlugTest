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

void PlugThread::startThread(int mode,int tmode)
{
    if(!isRun) {
        mMode = mode;
        tMode = tmode;
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
    for(int i = 0 ; i < ARRAY_SIZE ; i++){
        DataPackets::bulid()->get(i)->en = false;
    }
    isRun = false;
}

bool PlugThread::isSwitchOpen(const QString& switchStateStr, const QString& targetValue)
{
    // targetValue 应该已经根据 tMode 设置为 "1" 或 "ON"

    if (tMode == 0) {
        // 模式0 (0/1): 精确匹配 "1"
        return (switchStateStr.trimmed() == "1");
    } else {
        // 模式1 (ON/OFF): 不区分大小写，判断是否包含
        return switchStateStr.trimmed().contains(targetValue, Qt::CaseInsensitive);
    }
}

bool PlugThread::isSwitchClosed(const QString& switchStateStr, const QString& targetValue)
{
    // targetValue 应该是 "0" 或 "OFF"
    if (tMode == 0) {
        // 模式0 (0/1): 精确匹配 "0"
        return (switchStateStr.trimmed() == "0");
    } else {
        // 模式1 (ON/OFF): 不区分大小写，包含 "OFF"
        return switchStateStr.trimmed().contains(targetValue, Qt::CaseInsensitive);
    }
}

void PlugThread::openFun(int i)
{
    sDataPacket *packet = DataPackets::bulid()->get(i);
    packet->open++;
    packet->action = 1;
    QString str ,str1;
    // int value = 1;
    // mSnmp->setInfo(mItem->swIp , mItem->oids[i] , QString::number(value));//开关

    QString targetValue = (tMode == 0) ? "1" : "ON";
    mSnmp->setInfo(mItem->swIp , mItem->oids[i] , targetValue);

    bool successBreak = false;

    for(int k=0; k<5; k++) {
        delay(mItem->delay);
        mSnmp->devDataV3(mItem->testIp , mItem->readVolOids[i] , str);//read
        mSnmp->devDataV3(mItem->swIp , mItem->oids[i] , str1);//read
        packet->value = str.toInt();

        bool isSwitchOn = isSwitchOpen(str1, targetValue);

        // 最终判断条件：电压大于200 且 开关状态正确
        if(str.toInt() > 200 && isSwitchOn) {
            successBreak = true;
            break;
        }
    }
    if(successBreak) {
        packet->ok++;
    } else {
        mSnmp->setInfo(mItem->swIp ,mItem->writeCrMinVolOids[0] , "2400");
        mSnmp->setInfo(mItem->swIp ,mItem->writeMinVolOids[0] , "2400");
        packet->err++;
    }
}

void PlugThread::closeFun(int i)
{
    sDataPacket *packet = DataPackets::bulid()->get(i);
    packet->close++;
    packet->action = 0;
    QString str1;

    // 确定目标值 ("0" 或 "OFF")
    QString targetValue = (tMode == 0) ? "0" : "OFF";

    // 发送关闭指令
    mSnmp->setInfo(mItem->swIp ,mItem->oids[i] , targetValue);

    bool successBreak = false;

    for(int k=0; k<5; k++) {
        delay(mItem->delay);
        mSnmp->devDataV3(mItem->swIp ,mItem->oids[i] , str1);//read

        // 调用辅助函数判断开关是否关闭
        bool isSwitchOff = isSwitchClosed(str1, targetValue);

        if(isSwitchOff) {
            successBreak = true;
            break;
        }
    }

    // 循环结束后，直接使用 successBreak 标志进行最终判断
    if(successBreak) {
        packet->ok++;
    } else {
        // 错误处理逻辑保持不变
        mSnmp->setInfo(mItem->swIp ,mItem->writeCrMinVolOids[0] , "2400");
        mSnmp->setInfo(mItem->swIp ,mItem->writeMinVolOids[0] , "2400");
        packet->err++;
    }
}


void PlugThread::open2Fun(int i)
{
    sDataPacket *packet = DataPackets::bulid()->get(i);
    packet->open++;
    packet->action = 1;
    QString str ,str1;

    QString targetValue = (tMode == 0) ? "1" : "ON";
    mSnmp->setInfo(mItem->testIp , mItem->oids[i] , targetValue);

    bool successBreak = false;

    for(int k=0; k<5; k++) {
        delay(mItem->delay);
        mSnmp->devDataV3(mItem->testIp , mItem->readCurOids[i] , str);
        mSnmp->devDataV3(mItem->testIp , mItem->oids[i] , str1);
        packet->value = str.toInt();

        bool isSwitchOn = isSwitchOpen(str1, targetValue);

        if(str.toInt() > 10 && isSwitchOn) {
            successBreak = true;
            break;
        }
    }

    if(successBreak) {
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
    int value = 0;
    mSnmp->setInfo(mItem->testIp , mItem->oids[i] , QString::number(value));//开关
    for(int k=0; k<5; k++) {
        delay(mItem->delay);
        mSnmp->devDataV3(mItem->testIp , mItem->readCurOids[i] , str);//read cur
        mSnmp->devDataV3(mItem->testIp , mItem->oids[i] , str1);//read switch
        packet->value = str.toInt();
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
    for(int i = 0 ; i < ARRAY_SIZE ; i++){
        DataPackets::bulid()->get(i)->en = true;
    }
    isRun = true;

    while(isRun) {
        if(!isRun) break;

        if(0 == mMode){
            delay(mItem->delayClose);
            closeFun(0);

            if(!isRun) break;

            delay(mItem->delayOpen);
            openFun(0);
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
