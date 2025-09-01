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

void SnmpThread::startRead(int mode)
{
    mMode = mode;
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

bool SnmpThread::devDataV3(const QString &ip ,const QString &id , QString &value)
{
    bool ret = false;
    if(id.isEmpty()) return ret;
    QString ipAddr = ip;
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
                    value = id_value;
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

// determine the smi type and get a value from the user
bool SnmpThread::determine_vb(SmiUINT32 val, Vb &vb, QString str)
{
    char buffer[255];
    if (val == sNMP_SYNTAX_NOSUCHINSTANCE) {
        vb.set_syntax(val);
    }

    switch (val) {
    // octet string
    case sNMP_SYNTAX_OCTETS:
    {
        qDebug() << "Octet String\n";
        qDebug() << "Please enter new value: ";
        //cin >> buffer;
        memset(buffer,0,sizeof(char)*255);
        strncpy_s(buffer,sizeof(char)*255,str.toStdString().c_str(),sizeof(char)*250);
        OctetStr octetstr(buffer);
        if (octetstr.valid()) {
            vb.set_value(octetstr);
            return true;
        }
        else {
            qDebug() << "Invalid OctetStr\n";
            return false;
        }
    }
        // IP Address
    case sNMP_SYNTAX_IPADDR:
    {
        qDebug() << "IP Address\n";
        qDebug() << "Please enter new value: ";
        //cin >> buffer;
        IpAddress ipaddress(buffer);
        if (ipaddress.valid()) {
            vb.set_value(ipaddress);
            return true;
        }
        else {
            qDebug() << "Invalid IP Address\n";
            return false;
        }
    }
        // Oid
    case sNMP_SYNTAX_OID:
    {
        qDebug() << "Oid\n";
        qDebug() << "Please enter new value: ";
        //cin >> buffer;
        Oid oid(buffer);
        if (oid.valid()) {
            vb.set_value(oid);
            return true;
        }
        else {
            qDebug() << "Invalid Oid\n";
            return false;
        }
    }
        // TimeTicks
    case sNMP_SYNTAX_TIMETICKS:
    {
        qDebug() << "TimeTicks\n";
        qDebug() << "Please enter new value: ";
        //cin >> buffer;
        unsigned long i;
        i = atol(buffer);
        TimeTicks timeticks(i);
        if (timeticks.valid()) {
            vb.set_value(timeticks);
            return true;
        }
        else {
            qDebug() << "Invalid TimeTicks\n";
            return false;
        }
    }
        // Gauge32
    case sNMP_SYNTAX_GAUGE32:
    {
        qDebug() << "Gauge32\n";
        qDebug() << "Please enter new value: ";
        //cin >> buffer;
        unsigned long i;
        i = atol(buffer);
        Gauge32 gauge32(i);
        if (gauge32.valid()) {
            vb.set_value(gauge32);
            return true;
        }
        else {
            qDebug() << "Invalid Gauge32\n";
            return false;
        }
    }
    case sNMP_SYNTAX_CNTR32:
    {
        qDebug() << "Counter32\n";
        qDebug() << "Please enter new value: ";
        //cin >> buffer;
        unsigned long i;
        i = atol(buffer);
        Counter32 counter32(i);
        if (counter32.valid()) {
            vb.set_value(counter32);
            return true;
        }
        else {
            qDebug() << "Invalid Counter32\n";
            return false;
        }
    }
    case sNMP_SYNTAX_CNTR64:
    {
        qDebug() << "Counter64\n";
        qDebug() << "Please enter value (low 32 bit): ";
        //cin >> buffer;
        unsigned long i;
        i = atol(buffer);
        Counter64 counter64;
        counter64.set_low(i);

        qDebug() << "Please enter value (high 32 bit): ";
        //cin >> buffer;
        i = atol(buffer);
        counter64.set_high(i);

        if (counter64.valid()) {
            vb.set_value(counter64);
            return true;
        }
        else {
            qDebug() << "Invalid Counter64\n";
            return false;
        }
    }
    case sNMP_SYNTAX_INT:
    {
        qDebug() << "Integer\n";
        qDebug() << "Please enter new value: ";
        //cin >> buffer;
        memset(buffer,0,sizeof(char)*255);
        strncpy_s(buffer,sizeof(char)*255,str.toStdString().c_str(),sizeof(char)*255);
        unsigned long i;
        i = atol(buffer);
        long l ;
        l = (long) i;
        vb.set_value(l);
        return true;
    }
    case sNMP_SYNTAX_NOSUCHOBJECT:
    {
        //        cout << "NO SUCH OBJECT\n";
        //        cout << "Object cannot be created.\n";
        //        return false;
        qDebug() << "Integer\n";
        qDebug() << "Please enter new value: ";
        //cin >> buffer;
        memset(buffer,0,sizeof(char)*255);
        strncpy_s(buffer,sizeof(char)*255,str.toStdString().c_str(),sizeof(char)*255);
        unsigned long i;
        i = atol(buffer);
        long l ;
        l = (long) i;
        vb.set_value(l);
        return true;
    }
    default:
        qDebug() << "Unknown Data Type " << val << "\n";
        return false;
    }
}

bool SnmpThread::setInfo(const QString &ip ,QString o , QString val)
{
    bool ret = false;
    if(o.isEmpty() ||val.isEmpty()) return ret;
    QString ipAddr = ip;
    QString rwCom = "private";
    QString roCom = "public";
    QString v3PrivPass = "privatekey1";
    QString v3AuthPass = "authkey123";
    QString name = "Busbar";

    int setVersion = version2c;
    Snmp::socket_startup();  // Initialize socket subsystem
    //---------[ make a GenAddress and Oid object to retrieve ]---------------
    UdpAddress address(ipAddr.toStdString().c_str());      // make a SNMP++ Generic address

    Oid oid(o.toStdString().c_str());      // default is sysName

    //---------[ determine options to use ]-----------------------------------
    snmp_version version=version2c;                 // default is v1
    if(setVersion == 1) version = version1;
    else if(setVersion == 3) version = version3;
    int retries=1;                                  // default retries is 1
    int timeout=2000;                                // default is 1 second
    u_short port=161;                               // default snmp port is 161
    OctetStr community(rwCom.toStdString().c_str());                   // community name
    OctetStr get_community;

    if (get_community.len() == 0)
        get_community = community;

    //----------[ create a SNMP++ session ]-----------------------------------
    int status;
    // bind to any port and use IPv6 if needed
    Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));


    //--------[ build up SNMP++ object needed ]-------------------------------
    Pdu pdu;                               // construct a Pdu object
    Vb vb;                                 // construct a Vb object
    vb.set_oid(oid);                      // set the Oid portion of the Vb
    pdu += vb;                             // add the vb to the Pdu

    address.set_port(port);

    OctetStr privPassword(v3PrivPass.toStdString().c_str());
    OctetStr authPassword(v3AuthPass.toStdString().c_str());
    OctetStr securityName(name.toStdString().c_str());
    int securityModel = SNMP_SECURITY_MODEL_USM;
    int securityLevel = SNMP_SECURITY_LEVEL_AUTH_PRIV;
    OctetStr contextName("");
    OctetStr contextEngineID("");
    long authProtocol = SNMP_AUTHPROTOCOL_HMACMD5;
    long privProtocol = SNMP_PRIVPROTOCOL_DES;
    v3MP *v3_MP;
    if (version == version3) {
        const char *engineId = "snmpSet";
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
        ctarget.set_readcommunity(get_community); // set the read community name
        ctarget.set_writecommunity(community);// set the write community name
    }

    SnmpTarget *target;

    if (version == version3){
        target = &utarget;
    }
    else
        target = &ctarget;
    // first get the variabel to determine its type
    if ((status = snmp.get(pdu,*target))== SNMP_CLASS_SUCCESS)
    {
        int ret = pdu.get_vb(vb,0);
        qDebug() << "Oid = " << vb.get_printable_oid() << endl
                 << "Current Value = " << vb.get_printable_value() << endl
                 <<ret<<endl;

        if (determine_vb(vb.get_syntax(), vb , val)) {
            //if (determine_vb(sNMP_SYNTAX_INT, vb , val)) {
            // do the Set
            Pdu setpdu;
            setpdu.set_security_level( securityLevel);
            setpdu.set_context_name (contextName);
            setpdu.set_context_engine_id(contextEngineID);
            vb.set_oid(oid);           // use the same oid as the inquire
            setpdu += vb;
            status = snmp.set(setpdu, *target);
            qDebug() << "Set Status = " << snmp.error_msg(status) << "\n";
            if(SNMP_CLASS_SUCCESS == status) ret = true;
        }
    }
    else qDebug() << "SNMP++ Set Error, " << snmp.error_msg(status) << "\n";

    Snmp::socket_cleanup();  // Shut down socket subsystem
    if(v3_MP) {
        delete v3_MP;
        v3_MP = NULL;
    }
    return ret;
}


