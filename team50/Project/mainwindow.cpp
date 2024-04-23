#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    on_resetDevice_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete device;
}


void MainWindow::resetInterface()
{

    //Hide all mode-specific functions
    ui->timeElapsed->setVisible(false);
    ui->progressBar->setVisible(false);
    ui->dateTimeEdit->setVisible(false);

    //Clear any printed session logs
    ui->PCLog->setText("");
    ui->sessionLog->setText("");

    //Reset timer and progress bar
    ui->timeElapsed->setText("0:00");
    ui->progressBar->setValue(0);

    //Stop any running session
    if (device->getRunning())
    {
        device->stop();
    }
}


void MainWindow::updateButtons()
{
    //Disable and recolour pause and play buttons according to current state
    if (!device->getRunning())
    {
        ui->playBttn->setStyleSheet("background-color: white;");
        ui->pauseBttn->setStyleSheet("background-color: white;");
        ui->playBttn->setDisabled(true);
        ui->pauseBttn->setDisabled(true);
        ui->stopBttn->setDisabled(true);
    }
    else if (device->getPaused())
    {
        ui->playBttn->setStyleSheet("background-color: white;");
        ui->pauseBttn->setStyleSheet("background-color: green;");
        ui->playBttn->setDisabled(false);
        ui->pauseBttn->setDisabled(true);
        ui->stopBttn->setDisabled(false);
    }
    else
    {
        ui->playBttn->setStyleSheet("background-color: green;");
        ui->pauseBttn->setStyleSheet("background-color: white;");
        ui->playBttn->setDisabled(true);
        ui->pauseBttn->setDisabled(false);
        ui->stopBttn->setDisabled(false);
    }
}

void MainWindow::updateLights()
{
    //Turn on the blue light if the device is running
    if (device->getRunning())
    {
        ui->blueLight->setStyleSheet("background-color: blue;");

        //Turn on the red light if the device is paused
        if (device->getPaused())
        {
            ui->redLight->setStyleSheet("background-color: red;");
            ui->greenLight->setStyleSheet("background-color: gray;");
        }
        else //Otherwise, turn on the green light
        {
            ui->greenLight->setStyleSheet("background-color: green;");
            ui->redLight->setStyleSheet("background-color: gray;");
        }
    }
    else //Otherwise, turn off all lights
    {
        ui->blueLight->setStyleSheet("background-color: gray;");
        ui->greenLight->setStyleSheet("background-color: gray;");
        ui->redLight->setStyleSheet("background-color: gray;");
    }
}

void MainWindow::updatePower(bool powered)
{
    resetInterface();


    //Change colour of power button based on state
    if (powered)
    {
        ui->powerBttn->setStyleSheet("background-color: green;");
    }
    else
    {
        ui->powerBttn->setStyleSheet("background-color: gray;");
    }

    //Turn off all lights
    ui->blueLight->setStyleSheet("background-color: gray;");
    ui->greenLight->setStyleSheet("background-color: gray;");
    ui->redLight->setStyleSheet("background-color: gray;");
    ui->battery->setEnabled(powered);

    //Update the buttons
    updateButtons();

    //Hide/unhide all on-screen text and buttons
    ui->newSessionBttn->setVisible(powered);
    ui->timedateBttn->setVisible(powered);
    ui->sessionLogBttn->setVisible(powered);
    ui->menuText->setVisible(powered);

    //Hide all mode-specific functions
    ui->dateTimeEdit->setVisible(false);
    ui->timeElapsed->setVisible(false);
    ui->progressBar->setVisible(false);

    //Reset the menu buttons
    ui->newSessionBttn->setStyleSheet("background-color: white;");
    ui->timedateBttn->setStyleSheet("background-color: white;");
    ui->sessionLogBttn->setStyleSheet("background-color: white;");
}

void MainWindow::on_newSessionBttn_clicked()
{
    resetInterface();

    //Show timer and progress bar, and turn on the blue light
    ui->blueLight->setStyleSheet("background-color: blue;");
    ui->timeElapsed->setVisible(true);
    ui->progressBar->setVisible(true);

    //Start the session
    device->startSession();

    //Update the action buttons and lights
    updateButtons();
    updateLights();

    //Update the menu buttons accordingly
    ui->newSessionBttn->setStyleSheet("background-color: blue;");
    ui->timedateBttn->setStyleSheet("background-color: white;");
    ui->sessionLogBttn->setStyleSheet("background-color: white;");

    //Disable reset options
    ui->resetDevice->setDisabled(true);
    ui->resetPatient->setDisabled(true);
}


void MainWindow::on_timedateBttn_clicked()
{
    resetInterface();

    ui->dateTimeEdit->setVisible(true);

    //Update the action buttons and lights
    updateButtons();
    updateLights();

    //Update the menu buttons accordingly
    ui->newSessionBttn->setStyleSheet("background-color: white;");
    ui->timedateBttn->setStyleSheet("background-color: blue;");
    ui->sessionLogBttn->setStyleSheet("background-color: white;");

    //Enable reset options
    ui->resetDevice->setDisabled(false);
    ui->resetPatient->setDisabled(false);
}


