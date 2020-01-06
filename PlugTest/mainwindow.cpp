#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mSerial = new SerialPortDlg(this);
    mDisplayWid = new DisplayWid(ui->widget);

    mPlugThread = new PlugThread(this);
    mPlugThread->initSerial(mSerial->getSerialPort());
    initCfg();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initCfg()
{
    sConfigItem *item = ConfigBase::bulid()->item;
    if(item->com.size()) {
        SerialPort *s = mSerial->getSerialPort();
        s->open(item->com, QSerialPort::Baud19200);
        initSerial();
    }

    ui->ipEdit->setText(item->ip);
    ui->spinBox->setValue(item->delay);
    ui->openEdit->setText(item->openCmd[0]);
    ui->closeEdit->setText(item->closeCmd[0]);
    ui->openEdit_2->setText(item->openCmd[1]);
    ui->closeEdit_2->setText(item->closeCmd[1]);
    ui->openEdit_3->setText(item->openCmd[2]);
    ui->closeEdit_3->setText(item->closeCmd[2]);
    ui->openEdit_4->setText(item->openCmd[3]);
    ui->closeEdit_4->setText(item->closeCmd[3]);


    QCheckBox *enBox[] = {ui->enBox_1, ui->enBox_2, ui->enBox_3};
    QLineEdit *oidEdit[] = {ui->oidEdit_1, ui->oidEdit_2, ui->oidEdit_3};
    QCheckBox *chBox[] = {ui->checkBox, ui->checkBox_2, ui->checkBox_3,ui->checkBox_4};
    for(int i=0; i<RTUCMD_SIZE; ++i) {
        chBox[i]->setChecked(item->rtuCmdEn[i]);
    }
    for(int i=0; i<SNMP_SIZE; ++i) {
        enBox[i]->setChecked(item->snmpEn[i]);
        oidEdit[i]->setText(item->oids[i]);
    }
}

void MainWindow::saveCfg()
{
     sConfigItem *item = ConfigBase::bulid()->item;
     SerialPort *s = mSerial->getSerialPort();
     item->com = s->getSerialName();

     item->ip = ui->ipEdit->text();
     item->delay = ui->spinBox->value();
     item->openCmd[0] = ui->openEdit->text();
     item->closeCmd[0] = ui->closeEdit->text();
     item->openCmd[1] = ui->openEdit_2->text();
     item->closeCmd[1] = ui->closeEdit_2->text();
     item->openCmd[2] = ui->openEdit_3->text();
     item->closeCmd[2] = ui->closeEdit_3->text();
     item->openCmd[3] = ui->openEdit_4->text();
     item->closeCmd[3] = ui->closeEdit_4->text();

     QCheckBox *enBox[] = {ui->enBox_1, ui->enBox_2, ui->enBox_3};
     QLineEdit *oidEdit[] = {ui->oidEdit_1, ui->oidEdit_2, ui->oidEdit_3};
     QCheckBox *chBox[] = {ui->checkBox, ui->checkBox_2, ui->checkBox_3,ui->checkBox_4};
     for(int i=0; i<RTUCMD_SIZE; ++i) {
         item->rtuCmdEn[i] = chBox[i]->isChecked();
     }
     for(int i=0; i<SNMP_SIZE; ++i) {
         item->snmpEn[i] = enBox[i]->isChecked();
         item->oids[i] = oidEdit[i]->text();
     }
     ConfigBase::bulid()->save();
}


void MainWindow::initSerial()
{
    QString str = tr("串口未打开");
    SerialPort *s = mSerial->getSerialPort();
    if(s->isOpened()) {
        str = s->getSerialName() + tr("已打开");
    }
    ui->label->setText(str);
}


void MainWindow::on_comBtn_clicked()
{
    mSerial->exec();
    initSerial();
}

bool MainWindow::inputCheck()
{
    QString str = ui->ipEdit->text();
    if(!cm_isIPaddress(str)) {
        CriticalMsgBox box(this, tr("IP地址设置错误"));
        return false;
    }

    int ret = ui->openEdit->text().size();
    if(ret < 10){
        CriticalMsgBox box(this, tr("打开命令输入错误"));
        return false;
    }

    ret = ui->closeEdit->text().size();
    if(ret < 10){
        CriticalMsgBox box(this, tr("关闭命令输入错误"));
        return false;
    }



    return true;
}

void MainWindow::on_startBtn_clicked()
{
    if(inputCheck()) {
        saveCfg();
        mPlugThread->startThread();
        ui->groupBox_1->setEnabled(false);
        ui->groupBox_2->setEnabled(false);
    }
}

void MainWindow::on_overBtn_clicked()
{
    mPlugThread->quitThread();
    ui->groupBox_1->setEnabled(true);
    ui->groupBox_2->setEnabled(true);
}
