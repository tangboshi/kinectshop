#include "kinectio.h"

kinectio::kinectio()
{
}

void kinectio::setDetectedPid(int pid){
    detectedPid = pid;
}

int kinectio::getDetectedPid(){
    return detectedPid;
}
