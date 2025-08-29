#ifndef SNMP_H
#define SNMP_H

#include <QThread>
#include <QScopedPointer>
#include <QMap>
#include <QtCore>
#include "snmplib/snmp_pp/snmp_pp.h"
using namespace Snmp_pp;

class SnmpThread : public QThread
{
    Q_OBJECT
public:
    explicit SnmpThread(QObject *parent = nullptr);
    ~SnmpThread();

    void startRead(const QString &addr);
    //bool makeRequest(const QString &oid);
    double getValue(int msec);
    bool devDataV3(const QString &id, int &value);

signals:
    void reqErrSig();
public slots:
    bool makeRequestSlot(const QString &oid);

private slots:
    void onRequestFailed( const qint32 request_id );

private:
    QMutex *mMutex;
    double mValues;
    QString mIp;
};

#endif // SNMP_H
