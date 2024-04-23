#include "device.h"

#include <iostream>

using namespace std;

Device::Device()
{
    //Start the device with power off and full battery
    powered = false;
    powerLevel = 100;

    //Start with a default date of Jan 1st 2000 at 12:00am
    time = "Sat Jan 1 00:00:00 2000";

    //Start with no connection and not running a session
    pcConnection = false;
    running = false;
    paused = false;

    //Set all timers and position variables to 0
    currentSensor = 0;
    runningTimer = 0;
    disconnectTimer = 0;
    currentRecord = 0;

    //Set patient to empty
    patient = nullptr;

    //Initialize the list of records
    records = new Record*[MAXRECORDS];

    for (int r = 0; r < MAXRECORDS; r++)
    {
        records[r] = nullptr;
    }
}

Device::~Device()
{
    for (int r = 0; r < MAXRECORDS; r++)
    {
        delete records[r];
    }

    delete [] records;
}

bool Device::getPowered()
{
    return powered;
}

bool Device::getRunning()
{
    return running;
}

bool Device::getPaused()
{
    return paused;
}

bool Device::getPCConnection()
{
    return pcConnection;
}

string Device::getTime()
{
    return time;
}

int Device::getPowerLevel()
{
    return powerLevel;
}

int Device::getRunningTimer()
{
    return runningTimer;
}

int Device::getDisconnectTimer()
{
    return disconnectTimer;
}

void Device::setDateTime(string t)
{
    time = t;
}

void Device::setPatient(Patient* p)
{
    patient = p;

    if (running)
    {
        if (patient == nullptr) //If the patient was disconnected while a session is running, pause that session
        {
            paused = true;
        }
        else
        {
            //Record the before signals for all sensors not already completed
            for (int i = currentSensor; i < SIGNALCOUNT; i++)
            {
                records[currentRecord]->setBeforeSignal(i, patient->getSignal(i));
            }
        }
    }

}

void Device::togglePower()
{
    if (powered && running) //If the device is about to be powered off and is running a session, stop that session
    {
        stop();
    }

    powered = !powered; //Switch the state of powered

    //powerLevel = 100; //Recharge battery
}

void Device::togglePCConnection()
{
    pcConnection = !pcConnection; //Switch the state of pcConnection
}

void Device::startSession()
{
    if (powered && !running) //Make sure that the device is powered and not already running a session
    {
        if (currentRecord >= MAXRECORDS) //Check if all records are full
        {
            //Delete the oldest record
            delete records[0];

            //Shift all other records towards the front of the list
            for (int x = 0; x < MAXRECORDS - 1; x++)
            {
                records[x] = records[x+1];
            }

            //Clear the last record
            delete records[MAXRECORDS - 1];

            //Set the current record to the last record
            currentRecord = MAXRECORDS - 1;
        }

        //Create a new record
        records[currentRecord] = new Record(time);

        //Start running & set the current sensor to 0
        running = true;
        currentSensor = 0;

        //Reset timers
        runningTimer = 0;
        disconnectTimer = 0;

        if (patient != nullptr) //Check if a patient is connected
        {
            //Record the before signals for all sensors
            for (int i = 0; i < SIGNALCOUNT; i++)
            {
                records[currentRecord]->setBeforeSignal(i, patient->getSignal(i));
            }

            //Start the session
            paused = false;
        }
        else
        {
            //Immediately pause the session
            paused = true;
        }
    }
}

void Device::play()
{
    if (powered && paused && patient != nullptr) //Check that the device is on and currently paused, and that the patient is connected
    {
        //Unpause and reset the disconnect timer only if the above conditions are true
        paused = false;
        disconnectTimer = 0;
    }
}

void Device::pause()
{
    if (powered && !paused) //Check that the device is on and currently not paused
    {
        paused = true; //Pause the device
    }
}

void Device::stop()
{
    if (powered && running) //Check that the device is on and currently running a therapy session
    {
        //Stop the therapy session, reset session and disconnect timers, and reset mode to 0 (Menu)
        running = false;
        runningTimer = 0;
        disconnectTimer = 0;

        //Delete the record for this session
        delete records[currentRecord];
        records[currentRecord] = nullptr;
    }
}

void Device::applyTherapy()
{
    if (powered) //make sure the device is powered
    {
        if (paused || patient == nullptr) //If there is no connection to the patient, or the session is paused, increment the disconnect timer
        {
            disconnectTimer++;
        }
        else if (running) //Check that the device is on and is running a session
        {
            if (runningTimer > (60*16)) //Ensure that the minute of "calculating baseline" has passed before beginning any treatment
            {
                int signal = patient->getSignal(currentSensor); //Get the signal for the current sensor from the patient

                if (signal > 13) //Decrease the frequency if the signal is above 13 Hz
                {
                    patient->setSignal(currentSensor, -1);
                }
                else if (signal < 8) //Increase the frequency if the signal is below 8 Hz
                {
                    patient->setSignal(currentSensor, 1);
                }
            }

            runningTimer++; //Increase the running timer
            if (runningTimer > (60*16) && (runningTimer - (60*16)) % 16 == 0) //If 1 second has passed (16 16ths of a second), advance to the next sensor
            {
                //Record after signal
                records[currentRecord]->setAfterSignal(currentSensor, patient->getSignal(currentSensor));

                //Move to the next sensor
                currentSensor++;

                if (currentSensor >= SIGNALCOUNT) //Check if all sensors have been checked already
                {
                    //End the session, reset the sensor, and advance to the next record for the next session
                    running = false;
                    currentSensor = 0;
                    currentRecord++;
                }

                powerLevel--; //Decrease the power by 1
                if (powerLevel <= 0) //Check if the device is out of battery
                {
                    //Stop the current session if it is still running
                    if (running)
                    {
                        stop();
                    }

                    //Power off the device
                    powered = false;
                }
            }
        }

        if (disconnectTimer >= (DISCONNECTTIME * 16)) //If the maximum disconnect time has elapsed, stop the session and turn off the device
        {
            stop();
            powered = false;
        }
    }
}

string Device::getRecords()
{
    //Define a return string
    string printedRecords = "";

    //Check if the device is connected to a PC
    if (pcConnection)
    {
        //Loop through all non-empty records
        for (int x = MAXRECORDS - 1; x >= 0; x--)
        {
            if (records[x] != nullptr)
            {
                //Get the current record and append it to the return string
                printedRecords = printedRecords + records[x]->printPC() + "\n";
                printedRecords = printedRecords + "----------------------------\n\n";
            }
        }
    }
    else
    {
        //Loop through all non-empty records
        for (int x = MAXRECORDS - 1; x >= 0; x--)
        {
            if (records[x] != nullptr)
            {
                //Get the current record and append it to the return string
                printedRecords = printedRecords + records[x]->printDevice() + "\n";
                printedRecords = printedRecords + "-----------------------\n";
            }
        }
    }

    return printedRecords;
}
