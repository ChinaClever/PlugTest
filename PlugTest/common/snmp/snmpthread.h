#ifndef SNMP_H
#define SNMP_H

#include <QThread>
#include <QScopedPointer>
#include <QMap>
#include <QtSnmpClient.h>
#include <QtCore>

#define MIB_OID_CLEVER  ".1.3.6.1.4.1.30966"


class SnmpThread : public QThread
{
    Q_OBJECT
public:
    explicit SnmpThread(QObject *parent = nullptr);
    ~SnmpThread();

    void startRead(const QString &addr);
    bool makeRequest(const QString &oid);
    int getValue(int msec);

signals:
    void reqErrSig();

private slots:
    void onResponseReceived( const qint32 request_id,const QtSnmpDataList& );
    void onRequestFailed( const qint32 request_id );

private:
    QMutex *mMutex;
    QtSnmpClient *m_snmp_client;
    int mValues;
};

#endif // SNMP_H
