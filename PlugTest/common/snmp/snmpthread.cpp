/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "snmpthread.h"
#include <cassert>
#include "configbase.h"

SnmpThread::SnmpThread(QObject *parent) : QThread(parent)
{
    mMutex = new QMutex();
    m_snmp_client =  new QtSnmpClient(this);
    m_snmp_client->setCommunity( "public" );
    connect( m_snmp_client,SIGNAL(responseReceived(qint32,QtSnmpDataList)), this, SLOT(onResponseReceived(qint32,QtSnmpDataList)) );
    connect( m_snmp_client, SIGNAL(requestFailed(qint32)), this, SLOT(onRequestFailed(qint32)) );
}

SnmpThread::~SnmpThread()
{
    wait();
}

void SnmpThread::startRead(const QString &addr)
{
    m_snmp_client->setAgentAddress(QHostAddress(addr));
    //m_snmp_client->cancelWork();
}

double SnmpThread::getValue(int msec)
{
    for(int i=0; i<msec; ++i) {
        if(mValues < 0) msleep(1);
        else break;
    }

    return mValues;
}


void SnmpThread::onResponseReceived(const qint32, const QtSnmpDataList& values )
{
    QMutexLocker locker(mMutex);
//    emit responseSig(values);

    for( const auto& value : values ) {
        //qDebug( "%s | %s : %s\n", qPrintable( "192" ),  qPrintable( value.address() ),  qPrintable( value.data()) );
       // qDebug()<<"onResponseReceived"<<value.data().toDouble();
        mValues = value.data().toDouble();
//        QStringList list = QString(value.address()).split(".");
//        for(int j = 1 ; j < list.size()-2; ++j)
//        {
//            if(j == 9){
//                mSlaveNumSet.insert(list.at(j).toInt());
//                break ;
//            }
//        }
        //qDebug()<<"mSlaveNumSet.size()"<<mSlaveNumSet.size();
    }

}

void SnmpThread::onRequestFailed( const qint32 request_id )
{
    qDebug() << request_id;
    emit reqErrSig();
}

bool SnmpThread::makeRequestSlot(const QString &oid)
{
    mValues = -1;
    bool ret = m_snmp_client->isBusy();
    if(!ret){
        m_snmp_client->requestValue(oid);
    }

    return ret;
}