void MainWindow::on_sessionLogBttn_clicked()
{
    resetInterface();

    //Display records on PC if connected to pc
    if (device->getPCConnection())
    {
        ui->PCLog->insertPlainText(QString::fromStdString(device->getRecords()));
    }
    else //Otherwise display records on the device session log
    {
        ui->sessionLog->insertPlainText(QString::fromStdString(device->getRecords()));
    }

    //Update the action buttons and lights
    updateButtons();
    updateLights();

    //Update the menu buttons accordingly
    ui->newSessionBttn->setStyleSheet("background-color: white;");
    ui->timedateBttn->setStyleSheet("background-color: white;");
    ui->sessionLogBttn->setStyleSheet("background-color: blue;");

    //Enable reset options
    ui->resetDevice->setDisabled(false);
    ui->resetPatient->setDisabled(false);
}


void MainWindow::on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    device->setDateTime(dateTime.toString().toStdString());
}


void MainWindow::on_playBttn_clicked()
{
    device->play();
    updateButtons();
    updateLights();
}


void MainWindow::on_pauseBttn_clicked()
{
    device->pause();
    updateButtons();
    updateLights();
}


void MainWindow::on_stopBttn_clicked()
{
    device->stop();
    updateButtons();
    updateLights();
    resetInterface();
}


void MainWindow::on_powerBttn_clicked()
{
    //Change power on device
    device->togglePower();

    //Update power on GUI
    updatePower(device->getPowered());
}


void MainWindow::on_condisconPC_clicked()
{
    device->togglePCConnection();

    //PC Connection button colour
    if (device->getPCConnection())
    {
        ui->condisconPC->setStyleSheet("background-color: green;");
    }
    else
    {
        ui->condisconPC->setStyleSheet("background-color: white;");
    }
}


void MainWindow::on_disconnectSensors_clicked()
{
    device->setPatient(nullptr);

    ui->connectSensors->setStyleSheet("background-color: white;");
    ui->disconnectSensors->setStyleSheet("background-color: red;");
}


void MainWindow::on_connectSensors_clicked()
{
    device->setPatient(&patient);

    ui->connectSensors->setStyleSheet("background-color: green;");
    ui->disconnectSensors->setStyleSheet("background-color: white;");
}


void MainWindow::on_advance_clicked()
{
    //Check if the device is running before doing any processing
    if (device->getRunning())
    {
        //Do as many runs of apply therapy as selected while the session is still running
        for (int i = 0; device->getRunning() && i < ui->advanceNum->text().toInt(); i++)
        {
             device->applyTherapy();
        }

        //Get the timer and determine minutes, seconds, and percent completion
        int timer = 0;
        int seconds = 0;
        int minutes = 0;
        if (device->getPaused())
        {
            //Set the text to red
            ui->timeElapsed->setStyleSheet("color: red;");

            //Set seconds and minutes according to remaining time before disconnect
            timer = device->getDisconnectTimer();
            seconds = DISCONNECTTIME - (timer / 16);
            minutes = seconds / 60;
        }
        else
        {
            //Set the text to black
            ui->timeElapsed->setStyleSheet("color: black;");

            //Set seconds and minutes according to time elapsed in current session
            timer = device->getRunningTimer();
            seconds = timer / 16;
            minutes = seconds / 60;

            //Update the progress bar
            double percentComplete = ((double)seconds / ((double)SIGNALCOUNT + 60.0)) * 100.0;
            ui->progressBar->setValue(percentComplete);
        }


        //Create the timer string
        string timeElapsed = "";
        timeElapsed += to_string(minutes);
        timeElapsed += ":";
        while (seconds >= 60)
        {
            seconds -= 60;
        }
        if (seconds < 10)
        {
            timeElapsed += "0";
        }
        timeElapsed += to_string(seconds);


        //Update the session timer
        ui->timeElapsed->setText(QString::fromStdString(timeElapsed));

        //Update buttons and lights
        updateButtons();
        updateLights();

        //Update battery percentage
        ui->battery->setValue(device->getPowerLevel());

        //If the device is currently off, but the interface has not yet been updated to match, update power on GUI
        if (!device->getPowered() && ui->battery->isVisible())
        {
            updatePower(false);
        }
    }
}


void MainWindow::on_resetPatient_clicked()
{
    //Generate a new patient and switch the current patient to that patient
    Patient p;
    patient = p;
}


void MainWindow::on_resetDevice_clicked()
{
    //Generate a new device
    device = new Device();

    //Reset all aspects of the interface
    resetInterface();
    updateButtons();
    updateLights();
    updatePower(false);
    //Update power on GUI
    updatePower(device->getPowered());

    //Update battery percentage
    ui->battery->setValue(device->getPowerLevel());

    //Restore the battery
    //ui->battery->setValue(100);

    //Reset simulation button colours
    ui->connectSensors->setStyleSheet("background-color: white;");
    ui->disconnectSensors->setStyleSheet("background-color: red;");
    ui->condisconPC->setStyleSheet("background-color: white;");
}

