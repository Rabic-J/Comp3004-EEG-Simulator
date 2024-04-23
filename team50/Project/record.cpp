#include "record.h"
#include <iostream>

using namespace std;

Record::Record(string t)
{
    rec_time = t;
    record = "";

    for (int i = 0; i < SIGNALCOUNT; i++)
    {
        beforeSignals[i] = 0;
        afterSignals[i] = 0;
    }
}

string Record::printDevice()
{
    return rec_time;
}


string Record::printPC()
{
    return record;
}

void Record::setBeforeSignal(int current, int before){
    beforeSignals[current] = before;

    record = "Time: " + rec_time + "\n\nSignals (Before -> After):\n";

    for (int i = 0; i < SIGNALCOUNT; i++)
    {
        if (beforeSignals[i] < 10)
        {
            record += " ";
        }

        record += to_string(beforeSignals[i]) + "   ->   ";

        if (afterSignals[i] < 10)
        {
            record += " ";
        }
        record += to_string(afterSignals[i]) + "\n";
    }
}

void Record::setAfterSignal(int current, int after){
    afterSignals[current] = after;

    record = "Time: " + rec_time + "\n\nSignals (Before -> After):\n";

    for (int i = 0; i < SIGNALCOUNT; i++)
    {
        if (beforeSignals[i] < 10)
        {
            record += " ";
        }

        record += to_string(beforeSignals[i]) + "   ->   ";

        if (afterSignals[i] < 10)
        {
            record += " ";
        }
        record += to_string(afterSignals[i]) + "\n";
    }
}


void Record::setRecord(string time)
{
    rec_time = time;
    record = "Time: " + rec_time + "\n\nSignals (Before -> After):\n";

    for (int i = 0; i < SIGNALCOUNT; i++)
    {
        if (beforeSignals[i] < 10)
        {
            record += " ";
        }

        record += to_string(beforeSignals[i]) + "   ->   ";

        if (afterSignals[i] < 10)
        {
            record += " ";
        }
        record += to_string(afterSignals[i]) + "\n";
    }
}

string Record::getRecord()
{
    return record;
}



