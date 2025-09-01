#include "devonoff.h"
#include "ui_devonoff.h"

DevOnOff::DevOnOff(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DevOnOff)
{
    ui->setupUi(this);
    mDisplayWid = new DisplayWid( 1 , ui->widget );
    init();
}

DevOnOff::~DevOnOff()
{
    delete ui;
}

void DevOnOff::init()
{
    mItem = ConfigBase::bulid()->item;
    ui->testIPEdit->setText(mItem->testIp);
    ui->swIPEdit->setText(mItem->swIp);
    ui->swOidEdit->setText(mItem->oids[0]);
    ui->readVolEdit->setText(mItem->readVolOids[0]);
    ui->writeCrMinVolEdit->setText(mItem->writeCrMinVolOids[0]);
    ui->writeMinVolEdit->setText(mItem->writeMinVolOids[0]);
    ui->delayOpenBox->setValue(mItem->delayOpen);
    ui->delayCloseBox->setValue(mItem->delayClose);
}
