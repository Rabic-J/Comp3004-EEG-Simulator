#ifndef PATIENT_H
#define PATIENT_H

#include "defs.h"

class Patient
{
public:
    Patient();
    void setSignal(int signal, int signalChange);
    int getSignal(int signal);

private:
    int sigs[SIGNALCOUNT];

};

#endif // PATIENT_H
