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
    explicit PlugThread(QThread *parent = nullptr);
    ~PlugThread();

    void startThread();
    void quitThread();

protected:
    void run();
    int rtuOpenCmd();
    int rtuCloseCmd();

    void openFun(int i);
    void closeFun(int i);

private:
    bool isRun;
    SnmpThread *mSnmp;
    SerialPort *mSerial;
    sConfigItem *mItem;
};

#endif // PLUGTHREAD_H
