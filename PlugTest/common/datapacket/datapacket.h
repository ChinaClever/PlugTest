#ifndef DATAPACKET_H
#define DATAPACKET_H
#include <QtCore>
#include <QColor>

#define ARRAY_SIZE 3 //一包数据最长

/**
 * 数据包
 */
struct sDataPacket
{
    bool en;
    uchar action;
    uint all;
    uint ok;
    uint err;
    double value;
};




#endif // DATAPACKET_H
