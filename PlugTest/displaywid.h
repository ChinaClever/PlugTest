#ifndef DISPLAYWID_H
#define DISPLAYWID_H

#include "comtablewid.h"
#include "datapackets.h"

class DisplayWid : public ComTableWid
{
    Q_OBJECT
public:
    DisplayWid(int mode , QWidget *parent = nullptr);

protected:
    void initVolWid();;
    void initCurWid();
    void updateData(int id);

protected slots:
   virtual void timeoutDone();

private:
    int mMode;
};

#endif // DISPLAYWID_H
