#ifndef RECORD_H
#define RECORD_H

#include "defs.h"
#include <string>
#include <QRandomGenerator>

using namespace std;

class Record
{
public:
    Record(string);
    string printDevice();
    string printPC();
    string getRecord();
    void setBeforeSignal(int, int);
    void setAfterSignal(int, int);
    void setRecord(string);

private:
    int beforeSignals[SIGNALCOUNT];
    int afterSignals[SIGNALCOUNT];
    string rec_time;
    string record;
};

#endif // RECORD_H
