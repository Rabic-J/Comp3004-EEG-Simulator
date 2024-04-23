#ifndef DEVICE_H
#define DEVICE_H


#include "patient.h"
#include "record.h"
#include <string>

using namespace std;

class Device
{
    public:
        Device();
        ~Device();
        bool getPowered();
        bool getRunning();
        bool getPaused();
        bool getPCConnection();
        string getTime();
        int getPowerLevel();
        int getRunningTimer();
        int getDisconnectTimer();
        void setDateTime(string);
        void setPatient(Patient*);
        void togglePower();
        void togglePCConnection();
        void startSession();
        void play();
        void pause();
        void stop();
        void applyTherapy();
        string getRecords();

    private:
        bool powered;
        bool running;
        bool paused;
        bool pcConnection;
        string time;
        int powerLevel;
        int runningTimer;
        int disconnectTimer;
        int currentSensor;
        int currentRecord;
        Patient* patient;
        Record** records;
};

#endif // DEVICE_H
