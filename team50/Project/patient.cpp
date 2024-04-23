#include "patient.h"
#include <iostream>

Patient::Patient()
{
    for(int site=0; site<SIGNALCOUNT; site++){
        sigs[site]= 3 + (rand() % 18);
    }
}


void Patient::setSignal(int signal, int signalChange){
    sigs[signal]=sigs[signal]+signalChange;
}

int Patient::getSignal(int signal){
    if(signal<SIGNALCOUNT && signal>=0){
        return sigs[signal];
    }
    return 0;
}
