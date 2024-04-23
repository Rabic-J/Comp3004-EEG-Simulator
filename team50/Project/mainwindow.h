#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "patient.h"
#include "device.h"
#include <QMainWindow>
#include "QPushButton"
#include "QDateTimeEdit"
#include "QTextEdit"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_newSessionBttn_clicked();

    void on_timedateBttn_clicked();

    void on_sessionLogBttn_clicked();

    void on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_playBttn_clicked();

    void on_pauseBttn_clicked();

    void on_stopBttn_clicked();

    void on_powerBttn_clicked();

    void on_condisconPC_clicked();

    void on_disconnectSensors_clicked();

    void on_connectSensors_clicked();

    void on_advance_clicked();

    void on_resetPatient_clicked();

    void on_resetDevice_clicked();

private:
    Ui::MainWindow *ui;
    Patient patient;
    Device* device;
    void resetInterface();
    void updateButtons();
    void updateLights();
    void updatePower(bool);
};
#endif // MAINWINDOW_H
