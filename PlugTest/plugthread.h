#ifndef PLUGTHREAD_H
#define PLUGTHREAD_H

#include "snmpthread.h"
#include "serialportdlg.h"
#include "configbase.h"
#include "datapackets.h"
#include "common.h"

class PlugThread : public QThread
{
    Q_OBJECT
public:
    explicit PlugThread(QObject *parent = nullptr);
    ~PlugThread();

//    void initSerial(SerialPort *s) {mSerial=s;}
    void startThread(int mode,int tmode);
    void quitThread();
    sConfigItem *mItem;

signals:
    void requestSig(const QString&);

protected:
    void run();
    void init();
    void delay(int ms);
//    int rtuOpenCmd(int index);
//    int rtuCloseCmd(int index);

    void openFun(int i);
    void closeFun(int i);
    void open2Fun(int i);
    void close2Fun(int i);
private:
    // 检查开关是否处于“开”状态
    bool isSwitchOpen(const QString& switchStateStr, const QString& targetValue);
    bool isSwitchClosed(const QString& switchStateStr, const QString& targetValue);
private:
    bool isRun;
    SnmpThread *mSnmp;
    int mMode;
    int tMode;
//    SerialPort *mSerial;
};

#endif // PLUGTHREAD_H
