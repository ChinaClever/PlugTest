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
    void startThread(int mode);
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
    bool isRun;
    SnmpThread *mSnmp;
    int mMode;
//    SerialPort *mSerial;
};

#endif // PLUGTHREAD_H
