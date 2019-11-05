/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "datapackets.h"



DataPackets *DataPackets::bulid()
{
    static DataPackets* sington = NULL;
    if(sington == NULL) {
        sington = new DataPackets();
    }
    return sington;
}

sDataPacket *DataPackets::get(int devNum)
{
    return  &mPackets[devNum];
}

void DataPackets::clear(int id)
{
    sDataPacket *p = get(id);
    memset(p, 0, sizeof(sDataPacket));
}

void DataPackets::clears()
{
    for(int i=0; i<ARRAY_SIZE; ++i) {
        clear(i);
    }
}
