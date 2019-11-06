#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "displaywid.h"
#include "plugthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void initSerial();
    void initCfg();
    void saveCfg();
    bool inputCheck();

private slots:
    void on_comBtn_clicked();

    void on_startBtn_clicked();

    void on_overBtn_clicked();

private:
    Ui::MainWindow *ui;
    DisplayWid *mDisplayWid;
    SerialPortDlg *mSerial;
    PlugThread *mPlugThread;
};

#endif // MAINWINDOW_H
