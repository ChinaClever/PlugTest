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

}

SnmpThread::~SnmpThread()
{
    wait();
}

void SnmpThread::startRead(const QString &addr)
{

    mIp = addr;
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


void SnmpThread::onRequestFailed( const qint32 request_id )
{
    qDebug() << request_id;
    emit reqErrSig();
}

bool SnmpThread::makeRequestSlot(const QString &oid)
{
    bool ret = true;

    return ret;
}

bool SnmpThread::devDataV3(const QString &id , int &value)
{
    bool ret = false;
    QString ipAddr = mIp;
    QString roCom = "public";
    QString v3PrivPass = "privatekey1";
    QString v3AuthPass = "authkey123";
    QString name = "Busbar";
    int readVersion = 1;

    Snmp::socket_startup();
    char ipaddr[255];
    memset(ipaddr,0,sizeof(char)*255);
    strncpy_s(ipaddr,sizeof(char)*255,ipAddr.toStdString().c_str(),sizeof(char)*255);
    UdpAddress address(ipaddr);
    if ( !address.valid()) {
        Snmp::socket_cleanup();
        return ret;
    }
    QString o = id;
    Oid oid(o.toStdString().c_str());
    if ( !oid.valid()) {
        Snmp::socket_cleanup();
        return ret;
    }

    char com[255];
    memset(com,0,sizeof(char)*255);
    strncpy_s(com,sizeof(char)*255,roCom.toStdString().c_str(),sizeof(char)*255);

    snmp_version version = version2c;
    if(readVersion == 1) version = version1;
    else if(readVersion == 3) version = version3;
    int retries = 1;
    int timeout = 1000;
    u_short port = 161;
    OctetStr community(com);

    int status;
    Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));
    //Snmp snmp(status, 0);

    if ( status != SNMP_CLASS_SUCCESS) {
        Snmp::socket_cleanup();
        return ret;
    }
    Pdu pdu;
    Vb vb;
    vb.set_oid(oid);
    pdu += vb;

    address.set_port(port);
#ifdef _SNMPv3
    memset(com,0,sizeof(char)*255);
    strncpy_s(com,sizeof(char)*255,v3PrivPass.toStdString().c_str(),sizeof(char)*255);
    OctetStr privPassword(com);
    memset(com,0,sizeof(char)*255);
    strncpy_s(com,sizeof(char)*255,v3AuthPass.toStdString().c_str(),sizeof(char)*255);
    OctetStr authPassword(com);
    memset(com,0,sizeof(char)*255);
    strncpy_s(com,sizeof(char)*255,name.toStdString().c_str(),sizeof(char)*255);
    OctetStr securityName(com);
    int securityModel = SNMP_SECURITY_MODEL_USM;
    int securityLevel = SNMP_SECURITY_LEVEL_AUTH_PRIV;
    OctetStr contextName("");
    OctetStr contextEngineID("");
    long authProtocol = SNMP_AUTHPROTOCOL_HMACMD5;
    long privProtocol = SNMP_PRIVPROTOCOL_DES;
    v3MP *v3_MP;
    if (version == version3) {
        const char *engineId = "snmpNext";
        const char *filename = "snmpv3_boot_counter";
        unsigned int snmpEngineBoots = 0;
        int status;
        status = getBootCounter(filename, engineId, snmpEngineBoots);
        if ((status != SNMPv3_OK) && (status < SNMPv3_FILEOPEN_ERROR)){
            qDebug() << "Error loading snmpEngineBoots counter: " << status << endl;
            return ret;
        }
        snmpEngineBoots++;
        status = saveBootCounter(filename, engineId, snmpEngineBoots);
        if (status != SNMPv3_OK){
            qDebug() << "Error saving snmpEngineBoots counter: " << status << endl;
            return ret;
        }

        int construct_status;
        v3_MP = new v3MP(engineId, snmpEngineBoots, construct_status);
        if (construct_status != SNMPv3_MP_OK){
            qDebug() << "Error initializing v3MP: " << construct_status << endl;
            return ret;
        }
        USM *usm = v3_MP->get_usm();
        usm->add_usm_user(securityName,
                          authProtocol, privProtocol,
                          authPassword, privPassword);
    }
    else{
        // MUST create a dummy v3MP object if _SNMPv3 is enabled!
        int construct_status;
        v3_MP = new v3MP("dummy", 0, construct_status);
    }
    snmp.set_mpv3(v3_MP);
#endif
    CTarget ctarget(address);
    UTarget utarget(address);
    if (version == version3) {
        utarget.set_version( version);          // set the SNMP version SNMPV1 or V2 or V3
        utarget.set_retry( retries);            // set the number of auto retries
        utarget.set_timeout( timeout);          // set timeout
        utarget.set_security_model( securityModel);
        utarget.set_security_name( securityName);
        pdu.set_security_level( securityLevel);
        pdu.set_context_name (contextName);
        pdu.set_context_engine_id(contextEngineID);
    }
    else {
        ctarget.set_version(version);
        ctarget.set_retry(retries);
        ctarget.set_timeout(timeout);
        ctarget.set_readcommunity( community);
    }

    SnmpTarget *target;

    if (version == version3){
        target = &utarget;
    }
    else target = &ctarget;
    {
        status = snmp.get(pdu,*target);
        if (status == SNMP_CLASS_SUCCESS){
            for ( int z = 0 ; z < pdu.get_vb_count() ; z++) {
                pdu.get_vb(vb,z);
                Oid tmp;
                vb.get_oid(tmp);
                if (oid.nCompare(oid.len(), tmp) != 0){
                    Snmp::socket_cleanup();
                    return ret;
                }
                if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW){
                    QString id = vb.get_printable_oid();
                    QString id_value = vb.get_printable_value();
                    value = id_value.toUInt();
                    ret = true;
                    //qDebug()<<id  <<"       id_value   " <<id_value;
                }
                else{
                    Snmp::socket_cleanup();
                    return ret;
                }
            }
            pdu.set_vblist(&vb, 1);
        }
    }
    Snmp::socket_cleanup();
    if(v3_MP) {
        delete v3_MP;
        v3_MP = NULL;
    }
    return ret;
}



