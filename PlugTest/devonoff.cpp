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
    ui->delayBox->setValue(mItem->delay);
}

void DevOnOff::get()
{
    mItem = ConfigBase::bulid()->item;
    mItem->testIp = ui->testIPEdit->text();
    mItem->swIp = ui->swIPEdit->text();
    mItem->oids[0] = ui->swOidEdit->text();
    mItem->readVolOids[0] = ui->readVolEdit->text();
    mItem->writeCrMinVolOids[0] = ui->writeCrMinVolEdit->text();
    mItem->writeMinVolOids[0] = ui->writeMinVolEdit->text();
    mItem->delayOpen = ui->delayOpenBox->value();
    mItem->delayClose = ui->delayCloseBox->value();
    mItem->delay = ui->delayBox->value();
}

