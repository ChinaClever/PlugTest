#ifndef DEVONOFF_H
#define DEVONOFF_H

#include <QWidget>
#include "displaywid.h"
#include "common/cfgcom/configbase.h"
namespace Ui {
class DevOnOff;
}

class DevOnOff : public QWidget
{
    Q_OBJECT

public:
    explicit DevOnOff(QWidget *parent = nullptr);
    ~DevOnOff();
    void init();

private:
    Ui::DevOnOff *ui;
    DisplayWid *mDisplayWid;
    sConfigItem *mItem;
};

#endif // DEVONOFF_H
