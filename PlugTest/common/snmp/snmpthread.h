#ifndef SNMP_H
#define SNMP_H

#include <QThread>
#include <QScopedPointer>
#include <QMap>
#include <QtSnmpClient.h>
#include <QtCore>


class SnmpThread : public QThread
{
    Q_OBJECT
public:
    explicit SnmpThread(QObject *parent = nullptr);
    ~SnmpThread();

    void startRead(const QString &addr);
    //bool makeRequest(const QString &oid);
    double getValue(int msec);

signals:
    void reqErrSig();
public slots:
    bool makeRequestSlot(const QString &oid);

private slots:
    void onResponseReceived( const qint32 request_id,const QtSnmpDataList& );
    void onRequestFailed( const qint32 request_id );

private:
    QMutex *mMutex;
    QtSnmpClient *m_snmp_client;
    double mValues;
};

#endif // SNMP_H
